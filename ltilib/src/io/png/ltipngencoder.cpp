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
 * file .......: ltipngencoder.cpp
 * authors ....: John M. Miano, miano@colosseumbuilders.com
 *               Stefan Syberichs, syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 02.11.99
 * description.: PNG encoder class definition
 * revisions ..: $Id: ltipngencoder.cpp,v 1.7 2003/02/17 07:16:14 alvarado Exp $
 */

#include <string>
#include "ltipngencoder.h"
//#include "ltipngpvt.h"
#include "ltipngsystemspecific.h"
#include "ltipngoutputstream.h"
#include "ltiAssert.h"
#include "ltipngendian.h"

namespace lti{
  namespace png {

//
//  Description:
//
//  Class Default Constructor
//

PngEncoder::PngEncoder (): filter_mask ((1 << FILTERNONE)), row_buffer (0) {
  memset (filter_buffers, 0, sizeof (filter_buffers));
  return;
}

//
//  Description:
//
//  Class Copy Constructor
//
PngEncoder::PngEncoder (const PngEncoder &source)
  :title_string (source.title_string),
   author_string (source.author_string),
   description_string (source.description_string),
   copyright_string (source.copyright_string),
   software_string (source.software_string),
   disclaimer_string (source.disclaimer_string),
   warning_string (source.warning_string),
   source_string (source.source_string),
   comment_string (source.comment_string),
   filter_mask (source.filter_mask),
   row_buffer (0) {
  memset (filter_buffers, 0, sizeof (filter_buffers));
  return;
}

//
//  Description:
//
//  Class Destructor
//
PngEncoder::~PngEncoder(void) {
  freeBuffers ();
  return;
}

//
//  Descriptor:
//
//  Assignment Operator
//
//  Parameters:
//  source: The object to copy
//
PngEncoder &PngEncoder::operator=(const PngEncoder &source) {
  if (this == &source) {
    return *this;
  }

  title_string = source.title_string;
  author_string = source.author_string;
  description_string = source.description_string;
  copyright_string = source.copyright_string;
  software_string = source.software_string;
  disclaimer_string = source.disclaimer_string;
  warning_string = source.warning_string;
  source_string = source.source_string;
  comment_string = source.comment_string;
  filter_mask = source.filter_mask;

  return *this;
}

//
//  Description:
//
//  This function writes an image to a PNG stream.
//
//  Parameters:
//  strm:  The output stream
//  image:  The image to output
//
void PngEncoder::writeImage (PngOutputStream &outputstream,
                             const image     &theimage,
                             PngEncoder::pngColorID       cid) {
  filter_mask |= (1 << FILTERNONE); // Require the none filter.
  try
  {
    doWrite (outputstream, theimage, cid);
  }
  catch (DeflateEncoder::DeflateError &error)
  {
    freeBuffers ();
    throw PngError (error.what ());
  }
  catch (...)
  {
    freeBuffers ();
    throw;
  }
  freeBuffers ();
  return;
}

void PngEncoder::writeImage (PngOutputStream &outputstream,
                             const channel8& theChannel,
                             const lti::palette& colors,
                             PngEncoder::pngColorID cid) {
  filter_mask |= (1 << FILTERNONE); // Require the none filter.
  try
  {
    doWrite (outputstream, theChannel, colors, cid);
  }
  catch (DeflateEncoder::DeflateError &error)
  {
    freeBuffers ();
    throw PngError (error.what ());
  }
  catch (...)
  {
    freeBuffers ();
    throw;
  }
  freeBuffers ();
  return;
}


//
//  Description:
//
//  This function frees the buffers allowed during the encoding
//  process.
//
void PngEncoder::freeBuffers (void) {
  for (unsigned int ii = 0; ii < FILTERBUFFERCOUNT; ++ ii) {
    delete [] filter_buffers [ii]; filter_buffers [ii] = 0;
  }
  delete [] row_buffer; row_buffer = 0;
  return;
}


//
//  Description:
//
//  This function creates a tEXt chunk.
//
//  Parameters:
//  keyword:  The chunk keyword
//  value:  The keyword value
//
void PngEncoder::writeText (PngOutputStream &outputstream,
                            const std::string &keyword,
                            const std::string &value) {
  if (keyword.length () > 79) {
    throw PngError ("tEXt Keyword Too Long");
  }

  outputstream.startChunk ("tEXt");
  outputstream.write (keyword.c_str (), keyword.length ());
  outputstream.writeByte (0);
  if (value.length () > outputstream.remainingBufferSpace ()) {
    throw PngError ("tEXt Value Too Long");
  }

  outputstream.write (value.c_str (), value.length ());
  outputstream.endChunk ();
  return;
}

//
//  Description:
//
//  This function outputs tEXt blocks for any keywords
//  that have values assigned to them.
//
//  Parameters:
//
//  outputstream : The stream to write to
//
void PngEncoder::writeTextBlocks (PngOutputStream &outputstream) {
  if (title_string != "") {
    writeText (outputstream, "Title", title_string);
  }
  if (author_string != "") {
    writeText (outputstream, "Author", author_string);
  }
  if (description_string != "") {
    writeText (outputstream, "Description", description_string);
  }
  if (copyright_string != "") {
    writeText (outputstream, "Copyright", copyright_string);
  }
  if (software_string != "") {
    writeText (outputstream, "Software", software_string);
  }
  if (disclaimer_string != "") {
    writeText (outputstream, "Disclaimer", disclaimer_string);
  }
  if (warning_string != "") {
    writeText (outputstream, "Warning", warning_string);
  }
  if (source_string != "") {
    writeText (outputstream, "Source", source_string);
  }
  if (comment_string != "") {
    writeText (outputstream, "Comment", comment_string);
  }
  return;
}

//
//  Description:
//
//  This function sets the compression level used.  The compression level
//  determines the depth to which hash chains are searched.
//
//  Parameters:
//
//  value:  The new compression level
//
void PngEncoder::setCompressionLevel (DeflateEncoder::CompressionLevel value) {
  deflate_encoder.setCompressionLevel (value);
  return;
}

//
//  Description:
//
//  The function returns the current compression level.
//
//  Return Value:
//
//  The compresion level.
//
DeflateEncoder::CompressionLevel PngEncoder::getCompressionLevel () const {
  DeflateEncoder::CompressionLevel result =   deflate_encoder.getCompressionLevel ();
  return result;
}

//
//  Description:
//
//  This function performs the actual output of the image.
//
//  Parameters:
//
//  outputstream : The stream to write the image to
//  image : The image to output
//  cid : color ID, at the moment only RGB24 (!), to be extended in future...
//
void PngEncoder::doWrite (PngOutputStream &outputstream,
                          const image &theimage,
                          PngEncoder::pngColorID cid) {

  unsigned int ii;

  if (cid == RGB24) {
    row_width = 3 * theimage.columns();
    filter_width = 3;
  } else if (cid == RGBalpha) {
    row_width=4*theimage.columns();
    filter_width=4;
  } else   {
   /*
    row_width = (theimage.getBitCount () * theimage.columns()
                 + CHAR_BIT - 1) / CHAR_BIT;
    filter_width = 1;
   */
  }
  row_buffer = new ubyte [row_width];

  for (ii = 0; ii < FILTERBUFFERCOUNT; ++ ii) {
    if (((1 << ii) & filter_mask) != 0)
      filter_buffers [ii] = new ubyte [row_width];
  }

  memset (filter_buffers [FILTERNONE], 0, row_width);

  // Fill in the image header.
  PngImageHeader header;
  header.width = SystemToBigEndian (static_cast<uint32>(theimage.columns ()));
  header.height = SystemToBigEndian (static_cast<uint32>(theimage.rows()));
  switch(cid)   {
    case RGBalpha   :
      case RGB24      : header.bitdepth=24; break;
      case Gray8      :;
      case Grayalpha8 :;
      case Palette8   : header.bitdepth = 8; break;
      case Gray4      :;
      case Palette4   : header.bitdepth = 4; break;
      case Gray2      :;
      case Palette2   : header.bitdepth = 2; break;
      case Blackwhite :;
      case Palette1   : header.bitdepth = 1; break;
      default         : header.bitdepth = 8;
    }

  // Ersatz für getColorCount wird benötigt !
  // vortläufig ist was anderes als Truecolor nicht nötig !
  if (header.bitdepth != 24) {
    /*
    // Determine if the image is grayscale.
    bool isgrayscale = true;
    for (unsigned int ii = 0; ii < image.getColorCount (); ++ ii)
    {
      if (image.getColorMap (ii).red != image.getColorMap (ii).green
          || image.getColorMap (ii).red != image.getColorMap (ii).blue)
      {
        isgrayscale = false;
        break;
      }
    }

    if (isgrayscale)
      header.colortype = Grayscale;
    else
      header.colortype = Palette;
    */
  } else  {
    // Korrektur der Bittiefe auf 8 Bit pro Kanal = Nomenklatur für PNG
    header.bitdepth = 8;
    header.colortype = RGB;
  }

  if (cid == RGBalpha) {
    header.colortype=RGBAlpha;
  }

  header.compressionmethod = 0;
  header.filtermethod = 0;
  header.interlacemethod = 0;

  // Output the PNG Signature and header.
  outputstream.writeRaw (reinterpret_cast<const char*>(PngSignature),
                         PngSignatureSize);
  outputstream.startChunk ("IHDR");
  outputstream.write (reinterpret_cast<const char*>(&header), sizeof (header));
  outputstream.endChunk ();

  // Output any text blocks with keywords defined.
  writeTextBlocks (outputstream);

  // Output the palette if it is required.
  // gibts im Moment noch nicht !!!
  /*
  if (header.colortype == Palette)
  {
    outputstream.startChunk ("PLTE");
    for (unsigned int ii = 0; ii < image.getColorCount (); ++ ii)
    {
      outputstream.writeByte (image.getColorMap (ii).red);
      outputstream.writeByte (image.getColorMap (ii).green);
      outputstream.writeByte (image.getColorMap (ii).blue);
    }
    outputstream.endChunk ();
  }
  */

  // hier werden die Bilddaten geschrieben, chunk IDAT
  writeImageData (outputstream, theimage, cid);

  // Write the IEND marker.
  outputstream.startChunk ("IEND");
  outputstream.endChunk ();

  return;
}


void PngEncoder::doWrite (PngOutputStream &outputstream,
                          const channel8& theChannel,
                          const lti::palette& colors,
                          PngEncoder::pngColorID cid) {

  unsigned int ii;

  if (cid == RGB24) {
    row_width = 3 * theChannel.columns();
    filter_width = 3;
  }
  else   {
    int bitsPerPixel;
    switch(cid) {
      case Blackwhite:
      case Palette1:
        bitsPerPixel = 2;
        break;
      case Gray2:
      case Gray4:
      case Gray8:
      case Palette2:
      case Palette4:
      case Palette8:
      case Grayalpha8:
        bitsPerPixel = 8;
        break;
      case RGB24:
      case RGBalpha:
        bitsPerPixel = 32;
      default:
        bitsPerPixel = 8;
    }
    row_width = (bitsPerPixel*theChannel.columns()+CHAR_BIT-1) / CHAR_BIT;
    filter_width = 1;
  }

  row_buffer = new ubyte [row_width];

  for (ii = 0; ii < FILTERBUFFERCOUNT; ++ ii) {
    if (((1 << ii) & filter_mask) != 0)
      filter_buffers [ii] = new ubyte [row_width];
  }

  memset (filter_buffers [FILTERNONE], 0, row_width);

  // Fill in the image header.
  PngImageHeader header;
  header.width = SystemToBigEndian(static_cast<uint32>(theChannel.columns()));
  header.height = SystemToBigEndian(static_cast<uint32>(theChannel.rows()));
  switch(cid)   {
      case RGB24      : header.bitdepth=24; break;
      case Gray8      :;
      case Grayalpha8 :;
      case Palette8   : header.bitdepth = 8; break;
      case Gray4      :;
      case Palette4   : header.bitdepth = 4; break;
      case Gray2      :;
      case Palette2   : header.bitdepth = 2; break;
      case Blackwhite :;
      case Palette1   : header.bitdepth = 1; break;
      default         : header.bitdepth = 8;
    }

  // Ersatz für getColorCount wird benötigt !

  // Check out if the channel is a grayscale image or a color image
  if (header.bitdepth != 24) {
    // Determine if the image is grayscale.
    bool isgrayscale = true;
    int ii;
    for (ii = 0; ii < colors.size(); ++ ii) {
      if ((colors.at(ii).getRed() != colors.at(ii).getGreen()) ||
          (colors.at(ii).getGreen() != colors.at(ii).getBlue())) {
        isgrayscale = false;
        break;
      }
    }

    if (isgrayscale)
      header.colortype = Grayscale;
    else
      header.colortype = Palette;
  } else  {
    // Korrektur der Bittiefe auf 8 Bit pro Kanal = Nomenklatur für PNG
    header.bitdepth = 8;
    header.colortype = RGB;
  }

  header.compressionmethod = 0;
  header.filtermethod = 0;
  header.interlacemethod = 0;

  // Output the PNG Signature and header.
  outputstream.writeRaw (reinterpret_cast<const char*>(PngSignature),
                         PngSignatureSize);
  outputstream.startChunk ("IHDR");
  outputstream.write (reinterpret_cast<const char*>(&header), sizeof (header));
  outputstream.endChunk ();

  // Output any text blocks with keywords defined.
  writeTextBlocks (outputstream);

  // Output the palette if it is required.
  if (header.colortype == Palette)  {
    outputstream.startChunk ("PLTE");
    int ii;
    for (ii = 0; ii < colors.size(); ++ ii) {
      outputstream.writeByte (colors.at(ii).getRed());
      outputstream.writeByte (colors.at(ii).getGreen());
      outputstream.writeByte (colors.at(ii).getBlue());
    }
    outputstream.endChunk ();
  }

  // hier werden die Bilddaten geschrieben, chunk IDAT
  writeImageData (outputstream, theChannel, colors, cid);

  // Write the IEND marker.
  outputstream.startChunk ("IEND");
  outputstream.endChunk ();

  return;

}


//
//  Description:
//
//  This function filters an image row.
//
//  Implicit Inputs
//
//  row_buffer contains the row to filter
//  filter_buffers [FILTERNONE] contains the last row
//
void PngEncoder::filterRow ()
{
  assert(filter_mask != 0);

  // We need this because MSVC++ does not follow standard
  // scoping rules in for statements.
  unsigned int ii;

  unsigned int mask = (1 << FILTERNONE);



  // Filter for each type in the filter_mask.
  if ((filter_mask & (1 << FILTERSUB)) != 0) {
    mask |= (1 << FILTERSUB);

    ubyte last;
    for (unsigned int ii = 0; ii < row_width; ++ ii) {
      if (ii >= filter_width) {
        last = row_buffer [ii-filter_width];
      }
      else {
        last = 0;
      }

      filter_buffers [FILTERSUB][ii] = row_buffer [ii] - last;
    } // for
  } // if

  if ((filter_mask & (1 << FILTERUP)) != 0) {
    mask |= (1 << FILTERUP);

    for (unsigned int ii = 0; ii < row_width; ++ ii) {
      filter_buffers [FILTERUP][ii] =
        row_buffer [ii] - filter_buffers [FILTERNONE][ii];
    }
  }

  if ((filter_mask & (1 << FILTERAVERAGE)) != 0)  {

    mask |= (1 << FILTERAVERAGE);
    ubyte last;
    ubyte above;
    for (unsigned int ii = 0; ii < row_width; ++ ii) {
      if (ii >= filter_width) {
        last = row_buffer [ii - filter_width];
      }
      else {
        last = 0;
      }

      above = filter_buffers [FILTERNONE][ii];
      filter_buffers [FILTERAVERAGE][ii] =
        row_buffer [ii] - (above + last) / 2;
    } // for
  } // if

  if ((filter_mask & (1 << FILTERPAETH)) != 0) {

    mask |= (1 << FILTERPAETH);
    ubyte last;
    ubyte lastabove;
    ubyte above;

    for (unsigned int ii = 0; ii < row_width; ++ ii) {
      if (ii >= filter_width) {
        last = row_buffer [ii-filter_width];
        lastabove = filter_buffers [FILTERNONE][ii - filter_width];
      }
      else {
        last = 0;
        lastabove = 0;
      }

      above = filter_buffers [FILTERNONE][ii];
      filter_buffers [FILTERPAETH][ii] =
        row_buffer [ii] - PaethPredictor (last, above, lastabove);
    } // for
  } // if


  // Filter None, The last row is nopt filtered but only copied
  // THIS MUST BE THE LAST FILTER!!!!!!!!!! We save the value
  // here to be used in the next call with the filters that require data
  // from the previous row.
  {
    for (unsigned int ii = 0; ii < row_width; ++ ii) {
      filter_buffers [FILTERNONE][ii] = row_buffer [ii];
    }
  }

  // If we only performed FilterNone then we do not need to proceed
  // any further.
  current_filter = FILTERNONE;

  if (mask == (1 << FILTERNONE)) {
    return;
  }

  // Find the best filter. We do a simple test for the
  // longest runs of the same value.

  unsigned int longestrun = 0;
  for (ii = 0; ii < FILTERBUFFERCOUNT; ++ ii) {
    if ((mask & (1<<ii)) != 0) {
      unsigned int run = 0;
      for (unsigned int jj = 4; jj < row_width; ++ jj) {
        if (filter_buffers [ii][jj] == filter_buffers [ii][jj-1]
            && filter_buffers [ii][jj] == filter_buffers [ii][jj-2]
            && filter_buffers [ii][jj] == filter_buffers [ii][jj-3]
            && filter_buffers [ii][jj] == filter_buffers [ii][jj-4])
        {
          ++ run;
        }
      } // for

      if (run > longestrun) {
        current_filter = ii;
        longestrun = run;
      }
    } // if
  } // for
  return;
}

//
//  Description:
//
//  This function specifies whether or not filters are used.
//
//  Parameters:
//
//  value: true=>Use Filters, false=>Don't use filters
//
void PngEncoder::setUseFilters (bool value) {
  if (value) {
    filter_mask = 0xFFFFFFFF;
  }
  else {
    filter_mask = (1<<FILTERNONE);
  }
}

//
//  Description:
//
//  This function tells if filters are being used.
//
//  Return Value:
//  true=>Filters are being used
//  false=>Filters are not being used
//
bool PngEncoder::getUseFilters () const
{
  if (filter_mask == 0x1) {
    return false;
  }
  else {
    return true;
  }
}

//
//  Description:
//
//  This function writes the image data to the output stream.
//
//  Parameters:
//
//  outputstream : The stream to write to
//  image : The image to output
//  cid : color ID, only RGB24 at the moment (!), to be extended in future
//
void PngEncoder::writeImageData(PngOutputStream &outputstream,
                                const image &theimage,
                                PngEncoder::pngColorID cid) {

  outputstream.startChunk ("IDAT");

  // start a new compressed stream, initialize tables etc.
  deflate_encoder.startCompressedStream (outputstream);
  if (cid == RGB24) {
    for (int ii = 0; ii < theimage.rows(); ++ ii) {
      const vector<rgbPixel> *row = &theimage[ii];

      int kk, jj;

      // load rowbuffer with image data
      for(kk=0, jj=0; jj < theimage.columns(); jj++, kk+=3) {
        row_buffer [kk] = (*row)[jj].getRed(); // Redofset=2
        row_buffer [kk+1] = (*row) [jj].getGreen(); // Greenoffset=1
        row_buffer [kk+2] = (*row)[jj].getBlue(); // Blueoffset=0
      }


      // filters one row of theimage
      filterRow ();
      ubyte filter = current_filter;

      //
      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(&filter),
                                    sizeof (filter));

      //
      deflate_encoder.compressData (outputstream,
        reinterpret_cast<const char*>(filter_buffers [current_filter]),
        row_width);

    } // for
  }// if RGB24
  else if (cid == RGBalpha) {
    for (int ii = 0; ii < theimage.rows(); ++ ii) {
      const vector<rgbPixel> *row = &theimage[ii];

      int kk, jj;

      // load rowbuffer with image data
      for(kk=0, jj=0; jj < theimage.columns(); jj++, kk+=4) {
        row_buffer [kk] = (*row)[jj].getRed(); // Redofset=2
        row_buffer [kk+1] = (*row) [jj].getGreen(); // Greenoffset=1
        row_buffer [kk+2] = (*row)[jj].getBlue(); // Blueoffset=0
        row_buffer [kk+3] = (*row)[jj].getDummy(); // Blueoffset=0
      }


      // filters one row of theimage
      filterRow ();
      ubyte filter = current_filter;

      //
      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(&filter),
                                    sizeof (filter));

      //
      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(filter_buffers [current_filter]),
                                    row_width);

    } // for
  } // if RGBalpha
  else {
    /*
    for (unsigned int ii = 0; ii < theimage.rows (); ++ ii) {
      ubyte *row = theimage [ii];
      static const ubyte filter = 0;
      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(&filter),
                                    sizeof (filter));
      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(row),
                                    row_width);

    }
    */
  }

  deflate_encoder.endCompressedStream (outputstream);
  outputstream.endChunk ();
  return;

}

void PngEncoder::writeImageData(PngOutputStream &outputstream,
                                const channel8& theChannel,
                                const lti::palette& colors,
                                PngEncoder::pngColorID cid) {

  outputstream.startChunk ("IDAT");

  // start a new compressed stream, initialize tables etc.
  deflate_encoder.startCompressedStream (outputstream);
  if (cid == RGB24) {
    for (int ii = 0; ii < theChannel.rows(); ++ ii) {
      const vector<ubyte> *row = &theChannel[ii];

      int kk, jj;

      // load rowbuffer with image data
      for(kk=0, jj=0; jj < theChannel.columns(); jj++, kk+=3) {
        row_buffer [kk]   = colors.at((*row)[jj]).getRed(); // Redofset=2
        row_buffer [kk+1] = colors.at((*row)[jj]).getGreen(); // Greenoffset=1
        row_buffer [kk+2] = colors.at((*row)[jj]).getBlue(); // Blueoffset=0
      }

      // filters one row of theimage
      filterRow ();
      ubyte filter = current_filter;

      //
      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(&filter),
                                    sizeof (filter));

      //
      deflate_encoder.compressData (outputstream,
        reinterpret_cast<const char*>(filter_buffers [current_filter]),
        row_width);

    } // for
  } // if RGB24
  else {
    int ii,jj;
    for (ii = 0; ii < theChannel.rows (); ++ ii) {
      const vector<ubyte> *row = &theChannel[ii];
      for (jj=0;jj<theChannel.columns();jj++) {
        row_buffer[jj] = row->at(jj);
      }

      filterRow();
//    static const ubyte filter = 0;
      const ubyte filter = current_filter;

      deflate_encoder.compressData (outputstream,
                                    reinterpret_cast<const char*>(&filter),
                                    sizeof (filter));
//      deflate_encoder.compressData (outputstream,
//                                    reinterpret_cast<const char*>(row),
//                                    row_width);

    deflate_encoder.compressData (outputstream,
      reinterpret_cast<const char*>(filter_buffers [current_filter]),
      row_width);


    }

  }

  deflate_encoder.endCompressedStream (outputstream);
  outputstream.endChunk ();
  return;

}

//
//  Description:
//
//  This function writes a image to a PNG file.
//
//  Parameters:
//
//  filename : The name of the output file
//  image : The image to compress
//  cid : color ID, only RGB24 at the moment (!), to be extended in future
//
void PngEncoder::writeImageFile (const std::string &filename,
                                 const image &theimage,
                                 PngEncoder::pngColorID cid) {

  PngOutputFileStream outputstream;
  outputstream.open (filename.c_str ());
  if (! outputstream) {
    throw PngError ("Can't open output file");
  }

  writeImage (outputstream, theimage, cid);
  // next line added by LTIlib authors
  outputstream.close();
  return;
}

void PngEncoder::writeImageFile (const std::string&     filename,
                                 const channel8&        theChannel,
                                 const palette&         colors,
                                 PngEncoder::pngColorID cid) {

  PngOutputFileStream outputstream;
  outputstream.open (filename.c_str ());
  if (! outputstream) {
    throw PngError ("Can't open output file");
  }

  writeImage (outputstream, theChannel, colors, cid);
  // next line added by LTIlib authors
  outputstream.close();
  return;
}


//
//  Description:
//
//  This function defines the block size used by the Deflate encoder.
//
//  Parameters:
//
//  value : The new block size
//

void PngEncoder::setBlockSize (unsigned long value) {
  deflate_encoder.setBlockSize (value);
  return;
}
} // namespace png
} // namespace lti

