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
 * file .......: ltiPoint.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.11.2002
 * revisions ..: $Id: ltiPoint.h,v 1.17 2004/11/17 09:35:23 doerfler Exp $
 */

#ifndef LTI_POINT_H
#define LTI_POINT_H

#include <iostream>
#include "ltiIoHandler.h"
#include "ltiAssert.h"
#include "ltiConfig.h"
#include "ltiMath.h" // for sqrt

namespace lti {

  /**
   * Two dimensional point, containing the coordinates x, y.
   *
   * The template type T will be the one used for each coordinate.  For
   * example tpoint<float> uses float for \a x and \a y.
   * 
   * This data structure simplifies the manipulation of 2D points providing
   * simple interfaces for adding, substracting, distance (L2), and more.
   *
   * @ingroup gGeomData
   */
  template <class T>
  class tpoint {
    public:
    /**
     * Used for the template-based interface for pixels as vectors.
     */
    typedef T value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * Coordinate x
     */
    T x;

    /**
     * Coordinate y;
     */
    T y;

    /**
     * Default constructor
     */
    explicit tpoint(const T newx=0,const T newy=0) : x(newx),y(newy) {};

    /**
     * Copy constructor
     */
    template <class U>
    tpoint(const tpoint<U>& p)
      : x(static_cast<T>(p.x)),y(static_cast<T>(p.y)) {};

    /**
     * Copy constructor
     */
    template <class U>
    tpoint<T>& castFrom(const tpoint<U>& p) {
      x = static_cast<T>(p.x);
      y = static_cast<T>(p.y);
      return (*this);
    };

    /**
     * Set the coordinate values and return a reference to this point
     */
    inline tpoint<T>& set(const T tx,const T ty) {
      x = tx;
      y = ty;
      return *this;
    }

    /**
     * Calculate distance to the point c
     */
    inline T distanceTo(const tpoint<T>& c) const;

    /**
     * Calculate %square of distance to the point c. This
     * method is faster than distanceTo (because it does not compute
     * the square root of a*a + b*b).
     */
    inline T distanceSqr(const tpoint<T>& c) const;

    /**
     * Return the square of the magnitude of the point
     */
    inline T absSqr() const;

    /**
     * Multiply tpoint<T> with a given factor
     */
    template<class U>
    inline tpoint<T>& multiply(const U c) {
      x = static_cast<T>(x*c);
      y = static_cast<T>(y*c);
      return *this;
    };

    /**
     * Multiply the other point tpoint<T> with a given factor
     */
    template<class U>
    inline tpoint<T>& multiply(const tpoint<T>& other,const U c) {
      x = static_cast<T>(other.x*c);
      y = static_cast<T>(other.y*c);
      return *this;
    };


    /**
     * Multiply tpoint<T> with a given factor
     */
    template<class U>
    inline tpoint<T> operator*(const U c) const {
      return tpoint<T>(static_cast<T>(x*c),static_cast<T>(y*c));
    };

    /**
     * Multiply tpoint<T> with a given factor
     */
    template<class U>
    inline tpoint<T>& operator*=(const U c) {
      x = static_cast<T>(x*c);
      y = static_cast<T>(y*c);
      return *this;
    };

    /**
     * Multiplies elementwise the components of this and the point c
     */
    inline tpoint<T> operator*(const tpoint<T>& c) const;

    /**
     * Multiplies elementwise the components of this and the point c
     */
    inline tpoint<T>& operator*=(const tpoint<T>& c);

    /**
     * Multiplies elementwise the components of \a a and \a b and leave the
     * result here.
     */
    inline tpoint<T>& emultiply(const tpoint<T>& a,const tpoint<T>& b);

    /**
     * Multiplies elementwise the components of this and the point c, and
     * leave the result here.
     */
    inline tpoint<T>& emultiply(const tpoint<T>& c);

    /**
     * Divide each component of tpoint<T> with a given factor
     */
    template<class U>
    inline tpoint<T>& divide(const U c) {
      x = static_cast<T>(x/c);
      y = static_cast<T>(y/c);
      return *this;
    };

    /**
     * Divide each component of other other tpoint<T> with a given factor
     */
    template<class U>
    inline tpoint<T>& divide(const tpoint<T>& other,const U c) {
      x = static_cast<T>(other.x/c);
      y = static_cast<T>(other.y/c);
      return *this;
    };

    /**
     * Divide each component of tpoint<T> by a given factor
     */
    template <class U>
    inline tpoint<T> operator/(const U c) const {
      return tpoint<T>(static_cast<T>(x/c),static_cast<T>(y/c));
    };

    /**
     * Divide each component of tpoint<T> by a given factor
     */
    template <class U>
    inline tpoint<T>& operator/=(const U c) {
      x = static_cast<T>(x/c);
      y = static_cast<T>(y/c);
      return *this;
    };

    /**
     * Elementwise division of each component of the points
     */
    inline tpoint<T> operator/(const tpoint<T>& c) const;

    /**
     * Elementwise division of each component of the points
     */
    inline tpoint<T>& operator/=(const tpoint<T>& c);

    /**
     * Elementwise division of each component of the points
     */
    inline tpoint<T>& edivide(const tpoint<T>& c);

    /**
     * Elementwise division of each component of the points
     */
    inline tpoint<T>& edivide(const tpoint<T>& a,const tpoint<T>& b);

    /**
     * Modulo c of the integer part of each component of the point
     */
    inline tpoint<T> operator%(const int c) const;

    /**
     * Add given point to this point and leave the result here.
     * @param p the other point to be added to this one
     * @return a reference to this point
     */
    inline tpoint<T>& add(const tpoint<T>& p);

    /**
     * Add the two other points and leave the result here
     * @param a first point to be added
     * @param b second point to be added
     * @return a reference to this point, which will contain a+b
     */
    inline tpoint<T>& add(const tpoint<T>& a,
                          const tpoint<T>& b);
    /**
     * Operator + is simmilar to add, but a new point is returned, i.e.
     * this point will not change.
     */
    inline tpoint<T> operator+(const tpoint<T>& p) const;

    /**
     * Operator += is an alias for add()
     */
    inline tpoint<T>& operator+=(const tpoint<T>& p);

    /**
     * Subtract
     */
    inline tpoint<T>& subtract(const tpoint<T>& p);


    /**
     * Subtract the two other points and leave the result here
     * @param a first point
     * @param b point to be subtracted from the first one
     * @return a reference to this point, which will contain a-b
     */
    inline tpoint<T>& subtract(const tpoint<T>& a,
                               const tpoint<T>& b);

    /**
     * Operator -
     */
    inline tpoint<T> operator-(const tpoint<T>& p) const;

    /**
     * Operator -=
     */
    inline tpoint<T>& operator-=(const tpoint<T>& p);

    /**
     * Dot product with another point.
     *
     * @return this->x*p.x + this->y*p.y.
     */
    inline T dot(const tpoint<T>& p) const;

    /**
     * Copy operator
     */
    inline tpoint<T>& copy(const tpoint<T>& p);

    /**
     * Operator =
     */
    inline tpoint<T>& operator=(const tpoint<T>& p) {return copy(p);};

    /**
     * Operator ==
     */
    inline bool operator==(const tpoint<T>& p) const;

    /**
     * Operator !=
     */
    inline bool operator!=(const tpoint<T>& p) const;

    /**
     * Operator <
     * a point is "smaller" than another one if its coordinates produce
     * an earlier display of the point in the monitor. i.e. if it
     * has a smaller y component or (if the y components are the same) if
     * the x component is smaller
     */
    inline bool operator<(const tpoint<T>& p) const;

    /**
     * Operator >
     * a point is "bigger" than another one if its coordinates produce
     * an later display of the point in the monitor. i.e. if it
     * has a bigger y component or (if the y components are the same) if
     * the x component is bigger
     */
    inline bool operator>(const tpoint<T>& p) const;

    /**
     * @name Access as vector
     */
    //@{
    /**
     * Used to simulate vector access.  It is slower than the normal
     * access to the elements x and y, but allow the use of point in templates
     * expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the color components will be [0] for x and [1] for y
     */
    inline T& operator[](const int i) {
      assert(i<2);
      return (i>0) ? y : x;
    }

    /**
     * Used to simulate read-only vector access.  It is slower than the normal
     * access to the elements x and y, but allow the use of point in templates
     * expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the color components will be [0] for x and [1] for y
     */
    inline const T& operator[](const int i) const {
      assert(i<2);
      return (i>0) ? y : x;
    }

    /**
     * Used to simulate the vector size
     */
    inline int size() const {
      return 2;
    }
    //@}
  };

  // implementation: not in Doc++!!!
  template <class T>
  inline T tpoint<T>::distanceTo(const tpoint<T>& c) const {
    tpoint<T> t((*this)-c);
    return sqrt(t.absSqr());
  }

  template <class T>
  inline T tpoint<T>::distanceSqr(const tpoint<T>& c) const {
    tpoint<T> t((*this)-c);
    return t.absSqr();
  }

  template <class T>
  inline tpoint<T> tpoint<T>::operator*(const tpoint<T>& c) const {
    return tpoint<T>(c.x*x,c.y*y);
  }


  template <class T>
  inline tpoint<T>& tpoint<T>::operator*=(const tpoint<T>& c) {
    x*=c.x;
    y*=c.y;
    return *this;
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::emultiply(const tpoint<T>& c) {
    x*=c.x;
    y*=c.y;
    return *this;
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::emultiply(const tpoint<T>& a,
                                         const tpoint<T>& b) {
    x=a.x*b.x;
    y=a.y*b.y;
    return *this;
  }


  template <class T>
  inline tpoint<T> tpoint<T>::operator/(const tpoint<T>& c) const {
    return tpoint<T>(x/c.x,y/c.y);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::operator/=(const tpoint<T>& c) {
    x/=c.x;
    y/=c.y;
    return *this;
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::edivide(const tpoint<T>& c) {
    x/=c.x;
    y/=c.y;
    return *this;
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::edivide(const tpoint<T>& a,
                                       const tpoint<T>& b) {
    x=a.x/b.x;
    y=a.y/b.y;
    return *this;
  }


  template <class T>
  inline tpoint<T> tpoint<T>::operator%(const int c) const {
    return tpoint<T>(static_cast<int>(x)%c,static_cast<int>(y)%c);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::add(const tpoint<T>& p) {
    x+=p.x;
    y+=p.y;
    return (*this);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::add(const tpoint<T>& a,
                                   const tpoint<T>& b) {
    x=a.x+b.x;
    y=a.y+b.y;

    return (*this);
  }


  template <class T>
  inline tpoint<T> tpoint<T>::operator+(const tpoint<T>& p) const {
    return tpoint<T>(x+p.x,y+p.y);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::operator+=(const tpoint<T>& p) {
    return add(p);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::subtract(const tpoint<T>& p) {
    x-=p.x;
    y-=p.y;
    return (*this);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::subtract(const tpoint<T>& a,
                                        const tpoint<T>& b) {
    x=a.x-b.x;
    y=a.y-b.y;

    return (*this);
  }

  template <class T>
  inline tpoint<T> tpoint<T>::operator-(const tpoint<T>& p) const {
    return tpoint<T>(x-p.x,y-p.y);
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::operator-=(const tpoint<T>& p) {
    return subtract(p);
  }

  template <class T>
  inline T tpoint<T>::dot(const tpoint<T>& p) const {
    return static_cast<T>((x*p.x) + (y*p.y));
  }

  template <class T>
  inline T tpoint<T>::absSqr() const {
    return static_cast<T>((x*x) + (y*y));
  }

  template <class T>
  inline tpoint<T>& tpoint<T>::copy(const tpoint<T>& p) {
    x = p.x;
    y = p.y;
    return (*this);
  }

  template <class T>
  inline bool tpoint<T>::operator==(const tpoint<T>& p) const {
    return ((p.y == y) && (p.x == x));
  }

  template <class T>
  inline bool tpoint<T>::operator!=(const tpoint<T>& p) const {
    return ((p.y != y) || (p.x != x));
  }

  template <class T>
  inline bool tpoint<T>::operator<(const tpoint<T>& p) const {
    if (y < p.y) {
      return true;
    } else if (y == p.y) {
      return (x < p.x);
    }
    return false;
  }

  template <class T>
  inline bool tpoint<T>::operator>(const tpoint<T>& p) const {
    if (y > p.y) {
      return true;
    } else if (y == p.y) {
      return (x > p.x);
    }
    return false;
  }

  /**
   * A point with integer coordinates.
   *
   * \deprecated Please use ipoint instead.
   */
  typedef tpoint<int> point;

  /**
   * A point with integer coordinates
   */
  typedef tpoint<int> ipoint;

  /**
   * A point with double coordinates
   */
  typedef tpoint<double> dpoint;

  /**
   * A point with float coordinates
   */
  typedef tpoint<float> fpoint;

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,tpoint<T>& p,const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && handler.read(p.x);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.y);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  };

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<class T>
  bool write(ioHandler& handler,const tpoint<T>& p,const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && handler.write(p.x);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.y);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  };

  /**
   * Three dimensional point, containing the coordinates x, y and z.
   * The template type T will be the one used for each coordinate.
   * 
   * This data structure simplifies the manipulation of 3D points providing
   * simple interfaces for adding, substracting, distance (L2), and more.
   *
   * @ingroup gGeomData
   */
  template <class T>
  class tpoint3D {
    public:

    /**
     * Used for the template-based interface for pixels as vectors.
     */
    typedef T value_type;

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * Coordinate x
     */
    T x;

    /**
     * Coordinate y;
     */
    T y;

    /**
     * Coordinate z;
     */
    T z;

    /**
     * Default constructor
     */
    explicit tpoint3D(const T newx=0,
                      const T newy=0,
                      const T newz=0) : x(newx),y(newy),z(newz) {};

    /**
     * Copy constructor
     */
    template <class U>
    tpoint3D(const tpoint3D<U>& p)
      : x(static_cast<T>(p.x)),y(static_cast<T>(p.y)),z(static_cast<T>(p.z)) {
    };

    /**
     * Copy constructor
     */
    template <class U>
    tpoint3D<T>& castFrom(const tpoint3D<U>& p) {
      x = static_cast<T>(p.x);
      y = static_cast<T>(p.y);
      z = static_cast<T>(p.z);
      return (*this);
    };

    /**
     * Set the coordinate values and return a reference to this point
     */
    inline tpoint3D<T>& set(const T tx,const T ty,const T tz) {
      x = tx;
      y = ty;
      z = tz;
      return *this;
    }

    /**
     * Calculate distance to the point c
     */
    inline T distanceTo(const tpoint3D<T>& c) const;

    /**
     * Calculate %square of distance to the point c. This
     * method is faster than distanceTo (because it does not calculate
     * the root of a*a + b*b).
     */
    inline T distanceSqr(const tpoint3D<T>& c) const;

    /**
     * Return the square of the magnitude of this point
     */
    inline T absSqr() const;

    /**
     * Multiply tpoint3D<T> with a given factor
     */
    template <class U>
    inline tpoint3D<T>& multiply(const U c) {
      x = static_cast<T>(x*c);
      y = static_cast<T>(y*c);
      z = static_cast<T>(z*c);
      return (*this);
    };

    /**
     * Multiply tpoint3D<T> with a given factor
     */
    template <class U>
    inline tpoint3D<T>& multiply(const tpoint3D<T>& other,const U c) {
      x = static_cast<T>(other.x*c);
      y = static_cast<T>(other.y*c);
      z = static_cast<T>(other.z*c);
      return (*this);
    };

    /**
     * Multiply tpoint3D<T> with a given factor
     */
    template <class U>
    inline tpoint3D<T> operator*(const U c) const {
      return tpoint3D<T>(static_cast<T>(x*c),
                         static_cast<T>(y*c),
                         static_cast<T>(z*c));
    };

    /**
     * Multiply tpoint3D<T> with a given factor
     */
    template <class U>
    inline tpoint3D<T>& operator*=(const U c) {
      x = static_cast<T>(x*c);
      y = static_cast<T>(y*c);
      z = static_cast<T>(z*c);
      return (*this);
    };

    /**
     * Multiply element-wise tpoint3D<T> with another point
     */
    inline tpoint3D<T> operator*(const tpoint3D<T>& c) const;

    /**
     * Multiply element-wise tpoint3D<T> with another point c
     */
    inline tpoint3D<T>& operator*=(const tpoint3D<T>& c);

    /**
     * Multiplies elementwise the components of \a a and \a b and leave the
     * result here.
     */
    inline tpoint3D<T>& emultiply(const tpoint3D<T>& a,const tpoint3D<T>& b);

    /**
     * Multiplies elementwise the components of this and the point c, and
     * leave the result here.
     */
    inline tpoint3D<T>& emultiply(const tpoint3D<T>& c);

    /**
     * Divide each component of tpoint3D<T> with a given factor
     */
    template<class U>
    inline tpoint3D<T>& divide(const U c) {
      x = static_cast<T>(x/c);
      y = static_cast<T>(y/c);
      z = static_cast<T>(z/c);
      return (*this);
    };

    /**
     * Divide each component of tpoint3D<T> with a given factor
     */
    template<class U>
    inline tpoint3D<T>& divide(const tpoint3D<T>& other,const U c) {
      x = static_cast<T>(other.x/c);
      y = static_cast<T>(other.y/c);
      z = static_cast<T>(other.z/c);
      return (*this);
    };

    /**
     * Divide each component of tpoint3D<T> with a given factor
     */
    template <class U>
    inline tpoint3D<T> operator/(const U c) const {
      return tpoint3D<T>(static_cast<T>(x/c),
                         static_cast<T>(y/c),
                         static_cast<T>(z/c));
    };

    /**
     * Divide each component of tpoint3D<T> with a given factor
     */
    template <class U>
    inline tpoint3D<T>& operator/=(const U c) {
      x = static_cast<T>(x/c);
      y = static_cast<T>(y/c);
      z = static_cast<T>(z/c);
      return (*this);
    };


    /**
     * Elementwise division of each component of the points
     */
    inline tpoint3D<T> operator/(const tpoint3D<T>& c) const;

    /**
     * Element-wise division
     */
    inline tpoint3D<T>& operator/=(const tpoint3D<T>& c);

    /**
     * Elementwise division of each component of the points
     */
    inline tpoint3D<T>& edivide(const tpoint3D<T>& c);

    /**
     * Elementwise division of each component of the points
     */
    inline tpoint3D<T>& edivide(const tpoint3D<T>& a,const tpoint3D<T>& b);

    /**
     * Modulo c of the integer part of each component of the point3D
     */
    inline tpoint3D<T> operator%(const int c) const;

    /**
     * Add the content of this point with the other point \a p and leave 
     * the result in this point.
     */
    inline tpoint3D<T>& add(const tpoint3D<T>& p);

    /**
     * Add the points \a p1 and \a p2 and leave the result in this point.
     */
    inline tpoint3D<T>& add(const tpoint3D<T>& p1,
                            const tpoint3D<T>& p2);

    /**
     * Operator +
     */
    inline tpoint3D<T> operator+(const tpoint3D<T>& p) const;

    /**
     * Operator +
     */
    inline tpoint3D<T>& operator+=(const tpoint3D<T>& p);

    /**
     * Subtract
     */
    inline tpoint3D<T>& subtract(const tpoint3D<T>& p);

    /**
     * Subtract
     */
    inline tpoint3D<T>& subtract(const tpoint3D<T>& p1,
                                 const tpoint3D<T>& p2);

    /**
     * Operator -
     */
    inline tpoint3D<T> operator-(const tpoint3D<T>& p) const;

    /**
     * Operator -
     */
    inline tpoint3D<T> operator-=(const tpoint3D<T>& p);

    /**
     * Dot product with another point
     */
    inline T dot(const tpoint3D<T>& p) const;

    /**
     * Copy operator
     */
    inline tpoint3D<T>& copy(const tpoint3D<T>& p);

    /**
     * Operator =
     */
    inline tpoint3D<T>& operator=(const tpoint3D<T>& p) {return copy(p);};

    /**
     * Operator ==
     */
    inline bool operator==(const tpoint3D<T>& p) const;

    /**
     * Operator !=
     */
    inline bool operator!=(const tpoint3D<T>& p) const;

    /**
     * Operator<
     *
     * A point3D is smaller than another one if its z component is
     * smaller, or if both z components are equal, if its y component is
     * smaller, of if both y are equal, if its x component is smaller
     */ 
    inline bool operator<(const tpoint3D<T>& p) const;

    /**
     * Operator>
     *
     * A point3D is smaller than another one if its z component is
     * smaller, or if both z components are equal, if its y component is
     * smaller, of if both y are equal, if its x component is smaller
     */ 
    inline bool operator>(const tpoint3D<T>& p) const;

    /**
     * @name access as vector
     */
    //@{
    /**
     * Used to simulate vector access.  It is slower than the normal
     * access to the elements x, y and z, but allow the use of point
     * in templates expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the color components will be [0] for x and [1] for y
     */
    inline T& operator[](const int i) {
      assert(i<3);
      switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return x;
      }
      return x;
    }

    /**
     * Used to simulate read-only vector access.  It is slower than
     * the normal access to the elements x, y and z, but allow the use
     * of point in templates expecting a vector-like structure.
     *
     * The correspondence between the elements of the vector and
     * the color components will be [0] for x and [1] for y
     */
    inline const T& operator[](const int i) const {
      assert(i<3);
      switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: return x;
      }
      return x;
    }

    /**
     * Used to simulate the vector size
     */
    inline int size() const {
      return 3;
    }
    //@}

  };

  // implementation: not in Doxygen!!!

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::edivide(const tpoint3D<T>& c) {
    x/=c.x;
    y/=c.y;
    z/=c.z;
    return *this;
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::edivide(const tpoint3D<T>& a,
                                           const tpoint3D<T>& b) {
    x=a.x/b.x;
    y=a.y/b.y;
    z=a.z/b.z;
    return *this;
  }

  template <class T>
  inline tpoint3D<T> tpoint3D<T>::operator/(const tpoint3D<T>& c) const {
    return tpoint3D<T>(x/c.x,y/c.y,z/c.z);
  }

  /**
   * Element-wise division
   */
  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::operator/=(const tpoint3D<T>& c) {
    x = static_cast<T>(x/c.x);
    y = static_cast<T>(y/c.y);
    z = static_cast<T>(z/c.z);
    return (*this);
  };

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::emultiply(const tpoint3D<T>& c) {
    x*=c.x;
    y*=c.y;
    z*=c.z;
    return *this;
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::emultiply(const tpoint3D<T>& a,
                                         const tpoint3D<T>& b) {
    x=a.x*b.x;
    y=a.y*b.y;
    z=a.z*b.z;
    return *this;
  }


  template <class T>
  inline tpoint3D<T> tpoint3D<T>::operator%(const int c) const {
    return tpoint3D<T>(static_cast<int>(x)%c,
                       static_cast<int>(y)%c,
                       static_cast<int>(z)%c);
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::add(const tpoint3D<T>& p) {
    x+=p.x;
    y+=p.y;
    z+=p.z;
    return (*this);
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::add(const tpoint3D<T>& p1,
                                       const tpoint3D<T>& p2) {
    x = p1.x + p2.x;
    y = p1.y + p2.y;
    z = p1.z + p2.z;

    return (*this);
  }

  template <class T>
  inline tpoint3D<T> tpoint3D<T>::operator+(const tpoint3D<T>& p) const {
    return tpoint3D<T>(x+p.x,y+p.y,z+p.z);
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::operator+=(const tpoint3D<T>& p) {
    return add(p);
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::subtract(const tpoint3D<T>& p) {
    x-=p.x;
    y-=p.y;
    z-=p.z;
    return (*this);
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::subtract(const tpoint3D<T>& p1,
                                            const tpoint3D<T>& p2) {
    x = p1.x - p2.x;
    y = p1.y - p2.y;
    z = p1.z - p2.z;

    return (*this);
  }

  template <class T>
  inline tpoint3D<T> tpoint3D<T>::operator-(const tpoint3D<T>& p) const {
    return tpoint3D<T>(x-p.x,y-p.y,z-p.z);
  }

  template <class T>
  inline tpoint3D<T> tpoint3D<T>::operator-=(const tpoint3D<T>& p) {
    return subtract(p);
  }

  template <class T>
  inline tpoint3D<T> tpoint3D<T>::operator*(const tpoint3D<T>& c) const {
    return tpoint3D<T>(c.x*x,c.y*x,c.z*z);
  }


  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::operator*=(const tpoint3D<T>& c) {
    x*=c.x;
    y*=c.y;
    z*=c.z;
    return *this;
  }



  template <class T>
  inline T tpoint3D<T>::dot(const tpoint3D<T>& p) const {
    return static_cast<T>((x*p.x) + (y*p.y) + (z*p.z));
  }

  template <class T>
  inline T tpoint3D<T>::absSqr() const {
    return static_cast<T>((x*x) + (y*y) + (z*z));
  }

  template <class T>
  inline T tpoint3D<T>::distanceTo(const tpoint3D<T>& c) const {
    tpoint3D<T> t((*this)-c);
    return sqrt(t.absSqr());
  }

  template <class T>
  inline T tpoint3D<T>::distanceSqr(const tpoint3D<T>& c) const {
    tpoint3D<T> t((*this)-c);
    return t.absSqr();
  }

  template <class T>
  inline tpoint3D<T>& tpoint3D<T>::copy(const tpoint3D<T>& p) {
    x = p.x;
    y = p.y;
    z = p.z;
    return (*this);
  }

  template <class T>
  inline bool tpoint3D<T>::operator==(const tpoint3D<T>& p) const {
    return ((p.y == y) && (p.x == x) && (p.z == z));
  }

  template <class T>
  inline bool tpoint3D<T>::operator!=(const tpoint3D<T>& p) const {
    return ((p.y != y) || (p.x != x) || (p.z != p.z));
  }

  template <class T>
  inline bool tpoint3D<T>::operator<(const tpoint3D<T>& p) const {
    return ((z < p.z) ||
            ((z==p.z) && ((y < p.y) ||
                          ((y == p.y) && (x < p.x)))));
  }

  template <class T>
  inline bool tpoint3D<T>::operator>(const tpoint3D<T>& p) const {
    return ((z > p.z) ||
            ((z==p.z) && ((y > p.y) ||
                          ((y == p.y) && (x > p.x)))));
  }

  /**
   * A three dimensional point with integer coordinates
   *
   * \deprecated Please use ipoint3D instead
   */
  typedef tpoint3D<int> point3D;

  /**
   * A three dimensional point with integer coordinates
   */
  typedef tpoint3D<int> ipoint3D;

  /**
   * A three dimensional point with integer coordinates
   */
  typedef tpoint3D<float> fpoint3D;
  /**
   * A three dimensional point with double coordinates
   */
  typedef tpoint3D<double> dpoint3D;

  /**
   * Read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <class T>
    bool read(ioHandler& handler,tpoint3D<T>& p,const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && handler.read(p.x);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.y);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.z);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  };

  /**
   * Write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<class T>
  bool write(ioHandler& handler,const tpoint3D<T>& p,const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }
    b = b && handler.write(p.x);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.y);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.z);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  };
}

namespace std {

  //inline ostream& operator<<(ostream& s,const lti::point& p);
  template <class T>
    inline ostream& operator<<(ostream& s,const lti::tpoint<T>& p) {
    s << "(" << p.x << ","
      << p.y << ")";
    return s;
  };

  //inline ostream& operator>>(istream& s,const lti::point& p);
  template <class T>
    inline istream& operator>>(istream& s,lti::tpoint<T>& p) {
    char c;
    T x,y;
    s >> c
      >> x >> c
      >> y >> c;
    p.x = x;
    p.y = y;

    return s;
  };

  //inline ostream& operator<<(ostream& s,const lti::point& p);
  template <class T>
  inline ostream& operator<<(ostream& s,const lti::tpoint3D<T>& p) {
    s << "("
      << p.x << ","
      << p.y << ","
      << p.z << ")";
    return s;
  };

  //inline ostream& operator>>(istream& s,const lti::point& p);
  template <class T>
  inline istream& operator>>(istream& s,lti::tpoint3D<T>& p) {
    char c;
    T x,y,z;
    s >> c
      >> x >> c
      >> y >> c
      >> z >> c;
    p.x = x;
    p.y = y;
    p.z = z;

    return s;
  };
}

#endif



