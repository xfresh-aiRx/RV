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
 * file .......: ltiDraw3DDistribution.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 21.1.2003
 * revisions ..: $Id: ltiDraw3DDistribution.h,v 1.9 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_DRAW3_D_DISTRIBUTION_H_
#define _LTI_DRAW3_D_DISTRIBUTION_H_

#include "ltiFunctor.h"
#include "ltiScene3D.h"
#include "ltiRGBPixel.h"
#include <map>
#include <vector>

namespace lti {
  /**
   *  Draws a three dimensional distribution.
   *  With this functor a three dimensional distribution is drawn.
   *  If the functor gets an id vector that gives each point in the
   *  distribution an id, all points with the same id get the same color and
   *  all differnt ids get different color. By default there is a colorMap
   *  with 15 different colors. If there are more than 15 different ids,
   *  different markers will be used.
   *  The apply methods, that haven't an id vector use the first not used
   *  negativ id to save the color used for drawing this distribution.
   *  With the hold option you can choose, if the old scene is erased before 
   *  the distribution is drawn or if it is drawn into the same scene.
   */
  class draw3DDistribution : public functor {
  public:
    /**
     * the parameters for the class draw3DDistribution
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
     * the palette that is used to draw distribution with differnt ids.
     */
    std::vector<rgbPixel> colorMap;

    /**
     * the markers that are used to draw distribution with differnt ids.
     */
    std::vector<std::string> markerMap;

    /**
     * if true, all three axis are scaled equal, otherwise each axis has
     * its own scaling factor.
     */
    bool scaleEqual;

    /**
     * Number of Ticks on each axis.
     */
    int nbTicks;

    /**
     * Color of the background grid.
     */
    rgbPixel gridColor;

    /**
     * If true a grid is drawn in the background of the distribution
     */
    bool grid;

    /**
     * If true the x-axis is red, the y-axis green and z-axis is yellow
     */
    bool colouredAxes;

    };

    /**
     * default constructor
     */
    draw3DDistribution();

    /**
     * Construct a functor using the given parameters
     */
    draw3DDistribution(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    draw3DDistribution(const draw3DDistribution& other);

    /**
     * destructor
     */
    virtual ~draw3DDistribution();

    /**
     * returns the name of this type ("draw3DDistribution")
     */
    virtual const char* getTypeName() const;

    /**
     * Draws a three dimensional distribution
     * @param drawer the scene that is used for drawing
     * @param dist the 3 dimensional distribution that is drawn.
     * @param ids the ids corresponding to the points in the distribution
     * @param hold if true the old scene is not erased.
     * @return true if apply successful or false otherwise.
     */
    bool apply(scene3D<rgbPixel>& drawer,const dmatrix& dist,
               const ivector& ids, const bool& hold=false) const;

    /**
     * Draws a three dimensional distribution. For the id the first free
     * negativ id is chosen.
     * @param drawer the scene that is used for drawing
     * @param dist the 3 dimensional distribution that is drawn.
     * @param hold if true the old scene is not erased.
     * @return true if apply successful or false otherwise.
     */
    bool apply(scene3D<rgbPixel>& drawer,const dmatrix& dist,
               const bool& hold=false) const;

    /**
     * Draws a three dimensions of a high dimensional distribution
     * @param drawer the scene that is used for drawing
     * @param dist the distribution
     * @param first the column in dist with the first dimension.
     * @param second the column in dist with the second dimension.
     * @param third the column in dist with the third dimension.
     * @param hold if true the old scene is not erased.
     * @return true if apply successful or false otherwise.
     */
    bool apply(scene3D<rgbPixel>& drawer,const dmatrix& dist,
               int first, int second, int third, const bool& hold=false) const;

    /**
     * Draws a three dimensions distribution.
     * @param drawer the scene that is used for drawing
     * @param x the first dimension.
     * @param y the second dimension.
     * @param z the third dimension.
     * @param hold if true the old scene is not erased.
     * @return true if apply successful or false otherwise.
     */
    bool apply(scene3D<rgbPixel>& drawer,const dvector& x,const dvector& y,
               const dvector& z,const bool& hold=false) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    draw3DDistribution& copy(const draw3DDistribution& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    draw3DDistribution& operator=(const draw3DDistribution& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * Saves the assignment of ids to colors.
     */
     std::map<int,rgbPixel> *idsToColor;

    /**
     * Saves the assignment of ids to marker types.
     */
     std::map<int,std::string> *idsToMarker;
   
  };
}

#endif

