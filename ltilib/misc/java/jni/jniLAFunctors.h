/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class de_rwth_0005faachen_techinfo_ltilib_LAFunctors */

#ifndef _Included_de_rwth_0005faachen_techinfo_ltilib_LAFunctors
#define _Included_de_rwth_0005faachen_techinfo_ltilib_LAFunctors
#ifdef __cplusplus
extern "C" {
#endif
/* Inaccessible static: props */
#undef de_rwth_0005faachen_techinfo_ltilib_LAFunctors_EPSILON
#define de_rwth_0005faachen_techinfo_ltilib_LAFunctors_EPSILON 1.0E-14
/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    openTheGate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_openTheGate
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    closeTheGate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_closeTheGate
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    matrixInversion
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_matrixInversion__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    matrixInversion
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_matrixInversion__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    safeMatrixInversion
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_safeMatrixInversion__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    safeMatrixInversion
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_safeMatrixInversion__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    jacobi
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_jacobi
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    luDecompose
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_luDecompose__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    luDecompose
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_luDecompose__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    svd
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_svd__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    svd
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_svd__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    det
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_det
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    l1Norm
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_l1Norm__Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    l1Norm
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_l1Norm__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    l2Norm
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_l2Norm__Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    l2Norm
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_l2Norm__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makePCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;ILde/rwth_aachen/techinfo/ltilib/Matrix;D)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makePCA
  (JNIEnv *, jobject, jobject, jint, jobject, jdouble);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeWhite
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;ILde/rwth_aachen/techinfo/ltilib/Matrix;D)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeWhite
  (JNIEnv *, jobject, jobject, jint, jobject, jdouble);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    usePCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_usePCA__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    usePCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_usePCA__Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    getPCA
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_getPCA
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    setPCA
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_setPCA
  (JNIEnv *, jobject, jstring);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    getBinaryPCA
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_getBinaryPCA
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    setBinaryPCA
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_setBinaryPCA
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeFastPCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;ILde/rwth_aachen/techinfo/ltilib/Matrix;D)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeFastPCA
  (JNIEnv *, jobject, jobject, jint, jobject, jdouble);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeFastWhite
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;ILde/rwth_aachen/techinfo/ltilib/Matrix;D)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeFastWhite
  (JNIEnv *, jobject, jobject, jint, jobject, jdouble);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeSeqPCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;I)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeSeqPCA
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeSeqWhite
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;I)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeSeqWhite
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    useSeqPCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_useSeqPCA__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    useSeqPCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_useSeqPCA__Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    getSeqPCA
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_getSeqPCA
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    setSeqPCA
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_setSeqPCA
  (JNIEnv *, jobject, jstring);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    getBinarySeqPCA
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_getBinarySeqPCA
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    setBinarySeqPCA
 * Signature: ([B)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_setBinarySeqPCA
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeFastSeqPCA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;I)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeFastSeqPCA
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    makeFastSeqWhite
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;I)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_makeFastSeqWhite
  (JNIEnv *, jobject, jobject, jint);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    cos2Similarity
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_cos2Similarity
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    euclidianSimilarity
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_euclidianSimilarity
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_LAFunctors
 * Method:    emphasizeDiagonal
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;D)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_LAFunctors_emphasizeDiagonal
  (JNIEnv *, jobject, jobject, jdouble);

#ifdef __cplusplus
}
#endif
#endif
