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
 * file .......: ltipngencoder.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 28.10.99
 * description.: PNG encoder class declaration
 * revisions ..: $Id: ltipngencoder.h,v 1.5 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_PNGENCODER_H
#define _LTI_PNGENCODER_H

#include "ltipngdeflateencoder.h"
#include <string>
#include <iostream>
#include "ltiImage.h"
#include "ltipngexcept.h"
#include "ltipngoutputstream.h"

namespace lti {
  namespace png {

  /**
   * Png Encoder class.
   *
   * PngEncoder encodes and stores PNG graphic files (Portable Network
   * Graphic) according to the PNG draft  version 1.2. Lti images can only
   * be stored as 24 bit RGB PNG so far. A color quantisation in the
   * savePNG functor could be usefull to support also the palette and
   * greyscale features of the PNG draft.(not yet implemented) The library
   * is based on a free library written by John M. Miano.
   *
   * PngEncoder is used by the ioPNG Functor.
   * <b> Use savePNG and loadPNG to handel PNG images !</b>
   *
   * @see ioPNG
   * @see savePNG
   * @see loadPNG
   * @see ioBMP
   */
  class PngEncoder {
  public:

    /**
     * Color palette for the encoder
     */
    enum pngColorID {
      Blackwhite=0, /*!<  1 bit per pixel (gray) */
      Gray2,        /*!<  2 bit per pixel (gray) */
      Gray4,        /*!<  4 bit per pixel (gray) */
      Gray8,        /*!<  8 bit per pixel (gray) */
      Palette1,     /*!<  1 bit per pixel */
      Palette2,     /*!<  2 bit per pixel */
      Palette4,     /*!<  4 bit per pixel */
      Palette8,     /*!<  8 bit per pixel */
      RGB24,        /*!< 24 bit per pixel */
      RGBalpha,     /*!< 32 bit per pixel */
      Grayalpha8    /*!<  8 bit per pixel (gray) */
    };

    /**
     * constructor
     */
    PngEncoder ();
    /**
     * copy destructor
     */
    PngEncoder (const PngEncoder &source);
    /**
     * destructor
     */
    ~PngEncoder ();

    /**
     * stores image in file named "filename" (standard)
     */
    void writeImageFile (const std::string &filename,
                         const image       &theimage,
                               pngColorID   cid);

    /**
     * writes image to open PngOutputStream
     */
    void writeImage (PngOutputStream &, const image&, pngColorID cid);


    /**
     * stores image in file named "filename" (standard)
     */
    void writeImageFile (const std::string &filename,
                         const channel8    &theimage,
                         const lti::palette &colors,
                               pngColorID   cid);

    /**
     * writes image to open PngOutputStream
     */
    void writeImage (PngOutputStream    &outStream,
                     const channel8     &theimage,
                     const lti::palette &colors,
                           pngColorID   cid);

    /**
     * assignement operator
     */
    PngEncoder &operator=(const PngEncoder &source);

    // Property functions for predefined tEXt strings.

    /**
     * set title string in text chunk
     */
    void setTitle (const std::string &);
    std::string getTitle () const;

    /**
     * set author string in text chunk
     */
    void setAuthor (const std::string &);
    std::string getAuthor () const;

    /**
     * set description string in text chunk
     */
    void setDescription (const std::string &);
    std::string getDescription () const;

    /**
     * set copyright string in text chunk
     */
    void setCopyright (const std::string &);
    std::string getCopyright () const;

    /**
     * set software string in text chunk
     */
    void setSoftware (const std::string &);
    std::string getSoftware () const;

    /**
     * set disclaimer string in text chunk
     */
    void setDisclaimer (const std::string &);
    std::string getDisclaimer () const;

    /**
     * set warning string in text chunk
     */
    void setWarning (const std::string &);
    std::string getWarning () const;

    /**
     * set source string in text chunk
     */
    void setSource (const std::string &);
    std::string getSource () const;

    /**
     * set comment string in text chunk
     */
    void setComment (const std::string &);
    std::string getComment () const;

    /**
     * set compression level for encoder
     */
    void setCompressionLevel (DeflateEncoder::CompressionLevel);
    DeflateEncoder::CompressionLevel getCompressionLevel () const;

    /**
     * initiate use of compression filters
     */
    void setUseFilters (bool);

    /**
     * determine use of compression filters
     */
    bool getUseFilters () const;

    /**
     * determine block size
     */
    unsigned long getBlockSize () const;

    /**
     * set block size
     */
    void setBlockSize (unsigned long);


  protected:

    /**
     * write raw image data (IDAT chunk only) to open PngOutputStream
     */
    void writeImageData (PngOutputStream &,
                         const image & themage,
                         pngColorID cid);

    void writeImageData (PngOutputStream &,
                         const channel8& theChannel,
                         const lti::palette& colors,
                         pngColorID cid);



    /**
     * free dynamic memory used in PngEncoder
     */
    void freeBuffers ();

    /**
     * write raw text message to open output stream
     */
    void writeText (PngOutputStream &outputstream,
                    const std::string &keyword,
                    const std::string &value);

    /**
     * write raw text block
     */
    void writeTextBlocks (PngOutputStream &outputstream);

    /**
     * size for image row in bytes
     */
    unsigned int row_width;
    /**
     * string for image title
     */
    std::string title_string;
    /**
     * string for image author
     */
    std::string author_string;
    /**
     * string for image description
     */
    std::string description_string;
    /**
     * string for image copyright entry
     */
    std::string copyright_string;
    /**
     * string for image coding software
     */
    std::string software_string;
    /**
     * string for disclaimer entry
     */
    std::string disclaimer_string;
    /**
     * string for warning entry
     */
    std::string warning_string;
    /**
     * string for image source entry
     */
    std::string source_string;
    /**
     * string for image comment string
     */
    std::string comment_string;

    /**
     * constant for filterbuffer
     */
    enum { FILTERBUFFERCOUNT = 5 };

    /**
     * field for filterbuffers
     */
    ubyte *filter_buffers [FILTERBUFFERCOUNT];

    /**
     * index for current filter
     */
    unsigned int current_filter;

    /**
     * index for filter mask
     */
    unsigned int filter_mask;

    /**
     * filter size in bytes
     */
    unsigned int filter_width;

    /**
     * function to filter a single row of the image
     */
    void filterRow ();

    /**
     * checks color format before writing, is encapsulated in a
     * try-block in writeImage()
     */
    void doWrite (PngOutputStream &outputstream,
                  const image &theimage,
                  pngColorID cid);

    void doWrite (PngOutputStream &outputstream,
                  const channel8 &theChannel,
                  const lti::palette& colors,
                  pngColorID cid);


    /**
     * buffer for image row (all channels)
     */
    ubyte *row_buffer;

    /**
     * deflate encoder instance
     */
    DeflateEncoder deflate_encoder;
  };

  /*
   * set title string to write into text chunk
   */
  inline void PngEncoder::setTitle (const std::string &value) {
    title_string = value;
    return;
  }

  /*
   * return title string
   */
  inline std::string PngEncoder::getTitle () const {
    return title_string;
  }

  /*
   * set author string to write into text chunk
   */
  inline void PngEncoder::setAuthor (const std::string &value) {
    author_string = value;
    return;
  }

  /*
   * return author string
   */
  inline std::string PngEncoder::getAuthor () const {
    return author_string;
  }

  /*
   * set description string to write into text chunk
   */
  inline void PngEncoder::setDescription (const std::string &value) {
    description_string = value;
    return;
  }

  /*
   * return image description string
   */
  inline std::string PngEncoder::getDescription () const {
    return description_string;
  }

  /*
   * set copyright string to write into text chunk
   */
  inline void PngEncoder::setCopyright (const std::string &value) {
    copyright_string = value;
    return;
  }

  /*
   * return copyright string
   */
  inline std::string PngEncoder::getCopyright () const {
    return copyright_string;
  }

  /*
   * set coding software string to write into text chunk
   */
  inline void PngEncoder::setSoftware (const std::string &value) {
    software_string = value;
    return;
  }

  /*
   * return coding software string
   */
  inline std::string PngEncoder::getSoftware () const {
    return software_string;
  }

  /*
   * set set disclaimer string to write into text chunk
   */
  inline void PngEncoder::setDisclaimer (const std::string &value) {
    disclaimer_string = value;
    return;
  }

  /*
   * return disclaimer string
   */
  inline std::string PngEncoder::getDisclaimer () const {
    return disclaimer_string;
  }

  /*
   * set warning string to write into text chunk
   */
  inline void PngEncoder::setWarning (const std::string &value) {
    warning_string = value;
    return;
  }

  /*
   * return warning string
   */
  inline std::string PngEncoder::getWarning () const {
    return warning_string;
  }

  /*
   * set image source string to write into text chunk
   */
  inline void PngEncoder::setSource (const std::string &value) {
    source_string = value;
    return;
  }

  /*
   * return image source string
   */
  inline std::string PngEncoder::getSource () const {
    return source_string;
  }

  /*
   * set comment string to write into text chunk
   */
  inline void PngEncoder::setComment (const std::string &value) {
    comment_string = value;
    return;
  }

  /*
   * return comment string
   */
  inline std::string PngEncoder::getComment () const {
    return comment_string;
  }

  } // namespace png
} // namespace lti


#endif

