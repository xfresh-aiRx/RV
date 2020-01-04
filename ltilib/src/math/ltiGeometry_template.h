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
 * file .......: ltiGeometry_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.03.02
 * revisions ..: $Id: ltiGeometry_template.h,v 1.7 2005/10/27 09:15:20 doerfler Exp $
 */

namespace lti {

  /*
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
   */
  template<class T>
  inline bool intersection(const tpoint<T>& p1,const tpoint<T>& p2,
                           const tpoint<T>& p3,const tpoint<T>& p4,
                           tpoint<T>& p) {
    tpoint<T> d1,d2;
    bool result = false;

    d1.subtract(p2,p1);
    d2.subtract(p4,p3);

    const T det = d1.y*d2.x - d1.x*d2.y;

    if (det!=static_cast<T>(0)) {
      // lines are not parallel (they still can intersect...)
      
      tpoint<T> d0;
      d0.subtract(p3,p1);

      const T t1 = d2.x*d0.y - d2.y*d0.x;
      const T t2 = d1.x*d0.y - d1.y*d0.x;

      if (det >= 0) {
        if (((t1>=0) && (det >= t1)) &&
            ((t2>=0) && (det >= t2))) {
          // compute where is the intersection point

          p.x = p1.x + (t1*d1.x)/det;
          p.y = p1.y + (t1*d1.y)/det;

          result = true;
        }
      } else {
        if (((t1<=0) && (det <= t1)) &&
            ((t2<=0) && (det <= t2))) {

          p.x = p1.x + (t1*d1.x)/det;
          p.y = p1.y + (t1*d1.y)/det;

          result = true;
        }
      }
    }

    return result;
  }

  template<class T>
  T minDistanceSqr(const tpoint<T>& p1,
                 const tpoint<T>& p2,
                 const tpoint<T>& p3,
                 tpoint<T>& p) {

    tpoint<T> d1,d0;

    d1.subtract(p2,p1);
    d0.subtract(p3,p1);

    const T d10 = d1.dot(d0);

    if (d10 < 0) {
      p.copy(p1);
      return p.distanceSqr(p3);
    }

    const T d11 = d1.dot(d1);
    if (d10 > d11) {
      p.copy(p2);
      return p.distanceSqr(p3);
    } else {
      if (d11 > 0) {
        p.x = p1.x+d10*d1.x/d11;
        p.y = p1.y+d10*d1.y/d11;
      } else {
        p.copy(p3);
        return T(0);
      }

      return p.distanceSqr(p3);
    }

    return T();
  }

  template<class T>
  T minDistanceSqr(const tpoint<T>& p1,
                 const tpoint<T>& p2,
                 const tpoint<T>& p3,
                 const tpoint<T>& p4,
                 tpoint<T>& pa,
                 tpoint<T>& pb) {
    tpoint<T> t0,t1;
    int idx(0);
    T d[4];

    if (!intersection(p1,p2,p3,p4,pa)) {

      d[0] = minDistanceSqr(p3,p4,p1,pb);
      d[1] = minDistanceSqr(p3,p4,p2,t1);
      d[2] = minDistanceSqr(p1,p2,p3,pa);
      d[3] = minDistanceSqr(p1,p2,p4,t0);

      for (int i=1;i<4;++i) {
        if (d[i]<d[idx]) {
          idx = i;
        }
      }

      switch (idx) {
        case 0:
          pa = p1;
          break;
        case 1:
          pa = p2;
          pb = t1;
          break;
        case 2:
          pb = p3;
          break;
        case 3:
          pa = t0;
          pb = p4;
          break;
        default:
          break;
      };
    } else {
      pb.copy(pa);
      return T(0);
    }

    return d[idx];
  }

  /*
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
   */
  template<class T>
  int clockwiseTurn(const tpoint<T>& p0,
                    const tpoint<T>& p1,
                    const tpoint<T>& p2) {

    const tpoint<T> d1(p1-p0);
    const tpoint<T> d2(p2-p1);

    const T tmp = d1.x*d2.y - d1.y*d2.x;
    if (tmp == T(0)) {
      if ((d1.x*d2.x < 0) || (d1.y*d2.y < 0)) {
        return -1;
      } else if (d1.absSqr() < d2.absSqr()) {
        return +1;
      }
    } else {
      return (tmp>0) ? 1 : -1;
    }

    return 0;
  }


}



