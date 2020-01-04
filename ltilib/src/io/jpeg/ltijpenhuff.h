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
 * revisions ..: $Id: ltijpenhuff.h,v 1.5 2003/02/17 07:16:04 alvarado Exp $
 */


#ifndef _LTI_JPENHUFF_H_
#define _LTI_JPENHUFF_H_

//
//  Title:  JpegEncoderHuffmanTable class definition
//
//  Description:
//
//    This class represents a Huffman Table used for compression
//    by the JpegEncoder class.
//

#include "ltijpeg.h"
#include "ltijpgexcep.h"


namespace lti {
  namespace jpeg {

class JpegEncoder ;

/**
 * JPEG encoder huffman table
 */
class JpegEncoderHuffmanTable
{
public:
  JpegEncoderHuffmanTable () ;
  virtual ~JpegEncoderHuffmanTable () {}

  // ENCODING FUNCTIONS

  // This function resets the table so that the object can be used
  // over again.
  void Reset () ;

  // This function increases the frequency for a huffman value.
  void IncrementFrequency (int value) ;

  // This function creates the Huffman codes from the code frequencies.
  void BuildTable () ;

  // This function returns the Huffman code and code length to encode the
  // specified value.
  void Encode (int value, lti::uint16 &code, lti::ubyte &size) const ;

  // Returns the number of bytes required to write the table to the output
  // file.
  int OutputSize () const ;

  // Function to print the table definition to the output stream.
  void PrintTable (JpegEncoder &encoder) const ;

private:
  JpegEncoderHuffmanTable (const JpegEncoderHuffmanTable &) ;
  JpegEncoderHuffmanTable &operator=(const JpegEncoderHuffmanTable &) ;

  // This function builds the structures needed for Huffman
  // decoding after the table data has been read.
  void MakeTable () ;

  // frequencies [n] is the number of times the value "n" needs to
  // be encoded.
  int frequencies [JpegMaxNumberOfHuffmanCodes + 1] ;

  // Values used to represent Huffman tables in a JPEG stream
  //  huff_bits [n] is the number of codes of length "n+1"
  //  huff_values is the list of Huffman values sorted in order
  //   of code length.
  lti::ubyte huff_bits [2 * JpegMaxHuffmanCodeLength] ; // 2x needed for encoding only.
  lti::ubyte huff_values [JpegMaxNumberOfHuffmanCodes] ;

  // Values used to encode values.
  //   ehufsi [n] is the number of bits required to code "n"
  //   ehufco [n] is the Huffman code for "n"
  lti::ubyte ehufsi [JpegMaxNumberOfHuffmanCodes+ 1] ;
  lti::uint16 ehufco [JpegMaxNumberOfHuffmanCodes+1] ;

  // The flag is set to true when the Huffman code sizes has been determined.
  // It is cleared when the object is Reset().
  bool sizes_found ;
} ;

}
}
#endif

