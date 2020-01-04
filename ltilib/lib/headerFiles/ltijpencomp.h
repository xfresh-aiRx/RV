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
 * file .......: jpgencomp.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpencomp.h,v 1.7 2003/02/17 07:16:03 alvarado Exp $
 */

#ifndef _LTI_JPENCOMP_H_
#define _LTI_JPENCOMP_H_

//
//  Title:  JpegEncoderComponent class definition
//
//  Description:
//
//    This class represents a single component while writing a JPEG image.
//

#include "ltijpeg.h"
#include "ltijfif.h"
#include "ltijpencobk.h"
#include "ltijpendu.h"
#include "ltijpenhuff.h"

#include "ltiImage.h"


namespace lti {
  namespace jpeg {

class JpegEncoder ;
class JpegEncoderComponent ;
class JpegEncoderHuffmanTable ;
class JpegEncoderQuantizationTable ;
class JpegEncoderCoefficientBlock ;

/**
 * JpegEncoderComponent class definition
 *
 * This class represents a single component while writing a JPEG image.
 */
class JpegEncoderComponent
{
public:
  JpegEncoderComponent () ;
  virtual ~JpegEncoderComponent () ;

  void PrintAcData (int, int, int) ;
  void PrintDcData (int, int) ;
  void GatherAcData (int, int, int) ;
  void GatherDcData (int, int) ;

  typedef void (JpegEncoderComponent::*DCOUTPUTFUNCTION) (int, int) ;
  typedef void (JpegEncoderComponent::*ACOUTPUTFUNCTION) (int, int, int) ;
  typedef void (JpegEncoderComponent::*COMPONENTPASSFUNCTION) (
                            int row, int col,
                            DCOUTPUTFUNCTION, ACOUTPUTFUNCTION,
                            int sss, int sse,
                            int ssa) ;


  void EncodeSequential (int row, int col,
                         DCOUTPUTFUNCTION dcfunction,
                         ACOUTPUTFUNCTION acfunction,
                         int, int, int) ;
  void ProgressiveDcFirst (int row, int col,
                           DCOUTPUTFUNCTION dcfunction, ACOUTPUTFUNCTION,
                           int, int,
                           int ssa) ;
  void ProgressiveDcRefine (int row, int col,
                            DCOUTPUTFUNCTION dcfunction, ACOUTPUTFUNCTION,
                            int, int,
                            int) ;
  void ProgressiveAcFirst (int row, int col,
                           ACOUTPUTFUNCTION acfunction,
                           int sss, int sse,
                           int ssa) ;
  void ProgressiveAcRefine (int row, int col,
                            ACOUTPUTFUNCTION acfunction,
                            int sss, int sse,
                            int ssa) ;

  void ResetEobRun () ;
  void PrintEobRun (ACOUTPUTFUNCTION acfunction) ;
  void PrintRefineEobRun (ACOUTPUTFUNCTION acfunction,
                          int sss, int sse,
                          int ssa) ;

  void SampleYComponent (JpegEncoder &encoder, const lti::image &, int maxhf, int maxvf) ;
  void SampleCbComponent (JpegEncoder &encoder, const lti::image &, int maxhf, int maxvf) ;
  void SampleCrComponent (JpegEncoder &encoder, const lti::image &, int maxhf, int maxvf) ;

  int GetHorizontalFrequency () const ;
  void SetHorizontalFrequency (int) ;
  int GetVerticalFrequency () const ;
  void SetVerticalFrequency (int) ;

  int DataUnitRows () const ;
  int DataUnitCols () const ;
  void ResetDcDifference () ;

  void SetHuffmanTables (JpegEncoderHuffmanTable &dc,
                         JpegEncoderHuffmanTable &ac) ;
  void SetQuantizationTable (JpegEncoderQuantizationTable &table) ;

  void FreeDynamicStorage () ;

private:
  JpegEncoderComponent (const JpegEncoderComponent &) ;
  JpegEncoderComponent &operator=(const JpegEncoderComponent &) ;

  void CalculateDuDimensions (const lti::image &image, int maxhf, int maxvf) ;

  typedef JPEGSAMPLE (*RGBTOYCBCRFUNCTION)(
                         JPEGSAMPLE red,
                         JPEGSAMPLE green,
                         JPEGSAMPLE blue) ;
//!  void Sample1to1Component (const BitmapImage &, RGBTOYCBCRFUNCTION) ;
  void Sample1to1Component (const lti::image &, RGBTOYCBCRFUNCTION) ;
//!  void SampleNtoNComponent (const BitmapImage &image, RGBTOYCBCRFUNCTION function) ;
  void SampleNtoNComponent (const lti::image &image,
                            RGBTOYCBCRFUNCTION function) ;

  JpegEncoder *jpeg_encoder ;

  //@{
  /**
   * DCT Coefficients and dimensions
   */
  int du_rows ;
  int du_cols ;
  JpegEncoderCoefficientBlock *dct_coefficients ;
  //@}

  //@{
  /**
   * EOB-run in context
   */
  int eob_run ;
  int eob_start_du_row ;
  int eob_start_du_col ;
  int eob_start_position ;
  //@}

  //@{
  /**
   * Sampling Frequencies and p
   */
  int v_frequency ;
  int h_frequency ;
  int v_period ;
  int h_period ;
  //@}

  //@{
  /**
   * Huffman and Quantization tables
   */
  JpegEncoderHuffmanTable *ac_table ;
  JpegEncoderHuffmanTable *dc_table ;
  JpegEncoderQuantizationTable *quantization_table ;
  //@}

  /**
   * Last DC value used to calculate DC differences
   */
  int last_dc_value ;

} ;

inline int JpegEncoderComponent::GetHorizontalFrequency () const
{
  return h_frequency ;
}

inline int JpegEncoderComponent::GetVerticalFrequency () const
{
  return v_frequency ;
}

inline int JpegEncoderComponent::DataUnitRows () const
{
  return du_rows ;
}

inline int JpegEncoderComponent::DataUnitCols () const
{
  return du_cols ;
}

inline void JpegEncoderComponent::ResetDcDifference ()
{
  last_dc_value = 0 ;
  return ;
}

inline void JpegEncoderComponent::SetHuffmanTables (JpegEncoderHuffmanTable &dc,
                                                    JpegEncoderHuffmanTable &ac)
{
  dc_table = &dc ;
  ac_table = &ac ;
  return ;
}


inline void JpegEncoderComponent::SetQuantizationTable (JpegEncoderQuantizationTable &table)
{
  quantization_table = &table ;
  return ;
}

}
}
#endif

