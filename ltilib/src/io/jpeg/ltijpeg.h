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
 * file .......: jpeg.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpeg.h,v 1.6 2003/06/23 12:17:44 alvarado Exp $
 */


#ifndef _LTI_JPEG_H_
#define _LTI_JPEG_H_

//
//  Title:  JPEG Definitions and Utility Functions
//
//  Author:  John M. Miano  miano@colosseumbuilders.com
//

#include "ltijpmarker.h"

#include "ltiTypes.h"


namespace lti {
  namespace jpeg {

// Table B.5
const int JpegMaxHuffmanTables = 4 ;
// Table B.4
const int JpegMaxQuantizationTables = 4 ;

// Table B.2
const int JpegMaxComponentsPerFrame = 255 ;
// Table B.3
const int JpegMaxComponentsPerScan = 4 ;

// Table B.2
const int JpegMinSamplingFrequency = 1 ;
const int JpegMaxSamplingFrequency = 4 ;


// A.1.3
const int JpegSampleWidth = 8 ;
const int JpegSampleSize = JpegSampleWidth * JpegSampleWidth ;

// Functions to convert to Zig-Zag order
/**
 * Functions to convert to Zig-Zag order JpegZigZagInputOrder
 */
int JpegZigZagInputOrder (unsigned int item) ;

/**
 * Functions to convert to Zig-Zag order JpegZigZagOutputOrder
 */
int JpegZigZagOutputOrder (unsigned int item) ;

// Datatype used to represent a sample value.
typedef lti::ubyte JPEGSAMPLE ; // For 12-Bits this would be lti::uint16
typedef lti::byte JPEGCOEFFICIENT ;

const int JpegMinSampleValue = 0 ;
const int JpegMaxSampleValue = 255 ; // For 12-Bits this would be 4095
const int JpegMidpointSampleValue = 128 ; // For 12-Bits this 2048


// Table B.5
const int JpegMaxHuffmanCodeLength = 16 ;
const int JpegMaxNumberOfHuffmanCodes = 256 ;

// B.2.3
const int JpegMaxDataUnitsPerMCU = 10 ;

// Section B.2.3 Table B.3
const int JpegMaxSuccessiveApproximation = 13 ;

// Section B.2.4.1 Table B.4

const int JpegMax8BitQuantizationValue = 255 ;
const int JpegMinQuantizationValue = 1 ;

extern const int JpegZigZagInputOrderCodes [JpegSampleSize] ;
extern const int JpegZigZagOutputOrderCodes [JpegSampleSize] ;

inline int JpegZigZagInputOrder (unsigned int item)
{
  return JpegZigZagInputOrderCodes [item] ;
}

inline int JpegZigZagOutputOrder (unsigned int item)
{
  return JpegZigZagOutputOrderCodes [item] ;
}

}
}
#endif

