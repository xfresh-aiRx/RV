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
 * file .......: ltiMergeHSVToImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergeHSVToImage.h,v 1.1 2003/11/14 16:07:21 alvarado Exp $
 */

#ifndef _LTI_MERGE_HSV_TO_IMAGE_H_
#define _LTI_MERGE_HSV_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge HSV channels (Hue, Saturation, Value)
   *
   * @ingroup gColor
   */
  class mergeHSVToImage : public mergeImage {
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
     * merge the hue channel H, saturation S and value channel V
     * into an image
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     * @param img the merged image
     */
    virtual bool apply(const matrix<float>& H,
                       const matrix<float>& S,
                       const matrix<float>& V,
                       image& img) const;

    /**
     * merge the hue channel H, saturation S and value channel V
     * into an image
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     * @param img the merged image
     */
    virtual bool apply(const channel8& H,
                       const channel8& S,
                       const channel8& V,
                       image& img) const;

    /**
     * merge the hue value H, saturation S and value value V
     * into a pixel
     * @param H the hue value
     * @param S the saturation value
     * @param V the value value
     * @param pixel the merged pixel
     */
    virtual bool apply(const float& H,
                       const float& S,
                       const float& V,
                       rgbPixel& pixel) const;

    /**
     * merge the hue value H, saturation S and value value V
     * into a pixel
     * @param H the hue value
     * @param S the saturation value
     * @param V the value value
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& H,
                       const ubyte& S,
                       const ubyte& V,
                       rgbPixel& pixel) const;

  };
}

#endif

