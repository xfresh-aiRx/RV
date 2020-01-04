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



#ifndef JNIMATHOBJECT_H
#define JNIMATHOBJECT_H

/* This file contains some stuff for converting LTI matrix and vector
   object references into Java long values.
   It is some sort of "global.h" for this JNI stuff. Ugly, very ugly...
*/

#include <jni.h>
#include "libBase.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

// #define _DEBUG_ME 1
// #include "jDebug.h"

/* Type definitions */

typedef lti::mathObject jniMathObject;
typedef lti::matrix<double> jniMatrix;
typedef lti::vector<double> jniVector;

/* MathObject functions */

inline jlong mathobject2jlong(jniMathObject *o) {
  return object2jlong(o);
}

inline jniMathObject *jlong2mathobject(jlong p) {
  return dynamic_cast<jniMathObject *>(jlong2object(p));
}


/* Matrix functions */

inline jniMatrix *jlong2matrix(jlong p) {
//   std::cerr << "matrix beforce cast: " << p << "\n";
//   std::string dummy;
//   std::cin >> dummy;
  jniMatrix *m=dynamic_cast<jniMatrix *>(jlong2mathobject(p));
  assert(m != 0);
  return m;
}

inline jlong matrix2jlong(jniMatrix *p) {
  return mathobject2jlong(p);
}

inline jniMatrix *getMatrix(JNIEnv *env, jobject m) {
  jfieldID fid;
  //jniMatrix *mat;

  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/Matrix");
  fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2matrix(env->GetLongField(m, fid));
}

inline void putMatrix(JNIEnv *env, jobject m, jniMatrix *mat) {
  jfieldID fid;
  //jniMatrix *mat;

  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/Matrix");
  fid=env->GetFieldID(cls, "nativeObject", "J");
  env->SetLongField(m,fid, matrix2jlong(mat));
}

/* Vector functions */

inline jniVector *jlong2vector(jlong p) {
  jniVector *v=dynamic_cast<jniVector *>(jlong2mathobject(p));
  assert(v != 0);
  return v;
}

inline jlong vector2jlong(jniVector *p) {
  return mathobject2jlong(p);
}

inline jniVector *getVector(JNIEnv *env, jobject m) {
  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/Vector");
  jfieldID fid=env->GetFieldID(cls, "nativeObject", "J");
  return jlong2vector(env->GetLongField(m, fid));
}

inline void putVector(JNIEnv *env, jobject m, jniVector *vec) {
  jfieldID fid;
  //jniMatrix *mat;

  jclass cls=env->FindClass("de/rwth_aachen/techinfo/ltilib/Vector");
  fid=env->GetFieldID(cls, "nativeObject", "J");
  env->SetLongField(m,fid, vector2jlong(vec));
}

#endif

