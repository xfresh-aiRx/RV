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
 * file .......: ltipngdeflateoutpustream.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: abstract class for output stream suitable for deflate compression.
 * revisions ..: $Id: ltipngdeflateoutputstream.h,v 1.4 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_DEFALTEOUTPUTSTREAM_H
#define _LTI_DEFALTEOUTPUTSTREAM_H

//#include <climits>
#include <limits.h>
#include "ltiAssert.h"
#include "ltiTypes.h"
#include <stdexcept>
#include "ltipngoutputbytestream.h"


namespace lti{
  namespace png {
/**
 * PNG deflate output stream
 */
class DeflateOutputStream : virtual public OutputByteStream {
public:
  DeflateOutputStream () {}
  virtual ~DeflateOutputStream () {}
  void writeBits (int bits, int count) ;
  void exitBitMode () ;

private:
  DeflateOutputStream (const DeflateOutputStream &) ;
  DeflateOutputStream &operator=(const DeflateOutputStream &) ;
} ;

inline void DeflateOutputStream::writeBits (int bits, int count) {
  assert(count <= 16) ;
  assert(bit_position >= 0 && bit_position <= CHAR_BIT) ;


  static const uint32 masks [32] =
  {
    1,     1<<1,  1<<2,  1<<3,  1<<4,  1<<5,  1<<6,  1<<7,
    1<<8,  1<<9,  1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15,
    1<<16, 1<<17, 1<<18, 1<<19, 1<<20, 1<<21, 1<<22, 1<<23,
    1<<24, 1<<25, 1<<26, 1<<27, 1<<28, 1<<29, 1<<30, 1<<31,
  } ;
  static const uint32 complements [8] =
  {
    ~1,     ~(1<<1),  ~(1<<2),  ~(1<<3),  ~(1<<4),  ~(1<<5),  ~(1<<6),  ~(1<<7),
  } ;

  for (int ii = 0 ; ii < count ; ++ ii, ++ bit_position) {
    if (bit_position >= CHAR_BIT) {
      nextByte () ;
      bit_position = 0 ;
    }
    if ((bits & masks [ii]) != 0) {
      *current_byte |= masks [bit_position] ;
    }
    else {
      *current_byte &= complements [bit_position] ;
    }
  }
  return ;
}

inline void DeflateOutputStream::exitBitMode () {
  if (bit_position != 0) {
    nextByte () ;
  }

  bit_position = -1 ;
  return ;
}


  } // namespace png
} // namespace lti

#endif

