/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiRegionsPolygonizer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 1.10.2003
 * revisions ..: $Id: ltiRegionsPolygonizer.h,v 1.5 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_REGIONS_POLYGONIZER_H_
#define _LTI_REGIONS_POLYGONIZER_H_

#include "ltiImage.h"
#include "ltiPolygonPoints.h"
#include "ltiFastRelabeling.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This functor takes a labeled mask and for each region therein it
   * computes a polygon approximation of its contour.  Additionally,
   * it ensures that those pixels in the contour with at least two
   * neighbors of different classes will be taken as polygon vertices.
   *
   * 
   */
  class regionsPolygonizer : public functor {
  public:
    /**
     * the parameters for the class regionsPolygonizer
     */
    class parameters : public functor::parameters {
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
       * When approximating a contour with a polygon, this is the maximum
       * deviation allowed between the polygon sides and the contour.
       *
       * Default value: 1
       */
      double maxPixelDistance;

      /**
       * Detect region changes.
       *
       * If true, this flag indicates that the polygon approximation must
       * contain as vertices those points where the neighbor region change,
       * independently if the polygon approximation without them would also
       * be good enough.  
       *
       * If false, only the border points of the region will
       * be considered, ignoring the background.
       *
       * Default value: true
       */
      bool detectNeighbors;
    };

    /**
     * default constructor
     */
    regionsPolygonizer();

    /**
     * Construct a functor using the given parameters
     */
    regionsPolygonizer(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regionsPolygonizer(const regionsPolygonizer& other);

    /**
     * destructor
     */
    virtual ~regionsPolygonizer();

    /**
     * returns the name of this type ("regionsPolygonizer")
     */
    virtual const char* getTypeName() const;

    /**
     * This apply method computes for each region in src a polygon
     * representation of its border.
     *
     * @param src input labeled mask
     * @param maxLabel maximum label in the input mask, (if you don't know it,
     *                 give for example src.maximum())
     * @param polygons polygon representation of each region indexed by their
     *                id.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int>& src,
               const int maxLabel,
                     std::vector<polygonPoints>& polygons) const;


    /**
     * Computes for each region in src:
     * - approximating polygon 
     * - border 
     * - at the border points the label of the neighbor region.
     *
     * The labeled mask given in \a src \b must contain only connected
     * regions, identified by an unique integer id, as given by functors like
     * lti::fastRelabeling.
     *
     * The maximum labeld in \a src must be given to save some time if
     * possible.
     *
     * @param src input labeled mask
     * @param maxLabel maximum label in the input mask, (if you don't know it,
     *                 give for example src.maximum())
     * @param polygons polygon representation of each region indexed by their
     *                id.
     * @param borders  border points for each region.
     * @param neighbors matrix containing for each border pixel the id of
     *                  a neighbor region and for all other pixels the
     *                  own region id.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<int>& src,
               const int maxLabel,
                     std::vector<polygonPoints>& polygons,
                     std::vector<borderPoints>& borders,
                     matrix<int>& neighbors) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionsPolygonizer& copy(const regionsPolygonizer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionsPolygonizer& operator=(const regionsPolygonizer& other);

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

