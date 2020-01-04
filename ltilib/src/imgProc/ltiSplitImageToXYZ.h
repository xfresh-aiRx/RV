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
 * file .......: ltiSplitImageToXYZ.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToXYZ.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_XYZ_H_
#define _LTI_SPLIT_IMAGE_TO_XYZ_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its XYZ norm channels
   *
   * The following is an excerpt of
   * <a href="http://www.cs.rit.edu/~ncs/color/t_convert.html">this page</a>
   *
   * RGB values in a particular set of primaries can be transformed to
   * and from CIE XYZ via a 3x3 matrix transform. These transforms
   * involve tristimulus values, that is a set of three linear-light
   * components that conform to the CIE color-matching functions. CIE
   * XYZ is a special set of tristimulus values. In XYZ, any color is
   * represented as a set of positive values.
   *
   * To transform from XYZ to RGB (with D65 white point), the matrix
   * transform used is [3]:
   *
   *   [ R ]   [  3.240479 -1.537150 -0.498535 ]   [ X ]<br>
   *   [ G ] = [ -0.969256  1.875992  0.041556 ] * [ Y ]<br>
   *   [ B ]   [  0.055648 -0.204043  1.057311 ]   [ Z ]<br>
   *
   * The range for valid R, G, B values is [0,1]. Note, this matrix
   * has negative coefficients. Some XYZ color may be transformed to
   * RGB values that are negative or greater than one. This means that
   * not all visible colors can be produced using the RGB system.
   *
   * The inverse transformation matrix is as follows:
   *
   *   [ X ]   [ 0.412453  0.357580  0.180423 ]   [ R ]<br>
   *   [ Y ] = [ 0.212671  0.715160  0.072169 ] * [ G ]<br>
   *   [ Z ]   [ 0.019334  0.119193  0.950227 ]   [ B ]<br>
   *
   *
   * @ingroup gColor
   */
  class splitImageToXYZ : public splitImage {
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
     * on-copy
     */
    virtual bool apply(const image& img,
                       channel& c1,
                       channel& c2,
                       channel& c3) const;
    /**
     * on-copy
     */
    virtual bool apply(const image& img,
                       channel8& c1,
                       channel8& c2,
                       channel8& c3) const;

    /**
     * on-copy
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& c1,
                       float& c2,
                       float& c3) const;
    /**
     * on-copy
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& c1,
                       ubyte& c2,
                       ubyte& c3) const;

  };

} // namespace lti
#endif

