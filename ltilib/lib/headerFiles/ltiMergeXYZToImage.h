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
 * file .......: ltiMergeXYZToImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergeXYZToImage.h,v 1.1 2003/11/14 16:07:21 alvarado Exp $
 */

#ifndef _LTI_MERGE_XYZ_TO_IMAGE_H_
#define _LTI_MERGE_XYZ_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge XYZ channels
   *
   * @ingroup gColor
   */
  class mergeXYZToImage : public mergeImage  {
  public:

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a poinbter to a clone of the functor
     */
    virtual functor* clone() const;

    /**
     * on-copy operator for 32-bit "floating-point" channels
     */
    virtual bool apply(const matrix<float>& c1,
                       const matrix<float>& c2,
                       const matrix<float>& c3,
                       image& img) const;

    /**
     * on-copy operator for 8-bit channels
     */
    virtual bool apply(const channel8& c1,
                       const channel8& c2,
                       const channel8& c3,
                       image& img) const;

    /**
     * on-copy operator for 32-bit "floating-point" values
     */
    virtual bool apply(const float& c1,
                       const float& c2,
                       const float& c3,
                       rgbPixel& pixel) const;

    /**
     * on-copy operator for 8-bit values
     */
    virtual bool apply(const ubyte& c1,
                       const ubyte& c2,
                       const ubyte& c3,
                       rgbPixel& pixel) const;
  };

}

#endif

