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
 * file .......: jpgendu.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpendu.cpp,v 1.5 2003/10/30 10:02:50 alvarado Exp $
 */
//
// JPEG Encoder Library.
//
// Title:   EncoderDataUnit Class Implementation
//
//


#include "ltijpendu.h"


namespace lti {
  namespace jpeg {

#if defined (_LTI_MSC_VER)
const double M_PI = acos (-1.0) ;
#endif

const double FC4 = cos (M_PI * 4.0 / 16.0) ;
const double FSEC2 = 0.5 / cos (M_PI * 2.0 / 16.0) ;
const double FSEC6 = 0.5 / cos (M_PI * 6.0 / 16.0) ;


//
//  Description:
//
//    This is an implementation of the Forward Discrete Transform based
//    on matrix factorization of the DCT matrix. My first factorization
//    always left a constant factor of 1/8 at the end which could be merged
//    with quantization or as part of an integer descaling.
//
//    Using the cosine product formula it was possible to eliminate some
//    multiplication operations which resulted in a more complex scaling
//    matrix.
//
//    I have documented the derivation process for this DCT process as well
//    as another I have tried. I have some more factorization ideas to try
//    out when I have to to get around to it. Unfortunately matrix factorization
//    is a very tedious process. When you see the documents it looks easy, but
//    believe me and all the legal pad I went through that it is not.
//
//    This implementation is a litteral mapping of the matrix implementation.
//    Each set of temporaries represents one matrix multiplication. Hopefully
//    your compile will optimize the temporaries out. It is possible to
//    reorder the operations to reduce the number of temporaries variables
//    required (The Intel C++ compile does this on its own) which seems to be
//    the best optimization to try next.
//
//
//    I have not implemented a scaled integer version of the FDCT because I
//    believe that most people will want quality over speed in encoding.
//
//  Parameters:
//    qt:  The quantization table
//    output:  The output DCT coefficients
//

void JpegEncoderDataUnit::ForwardDct (JpegEncoderQuantizationTable &qt,
                                      JpegEncoderCoefficientBlock &output)
{
  double tmp [JpegSampleWidth][JpegSampleWidth] ;

  for (int col = 0 ; col < JpegSampleWidth ; ++ col)
  {
    // Shift this first matrix multiplication includes the range shift from
    // 0...255 to -128...127. Notice that the shift term is canceled out
    // in the alst four elements.
    double a0 = data [0][col] + data [7][col] - 2 * JpegMidpointSampleValue ;
    double a1 = data [1][col] + data [6][col] - 2 * JpegMidpointSampleValue ;
    double a2 = data [2][col] + data [5][col] - 2 * JpegMidpointSampleValue ;
    double a3 = data [3][col] + data [4][col] - 2 * JpegMidpointSampleValue ;
    double a4 = data [3][col] - data [4][col] ;
    double a5 = data [2][col] - data [5][col] ;
    double a6 = data [1][col] - data [6][col] ;
    double a7 = data [0][col] - data [7][col] ;

    double b0 = a0 + a3 ;
    double b1 = a1 + a2 ;
    double b2 = a1 - a2 ;
    double b3 = a0 - a3 ;
    double b4 = a4 ;
    double b5 = a5 ;
    double b6 = a6 ;
    double b7 = a7 ;

    double c0 = b0 ;
    double c1 = b1 ;
    double c2 = b2 + b3 ;
    double c3 = b3 ;
    double c4 = b4 + b5 ;
    double c5 = b5 + b6 ;
    double c6 = b6 + b7 ;
    double c7 = b7 ;

    double d0  = c0 ;
    double d1  = c1 ;
    double d2  = c2 ;
    double d3  = c3 ;
    double d4  = c4 + c6 ;
    double d5  = c5 ;
    double d6  = c6 ;
    double d7  = c7 ;

    double e0 = d0 + d1 ;
    double e1 = d0 - d1 ;
    double e2 = FC4 * d2 ;
    double e3 = d3 ;
    double e4 = FC4 * d4 ;
    double e5 = FC4 * d5 ;
    double e6 = d6 ;
    double e7 = d7 ;

    double f0 = e0 ;
    double f1 = e1 ;
    double f2 = e2 ;
    double f3 = e3 ;
    double f4 = e4 + e6 ;
    double f5 = e5 ;
    double f6 = e4 - e6 ;
    double f7 = e7 ;

    double g0 = f0 ;
    double g1 = f1 ;
    double g2 = f2 ;
    double g3 = f3 ;
    double g4 = FSEC2 * f4 ;
    double g5 = f7 - f5 ;
    double g6 = FSEC6 * f6 ;
    double g7 = f5 + f7 ;

    double h0 = g0 ;
    double h1 = g1 ;
    double h2 = g2 + g3 ;
    double h3 = g3 - g2 ;
    double h4 = g4 + g7 ;
    double h5 = g5 + g6 ;
    double h6 = g5 - g6 ;
    double h7 = g7 - g4 ;

    tmp [0][col] = h0 ;
    tmp [1][col] = h4 ;
    tmp [2][col] = h2 ;
    tmp [3][col] = h6 ;
    tmp [4][col] = h1 ;
    tmp [5][col] = h5 ;
    tmp [6][col] = h3 ;
    tmp [7][col] = h7 ;
  }

  for (int row = 0 ; row < JpegSampleWidth ; ++ row)
  {
    double a0 = tmp [row][0] + tmp [row][7] ;
    double a1 = tmp [row][1] + tmp [row][6] ;
    double a2 = tmp [row][2] + tmp [row][5] ;
    double a3 = tmp [row][3] + tmp [row][4] ;
    double a4 = tmp [row][3] - tmp [row][4] ;
    double a5 = tmp [row][2] - tmp [row][5] ;
    double a6 = tmp [row][1] - tmp [row][6] ;
    double a7 = tmp [row][0] - tmp [row][7] ;

    double b0 = a0 + a3 ;
    double b1 = a1 + a2 ;
    double b2 = a1 - a2 ;
    double b3 = a0 - a3 ;
    double b4 = a4 ;
    double b5 = a5 ;
    double b6 = a6 ;
    double b7 = a7 ;

    double c0 = b0 ;
    double c1 = b1 ;
    double c2 = b2 + b3 ;
    double c3 = b3 ;
    double c4 = b4 + b5 ;
    double c5 = b5 + b6 ;
    double c6 = b6 + b7 ;
    double c7 = b7 ;

    double d0  = c0 ;
    double d1  = c1 ;
    double d2  = c2 ;
    double d3  = c3 ;
    double d4  = c4 + c6 ;
    double d5  = c5 ;
    double d6  = c6 ;
    double d7  = c7 ;

    double e0 = d0 + d1 ;
    double e1 = d0 - d1 ;
    double e2 = FC4 * d2 ;
    double e3 = d3 ;
    double e4 = FC4 * d4 ;
    double e5 = FC4 * d5 ;
    double e6 = d6 ;
    double e7 = d7 ;

    double f0 = e0 ;
    double f1 = e1 ;
    double f2 = e2 ;
    double f3 = e3 ;
    double f4 = e4 + e6 ;
    double f5 = e5 ;
    double f6 = e4 - e6 ;
    double f7 = e7 ;

    double g0 = f0 ;
    double g1 = f1 ;
    double g2 = f2 ;
    double g3 = f3 ;
    double g4 = FSEC2 * f4 ;
    double g5 = f7 - f5 ;
    double g6 = FSEC6 * f6 ;
    double g7 = f5 + f7 ;

    double h0 = g0 ;
    double h1 = g1 ;
    double h2 = g2 + g3 ;
    double h3 = g3 - g2 ;
    double h4 = g4 + g7 ;
    double h5 = g5 + g6 ;
    double h6 = g5 - g6 ;
    double h7 = g7 - g4 ;

    double i0 = h0 * qt.float_scaling [row][0] ;
    double i1 = h4 * qt.float_scaling [row][1] ;
    double i2 = h2 * qt.float_scaling [row][2] ;
    double i3 = h6 * qt.float_scaling [row][3] ;
    double i4 = h1 * qt.float_scaling [row][4] ;
    double i5 = h5 * qt.float_scaling [row][5] ;
    double i6 = h3 * qt.float_scaling [row][6] ;
    double i7 = h7 * qt.float_scaling [row][7] ;

    if (i0 >= 0.0)
      output [row][0] = (lti::int16) (i0 + 0.5) ;
    else
      output [row][0] = (lti::int16) (i0 - 0.5) ;
    if (i1 >= 0.0)
      output [row][1] = (lti::int16) (i1 + 0.5) ;
    else
      output [row][1] = (lti::int16) (i1 - 0.5) ;
    if (i2 >= 0.0)
      output [row][2] = (lti::int16) (i2 + 0.5) ;
    else
      output [row][2] = (lti::int16) (i2 - 0.5) ;
    if (i3 >= 0.0)
      output [row][3] = (lti::int16) (i3 + 0.5) ;
    else
      output [row][3] = (lti::int16) (i3 - 0.5) ;
    if (i4 >= 0.0)
      output [row][4] = (lti::int16) (i4 + 0.5) ;
    else
      output [row][4] = (lti::int16) (i4 - 0.5) ;
    if (i5 >= 0.0)
      output [row][5] = (lti::int16) (i5 + 0.5) ;
    else
      output [row][5] = (lti::int16) (i5 - 0.5) ;
    if (i6 >= 0.0)
      output [row][6] = (lti::int16) (i6 + 0.5) ;
    else
      output [row][6] = (lti::int16) (i6 - 0.5) ;
    if (i7 >= 0.0)
      output [row][7] = (lti::int16) (i7 + 0.5) ;
    else
      output [row][7] = (lti::int16) (i7 - 0.5) ;
  }

  return ;
}

  }
}

