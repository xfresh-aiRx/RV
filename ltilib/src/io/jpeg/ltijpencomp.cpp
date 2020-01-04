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
 * file .......: jpgencomp.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpencomp.cpp,v 1.8 2003/02/17 07:16:03 alvarado Exp $
 */


//
// JPEG Encoder Library.
//
// Title:   JpegEncoderComponent Class Implementation
//
//
//

// supress MSVC++ "debug message truncation warning"
#include "ltiObject.h"

#include "ltijpencomp.h"
#include "ltijpegenco.h"


namespace lti {
  namespace jpeg {

//
//  Description:
//
//    Class Default Constructor
//

JpegEncoderComponent::JpegEncoderComponent ()
{

  jpeg_encoder = NULL ;
  dct_coefficients = NULL ;

  du_rows = 0 ;
  du_cols = 0 ;

  eob_run = 0 ;
  eob_start_du_row = 0 ;
  eob_start_du_col = 0 ;
  eob_start_position = 0 ;

  v_frequency = 1 ;
  h_frequency = 1 ;

  ac_table = NULL ;
  dc_table = NULL ;

  return ;
}

//
//  Description:
//
//    Class Destructor
//
JpegEncoderComponent::~JpegEncoderComponent ()
{
  FreeDynamicStorage () ;
  return ;
}

//
//  Description:
//
//    This function gets rid of all the memory allocated by the component.
//    This allows the encoder to free memory after each image is written
//    rather than waiting until the decoder is deleted.
//
void JpegEncoderComponent::FreeDynamicStorage ()
{
  delete [] dct_coefficients ; dct_coefficients = NULL ;
  return ;
}

//
//  Description:
//
//    This function allocates the buffer used to hold DCT coefficients.
//
//  Parameters:
//    image: The image being output
//    maxhf: The maximum horizontal frequency for all components.
//    maxvf: The maximum vertical frequency for all components.
//
void JpegEncoderComponent::CalculateDuDimensions (const lti::image &image,
                                                  int maxhf,
                                                  int maxvf)
{
//!  du_cols = (image.Width () * h_frequency + (JpegSampleWidth * maxhf - 1))
  du_cols = (image.columns () * h_frequency + (JpegSampleWidth * maxhf - 1))
          / (JpegSampleWidth * maxhf) ;
//!  du_rows = (image.Height () * v_frequency + (JpegSampleWidth * maxvf - 1))
  du_rows = (image.rows () * v_frequency + (JpegSampleWidth * maxvf - 1))
          / (JpegSampleWidth * maxvf) ;

  v_period = maxvf / v_frequency ;
  h_period = maxhf / h_frequency ;

  dct_coefficients = new JpegEncoderCoefficientBlock [du_cols * du_rows] ;
  return ;
}

//
//  Description:
//
//    This function samples a component where the horizontal and
//    vertical sampling frequencies are equal to the maximum values
//    for all components.
//
//  Parameters:
//    image:  The image to sample
//    function:  A color conversion function
//

//! void JpegEncoderComponent::Sample1to1Component (const BitmapImage &image, RGBTOYCBCRFUNCTION function)
void JpegEncoderComponent::Sample1to1Component (const lti::image &image,
                                                RGBTOYCBCRFUNCTION function)
{
//!  lti::ubyte red ;
//!  lti::ubyte green ;
//!  lti::ubyte blue ;
  lti::rgbPixel rgb;

  JpegEncoderDataUnit data ;

  int index = 0 ;
  for (int ii = 0 ; ii < du_rows ; ++ ii)
  {
    for (int jj = 0 ; jj < du_cols ; ++ jj, ++ index)
    {
      int row = ii * JpegSampleWidth ;
      for (int dr = 0 ; dr < JpegSampleWidth ; ++ dr, ++ row)
      {
        int col = jj * JpegSampleWidth ;
        for (int dc = 0 ; dc < JpegSampleWidth ; ++ dc, ++ col)
        {
          if (row < image.rows ())
          {
             if (col < image.columns ())
             {
//!           	  image.GetRGB (row, col, red, green, blue) ;
//!           	  data [dr][dc] = function (red, green, blue) ;
              rgb = image.at(row, col);
           	  data [dr][dc] = function (rgb.getRed(), rgb.getGreen(), rgb.getBlue()) ;
             }
             else if (dc != 0)
             {
               // Extend the rightmost column
               data [dr][dc] = data [dr][dc-1] ;
             }
             else
             {
               // This data unit is completely off the edge of the image.
               data [dr][dc] = JpegMidpointSampleValue ;
             }
          }
          else if (dr != 0)
          {
            // Extend the last row of the image.
            data [dr][dc] = data [dr-1][dc] ;
          }
          else
          {
            // This data unit is completely off the edge of the image.
            data [dr][dc] = JpegMidpointSampleValue ;
          }
        } // Data Unit Columns
      } // Data Unit Rows

      data.ForwardDct (*quantization_table, dct_coefficients [index]) ;
    } // Columns of Data Units
  } // Rows of Data Units
  return ;
}

//
//  Description:
//
//    This function downsamples a component where the horizontal and
//    vertical sampling frequencies are not equal to the maximum values
//    for all components.
//
//  Parameters:
//    image:  The image to sample
//    function:  A color conversion function
//
void JpegEncoderComponent::SampleNtoNComponent (const lti::image &image,
                                                RGBTOYCBCRFUNCTION function)
{
  JpegEncoderDataUnit data ;

  // Sample the image.
  int index = 0 ;
  int imagerow = 0 ;
  for (int yy = 0 ;
       yy < du_rows ;
       ++ yy, imagerow += JpegSampleWidth * v_period)
  {
    int imagecol = 0 ;
    for (int xx = 0 ;
         xx < du_cols ;
         ++ xx, ++ index, imagecol += JpegSampleWidth * h_period)
    {
      // Loop for each row in the data unit.
      for (int outrow = 0 ; outrow < JpegSampleWidth  ; ++ outrow)
      {
        // Loop for each column in the data unit.
        for (int outcol = 0 ; outcol < JpegSampleWidth ; ++ outcol)
        {
          // The nested loops handle sampling.

          int value = 0 ;   // Sum of all sample values
          int count = 0 ;  // Number of sample values.
          // Vertical sampling loop.
          for (int srcrow = 0 ; srcrow < v_period ; ++ srcrow)
          {
            // Horizontal sampling loop.
            int row = imagerow + srcrow + outrow * v_period ;
            for (int srccol = 0 ; srccol < h_period ; ++ srccol)
            {
              // Extract the values from the image.
              int col = imagecol + srccol + outcol * h_period ;
              if (row < image.rows () && col < image.columns ())
              {
                ++ count ;
                lti::rgbPixel rgb ;
                rgb = image.at (row, col) ;
                value += function (rgb.getRed(), rgb.getGreen(), rgb.getBlue()) ;
              }
            }
          }
          // Convert the total sample value to an average.
          if (count != 0)
          {
			data [outrow][outcol] = (JpegEncoderDataUnit::DATAUNITVALUE)((value + count - 1) / count) ;
          }
          else
          {
            // If we get here that means the data unit goes past the edge of
            // the input image. We attempt to duplicate the last column and if
            // we can't to that we copy the last row. If the entire data unit
            // is off the edge then we use zero for the value.
            if (outcol != 0)
              data [outrow][outcol] = data [outrow][outcol - 1] ;
            else if (outrow != 0)
              data [outrow][outcol] = data [outrow-1][outcol] ;
            else
              data [outrow][outcol] = 0 ;
          }
        }
      }

      data.ForwardDct (*quantization_table, dct_coefficients [index]) ;
    }
  }
  return ;
}

//
//  Description:
//
//    This function samples the Y component for the image.
//
//    What we do here is determine how much memory is required
//    to hold the image, what color conversion function to use
//    for the component, and if downsampling is required.
//
//  Parameters:
//    encoder:  The JPEG encoder that owns the component
//    image:  The image to be sampled
//    maxhf:  The maximum horizontal frequency of all components
//    maxvf:  The maximum vertical frequency of all components
//
void JpegEncoderComponent::SampleYComponent (JpegEncoder &encoder,
                                             const lti::image &image,
                                             int maxhf,
                                             int maxvf)
{
  jpeg_encoder = &encoder ;
  CalculateDuDimensions (image, maxhf, maxvf) ;

  if (maxhf == h_frequency && maxvf == v_frequency)
  {
    Sample1to1Component (image, RGBToY) ;
  }
  else
  {
    SampleNtoNComponent (image, RGBToY) ;
  }

  return ;
}

//
//  Description:
//
//    This function samples the Cb component for the image.
//
//    What we do here is determine how much memory is required
//    to hold the image, what color conversion function to use
//    for the component, and if downsampling is required.
//
//  Parameters:
//    encoder:  The JPEG encoder that owns the component
//    image:  The image to be sampled
//    maxhf:  The maximum horizontal frequency of all components
//    maxvf:  The maximum vertical frequency of all components
//
void JpegEncoderComponent::SampleCbComponent (JpegEncoder &encoder,
                                              const lti::image &image,
                                              int maxhf,
                                              int maxvf)
{
  jpeg_encoder = &encoder ;
  CalculateDuDimensions (image, maxhf, maxvf) ;

  if (maxhf == h_frequency && maxvf == v_frequency)
  {
    Sample1to1Component (image, RGBToCb) ;
  }
  else
  {
    SampleNtoNComponent (image, RGBToCb) ;
  }

  return ;
}

//
//  Description:
//
//    This function samples the Cr component for the image.
//
//    What we do here is determine how much memory is required
//    to hold the image, what color conversion function to use
//    for the component, and if downsampling is required.
//
//  Parameters:
//    encoder:  The JPEG encoder that owns the component
//    image:  The image to be sampled
//    maxhf:  The maximum horizontal frequency of all components
//    maxvf:  The maximum vertical frequency of all components
//

void JpegEncoderComponent::SampleCrComponent (JpegEncoder &encoder,
                                              const lti::image &image,
                                              int maxhf,
                                              int maxvf)
{
  jpeg_encoder = &encoder ;
  CalculateDuDimensions (image, maxhf, maxvf) ;

  if (maxhf == h_frequency && maxvf == v_frequency)
  {
    Sample1to1Component (image, RGBToCr) ;
  }
  else
  {
    SampleNtoNComponent (image, RGBToCr) ;
  }

  return ;
}

//
//  Description:
//
//    This function is use to gather Huffman statistics for DC coefficients.
//    The calling sequences is idential to PrintDcData (). Only the huffvalue
//    parameter is used here.
//
//  Parameters:
//    huffvalue:  The Huffman value (not code) to process.
//
void JpegEncoderComponent::GatherDcData (int huffvalue, int)
{
  dc_table->IncrementFrequency (huffvalue) ;
  return ;
}

//
//  Description:
//
//    This function is use to gather Huffman statistics for AC coefficients.
//    The calling sequences is idential to PrintAcData (). Only the huffvalue
//    parameter is used here.
//
//    If huffval==-1 then an unencoded bit string would be output by
//    PrintAcData (). In that situation this function does nothing.
//
//  Parameters:
//    huffvalue:  The Huffman value (not code) to process
//
void JpegEncoderComponent::GatherAcData (int huffvalue, int, int)
{
  if (huffvalue >= 0)
  {
    ac_table->IncrementFrequency (huffvalue) ;
  }
  return ;
}

//
//  Description:
//
//    This function is use to output Huffman-encoded data for a DC value.
//
//  Parameters:
//    huffvalue:  The Huffman value
//    bits: The additional data bits
//
//    8-bit DC values are in the range 0..11. The value specifies the number
//    of additional bits of refining data (bits).
//

void JpegEncoderComponent::PrintDcData (int huffvalue, int bits)
{
  lti::uint16 huffcode ;
  lti::ubyte huffsize ;

  // Section F.1.2.1
  dc_table->Encode (huffvalue, huffcode, huffsize) ;
  jpeg_encoder->OutputBits (huffcode, huffsize) ;
  if (huffvalue != 0)
    jpeg_encoder->OutputBits (bits, huffvalue) ;
  return ;
}

//
//  Description:
//
//    This function is use to output Huffman-encoded data for an AC value.
//
//  Parameters:
//    huffvalue:  The Huffman value
//    value: The additional data bits
//    size: The number of additional data bits.
//
//    When huffvalue==-1 this function is also used to output unencoded bit
//    strings.
//
void JpegEncoderComponent::PrintAcData (int huffvalue, int value, int size)
{
  lti::uint16 huffcode ;
  lti::ubyte huffsize ;

  // Section F.1.2.2.1
  if (huffvalue >= 0)
  {
    ac_table->Encode (huffvalue, huffcode, huffsize) ;
    jpeg_encoder->OutputBits (huffcode, huffsize) ;
  }
  if (size != 0)
    jpeg_encoder->OutputBits (value, size) ;
  return ;
}
//
//  Description:
//
//    This function is used for two purposes in a sequential scan:
//
//      o To gather statistics for generating Huffman Tables
//      o To encode and output a data unit.
//
//    The dcfunction and acfunction arguments are determine which of these
//    actions are performed. If these arguments are PrintDcData () and
//    PrintAcData () the data unit is encoded and written to the output
//    stream.  If the arguments are GatherDcData () and GatherAcData ()
//    usage statistics are gathered.
//
//    While creating a separate function for each purpose may have been clearer,
//    it would create maintenance problems because they would have to be kept
//    in strict synchronization with each other.
//
//  Parameters:
//    row,col: Data unit position
//    dcfunction: Function for outputting DC coefficient values.
//    acfunction: Function for outputting AC coefficient values.
//
//    This function is of the type COMPONENTPASSFUNCTION.
//
void JpegEncoderComponent::EncodeSequential (
                            int row, int col,
                            DCOUTPUTFUNCTION dcfunction,
                            ACOUTPUTFUNCTION acfunction,
                            int, int, int)
{
  JpegEncoderCoefficientBlock *du = &dct_coefficients [row * du_cols + col] ;

  // DC value calculation
  // Section F.1.2.1.3
  int diff = (*du) [0][0] - last_dc_value ;
  last_dc_value = (*du)[0][0] ;

  // Break the DC coefficient into a category (Table F.12) and
  // additional bits.
  int bits ;
  if (diff >= 0)
  {
    bits = diff ;
  }
  else
  {
    diff = -diff ;
    bits = ~diff ;
  }
  int ssss = 0 ;  // Category
  while (diff != 0)
  {
    ++ ssss ;
    diff >>= 1 ;
  }
  (this->*dcfunction) (ssss, bits) ;

  // AC coefficient coding
  // F.1.2.2.3 Figure F.2
  int zerorun = 0 ; // Called rrrr in the specification.
  for (int index = 1 ; index < JpegSampleSize ; ++ index)
  {
    if (du->ZigZagInput (index) != 0)
    {
      // 16 is the longest run of zeros that can be encoded except for the
      // final EOB code.
      while (zerorun >= 16)
      {
        // 0xF0 is the code to skip 16 zeros (Figure F.1)
        (this->*acfunction) (0xF0, 0, 0) ;
        zerorun -= 16 ;
      }

      // Non-zero AC coefficients are encoded with
      // 8-bit Huffman-encoded values of the form rrrrssss followed by
      // 1..10 additional bits of data. rrrr is the number of zero values
      // to skip (0..15). ssss is the category (1..10) which specifies the
      // number of additional raw bits to follow. (Figure F.1)
      int value = du->ZigZagInput (index) ;
      int bits ;
      if (value >= 0)
      {
        bits = value ;
      }
      else
      {
        value = -value ;
        bits = ~value ;
      }
      int ssss = 0 ;
      while (value != 0)
      {
        value >>= 1 ;
        ++ ssss ;
      }

      int rrrrssss = (zerorun << 4) | ssss ;
      (this->*acfunction) (rrrrssss, bits, ssss) ;
      zerorun = 0 ;
    }
    else
    {
      ++ zerorun ;
    }
  }
  // The code 0x00 indicates all remaining AC coefficients are zero.
  if (zerorun > 0)
  {
    (this->*acfunction) (0, 0, 0) ;
  }

  return ;
}

//
//  Description:
//
//    This function outputs an End of Band run in an initial AC coefficient
//    scan of a progressive frame.
//
//  Parameters:
//    acfunction: Function for outputting AC coefficient values
//
void JpegEncoderComponent::PrintEobRun (ACOUTPUTFUNCTION acfunction)
{
  // Figure G.4
  if (eob_run != 0)
  {
    int bits = eob_run ;
    int value = bits >> 1 ;
    int ssss = 0 ; // Category (Table G.1)
    while (value != 0)
    {
      value >>= 1 ;
      ++ ssss ;
    }
    (this->*acfunction)(ssss << 4, bits, ssss) ;
    eob_run = 0 ;
  }

  return ;
}

//
//  Description:
//
//    This function is use to output a data unit for the first pass
//    DC progressive scan. The DC coefficients are encoded in the same manner
//    as in a sequential scan except for the point transform.
//
//    This function gets called twice for each data unit in the scan. The
//    first pass is used to gather Huffman statistics and the second is
//    used to Huffman-encode the data and write it to the output stream.
//    We use pointers to the statistics/output functions to ensure that
//    both passes are performed in the exact same manner.
//
//  Parameters:
//    row,col: Data unit position
//    dcfunction: Function for outputting DC coefficient values.
//    successiveapproximation: Successive Approximation
//
//    This function is of the type COMPONENTPASSFUNCTION.
//
void JpegEncoderComponent::ProgressiveDcFirst (
                          int row, int col,
                          DCOUTPUTFUNCTION dcfunction, ACOUTPUTFUNCTION,
                          int, int,
                          int successiveapproximation)
{
  // G.1.2.1

  // DC value calculation
  // A.4
  int value = dct_coefficients [row * du_cols + col][0][0]
            >> successiveapproximation ;

  // Section F.1.2
  int diff = value - last_dc_value ;
  last_dc_value = value ;

  // Break the difference into a category for Huffman coding and additional
  // raw bits for refinement.
  int bits ;
  if (diff >= 0)
  {
    bits = diff ;
  }
  else
  {
    diff = -diff ;
    bits = ~diff ;
  }
  int ssss = 0 ;  // Category
  while (diff != 0)
  {
    ++ ssss ;
    diff >>= 1 ;
  }
  (this->*dcfunction) (ssss, bits) ;

  return ;
}

//
//  Description:
//
//    This function outputs DC coefficient data for refining scans in a
//    progressive frame. This is the only thing simple about progressive
//    JPEG. In this scan we simply encode an additional bit for each
//    DC coefficient.
//
//    Since there is no Huffman coding for this refining DC scans this function
//    only gets called once per data unit in a scan. Therefore we do not
//    use the output function pararmeters.
//
//  Parameters:
//    row,col: Data unit position
//    ssa:  Successive Approximation
//
//    This function is of the type COMPONENTPASSFUNCTION.
//
void JpegEncoderComponent::ProgressiveDcRefine (
                            int row, int col,
                            DCOUTPUTFUNCTION, ACOUTPUTFUNCTION,
                            int, int,
                            int ssa)
{
  // Section G.1.2.1
  int value = (dct_coefficients [row * du_cols + col][0][0] >> ssa) & 0x1 ;
  jpeg_encoder->OutputBits (value, 1) ;
  return ;
}

//
//  Description:
//
//    This function encodes a data unit for the first AC coefficient scan
//    for a spectral range in a progressive frame. The procedure here
//    is taken literally from the JPEG specification.
//
//    The AC encoding complexity is significantly increased over that of
//    sequential scans because End of Bands can span data units.
//
//  Parameters:
//    row,col: Data unit position
//    acfunction: Function for outputting AC coefficient values
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Successive Approximation
//
void JpegEncoderComponent::ProgressiveAcFirst (
                            int row, int col,
                            ACOUTPUTFUNCTION acfunction,
                            int sss, int sse,
                            int ssa)
{
  JpegEncoderCoefficientBlock *du = &dct_coefficients [row * du_cols + col] ;

  // G.1.2.2 Figure G.3
  int zerorun = 0 ;
  for (int ii = sss ; ii <= sse ; ++ ii)
  {
    int value = du->ZigZagInput (ii) ;
    // Point Transform
    value = value / (1 << ssa) ;

    if (value == 0)
    {
      ++ zerorun ;
    }
    else
    {
      PrintEobRun (acfunction) ;
      // Figure G.5
      while (zerorun >= 16)
      {
        (this->*acfunction)(0xF0, 0, 0) ;
        zerorun -= 16 ;
      }

      int bits ;
      if (value >= 0)
      {
        bits = value ;
      }
      else
      {
        value = -value ;
        bits = ~value ;
      }
      int ssss = 0 ;
      while (value != 0)
      {
        ++ ssss ;
        value >>= 1 ;
      }
      int rrrrssss = (zerorun << 4) | ssss ;
      (this->*acfunction) (rrrrssss, bits, ssss) ;
      zerorun = 0 ;

      if (ii >= sse)
        return ;
    }
  }
  ++ eob_run ;

  // Do not allow the EOB run to exceed 0x7FFF.
  // G.1.2.2
  if (eob_run == 0x7FFF)
    PrintEobRun (acfunction) ;
  return ;
}

//
//  Description:
//
//    This function encodes the AC coefficients for a refining scan in a
//    progressive frame.
//
//    The JPEG standard is nebulous here (Section G.1.2.3). It is
//    unfortunate that for such a brain-damaged encoding method as
//    this that they should be unclear. In addition to the complexity
//    of having EOB runs span data units, data does not get written
//    out in the order it occurs.
//
//    This is why there are no section references in the code other than
//    the one above. I am simply guessing here, sorry. I created this code by
//    guessing and running the output through various applications that handle
//    progressive JPEG until I could find no complaints.
//
//    If you thing this is bad wait until you get to the part about decoding
//    progressive scans (G.2)!
//
//  Parameters:
//    row,col: Data unit position
//    acfunction: Function for outputting AC coefficient values
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Successive Approximation
//
void JpegEncoderComponent::ProgressiveAcRefine (
                            int row, int col,
                            ACOUTPUTFUNCTION acfunction,
                            int sss, int sse,
                            int ssa)
{
  JpegEncoderCoefficientBlock *du = &dct_coefficients [row * du_cols + col] ;
  // Number of zero coefficients - Note that existing non-zero coefficients
  // are not included in this count.
  int zerorun = 0 ;
  // The start of the zero run.
  int zerostart = sss ;
  // Number of existing non-zero coefficients - used only for error checking.
  int correctioncount = 0 ;

  for (int ii = sss ; ii <= sse ; ++ ii)
  {
    // Point Transform
    int value = du->ZigZagInput (ii) / (1 << ssa) ;

    // We have three types of values:
    //  o A Zero
    //  o A coefficient that was zero in all previous scan that we are
    //    going to make non-zero in this scan (value = +/-1)
    //  o An coefficient that was made non-zero in a previous scan
    //      (value > 1 OR value < -1)
    if (value == 0)
    {
      ++ zerorun ;
    }
    else if (value == 1 || value == -1)
    {
      // If way have an EOB run then print it out.
      PrintRefineEobRun (acfunction, sss, sse, ssa) ;

      // The longest zero run we can have is 16.
      while (zerorun >= 16)
      {
        (this->*acfunction)(0xF0, 0, 0) ;
        zerorun -= 16 ;

        // Refine all the existing coefficients skipped by the zero run.
        for (int zerocount = 0 ;
             zerocount < 16 ;
             ++ zerostart)
        {
          if (zerostart < sss || zerostart > sse
              || correctioncount > JpegSampleSize)
          {
            throw JpegFatalError ("INTERNAL ERROR - Invalid Zero Run") ;

          }

          int oldvalue = du->ZigZagInput (zerostart) / (1 << ssa) ;
          if (oldvalue < 0)
            oldvalue = - oldvalue ;
          if (oldvalue > 1)
          {
            (this->*acfunction)(-1, (oldvalue & 0x1), 1) ;
            -- correctioncount ;
          }
          else if (oldvalue == 0)
          {
            // Because we need to count only zero values we our loop counter
            // gets incremented here.
            ++ zerocount ;
          }
          else
          {
            // If the value is +/- 1 we should have already processed it.
            throw JpegFatalError ("INTERNAL ERROR - Bad Value") ;
          }
        }
      }

      // This is the first time this value has been nonzero.
      int output = (zerorun << 0x4) | 1 ;
      if (value > 0)
        (this->*acfunction)(output, 1, 1) ;
      else
        (this->*acfunction)(output, 0, 1) ;
      zerorun = 0 ;

      // No go back and refine all the previously non-zero coefficients
      // skipped by this zero run.
      for (int jj = zerostart ; jj < ii ; ++ jj)
      {
        int oldvalue = du->ZigZagInput (jj) / (1 << ssa) ;
        if (oldvalue < 0)
          oldvalue = - oldvalue ;
        if (oldvalue > 1)
        {
          (this->*acfunction)(-1, (oldvalue & 0x1), 1) ;
          -- correctioncount ;
        }
      }
      zerostart = ii + 1 ;
      if (ii == sse)
        return ;  // All finished with this data unit.
    }
    else
    {
      // We only use this counter for error checking. It contains the
      // number of previously non-zero coefficients that we have skipped
      /// as part of a zero run.
      ++ correctioncount ;
    }
  }
  // If we get here then the data unit ends with a string of zero coefficients
  // or previously non-zero coefficients that we are skipping.
  if (eob_run == 0)
  {
    // We are beginning and End of Band run. Mark the starting position
    // including the spectral position.
    eob_start_du_row = row ;
    eob_start_du_col = col ;
    eob_start_position = zerostart ;
  }
  ++ eob_run ;
  // G.1.2.2
  if (eob_run == 0x7FFF)
    PrintRefineEobRun (acfunction, sss, sse, ssa) ;
  return ;
}

//
//  Description:
//
//    This function Resets the End of Band run counters. It should be called
//    before beginning to output a scan.
//
void JpegEncoderComponent::ResetEobRun ()
{
  eob_run = 0 ;

  // We use values here that should make an error easier to detect.
  eob_start_du_row = du_cols * du_rows ;
  eob_start_du_col = du_cols * du_rows ;
  eob_start_position = JpegSampleSize ;

  return ;
}

//
//  Description:
//
//    This function outputs an End of Band run in a refining AC ceofficient
//    scan of a progressive frame. As for the rest of the refining AC scan
//    data I am mostly guessing here. I have taken the process from
//    Figure G.4 and the vague description in Section G.1.2.3. It seems to
//    work.
//
//  Parameters:
//    acfunction: Function for outputting AC coefficient values
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Successive Approximation
//
void JpegEncoderComponent::PrintRefineEobRun (ACOUTPUTFUNCTION acfunction,
                            int sss, int sse,
                            int ssa)
{
  if (eob_run != 0)
  {
    int bits = eob_run ;
    int value = bits >> 1 ;
    int ssss = 0 ; // Category (Table G.1)
    while (value != 0)
    {
      value >>= 1 ;
      ++ ssss ;
    }
    (this->*acfunction)(ssss << 4, bits, ssss) ;

    // Now that we have output the EOB run we need to refine each coefficient
    // that we skipped that had previously been non-zero.
    int  eobcounter = 0 ;
    int row = eob_start_du_row ;
    int col = eob_start_du_col ;
    while (eobcounter < eob_run)
    {
      JpegEncoderCoefficientBlock *du
        = &dct_coefficients [row * du_cols + col] ;
      for (int kk = eob_start_position ;
           kk <= sse ;
           ++ kk)
      {
        int value = du->ZigZagInput (kk) / (1<< ssa) ;
        if (value < 0)
          value = - value ;
        if (value > 1)
        {
          (this->*acfunction)(-1, (value & 0x1), 1) ;
        }
      }
      // Except for the first data unit we go to the first spectral value
      // in the scan.
      eob_start_position = sss ;

      ++ eobcounter ;
      ++ col ;
      if (col == du_cols)
      {
        col = 0 ;
        ++ row ;
        if (row >= du_cols && eobcounter != eob_run)
          throw JpegFatalError ("INTERNAL ERROR - EOB run extends past last row") ;
      }
    }

    // Some values that are certain to cause errors if improperly used.
    eob_start_position = JpegSampleSize ;
    eob_start_du_row = du_cols * du_rows ;
    eob_start_du_col = du_cols * du_rows ;

    eob_run = 0 ; // Reset the counter.
  }

  return ;
}

//
//  Description
//
//    This function sets the horizontal sampling frequency for the
//    component.
//
//  Parameters:
//    value:  The new sampling frequency.
//
void JpegEncoderComponent::SetHorizontalFrequency (int value)
{
  if (value > JpegMaxSamplingFrequency || value < JpegMinSamplingFrequency) {
    JpegValueOutOfRange tmpExc;
      throw tmpExc;
  }

  h_frequency = value ;
    return ;
}

//
//  Description
//
//    This function sets the vertical sampling frequency for the
//    component.
//
//  Parameters:
//    value:  The new sampling frequency.
//
void JpegEncoderComponent::SetVerticalFrequency (int value)
{
  if (value > JpegMaxSamplingFrequency || value < JpegMinSamplingFrequency) {
    JpegValueOutOfRange tmpExc;
    throw tmpExc;
  }

  v_frequency = value ;
  return ;
}
}
}

