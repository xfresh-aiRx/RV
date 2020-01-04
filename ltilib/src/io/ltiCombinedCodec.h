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
 * revisions ..: $Id: ltiCombinedCodec.h,v 1.3 2003/02/17 07:15:51 alvarado Exp $
 */

#ifndef _LTI_COMBI_CODEC_H_
#define _LTI_COMBI_CODEC_H_

#include "ltiDataCodec.h"

namespace lti {
  /**
   * This is the base class for data encoding and decoding.
   * This an abstract class.
   */
  class combinedCodec : public dataCodec {
  public:

    /**
     * default constructor
     */
    combinedCodec(const dataCodec& c1, const dataCodec& c2);

    /**
     * Construct a functor using the given parameters
     */
    combinedCodec(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    combinedCodec(const combinedCodec& other);

    /**
     * destructor
     */
    virtual ~combinedCodec();

    /**
     * returns the name of this type ("combinedCodec")
     */
    virtual const char* getTypeName() const;

    combinedCodec& copy(const combinedCodec& other);


    combinedCodec& operator=(const combinedCodec& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

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
                                      int nsrc, int& ndest) const;

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
                                      int nsrc, int& ndest) const;


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

  private:
    const dataCodec* codec1;
    const dataCodec* codec2;

  };
}

#endif

