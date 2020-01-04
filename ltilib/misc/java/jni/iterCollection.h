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



#ifndef JNI_ITERATOR_COLLECTION_H
#define JNI_ITERATOR_COLLECTION_H

#include "functorCollection.h"
#include "ltiAbsValue.h"
#include "ltiLogarithm.h"
#include "ltiSquare.h"
#include "ltiSqRoot.h"

class IteratingFunctorCollection: public functorCollection {
 public:
  lti::absoluteValue< double > absoluteMatrix;
  lti::absoluteValue< double > absoluteVector;
  lti::logarithm< double > logarithmMatrix;
  lti::logarithm< double > logarithmVector;
  lti::square<double > squareMatrix;
  lti::square<double > squareVector;
  lti::squareRoot< double > squareRootMatrix;
  lti::squareRoot< double > squareRootVector;
};


/* Iterating functor collection */

inline jlong iteratingfunctor2jlong(IteratingFunctorCollection *f) {
  return object2jlong(f);
}

inline IteratingFunctorCollection *jlong2iteratingfunctor(jlong n) {
  IteratingFunctorCollection *c=dynamic_cast<IteratingFunctorCollection *>(jlong2object(n));
  assert(c != 0);
  return c;
}

inline IteratingFunctorCollection *getIteratingFunctor(JNIEnv *env, jobject f) {
  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/IteratingFunctors");
  jfieldID fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2iteratingfunctor(env->GetLongField(f, fid));
}



#endif

