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
 * file .......: jpgendu.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpendu.h,v 1.6 2003/04/15 07:55:33 alvarado Exp $
 */

#ifndef _LTI_JPENDU_H_
#define _LTI_JPENDU_H_

//
//  Title:  JpegEncoderDataUnit class definition
//
//  Description:
//
//    This class represents a data unit in the JPEG encoder.
//

#include "ltijpeg.h"
#include "ltijpencobk.h"
#include "ltijpenquan.h"

#include <cmath>


namespace lti {
  namespace jpeg {

/**
 * Jpeg Encoder Data Unit.
 *
 * This class represents a data unit in the JPEG encoder.
 */
class JpegEncoderDataUnit
{
public:
  typedef JPEGSAMPLE DATAUNITVALUE ;

  JpegEncoderDataUnit () {} ;
  ~JpegEncoderDataUnit () {} ; //Must not be virtual.

  DATAUNITVALUE *operator[] (unsigned int) ;
  DATAUNITVALUE &ZigZagInput (int) ;

  void ForwardDct (JpegEncoderQuantizationTable &,
                   JpegEncoderCoefficientBlock &) ;

private:
  JpegEncoderDataUnit (const JpegEncoderDataUnit &) ;
  JpegEncoderDataUnit &operator=(const JpegEncoderDataUnit &) ;

  DATAUNITVALUE data [JpegSampleWidth][JpegSampleWidth] ;
} ;

inline JpegEncoderDataUnit::DATAUNITVALUE *JpegEncoderDataUnit::operator [](unsigned int ii)
{
  return data [ii] ;
}

inline JpegEncoderDataUnit::DATAUNITVALUE &JpegEncoderDataUnit::ZigZagInput (int ii)
{
  int order = JpegZigZagInputOrder (ii) ;
  return ((DATAUNITVALUE *) data) [order] ;
}

}
}

#endif

