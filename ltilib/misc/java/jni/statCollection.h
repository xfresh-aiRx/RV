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



#ifndef JNI_STATCOLLECTION_H
#define JNI_STATCOLLECTION_H

#include "functorCollection.h"

#include "ltiConfig.h"

//#include "ltiLinearAlgebraFunctor.h"
#ifdef HAVE_LAPACK
#include "ltiLDA.h"
#include "ltiSerialLDA.h"
#else
#warning "Not configured for LAPACK. LDA will not work."
#endif
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"
#include "ltiGaussDist.h"
#include "ltiGammaDist.h"
#include "ltiExpDist.h"
#include "ltiPoissonDist.h"
#include "ltiUniformDist.h"
#include "ltiBoundsFunctor.h"
#include "ltiEntropyFunctor.h"
#include "ltiGaussianMixtureModel.h"

class StatFunctorCollection: public functorCollection {
public:
  // this is not really public, but we need it anyway
  std::string status;
  lti::meansFunctor<double> means;
  lti::varianceFunctor<double> variance;
  lti::boundsFunctor<double> bounds;
  lti::gaussianDistribution gaussDist;
  lti::exponentialDistribution expDist;
  lti::gammaDistribution gammaDist;
  lti::poissonDistribution poissonDist;
  lti::uniformDistribution uniDist;
#ifdef HAVE_LAPACK
  lti::linearDiscriminantAnalysis<double> lda;
  lti::serialLDA<double> slda;
#endif
  lti::entropyFunctor<double> entropy;
  lti::gaussianMixtureModel<double> mixture;
};


/* Stat functor collection conversion functions */

inline jlong statfunctor2jlong(StatFunctorCollection *f) {
  return object2jlong(f);
}

inline StatFunctorCollection *jlong2statfunctor(jlong n) {
  StatFunctorCollection *c=dynamic_cast<StatFunctorCollection *>(jlong2object(n));
  assert(c != 0);
  return c;
}

inline StatFunctorCollection *getStatFunctor(JNIEnv *env, jobject f) {
  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/StatFunctors");
  jfieldID fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2statfunctor(env->GetLongField(f, fid));
}


#endif

