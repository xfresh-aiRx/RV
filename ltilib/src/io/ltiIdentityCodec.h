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
 * file .......: ltiIdentityCodec.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 5.11.2002
 * revisions ..: $Id: ltiIdentityCodec.h,v 1.1 2003/09/20 14:43:33 wickel Exp $
 */

#ifndef _LTI_IDENTITY_CODEC_H_
#define _LTI_IDENTITY_CODEC_H_

#include "ltiDataCodec.h"

namespace lti {
  /**
   * This is a dummy codec that does absolutely nothing. It is convenient
   * for using "no compression" as just one of many compression methods.
   */
  class identityCodec : public dataCodec {
  public:

    /**
     * default constructor
     */
    identityCodec();

    /**
     * Construct a functor using the given parameters
     */
    identityCodec(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    identityCodec(const identityCodec& other);

    /**
     * destructor
     */
    virtual ~identityCodec();

    /**
     * returns the name of this type ("identityCodec")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    identityCodec& copy(const identityCodec& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    identityCodec& operator=(const identityCodec& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

  protected:

    //TODO: comment your apply methods!

    virtual bool decodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc, int& ndest) const;

    virtual bool encodeImplementation(const buffer& src, buffer& dest,
                                      int nsrc, int& ndest) const;


  };
}

#endif

