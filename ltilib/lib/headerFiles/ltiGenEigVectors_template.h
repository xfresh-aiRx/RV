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
 * file .......: ltiGenEigVectors_template.h
 * authors ....:
 * organization: LTI, RWTH Aachen
 * creation ...: 16.06.99
 * revisions ..: $Id: ltiGenEigVectors_template.h,v 1.18 2004/08/11 11:37:21 doerfler Exp $
 */


#include "ltiValidator.h"

// do not include this file directly!


#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti
{

  //--- generalEigenSystem ---
  //--------------

  /// default constructor
  template <class T>
  generalEigenVectors<T>::generalEigenVectors()
    : eigenSystem<T>(), lapackInterface() {
    parameters p;
    setParameters(p);
  }

  /// constructor, sets the parameters
  template <class T>
  generalEigenVectors<T>::generalEigenVectors(const parameters& theParams)
    : eigenSystem<T>(), lapackInterface() {
    setParameters(theParams);
  }

  /// constructor, sets the parameters
  template <class T>
  generalEigenVectors<T>::generalEigenVectors(const bool left)
    : eigenSystem<T>(), lapackInterface() {
    parameters p;
    p.leftVector=left;
    setParameters(p);
  }

  /// destructor
  template <class T>
  generalEigenVectors<T>::~generalEigenVectors() {
  }


  template <class T>
  functor* generalEigenVectors<T>::clone() const {
    return new generalEigenVectors<T>(*this);
  }


  template <class T>
  const typename generalEigenVectors<T>::parameters&
  generalEigenVectors<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if (isNull(params)) {
      throw functor::invalidParametersException(getTypeName());
    }

    return *params;
  }


  template <class T>
  bool generalEigenVectors<T>::apply(const matrix<T>& theMatrix,
             vector<T>& eigenvalues,
             matrix<T>& eigenvectors) const
  {
    vector<T> imag;

    return apply(theMatrix,eigenvalues,imag,eigenvectors);
  }

  // onCopy version of apply
  template<class T>
  bool generalEigenVectors<T>::apply(const matrix<T>& theMatrix,
             vector<T>& eigenvalues,
             vector<T>& iEigenvalues,
             matrix<T>& eigenvectors) const
  {

    // do things here
    //std::cout<<"running"<<endl;
    const parameters& param = getParameters();
    bool success;

    //cerr << "S";

    //debug("Computing Eigenvectors of this matrix:\n" << theMatrix);

    lti::validator validate;

    if (!validate.apply(theMatrix)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("Covariance matrix has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    if (param.leftVector) {
      success=leftEigenVectors(theMatrix,eigenvalues,iEigenvalues,eigenvectors);
    } else {
      success=rightEigenVectors(theMatrix,eigenvalues,iEigenvalues,eigenvectors);
    }
    if (param.sort) {
      //cerr << eigenvalues << "\n";
    }
    return success;
  }



  template<class T>
  bool generalEigenVectors<T>::leftEigenVectors(const matrix<T>& theMatrix,
             vector<T>& eigenvalues,
             vector<T>& iEigenvalues,
             matrix<T>& eigenvectors) const {
    matrix<T> dest;

    lti::validator validate;
    if (!validate.apply(theMatrix)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("Matrix has invalid elements!\n");
      eigenvalues.clear();
      iEigenvalues.clear();
      eigenvectors.clear();
      return false;
    }

    long n=theMatrix.columns();
    long m=theMatrix.rows();
    long ldvr=1;
    long ldvl=n;
    T* work = new T[4*n];
    long lwork=4*n;
    long info=0;

    //cerr << "L1";
    dest.transpose(theMatrix);
    //cerr << "L2";
    eigenvectors.resize(n,n);
    //cerr << "L3";
    eigenvalues.resize(n);
    //cerr << "L4";
    iEigenvalues.resize(n);
    //cerr << "L5";
    if (dest.getMode() != matrix<T>::Connected) {
      delete[] work;
      this->setStatusString("Something went wrong! Matrix not connected!");
      return false;
    }
    //cerr << "L6";
    char jobvl='V';
    char jobvr='N';

    geev(&jobvl,&jobvr,
         &n,&dest.at(0,0),&m,
         &eigenvalues.at(0),&iEigenvalues.at(0),
         &eigenvectors.at(0,0),&ldvl,
         &eigenvectors.at(0,0),&ldvr,
         &work[0],&lwork,&info);

    //cerr << " " << work[0] << "\n";
    //cerr << "L7";
    eigenvectors.transpose();
    delete[] work;
    return true;
  }


  template<class T>
  bool generalEigenVectors<T>::rightEigenVectors(const matrix<T>& theMatrix,
             vector<T>& eigenvalues,
             vector<T>& iEigenvalues,
             matrix<T>& eigenvectors) const {
    matrix<T> dest;

    lti::validator validate;
    if (!validate.apply(theMatrix)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("Matrix has invalid elements!\n");
      eigenvalues.clear();
      iEigenvalues.clear();
      eigenvectors.clear();
      return false;
    }

    long n=theMatrix.columns();
    long m=theMatrix.rows();
    long ldvr=n;
    long ldvl=1;
    T* work = new T[40*n];
    long lwork=40*n;
    long info=0;

    //cerr << "R1";
    dest.transpose(theMatrix);
    //cerr << "R2";
    eigenvectors.resize(n,n);
    //cerr << "R3";
    eigenvalues.resize(n);
    //cerr << "R4";
    iEigenvalues.resize(n);
    //cerr << "R5";
    if (dest.getMode() != matrix<T>::Connected) {
      delete[] work;
      this->setStatusString("Something went wrong! Matrix not connected!");
      return false;
    }
    //cerr << "R6";
    ldvr=n;
    char jobvl='N';
    char jobvr='V';
    geev(&jobvl,&jobvr,
         &n,&dest.at(0,0),&m,
         &eigenvalues.at(0),&iEigenvalues.at(0),
         &eigenvectors.at(0,0),&ldvl,
         &eigenvectors.at(0,0),&ldvr,
         &work[0],&lwork,&info);
    //cerr << " " << work[0] << "\n";
    //cerr << "R7";
    eigenvectors.transpose();

    delete[] work;
    return true;
  }


  template<>
  int generalEigenVectors<float>::geev(char* jobvl, char* jobvr,
                                       long* n, float* a, long* lda,
                                       float* wr, float* wi,
                                       float* vl, long* ldvl,
                                       float* vr, long* ldvr,
                                       float* work, long* lwork, long* info) const;

  template<>
  int generalEigenVectors<double>::geev(char* jobvl, char* jobvr,
                                        long* n, double* a,
                                        long* lda,
                                        double* wr, double* wi,
                                        double* vl, long* ldvl,
                                        double* vr, long* ldvr,
                                        double* work, long* lwork, long* info) const;
}

#include "ltiUndebug.h"


