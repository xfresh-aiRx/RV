/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 *
 * This code is based on the original C++ code by John M. Miano
 * (Colosseum Builders, Inc.) which contains following licence: (the
 * original licence file can be found on 00readme.txt in the io/png
 * and io/jpeg directories):
 *
 ******************************************************************************
 *
 * Colosseum Builders C++ Image Library
 *
 * Copyright 1997, 1998, 1999
 * All Rights Reserved
 *
 * Permision is granted to use, modify this software provided these conditions
 * are followed:
 *
 * 1. You understand this software comes with no warranties of any kind
 *    whatsoever.
 * 2. You assume full responsibility for the use of this software and transmit
 *    no liability to the authors.
 * 3. You assume the responsility for obtaining licenses for from any patent
 *    holders.
 * 4. If you distribute this code in source form, you must include this file
 *    unmodified and document the changes you have made.
 * 5. If you incorporate this code in a product distributed without source code
 *    you state included in your documentation that it is based in part on
 *    software from Colosseum Builders, Inc.
 *
 *    THIS SOFTWARE IS PROVIDED AS IS. WE CAN'T TEST IT ON YOUR SYSTEM SO
 *    USE AT YOUR OWN RISK. YOU HAVE THE SOURCE SO YOU HAVE NO EXCUSES.
 *
 ******************************************************************************
 *
 * The modifications of the original code are mainly the adaptation
 * for the LTI-Lib types and the use of namespaces to create an own
 * scope for the original functions
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltipngdeflateencoder.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: Deflate Encoder Class Implementation
 * revisions ..: $Id: ltipngdeflateencoder.cpp,v 1.3 2003/02/17 07:16:13 alvarado Exp $
 */


#include <climits>
#include <cstring>

#include "ltipngdeflateencoder.h"
#include "ltipngdeflatehuffmanencoder.h"
#include "ltipngadler32.h"
#include "ltipngdeflatepvt.h"
#include "ltipngdeflateoutputstream.h"
#include "ltiAssert.h"

namespace lti{
  namespace png {


// Size definitions for the lookahead buffer.
const unsigned int LOOKAHEADSIZE = (1 << 9);
const unsigned int LOOKAHEADMASK = LOOKAHEADSIZE - 1;

// Hash Table size definitions.
const unsigned int HASHBITS = 5;
const unsigned int HASHTABLESIZE = 1 << (3 * HASHBITS);

// End of stream marker for the lookahead buffer.
const unsigned int ENDSTREAM = 0xFFFF;


//
//  Description:
//
//  This function advances a lookahead buffer index.
//
//  Parameter:
//
//  position : The index to advance (in/out)
//
inline void AdvanceLookaheadPosition (unsigned int &position) {
  ++ position;
  position &= LOOKAHEADMASK;
  return;
}

//
//  Description:
//
//  This function advances an LZ buffer index.
//
//  Parameter:
//
//  position : The index to advance (in/out)
//
inline void AdvanceWindowPosition (unsigned int &position) {
  ++ position;
  position &= DEFLATEWINDOWMASK;
  return;
}


//
//  Description:
//
//  This function converts an LZ window position and offset to
//  a position in the LZ window.
//
//  Parameters:
//
//  windowposition : The current position in the LZ Window
//  offset : The LZ window offset
//
//  Return Value:
//
//  The window position
//
inline unsigned int WindowOffsetToPosition (unsigned int windowposition,
                                            unsigned int offset) {

  unsigned int result = (DEFLATEWINDOWSIZE + windowposition - offset) & DEFLATEWINDOWMASK;
  return result;
}

//
//  Description
//
//  This is the hash function used by compressor. It converts
//  a 3-byte sequence into an integer.
//
//  Parameters:
//
//  v1, v2, v3 : The 3-byte sequence to hash.
//
//  Return Value:
//
//  The hash value in the range (0 .. HASHTABLESIZE - 1)
//
inline unsigned int Hash (ubyte v1, ubyte v2, ubyte v3) {
  const unsigned int mask = (1 << HASHBITS) - 1;
  unsigned int value = (v1 & mask )
                     | ((v2 & mask) << HASHBITS)
                     | ((v3 & mask) << (2 * HASHBITS));
  return value;

}

//
//  Description:
//
//  This function converts a distance value into a code,
//  count of extra bits, and extra bits.
//
//  Parameters:
//
//  distance  : The distance value to convert
//  code      : The corresponding distance code
//  extra     : The number of extra bits
//  value     : The extra bit value
//
void DistanceToCode (unsigned int distance, unsigned int &code,
                            unsigned int &extra, unsigned int &value) {
  // maxvalue [n] is the maximum distance value for the code n.
  static const unsigned int maxvalue [DEFLATEMAXDISTANCECODES] =
    {
         1,     2,     3,     4,     6,     8,    12,    16,    24,    32,
        48,    64,    96,   128,   192,   256,   384,   512,   768,  1024,
      1536,  2048,  3072,  4096,  6144,  8192, 12288, 16384, 24576, 32768,
    };
  // extras [n] is the number of extra bits for the code n.
  static const unsigned int extras [DEFLATEMAXDISTANCECODES] =
    {
      0, 0,  0,  0,  1,  1,  2,  2,  3, 3,
      4, 4,  5,  5,  6,  6,  7,  7,  8, 8,
      9, 9, 10, 10, 11, 11, 12, 12, 13, 13,
    };

  // bases [n] is the smallest distance value for code n.
  static const unsigned int bases [DEFLATEMAXDISTANCECODES] =
    {
         1,    2,    3,    4,    5,    7,    9,    13,    17,    25,
        33,   49,   65,   97,  129,  193,  257,   385,   513,   769,
      1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577,
    };

  for (code = 0; code < DEFLATEMAXDISTANCECODES; ++ code) {
    if (distance <=  maxvalue [code])
      break;
  }
  extra = extras [code];
  value = distance - bases [code];
  return;
}
//
//  Description:
//
//  This function converts a length value to a length code,
//  count of extra bits, and extra bit value.
//
//  Parameters:
//
//  length : The value to convert
//  code      : The corresponding length code
//  extra     : The number of extra bits
//  value     : The extra bit value
//
inline static void LengthToCode (unsigned int length, unsigned int &code,
                                 unsigned int &extra, unsigned int &value) {
  assert (length <= DEFLATELONGESTLENGTH);

  // codes [n] is the length for for length n - 3.
  static const uint16 codes [DEFLATELONGESTLENGTH-2] =
    {
      257, 258, 259, 260, 261, 262, 263, 264,
      265, 265, 266, 266, 267, 267, 268, 268,
      269, 269, 269, 269, 270, 270, 270, 270,
      271, 271, 271, 271, 272, 272, 272, 272,
      273, 273, 273, 273, 273, 273, 273, 273,
      274, 274, 274, 274, 274, 274, 274, 274,
      275, 275, 275, 275, 275, 275, 275, 275,
      276, 276, 276, 276, 276, 276, 276, 276,
      277, 277, 277, 277, 277, 277, 277, 277,
      277, 277, 277, 277, 277, 277, 277, 277,
      278, 278, 278, 278, 278, 278, 278, 278,
      278, 278, 278, 278, 278, 278, 278, 278,
      279, 279, 279, 279, 279, 279, 279, 279,
      279, 279, 279, 279, 279, 279, 279, 279,
      280, 280, 280, 280, 280, 280, 280, 280,
      280, 280, 280, 280, 280, 280, 280, 280,
      281, 281, 281, 281, 281, 281, 281, 281,
      281, 281, 281, 281, 281, 281, 281, 281,
      281, 281, 281, 281, 281, 281, 281, 281,
      281, 281, 281, 281, 281, 281, 281, 281,
      282, 282, 282, 282, 282, 282, 282, 282,
      282, 282, 282, 282, 282, 282, 282, 282,
      282, 282, 282, 282, 282, 282, 282, 282,
      282, 282, 282, 282, 282, 282, 282, 282,
      283, 283, 283, 283, 283, 283, 283, 283,
      283, 283, 283, 283, 283, 283, 283, 283,
      283, 283, 283, 283, 283, 283, 283, 283,
      283, 283, 283, 283, 283, 283, 283, 283,
      284, 284, 284, 284, 284, 284, 284, 284,
      284, 284, 284, 284, 284, 284, 284, 284,
      284, 284, 284, 284, 284, 284, 284, 284,
      284, 284, 284, 284, 284, 284, 284, 285,
    };

  // extras [n] is the number of extra bits for code n.
  static const ubyte extras [DEFLATEMAXLENGTHCODES - DEFLATEFIRSTLENGTHCODE] =
    {
      0, 0, 0, 0, 0, 0, 0, 0,
      1, 1, 1, 1, 2, 2, 2, 2,
      3, 3, 3, 3, 4, 4, 4, 4,
      5, 5, 5, 5, 0,
    };
  static const uint16 basevalues [DEFLATEMAXLENGTHCODES - DEFLATEFIRSTLENGTHCODE] =
    {
        3,   4,   5,   6,   7,   8,   9,  10,
       11,  13,  15,  17,  19,  23,  27,  31,
       35,  43,  51,  59,  67,  83,  99, 115,
      131, 163, 195, 227, 258,
    };

  code = codes [length - 3];
  extra = extras [code - DEFLATEFIRSTLENGTHCODE];
  value = length - basevalues [code - DEFLATEFIRSTLENGTHCODE];
  return;
}



//
//  Description:
//
//  Class Default Constructor
//
DeflateEncoder::DeflateEncoder ()
: distance_table (DEFLATEMAXDISTANCECODES),
  length_table (DEFLATEMAXLENGTHCODES),
  length_length_table (DEFLATEMAXLENGTHLENGTHCODES),
  compression_level (DEFAULTCOMPRESSION),
  lz_window (0),
  lookahead_buffer (0),
  block_buffer (0),
  block_buffer_size (0x4000),
  compression_in_progress (false) {
  return;
}

//
//  Description:
//
//  Class Destructor
//
DeflateEncoder::~DeflateEncoder () {
  freeBuffers ();
  return;
}


//
//  Description:
//
//  This function frees the buffers allocated during the encoding
//  process.
//
void DeflateEncoder::freeBuffers () {
  delete [] lz_window; lz_window = 0;
  delete [] lookahead_buffer; lookahead_buffer = 0;
  delete [] hash_values; hash_values = 0;
  delete [] hash_table; hash_table = 0;
  delete [] block_buffer; block_buffer = 0;
  return;
}

//
//  Description:
//
//  This function returns the current block size setting.
//
//  Return Value:
//
//  The current block size.
//
unsigned long DeflateEncoder::getBlockSize () const {
  return block_buffer_size;
}

//
//  Description:
//
//  This function sets the block buffer size.
//
//  Parameters:
//
//  value : The new block buffer size
//
//  Restrictions:
//
//  This function may not be called when compression is underway.
//
void DeflateEncoder::setBlockSize (unsigned long value) {
  if (compression_in_progress)
    throw DeflateError ("Compression In Progress");

  if (value < 500) {
    block_buffer_size = 500;
  }
  else {

    block_buffer_size = value;
  }

  return;
}

//
//  Description:
//
//  This function sets the compression level used.  The compression level
//  determines the depth to which hash chains are searched.
//
//  Parameters:
//
//  value:  The new compression level
//
//  Restrictions:
//
//  This function may be called when compression is underway, but
//  it has no effect for the current compression operation.
//
void DeflateEncoder::setCompressionLevel (DeflateEncoder::CompressionLevel value) {
  if (value < 0 || value > 3) {
    throw DeflateError ("Invalid Compression Level");
  }

  compression_level = value;
  return;
}

//
//  Description:
//
//  The function returns the current compression level.
//
//  Return Value:
//
//  The compresion level.
//
DeflateEncoder::CompressionLevel DeflateEncoder::getCompressionLevel () const {
  return compression_level;
}



//
//  Description:
//
//  This function calculates the hash value from a location
//  in the input stream. We create the hash value by
//  extracting a fixed number of the low order bits from each
//  of the next three data values.
//
//  Parameters:
//  index: The index into the lookahead buffer
//
//  Return Value:
//  The hash value
//
inline unsigned int DeflateEncoder::hashValue (unsigned int index) {

  unsigned int i1 = index & LOOKAHEADMASK;
  unsigned int i2 = (index + 1) & LOOKAHEADMASK;
  unsigned int i3 = (index + 2) & LOOKAHEADMASK;

  unsigned int result = Hash (lookahead_buffer [i1],
                        lookahead_buffer [i2],
                        lookahead_buffer [i3]);
  return result;
}


//
//  Description:
//
//  This function moves a hash entry corresponding to a position
//  in the LZ window to a specified hash chain.
//
//  Parameter:
//  entry: The Hash Entry (index into the LZ window)
//  hashvalue:  The new hashvalue for the entry.
//
inline void DeflateEncoder::moveHashEntry (unsigned int entry, unsigned int hashvalue) {

  HashEntry *he = &hash_values [entry];

  if (he->previous != 0) {
    he->previous->next = he->next;
  }

  if (he->next != 0) {
    he->next->previous = he->previous;
  }

  he->next = hash_table [hashvalue].next;
  he->previous = &hash_table [hashvalue];
  hash_table [hashvalue].next = he;
  if (he->next != 0) {
    he->next->previous = he;
  }

  return;
}

//
//  Description:
//
//  This function finds the length encoding for
//  a length or distance table.
//
//  Parameters:
//  function: The function to process the code
//  lengths: lengths [n] = the length of the huffman code for n.
//  count: The number of code lengths
//
void DeflateEncoder::findLengthCodes (DeflateOutputStream &outputstream,
                                      LENGTHFUNCTION function,
                                      DeflateHuffmanEncoder &encoder,
                                      unsigned int count) {

  assert(count <= DEFLATEMAXLENGTHCODES);

  unsigned int lengths [DEFLATEMAXLENGTHCODES];
  {
    for (unsigned int ii = 0; ii < count; ++ ii) {
      unsigned int code; // Not used
      encoder.encode (ii, code, lengths [ii]);
    }
  }

  {
    for (unsigned int ii = 0, jj; ii < count; ii = jj) {
      if (lengths [ii] != 0) {
        (this->*function) (outputstream, ii, lengths [ii], 0, 0);
        jj = ii + 1;
      }
      else  {

        // Attempt to compact runs of zero length codes.
        // First find the number of consecutive zeros.
        for (jj = ii + 1; lengths [jj] == lengths [jj-1]  && jj < count;
             ++ jj)
        {
        }

        // We need at least 3 consecutive zeros to compact them.
        switch (jj - ii) {
          case 1:
            (this->*function) (outputstream, ii, lengths [ii], 0, 0);
            break;
          case 2:
            (this->*function) (outputstream, ii, lengths [ii], 0, 0);
            (this->*function) (outputstream, ii + 1, lengths [ii], 0, 0);
            break;
          default:
            {
              // We have at least three zeros.

              int kk = jj - ii;
              if (kk > 138) {
                kk = 138;
                jj = ii + kk;
              }
              if (kk > 10) {
                (this->*function) (outputstream, ii, 18, 7, kk - 11);
              }
              else {
                (this->*function) (outputstream, ii, 17, 3, kk - 3);
              }
            } // default
            break;
        } // switch
      } // else
    } // for
  } // block end
  return;
}

//
//  Description:
//
//  This function is passed as a parameter to FindLengthCodes.
//  It is use to find the frequency for each code.
//
//  Parameters:
//  code:  The code generated by FindLengthCodes
//
void DeflateEncoder::gatherLengthCounts (DeflateOutputStream &,
                                         unsigned int,
                                         unsigned int code,
                                         unsigned int,
                                         unsigned int) {
  length_length_table.incrementFrequency (code);
  return;
}

//
//  Description:
//
//  This function is passed as a parameter to FindLengthCodes.
//  It is use to encode and output the code to the output stream.
//
//  Parameters:
//
//  outputstream : The output stream for writing data to
//  code:  The code generated by FindLengthCodes
//  extra:  The number of extra bits of data for the code
//  value:  The extra value
//
void DeflateEncoder::outputLengthCounts (DeflateOutputStream &outputstream,
                                         unsigned int,   // Index - Not used here
                                         unsigned int code,
                                         unsigned int extra,
                                         unsigned int value) {
  unsigned int huffmancode;
  unsigned int huffmansize;
  length_length_table.encode (code, huffmancode, huffmansize);

  outputstream.writeBits (huffmancode, huffmansize);
  if (extra != 0) {
    outputstream.writeBits (value, extra);
  }

  return;
}


//
//  Description:
//
//  This function initializes the Hash Table.
//
//  This function needs to be called once at the start
//  of the compression process.
//
void DeflateEncoder::initializeHashTable () {

  unsigned int ii;
  memset (hash_values, 0, sizeof (HashEntry) * DEFLATEWINDOWSIZE);
  memset (hash_table, 0, sizeof (HashEntry) * (1 << (3 * HASHBITS)));

  for (ii = 0; ii < DEFLATEWINDOWSIZE; ++ ii) {
    hash_values [ii].index = ii;
  }

  // Initialize the hash table to allow initial zero runs. Here we are
  // setting up the hash table so that it appears that we have read
  // 258 zero values before the actual compression begins. This way
  // if the first 258 data bytes contains a run of zeros then we already
  // have a code to compress them with.

  hash_table [Hash (0, 0, 0)].next = &hash_values [DEFLATEWINDOWSIZE - 1];
  hash_values [DEFLATEWINDOWSIZE - 1].next = &hash_table [0];

  for (ii = DEFLATEWINDOWSIZE - 2; ii > DEFLATEWINDOWSIZE - DEFLATELONGESTLENGTH - 1; -- ii) {
    hash_values [ii + 1].next = &hash_values [ii] ;
    hash_values [ii].previous = &hash_values [ii + 1];
  }
  return;
}

//
//  Description:
//
//  This function writes a Deflate block header and Huffman table
//  descriptions to the output stream.
//
//  Parameters:
//
//  outputstream : The stream to write the header to
//  lastblock: true => This is the last block in the image
//             false => There are more blocks to come
//
void DeflateEncoder::outputDeflateHeader (DeflateOutputStream &outputstream,
                                          bool lastblock) {

  length_table.incrementFrequency (DEFLATEENDCODE);
  length_table.buildTable (DEFLATEMAXLENGTHCODESIZE);
  distance_table.buildTable (DEFLATEMAXDISTANCECODESIZE);

  // Determine the count of length/literal and distances that
  // are used.
  unsigned int lengthcount;
  for (lengthcount = DEFLATEMAXLENGTHCODES;lengthcount > 0; -- lengthcount) {
    unsigned int code;
    unsigned int size;
    length_table.encode (lengthcount - 1, code, size);
    if (size != 0) {
      break;
    }
  }

  unsigned int distancecount;

  for (distancecount=DEFLATEMAXDISTANCECODES;distancecount>0; -- distancecount) {

    unsigned int code;
    unsigned int size;
    distance_table.encode (distancecount - 1, code, size);
    if (size != 0) {
      break;
    }
  }

  // Gather the Huffman statistics for encoding the
  // lengths then create the Huffman table for doing so.
  length_length_table.reset ();
  findLengthCodes (outputstream,
                   &DeflateEncoder::gatherLengthCounts,
                   length_table,
                   lengthcount);

  findLengthCodes (outputstream,
                   &DeflateEncoder::gatherLengthCounts,
                   distance_table,
                   distancecount);

  length_length_table.buildTable (DEFLATEMAXLENGTHLENGTHCODESIZE);

  // Count the number of lengths we have to output.
  unsigned int hclen;
  for (hclen = DEFLATEMAXLENGTHLENGTHCODES; hclen > 0; -- hclen) {
    unsigned int code;
    unsigned int size;
    length_length_table.encode (DEFLATELENGTHORDER [hclen-1], code, size);
    if (size != 0) {
      break;
    }
  }

  // Write the Deflate header to the IDAT bock.
  if (lastblock) {
    outputstream.writeBits (1, 1);
  }
  else {
    outputstream.writeBits (0, 1);
  }

  outputstream.writeBits (2, 2); // Dynamic Huffman Codes
  outputstream.writeBits (lengthcount - 257, 5);
  outputstream.writeBits (distancecount - 1, 5);
  outputstream.writeBits (hclen - 4, 4);

  // Output the data for the Huffman table that encodes the Huffman tables
  for (unsigned int ii = 0; ii < hclen; ++ ii)  {
    unsigned int code;
    unsigned int size;
    length_length_table.encode (DEFLATELENGTHORDER [ii], code, size);
    outputstream.writeBits (size, 3);
  }

  // Huffman encode the lengths for the Length/Literal and Distance
  // Huffman tables.
  findLengthCodes (outputstream,
                   &DeflateEncoder::outputLengthCounts,
                   length_table,
                   lengthcount);

  findLengthCodes (outputstream,
                   &DeflateEncoder::outputLengthCounts,
                   distance_table,
                   distancecount);
  return;
}

//
//  Description:
//
//  This function writes a ZLIB header to the output stream.
//
//  Parameters:
//
//  outputstream : The stream for writing the header to
//
void DeflateEncoder::outputZLibHeader (DeflateOutputStream &outputstream) {

  ubyte cmf = 0x78;  // 7=>32K Sliding Window, 8=> Deflate Compression
  ubyte flg = compression_level << 6;

  uint16 check = (cmf << 8) | flg;
  flg |= 31 - (check % 31);
  outputstream.writeBits (cmf, 8);
  outputstream.writeBits (flg, 8);
  return;
}

//
//  Description:
//
//  This function Huffman encodes and outputs the block buffer data.
//  The buffer is encoded so that
//
//  0..255  is a literal byte
//  256-514 is a length code of N-256
//
//  Each length code is followed by a distance code.
//
//  Parameters:
//
//  outputstream : The stream to write the compressed data to.
//
void DeflateEncoder::outputBlockData (DeflateOutputStream &outputstream) {
  unsigned int huffmancode;
  unsigned int huffmansize;
  unsigned int code;
  unsigned int extra;
  unsigned int value;
  unsigned int limit = block_buffer_count;

  for (unsigned int ii = 0; ii < limit; ++ ii) {
    if (block_buffer [ii] < 256) {
      length_table.encode (block_buffer [ii], huffmancode, huffmansize);
      outputstream.writeBits (huffmancode, huffmansize);
    }
    else {
      unsigned int length = block_buffer [ii] - 256;
      ++ ii;
      unsigned int distance = block_buffer [ii];
      LengthToCode (length, code, extra, value);
      length_table.encode (code, huffmancode, huffmansize);

      outputstream.writeBits (huffmancode, huffmansize);
      if (extra != 0)
        outputstream.writeBits (value, extra);

      DistanceToCode (distance, code, extra, value);
      distance_table.encode (code, huffmancode, huffmansize);

      outputstream.writeBits (huffmancode, huffmansize);
      if (extra != 0)
        outputstream.writeBits (value, extra);
    }
  }

  length_table.encode (DEFLATEENDCODE, huffmancode, huffmansize);
  outputstream.writeBits (huffmancode, huffmansize);
  block_buffer_count = 0;
  prepareNewBlock ();
  return;
}

//
//  Description:
//
//  This function finds the longest match of the lookahead buffer
//  in the LZ Window.
//
//  Parameters:
//
//  count : The number of characters in the lookahead buffer.
//  bestlength : The longest match
//  bestoffset : The offset of the longest match
//
void DeflateEncoder::longestMatch (unsigned int count,unsigned int &bestlength,unsigned int &bestoffset) {

  bestlength = 0;

  unsigned int hashvalue = hashValue (lookahead_output_position);
  if (hash_table [hashvalue].next == 0) {
    return;
  }


  // count and DEFLATELONGESTLENGTH are the limits for the longest match.
  unsigned int chain;
  unsigned int matchlimit = (count < DEFLATELONGESTLENGTH ? count : DEFLATELONGESTLENGTH);
  HashEntry *current;
  for (chain = 0 , current = hash_table [hashvalue].next;
       current != 0 && chain < search_limit;
       current = current->next, ++ chain) {

    unsigned int src = lookahead_output_position;
    unsigned int dest = current->index;
    unsigned int len ;

#if 0
    unsigned int limit = WindowOffsetToPosition (lz_position, current->index);
#else
    unsigned int limit = DEFLATEWINDOWSIZE
                - ((DEFLATEWINDOWSIZE + current->index - lz_position)
                    & DEFLATEWINDOWMASK);
#endif
    if (limit > matchlimit) {
      limit = matchlimit;
    }

    for (len = 0;
         len < limit;
         ++ len, AdvanceWindowPosition (dest), AdvanceLookaheadPosition (src)) {
      unsigned int lookahead = lookahead_buffer [src];
      unsigned int window = lz_window [dest];
      if (lookahead != window) {
        break;
      }
    }

    // We only care about matches longer than 3
    if (len >= 3 && len > bestlength) {
      bestlength = len;
      bestoffset = DEFLATEWINDOWSIZE
                - ((DEFLATEWINDOWSIZE + current->index - lz_position)
                    & DEFLATEWINDOWMASK);
      if (bestlength == DEFLATELONGESTLENGTH || bestlength == count) {
        break;
      }
    }
  } // outer for loop
  return;
}

//
//  Description:
//
//  This function performs the initializations required
//  before creating a new compressed data block.
//
void DeflateEncoder::prepareNewBlock () {
  length_table.reset ();
  distance_table.reset ();
  return;
}

//
//  Description:
//
//  This function compresses data within the lookahead buffer.
//
//  Parameters:
//
//  outputstream : The stream to write the compressed data to
//  count : The number of bytes in the lookahead buffer
//  endofstream : true => no more data,
//                  false => More data to be compressed
//
void DeflateEncoder::compressLookaheadData (DeflateOutputStream &outputstream,
                                            unsigned int count,
                                            bool endofstream) {
  // Here were are determining the number of characters to leave in the
  // lookahead buffer. For all but the last pass we stop seaching when
  // the number of characters left is less than the largest possible match
  // for a copy operation. This keeps use from using less than ideal matches.
  // The remaining characters will get processed on a subsequent call after
  // the buffer gets refilled.
  unsigned int limit;
  if (endofstream) {
    limit = 3;
  }
  else {
    limit = DEFLATELONGESTLENGTH;
  }

  while (count >= limit) {

    // See if we can find a match for the input in the
    // LZ window.
    unsigned int length;
    unsigned int offset;
    longestMatch (count, length, offset);
    if (length == 0) {
      // There is no match of at least three. Encode this value a
      // literal value.
      ubyte literal = lookahead_buffer [lookahead_output_position];
      unsigned int hashvalue = hashValue (lookahead_output_position);
      moveHashEntry (lz_position, hashvalue);

      lz_window[lz_position] = literal;

      block_buffer [block_buffer_count] = literal;
      ++ block_buffer_count;

      length_table.incrementFrequency (literal);

      AdvanceLookaheadPosition (lookahead_output_position);
      AdvanceWindowPosition (lz_position);

      -- count;
    }
    else {
      // We have found a match.  First update the hash table and
      // copy the data in the LZ window.
      unsigned int source = WindowOffsetToPosition (lz_position, offset);
      for (unsigned int ii = 0; ii < length; ++ ii, AdvanceWindowPosition (source)) {
        unsigned int hashvalue = hashValue (lookahead_output_position);
        moveHashEntry (lz_position, hashvalue);

        lz_window [lz_position] = lz_window [source];
        AdvanceWindowPosition (lz_position);
        AdvanceLookaheadPosition (lookahead_output_position);
      }

      block_buffer [block_buffer_count] = 256 + length;
      ++ block_buffer_count;
      block_buffer [block_buffer_count] = offset;
      ++ block_buffer_count;

      // Gather Huffman Statistics
      unsigned int code;
      unsigned int extra;
      unsigned int value;
      LengthToCode (length, code, extra, value);
           length_table.incrementFrequency (code);
      DistanceToCode (offset, code, extra, value);
           distance_table.incrementFrequency (code);

      count -= length;
    }  // else

    // Since a code can either require 1 or 2 values
    // we need to allow at least one extra space.
    if (block_buffer_count >= block_buffer_size - 1) {
      outputDeflateHeader (outputstream, false);
      outputBlockData (outputstream);
    }
  } // while

  // During the final call to this function we need to consume all the characters.
  // Since there is a minimum of 3 characters in a copy operation, we could have
  // up to 2 character remaining that have to be transfered as literals.
  if (endofstream) {
    assert(count <= 2);
    for (; count > 0; -- count) {
      ubyte literal = lookahead_buffer [lookahead_output_position];
      block_buffer [block_buffer_count] = literal;
      length_table.incrementFrequency (literal);
      ++ block_buffer_count;
      AdvanceLookaheadPosition (lookahead_output_position);
    }
    outputDeflateHeader (outputstream, true);
    outputBlockData (outputstream);
  }
  return;
}


//
//  Description:
//
//  An application calls this function when it wants to start a Deflate
//  compressed stream.
//
//  Parameters:
//
//  outputstream  : The output stream the encoder writes to.
//
//  Restrictions:
//
//  endCompressedStream must be called before calling this function
//  again.
//

void DeflateEncoder::startCompressedStream (DeflateOutputStream &outputstream) {
  if (compression_in_progress) {
    throw DeflateError ("Compression Already Underway");
  }

  // Buffer Allocation
  lz_window = new ubyte [DEFLATEWINDOWSIZE];
  lookahead_buffer = new ubyte [LOOKAHEADSIZE];
  hash_values = new HashEntry [DEFLATEWINDOWSIZE];
  hash_table = new HashEntry [HASHTABLESIZE];

  // Convert the compression level to the maximum depth hash
  // chains are searched.
  switch (compression_level) {
    case FASTESTCOMPRESSION: search_limit = 1;        break;
    case FASTCOMPRESSION:    search_limit = 64;       break;
    case DEFAULTCOMPRESSION: search_limit = 128;      break;
    case MAXIMUMCOMPRESSION: search_limit = UINT_MAX; break;
    default: throw DeflateError ("Invalid Compression Level");
  }

  // Checksum Initialization
  adler_value = 1;

  // Block Buffer Initialization
  block_buffer = new uint16 [block_buffer_size];
  block_buffer_count = 0;

  // LZ Window and Lookahead buffer initialization
  memset (lz_window, 0, DEFLATEWINDOWSIZE);
  lz_position = 0;
  lookahead_input_position = 0;
  lookahead_output_position = 0;
  initializeHashTable ();

  // Hash Table Initialization
  prepareNewBlock ();

  // Output the Deflate Header.
  outputstream.enterBitMode (0);
  outputZLibHeader (outputstream);

  compression_in_progress = true;
  return;
}

//
//  Description:
//
//  Applications call this function to suuply raw data to the
//  encoder. An application may make any number of calls
//  to this function.
//
//  Parameters:
//
//  outputstream  : The output stream the encoder writes to.
//  outputbuffer  : A pointer to the data to compress
//  outputcount   : The number of bytes in the output buffer.
//
//  Restrictions:
//
//  This function must be called after startCompressedStream and
//  before calling endCompressedStream.
//
//  An application should use the same outputstream supplied to
//  startCompressedStream in each call to this function.
//
void DeflateEncoder::compressData (DeflateOutputStream &outputstream,
                                   const char outputbuffer [],
                                   unsigned int outputcount) {

  // Ensure that the compressor is in a valid state.
  if (! compression_in_progress)
    throw DeflateError ("Compression Not Underway");

  // Feed the raw data unto the lookahead buffer.
  for (unsigned int ii = 0; ii < outputcount; ++ ii) {
    adler_value = Adler (adler_value, outputbuffer [ii]);
    lookahead_buffer [lookahead_input_position] = outputbuffer [ii];
    AdvanceLookaheadPosition (lookahead_input_position);

    // When the lookahead buffer is full, compress the data.
    if (lookahead_input_position == lookahead_output_position) {
      compressLookaheadData (outputstream, LOOKAHEADSIZE, false);
    }
  }
  return;
}

//
//  Description:
//
//  An application calls this function to inform the encoder that
//  all the data has been transmitted to the encoder.
//
//  Here we flush the lookahead buffer and write the checksum.
//
//  Parameters:
//
//  outputstream : The outputstream the encoder writes to.
//
//  Restrictions:
//
//  This function must follow a call to startCompressedStream.
//
//  An application should use the same outputstream supplied to
//  startCompressedStream.
//
void DeflateEncoder::endCompressedStream (DeflateOutputStream &outputstream) {
  if (! compression_in_progress) {
    throw DeflateError ("Compression Not Underway");
  }

  // Compress anything that remains in the lookahead buffer.
  unsigned int count = LOOKAHEADMASK
                     & (LOOKAHEADSIZE - lookahead_output_position + lookahead_input_position);

  compressLookaheadData (outputstream, count, true);

  // Output the final Adler32 value. We need to byte align this value.
  outputstream.exitBitMode ();
  outputstream.writeBigEndianLong (adler_value);

  // Final cleanup.
  freeBuffers ();
  compression_in_progress = false;
  return;
}

  } // namespace png
} // namespace lti

