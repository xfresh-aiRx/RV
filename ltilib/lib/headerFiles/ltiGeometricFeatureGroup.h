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
 * file .......: ltiGeometricFeatureGroup.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 22.07.2003
 * revisions ..:
 */

#ifndef LTI_GeometricFeatureGroup_H
#define LTI_GeometricFeatureGroup_H

//#include <iostream>
#include "ltiIoObject.h"
//#include "ltiIoHandler.h"
#include "ltiConfig.h"
#include "ltiRectangle.h"

/**
 * @file ltiGeometricFeatureGroup.h
 *
 * This file contains the definitions of the geometric feature types
 * used in the LTI-Lib to describe objects by their geometric features.
 */

namespace lti {

  /**
   * Description of the simpliest geometric features given by the rectangle
   * surrounding the object, the object's area and the center of gravity.
   *
   * Other geometric feature classes can be derived.
   *
   * @ingroup gGeomData
   */
  class geometricFeatureGroup0 : public ioObject {
  public:
    /**
     * default constructor
     */
    geometricFeatureGroup0();

    /**
     * constructor
     */
    geometricFeatureGroup0(const point& initMinX, const point& initMaxX,
			   const point& initMinY, const point& initMaxY,
			   const int initArea, 
			   const tpoint<float>& initCog);

    /**
     * copy constructor
     */
    geometricFeatureGroup0(const geometricFeatureGroup0& other);

    /**
     * @name Duplication
     */
    //@{

    /**
     * copy member
     */
    geometricFeatureGroup0& copy(const geometricFeatureGroup0& other);

    /**
     * alias for copy
     */
    geometricFeatureGroup0& operator=(const geometricFeatureGroup0& other);

    //@}

    /**
     * @name Computation of the geometric features.
     */
    //@{

    /**
     * Partially recomputes the geometric feature vector considering the new point.
     * This method assumes, that the center of gravity contains the sum of all added
     * points' coordinates so far. To compute the center of gravity from the sum you
     * must call completeComputation(). Use with care.
     */
    bool addObjectPoint(int x, int y);

    /**
     * Partially recomputes the geometric feature vector considering the new point.
     * This method assumes, that the center of gravity contains the sum of all added
     * points' coordinates so far. To compute the center of gravity from the sum you
     * must call completeComputation(). Use with care. Calls addObjectPoint(int x, int y).
     */
    bool addObjectPoint(point p);    

    /**
     * Completes the computation of the geometric features, i.e. sum attributes
     * contain intermediate results during the computation process, which are
     * replaced by the proper attributes value on call of this method. Use with care.
     */
    bool completeComputation();

    /**
     * Recomputes the geometric feature vector by adding a second geometric feature
     * vector.
     * Here both feature vectors are considered complete, i.e. both center of gravities
     * are weighted according to their area.
     */
    bool operator+=(const geometricFeatureGroup0& other);
    //@}

    /**
     * @name Comparison
     */
    //@{

    /**
     * test for equality
     */
    bool operator==(const geometricFeatureGroup0& other) const;

    /**
     * test for inequality
     */
    bool operator!=(const geometricFeatureGroup0& other) const;

    /**
     * Check whether this feature group is close to the given feature group, i.e.
     * if the two circumscribing rectangles overlap if one is extended by the given
     * distance.
     * @param other the geometric features to be tested with
     * @param dist  the allowed distance between close rectangles
     * @return true if the two feature groups are close
     */
    bool isClose(const geometricFeatureGroup0& other, const point& dist) const;

    //@}

    /**
     * @name Input and Output
     */
    //@{
    /**
     * read the geometricFeatureGroup from the given ioHandler.
     * The complete flag indicates
     * if the enclosing begin and end should be also be readed
     */
    bool  read(ioHandler& handler,
	       const bool complete = true);
    
    /**
     * write the geometricFeatureGroup0 in the given ioHandler.
     * The complete flag indicates
     * if the enclosing begin and end should be also be written or not
     */
    bool write(ioHandler& handler,
	       const bool complete = true);    
    //@}

    /**
     * the point on the object's contour with the smallest x-coordinate
     */
    point minX;

    /**
     * the point on the object's contour with the biggest x-coordinate
     */
    point maxX;   

    /**
     * the point on the object's contour with the smallest y-coordinate
     */
    point minY;

    /**
     * the point on the object's contour with the biggest y-coordinate
     */
    point maxY;     

    /**
     * number of pixels covered by the object
     */
    int area;

    /**
     * center of gravity
     */
    tpoint<float> cog;

  };

}

namespace std {

  inline ostream& operator<<(ostream& s, const lti::geometricFeatureGroup0& p) {
    s << "(" <<  p.minX << ", " <<  p.maxX
      << ", " <<  p.minY << ", " <<  p.maxY
      << p.area << ", " << p.cog << ")";
    return s;
  };

  inline istream& operator>>(istream& s,lti::geometricFeatureGroup0& p) {
    char c;
    s >> c
      >> p.minX >> c
      >> p.maxX >> c
      >> p.minY >> c
      >> p.maxY >> c
      >> p.area >> c
      >> p.cog >> c;

    return s;
  };
}

#endif

