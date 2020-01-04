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
 * file .......: jpegdeco.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpegdeco.h,v 1.7 2003/02/17 07:16:03 alvarado Exp $
 */

#ifndef _LTI_JPEGDECO_H_
#define _LTI_JPEGDECO_H_

//
//  Title:  JPEG Decoder Class Definition
//


#include "ltijpgexcep.h"
#include "ltijfif.h"
#include "ltijpdehuff.h"
#include "ltijpdequan.h"
#include "ltijpdecomp.h"

#include "ltiImage.h"
#include "ltiIOFunctor.h"

#include <iostream>
#include <fstream>
#include <limits.h>


namespace lti {
  namespace jpeg {
class JpegDecoderComponent ;
class JpegHuffmanDecoder ;
class JpegDecoderQuantizationTable ;


/**
 * JPEG Decoder Class Definition
 */
class JpegDecoder
{
public:
  JpegDecoder () ;
//!  JpegDecoder (const JpegDecoder &) ;
  virtual ~JpegDecoder () ;

  virtual void readImageFile(const std::string &filename, lti::image &image, bool &isGray) ;
  virtual void readImageFile(std::ifstream &filename, lti::image &image, bool &isGray) ;
  virtual void checkImageFile(const std::string &filename, lti::point &size, std::string &comment, bool &isGray);
  bool GetVerbose () const ;
  void SetVerbose (bool) ;

  virtual void UpdateImage () ;

protected:
  // While we don't expect classes to be derived from this one,
  // we have placed the functions we expect friend classes to
  // call here.

  lti::ubyte ReadByte () ;
  lti::uint16 ReadWord () ;
  int NextBit () ;
  int Receive (int count) ;
  bool IsProgressive () const ;
  int McuRows () const ;
  int McuCols () const ;
  int FrameHeight () const ;
  int FrameWidth () const ;
  int MaxVFrequency () const ;
  int MaxHFrequency () const ;

  int RestartInterval () const ;

private:

  void ReadImage (std::istream &istrm, lti::image &image) ;

  void Initialize () ;
  void GetScanCount () ;

  void ReadStreamHeader () ;
  void ReadMarker () ;
  void ReadHeaderMarker () ;
  void SkipMarker() ;
  void CheckHeader(std::istream &istrm, lti::point &size, std::string &comment, bool &isGray);
  void ReadApplication (lti::ubyte type) ;

  void ReadHuffmanTable () ;
  void ReadQuantization () ;
  void ReadRestartInterval () ;
  void ReadStartOfFrame (lti::ubyte type, bool check_only = false) ;
  void ReadStartOfScan () ;
  void CalculateMcuDimensions ();
  bool ScanIsInterleaved () const ;

  void ReadProgressiveScanData ( int sss, int sse,
                                int sah, int sal) ;
  void ReadDcFirst (int ssa) ;
  void ReadDcRefine (int ssa) ;
  void ReadAcFirst (int sss, int sse, int ssa) ;
  void ReadAcRefine (int sss,
                     int sse,
                     int ssa) ;
  void FreeAllocatedResources () ;

  void ReadSequentialScanData () ;

  void ReadSequentialInterleavedScan () ;
  void ReadSequentialNonInterleavedScan () ;
  void ResetDcDifferences () ;
  void ProcessRestartMarker () ;

  void RefineAcCoefficient (lti::int16 &value, int ssa) ;

  // Huffman tables
  JpegHuffmanDecoder *ac_tables ;
  JpegHuffmanDecoder *dc_tables ;

  // Quantization tables
  JpegDecoderQuantizationTable *quantization_tables ;

  // Bit I/O state
  int bit_position ;        // Called CNT in Section F.2.2.5
  unsigned char bit_data ;  // Called B in Section F.2.2.5

  bool eoi_found ;
  bool sof_found ;

  bool verbose_flag ;

  std::istream *input_stream ;

  int restart_interval ;

  int frame_width ;
  int frame_height ;
  int frame_type ;

  int max_horizontal_frequency ;
  int max_vertical_frequency ;

  bool progressive_frame ;

  std::string img_comment;

  int component_count ;
  JpegDecoderComponent *components ;
  int *component_indices ;

  // Address of the image that is currently being processed.
//!  BitmapImage *current_image ;
  lti::image *current_image ;

  // Progress Counters
  int current_scan ;
  int scan_count ;

  int mcu_rows ;
  int mcu_cols ;

  int mcu_height ;
  int mcu_width ;

  int scan_component_count ;
  JpegDecoderComponent **scan_components ;

  int expected_restart ;

  bool strict_jfif ;

  lti::endianness *big_endian;

  friend class JpegDecoderQuantizationTable ;
  friend class JpegHuffmanDecoder ;
  friend class JpegDecoderComponent ;
} ;


inline bool JpegDecoder::IsProgressive () const
{
  return progressive_frame ;
}

inline int JpegDecoder::McuRows () const
{
  return mcu_rows ;
}

inline int JpegDecoder::McuCols () const
{
  return mcu_cols ;
}

inline int JpegDecoder::RestartInterval () const
{
  return restart_interval ;
}

inline int JpegDecoder::FrameHeight () const
{
  return frame_height ;
}

inline int JpegDecoder::FrameWidth () const
{
  return frame_width ;
}

inline int JpegDecoder::MaxVFrequency () const
{
  return max_vertical_frequency ;
}

inline int JpegDecoder::MaxHFrequency () const
{
  return max_horizontal_frequency ;
}

inline bool JpegDecoder::ScanIsInterleaved () const
{
  if (scan_component_count != 1)
    return true ;
  else
    return false ;
}

inline bool JpegDecoder::GetVerbose () const
{
  return verbose_flag ;
}

inline void JpegDecoder::SetVerbose (bool value)
{
  verbose_flag = value ;
  return ;
}

}
}

#endif

