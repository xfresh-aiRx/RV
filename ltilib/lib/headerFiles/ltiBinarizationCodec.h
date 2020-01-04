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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiBinarizationCodec.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 5.11.2002
 * revisions ..: $Id: ltiBinarizationCodec.h,v 1.3 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_BINARIZATION_CODEC_H_
#define _LTI_BINARIZATION_CODEC_H_

#include "ltiDataCodec.h"

namespace lti {
  /**
   * Encodes a byte array to an array of bits. In the encoded array, each
	 * bit will represent a byte in the original array.
   */
  class binarizationCodec : public dataCodec {
  public:

    /**
     * The parameters for the class binarizationCodec.
     */
    class parameters : public dataCodec::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
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
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * The threshold above which elements are encoded to "1".
       */
      bufferElement threshold;

      /**
       * The value to which "1" is decoded.
       */
      bufferElement maxValue;
    };

    /**
     * default constructor
     */
    binarizationCodec();

    /**
     * Construct a functor using the given parameters
     */
    binarizationCodec(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    binarizationCodec(const binarizationCodec& other);

    /**
     * destructor
     */
    virtual ~binarizationCodec();

    /**
     * returns the name of this type ("binarizationCodec")
     */
    virtual const char* getTypeName() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    binarizationCodec& copy(const binarizationCodec& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    binarizationCodec& operator=(const binarizationCodec& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


    /**
     * Computes the default buffer size for encoded data. The
     * source of the original data buffer is given as argument.
     * The default is to return the given value.
     * @param originalSize the size of the data buffer to be encoded
     * @return the estimated size of the encoded data.
     */
    virtual int estimateEncodedSize(int originalSize) const;

    /**
     * Computes the default buffer size for decoded data. The
     * source of the original data buffer is given as argument.
     * The default is to return the given value.
     * @param originalSize the size of the data buffer to be decoded
     * @return the estimated size of the encoded data.
     */
    virtual int estimateDecodedSize(int originalSize) const;

  protected:
    //TODO: comment your apply methods!

    virtual bool decodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc,  int& ndest) const;

    virtual bool encodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc, int& ndest) const;


  };
}

#endif

