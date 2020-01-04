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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltivarianceFunctor.cpp
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiVarianceFunctor_template.h,v 1.29 2004/10/18 12:13:12 zieren Exp $
 */

#include "ltiVarianceFunctor.h"
#include "ltiGenericVector.h"
#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // varianceFunctor::parameters
  // --------------------------------------------------

  // implementation in .h file due to a MS-VC bug

  // --------------------------------------------------
  // varianceFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  varianceFunctor<T>::varianceFunctor()
    : statisticsFunctor(){
    parameters tmp;
    setParameters(tmp);
  }

  // copy constructor
  template <class T>
  varianceFunctor<T>::varianceFunctor(const varianceFunctor& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  varianceFunctor<T>::~varianceFunctor() {
  }

  // returns the name of this type
  template <class T> const char* varianceFunctor<T>::getTypeName() const {
    return "varianceFunctor";
  }

  // copy member
  template <class T>
  varianceFunctor<T>&
  varianceFunctor<T>::copy(const varianceFunctor<T>& other) {
    statisticsFunctor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* varianceFunctor<T>::clone() const {
    return new varianceFunctor<T>(*this);
  }

  // return parameters
  template <class T>
  const typename varianceFunctor<T>::parameters&
  varianceFunctor<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type vector<T>!
  template <class T>
  T varianceFunctor<T>::apply(const vector<T>& src) const {
    T sum;
    return apply(src,sum);
  };

  // On copy apply for type vector<T>!
  template <class T>
  T& varianceFunctor<T>::apply(const vector<T>& src, T& dest) const {
    dest=T(0);
    
    T n=static_cast<T>(src.size());
    if (n<2) return dest;

    typename vector<T>::const_iterator i=src.begin();
    typename vector<T>::const_iterator e=src.end();
    
    T mu=T(0);
    for (; i!=e; ++i) {
      dest+=(*i)*(*i);
      mu+=*i;
    }
    mu*=mu;
    mu/=n;
    dest-=mu;

    if (getParameters().type == parameters::Empirical) {
      dest/=n-1;
    } else {
      dest/=n;
    }

    return dest;
  };

  // On copy apply for type matrix<T>!
  template <class T>
  vector<T>& varianceFunctor<T>::apply(const matrix<T>& src,vector<T>& dest) const {
    vector<T> tmp;
    if (getParameters().rowWise) {
      varianceOfRows(src,dest);
    } else {
      varianceOfColumns(src,dest);
    }
    return dest;
  };

  template <class T>
  void varianceFunctor<T>::varianceOfRows(const matrix<T>& src, vector<T>& dest) const {

    dest.resize(src.columns(),T(0),false,true);

    if (src.rows()<2) return;

    vector<T> sum(src.columns(),T(0));
    vector<T> tmp;

    // compute the variance of the rows of src.
    // var=1/k \sum_i (x_i-\mu)^2
    // this is equal to 1/k \sum_i x_i^2 - s^2/n
    // where s = \sum_i x_i and n = number of rows
    // k is either n or n-1, depending on the type of variance
    for (int i=0; i<src.rows(); i++) {
      src.getRowCopy(i,tmp);
      sum.add(tmp);
      tmp.emultiply(tmp);
      dest.add(tmp);
    }
    sum.emultiply(sum);
    sum.divide(static_cast<T>(src.rows()));
    dest.subtract(sum);
    if (getParameters().type == parameters::Empirical) {
      dest.divide(static_cast<T>(src.lastRow()));
    } else {
      dest.divide(static_cast<T>(src.rows()));
    }
  }

  template <class T>
  void varianceFunctor<T>::varianceOfColumns(const matrix<T>& src,
                                             vector<T>& dest) const {
    dest.resize(src.rows(),T(0),false,true);

    if (src.columns()<2) return;

    vector<T> sum(src.rows(),T(0));
    vector<T> tmp;

    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      sum.add(tmp);
      tmp.emultiply(tmp);
      dest.add(tmp);
    }
    sum.emultiply(sum);
    sum.divide(static_cast<T>(src.columns()));
    dest.subtract(sum);
    if (getParameters().type == parameters::Empirical) {
      dest.divide(static_cast<T>(src.lastColumn()));
    } else {
      dest.divide(static_cast<T>(src.columns()));
    }
  }


  template <class T>
  T varianceFunctor<T>::covariance(const vector<T>& a,
                                   const vector<T>& b) const {
    typename vector<T>::const_iterator ia=a.begin();
    typename vector<T>::const_iterator ib=b.begin();

    assert(a.size() == b.size());
    T n=T(a.size());
    T sx=T(0);
    T sy=T(0);

    T sxy=T(0);
    for ( ; ia != a.end(); ++ia, ++ib) {
      sxy+=(*ia)*(*ib);
      sx+=*ia;
      sy+=*ib;
    }
    sxy-=sx*sy/n;
    if (getParameters().type == parameters::Empirical) {
      return sxy/(n-1);
    } else {
      return sxy/n;
    }

  }

  /*
   * This is an old, slower version:
   */
  template <class T>
  void varianceFunctor<T>::slowCovarianceMatrixOfRows(const matrix<T>& src,
                                                  matrix<T>& dest) const {
    vector<T> mu;
    // dimension of points
    int d=src.columns();
    // number of points
    int n=src.rows();
    int i;

    matrix<T> tmp=src;

    // calculate mean vector
    mean.meanOfRows(src,mu);
    for (i=0; i<n; i++) {
      // compute x_ij-b_j
      tmp.getRow(i).subtract(mu);
    }
    // now in tmp x_ij is replaced by x_ij-mu_j

    // this matrix is supposed to keep (x_i-mu)(x_i-mu)^T
    matrix<T> tmpProd(d,d);

    dest.resize(d,d,T(0),false,true);
    if (n > 1) {
      for (i=0; i<n; i++) {
        vector<T> tmpV;
        tmpV=tmp.getRow(i);
        tmpProd.outerProduct(tmpV,tmpV);
        dest.add(tmpProd);
      }
      if (getParameters().type == parameters::Empirical) {
        dest.divide(static_cast<T>(n-1));
      } else {
        dest.divide(static_cast<T>(n));
      }
    }
    if (getParameters().correlation) {
      covar2corrcoef(dest);
    }
  }

  /*
   * This is a new, faster version, written by Gu Xin:
   */
  template<class T>
  void varianceFunctor<T>::covarianceMatrixOfRows(const matrix<T>& src,
                                                  matrix<T>& dest) const {

    vector<T> mu;
    covarianceMatrixOfRows(src,dest,mu);
  }


  /*
   * This is a new, faster version, written by Gu Xin:
   */
  template<class T>
  void varianceFunctor<T>::covarianceMatrixOfRows(const matrix<T>& src,
                                                  matrix<T>& dest,
                                                  vector<T>& mu) const {

    // no data?
    if (src.empty()) {
      dest.clear();
      return;
    }

    //Implementation for the sum of Matrix[X];

    int n=src.rows();       //get the number of rows from Matrix[src];
    int d=src.columns();    //get the number of columns from Matrix[src];

    dest.resize(d,d,T(0),false,true);

    if (n<2) {  //just one row
      return;
    }

    matrix<T> tmp(d,d);
    _lti_debug("cv1");

    // mean vector
    mu.resize(d,T(0));

    /*
     * the loop gets out the result of the Matrix[X];
     * Matrix[X]=sum(Matrix[X(i)]);-- i from 0 to n-1--
     */
    for(int i=0;i<n;i++){
      const vector<T>& tmpV=src.getRow(i);
      mu.add(tmpV);
      tmp.outerProduct(tmpV,tmpV);
      dest.add(tmp);          //add the Matrix[X(i)] to Matrix[sumMatrix];
    }
    //End of the Implementation for the sum of Matrix[X];

    _lti_debug("cv2");
    //Implementation for the Matrix[meanOf];
    mu.divide(static_cast<T>(n));

    _lti_debug("cv3");
    tmp.outerProduct(mu,mu);
    tmp.multiply(static_cast<T>(n));
    _lti_debug("cv4");

    //get the result of the difference from Matrix[X] and Matrix[meanOf];
    dest.subtract(tmp);
    _lti_debug("cv5");
    if (getParameters().type == parameters::Empirical) {
      dest.divide(static_cast<T>(n-1));
    } else {
      dest.divide(static_cast<T>(n));
    }
    _lti_debug("cv6");
    if (getParameters().correlation) {
      covar2corrcoef(dest);
    }
    _lti_debug("cv7");
  }


  template <class T>
  void varianceFunctor<T>::covarianceMatrixOfColumns(const matrix<T>& src,
                                                     matrix<T>& dest) const {

    vector<T> mu;
    covarianceMatrixOfColumns(src,dest,mu);
  }


  template <class T>
  void varianceFunctor<T>::covarianceMatrixOfColumns(const matrix<T>& src,
                                                     matrix<T>& dest,
                                                     vector<T>& mu) const {

    // no data?
    if (src.empty()) {
      dest.clear();
      return;
    }

    // dimension of points
    int d=src.rows();
    // number of points
    int n=src.columns();
    int i;
    dest.resize(d,d,T(0),false,true);
   
    // just one comlumn?
    if (n<2) {
      return;
    }
    

    matrix<T> tmp=src;

    // calculate mean vector
    mean.meanOfColumns(src,mu);
    for (i=0; i<d; i++) {
      // this one computes the difference between a row of tmp and mu(i)
      tmp.getRow(i).subtract(mu.at(i));
    }
    // now in tmp x_ij is replaced by x_ij-mu_i

    // this matrix is supposed to keep (x_;j-mu)(x_;j-mu)^T
    matrix<T> tmpProd(d,d);

    vector<T> tmpV;
    for (i=0; i<n; i++) {
      tmp.getColumnCopy(i,tmpV);
      tmpProd.outerProduct(tmpV,tmpV);
      dest.add(tmpProd);
    }
    if (getParameters().type == parameters::Empirical) {
      dest.divide(static_cast<T>(n-1));
    } else {
      dest.divide(static_cast<T>(n));
    }
    if (getParameters().correlation) {
      covar2corrcoef(dest);
    }
  }

  template <class T>
  void varianceFunctor<T>::covar2corrcoef(matrix<T> &cv) {
    int row,column;
    vector<T> diag(cv.rows());
    typename vector<T>::iterator i=diag.begin();
    genericVector<bool> valid(cv.rows());

    for (row=0; row<cv.rows(); row++) {
      *i=sqrt(cv.at(row,row));
      valid.at(row)=(*i > std::numeric_limits<T>::epsilon());
      ++i;
    }
    typename matrix<T>::iterator k=cv.begin();
    row=0;
    column=0;
    for (row=0; row<cv.rows(); row++) {
      for (column=0; column<cv.columns(); column++, ++k) {
        // do not do anything with columns that have zero variance
        if (valid.at(row) && valid.at(column)) {
          *k/=diag.at(row)*diag.at(column);
        } else {
          *k=T(0);
        }
      }
    }
  }


}
#include "ltiUndebug.h"


