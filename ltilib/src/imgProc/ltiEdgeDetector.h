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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiEdgeDetector.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2002
 * revisions ..: $Id: ltiEdgeDetector.h,v 1.14 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_EDGE_DETECTOR_H_
#define _LTI_EDGE_DETECTOR_H_

#include "ltiModifier.h"

namespace lti {
  /**
   * Parent abstract class for all edge detectors.
   *
   * An edge detector finds the edges in an gray-valued image.  The
   * definition of "edge" is usually related to an abrupt change in
   * the intensity value of pixels.
   *
   * The result will be usually a channel8 containing only the two
   * values specified in the parameter to denote edge and no-edge.
   */
  class edgeDetector : public modifier {
  public:
    /**
     * the parameters for the class edgeDetector
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
       * Value used to denote a no-edge pixel.
       *
       * The value must be between 0 and 255.  For channels the used
       * value will be scaled by 255.
       *
       * Default value: 0
       */
      ubyte noEdgeValue;

      /**
       * Value used to denote an edge pixel
       *
       * The value must be between 0 and 255.  For channels the used
       * value will be scaled by 255.
       *
       * Default value: 255
       */
      ubyte edgeValue;

    };

    /**
     * default constructor
     */
    edgeDetector();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    edgeDetector(const edgeDetector& other);

    /**
     * destructor
     */
    virtual ~edgeDetector();

    /**
     * returns the name of this type ("edgeDetector")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel8& dest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the result in each channel of image.
     * @param srcdest image with the source data.  The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the result in each channel of the destination image.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the sum of the results in the given channel.  Note that
     * if you give as parameters::edgeValue or parameters::noEdgeValue values
     * greater than 255/3, you can have some overflow effects on the resulting
     * channel.
     *
     * @param src image with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,channel8& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    edgeDetector& copy(const edgeDetector& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    edgeDetector& operator=(const edgeDetector& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

