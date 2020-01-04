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
 * revisions ..: $Id: ltiHTypes_template.h,v 1.12 2005/10/31 08:26:54 libuda Exp $
 */

#include "ltiMatrixInversion.h"

namespace lti {

  // -----------------------------------------
  // hMatrix
  // -----------------------------------------

  template <class T,class P>
  hMatrix<T,P>::hMatrix()
  : mathObject(),theElements(0),theRows(0),
    theSize(point(P::totalDimensionality,P::totalDimensionality)) {

    initMem();
    unit();
  }

  template <class T,class P>
  hMatrix<T,P>::hMatrix(const hMatrix<T,P>& other)
  : mathObject(),theElements(0),theRows(0),
    theSize(point(P::totalDimensionality,P::totalDimensionality)) {

    initMem();
    copy(other);
  }

  template <class T,class P>
  hMatrix<T,P>::hMatrix(const matrix<T>& other)
  : mathObject(),theElements(0),theRows(0),
    theSize(point(P::totalDimensionality,P::totalDimensionality)) {

    initMem();
    copy(other);
  }

  template <class T,class P>
  hMatrix<T,P>::~hMatrix() {
    delete[] theRows;
    theRows = 0;
    delete[] theElements;
    theElements = 0;
  }

  template <class T,class P>
  void hMatrix<T,P>::initMem() {
    delete[] theElements;
    theElements = new T[P::totalDimensionality*P::totalDimensionality];
    delete[] theRows;
    theRows = new T*[P::totalDimensionality];

    int j = 0;
    for (int i=0;i<P::totalDimensionality;++i,j+=P::totalDimensionality) {
      theRows[i] = &theElements[j];
    }
    postElement = &theElements[j];
  }

  template <class T,class P>
  void hMatrix<T,P>::clear() {
    T* ptr;
    for (ptr=theRows[0];ptr!=postElement;++ptr) {
      *ptr = T(0);
    }
  }

  template <class T,class P>
  void hMatrix<T,P>::unit() {
    T* ptr;
    int i;
    const int c = theSize.x+1;
    for (ptr=theRows[0],i=0;ptr!=postElement;++ptr,++i) {
      if ((i%c) == 0) {
        *ptr = T(1);
      }
      else {
        *ptr = T(0);
      }
    }
  }

  template <class T,class P>
  const point& hMatrix<T,P>::size() const {
    return theSize;
  }

  template <class T,class P>
  mathObject* hMatrix<T,P>::clone() const {
    return new hMatrix<T,P>(*this);
  }

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::copy(const hMatrix<T,P>& other) {
    memcpy(theRows[0],other[0],theSize.x*theSize.y*sizeof(T));
    return *this;
  }

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::copy(const matrix<T>& other) {
    return castFrom(other);
  }

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::castFrom(const matrix<T>& other) {
    point aSize(min(theSize.x,other.columns()),
                min(theSize.y,other.rows()));

    int j,i;
    for (j=0;j<aSize.y;++j) {
      for (i=0;i<aSize.x;++i) {
        at(j,i) = other.at(j,i);
      }

      // clear the rest if necessary:
      for (;i<theSize.x;++i) {
        at(j,i) = T(0);
      }
    }


    for (;j<theSize.y;++j) {
      for (i=0;i<theSize.x;++i) {
        at(j,i) = T(0);
      }
    }

    return *this;
  }

  template <class T,class P>
  matrix<T>& hMatrix<T,P>::castTo(matrix<T>& result) const {
    result.resize(theSize,T(),false,false);
    result.fill(theRows[0]);
    return result;
  }

  /**
   * write the object in the given ioHandler
   */
  template <class T,class P>
  bool hMatrix<T,P>::write(ioHandler& handler,
                           const bool complete) const {
    bool b(true);

    if (complete) {
      b = b && handler.writeBegin();
    }

    int j,i;
    lti::write(handler,"size",theSize);

    handler.writeBegin();
    handler.writeSymbol(std::string("data"));
    handler.writeKeyValueSeparator();
    handler.writeBegin();

    const int until = theSize.x - 1;

    for (j=0;j<theSize.y;++j) {
      handler.writeBegin();
      for (i=0;i<until;++i) {
        lti::write(handler,at(j,i));
        handler.writeDataSeparator();
      }
      lti::write(handler,at(j,i));
      handler.writeEnd();
      handler.writeEOL();
    }

    handler.writeEnd();
    handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template <class T,class P>
  bool hMatrix<T,P>::read(ioHandler& handler,
                          const bool complete) {
    bool b(true);

    if (complete) {
      b = b && handler.readBegin();
    }

    int j,i;
    point tmp;
    lti::read(handler,"size",tmp);

    if (tmp != theSize) {
      if (complete) {
        handler.readEnd();
      }
      return false;
    }

    handler.readBegin();
    if (handler.trySymbol(std::string("data"))) {
      handler.readKeyValueSeparator();
      handler.readBegin();

      const int until = theSize.x - 1;

      for (j=0;j<theSize.y;++j) {
        handler.readBegin();
        for (i=0;i<until;++i) {
          lti::read(handler,at(j,i));
          handler.readDataSeparator();
        }
        lti::read(handler,at(j,i));
        handler.readEnd();
      }

      handler.readEnd();
    }
    handler.readEnd();

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  };

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::multiply(const hMatrix<T,P>& other) {
    hMatrix<T,P> me(*this);
    return multiply(me,other);
  }

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::leftMultiply(const hMatrix<T,P>& other) {
    hMatrix<T,P> me(*this);
    return multiply(other,me);
  }

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::multiply(const hMatrix<T,P>& a,
                                       const hMatrix<T,P>& b) {
    int i,j,k;
    T sum;

    for (i=0;i<theSize.x;i++) {
      for (j=0;j<theSize.y;j++) {
        sum = T(0);
        for (k=0;k<theSize.x;k++) {
          sum += a.at(j,k)*b.at(k,i);
        }
        at(j,i) = sum;
      }
    }

    return *this;
  }

  template <class T,class P>
  P& hMatrix<T,P>::multiply(const P& other,
                            P& result) const {
    int i,j;
    T sum;
    for (j=0;j<theSize.y;++j) {
      sum = T(0);
      for (i=0;i<theSize.x;++i) {
        sum += at(j,i)*other[i];
      }
      result[j] = sum;
    }
    return result;
  }

  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::invert() {

    matrixInversion<T> inv;
    matrix<T> tmpMat,tmpiMat;
    castTo(tmpMat);
    inv.apply(tmpMat,tmpiMat);
    castFrom(tmpiMat);

    return *this;
  }

  template <class T,class P>
    hMatrix<T,P>& hMatrix<T,P>::invert(const hMatrix<T,P>& other) {

    matrixInversion<T> inv;
    matrix<T> tmpMat,tmpiMat;
    other.castTo(tmpMat);
    inv.apply(tmpMat,tmpiMat);
    castFrom(tmpiMat);
    return *this;
  }


  template <class T,class P>
  hMatrix<T,P>& hMatrix<T,P>::transpose() {
    hMatrix<T,P> me(*this);
    return transpose(me);
  }

  template <class T,class P>
    hMatrix<T,P>& hMatrix<T,P>::transpose(const hMatrix<T,P>& other) {
    int j,i;

    for (j=0;j<theSize.y;++j) {
      for (i=0;i<theSize.x;++i) {
        at(i,j)=other.at(j,i);
      }
    }

    return *this;
  }

  /*
   * Set the scale factor of the transformation
   *
   * This is the element with the greatest indices.  Changing its value from
   * one will imply a scaling of everything, including the translation factors.
   */
  template <class T,class P>
  void hMatrix<T,P>::setScaleFactor(const T& s) {
    at(theSize.y-1,theSize.x-1) = s;
  }

  /*
   * Return the scale factor used in the transformation
   *
   * The scale factor is the element with the greatest indices.  Changing its
   * value from one will imply a scaling of everything, including the
   * translation factors.
   */
  template <class T,class P>
  const T& hMatrix<T,P>::getScaleFactor() const {
    return at(theSize.y-1,theSize.x-1);
  }
  
  /*
   * Multiply the scale factor with this value.
   *
   * The scale factor is the element with the greatest indices.  Changing its
   * value from one will imply a scaling of everything, including the
   * translation factors.
   */
  template <class T,class P>
  void hMatrix<T,P>::scale(const T& s) {
    at(theSize.y-1,theSize.x-1) *= s;
  };
  
  /*
   * Multiply a diagnonal matrix containing "s" in all elements but the last
   * one with this matrix.
   *
   * This corresponds to scaling the rotation sub-matrix, but not the
   * translation components.
   */
  template <class T,class P>
  void hMatrix<T,P>::scaleR(const T& s) {
    hMatrix<T,P> smat; // initialized with unit matrix.
    int i;
    for (i=theSize.x-2;i>=0;--i) {
      smat.at(i,i)=s;
    }
    leftMultiply(smat);
  };

  /*
   * This function sets the translation vector with the given point,
   * i.e. the given homogeneous point will be normalized and its components
   * (except the h component) will be used as the translation vector.
   *
   * The translation vector corresponds to the all elements of the last column
   * except the last one, which is the scale factor.
   */
  template <class T,class P>
  void hMatrix<T,P>::setTranslation(const P& thePoint) {
    static const int lastx = theSize.x-1;
    static const int lasty = theSize.y-1;
    T h = thePoint.h;
    int j;
    for (j=0;j<lasty;++j) {
      at(j,lastx) = thePoint[j]/h;
    }
  };

  /*
   * Return a non-homegeneous point with the actual translation vector.
   *
   * The translation vector corresponds to the all elements of the last column
   * except the last one, which is the scale factor.
   */
  template <class T,class P>
  P hMatrix<T,P>::getTranslation() const {
    P tmp;
    static const int lastx = theSize.x-1;
    static const int lasty = theSize.y-1;
    int j;
    for (j=0;j<lasty;++j) {
      tmp[j] = at(j,lastx);
    }
    return tmp;
  };


  /*
   * Add the normalized homogeneous vector to the one in the matrix
   */
  template <class T,class P>
  void hMatrix<T,P>::translate(const P& thePoint) {
    static const int lastx = theSize.x-1;
    static const int lasty = theSize.y-1;
    T h = thePoint.h;
    int j;
    for (j=0;j<lasty;++j) {
      at(j,lastx) += (thePoint[j]/h);
    }
  };

  /*
   * Set the rotation matrix with a new rotation matrix generated
   * from the given %parameters.
   * @param angle the rotation angle in radians.
   * @param axis this vector is the rotation-axis.
   * @param center the contents of this point will be normalized to get the
   *               center of the rotation.
   */
  template <class T,class P>
  void hMatrix<T,P>::setRotation(const double& angle,
                                 const hPoint3D<T>& axis,
                                 const hPoint3D<T>& center) {

    int off = -1;
    T tx,ty,tz;

    tx = center.x;
    ty = center.y;
    tz = center.z;


    // the wished rotation angle
    
    float ctmp, stmp;
    sincos(static_cast<float>(angle), stmp, ctmp);

    const T sina = static_cast<T>(stmp);
    const T cosa = static_cast<T>(ctmp);

    const T sqrXZ = axis.x*axis.x + axis.z*axis.z; 
    const T sqrXYZ = sqrXZ + axis.y*axis.y;
    const T absXZ = sqrt(sqrXZ);

    // psi (or here just p) is the rotation angle on y
    T sinp,cosp;
    if (absXZ == 0) {
      sinp = T(0);
      cosp = T(1);
    }
    else {
      sinp = axis.x/absXZ;
      cosp = axis.z/absXZ;
    }

    // theta is the rotation angle on x
    const T sint = axis.y/sqrt(sqrXYZ);
    const T cost = sqrt(sqrXZ/sqrXYZ);

    // the following code was partially automatic generated, and can
    // still be optimized:

    // temporary data
    const T cosp2 = cosp*cosp;
    const T cost2 = cost*cost;
    const T sint2 = static_cast<T>(1) - cost2;
    const T t7 = cosp2*cosa*cost2;
    const T t8 = cosa*cost2;
    const T t9 = cosp2*cost2;
    const T t11 = cost*cosp;
    const T t13 = t11*sina;
    const T t15 = cost*sinp;
    const T t17 = sint*cosa;
    const T t18 = t15*t17;
    const T t19 = t15*sint;
    const T t21 = cosp*sinp;
    const T t22 = t21*t8;
    const T t23 = sint*sina;
    const T t24 = t21*cost2;
    const T t26 = tx*cosp2;
    const T t29 = cosp*cost2;
    const T t31 = cost*ty;
    const T t32 = sinp*sint;
    const T t34 = t32*cosa;
    const T t38 = sinp*cosa*cost2;
    const T t40 = tx*cosa;
    const T t41 = cosp*sina;
    const T t48 = t26*(cost2-t8) + t31*(t32 - t34 + t41) + 
                  tz*(sinp*t29 - cosp*t38 - sint*sina) +
                  (tx - t40)*sint2;
    const T t51 = t15*sina;
    const T t52 = t11*t17;
    const T t53 = t11*sint;
    const T t55 = cost*tx;
    const T t61 = cost*tz;
    const T t62 = sinp*sina;
    const T t64 = sint*cosp;
    const T t65 = t64*cosa;
    const T t80 = cosp2*tz;
    
    // Rotation matrix:
    at(0,0) = t7+cosa-t8+cost2-t9;
    at(0,1) = -t13+t18-t19;

    at(1,0) = t13+t18-t19;
    at(1,1) = t8+sint2;

    // if a 3D matrix:
    if (theSize.x>=4) {
      at(0,2) = t22+t23-t24;
      at(1,2) = t51-t52+t53;
      at(2,0) = t22-t23-t24;
      at(2,1) = -t51-t52+t53;
      at(2,2) = -t7+cosa+t9;
      off = 0;
    }

    // translation:
    at(0,3+off) = t48;
    at(1,3+off) = (t55*(t32-t41-t34)
                   +ty*cost2*(static_cast<T>(1)-cosa)
                   +t61*(t65-t62-t64));
                     
    // if a 3D matrix:
    if (theSize.x>=4) {
      at(2,3) = (tx*(sinp*t29 + sint*sina - cosp*t38)
                 +t31*(t65+t62-t64)
                 +t80*(t8-cost2)
                 +tz-tz*cosa);
      at(3,2) = 0;
    }
    
    at(3+off,0) = 0;
    at(3+off,1) = 0;
    at(3+off,3+off) = T(1);
    
  }

  /*
   * Left Multiply the matrix with a new rotation matrix generated
   * from the given %parameters.
   * @param angle the rotation angle in radians.
   * @param axis this vector is the rotation-axis.
   * @param center the contents of this point will be normalized to get the
   *               center of the rotation.
   */
  template <class T,class P>
  void hMatrix<T,P>::rotate(const double& angle,
                            const hPoint3D<T>& axis,
                            const hPoint3D<T>& center) {
    
    hMatrix<T,P> tmp;
    tmp.setRotation(angle,axis,center);
    tmp.multiply(*this);
    copy(tmp);
  }

  template <class T,class P>
  void hMatrix<T,P>::setSimilarityTransform(const tpoint<T>& t,
                                            const T& angle,
                                            const T& scaling) {
    unit();
    T sina, cosa;
    sincos(angle, sina, cosa);
    at(0,0) = scaling*cosa;
    at(1,0) = scaling*sina;
    at(0,1) =-at(1,0);
    at(1,1) = at(0,0);
    
    at(0,theSize.x-1) = t.x;
    at(1,theSize.x-1) = t.y;
  }

  // -----------------------------------------
  // hMatrix2D
  // -----------------------------------------
  /*
   * constructor
   */
  template <class T>
  hMatrix2D<T>::hMatrix2D()
    : hMatrix<T,hPoint2D<T> >() {
  }

  /*
   * copy constructor
   */
  template <class T>
  hMatrix2D<T>::hMatrix2D(const hMatrix2D<T>& other)
    : hMatrix<T,hPoint2D<T> >(other) {
  }

  /*
   * copy constructor
   */
  template <class T>
  hMatrix2D<T>::hMatrix2D(const matrix<T>& other)
    : hMatrix<T,hPoint2D<T> >(other) {
  }

  /*
   * destructor
   */
  template <class T>
  hMatrix2D<T>::~hMatrix2D() {
  }

  /*
   * multiply with a point and leave the result in the second
   * parameters.
   * Return a reference to the second parameters
   */
  template <class T>
  tpoint<T>& hMatrix2D<T>::multiply(const tpoint<T>& other,
                                    tpoint<T>& result) const {

    hPoint2D<T> tmp(other);
    hPoint2D<T> r;
    hMatrix<T,hPoint2D<T> >::multiply(tmp,r);
    r.castTo(result);

    return result;
  }

  // -----------------------------------------
  // hMatrix3D
  // -----------------------------------------
  /*
   * constructor
   */
  template <class T>
  hMatrix3D<T>::hMatrix3D()
    : hMatrix<T,hPoint3D<T> >() {
  }

  /*
   * copy constructor
   */
  template <class T>
  hMatrix3D<T>::hMatrix3D(const hMatrix3D<T>& other)
    : hMatrix<T,hPoint3D<T> >(other) {
  }

  template <class T>
  hMatrix3D<T>::hMatrix3D(const hMatrix2D<T>& other)
    : hMatrix<T,hPoint3D<T> >() {
    // rotation submatrix
    this->at(0,0)=other.at(0,0);
    this->at(0,1)=other.at(0,1);
    this->at(1,0)=other.at(1,0);
    this->at(1,1)=other.at(1,1);
    // translation
    this->at(0,3)=other.at(0,2);
    this->at(1,3)=other.at(1,2);
    // perspective distortion
    this->at(3,0)=other.at(2,0);
    this->at(3,1)=other.at(2,1);
    // scaling factor
    this->at(3,3)=other.at(2,2);
  }

  /*
   * copy constructor
   */
  template <class T>
  hMatrix3D<T>::hMatrix3D(const matrix<T>& other)
    : hMatrix<T,hPoint3D<T> >(other) {
  }

  /*
   * destructor
   */
  template <class T>
  hMatrix3D<T>::~hMatrix3D() {
  }

  /*
   * multiply with a point and leave the result in the second
   * parameters.
   * Return a reference to the second parameters
   */
  template <class T>
  tpoint3D<T>& hMatrix3D<T>::multiply(const tpoint3D<T>& other,
                                      tpoint3D<T>& result) const {

    hPoint3D<T> tmp(other);
    hPoint3D<T> r;
    hMatrix<T,hPoint3D<T> >::multiply(tmp,r);
    r.castTo(result);

    return result;
  }

  /*
   * multiply with a point and leave the result in the second
   * parameters.
   * Return a reference to the second parameters
   */
  template <class T>
  hPoint2D<T>& hMatrix3D<T>::multiply(const hPoint2D<T>& other,
                                            hPoint2D<T>& result) const {

    hPoint3D<T> tmp(other);
    hPoint3D<T> r;
    hMatrix<T,hPoint3D<T> >::multiply(tmp,r);
    r.project(result);

    return result;
  }

  /*
   * multiply with a point and leave the result in the second
   * parameters.
   * Return a reference to the second parameters
   */
  template <class T>
  tpoint<T>& hMatrix3D<T>::multiply(const tpoint<T>& other,
                                            tpoint<T>& result) const {

    hPoint3D<T> tmp(other);
    hPoint3D<T> r;
    hMatrix<T,hPoint3D<T> >::multiply(tmp,r);
    r.project(result);

    return result;
  }

}

