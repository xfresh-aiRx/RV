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
 * file .......: ltiSplitImageToGSL.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToGSL.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_GSL_H_
#define _LTI_SPLIT_IMAGE_TO_GSL_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its l1, l2 and l3 components, as described in 
   * T. Gevers and A. Smeulders "Color-based object recognition"
   * Pattern Recognition, Vol. 32, 1999, pp. 453-464.
   *
   * This color space is appropriate when trying to detect only 
   * material changes, but ignoring shadow, shape or highlight edges.
   *
   * The channels are defined as follows:
   * \f[
   * \begin{aligned}
   * l_1 &= \frac{|R-G|}{|R-G| + |R-B| + |G-B|}\\
   * l_2 &= \frac{|R-B|}{|R-G| + |R-B| + |G-B|}\\
   * l_3 &= \frac{|G-B|}{|R-G| + |R-B| + |G-B|}
   * \end{aligned}
   * \f]
   *
   * There is no merge functor for this split method.
   *
   * @ingroup gColor
   */
  class splitImageToGSL : public splitImage {
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
     * split the image in red green and blue channels.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param l1 first channel
     * @param l2 second channel
     * @param l3 third channel
     */
    virtual bool apply(const image& img,
                       channel& l1,
                       channel& l2,
                       channel& l3) const;

    /**
     * split the image in red green and blue channels.  The values of
     * each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param l1 first channel
     * @param l2 second channel
     * @param l3 third channel
     */
    virtual bool apply(const image& img,
                       channel8& l1,
                       channel8& l2,
                       channel8& l3) const;

    /**
     * split the pixel in red green and blue values.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param l1 first channel
     * @param l2 second channel
     * @param l3 third channel
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& l1,
                       float& l2,
                       float& l3) const;

    /**
     * split the pixel in red green and blue values.  The values of
     * each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param l1 first channel
     * @param l2 second channel
     * @param l3 third channel
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& l1,
                       ubyte& l2,
                       ubyte& l3) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param l1 the extracted channel
     */
    virtual bool getFirst(const image& img, channel& l1) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param l1 the extracted channel
     */
    virtual bool getFirst(const image& img, channel8& l1) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param l2 the extracted channel
     */
    virtual bool getSecond(const image& img, channel& l2) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param l2 the extracted channel
     */
    virtual bool getSecond(const image& img, channel8& l2) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param l3 the extracted channel
     */
    virtual bool getThird(const image& img, channel& l3) const;

    /**
     * Returns the third of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param l3 the extracted channel
     */
    virtual bool getThird(const image& img, channel8& l3) const;
  };

} // namespace lti
#endif

