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
 * file .......: ltiGaborKernels_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiGaborKernels_template.h,v 1.9 2005/10/28 15:56:42 doerfler Exp $
 */

#include "ltiMath.h"
#include "ltiGaussKernels.h"

namespace lti {
  // 2D filter kernel

  // constructor
  template<class T>
  gaborKernel<T>::gaborKernel(const int& size,
                              const double& theVariance,
                              const double& frequency,
                              const double& angle) : kernel2D<T>() {
    generate(size,theVariance,frequency,angle);
  }

  // generate filter
  template<class T>
  void  gaborKernel<T>::generate(const int& size,
                                 const double& theVariance,
                                 const double& frequency,
                                 const double& angle) {

    gaussKernel2D<T> gauss(size,theVariance);

    outerProduct(gauss.getRowFilter(0),gauss.getColFilter(0));

    point p;
    double cosa, sina;
    sincos(angle, sina, cosa);

    const double cost=frequency*cosa;
    const double sint=frequency*sina;

    for (p.y=this->firstRow();p.y<=this->lastRow();p.y++) {
      for (p.x=this->firstColumn();p.x<=this->lastColumn();p.x++) {
        this->at(p)=static_cast<T>(this->at(p)*cos(cost*p.x+sint*p.y));
      }
    }
  }

  // Separable filter kernel

  // constructor
  template<class T>
  gaborKernelSep<T>::gaborKernelSep(const int& size,
                                    const double& theVariance,
                                      const double& frequency,
                                      const double& angle)
    : sepKernel<T>() {
    generate(size,theVariance,frequency,angle);
  }

  // generate filter
  template<class T>
    void  gaborKernelSep<T>::generate(const int& size,
                                     const double& theVariance,
                                     const double& frequency,
                                     const double& angle) {

    gaussKernel1D<T> gauss(size,theVariance);

    this->setNumberOfPairs(2);

    this->getRowFilter(0).copy(gauss);
    this->getRowFilter(1).copy(gauss);

    this->getColFilter(0).copy(gauss);
    this->getColFilter(1).copy(gauss);

    int x;
    double cosa, sina;
    sincos(angle, sina, cosa);
    const double cost=frequency*cosa;
    const double sint=frequency*sina;

    for (x  = gauss.firstIdx(); x <= gauss.lastIdx(); x++) {
      sincos(cost*x, sina, cosa);
      this->getRowFilter(0).at(x) *= static_cast<T>(cosa);
      this->getRowFilter(1).at(x) *= static_cast<T>(sina);
      sincos(sint*x, sina, cosa);
      this->getColFilter(0).at(x) *= static_cast<T>(cosa);
      this->getColFilter(1).at(x) *= static_cast<T>(sina);
    }
  }
}

