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
 * file .......: ltiFastGeneralizedEigenSystem.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 09.05.2003
 * revisions ..: $Id: ltiFastGeneralizedEigenSystem.cpp,v 1.1 2003/05/19 11:57:40 doerfler Exp $
 */

#include "ltiFastGeneralizedEigenSystem.h"
#ifdef HAVE_LAPACK
#include "ltiFastGeneralizedEigenSystem_template.h"
#include "clapack.h"

namespace lti {


  template<>
  int fastGeneralizedEigenSystem<float>::sygv(long* itype, char* jobz, 
                                              char* uplo, 
                                              long* n, float* a, long* lda,
                                              float* b, long* ldb,
                                              float* w,
                                              float* work, long* lwork,
                                              long* info) const {
    lockInterface();
    int tmp=LA_SSYGV(itype,jobz,uplo,n,a,lda,b,ldb,w,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int fastGeneralizedEigenSystem<double>::sygv(long* itype, char* jobz, 
                                               char* uplo, 
                                               long* n, double* a, long* lda,
                                               double* b, long* ldb,
                                               double* w,
                                               double* work, long* lwork,
                                               long* info) const {
    lockInterface();
    int tmp=LA_DSYGV(itype,jobz,uplo,n,a,lda,b,ldb,w,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int fastGeneralizedEigenSystem<float>::sygvd(long* itype, char* jobz, 
                                               char* uplo, 
                                               long* n, float* a, long* lda,
                                               float* b, long* ldb,
                                               float* w,
                                               float* work, long* lwork,
                                               long* iwork, long* liwork,
                                               long* info) const {
    lockInterface();
    int tmp=LA_SSYGVD(itype,jobz,uplo,n,a,lda,b,ldb,w,
                      work,lwork,iwork,liwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int fastGeneralizedEigenSystem<double>::sygvd(long* itype, char* jobz, 
                                                char* uplo, 
                                                long* n, double* a, long* lda,
                                                double* b, long* ldb,
                                                double* w,
                                                double* work, long* lwork,
                                                long* iwork, long* liwork,
                                                long* info) const {
    lockInterface();
    int tmp=LA_DSYGVD(itype,jobz,uplo,n,a,lda,b,ldb,w,
                      work,lwork,iwork,liwork,info);
    unlockInterface();
    return tmp;
  }

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
                                               long* info) const {

    lockInterface();
    int tmp=LA_SSYGVX(itype,jobz,range,uplo,n,a,lda,b,ldb,
                      vl,vu,il,iu,abstol,m,w,z,ldz,
                      work,lwork,iwork,ifail,info);
    unlockInterface();
    return tmp;
  }

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
                                               long* info) const {

    lockInterface();
    int tmp=LA_DSYGVX(itype,jobz,range,uplo,n,a,lda,b,ldb,
                      vl,vu,il,iu,abstol,m,w,z,ldz,
                      work,lwork,iwork,ifail,info);
    unlockInterface();
    return tmp;
  }

  // explicit instantiations
  template class fastGeneralizedEigenSystem<double>;
  template class fastGeneralizedEigenSystem<float>;
  
  
}

#endif

