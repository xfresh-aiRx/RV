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
 * file .......: ltipnginputstream.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: These classes implement a PNG input stream attached to a file
 *               from a stream of bytes.
 * revisions ..: $Id: ltipnginputstream.cpp,v 1.8 2003/10/30 10:02:50 alvarado Exp $
 */

#include "ltipnginputstream.h"
#include "ltipngcrc32.h"
#include "ltipngexcept.h"
#include "ltipngsystemspecific.h"

using std::ios;

namespace lti {
  namespace png {

bool PngInputStream::getNextChunk () {

  chunk_length = readChunkLength () ;
  if (chunk_length == 0)
    return false ;

  readChunkData (chunk_length + sizeof (chunk_length), chunk_data) ;
  uint32 filecrc = readCrc () ;
  Crc32 crc ;

  crc.update (reinterpret_cast<const char*>(chunk_data),
             static_cast<unsigned int>(chunk_length + sizeof (chunk_length))) ;
  uint32 crcvalue = crc.value () ;

  if (filecrc != crcvalue) {
    throw StreamError ("Invalid Chunk CRC") ;
  }

  chunk_type [0] = *chunk_data ; ++ chunk_data ;
  chunk_type [1] = *chunk_data ; ++ chunk_data ;
  chunk_type [2] = *chunk_data ; ++ chunk_data ;
  chunk_type [3] = *chunk_data ; ++ chunk_data ;

  current_byte = chunk_data ;
  buffer_limit = &chunk_data [chunk_length] ;
  return true ;
}

void PngInputStream::fillBuffer () {

  if (! getNextChunk ()) {
    throw StreamError ("Premature End of File") ;
  }

  if (memcmp (chunk_type, "IDAT", sizeof (chunk_type)) != 0) {
    throw StreamError ("Missing IDAT Chunk") ;
  }

  current_byte = chunk_data ;
  buffer_limit = &chunk_data [chunk_length] ;
  return ;
}

////////////////////////////////////////////////////////////////////////////////////
//                       pnginputfilestream
////////////////////////////////////////////////////////////////////////////////////

//
//  Description:
//
//  Class [Default] Constructor
//
//  Parameters:
//
//  buffersize : Size of the size of the input buffer to create.
//
PngInputFileStream::PngInputFileStream (unsigned int buffersize)
                   : buffer_size (buffersize),
                     input_buffer (new ubyte [buffersize]) {
  return ;
}

//
//  Description:
//
//  Class Destructor
//

PngInputFileStream::~PngInputFileStream () {
  delete [] input_buffer ; input_buffer = 0 ;
  return ;
}

//
//  Description:
//
//  This function reads and returns the chunk length
//  from the input stream.
//
//  Return Value:
//
//  The length of the next chunk in system format.
//
uint32 PngInputFileStream::readChunkLength () {

  uint32 result ;
  input_stream->read (reinterpret_cast<char*>(&result),
                     sizeof (result)) ;
  if (input_stream->gcount () != sizeof (result)) {
    throw StreamError ("Premature End of File") ;
  }

  result = BigEndianToSystem (result) ;
  return result ;
}

//
//  Description:
//
//  This function reads the chunk data from the input file.
//
//  Parameters:
//
//  length : Number of bytes to read
//  buffer (out) : Returns a pointer to the buffer containing the data.
//
void PngInputFileStream::readChunkData (uint32 length, ubyte *&buffer) {

  if (length > buffer_size)
  {
    delete [] input_buffer ;
    input_buffer = new ubyte [length] ;
    buffer_size = length ;
  }

  input_stream->read (reinterpret_cast<char*>(input_buffer), length) ;
  unsigned int count = input_stream->gcount () ;
  if (count != length) {
    throw StreamError ("Premature End of File") ;
  }

  buffer = input_buffer ;
  return ;
}

//
//  Description:
//
//  This function reads the chunk CRC from the input file.
//
//  Return Value:
//
//  The chunk CRC
//
uint32 PngInputFileStream::readCrc () {
  uint32 result ;
  input_stream->read (reinterpret_cast<char*>(&result),
                     sizeof (result)) ;
  if (input_stream->gcount () != sizeof (result)) {
    throw StreamError ("Premature End of File") ;
  }
  result = BigEndianToSystem (result) ;
  return result ;
}

//
//  Description:
//
//  This function tells the caller if the end of the
//  file has been reached.
//
//  Return Value:
//
//  true => The end of file has been reached.
//  false => There is more data in the file.
//
bool PngInputFileStream::endReached () {
  bool result = input_stream->eof () ;
  return result ;
}

//
//  Description:
//
//  This function returns the state of the input stream.
//
//  Return Value:
//
//  The state ofthe output stream.
//
PngInputFileStream::operator bool () {

#if defined (_LTI_MSC_VER)
  bool result = static_cast<void*> (input_stream) != 0 ? true : false ;
#else
  bool result = static_cast<bool> (input_stream) ;
#endif
  return result ;
}
//
//  Description:
//
//  This function opens the input file.
//
//  Parameters:
//
//  filename : The name of the file to open
//
void PngInputFileStream::open (const std::string &filename) {

  // pngInputMode is platform specific and defined in ltipngsystemspecific.h
  input_stream=new std::ifstream(filename.c_str (),
                                 std::ios::in|std::ios::binary) ;
  ownStream=true;
  return ;
}

void PngInputFileStream::open (std::ifstream& file) {

  // pngInputMode is platform specific and defined in ltipngsystemspecific.h
  input_stream=&file;//new std::ifstream(file.filedesc());
  oldFlags=input_stream->flags();
  ownStream=false;
  // input_stream->flags(std::ios::in|std::ios::binary);

  return ;
}
//
//  Description:
//
//    This function closes the output file.
//
void PngInputFileStream::close () {
  if (ownStream) {
    input_stream->close () ;
    delete input_stream;
  } else {
    input_stream->flags(oldFlags);
  }
  return ;
}
//
//  Description:
//
//  This function reads raw bytes from the input file.
//
//
//  Parameters:
//
//  count : The number of bytes to read
//  buffer (out) : A pointer to the data read.
//
void PngInputFileStream::readRaw (unsigned int count, ubyte *&buffer) {

  if (count > buffer_size)   {
    delete [] input_buffer ;
    input_buffer = new ubyte [count] ;
    buffer_size = count ;
  }

  input_stream->read (reinterpret_cast<char*>(input_buffer), count) ;

  if (input_stream->gcount () != static_cast<int>(count)) {
    throw PngError ("Cannot read ifstream!") ;
  }

  buffer = input_buffer ;
  return ;
}

  } // namespace png
} // namespace lti

