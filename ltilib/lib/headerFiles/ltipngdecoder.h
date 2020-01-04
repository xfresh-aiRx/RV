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
 * file .......: ltipngdecoder.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 25.10.99
 * description.: PNG decoder class declaration
 * revisions ..: $Id: ltipngdecoder.h,v 1.6 2003/02/17 07:16:13 alvarado Exp $
 */


#ifndef _LTI_PNGDECODER_H
#define _LTI_PNGDECODER_H

#include "ltipnginputstream.h"
#include "ltipnginflatedecoder.h"
#include "ltipngexcept.h"
#include "ltiImage.h"


namespace lti{
  namespace png {
/**
 * PNG decoder class.
 * PngDecoder loads and decodes PNG graphic files (Portable Network Graphic)
 * according to the PNG draft version 1.2. All kinds of color formats
 * and ancillary chunnks are recognized and loaded into a lti image.
 * Chunks like color histogramm, significant bits etc. are interpreted
 * but have no impact on the image appearance (only
 * informational). Members for getting this information are not yet
 * implented.
 * The library is based on a free library written by John M. Miano.
 * PngDecoder is used by the ioPNG functors.<b> Use loadPNG and
 * savePNG to handel PNG images !</b>
 *
 * @see loadPNG
 * @see savePNG
 * @see loadBMP
 * @see saveBMP
 */
class PngDecoder{
public:
  /// constrcutor
  PngDecoder ();
  /// destructor
  virtual ~PngDecoder ();

  /// reads image from openPngInputstream
  virtual void readImage (PngInputStream &inputstream, image& theimage);
  /// reads image from file (default)
  virtual void readImageFile (const std::string &filename, image& theimage);

  /// reads image from file (default)
  virtual void readImageFile (std::ifstream &file, image& theimage);

  /// reads channel8 from openPngInputstream
  virtual void readImage (PngInputStream &inputstream,
                          channel8& theChannel,
                          lti::palette& colors);
  /// reads channel8 from file (default)
  virtual void readImageFile (const std::string &filename,
                              channel8& theChannel,
                              lti::palette& colors);
  // Check Header Information
  bool checkHeader(const std::string& filename,
                 point& imageSize,
                 int&   bitsPerPixel,
                 int&   imageType);
private:

  /// structure for decoding interlaced images
  struct InterlaceInfo
  {
    unsigned int row_interval;
    unsigned int col_interval;
    unsigned int start_row;
    unsigned int start_col;
  } ;

  /// structure for color map entry
  struct ColorMapEntry
  {
    ubyte blue;
    ubyte green;
    ubyte red;
  };

  //enum colorSet {NONE = 0,RED, GREEN, BLUE};

  /// color Map for 8 bit images
  ColorMapEntry colorMap[256];

  /// constants for pointing out bytes in colorbytestreams
  static const byte REDOFFSET;
  static const byte GREENOFFSET;
  static const byte BLUEOFFSET;

  /// Function to read the PNG signature.
  void readSignature (PngInputStream &inputstream);

  /// match two chunks by signature
  bool matchChunks (const ubyte *operand1, const char *operand2);

  /// general chunk processing function
  void processChunk (PngInputStream &inputstream, image &theimage);

  /// general chunk processing function
  void processChunk (PngInputStream &inputstream,
                     channel8 &theChannel,
                     lti::palette& colors);

  /// decode header: size, color depth,coding scheme etc.
  void processHeader (PngInputStream &inputstream, image &theimage);
  /// decode header: size, color depth,coding scheme etc.
  void processHeader (PngInputStream &inputstream,
                      channel8 &theChannel,
                      lti::palette& colors);
  /// decode header: size, color depth,coding scheme etc.
  void processHeader (PngInputStream &inputstream);




  /// decode palette chunk: build color map from chunk data
  void processPalette (PngInputStream &inputstream, image &theimage);

  /// decode palette chunk: build color map from chunk data
  void processPalette (PngInputStream &inputstream,
                       channel8& theChannel,
                       lti::palette& colors);


  /// decode Background: extract background color
  void processBackground (PngInputStream &inputstream);

  /// decode file gama value
  void processGamma (PngInputStream &inputstream);

  // decode actual image data (i.e. the bitmap)
  // void processData (PngInputStream &inputstream, image &theimage);

  /// decode chromaticity values (only informational)
  void processChromaticities (PngInputStream &inputstream);

  /// decode color histogramm (only informational)
  void processHistogram (PngInputStream &inputstream);

  /// decode physical pixel dimensions (only informational)
  void processPhysicalPixelDimensions (PngInputStream &inputstream);

  ///  decode significant bits chunk (only informational)
  void processSignificantBits (PngInputStream &inputstream);

  ///  decode Textual data chunk
  void processTextualData (PngInputStream &inputstream);

  ///  decode time chunk
  void processImageTime (PngInputStream &inputstream);

  /// decode global transparency value (instead of alpha channel)
  void processTransparency (PngInputStream &inputstream);

  /// decode compressed text chunk
  void processCompressedText (PngInputStream &inputstream);

  /// release denymic memory allocated during decode
  void freeData ();

  /// reading pixel data for the image.
  void readPixelData (PngInputStream &inputstream, image &theimage);
  /// reading pixel data for the image.
  void readPixelData (PngInputStream &inputstream,
                      channel8 &theChannel,
                      lti::palette& colors);

  /// copy rows of non-interlaced image to buffer
  void copyNoninterlacedRowToImage (unsigned int row, image &theimage);
  /// copy rows of non-interlaced image to buffer
  void copyNoninterlacedRowToImage (unsigned int row, channel8 &theChannel);

  /// copy rows of interlaced image to buffer
  void copyInterlacedRowToImage (unsigned int row, unsigned int width,
                                 image &theimage) ;
  /// copy rows of interlaced image to buffer
  void copyInterlacedRowToImage (unsigned int row, unsigned int width,
                                 channel8& theChannel) ;


  /// read non-interlaced image from stream
  void readNoninterlaced (PngInputStream &inputstream, image &theimage);
  /// read non-interlaced image from stream
  void readNoninterlaced (PngInputStream &inputstream,
                          channel8 &theChannel);

  /// read interlaced image from stream
  void readInterlaced (PngInputStream &inputstream, image &theimage);
  /// read interlaced image from stream
  void readInterlaced (PngInputStream &inputstream,
                       channel8 &theChannel);

  /// filter current row in buffer
  void filterRow (unsigned int filter);

  /// Background Color Values
  ubyte background_red;
  ubyte background_green;
  ubyte background_blue;
  ubyte background_gray;

  //bool verbose_flag ;

  /// actual size of color map
  unsigned int palette_size;

  /// is set to TRUE while reading IDAT blocks
  bool reading_pixel_data;

  /// IDAT read state: bit offset into current byte.
  unsigned int bit_offset;
  /// IDAT read state: Byte offset into current IDAT chunk
  unsigned int byte_offset;
  /// IDAT read state
  ubyte bit_buffer;

  // Image information from the header
  /// image height as taken from header chunk
  uint32 image_height;
  /// image width taken from header chunk
  uint32 image_width;
  /// color depth as taken from header chunk (bits per channel !!!)
  ubyte image_depth;
  /// color type as taken from header chunk
  unsigned int image_color_type;
  /// compressions method as taken from header chunk (0)
  ubyte image_compression_method;
  /// filter method as taken from header chunk (0)
  ubyte image_filter_method;
  /// interlace method as taken from header chunk (0 or 1)
  ubyte image_interlace_method;

  /// set of 2 row input buffers
  ubyte *row_buffers [2];
  /// index for current buffer
  int current_row_buffer;
  /// size of row buffer in byte
  unsigned int row_buffer_width;

  /// current interlace pass
  unsigned int interlace_pass;
  /// value from file gamma chunk
  double file_gamma;

  // Values used to parse the chunk ordering.
  /// false until IDAT chunk read
  bool data_read;
  /// false until Palette chunk read
  bool palette_read;
  /// false until Header chunk read
  bool header_read;
  /// false until End chunk read
  bool end_read;

  /// Number of passes in an interlaced image.
  enum { InterlacePasses = 7 };

  /// interlace pattern
  static const InterlaceInfo interlace_values [] ;

  /// instance of infalte decoder class
  InflateDecoder inflate_decoder ;
} ;


  } // namespace png
} // namespace lti

#endif

