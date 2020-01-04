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
 * file .......: jpdecomp.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * creation ...: 25.10.99
 * revisions ..: $Id: ltijpdecomp.h,v 1.5 2003/02/17 07:16:03 alvarado Exp $
 */
#ifndef _LTI_JPDECOMP_H_
#define _LTI_JPDECOMP_H_


//
//  Title:  JPEG Decoder Component Class Definition
//
//  Description:
//
//    This class represents a component within the JPEG decoder.
//


#include "ltijpgexcep.h"
#include "ltijpdequan.h"
#include "ltijpegdeco.h"
#include "ltijpeg.h"
#include "ltijfif.h"
#include "ltijpdedu.h"
#include "ltijpdecobk.h"
#include "ltijpdehuff.h"

#include "ltiImage.h"


namespace lti {
  namespace jpeg {

/**
 * JPEG decoder component
 *
 * This class represents a component within the JPEG decoder.
 */
class JpegDecoderComponent
{
public:
  JpegDecoderComponent () ;
  ~JpegDecoderComponent () ;

  // We have made the color conversions static because RGB
  // conversion requires the data from three components.
  // Grayscale conversion is static strictly for consistency
  // with RGB.
//!  static void RGBConvert (JpegDecoderComponent &c1, JpegDecoderComponent &c2, JpegDecoderComponent &c3, BitmapImage &image) ;
  static void RGBConvert (JpegDecoderComponent &c1,
                          JpegDecoderComponent &c2,
                          JpegDecoderComponent &c3,
                          lti::image &image) ;
//!  static void GrayscaleConvert (JpegDecoderComponent &cc, BitmapImage &image) ;
  static void GrayscaleConvert (JpegDecoderComponent &cc, lti::image &image) ;

  int HorizontalFrequency () const ;
  void HorizontalFrequency (int) ;

  int VerticalFrequency () const ;
  void VerticalFrequency (int) ;

  void SetQuantizationTable (JpegDecoderQuantizationTable &table) ;
  void AllocateComponentBuffers (const JpegDecoder &decoder) ;
  void FreeComponentBuffers () ;
  void SetHuffmanTables (JpegHuffmanDecoder &dc, JpegHuffmanDecoder &ac) ;
  void Upsample () ;

  void CheckAcTable () ;
  void CheckDcTable () ;
  void CheckQuantizationTable () ;

  void DecodeSequential (JpegDecoder &decoder,
                         int mcurow,
                         int mcucol) ;

  long NoninterleavedRows () const ;
  long NoninterleavedCols () const ;
  void ResetDcDifference () ;

  void DecodeDcFirst (JpegDecoder &decoder,
                                    int row,
                                    int col,
                                    int ssa) ;
  void DecodeDcRefine (JpegDecoder &decoder,
                                     int row,
                                     int col,
                                     int ssa) ;

  void DecodeAcFirst (JpegDecoder &decoder,
                                    int row,
                                    int col,
                                    int sss,
                                    int sse,
                                    int ssa) ;

  void DecodeAcRefine (JpegDecoder &decoder,
                                     int row,
                                     int col,
                                     int sss,
                                     int sse,
                                     int ssa) ;

  void ProgressiveInverseDct () ;

private:
  // Dummy Declarations for Required Member Functions
  JpegDecoderComponent (const JpegDecoderComponent &) ;
  JpegDecoderComponent operator=(const JpegDecoderComponent &) ;

  // Jfif/Frame component ID
  int component_id ;

  // Sampling Frequencies
  int horizontal_frequency ;
  int vertical_frequency ;

  // These values are the number of samples to take for each data
  // point. They come from the sampling frequencies and the maximum
  // sampling frequencies of all the components in the image.
  // sampling frequencies of all the components in the image.
  int v_sampling ;
  int h_sampling ;

  // Last encoded DC value.
  int last_dc_value ;

  // Entropy tables used by the component.
  JpegHuffmanDecoder *ac_table ;
  JpegHuffmanDecoder *dc_table ;

  // Quantization table used by the component
  JpegDecoderQuantizationTable *quantization_table ;

  // End of band Run - Progressive Specific
  int eob_run ;

  // Non-interleaved dimensions.
  int noninterleaved_rows ;
  int noninterleaved_cols ;

  int du_rows ;
  int du_cols ;

  JpegDecoderDataUnit *data_units ;
  JPEGSAMPLE *upsample_data ;
  JpegDecoderCoefficientBlock *coefficient_blocks ;
} ;

inline int JpegDecoderComponent::HorizontalFrequency () const
{
  return horizontal_frequency ;
}

inline int JpegDecoderComponent::VerticalFrequency () const
{
  return vertical_frequency ;
}

inline long JpegDecoderComponent::NoninterleavedRows () const
{
  return noninterleaved_rows ;
}

inline long JpegDecoderComponent::NoninterleavedCols () const
{
  return noninterleaved_cols ;
}

inline void JpegDecoderComponent::ResetDcDifference ()
{
  last_dc_value = 0 ;
  return ;
}

}
}

#endif

