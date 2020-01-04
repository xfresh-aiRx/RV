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
 * file .......: ltipngoutputstream.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: This an abstract class the represents an PNG output stream.
 * revisions ..: $Id: ltipngoutputstream.cpp,v 1.4 2003/10/30 10:02:50 alvarado Exp $
 */


#include "ltiAssert.h"
#include "ltipngcrc32.h"
#include "ltipngendian.h"
#include "ltipngoutputstream.h"
#include "ltiAssert.h"
#include "ltipngsystemspecific.h"

namespace lti{
  namespace png {
//
//  Description:
//
//  Default Class Constructor
//
PngOutputStream::PngOutputStream () {
  memset (chunk_type, 0, sizeof (chunk_type)) ;
  return ;
}


//
//  Description:
//
//  This function starts a new chunk.
//
//  Parameters:
//
//  chunktype : The 4-byte chunk type
//
void PngOutputStream::startChunk (char chunktype [4]) {

  assert(memcmp (chunk_type, "\0\0\0\0", sizeof (chunk_type)) == 0) ;
  memcpy (chunk_type, chunktype, sizeof (chunktype)) ;
  return ;
}


//
//  Description:
//
//  This function ends the current chunk.
//
void PngOutputStream::endChunk () {

  assert(memcmp (chunk_type, "\0\0\0\0", sizeof (chunk_type)) != 0) ;
  flushBuffer () ;
  memset (chunk_type, 0, sizeof (chunk_type)) ;
  return ;
}


///////////////////////////////////////////////////////////////////////////////////////////
//                    pngoutputfilestream
///////////////////////////////////////////////////////////////////////////////////////////

//
//  Description:
//
//  Default class constructor
//
PngOutputFileStream::PngOutputFileStream ()
: buffer_size (1 << 16),
  output_buffer (0)
{
  return ;
}

//
//  Description:
//
//  Class Destructor
//
PngOutputFileStream::~PngOutputFileStream () {
  close () ;
  return ;
}

//
//  Description:
//
//  This function writes the contents of the buffer to
//  the output stream
//
void PngOutputFileStream::flushBuffer () {

  assert(memcmp (chunk_type, "\0\0\0\0", sizeof (chunk_type)) != 0) ;
  Crc32 crc ;
  crc.update (chunk_type, sizeof (chunk_type)) ;
  unsigned int length = current_byte - output_buffer ;
  crc.update (reinterpret_cast<char*>(output_buffer), length) ;
  uint32 chunklength = SystemToBigEndian ((uint32)length) ;

  output_stream.write (reinterpret_cast<char*>(&chunklength), sizeof (chunklength)) ;
  output_stream.write (reinterpret_cast<char*>(chunk_type), sizeof (chunk_type)) ;
  output_stream.write (reinterpret_cast<char*>(output_buffer), length) ;

  uint32 crcvalue = crc.value () ;
  crcvalue = SystemToBigEndian (crcvalue) ;

  output_stream.write (reinterpret_cast<char*>(&crcvalue), sizeof (crcvalue)) ;

  current_byte = output_buffer ;
  return ;
}


//
//  Description:
//
//  This function opens an output file.
//
//  Parameters:
//
//  filename : The output file name
//
void PngOutputFileStream::open (const std::string &filename) {

  if (output_buffer == 0){
    output_buffer = new ubyte [buffer_size] ;
  }


  output_stream.open (filename.c_str (), std::ios::out|std::ios::binary) ;

  if (output_stream) {
    current_byte = output_buffer ;
    buffer_limit = &output_buffer [buffer_size] ;
  }
  return ;

}


//
//  Description:
//
//  This function closes the output file.
//
void PngOutputFileStream::close ()  {

  delete [] output_buffer ; output_buffer = 0 ;
  current_byte = 0 ;
  buffer_limit = 0 ;
  output_stream.close () ;
  return ;
}

//
//  Description:
//
//  This function returns the state of the output file stream.
//
//  Return Value:
//
//  The state of the output_stream member
//
PngOutputFileStream::operator bool () {

#if defined (_LTI_MSC_VER)
  bool result = static_cast<void*> (output_stream) != 0 ? true : false ;
#else
  bool result = static_cast<bool> (output_stream) ;
#endif

  return result ;
}

//
//  Description:
//
//  This function writes a string of raw bytes to the output stream,
//  bypassing chunks.
//
//  Parameters:
//
//  buffer : The output buffer
//  size  : The number of bytes to write
//
void PngOutputFileStream::writeRaw (const char* buffer, unsigned int size) {

  assert(memcmp (chunk_type, "\0\0\0\0", sizeof (chunk_type)) == 0) ;
  output_stream.write (buffer, size) ;
  return ;
}
  } // namespace png
} // namespace lti

