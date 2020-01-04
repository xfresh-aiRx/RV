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
 * file .......: ltiMergeRGBToImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergeRGBToImage.h,v 1.2 2004/01/11 10:35:46 alvarado Exp $
 */

#ifndef _LTI_MERGE_RGB_TO_IMAGE_H_
#define _LTI_MERGE_RGB_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge RGB channels
   *
   * Merge the three given channels in a color image.
   * @see splitImageToRGB
   * @ingroup gColor
   */
  class mergeRGBToImage : public mergeImage {
  public:
    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * merge the channels red, green and blue to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param img the merged image
     */
    virtual bool apply(const matrix<float>& red,
                       const matrix<float>& green,
                       const matrix<float>& blue,
                       image& img) const;

    /**
     * merge the channels red, green and blue to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param img the merged image
     */
    virtual bool apply(const channel8& red,
                       const channel8& green,
                       const channel8& blue,
                       image& img) const;

    /**
     * Merge the channels red, green, blue and the dummy (or alpha) to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha the alpha or dummy channel
     * @param img the merged image
     */
    bool apply(const matrix<float>& red,
               const matrix<float>& green,
               const matrix<float>& blue,
               const matrix<float>& alpha,
               image& img) const;

    /**
     * Merge the channels red, green, blue and the dummy (or alpha) to an image
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha the alpha or dummy channel
     * @param img the merged image
     */
    bool apply(const channel8& red,
               const channel8& green,
               const channel8& blue,
               const channel8& alpha,
               image& img) const;

    /**
     * merge the values red, green and blue to a pixel.
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     * @param pixel the pixel to be splitted
     */
    virtual bool apply(const float& red,
		       const float& green,
		       const float& blue,
		       rgbPixel& pixel) const;

    /**
     * merge the values red, green and blue to a pixel
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     * @param pixel the merged pixel
     */
    virtual bool apply(const ubyte& red,
		       const ubyte& green,
		       const ubyte& blue,
		       rgbPixel& pixel) const;
  };
}

#endif

