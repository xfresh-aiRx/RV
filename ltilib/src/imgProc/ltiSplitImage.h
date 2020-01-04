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
 * file .......: ltiSplitImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImage.h,v 1.3 2004/05/03 12:18:39 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_H_
#define _LTI_SPLIT_IMAGE_H_

#include "ltiFunctor.h"
#include "ltiImage.h"

namespace lti {

  /**
   * Parent for all classes that split image into differen color
   * spaces components (color channels).
   *
   * \#include "ltiSplitImage.h"
   *
   * Split color image into color-channels.
   *
   * @ingroup gColor
   */
  class splitImage : public functor {
  public:
    /**
     * default constructor
     */
    splitImage()
      : functor() {};
    /**
     * destructor
     */
    virtual ~splitImage() {};

    /**
     * on-copy operator for 32-bit "floating point" channels
     */
    virtual bool apply(const image& img,
                       channel& c1,
                       channel& c2,
                       channel& c3) const = 0;
    /**
     * on-copy operator for 8-bit channels
     */
    virtual bool apply(const image& img,
                       channel8& c1,
                       channel8& c2,
                       channel8& c3) const = 0;

    /**
     * on-copy operator for 32-bit floating point values
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& c1,
                       float& c2,
                       float& c3) const = 0;
    
    /**
     * on-copy operator for 8-bit values
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& c1,
                       ubyte& c2,
                       ubyte& c3) const = 0;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

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
     * return the minimum of three integers
     */
    virtual int maximum(const int a, const int b, const int c) const;

    /**
     * return the maximum of three integers
     */
    virtual int minimum(const int a, const int b, const int c) const;

  };

} // namespace lti
#endif

