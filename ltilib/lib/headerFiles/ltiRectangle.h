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
 * file .......: ltiRectangle.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.11.2002
 * revisions ..: $Id: ltiRectangle.h,v 1.9 2004/11/30 13:07:44 winkler Exp $
 */

#ifndef LTI_RECTANGLE_H
#define LTI_RECTANGLE_H

#include <iostream>
#include "ltiIoHandler.h"
#include "ltiConfig.h"
#include "ltiPoint.h"
#include "ltiMath.h"

/**
 * @file ltiRectangle.h
 *
 * This file contains the definitions of the rectangle and trectangle types
 * used in the LTI-Lib to describe rectangular areas through two points.
 */

namespace lti {

  /**
   * Policy class to compute the size of a rectangle's side and the
   * inverse transformation.
   * 
   * For integer types it assumes the rectangle lies on a discrete grid with
   * grid elements of size 1x1, which means that the size of a side defined by
   * two scalar values x1 and x2 (x2>x1) is x2-x1+1.
   *
   * For floating point values the size is just x2-x1, because of the assumtion
   * that the rectangle lies on a real valued 2D space.
   */
  template <class T>
  class rectSide {
  public:
    /**
     * return the size of a rectangle's side which is aligned with one
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
  class rectSide<float> {
  public:
    static inline float size(const float x1,const float x2) {
      return (x2-x1);
    }
    static inline void invSize(const float sz,float& x1,float& x2) {
      x1=-sz/2.0f;
      x2=x1+sz;
    }
  };

  template <>
  class rectSide<double> {
  public:
    static inline double size(const double& x1,const double& x2) {
      return (x2-x1);
    }
    static inline void invSize(const double sz,double& x1,double& x2) {
      x1=-sz/2.0;
      x2=x1+sz;
    }
  };

  /**
   * Rectangle representation class.
   *
   * A rectangle is described by a pair of points: the upper-left and
   * bottom-right corners.
   *
   * A consistent rectangle will have the x and y coordinates of its upper-
   * left corner smaller than the ones of its bottom-right corner (note
   * the use of a left coordinate system, as usual with imaging systems).
   *
   * You can always check if a rectangle is consistent with the method 
   * \c isConsistent() or even force its consistency with
   * \c ensureConsistency().
   *
   * This is a template class, where the first template parameter \c T 
   * denotes the type used for the two points, which will be of type
   * <code>tpoint<T></code>.  For example, the type trectangle<int> (which
   * has the alias rectangle) contains to points (tpoint<int>) as corner 
   * representation.
   * The second template parameter is optional and allows the trectangle 
   * to behave correctly in a discrete integer grid or in a continuous real
   * grid (please see the lti::rectSide policy for more information).
   *
   * @ingroup gGeomData
   */
  template <class T,class S=rectSide<T> >
  class trectangle {
  public:
    /**
     * default constructor
     * The two points \a ul and \a br will be initialized with (0,0)
     */
    trectangle()
      : ul(tpoint<T>(0,0)),br(tpoint<T>(0,0)) {
    };

    /**
     * constructor
     */
    trectangle(const tpoint<T>& p1,const tpoint<T>& p2)
      : ul(p1), br(p2) {
    };

    /**
     * constructor
     */
    trectangle(const T& x1,const T& y1,
               const T& x2,const T& y2)
      : ul(tpoint<T>(x1,y1)),br(tpoint<T>(x2,y2)) {
    };

    /**
     * construct a square with the given side size.
     *
     * If the size is even and T is an integer type, the rectangle
     * will be created from -|_size/2_| to size + |_size/2_| in both
     * coordinates x and y.
     */
    trectangle(const T& size) {
      T low,high;
      S::invSize(size,low,high);
      ul.set(low,low);
      br.set(high,high);
    };

    /**
     * copy constructor
     */
    trectangle(const trectangle<T>& other)
      : ul(other.ul),br(other.br) {
    };

    /**
     * @name Location and Dimensions
     */
    //@{

    /**
     * resize trectangle<T> with new dimensions, but keep its center
     */
    void resize(const tpoint<T>& dim) {
      tpoint<T> cen = getCenter();
      S::invSize(dim.x,ul.x,br.x);
      S::invSize(dim.y,ul.y,br.y);
      ul.add(cen);
      br.add(cen);
    };

    /**
     * resize trectangle<T> with new dimensions, but keep its center
     */
    void resize(const T& x, const T& y) {
      tpoint<T> cen = getCenter();
      S::invSize(x,ul.x,br.x);
      S::invSize(y,ul.y,br.y);
      ul.add(cen);
      br.add(cen);
    };

    /**
     * Get dimensions of trectangle<T>.  If one coordinate of the two
     * points coincide and the rectangle contains fixed point typed
     * points, the returned dimension is one.  For floating point
     * types this case will return 0.
     *
     * \warning Note that the result of this method depends on the type T
     *
     * For integer types it will be assumed that the rectangle lies on
     * a discrete grid with grid elements of size 1x1, and that is not
     * the rectangle itself what is described but the number of grid
     * elements.  This means that the size of a side defined by two
     * scalar values x1 and x2 (x2>x1) is x2-x1+1.  This is useful for 
     * example when dealing with rectangular regions of images.
     *
     * For floating point values the size is just x2-x1, because of
     * the assumtion that the rectangle lies on a real valued 2D space.
     */
    tpoint<T> getDimensions() const {
      return tpoint<T>(S::size(ul.x,br.x),
                       S::size(ul.y,br.y));
    };

    /**
     * Get the area of this rectangle. This value is always
     * positive, regardless of the position of the upper left and
     * bottom right corner. A rectangle whose corners coincide
     * has an area of 1 for fixed point types or 0 for floating point ones.
     *
     * \warning Note that the result of this method depends on the type T
     *
     * For integer types it will be assumed that the rectangle lies on
     * a discrete grid with grid elements of size 1x1, and that is not
     * the rectangle itself what is described but the index of grid
     * elements.  This means that the size of a side defined by two
     * scalar values x1 and x2 (x2>x1) is x2-x1+1.  This is useful for 
     * example when dealing with rectangular regions of images.
     *
     * For floating point values the size is just x2-x1, because of
     * the assumtion that the rectangle lies on a real valued 2D space.
     */
    T getArea() const {
      return abs(S::size(ul.x,br.x)*
                 S::size(ul.y,br.y));
    };

    /**
     * Get center of trectangle<T>
     */
    tpoint<T> getCenter() const {
      return tpoint<T>((br.x+ul.x)/2,(br.y+ul.y)/2);
    };

    /**
     * Set the center of this rectangle.
     */
    void setCenter(tpoint<T> center) {
      tpoint<T> dim(abs(br.x-ul.x),abs(br.y-ul.y));
      ul.set(center.x - dim.x/2,center.y - dim.y/2);
      br.set(ul.x + dim.x,ul.y + dim.y);
    }

    /**
     * Ensure consistency of upper-left and bottom-right corners.
     * A trectangle<T> is called "consistent" if the upper-left
     * tpoint<T> has lower coordinate-values than the bottom-right
     * tpoint<T>.
     */
    void ensureConsistency() {
      trectangle<T> t(*this);

      if (t.ul.x > t.br.x) {
        ul.x = t.br.x;
        br.x = t.ul.x;
      }

      if (t.ul.y > t.br.y) {
        ul.y = t.br.y;
        br.y = t.ul.y;
      }
    };

    /**
     * Check for consistent trectangle<T>.
     * @return True if upper-left corner and bottom-right are correct
     */
    bool isConsistent() const {
      return !((ul.x > br.x) || (ul.y > br.y));
    };

    /**
     * Shift trectangle<T> by delta
     */
    void shift(const tpoint<T>& delta) {
      ul.add(delta);
      br.add(delta);
    };

    //@}

    /**
     * @name Simple operations
     */
    //@{

    /**
     * Check if tpoint<T> p is inside the trectangle<T>. (border included!)
     */
    bool isInside(const tpoint<T>& p) const {
      return ((p.x>=ul.x) && (p.x<=br.x) &&
              (p.y>=ul.y) && (p.y<=br.y));
    };

    /**
     * Check if the given coordinates are inside the trectangle<T>. 
     * (border included!)
     */
    bool isInside(const T& x, const T& y) const {
      return ((x>=ul.x) && (x<=br.x) &&
              (y>=ul.y) && (y<=br.y));
    };

    //@}


    /**
     * @name Duplication
     */
    //@{

    /**
     * copy member
     */
    trectangle<T>& copy(const trectangle<T>& other) {
      ul.copy(other.ul);
      br.copy(other.br);
      return (*this);
    };

    /**
     * cast from a rectangle of another type
     */
    template <class U,class SU>
    trectangle<T>& castFrom(const trectangle<U,SU>& other) {
      ul.x = static_cast<T>(other.ul.x);
      ul.y = static_cast<T>(other.ul.y);
      br.x = static_cast<T>(other.br.x);
      br.y = static_cast<T>(other.br.y);

      return (*this);
    };

    /**
     * alias for copy
     */
    trectangle<T>& operator=(const trectangle<T>& other) {
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
    bool operator==(const trectangle<T>& other) const {
      return ((other.ul == ul) &&
              (other.br == br));
    };

    /**
     * test for inequality
     */
    bool operator!=(const trectangle<T>& other) const {
      return ((other.ul != ul) ||
              (other.br != br));
    };

    //@}

    /**
     * @name Geometrical combination
     */
    //@{

    /**
     * Intersection of this trectangle with tRect
     * \deprecated Please use intersect instead
     */
    trectangle<T>& intersectRect(const trectangle<T>& tRect) {
      return intersect(tRect);
    };

    /**
     * Intersection of this trectangle with tRect
     *
     * (the largest rectangle which is contained both inside this
     * rectangle and inside tRect).
     *
     * The result will be left in this instance. If the rectangles do
     * not intersect, the resul will be inconsistent, so you should
     * call isConsistent() to check for this case unless you know for
     * sure that it cannot occur.
     *
     * @return a reference to this (modified) instance.
     */
    trectangle<T>& intersect(const trectangle<T>& tRect) {
      if (tRect.ul.x > ul.x) {
        ul.x = tRect.ul.x;
      }
      if (tRect.ul.y > ul.y) {
        ul.y = tRect.ul.y;
      }
      if (tRect.br.x < br.x) {
        br.x = tRect.br.x;
      }
      if (tRect.br.y < br.y) {
        br.y = tRect.br.y;
      }
      return (*this);
    };


    /**
     * Check whether this rectangle overlaps with the given rectangle.
     */
    bool overlaps(const trectangle<T>& tRect) const {
      trectangle<T> rect(*this);
      rect.intersect(tRect);
      return rect.isConsistent();
    };

    /**
     * Check whether this rectangle is close to the given rectangle, i.e.
     * if the two rectangles overlap if one is extended by the given distance.
     */
    bool isClose(const trectangle<T>& tRect, const tpoint<T>& dist) const {
      const tpoint<T> br1 ( tRect.br + dist );
      const tpoint<T> ul1 ( tRect.ul - dist );
      
      if ( br.x >= ul1.x ) {
	if ( br.x <= br1.x ) {
	  if ( br.y >= ul1.y ) {
	    if ( br.y <= br1.y ) {
	      return true;
	      
	    } else if ( ul.y <= br1.y ) {
	      return true;
	      
	    } else {
	      return false;
	    }
	    
	  } else {
	    return false;
	  }
	  
	} else if ( ul.x <= br1.x ) {
	  if ( ul.y >= ul1.y ) {
	    if ( ul.y <= br1.y ) {
	      return true;
	      
	    } else {
	      return false;
	    }
	    
	  } else if ( br.y >= ul1.y ) {
	    if ( ul.y <= br1.y ) {
	      return true;
	      
	    } else {
	      return false;
	    }
	    
	  } else {
	    return false;
	  }
	}
      }
      
      return false;
    };


    /**
     * trectangle = intersection of trectangle with tRect.
     * (alias for intersect)
     */
    trectangle<T>& operator&=(const trectangle<T>& tRect) {
      return intersect(tRect);
    };

    /**
     * new trectangle = intersection of trectangle with tRect
     */
    trectangle<T> operator&(const trectangle<T>& tRect) {
      trectangle<T> tempRect(*this);
      return tempRect.intersect(tRect);
    };
    
    /**
     * Union of this trectangle with tRect
     * \deprecated please use join instead.
     */
    trectangle<T>& unionRect(const trectangle<T>& tRect) {
      return join(tRect);
    }

    /**
     * Union of this trectangle with tRect
     * (the smallest rectangle containing both this trectangle and tRect)
     * The result will be left in this instance.
     * @return a reference to this (modified) instance.
     */
    trectangle<T>& join(const trectangle<T>& tRect) {
      if (tRect.ul.x < ul.x) {
        ul.x = tRect.ul.x;
      }
      if (tRect.ul.y < ul.y) {
        ul.y = tRect.ul.y;
      }
      if (tRect.br.x > br.x) {
        br.x = tRect.br.x;
      }
      if (tRect.br.y > br.y) {
        br.y = tRect.br.y;
      }
      return (*this);
    };

    /**
     * trectangle = union of this trectangle with tRect
     * (alias for join)
     */
    trectangle<T>& operator|=(const trectangle<T>& tRect) {
      return join(tRect);
    };

    /**
     * new trectangle = union of this trectangle  with tRect
     */
    trectangle<T> operator|(const trectangle<T>& tRect) {
      trectangle<T> tempRect(*this);
      return tempRect.join(tRect);
    };

    //@}


    /**
     * upper-left tpoint<T>
     */
    tpoint<T> ul;

    /**
     * bottom-right tpoint<T>
     */
    tpoint<T> br;

  };

  /**
   * A rectangle with integer coordinates
   *
   * \deprecated  Please use irectangle instead.
   */
  typedef trectangle<int> rectangle;

  /**
   * A rectangle with integer coordinates
   *
   * \deprecated  Please use irectangle instead.
   */
  typedef trectangle<int> irectangle;

  /**
   * A rectangle with float coordinates
   *
   * \deprecated  Please use irectangle instead.
   */
  typedef trectangle<float> frectangle;

  /**
   * A rectangle with double coordinates
   *
   * \deprecated  Please use irectangle instead.
   */
  typedef trectangle<double> drectangle;

  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template<class T,class S>
  bool read(ioHandler& handler,trectangle<T,S>& p,const bool complete=true) {
    bool b;

    if (complete) {
      handler.readBegin();
    }

    read(handler,p.ul);
    handler.readDataSeparator();
    b = read(handler,p.br);

    if (complete) {
      b = handler.readEnd();
    }

    return b;
  };

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template <class T,class S>
  bool write(ioHandler& handler,const trectangle<T,S>& p,
             const bool complete=true) {
    bool b;

    if (complete) {
      handler.writeBegin();
    }
    write(handler,p.ul);
    handler.writeDataSeparator();
    b = write(handler,p.br);

    if (complete) {
      b = handler.writeEnd();
    }

    return b;
  };
}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point& p);
  template <class T,class S>
  inline ostream& operator<<(ostream& s,const lti::trectangle<T,S>& p) {
    s << "(" << p.ul << ","
      << p.br << ")";
    return s;
  };

  //inline ostream& operator>>(istream& s,const lti::point& p);
  template <class T,class S>
  inline istream& operator>>(istream& s,lti::trectangle<T,S>& p) {
    char c;
    lti::tpoint<T> ul,br;
    s >> c
      >> ul >> c
      >> br >> c;
    p.ul = ul;
    p.br = br;

    return s;
  };
}

#endif

