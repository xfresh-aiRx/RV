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
 * file .......: ltiPolygonApproximation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 16.11.2002
 * revisions ..: $Id: ltiPolygonApproximation.h,v 1.10 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_POLYGON_APPROXIMATION_H_
#define _LTI_POLYGON_APPROXIMATION_H_

#include "ltiModifier.h"
#include "ltiContour.h"
#include "ltiPolygonPoints.h"

namespace lti {
  /**
   * Computes a polygon approximation for a given border point list.
   *
   * This is just a wrapper functor for the method \c approximate of the
   * lti::polygonPoints class.
   */
  class polygonApproximation : public modifier {
  public:
    /**
     * the parameters for the class polygonApproximation
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
       * Minimal "distance" between vertices of the polygon.
       *
       * if 0 or negative, only the maxDistance parameter will be
       * considered).  The "distance" used here is \b NOT the
       * euclidean distance between the vertices but the number of
       * elements between both vertices in the border list.  These
       * is usually therefore a city-block distance or an
       * 8-neighborhood distance depending on the border-points
       * description used.
       *
       * Default value: -1 (i.e. use only maxDistance).
       */
      int minStep;

      /**
       * If \c true, the first two vertices will be computed as the two
       * points in the border with the maximal distance.
       *
       * If \c false, the first point in the list will be a vertex and
       * the border point with the maximal distance to it too.  This
       * faster method is the default.  It provides usually good
       * results.
       *
       * Default value: false
       */
      bool searchMaxDistance;

      /**
       * Specify the maximal allowed \b euclidean distance between the
       * border points and the approximated polygon.  (if negative,
       * each "minStep" pixel of the border points will be
       * taken).
       *
       * Default value: 1
       */
      double maxDistance;

      /**
       * If \c true, only the found vertices will be in the list.
       * If \c false, the last point of the list will be adjacent to the
       * first point in the list.
       *
       * Default: true
       */
      bool closed;
    };

    /**
     * default constructor
     */
    polygonApproximation();

    /**
     * Construct a functor using the given parameters
     */
    polygonApproximation(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    polygonApproximation(const polygonApproximation& other);

    /**
     * destructor
     */
    virtual ~polygonApproximation();

    /**
     * returns the name of this type ("polygonApproximation")
     */
    virtual const char* getTypeName() const;

    /**
     * Approximate the given border points as a polygon.
     *
     * At the end of the approximation process the polygon-points-list
     * contains the coordinates of the vertices.
     *
     * @param src border points with the contour that must be approximated
     * @param dest polygon approximation of the contour.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,polygonPoints& dest) const;

    /**
     * Approximate the given border points as a polygon, ensuring that
     * the points in the given point list are inserted as vertices.
     * This point list \b must fulfill followint two conditions:
     * -# It must have the same sequence than theBorderPoints.
     * -# The points must be contained by the theBorderPoints
     *
     * At the end of the approximation process the polygon-points-list
     * contains the coordinates of the vertices.
     *
     * @param src border points with the contour to be approximated
     * @param forcedVertices list of vertices that must be included in
     *                       the polygon representation.  This list must be a 
     *                       subset and respect  the same ordering as in
     *                       theBorderPoints
     * @param dest polygon approximation of the contour.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
               const pointList& forcedVertices,
               polygonPoints& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    polygonApproximation& copy(const polygonApproximation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    polygonApproximation& operator=(const polygonApproximation& other);

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

