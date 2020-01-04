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
 * file .......: jpdehuff.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpdehuff.cpp,v 1.6 2003/02/17 07:16:03 alvarado Exp $
 */

//
// JPEG Decoder Huffman Table Class Implementation
//


#include "ltijpdehuff.h"
#include "ltijpegdeco.h"

using namespace std ;

namespace lti {
  namespace jpeg {

//
//  Description:
//
//    Class defualt constructor
//

JpegHuffmanDecoder::JpegHuffmanDecoder ()
{
  table_defined = false ;
  return ;
}

//
//  Description:
//
//    This function reads a Huffman table from the input stream. A DHT
//    marker can define more than one Huffman table. This function reads
//    just one of those tables.
//
//  Parameters:
//    decoder:  The JPEG decoder that owns the Huffman table
//
//  Return Value:
//    The size of the Huffman table in the input stream
//    (the number of bytes read).
//
int JpegHuffmanDecoder::ReadTable (JpegDecoder &decoder)
{
  // We declare this here because MSVC++ does not handle for
  // statement scoping rules correctly.
  int jj ;

  // B.2.4.2
  lti::ubyte huffbits [JpegMaxHuffmanCodeLength] ;

  int count = 0 ; // Count of codes in the Huffman table.

  // Read the 16 1-byte length counts and count the number of
  // codes in the table.
  for (jj = 0 ; jj < JpegMaxHuffmanCodeLength ; ++ jj)
  {
    // These values are called Li in the standard.
    huffbits [jj] = decoder.ReadByte () ;
    count += huffbits [jj] ;
  }
  if (count > JpegMaxNumberOfHuffmanCodes)
     throw JpegBadData ("Huffman count > 256") ;

  // Read the Huffman values.
  for (jj = 0 ; jj < count ; ++ jj)
  {
    // These values are called Vi in the standard.
    huff_values [jj] = decoder.ReadByte () ;
  }

  // Generate the Structures for Huffman Decoding.
  MakeTable (huffbits) ;

  table_defined = true ; // This table can now be used.

  return JpegMaxHuffmanCodeLength + count ;
}

//
//  Description:
//
//    This function generates the data used for Huffman decoding.
//
//    The implicit outputs are the member variables mincode [n],
//     maxcode [n] and valptr [n]. These are the minimum Huffman Code of
//    length n+1, the maximum Huffman Code of length n+1, and the index
//    into huff_values [] for the first value with a Huffman code of length
//    n+1.
//
//  Parameters:
//    huffibts: The count of Huffman codes of length n+1)
//
void JpegHuffmanDecoder::MakeTable (lti::ubyte huffbits [JpegMaxHuffmanCodeLength])
{
  // We have to declare the loop indices here because MSVC++ does not
  // handle scoping in for statements correctly.
  int ii, jj, kk ;

  // These values in these arrays correspond to the elements of the
  // "values" array. The Huffman code for values [N] is huffcodes [N]
  // and the length of the code is huffsizes [N].

  lti::uint16 huffcodes [JpegMaxNumberOfHuffmanCodes] ;
  int huffsizes [JpegMaxNumberOfHuffmanCodes + 1] ;

  // Section C.2 Figure C.1
  // Convert the array "huff_bits" containing the count of codes
  // for each length 1..16 into an array containing the length for each
  // code.
  for (ii = 0, kk = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
     for (int jj = 0 ; jj < huffbits [ii] ; ++ jj)
     {
        huffsizes [kk] = ii + 1 ;
        ++ kk ;
     }
     huffsizes [kk] = 0 ;
  }

  // Section C.2 Figure C.2
  // Calculate the Huffman code for each Huffman value.
  lti::uint16 code = 0 ;
  int si ;
  for (kk = 0, si = huffsizes [0] ;
       huffsizes [kk] != 0  ;
       ++ si, code <<= 1)
  {
     for ( ; huffsizes [kk] == si ; ++ code, ++ kk)
     {
        huffcodes [kk] = code ;
     }
  }

  // Section F.2.2. Figure F.15
  // Create three arrays.
  // mincode [n] : The smallest Huffman code of length n + 1.
  // maxcode [n] : The largest Huffman code of length n + 1.
  // valptr [n] : Index into the values array. First value with a code
  //                    of length n + 1.
  for (ii=0, jj=0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
     // ii is the index into Huffman code lengths
     // jj is the index into Huffman code values
     if (huffbits [ii] != 0)
     {
        // The jj'th Huffman value is the first with a Huffman code
        // of length ii.
        valptr [ii] = (lti::ubyte)jj ;
        mincode [ii] = huffcodes [jj] ;
        jj += huffbits [ii] ;
        maxcode [ii] = huffcodes [jj - 1] ;
     }
     else
     {
        // There are no Huffman codes of length (ii + 1).
        maxcode [ii] = -1 ;
        // An illegal value > maxcode[]
        mincode [ii] = JpegMaxNumberOfHuffmanCodes + 1 ;
        valptr [ii] = 0 ;
     }
  }
  return ;
}

//
//  Description:
//
//    This function decodes the next Huffman-encoded value in the input
//    stream.
//
//  Parameters:
//    decoder:  The JPEG decoder that owns the Huffman table.
//
int JpegHuffmanDecoder::Decode (JpegDecoder &decoder)
{
   // This function decodes the next byte in the input stream using this
   // Huffman table.

   // Section A F.2.2.3 Figure F.16

  lti::uint16 code = (lti::uint16)decoder.NextBit () ;
   int codelength ; // Called I in the standard.

   // Here we are taking advantage of the fact that 1 bits are used as
   // a prefix to the longer codes.
   for (codelength = 0 ;
        (code > maxcode [codelength] && codelength < JpegMaxHuffmanCodeLength) ;
        ++ codelength)
   {
      code = (lti::uint16) ((code << 1) | decoder.NextBit ()) ;
   }

   if (codelength >= JpegMaxHuffmanCodeLength)
    throw JpegBadData ("Bad Huffman Code Length") ;

   // Now we have a Huffman code of length (codelength + 1) that
   // is somewhere in the range
   // mincode [codelength]..maxcode [codelength].

   // This code is the (offset + 1)'th code of (codelength + 1) ;
   int offset = code - mincode [codelength] ;

   // valptr [codelength] is the first code of length (codelength + 1)
   // so now we can look up the value for the Huffman code in the table.
   int index = valptr [codelength] + offset ;
   return huff_values [index] ;
}

//
//  Description:
//
//    This is a debugging function for writing the contents of the Huffman
//    table to a stream.
//
//  Parameters:
//    strm:  The output stream
//
void JpegHuffmanDecoder::Dump (std::ostream &strm) const
{
  // We have the loop index here because MSVC++ does not handle
  // scoping in for statements correctly.

  int ii;
  int count=0;

  for (ii = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
    if (valptr [ii] < 255)
      count = valptr [ii] ;
  }

  strm << "   Code Values: " ;
  for (ii = 0 ; ii < count ; ++ ii)
    strm << dec << ii << hex << (lti::uint16) huff_values [ii] << " " ;
  strm << endl ;
  strm << "Length" << "\t\t" << "Mincode" << "\t\t"
       << "Maxcode" << "\t\t" << "Valptr" << endl ;
  strm << "-------------------------------------------------------" << endl ;
  for (ii = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
    strm << dec << ii << "\t\t" << (int) mincode [ii] << "\t\t"
         << (int)maxcode [ii] << "\t\t"<< (int) valptr [ii] << endl ;
  }
  return ;
}

//
//  Description:
//
//   This function tells if the Huffman table has been defined
//   by the JPEG input stream.  It is used to detect corrupt
//   streams that have scans that use a Huffman table before
//   it has been defined.
//
//  Return Value:
//
//    true => The table has been defind
//    false => The table has not been defined
//

bool JpegHuffmanDecoder::Defined () const
{
  return table_defined ;
}

}
}

