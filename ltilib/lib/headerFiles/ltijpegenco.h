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
 * file .......: jpgenco.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpegenco.h,v 1.10 2003/04/15 07:55:33 alvarado Exp $
 */

#ifndef _LTI_JPEGENCO_H_
#define _LTI_JPEGENCO_H_

//
//  JPEG Encoder Library.
//
//  Title: JpegEncoder class definition.
//
//  Description:
//
//    This class is an encoder for JPEG image. The process for using this
//    class is to call the property functions to set the attributes for the
//    stream then call the WriteImage function to create a JPEG stream with
//    those attributes.
//

#include "ltijpgexcep.h"
#include "ltijfif.h"
#include "ltijpencomp.h"
#include "ltijpenquan.h"
#include "ltijpenhuff.h"

#include "ltiImage.h"

#include <cmath>
#include <string>
#include <fstream>


namespace lti {
  namespace jpeg {

/**
 * JpegEncoder class definition.
 *
 * This class is an encoder for JPEG image. The process for using this
 * class is to call the property functions to set the attributes for the
 * stream then call the WriteImage function to create a JPEG stream with
 * those attributes.
 */
class JpegEncoder
{
public:
  JpegEncoder () ;
  virtual ~JpegEncoder () ;
  // Required Member Functions

  void writeImageFile(const std::string &filename, const lti::image &image);
  /**
   *  This function writes an image to the output stream.
   *
   *  Parameters:
   *  @param strm The output stream to write the image to. This must have
   *         been opened in binary mode.
   * @param image The image to output.
   */
  virtual void WriteImage (std::ostream &strm, const lti::image &image) ;

  /*
   * Property Functions
   */

  //@{
  /**
   * Image Quality (1-100)
   */
  int GetQuality () const ;
  void SetQuality (int) ;
  //@}

  //@{
  /**
   * Grayscale Mode (True=Gray Scale, False=Color)
   */
  bool GetGrayscale () const ;
  void SetGrayscale (bool) ;
  //@}

  //@{
  /**
   * Progressive Mode (True=Progressive, False=Sequential)
   */
  bool GetProgressive () const ;
  void SetProgressive (bool) ;
  //@}

  //@{
  /**
   * Number of rows between restart markers (0=> No restart markers)
   */
  int GetRowsPerRestart () const ;
  void SetRowsPerRestart (int) ;
  //@}

  //@{
  /**
   * Comment String
   */
  std::string GetComment () const ;
  void SetComment (const std::string &c) { comment_string = c; }
  //@}

  //@{
  /**
   * Component Sampling Frequencies (1-4)
   */
  void SetSamplingFrequency (int component, int hf, int vf) ;
  void GetSamplingFrequency (int component, int &hf, int &vf) ;
  //@}

  //@{
  /**
   * spectral selection end (0-63), successive approximation (0-13) }
   */
  void SetScanAttributes (int scan, long components, int sse, int ssa) ;
  void GetScanAttributes (int scan, long &components, int &sse, int &ssa) ;
  //@}

  /**
   * Maximum number of scans
   */
  enum { MaxScans = 256, } ;

  /**
   * Component Identifiers
   */
  enum { YComponent = 1, CbComponent = 2, CrComponent = 3, } ;

protected:
  //@{
  /**
   * Output Functions used by other classes to write data to the output stream.
   */
  void OutputBits (int bits, int count) ;
  void OutputByte (lti::ubyte) ;
  void OutputWord (lti::uint16) ;
  //@}

private:


  //!  void DoCopy (const JpegEncoder &) ;
  void Initialize () ;

  /**
   * This structure is used to represent a scan.
   */
  struct Scan
  {
    /**
     * Bitmap of components in the scan.
     */
    long component_mask ;
    /**
     * Spectral Selection for the scan.
     */
    int spectral_selection_start ;
    int spectral_selection_end ;
    /**
     * Successive Approximation for the first iteration of the scan
     */
    int successive_approximation ;
    /**
     * Successive Approximation Progress. Updated as scans are output.
     */
    int successive_approximation_high ;
    int successive_approximation_low ;
  } ;

  /**
   * This function determines if the output scan parameters are valid. It
   * throwse the EJpegError exception an inconsistency is found.
   */
  void ValidateParameters () ;

  /**
   * Basic Output Methods
   */
  void FlushBitBuffer () ;

  void OutputMarker (lti::ubyte) ;

  //@{
  /**
   * Block Output Method
   */
  void PrintQuantizationTables () ;
//!  void PrintSequentialFrame (BitmapImage &image) ;
  void PrintSequentialFrame (const lti::image &image) ;
//!  void PrintProgressiveFrame (BitmapImage &image) ;
  void PrintProgressiveFrame (const lti::image &image) ;
  void PrintComment (const std::string &) ;
  void OutputJfifHeader () ;

  void OutputRestartInterval (int restartinterval) ;

  void PrintHuffmanTables (const Scan &scan, bool usedc, bool useac) ;
  //@}

  /**
   * Sequential Scan Output
   */
  void PrintSequentialScan (const Scan &scan) ;

  //@{
  /**
   * Progressive Output Function
   */
  void PrintProgressiveScan (const Scan &scan) ;
  void PrintDcFirst (const Scan &scan) ;
  void PrintDcRefine (const Scan &scan) ;
  void PrintAcFirst (const Scan &scan) ;
  void PrintAcRefine (const Scan &scan) ;
  void FirstAcData (const Scan &scan, bool outputrestarts, JpegEncoderComponent::ACOUTPUTFUNCTION acfunction) ;
  void RefineAcData (const Scan &scan, bool outputrestarts, JpegEncoderComponent::ACOUTPUTFUNCTION acfunction) ;


  void InterleavedPass (
                      bool writedata,
                      JpegEncoderComponent::COMPONENTPASSFUNCTION passfunction,
                      JpegEncoderComponent::DCOUTPUTFUNCTION dcfunction,
                      JpegEncoderComponent::ACOUTPUTFUNCTION acfunction,
                      int sss,
                      int sse,
                      int ssa) ;
  void NoninterleavedPass (
                      bool writedata,
                      JpegEncoderComponent::COMPONENTPASSFUNCTION passfunction,
                      JpegEncoderComponent::DCOUTPUTFUNCTION dcfunction,
                      JpegEncoderComponent::ACOUTPUTFUNCTION acfunction,
                      int sss,
                      int sse,
                      int ssa) ;

  void ResetDcValues () ;
  void CalculateMcuDimensions () ;
  void CountPassesForProgressReporting () ;

  void FindComponentsInScan (Scan &scan) ;
  void CreateQuantizationTables (int quality) ;
  //@}

  /**
   * @name Data used for bit I/O
   */
  //@{
  /**
   * Number of bits that habe been buffered
   */
  int bit_count ;
  lti::ubyte bit_buffer ;
  //@}

  //@{
  /**
   * Quantization Table
   */
  JpegEncoderQuantizationTable *chrominance_quanttbl ;  // For Y
  JpegEncoderQuantizationTable *luminance_quanttbl ;    // For Cb and Cr
  //@}

  //@{
  /**
   * Huffman Tables
   */
  JpegEncoderHuffmanTable *ac_tables ;
  JpegEncoderHuffmanTable *dc_tables ;
  //@}

  //@{
  /**
   * Property
   */
  bool gray_scale ;
  int rows_per_restart ;
  int restart_interval ;
  int image_quality ;
  std::string comment_string ;
  bool progressive_mode ;

  int total_passes ;
  int current_pass ;
  //@}

  //@{
  /**
   * Image Size
   */
  int frame_width ;
  int frame_height ;
  //@}

  //@{
  /**
   * Maximum Frequencies in all components
   */
  int max_horizontal_frequency ;
  int max_vertical_frequency ;
  //@}

  //@{
  /**
   * MCU Dimensions
   */
  int  mcu_rows ;
  int  mcu_cols ;
  //@}

  const lti::image *current_image ;
  std::ostream *output_stream ;

  int scan_count ;

  /**
   * Scan Descriptors
   */
  Scan image_scans [MaxScans] ;
  /**
   * Components
   */
  enum { MaxComponents = 4, } ;
  JpegEncoderComponent image_components [MaxComponents] ;
  /**
   * Components used in the scan being processed.
   */
  int scan_component_count ;
  JpegEncoderComponent *scan_components [JpegMaxComponentsPerScan] ;

  lti::endianness *big_endian;

  friend class JpegEncoderHuffmanTable ;
  friend class JpegEncoderComponent ;
} ;

//
//  The RowsPerRestart parameter specifies the number of MCU rows
//  output between restart markers. A value of zero means no restart
//  markers are used.
//
inline int JpegEncoder::GetRowsPerRestart () const
{
  return rows_per_restart ;
}

inline void JpegEncoder::SetRowsPerRestart (int rows)
{
  rows_per_restart = rows ;
  return ;
}

inline void JpegEncoder::SetGrayscale (bool value)
{
  gray_scale = value ;
  return ;
}

inline bool JpegEncoder::GetGrayscale () const
{
  return gray_scale ;
}

}
}

#endif

