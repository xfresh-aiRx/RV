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
 * file .......: ltipnginflatedecoder.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: class declaration for Deflate/Inflate compression decoder
 * revisions ..: $Id: ltipnginflatedecoder.h,v 1.5 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_INFLATEDECODER_H
#define _LTI_INFLATEDECODER_H

#include <iostream>
#include <stdexcept>
#include "ltiTypes.h"

namespace lti{
  namespace png {

class InflateInputStream ;
class InflateHuffmanDecoder ;

/**
 * PNG inflate decoder
 */
class InflateDecoder {
public:
  InflateDecoder () ;
  virtual ~InflateDecoder () ;
  void setVerbose (bool) ;
  bool getVerbose () const ;
  void openStream (InflateInputStream &) ;
  int decode (InflateInputStream &strm, unsigned int count, ubyte output []) ;

  class StreamError ;

private:
  InflateDecoder (const InflateDecoder &) ;
  InflateDecoder &operator=(InflateDecoder &) ;

  // Deflate Functions
  void startNewDataSet (InflateInputStream &) ;
  void checkAdler (InflateInputStream &) ;
  void readLengths (InflateInputStream &strm,
                    InflateHuffmanDecoder &,
                    unsigned int [],
                    unsigned int) ;
  void freeData () ;
  // Deflate State Variables
  uint32 stream_adler ;       // Adler32 Checksum value
  ubyte *lz_window ;         // LZ77 Sliding Window
  unsigned int window_position ;  // Output position in the LZ77 window
  unsigned int copy_position ;    // Input posiiton in the LZ77 window
  unsigned int copy_count ;   // Number bytes remaining from a copy operation
  InflateHuffmanDecoder *literal_table ;
  InflateHuffmanDecoder *distance_table ;
  bool final_data_set ;  // Set when Final data set is read
  bool no_more_data ;    // Set when no more data is available.
  uint16 literal_count ; // Number remaining literal bytes


} ;

/**
 * PNG Inflate Decoder
 */
class InflateDecoder::StreamError : public exception {

private:
  std::string message ;
public:
  StreamError (const std::string &msg) : message (msg) {}
  StreamError (const StreamError &source) : message (source.message) {}
  StreamError &operator=(const StreamError &source) {
    if (this != &source) {
      message = source.message ;
    }
    return *this ;
  }

  ~StreamError() throw () {};

  virtual const char *what () const throw () { return message.c_str () ; }
} ;

  } // namespace png
} // namespace lti

#endif

