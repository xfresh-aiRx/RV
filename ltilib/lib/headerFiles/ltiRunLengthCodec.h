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
 * file .......: ltiRunLengthCodec.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 5.11.2002
 * revisions ..: $Id: ltiRunLengthCodec.h,v 1.2 2003/02/17 07:15:52 alvarado Exp $
 */

#ifndef _LTI_RUNLENGTH_CODEC_H_
#define _LTI_RUNLENGTH_CODEC_H_

#include "ltiDataCodec.h"

namespace lti {
  /**
   * Encodes a byte array to an array of hexadecimal digits or
   * performs the corresponding decoding method, depending on the parameters.
   */
  class runLengthCodec : public dataCodec {
  public:

    /**
     * default constructor
     */
    runLengthCodec();

    /**
     * Construct a functor using the given parameters
     */
    runLengthCodec(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    runLengthCodec(const runLengthCodec& other);

    /**
     * destructor
     */
    virtual ~runLengthCodec();

    /**
     * returns the name of this type ("runLengthCodec")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    runLengthCodec& copy(const runLengthCodec& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    runLengthCodec& operator=(const runLengthCodec& other);

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

