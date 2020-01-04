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
 * file .......: ltiGaussKernels_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiGaussKernels_template.h,v 1.11 2005/04/27 15:14:30 doerfler Exp $
 */

#include "ltiMath.h"

namespace lti {
  // 1D filter kernel

  // constructor
  template<class T>
    gaussKernel1D<T>::gaussKernel1D(const int& size,
                                    const double& theVariance)
    : kernel1D<T>() {

    generate(size,theVariance);
  }

  // generate filter
  template<class T>
    void  gaussKernel1D<T>::generate(const int& size,
                                     const double& theVariance) {

    double var = theVariance;

    // calculate the variance such that the element at (size/2) is
    // the element at 0 divided by (1+int(size/2))
    if (theVariance<0) {
      int n = (size/2);
      if (n<1) {
        n = 1;
      }

      // log == ln (natural logarithm)
      var = (n*n)/(2*log(static_cast<double>(n+1)));
    }

    int i;
    this->norm = typeInfo<T>::suggestedNorm();
    double snorm = double(this->norm);

    double accu,factor;
    variance = var;  // initialize member variance
    resize(-(size/2),size-(size/2)-1,T(0),false,false);
    factor = 1.0/sqrt(2*Pi*variance);

    accu=0;
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(snorm*factor*exp(-double(i*i)/(2*variance)));
      accu+=this->at(i);
    }
    accu /= snorm;
    // make sure the sum of all elements is one!
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(this->at(i)/accu);
    }

  }

  template<class T>
  const double& gaussKernel1D<T>::getActualVariance() const {
    return variance;
  }


  ///////////////////
  // 2D filter kernel
  ///////////////////

  // constructor
  template<class T>
    gaussKernel2D<T>::gaussKernel2D(const int& size,
                                    const double& theVariance)
    : sepKernel<T>() {
    generate(size,theVariance);
  }

  // generate filter
  template<class T>
  void gaussKernel2D<T>::generate(const int& size,
                                  const double& theVariance) {

    gaussKernel1D<T> oneD(size,theVariance);

    this->setNumberOfPairs(1);

    this->getRowFilter(0).copy(oneD);
    this->getColFilter(0).copy(oneD);
  }

  template<class T>
  const double& gaussKernel2D<T>::getActualVariance() const {
    return variance;
  }

}

