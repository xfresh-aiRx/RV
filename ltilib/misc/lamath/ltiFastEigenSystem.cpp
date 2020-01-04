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
 * file .......: ltiFastEigenSystem.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 04.12.02
 * revisions ..: $Id: ltiFastEigenSystem.cpp,v 1.4 2005/11/04 15:34:55 doerfler Exp $
 */

#include "ltiFastEigenSystem.h"
#ifdef HAVE_LAPACK
#include "ltiFastEigenSystem_template.h"
#include "clapack.h"

namespace lti {


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
                                  long* info) const {
    lockInterface();

    assert(notNull(jobz));
    assert(notNull(range));
    assert(notNull(uplo));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(vl));
    assert(notNull(vu));
    assert(notNull(il));
    assert(notNull(iu));
    assert(notNull(abstol));
    assert(notNull(m));
    assert(notNull(w));
    assert(notNull(z));
    assert(notNull(ldz));
    assert(notNull(isuppz));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(iwork));
    assert(notNull(liwork));
    assert(notNull(info));

    int tmp=LA_SSYEVR(jobz,range,uplo,n,a,lda,vl,vu,il,iu,abstol,
                      m,w,z,ldz,isuppz,work,lwork,iwork,liwork,info);
    unlockInterface();
    return tmp;
  }

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
                                   long* info) const {
    lockInterface();

    assert(notNull(jobz));
    assert(notNull(range));
    assert(notNull(uplo));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(vl));
    assert(notNull(vu));
    assert(notNull(il));
    assert(notNull(iu));
    assert(notNull(abstol));
    assert(notNull(m));
    assert(notNull(w));
    assert(notNull(z));
    assert(notNull(ldz));
    assert(notNull(isuppz));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(iwork));
    assert(notNull(liwork));
    assert(notNull(info));

    int tmp=LA_DSYEVR(jobz,range,uplo,n,a,lda,vl,vu,il,iu,abstol,
                      m,w,z,ldz,isuppz,work,lwork,iwork,liwork,info);

    
    unlockInterface();
    return tmp;
  }

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
                                  long* info) const {
    lockInterface();

    assert(notNull(jobz));
    assert(notNull(range));
    assert(notNull(uplo));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(vl));
    assert(notNull(vu));
    assert(notNull(il));
    assert(notNull(iu));
    assert(notNull(abstol));
    assert(notNull(m));
    assert(notNull(w));
    assert(notNull(z));
    assert(notNull(ldz));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(iwork));
    assert(notNull(ifail));
    assert(notNull(info));

    int tmp=LA_SSYEVX(jobz,range,uplo,n,a,lda,vl,vu,il,iu,abstol,
                      m,w,z,ldz,work,lwork,iwork,ifail,info);
    unlockInterface();
    return tmp;
  }

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
                                   long* info) const {
    lockInterface();

    assert(notNull(jobz));
    assert(notNull(range));
    assert(notNull(uplo));
    assert(notNull(n));
    assert(notNull(a));
    assert(notNull(lda));
    assert(notNull(vl));
    assert(notNull(vu));
    assert(notNull(il));
    assert(notNull(iu));
    assert(notNull(abstol));
    assert(notNull(m));
    assert(notNull(w));
    assert(notNull(z));
    assert(notNull(ldz));
    assert(notNull(work));
    assert(notNull(lwork));
    assert(notNull(iwork));
    assert(notNull(ifail));
    assert(notNull(info));

    int tmp=LA_DSYEVX(jobz,range,uplo,n,a,lda,vl,vu,il,iu,abstol,
                      m,w,z,ldz,work,lwork,iwork,ifail,info);

    
    unlockInterface();
    return tmp;
  }

  // explicit instantiations
  template class fastEigenSystem<double>;
  template class fastEigenSystem<float>;


}

#endif

