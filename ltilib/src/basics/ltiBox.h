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
 * file .......: ltiBox.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.11.2002
 * revisions ..: $Id: ltiBox.h,v 1.1 2003/03/31 06:55:22 alvarado Exp $
 */

#ifndef LTI_BOX_H
#define LTI_BOX_H

#include <iostream>
#include "ltiIoHandler.h"
#include "ltiConfig.h"
#include "ltiPoint.h"
#include "ltiMath.h"

/**
 * @file ltiBox.h
 *
 * This file contains the definitions of the box and tbox types
 * used in the LTI-Lib to describe rectangular areas through two points.
 */

namespace lti {

  /**
   * Policy class to compute the size of a box's side and the
   * inverse transformation.
   * 
   * For integer types it assumes the box lies on a discrete grid with
   * grid elements of size 1x1x1, which means that the size of a side
   * defined by two scalar values x1 and x2 (x2>x1) is x2-x1+1.
   *
   * For floating point values the size is just x2-x1, because of the assumtion
   * that the box lies on a real valued 2D space.
   */
  template <class T>
  class boxSide {
  public:
    /**
     * return the size of a box's side which is aligned with one
     * of the two coordinates, assuming that the delimiting coordinates are
     * x1 and x2.
     *
     * For example:
     * - if T is int, x1=0 and x2=2, then size returns 3
     * - if T is float, x1=0 and x2=2 then size returns 2
     */
    static inline T size(const T x1,const T x2) {
      return (x2-x1+static_cast<T>(1));
    }

    /**
     * For a given side size, compute two coordinates which generates it.
     *
     * For example:
     * - if T is int and sz is 3, the returned values would be
     *   x1=-1 and x2=1.
     * - if T is float and sz = 3, the returned values would be
     *   x1=-1.5 and x2=1.5
     */
    static inline void invSize(const T sz,T& x1,T& x2) {
      x1=(static_cast<T>(1)-sz)/2;
      x2=x1+sz-1;
    }
  };
  
  // specialization for float and double
  template <>
  class boxSide<float> {
  public:
    static inline float size(const float x1,const float x2) {
      return (x2-x1);
    }
    static inline void invSize(const float sz,float& x1,float& x2) {
      x1=sz/2.0f;
      x2=x1+sz;
    }
  };

  template <>
  class boxSide<double> {
  public:
    static inline double size(const double& x1,const double& x2) {
      return (x2-x1);
    }
    static inline void invSize(const double sz,double& x1,double& x2) {
      x1=sz/2.0;
      x2=x1+sz;
    }
  };

  /**
   * Class to describe a 3D box aligned with the coordinate system.  It is
   * specified through two points with the minimum and maximum coordinates.
   *
   * @ingroup gGeomData
   */
  template <class T,class S=boxSide<T> >
  class tbox {
  public:
    /**
     * default constructor
     */
    tbox()
      : min(tpoint3D<T>(0,0,0)),max(tpoint3D<T>(0,0,0)) {
    };

    /**
     * constructor
     */
    tbox(const tpoint3D<T>& p1,const tpoint3D<T>& p2)
      : min(p1), max(p2) {
    };

    /**
     * constructor
     */
    tbox(const T& x1,const T& y1,const T& z1,
         const T& x2,const T& y2,const T& z2)
      : min(tpoint3D<T>(x1,y1,z1)),max(tpoint3D<T>(x2,y2,z2)) {
    };

    /**
     * construct a cube with the given side size.
     *
     * If the size is even and T is an integer type, the box
     * will be created from -|_size/2_| to size + |_size/2_| in both
     * coordinates x and y.
     */
    tbox(const T& size) {
      T low,high;
      S::invSize(size,low,high);
      min.set(low,low,low);
      max.set(high,high,high);
    };

    /**
     * copy constructor
     */
    tbox(const tbox<T>& other)
      : min(other.min),max(other.max) {
    };

    /**
     * @name Dimensions and location
     */
    //@{

    /**
     * resize tbox<T> with new dimensions, but keep its center
     */
    void resize(const tpoint3D<T>& dim) {
      tpoint3D<T> cen = getCenter();
      S::invSize(dim.x,min.x,max.x);
      S::invSize(dim.y,min.y,max.y);
      S::invSize(dim.z,min.z,max.z);
      min.add(cen);
      max.add(cen);
    };

    /**
     * resize tbox<T> with new dimensions, but keep its center
     */
    void resize(const T& x, const T& y, const T& z) {
      tpoint3D<T> cen = getCenter();

      S::invSize(x,min.x,max.x);
      S::invSize(y,min.y,max.y);
      S::invSize(z,min.z,max.z);

      min.add(cen);
      max.add(cen);
    };

    /**
     * Get dimensions of tbox<T>.  If one coordinate of the two
     * points coincide and the box contains fixed point typed
     * points, the returned dimension is one.  For floating point
     * types this case will return 0.
     *
     * \warning Note that the result of this method depends on the type T
     *
     * For integer types it will be assumed that the box lies on
     * a discrete grid with grid elements of size 1x1, and that is not
     * the box itself what is described but the number of grid
     * elements.  This means that the size of a side defined by two
     * scalar values x1 and x2 (x2>x1) is x2-x1+1.  This is useful for 
     * example when dealing with rectangular regions of images.
     *
     * For floating point values the size is just x2-x1, because of
     * the assumtion that the box lies on a real valued 2D space.
     */
    tpoint3D<T> getDimensions() const {
      return tpoint3D<T>(S::size(min.x,max.x),
                         S::size(min.y,max.y),
                         S::size(min.z,max.z));
    };

    /**
     * Get the volume of this box. This value is always positive,
     * regardless of the position of the minimum and maximum
     * corners. A box whose corners coincide has an area of 1 for
     * fixed point types or 0 for floating point ones.
     *
     * \warning Note that the result of this method depends on the type T
     *
     * For integer types it will be assumed that the box lies on
     * a discrete grid with grid elements of size 1x1, and that is not
     * the box itself what is described but the index of grid
     * elements.  This means that the size of a side defined by two
     * scalar values x1 and x2 (x2>x1) is x2-x1+1.  This is useful for 
     * example when dealing with rectangular regions of images.
     *
     * For floating point values the size is just x2-x1, because of
     * the assumtion that the box lies on a real valued 2D space.
     */
    T getVolume() const {
      return abs(S::size(min.x,max.x)*
                 S::size(min.y,max.y)*
                 S::size(min.z,max.z));
    };

    /**
     * Get center of tbox<T>
     */
    tpoint3D<T> getCenter() const {
      return tpoint3D<T>((max.x+min.x)/2,(max.y+min.y)/2,(max.z+min.z)/2);
    };

    /**
     * Set the center of this box.
     */
    void setCenter(tpoint3D<T> center) {
      tpoint3D<T> dim(abs(max.x-min.x),abs(max.y-min.y),abs(max.z-min.z));
      min.set(center.x - dim.x/2,center.y - dim.y/2,center.z - dim.z/2);
      max.set(min.x + dim.x, min.y + dim.y, min.z + dim.z);
    }

    /**
     * Ensure consistency of minimum and maximum corners.  A tbox<T>
     * is called "consistent" if the minimum tpoint3D<T> has lower
     * coordinate-values than the maximum tpoint3D<T>.
     */
    void ensureConsistency() {
      tbox<T> t(*this);

      if (t.min.x > t.max.x) {
        min.x = t.max.x;
        max.x = t.min.x;
      }

      if (t.min.y > t.max.y) {
        min.y = t.max.y;
        max.y = t.min.y;
      }

      if (t.min.z > t.max.z) {
        min.z = t.max.z;
        max.z = t.min.z;
      }
    };

    /**
     * Check for consistent tbox<T>.
     * @return True if minimum and maximum corners are correct
     */
    bool isConsistent() const {
      return !( (min.x > max.x) || (min.y > max.y) || (min.z > max.z) );
    };

    /**
     * Shift tbox<T> by delta
     */
    void shift(const tpoint3D<T>& delta) {
      min.add(delta);
      max.add(delta);
    };
    //@}

    /**
     * @name Simple operations
     */
    //@{

    /**
     * Check if tpoint3D<T> p is inside the tbox<T>. (border included!)
     */
    bool isInside(const tpoint3D<T>& p) const {
      return ( (p.x>=min.x) && (p.x<=max.x) &&
               (p.y>=min.y) && (p.y<=max.y) &&
               (p.z>=min.z) && (p.z<=max.z) );
    };

    /**
     * Check if the given coordinates are inside the tbox<T>. 
     * (border included!)
     */
    bool isInside(const T& x, const T& y, const T& z) const {
      return ( (x>=min.x) && (x<=max.x) &&
               (y>=min.y) && (y<=max.y) &&
               (z>=min.z) && (z<=max.z) );
    };

    //@}


    /**
     * @name Duplication
     */
    //@{

    /**
     * copy member
     */
    tbox<T>& copy(const tbox<T>& other) {
      min.copy(other.min);
      max.copy(other.max);
      return (*this);
    };

    /**
     * cast from a box of another type
     */
    template <class U,class SU>
    tbox<T>& castFrom(const tbox<U,SU>& other) {

      min.castFrom(other.min);
      max.castFrom(other.max);

      return (*this);
    };

    /**
     * alias for copy
     */
    tbox<T>& operator=(const tbox<T>& other) {
      return (copy(other));
    };

    //@}

    /**
     * @name Comparison
     */
    //@{

    /**
     * test for equality
     */
    bool operator==(const tbox<T>& other) const {
      return ( (other.min == min) &&
               (other.max == max) );
    };

    /**
     * test for inequality
     */
    bool operator!=(const tbox<T>& other) const {
      return ( (other.min != min) ||
               (other.max != max) );
    };

    //@}

    /**
     * @name Geometrical combination
     */
    //@{

    /**
     * Intersection of this tbox with tRect
     *
     * (the largest box which is contained both inside this
     * box and inside tRect).
     *
     * The result will be left in this instance. If the boxs do
     * not intersect, the resul will be inconsistent, so you should
     * call isConsistent() to check for this case unless you know for
     * sure that it cannot occur.
     *
     * @return a reference to this (modified) instance.
     */
    tbox<T>& intersect(const tbox<T>& tRect) {
      if (tRect.min.x > min.x) {
        min.x = tRect.min.x;
      }
      if (tRect.min.y > min.y) {
        min.y = tRect.min.y;
      }
      if (tRect.min.z > min.z) {
        min.z = tRect.min.z;
      }

      if (tRect.max.x < max.x) {
        max.x = tRect.max.x;
      }
      if (tRect.max.y < max.y) {
        max.y = tRect.max.y;
      }
      if (tRect.max.z < max.z) {
        max.z = tRect.max.z;
      }
      return (*this);
    };


    /**
     * Check whether this box overlaps with the given box.
     */
    bool overlaps(const tbox<T>& tRect) const {
      tbox<T> rect(*this);
      rect.intersect(tRect);
      return rect.isConsistent();
    };

    /**
     * tbox = intersection of tbox with tRect.
     * (alias for intersect)
     */
    tbox<T>& operator&=(const tbox<T>& tRect) {
      return intersect(tRect);
    };

    /**
     * new tbox = intersection of tbox with tRect
     */
    tbox<T> operator&(const tbox<T>& tRect) {
      tbox<T> tempRect(*this);
      return tempRect.intersect(tRect);
    };
    
    /**
     * Union of this tbox with tRect
     * (the smallest box containing both this tbox and tRect)
     * The result will be left in this instance.
     * @return a reference to this (modified) instance.
     */
    tbox<T>& join(const tbox<T>& tRect) {
      if (tRect.min.x < min.x) {
        min.x = tRect.min.x;
      }
      if (tRect.min.y < min.y) {
        min.y = tRect.min.y;
      }
      if (tRect.min.z < min.z) {
        min.z = tRect.min.z;
      }

      if (tRect.max.x > max.x) {
        max.x = tRect.max.x;
      }
      if (tRect.max.y > max.y) {
        max.y = tRect.max.y;
      }
      if (tRect.max.z > max.z) {
        max.z = tRect.max.z;
      }
      return (*this);
    };

    /**
     * tbox = union of this tbox with tRect
     * (alias for join)
     */
    tbox<T>& operator|=(const tbox<T>& tRect) {
      return join(tRect);
    };

    /**
     * new tbox = union of this tbox  with tRect
     */
    tbox<T> operator|(const tbox<T>& tRect) {
      tbox<T> tempRect(*this);
      return tempRect.join(tRect);
    };

    //@}

    /**
     * minimum tpoint3D<T>
     */
    tpoint3D<T> min;

    /**
     * maximum tpoint3D<T>
     */
    tpoint3D<T> max;

  };

  /**
   * A box with integer coordinates
   */
  typedef tbox<int> box;

  /**
   * read the box from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template<class T,class S>
  bool read(ioHandler& handler,tbox<T,S>& p,const bool complete=true) {
    bool b;

    if (complete) {
      handler.readBegin();
    }

    read(handler,p.min);
    handler.readDataSeparator();
    b = read(handler,p.max);

    if (complete) {
      b = handler.readEnd();
    }

    return b;
  };

  /**
   * write the box in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template <class T,class S>
  bool write(ioHandler& handler,const tbox<T,S>& p,
             const bool complete=true) {
    bool b;

    if (complete) {
      handler.writeBegin();
    }
    write(handler,p.min);
    handler.writeDataSeparator();
    b = write(handler,p.max);

    if (complete) {
      b = handler.writeEnd();
    }

    return b;
  };
}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point& p);
  template <class T,class S>
  inline ostream& operator<<(ostream& s,const lti::tbox<T,S>& p) {
    s << "(" << p.min << ","
      << p.max << ")";
    return s;
  };

  //inline ostream& operator>>(istream& s,const lti::point& p);
  template <class T,class S>
  inline istream& operator>>(istream& s,lti::tbox<T,S>& p) {
    char c;
    lti::tpoint3D<T> min,max;
    s >> c
      >> min >> c
      >> max >> c;
    p.min = min;
    p.max = max;

    return s;
  };
}

#endif

