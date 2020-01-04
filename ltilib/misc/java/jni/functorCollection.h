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



#ifndef JNIFUNCTORCOLLECTION_H
#define JNIFUNCTORCOLLECTION_H

#include <jni.h>
#include "mathObject.h"
#include "libBase.h"

class functorCollection: public jniLibBase {
};

class UnaryFunctorCollection: public functorCollection {
};

/* general functor collection conversion functions */

inline jlong functor2jlong(functorCollection *f) {
  return object2jlong(f);
}

inline functorCollection *jlong2functor(jlong n) {
  functorCollection *c=dynamic_cast<functorCollection *>(jlong2object(n));
  assert(c != 0);
  return c;
}

inline functorCollection *getFunctor(JNIEnv *env, jobject f) {
  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/Functors");
  jfieldID fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2functor(env->GetLongField(f, fid));
}

/* Unary functor collection */

inline jlong unaryfunctor2jlong(UnaryFunctorCollection *f) {
  return object2jlong(f);
}

inline UnaryFunctorCollection *jlong2unaryfunctor(jlong n) {
  UnaryFunctorCollection *c=dynamic_cast<UnaryFunctorCollection *>(jlong2object(n));
  assert(c != 0);
  return c;
}

inline UnaryFunctorCollection *getUnaryFunctor(JNIEnv *env, jobject f) {
  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/Functors");
  jfieldID fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2unaryfunctor(env->GetLongField(f, fid));
}



#endif

