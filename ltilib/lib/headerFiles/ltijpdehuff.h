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
 * file .......: jpdehuff.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpdehuff.h,v 1.5 2003/02/17 07:16:03 alvarado Exp $
 */

#ifndef _LTI_JPDEHUFF_H_
#define _LTI_JPDEHUFF_H_

//
//  Title:  JPEG Decoder Huffman Table Class Implementation
//
//  Description:
//
//    This class represents a Huffman Table used by the JpegDecoder
//    class.
//


#include "ltijpeg.h"
#include "ltijpgexcep.h"


namespace lti {
  namespace jpeg {

class JpegDecoder ;

/**
 * Jpeg Huffman decoder
 */
class JpegHuffmanDecoder
{
public:
  JpegHuffmanDecoder () ;
  virtual ~JpegHuffmanDecoder () {}

  // DECODING FUNCTIONS

  // Returns true if the table has been defined...in other words,
  // if ReadTable () has completed successfully. This function is
  // called before the table is used to decode a scan to ensure
  // the the image does not reference a Huffman Table that has
  // not been defined.
  bool Defined () const ;

  // This function reads a Huffman table from the input stream.
  int ReadTable (JpegDecoder &) ;

  // Function to decode the next value in the input stream.
  int Decode (JpegDecoder &) ;

  // This is a debugging function that writes the Huffman table
  // to a streamt.
  void Dump (std::ostream &strm) const ;

private:
  JpegHuffmanDecoder (const JpegHuffmanDecoder &) ;
  JpegHuffmanDecoder &operator=(const JpegHuffmanDecoder &) ;

  // This function builds the structures needed for Huffman
  // decoding after the table data has been read.
  void MakeTable (lti::ubyte huffbits [JpegMaxHuffmanCodeLength]) ;

  // Maximum Huffman code value of length N
  int maxcode [JpegMaxHuffmanCodeLength] ;
  // Minimum Huffman code value of length N
  int mincode [JpegMaxHuffmanCodeLength] ;
  // Index into "values" for minimum code of length N
  lti::ubyte valptr [JpegMaxHuffmanCodeLength] ;
  // Huffman values
  lti::ubyte huff_values [JpegMaxNumberOfHuffmanCodes] ;

  bool table_defined ;
} ;

  }
}

#endif

