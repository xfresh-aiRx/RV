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
 * file .......: ltipnginflatedecoder.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: implementation for Deflate/Inflate compression decoder
 * revisions ..: $Id: ltipnginflatedecoder.cpp,v 1.4 2003/02/17 07:16:14 alvarado Exp $
 */



#include <iomanip>

#include "ltipnginflatehuffmandecoder.h"
#include "ltipnginflatedecoder.h"
#include "ltipngdeflatepvt.h"
#include "ltipnginflateinputstream.h"
#include "ltipngadler32.h"
#include "ltipngendian.h"

#define VERYVERBOSE

namespace lti {
  namespace png {

// Deflate Compression Types
enum CompressionType {
  Uncompressed = 0,
  FixedHuffmanCodes = 1,
  DynamicHuffmanCodes = 2,
} ;


//
// Class Constructor
//
// Nothing gets done here except initializing variables to a known state.
//
InflateDecoder::InflateDecoder ()
: lz_window (0),
  literal_table (new InflateHuffmanDecoder),
  distance_table (new InflateHuffmanDecoder) {
  return ;
}

//
// Class Destructor
//
InflateDecoder::~InflateDecoder () {
  freeData ();

  if(distance_table != 0) {
    delete distance_table;
    distance_table = 0 ;
  }

  if(literal_table != 0) {
    delete literal_table;
    literal_table = 0 ;
  }

  return ;
}

//
// Description:
//
// This function frees all of the dynamically allocated data.
//
void InflateDecoder::freeData () {
  delete [] lz_window ; lz_window = 0 ;
  return ;
}

//
//  Description:
//
//  This function reads Huffman-encoded code lengths for another
//  Huffman table. The Huffman encoded values range from 0..18.
//  The values have the following meanings:
//
//    0..15=>A literal length value
//    16=>Repeat the last code N times. N is found by reading
//        the next 2 bits and adding 3.
//    17=>Set the N length codes to zero. N is found by reading the
//        next 3 bits and adding 3.
//    18=>Set the N length codes to zero. N is found by reading the
//        next 7 bits and adding 11.
//
//  Parameters:
//  ht:  The Huffman table used to decode the input stream.
//  lengths: The output length values
//  lengthcount:  The number of length values to read.
//
//
void InflateDecoder::readLengths (InflateInputStream &strm,
                                  InflateHuffmanDecoder &ht,
                                  unsigned int lengths [],
                                  unsigned int lengthcount) {

  for (unsigned int index = 0 ; index < lengthcount ;) {

    int command = ht.decode (strm) ;
    if (command < 16) {
      // Raw Length
      lengths [index] = command ;
      ++ index ;
    }
    else if (command == 16) {
      // Repeat previous
      int count = strm.getBits (2) + 3 ;

      for (int ii = 0 ; ii < count ; ++ ii) {
        if (index == lengthcount) {
          throw StreamError ("Length Command Out of Range") ;
        }

        lengths [index] = lengths [index - 1] ;
        ++ index ;
      }
    }
    else if (command == 17) {
      // Run of zeros
      int count = strm.getBits (3) + 3 ;

      for (int ii = 0 ; ii < count ; ++ ii) {
        if (index == lengthcount) {
          throw  StreamError ("Length Command Out of Range") ;
        }

        lengths [index] = 0 ;
        ++ index ;
      }
    }
    else if (command == 18) {
      // Longer run of zeros
      int count = strm.getBits (7) + 11 ;

      for (int ii = 0 ; ii < count ; ++ ii) {
        if (index == lengthcount) {
          throw StreamError ("Length Command Out of Range") ;
        }
        lengths [index] = 0 ;
        ++ index ;
      }
    }
    else {
       throw StreamError ("Bad Length Code") ;
    }
  }
  return ;
}

int InflateDecoder::decode (InflateInputStream &strm, unsigned int count, ubyte output []) {
  unsigned int ii = 0 ;
  while (ii < count) {
    if (literal_count > 0) {
      while (literal_count > 0 && ii < count) {
        ubyte result ;
        -- literal_count ;
        result = strm.getByte () ;
        lz_window [window_position] = result ;
        window_position = (window_position + 1) & DEFLATEWINDOWMASK ;
        stream_adler = Adler (stream_adler, result) ;
        output [ii] = result ; ++ ii ;
      }
      if (literal_count == 0 && ! final_data_set) {
        startNewDataSet (strm);
      }
    }
    else if (copy_count > 0) {

      // We have two idential loops except for the test. For performance,
      // instead of having two loop test, we identify which test will fail
      // first and test it alone.
      if (count - ii > copy_count) {
        // See if we are still processing a copy operation.
        while (copy_count != 0) {
          // Copy the value in the LZ window.
          ubyte value = lz_window [copy_position] ;
          lz_window [window_position] = value ;
          -- copy_count ;

          // Advance the copy and window positions.
          copy_position = (copy_position + 1) & DEFLATEWINDOWMASK; // 0x7FFF ;
          window_position = (window_position+1) & DEFLATEWINDOWMASK; //0x7FFF ;
          // Update the Adler checksum.
          stream_adler = Adler (stream_adler, value) ;
          output [ii] = value ; ++ ii ;
        }
      }
      else {
        // See if we are still processing a copy operation.
        while (ii < count) {
          // Copy the value in the LZ window.
          ubyte value = lz_window [copy_position] ;
          lz_window [window_position] = value ;
          -- copy_count ;

          // Advance the copy and window positions.
          copy_position = (copy_position + 1) & DEFLATEWINDOWMASK; //0x7FFF ;
          window_position = (window_position+1) & DEFLATEWINDOWMASK; //0x7FFF ;

          // Update the Adler checksum.
          stream_adler = Adler (stream_adler, value) ;
          output [ii] = value ; ++ ii ;
        }
      }
    }
    else {

      // The number of extra bits for code-257
      static const int length_extra [DEFLATELENGTHCODECOUNT] =
      {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2,
        2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0,
      } ;
      // The smallest length value for code-257. The actual length value is
      // the sum of this value and the extra bits.
      static const int length_base [DEFLATELENGTHCODECOUNT] =
      {
         3,   4,   5,   6,   7,   8,   9,  10,  11,  13,
        15,  17,  19,  23,  27,  31,  35,  43,  51,  59,
        67,  83,  99, 115, 131, 163, 195, 227, 258
      } ;
      // The number of extra bits for a distance code.
      static const int distance_extra [DEFLATEMAXDISTANCECODES] =
      {
        0,  0,  0,  0,  1,  1,  2,  2,  3,  3, 4,  4,  5,  5,  6,
        6,  7,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13,
      } ;
      // The smallest value for a distance code.
      static const int distance_base [DEFLATEMAXDISTANCECODES] =
      {
           1,    2,     3,     4,     5,    7,    9,    13,    17,    25,
          33,   49,    65,    97,   129,  193,  257,   385,   513,   769,
        1025, 1537,  2049,  3073,  4097, 6145, 8193, 12289, 16385, 24577,
      } ;
      unsigned int value = literal_table->decode (strm) ;
      unsigned int length ;
      unsigned int distance ;

      if (value < DEFLATELITERALCOUNT) {
        // This is a data value. Add the value to the LZ window and update the
        // Adler checksum.
        lz_window [window_position] = value ;
        window_position = (window_position + 1) & DEFLATEWINDOWMASK ;
        stream_adler = Adler (stream_adler, value) ;
        output [ii] = value ; ++ ii ;
      }
      else if (value == DEFLATEENDCODE) {
        // We just read the end marker. There should be another data set in the
        // input stream that contains the data value.
        if (final_data_set) {
          // The current data set end the final bit set. That means there should
          // be no more data sets in the stream.
          no_more_data = true ;
          return ii ;
        }
        else {
          // The data value is in the next data set.
          startNewDataSet (strm) ;
        }
      }
      else if (value < DEFLATEMAXLENGTHCODES) {
        // The code specifies a length value. Read the extra bits
        // to find the actual length value.
        int extra = length_extra [value - DEFLATEFIRSTLENGTHCODE] ;
        length = length_base [value - DEFLATEFIRSTLENGTHCODE] ;
        if (extra != 0)
          length += strm.getBits (length_extra [value - DEFLATEFIRSTLENGTHCODE]) ;

        // The length value is followed by the distance value. Decode the
        // value then add the extra bits to get the distance value.
        value = distance_table->decode (strm) ;
        if (value > 29)
          throw StreamError ("Invalid Huffman Distance Value") ;
        extra = distance_extra [value] ;
        distance = distance_base [value] ;
        if (extra != 0)
          distance += strm.getBits (extra) ;

        // Set of the state variables that are used to find the following copied
        // bytes.
        copy_position = (DEFLATEWINDOWSIZE + window_position - distance) & DEFLATEWINDOWMASK ;
        copy_count = length ;
        // Return the first copy byte.
        value = lz_window [copy_position] ;
        lz_window [window_position] = value ;
        copy_position = (copy_position + 1) & DEFLATEWINDOWMASK ;
        window_position = (window_position + 1) & DEFLATEWINDOWMASK ;
        -- copy_count ;
        stream_adler = Adler (stream_adler, value) ;
        output [ii] = value ; ++ ii ;
      }
      else {
        throw StreamError ("Invalid Huffman Literal Value") ;
      }
    }
  }
  return ii ;
}

//
//  Description:
//
//  This function processes the start of a new data set within a compressed
//  stream. The start of a data set has the following format:
//
//  final: 1-bit (1 => this is the last data set)
//  compression type:  2-bits
//  The remainder depends upon the compression type.
//
//  Compression Type:  Uncompressed
//
//  Advance to the next byte boundary. The next two bytes is the
//  length of the uncompressed data. The following two bytes
//  are the ones complement of the length.  [length] uncompressed
//  bytes follow.
//
//  Compression Type:  Fixed Huffman Codes
//
//  The huffman encoded data bits immediately follow the type field. The
//  data is encoded using the huffman lengh codes defined in the deflate
//  specification.
//
//  Compression Type: Dynamic Huffman Codes
//
//  The trick here is that the literal and distance Huffman
//  tables are Huffman-encoded. The next values in the input
//  stream are:
//  number of literal codes: 5-bits + 257
//  number of distance codes: 5-bits + 1
//  number of code lengths: 4-bits + 4
//
//  Code Lengths: 3-bits * (code-lengths + 4)
//
//  The code lengths are used to create a huffman table that encodes
//  the literal table followed by the length table.
//
void InflateDecoder::startNewDataSet (InflateInputStream &strm) {
  strm.enterBitMode (0) ;
  if (strm.getBits (1) == 0) {
    final_data_set = false ;
  } else {
    final_data_set = true ;
  }

  CompressionType compressiontype =
      static_cast<CompressionType>(strm.getBits (2));

  if (compressiontype == Uncompressed) {
    strm.exitBitMode () ;
    literal_count = strm.getLittleEndianWord () ;
    uint16 testcount = strm.getLittleEndianWord () ;
    if ((literal_count & 0xFFFF) != (~testcount & 0xFFFF) || literal_count == 0) {
      throw StreamError ("Invalid Literal Count") ;
    }
  }
  else if (compressiontype == FixedHuffmanCodes) {
    // These are the length values that define the
    // literal huffman table.
    static const unsigned int literals [288] =
      {
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8,
      } ;
    // These length values define the distance huffman table.
    static const unsigned int distances [32] =
      {
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
      } ;
    literal_table->makeTable (DEFLATEMAXLENGTHCODESIZE, 288, literals) ;   // [288]
    distance_table->makeTable (DEFLATEMAXDISTANCECODESIZE, 32, distances) ; // unsigned int[32]
    literal_count = 0 ;
  }
  else if (compressiontype == DynamicHuffmanCodes) {

    unsigned int HLIT = strm.getBits (5) ;
    unsigned int HDIST = strm.getBits (5) ;
    unsigned int HCLEN = strm.getBits (4) ;


    // Read the length codes used to huffman encode the literal and
    // distance tables. The unusual thing here is the Huffman values
    // are not in the order 0..18 but rather the order defined by
    // the lengthindices array.
    if (HCLEN + 4 > DEFLATEMAXLENGTHLENGTHCODES)
      throw StreamError ("Invalid Huffman Code Length") ;
    unsigned int lengths [DEFLATEMAXLENGTHLENGTHCODES] ; // =19
    memset (lengths, 0, sizeof (lengths)) ;

    for (unsigned int ii = 0 ; ii < HCLEN + 4 ; ++ ii) {
      lengths [DEFLATELENGTHORDER [ii]] = strm.getBits (3) ;
    }

    InflateHuffmanDecoder ht ;
    ht.makeTable (DEFLATEMAXLENGTHLENGTHCODESIZE,
                  DEFLATEMAXLENGTHLENGTHCODES,
                  lengths) ; //unsigned  int[19]

    // Using the Huffman table we just created read the length/literals
    // and distances Huffman tables.
    unsigned int literals [288] ;
    readLengths (strm, ht, literals, HLIT + 257) ;
    unsigned int distances [32] ;
    readLengths (strm, ht, distances, HDIST + 1) ;

    literal_table->makeTable (DEFLATEMAXLENGTHCODESIZE, HLIT + 257, literals) ; //[288]
    distance_table->makeTable (DEFLATEMAXDISTANCECODESIZE, HDIST + 1, distances) ; //[32]
    literal_count = 0 ;
  }
  else {
    throw StreamError ("Invalid Compression Type") ;
  }
  return ;
}

//
//  Description:
//
//    This function is called after all of the data is read to check the
//    the Adler checksum.
//
void InflateDecoder::checkAdler (InflateInputStream &strm) {
  strm.exitBitMode () ;
  // After the end marker the next 4 bytes should be the adler checksum
  uint32 streamvalue = strm.getBigEndianLong () ;

  if (streamvalue != stream_adler)
    throw StreamError ("Stream Adler 32 Checksum error") ;

  return ;
}

void InflateDecoder::openStream (InflateInputStream &strm) {
  no_more_data = false ;
  lz_window = new ubyte [DEFLATEWINDOWSIZE] ;
  memset (lz_window, 0, DEFLATEWINDOWSIZE) ;

  stream_adler = 1 ;  // The Adler 32 is always initialized to one.

  // Initialize the LZ compression state variables.
  window_position = 0 ;
  copy_position = 0 ;
  copy_count = 0 ;

  // Read the copressed stream header.
  ubyte data1 = strm.getByte () ;
  int CM = (data1 & 0x0F) ; // Compression Method
  int CINFO = (data1 & 0xF0) >> 4 ;
  int windowsize = (1 << (CINFO + 8)) ;

  ubyte data2 = strm.getByte () ;
  bool FDICT = (data2 & (1 << 5)) != 0 ;
  //int FLEVEL = (data2 & 0xC0) >> 6 ;

  // The header values are checked below after they have been printed out.

  // Make sure the header values are valid for PNG.
  if (CM != 8) {
    throw StreamError ("Invalid Compression Method - Not (8) Deflate") ;
  }

  if ((data2 | (data1 << 8)) % 31 != 0) {
    throw StreamError ("Corrupt Compression Header flags") ;
  }

  if (windowsize > (1 << 15)) {
    throw StreamError ("Invalid Compression Window") ;
  }

  if (FDICT) {
    throw StreamError ("Preset dictionary flag set") ;
  }

  // Read the start of the new Deflate data set.
  startNewDataSet (strm) ;
  return ;
}

  } // namespace png
} // namespace lti

