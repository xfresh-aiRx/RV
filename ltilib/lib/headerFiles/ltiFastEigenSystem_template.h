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
 * file .......: ltiFastEigenSystem_template.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 04.12.02
 * revisions ..: $Id: ltiFastEigenSystem_template.h,v 1.10 2005/11/04 15:34:55 doerfler Exp $
 */


#include "ltiValidator.h"

// do not include this file directly!

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti
{
  /// default constructor
  template <class T>
  fastEigenSystem<T>::fastEigenSystem()
    : eigenSystem<T>(), lapackInterface() {
    parameters p;
    setParameters(p);
  }

  /// constructor, sets the parameters
  template <class T>
  fastEigenSystem<T>::fastEigenSystem(const parameters& theParams)
    : eigenSystem<T>(), lapackInterface() {
    setParameters(theParams);
  }

  /// constructor, sets the parameters
  template <class T>
  fastEigenSystem<T>::fastEigenSystem(const int dimensions)
    : eigenSystem<T>(), lapackInterface() {
    parameters p;
    p.dimensions=dimensions;
    setParameters(p);
  }

  /// destructor
  template <class T>
  fastEigenSystem<T>::~fastEigenSystem() {
  }


  template <class T>
  functor* fastEigenSystem<T>::clone() const {
    return new fastEigenSystem<T>(*this);
  }


  template <class T>
  const typename fastEigenSystem<T>::parameters&
  fastEigenSystem<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if (isNull(params)) {
      throw functor::invalidParametersException(getTypeName());
    }

    return *params;
  }


  template <class T>
  bool fastEigenSystem<T>::apply(const matrix<T>& theMatrix,
                                       vector<T>& eigenvalues,
                                       matrix<T>& eigenvectors) const
  {

    if (theMatrix.empty()) {
      eigenvalues.clear();
      eigenvectors.clear();
      return true; // nothing to do, matrix is empty!
    }

    // do things here
    const parameters& param = getParameters();
    bool success;

    lti::validator validate;

    if (!validate.apply(theMatrix)) {
      _lti_debug("\n-- Matrix is invalid! --\n");
      this->setStatusString("Covariance matrix has invalid elements!\n");
      return false;
    } else {
      _lti_debug("+\n");
    }

    // RANGE  (input) CHARACTER*1
    //        = 'A':  all eigenvalues will be found.
    //        = 'V':  all eigenvalues in the half-open interval
    //                (VL,VU] will be found.
    //        = 'I':  the  IL-th  through IU-th eigenvalues
    //                will be found.
    char range='A'; 

    // JOBZ   (input) CHARACTER*1
    //        = 'N':  Compute eigenvalues only;
    //        = 'V':  Compute eigenvalues and eigenvectors.
    char jobz='V';  

    // UPLO   (input) CHARACTER*1
    //        = 'U':  Upper triangle of A is stored;
    //        = 'L':  Lower triangle of A is stored.
    char uplo='U';  
    
    // N      (input) INTEGER
    //        The order of the matrix A.  N >= 0.
    long n = theMatrix.columns(); 

    // A  (input/output)  DOUBLE PRECISION array, dimension (LDA, N)
    //    On  entry, the symmetric matrix A.  If UPLO = 'U',
    //    the leading N-by-N upper triangular part of A con­
    //    tains  the  upper triangular part of the matrix A.
    //    If UPLO = 'L', the leading N-by-N lower triangular
    //    part  of  A  contains the lower triangular part of
    //    the matrix A.  On exit,  the  lower  triangle  (if
    //    UPLO='L')  or  the upper triangle (if UPLO='U') of
    //    A, including the diagonal, is destroyed.
//     matrix<T> a;    
//     a.transpose(theMatrix);
    //no need to transpose: it's symmetric
    matrix<T> a(theMatrix);
    if (a.getMode() != matrix<T>::Connected) {
      throw matrixNotConnected();
    }

    // LDA    (input) INTEGER
    //        The leading dimension of  the  array  A.   LDA  >= max(1,N).
    long lda = n;

    // VL     (input) DOUBLE PRECISION
    // VU     (input) DOUBLE PRECISION
    //        If RANGE='V', the lower and upper bounds of the
    //        interval to be searched for eigenvalues.
    //        VL < VU. 
    //        Not referenced if RANGE = 'A' or 'I'.    
    T vl=T(0);
    T vu=T(0);

    // IL     (input) INTEGER
    // IU     (input) INTEGER If RANGE='I', the  indices
    //        (in  ascending  order) of the smallest and 
    //        largest eigenvalues to be returned.
    //        1 <= IL <= IU  <=  N,if  N > 0;
    //        IL = 1 and IU = 0 if N = 0.
    //        Not referenced if RANGE = 'A' or 'V'.
    long il=1;
    long iu=n;      

    // ABSTOL (input) DOUBLE PRECISION
    //        The absolute error tolerance for the
    //        eigenvalues.  
    //        An approximate eigenvalue is accepted as
    //        converged when it is determined to lie in an
    //        interval [a,b] of width less than or equal to
    //
    //               ABSTOL + EPS *   max( |a|,|b| ) ,
    // 
    //        where EPS is the machine precision.  If ABSTOL
    //        is less than or equal to zero, then EPS*|T|
    //        will be used in its place, where |T| is the
    //        1-norm of the tridiagonal matrix obtained by
    //        reducing A to tridiagonal form.
    T abstol=std::numeric_limits<T>::epsilon();  

    // M      (output) INTEGER
    //        The  total number of eigenvalues found. 
    //        0 <= M <= N.
    //        If RANGE = 'A', M = N     , and 
    //        if RANGE = 'I', M = IU-IL+1.
    long m=0;

    // LDZ    (input) INTEGER
    //        The  leading  dimension of the array Z.
    //        LDZ >= 1, and if JOBZ = 'V', LDZ >= max(1,N).
    long ldz=n;


    // INFO   (output) INTEGER
    //        = 0:  successful exit
    //        < 0:  if INFO = -i, the i-th argument had an illegal value
    //        > 0:  Internal error
    long info=0;

    if (param.dimensions>0 && param.dimensions<n) {
      il=n-param.dimensions+1;
      range='I';
    }

    // Z      (output) DOUBLE PRECISION array,  dimension  (LDZ,max(1,M))
    //        If JOBZ = 'V', then if INFO = 0, the first M columns of Z
    //        contain the orthonormal eigenvectors of the matrix A
    //        corresponding to the selected eigenvalues, with the i-th column
    //        of Z holding the eigenvector associated with W(i).  If JOBZ =
    //        'N', then Z is not referenced.  Note: the user must ensure that
    //        at least max(1,M) columns are supplied in the array Z; if RANGE
    //        = 'V', the exact value of M is not known in advance and an upper
    //        bound must be used.
    eigenvectors.resize(iu-il+1,ldz);

    // W       (output) DOUBLE PRECISION array, dimension (N)
    //         The  first  M elements contain the selected eigen­
    //         values in ascending order.
    vector<T> w;
    w.resize(n);

    // LWORK   (input) INTEGER
    //         The  dimension  of  the  array  WORK.   LWORK >= max(1,26*N).
    //         For  optimal  efficiency,  LWORK >= (NB+6)*N, where NB is the
    //         max of the blocksize for DSYTRD and DORMTR returned by ILAENV.
    //
    //         If  LWORK = -1, then a workspace query is assumed;
    //         the routine only calculates the  optimal  size  of
    //         the  WORK  array,  returns this value as the first
    //         entry of the WORK  array,  and  no  error  message
    //         related to LWORK is issued by XERBLA.
    long lwork=-1;

    //  WORK   (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
    //         On  exit, if INFO = 0, WORK(1) returns the optimal LWORK.
    T twork[1];


    // IWORK   (workspace/output)   INTEGER   array,  dimension (LIWORK)
    //         On exit, if INFO = 0, IWORK(1) returns the optimal LWORK.
    long tiwork[1];


    T* work;
    long* iwork;
    if (param.useRRR) {

      // ISUPPZ  (output) INTEGER array, dimension ( 2*max(1,M) )
      //         The support of the eigenvectors in  Z,  i.e.,  the
      //         indices  indicating the nonzero elements in Z. The
      //         i-th  eigenvector  is  nonzero  only  in  elements
      //         ISUPPZ( 2*i-1 ) through ISUPPZ( 2*i ).
      vector<long> isuppz(2*n); 
      
      // LIWORK  (input) INTEGER
      //         The dimension of the array IWORK.   LIWORK >= max(1,10*N).
      //
      //         If LIWORK = -1, then a workspace query is assumed;
      //         the routine only calculates the  optimal  size  of
      //         the  IWORK  array, returns this value as the first
      //         entry of the IWORK array,  and  no  error  message
      //         related to LIWORK is issued by XERBLA.
      long liwork=-1;

      if (param.dimensions>0 && param.dimensions<n) {
        isuppz.resize(2*param.dimensions);
      }

      // this first call is supposed to only compute the sizes for optimal
      // workspaces. 
      // The results are the tiwork and twork.
      evr(&jobz, &range, &uplo,
          &n, &a.at(0,0), &lda,
          &vl, &vu, &il, &iu, &abstol,
          &m, &w.at(0), &eigenvectors.at(0,0), &ldz, &isuppz.at(0),
          &twork[0],&lwork,&tiwork[0],&liwork,&info);

      // reserve the proper workspace arrays
      lwork =iround(twork[0]);
      work = new T[lwork];

      liwork=tiwork[0];
      iwork = new long[liwork];

      // now the proper eigenvalues can be computed with the correct help 
      // workspaces
      evr(&jobz, &range, &uplo,
          &n, &a.at(0,0), &n,
          &vl, &vu, &il, &iu, &abstol,
          &m, &w.at(0), &eigenvectors.at(0,0), &ldz, &isuppz.at(0),
          &work[0],&lwork,&iwork[0],&liwork,&info);

    } else {

      //  IFAIL   (output) INTEGER array, dimension (N)
      //          If JOBZ = 'V', then if INFO = 0, the first M elements of
      //          IFAIL are zero.  If INFO > 0, then IFAIL contains the
      //          indices of the eigenvectors that failed to converge.
      //          If JOBZ = 'N', then IFAIL is not referenced.
      vector<long> ifail(n); 

      // this first call is supposed to only compute the sizes for optimal
      // workspaces. 
      // The results are the twork.
      evx(&jobz, &range, &uplo,
          &n, &a.at(0,0), &lda,
          &vl, &vu, &il, &iu, &abstol,
          &m, &w.at(0), &eigenvectors.at(0,0), &ldz,
          &twork[0],&lwork,&tiwork[0],&ifail[0],&info);
      
      // reserve the proper workspace arrays
      lwork =iround(twork[0]);
      work = new T[lwork];
      
      iwork = new long[5*n];
      
      // now the proper eigenvalues can be computed with the correct help 
      // workspaces
      evx(&jobz, &range, &uplo,
          &n, &a.at(0,0), &n,
          &vl, &vu, &il, &iu, &abstol,
          &m, &w.at(0), &eigenvectors.at(0,0), &ldz,
          &work[0],&lwork,&iwork[0],&ifail[0],&info);


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

    eigenvalues.resize(tmpRows);

    //swap eigenvalues
    for(i=0; i<tmpRows; i++) {
      eigenvalues.at(i) = w.at(tmpRows-1-i);
    }

    success=(info==0);

    delete[] work;
    delete[] iwork;

    if (!success) {
      // INFO   (output) INTEGER
      //        = 0:  successful exit
      //        < 0:  if INFO = -i, the i-th argument had an illegal value
      //        > 0:  Internal error
      if (info<0) {
        this->setStatusString("Fast eigensystem got an LAPACK error: ");
        this->appendStatusString(static_cast<int>(-info));
        this->appendStatusString("-th argument seems to be wrong");
      } else {
        this->setStatusString("Fast eigensystem got an internal LAPACK error");
      }
    }

    return success;
  }

//   template <class T>
//   bool fastEigenSystem<T>::apply(const matrix<T>& theMatrix,
//                                        vector<T>& eigenvalues,
//                                        matrix<T>& eigenvectors) const
//   {

//     if (theMatrix.empty()) {
//       eigenvalues.clear();
//       eigenvectors.clear();
//       return true; // nothing to do, matrix is empty!
//     }

//     // do things here
//     const parameters& param = getParameters();
//     bool success;

//     lti::validator validate;

//     if (!validate.apply(theMatrix)) {
//       _lti_debug("\n-- Matrix is invalid! --\n");
//       this->setStatusString("Covariance matrix has invalid elements!\n");
//       return false;
//     } else {
//       _lti_debug("+\n");
//     }

//     // RANGE  (input) CHARACTER*1
//     //        = 'A':  all eigenvalues will be found.
//     //        = 'V':  all eigenvalues in the half-open interval
//     //                (VL,VU] will be found.
//     //        = 'I':  the  IL-th  through IU-th eigenvalues
//     //                will be found.
//     char range='A'; 

//     // JOBZ   (input) CHARACTER*1
//     //        = 'N':  Compute eigenvalues only;
//     //        = 'V':  Compute eigenvalues and eigenvectors.
//     char jobz='V';  

//     // UPLO   (input) CHARACTER*1
//     //        = 'U':  Upper triangle of A is stored;
//     //        = 'L':  Lower triangle of A is stored.
//     char uplo='U';  
    
//     // N      (input) INTEGER
//     //        The order of the matrix A.  N >= 0.
//     long n = theMatrix.columns(); 

//     // A  (input/output)  DOUBLE PRECISION array, dimension (LDA, N)
//     //    On  entry, the symmetric matrix A.  If UPLO = 'U',
//     //    the leading N-by-N upper triangular part of A con­
//     //    tains  the  upper triangular part of the matrix A.
//     //    If UPLO = 'L', the leading N-by-N lower triangular
//     //    part  of  A  contains the lower triangular part of
//     //    the matrix A.  On exit,  the  lower  triangle  (if
//     //    UPLO='L')  or  the upper triangle (if UPLO='U') of
//     //    A, including the diagonal, is destroyed.
//     matrix<T> a;    
//     a.transpose(theMatrix);
//     if (a.getMode() != matrix<T>::Connected) {
//       throw matrixNotConnected();
//     }

//     // LDA    (input) INTEGER
//     //        The leading dimension of  the  array  A.   LDA  >= max(1,N).
//     long lda = n;

//     // VL     (input) DOUBLE PRECISION
//     // VU     (input) DOUBLE PRECISION
//     //        If RANGE='V', the lower and upper bounds of the
//     //        interval to be searched for eigenvalues.
//     //        VL < VU. 
//     //        Not referenced if RANGE = 'A' or 'I'.    
//     T vl=T(0);
//     T vu=T(0);

//     // IL     (input) INTEGER
//     // IU     (input) INTEGER If RANGE='I', the  indices
//     //        (in  ascending  order) of the smallest and 
//     //        largest eigenvalues to be returned.
//     //        1 <= IL <= IU  <=  N,if  N > 0;
//     //        IL = 1 and IU = 0 if N = 0.
//     //        Not referenced if RANGE = 'A' or 'V'.
//     long il=1;
//     long iu=n;      

//     // ABSTOL (input) DOUBLE PRECISION
//     //        The absolute error tolerance for the
//     //        eigenvalues.  
//     //        An approximate eigenvalue is accepted as
//     //        converged when it is determined to lie in an
//     //        interval [a,b] of width less than or equal to
//     //
//     //               ABSTOL + EPS *   max( |a|,|b| ) ,
//     // 
//     //        where EPS is the machine precision.  If ABSTOL
//     //        is less than or equal to zero, then EPS*|T|
//     //        will be used in its place, where |T| is the
//     //        1-norm of the tridiagonal matrix obtained by
//     //        reducing A to tridiagonal form.
//     T abstol=std::numeric_limits<T>::epsilon();  

//     // M      (output) INTEGER
//     //        The  total number of eigenvalues found. 
//     //        0 <= M <= N.
//     //        If RANGE = 'A', M = N     , and 
//     //        if RANGE = 'I', M = IU-IL+1.
//     long m=0;

//     // LDZ    (input) INTEGER
//     //        The  leading  dimension of the array Z.
//     //        LDZ >= 1, and if JOBZ = 'V', LDZ >= max(1,N).
//     long ldz=n;

//     // ISUPPZ  (output) INTEGER array, dimension ( 2*max(1,M) )
//     //         The support of the eigenvectors in  Z,  i.e.,  the
//     //         indices  indicating the nonzero elements in Z. The
//     //         i-th  eigenvector  is  nonzero  only  in  elements
//     //         ISUPPZ( 2*i-1 ) through ISUPPZ( 2*i ).
//     vector<long> isuppz(2*n); 

//     // INFO   (output) INTEGER
//     //        = 0:  successful exit
//     //        < 0:  if INFO = -i, the i-th argument had an illegal value
//     //        > 0:  Internal error
//     long info=0;

//     if (param.dimensions>0 && param.dimensions<n) {
//       il=n-param.dimensions+1;
//       range='I';
//       isuppz.resize(2*param.dimensions);
//     }

//     // Z      (output) DOUBLE PRECISION array,  dimension  (LDZ,max(1,M))
//     //        If JOBZ = 'V', then if INFO = 0, the first M columns of Z
//     //        contain the orthonormal eigenvectors of the matrix A
//     //        corresponding to the selected eigenvalues, with the i-th column
//     //        of Z holding the eigenvector associated with W(i).  If JOBZ =
//     //        'N', then Z is not referenced.  Note: the user must ensure that
//     //        at least max(1,M) columns are supplied in the array Z; if RANGE
//     //        = 'V', the exact value of M is not known in advance and an upper
//     //        bound must be used.
//     eigenvectors.resize(iu-il+1,ldz);

//     // W       (output) DOUBLE PRECISION array, dimension (N)
//     //         The  first  M elements contain the selected eigen­
//     //         values in ascending order.
//     vector<T> w;
//     w.resize(n);

//     // LWORK   (input) INTEGER
//     //         The  dimension  of  the  array  WORK.   LWORK >= max(1,26*N).
//     //         For  optimal  efficiency,  LWORK >= (NB+6)*N, where NB is the
//     //         max of the blocksize for DSYTRD and DORMTR returned by ILAENV.
//     //
//     //         If  LWORK = -1, then a workspace query is assumed;
//     //         the routine only calculates the  optimal  size  of
//     //         the  WORK  array,  returns this value as the first
//     //         entry of the WORK  array,  and  no  error  message
//     //         related to LWORK is issued by XERBLA.
//     long lwork=-1;

//     //  WORK   (workspace/output) DOUBLE PRECISION array, dimension (LWORK)
//     //         On  exit, if INFO = 0, WORK(1) returns the optimal LWORK.
//     T twork[1];


//     // IWORK   (workspace/output)   INTEGER   array,  dimension (LIWORK)
//     //         On exit, if INFO = 0, IWORK(1) returns the optimal LWORK.
//     long tiwork[1];

//     // LIWORK  (input) INTEGER
//     //         The dimension of the array IWORK.   LIWORK >= max(1,10*N).
//     //
//     //         If LIWORK = -1, then a workspace query is assumed;
//     //         the routine only calculates the  optimal  size  of
//     //         the  IWORK  array, returns this value as the first
//     //         entry of the IWORK array,  and  no  error  message
//     //         related to LIWORK is issued by XERBLA.
//     long liwork=-1;

//     // this first call is supposed to only compute the sizes for optimal
//     // workspaces. 
//     // The results are the tiwork and twork.
//     evr(&jobz, &range, &uplo,
//         &n, &a.at(0,0), &lda,
//         &vl, &vu, &il, &iu, &abstol,
//         &m, &w.at(0), &eigenvectors.at(0,0), &ldz, &isuppz.at(0),
//         &twork[0],&lwork,&tiwork[0],&liwork,&info);

//     // reserve the proper workspace arrays
//     lwork =iround(twork[0]);
//     T* work = new T[lwork];

//     liwork=tiwork[0];
//     long* iwork = new long[liwork];

//     // now the proper eigenvalues can be computed with the correct help 
//     // workspaces
//     evr(&jobz, &range, &uplo,
//         &n, &a.at(0,0), &n,
//         &vl, &vu, &il, &iu, &abstol,
//         &m, &w.at(0), &eigenvectors.at(0,0), &ldz, &isuppz.at(0),
//         &work[0],&lwork,&iwork[0],&liwork,&info);

//     //transpose and sort in descending order
//     int tmpRows,tmpCols,i,j;
//     tmpRows = eigenvectors.rows();
//     tmpCols = eigenvectors.columns();

//     matrix<T> temp;

//     if (eigenvectors.getMode() == matrix<T>::Connected) {
//       eigenvectors.detach(temp); // transport data to 'temp'
//     }
//     else {
//       temp.copy(eigenvectors);
//     }

//     eigenvectors.resize(tmpCols,tmpRows,T(),false,false);
//     for(i=0;i<tmpRows;++i) {
//       for(j=0;j<tmpCols;++j) {
//         eigenvectors.at(j,i)=temp.at(tmpRows-1-i,j);
//       }
//     }

//     eigenvalues.resize(tmpRows);

//     //swap eigenvalues
//     for(i=0; i<tmpRows; i++) {
//       eigenvalues.at(i) = w.at(tmpRows-1-i);
//     }

//     success=(info==0);

//     delete[] work;
//     delete[] iwork;

//     if (!success) {
//       // INFO   (output) INTEGER
//       //        = 0:  successful exit
//       //        < 0:  if INFO = -i, the i-th argument had an illegal value
//       //        > 0:  Internal error
//       if (info<0) {
//         this->setStatusString("Fast eigensystem got an LAPACK error: ");
//         this->appendStatusString(static_cast<int>(-info));
//         this->appendStatusString("-th argument seems to be wrong");
//       } else {
//         this->setStatusString("Fast eigensystem got an internal LAPACK error");
//       }
//     }

//     return success;
//   }


  template<>
  int fastEigenSystem<float>::evr(char* jobz, char* range, char* uplo,
                                  long* n, float* a, long* lda,
                                  float* vl, float* vu,
                                  long* il, long* iu,
                                  float* abstol,
                                  long* m, float* w,
                                  float* z, long* ldz, long* isuppz,
                                  float* work, long* lwork,
                                  long* iwork, long* liwork,
                                  long* info) const;


  template<>
  int fastEigenSystem<double>::evr(char* jobz, char* range,char* uplo,
                                   long* n, double* a, long* lda,
                                   double* vl, double* vu,
                                   long* il, long* iu,
                                   double* abstol,
                                   long* m, double* w,
                                   double* z, long* ldz, long* isuppz,
                                   double* work, long* lwork,
                                   long* iwork, long* liwork,
                                   long* info) const;

  template<>
  int fastEigenSystem<float>::evx(char* jobz, char* range, char* uplo,
                                  long* n, float* a, long* lda,
                                  float* vl, float* vu,
                                  long* il, long* iu,
                                  float* abstol,
                                  long* m, float* w,
                                  float* z, long* ldz,
                                  float* work, long* lwork,
                                  long* iwork, long* ifail,
                                  long* info) const;


  template<>
  int fastEigenSystem<double>::evx(char* jobz, char* range,char* uplo,
                                   long* n, double* a, long* lda,
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


