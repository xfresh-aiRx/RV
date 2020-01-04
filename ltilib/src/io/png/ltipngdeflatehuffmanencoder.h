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
 * file .......: ltipngdeflatehuffmanencoder.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: huffman encoding class designed for Deflate compression.
 * revisions ..: $Id: ltipngdeflatehuffmanencoder.h,v 1.4 2003/02/17 07:16:14 alvarado Exp $
 */


#ifndef _LTI_DEFLATEHUFFENCODER_H
#define _LTI_DEFLATEHUFFENCODER_H


#include "ltiVector.h"

namespace lti{
  namespace png {

/**
 * PNG deflate huffman encoder
 */
class DeflateHuffmanEncoder
{
public:
  DeflateHuffmanEncoder (unsigned int valuecount=256);
  virtual ~DeflateHuffmanEncoder () {}

  // ENCODING FUNCTIONS

  // This function resets the table so that the object can be used
  // over again.
  void reset ();

  // This function increases the frequency for a huffman value.
  void incrementFrequency (unsigned int value);

  // This function creates the Huffman codes from the code frequencies.
  void buildTable (unsigned int maxlength);

  // This function returns the Huffman code and code length to encode the
  // specified value.
  void encode (unsigned int value, unsigned int &code, unsigned int &size) const;

private:
  DeflateHuffmanEncoder (const DeflateHuffmanEncoder &);
  DeflateHuffmanEncoder &operator=(const DeflateHuffmanEncoder &);

  // This function builds the structures needed for Huffman
  // decoding after the table data has been read.
  void makeTable ();

  // frequencies [n] is the number of times the value "n" needs to
  // be encoded.

  vector<unsigned int> frequencies;

  // Values used to encode values.
  //   ehufsi [n] is the number of bits required to code "n"
  //   ehufco [n] is the Huffman code for "n"

  vector<unsigned int> ehufsi;

  vector<unsigned int> ehufco;

  unsigned int value_count;
};
  } // ns png
} // namespace lti

#endif

