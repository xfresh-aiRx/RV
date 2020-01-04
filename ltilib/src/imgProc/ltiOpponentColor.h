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
 * file .......: ltiOpponentColor.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 7.8.2001
 * revisions ..: $Id: ltiOpponentColor.h,v 1.10 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_OPPONENT_COLOR_H_
#define _LTI_OPPONENT_COLOR_H_

#include "ltiTransform.h"
#include "ltiImage.h"

namespace lti {
  /**
   * This functor generates an "opponent color" channel from the given two
   * channels, one representing the center, the other representing
   * the surround.
   *
   * The idea is to inhibit the response of a center region in a receptive
   * field with the responce of a surround region.
   *
   * \code
   *
   *
   *                       ________
   *                      /        \
   *                     /   ____   \
   *                    |   /    \   |
   *                    |  |Center|  |
   *                    |   \____/   |
   *                     \ Surround /
   *                      \________/
   *
   * \endcode
   *
   * Usually, the surround pyramid-level is higher (lower resolution)
   * than the center, to simulate this consideration of a larger region
   * in the receptive field.
   *
   * Common used opponent color channels (Center-Surround) are the Red-Green,
   * Green-Red, Blue-Yellow, Yellow-Blue.  But of course other combinations
   * can be employed (-Red+Green, etc.).
   *
   */
  class opponentColor : public transform {
  public:
    /**
     * the parameters for the class opponentColor
     */
    class parameters : public transform::parameters {
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
       * defines the level of the GaussianPyramid for the center channel.
       * default: 0
       */
      lti::ubyte centerLevel;

      /**
       * Multiply factor for the center channel before adding to the opponent
       * channel (i.e. the sourround channel)
       * default: -1
       */
      float centerSign;

      /**
       * defines the level of the GaussianPyramid for the surround channel.
       * default: 1
       */
      lti::ubyte surroundLevel;

      /**
       * Multiply factor for the surround channel
       * before adding to the opponent channel (i.e. the center channel)
       * default: +1
       */
      float surroundSign;

      /**
       * Type to specify the way negative values should be considered.
       */
      enum eFunctionType{
	Normal, /**< linear condition, the negative values are keep as they
                     are */
	Abs,    /**< absolute value, the negative values are multiplied with
                     -1 */
	Ramp    /**< rectification, negative values are not allowed, and will
                     be mapped to zero */
      };

      /**
       * defines the handle with the negative values in the dest-image
       * Normal: no changes
       * Abs   : absolute value
       * Ramp  : values<0 to zero
       */
      eFunctionType outFunc;

    };

    /**
     * default constructor
     */
    opponentColor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    opponentColor(const opponentColor& other);

    /**
     * destructor
     */
    virtual ~opponentColor();

    /**
     * returns the name of this type ("opponentColor")
     */
    virtual const char* getTypeName() const;

    /**
     * @param centerSrc channel with the source data.
     * @param surroundSrc channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::channel& centerSrc,
               const lti::channel& surroundSrc,
                     lti::channel& dest) const;

    /*
     * standard-apply-method with parameter-handling
     * @param centerSrc channel with the source data.
     * @param surroundSrc channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const lti::channel& centerSrc,
               const lti::channel& surroundSrc,
               const int& centerLevel,
               const float& centerSign,
               const int& surroundLevel,
               const float& surroundSign,
               const parameters::eFunctionType& outFunc,
                     lti::channel& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opponentColor& copy(const opponentColor& other);

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

