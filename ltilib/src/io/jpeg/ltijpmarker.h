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
 * file .......: jpmarker.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpmarker.h,v 1.3 2003/02/17 07:16:04 alvarado Exp $
 */
#ifndef _LTI_MARKERS_H_
#define _LTI_MARKERS_H_
//

namespace lti {
  namespace jpeg {
//
//  JPEG Markers as defined in Section B.1.1.3 of the JPEG standard.
//

// These definitions do not include the preceding 0xFF byte.
enum JpegMarkers
{
  // Start of Frame Markers, Non-Differential Huffman Coding
  SOF0 = 0xC0,    // Baseline DCT
  SOF1 = 0xC1,    // Sequential DCT
  SOF2 = 0xC2,    // Progressive DCT
  SOF3 = 0xC3,    // Spatial (sequential) lossless
  // Start of Frame Markers, Differential Huffman Coding
  SOF5 = 0xC5,    // Differential Sequential DCT
  SOF6 = 0xC6,    // Differential Progressive DCT
  SOF7 = 0xC7,    // Differential Spatial
  // Start of Frame Markers, Non-Differential Arithmetic Coding
  SOF9 = 0xC9,    // Extended Sequential DCT
  SOFA = 0xCA,    // Progressive DCT
  SOFB = 0xCB,    // Spacial (sequential) Lossless
  // Start of Frame Markers, Differential Arithmetic Coding
  SOFD = 0xCD,    // Differential Sequential DCT
  SOFE = 0xCE,    // Differential Progressive DCT
  SOFF = 0xCF,    // Differential Spatial
  // Other Markers
  DHT = 0xC4,     // Define Huffman Tables
  DAC = 0xCC,     // Define Arithmetic Coding Conditions
  RST0 = 0xD0,    // Restart Marker
  RST1 = 0xD1,    // Restart Marker
  RST2 = 0xD2,    // Restart Marker
  RST3 = 0xD3,    // Restart Marker
  RST4 = 0xD4,    // Restart Marker
  RST5 = 0xD5,    // Restart Marker
  RST6 = 0xD6,    // Restart Marker
  RST7 = 0xD7,    // Restart Marker
  SOI = 0xD8,     // Start of Image
  EOI = 0xD9,     // End of Image
  SOS = 0xDA,     // Start of Scan
  DQT = 0xDB,     // Define Quantization Table
  DNL = 0xDC,     // Define Number of Lines
  DRI = 0xDD,     // Define Restart Intervale
  DHP = 0xDE,     // Define Hierarchical Progression
  EXP = 0xDF,     // Expand Reference Components
  APP0 = 0xE0,    // Application Segments
  APP1 = 0xE1,    // Application Segments
  APP2 = 0xE2,    // Application Segments
  APP3 = 0xE3,    // Application Segments
  APP4 = 0xE4,    // Application Segments
  APP5 = 0xE5,    // Application Segments
  APP6 = 0xE6,    // Application Segments
  APP7 = 0xE7,    // Application Segments
  APP8 = 0xE8,    // Application Segments
  APP9 = 0xE9,    // Application Segments
  APPA = 0xEA,    // Application Segments
  APPB = 0xEB,    // Application Segments
  APPC = 0xEC,    // Application Segments
  APPD = 0xED,    // Application Segments
  APPE = 0xEE,    // Application Segments
  APPF = 0xEF,    // Application Segments
  // C8, F0-FD, 01, 02-BF reserved
  COM = 0xFE,     // Comment
  SOB = 0xFF,     // Start of Block - Byte that precedes all others - not in the standard.
} ;

}
}
#endif

