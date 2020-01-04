/*
 * Copyright (C) 2003 Vlad Popovici, EPFL STI-ITS, Switzerland
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


/*--------------------------------------------------------------------
 * file .......: ltiFlipImage.h
 * authors ....: Vlad Popovici
 * organization: EPFL STI-ITS/LTS1
 * creation ...: 20.6.2003
 * revisions ..: $Id: ltiFlipImage.h,v 1.8 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_FLIP_IMAGE_H_
#define _LTI_FLIP_IMAGE_H_

#include "ltiModifier.h"

#undef None

namespace lti {

  /**
   * Flips an image horizontally or vertically.
   *
   * This is a very simple functor that returns a flipped version
   * of the original image. The mirroring can be done along the
   * horizontal or vertical axes, or both.
   *
   * This functor is about 20 times faster in release-mode than using
   * the lti::geometricTransform functor.  In the debug mode it is
   * even 40 times faster!  The reason is that the latter functor is
   * much more general, and the overhead required for the interpolation and
   * rotation (which is zero in this case) is really expensive.
   *
   * @see lti::geometricTransform, lti::rotation, lti::scaling
   *
   * @ingroup gGeometry
   */
  class flipImage : public modifier {
  public:
    /**
     * the parameters for the class flipImage
     */
    class parameters : public modifier::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Flipping directions.
       */
      enum eFlipDirection {
        None = 0,       /**< Do not flip anything */
	Horizontal,     /**< Horizontal flip means to map the top at the 
                             bottom and viceversa */
	Vertical,       /**< Vertical flip means to map the right side into
                             the left and viceversa */
        Both            /**< Turns image 180 degrees */
      };

      /**
       * Axis along which the image will be flipped.
       *
       * Default value: Horizontal
       */
      eFlipDirection direction;

    };

    /**
     * default constructor
     */
    flipImage();

    /**
     * Construct a functor using the given parameters
     */
    flipImage(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    flipImage(const flipImage& other);

    /**
     * destructor
     */
    virtual ~flipImage();

    /**
     * returns the name of this type ("flipImage")
     */
    virtual const char* getTypeName() const;
    
    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<rgbPixel>& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<rgbPixel>& src,matrix<rgbPixel>& dest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<float>& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src,matrix<float>& dest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<ubyte>& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src,matrix<ubyte>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    flipImage& copy(const flipImage& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    flipImage& operator=(const flipImage& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif


