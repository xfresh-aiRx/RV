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
 * file .......: ltiFastSVD_template.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 08.12.02
 * revisions ..: $Id: ltiFastSVD_template.h,v 1.8 2004/07/08 12:47:21 doerfler Exp $
 */


#include "ltiValidator.h"

// do not include this file directly!

#undef _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti
{


  /// default constructor
  template <class T>
  fastSVD<T>::fastSVD()
    : singularValueDecomp<T>(), lapackInterface() {
    parameters p;
    setParameters(p);
  }

  /// constructor, sets the parameters
  template <class T>
  fastSVD<T>::fastSVD(const parameters& theParams)
    : singularValueDecomp<T>(), lapackInterface() {
    setParameters(theParams);
  }


  /// destructor
  template <class T>
  fastSVD<T>::~fastSVD() {
  }


  template <class T>
  functor* fastSVD<T>::clone() const {
    return new fastSVD<T>(*this);
  }


  template <class T>
  const typename fastSVD<T>::parameters&
  fastSVD<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if (isNull(params)) {
      throw functor::invalidParametersException();
    }

    return *params;
  }


  template <class T>
  bool fastSVD<T>::apply(const matrix<T>& theMatrix,
                         matrix<T>& leftSV,
                         vector<T>& singularValues,
                         matrix<T>& rightSVtrans) const
  {

    // do things here
    const parameters& param = getParameters();

    lti::validator validate;

    if (!validate.apply(theMatrix)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("data matrix has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    matrix<T> a;

    char jobz,jobu,jobvt;
    long m=theMatrix.rows();
    long n=theMatrix.columns();
    long minMN=min(m,n);
    long ldu=m;
    long ldvt=minMN;
    long info=0;

    _lti_debug3("m=" << m << " n=" << n << " minMN=" << minMN << "\n");


    if (param.useMinDimensions) {
      _lti_debug2("Using min dimensions");
      jobz='S';
      jobu='S';
      jobvt='S';
      leftSV.resize(minMN,m);
      rightSVtrans.resize(n,minMN);
    } else {
      _lti_debug2("Using all dimensions");
      jobz='A';
      jobu='A';
      jobvt='A';
      leftSV.resize(m,m);
      rightSVtrans.resize(n,n);
      ldvt=n;
    }

    a.transpose(theMatrix);
    singularValues.resize(minMN);
    if (a.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }

    T twork[1];
    long lwork=-1;

    if (param.useDC) {

      long* iwork = new long[8*minMN];

      sdd(&jobz, &m, &n, &a.at(0,0), &m,
          &singularValues.at(0), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &iwork[0], &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      sdd(&jobz, &m, &n, &a.at(0,0), &m,
          &singularValues.at(0), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork,  &iwork[0], &info);

      delete[] iwork;
      delete[] work;

    } else {

      svd(&jobu, &jobvt, &m, &n, &a.at(0,0), &m,
          &singularValues.at(0), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      svd(&jobu, &jobvt, &m, &n, &a.at(0,0), &m,
          &singularValues.at(0), &leftSV.at(0,0), &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork, &info);

      delete[] work;
    }


    _lti_debug3("param.transposeU=" << param.transposeU
                << "  param.transposeV=" << param.transposeV <<"\n");

    //transpose
    if (!param.transposeU) {
      leftSV.transpose();
    }

    if (param.transposeV) {
      rightSVtrans.transpose();
    }

    return (info==0);
  }

  template <class T>
  bool fastSVD<T>::apply(matrix<T>& theMatrix,
                         vector<T>& singularValues,
                         matrix<T>& rightSVtrans) const {

    // do things here
    const parameters& param = getParameters();

    lti::validator validate;

    if (!validate.apply(theMatrix)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("data matrix has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }


    char jobz,jobu,jobvt;
    long m=theMatrix.rows();
    long n=theMatrix.columns();
    long minMN=min(m,n);
    long ldu=m;
    long ldvt=minMN;
    long info=0;

    theMatrix.transpose();

    int mode=0;
    jobu='O';

    if (m>=n) {
      mode=1;
      jobz='O';
      jobvt='A';
      rightSVtrans.resize(n,n);
      ldu=0;
    } else if (param.useMinDimensions) {
      mode=2;
      jobz='S';
      jobvt='S';
      rightSVtrans.resize(n,minMN);
    } else {
      mode=3;
      jobz='A';
      jobvt='A';
      rightSVtrans.resize(n,n);
      ldvt=n;
    }


    singularValues.resize(minMN);
    if (theMatrix.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }

    T twork[1];
    long lwork=-1;

    if (param.useDC) {

      T* uTemp = new T[ldu*m];
      if (ldu==0) ldu=1;
      long* iwork = new long[8*minMN];

      sdd(&jobz, &m, &n, &theMatrix.at(0,0), &m,
          &singularValues.at(0), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &iwork[0], &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      sdd(&jobz, &m, &n, &theMatrix.at(0,0), &m,
          &singularValues.at(0), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork,  &iwork[0], &info);

      if (jobz!='O') {
        theMatrix.fill(uTemp,0,0,m-1,m-1);
      }

      delete[] iwork;
      delete[] work;
      delete[] uTemp;

    } else {

      T* uTemp = new T[ldu*m];
      if (ldu==0) ldu=1;

      svd(&jobu, &jobvt, &m, &n, &theMatrix.at(0,0), &m,
          &singularValues.at(0), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &twork[0], &lwork, &info);

      lwork=int(twork[0]);
      T* work = new T[lwork];

      svd(&jobu, &jobvt, &m, &n, &theMatrix.at(0,0), &m,
          &singularValues.at(0), &uTemp[0], &ldu,
          &rightSVtrans.at(0,0), &ldvt,
          &work[0], &lwork, &info);

      delete[] uTemp;
      delete[] work;
    }


    //transpose

    theMatrix.transpose();

    if (param.transposeV) {
      rightSVtrans.transpose();
    }

    return (info==0);

  }

  template<>
  int fastSVD<float>::svd(char* jobu, char* jobvt,
                          long* m, long* n, float* a, long* lda,
                          float* s, float* u, long* ldu, float* vt, long* ldvt,
                          float* work, long* lwork,
                          long* info) const;

  template<>
  int fastSVD<double>::svd(char* jobu, char* jobvt,
                           long* m, long* n, double* a, long* lda,
                           double* s, double* u, long* ldu,
                           double* vt, long* ldvt,
                           double* work, long* lwork,
                           long* info) const;

  template<>
  int fastSVD<float>::sdd(char* jobz, long* m, long* n, float* a, long* lda,
                          float* s, float* u, long* ldu, float* vt, long* ldvt,
                          float* work, long* lwork, long* iwork,
                          long* info) const;

  template<>
  int fastSVD<double>::sdd(char* jobz, long* m, long* n, double* a, long* lda,
                           double* s, double* u, long* ldu,
                           double* vt, long* ldvt,
                           double* work, long* lwork, long* iwork,
                           long* info) const;
}

#include "ltiUndebug.h"


