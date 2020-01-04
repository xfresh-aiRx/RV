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
 * file .......: ltipngdeflatepvt.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: PNG Encoder/Decoder private definitions
 * revisions ..: $Id: ltipngdeflatepvt.h,v 1.3 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_DEFLATEPVT_H
#define _LTI_DEFLATEPVT_H


#include <string>
#include "ltiAssert.h"

namespace lti{
  namespace png {
const unsigned int DEFLATEMAXLENGTHLENGTHCODES = 19 ;

const unsigned int DEFLATEMAXLENGTHCODES = 286 ;
const unsigned int DEFLATEMAXDISTANCECODES = 30 ;
const unsigned int DEFLATEMAXNUMBEROFHUFFMANCODES = DEFLATEMAXLENGTHCODES ;
const unsigned int DEFLATEFIRSTLENGTHCODE = 257 ;
const unsigned int DEFLATELENGTHCODECOUNT = DEFLATEMAXLENGTHCODES
                                          - DEFLATEFIRSTLENGTHCODE ;
const unsigned int DEFLATELITERALCOUNT = 256 ;
const unsigned int DEFLATEENDCODE = DEFLATELITERALCOUNT ;

// Maximum Huffman Code Sizes
const unsigned int DEFLATEMAXLENGTHLENGTHCODESIZE = 7 ;
const unsigned int DEFLATEMAXDISTANCECODESIZE = 15 ;
const unsigned int DEFLATEMAXLENGTHCODESIZE = 15 ;

const unsigned int DEFLATEWINDOWSIZE = (1 << 15) ;
const unsigned int DEFLATEWINDOWMASK = DEFLATEWINDOWSIZE - 1 ;

const unsigned int DEFLATELONGESTLENGTH = 258 ;

static const ubyte DEFLATELENGTHORDER [DEFLATEMAXLENGTHLENGTHCODES] =
{
  16, 17, 18, 0,  8, 7,  9, 6, 10, 5,
  11,  4, 12, 3, 13, 2, 14, 1, 15,
} ; //19

static inline const char *Binary (int vv, int ll) {

  static const char digit [2] = { '0', '1' } ;
  static char value [33] ;
  value [0] = '\000' ;
  for (unsigned int ii = ll , jj = 0 ; ii > 0 ; -- ii, ++ jj) {
    assert(jj<32);
    value [jj] = digit [(vv & (1 << (ii-1))) != 0 ] ;
    value [jj+1] = 0 ;
  }
  return value ;
}
  } // namespace png
} // namespace lti

#endif

