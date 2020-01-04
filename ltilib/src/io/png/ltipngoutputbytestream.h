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
 * file .......: ltipngoutputbytestream.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: This is an abstract class the represents an output stream of bytes.
 * revisions ..: $Id: ltipngoutputbytestream.h,v 1.5 2005/06/22 15:42:10 doerfler Exp $
 */


#ifndef _LTI_OUTPUTBYTESTREAM_H
#define _LTI_OUTPUTBYTESTREAM_H


#include <stdexcept>
#include "ltiTypes.h"

namespace lti {
  namespace png {
/**
 * png output byte stream
 */
class OutputByteStream {

public:

  OutputByteStream () ;
  virtual ~OutputByteStream () {} ;

  void write (const char *, int) ;
  void writeByte (ubyte) ;
  void writeBigEndianWord (uint16) ;
  void writeBigEndianLong (uint32) ;

  void enterBitMode (int initialposition) ;
  void nextByte () ;

  unsigned int remainingBufferSpace () const ;
  class StreamError ;

protected:

  virtual void flushBuffer () = 0 ;
  ubyte *current_byte ; // Current Byte Position
  ubyte *buffer_limit ; // Limit of the output byffer
  int bit_position ;

} ;

inline void OutputByteStream::nextByte () {
  ++ current_byte ;
  if (current_byte >= buffer_limit)
    flushBuffer () ;
  *current_byte = 0 ;
  return ;
}

/**
 * PNG output byte stream
 */
class OutputByteStream::StreamError : public std::exception {

private:

  std::string message ;

public:

  StreamError (const std::string &msg) : message (msg) {}
  StreamError (const StreamError &source) : message (source.message) {}
  virtual ~StreamError () throw () {}
  StreamError &operator=(const StreamError &source)  {
    if (&source != this) {
      message = source.message ;
    }
    return *this ;
  }

  const char *what () const throw () { return message.c_str () ; }
} ;

  } // namespace png
} // namespace lti

#endif

