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
 * file .......: ltiDataTransformer.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 5.11.2002
 * revisions ..: $Id: ltiDataCodec.h,v 1.9 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_DATA_CODEC_H_
#define _LTI_DATA_CODEC_H_

#include "ltiDataTransformer.h"

namespace lti {
  /**
   * This is the base class for data encoding and decoding.
   * This an abstract class.
   */
  class dataCodec : public dataTransformer {
  public:
    /**
     * The parameters for the class dataTransformer.
     */
    class parameters : public dataTransformer::parameters {
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

      typedef enum {
        encode,
        decode
      } codingDirection;

      /**
       * The direction of the coding.
       */
      codingDirection direction;

      /**
       * If true, causes the encoding method to conclude the data block
       * with an end-of-data marker, whose value depends on the actual coding
       * method. This parameter is not implemented at present.
       */
      bool useEOD;

    };

    /**
     * default constructor
     */
    dataCodec();

    /**
     * Construct a functor using the given parameters
     */
    dataCodec(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    dataCodec(const dataCodec& other);

    /**
     * destructor
     */
    virtual ~dataCodec();

    /**
     * returns the name of this type ("dataCodec")
     */
    virtual const char* getTypeName() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


    /**
     * Encodes data in place.
     */
    virtual bool encode(bufferElement* srcdest, int nsrc, int& ndest) const;

    /**
     * Encodes data in place.
     */
    virtual bool encode(buffer& srcdest) const;

    /**
     * Encodes data on copy.
     */
    virtual bool encode(const bufferElement* src, int nsrc, bufferElement* dest, int& ndest) const;

    /**
     * Encodes data on copy.
     */
    virtual bool encode(const buffer& src, buffer& dest) const;


    /**
     * Decodes data in place.
     */
    virtual bool decode(bufferElement* srcdest, int nsrc, int& ndest) const;

    /**
     * Decodes data in place.
     */
    virtual bool decode(buffer& srcdest) const;

    /**
     * Decodes data on copy.
     */
    virtual bool decode(const bufferElement* src, int nsrc, bufferElement* dest, int& ndest) const;

    /**
     * Decodes data on copy.
     */
    virtual bool decode(const buffer& src, buffer& dest) const;


    /**
     * Operates on the given arguments. If the data is encoded or
     * decoded depends on the coding direction parameter.
     * @param srcdest pointer to an array of bufferElement. The input
     *        array must be allocated and have enough space for
     *        input as well as output data.
     * @param nsrc the number of input bytes that should be transformed.
     * @param ndest the number of bytes of the transformed data. If the
     *        buffer was too small, it will contain -1 when the function
     *        returns.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(bufferElement* srcdest, int nsrc, int& ndest) const;

    /**
     * Operates on the given argument. If the data is encoded or
     * decoded depends on the coding direction parameter.
     * @param srcdest vector with the input data which will
     *        also receive the transformed data.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(buffer& srcdest) const;

    /**
     * Operates on a copy of the given arguments.
     * @param src pointer to an array of bufferElement which contains the
     *            source data
     * @param nsrc the number of input bytes that should be transformed.
     * @param dest pointer to an array of bufferElement which will receive
     *             the transformed data. The array must be allocated
     *             by the caller
     * @param ndest When called, must contain the size of the output
     *              buffer. When the function returns, it contains the
     *              number of bytes actually used, or -1 if the buffer
     *              was too small.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const bufferElement* src, int nsrc, bufferElement* dest, int& ndest) const;

    /**
     * Operates on the given argument.
     * @param src vector with the input data
     * @param dest vector with the encoded or decoded data
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const buffer& src, buffer& dest) const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Copy another dataCodec
     */
    dataCodec& copy(const dataCodec& other);

    /**
     * Copy another dataCodec
     */
    dataCodec& operator=(const dataCodec& other);

  protected:
    /**
     * Implementation of on-copy data decoder. <b>This method
     * must not, under no circumstances, resize the destination vector.</b>
     * @return true if the decoding could be performed successfully.
     * @param src the source of the data.
     * @param dest the destination for the decoded data.
     * @param nsrc tells how many elements of the src vector
     *        must be decoded.
     * @param ndest tells how many elements of the destination vector
     *        can be used for the decoded data.
     *        When the method returns,
     *        this value tells how many elements were actually used,
     *        or notEnoughSpace if not enough elements were available.
     */
    virtual bool decodeImplementation(const buffer& src,
                                      buffer& dest,
                                      int nsrc, int& ndest) const=0;

    /**
     * Implementation of on-copy data encoder. <b>This method
     * must not, under no circumstances, resize the destination vector.</b>
     * @return true if the decoding could be performed successfully.
     * @param src the source of the data.
     * @param dest the destination for the encoded data.
     * @param nsrc tells how many elements of the src vector
     *        must be decoded.
     * @param ndest tells how many elements of the destination vector
     *        can be used for the decoded data.
     *        When the method returns,
     *        this value tells how many elements were actually used,
     *        or notEnoughSpace if not enough elements were available.
     */
    virtual bool encodeImplementation(const buffer& src,
                                      buffer& dest,
                                      int nsrc, int& ndest) const=0;


  public:
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
     * source of the encoded data buffer is given as argument.
     * The default is to return the given value.
     * @param encodedSize the size of the data buffer to be decoded
     * @return the estimated size of the decoded data.
     */
    virtual int estimateDecodedSize(int encodedSize) const;

  protected:
    /**
     * Computes a new reasonable buffer size for encoded data
     * if the given buffer size is too small. The default is
     * 2*orginialSize;
     * @param originalSize the size of the data buffer
     * @return the recommended new size of the data buffer
     */
    virtual int getNewEncodingBufferSize(int originalSize) const;

    /**
     * Computes a new reasonable buffer size for encoded data
     * if the given buffer size is too small. The default is
     * 2*orginialSize;
     * @param originalSize the size of the data buffer
     * @return the recommended new size of the data buffer
     */
    virtual int getNewDecodingBufferSize(int originalSize) const;

  };
}

#endif

