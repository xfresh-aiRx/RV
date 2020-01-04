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
 * file .......: ltiSplitImageToGSC.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToGSC.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_GSC_H_
#define _LTI_SPLIT_IMAGE_TO_GSC_H_

#include "ltiSplitImage.h"
#include "ltiArctanLUT.h"

namespace lti {

  /**
   * Split image in its c1, c2 and c3 components, as described in 
   * T. Gevers and A. Smeulders "Color-based object recognition"
   * Pattern Recognition, Vol. 32, 1999, pp. 453-464.
   *
   * This color space is appropriate when trying to detect only highlight or
   * material changes, but ignoring shadow or shape edges.
   *
   * The channels are defined as follows:
   * \f[
   * \begin{aligned}
   * c_1 &= \arctan \left( \frac{R}{\max(G,B)} \right) \cdot \frac{2}{\pi}\\
   * c_2 &= \arctan \left( \frac{G}{\max(R,B)} \right) \cdot \frac{2}{\pi}\\
   * c_3 &= \arctan \left( \frac{B}{\max(R,G)} \right) \cdot \frac{2}{\pi}
   * \end{aligned}
   * \f]
   *
   * They denote the angles of the body reflection vector and are invariant for
   * matte, dull objects.
   *
   * Please note the slightly difference with the original paper definition:
   * all channels are normalized in order to get values between 0 and 1.
   *
   * There is no merge functor for this split method.
   *
   * @ingroup gColor
   */
  class splitImageToGSC : public splitImage {
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
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     */
    virtual bool apply(const image& img,
                       channel& c1,
                       channel& c2,
                       channel& c3) const;

    /**
     * split the image in red green and blue channels.  The values of
     * each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     */
    virtual bool apply(const image& img,
                       channel8& c1,
                       channel8& c2,
                       channel8& c3) const;

    /**
     * split the pixel in red green and blue values.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     */
    virtual bool apply(const rgbPixel& pixel,
           float& c1,
           float& c2,
           float& c3) const;

    /**
     * split the pixel in red green and blue values.  The values of
     * each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     */
    virtual bool apply(const rgbPixel& pixel,
           ubyte& c1,
           ubyte& c2,
           ubyte& c3) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c1 the extracted channel
     */
    virtual bool getFirst(const image& img, channel& c1) const;

    /**
     * Returns the first of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c1 the extracted channel
     */
    virtual bool getFirst(const image& img, channel8& c1) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c2 the extracted channel
     */
    virtual bool getSecond(const image& img, channel& c2) const;

    /**
     * Returns the second of the three channels into which the image is split.
     * If you need only one channel, this might be faster than calling apply().
     * @param img the source image
     * @param c2 the extracted channel
     */
    virtual bool getSecond(const image& img, channel8& c2) const;

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

  protected:
    /**
     * arctan LUT
     */
    arctanLUT atan2;
  };

} // namespace lti
#endif

