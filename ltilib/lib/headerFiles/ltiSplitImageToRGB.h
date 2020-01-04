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
 * file .......: ltiSplitImageToRGB.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToRGB.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_RGB_H_
#define _LTI_SPLIT_IMAGE_RGB_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its Red - Green - Blue channels.
   * You can split all the channels at the same time (with apply) or
   * get just one channel using the shortcut functions getRed(), getGreen()
   * or getBlue().
   * @see lti::mergeRGBToImage
   *
   * @ingroup gColor
   */
  class splitImageToRGB : public splitImage {
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
     * Split the image in red green and blue channels.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& img,
                       channel& red,
                       channel& green,
                       channel& blue) const;

    /**
     * split the image in red green and blue channels.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha or dummy channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& img,
                       channel& red,
                       channel& green,
                       channel& blue,
                       channel& alpha) const;

    /**
     * split the image in red green and blue channels.  The values of
     * each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& img,
                       channel8& red,
                       channel8& green,
                       channel8& blue) const;

    /**
     * split the image in red green and blue channels.  The values of
     * each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param red the red channel
     * @param green the green channel
     * @param blue the blue channel
     * @param alpha or dummy channel
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& img,
                       channel8& red,
                       channel8& green,
                       channel8& blue,
                       channel8& alpha) const;

    /**
     * split the pixel in red green and blue values.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& red,
                       float& green,
                       float& blue) const;

    /**
     * split the pixel in red green and blue values.  The values of
     * each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param red the red value
     * @param green the green value
     * @param blue the blue value
     */
    virtual bool apply(const rgbPixel& pixel,
           ubyte& red,
           ubyte& green,
           ubyte& blue) const;

    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param red the red channel
     */
    bool getRed(const image& img,
                channel& red) const;


    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param red the red channel
     */
    bool getRed(const image& img,
                channel8& red) const;

    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param green the green channel
     */
    bool getGreen(const image& img,
                  channel& green) const;


    /**
     * shortcut to get the green channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param green the green channel
     */
    bool getGreen(const image& img,
                  channel8& green) const;

    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param blue the blue channel
     */
    bool getBlue(const image& img,
                 channel& blue) const;


    /**
     * shortcut to get the red channel only
     * The values of each pixel will be between 0 and 255
     * @param img the image to be splitted
     * @param blue the blue channel
     */
    bool getBlue(const image& img,
                 channel8& blue) const;

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

