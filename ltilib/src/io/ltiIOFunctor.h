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
 * file .......: ltiIOFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiIOFunctor.h,v 1.14 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_IOFUNCTOR_H_
#define _LTI_IOFUNCTOR_H_

#include <iostream>
#include <string>
#include "ltiFunctor.h"
#include "ltiImage.h"

namespace lti {
  /** Base class to all io functors.
   *
   * All functors to load and save images are considered io functors.
   *
   * This is an abstract class and therefore no instances of this class
   * should exist!
   */
  class ioFunctor : public functor {
  public:
    /**
     * ioFunctor parameters class
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : functor::parameters() {};

      /**
       * copy constructor
       */
      parameters(const parameters& other) : functor::parameters() {
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
       * returns name of this type
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
       * name of the file to read/write
       */
      std::string filename;
    };

    /**
     * default constructor
     */
    ioFunctor()
      : functor() {};

    /**
     * constructor, sets the parameters
     */
    ioFunctor(const parameters& theParam);

    /**
     * constructor, sets the filename
     */
    ioFunctor(const std::string& theFilename);

    /**
     * destructor
     */
    virtual ~ioFunctor();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * load Image
     * @param theImage the file specified in the parameters will be loaded
     *                 in this image.  Note that independently of the sort
     *                 of image in the file, this will always be converted to
     *                 a color lti::image.
     * @return a reference to the loaded image.
     */
    virtual bool apply(image& theImage) {
      throw invalidMethodException("apply(image) not implemented for this functor");
    };

    /**
     * load Channel
     * Use this method if you know that the file contains a gray valued image.
     * @param theChannel the image on the file will be loaded here
     * @param colors theChannel contains just indexes to the pixel values
     *                  in this palette (vector or rgbPixel).
     * @return true if successful, false otherwise.
     */
    virtual bool apply(channel8& theChannel,lti::palette& colors) {
      throw invalidMethodException("apply(channel8,palette) not implemented");
    };

    /**
     * this method loads an image or a channel8, depending on the contents
     * of the file header.
     * if the number of bits per pixel is 24, the image will be initialized,
     * if the number of bits is less or equal 8, the channel and palette
     * will be initialized.
     *
     * If the file contains an indexed image, the palette will contain the
     * corresponding colors.  If the file contains a "true color" image, the
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
    virtual int apply(image& theImage, channel8& theChannel,
		      lti::palette& colors) {
      throw invalidMethodException("apply(image&,channel&) not implemented");
    };

    /**
     * save image.
     * The file name is specified in the parameters.
     * @param theImage the image to be stored.
     * @return false if error, otherwise true
     */
    virtual bool apply(const image& theImage) {
      throw invalidMethodException("apply(const image&) not implemented");
    };

    /**
     * save float channel
     * @param theChannel channel to be stored.
     *                   This channel must contain values between
     *                   0.0f and 1.0f.
     * @return false if error, otherwise true
     */
    virtual bool apply(const channel& theChannel) {
      throw invalidMethodException("apply(const channel&) not implemented");
    };

    /**
     * save 8-bit channel with the given color palette.
     * @param theChannel the channel to be saved
     * @param colors if a color palette is given, <code>theChannel</code>
     *               will be considered to contain indices to this palette.
     *               If nothing (or an empty palette) is given,
     *               <code>theChannel</code> will be considered as a gray
     *               valued channel.
     * @return false if error, otherwise true
     */
    virtual bool apply(const channel8& theChannel,
                       const lti::palette& colors=emptyPalette) {
      throw invalidMethodException("apply(const channel&) not implemented");
    };

    /**
     * this will save an image as a 24 bit RGB image
     */
    virtual bool save(const std::string& filename,
                      const image& theImage) {
      throw invalidMethodException("save not implemented for this functor");
      return false;
    };

    /**
     * this will save a channel8
     * @param filename name of the destination file
     * @param theChannel the channel to be save
     * @param colors the palette to be used
     */
    virtual bool save(const std::string& filename,
              const channel8& theChannel,
              const lti::palette& colors=emptyPalette) {
      throw invalidMethodException("save not implemented for this functor");
      return false;
    };

    /**
     * shortcut for load image
     * @param filename name of the file to be readed
     * @param theImage variable where the image will to be stored
     */
    virtual bool load(const std::string& filename,image& theImage) {
      throw invalidMethodException("load not implemented for this functor");
      return false;
    };

    /**
     * shortcut for load image.
     *
     * Use this method if you know that the file contains a gray valued image.
     * @param filename name of the file to be readed
     * @param theChannel variable where the image will be stored
     * @param colors the palette used will be stored here
     */
    virtual bool load(const std::string& filename,channel8& theChannel,
                 lti::palette& colors) {
      throw invalidMethodException("load not implemented for this functor");
      return false;
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;
  };

  /**
   * Endianness
   * This class is used to read data from a file with a known
   * endianness.
   *
   * The problem this class try to solve is following:
   *
   * Let us assume we have a binary file (and not an ASCII file) with
   * following byte stream:
   *
   * 0x01 0x02 0x03 0x04 ...
   *
   * and we use following code to read an int (4 bytes integers):
   *
   * \code
   *   lti::uint32 integer;             // hold the values been readed
   *   std::istream in("theFile.dat");  // the input stream
   *
   *  ...
   *  in.read(&integer,4);             // reads 4 bytes on "integer"
   * \endcode
   *
   * With a little endian system (for example Intel Pentium), the
   * contents of <code>integer</code> will be 0x04030201.
   *
   * With a big endian system (for example Motorola/IBM PowerPC), the
   * contents of <code>integer</code> will be 0x01020304.
   *
   * We need a class, which allow us to load data from a file with the
   * correct endiannes!!!
   *
   * The actual system endianess should be already defined:
   *
   * - For Windows systems there is no problem if they run on Intel
   *   Processors (always little-endian).  If you need to change this
   *   (for example you have an Alpha processor, change the endianness
   *   on the file ltiWinConfig.h)
   *
   * - For Unix systems the processor and endianness are determined
   *   automatically by the configure script.  If the file config.h
   *   exists, everything should be ok!!
   */
  class endianness : public object {
  public:
    /** eEndianness
        which sort of endianness uses the file
     */
    enum eEndianness {
      LittleEndian, /**< the file uses little endianness */
      BigEndian     /**< the file uses big endianness */
    };

    /** default constructor
        @param fileEndianness specify which endianness uses the file stream
     */
    endianness(const eEndianness fileEndianness = LittleEndian);

    /**
     * destructor
     */
    virtual ~endianness();

    /**
     * returns the name of this class ("endianness")
     */
    virtual const char* getTypeName() const;

    /** read a byte from the stream
        @param in input stream
        @param data variable where the data should be stored
        @return a reference to the variable with the readed data
    */
    byte& read(std::istream& in,byte& data) const;

    /** read an unsigned byte from the stream
        @param in input stream
        @param data variable where the data should be stored
        @return a reference to the variable with the readed data
    */
    ubyte& read(std::istream& in,ubyte& data) const;

    /** read a int16 (2 bytes)
        @param in input stream
        @param data variable where the data should be stored
        @return a reference to the variable with the readed data
    */
    int16& read(std::istream& in,int16& data) const;

    /** read a uint16 (2 bytes)
        @param in input stream
        @param data variable where the data should be stored
        @return a reference to the variable with the readed data
    */
    uint16& read(std::istream& in,uint16& data) const;

    /** read a int32
        @param in input stream
        @param data variable where the data should be stored
        @return a reference to the variable with the readed data
    */
    int32& read(std::istream& in,int32& data) const;

    /** read a uint32
        @param in input stream
        @param data variable where the data should be stored
        @return a reference to the variable with the readed data
    */
    uint32& read(std::istream& in,uint32& data) const;

    /** write a byte
        @param out output stream
        @param data variable with the data to be stored on the file
        @return a reference to the variable with the data
    */
    const byte& write(std::ostream& out,const byte& data) const;

    /** write a ubyte
        @param out output stream
        @param data variable with the data to be stored on the file
        @return a reference to the variable with the data
    */
    const ubyte& write(std::ostream& out,const ubyte& data) const;

    /** write a int16 (2 bytes)
        @param out output stream
        @param data variable with the data to be stored on the file
        @return a reference to the variable with the data
    */
    const int16& write(std::ostream& out,const int16& data) const;

    /** write a uint16 (2 bytes)
        @param out output stream
        @param data variable with the data to be stored on the file
        @return a reference to the variable with the data
    */
    const uint16& write(std::ostream& out,const uint16& data) const;

    /** write a int32
        @param out output stream
        @param data variable with the data to be stored on the file
        @return a reference to the variable with the data
    */
    const int32& write(std::ostream& out,const int32& data) const;

    /** write a uint32
        @param out output stream
        @param data variable with the data to be stored on the file
        @return a reference to the variable with the data
    */
    const uint32& write(std::ostream& out,const uint32& data) const;

  protected:
    /**
     * This class encapsulates the endianness of the processor used.
     * The "defaultEndian" will always be the same as the system one.
     */
    class defaultEndian : public object {
    public:
      virtual const char* getTypeName() const;
      /**
       * read a byte
       */
      virtual byte& read(std::istream& in,byte& data) const;
      /**
       * read a ubyte
       */
      virtual ubyte& read(std::istream& in,ubyte& data) const;
      /**
       * read a int16 (2 bytes)
       */
      virtual int16& read(std::istream& in,int16& data) const;
      /**
       * read a uint16 (2 bytes)
       */
      virtual uint16& read(std::istream& in,uint16& data) const;
      /**
       * read a int32
       */
      virtual int32& read(std::istream& in,int32& data) const;
      /**
       * read a uint32
       */
      virtual uint32& read(std::istream& in,uint32& data) const;

      /**
       * write a byte
       */
      virtual const byte& write(std::ostream& out,const byte& data) const;
      /**
       * write a ubyte
       */
      virtual const ubyte& write(std::ostream& out,const ubyte& data) const;
      /**
       * write a int16 (2 bytes)
       */
      virtual const int16& write(std::ostream& out,const int16& data) const;
      /**
       * write a uint16 (2 bytes)
       */
      virtual const uint16& write(std::ostream& out,const uint16& data) const;
      /**
       * write a int32
       */
      virtual const int32& write(std::ostream& out,const int32& data) const;
      /**
       * write a uint32
       */
      virtual const uint32& write(std::ostream& out,const uint32& data) const;
    };

    /**
     * if file endianness is the opposite as the system endianness
     */
    class oppositeEndian : public defaultEndian  {
    public:
      virtual const char* getTypeName() const;
      /**
       * read a int16 (2 bytes)
       */
      int16& read(std::istream& in,int16& data) const;
      /**
       * read a uint16 (2 bytes)
       */
      uint16& read(std::istream& in,uint16& data) const;
      /**
       * read a int32
       */
      int32& read(std::istream& in,int32& data) const;
      /**
       * read a uint32
       */
      uint32& read(std::istream& in,uint32& data) const;

      /**
       * write a int16 (2 bytes)
       */
      const int16& write(std::ostream& out,const int16& data) const;
      /**
       * write a uint16 (2 bytes)
       */
      const uint16& write(std::ostream& out,const uint16& data) const;
      /**
       * write a int32
       */
      const int32& write(std::ostream& out,const int32& data) const;
      /**
       * write a uint32
       */
      const uint32& write(std::ostream& out,const uint32& data) const;
    };

    // the correct read operator is pointed by this pointer:
    defaultEndian* theEndian;
  };
}

#endif

