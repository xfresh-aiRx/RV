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
 * file .......: ltipnginflateinptstream.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: This class defines input required by the Inflate decoders.
 * revisions ..: $Id: ltipnginflateinputstream.h,v 1.4 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_INFLATEINPUTSTREAM_H
#define _LTI_INFLATEINPUTSTREAM_H


#include <climits>
#include "ltipnginputbytestream.h"

namespace lti{
  namespace png {

/**
 * Inflate Input Stream
 */
class InflateInputStream : virtual public InputByteStream {

public:

  int nextBit () ;
  int getBits (unsigned int count) ;
  void exitBitMode () ;
} ;

//
//  Description:
//
//    Retrieves the next bit in the input stream. The bits are extracted low to high.
//
//  Return Value:
//
//    The next bit in the input stream.
//
inline int InflateInputStream::nextBit () {
  if (bit_position >= CHAR_BIT) {
    ++ current_byte ;
    bit_position = 0 ;
    if (current_byte >= buffer_limit) {
      fillBuffer () ;
      if (! moreData ())
        throw StreamError ("Premature end of stream") ;
    }
  }
  // Extract the bit value
  int result ;
  static const unsigned int masks[] = { 1U, 2U, 4U, 8U, 16U, 32U, 64U, 128U, } ;
  if ((masks [bit_position] & *current_byte) != 0) {
    result = 1 ;
  }
  else {
    result = 0 ;
  }

  ++ bit_position ;
  return result ;
}

//
//  Description:
//
//  This function extracts a number of bits from the input stream. The bits
//  are extracted from low to high.
//
//  Parameter:
//
//  count: The number of bits to read.
//
//  Return Value:
//
//  'count' bits from the input stream.
//
inline int InflateInputStream::getBits (unsigned int count)  {

  static const unsigned int masks[] = { 0, 1U, 3U, 7U, 15U, 31U, 63U, 127U, 255U, } ;
  int result ;

  if(bit_position < 0) {
    throw StreamError("inflate inputstream: bit_position < 0");
  }

  if (count <= CHAR_BIT - (unsigned int)bit_position)  {
    // Easy case. We have already enough bits to process the request.
    result = (*current_byte >> bit_position) & masks [count] ;
    bit_position += count ;
  }
  else  {
    result = 0 ;
    int bitswritten = 0 ;
    // Save the remaining bits from the current byte.
    if (bit_position < CHAR_BIT)  {
      bitswritten = CHAR_BIT - bit_position ;
      result = (*current_byte >> bit_position)  & masks [bitswritten] ;
      count -= bitswritten ;
    }

    while (count > 0)  {
      // Move to the next byte.
      ++ current_byte ;
      bit_position = 0 ;
      if (current_byte >= buffer_limit) {
        fillBuffer () ;
        if (! moreData ()) {
          throw StreamError ("Premature end of file") ;
        }
      }

      if (count > CHAR_BIT)  {
        // We need the all of the new byte for the request.
        count -= CHAR_BIT ;
        result |= (*current_byte << bitswritten) ;
        bit_position = 0 ;
        bitswritten += CHAR_BIT ;
      }
      else  {
        result |= (*current_byte & masks [count]) << bitswritten ;
        bit_position += count ;
        break ; // All Done
      }
    }
  }
  return result ;
}

inline void InflateInputStream::exitBitMode () {
  if (bit_position >= 0) {
    ++ current_byte ;
  }
  bit_position = -1 ;
  return ;
}
  } // namespace png
} // namespace lti

#endif

