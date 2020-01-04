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
 * file .......: ltiBinaryKernels_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 15.08.00
 * revisions ..: $Id: ltiBinaryKernels_template.h,v 1.9 2005/10/25 11:25:51 doerfler Exp $
 */

#include "ltiMath.h"

namespace lti {

  // 2D filter kernel

  // constructor
  template<class T>
    cityBlockKernel<T>::cityBlockKernel(const int& size, const T& on)
    : kernel2D<T>() {
    generate(size,on);
  }

  // generate filter
  template<class T>
    void  cityBlockKernel<T>::generate(const int& size, const T& on) {

    int realSize = size;

    if ((realSize % 2) == 0) {
      realSize++;
    }

    if (realSize < 3) {
      realSize = 3;
    }

    int border = realSize/2;

    this->resize(-border,-border,border,border,0,false,true);

    for (int i=-border;i<=0;i++) {
      for (int j=-border-i;j<=border+i;j++) {
        this->at(i,j) = on;
        this->at(-i,j) = on;
      }
    }

    setNorm(typeInfo<T>::suggestedNorm());
  }

  // constructor
  template<class T>
    chessBoardKernel<T>::chessBoardKernel(const int& size, const T& on)
    : sepKernel<T>() {
    generate(size,on);
  }

  // generate filter
  template<class T>
    void  chessBoardKernel<T>::generate(const int& size, const T& on) {

    int realSize = size;

    if ((realSize % 2) == 0) {
      realSize++;
    }

    if (realSize < 3) {
      realSize = 3;
    }

    int border = realSize/2;

    kernel1D<T> oneD(-border,border,on);

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(oneD);
    this->getColFilter(0).copy(oneD);

    setNorm(typeInfo<T>::suggestedNorm());
  }

  // constructor
  template<class T>
    octagonalKernel<T>::octagonalKernel(const int& size, const T& on)
    : kernel2D<T>() {
    generate(size,on);
  }

  // generate filter
  template<class T>
    void  octagonalKernel<T>::generate(const int& size, const T& on) {

    int realSize = size;

    if ((realSize % 2) == 0) {
      realSize++;
    }

    if (realSize < 3) {
      realSize = 3;
    }

    int border = realSize/2;

    resize(-border,-border,border,border,on,false,true);

    int cant = (border-1)/2;

    for (int i=0;i<=cant;i++) {
      for (int j=0;j<=cant-i;j++) {
        this->at(i-border,j-border) = static_cast<T>(0);
        this->at(border-i,j-border) = static_cast<T>(0);
        this->at(border-i,border-j) = static_cast<T>(0);
        this->at(i-border,border-j) = static_cast<T>(0);
      }
    }

    setNorm(typeInfo<T>::suggestedNorm());
  }

  // constructor
  template<class T>
    euclideanKernel<T>::euclideanKernel(const int& size, const T& on)
    : kernel2D<T>() {
    generate(size,on);
  }

  // generate filter
  template<class T>
    void  euclideanKernel<T>::generate(const int& size, const T& on) {

    int realSize = size;

    if ((realSize % 2) == 0) {
      realSize++;
    }

    if (realSize < 3) {
      realSize = 3;
    }

    int border = realSize/2;

    resize(-border,-border,border,border,static_cast<T>(0),false,true);

    for (int i=border;i>=0;i--) {
      int e;
      e=int(sqrt(border*border-i*i));
      for (int j=-e;j<=e;j++) {
        this->at(i,j) = on;
        this->at(-i,j) = on;
      }
    }

    setNorm(typeInfo<T>::suggestedNorm());
  }


}

