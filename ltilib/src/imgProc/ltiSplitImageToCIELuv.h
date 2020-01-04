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
 * file .......: ltiSplitImageToCIELuv.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToCIELuv.h,v 1.3 2004/01/13 14:51:09 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_CIELUV_H_
#define _LTI_SPLIT_IMAGE_TO_CIELUV_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in its L\f$^*\f$u\f$^*\f$v\f$^*\f$ channels.
   *
   * The following is an excerpt of
   * <a href="http://www.cs.rit.edu/~ncs/color/t_convert.html">this page</a>
   *
   * CIE 1976 \f$L^*u^*v^*\f$ (CIELUV) is
   * based directly on CIE XYZ and is another attempt to linearize the
   * perceptibility of color differences. The non-linear relations for
   * \f$L^*\f$, \f$u^*\f$, and \f$v^*\f$ are given below:
   *
   * \f$L^*\f$ =  116 * (Y/Yn)1/3 - 16<br>
   * \f$u^*\f$ =  13L<sup>*</sup> * ( u' - un' )<br>
   * \f$v^*\f$ =  13L<sup>*</sup> * ( v' - vn' )<br>
   *
   * The quantities un' and vn' refer to the reference white of the
   * light source; for the \f$2^\circ\f$ observer and illuminant C,
   * un' = 0.2009,
   * vn' = 0.4610 [ 1 ]. Equations for u' and v' are given below:
   *
   *   u' = 4X / (X + 15Y + 3Z) = 4x / ( -2x + 12y + 3 )<br>
   *   v' = 9Y / (X + 15Y + 3Z) = 9y / ( -2x + 12y + 3 )<br>
   *
   * The transformation from (u',v') to (x,y) is:
   *
   * x = 27u' / ( 18u' - 48v' + 36 )<br>
   * y = 12v' / ( 18u' - 48v' + 36 )<br>
   *
   * The transformation from CIELUV to XYZ is performed as following:
   *
   * u' = u / ( 13\f$L^*\f$) + un<br>
   * v' = v / ( 13\f$L^*\f$ ) + vn<br>
   * Y = (( \f$L^*\f$ + 16 ) / 116 )3<br>
   * X = - 9Yu' / (( u' - 4 ) v' - u'v' )<br>
   * Z = ( 9Y - 15v'Y - v'X ) / 3v'
   *
   *
   * The \f$L^*\f$ value with these definitions will be between 0 and 100.
   * This functor ensure that the outputs will be put into the usual
   * values in the LTI-Lib, i.e. for channels the outputs will be normalized
   * by 100, so that the luminance channel will be between 0 and 1.0.
   *
   * For channel8 outputs this color space doesn't make much sense, since some
   * values are negative.
   *
   * @ingroup gColor
   */
  class splitImageToCIELuv : public splitImage {
  public:
    /**
     * default constructor.
     * Initializes (only once) the lut for the cubic root, with 2048 entries.
     */
    splitImageToCIELuv();

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
     * Note that the output channels will contain values outside the interval
     * 0.0-1.0.
     */
    virtual bool apply(const image& img,
           channel& c1,
           channel& c2,
           channel& c3) const;

    /**
     * on-copy
     * Note that the output values will contain values outside the interval
     * 0.0-1.0.
     */
    virtual bool apply(const rgbPixel& pixel,
           float& c1,
           float& c2,
           float& c3) const;

  protected:

    /**
     * on-copy
     * Do not use this method yet!  The output channels will contain invalid
     * data due to underflows and overflows of the channel valid value range!
     */
    virtual bool apply(const image& img,
                       channel8& c1,
                       channel8& c2,
                       channel8& c3) const;

    /**
     * on-copy
     * Do not use this method yet!  The output values will contain invalid
     * data due to underflows and overflows of the valid value range!
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& c1,
                       ubyte& c2,
                       ubyte& c3) const;

    /**
     * look-up-table for cubic root
     */
    static const float* lut;
    /**
     * size of the lut for cubic root
     */
    static const int lutSize;
  };

} // namespace lti
#endif

