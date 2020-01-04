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
 * file .......: ltiBMPFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiBMPFunctor.h,v 1.20 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_BMPFUNCTOR_H_
#define _LTI_BMPFUNCTOR_H_

#include "ltiIOFunctor.h"
#include "ltiImage.h"
#include <fstream>

namespace lti {
  /** Base class to functors which load and save images in BMP format.

      This class includes types for read and write %header objects of the
      BMP file format.
      @param class lti::ioBMP::parameters
      @see lti::ioBMP::header,lti::ioBMP::infoHeader,lti::ioBMP::palette
   */
  class ioBMP : public ioFunctor {
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
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
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
       * compress the data.
       * (RLE4 or RLE8 will be used if the number of bits are 4 or 8).
       * The default value is false
       */
      bool compression;

      /**
       * bits per pixel (default value 24)
       */
      int bitsPerPixel;

      /**
       * quantization of colors.
       * If 0 no quantization needed
       * If >0 then a k-mean analysis of the colors will be made and
       * the used color number will be reduce to <em>quantColors</em>
       * (unused by now)
       */
      int quantColors;
    };

    /**
     * BITMAPFILEHEADER:
     *
     * This class is defined within the class lti::ioBMP.
     */
    class header {
    public:
      /**
       * file type. For Bitmaps this must be 'BM'
       */
      const uint16 type ;

      /**
       * file size in byte
       */
      uint32 size ;

      /**
       * must be 0
       */
      const uint16 reserved1 ;

      /**
       * must be 0
       */
      const uint16 reserved2 ;

      /**
       * offset (in bytes) from this header to the bitmap pixels
       */
      uint32 offsetPixels ;

      /**
       * default constructor
       */
      header();

      /**
       * reads header from the stream "in".
       * @return false if stream had an invalid header.
       */
      bool read(std::ifstream& in);

      /**
       * writes header to the stream "out".
       * @return false if an error occurs.
       */
      bool write(std::ofstream& out);

      /**
       * size of the header (in bytes)
       */
      inline int length() const {return 14;};

    protected:
      endianness io;
    };

    /**
     * BITMAPINFOHEADER
     *
     * This class is defined within the class lti::ioBMP.
     */
    class infoHeader {
    public:
      /**
       * constructor
       */
      infoHeader();

      /**
       * size
       */
      uint32 size;

      /**
       * width
       */
      uint32 width;

      /**
       * height
       */
      uint32 height;

      /**
       * planes
       */
      uint16 planes ;

      /**
       * bitcount
       */
      uint16 bitCount ;

      /**
       * compression
       */
      uint32 compression;

      /**
       * sizeImage
       */
      uint32 sizeImage;

      /**
       * xPixPerMeter
       */
      uint32 xPixPerMeter;

      /**
       * yPixPerMeter
       */
      uint32 yPixPerMeter;

      /**
       * colorsUsed
       */
      uint32 colorsUsed;

      /**
       * colorsImportant
       */
      uint32 colorsImportant;

      /**
       * read
       */
      bool read(std::ifstream& in);

      /**
       * write
       */
      bool write(std::ofstream& out);

      /**
       * size of the info header (in bytes)
       */
      inline int length() const {return 40;};

    protected:
      endianness io;
    };

    /** Color Palette
     *
     * This class is defined within the class lti::ioBMP.
     */
    class palette : public vector<rgbPixel> {
    public:
      /**
       * default constructor
       * @param entries number of entries in the palette
       */
      palette(int entries = 0) : vector<rgbPixel>(entries) {};

      /**
       * read the palette from the in stream
       * @param in input stream
       * @return true if everything is ok, false otherwise
       */
      bool read(std::ifstream& in);

      /**
       * write palette to the out stream
       * @param out output stream
       * @return true if everything is ok, false otherwise
       */
      bool write(std::ofstream& out);

    protected:
      /**
       * use the correct endianness to read and write!
       */
      endianness io;
    };

    /**
     * default constructor
     */
    ioBMP();

    /**
     * destructor
     */
    ~ioBMP() {};

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;
  };

  /**
   * Functor to read a bitmap (BMP) file.
   *
   * It is NOT thread save, this means, the SAME instance can not be
   * used from different threads or processes at the same time.  If
   * this occurs an unpredictible behaviour must be expected!.  If you
   * need to read or write many BMPs at the same time, use in each
   * thread an instance of this functor, or protect your code with
   * semaphores.
   */
  class loadBMP : public ioBMP {
  public:
    /**
     * default constructor
     */
    loadBMP();

    /**
     * destructor
     */
    ~loadBMP() {};

    /**
     * returns the name of this type ("loadBMP")
     */
    virtual const char* getTypeName() const;

    /** load Image
     * @param theImage the file specified in the parameters will be loaded
     *                 in this image.  Note that independently of the soft
     *                 of image in the file, this will always be converted to
     *                 a color lti::image.
     * @return a reference to the loaded image.
     */
    bool apply(image& theImage);

    /**
     * load channel
     *
     * Use this method if you know that the file contains a gray
     * valued image.  If you try to load a 24-bit image with this
     * method, some quantization algorithms will be used to reduce the
     * number of colors to 256.
     *
     * @param theChannel the image on the file will be loaded here
     * @param colors theChannel contains just indexes to the pixel values
     *               in this vector.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& theChannel,lti::palette& colors);

    /**
     * load channel
     *
     * Use this method if you know that the file contains a gray
     * valued image.
     *
     * @param theChannel the image on the file will be loaded here
     * @return true if successful, false otherwise.
     */
    bool apply(channel& theChannel);

    /**
     * this method loads an image or a channel8, depending on the contents
     * of the file header.
     * if the number of bits per pixel is 24, the image will be initialized,
     * if the number of bits is less or equal 8, the channel and palette
     * will be initialized.
     *
     * If the file contains an indexed image, the palette will contain the
     * corresponding colors.  If the file containes a "true color" image, the
     * palette will have size 0.
     *
     * @param theImage if the file contains a 24-bit image, this parameter
     *                 will be initialized
     * @param theChannel if the channel contains an 8-bit image, this
     *                   parameters will contain the readed data
     * @param colors if the channel was loaded this parameter will contain
     *               the used colors.
     * @return the number of bits per pixel of the loaded image or 0 if an
     *         error occured.  The valid values are 1, 4, 8, or 24
     */
    int apply(image& theImage, channel8& theChannel,lti::palette& colors);

    /**
     * shortcut for load BMP
     * @param filename name of the file to be readed
     * @param theImage variable where the image will to be stored
     */
    bool load(const std::string& filename,image& theImage);

    /**
     * shortcut for load BMP
     * @param file an already opened stream for reading binary data
     * @param theImage variable where the image will to be stored
     */
    bool load(std::ifstream& file,image& theImage);

    /**
     * shortcut for load BMP.
     *
     * Use this method if you know that the file contains a gray valued image.
     * If you try to load a 24-bit image with this method, some quantization
     * algorithms will be used to reduce the number of colors to 256.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     * @param colors the palette used will be stored here
     */
    bool load(const std::string& filename,
              channel8& theChannel,
              lti::palette& colors);

    /**
     * shortcut for load BMP.
     *
     * Use this method if you know that the file contains a gray valued image.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     */
    bool load(const std::string& filename,
        channel& theChannel);

    /**
     * this method loads an image or a channel8, depending on the contents
     * of the file header.
     * if the number of bits per pixel is 24, the image will be initialized,
     * if the number of bits is less or equal 8, the channel and palette
     * will be initialized.
     *
     * The wrong type will be resized to (0,0)
     * @param filename name of the file to be readed
     * @param theImage if the file contains a 24bit color image, this
     *                 parameter will be resized and initialized with
     *                 the contents of the file
     * @param theChannel if the file contains a 8bit or less indexed image,
     *                 this parameter and the palette will be initialized
     *                 with the contents of the file.
     * @param colors   the color palette for the indexed image.
     * @return the number of bits per pixel of the loaded image or 0 if an
     *         error occured.  The valid values are 1, 4, 8, or 24
     */
    int load(const std::string& filename,
             image& theImage,
             channel8& theChannel,
             lti::palette& colors);

    /**
     * check the data of the bitmap header
     * @param filename name of the bitmap file to be tested
     * @param imageSize returns the size of the bitmap: imageSize.x is the
     *                  number of columns and imageSize.y the numbeer of
     *                  rows.
     * @param bitsPerPixel number of bits per pixel
     * @param colorEntries number of color entries in the palette (only
     *                     used if bitsPerPixel<=8
     * @return true if file is ok
     */
    bool checkHeader(const std::string& filename,
                     point& imageSize,
                     int&   bitsPerPixel,
                     int&   colorEntries);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

  private:
    bool load1bit(std::ifstream& in,image& theImage);
    bool load1bit(std::ifstream& in,channel8& theChannel);
    bool load4bit(std::ifstream& in,image& theImage);
    bool load4bit(std::ifstream& in,channel8& theChannel);
    bool load8bit(std::ifstream& in,image& theImage);
    bool load8bit(std::ifstream& in,channel8& theChannel);
    bool load24bit(std::ifstream& in,image& theImage);

    bool applyFromStream(std::ifstream& in, image& theImage);

    header theHeader;
    infoHeader theInfoHeader;
    palette thePalette;
  };

  /**
   * Functor to save a bitmap (BMP) file.
   *
   * To save a BMP image just initialize the %parameters of the
   * "%saveBMP" %object and call the apply member, or use one of the
   * shortcuts.
   *
   * Example:
   *
   * \code
   *    lti::image anImg;                      // an image
   *    lti::channel8 aChnl;                   // an 8-bit Channel
   *    ...
   *    lti::saveBMP saveImg;                  // the save functor
   *
   *    // save the image as a 24 bit bitmap file:
   *    saveImg.save("/tmp/theFile.bmp",anImg);
   *
   *    // save the channel as a 8 bit bitmap file:
   *    saveImg.save("/the/theChnl8.bmp",aChnl);
   *
   *    // save the channel as a 24 bit bitmap file:
   *    lti::saveBMP::parameters param;          // functor parameters
   *
   *    param.filename = "~/tmp/theChnl24.bmp";  // name of the BMP file
   *    param.bitsPerPixel = 24;                 // use 24 bits!
   *    saveImg.setParameters(param);            // set the functor parameters
   *
   *    saveImg.apply(aChnl);                    // save the channel
   * \endcode
   *
   * @see ioBMP::parameters
   */
  class saveBMP : public ioBMP {
  public:
    /**
     * default constructor
     */
    saveBMP();

    /**
     * destructor
     */
    ~saveBMP() {};

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * save image as BMP.
     *
     * Note that if the parameters contain a number of bits per pixels
     * less than 24 (this means 8, 4 or 1)(see
     * lti::ioBMP::parameters), this method will apply a quantization
     * algorithm, which will reduce the number of used colors as
     * requested.  This is very time consuming!  For 24 images this
     * method is relativly fast.
     *
     * For a faster 8-bit BMP use the method apply(const
     * channel8&,const lti::palette).
     * @param theImage the image to be stored.
     */
    bool apply(const image& theImage);

    /**
     * save float channel
     * @param theChannel channel to be stored.
     *                   This channel must contain values between 0.0f and
     *                   1.0f.
     */
    bool apply(const channel& theChannel);

    /**
     * save 8-bit channel as a bitmap.
     * @param theChannel the channel to be saved
     * @param colors if a color palette is given, <code>theChannel</code>
     *                will be considered to contain indices to this palette.
     *                If nothing (or an empty palette) is given,
     *                <code>theChannel</code> will be considered as a gray
     *                valued channel.
     * See ioBMP::parameters for more options
     */
    bool apply(const channel8& theChannel,
               const lti::palette& colors=emptyPalette);

    /**
     * this will save an image as a 24 bit RGB bitmap image
     */
    bool save(const std::string& filename,
              const image& theImage);

    /**
     * this will save a channel8 as an 8 bit RGB bitmap image
     * @param filename name of the destination file
     * @param theChannel the channel to be save
     * @param colors the palette to be used
     *               (see apply(const channel& theChannel) for details)
     */
    bool save(const std::string& filename,
              const channel8& theChannel,
              const lti::palette& colors=emptyPalette);

    /**
     * this will save a channel as an 8 bit RGB bitmap image.
     * The values of the channel must be between 0.0f and 1.0f!
     * @param filename name of the destination file
     * @param theChannel the channel to be save
     * @param colors the palette to be used
     *               (see apply(const channel8& theChannel) for details)
     */
    bool save(const std::string& filename,
              const channel& theChannel,
              const lti::palette& colors=emptyPalette);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

  private:
    bool save1bit(std::ofstream& out,const image& theImage);
    bool save1bit(std::ofstream& out,const channel8& theChannel);
    bool save4bit(std::ofstream& out,const image& theImage);
    bool save4bit(std::ofstream& out,const channel8& theChannel);
    bool save8bit(std::ofstream& out,const image& theImage);
    bool save8bit(std::ofstream& out,const channel8& theChannel);
    bool save24bit(std::ofstream& out,const image& theImage);

    header theHeader;
    infoHeader theInfoHeader;
    palette thePalette;
  };
}  //namespace lti

#endif

