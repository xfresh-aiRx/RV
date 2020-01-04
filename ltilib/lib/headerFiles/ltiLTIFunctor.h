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
 * file .......: ltiLTIFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 03.06.2003
 * revisions ..: $Id: ltiLTIFunctor.h,v 1.12 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_LTIFUNCTOR_H_
#define _LTI_LTIFUNCTOR_H_

#include "ltiIOFunctor.h"
#include "ltiImage.h"
#include "ltiDataCodec.h"
#include <fstream>

#undef None

namespace lti {
  /** 
   * Base class to functors which load and save images in LTI format.
   *
   * The LTI format is a raw data file, in which only size of the image
   * and the data, without any compression type (yet), are stored.
   *
   * It is necessary because standard file formats do not provide functionaly
   * to save floating-point or integer (32 bit) channels.
   *
   * @see lti::ioLTI::parameters
   * @see lti::ioLTI::header
   * 
   * This functor is at a very primitive development stage.  The
   * format will change in the future, mainly because at this time, it
   * depends on the implemented platform, i.e. maybe a file stored on
   * Linux won't be readable on Windows and vice-versa.  Endianness and 
   * floating point number format still need to be formally specified.
   *
   * It is although provided for those applications which need to store 
   * imatrices or channels, for which no other alternative is provided.
   */
  class ioLTI : public ioFunctor {
  public:

    /**
     * The compression methods usable for compressing the data.
     * You should be aware that runlength compression makes only
     * sense with matrices of bytes. RunLength tries to find
     * runs of equal bytes, which basically never happens in 
     * matrices of elements that occupy more than one byte.
     */
    enum eCompressionType {
      None=0,              ///< use no compression
      RunLength=1,         ///< uses runlength compression
      Flate=2             ///< uses zLib deflate compression
    };

    /**
     * Parameter class of the ioLTI class
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

      // no parameters at this point
      /**
       * Compression method used when storing the data. 
       */
      eCompressionType compression;
    };

    /**
     * LTI-Format file header
     *
     * This class is defined within the class lti::ioLTI. The header is
     * always stored uncompressed.
     */
    class header {
    public:
      /**
       * file type. For LTI-Types this must be 'LT'
       */
      const uint16 type;

      /**
       * Contents type:
       * -  0 : undefined
       * - 'b': ubyte channel
       * - 'i': int channel
       * - 'f': float channel
       * - 'c': rgbPixel image
       */
      union {
        uint16 contents;
        struct {
          /**
           * Contents type:
           * -  0 : undefined
           * - 'b': ubyte channel
           * - 'i': int channel
           * - 'f': float channel
           * - 'c': rgbPixel image
           */
          ubyte contents;
          /**
           * Compression type:
           * -  0 : no compression
           * -  1 : runlength compression 
           * -  2 : flate compression (only with zlib)
           */
          ubyte compression;
        } encoding;
      };

      /**
       * file size in byte (excluding the header, so only the data section
       * is counted).
       */
      uint32 size;

      /**
       * Must be 0
       * This can be used in the future to define some compressed formats
       */
      const uint32 reserved1;

      /**
       * Must be 0
       * This can be used in the future to define some compressed formats
       */
      const uint32 reserved2;

      /**
       * number of rows
       */
      uint32 rows;

      /**
       * number of columns
       */
      uint32 columns;

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
      inline int length() const {return 24;};

    protected:
      endianness io;
    };

    /**
     * default constructor
     */
    ioLTI();

    /**
     * destructor
     */
    ~ioLTI();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters. Furthermore, 
     * the method will create the appropriate compression 
     * codec from the parameters compression value.
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  protected:
    /**
     * Pointer to compression functor used
     */
    dataCodec* codec;

  };

  /**
   * Functor to read a bitmap (LTI) file.
   *
   * It is NOT thread save, this means, the SAME instance can not be
   * used from different threads or processes at the same time.  If
   * this occurs an unpredictible behaviour must be expected!.  If you
   * need to read or write many LTIs at the same time, use in each
   * thread an instance of this functor, or protect your code with
   * semaphores.
   */
  class loadLTI : public ioLTI {
  public:
    /**
     * default constructor
     */
    loadLTI();

    /**
     * destructor
     */
    ~loadLTI() {};

    /**
     * returns the name of this type ("loadLTI")
     */
    virtual const char* getTypeName() const;


    /**
     * Loads a matrix of a given type. There are implementations for 
     * image, channel, channel8,  imatrix, and dmatrix.
     * If the content of the file does not match the parameter type,
     * the method will try to convert it by using the matrix's castFrom
     * method, if such a conversion makes sense. Otherwise, it will
     * return false.
     *
     * @param theChannel the matrix on the file will be loaded here
     * @return true if successful, false otherwise.
     */
    //@{
    bool apply(matrix<rgbPixel>& theChannel);
    bool apply(imatrix& theChannel);
    bool apply(dmatrix& theChannel);
    bool apply(fmatrix& theChannel);
    bool apply(matrix<ubyte>& theChannel);
    //@}


    /**
     * shortcut for load LTI.
     *
     * @param filename name of the file to be readed
     * @param theChannel variable where the matrix will be stored
     * @return true if successful, false otherwise.
     */
    //@{
    bool load(const std::string& filename, matrix<rgbPixel>& theChannel);
    bool load(const std::string& filename, fmatrix& theChannel);
    bool load(const std::string& filename, matrix<ubyte>& theChannel);
    bool load(const std::string& filename, imatrix& theChannel);
    bool load(const std::string& filename, dmatrix& theChannel);
    //@}

    /**
     * check the data of the bitmap header
     * @param filename name of the bitmap file to be tested
     * @param imageSize returns the size of the bitmap: imageSize.x is the
     *                  number of columns and imageSize.y the numbeer of
     *                  rows.
     * @param imageType type of the image found in file.  It can be:
     *                  - 'c' for images   (from color image)
     *                  - 'b' for channel8 (from byte channel)
     *                  - 'f' for channel  (from float channel)
     *                  - 'i' for imatrix  (from int)
     *     
     * @param compr compression type.  It can be:
     *                  - None
     *                  - RunLength
     *                  - Flate (only available if zLib is found)
     * @return true if file is ok
     */
    bool checkHeader(const std::string& filename,
                     point& imageSize,
                     char&  imageType,
                     eCompressionType& compr);
    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    header theHeader;
  };

  /**
   * Functor to save a bitmap (LTI) file.
   *
   * To save a LTI image just initialize the %parameters of the
   * "%saveLTI" %object and call the apply member, or use one of the
   * shortcuts.
   *
   * Example:
   *
   * \code
   *    lti::image anImg;                      // an image
   *    lti::channel8 aChnl;                   // an 8-bit Channel
   *    ...
   *    lti::saveLTI saveImg;                  // the save functor
   *
   *    // save the image as a 24 bit bitmap file:
   *    saveImg.save("/tmp/theFile.lti",anImg);
   *
   *    // save the channel as a 8 bit bitmap file:
   *    saveImg.save("/the/theChnl8.lti",aChnl);
   *
   *    // save the channel as a 24 bit bitmap file:
   *    lti::saveLTI::parameters param;          // functor parameters
   *
   *    param.filename = "~/tmp/theChnl24.lti";  // name of the LTI file
   *    param.bitsPerPixel = 24;                 // use 24 bits!
   *    saveImg.setParameters(param);            // set the functor parameters
   *
   *    saveImg.apply(aChnl);                    // save the channel
   * \endcode
   *
   * @see ioLTI::parameters
   */
  class saveLTI : public ioLTI {
  public:
    /**
     * default constructor
     */
    saveLTI();

    /**
     * destructor
     */
    ~saveLTI() {};

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

//     /**
//      * save image as LTI.
//      *
//      * Note that if the parameters contain a number of bits per pixels
//      * less than 24 (this means 8, 4 or 1)(see
//      * lti::ioLTI::parameters), this method will apply a quantization
//      * algorithm, which will reduce the number of used colors as
//      * requested.  This is very time consuming!  For 24 images this
//      * method is relativly fast.
//      *
//      * For a faster 8-bit LTI use the method apply(const
//      * channel8&,const lti::palette).
//      * @param theImage the image to be stored.
//      * @return true if successful, false otherwise.
//      */
//     bool apply(const image& theImage);

    /**
     * Save matrix of any type. There is a template implementation
     * for the types image (matrix<rgbPixel>), imatrix (matrix<int>), 
     * channel (matrix<float>) and channel8 (matrix<ubyte>).
     * @param theChannel matrix to be stored. 
     */
    //@{
    bool apply(const matrix<rgbPixel>& theChannel);
    bool apply(const fmatrix& theChannel);
    bool apply(const matrix<ubyte>& theChannel);
    bool apply(const imatrix& theChannel);
    bool apply(const dmatrix& theChannel);


    /**
     * This will save a matrix in the appropriate file format.
     * @param filename the name of the file which will receive the matrix
     * @param theImage the matrix to be written
     * @return true, iff the matrix could be saved successfully
     */
    //@{
    bool save(const std::string& filename, const matrix<rgbPixel>& theImage);
    bool save(const std::string& filename, const imatrix& theImage);
    bool save(const std::string& filename, const dmatrix& theImage);
    bool save(const std::string& filename, const fmatrix& theImage);
    bool save(const std::string& filename, const matrix<ubyte>& theImage);
    //@}

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

  private:

    header theHeader;
  };
}  //namespace lti

#endif

