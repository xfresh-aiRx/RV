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
 * file .......: ltiSplitImageToHSI.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToHSI.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_HSI_H_
#define _LTI_SPLIT_IMAGE_TO_HSI_H_

#include "ltiSplitImage.h"

namespace lti {

  // declared somewhere else...
  template<class T> class triMatrix;

  /**
   * Split image in its Hue - Saturation - Intensity channels
   *
   * @ingroup gColor
   */
  class splitImageToHSI : public splitImage {
  public:
    /**
     * Constructor
     */
    splitImageToHSI(void);

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * split the image in hue channel H, saturation S and intensity
     * channel I.
     * The values of each image will be between 0.0f and 1.0f
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param I the intensity channel
     */
    virtual bool apply(const image& img,
                       channel& H,
                       channel& S,
                       channel& I) const;

    /**
     * split the image in hue channel H, saturation S and intensity
     * channel I.
     * The values of each image will be between 0 and 255
     * @param img the image to be splitted
     * @param H the hue channel
     * @param S the saturation channel
     * @param I the intensity channel
     */
    virtual bool apply(const image& img,
                       channel8& H,
                       channel8& S,
                       channel8& I) const;

    /**
     * split the pixel in hue value H, saturation S and intensity
     * value I.
     * The values of each pixel will be between 0.0f and 1.0f
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param I the intensity value
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& H,
                       float& S,
                       float& I) const;

    /**
     * split the pixel in hue value H, saturation S and intensity
     * value I.
     * The values of each pixel will be between 0 and 255
     * @param pixel the pixel to be splitted
     * @param H the hue value
     * @param S the saturation value
     * @param I the intensity value
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& H,
                       ubyte& S,
                       ubyte& I) const;

    /**
     * return the hue of the image.  If you need also the saturation and
     * the intensity please use the apply methods, which are much faster!
     */
    bool getHue(const image& img, channel& hue) const;

    /**
     * return the hue of the image.  If you need also the saturation and
     * the intensity please use the apply methods, which are much faster!
     */
    bool getHue(const image& img, channel8& hue) const;

    /**
     * return the saturation of the image.  If you need also the hue and
     * the intensity please use the apply methods, which are much faster!
     */
    bool getSaturation(const image& img, channel& saturation) const;

    /**
     * return the saturation of the image.  If you need also the hue and
     * the saturation please use the apply methods, which are much faster!
     */
    bool getSaturation(const image& img, channel8& saturation) const;

    /**
     * return the intensity of the image.  If you need also the hue and
     * the intensity please use the apply methods, which are much faster!
     */
    bool getIntensity(const image& img, channel& intensity) const;

    /**
     * return the intensity of the image.  If you need also the hue and
     * the intensity please use the apply methods, which are much faster!
     */
    bool getIntensity(const image& img, channel8& intensity) const;


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

  private:
    /**
     * Triangular matrix for RGB-HSI-Conversion
     */
    static triMatrix<int>* HueHSI;

    /**
     * HueHSI initialized?
     * The conversion matrix (LUT) needs to be initialized only once!
     */
    static bool hueInitialized;
  };

} // namespace lti
#endif

