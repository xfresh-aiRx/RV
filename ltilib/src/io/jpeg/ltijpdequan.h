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
 * file .......: jpdequan.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpdequan.h,v 1.5 2003/02/17 07:16:03 alvarado Exp $
 */

#ifndef _LTI_JPDEQUAN_H_
#define _LTI_JPDEQUAN_H_

//
//  Title:  JPEG Decoder Quantization Table Class Implementation
//
//  Description:
//
//    This class represents a quantization table used by the
//    JPEG decoder.
//


#include "ltijpeg.h"
#include "ltijpgexcep.h"

#include "ltiImage.h"

#include <iostream>


namespace lti {
  namespace jpeg {

class JpegDecoder ;
/**
 * jpeg decoder quantization table
 *
 *    This class represents a quantization table used by the
 *    JPEG decoder.
 */
class JpegDecoderQuantizationTable
{
public:
  JpegDecoderQuantizationTable() ;
  ~JpegDecoderQuantizationTable() {}

  /**
   * This function tells the caller if the quantization table has been
   * defined by the JPEG input stream.
   */
  bool Defined () const ;

  /**
   * Function to read the quantization table from the input stream.
   */
  void ReadTable (JpegDecoder &decoder, int precision) ;

  /**
   * This function builds the scaled quantization tables used in
   * fast IDCT implementations.
   */
  void BuildScaledTables () ;

  /**
   * This function prints the contents of the quantization table to
   * an output stream.
   */
  void Print (std::ostream &) const ;

private:
  /**
   * Dummy Declarations for Required Member Functions
   */
  JpegDecoderQuantizationTable (const JpegDecoderQuantizationTable&) ;
  JpegDecoderQuantizationTable &operator=(const JpegDecoderQuantizationTable&) ;

  /**
   * Quantization Values in Zig-Zag Order.
   */
  lti::uint16 data_values [JpegSampleSize] ;


  /**
   * Scaling factor used for the scaled integer values.
   */
  enum { QuantizationIntegerScale = 12, } ;

  /**
   * Scaled quantization values used for the fast IDCT implementations.
   */
  double float_scaling [JpegSampleWidth][JpegSampleWidth] ;
  long integer_scaling [JpegSampleWidth][JpegSampleWidth] ;

  /**
   * This flag gets set to true when the quantization is defined in the
   * JPEG input stream. It is used to ensure that an compressed scan does
   * not attempt to use an undefined quantization table.
   */
  bool table_defined ;

  friend class JpegDecoderDataUnit ;
} ;

inline bool JpegDecoderQuantizationTable::Defined () const
{
  return table_defined ;
}


inline std::ostream &operator<<(std::ostream &strm,
                                const JpegDecoderQuantizationTable &du)
{
  du.Print (strm) ;
  return strm ;
}

} // jpeg
} // lti

#endif

