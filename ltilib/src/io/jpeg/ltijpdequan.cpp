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
 * file .......: jpgexcep.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpdequan.cpp,v 1.5 2003/02/17 07:16:03 alvarado Exp $
 */

//
// JPEG Decoder Quantization Table Class Implementation
//

#include "ltijpdequan.h"
#include "ltijpegdeco.h"

using namespace std ;

namespace lti {
  namespace jpeg {


//
// This table consists of the values
//
//   F (i, j) = X (i) X (j) / 8
//
// where
//
//  X (n) = 1, n = 0, 4
//  X (n) = 1 / sqrt(2) / cos (n*PI/16)
//

static const double floatscaling [JpegSampleWidth][JpegSampleWidth] =
{
{ 0.125,                  0.09011997775086849627, 0.09567085809127244544, 0.1063037618459070632,  0.125,                  0.159094822571604233,  0.2309698831278216846, 0.4530637231764438333, },
{ 0.09011997775086849627, 0.0649728831185362593,  0.0689748448207357645,  0.07664074121909414394, 0.09011997775086849627, 0.1147009749634507608, 0.1665200058287998886, 0.3266407412190940884, },
{ 0.09567085809127244544, 0.0689748448207357645,  0.0732233047033631207,  0.08136137691302557096, 0.09567085809127244544, 0.1217659055464329343, 0.1767766952966368932, 0.3467599613305368256, },
{ 0.1063037618459070632,  0.07664074121909414394, 0.08136137691302557096, 0.09040391826073060355, 0.1063037618459070632,  0.135299025036549253,  0.1964237395967755595, 0.3852990250365491698, },
{ 0.125,                  0.09011997775086849627, 0.09567085809127244544, 0.1063037618459070632,  0.125,                  0.159094822571604233,  0.2309698831278216846, 0.4530637231764438333, },
{ 0.159094822571604233,   0.1147009749634507608,  0.1217659055464329343,  0.135299025036549253,   0.159094822571604233,   0.2024893005527218515, 0.2939689006048396558, 0.5766407412190940329, },
{ 0.2309698831278216846,  0.1665200058287998886,  0.1767766952966368932,  0.1964237395967755595,  0.2309698831278216846,  0.2939689006048396558, 0.4267766952966368654, 0.8371526015321518744, },
{ 0.4530637231764438333,  0.3266407412190940884,  0.3467599613305368256,  0.3852990250365491698,  0.4530637231764438333,  0.5766407412190940329, 0.8371526015321518744, 1.642133898068010689,  },
} ;

//
//  Description:
//
//    Class Default Constructor
//
JpegDecoderQuantizationTable::JpegDecoderQuantizationTable ()
{
  table_defined = false ;
  memset (data_values, 0, sizeof (data_values)) ;
  return ;
}

//
//  Description:
//
//    This function reads a quantization table from a JPEG stream.
//
//  Parameters:
//    decoder:  The JPEG decoder that owns the table and the JPEG stream.
//    precision: The quantization table precision
//
void JpegDecoderQuantizationTable::ReadTable (JpegDecoder &decoder,
                                              int precision)
{
  // B 2.4.1
  // Determine if 16-bit or 8-bit precision is used for the quantization
  // values in the file.
  if (precision == 1)
  {
// Our source code only allows 8-bit data. The standard says
// 16-bit quantization tables are not allowed with 8-bit data.
// The commented code shows how 16-bit tables would be implemented.
//
//    // Read 16-bit values.
//    for (unsigned int ii = 0 ; ii < SampleSize ; ++ ii)
//    {
//      data_values[ii] = decoder.ReadWord () ;
//      if (data_values[ii] == 0)
//        throw JpegBadData ("Zero value in quantization table") ;
//    }
    throw JpegBadData ("Only 8-bit Data is Supported") ;
  }
  else if (precision == 0)
  {
    // Read 8-bit values.
    for (int ii = 0 ; ii < JpegSampleSize ; ++ ii)
    {
      data_values[ii] = decoder.ReadByte () ;
      if (data_values[ii] == 0)
        throw JpegBadData ("Zero value in quantization table") ;
    }
  }
  else
  {
    throw JpegBadData ("Invalid Quantization Table Precision") ;
  }

  BuildScaledTables () ;

  table_defined = true ;
  return ;
}

//
//  Description:
//
//    This function creates scaled quantization tables that
//    allow quantization to be merged with the IDCT process.
//    We factor the DCT matrix so that the first step in the
//    IDCT is to multiply each value by a constant. Here we
//    merge that constant with the quantization table valus.
//
void JpegDecoderQuantizationTable::BuildScaledTables ()
{
  int ii ;  // Overcome MSVC++ Wierdness

  for (ii = 0 ; ii < JpegSampleWidth ; ++ ii)
  {
    for (int jj = 0 ; jj < JpegSampleWidth ; ++ jj)
    {
      float_scaling [ii][jj] = data_values [JpegZigZagOutputOrder (ii * 8 + jj)]
                             * floatscaling [ii][jj] ;
    }
  }

  for (ii = 0 ; ii < JpegSampleWidth ; ++ ii)
  {
    for (int jj = 0 ; jj < JpegSampleWidth ; ++ jj)
    {
      integer_scaling [ii][jj] = (long) ((1 << QuantizationIntegerScale)
                               * floatscaling [ii][jj]
                               * data_values  [JpegZigZagOutputOrder (ii * 8 + jj)]) ;
    }
  }
  return ;
}

//
//  Description:
//
//    This is a debugging function that prints the contents
//    of the quantization table to a stream.
//
//  Parameters:
//
//    strm:  The output stream
//
void JpegDecoderQuantizationTable::Print (std::ostream &strm) const
{
  for (int ii = 0 ; ii < JpegSampleWidth ; ++ ii)
  {
    strm << endl << "        "  ;
    for (int jj = 0 ; jj < JpegSampleWidth ; ++ jj)
    {
      strm << dec
           << data_values [ii * JpegSampleWidth + jj]
           << " " ;
    }
  }
  return ;
}

} // jpeg
} // lti

