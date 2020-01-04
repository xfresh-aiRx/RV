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
 * file .......: ltiHTypes.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 07.06.01
 * revisions ..: $Id: ltiHTypes.h,v 1.11 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef LTI_H_TYPES_H
#define LTI_H_TYPES_H

#include "ltiTypes.h"
#include "ltiMatrix.h"

/**
 * @file ltiHTypes.h
 * This file contains the definitions of the basic types
 * for projective algebra in 2 and 3 dimensions.
 *
 * The types are basically points and matrices
 */

namespace lti {
  /**
   * Homogeneous point in two dimensions
   * Note that an homogeneous two-dimensional point has three elements:
   * x,y and h.  All arithmetical operations will be calculated for the
   * corresponding non-homogeneous 2D point, and then homogenized.  For example
   * the addition of two hPoints2D will be:
   * \f[ \begin{pmatrix}x_1 \\ y_1 \\ h_1 \end{pmatrix} +
   *     \begin{pmatrix}x_2 \\ y_2 \\ h_2 \end{pmatrix} =
   *     \begin{pmatrix}x_1/h_1 \\ y_1/h_1 \\ 1 \end{pmatrix} +
   *     \begin{pmatrix}x_2/h_2 \\ y_2/h_2 \\ 1 \end{pmatrix} =
   *     \begin{pmatrix}\frac{x_1}{h_1}+\frac{x_2}{h_2} \\
   *                    \frac{y_1}{h_1}+\frac{y_2}{h_2} \\ 1 \end{pmatrix} =
   *     \begin{pmatrix} x_1+x_2 \cdot \frac{h_1}{h_2} \\
   *                     y_1+y_2 \cdot \frac{h_1}{h_2} \\ h_1 \end{pmatrix}\f]
   */
  template <class T>
  class hPoint2D {
    public:

    /**
     * coordinate x
     */
    T& x;

    /**
     * coordinate y;
     */
    T& y;


    /**
     * component h
     */
    T& h;

    /**
     * this constant value indicates the dimensionality of this point
     * as vector: i.e. this will contain the value 3
     */
    static const int totalDimensionality;

    /**
     * default constructor
     */
    explicit hPoint2D(const T newx=0,const T newy=0,const T newh=T(1))
      : x(data[0]),y(data[1]),h(data[2]) {
      x=newx;
      y=newy;
      h=newh;
    };

    /**
     * copy constructor
     */
    hPoint2D(const hPoint2D<T>& p)
      : x(data[0]),y(data[1]),h(data[2]) {
      copy(p);
    };

    /**
     * copy constructor
     */
    template <class U>
    hPoint2D<T>& castFrom(const hPoint2D<U>& p) {
      x = static_cast<T>(p.x);
      y = static_cast<T>(p.y);
      h = static_cast<T>(p.h);
      return (*this);
    };

    /**
     * copy constructor
     */
    hPoint2D(const tpoint<T>& p)
      : x(data[0]),y(data[1]),h(data[2]) {
      copy(p);
    };

    /**
     * copy constructor
     */
    template <class U>
    hPoint2D<T>& castFrom(const tpoint<U>& p) {
      x = static_cast<T>(p.x);
      y = static_cast<T>(p.y);
      h = static_cast<T>(T(1));
      return (*this);
    };

    /**
     * copy this point normalized into the given tpoint and return a
     * reference to it
     */
    inline tpoint<T>& castTo(tpoint<T>& p) const {
      p.x = x/h;
      p.y = y/h;
      return p;
    };

    /**
     * multiply hPoint2D<T> with a given factor
     * This will affect the (x,y) values! (multiplying h would had no effect!)
     */
    inline hPoint2D<T>& multiply(const T c);

    /**
     * multiply hPoint2D<T> with a given factor
     * This will affect the (x,y) values! (multiplying h would had no effect!)
     */
    inline hPoint2D<T> operator*(const T c) const;

    /**
     * this product multiplies elementwise the components of this and
     * the point c.
     */
    inline hPoint2D<T> operator*(const hPoint2D<T>& c) const;

    /**
     * this product multiplies elementwise the components of this and
     * the point c
     */
    inline hPoint2D<T>& operator*=(const hPoint2D<T>& c);

    /**
     * divide each component of hPoint2D<T> (x,y only) with a given factor
     */
    inline hPoint2D<T>& divide(const T c);

    /**
     * divide each component of hPoint2D<T> with a given factor
     */
    inline hPoint2D<T> operator/(const T c) const;

    /**
     * elementwise division of each component of the points
     */
    inline hPoint2D<T> operator/(const hPoint2D<T>& c) const;

    /**
     * elementwise division of each component of the points
     */
    inline hPoint2D<T>& operator/=(const hPoint2D<T>& c);

    /**
     * add
     */
    inline hPoint2D<T>& add(const hPoint2D<T>& p);

    /**
     * operator +
     */
    inline hPoint2D<T> operator+(const hPoint2D<T>& p) const;

    /**
     * operator +=
     */
    inline hPoint2D<T>& operator+=(const hPoint2D<T>& p);

    /**
     * subtract
     */
    inline hPoint2D<T>& subtract(const hPoint2D<T>& p);

    /**
     * operator -
     */
    inline hPoint2D<T> operator-(const hPoint2D<T>& p) const;

    /**
     * operator -=
     */
    inline hPoint2D<T>& operator-=(const hPoint2D<T>& p);

    /**
     * dot product with another point.
     * This consider the normalized x and y components only!
     * i.e. the result will be (x*p.x+y*p.y)/(h*p.h)
     */
    inline T dot(const hPoint2D<T>& p) const;

    /**
     * Dot product with a 3D vector. Typically, this would be a
     * transformation vector (e.g. a row of an hMatrix) which projects
     * the point onto an axis. The result is a normal dot product,
     * i.e. x*p.at(0)+y*p.at(1)+h*p.at(2).
     * @param p three dimensional vector
     * @return normal dot product between hPoint2D and vector
     */
    inline T dot(const vector<T>& p) const;

    /**
     * copy operator
     */
    inline hPoint2D<T>& copy(const hPoint2D<T>& p);

    /**
     * operator =
     */
    inline hPoint2D<T>& operator=(const hPoint2D<T>& p) {return copy(p);};

    /**
     * copy operator
     */
    inline hPoint2D<T>& copy(const tpoint<T>& p);

    /**
     * operator =
     */
    inline hPoint2D<T>& operator=(const tpoint<T>& p) {return copy(p);};

    /**
     * operator ==
     * This compares the normalized x and y coordinates,
     * i.e. x*p.h == p.x*h and the same thing for y
     */
    inline bool operator==(const hPoint2D<T>& p) const;

    /**
     * operator !=
     */
    inline bool operator!=(const hPoint2D<T>& p) const;

    /**
     * normalize this point.  This makes the h component 1
     * @return a reference to this point
     */
    inline hPoint2D<T>& normalize();

    /**
     * copy in this point the normalized point p
     * @return a reference to this point
     */
    inline hPoint2D<T>& normalize(const hPoint2D<T>& p);

    /**
     * operator to access the point as a vector.
     * The position x corresponds to the element 0.
     * The position y corresponds to the element 1.
     * The position h corresponds to the element 2.
     */
    inline T& operator[](const int& i);

    /**
     * operator to access the point as a vector.
     * The position x corresponds to the element 0.
     * The position y corresponds to the element 1.
     * The position h corresponds to the element 2.
     */
    inline const T& operator[](const int& i) const;

    /**
     * return the square of the magnitud of this point (after its
     * normalization)
     */
    inline T absSqr() const;

  protected:
    /**
     * the content of the point
     */
    T data[3];

  };

  // implementation: not in Doc++!!!

  /*
   * this constant value indicates the dimensionality of this point
   * as vector: i.e. this will contain the value 3
   */
  template <class T>
  const int hPoint2D<T>::totalDimensionality = 3;


  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::multiply(const T c) {
    x*=c;
    y*=c;
    // h will be keeped
    return (*this);
  }

  template <class T>
  inline hPoint2D<T> hPoint2D<T>::operator*(const T c) const {
    return hPoint2D<T>(x*c,y*c,h);
  }

  template <class T>
  inline hPoint2D<T> hPoint2D<T>::operator*(const hPoint2D<T>& c) const {
    return hPoint2D<T>(c.x*x/c.h,c.y*y/c.h,h);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::operator*=(const hPoint2D<T>& c) {
    x=x*c.x/c.h;
    y=x*c.y/c.h;
    return *this;
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::divide(const T c) {
    x/=c;
    y/=c;
    // h remains as it was...
    return (*this);
  }

  template <class T>
  inline hPoint2D<T> hPoint2D<T>::operator/(const T c) const {
    return hPoint2D<T>(x/c,y/c,h);
  }

  template <class T>
  inline hPoint2D<T> hPoint2D<T>::operator/(const hPoint2D<T>& c) const {
    return hPoint2D<T>(c.h*x/c.x,c.h*y/c.y,h);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::operator/=(const hPoint2D<T>& c) {
    x = c.h*x/c.x;
    y = c.h*y/c.y;
    // h the same ...
    return *this;
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::add(const hPoint2D<T>& p) {
    x+= (h*p.x/p.h);
    y+= (h*p.y/p.h);
    // h the same...
    return (*this);
  }

  template <class T>
  inline hPoint2D<T> hPoint2D<T>::operator+(const hPoint2D<T>& p) const {
    return hPoint2D<T>(x+(h*p.x/p.h),y+(h*p.y/p.h),h);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::operator+=(const hPoint2D<T>& p) {
    return add(p);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::subtract(const hPoint2D<T>& p) {
    x-=(p.x*h/p.h);
    y-=(p.y*h/p.h);
    // h the same...
    return (*this);
  }

  template <class T>
  inline hPoint2D<T> hPoint2D<T>::operator-(const hPoint2D<T>& p) const {
    return hPoint2D<T>(x-(p.x*h/p.h),y-(p.y*h/p.h),h);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::operator-=(const hPoint2D<T>& p) {
    return subtract(p);
  }

  template <class T>
  inline T hPoint2D<T>::dot(const hPoint2D<T>& p) const {
    return hPoint2D<T>(((x*p.x) + (y*p.y))/(h*p.h));
  }

  template <class T>
  inline T hPoint2D<T>::dot(const vector<T>& p) const {
    assert(p.size()==3);
    T d=T(0);
    d=x*p.at(0)+
      y*p.at(1)+
      h*p.at(2);
    return d;
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::copy(const hPoint2D<T>& p) {
    x = p.x;
    y = p.y;
    h = p.h;
    return (*this);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::copy(const tpoint<T>& p) {
    x = p.x;
    y = p.y;
    h = static_cast<T>(1);
    return (*this);
  }

  template <class T>
  inline bool hPoint2D<T>::operator==(const hPoint2D<T>& p) const {
    return ((h*p.y == p.h*y) && (h*p.x == p.h*x));
  }

  template <class T>
  inline bool hPoint2D<T>::operator!=(const hPoint2D<T>& p) const {
    return ((h*p.y != p.h*y) || (h*p.x != p.h*x));
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::normalize() {
    x /= h;
    y /= h;
    h = T(1);
    return (*this);
  }

  template <class T>
  inline hPoint2D<T>& hPoint2D<T>::normalize(const hPoint2D<T>& p) {
    x = p.x/p.h;
    y = p.y/p.h;
    h = T(1);
    return (*this);
  }

  template <class T>
  inline T& hPoint2D<T>::operator[](const int& i) {
    assert(i<totalDimensionality);
    return data[i];
  }

  template <class T>
  inline const T& hPoint2D<T>::operator[](const int& i) const {
    assert(i<totalDimensionality);
    return data[i];
  }

  template <class T>
    inline T hPoint2D<T>::absSqr() const {
    return (x*x+y*y)/(h*h);
  }

  /**
   * A point with double coordinates
   */
  typedef hPoint2D<double> dhPoint2D;

  /** @name Storable interface for points
   *  Members for the storable interface
   */
  //@{
  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  template <class T>
    bool read(ioHandler& handler,hPoint2D<T>& p,const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && handler.read(p.x);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.y);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.h);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  };

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  template<class T>
  bool write(ioHandler& handler,const hPoint2D<T>& p,
             const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && handler.write(p.x);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.y);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.h);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  };
  //@}



  /**
   * Homogeneous point in three dimensions.
   *
   * Note that an homogeneous three-dimensional point has four elements:
   * x,y,z and h.  All arithmetical operations will be calculated for the
   * corresponding non-homogeneous 3D point, and then homogenized.  For example
   * the addition of two hPoints3D will be:
   * \f[ \begin{pmatrix}x_1 \\ y_1 \\ z_1 \\ h_1 \end{pmatrix} +
   *     \begin{pmatrix}x_2 \\ y_2 \\ z_2 \\ h_2 \end{pmatrix} =
   *     \begin{pmatrix}x_1/h_1 \\ y_1/h_1 \\ z_1/h_1 \\ 1 \end{pmatrix} +
   *     \begin{pmatrix}x_2/h_2 \\ y_2/h_2 \\ z_2/h_2 \\ 1 \end{pmatrix} =
   *     \begin{pmatrix}\frac{x_1}{h_1}+\frac{x_2}{h_2} \\
   *                    \frac{y_1}{h_1}+\frac{y_2}{h_2} \\
   *                    \frac{z_1}{h_1}+\frac{z_2}{h_2} \\ 1 \end{pmatrix} =
   *     \begin{pmatrix} x_1+x_2 \cdot \frac{h_1}{h_2} \\
   *                     y_1+y_2 \cdot \frac{h_1}{h_2} \\
   *                     z_1+z_2 \cdot \frac{h_1}{h_2} \\ h_1 \end{pmatrix}\f]
   */
  template <class T>
  class hPoint3D {
    public:

    /**
     * coordinate x
     */
    T& x;

    /**
     * coordinate y;
     */
    T& y;

    /**
     * coordinate z;
     */
    T& z;

    /**
     * component h
     */
    T& h;

    /**
     * This constant value indicates the dimensionality of this point
     * as vector: i.e. this will contain the value 4
     */
    static const int totalDimensionality;

    /**
     * default constructor
     */
    explicit hPoint3D(const T newx=0,
                      const T newy=0,
                      const T newz=0,
                      const T newh=T(1))
      : x(data[0]),y(data[1]),z(data[2]),h(data[3]) {
      x = newx;
      y = newy;
      z = newz;
      h = newh;
    };

    /**
     * copy constructor
     */
    hPoint3D(const hPoint3D<T>& p)
      : x(data[0]),y(data[1]),z(data[2]),h(data[3]) {
      copy(p);
    };

    /**
     * copy constructor
     */
    hPoint3D(const hPoint2D<T>& p)
      : x(data[0]),y(data[1]),z(data[2]),h(data[3]) {
      copy(p);
    };

    /**
     * copy constructor
     */
    hPoint3D(const tpoint<T>& p)
      : x(data[0]),y(data[1]),z(data[2]),h(data[3]) {
      copy(p);
    };

    /**
     * copy constructor
     */
    hPoint3D(const tpoint3D<T>& p)
      : x(data[0]),y(data[1]),z(data[2]),h(data[3]) {
      copy(p);
    };

    /**
     * copy constructor
     */
    template <class U>
    hPoint3D<T>& castFrom(const hPoint3D<U>& p) {
      x = static_cast<T>(p.x);
      y = static_cast<T>(p.y);
      z = static_cast<T>(p.z);
      h = static_cast<T>(p.h);
      return (*this);
    };


    /**
     * copy constructor
     */
    template <class U>
    hPoint3D<T>& castFrom(const tpoint3D<U>& p) {
      x = static_cast<T>(p.x);
      y = static_cast<T>(p.y);
      z = static_cast<T>(p.z);
      h = static_cast<T>(T(1));
      return (*this);
    };

    /**
     * copy this point normalized into the given tpoint and return a
     * reference to it
     */
    inline tpoint3D<T>& castTo(tpoint3D<T>& p) const {
      p.x = x/h;
      p.y = y/h;
      p.z = z/h;
      return p;
    };

    /**
     * copy this point into the given hPoint2D and return a
     * reference to it.  The z coordinate will be ignored (this corresponds
     * to an orthogonal projection of the point in the x-y plane
     */
    inline hPoint2D<T>& project(hPoint2D<T>& p) const {
      p.x = x;
      p.y = y;
      p.h = h;
      return p;
    };

    /**
     * copy this point normalized into the given tpoint and return a
     * reference to it.  The z coordinate will be ignored (this corresponds
     * to an orthogonal projection of the point in the x-y plane
     */
    inline tpoint<T>& project(tpoint<T>& p) const {
      p.x = x/h;
      p.y = y/h;
      return p;
    };

    /**
     * multiply hPoint3D<T> with a given factor
     * This will affect the (x,y) values! (multiplying h would had no effect!)
     */
    inline hPoint3D<T>& multiply(const T c);

    /**
     * multiply hPoint3D<T> with a given factor
     * This will affect the (x,y) values! (multiplying h would had no effect!)
     */
    inline hPoint3D<T> operator*(const T c) const;

    /**
     * this product multiplies elementwise the components of this and
     * the point c.
     */
    inline hPoint3D<T> operator*(const hPoint3D<T>& c) const;

    /**
     * this product multiplies elementwise the components of this and
     * the point c
     */
    inline hPoint3D<T>& operator*=(const hPoint3D<T>& c);

    /**
     * divide each component of hPoint3D<T> (x,y only) with a given factor
     */
    inline hPoint3D<T>& divide(const T c);

    /**
     * divide each component of hPoint3D<T> with a given factor
     */
    inline hPoint3D<T> operator/(const T c) const;

    /**
     * elementwise division of each component of the points
     */
    inline hPoint3D<T> operator/(const hPoint3D<T>& c) const;

    /**
     * elementwise division of each component of the points
     */
    inline hPoint3D<T>& operator/=(const hPoint3D<T>& c);

    /**
     * add
     */
    inline hPoint3D<T>& add(const hPoint3D<T>& p);

    /**
     * operator +
     */
    inline hPoint3D<T> operator+(const hPoint3D<T>& p) const;

    /**
     * operator +=
     */
    inline hPoint3D<T>& operator+=(const hPoint3D<T>& p);

    /**
     * subtract
     */
    inline hPoint3D<T>& subtract(const hPoint3D<T>& p);

    /**
     * operator -
     */
    inline hPoint3D<T> operator-(const hPoint3D<T>& p) const;

    /**
     * operator -=
     */
    inline hPoint3D<T>& operator-=(const hPoint3D<T>& p);

    /**
     * dot product with another point.
     * This consider the normalized x and y components only!
     * i.e. the result will be (x*p.x+y*p.y)/(h*p.h)
     */
    inline T dot(const hPoint3D<T>& p) const;

    /**
     * Dot product with a 4D vector. Typically, this would be a
     * transformation vector (e.g. a row of an hMatrix) which projects
     * the point onto an axis. The result is a normal dot product,
     * i.e. x*p.at(0)+...+h*p.at(3).
     * @param p four dimensional vector
     * @return normal dot product between hPoint3D and vector
     */
    inline T dot(const vector<T>& p) const;

    /**
     * copy operator
     */
    inline hPoint3D<T>& copy(const hPoint3D<T>& p);

    /**
     * copy operator (assume z=0)
     */
    inline hPoint3D<T>& copy(const hPoint2D<T>& p);

    /**
     * operator =
     */
    inline hPoint3D<T>& operator=(const hPoint3D<T>& p) {return copy(p);};

    /**
     * copy operator
     */
    inline hPoint3D<T>& copy(const tpoint3D<T>& p);

    /**
     * copy operator (assume z=0)
     */
    inline hPoint3D<T>& copy(const tpoint<T>& p);

    /**
     * operator =
     */
    inline hPoint3D<T>& operator=(const tpoint3D<T>& p) {return copy(p);};

    /**
     * operator ==
     * This compares the normalized x and y coordinates,
     * i.e. x*p.h == p.x*h and the same thing for y
     */
    inline bool operator==(const hPoint3D<T>& p) const;

    /**
     * operator !=
     */
    inline bool operator!=(const hPoint3D<T>& p) const;

    /**
     * normalize this point.  This makes the h component 1
     * @return a reference to this point
     */
    inline hPoint3D<T>& normalize();

    /**
     * normalize this point.  This makes the h component 1
     * @return a reference to this point
     */
    inline hPoint3D<T>& normalize(const hPoint3D<T>& p);

    /**
     * operator to access the point as a vector.
     * The position x corresponds to the element 0.
     * The position y corresponds to the element 1.
     * The position z corresponds to the element 2.
     * The position h corresponds to the element 3.
     */
    inline T& operator[](const int& i);

    /**
     * operator to access the point as a vector.
     * The position x corresponds to the element 0.
     * The position y corresponds to the element 1.
     * The position z corresponds to the element 2.
     * The position h corresponds to the element 3.
     */
    inline const T& operator[](const int& i) const;

    /**
     * return the square of the magnitud of this point (after its
     * normalization)
     */
    inline T absSqr() const;

  protected:
    /**
     * the content of the point
     */
    T data[4];
  };

  // implementation: not in Doc++!!!

  /*
   * this constant value indicates the dimensionality of this point
   * as vector: i.e. this will contain the value 3
   */
  template <class T>
    const int hPoint3D<T>::totalDimensionality = 4;

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::multiply(const T c) {
    x*=c;
    y*=c;
    z*=c;
    // h will be keeped
    return (*this);
  }

  template <class T>
  inline hPoint3D<T> hPoint3D<T>::operator*(const T c) const {
    return hPoint3D<T>(x*c,y*c,z*c,h);
  }

  template <class T>
  inline hPoint3D<T> hPoint3D<T>::operator*(const hPoint3D<T>& c) const {
    return hPoint3D<T>(c.x*x/c.h,c.y*y/c.h,c.z*z/c.h,h);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::operator*=(const hPoint3D<T>& c) {
    x=x*c.x/c.h;
    y=y*c.y/c.h;
    z=z*c.z/c.h;
    return *this;
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::divide(const T c) {
    x/=c;
    y/=c;
    z/=c;
    // h remains as it was...
    return (*this);
  }

  template <class T>
  inline hPoint3D<T> hPoint3D<T>::operator/(const T c) const {
    return hPoint3D<T>(x/c,y/c,z/c,h);
  }

  template <class T>
  inline hPoint3D<T> hPoint3D<T>::operator/(const hPoint3D<T>& c) const {
    return hPoint3D<T>(c.h*x/c.x,c.h*y/c.y,c.h*z/c.z,h);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::operator/=(const hPoint3D<T>& c) {
    x = c.h*x/c.x;
    y = c.h*y/c.y;
    z = c.h*z/c.z;
    // h the same ...
    return *this;
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::add(const hPoint3D<T>& p) {
    x+= (h*p.x/p.h);
    y+= (h*p.y/p.h);
    z+= (h*p.z/p.h);
    // h the same...
    return (*this);
  }

  template <class T>
  inline hPoint3D<T> hPoint3D<T>::operator+(const hPoint3D<T>& p) const {
    return hPoint3D<T>(x+(h*p.x/p.h),y+(h*p.y/p.h),z+(h*p.z/p.h),h);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::operator+=(const hPoint3D<T>& p) {
    return add(p);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::subtract(const hPoint3D<T>& p) {
    x-=(p.x*h/p.h);
    y-=(p.y*h/p.h);
    z-=(p.z*h/p.h);
    // h the same...
    return (*this);
  }

  template <class T>
  inline hPoint3D<T> hPoint3D<T>::operator-(const hPoint3D<T>& p) const {
    return hPoint3D<T>(x-(p.x*h/p.h),y-(p.y*h/p.h),z-(p.z*h/p.h),h);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::operator-=(const hPoint3D<T>& p) {
    return subtract(p);
  }

  template <class T>
  inline T hPoint3D<T>::dot(const hPoint3D<T>& p) const {
    return hPoint3D<T>(((x*p.x) + (y*p.y) + (z*p.z))/(h*p.h));
  }

  template <class T>
  inline T hPoint3D<T>::dot(const vector<T>& p) const {
    assert(p.size()==4);
    T d=T(0);
    d=x*p.at(0)+
      y*p.at(1)+
      z*p.at(2)+
      h*p.at(3);
    return d;
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::copy(const hPoint3D<T>& p) {
    x = p.x;
    y = p.y;
    z = p.z;
    h = p.h;
    return (*this);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::copy(const hPoint2D<T>& p) {
    x = p.x;
    y = p.y;
    z = T(0);
    h = p.h;
    return (*this);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::copy(const tpoint3D<T>& p) {
    x = p.x;
    y = p.y;
    z = p.z;
    h = static_cast<T>(1);
    return (*this);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::copy(const tpoint<T>& p) {
    x = p.x;
    y = p.y;
    z = T(0);
    h = static_cast<T>(1);
    return (*this);
  }

  template <class T>
  inline bool hPoint3D<T>::operator==(const hPoint3D<T>& p) const {
    return ((h*p.y == p.h*y) && (h*p.x == p.h*x) && (h*p.z == p.h*z));
  }

  template <class T>
  inline bool hPoint3D<T>::operator!=(const hPoint3D<T>& p) const {
    return ((h*p.y != p.h*y) || (h*p.x != p.h*x) || (h*p.z != p.h*z));
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::normalize() {
    x /= h;
    y /= h;
    z /= h;
    h = T(1);
    return (*this);
  }

  template <class T>
  inline hPoint3D<T>& hPoint3D<T>::normalize(const hPoint3D<T>& p) {
    x = p.x/p.h;
    y = p.y/p.h;
    z = p.z/p.h;
    h = T(1);
    return (*this);
  }

  template <class T>
  inline T& hPoint3D<T>::operator[](const int& i) {
    assert(i<totalDimensionality);
    return data[i];
  }

  template <class T>
  inline const T& hPoint3D<T>::operator[](const int& i) const {
    assert(i<totalDimensionality);
    return data[i];
  }

  template <class T>
    inline T hPoint3D<T>::absSqr() const {
    return (x*x+y*y+z*z)/(h*h);
  }


  /**
   * A point with double coordinates
   */
  typedef hPoint3D<double> dhPoint3D;

  /** @name Storable interface for points
   *  Members for the storable interface
   */
  //@{
  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  template <class T>
    bool read(ioHandler& handler,hPoint3D<T>& p,const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    b = b && handler.read(p.x);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.y);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.z);
    b = b && handler.readDataSeparator();
    b = b && handler.read(p.h);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  };

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  template<class T>
  bool write(ioHandler& handler,const hPoint3D<T>& p,
             const bool complete=true) {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    b = b && handler.write(p.x);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.y);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.z);
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.h);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  };
  //@}


  /**
   * Homogeneous matrix for projective space transformations.
   *
   * The template class T indicates the contained type and the class
   * P the point type (hPoint2D or hPoint3D)
   */
  template <class T, class P>
  class hMatrix : public mathObject {
    public:
    /**
     * type of the contained data
     */
    typedef T value_type;

    /**
     * return type of the size() member
     */
    typedef point size_type;

    /**
     * Default constructor.
     *
     * Initialize the matrix with the identity matrix (all elements in the
     * diagonal are one, the rest are zero).
     */
    hMatrix();

    /**
     * Copy constructor
     */
    hMatrix(const hMatrix<T,P>& other);

    /**
     * Copy constructor
     */
    hMatrix(const matrix<T>& other);

    /**
     * Destructor
     */
    virtual ~hMatrix();

    /**
     * Clean matrix (all elements with 0)
     */
    void clear();

    /**
     * Unit matrix.
     *
     * Initialize this matrix with the unit matrix having zeros in all
     * but the diagonal elements, which will be set to  one.
     */
    void unit();

    /**
     * Return the size of the %matrix in a lti::point structure.
     *
     * @return lti::point with the number of columns in its
     *         <code>x</code> coordinate and the number of rows in its
     *         <code>y</code> coordinate.
     */
    inline const point& size() const;

    /**
     * Returns the name of this class
     */
    virtual const char* getTypeName() const {
      return "hMatrix";
    };

    /**
     * Returns a copy of this object
     */
    virtual mathObject* clone() const;

    /**
     * Copy operator
     */
    hMatrix<T,P>& copy(const hMatrix<T,P>& other);

    /**
     * Copy operator
     */
    hMatrix<T,P>& copy(const matrix<T>& other);

    /**
     * Copy operator
     */
    hMatrix<T,P>& operator=(const hMatrix<T,P>& other) {
      return copy(other);
    }

    /**
     * Copy the contents of this %hmatrix into the given lti::matrix
     */
    matrix<T>& castTo(matrix<T>& result) const;

    /**
     * Copy the content of the given lti::matrix into this hmatrix.
     */
    hMatrix<T,P>& castFrom(const matrix<T>& other);

    /**
     * Write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,
                       const bool complete = true) const;

    /**
     * Read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    /**
     * Multiply this matrix with another homogeneous matrix and
     * leave the result here.
     */
    hMatrix<T,P>& multiply(const hMatrix<T,P>& other);

    /**
     * Left-Multiply this matrix with another homogeneous matrix and
     * leave the result here.
     */
    hMatrix<T,P>& leftMultiply(const hMatrix<T,P>& other);

    /**
     * Multiply the matrices a and b and leave the result here.
     */
    hMatrix<T,P>& multiply(const hMatrix<T,P>& a,const hMatrix<T,P>& b);
    
    /**
     * Return a new object which is the result of multiplying this matrix
     * with the other one
     */
    inline hMatrix<T,P> operator*(const hMatrix<T,P>& other) const {
      hMatrix<T,P> tmp;
      tmp.multiply(*this,other);
      return tmp;
    };

    /**
     * Alias for multiply
     */
    inline hMatrix<T,P>& operator*=(const hMatrix<T,P>& other) {
      return multiply(other);
    }

    /**
     * Multiply with a point and leave the result in the second
     * parameters.
     * Return a reference to the second parameters
     */
    P& multiply(const P& other, P& result) const;

    /**
     * Multiply with a homogeneous point
     */
    P operator*(const P& p) const {
      P temp;
      multiply(p,temp);

      return temp;
    };

    /**
     * Return value at row m and column n
     */
    inline T &at(const int& m,const int& n) {
      return theRows[m][n];
    };

    /**
     * Return value a row m and column n
     */
    inline const T &at(const int& m,const int& n) const {
      return theRows[m][n];
    };

    /**
     * Access operator to a row
     */
    inline T* operator[](const int& m) {
      return theRows[m];
    }

    /**
     * Read-only access operator to a row
     */
    inline const T* operator[](const int& m) const {
      return theRows[m];
    }

    /**
     * Invert this matrix an return a reference to it
     */
    hMatrix<T,P>& invert();

    /**
     * Copy here the other matrix inverted.
     */
    hMatrix<T,P>& invert(const hMatrix<T,P>& other);

    /**
     * Transpose this matrix an return a reference to it
     */
    hMatrix<T,P>& transpose();

    /**
     * Copy the transposed other matrix here.
     */
    hMatrix<T,P>& transpose(const hMatrix<T,P>& other);

    /**
     * Set the scale factor of the transformation.
     *
     * This is the element with the greatest indices.  Changing its value from
     * one will imply a scaling of everything, including the translation
     * factors.
     */
    void setScaleFactor(const T& s);

    /**
     * Return the scale factor used in the transformation.
     *
     * The scale factor is the element with the greatest indices.  Changing its
     * value from one will imply a scaling of everything, including the
     * translation factors.     
     */
    const T& getScaleFactor() const;

    /**
     * Multiply the scale factor with this value.
     *
     * The scale factor is the element with the greatest indices.  Changing its
     * value from one will imply a scaling of everything, including the
     * translation factors.
     */
    void scale(const T& s);

    /**
     * Multiply a scaling matrix with this one.
     *
     * The scaling matrix is obtaind multiplying a unit matrix with the
     * scalar \a s and setting the element with the greatest indices to
     * 1.0.
     *
     * This corresponds to scaling the rotation sub-matrix, but leaving the
     * translation components untouched.
     */
    void scaleR(const T& s);


    /**
     * This function sets the translation vector with the normalized data,
     * i.e. the given homogeneous point will be normalized and its components
     * (except the h component) will be used as the translation vector.
     */
    void setTranslation(const P& thePoint);

    /**
     * return a non-homegeneous point with the actual translation vector
     */
    P getTranslation() const;

    /**
     * Add the normalized homogeneous vector to the one in the matrix
     */
    void translate(const P& thePoint);

    /**
     * Multiply the rotation matrix with a new rotation matrix generated
     * from the given %parameters.
     * @param angle the rotation angle in radians.
     * @param axis this vector is the rotation-axis.
     * @param center the contents of this point will be normalized to get the
     *               center of the rotation.
     */
    void rotate(const double& angle,
                const hPoint3D<T>& axis=hPoint3D<T>(0,0,1),
                const hPoint3D<T>& center=hPoint3D<T>());

    /**
     * Set rotation submatrix with a new rotation matrix generated
     * from the given %parameters.
     * @param angle the rotation angle in radians.
     * @param axis this vector is the rotation-axis.
     * @param center the contents of this point will be normalized to get the
     *               center of the rotation.
     */
    void setRotation(const double& angle,
                     const hPoint3D<T>& axis=hPoint3D<T>(0,0,T(1)),
                     const hPoint3D<T>& center=hPoint3D<T>());

    /**
     * Set similarity transformation.
     *
     * The similarity transformation is defined as:
     * \f[ \begin{bmatrix} 
     *        s\cdot \cos(\alpha) & s\cdot -\sin(\alpha) & 0 & t_x \\
     *        s\cdot \sin(\alpha) & s\cdot  \cos(\alpha) & 0 & t_y \\
     *                 0          &           0          & 1 & 0   \\
     *                 0          &           0          & 0 & 1 
     *     \end{bmatrix} \f]
     * with \a s the scaling factor, \f$\alpha\f$ the rotation angle and
     * \f$(t_x,t_y)\f$ the translation.
     * @param t point with the two-dimensional translation amount
     * @param angle angle amount
     * @param scaling scaling amount
     */
    void setSimilarityTransform(const tpoint<T>& t,
                                const T& angle,
                                const T& scaling);

  protected:
    /**
     * Memory block with all elements
     */
    T* theElements;

    /**
     * Pointers to each row.
     */
    T** theRows;

    /**
     * Pointer to the element after the last element of the data
     */
    T* postElement;

    /**
     * Initialize the memory.
     *
     * This method allocates theElements and theRows and ensures that
     * theRows points to each row in theElements.
     */
    void initMem();

    /**
     * The real size of the matrix
     */
    const point theSize;
  };

  /**
   * Homogeneous matrix to represent linear transformations in a
   * two-dimensional projective space.
   *
   * The two-dimensional transformations are represented by 3x3 matrices of
   * the form:
   * \f[\begin{bmatrix}
   *       r_1 & r_2 & t_x \\ r_3 & r_4 & t_y \\ 0 & 0 & 1
   *    \end{bmatrix}\f]
   * where the terms \f$r_i\f$ represent a rotation and scaling and the
   * terms \f$t_\zeta\f$ the translation.
   */
  template <class T>
  class hMatrix2D : public hMatrix<T,hPoint2D<T> > {
    public:
    /**
     * Default constructor
     */
    hMatrix2D();

    /**
     * Copy constructor
     */
    hMatrix2D(const hMatrix2D<T>& other);

    /**
     * Copy constructor
     */
    hMatrix2D(const matrix<T>& other);

    /**
     * Destructor
     */
    virtual ~hMatrix2D();

    /**
     * Multiply this matrix with another homogeneous matrix and
     * leave the result here.
     */
    inline hMatrix2D<T>& multiply(const hMatrix2D<T>& other) {
      hMatrix<T,hPoint2D<T> >::multiply(other);
      return *this;
    };

    /**
     * Multiply the matrices a and b and leave the result here.
     */
    inline hMatrix2D<T>& multiply(const hMatrix2D<T>& a,
                                  const hMatrix2D<T>& b) {
      hMatrix<T,hPoint2D<T> >::multiply(a,b);
      return *this;
    };

    /**
     * Return a new object which is the result of multiplying this matrix
     * with the other one
     */
    inline hMatrix2D<T> operator*(const hMatrix2D<T>& other) const {
      hMatrix2D<T> tmp;
      tmp.multiply(*this,other);
      return tmp;
    };

    /**
     * Alias for multiply
     */
    inline hMatrix2D<T>& operator*=(const hMatrix2D<T>& other) {
      return multiply(other);
    }

    /**
     * Multiply with a point and leave the result in the second
     * parameters.
     * Return a reference to the second parameters
     */
    inline hPoint2D<T>& multiply(const hPoint2D<T>& other,
                                hPoint2D<T>& result) const {
      return hMatrix<T,hPoint2D<T> >::multiply(other,result);
    };

    /**
     * Multiply with a point
     */
    hPoint2D<T> operator*(const hPoint2D<T>& p) const {
      hPoint2D<T> temp;
      multiply(p,temp);

      return temp;
    };

    /**
     * Multiply with a point and leave the result in the second
     * parameters.
     * Return a reference to the second parameters
     */
    tpoint<T>& multiply(const tpoint<T>& other,
                              tpoint<T>& result) const;

    /**
     * Multiply with a point
     */
    tpoint<T> operator*(const tpoint<T>& p) const {
      tpoint<T> temp;
      multiply(p,temp);

      return temp;
    };
  };

  /**
   * Homogeneous matrix in 3D projective space.
   *
   * The three-dimensional transformations are represented by 4x4 matrices of
   * the form:
   * \f[\begin{bmatrix}
   *       r_1 & r_2 & r_3 & t_x \\ 
   *       r_4 & r_5 & r_6 & t_y \\ 
   *       r_7 & r_8 & r_9 & t_z \\
   *       0   & 0   & 0   & 1
   *    \end{bmatrix}\f]
   * where the terms \f$r_i\f$ represent a rotation and scaling and the
   * terms \f$t_\zeta\f$ the translation.
   */
  template <class T>
  class hMatrix3D : public hMatrix<T,hPoint3D<T> > {
    public:
    /**
     * default constructor
     */
    hMatrix3D();

    /**
     * copy constructor
     */
    hMatrix3D(const hMatrix3D<T>& other);

    /**
     * copy constructor
     */
    hMatrix3D(const hMatrix2D<T>& other);

    /**
     * copy constructor
     */
    hMatrix3D(const matrix<T>& other);

    /**
     * destructor
     */
    virtual ~hMatrix3D();

    /**
     * multiply this matrix with another homogeneous matrix and
     * leave the result here.
     */
    inline hMatrix3D<T>& multiply(const hMatrix3D<T>& other) {
      hMatrix<T,hPoint3D<T> >::multiply(other);
      return *this;
    };

    /**
     * multiply the matrices a and b and leave the result here.
     */
    inline hMatrix3D<T>& multiply(const hMatrix3D<T>& a,
                                  const hMatrix3D<T>& b) {
      hMatrix<T,hPoint3D<T> >::multiply(a,b);
      return *this;
    };

    /**
     * return a new object which is the result of multiplying this matrix
     * with the other one
     */
    inline hMatrix3D<T> operator*(const hMatrix3D<T>& other) {
      hMatrix3D<T> tmp;
      tmp.multiply(*this,other);
      return tmp;
    };

    /**
     * alias for multiply
     */
    inline hMatrix3D<T>& operator*=(const hMatrix3D<T>& other) {
      return multiply(other);
    }

    /**
     * multiply with a point and leave the result in the second
     * parameters.
     * Return a reference to the second parameters
     */
    inline hPoint3D<T>& multiply(const hPoint3D<T>& other,
                                hPoint3D<T>& result) const {
      return hMatrix<T,hPoint3D<T> >::multiply(other,result);
    };

    /**
     * multiply with a point
     */
    hPoint3D<T> operator*(const hPoint3D<T>& p) const {
      hPoint3D<T> temp;
      multiply(p,temp);

      return temp;
    };

    /**
     * multiply with a point and leave the result in the second
     * parameters.
     * Return a reference to the second parameters
     */
    tpoint3D<T>& multiply(const tpoint3D<T>& other,
                                tpoint3D<T>& result) const;

    /**
     * multiply with a point
     */
    tpoint3D<T> operator*(const tpoint3D<T>& p) const {
      tpoint3D<T> temp;
      multiply(p,temp);

      return temp;
    };

    /**
     * multiply with a point (assume z=0) and leave the result in the second
     * parameter.
     * Return a reference to the second parameters
     */
    hPoint2D<T>& multiply(const hPoint2D<T>& other,
                                hPoint2D<T>& result) const;

    /**
     * multiply with a point (assume z=0).
     */
    hPoint2D<T> operator*(const hPoint2D<T>& p) const {
      hPoint2D<T> temp;
      multiply(p,temp);

      return temp;
    };

    /**
     * multiply with a point (assume z=0) and leave the result in the second
     * parameters.
     * @return a reference to the second parameter
     */
    tpoint<T>& multiply(const tpoint<T>& other,
                              tpoint<T>& result) const;

    /**
     * multiply with a point (assume z=0)
     */
    tpoint<T> operator*(const tpoint<T>& p) const {
      tpoint<T> temp;
      multiply(p,temp);

      return temp;
    };

  };


  /**
   * homogeneous transformation matrix for 2D homogeneous points
   * of type double
   */
  typedef hMatrix2D<double> dhMatrix2D;

  /**
   * homogeneous transformation matrix for 2D homogeneous points
   * of type float
   */
  typedef hMatrix2D<float> fhMatrix2D;


  /**
   * homogeneous transformation matrix for 3D homogeneous points
   * of type double
   */
  typedef hMatrix3D<double> dhMatrix3D;

  /**
   * homogeneous transformation matrix for 3D homogeneous points
   * of type float
   */
  typedef hMatrix3D<float> fhMatrix3D;

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  template<class T,class P>
    bool write(ioHandler& handler,const hMatrix<T,P>& mat,
               const bool complete=true) {
    return mat.write(handler,complete);
  }

  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be read or not
   */
  template<class T,class P>
    bool read(ioHandler& handler,hMatrix<T,P>& mat,
              const bool complete=true) {
    return mat.read(handler,complete);
  }

}

namespace std {
  template<class T>
    ostream& operator<<(ostream& out,const lti::hPoint2D<T>& p) {
    out << "(" << p.x << "," << p.y << "," << p.h << ")";
    return out;
  }

  template<class T>
  ostream& operator<<(ostream& out,const lti::hPoint3D<T>& p) {
    out << "(" << p.x << "," << p.y << "," << p.z << "," << p.h << ")";
    return out;
  }

  template<class T,class P>
  ostream& operator<<(ostream& out,const lti::hMatrix<T,P>& p) {
    out << endl << "(";
    for (int j=0;j<P::totalDimensionality;++j) {
      if (j>0) {
        out << " (";
      } else {
        out << "(";
      }
      for (int i=0;i<P::totalDimensionality;++i) {
        out.width(14);
        out << p.at(j,i);
      }
      out << ")" << endl ;
    }
    out << ")" << endl;
    return out;
  }

}


#endif


