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
 * file .......: jpgenhuff.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpenhuff.cpp,v 1.7 2003/02/17 07:16:04 alvarado Exp $
 */

//
// JPEG Encoder Library.
//
// Title:   EncoderHuffmanTable Class Implementation
//
//

#include "ltijpenhuff.h"
#include "ltijpegenco.h"


namespace lti {
  namespace jpeg {

//
//  Description:
//
//    Class default constructor
//
JpegEncoderHuffmanTable::JpegEncoderHuffmanTable ()
{
  Reset () ;
  return ;
}

//
//  Description:
//
//    After Huffman codes have been generated the object is in a state
//    where it cannot be used to create a new set of code. This function
//    places the object in a state where it can be reused to generate a
//    new set of Huffman Codes.
//

void JpegEncoderHuffmanTable::Reset ()
{
  memset (frequencies, 0, sizeof (frequencies)) ;
  // We add a dummy Huffman value at the end of the table with a minimumal
  // frequency. Since we create the Huffman codes using the in the frequency
  // table this dummy value will be assigned the longest all one huffman code.
  // This ensures that no Huffman code consists entirely of 1s.
  frequencies [256] = 1 ;
  sizes_found = false ;
  return ;
}

//
//  Description:
//
//    Function to increment the frequency for a value.
//
//  Parameters:
//    value:  The value whose frequency is to be incremented
//

void JpegEncoderHuffmanTable::IncrementFrequency (int value)
{
  // Once the Huffman codes have been generated for this object, the Reset()
  // function must be called before we can gather data again.
  if (sizes_found)
    throw JpegFatalError ("INTERNAL ERROR - Huffman Code sizes already found") ;

  if ((value >= JpegMaxNumberOfHuffmanCodes) || (value < 0)) {
    JpegIndexOutOfRange tmpExc;
    throw tmpExc;
  }

  ++ frequencies [value] ;
  return ;
}

//
//  Description:
//
//    This function generates the Huffman Codes using the frequency data. The code
//    generation process is taken directly from the JPEG standard.
//
//    The outputs from this function are the following member variables:
//
//     ehufsi [n] : The length of the Huffman Code for value "n"
//     ehufco [n] : The Huffman Code for value "n"
//     huff_bits [n] : The number of Huffman codes of length "n+1"
//     huff_values [n] : The Huffman Values sorted by code length.
//
//    The first two arrays are used to encode Huffman values. The last two
//    are for writing the table to the output file.
//
//    The code generation process is:
//
//    1. Arrange the Huffman Values into a binary tree so that the most
//       frequently used codes are closest to the root of the tree. At the end
//       of this process the temporary array codesize [n] contains the length
//       of the pure Huffman code for the value "n"
//
//    2. Determine the number of Huffman Codes of for each code length. This
//       step places the number of codes of length "n+1" in huff_bits[].
//
//    3. The JPEG standard only allows Huffman codes of up to 16-bits. If any
//       codes longer than 16-bits were generated in the previous steps then
//       we need to reduce the maximum depth of the tree created in step 1.
//       The input and output to this step is the array huff_bits[] created in
//       the previous step.
//
//    4. Remove the dummy all 1-bit code (See the Reset() function).
//
//    5. Sort the Huffman values in code length order. codesize [n] is the
//       input to this step and huff_values [n] is the output. At this point
//       all the information needed to write the Huffman Table to the output
//       stream has been found.
//
//    6. Determine the code size for each value. At the end of this step
//       the temporary array huffsizes [n] is the Huffman code length for
//       huff_values [n].
//
//    7. Determine the Huffman code for each value. The temporary array
//       huffcodes [n] is the Huffman Code of length huffsizes [n] for
//       the value huff_value [n].
//
//    8. Using huffsizes [] and huffcodes created in steps 6 and 7 create
//       the arrays ehufco [n] and ehufsi [n] giving the Huffman Code and
//       Code Size for n.
//
void JpegEncoderHuffmanTable::BuildTable ()
{
  // We need this because MSVC++ does not support standard
  // scoping in for statements.
  int ii, kk ;

  // See if we have already calculated the Huffman codes.
  if (sizes_found)
    return ;

  // The tmp array is used for validating the integrity of the Huffman code
  // table. We need a temporary array since frequencies [] gets trashed
  // during the code generation process.
	int tmp [JpegMaxNumberOfHuffmanCodes + 1] ;
  for (ii = 0 ; ii < JpegMaxNumberOfHuffmanCodes + 1 ; ++ ii)
   	tmp [ii] = frequencies [ii] ;

  // Figure K.1
  // Build the Huffman Code Length Lists
  int others [JpegMaxNumberOfHuffmanCodes + 1] ;
  for (ii = 0 ; ii < JpegMaxNumberOfHuffmanCodes + 1 ; ++ ii)
    others [ii] = -1 ;

  int codesize [JpegMaxNumberOfHuffmanCodes + 1] ;
  memset (codesize, 0, sizeof (codesize)) ;
  while (true)
  {
    // Find the two smallest non-zero values
    int v1 = -1 ;
    int v2 = -1 ;
    for (int ii = 0 ; ii < JpegMaxNumberOfHuffmanCodes + 1 ; ++ ii)
    {
      if (frequencies [ii] != 0)
      {
        // K.2 says to take the highest value value for v1 and v2
        // in case of a tie. This ensures the dummy value gets
        // the last Huffman code.
        if (v1 < 0 || frequencies [ii] <= frequencies [v1])
        {
          v2 = v1 ;
          v1 = ii ;
        }
        else if (v2 < 0 || frequencies [ii] <= frequencies [v2])
        {
          v2 = ii ;
        }
      }
    }
    if (v2 < 0)
      break ;

    // Join the two tree nodes.
    frequencies [v1] = frequencies [v1] + frequencies [v2] ;
    frequencies [v2] = 0 ;

    for (++ codesize [v1] ; others [v1] >= 0 ; ++ codesize [v1])
      v1 = others [v1] ;

    others [v1] = v2 ;

    for (++ codesize [v2] ; others [v2] >= 0 ; ++ codesize [v2])
      v2 = others [v2] ;
  }

  // Figure K.2
  // Determine the number of codes of length [n]
  memset (huff_bits, 0, sizeof (huff_bits)) ;
  for (ii = 0 ; ii < JpegMaxNumberOfHuffmanCodes + 1 ; ++ ii)
  {
    if (codesize [ii] != 0)
    {
      ++ huff_bits [codesize [ii] - 1] ;
    }
  }

  // Figure K.3
  // Ensure that no code is longer than 16-bits.
  for (ii = 2 * JpegMaxHuffmanCodeLength -  1 ;
       ii >= JpegMaxHuffmanCodeLength ;
       -- ii)
  {
    while (huff_bits [ii] != 0)
    {
      int jj = ii - 1 ;
      do
      {
        -- jj ;
      }
      while (huff_bits [jj] == 0) ;

      huff_bits [ii] -= 2 ;
      ++ huff_bits [ii - 1] ;
      huff_bits [jj + 1] += 2 ;
      -- huff_bits [jj] ;
    }
  }

  // Remove the reserved code from the end of the list.
  for (ii = JpegMaxHuffmanCodeLength - 1 ; ii >= 0 ; -- ii)
  {
    if (huff_bits [ii] != 0)
    {
      -- huff_bits [ii] ;
      break ;
    }
  }

  // Make sure that the total number of symbols is correct.
  int count = 0 ;
  for (ii = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
    count += huff_bits [ii] ;
  }
  if (count >= JpegMaxNumberOfHuffmanCodes)
    throw JpegFatalError ("INTERNAL ERROR - Too many codes defined") ;

  // Figure K.4
  // Sort the values in order of code length
  memset (huff_values, 0, sizeof (huff_values)) ;
  for (ii = 1, kk = 0 ; ii < 2 * JpegMaxHuffmanCodeLength ; ++ ii)
  {
    for (int jj = 0 ; jj < JpegMaxNumberOfHuffmanCodes ; ++ jj)
    {
      if ((int)codesize [jj]  == ii)
      {
        huff_values [kk] = (lti::ubyte)jj ;
        ++ kk ;
      }
    }
  }

  // Section C.2 Figure C.1
  // Convert the array "huff_bits" containing the count of codes for each
  // length 1..16 into an array containing the length for each code.
  int huffsizes [JpegMaxNumberOfHuffmanCodes] ;
  memset (huffsizes, 0, sizeof (huffsizes)) ;
  for (ii = 0, kk = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
     for (int jj = 0 ; jj < huff_bits [ii] ; ++ jj)
     {
        huffsizes [kk] = ii + 1 ;
        ++ kk ;
     }
     huffsizes [kk] = 0 ;
  }

  // Section C.2 Figure C.2
  // Calculate the Huffman code for each Huffman value.
  lti::uint16 code = 0 ;
  int huffcodes [JpegMaxNumberOfHuffmanCodes] ;
  memset (huffcodes, 0, sizeof (huffcodes)) ;
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

  // Section C.2 Figure C.3
  memset (ehufco, 0, sizeof (ehufco)) ;
  memset (ehufsi, 0, sizeof (ehufsi)) ;
  for (kk = 0 ; kk < JpegMaxNumberOfHuffmanCodes ; ++ kk)
  {
    if (huffsizes [kk] != 0)
    {
      int ii = huff_values [kk] ;
      ehufco [ii] = (lti::uint16)huffcodes [kk] ;
      ehufsi [ii] = (lti::ubyte)huffsizes [kk] ;
    }
  }

  // Validations
  // This remaining code is not necessary other than to ensure the
  // integrity of the Huffman table that is generated.

  // Make sure each value is used exactly once.
  for (ii = 0 ; ii < JpegMaxNumberOfHuffmanCodes ; ++ ii)
  {
    int count = 0 ;
    if (tmp [ii] != 0)
    {
      if (ehufsi [ii] == 0)
        throw JpegFatalError ("INTERNAL ERROR - Missing Huffman Code Size") ;

      for (int jj = 0 ; jj < JpegMaxNumberOfHuffmanCodes ; ++ jj)
      {
        if (ii == huff_values [jj] && huffsizes [jj] != 0)
          ++ count ;
        if (count > 1)
          throw JpegFatalError ("INTERNAL ERROR - Duplicate Huffman Value") ;
      }
      if (count == 0)
        throw JpegFatalError ("INTERNAL ERROR - Missing Huffman Value") ;
    }
  }

  // Ensure that each huffman code is used once annd that the values
  // are in range.
  for (ii = 0 ; ii < JpegMaxNumberOfHuffmanCodes ; ++ ii)
  {
    if (ehufsi [ii] != 0)
    {
      if (ehufsi [ii] > JpegMaxHuffmanCodeLength)
        throw JpegFatalError ("INTERNAL ERROR - Invalid Huffman Range") ;

      for (int jj = ii + 1 ; jj < JpegMaxNumberOfHuffmanCodes ; ++ jj)
      {
        if (ehufco [ii] == ehufco [jj] && ehufsi [jj] != 0)
          throw JpegFatalError ("INTERNAL ERROR - Duplicate Huffman Code") ;
      }
    }
  }

  sizes_found = true ;
  return ;
}

//
//  Description:
//
//    This function returns the Huffman Code and Code Size for a given value.
//
//  Parameters:
//    value:  The value to encode
//    code:   The Huffman Code
//    size:   The Huffman Code Length
//
void JpegEncoderHuffmanTable::Encode (int value,
                                      lti::uint16 &code,
                                      lti::ubyte &size) const
{
  if (value >= JpegMaxNumberOfHuffmanCodes) {
    JpegIndexOutOfRange tmpExc;
    throw tmpExc;
  }

  if (ehufsi [value] == 0)
    throw JpegFatalError ("INTERNAL ERROR - Missing Huffman Code") ;

  code = ehufco [value] ;
  size = ehufsi [value] ;
  return ;
}

//
//  Description:
//
//    This function writes the Huffman table data to the output stream in the
//    format specified by the JPEG standard.
//
//  Parameters:
//    encoder:    The JpegEncoder that defines the output stream.
//
void JpegEncoderHuffmanTable::PrintTable (JpegEncoder &encoder) const
{
  // We need this declaration here because MSVC++ does not support
  // standard scoping in for statements.
  int ii ;

  // B.2.4.2
  lti::ubyte data ;
  int count = 0 ; // Number of codes in the table.

  // Write 16 1-byte values containing the count of codes for
  // each possible Huffman code length and count the number of
  // codes used.
  for (ii = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
    count += huff_bits [ii] ;
    data = huff_bits [ii] ;
    encoder.OutputByte (data) ;
  }

  // Write the variable length part of the table, the Huffman values
  // sorted by Huffman Code.
  for (ii = 0 ; ii < count ; ++ ii)
  {
    data = huff_values [ii] ;
    encoder.OutputByte (data) ;
  }
  return ;
}
//
//  Description:
//
//    This function determines the size of the Huffman table when it is
//    written to a DHT marker. This function is used to calculate the
//    2-byte marker length that comes right after the FF-DHT sequence in
//    the output stream. Therefore we need to find the length before we
//    actually write the table.
//
int JpegEncoderHuffmanTable::OutputSize () const
{
  int count = 0 ;
  for (int ii = 0 ; ii < JpegMaxHuffmanCodeLength ; ++ ii)
  {
    count += huff_bits [ii] ;
  }

  // 1 byte for each value + one byte for each of 16 code lengths +
  // 1 byte for the table class and ID.
  return count + JpegMaxHuffmanCodeLength + 1 ;
}

}
}

