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
 * file .......: jpdecomp.cpp
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijpdecomp.cpp,v 1.7 2003/02/17 07:16:03 alvarado Exp $
 */

//
// JPEG Decoder Component Class Implementation
//


#include "ltijpdecomp.h"

namespace lti {
  namespace jpeg {
//
//  Description:
//
//    SequentialOnly
//
//    This function extends the sign bit of a decoded value.
//
//  Parameters:
//    vv: The bit value
//    tt: The length of the bit value
//

static inline int Extend (int vv, int tt)
{
  // Extend function defined in Section F.2.2.1 Figure F.12
   // The tt'th bit of vv is the sign bit. One is for
   // positive values and zero is for negative values.
   int vt = 1 << (tt - 1) ;
   if (vv < vt)
   {
      vt = (-1 << tt) + 1 ;
      return vv + vt ;
   }
   else
   {
      return vv ;
   }
}

//
//  Description:
//
//    Class default constructor
//
JpegDecoderComponent::JpegDecoderComponent ()
{
  component_id = 0 ;
  horizontal_frequency = 0 ;
  vertical_frequency = 0 ;
  v_sampling = 0 ;
  h_sampling = 0 ;
  last_dc_value = 0 ;
  ac_table = NULL ;
  dc_table = NULL ;
  quantization_table = NULL ;
  eob_run = 0 ;
  noninterleaved_rows = 0 ;
  noninterleaved_cols = 0 ;
  data_units = NULL ;
  coefficient_blocks = NULL ;
  upsample_data = NULL ;
  return ;
}

//
//  Description:
//
//    Class Destructor
//
JpegDecoderComponent::~JpegDecoderComponent ()
{
  delete [] data_units ; data_units = NULL ;
  delete [] upsample_data ; upsample_data = NULL ;
  return ;
}

//
//  Description:
//
//    This function sets the horizontal sampling frequency
//    for the component.
//
//  Parameters:
//    value: The sampling factor (1..4)
//
void JpegDecoderComponent::HorizontalFrequency (int value)
{
  if ((static_cast<int>(value) < JpegMinSamplingFrequency) ||
      (static_cast<int>(value) > JpegMaxSamplingFrequency)) {
    JpegValueOutOfRange tmpExc;
    throw tmpExc;
  }

  horizontal_frequency = value ;
  return ;
}

//
//  Description:
//
//    This function sets the vertical sampling frequency
//    for the component.
//
//  Parameters:
//    value: The sampling factor (1..4)
//
void JpegDecoderComponent::VerticalFrequency (int value)
{
  if ((static_cast<int>(value) < JpegMinSamplingFrequency) ||
      (static_cast<int>(value) > JpegMaxSamplingFrequency)) {
    JpegValueOutOfRange tmpExc;
    throw tmpExc;
  }

  vertical_frequency = value ;

  return ;
}

//
//  Description:
//
//    This function associates a quantization table with the component.
//
//  Parameters:
//    table:  The quantization table
//
void JpegDecoderComponent::SetQuantizationTable (
                                     JpegDecoderQuantizationTable &table)
{
  quantization_table = &table ;
  return ;
}

//
//  Description:
//
//    This function determines the dimensions for the component and allocates
//    the storage to hold the component's data.
//
//  Parameters:
//    decoder:  The jpeg decoder this component belongs to.
//
void JpegDecoderComponent::AllocateComponentBuffers (
                               const JpegDecoder &decoder)
{
  if (data_units == NULL)
  {
    // Determine sampling for the component. This is the amount of
    // stretching needed for the component.
    v_sampling = decoder.MaxVFrequency () / vertical_frequency ;
    h_sampling = decoder.MaxHFrequency () / horizontal_frequency ;

    // Determine the component's dimensions in a non-interleaved scan.
    noninterleaved_rows = (decoder.FrameHeight ()
                           + v_sampling * JpegSampleWidth - 1)
                          / (v_sampling * JpegSampleWidth) ;
    noninterleaved_cols = (decoder.FrameWidth ()
                           + h_sampling * JpegSampleWidth - 1)
                          / (h_sampling * JpegSampleWidth) ;

    du_rows = decoder.McuRows () * vertical_frequency ;
    du_cols = decoder.McuCols () * horizontal_frequency ;

    data_units = new JpegDecoderDataUnit [du_rows * du_cols] ;
  }

  if (decoder.IsProgressive () && coefficient_blocks == NULL)
  {
    int count = du_rows * du_cols ;
    coefficient_blocks = new JpegDecoderCoefficientBlock [count] ;
    memset (coefficient_blocks,
            0,
            sizeof (JpegDecoderCoefficientBlock) * count) ;
  }
  return ;
}

//
//  Description:
//
//    This function frees the memory allocated by the component
//    during the decompression process.
//
void JpegDecoderComponent::FreeComponentBuffers ()
{
  delete [] data_units ; data_units = NULL ;
  delete [] coefficient_blocks ; coefficient_blocks = NULL ;
  delete [] upsample_data ; upsample_data = NULL ;
  return ;
}

//
//  Description:
//
//    This function asigned Huffman tables to the component.
//
//  Parameters:
//    dc:  The DC Huffman table
//    ac:  The AC Huffman table
//
void JpegDecoderComponent::SetHuffmanTables (JpegHuffmanDecoder &dc,
                                             JpegHuffmanDecoder &ac)
{
  dc_table = &dc ;
  ac_table = &ac ;
  return ;
}

//
//  Description:
//
//    This function ensures that this component has a defined
//    AC table assigned to it. If not, it throws an exception.
//
void JpegDecoderComponent::CheckAcTable ()
{
  // If this occurs then we have a programming error.
  if (ac_table == NULL)
    throw JpegFatalError ("INTERNAL ERROR - AC Table Not Assigned") ;

  if (! ac_table->Defined ())
    throw JpegError ("AC Table Not Defined") ;

  return ;
}

//
//  Sequential and Progressive
//
//  This function is called before processing a scan. It ensures that the
//  DC Huffman table used by the component has actually been defined.
//
void JpegDecoderComponent::CheckDcTable ()
{
  if (dc_table == NULL)
    throw JpegFatalError ("INTERNAL ERROR - DC Table Not Assigned") ;

  // This condition could be caused by a corrupt JPEG stream.
  if (! dc_table->Defined ())
    throw JpegFatalError ("INTERNAL ERROR - DC Table Not Defined") ;

  return ;
}

//
//  Description:
//
//    Sequential and Progressive
//
//    This function is called before processing a scan. It ensures that the
//    Quantization table used by the component has actually been defined.
//
void JpegDecoderComponent::CheckQuantizationTable ()
{
  if (quantization_table == NULL)
    throw JpegFatalError ("INTERNAL ERROR - Quantization Table Not Assigned") ;

  if (! quantization_table->Defined ())
    throw JpegError ("Quantization Table Not Defined") ;

  return ;
}

//
//  Description:
//
//    This function decodes a data unit in a sequential scan.
//
//  Parameters:
//    decoder: The decoder that owns this component
//    mcurow, mcucol:  The row and column for this data unit.
//
void JpegDecoderComponent::DecodeSequential (JpegDecoder &decoder,
                                             int mcurow,
                                             int mcucol)
{
  JpegDecoderCoefficientBlock data ;
  memset (&data, 0, sizeof (data)) ;

  // Decode the DC differce value.
  // Section F.2.2.1
  int count ; // called T in F.2.2.1
  count = dc_table->Decode (decoder) ;
  int bits = decoder.Receive (count) ;
  int diff = Extend (bits, count) ;

  // Create the DC value from the difference and the previous DC value.
  int dc = diff + last_dc_value ;
  last_dc_value = dc ;
  data [0][0] = (lti::int16)dc ;

  // Decode the AC coefficients.
  // Section F.2.2.2 Figure F.13
  int kk;
  for (kk = 1 ; kk < JpegSampleSize ; ++ kk)
  {
    lti::uint16 rs = (lti::uint16)ac_table->Decode (decoder) ;
    lti::uint16 ssss = (lti::uint16) (rs & 0xF) ;
    lti::uint16 rrrr = (lti::uint16) (rs >> 0x4) ;

    if (ssss == 0)
    {
      // ssss is zero then rrrr should either be 15 or zero according to
      // Figure F.1. 0 means that the rest of the coefficients are zero
      // while 15 means the next 16 coefficients are zero. We are not checking
      // for other values because Figure F.13 shows values other than 15
      // as being treated as zero.
      if (rrrr  != 15)
        break ;
      kk += 15 ; // Actually 16 since one more gets added by the loop.
    }
    else
    {
      // If ssss is non-zero then rrrr gives the number of zero coefficients
      // to skip.

      kk += rrrr ;
      if (kk >= JpegSampleSize)
        throw JpegFatalError ("Value out of range") ;

      // Receive and extend the additional bits.
      // Section F2.2.2 Figure F.14
      int bits = decoder.Receive (ssss) ;
      int value = Extend (bits, ssss) ;
      (&data [0][0])[JpegZigZagInputOrder (kk)] = (lti::int16)value ;
    }
  }
  data_units [mcurow * du_cols + mcucol].InverseDCT (data,
                                                     *quantization_table) ;
  return ;
}

//
//  Description:
//
//    This function upsamples the data for the component. Here we take
//    the values from the data_units array and copy it to the
//    upsample_data. If the horizontal or vertical sampling frequencies
//    are less than the maximum for the image then we need to
//    stretch the data during the copy.
//
void JpegDecoderComponent::Upsample ()
{
  int output = 0;
  int imagesize = du_rows * v_sampling * du_cols
                              * h_sampling * JpegSampleSize ;
  if (imagesize == 0)
    return ;  // No data for this component yet.

  if (upsample_data == NULL)
    upsample_data = new JPEGSAMPLE [imagesize] ;

  // Simple case where component does not need to be upsampled.
  if (v_sampling == 1 && h_sampling == 1)
  {
    int startdu = 0 ;

    for (int durow = 0 ; durow < du_rows ; ++ durow)
    {
      int ii;
      for (ii = 0 ; ii < JpegSampleWidth ; ++ ii)
      {
        int du = startdu ;
        for (int ducol = 0 ; ducol < du_cols ; ++ ducol)
        {
      	  upsample_data [output] = data_units [du][ii][0] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][1] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][2] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][3] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][4] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][5] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][6] ; ++ output ;
      	  upsample_data [output] = data_units [du][ii][7] ; ++ output ;
          ++ du ;
      	}
      }
      startdu += du_cols ;
    }
  }
  else
  {
    output = 0 ;
    int startdu = 0 ;
    for (int durow = 0 ; durow < du_rows ; ++ durow)
    {
      for (int ii = 0 ; ii < JpegSampleWidth ; ++ ii)
      {
         for (int vv = 0 ; vv < v_sampling ; ++ vv)
         {
           int du = startdu ;
    	      for (int ducol = 0 ; ducol < du_cols ; ++ ducol)
           {
             int jj ;
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][0] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][1] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][2] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][3] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][4] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][5] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][6] ; ++ output ;
             }
             for (jj = 0 ; jj < h_sampling ; ++ jj)
             {
               upsample_data [output] = data_units [du][ii][7] ; ++ output ;
             }
             ++ du ;
           }
      	}
      }
      startdu += du_cols ;
    }
  }
  return ;
}

//
//  Description:
//
//    This static member function grayscale converts component
//    image data in the upsample_data array and writes it to the
//    the output image.  Actually for a grayscale conversion all
//    we do is copy.
//
//  Parameters:
//    cc:  The component
//    image:  The output image
//
void JpegDecoderComponent::GrayscaleConvert (JpegDecoderComponent &cc, lti::image &image)
{
  int rowstart = 0 ;
  for (int ii = 0 ; ii < image.rows () ; ++ ii)
  {
    int offset = rowstart ;
    lti::vector<lti::rgbPixel> *outrow = &image [ii] ;
    for (int jj = 0 ; jj < image.columns () ; ++ jj)
    {
//      outrow[jj] = cc.upsample_data [offset] ;
	  ubyte c = cc.upsample_data [offset];
	  (*outrow)[jj] = rgbPixel(c, c, c);
      ++ offset ;
    }
    rowstart += cc.du_cols * cc.h_sampling * JpegSampleWidth ;
  }
  return ;
}

//
//  Description:
//
//    This static member function converts the upsample_data in three
//    components from YCbCr to RGB and writes it to an image.
//
//  Parameters:
//    c1: The component containing the Y data
//    c2: Ditto for Cb
//    c3: Ditto for Cr
//    image: The output image
//
void JpegDecoderComponent::RGBConvert (JpegDecoderComponent &c1,
                                       JpegDecoderComponent &c2,
                                       JpegDecoderComponent &c3,
                                       lti::image &image)
{
  if (c1.upsample_data == NULL
      || c2.upsample_data == NULL
      || c3.upsample_data == NULL)
  {
    // If we get here then do do not yet have data for all components.
    return ;
  }

  int rowstart = 0 ;
  for (int ii = 0 ; ii < image.rows () ; ++ ii)
  {
    int offset = rowstart ;
    lti::vector<lti::rgbPixel> *outrow = &image [ii] ;
    for (int jj = 0 ; jj < image.columns () ; jj ++)
    {
      JPEGSAMPLE red = YCbCrToR (c1.upsample_data [offset],
                                 c2.upsample_data [offset],
                                 c3.upsample_data [offset]) ;
      JPEGSAMPLE green = YCbCrToG (c1.upsample_data [offset],
                                  c2.upsample_data [offset],
                                  c3.upsample_data [offset]) ;
      JPEGSAMPLE blue = YCbCrToB (c1.upsample_data [offset],
                                  c2.upsample_data [offset],
                                  c3.upsample_data [offset]) ;
	  (*outrow)[jj] = lti::rgbPixel(red, green, blue);

      ++ offset ;
    }
    rowstart += c1.du_cols * c1.h_sampling * JpegSampleWidth ;
  }
  return ;
}

//
//  Description:
//
//    Progressive Only
//
//    This function decodes the DC coefficient for a data unit in the first
//    DC scan for the component.
//
//    According to G.2 "In order to avoid repetition, detail flow diagrams
//    of progressive decoder operation are not included. Decoder operation is
//    defined by reversing the function of each stop described in the encoder
//    flow charts, and performing the steps in reverse order."
//
//  Parameters:
//    decoder:  The JPEG decoder
//    row:  The data unit row
//    col:  The data unit column
//    ssa:  Successive Approximation
//
void JpegDecoderComponent::DecodeDcFirst (JpegDecoder &decoder,
                                          int row,
                                          int col,
                                          int ssa)
{
  // We decode the first DC coeffient that same way as in a sequential
  // scan except for the point transform according to G.1.2.1

  // Section F.2.2.1
  int count ; // called T in F.2.2.1
  count = dc_table->Decode (decoder) ;
  int bits = decoder.Receive (count) ;
  int diff = Extend (bits, count) ;
  int value = diff + last_dc_value ;
  last_dc_value = value ;
  coefficient_blocks [row * du_cols + col][0][0] = (lti::int16)(value << ssa) ;
  return ;
}

//
//  Description:
//
//    Progressive Only
//
//    This function decodes the DC coefficient for a data unit in refining
//    DC scans for the component.
//
//    According to G.2 "In order to avoid repetition, detail flow diagrams
//    of progressive decoder operation are not included. Decoder operation is
//    defined by reversing the function of each stop described in the encoder
//    flow charts, and performing the steps in reverse order."
//
//  Parameters:
//    decoder:  The JPEG decoder
//    row:  The data unit row
//    col:  The data unit column
//    ssa:  Successive Approximation
//
void JpegDecoderComponent::DecodeDcRefine (JpegDecoder &decoder,
                                           int row,
                                           int col,
                                           int ssa)
{
  // Reversing G.1.2.1
  if (decoder.Receive (1) != 0)
  {
    coefficient_blocks [row * du_cols + col][0][0] |= (1 << ssa) ;
  }
  return ;
}

//
//  Description:
//
//    Progressive Only
//
//    This function decodes the AC coefficients for a data unit in the first
//    AC scans for a spectral range within the component.
//
//    According to G.2 "In order to avoid repetition, detail flow diagrams
//    of progressive decoder operation are not included. Decoder operation is
//    defined by reversing the function of each stop described in the encoder
//    flow charts, and performing the steps in reverse order."
//
//    This function comes from reversing the steps in Figures G.3-G.5.
//
//  Parameters:
//    decoder:  The JPEG decoder
//    row:  The data unit row
//    col:  The data unit column
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Successive Approximation
//
void JpegDecoderComponent::DecodeAcFirst (JpegDecoder &decoder,
                                          int row,
                                          int col,
                                          int sss,
                                          int sse,
                                          int ssa)
{
  JpegDecoderCoefficientBlock *data =
  				&coefficient_blocks [row * du_cols + col] ;
  if (eob_run > 0)
  {
    // If a previous call created a nonzero EOB run then we decrement the
    // counter and return.
    -- eob_run ;
  }
  else
  {
    for (int kk = sss ; kk <= sse ; )
    {
      // Decode the next value in the input stream.
      lti::uint16 rs = (lti::uint16)ac_table->Decode (decoder) ;
      lti::ubyte ssss = (lti::ubyte) (rs & 0xF) ;
      lti::ubyte rrrr = (lti::ubyte) (rs >> 0x4) ;

      if (ssss == 0)
      {
        if (rrrr == 15)
        {
          // A zero value ssss with rrrr == 15 means to skip
          // 16 zero coefficients.
            kk += 16 ;
        }
        else
        {
          // A zero value ssss with rrrr != 15 means to create
          // End of Band run.

          // The EOB run includes the current block. This is why we
          // do no processing for rrrr = 0 and substract one when
          // rrrrr != 0.
          if (rrrr != 0)
          {
            int bits = decoder.Receive (rrrr) ;
            eob_run = (1 << rrrr) + bits - 1 ;
          }
          break ;
        }
      }
      else
      {
        // When ssss != 0, rrrr gives the number of zero elements to skip
        // before the next non-zero coefficient.
        kk += rrrr ;
        if (static_cast<int>(kk) >= JpegSampleSize)
          throw JpegBadData ("Data out of range") ;

        // Extend the value and store.
        int bits = decoder.Receive (ssss) ;
        int value = Extend (bits, ssss) ;
        (&((*data)[0][0]))[JpegZigZagInputOrder (kk)] = (lti::uint16)(value << ssa) ;
        ++ kk ;
      }
    }
  }
  return ;
}

//
//  Description:
//
//    Progressive Only
//
//    This function decodes the AC coefficients for a data unit in the
//    refining AC scans for a spectral range within the component.
//
//    According to G.2 "In order to avoid repetition, detail flow diagrams
//    of progressive decoder operation are not included. Decoder operation is
//    defined by reversing the function of each stop described in the encoder
//    flow charts, and performing the steps in reverse order."
//
//    Section G.1.2.3 defines how to encode refining scans for AC
//    coefficients. Unfortunately this section is vague and
//    undecipherable. Reversing an undecipherable process results
//    in something unimaginable. This is a "best-guess" interpretation
//    that seems to work.
//
//    The basic process at work is that zero counts do not include nonzero
//    values. Whenever we skip a value due to zero count or End of Band runs
//    we have to read one bit to refine each non-zero value we skip. The
//    process is ugly and it means that data is encoding out of order.
//
//  Parameters:
//    decoder:  The JPEG decoder
//    row:  The data unit row
//    col:  The data unit column
//    sss:  Spectral Selection Start
//    sse:  Spectral Selection End
//    ssa:  Successive Approximation
//                                                            4
void JpegDecoderComponent::DecodeAcRefine (JpegDecoder &decoder,
                                           int row,
                                           int col,
                                           int sss,
                                           int sse,
                                           int ssa)
{
  JpegDecoderCoefficientBlock *data =
  				&coefficient_blocks [row * du_cols + col] ;
  // kk is incremented within the loop.
  for (int kk = sss ; kk <= sse ;)
  {
    if (eob_run != 0)
    {
      // An EOB run has caused us to skip entire data units. We need
      // to refine any previously non-zero coefficients.
      // Notice that we do not initialize kk here. We could be using
      // an EOB run to skip all the remaining coefficients in the current
      // one.

      for ( ; kk <= sse ; ++ kk)
      {
        if ((&((*data)[0][0]))[JpegZigZagInputOrder (kk)] != 0)
        {
          decoder.RefineAcCoefficient (
                      (&((*data)[0][0]))[JpegZigZagInputOrder (kk)],
                      ssa) ;
        }
      }
      -- eob_run ;
    }
    else
    {
      lti::uint16 rs = (lti::uint16)ac_table->Decode (decoder) ;
      lti::ubyte ssss = (lti::ubyte) (rs & 0xF) ;
      lti::ubyte rrrr = (lti::ubyte) (rs >> 0x4) ;

      if (ssss == 0)
      {
        // ssss == 0 means that we either have an EOB run or we need to
        // 16 non-zero coefficients.

        if (rrrr == 15)
        {
          // ssss == 0 and rrrr == 15 => Skip over 16 zero coefficients
          for (int ii = 0 ;
              kk <= sse  && ii < 16 ;
              ++ kk)
          {
            if (kk >  sse)
              throw JpegBadData ("Corrupt Scan Data") ;

            if ((&((*data)[0][0]))[JpegZigZagInputOrder (kk)] != 0)
            {
              decoder.RefineAcCoefficient (
                            (&((*data)[0][0]))[JpegZigZagInputOrder (kk)],
                            ssa) ;
            }
            else
            {
              ++ ii ;
            }
          }
        }
        else
        {
          // We are reading an EOB run.
          if (rrrr == 0)
          {
            eob_run = 1 ;
          }
          else
          {
            int bits = decoder.Receive (rrrr) ;
            eob_run = (1 << rrrr) + bits ;
          }
        }
      }
      else if (ssss == 1)
      {
        // ssss == 1 means that we are creating a new non-zero
        // coefficient. rrrr gives the number of zero coefficients to
        // skip before we reach this one.
        // Save the value for the new coefficient. Unfortunately the data
        // is stored out of order.
        int newvalue = decoder.Receive (1) ;

        // Skip the zero coefficients.
        for (int zerocount = 0 ;
             (static_cast<int>(kk) <  JpegSampleSize) &&
               ((zerocount < rrrr) ||
               (&((*data)[0][0]))[JpegZigZagInputOrder (kk)] != 0) ;
             ++ kk)
        {
          if (kk >  sse)
            throw JpegBadData ("Error in progressive scan") ;

          if ((&((*data)[0][0]))[JpegZigZagInputOrder (kk)] != 0)
          {
            decoder.RefineAcCoefficient ((&((*data)[0][0]))[JpegZigZagInputOrder (kk)],
                                 ssa) ;
          }
          else
          {
            ++ zerocount ;
          }
        }

        if (kk >  sse)
          throw JpegBadData ("Error in progressive scan") ;

        if (newvalue)
        {
          (&((*data)[0][0]))[JpegZigZagInputOrder (kk)] = (lti::int16)(1 << ssa) ;
        }
        else
        {
          (&((*data)[0][0]))[JpegZigZagInputOrder (kk)] = (lti::int16)(-1 << ssa) ;
        }
        ++ kk ;
      }
      else
      {
        // The value of SSSS must be zero or one. Since we add data one
        // bit at at a time data values larger than 1 make no sense.
        throw JpegBadData ("Invalid value in input stream") ;
      }
    }
  }
  return ;
}

//
//  Description:
//
//    Progressive Only
//
//    This function performs the IDCT on all the data in
//    coefficient_blocks and stores the result in data_units.
//
//    This function gets called whenever the image data is written
//    to the image.  For a sequential image the IDCT only needs to
//    performed once no matter how often the data gets updated in the
//    image but to continuously update the progressive image data
//    an update after each scan gives new results.
//
void JpegDecoderComponent::ProgressiveInverseDct ()
{
  // If UpdateImage gets called before the image is completely
  // decoded the these values may be NULL.
  if (data_units == NULL || coefficient_blocks == NULL)
    return ;

  int limit = du_cols * du_rows ;
  for (int ii = 0 ; ii < limit ; ++ ii)
  {
    data_units [ii].InverseDCT (coefficient_blocks [ii], *quantization_table) ;
  }
  return ;
}

}
}

