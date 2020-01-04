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
 * file .......: ltipnginflatehuffmanndecoder.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: implementation of Huffmann decoder for PNG images
 * revisions ..: $Id: ltipnginflatehuffmandecoder.cpp,v 1.6 2003/02/17 07:16:14 alvarado Exp $
 */

#include <iostream>

#include "ltipnginflatehuffmandecoder.h"
#include "ltipnginflateinputstream.h"
#include "ltiAssert.h"

namespace lti {
  namespace png {
const unsigned int BADCODE = 0xFFFFFFF ;


static inline const char *Binary (int vv, int ll)
{
  static const char digit [2] = { '0', '1' } ;
  static char value [33] ;
  value [0] = '\000' ;
  for (unsigned int ii = ll , jj = 0 ; ii > 0 ; -- ii, ++ jj) {
    value [jj] = digit [(vv & (1 << (ii-1))) != 0 ] ;
    value [jj+1] = 0 ;
  }
  return value ;
}

//
//  Description:
//
//  This function creates a Huffman table from an array of code lengths.
//
//  Parameters:
//  valuecount: The number of huffman values
//  codelengths: Array of code lengths [0..valuecount-1]
//  codelengths [n] is the length of the Huffman code for the value n.
//
void InflateHuffmanDecoder::makeTable (unsigned int maxcodesize,
                                unsigned int valuecount,
                                const unsigned int codelengths []) {


  huff_values.resize(valuecount,(unsigned int)0,false,false) ;
  max_code_size = maxcodesize ;

  // We need this because MSVC++ does not follow standard scoping rules
  // in for statements.
  unsigned int ii ;

  value_count = valuecount ;
  vector<unsigned int> huffsizes;
  huffsizes.resize(valuecount + 1,(unsigned int)0,false,false);
  huffsizes.at(valuecount)=0;

  // Set up arrays to associate codes with code lengths. We have to do a copy
  // because this function can be called using fixed Huffman codes.
  // the following code lines are an efficient implementation of:
  //    for (ii = 0 ; ii < valuecount ; ++ ii) {
  //      huff_values.at(ii) = ii ;
  //      huffsizes.at(ii) = codelengths[ii] ;
  //    }

  memcpy(&huffsizes.at(0),codelengths,valuecount*sizeof(int));
  unsigned int *uiptr;

  for (ii=0, uiptr=&huff_values.at(0);
       ii < valuecount;
       ++ii, ++uiptr) {
    *uiptr = ii;
  }

  // Sort the values. Primary key is code size. Secondary key is value.
  //    for (ii = 0 ; ii < valuecount - 1 ; ++ ii) {
  //      for (unsigned int jj = ii + 1 ; jj < valuecount ; ++ jj) {
  //        if (huffsizes.at(jj) < huffsizes.at(ii)
  //            || (huffsizes.at(jj) == huffsizes.at(ii)
  //                && huff_values.at(jj) < huff_values.at(ii))) {
  //          unsigned int tmp ;
  //          tmp = huffsizes.at(jj) ;
  //          huffsizes.at(jj) = huffsizes.at(ii) ;
  //          huffsizes.at(ii) = tmp ;
  //          tmp = huff_values.at(jj) ;
  //          huff_values.at(jj) = huff_values.at(ii) ;
  //          huff_values.at(ii) = tmp ;
  //        }
  //      }
  //    }

  unsigned int *iiptr,*jjptr,*iiptrhv,*jjptrhv,*ehs,*ejhs;

  ehs = &huffsizes.at(0);
  ehs += (valuecount-1);
  ejhs = ehs+1;
  unsigned int tmp ;

  for (iiptr = &huffsizes.at(0),iiptrhv = &huff_values.at(0);
       iiptr != ehs;
       ++iiptr, ++iiptrhv) {
    for (jjptr = iiptr + 1,jjptrhv=iiptrhv+1 ;
         jjptr != ejhs;
         ++ jjptr,++jjptrhv) {
      if ((*jjptr < *iiptr) ||
          ((*jjptr == *iiptr) && (*jjptrhv < *iiptrhv))) {
        tmp = *jjptr;
        *jjptr = *iiptr;
        *iiptr = tmp;

        tmp = *jjptrhv;
        *jjptrhv = *iiptrhv;
        *iiptrhv = tmp;
      }
    }
  }

  // These values in these arrays correspond to the elements of the
  // "values" array. The Huffman code for values [N] is huffcodes [N]
  // and the length of the code is huffsizes [N].

  vector<unsigned int> huffcodes (valuecount,(unsigned int)0) ;
  unsigned int lastlen ;
  unsigned int code ;
  //    for (ii = 0, lastlen = 0, code = 0  ; ii < valuecount ; ++ ii) {
  //      while (lastlen != huffsizes.at(ii)) {
  //        ++ lastlen ;
  //        code <<= 1 ;
  //      }

  //      if (lastlen != 0) {
  //        huffcodes.at(ii) = code ;
  //        ++ code ;
  //      }
  //    }

  for (iiptr=&huffsizes.at(0), iiptrhv=&huffcodes.at(0), lastlen=0, code=0;
       iiptr!=ejhs;
       ++ iiptr, ++iiptrhv) {
    while (lastlen != *iiptr) {
      ++ lastlen ;
      code <<= 1 ;
    }

    if (lastlen != 0) {
      *iiptrhv = code ;
      ++ code ;
    }
  }

  // mincode [n] : The smallest Huffman code of length n + 1.
  // maxcode [n] : The largest Huffman code of length n + 1.
  // valptr [n] : Index into the values array. First value with a code
  //                    of length n + 1.

  maxcode.resize(max_code_size,-1,false,true);
  mincode.resize(max_code_size,BADCODE,false,true);
  valptr.resize(max_code_size,0,false,true);  //valptr.resize(valuecount);

  unsigned int last ;
  for (ii = 0, last = 0 ; ii < valuecount ; ++ ii) {
    if (last != huffsizes.at(ii)) {
      last = huffsizes.at(ii) ;
      valptr.at(last-1) = ii ;
      mincode [last-1] = huffcodes.at(ii) ;
    }
    if (last != 0) {
      maxcode.at(last-1) = huffcodes.at(ii) ;
    }
  }

  return ;


}

//
//  Description:
//
//  This function decodes the next Huffman-encoded value in the input
//  stream.
//
//  Parameters:
//  decoder:  The PNG Decoder object.
//
#if ! defined (INLINEHUFFMAN)
int InflateHuffmanDecoder::decode (InflateInputStream &inputstream) const {
  // We can't cheat by reading multiple bits here because read are reversing the
  // bits as they appear in the input stream.

  int code = inputstream.nextBit () ;
  unsigned int codelength ; // Code length minus 1

  // Here we are taking advantage of the fact that 1 bits are used as
  // a prefix to the longer codes.
  for (codelength = 0;(code > maxcode.at(codelength));++ codelength) {
    assert(codelength < max_code_size);
    code = ((code << 1) | inputstream.nextBit ()) ;
  }

  // Now we have a Huffman code of length (codelength + 1) that
  // is somewhere in the range
  // mincode [codelength]..maxcode [codelength].

  // This code is the (offset + 1)'th code of (codelength + 1) ;
  int offset = code - mincode.at(codelength) ;

  // valptr [codelength] is the first code of length (codelength + 1)
  // so now we can look up the value for the Huffman code in the table.


  int index = valptr.at(codelength) + offset ;

  unsigned int value = huff_values.at(index) ;


  return value ;
}
#endif


  } // namespace png
} // namespace lti

