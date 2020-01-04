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
 * file .......: ltiOgdKernels_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiOgdKernels_template.h,v 1.12 2005/10/28 15:56:42 doerfler Exp $
 */

#include "ltiMath.h"
#include "ltiGaussKernels.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"

namespace lti {
  // first order ogd

  // default constructor
  template<class T>
    ogd1Kernel<T>::ogd1Kernel() : sepKernel<T>() {
  }

  // constructor
  template<class T>
    ogd1Kernel<T>::ogd1Kernel(const int& size,
                              const double& theVariance,
                              const double& theAngle)
    : sepKernel<T>() {
    generate(size,theVariance,theAngle);
  }

  // generate filter
  template<class T>
  void  ogd1Kernel<T>::generate(const int& size,
                                const double& theVariance,
                                const double& theAngle) {

    gaussKernel1D<T> gauss(size,theVariance);

    this->setNumberOfPairs(2);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);
    this->getRowFilter(1).copy(gauss);
    this->getColFilter(1).copy(gauss);

    int x;
    double cost,sint;

    sincos(theAngle, sint, cost);

    for (x  = gauss.firstIdx();
         x <= gauss.lastIdx();
         x++) {
      this->getRowFilter(0).at(x) *= static_cast<T>(-x/theVariance);
      this->getColFilter(0).at(x) *= static_cast<T>(cost);
      this->getRowFilter(1).at(x) *= static_cast<T>(sint);
      this->getColFilter(1).at(x) *= static_cast<T>(-x/theVariance);
    }
  }

  template<class T>
  void  ogd1Kernel<T>::generateBasisKernel(const int& whichBasis,
                                           const int& size,
                                           const double& theVariance) {

    gaussKernel1D<T> gauss(size,theVariance);

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);

    int x;
    kernel1D<T>* f = 0;

    if (whichBasis == 0) {
      f = &this->getRowFilter(0);
    } else {
      f = &this->getColFilter(0);
    }

    const double actualVariance=gauss.getActualVariance();
    for (x  = gauss.firstIdx();
         x <= gauss.lastIdx();
         x++) {
      f->at(x) *= static_cast<T>(-x/actualVariance);
    }
  }

  // second order filter kernel

  // constructor
  template<class T>
  ogd2Kernel<T>::ogd2Kernel()
    : sepKernel<T>() {
  }

  template<class T>
  ogd2Kernel<T>::ogd2Kernel(const int& size,
                            const double& theVariance,
                            const double& theAngle)
    : sepKernel<T>() {
    generate(size,theVariance,theAngle);
  }

  // generate filter
  template<class T>
  void  ogd2Kernel<T>::generate(const int& size,
                                const double& theVariance,
                                const double& theAngle) {

    gaussKernel1D<T> gauss(size,theVariance);

    this->setNumberOfPairs(3);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);
    this->getRowFilter(1).copy(gauss);
    this->getColFilter(1).copy(gauss);
    this->getRowFilter(2).copy(gauss);
    this->getColFilter(2).copy(gauss);

    int x;
    double xx;
    double sina, cosa;
    sincos(theAngle, sina, cosa);
    const double cost=cosa;
    const double sint=sina;
    const double cost2=sqr(cost);
    const double sint2=sqr(sint);
    const double costw=2*cost;
    const double var2 = sqr(theVariance);

    for (x  = gauss.firstIdx();
         x <= gauss.lastIdx();
         x++) {
      xx = sqr(double(x));
      this->getRowFilter(0).at(x) *= static_cast<T>((xx-theVariance)/var2);
      this->getColFilter(1).at(x) *= static_cast<T>(cost2);

      this->getRowFilter(1).at(x) *= static_cast<T>(sint2);
      this->getColFilter(1).at(x) *= static_cast<T>((xx-theVariance)/var2);

      this->getRowFilter(2).at(x) *= static_cast<T>(costw*x/theVariance);
      this->getColFilter(2).at(x) *= static_cast<T>(sint*x/theVariance);
    }
  }

  template<class T>
  void  ogd2Kernel<T>::generateBasisKernel(const int& basis,
                                           const int& size,
                                           const double& theVariance) {
    gaussKernel1D<T> gauss(size,theVariance);
    const double actualVariance=gauss.getActualVariance();
    _lti_debug4("Actual variance: " << actualVariance<<"\n");

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(gauss);
    this->getColFilter(0).copy(gauss);

    int x;
    kernel1D<T>* f = 0;

    if (basis == 0) {
      f = &this->getRowFilter(0);
    } else if (basis == 1) {
      f = &this->getColFilter(0);
    }

    if (basis < 2) {

      const double varSqr=actualVariance*actualVariance;
      float ksum=0.f;
      for (x  = gauss.firstIdx();
           x <= gauss.lastIdx();
           x++) {
        f->at(x) *= static_cast<T>(((x*x)-actualVariance)/varSqr);
        ksum+=f->at(x);
      }
      ksum/=size;
      for (x  = gauss.firstIdx();
           x <= gauss.lastIdx();
           x++) {
        f->at(x) -= ksum;
      }
      _lti_debug4("kernel sum equal= " << f->sumOfElements() <<"\n");

    } else {
      for (x = gauss.firstIdx();
           x <= gauss.lastIdx();
           x++) {
        this->getRowFilter(0).at(x) *= static_cast<T>(x/actualVariance);
        this->getColFilter(0).at(x) *= static_cast<T>(x/actualVariance);
      }
        _lti_debug4("kernel sum XY= " << this->getRowFilter(0).sumOfElements() <<"\n");
    }
  }
}

#include "ltiUndebug.h"
