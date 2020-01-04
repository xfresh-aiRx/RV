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
 * file .......: ltiSplitImageToYIQ.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToYIQ.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_YIQ_H_
#define _LTI_SPLIT_IMAGE_TO_YIQ_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its Luminance Inphase Quadrature channels.
   *
   * The following is an excerpt of
   * <a href="http://www.cs.rit.edu/~ncs/color/t_convert.html">this page</a>
   *
   * The YIQ system is the color primary system adopted by National
   * Television System Committee (NTSC) for color TV broadcasting. The
   * YIQ color solid is made by a linear transformation of the RGB
   * cube. Its purpose is to exploit certain characteristics of the
   * human eye to maximize the utilization of a fixed bandwidth. The
   * human visual system is more sensitive to changes in luminance
   * than to changes in hue or saturation, and thus a wider bandwidth
   * should be dedicated to luminance than to color information. Y is
   * similar to perceived luminance, I and Q carry color information
   * and some luminance information. The Y signal usually has 4.2 MHz
   * bandwidth in a 525 line system. Originally, the I and Q had
   * different bandwidths (1.5 and 0.6 MHz), but now they commonly
   * have the same bandwidth of 1 MHz.
   *
   * Here is the RGB -> YIQ conversion:
   *
   * [ Y ]     [ 0.299   0.587   0.114 ] [ R ]<br>
   * [ I ]  =  [ 0.596  -0.275  -0.321 ] [ G ]<br>
   * [ Q ]     [ 0.212  -0.523   0.311 ] [ B ]<br>
   *
   * Here is the YIQ -> RGB conversion:
   *
   * [ R ]     [ 1   0.956   0.621 ] [ Y ]<br>
   * [ G ]  =  [ 1  -0.272  -0.647 ] [ I ]<br>
   * [ B ]     [ 1  -1.105   1.702 ] [ Q ]<br>
   *
   * @ingroup gColor
   */
  class splitImageToYIQ : public splitImage {
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
     * split the image in YIQ.
     * YIQ splitting produces unnormalized channels !
     * @param img the image to be splitted
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     */
    virtual bool apply(const image& img,
                       channel& Y,
                       channel& I,
                       channel& Q) const;

    /**
     * split the image in YIQ.
     * YIQ splitting produces unnormalized channels !
     * @param img the image to be splitted
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     */
    virtual bool apply(const image& img,
                       channel8& Y,
                       channel8& I,
                       channel8& Q) const;

    /**
     * split the pixel in YIQ.
     * YIQ splitting produces unnormalized values !
     * @param pixel the pixel to be splitted
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& Y,
                       float& I,
                       float& Q) const;

    /**
     * split the pixel in YIQ.
     * YIQ splitting produces unnormalized values !
     * @param pixel the pixel to be splitted
     * @param Y the perceived luminance
     * @param I color information and some luminance
     * @param Q color information and some luminance
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& Y,
                       ubyte& I,
                       ubyte& Q) const;
  };


} // namespace lti
#endif

