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
 * file .......: ltipngdecoder.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 26.10.99
 * description.: PNG deooder class definition
 * revisions ..: $Id: ltipngdecoder.cpp,v 1.8 2003/02/17 07:16:13 alvarado Exp $
 */

#include <iomanip>
#include <fstream>
#include <climits>
#include <iostream>
#include <string>

#include "ltipngadler32.h"
#include "ltipngdecoder.h"
//#include "ltipngpvt.h"
#include "ltipngsystemspecific.h"
#include "ltipnginputstream.h"
#include "ltipngexcept.h"

namespace lti{

  namespace png {

/// constants for pointing out bytes in colorbytestreams
const byte PngDecoder::REDOFFSET = 2;
const byte PngDecoder::GREENOFFSET= 1;
const byte PngDecoder::BLUEOFFSET=0;


// These values define the Adam7 interlace pattern for each pass.
const PngDecoder::InterlaceInfo PngDecoder::interlace_values [InterlacePasses] =
  {
    { 8, 8, 0, 0, },
    { 8, 8, 0, 4, },
    { 8, 4, 4, 0, },
    { 4, 4, 0, 2, },
    { 4, 2, 2, 0, },
    { 2, 2, 0, 1, },
    { 2, 1, 1, 0, },
  };


//
// Class Constructor
//
// Nothing gets done here except initializing variables to a known state.
//
PngDecoder::PngDecoder(): background_red (0),
                          background_green (0),
                          background_blue (0),
                          background_gray (0) {
  row_buffers [0] = 0;
  row_buffers [1] = 0;
  return;
}

//
//  Description:
//
//  Class Destructor
//
PngDecoder::~PngDecoder () {
  freeData();
  return;
}

//
// Description:
//
// This function frees all of the dynamically allocated data.
//
void PngDecoder::freeData (){
  delete[] row_buffers[0];
  row_buffers[0] = 0;

  delete[] row_buffers[1];
  row_buffers [1] = 0;

  return;
}

//
//  Description:
//
//  This function reads a PNG image.
//
//  Parameters:
//
//  strm:  The input stream
//  image:  The output image
//
void PngDecoder::readImage (PngInputStream &inputstream, image& theimage) {
  try  {
    reading_pixel_data = false;
    data_read = false;
    header_read = false;
    end_read = false;
    palette_read = false;

    palette_size = 0;

    // At the start of the image we should find
    // 1. PNG Signature
    // 2. IHDR Chunk
    // After that almost anything goes.

    readSignature (inputstream);

    inputstream.getNextChunk ();
    if (! matchChunks (inputstream.getChunkType (), "IHDR")) {
      throw BadPngStream ("Missing IHDR Chunk");
    }

    processChunk (inputstream, theimage);

    // Now chunks can come in almost any order.
    while (inputstream.getNextChunk () && ! end_read) {
      processChunk (inputstream, theimage);
    }

    freeData ();
  }
  catch (...) {
    freeData ();
    throw;
  }

  if (! data_read) {
    throw BadPngStream ("Image Contains no Pixel Data");
  }
  return;
}

void PngDecoder::readImage (PngInputStream &inputstream,
                            channel8& theChannel,
                            lti::palette& colors) {
  try  {
    reading_pixel_data = false;
    data_read = false;
    header_read = false;
    end_read = false;
    palette_read = false;

    palette_size = 0;

    // At the start of the image we should find
    // 1. PNG Signature
    // 2. IHDR Chunk
    // After that almost anything goes.

    readSignature (inputstream);

    inputstream.getNextChunk ();
    if (! matchChunks (inputstream.getChunkType (), "IHDR")) {
      throw BadPngStream ("Missing IHDR Chunk");
    }

    processChunk (inputstream, theChannel, colors);

    // Now chunks can come in almost any order.
    while (inputstream.getNextChunk () && ! end_read) {
      processChunk (inputstream, theChannel, colors);
    }

    freeData ();
  }
  catch (...) {
    freeData ();
    throw;
  }

  if (! data_read) {
    throw BadPngStream ("Image Contains no Pixel Data");
  }
  return;
}


//
//  Description:
//
//  This function reads the PNG signature from the input stream. It
//  throws an exception if the stream does not have a valid signature.
//
//  Parameters:
//
//  inputstream : The stream to read the signature from.
//
void PngDecoder::readSignature (PngInputStream &inputstream) {

  ubyte *sigbuf;

  inputstream.readRaw (PngSignatureSize, sigbuf);
  if (memcmp (sigbuf, PngSignature, PngSignatureSize) != 0) {
    throw BadPngStream ("Not a PNG file");
  }
  return;
}


//
//  Description:
//
//  This funnction compares two chunk signatures and returns true
//  if the signatures match
//
//  Parameters:
//
//  operand1, operand2: chunk labels (strings)
//
bool PngDecoder::matchChunks (const ubyte *operand1, const char *operand2) {
  if (memcmp (operand1, operand2, 4) == 0) {
    return true;
  } else {
    return false;
  }
}

//
//  Description:
//
//  This function reads a complete chunk from the input stream. A chunk
//  consists of:
//
//      Chunk Length:  4 Bytes
//      Chunk Type:    4 Bytes
//      Chunk Data:    "Chunk Length" Bytes
//      Chunk CRC:     4 Bytes
//
//  The chunk CRC value is calculated from the type and data fields.
//
//  Parameters:
//
//  inputstream : The stream to read the image from
//  image : The output image object
//
void PngDecoder::processChunk (PngInputStream &inputstream, image &theimage) {

  const ubyte *chunktype = inputstream.getChunkType ();

  if (matchChunks (chunktype, "IHDR")) {
    processHeader (inputstream, theimage);
  }
  else if (matchChunks (chunktype, "IDAT")) {
    readPixelData (inputstream, theimage);
  }
  else if (matchChunks (chunktype, "PLTE")) {
    processPalette (inputstream, theimage);
  }
  else if (matchChunks (chunktype, "IEND")) {
    end_read = true;
  }
  else if (matchChunks (chunktype, "bKGD")) {
    processBackground (inputstream);
  }
  else if (matchChunks (chunktype, "gAMA")) {
    processGamma (inputstream);
  }
  else if (matchChunks (chunktype, "cHRM")){
    processChromaticities (inputstream);
  }
  else if (matchChunks (chunktype, "hIST")) {
    processHistogram (inputstream);
  }
  else if (matchChunks (chunktype, "sBIT")) {
    processSignificantBits (inputstream);
  }
  else if (matchChunks (chunktype, "tRNS")) {
    processTransparency (inputstream);
  }
  else if (matchChunks (chunktype, "pHYs")) {
    processPhysicalPixelDimensions (inputstream);
  }
  else if (matchChunks (chunktype, "tIME")) {
    processImageTime (inputstream);
  }
  else if (matchChunks (chunktype, "tEXt")) {
    processTextualData (inputstream);
  }
  else if (matchChunks (chunktype, "zEXt")) {
    processCompressedText (inputstream);
  }
  else if ((chunktype [0] & (1 << 5)) == 0) {
    throw BadPngStream ("Unknown critical chunk");
  }

  return;
}

void PngDecoder::processChunk (PngInputStream &inputstream,
                               channel8 &theChannel,
                               lti::palette& colors) {

  const ubyte *chunktype = inputstream.getChunkType ();

  if (matchChunks (chunktype, "IHDR")) {
    processHeader (inputstream, theChannel, colors);
  }
  else if (matchChunks (chunktype, "IDAT")) {
    readPixelData (inputstream, theChannel, colors);
  }
  else if (matchChunks (chunktype, "PLTE")) {
    processPalette (inputstream, theChannel, colors);
  }
  else if (matchChunks (chunktype, "IEND")) {
    end_read = true;
  }
  else if (matchChunks (chunktype, "bKGD")) {
    processBackground (inputstream);
  }
  else if (matchChunks (chunktype, "gAMA")) {
    processGamma (inputstream);
  }
  else if (matchChunks (chunktype, "cHRM")){
    processChromaticities (inputstream);
  }
  else if (matchChunks (chunktype, "hIST")) {
    processHistogram (inputstream);
  }
  else if (matchChunks (chunktype, "sBIT")) {
    processSignificantBits (inputstream);
  }
  else if (matchChunks (chunktype, "tRNS")) {
    processTransparency (inputstream);
  }
  else if (matchChunks (chunktype, "pHYs")) {
    processPhysicalPixelDimensions (inputstream);
  }
  else if (matchChunks (chunktype, "tIME")) {
    processImageTime (inputstream);
  }
  else if (matchChunks (chunktype, "tEXt")) {
    processTextualData (inputstream);
  }
  else if (matchChunks (chunktype, "zEXt")) {
    processCompressedText (inputstream);
  }
  else if ((chunktype [0] & (1 << 5)) == 0) {
    throw BadPngStream ("Unknown critical chunk");
  }

  return;
}

//
//  Description:
//
//  This function processes an IHDR chuck. This chunk contains the image
//  header which defines the dimensions and color type.
//
//  The main functions here are to allocate space in the image object and
//  to create the color table for grayscale images.
//
//  Parameters:
//
//  inputstream : The input stream to read the header from
//  image : The image to output
//
void PngDecoder::processHeader (PngInputStream &inputstream) {

  int length, size;

  if (header_read) {
    throw BadPngStream ("Duplicate IHDR Block");
  }
  header_read = true;

  if ((length=inputstream.getChunkDataLength ()) !=
      (size=sizeof(PngImageHeader))) {

    throw BadPngStream ("Invalid Header Length");

  }

  // Save the data from the image header.
  PngImageHeader *header = (PngImageHeader *) inputstream.getChunkData();
  image_height = BigEndianToSystem (header->height);
  image_width = BigEndianToSystem (header->width);
  image_depth = header->bitdepth;
  image_color_type = (PngColorType) header->colortype;
  image_compression_method = header->compressionmethod;
  image_filter_method = header->filtermethod;
  image_interlace_method = header->interlacemethod;


  // Ensure the that the values in the image header are valid.
  if (image_compression_method != 0) {
    throw BadPngStream ("Invalid Compression Method");
  }

  if (image_filter_method != 0) {
    throw BadPngStream ("Invalid Filter Method");
  }

  if (image_interlace_method != 0 && image_interlace_method != 1) {
    throw BadPngStream ("Invalid Interlace Method");
  }

  switch (image_depth) {
    case 1:
    case 2:
    case 4:
    case 8:
    case 16: break;
    default: throw BadPngStream ("Invalid Bit Depth");
  }

  // Ensure that the color type and bit depth values are consistent.
  switch (image_color_type) {
    case Grayscale: break;  // All bit depths are legal for grayscale.
    case RGB:
    case RGBAlpha:
    case GrayscaleAlpha:
      if (image_depth != 8 && image_depth != 16) {
        throw BadPngStream ("Invalid Bit Depth for Color Type");
      }
      break;
    case Palette:
      if (image_depth == 16) {
        throw BadPngStream ("Invalid Bit Depth for Color Type");
      }
      break;
    default: throw BadPngStream ("Invalid Color Type");
  }
  return;
}


void PngDecoder::processHeader (PngInputStream &inputstream, image &theimage) {

  processHeader(inputstream);

  // Allocate space space in the image object.
  if (image_color_type == RGB || image_color_type == RGBAlpha) {
    //theimage.setSize (0, 24, image_width, image_height);
    theimage.resize (image_height, image_width);
  }
  else {
    // For an interlaced image use a full byte to represent each pixel to
    // simplify decoding. We do the same for 2-bit images since the
    // BitmapImage class does not support 2-bit images.
    if (image_interlace_method != 0 || image_depth == 2) {
      if (image_depth <= 8) {
        theimage.resize (image_height, image_width);
      }
      else {
        theimage.resize (image_height, image_width);
      }
    }
    else if (image_depth == 16) {
        theimage.resize (image_height, image_width);
    }
    else {
      // Here we handle 1,4, and 8-bit palette and grayscale im
      theimage.resize (image_height, image_width);
    }

    // For grayscale images we need to create a colormap.
    if (image_color_type == Grayscale || image_color_type == GrayscaleAlpha) {
      switch (image_depth) {
        case 1:
        case 2:
        case 4: {
          for (int ii = 0; ii < (1<<image_depth); ++ ii) {
            ubyte value = (ii * PNGMAX8BITSAMPLEVALUE + (1<<image_depth) - 2)
                             / ((1<<image_depth) - 1);

            colorMap[ii].red = value;
            colorMap[ii].green = value;
            colorMap[ii].blue = value;
           } // for
        } // case 4
        break;
        case 8:
        case 16: {
          for (int ii = 0; ii <= PNGMAX8BITSAMPLEVALUE; ++ ii) {
            colorMap[ii].red = ii;
            colorMap[ii].green = ii;
            colorMap[ii].blue = ii;
          } // for
        } // case 16
        break;
        default: throw BadPngStream ("Bad Bit Depth");
      } // switch
    } // if
  } // else
  return;
}

void PngDecoder::processHeader (PngInputStream &inputstream,
                                channel8 &theChannel,
                                lti::palette& colors) {

  processHeader(inputstream);

  // Allocate space space in the image object.
  if (image_color_type == RGB || image_color_type == RGBAlpha) {
    //theChannel.setSize (0, 24, image_width, image_height);
    theChannel.resize (image_height, image_width);
  } else {
    // For an interlaced image use a full byte to represent each pixel to
    // simplify decoding. We do the same for 2-bit images since the
    // BitmapImage class does not support 2-bit images.
    if (image_interlace_method != 0 || image_depth == 2) {
      if (image_depth <= 8) {
        theChannel.resize (image_height, image_width);
      }
      else {
        theChannel.resize (image_height, image_width);
      }
    }
    else if (image_depth == 16) {
      theChannel.resize (image_height, image_width);
    }
    else {
      // Here we handle 1,4, and 8-bit palette and grayscale im
      theChannel.resize (image_height, image_width);
    }

    colors.resize(256);

    // For grayscale images we need to create a colormap.
    if (image_color_type == Grayscale ||
        image_color_type == GrayscaleAlpha) {
      switch (image_depth) {
        case 1:
        case 2:
        case 4: {
          for (int ii = 0; ii < (1<<image_depth); ++ ii) {
            ubyte value = (ii * PNGMAX8BITSAMPLEVALUE + (1<<image_depth) - 2)
                             / ((1<<image_depth) - 1);

            colorMap[ii].red = value;
            colorMap[ii].green = value;
            colorMap[ii].blue = value;
            colors.at(ii) = value;
           } // for
        } // case 4
        break;
        case 8:
        case 16: {
          for (int ii = 0; ii <= PNGMAX8BITSAMPLEVALUE; ++ ii) {
            colorMap[ii].red = ii;
            colorMap[ii].green = ii;
            colorMap[ii].blue = ii;
            colors.at(ii)=ii;
          } // for
        } // case 16
        break;
        default: throw BadPngStream ("Bad Bit Depth");
      } // switch
    } // if
  } // else
  return;
}

//
//  Description:
//
//  This function processes the data in a PLTE block. A PLTE block
//  defines the color palette for the image.
//
//  Parameters:
//
//  inputstream : The stream to read the palette from
//  image : The image to output
//
void PngDecoder::processPalette (PngInputStream &inputstream, image &theimage) {
  // There can only be one palette for the image
  if (palette_read) {
    BadPngStream EPngError ("Duplicate PLTE block");
  }
  palette_read = true;

  // Grayscale images are not allowed to have an palette.
  if (image_color_type == Grayscale || image_color_type == GrayscaleAlpha) {
    BadPngStream EPngError ("Grayscale image contains a PLTE block");
  }

  // The palette size must be divisable by 3.
  uint32 chunklength = inputstream.getChunkDataLength ();
  if (chunklength  % 3 != 0) {
    throw BadPngStream ("PLTE chunk length not divisible by 3");
  }
  if (chunklength > 3 * 256) {
    throw BadPngStream ("PLTE chunk length too large");
  }

  palette_size = chunklength / 3;

  // PLTE chunks are permitted with RGB images to suggest colors
  // for quantization.  Our implementation does not do anything
  // with this information.
  if (image_color_type == RGB || image_color_type == RGBAlpha) {
    return;
  }


  // override predefinition, store the palette in the image.
  const ubyte *chunkdata = inputstream.getChunkData ();
  for (unsigned int ii = 0, jj = 0; ii < chunklength / 3; ++ ii, jj += 3) {
    colorMap[ii].red = chunkdata [jj];
    colorMap[ii].green = chunkdata [jj+1];
    colorMap[ii].blue = chunkdata [jj+2];
  }


  return;
}

void PngDecoder::processPalette (PngInputStream &inputstream,
                                 channel8 &theChannel,
                                 lti::palette& colors) {

  colors.resize(256,rgbPixel(),false,false);

  // There can only be one palette for the image
  if (palette_read) {
    BadPngStream EPngError ("Duplicate PLTE block");
  }
  palette_read = true;

  // Grayscale images are not allowed to have an palette.
  if (image_color_type == Grayscale || image_color_type == GrayscaleAlpha) {
    BadPngStream EPngError ("Grayscale image contains a PLTE block");
  }

  // The palette size must be divisable by 3.
  uint32 chunklength = inputstream.getChunkDataLength ();
  if (chunklength  % 3 != 0) {
    throw BadPngStream ("PLTE chunk length not divisible by 3");
  }
  if (chunklength > 3 * 256) {
    throw BadPngStream ("PLTE chunk length too large");
  }

  palette_size = chunklength / 3;

  // PLTE chunks are permitted with RGB images to suggest colors
  // for quantization.  Our implementation does not do anything
  // with this information.
  if (image_color_type == RGB || image_color_type == RGBAlpha) {
    return;
  }


  // override predefinition, store the palette in the image.
  const ubyte *chunkdata = inputstream.getChunkData ();
  for (unsigned int ii = 0, jj = 0; ii < chunklength / 3; ++ ii, jj += 3) {
    colors.at(ii).setRed(colorMap[ii].red = chunkdata [jj]);
    colors.at(ii).setGreen(colorMap[ii].green = chunkdata [jj+1]);
    colors.at(ii).setBlue(colorMap[ii].blue = chunkdata [jj+2]);
  }

  return;
}

//
//  Description:
//
//  This function processes an IDAT data stream. It decompresses the
//  pixel data and stores it in the image.
//
//  The Deflate compression system supports many options that are not
//  permitted in PNG. This is the reason for checking for specific
//  parameter values.
//
//  Parameters:
//
//  inputstream : The read to read the pixel data form
//  image : The output image
//
void PngDecoder::readPixelData (PngInputStream &inputstream, image &theimage) {
  // Ensure that we only have one IDAT stream in an image stream.
  if (data_read) {
    throw BadPngStream ("Image Contains Multiple Data Segments");
  }

  data_read = true;

  // If the image requires a palette then it must have been read by
  // the time we get here.
  if (image_color_type == Palette && ! palette_read) {
    throw BadPngStream ("PLTE block must occur before IDAT");
  }

  // This block determines the number of bytes needed to store each
  // data row of the image. Then allocate two buffers to hold row
  // data.  We need to row buffers to support filtering.
  unsigned int rowbits; // The number of bits of data per row.

  switch (image_color_type) {
    case Grayscale:
    case Palette:
        rowbits = image_width * image_depth;
        row_buffer_width = (rowbits + CHAR_BIT - 1) / CHAR_BIT;
        break;
    case GrayscaleAlpha:
        row_buffer_width = 2 * image_width * image_depth / CHAR_BIT;
        break;
    case RGB:
        row_buffer_width = image_width * 3 * image_depth / CHAR_BIT;
        break;
    case RGBAlpha:
        row_buffer_width = image_width * 4 * image_depth / CHAR_BIT;
        break;
    default:
        throw BadPngStream ("Invalid Color Type");
  }

  row_buffers [0] = new ubyte [row_buffer_width];
  row_buffers [1] = new ubyte [row_buffer_width];

  inflate_decoder.openStream (inputstream);

  // Read the image data.
  if (image_interlace_method == 0) {
    readNoninterlaced(inputstream, theimage);
  }
  else {
    for (interlace_pass = 0;
         interlace_pass < InterlacePasses;
         ++interlace_pass) {
      readInterlaced (inputstream, theimage);
    }
  }

  reading_pixel_data = false;
  return;
}

void PngDecoder::readPixelData (PngInputStream &inputstream,
                                channel8 &theChannel,
                                lti::palette& colors) {
  // Ensure that we only have one IDAT stream in an image stream.
  if (data_read) {
    throw BadPngStream ("Image Contains Multiple Data Segments");
  }

  data_read = true;

  // If the image requires a palette then it must have been read by
  // the time we get here.
  if (image_color_type == Palette && ! palette_read) {
    throw BadPngStream ("PLTE block must occur before IDAT");
  }

  // This block determines the number of bytes needed to store each
  // data row of the image. Then allocate two buffers to hold row
  // data.  We need to row buffers to support filtering.
  unsigned int rowbits; // The number of bits of data per row.

  switch (image_color_type) {
    case Grayscale:
    case Palette:
        rowbits = image_width * image_depth;
        row_buffer_width = (rowbits + CHAR_BIT - 1) / CHAR_BIT;
        break;
    case GrayscaleAlpha:
        row_buffer_width = 2 * image_width * image_depth / CHAR_BIT;
        break;
    case RGB:
        row_buffer_width = image_width * 3 * image_depth / CHAR_BIT;
        break;
    case RGBAlpha:
        row_buffer_width = image_width * 4 * image_depth / CHAR_BIT;
        break;
    default:
        throw BadPngStream ("Invalid Color Type");
  }

  row_buffers [0] = new ubyte [row_buffer_width];
  row_buffers [1] = new ubyte [row_buffer_width];

  inflate_decoder.openStream (inputstream);

  // Read the image data.
  if (image_interlace_method == 0) {
    readNoninterlaced(inputstream, theChannel);
  }
  else {
    for (interlace_pass = 0;
         interlace_pass < InterlacePasses;
         ++interlace_pass) {
      readInterlaced (inputstream, theChannel);
    }
  }

  reading_pixel_data = false;
  return;
}

//
//  Description:
//
//  This function filters a row of data.
//
//  Parameters:
//
//  filter:  The type of filter
//
void PngDecoder::filterRow (unsigned int filter) {

  int lastrow = ! current_row_buffer; // Index of the previous row
  unsigned int col;                   // Current Column
  unsigned int offset;                // Pixel width

  // Filtering is done on corresponding items within a record. Determine
  // the number of bytes between corresponding items.
  switch (image_color_type) {
    case Grayscale: case Palette: offset = 1; break;
    case RGB:                     offset = 3 * image_depth / CHAR_BIT; break;
    case GrayscaleAlpha:          offset = 2 * image_depth / CHAR_BIT; break;
    case RGBAlpha:                offset = 4 * image_depth / CHAR_BIT; break;
  default: throw BadPngStream ("Invalid Color Type");
  }

  // Filter the row based upon the filter type.
  switch (filter) {
    case FILTERNONE: break;
    case FILTERSUB:
      // The value is the difference from the value to the left.
      for (col = offset; col < row_buffer_width; ++ col) {
        row_buffers [current_row_buffer][col] =
             (row_buffers [current_row_buffer][col] +
              row_buffers [current_row_buffer][col-offset]) & 0xFF;
      }
      break;
    case FILTERUP:
      // The value is the difference from the value in the previous row.
      for (col = 0; col < row_buffer_width; ++ col) {
              row_buffers [current_row_buffer][col] =
             (row_buffers [current_row_buffer][col]
              + row_buffers [lastrow][col]) & 0xFF;
      }
      break;
    case FILTERAVERAGE:
      for (col = 0; col < row_buffer_width; ++ col) {
        int left;
        int above = row_buffers [lastrow][col];
        if (col < offset) {
          left = 0;
        } // if
        else {
          left = row_buffers [current_row_buffer][col-offset];
        }

        row_buffers [current_row_buffer][col] =
              (row_buffers [current_row_buffer][col]
               + (left + above) / 2) & 0xFF;
      } // for
      break;
    case FILTERPAETH:
      for (col = 0; col < row_buffer_width; ++ col) {
        ubyte left, above, aboveleft;
        above = row_buffers [lastrow][col];
        if (col < offset) {
          left = 0;
          aboveleft = 0;
        }// if
        else {
          left = row_buffers [current_row_buffer][col-offset];
          aboveleft = row_buffers [lastrow][col-offset];
        }
        int vv = (int) row_buffers [current_row_buffer][col];
        int pp = PaethPredictor (left, above, aboveleft);
        row_buffers [current_row_buffer][col] = (pp + vv) & 0xFF;
      } // for
      break;
    default: throw BadPngStream ("Invalid Filter Method");
  }
  return;
}

//
//  Description:
//
//    This function copies the data from a non-interlaced row to the image.
//
//  Parameters:
//
//    row :  The output row number
//    image : The image to output
//
void PngDecoder::copyNoninterlacedRowToImage (unsigned int row, image &theimage) {

  unsigned int jj;

  switch (image_color_type) {
    case Grayscale:
    case Palette: {
      switch (image_depth) {
        case 1: {

          ubyte value;
          unsigned int col;
          vector<rgbPixel> *rowbuffer = &theimage[row];
          // the loop is a bit unorthodox: the counter JJ is incremented within the loop...
          for(jj = 0, col = 0; jj < image_width;) {
            // fetch a byte...
            value = row_buffers[current_row_buffer][col]; ++col;

            // and extract all bits
            for(byte nextBit=7; ((nextBit >= 0) && (jj<image_width));nextBit--) {
                // mask every bit in the data byte;
                if((value & (1<<nextBit))>0) {
                  (*rowbuffer)[jj].setRed(colorMap[1].red);
                  (*rowbuffer)[jj].setGreen(colorMap[1].green);
                  (*rowbuffer)[jj].setBlue(colorMap[1].blue);
                } // if
                else {
                  (*rowbuffer)[jj].setRed(colorMap[0].red);
                  (*rowbuffer)[jj].setGreen(colorMap[0].green);
                  (*rowbuffer)[jj].setBlue(colorMap[0].blue);
                } // else
                jj++; // next Byte in the pixel row buffer...

            } // for

          } // for

        }// case 1
        break;

        case 2: {
          ubyte value;
          // We convert 2 bits to 8 bits/pixel.
          vector<rgbPixel> *rowbuffer = &theimage[row];
          for (unsigned int ii = 0; ii < image_width; ++ ii) {
            int byteoffset = ii / 4;
            int bitoffset = ii % 4;
            int rawvalue = row_buffers [current_row_buffer][byteoffset];
            value = (rawvalue >> 2 * (3 - bitoffset)) & 0x3;
            (*rowbuffer)[ii].setRed(colorMap[value].red);
            (*rowbuffer)[ii].setGreen(colorMap[value].green);
            (*rowbuffer)[ii].setBlue(colorMap[value].blue);

          } // for
        } // case 2
        break;

        case 4: {
          ubyte help, help2;
          ubyte value=0;
          unsigned int col;
          bool openByte = false;
          vector<rgbPixel> *rowbuffer = &theimage[row];

          for(jj = 0, col = 0; jj < image_width; jj++) {
            // there is no nibble opne from last operation
            switch(openByte) {
              case false:
                value = row_buffers[current_row_buffer][col];
                help2 = (value & 0xF0)>> 4;
                help = colorMap[(value & 0xF0)>>4].red;
                (*rowbuffer)[jj].setRed(colorMap[(value & 0xF0)>>4].red);
                (*rowbuffer)[jj].setGreen(colorMap[(value & 0xF0)>>4].green);
                (*rowbuffer)[jj].setBlue(colorMap[(value & 0xF0)>>4].blue);
                openByte = true;
              break;

              // nibble from last byte left...
              case true:
                help2 = (value & 0x0F);
                help = colorMap[value & 0x0F].red;
                (*rowbuffer)[jj].setRed(colorMap[value & 0x0F].red);
                (*rowbuffer)[jj].setGreen(colorMap[value & 0x0F].green);
                (*rowbuffer)[jj].setBlue(colorMap[value & 0x0F].blue);
                openByte = false;
                col++;
              break;
            } // switch

          } // for

        } //case 4
        break;

        case 8: {
          ubyte value;
          unsigned int col;
          vector<rgbPixel> *rowbuffer = &theimage[row];
          for(jj = 0, col = 0; jj < image_width; jj++) {
            value = row_buffers[current_row_buffer][col];
            ++col;
            (*rowbuffer)[jj].setRed(colorMap[value].red);
            (*rowbuffer)[jj].setGreen(colorMap[value].green);
            (*rowbuffer)[jj].setBlue(colorMap[value].blue);
          } // for

        } // case 8
        break;

        case 16: {
          ubyte value;
          vector<rgbPixel> *rowbuffer = &theimage[row];
          jj = 0;
          for (unsigned int ii = 0; ii < row_buffer_width; ii += 2) {
            value = row_buffers [current_row_buffer][ii];
            (*rowbuffer)[ii/2].setRed(colorMap[value].red);
            (*rowbuffer)[ii/2].setGreen(colorMap[value].green);
            (*rowbuffer)[ii/2].setBlue(colorMap[value].blue);
          } // for
        } // case 16
        break;
        default:
          throw BadPngStream ("Invalid Bit Depth");
      } // switch image_depth
    } // case palette
    break;

    case RGB: {
      if (image_depth == 8) {
        vector<rgbPixel> *rowbuffer = &theimage[row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {
          ubyte red   = row_buffers [current_row_buffer][col]; col++; // ++col;
          ubyte green = row_buffers [current_row_buffer][col]; col++; // ++col
          ubyte blue  = row_buffers [current_row_buffer][col]; col++; // ++col

          (*rowbuffer)[jj].setRed(red);
          (*rowbuffer)[jj].setGreen(green);
          (*rowbuffer)[jj].setBlue(blue);
          jj++;
        } // for
      } // if
      else if (image_depth == 16) {
        vector<rgbPixel> *rowbuffer = &theimage [row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {
          ubyte red   = row_buffers [current_row_buffer][col]; col += 2;
          ubyte green = row_buffers [current_row_buffer][col]; col += 2;
          ubyte blue  = row_buffers [current_row_buffer][col]; col += 2;
          (*rowbuffer)[jj].setRed(red);
          (*rowbuffer)[jj].setGreen(green);
          (*rowbuffer)[jj].setBlue(blue);
          jj++;
        } // for
      } // else
      else {
          throw BadPngStream ("Invalid Image Depth");
      }
    } // case RGB
    break;

    case GrayscaleAlpha: {
      // working now: 8 bit per grayscale and 8 bit alpha channel
      if (image_depth == 8) {
        ubyte result;
        vector<rgbPixel> *rowbuffer = &theimage [row];
        for (unsigned int ii = 0; ii < image_width; ++ ii) {
          ubyte value = row_buffers [current_row_buffer][ii*2];
          ubyte alpha = row_buffers [current_row_buffer][ii*2 + 1];
          result = (alpha * value + (PNGMAX8BITSAMPLEVALUE - alpha)
                          * background_gray)
                          / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[ii].setRed(result);
          (*rowbuffer)[ii].setGreen(result);
          (*rowbuffer)[ii].setBlue(result);

        } // for
      } // if
      else if (image_depth == 16) {
        ubyte result;
        vector<rgbPixel> *rowbuffer = &theimage [row];
        for (unsigned int ii = 0; ii < image_width; ++ ii) {
          ubyte value = row_buffers [current_row_buffer][ii*4];
          ubyte alpha = row_buffers [current_row_buffer][ii*4 + 2];
          result = (alpha * value + (PNGMAX8BITSAMPLEVALUE - alpha)
                          * background_gray)
                          / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[ii].setRed(result);
          (*rowbuffer)[ii].setGreen(result);
          (*rowbuffer)[ii].setBlue(result);
        } // for
      }  // else
      else {
        throw BadPngStream ("Invalid Image Depth for Grayscale/Alpha");
      } // else
    } // case
    break;

    case RGBAlpha: {
      // background is initialized with black (0,0,0)
      if (image_depth == 8) {
        vector<rgbPixel> *rowbuffer = &theimage [row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {
          ubyte inred   = row_buffers [current_row_buffer][col]; ++ col;
          ubyte ingreen = row_buffers [current_row_buffer][col]; ++ col;
          ubyte inblue  = row_buffers [current_row_buffer][col]; ++ col;
          ubyte alpha   = row_buffers [current_row_buffer][col]; ++ col;
          ubyte outred = (alpha * inred
                                + (PNGMAX8BITSAMPLEVALUE - alpha) * background_red)
                               / PNGMAX8BITSAMPLEVALUE;
          ubyte outgreen = (alpha * ingreen
                                  + (PNGMAX8BITSAMPLEVALUE - alpha) * background_green)
                                  / PNGMAX8BITSAMPLEVALUE;
          ubyte outblue = (alpha * inblue
                                 + (PNGMAX8BITSAMPLEVALUE - alpha) * background_blue)
                                 / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[jj].setRed(outred);
          (*rowbuffer)[jj].setGreen(outgreen);
          (*rowbuffer)[jj].setBlue(outblue);
          jj++;

        } // for
      }  // if
      else if (image_depth == 16) {
        // 16 bit per channel = 48 (64) bit Truecolor, but only the Lowbyte is used
        // so there are still 16,7 Mio. colors.
        vector<rgbPixel> *rowbuffer = &theimage [row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {

          ubyte inred   = row_buffers [current_row_buffer][col]; col += 2;
          ubyte ingreen = row_buffers [current_row_buffer][col]; col += 2;
          ubyte inblue  = row_buffers [current_row_buffer][col]; col += 2;
          ubyte alpha   = row_buffers [current_row_buffer][col]; col += 2;

          ubyte outred = (alpha * inred
                                + (PNGMAX8BITSAMPLEVALUE - alpha) * background_red)
                               / PNGMAX8BITSAMPLEVALUE;
          ubyte outgreen = (alpha * ingreen
                                  + (PNGMAX8BITSAMPLEVALUE - alpha) * background_green)
                                 / PNGMAX8BITSAMPLEVALUE;
          ubyte outblue = (alpha * inblue
                                 + (PNGMAX8BITSAMPLEVALUE - alpha) * background_blue)
                                / PNGMAX8BITSAMPLEVALUE;

           (*rowbuffer)[jj].setRed(outred);
           (*rowbuffer)[jj].setGreen(outgreen);
           (*rowbuffer)[jj].setBlue(outblue);
           jj++;
        }// for
      } // else
      else  {
        throw BadPngStream ("Invalid Image Depth for RGB/Alpha");
      }
    } // case
    break;

    default:
      throw BadPngStream ("Invalid Color Type");
  } // switch
  return;
} // copy noninterlaced

void PngDecoder::copyNoninterlacedRowToImage (unsigned int row,
                                              channel8& theChannel) {

  unsigned int jj;

  switch (image_color_type) {
    case Grayscale:
    case Palette: {
      switch (image_depth) {
        case 1: {

          ubyte value;
          unsigned int col;
          vector<ubyte> *rowbuffer = &theChannel[row];
          // the loop is a bit unorthodox: the counter JJ is incremented
          // within the loop...
          for(jj = 0, col = 0; jj < image_width;) {
            // fetch a byte...
            value = row_buffers[current_row_buffer][col]; ++col;

            // and extract all bits
            for(byte nextBit=7;
                ((nextBit >= 0) && (jj<image_width));
                nextBit--) {
              // mask every bit in the data byte;
              if((value & (1<<nextBit))>0) {
                (*rowbuffer)[jj] = 1;
              } // if
              else {
                (*rowbuffer)[jj] = 0;
              } // else
              jj++; // next Byte in the pixel row buffer...

            } // for

          } // for

        }// case 1
        break;

        case 2: {
          ubyte value;
          // We convert 2 bits to 8 bits/pixel.
          vector<ubyte> *rowbuffer = &theChannel[row];
          for (unsigned int ii = 0; ii < image_width; ++ ii) {
            int byteoffset = ii / 4;
            int bitoffset = ii % 4;
            int rawvalue = row_buffers [current_row_buffer][byteoffset];
            value = (rawvalue >> 2 * (3 - bitoffset)) & 0x3;
            (*rowbuffer)[ii] = value;
          } // for
        } // case 2
        break;

        case 4: {
          ubyte help, help2;
          ubyte value=0;
          unsigned int col;
          bool openByte = false;
          vector<ubyte> *rowbuffer = &theChannel[row];

          for(jj = 0, col = 0; jj < image_width; jj++) {
            // there is no nibble opne from last operation
            switch(openByte) {
              case false:
                value = row_buffers[current_row_buffer][col];
                help2 = (value & 0xF0)>> 4;
                help = colorMap[(value & 0xF0)>>4].red;
                (*rowbuffer)[jj] = (value & 0xF0)>>4;
                openByte = true;
                break;

                // nibble from last byte left...
              case true:
                help2 = (value & 0x0F);
                help = colorMap[value & 0x0F].red;
                (*rowbuffer)[jj] = value & 0x0F;
                openByte = false;
                col++;
                break;
            } // switch

          } // for

        } //case 4
        break;

        case 8: {
          ubyte value;
          unsigned int col;
          vector<ubyte> *rowbuffer = &theChannel[row];
          for(jj = 0, col = 0; jj < image_width; jj++) {
            value = row_buffers[current_row_buffer][col];
            ++col;
            (*rowbuffer)[jj] = value;
          } // for

        } // case 8
        break;

        case 16: {
          ubyte value;
          vector<ubyte> *rowbuffer = &theChannel[row];
          jj = 0;
          for (unsigned int ii = 0; ii < row_buffer_width; ii += 2) {
            value = row_buffers [current_row_buffer][ii];
            (*rowbuffer)[ii/2] = value;
          } // for
        } // case 16
        break;
        default:
          throw BadPngStream ("Invalid Bit Depth");
      } // switch image_depth
    } // case palette
    break;

    case RGB: {
      if (image_depth == 8) {
        vector<ubyte> *rowbuffer = &theChannel[row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {
          ubyte red   = row_buffers [current_row_buffer][col]; col++; // ++col;
          ubyte green = row_buffers [current_row_buffer][col]; col++; // ++col
          ubyte blue  = row_buffers [current_row_buffer][col]; col++; // ++col

          // TODO: Quantization
          (*rowbuffer)[jj] = ubyte((int(red)+int(green)+int(blue))/3);
          jj++;
        } // for
      } // if
      else if (image_depth == 16) {
        vector<ubyte> *rowbuffer = &theChannel [row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {
          ubyte red   = row_buffers [current_row_buffer][col]; col += 2;
          ubyte green = row_buffers [current_row_buffer][col]; col += 2;
          ubyte blue  = row_buffers [current_row_buffer][col]; col += 2;
          (*rowbuffer)[jj] = ubyte((int(red)+int(green)+int(blue))/3);
          jj++;
        } // for
      } // else
      else {
        throw BadPngStream ("Invalid Image Depth");
      }
    } // case RGB
    break;

    case GrayscaleAlpha: {
      // working now: 8 bit per grayscale and 8 bit alpha channel
      if (image_depth == 8) {
        ubyte result;
        vector<ubyte> *rowbuffer = &theChannel [row];
        for (unsigned int ii = 0; ii < image_width; ++ ii) {
          ubyte value = row_buffers [current_row_buffer][ii*2];
          ubyte alpha = row_buffers [current_row_buffer][ii*2 + 1];
          result = (alpha * value + (PNGMAX8BITSAMPLEVALUE - alpha)
                    * background_gray)
            / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[ii] = result;
        } // for
      } // if
      else if (image_depth == 16) {
        ubyte result;
        vector<ubyte> *rowbuffer = &theChannel [row];
        for (unsigned int ii = 0; ii < image_width; ++ ii) {
          ubyte value = row_buffers [current_row_buffer][ii*4];
          ubyte alpha = row_buffers [current_row_buffer][ii*4 + 2];
          result = (alpha * value + (PNGMAX8BITSAMPLEVALUE - alpha)
                          * background_gray)
                          / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[ii] = result;
        } // for
      }  // else
      else {
        throw BadPngStream ("Invalid Image Depth for Grayscale/Alpha");
      } // else
    } // case
    break;

    case RGBAlpha: {
      // background is initialized with black (0,0,0)
      if (image_depth == 8) {
        vector<ubyte> *rowbuffer = &theChannel [row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {
          ubyte inred   = row_buffers [current_row_buffer][col]; ++ col;
          ubyte ingreen = row_buffers [current_row_buffer][col]; ++ col;
          ubyte inblue  = row_buffers [current_row_buffer][col]; ++ col;
          ubyte alpha   = row_buffers [current_row_buffer][col]; ++ col;
          ubyte outred = (alpha * inred
                          + (PNGMAX8BITSAMPLEVALUE - alpha) * background_red)
            / PNGMAX8BITSAMPLEVALUE;
          ubyte outgreen = (alpha * ingreen
                            + (PNGMAX8BITSAMPLEVALUE - alpha) * background_green)
            / PNGMAX8BITSAMPLEVALUE;
          ubyte outblue = (alpha * inblue
                           + (PNGMAX8BITSAMPLEVALUE - alpha) * background_blue)
            / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[jj] = ubyte((int(outred)+int(outgreen)+int(outblue))/3);
          jj++;

        } // for
      }  // if
      else if (image_depth == 16) {
        // 16 bit per channel = 48 (64) bit Truecolor, but only the Lowbyte
        // is used so there are still 16,7 Mio. colors.
        vector<ubyte> *rowbuffer = &theChannel [row];
        jj = 0;
        for (unsigned int ii = 0, col = 0; ii < 3 * image_width; ii += 3) {

          ubyte inred   = row_buffers [current_row_buffer][col]; col += 2;
          ubyte ingreen = row_buffers [current_row_buffer][col]; col += 2;
          ubyte inblue  = row_buffers [current_row_buffer][col]; col += 2;
          ubyte alpha   = row_buffers [current_row_buffer][col]; col += 2;

          ubyte outred = (alpha * inred
                          + (PNGMAX8BITSAMPLEVALUE - alpha) * background_red)
            / PNGMAX8BITSAMPLEVALUE;
          ubyte outgreen = (alpha * ingreen
                            + (PNGMAX8BITSAMPLEVALUE - alpha) * background_green)
            / PNGMAX8BITSAMPLEVALUE;
          ubyte outblue = (alpha * inblue
                           + (PNGMAX8BITSAMPLEVALUE - alpha) * background_blue)
            / PNGMAX8BITSAMPLEVALUE;

          (*rowbuffer)[jj] = ubyte((int(outred)+int(outgreen)+int(outblue))/3);
          jj++;
        }// for
      } // else
      else  {
        throw BadPngStream ("Invalid Image Depth for RGB/Alpha");
      }
    } // case
    break;

    default:
      throw BadPngStream ("Invalid Color Type");
  } // switch
  return;
} // copy noninterlaced

//
//  Description:
//
//  This function reads the pixel data for a non-interlaced image.
//
//  Parameters:
//
//  inputstream : The input stream to read the image from.
//  image : The image to output
//
void PngDecoder::readNoninterlaced (PngInputStream &inputstream,
                                    image &theimage) {
  // Initialize the input buffers.
  current_row_buffer = 0;
  memset (row_buffers [0], 0, row_buffer_width);
  memset (row_buffers [1], 0, row_buffer_width);


  for (unsigned int row = 0; row < image_height; ++ row) {
    ubyte filterbuffer;
    inflate_decoder.decode (inputstream, sizeof (filterbuffer), &filterbuffer);
    PngFilterType filter = static_cast <PngFilterType> (filterbuffer);
    inflate_decoder.decode (inputstream,
                            row_buffer_width,
                            row_buffers [current_row_buffer]);
    filterRow (filter);
    copyNoninterlacedRowToImage (row, theimage);

    current_row_buffer = ! current_row_buffer; // Switch buffers
  }
  return;
}

void PngDecoder::readNoninterlaced (PngInputStream &inputstream,
                                    channel8 &theChannel) {
  // Initialize the input buffers.
  current_row_buffer = 0;
  memset (row_buffers [0], 0, row_buffer_width);
  memset (row_buffers [1], 0, row_buffer_width);


  for (unsigned int row = 0; row < image_height; ++ row) {
    ubyte filterbuffer;
    inflate_decoder.decode (inputstream, sizeof (filterbuffer), &filterbuffer);
    PngFilterType filter = static_cast <PngFilterType> (filterbuffer);
    inflate_decoder.decode (inputstream,
                            row_buffer_width,
                            row_buffers [current_row_buffer]);
    filterRow (filter);
    copyNoninterlacedRowToImage (row, theChannel);
    current_row_buffer = ! current_row_buffer; // Switch buffers
  }
  return;
}



//
//  Description:
//
//  This function reads all the rows for an interlaced pass.
//
//  Parameters:
//
//  inputstream : The input stream to read the image from
//  image : The image object to return
//
void PngDecoder::readInterlaced (PngInputStream &inputstream,
                                 image &theimage) {
  const InterlaceInfo *info = &interlace_values [interlace_pass];

  // If the image is too small we may not have to do any work.
  if (info->start_row >= image_height || info->start_col  >= image_width) {
    return;
  }

  current_row_buffer = 0;
  memset (row_buffers [0], 0, row_buffer_width);
  memset (row_buffers [1], 0, row_buffer_width);

  unsigned int pixelsthisrow = (image_width - info->start_col
                                + info->col_interval - 1)
                               / info->col_interval;
  unsigned int rowbytes;
  switch (image_color_type) {
    case Grayscale:
    case Palette:
      rowbytes = (pixelsthisrow * image_depth + CHAR_BIT - 1) / CHAR_BIT;
    break;
    case RGB:
      rowbytes = pixelsthisrow * 3 * image_depth / CHAR_BIT;
    break;
    case RGBAlpha:
      rowbytes = pixelsthisrow * 4 * image_depth / CHAR_BIT;
    break;
    case GrayscaleAlpha:
      rowbytes = pixelsthisrow * 2 * image_depth / CHAR_BIT;
    break;
    default:
      throw BadPngStream ("Invalid Color Type");
  }

  for (unsigned int destrow=info->start_row;destrow < image_height;destrow+=info->row_interval) {
    ubyte filterbuffer;
    // The filter type precedes the row data.
    inflate_decoder.decode (inputstream, sizeof (filterbuffer), &filterbuffer);
    PngFilterType filter = static_cast <PngFilterType>(filterbuffer);
    // Read the row data.
    inflate_decoder.decode (inputstream, rowbytes, row_buffers [current_row_buffer]);
    // Filter the data
    filterRow (filter);
    // pick pixel values out of bytestream
    copyInterlacedRowToImage (destrow, rowbytes, theimage);
    // Switch buffers
    current_row_buffer = ! current_row_buffer;
  }

  return;
}

void PngDecoder::readInterlaced (PngInputStream &inputstream,
                                 channel8 &theChannel) {
  const InterlaceInfo *info = &interlace_values [interlace_pass];

  // If the image is too small we may not have to do any work.
  if (info->start_row >= image_height || info->start_col  >= image_width) {
    return;
  }

  current_row_buffer = 0;
  memset (row_buffers [0], 0, row_buffer_width);
  memset (row_buffers [1], 0, row_buffer_width);

  unsigned int pixelsthisrow = (image_width - info->start_col
                                + info->col_interval - 1)
                               / info->col_interval;
  unsigned int rowbytes;
  switch (image_color_type) {
    case Grayscale:
    case Palette:
      rowbytes = (pixelsthisrow * image_depth + CHAR_BIT - 1) / CHAR_BIT;
    break;
    case RGB:
      rowbytes = pixelsthisrow * 3 * image_depth / CHAR_BIT;
    break;
    case RGBAlpha:
      rowbytes = pixelsthisrow * 4 * image_depth / CHAR_BIT;
    break;
    case GrayscaleAlpha:
      rowbytes = pixelsthisrow * 2 * image_depth / CHAR_BIT;
    break;
    default:
      throw BadPngStream ("Invalid Color Type");
  }

  for (unsigned int destrow=info->start_row;destrow <image_height;
       destrow+=info->row_interval) {
    ubyte filterbuffer;
    // The filter type precedes the row data.
    inflate_decoder.decode (inputstream, sizeof (filterbuffer), &filterbuffer);
    PngFilterType filter = static_cast <PngFilterType>(filterbuffer);
    // Read the row data.
    inflate_decoder.decode (inputstream, rowbytes,
                            row_buffers [current_row_buffer]);
    // Filter the data
    filterRow (filter);
    // pick pixel values out of bytestream
    copyInterlacedRowToImage (destrow, rowbytes, theChannel);
    // Switch buffers
    current_row_buffer = ! current_row_buffer;
  }

  return;
}

//
//  Description:
//
//  This function copies an interlaced row to the output image.
//
//  Parameters:
//
//  row: The output row number
//  rowwidth: The number of bytes of data in the row
//  image : The image to output
//
void PngDecoder::copyInterlacedRowToImage (unsigned int row,
                                           unsigned int rowwidth,
                                           image &theimage) {
  const InterlaceInfo *info = &interlace_values [interlace_pass];
  vector<rgbPixel> *imagebuffer = &theimage [row];

  switch (image_color_type) {
    case Grayscale:
    case Palette:
      switch (image_depth) {
      case 1: {
        for (unsigned int col = 0, destcol = info->start_col;col < rowwidth;++ col) {
          for (int ii=CHAR_BIT-1;ii>=0 && destcol<image_width;--ii,destcol+=info->col_interval) {
            ubyte value = (row_buffers [current_row_buffer][col] >> ii) & 0x1;
            (*imagebuffer)[destcol].setRed(colorMap[value].red);
            (*imagebuffer)[destcol].setGreen(colorMap[value].green);
            (*imagebuffer)[destcol].setBlue(colorMap[value].blue);
          } // for
        } // for
      } // case1
      break;

      case 2: {
        // 2 Bits per pixel gets converted to 8 Bits.
        for(unsigned int col = 0, destcol = info->start_col; col < rowwidth; ++ col) {
          for(int ii=3*CHAR_BIT/4;
              ii>=0 && destcol<image_width;
              ii-=CHAR_BIT/4,destcol+=info->col_interval) {
                ubyte value=(row_buffers [current_row_buffer][col] >> ii) & 0x3;
                (*imagebuffer)[destcol].setRed(colorMap[value].red);
                (*imagebuffer)[destcol].setGreen(colorMap[value].green);
                (*imagebuffer)[destcol].setBlue(colorMap[value].blue);
          } // for
        } // for
      } // case
      break;

      case 4: {
        for (unsigned int col = 0, destcol = info->start_col;col < rowwidth;++ col) {
          for (int ii = CHAR_BIT/2;
               ii >= 0 && destcol < image_width;
               ii -= CHAR_BIT/2, destcol += info->col_interval) {
                 ubyte value = (row_buffers [current_row_buffer][col] >> ii) & 0xF;
                (*imagebuffer)[destcol].setRed(colorMap[value].red);
                (*imagebuffer)[destcol].setGreen(colorMap[value].green);
                (*imagebuffer)[destcol].setBlue(colorMap[value].blue);

          } // for
        } // for
      } // for
      break;

      case 8: {
        for (unsigned int col = 0, destcol = info->start_col;
             col < rowwidth;
             ++ col, destcol += info->col_interval) {
               ubyte value = row_buffers [current_row_buffer][col];
               (*imagebuffer)[destcol].setRed(colorMap[value].red);
               (*imagebuffer)[destcol].setGreen(colorMap[value].green);
               (*imagebuffer)[destcol].setBlue(colorMap[value].blue);
        } // for
      } // case 8
      break;

      case 16: {
        for (unsigned int col = 0, destcol = info->start_col;
             col < rowwidth;
             col += 2, destcol += info->col_interval) {
               ubyte value = row_buffers [current_row_buffer][col];
              (*imagebuffer)[destcol].setRed(colorMap[value].red);
              (*imagebuffer)[destcol].setGreen(colorMap[value].green);
              (*imagebuffer)[destcol].setBlue(colorMap[value].blue);
        } // for
      } //case 16
      break;
      default:
        throw BadPngStream ("Invalid Image Depth");
    } //  case Palette
    break;

    case RGB: {
      if (image_depth == 8) {
        for (unsigned int col=0,destcol=info->start_col;col < rowwidth;destcol += info->col_interval) {
          ubyte red = row_buffers [current_row_buffer][col]; ++ col;
          ubyte green = row_buffers [current_row_buffer][col]; ++ col;
          ubyte blue = row_buffers [current_row_buffer][col]; ++ col;

          (*imagebuffer)[destcol].setRed(red);
          (*imagebuffer)[destcol].setGreen(green);
          (*imagebuffer)[destcol].setBlue(blue);
        } // for
      } // if
      else if (image_depth == 16) {
        vector<rgbPixel> *imagebuffer = &theimage[row];
        for (unsigned int col=0,destcol=info->start_col;col < rowwidth;destcol +=info->col_interval) {
          ubyte red = row_buffers [current_row_buffer][col]; col += 2;
          ubyte green = row_buffers [current_row_buffer][col]; col += 2;
          ubyte blue = row_buffers [current_row_buffer][col]; col += 2;

          (*imagebuffer)[destcol].setRed(red);
          (*imagebuffer)[destcol].setGreen(green);
          (*imagebuffer)[destcol].setBlue(blue);
        } // for
      } // else
      else {
          throw BadPngStream ("Invalid Image Depth");
      }
    } // case RGB
    break;

    case RGBAlpha: {
      for (unsigned int col = 0, destcol = info->start_col;
           col < rowwidth;
           destcol += info->col_interval) {

             ubyte inred = row_buffers [current_row_buffer][col];
             col += image_depth / CHAR_BIT;
             ubyte ingreen = row_buffers [current_row_buffer][col];
             col += image_depth / CHAR_BIT;
             ubyte inblue = row_buffers [current_row_buffer][col];
             col += image_depth / CHAR_BIT;
             ubyte alpha = row_buffers [current_row_buffer][col];
             col += image_depth / CHAR_BIT;

             ubyte outred = (alpha * inred
                             + (PNGMAX8BITSAMPLEVALUE - alpha) * background_red)
                             / PNGMAX8BITSAMPLEVALUE;
             ubyte outgreen = (alpha * ingreen
                               + (PNGMAX8BITSAMPLEVALUE - alpha) * background_green)
                              / PNGMAX8BITSAMPLEVALUE;
             ubyte outblue = (alpha * inblue
                              + (PNGMAX8BITSAMPLEVALUE - alpha) * background_blue)
                             / PNGMAX8BITSAMPLEVALUE;

             (*imagebuffer)[destcol].setRed(outred);
             (*imagebuffer)[destcol].setGreen(outgreen);
             (*imagebuffer)[destcol].setBlue(outblue);
      } // for
    } // case RGBAlpha
    break;

    case GrayscaleAlpha: {
      for (unsigned int col=0,destcol=info->start_col;col<rowwidth;destcol+=info->col_interval) {
        ubyte invalue = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;
        ubyte alpha = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;

        ubyte outvalue = (alpha * invalue
                          + (PNGMAX8BITSAMPLEVALUE - alpha) * background_gray)
                          / PNGMAX8BITSAMPLEVALUE;
        (*imagebuffer)[destcol].setRed(outvalue);
        (*imagebuffer)[destcol].setGreen(outvalue);
        (*imagebuffer)[destcol].setBlue(outvalue);
      } // for
    } // case GrayscaleAlpha
    break;

    default:
      throw BadPngStream ("Invalid Color Type");
  } // switch color type
  return;
}

void PngDecoder::copyInterlacedRowToImage (unsigned int row,
                                           unsigned int rowwidth,
                                           channel8& theChannel) {
  const InterlaceInfo *info = &interlace_values [interlace_pass];
  vector<ubyte> *imagebuffer = &theChannel [row];

  switch (image_color_type) {
    case Grayscale:
    case Palette:
      switch (image_depth) {
        case 1: {
          for (unsigned int col = 0, destcol = info->start_col;
             col < rowwidth;
               ++ col) {
            for (int ii=CHAR_BIT-1;ii>=0 && destcol<image_width;
                 --ii,destcol+=info->col_interval) {
              ubyte value =
                (row_buffers [current_row_buffer][col] >> ii) & 0x1;
              (*imagebuffer)[destcol] = value;
            } // for
          } // for
        } // case1
        break;

        case 2: {
          // 2 Bits per pixel gets converted to 8 Bits.
          for(unsigned int col = 0, destcol = info->start_col;
              col < rowwidth; ++ col) {
            for(int ii=3*CHAR_BIT/4;
                ii>=0 && destcol<image_width;
                ii-=CHAR_BIT/4,destcol+=info->col_interval) {
              ubyte value=(row_buffers [current_row_buffer][col] >> ii) & 0x3;
              (*imagebuffer)[destcol] = value;
            } // for
          } // for
        } // case
        break;

        case 4: {
          for (unsigned int col = 0, destcol = info->start_col;
               col < rowwidth;++ col) {
            for (int ii = CHAR_BIT/2;
                 ii >= 0 && destcol < image_width;
                 ii -= CHAR_BIT/2, destcol += info->col_interval) {
              ubyte value =
                (row_buffers [current_row_buffer][col] >> ii) & 0xF;
              (*imagebuffer)[destcol] = value;
            } // for
          } // for
        }
        break;

        case 8: {
          for (unsigned int col = 0, destcol = info->start_col;
               col < rowwidth;
               ++ col, destcol += info->col_interval) {
            ubyte value = row_buffers [current_row_buffer][col];
            (*imagebuffer)[destcol] = value;
          } // for
        } // case 8
        break;

        case 16: {
          for (unsigned int col = 0, destcol = info->start_col;
               col < rowwidth;
               col += 2, destcol += info->col_interval) {
            ubyte value = row_buffers [current_row_buffer][col];
            (*imagebuffer)[destcol] = value;
          } // for
        } //case 16
        break;
        default:
          throw BadPngStream ("Invalid Image Depth");
      } //  case Palette
      break;

    case RGB: {
      if (image_depth == 8) {
        for (unsigned int col=0,destcol=info->start_col;
             col < rowwidth;destcol += info->col_interval) {
          ubyte red = row_buffers [current_row_buffer][col]; ++ col;
          ubyte green = row_buffers [current_row_buffer][col]; ++ col;
          ubyte blue = row_buffers [current_row_buffer][col]; ++ col;

          // TODO: Quantization!
          (*imagebuffer)[destcol] = ubyte((int(red)+int(green)+int(blue))/3);
        } // for
      } // if
      else if (image_depth == 16) {
        vector<ubyte> *imagebuffer = &theChannel[row];
        for (unsigned int col=0,destcol=info->start_col;
             col < rowwidth;destcol +=info->col_interval) {
          ubyte red = row_buffers [current_row_buffer][col]; col += 2;
          ubyte green = row_buffers [current_row_buffer][col]; col += 2;
          ubyte blue = row_buffers [current_row_buffer][col]; col += 2;

          // TODO: Quantization!
          (*imagebuffer)[destcol] = ubyte((int(red)+int(green)+int(blue))/3);

        } // for
      } // else
      else {
        throw BadPngStream ("Invalid Image Depth");
      }
    } // case RGB
    break;

    case RGBAlpha: {
      for (unsigned int col = 0, destcol = info->start_col;
           col < rowwidth;
           destcol += info->col_interval) {

        ubyte inred = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;
        ubyte ingreen = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;
        ubyte inblue = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;
        ubyte alpha = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;

        ubyte outred = (alpha * inred
                        + (PNGMAX8BITSAMPLEVALUE - alpha) * background_red)
          / PNGMAX8BITSAMPLEVALUE;
        ubyte outgreen = (alpha * ingreen
                          + (PNGMAX8BITSAMPLEVALUE - alpha) * background_green)
          / PNGMAX8BITSAMPLEVALUE;
        ubyte outblue = (alpha * inblue
                         + (PNGMAX8BITSAMPLEVALUE - alpha) * background_blue)
          / PNGMAX8BITSAMPLEVALUE;

        // TODO: Quantization
        (*imagebuffer)[destcol] =
          ubyte((int(outred)+int(outgreen)+int(outblue))/3);
      } // for
    } // case RGBAlpha
    break;

    case GrayscaleAlpha: {
      for (unsigned int col=0,destcol=info->start_col;
           col<rowwidth;destcol+=info->col_interval) {
        ubyte invalue = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;
        ubyte alpha = row_buffers [current_row_buffer][col];
        col += image_depth / CHAR_BIT;

        ubyte outvalue = (alpha * invalue
                          + (PNGMAX8BITSAMPLEVALUE - alpha) * background_gray)
          / PNGMAX8BITSAMPLEVALUE;
        // TODO: Quantization

        (*imagebuffer)[destcol] = outvalue;
      } // for
    } // case GrayscaleAlpha
    break;

    default:
      throw BadPngStream ("Invalid Color Type");
  } // switch color type
  return;
}




//
//  Description:
//
//  This function processes a bKGD chuck. This chunk defines the background
//  color for the image. We only use this for Alpha channel processing.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processBackground (PngInputStream &inputstream) {
  ubyte index;
  const ubyte *chunkdata = inputstream.getChunkData ();
  switch (image_color_type) {
    case Grayscale: case GrayscaleAlpha:
      if (inputstream.getChunkDataLength () == 0) {
        throw BadPngStream ("bKGD Chunk too small");
      }

      index = chunkdata [0];
      if (index >= (1<<image_depth)) {
        throw BadPngStream ("bKGD palette index too large");
      }
      background_gray = index;
      break;

    case Palette:
      if (inputstream.getChunkDataLength () == 0) {
        throw BadPngStream ("bKGD Chunk too small");
      }
      index = chunkdata [0];
      if (index >= (1<<image_depth)) {
        throw BadPngStream ("bKGD palette index too large");
      }
      background_red = colorMap[index].red;
      background_green = colorMap[index].green;
      background_blue = colorMap[index].blue;
      break;

    case RGB:
    case RGBAlpha:
      if (inputstream.getChunkDataLength () < 6) {
        throw BadPngStream ("bKGD Chunk too small");
      }
      background_red = chunkdata [1];
      background_green = chunkdata [3];
      background_blue = chunkdata [5];
      break;

    default:
      throw BadPngStream ("Invalid Color Type");
  }
  return;
}


//
//  Description:
//
//  This function processes a gAMA chunk. The game value is stored
//  as a 4-byte integer which is the Gamma value times 100,000.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processGamma (PngInputStream &inputstream) {
  if (palette_read) {
    throw BadPngStream ("gAMA chunk may not occur before PLTE");
  }

  if (data_read) {
    throw BadPngStream ("gAMA chunk may not occur before IDAT");
  }

  uint32 value;
  if (inputstream.getChunkDataLength () < 4) {
    throw BadPngStream ("gAMA chunk too small");
  }
  const ubyte *chunkdata = inputstream.getChunkData ();
  value = chunkdata [0] | (chunkdata [1] << CHAR_BIT)
                        | (chunkdata [2] << (2*CHAR_BIT))
                        | (chunkdata [3] << (3*CHAR_BIT));

  value = BigEndianToSystem (value);
  file_gamma = (double) value / 100000.0;

}

//
//  Description:
//
//  This function processes a cHRM chunk. This chunk defines
//  precise color values for the image.
//
//  We do nothing with this chunk but dump its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processChromaticities (PngInputStream &inputstream) {
  if (palette_read) {
    throw BadPngStream ("cHRM chunk may not occur after PLTE");
  }
  if (data_read) {
    throw BadPngStream ("cHRM chunk may not occur after IDAT");
  }

  if (inputstream.getChunkDataLength () != sizeof (PngChromaticitiesData)) {
    throw BadPngStream ("Invalid cHRM chunk length");
  }

  /*
  const PngChromaticitiesData *data =
    reinterpret_cast <const PngChromaticitiesData *> (inputstream.getChunkData ());
  */

  /*
  uint32 whitepointx = BigEndianToSystem (data->whitepointx);
  uint32 whitepointy = BigEndianToSystem (data->whitepointy);
  uint32 redx = BigEndianToSystem (data->redx);
  uint32 redy = BigEndianToSystem (data->redy);
  uint32 greenx = BigEndianToSystem (data->greenx);
  uint32 greeny = BigEndianToSystem (data->greeny);
  uint32 bluex = BigEndianToSystem (data->bluex);
  uint32 bluey = BigEndianToSystem (data->bluey);
  */
 return;
}

//
//  Description:
//
//  This function processes an hIST chunk. This chunk defines the frequency
//  that each color in the palette is used within the imamge. This
//  information can be used to assist in quantization.
//
//  We do nothing with this chunk but dump its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processHistogram (PngInputStream &inputstream) {
  if (! palette_read) {
    throw BadPngStream ("hIST chunk may not appear before PLTE");
  }
  if (data_read) {
    throw BadPngStream ("hIST chunk must appear before IDAT");
  }

  if (inputstream.getChunkDataLength ()  != 2 * palette_size) {
    throw BadPngStream ("Bad size for hIST chunk");
  }

  // const uint16 *values = reinterpret_cast <const uint16 *> (inputstream.getChunkData ());


  return;
}

//
//  Description:
//
//  This function processes the pHYs chunk. This chunk defines the
//  dimensions for the image pixels.
//
//  We do nothing with this chunk except print its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processPhysicalPixelDimensions (PngInputStream &inputstream)
{
  if (data_read) {
    throw BadPngStream ("pHYS chunk must come before IDAT chunks");
  }

  if (inputstream.getChunkDataLength () != sizeof (PngPixelDimensions)) {

    throw BadPngStream ("pHYs chunk size invalid");

  }

  const PngPixelDimensions *pd
          = reinterpret_cast <const PngPixelDimensions *> (inputstream.getChunkData ());
  if (pd->unit > 1) {
    throw BadPngStream ("pHYs contains an invalid unit");
  }

  //static const char *unitstrings [2] = { "unknown unit", "meter" };
  return;
}

//
//  Description:
//
//  This function processes the sBIT chunk. This chunk can be used to
//  set the number of significant bits used in color values.
//
//  We do nothing with this chunk but print its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processSignificantBits (PngInputStream &inputstream) {
  if (data_read) {
    throw BadPngStream ("sBIT chunk must occur before IDAT chunks");
  }
  if (palette_read) {
    throw BadPngStream ("sBIT chunk must occur before PTLE chunk");
  }

  //const ubyte *chunkdata = inputstream.getChunkData ();

  switch (image_color_type) {
    case Grayscale: {
      if (inputstream.getChunkDataLength () < 1) {
          throw BadPngStream ("sBIT chunk length invalid");
      }

      // cout << "  Significant Bits: " << static_cast <int>(chunkdata [0]) << endl;
    } // case
    break;

    case Palette:
    case RGB: {
      if (inputstream.getChunkDataLength () < 3) {
        throw BadPngStream ("sBIT chunk length invalid");
      }

    }
    break;

    case GrayscaleAlpha: {
      if (inputstream.getChunkDataLength () < 2) {
        throw BadPngStream ("sBIT chunk length invalid");
      }
    }
    break;

    case RGBAlpha: {
      if (inputstream.getChunkDataLength () < 4) {
        throw BadPngStream ("sBIT chunk length invalid");
      }
    }
    break;

    default:
      throw BadPngStream ("Invalid Color Type");
  }
  return;
}

//
//  Description:
//
//  This function processes the tEXt chunk. This chunk stores text
//  information in the image.
//
//  We do nothing with the chunk except print its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processTextualData (PngInputStream &inputstream) {
  bool end_found = false;
  unsigned int offset;
  unsigned int datalength = inputstream.getChunkDataLength ();
  const ubyte *chunkdata = inputstream.getChunkData ();
  for (offset = 0;offset < datalength && offset < 80; ++ offset) {
    if (chunkdata [offset] == '\000') {
      end_found = true;
      break;
    } // if
  }  // for

  if (! end_found) {
    throw BadPngStream ("tEXt keyword not found");
  }
  return;
}

//
//  Description:
//
//  This function processes the tIME chunk. This chunk stores the last time
//  the image was modified.
//
//  We do nothing with the chunk except print its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processImageTime (PngInputStream &inputstream) {
  if (inputstream.getChunkDataLength () != sizeof (PngTimeData)) {
     throw BadPngStream ("tIME chunk size invalid");
  }

  const PngTimeData *td = reinterpret_cast<const PngTimeData *>(inputstream.getChunkData ());

  // the year has to be unendianized ! (Syberichs, 18.10.1999)
  uint16 theyear;

  theyear= BigEndianToSystem(td->year);

  // the other values can be taken from td stcucture

  return;
}

//
//  Description:
//
//  This function processes the tRNS chunk. This chunk allows transparency
//  to be define for color types without an alpha channel.
//
//  We do nothing with the chunk except print its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processTransparency (PngInputStream &inputstream) {
  if (data_read) {
    throw BadPngStream ("tRNS chunk cannot occur before IDAT chunks");
  }

  //const ubyte *chunkdata = inputstream.getChunkData ();
  //const uint16 *wordvalues = reinterpret_cast <const uint16 *> (chunkdata);

  switch (image_color_type) {
    case Palette: {
      if (! palette_read) {
        throw BadPngStream ("tRNS chunk must occur after PLTE chunk");
      }

      if (palette_size != inputstream.getChunkDataLength ()) {
        throw BadPngStream ("tRNS block length invalid");
      }

      //for (unsigned int ii = 0; ii < inputstream.getChunkDataLength (); ++ ii)
      //cout << ii << ") " << chunkdata [ii] << endl;
    } // case
    break;

    case Grayscale: {
      if (inputstream.getChunkDataLength () < 2) {
        throw BadPngStream ("tRNS chunk length invalid");
      }
      //cout << "  Transparency: " << BigEndianToSystem (wordvalues [0]) << endl;
    }
    break;

    case RGB: {
      if (inputstream.getChunkDataLength () < 6) {
        throw BadPngStream ("tRNS chunk length invalid");
      }
      /*
      cout << "  Red Transparency: "
           << BigEndianToSystem (wordvalues [0]) << endl;
      cout << "  Green Transparency: "
           << BigEndianToSystem (wordvalues [1]) << endl;
      cout << "  Blue Transparency: "
           << BigEndianToSystem (wordvalues [2]) << endl;
      */
    }
    break;

    default:
      throw BadPngStream ("Invalid Color Type of tRNS chunk");
  }
  return;
}

//
//  Description:
//
//  This function processes the zTXt chunk. This chunk stores text
//  information.  This chunk is similar to a tEXt chunk except that the
//  data is compressed.
//
//  We do nothing with the chunk except print its contents.
//
//  Parameters:
//
//  inputstream : The stream to read the chunk data from
//  image : The output image
//
void PngDecoder::processCompressedText (PngInputStream &inputstream) {
  bool end_found = false;
  unsigned int offset;
  const ubyte *chunkdata = inputstream.getChunkData ();
  unsigned int datalength = inputstream.getChunkDataLength ();

  for (offset = 0; offset < datalength && offset < 80; ++ offset) {
    if (chunkdata [offset] != '\000') {
      end_found = true;
      break;
    }
  }
  if (! end_found) {
    throw BadPngStream ("zEXt keyword not found");
  }

  /*
  cout << "Keyword: '" << chunkdata << "'" << endl;
  cout << "Compression Method: " << chunkdata [offset + 1] << endl;
  */
  return;
}

//
//  Description:
//
//  This function reads a PNG file.
//
//  Parameters:
//
//  filename : The name of the file to read.
//  image : The image object to read into.
//
void PngDecoder::readImageFile (const std::string &filename, image &theimage) {

  PngInputFileStream input;
  input.open (filename);
  if (!input) {
    throw PngError ("Can't open input file");
  }
  readImage (input, theimage);
  // next line added by LTIlib authors
  input.close();
  return;
}

void PngDecoder::readImageFile (std::ifstream &file, image &theimage) {

  PngInputFileStream input;
  input.open (file);
  if (!input) {
    throw PngError ("Can't open input file");
  }
  readImage (input, theimage);
  // next line added by LTIlib authors
  input.close();
  return;
}

void PngDecoder::readImageFile (const std::string &filename,
                                channel8 &theChannel,
                                lti::palette& colors) {

  PngInputFileStream input;
  input.open (filename);
  if (!input) {
    throw PngError ("Can't open input file");
  }
  readImage (input, theChannel, colors);
  // next line added by LTIlib authors
  input.close();
  return;
}

bool PngDecoder::checkHeader(const std::string& filename,
                            point& imageSize,
                            int& bitsPerPixel,
                            int& imageType) {
  header_read=false;

  PngInputFileStream inputstream;
  inputstream.open (filename);


  try  {
    readSignature (inputstream);

    inputstream.getNextChunk ();

    if (! matchChunks (inputstream.getChunkType (), "IHDR"))
      {inputstream.close();return false;}
    processHeader (inputstream);
    imageSize=point(image_width,image_height);
    bitsPerPixel=image_depth;
    imageType=image_color_type;

    freeData ();
  }
  catch (...)
  {
    freeData ();
    throw;
  }

  if (! data_read) {inputstream.close();return false;}

  inputstream.close();
  return true;
}


  } // namespace png
} // namespace lti

