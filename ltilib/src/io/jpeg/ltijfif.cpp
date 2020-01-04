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
 * file .......: jfif.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijfif.cpp,v 1.5 2003/02/17 07:16:03 alvarado Exp $
 */

//
// JPEG Encoder Library.
//
// Title:   JFIF Implementation
//
// Description:
//
//    This module contains the RGB/YcbCr conversion functions specified
//    by the JFIF standard. This functions are implemented using scaled
//    integers.
//
//    If you are using MS C++ you may wish to re-write this since that compile
//    generates terrible code for these function. Borland C++ is great here,
//    and G++ works well if you turn on optimizations.
//

//
#include "ltijfif.h"


namespace lti {
  namespace jpeg {

const int jpeg_ScaleFactor = 12 ;
const int jpeg_ScaleValue = (1<<jpeg_ScaleFactor) ;
const int jpeg_Rounding = (1<<(jpeg_ScaleFactor-1)) ;

static inline int Scaled (double value)
{
  return (int) (jpeg_ScaleValue * value) ;
}

JPEGSAMPLE YCbCrToR (int yy, int /*cb*/, int cr)
{
  int result = yy + ((Scaled (1.402) * (cr - JpegMidpointSampleValue) + jpeg_Rounding) >> jpeg_ScaleFactor) ;
  if (result < 0)
    result = 0 ;
  else if (result > JpegMaxSampleValue)
    result = JpegMaxSampleValue ;
  return (JPEGSAMPLE) result ;
}

JPEGSAMPLE YCbCrToG (int yy, int cb, int cr)
{
  int result = yy - ((Scaled (0.34414) * (cb - JpegMidpointSampleValue)
                     + Scaled (0.71414) * (cr - JpegMidpointSampleValue)
                     + jpeg_Rounding) >> jpeg_ScaleFactor) ;

  if (result < 0)
    result = 0 ;
  else if (result > JpegMaxSampleValue)
    result = JpegMaxSampleValue ;
  return (JPEGSAMPLE) result ;
}

JPEGSAMPLE YCbCrToB (int yy, int cb, int /*cr*/)
{
  int result = yy + ((Scaled (1.772) * (cb - JpegMidpointSampleValue)
             + jpeg_Rounding) >> jpeg_ScaleFactor) ;
  if (result < 0)
    result = 0 ;
  else if (result > JpegMaxSampleValue)
    result = JpegMaxSampleValue ;
  return (JPEGSAMPLE) result ;
}

JPEGSAMPLE RGBToY (JPEGSAMPLE red, JPEGSAMPLE green, JPEGSAMPLE blue)
{
  int result = (Scaled (0.299) * red
             + Scaled (0.587) * green + Scaled (0.114) * blue
             + jpeg_Rounding) >> jpeg_ScaleFactor ;
  if (result > JpegMaxSampleValue)
  	result = JpegMaxSampleValue ;
  else if (result < JpegMinSampleValue)
  	result = JpegMinSampleValue ;
  return (lti::ubyte)result ;
}

JPEGSAMPLE RGBToCb (JPEGSAMPLE red, JPEGSAMPLE green, JPEGSAMPLE blue)
{
  int result = ((JpegMidpointSampleValue<<jpeg_ScaleFactor)
             + jpeg_Rounding - Scaled (0.1687) * red
             - Scaled (0.3313) * green + Scaled (0.5) * blue) >> jpeg_ScaleFactor  ;
  if (result > JpegMaxSampleValue)
  	result = JpegMaxSampleValue ;
  else if (result < JpegMinSampleValue)
  	result = JpegMinSampleValue ;
  return (lti::ubyte)result ;
}

JPEGSAMPLE RGBToCr (JPEGSAMPLE red, JPEGSAMPLE green, JPEGSAMPLE blue)
{
  int result = ((JpegMidpointSampleValue<<jpeg_ScaleFactor)
             + jpeg_Rounding + Scaled (0.5) * red
             - Scaled (0.4187) * green - Scaled (0.0813) * blue) >> jpeg_ScaleFactor ;
  if (result > JpegMaxSampleValue)
  	result = JpegMaxSampleValue ;
  else if (result < JpegMinSampleValue)
  	result = JpegMinSampleValue ;
  return (lti::ubyte)result ;
}

JfifHeader::JfifHeader() {
  big_endian = new lti::endianness(lti::endianness::BigEndian);
}

JfifHeader::~JfifHeader() {
  delete big_endian;
}

void JfifHeader::read(std::istream &in_stream) {
  int i;
  big_endian->read(in_stream, data.length);
  for (i = 0; i < 5; i++)
	big_endian->read(in_stream, data.identifier[i]);
  for (i = 0; i < 2; i++)
	big_endian->read(in_stream, data.version[i]);
  big_endian->read(in_stream, data.units);
  big_endian->read(in_stream, data.xdensity);
  big_endian->read(in_stream, data.ydensity);
  big_endian->read(in_stream, data.xthumbnail);
  big_endian->read(in_stream, data.ythumbnail);
}

void JfifHeader::write(std::ostream &out_stream) {
  int i;
  big_endian->write(out_stream, data.length);
  for (i = 0; i < 5; i++)
	big_endian->write(out_stream, data.identifier[i]);
  for (i = 0; i < 2; i++)
	big_endian->write(out_stream, data.version[i]);
  big_endian->write(out_stream, data.units);
  big_endian->write(out_stream, data.xdensity);
  big_endian->write(out_stream, data.ydensity);
  big_endian->write(out_stream, data.xthumbnail);
  big_endian->write(out_stream, data.ythumbnail);
}

} // namespace jpeg
}

