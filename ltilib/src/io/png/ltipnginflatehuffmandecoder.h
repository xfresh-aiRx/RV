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
 * file .......: ltipnginflatehuffmandecoder.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: This class handles Huffman decoding for PNG images.
 * revisions ..: $Id: ltipnginflatehuffmandecoder.h,v 1.6 2003/02/17 07:16:14 alvarado Exp $
 */


#ifndef _LTI_INFLATEHUFFMANDECODER_H
#define _LTI_INFLATEHUFFMANDECODER_H

#include <iostream>
#include "ltiVector.h"
#include "ltipnginflateinputstream.h"
#include "ltiAssert.h"

namespace lti {
  namespace png {

/**
 * Inflate Huffman Decoder
 */
class InflateHuffmanDecoder {
public:
  InflateHuffmanDecoder (){}
  virtual ~InflateHuffmanDecoder () {}

  //@{
  /**
   * Decoding functions
   */
  void makeTable (unsigned int maxcodesize,
                  unsigned int lengthcount,
                  const unsigned int huffbits []) ;


  int decode (InflateInputStream &) const ;
  //@}

private:
  InflateHuffmanDecoder (const InflateHuffmanDecoder &) ;
  InflateHuffmanDecoder &operator=(const InflateHuffmanDecoder &) ;

  // Maximum Huffman code value of length N
  vector<int> maxcode;
  // Minimum Huffman code value of length N
  vector<int> mincode;
  // Index into "values" for minimum code of length N
  vector<unsigned int> valptr  ;
  // Huffman values
  vector<unsigned int> huff_values ;

  unsigned int value_count ;
  unsigned int max_code_size ;
} ;
  } // namespace png
} // namespace lti




#if defined (INLINEHUFFMAN)

namespace lti{
  namespace png {

inline int InflateHuffmanDecoder::decode (InflateInputBitStream &inputstream) const {
  // We can't cheat by reading multiple bits here because read are reversing the
  // bits as they appear in the input stream.

  int code = inputstream.nextBit () ;
  unsigned int codelength ; // Code length minus 1

  // Here we are taking advantage of the fact that 1 bits are used as
  // a prefix to the longer codes.
  for (codelength = 0 ; (code > maxcode [codelength]) ; ++ codelength)  {
    assert(codelength < max_code_size)
    code = ((code << 1) | inputstream.nextBit ()) ;
  }

  if (codelngth >= max_code_size) {
    throw InflateInputBitStream::StreamError("Invalid Huffman Code") ;
  }


  // Now we have a Huffman code of length (codelength + 1) that
  // is somewhere in the range
  // mincode [codelength]..maxcode [codelength].

  // This code is the (offset + 1)'th code of (codelength + 1) ;
  int offset = code - mincode [codelength] ;

  // valptr [codelength] is the first code of length (codelength + 1)
  // so now we can look up the value for the Huffman code in the table.
  int index = valptr [codelength] + offset ;
  unsigned int value = huff_values [index] ;
  return value ;
}

  } // namespace png
} // namespace lti
#endif



#endif

