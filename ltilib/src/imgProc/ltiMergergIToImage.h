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
 * file .......: ltiMergergIToImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergergIToImage.h,v 1.1 2003/11/14 16:07:22 alvarado Exp $
 */

#ifndef _LTI_MERGE_RGI_TO_IMAGE_H_
#define _LTI_MERGE_RGI_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge chromaticity channels rgI
   *
   * @ingroup gColor
   */
  class mergergIToImage : public mergeImage {
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
     * merge the chromacity channels r and g
     * and intensity channel into a pixel
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     * @param pixel the pixel to be splitted
     */
    virtual bool apply(const matrix<float>& r,
                       const matrix<float>& g,
                       const matrix<float>& I,
                       image& pixel) const;

    /**
     * merge the chromacity channels r and g
     * and intensity channel into a pixel
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     * @param pixel the pixel to be splitted
     */
    virtual bool apply(const channel8& r,
                       const channel8& g,
                       const channel8& I,
                       image& pixel) const;

    /**
     * merge the chromacity values r and g and intensity value
     * into a pixel
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     * @param pixel the pixel to be splitted
     */
    virtual bool apply(const float& r,
		       const float& g,
		       const float& I,
		       rgbPixel& pixel) const;

    /**
     * merge the chromacity values r and g and intensity value
     * into a pixel
     * @param r the chromacity value
     * @param g the chromacity value
     * @param I the intensity value
     * @param pixel the pixel to be splitted
     */
    virtual bool apply(const ubyte& r,
		       const ubyte& g,
		       const ubyte& I,
		       rgbPixel& pixel) const;

  };
}

#endif

