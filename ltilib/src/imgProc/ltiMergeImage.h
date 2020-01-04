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
 * file .......: ltiMergeImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergeImage.h,v 1.2 2004/05/03 12:18:38 alvarado Exp $
 */

#ifndef _LTI_MERGE_IMAGE_H_
#define _LTI_MERGE_IMAGE_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiMath.h"

namespace lti {
  /**
   * Base class for all merge image functor-classes.
   *
   * These functors merge color-channels to a color image.
   *
   * This is a virtual class, so you cannot instantiate it.
   *
   * @ingroup gColor
   */
  class mergeImage : public functor {
  public:
    /**
     * default constructor
     */
    mergeImage()  : functor() {};

    /**
     * destructor
     */
    virtual ~mergeImage() {};

    /**
     * on-copy operator for 32-bit "floating-point" channels
     */
    virtual bool apply(const matrix<float>& c1,
                       const matrix<float>& c2,
                       const matrix<float>& c3,
                       image& img) const = 0;
    /**
     *  on-copy operator for 8-bit channels
     */
    virtual bool apply(const channel8& c1,
                       const channel8& c2,
                       const channel8& c3,
                       image& img) const = 0;

    /**
     * on-copy operator for 32-bit floating point values
     */
    virtual bool apply(const float& c1,
		       const float& c2,
		       const float& c3,
		       rgbPixel& pixel) const = 0;

    /**
     * on-copy operator for 8-bit values
     */
    virtual bool apply(const ubyte& c1,
		       const ubyte& c2,
		       const ubyte& c3,
		       rgbPixel& pixel) const = 0;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;
  };
}

#endif

