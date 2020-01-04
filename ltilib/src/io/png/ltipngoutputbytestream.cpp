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
 * file .......: ltipngoutputbytestream.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: This is an abstract class the represents an output stream of bytes.
 * revisions ..: $Id: ltipngoutputbytestream.cpp,v 1.3 2003/02/17 07:16:14 alvarado Exp $
 */


#include "ltipngoutputbytestream.h"
#include "ltiAssert.h"
#include "ltiTypes.h"
#include "ltipngendian.h"

namespace lti {
  namespace png {

//
//  Description:
//
//    Default Constructor
//
OutputByteStream::OutputByteStream () : bit_position (-1) {
  return ;
}

//
//  Description:
//
//  Function to write a block of bytes.
//
//  Parameters:
//
//  buffer : The output buffer
//  count : The number of bytes to write
//
void OutputByteStream::write (const char *buffer, int count) {

  assert(bit_position == -1) ;

  while (count > 0) {
    if (count <= buffer_limit - current_byte) {
      memcpy (current_byte, buffer, count) ;
      current_byte += count ;
      count = 0 ;
    }
    else {
      int outputcount = buffer_limit - current_byte ;
      memcpy (current_byte, buffer, outputcount) ;
      buffer_limit += outputcount ;
      buffer += outputcount ;
      count -= outputcount ;
      flushBuffer () ;
    }
  }
  return ;
}

//
//  Description:
//
//  This function writes a single byte to the output stream.
//
//  Parameters:
//
//  value : The byte to write
//
void OutputByteStream::writeByte (ubyte value) {

  assert(bit_position == -1) ;
  if (current_byte >= buffer_limit) {
    flushBuffer () ;
  }
  *current_byte = value ;
  ++ current_byte ;
  return ;
}
//
//  Description:
//
//  This function writes a 2-byte integer to the output stream
//  in big endian format.
//
//  Parameters:
//
//  value : The value to output
//
void OutputByteStream::writeBigEndianWord (uint16 value) {
  assert(bit_position == -1) ;
  if (current_byte + sizeof (value) >= buffer_limit) {
    flushBuffer () ;
  }

  *reinterpret_cast<uint16*>(current_byte) = SystemToBigEndian (value) ;
  current_byte += sizeof (value) ;
  if (current_byte >= buffer_limit) {
    flushBuffer () ;
  }
  return ;
}

//
//  Description:
//
//  This function writes a 4-byte integer to the output stream
//  in big endian format.
//
//  Parameters:
//
//  value : THe value to output
//
void OutputByteStream::writeBigEndianLong (uint32 value) {

  assert(bit_position == -1) ;
  if (current_byte + sizeof (value) >= buffer_limit) {
    flushBuffer () ;
  }

  *reinterpret_cast<uint32*>(current_byte) = SystemToBigEndian (value) ;
  current_byte += sizeof (value) ;
  if (current_byte >= buffer_limit) {
    flushBuffer () ;
  }
  return ;
}


//
//  Description:
//
//  This function puts the output stream in bit mode.
//
//  Parameters:
//
//  initialposition : The first bit position (usually 8 or 0)
//
void OutputByteStream::enterBitMode (int initialposition) {

  assert(bit_position == -1) ;
  if (current_byte >= buffer_limit) {
    flushBuffer () ;
  }

  bit_position = initialposition ;
  *current_byte = 0 ;
  return ;
}
//
//  Description:
//
//  This function returns the number of bytes remaining in the
//  output buffer
//
//  Return Value:
//
//  The number of bytes remaining in the output buffer.
//
unsigned int OutputByteStream::remainingBufferSpace () const {

  unsigned int result = buffer_limit - current_byte ;
  return result ;
}

  } // namespace png
} // namespace lti

