/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file is part of the LTI-Computer Vision Library (LTI-Lib)
 *
 * The LTI-Lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * The LTI-Lib is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the LTI-Lib; see the file LICENSE.  If
 * not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiPNGFunctor.h
 * authors ....: Stefan Syberichs, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiPNGFunctor.h,v 1.23 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_PNGFUNCTOR_H_
#define _LTI_PNGFUNCTOR_H_

#include "ltiConfig.h"

#if defined HAVE_LIBPNG || defined HAVE_LOCALPNG

#include "ltiIOFunctor.h"
#include "ltiImage.h"
#include <fstream>

#ifdef HAVE_LIBPNG
#include <cstdio>
#else
#include "ltipngdecoder.h"
#include "ltipngencoder.h"
#endif

namespace lti {
  /**
   * Base class to functors which load and save images in PNG format.
   *
   * There are two implementations of this functor.
   * - The first one uses the C++ PNG interface from Miano (at
   *   Colosseum Builders C++ Image Library), for which several files
   *   are required.  These are published under another licence and
   *   therefore are not included in the default LTI-Lib.  You can use
   *   them in non-commercial applications and get them from
   *   ltilib.sourceforge.net.  See file ltilib/src/io/png/00readme.txt
   *   for more information.
   *   If the file ltiPNGFunctor.cpp and the directory png/ are found
   *   in ltilib/src/io, and HAVE_LIBPNG is not defined in basis/ltiConfig.h
   *   this will be the implementation used.
   *
   * - The second implementation uses the standard PNG Library, which is
   *   usually installed in all Linux distributions, but not for Windows.
   *   The implementation is included in the standard LTI-Lib, (file
   *   is called ltiPNGLibFunctor.cpp).
   *   The autoconf script (./configure) will automatically detect if you
   *   have the libpng installed, and activate this implementation if
   *   possible.
   *
   * We recomend to use the libpng, it is more robust against not 100%
   * correct PNG-files, and it is faster.  Mianos' implementation is mainly
   * for windows users that do not want to install the pnglib.
   *
   */
  class ioPNG : public ioFunctor {
  public:
    /**
     * Parameter class of the ioBMP class
     */
    class parameters : public ioFunctor::parameters {
    public:

      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other) : ioFunctor::parameters() {
        copy(other);
      };

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this class
       */
      virtual const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * bits per pixel
       *
       * Default value: 24
       */
      int bitsPerPixel;

      /**
       * quantization of colors.
       * if 0 no quantization will be made.
       * if >0 then a k-mean analysis of the colors will be made and
       * the used color number will be reduce to <code>quantColors</code>.
       * <b> Not implemented yet</b>
       *
       * Default value: 0
       */
      int quantColors;

      /**
       * True if Alpha channel is to be considered. If this is true,
       * bitsPerPixel <b>must</b> be 32!
       *
       * Default value: false
       */
      bool useAlphaChannel;

    };

    /**
     * default constructor
     */
    ioPNG();

    /**
     * destructor
     */
    ~ioPNG();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    // returns a pointer to a clone of the functor.
    //  virtual functor* clone() const { return (new ioBMP(*this));};

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;
  };

  /**
   * Functor to read a Portable Network Graphic (PNG) file.
   * It is NOT thread save, this means, the SAME instance can not be used
   * from different threads or processes at the same time.  If this occurs
   * an unpredictible behaviour must be expected!.
   * If this is required, just use a different instance of the functor
   * for each thread!
   *
   * Example:
   *
   * \code
   *
   * lti::image anImg;                      // an image
   * ...
   * lti::loadPNG myImg;                    // the load functor
   * lti::loadPNG::parameters param;        // functor parameters
   * param.filename = "~/tmp/theFile.png";  // name of the PNG file
   * myImg.setParameters(param);            // set the functor parameters
   * myImg.apply(anImg);                    // load the file
   *
   * \endcode
   *
   * You can also use the more confortable shortcut:
   *
   * \code
   * lti::image anImg;                      // an image
   * ...
   * lti::loadPNG loadImg;                  // the load functor
   * loadImg.load("~/tmp/theFile.png",img); // load the image.
   * \endcode
   *
   * @exception BadPngStream is thrown when the Stream is corrupted.
   * @see savePNG
   * @see image
   * @see loadPNG::parameters
   *
   * For more information on implementation issues see ioPNG
   */
  class loadPNG : public ioPNG {
  public:
    /**
     * default constructor
     */
    loadPNG();

    /**
     * destructor
     */
    ~loadPNG();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * load Image.
     * @exception BadPngStream is thrown when the Stream is corrupted.
     */
    bool apply(image& theImage);

    /**
     * load Image.
     * @exception BadPngStream is thrown when the Stream is corrupted.
     */
    bool apply(channel8& theChannel,lti::palette& colors);

    /**
     * shortcut for load PNG
     */
    bool load(const std::string& filename,image& theImage);

    /**
     * method for loading a PNG image from an already opened
     * stream.
     */
    bool load(std::ifstream& file,image& theImage);

    /**
     * shortcut for load PNG
     */
    bool load(const std::string& filename,
              channel8& theImage,
              lti::palette& colors);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * check the data of the PNG header
     * @param filename name of the PNG file to be tested
     * @param imageSize returns the size of the PNG: imageSize.x is the
     *                  number of columns and imageSize.y the numbeer of
     *                  rows.
     * @param bitsPerPixel number of bits per pixel
     * @param imageType For color types 0 (gray) and 4 (gray-alpha), the
     *        mappings apply to the gray sample values (but not to the alpha
     *        sample). For color types 2 (RGB), 3 (indexed RGB), and 6 (RGBA),
     *        the mappings apply independently to each of the red, green, and
     *        blue sample values (but not the alpha sample). In the case of
     *        color type 3 (indexed RGB), the mapping refers to the RGB samples
     *        and not to the index values.
     * @return true if file is ok
     */
    bool checkHeader(const std::string& filename,
                           point& imageSize,
                           int&   bitsPerPixel,
                           int&   imageType);

  private:
#ifdef HAVE_LIBPNG
    /**
     * method for loading a PNG image from an already opened
     * file.
     */
    bool load(FILE* file,image& theImage);

#endif
  };

  /**
   * Functor to save a Protable Network graphic (PNG) file.
   * To save a PNG image just initialize the parameters of the
   * "savePNG" object and call the apply member.
   *
   * Example:
   *
   * \code
   *
   * lti::image anImg;                      // an image
   * ...
   * lti::savePNG saveImg;                  // the save functor
   * lti::savePNG::parameters param;        // functor parameters
   * param.filename = "~/tmp/theFile.png";  // name of the PNG file
   * saveImg.setParameters(param);          // set the functor parameters
   * saveImg.apply(anImg);                  // save the file
   *
   * \endcode
   *
   * You can also use the more confortable shortcut:
   *
   * \code
   * lti::image anImg;                      // an image
   * ...
   * lti::savePNG saveImg;                  // the save functor
   * saveImg.save("~/tmp/theFile.png",img); // save the image.
   * \endcode
   *
   * @see loadPNG
   * @see image
   * @see savePNG::parameters
   */
  class savePNG : public ioPNG {
  public:
    /**
     * default constructor
     */
    savePNG();

    /**
     * destructor
     */
    ~savePNG();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * save image as PNG
     */
    bool apply(const image& theImage);

    /**
     * save float channel as grey image
     */
    bool apply(const channel& theChannel);

    /**
     * save 8-bit channel as 24-bbit RGB by assigning each
     * R, G and B the same grey-value
     */
    bool apply(const channel8& theChannel,
               const lti::palette& colors = emptyPalette);

    /**
     * this will save an image as a 24 bit RGB bitmap image
     */
    bool save(const std::string& filename,
              const image& theImage);

    /**
     * save an "indexed" image contained in the given channel8 (indices) and
     * the given color palette
     */
    bool save(const std::string& filename,
              const channel8& theChannel,
              const lti::palette& colors = emptyPalette);

    /**
     * save a channel as a grey image (0 = Black, 1 = White)
     * the given color palette
     */
    bool save(const std::string& filename,
              const channel& theChannel);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;
  };
}  //namespace lti

#endif
#endif

