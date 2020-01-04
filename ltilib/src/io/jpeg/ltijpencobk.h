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
 * file .......: jpgencobk.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpencobk.h,v 1.5 2003/02/17 07:16:03 alvarado Exp $
 */

#ifndef _LTI_JPENCOBK_H_
#define _LTI_JPENCOBK_H_

//
//  Title:  JpegEncoderCoefficientBlock class definition
//
//  Description:
//
//    This class is used to store DCT coefficients within component object.
//

#include "ltijpeg.h"


namespace lti {
  namespace jpeg {

/**
 * JpegEncoderCoefficientBlock class definition
 *
 *  This class is used to store DCT coefficients within component object.
 */
class JpegEncoderCoefficientBlock
{
public:
  lti::int16 *operator[](unsigned int index) ;
  lti::int16 &ZigZagInput (int index) ;
private:
  lti::int16 data [JpegSampleWidth][JpegSampleWidth] ;
} ;

inline lti::int16 *JpegEncoderCoefficientBlock::operator[](unsigned int index)
{
  return data [index] ;
}

inline lti::int16 &JpegEncoderCoefficientBlock::ZigZagInput ( int index)
{
  return ((lti::int16 *) data) [JpegZigZagInputOrder (index)] ;
}

}
}
#endif

