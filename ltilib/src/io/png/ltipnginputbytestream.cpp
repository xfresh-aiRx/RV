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
 * file .......: ltipnginputbytestream.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: InputByteStream is an abstract class for handing the input
 *               from a stream of bytes.
 * revisions ..: $Id: ltipnginputbytestream.cpp,v 1.4 2003/02/17 07:16:14 alvarado Exp $
 */



#include "ltipnginputbytestream.h"
#include "ltiAssert.h"
#include "ltipngendian.h"

namespace lti {
  namespace png {


//
// Description:
//
// Default Constructor
//
InputByteStream::InputByteStream ()
: current_byte (0),
  buffer_limit (0),
  bit_position (-1) {
  return ;
}
//
//  Description:
//
//  The function returns the next byte in the input stream.
//
//  Return Value:
//
// The next byte.
//
ubyte InputByteStream::getByte () {

  assert(bit_position < 0);
  if (current_byte >= buffer_limit) {
    fillBuffer () ;
  }

  byte result = *current_byte ;
  ++ current_byte ;
  return result ;
}
//
//  Description:
//
//  The function returns the next 2-byte integer in the input stream interpreted
//  as a big endian value.
//
//  Return Value:
//
//  The next 2-byte integer converted from big-endian to system format..
//

uint16 InputByteStream::getBigEndianWord () {
  assert(bit_position < 0);
  uint16 result ;
  read (reinterpret_cast<char*>(&result), sizeof (result)) ;
  result = BigEndianToSystem (result) ;
  return result ;
}

uint16 InputByteStream::getLittleEndianWord ()
{
  assert(bit_position < 0);
  uint16 result ;
  read (reinterpret_cast<char*>(&result), sizeof (result)) ;
  result = LittleEndianToSystem (result) ;
  return result ;
}

//
//  Description:
//
//  The function returns the next 4-byte integer in the input stream interpreted
//  as a big endian value.
//
//  Return Value:
//
//  The next 4-byte integer converted from big-endian to system format..
//
uint32 InputByteStream::getBigEndianLong () {

  assert(bit_position < 0);
  uint32 result ;
  read (reinterpret_cast<char*>(&result), sizeof (result)) ;
  result = BigEndianToSystem (result) ;
  return result ;
}

uint32 InputByteStream::getLittleEndianLong ()
{
  assert(bit_position < 0);
  uint32 result ;
  read (reinterpret_cast<char*>(&result), sizeof (result)) ;
  result = LittleEndianToSystem (result) ;
  return result ;
}


//
//  Description:
//
//  The function reads a block from the input stream.
//
//  Parameters:
//
//  buffer: The input buffer
//  count:  The number of bytes to read
//
//  Return Value:
//
//  The number of bytes read.
//
int  InputByteStream::read (char *buffer, int count) {

  assert(bit_position < 0);
  int result ;
  size_t remaining = buffer_limit - current_byte ;
  if (count <= (int32) remaining) {
    // We can respond to this request using the data already in the buffer.
    memcpy (buffer, current_byte, count) ;
    current_byte += count ;
    result = count ;
  }
  else {

    // Consume all of the characters already in the buffer.
    result = remaining ;
    memcpy (buffer, current_byte, remaining) ;
    buffer += remaining ;
    count -= remaining ;
    // Refill the buffer and continue
    while (count > 0 && moreData ()) {
      fillBuffer () ;
      remaining = buffer_limit - current_byte ;
      if (count <= (int32) remaining) {
        // We have read enough data to handle this request.
        memcpy (buffer, current_byte, count) ;
        current_byte += count ;
        result += count ;
        break ;
      }
      else {
        // The new buffer does not have enough data to
        // handle this request.
        result += remaining ;
        memcpy (buffer, current_byte, remaining) ;
        buffer += remaining ;
        count -= remaining ;
      }
    }
  }
  return result ;
}

} // namespace png
} // namespace lti

