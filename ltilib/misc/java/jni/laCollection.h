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



#ifndef JNI_LACOLLECTION_H
#define JNI_LACOLLECTION_H

#include "functorCollection.h"

//#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrixInversion.h"
#include "ltiGenEigVectors.h"
#include "ltiL1Distance.h"
#include "ltiL2Distance.h"
#include "ltiCosinusSimilarity.h"
#include "ltiEuclideanSimilarity.h"
#include "ltiEntropyFunctor.h"
#include "ltiSVD.h"
#include "ltiRegularizedPCA.h"
#include "ltiSerialPCA.h"
#include "ltiEigenSystem.h"
#include "ltiMatrixDecomposition.h"

class LAFunctorCollection: public functorCollection {
 public:
  lti::matrixInversion<double> inverter;
#ifdef HAVE_LAPACK
  lti::generalEigenVectors<double> eigenSystem;
#else
  lti::jacobi<double> eigenSystem;
#warning "Not configured for LAPACK. EigenSystem methods may not work."
#endif

  lti::luDecomposition<double> luDecomposer;
  lti::l1Distance<double> l1norm;
  lti::l2Distance<double> l2norm;
  lti::regularizedPCA<double> pca;
  lti::serialPCA<double> spca;
  lti::singularValueDecomp<double> svd;
  //lti::linearDiscriminantAnalysis<double> lda;
  lti::cosinus2Similarity<double> cos2;
  lti::euclideanSimilarity<double> euclidian;
};


/* LA functor collection conversion functions */

inline jlong lafunctor2jlong(LAFunctorCollection *f) {
  return object2jlong(f);
}

inline LAFunctorCollection *jlong2lafunctor(jlong n) {
  LAFunctorCollection *c=dynamic_cast<LAFunctorCollection *>(jlong2object(n));
  assert(c != 0);
  return c;
}

inline LAFunctorCollection *getLAFunctor(JNIEnv *env, jobject f) {
  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/LAFunctors");
  jfieldID fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2lafunctor(env->GetLongField(f, fid));
}

#endif

