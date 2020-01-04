/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class de_rwth_0005faachen_techinfo_ltilib_StatFunctors */

#ifndef _Included_de_rwth_0005faachen_techinfo_ltilib_StatFunctors
#define _Included_de_rwth_0005faachen_techinfo_ltilib_StatFunctors
#ifdef __cplusplus
extern "C" {
#endif
/* Inaccessible static: props */
/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    openTheGate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_openTheGate
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    closeTheGate
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_closeTheGate
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    meanOfRows
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_meanOfRows
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    meanOfColumns
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_meanOfColumns
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    meanOfVector
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_meanOfVector
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    geometricMeanOfVector
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_geometricMeanOfVector
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    varianceOfRows
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_varianceOfRows
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    fillGaussVector
 * Signature: (DDLde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_fillGaussVector
  (JNIEnv *, jobject, jdouble, jdouble, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    varianceOfColumns
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_varianceOfColumns
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    covarianceMatrixOfRows
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_covarianceMatrixOfRows
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    covarianceMatrixOfColumns
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_covarianceMatrixOfColumns
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    covariance
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_covariance
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    boundsOfRows
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_boundsOfRows
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    boundsOfColumns
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_boundsOfColumns
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    min
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_min
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    max
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_max
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    entropy
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;)D
 */
JNIEXPORT jdouble JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_entropy
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    makeClassLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;ILde/rwth_aachen/techinfo/ltilib/Matrix;)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_makeClassLDA
  (JNIEnv *, jobject, jobject, jobject, jint, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    computeSb
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_computeSb
  (JNIEnv *, jobject, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    makeLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;ILde/rwth_aachen/techinfo/ltilib/Matrix;)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_makeLDA
  (JNIEnv *, jobject, jobject, jobject, jint, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    useLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_useLDA__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2Lde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    useLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;Lde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_useLDA__Lde_rwth_1aachen_techinfo_ltilib_Vector_2Lde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    getLDA
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_getLDA
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    setLDA
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_setLDA
  (JNIEnv *, jobject, jstring);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    makeSeqLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;)I
 */
JNIEXPORT jint JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_makeSeqLDA
  (JNIEnv *, jobject, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    useSeqLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;ILde/rwth_aachen/techinfo/ltilib/Matrix;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_useSeqLDA__Lde_rwth_1aachen_techinfo_ltilib_Matrix_2ILde_rwth_1aachen_techinfo_ltilib_Matrix_2
  (JNIEnv *, jobject, jobject, jint, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    useSeqLDA
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Vector;ILde/rwth_aachen/techinfo/ltilib/Vector;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_useSeqLDA__Lde_rwth_1aachen_techinfo_ltilib_Vector_2ILde_rwth_1aachen_techinfo_ltilib_Vector_2
  (JNIEnv *, jobject, jobject, jint, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    getSeqLDA
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_getSeqLDA
  (JNIEnv *, jobject);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    setSeqLDA
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_setSeqLDA
  (JNIEnv *, jobject, jstring);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    fitMixtureModel
 * Signature: (Lde/rwth_aachen/techinfo/ltilib/Matrix;[Lde/rwth_aachen/techinfo/ltilib/Vector;[Lde/rwth_aachen/techinfo/ltilib/Matrix;Lde/rwth_aachen/techinfo/ltilib/Vector;DI)Z
 */
JNIEXPORT jboolean JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_fitMixtureModel
  (JNIEnv *, jobject, jobject, jobjectArray, jobjectArray, jobject, jdouble, jint);

/*
 * Class:     de_rwth_0005faachen_techinfo_ltilib_StatFunctors
 * Method:    getStatus
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_de_rwth_1aachen_techinfo_ltilib_StatFunctors_getStatus
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif