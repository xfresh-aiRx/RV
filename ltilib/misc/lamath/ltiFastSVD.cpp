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
 * file .......: ltiFastSVD.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 06.12.02
 * revisions ..: $Id: ltiFastSVD.cpp,v 1.2 2003/02/17 07:39:02 alvarado Exp $
 */

#include "ltiFastSVD.h"
#ifdef HAVE_LAPACK
#include "ltiFastSVD_template.h"
#include "clapack.h"

namespace lti {


  template<>
  int fastSVD<float>::svd(char* jobu, char* jobvt,
                          long* m, long* n, float* a, long* lda,
                          float* s, float* u, long* ldu, float* vt, long* ldvt,
                          float* work, long* lwork,
                          long* info) const {
    lockInterface();
    int tmp=LA_SGESVD(jobu,jobvt,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int fastSVD<double>::svd(char* jobu, char* jobvt,
                           long* m, long* n, double* a, long* lda,
                           double* s, double* u, long* ldu,
                           double* vt, long* ldvt,
                           double* work, long* lwork,
                           long* info) const {
    lockInterface();
    int tmp=LA_DGESVD(jobu,jobvt,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int fastSVD<float>::sdd(char* jobz, long* m, long* n, float* a, long* lda,
                          float* s, float* u, long* ldu, float* vt, long* ldvt,
                          float* work, long* lwork, long* iwork,
                          long* info) const {
    lockInterface();
    int tmp=LA_SGESDD(jobz,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,iwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int fastSVD<double>::sdd(char* jobz, long* m, long* n, double* a, long* lda,
                           double* s, double* u, long* ldu,
                           double* vt, long* ldvt,
                           double* work, long* lwork, long* iwork,
                           long* info) const {
    lockInterface();
    int tmp=LA_DGESDD(jobz,m,n,a,lda,
                      s,u,ldu,vt,ldvt,
                      work,lwork,iwork,info);
    unlockInterface();
    return tmp;
  }

  // explicit instantiations
  template class fastSVD<double>;
  template class fastSVD<float>;


}

#endif

