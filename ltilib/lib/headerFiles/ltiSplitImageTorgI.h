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
 * file .......: ltiSplitImageTorgI.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageTorgI.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_RGI_H_
#define _LTI_SPLIT_IMAGE_TO_RGI_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its chromaticity channels.
   *
   * The chromaticity channels are defined as follows:
   * - intensity channel \f$ I = \frac{R+G+B}{3} \f$
   * - chromacity red    \f$ r = \frac{R}{R+G+B} \f$
   * - chromacity green  \f$ g = \frac{G}{R+G+B} \f$
   *
   * You can get all channels at the same time using apply() or just
   * get one using the shortcut methods getR(), getG() or getIntensity().
   *
   * @ingroup gColor
   */
  class splitImageTorgI : public splitImage {
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
     * split the image in chromacity channels r and g and intensity
     * channel.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param r the chromacity channel
     * @param g the chromacity channel
     * @param I the intensity channel
     */
    virtual bool apply(const image& img,
                       channel& r,
                       channel& g,
                       channel& I) const;

    /**
     * split the image in chromacity channels r and g and intensity
     * channel.
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param r the chromacity channel
     * @param g the chromacity channel
     * @param I the intensity channel
     */
    virtual bool apply(const image& img,
                       channel8& r,
                       channel8& g,
                       channel8& I) const;

    /**
     * split the pixel in chromacity values r and g and intensity
     * value.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& r,
                       float& g,
                       float& I) const;

    /**
     * split the pixel in chromacity values r and g and intensity
     * value.
     * The values of each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& r,
                       ubyte& g,
                       ubyte& I) const;

    /** shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param r the chromacity red channel
     */
    bool getR(const image& img,
              channel& r) const;


    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param r the chromacity red channel
     */
    bool getR(const image& img,
              channel8& r) const;

    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param g the chromacity green channel
     */
    bool getG(const image& img,
              channel& g) const;

    /**
     * shortcut to get the green channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param g the chromacity green channel
     */
    bool getG(const image& img,
              channel8& g) const;

    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param I the intensity channel
     */
    bool getIntensity(const image& img,
                      channel& I) const;


    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param I the intensity channel
     */
    bool getIntensity(const image& img,
                      channel8& I) const;

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

  };

} // namespace lti
#endif

