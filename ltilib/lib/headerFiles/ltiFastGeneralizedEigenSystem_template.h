/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiFastEigenSystem_template.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 08.05.2003
 * revisions ..: $Id: ltiFastGeneralizedEigenSystem_template.h,v 1.3 2004/05/03 12:21:14 alvarado Exp $
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
  fastGeneralizedEigenSystem<T>::fastGeneralizedEigenSystem()
    : generalizedEigenSystem<T>(), lapackInterface() {
    parameters p;
    setParameters(p);
  }

  /// constructor, sets the parameters
  template <class T>
  fastGeneralizedEigenSystem<T>::fastGeneralizedEigenSystem(const parameters& theParams)
    : generalizedEigenSystem<T>(), lapackInterface() {
    setParameters(theParams);
  }

  /// constructor, sets the parameters
  template <class T>
  fastGeneralizedEigenSystem<T>::fastGeneralizedEigenSystem(const int dimensions)
    : generalizedEigenSystem<T>(), lapackInterface() {
    parameters p;
    p.dimensions=dimensions;
    setParameters(p);
  }

  /// destructor
  template <class T>
  fastGeneralizedEigenSystem<T>::~fastGeneralizedEigenSystem() {
  }


  template <class T>
  functor* fastGeneralizedEigenSystem<T>::clone() const {
    return new fastGeneralizedEigenSystem<T>(*this);
  }


  template <class T>
  const typename fastGeneralizedEigenSystem<T>::parameters&
  fastGeneralizedEigenSystem<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if (isNull(params)) {
      throw functor::invalidParametersException(getTypeName());
    }

    return *params;
  }


  template <class T>
  bool fastGeneralizedEigenSystem<T>::apply(const matrix<T>& a,
                                            const matrix<T>& b,
                                            vector<T>& eigenvalues) const
  {

    const parameters& param = getParameters();
    bool success;

    lti::validator validate;

    if (!validate.apply(a)) {
      _lti_debug("\n-- Matrix A is invalid! --\n");
      this->setStatusString("Matrix A has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    if (!validate.apply(b)) {
      _lti_debug("\n-- Matrix B is invalid! --\n");
      this->setStatusString("Matrix B has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    long itype;
    switch (param.problemType) {
      case parameters::Type1:
        itype=1;
        break;
      case parameters::Type2:
        itype=2;
        break;
      case parameters::Type3:
        itype=3;
        break;
      default:
        this->setStatusString("Illegal value of parameter problemType");
        return false;
    }

    char jobz='N';
    char uplo='U';
    long n=a.columns();
    long lda=n;
    long ldb=b.columns();
    long il=1;
    long iu=n;
    long info=0;


    matrix<T> tmpA;
    tmpA.transpose(a);
    if (tmpA.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }
    matrix<T> tmpB;
    tmpB.transpose(b);
    if (tmpB.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }

    // workspace variables
    T twork[1];
    long lwork=-1;
    T* work=0;
    long* iwork=0;

    // find all eigenvectors and eigenvalues
    if (param.dimensions==0 || param.dimensions>=n) {
      
      _lti_debug2("get all eigenvalues/-vectors\n");
      
      eigenvalues.resize(n);

      //use divide and conquer
      if (param.useDC) {
        _lti_debug2("divide and conquer\n");
        long tiwork[1];
        long liwork=-1;
        sygvd(&itype, &jobz, &uplo, 
              &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
              &eigenvalues.at(0), &twork[0], &lwork, 
              &tiwork[0], &liwork, &info);
        lwork=int(twork[0]);
        work = new T[lwork];
        liwork=tiwork[0];
        iwork = new long[liwork];

        sygvd(&itype, &jobz, &uplo, 
              &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
              &eigenvalues.at(0), &work[0], &lwork, &iwork[0], &liwork, &info);

      } else {
        _lti_debug2("no divide and conquer\n");
        sygv(&itype, &jobz, &uplo, 
             &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
             &eigenvalues.at(0), &twork[0], &lwork, &info);
        
        lwork=int(twork[0]);
        work = new T[lwork];

        sygv(&itype, &jobz, &uplo, 
             &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
             &eigenvalues.at(0), &work[0], &lwork, &info);
      }

   
      
    } else {
      //find some eigenvalues/-vectors
      
      _lti_debug2("find some eigenvalues/-vectors\n");
      T vl=T(0);
      T vu=T(0);
      T abstol=T(0);
      long m=0;
      il=n-param.dimensions+1;
      char range='I';
      long ifail=0;
      long ldz=1;
      T* dummy=new T[1];

      eigenvalues.resize(iu-il+1);
      
      iwork = new long[5*n];

      sygvx(&itype, &jobz, &range, &uplo, 
            &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
            &vl, &vu, &il, &iu, &abstol, &m, 
            &eigenvalues.at(0), &dummy[0], &ldz,
            &twork[0], &lwork, &iwork[0], &ifail, &info);
      
      lwork=int(twork[0]);
      work = new T[lwork];
      
      sygvx(&itype, &jobz, &range, &uplo, 
            &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
            &vl, &vu, &il, &iu, &abstol, &m, 
            &eigenvalues.at(0), &dummy[0], &ldz,
            &work[0], &lwork, &iwork[0], &ifail, &info);

      delete[] dummy;
    }

    int tmpRows,i;
    tmpRows = eigenvalues.size();
      
    //swap eigenvalues
    T ttt;
    for(i=0; i<tmpRows/2; i++) {
      ttt=eigenvalues.at(i);
      eigenvalues.at(i)=eigenvalues.at(tmpRows-1-i);
      eigenvalues.at(tmpRows-1-i)=ttt;
    }


    success=(info==0);

    delete[] work;
    delete[] iwork;

    return success;

  }

  template <class T>
  bool fastGeneralizedEigenSystem<T>::apply(const matrix<T>& a,
                                            const matrix<T>& b,
                                            vector<T>& eigenvalues,
                                            matrix<T>& eigenvectors) const
  {

    // do things here
    //std::cout<<"running"<<endl;
    const parameters& param = getParameters();
    bool success;

    //cerr << "S";

    //debug("Computing Eigenvectors of this matrix:\n" << theMatrix);

    lti::validator validate;

    if (!validate.apply(a)) {
      _lti_debug("\n-- Matrix A is invalid! --\n");
      this->setStatusString("Matrix A has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    if (!validate.apply(b)) {
      _lti_debug("\n-- Matrix B is invalid! --\n");
      this->setStatusString("Matrix B has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    long itype;
    switch (param.problemType) {
      case parameters::Type1:
        itype=1;
        break;
      case parameters::Type2:
        itype=2;
        break;
      case parameters::Type3:
        itype=3;
        break;
      default:
        this->setStatusString("Illegal value of parameter problemType");
        return false;
    }

    char jobz='V';
    char uplo='U';
    long n=a.columns();
    long lda=n;
    long ldb=b.columns();
    long il=1;
    long iu=n;
    long info=0;


    matrix<T> tmpB;
    tmpB.transpose(b);
    if (tmpB.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }

    // workspace variables
    T twork[1];
    long lwork=-1;
    T* work=0;
    long* iwork=0;

    // find all eigenvectors and eigenvalues
    if (param.dimensions==0 || param.dimensions>=n) {
      
      _lti_debug2("get all eigenvalues/-vectors\n");
      
      eigenvectors.transpose(a);
      eigenvalues.resize(n);

      //use divide and conquer
      if (param.useDC) {
        _lti_debug2("divide and conquer\n");
        long tiwork[1];
        long liwork=-1;
        sygvd(&itype, &jobz, &uplo, 
              &n, &eigenvectors.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
              &eigenvalues.at(0), &twork[0], &lwork, 
              &tiwork[0], &liwork, &info);
        lwork=int(twork[0]);
        work = new T[lwork];
        liwork=tiwork[0];
        iwork = new long[liwork];

        sygvd(&itype, &jobz, &uplo, 
              &n, &eigenvectors.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
              &eigenvalues.at(0), &work[0], &lwork, &iwork[0], &liwork, &info);

      } else {
        _lti_debug2("no divide and conquer\n");
        sygv(&itype, &jobz, &uplo, 
             &n, &eigenvectors.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
             &eigenvalues.at(0), &twork[0], &lwork, &info);
        
        lwork=int(twork[0]);
        work = new T[lwork];

        sygv(&itype, &jobz, &uplo, 
             &n, &eigenvectors.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
             &eigenvalues.at(0), &work[0], &lwork, &info);
      }

   
      
    } else {
      //find some eigenvalues/-vectors
      
      _lti_debug2("find some eigenvalues/-vectors\n");
      T vl=T(0);
      T vu=T(0);
      T abstol=T(0);
      long m=0;
      il=n-param.dimensions+1;
      char range='I';
      long ifail=0;
      long ldz=n;

      matrix<T> tmpA;
      tmpA.transpose(a);
      if (tmpA.getMode() != matrix<T>::Connected) {
        throw matrixNotConnected();
      }
      eigenvectors.resize(iu-il+1,ldz);
      eigenvalues.resize(iu-il+1);
      
      iwork = new long[5*n];

      sygvx(&itype, &jobz, &range, &uplo, 
            &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
            &vl, &vu, &il, &iu, &abstol, &m, 
            &eigenvalues.at(0), &eigenvectors.at(0,0), &ldz,
            &twork[0], &lwork, &iwork[0], &ifail, &info);
      
      lwork=int(twork[0]);
      work = new T[lwork];
      
      sygvx(&itype, &jobz, &range, &uplo, 
            &n, &tmpA.at(0,0), &lda, &tmpB.at(0,0), &ldb, 
            &vl, &vu, &il, &iu, &abstol, &m, 
            &eigenvalues.at(0), &eigenvectors.at(0,0), &n,
            &work[0], &lwork, &iwork[0], &ifail, &info);

    }

    //transpose and sort in descending order
    int tmpRows,tmpCols,i,j;
    tmpRows = eigenvectors.rows();
    tmpCols = eigenvectors.columns();
      
    matrix<T> temp;
      
    if (eigenvectors.getMode() == matrix<T>::Connected) {
      eigenvectors.detach(temp); // transport data to 'temp'
    }
    else {
      temp.copy(eigenvectors);
    }
      
    eigenvectors.resize(tmpCols,tmpRows,T(),false,false);
    for(i=0;i<tmpRows;++i) {
      for(j=0;j<tmpCols;++j) {
        eigenvectors.at(j,i)=temp.at(tmpRows-1-i,j);
      }
    }
      
    //swap eigenvalues
    T ttt;
    for(i=0; i<tmpRows/2; i++) {
      ttt=eigenvalues.at(i);
      eigenvalues.at(i)=eigenvalues.at(tmpRows-1-i);
      eigenvalues.at(tmpRows-1-i)=ttt;
    }


    success=(info==0);

    delete[] work;
    delete[] iwork;

    return success;
  }


  template<>
  int fastGeneralizedEigenSystem<float>::sygv(long* itype, char* jobz, 
                                              char* uplo, 
                                              long* n, float* a, long* lda,
                                              float* b, long* ldb,
                                              float* w,
                                              float* work, long* lwork,
                                              long* info) const;

  template<>
  int fastGeneralizedEigenSystem<double>::sygv(long* itype, char* jobz, 
                                               char* uplo, 
                                               long* n, double* a, long* lda, 
                                               double* b, long* ldb, double* w,
                                               double* work, long* lwork,
                                               long* info) const;

  template<>
  int fastGeneralizedEigenSystem<float>::sygvd(long* itype, char* jobz, 
                                               char* uplo, 
                                               long* n, float* a, long* lda,
                                               float* b, long* ldb,
                                               float* w,
                                               float* work, long* lwork,
                                               long* iwork, long* liwork,
                                               long* info) const;
  
  template<>
  int fastGeneralizedEigenSystem<double>::sygvd(long* itype, char* jobz, 
                                                char* uplo, 
                                                long* n, double* a, long* lda, 
                                                double* b, long* ldb, 
                                                double* w,
                                                double* work, long* lwork,
                                                long* iwork, long* liwork,
                                                long* info) const;

  template<>
  int fastGeneralizedEigenSystem<float>::sygvx(long* itype, char* jobz, 
                                               char* range, char* uplo, 
                                               long* n, float* a, long* lda,
                                               float* b, long* ldb,
                                               float* vl, float* vu,
                                               long* il, long* iu,
                                               float* abstol,
                                               long* m, float* w, 
                                               float* z, long* ldz,
                                               float* work, long* lwork,
                                               long* iwork, long* ifail,
                                               long* info) const;

  template<>
  int fastGeneralizedEigenSystem<double>::sygvx(long* itype, char* jobz, 
                                               char* range, char* uplo, 
                                               long* n, double* a, long* lda,
                                               double* b, long* ldb,
                                               double* vl, double* vu,
                                               long* il, long* iu,
                                               double* abstol,
                                               long* m, double* w, 
                                               double* z, long* ldz,
                                               double* work, long* lwork,
                                               long* iwork, long* ifail,
                                               long* info) const;

}

#include "ltiUndebug.h"


