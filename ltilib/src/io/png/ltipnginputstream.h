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
 * file .......: ltipnginputstream.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: These classes define a PNG input stream attached to a file.
 * revisions ..: $Id: ltipnginputstream.h,v 1.7 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_PNGINPUTSTREAM_H
#define _LTI_PNGINPUTSTREAM_H


#include <fstream>
#include "ltipngendian.h"
#include "ltipnginflateinputstream.h"

namespace lti {
  namespace png {

/**
 * PNG input stream
 */
class PngInputStream : virtual public InflateInputStream {

public:

  bool getNextChunk () ;
  const ubyte *getChunkType () const ;
  uint32 getChunkDataLength () const ;
  const ubyte *getChunkData () const ;
  virtual void readRaw (unsigned int count, ubyte *&buffer) = 0 ;

protected:

  virtual void fillBuffer () ;

  virtual uint32 readChunkLength () = 0 ;
  virtual void readChunkData (uint32 length, ubyte *&buffer) = 0 ;
  virtual uint32 readCrc () = 0 ;

  ubyte chunk_type [4] ;
  ubyte *chunk_data ;
  uint32 chunk_length ;
} ;

inline const ubyte *PngInputStream::getChunkType () const {
  return chunk_type ;
}

inline uint32 PngInputStream::getChunkDataLength () const {
  return chunk_length ;
}

inline const ubyte *PngInputStream::getChunkData () const {


  return chunk_data ;

}

///////////////////////////////////////////////////////////////////////////
//               pnginputfilestream
///////////////////////////////////////////////////////////////////////////


/**
 * Input File Stream for PNG files.
 */
class PngInputFileStream : virtual public PngInputStream {

public:

  PngInputFileStream (unsigned int buffersize = 4096) ;
  virtual ~PngInputFileStream () ;

  virtual uint32 readChunkLength () ;
  virtual void readChunkData (uint32 length, ubyte *&buffer) ;
  virtual uint32 readCrc () ;
  virtual bool endReached () ;
  virtual void readRaw (unsigned int count, ubyte *&buffer) ;

  operator bool () ;
  void open (const std::string &filename) ;
  void open (std::ifstream &file) ;
  void close () ;

private:

  PngInputFileStream (const PngInputFileStream &) ;
  PngInputFileStream &operator=(const PngInputFileStream &) ;

  unsigned int buffer_size ;
  ubyte *input_buffer ;
  std::ifstream* input_stream;
  bool ownStream;
  std::ios::fmtflags oldFlags;
};

  } // namespace png
} // namespace lti

#endif

