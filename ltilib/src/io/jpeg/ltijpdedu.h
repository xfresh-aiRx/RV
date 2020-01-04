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
 * file .......: jpdedu.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpdedu.h,v 1.6 2003/04/15 07:55:33 alvarado Exp $
 */

#ifndef _LTI_JPDEDU_H_
#define _LTI_JPDEDU_H_

//
//  Decoder Data Unit Class Definition
//
//  Descrition:
//
//    The DataUnit class represents an 8x8 sample block for one
//    component of the JPEG image.
//
//

#include "ltijpeg.h"
#include "ltijpdequan.h"
#include "ltijpdecobk.h"

#include <iostream>
#include <cmath>


namespace lti {
  namespace jpeg {

class JpegDecoderDataUnit ;

std::ostream &operator<<(std::ostream &, JpegDecoderDataUnit &) ;

/**
 * class JpegDecoderDataUnit
 */
class JpegDecoderDataUnit {

public:
  /**
   * Declaration of a type for pointers to member functions for
   * implementing the IDCT. The input parameters are The IDCT
   * coefficients and the [de]quantization table.
   */
  typedef JpegDecoderDataUnit &(JpegDecoderDataUnit::*IDctFunction) (
                                const JpegDecoderCoefficientBlock,
                                const JpegDecoderQuantizationTable  &) ;

  /**
   * constructor
   */
  JpegDecoderDataUnit() {}

  /**
   * destructor
   */
  virtual ~JpegDecoderDataUnit () {}

  /**
   * Utility function to write the IDCT values to an output stream.
   */
  void Print (std::ostream &) const ;

  /**
   * General IDCT Function
   */
  JpegDecoderDataUnit& InverseDCT (
                        const JpegDecoderCoefficientBlock cb,
                        const JpegDecoderQuantizationTable  &qt) ;

  /**
   * These are the IDCT implementations.
   */
  JpegDecoderDataUnit &FloatInverseDCT (const JpegDecoderCoefficientBlock,
                             const JpegDecoderQuantizationTable  &) ;

  /**
   * These are the IDCT implementations.
   */
  JpegDecoderDataUnit &IntegerInverseDCT (const JpegDecoderCoefficientBlock,
                               const JpegDecoderQuantizationTable  &) ;

  /**
   * Operators to retrieve the individual IDCT values.
   */
  JPEGSAMPLE *operator [] (unsigned int ii) ;

  /**
   * Operators to retrieve the individual IDCT values.
   */
  JPEGSAMPLE const* operator [] (unsigned int ii) const ;

private:
  /**
   * Dummy Declarations For Required Members Functions
   */
  JpegDecoderDataUnit (const JpegDecoderDataUnit &) ;

  /**
   * Dummy Declarations For Required Members Functions
   */
  JpegDecoderDataUnit &operator=(const JpegDecoderDataUnit &) ;

  /**
   * The IDCT values.
   */
  lti::ubyte values [JpegSampleWidth][JpegSampleWidth] ;

  /**
   * This is a pointer to the member function that implements
   * the desired IDCT function.
   */
  static IDctFunction idct_function ;
} ;

inline JPEGSAMPLE *JpegDecoderDataUnit::operator [] (unsigned int ii) {
 return values [ii] ;
}

inline JPEGSAMPLE const* JpegDecoderDataUnit::operator[](unsigned int ii) const
{
  return values [ii] ;
}

inline std::ostream &operator<<(std::ostream &strm,
                                const JpegDecoderDataUnit &du) {
  du.Print (strm) ;
  return strm ;
}

inline JpegDecoderDataUnit &JpegDecoderDataUnit::InverseDCT (
                        const JpegDecoderCoefficientBlock cb,
                        const JpegDecoderQuantizationTable  &qt) {
  return (this->*idct_function) (cb, qt) ;
}

}
}

#endif

