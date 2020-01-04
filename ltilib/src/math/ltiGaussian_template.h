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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiGaussian.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 18.3.2002
 * revisions ..: $Id: ltiGaussian_template.h,v 1.7 2004/05/06 10:31:26 alvarado Exp $
 */

#include "ltiGaussian.h"
#include "ltiMatrixInversion.h"
#include "ltiMatrixDecomposition.h"

namespace lti {
  // --------------------------------------------------
  // gaussian
  // --------------------------------------------------

  // default constructor
  template<class T>
  gaussian<T>::gaussian()
    : mathFunction(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  template<class T>
  gaussian<T>::gaussian(const T mean, const T variance)
    : mathFunction() {
    parameters defPar;
    defPar.setMeanAndVariance(mean,variance);
    setParameters(defPar);
  }

  template<class T>
  gaussian<T>::gaussian(const vector<T>& mean, const matrix<T>& covar)
    : mathFunction() {
    parameters defPar;
    defPar.setMeanAndCovariance(mean,covar);
    setParameters(defPar);
  }

  // copy constructor
  template<class T>
  gaussian<T>::gaussian(const gaussian<T>& other)
    : mathFunction() {
    copy(other);
  }

  // destructor
  template<class T>
  gaussian<T>::~gaussian() {
  }

  // returns the name of this type
  template<class T>
  const char* gaussian<T>::getTypeName() const {
    return "gaussian";
  }

  // copy member
  template<class T>
  gaussian<T>&
    gaussian<T>::copy(const gaussian<T>& other) {
      mathFunction::copy(other);


    return (*this);
  }

  // alias for copy member
  template<class T>
  gaussian<T>&
    gaussian<T>::operator=(const gaussian<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* gaussian<T>::clone() const {
    return new gaussian<T>(*this);
  }

  // return parameters
  template<class T>
  const typename gaussian<T>::parameters&
  gaussian<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  /*
   * set functor's parameters.
   * This member makes a copy of <em>theParam</em>: the functor
   * will keep its own copy of the parameters!
   * @return true if successful, false otherwise
   */
  template<class T>
  bool gaussian<T>::updateParameters() {
    const parameters& param = getParameters();

    matrixInversion<T> inverter;
    luDecomposition<T> det;

    if (param.covariance.rows() != param.covariance.columns()) {
      setStatusString("Covariance matrix must be a square matrix");
      return false;
    }

    if (!inverter.apply(param.covariance,invCovariance)) {
      setStatusString("Covariance matrix cannot be inverted");
      return false;
    };

    meanVct.copy(param.mean);
    norm = static_cast<T>(1.0/(sqrt(det.det(param.covariance))*
                          pow(2.0*Pi,double(meanVct.size())/2.0)));

    mean = meanVct.at(0);
    invVarianceH = static_cast<T>(-2)*invCovariance.at(0);

    return (meanVct.size() == invCovariance.rows());
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type vector<T>!
  template<class T>
  bool gaussian<T>::apply(const vector<T>& src,T& dest) const {
    if (src.size() != meanVct.size()) {
      setStatusString("Wrong vector dimensions");
      return false;
    }

    vector<T> d,t;
    d.subtract(src,meanVct);
    invCovariance.multiply(d,t);
    dest = static_cast<T>(norm*exp(d.dot(t)/(-2.0)));
    return true;
  };

  /*
   * alias for apply
   * @param src vector<T> with the source data.
   * @return function value or negative if error occurred
   */
  template<class T>
  T gaussian<T>::operator()(const vector<T>& vct) {
    if (vct.size() != meanVct.size()) {
      setStatusString("Wrong vector dimensions");
      return T(-1);
    }

    vector<T> d,t;
    d.subtract(vct,meanVct);
    invCovariance.multiply(d,t);
    return static_cast<T>(norm*exp(d.dot(t)/(-2.0)));
  }

  // On copy apply for type vector<T>!
  template<class T>
  bool gaussian<T>::apply(const T src,T& dest) const {
    dest = norm*exp((src-mean)*(src-mean)*invVarianceH);
    return true;
  };

  /*
   * alias for apply
   * @param src vector<T> with the source data.
   * @return function value or negative if error occurred
   */
  template<class T>
  T gaussian<T>::operator()(const T src) {
    return norm*exp((src-mean)*(src-mean)*invVarianceH);
  }

  /*
   * shortcut to set the mean vector and covariance matrix
   */
  template<class T>
  void gaussian<T>::setMeanAndCovariance(const vector<T>& mean,
                                         const matrix<T>& variance) {
    parameters p(getParameters());
    p.setMeanAndCovariance(mean,variance);
    setParameters(p); // recompute normalization constant and matrix inverses
  }

  /*
   * shortcut to set a one dimensional gaussian with the mean value and
   * the variance.
   */
  template<class T>
  void gaussian<T>::setMeanAndVariance(const T mean,
                                       const T variance) {
    parameters p(getParameters());
    p.setMeanAndVariance(mean,variance);
    setParameters(p); // recompute normalization constant and matrix inverse
  }

}

