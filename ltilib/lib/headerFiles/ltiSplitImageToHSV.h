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
 * file .......: ltiSplitImageToHSV.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToHSV.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_HSV_H_
#define _LTI_SPLIT_IMAGE_TO_HSV_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its Hue - Saturation - Value channels.
   *
   * The "value" corresponds to the maximum of the three R,G,B values.
   *
   * @ingroup gColor
   */
  class splitImageToHSV : public splitImage {
  public:

    /**
     * returns the name of this type
     */
    virtual const char* getTypename() const;

    /**
     * returns a pointer to a clone of the functor
     */
    virtual functor* clone() const;

    /**
     * split the image in hue channel H, saturation S and value
     * channel V.
     * The values of each image will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     */
    virtual bool apply(const image& img,
                       channel& H,
                       channel& S,
                       channel& V) const;

    /**
     * split the image in hue channel H, saturation S and value
     * channel V.
     * The values of each image will be between 0 and 255
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param V the value channel
     */
    virtual bool apply(const image& img,
                       channel8& H,
                       channel8& S,
                       channel8& V) const;

    /**
     * split the pixel in hue value H, saturation S and Value V.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param V the Value value
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& H,
                       float& S,
                       float& V) const;

    /**
     * split the pixel in hue value H, saturation S and Value V.
     * The values of each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param V the Value value
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& H,
                       ubyte& S,
                       ubyte& V) const;

    /**
     * shortcut to get the value channel only The values of each pixel
     * will be between 0.0f and 1.0f.
     * @param img the image to be splitted
     * @param value the value channel
     */
    bool getValue(const image& img,
                  channel& value) const;


    /**
     * shortcut to get the value channel only The values of each pixel
     * will be between 0 and 255.
     * @param img the image to be splitted
     * @param value the value channel
     */
    bool getValue(const image& img,
                  channel8& value) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c3 the extracted channel
     */
    virtual bool getThird(const image& img, channel& c3) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c3 the extracted channel
     */
    virtual bool getThird(const image& img, channel8& c3) const;

  };

} // namespace lti
#endif

