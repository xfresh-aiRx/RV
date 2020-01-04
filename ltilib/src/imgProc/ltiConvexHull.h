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
 * file .......: ltiConvexHull.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 16.11.2002
 * revisions ..: $Id: ltiConvexHull.h,v 1.8 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_CONVEX_HULL_H_
#define _LTI_CONVEX_HULL_H_

#include "ltiModifier.h"
#include "ltiPolygonPoints.h"
#include "ltiContour.h"

namespace lti {
  /**
   * Computes the convex hull of a set of points.
   *
   * This functors is just a wrapper for the method \c convexHullFrom of
   * the lti::tpolygonPoints class.
   */
  class convexHull : public modifier {
  public:
    /**
     * the parameters for the class convexHull
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


    };

    /**
     * default constructor
     */
    convexHull();

    /**
     * Construct a functor using the given parameters
     */
    convexHull(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    convexHull(const convexHull& other);

    /**
     * destructor
     */
    virtual ~convexHull();

    /**
     * returns the name of this type ("convexHull")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const pointList& src,polygonPoints& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const ioPoints& src,polygonPoints& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const tpointList<float>& src,tpolygonPoints<float>& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param dest pointList where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const tpointList<double>& src,
                     tpolygonPoints<double>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    convexHull& copy(const convexHull& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    convexHull& operator=(const convexHull& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif

