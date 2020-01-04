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
 * file .......: ltiGenEigVectors.cpp
 * authors ....:
 * organization: LTI, RWTH Aachen
 * creation ...: 16.06.99
 * revisions ..: $Id: ltiGenEigVectors.cpp,v 1.9 2003/02/17 07:39:02 alvarado Exp $
 */

#include "ltiGenEigVectors.h"
#ifdef HAVE_LAPACK
#include "ltiGenEigVectors_template.h"
#include "clapack.h"

namespace lti {

  template<>
  int generalEigenVectors<float>::geev(char* jobvl,char* jobvr, 
                                       long* n,
                                       float* a,
                                       long* lda, float* wr, float* wi,
                                       float* vl,
                                       long* ldvl,  float* vr, long* ldvr,
                                       float* work, long* lwork,
                                       long* info) const {
    lockInterface();
    int tmp=LA_SGEEV(jobvl, jobvr, n,a,lda,wr,wi,vl,ldvl,vr,ldvr,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  template<>
  int generalEigenVectors<double>::geev(char* jobvl, char* jobvr,
                                        long* n,
                                        double* a, long* lda, double* wr,
                                        double* wi, double* vl, long* ldvl,
                                        double* vr, long* ldvr, double* work,
                                        long* lwork, long* info) const {
    lockInterface();
    int tmp=LA_DGEEV(jobvl, jobvr, n,a,lda,wr,wi,vl,ldvl,vr,ldvr,work,lwork,info);
    unlockInterface();
    return tmp;
  }

  // explicit instantiations
  template class generalEigenVectors<double>;
  template class generalEigenVectors<float>;


}

#endif

