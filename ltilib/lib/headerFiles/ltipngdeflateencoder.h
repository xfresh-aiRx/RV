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
 * file .......: ltipngdeflateencoder.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: A class for implementing Deflate compression.
 * revisions ..: $Id: ltipngdeflateencoder.h,v 1.5 2003/02/17 07:16:14 alvarado Exp $
 */


#ifndef _LTI_DEFLATEENCODER_H
#define _LTI_DEFLATEENCODER_H


#include <iostream>
#include "ltipngdeflatehuffmanencoder.h"
#include "ltipngdeflateoutputstream.h"
#include "ltiTypes.h"

namespace lti{
  namespace png {

/**
 * PNG Deflate Encoder
 */
class DeflateEncoder {
public:

  DeflateEncoder () ;

  virtual ~DeflateEncoder () ;

  void startCompressedStream (DeflateOutputStream &outputstream) ;

  void compressData (DeflateOutputStream & outputstream,
                     const char data[],
                     unsigned int size) ;

  void endCompressedStream (DeflateOutputStream &outputstream) ;

  enum CompressionLevel  {
    FASTESTCOMPRESSION = 0,
    FASTCOMPRESSION = 1,
    DEFAULTCOMPRESSION = 2,
    MAXIMUMCOMPRESSION = 3,
  } ;

  void setCompressionLevel (CompressionLevel value) ;

  CompressionLevel getCompressionLevel () const ;

  unsigned long getBlockSize () const ;

  void setBlockSize (unsigned long) ;

  class DeflateError ;

private:

  //DeflateEncoder (const DeflateEncoder &) ;

  //DeflateEncoder &operator=(const DeflateEncoder &) ;

  /**
   * hash entry for png deflate encoder
   */
  struct HashEntry {
    uint16 index ;      // Index into the LZ Window
    HashEntry *next ;   // Next collision entry
    HashEntry *previous ;
  } ;

  void prepareNewBlock () ;

  void longestMatch (unsigned int count,
                     unsigned int &length,
                     unsigned int &offset) ;

  unsigned int hashValue (unsigned int) ;

  void moveHashEntry (unsigned int entry, unsigned int hashvalue) ;

  typedef void (DeflateEncoder::*LENGTHFUNCTION) (DeflateOutputStream &outputstream,
                                              unsigned int index,
                                              unsigned int code,
                                              unsigned int length,
                                              unsigned int extra) ;

  void gatherLengthCounts (DeflateOutputStream &outputstream,
                           unsigned int,
                           unsigned int code,
                           unsigned int,
                           unsigned int) ;

  void outputLengthCounts (DeflateOutputStream &outputstream,
                           unsigned int index,
                           unsigned int code,
                           unsigned int extra,
                           unsigned int value) ;

  void findLengthCodes (DeflateOutputStream &outputstream,
                        LENGTHFUNCTION,
                        DeflateHuffmanEncoder &encoder,
                        unsigned int count) ;

  bool compressDataBlock () ;

  void compressLookaheadData (DeflateOutputStream &outputstream,
                              unsigned int count,
                              bool endflag) ;

  void outputBlock (const void *data, unsigned int length) ;

  void freeBuffers () ;

  void fillBuffer (unsigned int count) ;

  void initializeHashTable () ;

  void outputDeflateHeader (DeflateOutputStream &outputstream, bool lastblock) ;

  void outputZLibHeader (DeflateOutputStream &outputstream) ;

  void outputBlockData (DeflateOutputStream &outputstream) ;

  // Array of have table entries, one entry for each position in the LZ Window.
  HashEntry *hash_values ;
  // Hash Table
  HashEntry *hash_table ;


  DeflateHuffmanEncoder distance_table ;

  DeflateHuffmanEncoder length_table ;

  DeflateHuffmanEncoder length_length_table ;

  // Compression vs. Time selected by the user.
  CompressionLevel compression_level ;
  // Number of hash entries to search for a match.
  // This is the internal translation of compression_level.
  unsigned int search_limit ;

  uint32 adler_value ;
  ubyte *lz_window ;
  ubyte *lookahead_buffer ;
  // Index of next position to add to the lookahead_buffer.
  unsigned int lookahead_input_position ;
  // Index of next position to remove from the lookahead_buffer.
  unsigned int lookahead_output_position ;
  // Current position in the lz_window buffer.
  unsigned int lz_position ;

  // LZ Compressed Block
  uint16 *block_buffer ;
  // Size of Block Buffer
  unsigned int block_buffer_size ;
  // Number of entries added to the block buffer so far.
  unsigned int block_buffer_count ;

  // false => Compression not underway
  // true => Compression underway
  bool compression_in_progress ;
} ;

/**
 * PNG deflate encoder
 */
class DeflateEncoder::DeflateError : public exception {
private:

  std::string message ;

public:

  DeflateError (const std::string &msg) : message (msg) {} ;

  DeflateError (const DeflateError &source) : message (source.message) {}

  ~DeflateError() throw () {};

  DeflateError &operator=(const DeflateError &source)  {
      if (&source != this) {
        message = source.message ;
      }
      return *this ;
    }

  virtual const char *what () const throw () { return message.c_str () ; }
} ;
  } // ns png
} // namespace lti


#endif

