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
 * file .......: ltiGeometry.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.03.02
 * revisions ..: $Id: ltiGeometry.h,v 1.9 2003/05/05 06:47:39 alvarado Exp $
 */

#ifndef _LTI_GEOMETRY_H_
#define _LTI_GEOMETRY_H_

#include "ltiMath.h"
#include "ltiTypes.h"
#include "ltiPoint.h"

/**
 * \file ltiGeometry.h
 * Definition of some usually used global functions for geometric
 * condition like line intersections.
 */

namespace lti {

  /**
   * compute if the line between p1 and p2 intersects with the line
   * between p3 and p4.  If they intersect in exactly one point
   * (normal case) the function returns true.  If the lines are
   * parallel or any of the lines have length 0 this function returns
   * false.
   *
   * @param p1 begin of first line
   * @param p2 end of first line
   * @param p3 begin of first line
   * @param p4 end of first line
   * @param p intersection point will be written here, in case there is one.
   *          if there is no intersection point (or infinity) the value
   *          will not change.
   *
   * @return true if lines intersect in exactly one point, false otherwise
   *
   * @ingroup gGeometry
   *
   * Defined in ltiGeometry.h
   */
  template<class T>
  bool intersection(const tpoint<T>& p1,const tpoint<T>& p2,
                    const tpoint<T>& p3,const tpoint<T>& p4,
                    tpoint<T>& p);

  /**
   * compute if the line between p1 and p2 intersects with the line
   * between p3 and p4.  If they intersect in exactly one point
   * (normal case) the function returns true.  If the lines are
   * parallel or any of the lines have length 0 this function returns
   * false.
   *
   * @param p1 begin of first line
   * @param p2 end of first line
   * @param p3 begin of first line
   * @param p4 end of first line
   *
   * @return true if lines intersect in exactly one point, false otherwise
   *
   * @ingroup gGeometry
   *
   * Defined in ltiGeometry.h
   */
  template<class T>
  inline bool intersection(const tpoint<T>& p1,const tpoint<T>& p2,
                           const tpoint<T>& p3,const tpoint<T>& p4) {
    tpoint<T> p;
    return intersection(p1,p2,p3,p4,p);
  }

  /**
   * compute the square of the minimal distance between the line
   * segment defined by the points p1 and p2 and the point p3.  The
   * point within the line with the minimal distance will be stored in
   * p.
   *
   * @param p1 start point of the line segment
   * @param p2 end point of the line segment
   * @param p3 point, for which the distance to the line segment will be
   *           computed.
   * @param p  the point in the line between p1 and p2 with the shortest
   *           distance will be stored here.
   * @return the square of the distance between the line p1_p2 and the point
   *         p3
   *
   * @ingroup gGeometry
   *
   * Defined in ltiGeometry.h
   */
  template<class T>
  T minDistanceSqr(const tpoint<T>& p1,
                 const tpoint<T>& p2,
                 const tpoint<T>& p3,
                 tpoint<T>& p);

  /**
   * compute the square of the minimal distance between the line
   * segment defined by the points p1 and p2 and the point p3.  The
   * point within the line with the minimal distance will be stored in
   * p.
   *
   * @param p1 start point of the line segment
   * @param p2 end point of the line segment
   * @param p3 point, for which the distance to the line segment will be
   *           computed.
   * @return the square of the distance between the line p1_p2 and the point
   *         p3
   *
   * @ingroup gGeometry
   *
   * Defined in ltiGeometry.h
   */
  template<class T>
  inline T minDistanceSqr(const tpoint<T>& p1,
                          const tpoint<T>& p2,
                          const tpoint<T>& p3) {
    tpoint<T> tmp;
    return minDistanceSqr(p1,p2,p3,tmp);
  }

  /**
   * compute the square of the minimal distance between the line
   * segment defined by the points p1 and p2 and the line segment
   * defined by the points p3 and p4.  The corresponding points with the
   * minimal distance will be stored in pa (in p1-p2) and pb (in p3-p4).
   *
   * @param p1 start point of the first line segment
   * @param p2 end point of the first line segment
   * @param p3 start point of the second line segment
   * @param p4 end point of the second line segment
   * @param pa point in the line p1-p2 with the minimal distance to
   *           the line segment p3-p4.
   * @param pb point in the line p3-p4 with the minimal distance to
   *           the line segment p1-p2.
   * @return the square of the distance between the line p1-p2 and p3-p4
   *
   * @ingroup gGeometry
   *
   * Defined in ltiGeometry.h
   */
  template<class T>
  T minDistanceSqr(const tpoint<T>& p1,
                   const tpoint<T>& p2,
                   const tpoint<T>& p3,
                   const tpoint<T>& p4,
                   tpoint<T>& pa,
                   tpoint<T>& pb);

  /**
   * compute if the path that follows the points p0, p1 and p2
   * makes a clock-wise turn (+1) a counter-clock-wise turn (-1) or
   * stays in a straight line (0).
   *
   * @param p0 first point
   * @param p1 second point
   * @param p2 third point
   *
   * @return +1 for a clockwise turn, -1 for a counter clockwise turn, 0
   *         if path stays in a straight line.
   *
   * @ingroup gGeometry
   *
   * Defined in ltiGeometry.h
   */
  template<class T>
  int clockwiseTurn(const tpoint<T>& p0,
                    const tpoint<T>& p1,
                    const tpoint<T>& p2);


}
#endif

