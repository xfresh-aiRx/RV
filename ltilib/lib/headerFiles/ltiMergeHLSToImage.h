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
 * file .......: ltiMergeHLSToImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergeHLSToImage.h,v 1.1 2003/11/14 16:07:21 alvarado Exp $
 */

#ifndef _LTI_MERGE_HLS_TO_IMAGE_H_
#define _LTI_MERGE_HLS_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge HLS channels (Hue, Luminance, Saturation)
   *
   * @ingroup gColor
   */
  class mergeHLSToImage : public mergeImage {
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
     * merge the hue channel H, luminance L and saturation
     * channel S into an image.
     * @param H the hue channel
     * @param L the luminance channel
     * @param S the saturation channel
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& H,
                       const matrix<float>& L,
                       const matrix<float>& S,
                       image& img) const;

    /**
     * merge the hue channel H, luminance L and saturation
     * channel S into an image.
     * @param H the hue channel
     * @param L the luminance channel
     * @param S the saturation channel
     * @param img the image to be splitted
     */
    virtual bool apply(const channel8& H,
                       const channel8& L,
                       const channel8& S,
                       image& img) const;

    /**
     * merge the hue value H, luminance L and saturation
     * value S into a pixel.
     * @param H the hue value
     * @param L the luminance value
     * @param S the saturation value
     * @param pixel the merged pixel
     */
    virtual bool apply(const float& H,
                       const float& L,
                       const float& S,
                       rgbPixel& pixel) const;

    /**
     * merge the hue value H, luminance L and saturation
     * value S into a pixel.
     * @param H the hue value
     * @param L the luminance value
     * @param S the saturation value
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& H,
                       const ubyte& L,
                       const ubyte& S,
                       rgbPixel& pixel) const;
  };
}

#endif

