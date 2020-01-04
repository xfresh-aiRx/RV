
package de.rwth_aachen.techinfo.ltilib;


/**
 * This class is a representation of a collection of functors on the
 * C++ side of the LTI library. When created, it creates all the necessary
 * functor objects. Each method represents a certain functor. 
 * @version $Id: LAFunctors.java,v 1.20 2003/06/13 12:32:21 wickel Exp $
 */

public class LAFunctors extends Functors {

  public final static double EPSILON=1E-14;

  /* static class methods ---------------------------------------- */

  /** Class initialization. */
//   static {
//     System.loadLibrary("ltiInterface");
//   }


  /* construction/finalization ----------------------------------- */

  /** Constructor. */
  public LAFunctors() {
    openTheGate();
    destroy=true;
  }


  /** Finalization */
  public void finalize() throws Throwable {
    if (destroy) {
      closeTheGate();
    }
    super.finalize();
  }



  /* private methods --------------------------------------------- */

  /**
   * Opens the gateway to the C++ side of this object. Basically, it is 
   * a C function that creates the C++ lti::functorcollection object and 
   * returns a
   * pointer to this object to the Java side via a Java long variable. 
   * This pointer is then accessed by each native function to retrieve
   * the C++ object reference. We need to do this because the native
   * functions must not have a state (thread-safety). It is called by the
   * constructor of this object. Is sets the nativeObject attribute of
   * the parent class.
   */
  private native void openTheGate(); 


  /**
   * Closes the gateway to the C++ side of this object. Basically, it
   * destroys the C++ lti::functorcollection object and sets the
   * corresponding java
   * variable to zero. It is called by the finalize() method.
   */
  private native void closeTheGate();


  /* public methods  --------------------------------------------- */

  /**
   * Puts the inverse of theMatrix into theResult. The method
   * uses LU Decomposition, so if the matrix is singular, you will
   * get an invalid result.
   * @param theMatrix the matrix whose inverse is to be computed.
   * @param theResult the matrix which is set to the inverse.
   */
  public native void matrixInversion(final Matrix theMatrix, 
                                     Matrix theResult);

  
  /**
   * Inverts matrix theMatrix.
   * @param theMatrix the matrix which must be inverted.
   */
  public native void matrixInversion(Matrix theMatrix);

  /**
   * Puts the inverse of theMatrix into theResult. The inversion method
   * uses SVD, so if the matrix is singular, the result will be as close
   * to a reasonable solution as you can get. 
   * 
   * @param theMatrix the matrix whose inverse is to be computed.
   * @param theResult the matrix which is set to the inverse.
   */
  public native void safeMatrixInversion(final Matrix theMatrix, 
                                         Matrix theResult);

  
  /**
   * Inverts matrix theMatrix. The inversion method
   * uses SVD, so if the matrix is singular, the result will be as close
   * to a reasonable solution as you can get. 
   * @param theMatrix the matrix which must be inverted.
   */
  public native void safeMatrixInversion(Matrix theMatrix);



  /**
   * Computes the Eigenvectors and Eigenvalues of a real, symmetric and square
   * matrix using the Jacobi method. Eigenvalues are contained in the columns
   * of the parameter <b>eigenvectors</b>. 
   * @param theMatrix matrix of doubles whose eigenvecors and eigenvalues
   *                  will be computed.
   * @param eigenvalues eigenvalues of <b>theMatrix</b> 
   * @param eigenvectors eigenvectors of <b>theMatrix</b> in columns
   */
  public native void jacobi(final Matrix theMatrix,
                            Vector eigenvalues,
                            Matrix eigenvectors);

  /**
   * onPlace version of LU decomposition.
   * Given a matrix theMatrix[0..n-1][0..n-1], this routine replaces it by
   * the LU decomposition of a rowwise permutation of itself. 
   * permutation[0..n-1] is an output vector that records the row
   * permutation effected by the partial pivoting. The function returns
   * +/-1 depending on whether the number of row interchanges was even or
   * odd, respectively.
   */
  public native int luDecompose(Matrix theMatrix, Vector permutation);

  /** 
   * onCopy version of LU decomposition
   * Given a matrix theMatrix[0..n-1][0..n-1], this routine returns a matrix
   * decomposition[0..n-1][0..n-1] which contains the LU decomposition of a
   * rowwise permutation of theMatrix. permutation[0..n-1] is an output
   * vector that records the row permutation effected by the partial
   * pivoting. The function returns +/-1 depending on whether the number of
   * row interchanges was even or odd, respectively.
   */
  public native int luDecompose(final Matrix theMatrix,
                                Matrix decompositon,
                                Vector permutation);

  /**
   * onCopy version of Singular Value Decomposition. Given some Matrix a,
   * the function will compute a decomposition into an orthogonal matrix u,
   * a diagonal matrix w, and another orthogonal matrix v, which satisfy
   * a=uwv. For space-saving reasons, w is passed as a vector. The number
   * of non-null entries in w is the rank of the matrix.
   * @param a matrix A which is to be decomposed.
   * @param u will contain the U matrix after the function returns.
   * @param w will contain the diagonal of the W matrix after the function returns.
   * @param v will contain the V matrix after the funtion returns.
   */
  public native void svd(final Matrix a, 
                         Matrix u, Vector w, Matrix v);

  /**
   * onPlace version of Singular Value Decomposition. Given some Matrix a,
   * the function will compute a decomposition into an orthogonal matrix u,
   * a diagonal matrix w, and another orthogonal matrix v, which satisfy
   * a=uwv. For space-saving reasons, w is passed as a vector. The number
   * of non-null entries in w is the rank of the matrix.
   * @param au when called, this has to be the matrix A which is to be decomposed. 
   *           When the function returns, it will contain the U matrix.
   * @param w will contain the diagonal of the W matrix after the function returns.
   * @param v will contain the V matrix after the funtion returns.
   */
  public native void svd(Matrix au, Vector w, Matrix v);

  /**
   * Computes the determinant of the matrix.
   */
  public native double det(Matrix theMatrix);

  public boolean isSingular(Matrix theMatrix) {
    return Math.abs(det(theMatrix)) < EPSILON;
  }

  /**
   * Computes the L1 distance between two vectors.
   */
  public native double l1Norm(final Vector a, final Vector b);

  /**
   * Computes the L1 distance between two matrices.
   */
  public native double l1Norm(final Matrix a, final Matrix b);

  /**
   * Computes the L2 distance between two vectors.
   */
  public native double l2Norm(final Vector a, final Vector b);

  /**
   * Computes the L2 distance between two matrices.
   */
  public native double l2Norm(final Matrix a, final Matrix b);


  /**
   * Creates a new PCA of the first matrix and stores the transformed
   * data in the second matrix. The PCA here either
   * uses ltilib's Jacobi transform
   * for computing the eigenvectors, so it is slow und not numerically
   * stable. If you have LAPACK installed, better use 
   * {@link #makeFastPCA(Matrix,int,Matrix,double)}, which uses LAPACK's
   * dgeev function.
   * @param a the matrix with the source data.
   * @param dim the destination dimension. If this is less or equal to zero,
   *        the dimension will be set such that the covariance matrix
   *        of the result data will be non-singular. 
   * @param b will return the transformed data.
   * @param kappa the regularization parameter
   * @return the result dimension. 
   */
  public native int makePCA(final Matrix a, int dim, Matrix b, double kappa);

  /**
   * Creates a whitening transform  of the data contained in the 
   * first matrix and stores the transformed data
   * in the second matrix. The PCA here uses ltilib's Jacobi transform
   * for computing the eigenvectors, so it is slow und not numerically
   * stable. If you have LAPACK installed, better use 
   * {@link #makeFastWhite(Matrix,int,Matrix,double)}, which uses LAPACK's
   * dgeev function.
   * @param a the matrix with the source data.
   * @param dim the destination dimension. If this is less or equal to zero,
   *        the dimension will be set such that the covariance matrix
   *        of the result data will be non-singular. 
   * @param b will return the transformed data.
   * @param kappa the regularization parameter
   * @return the result dimension. 
   */
  public native int makeWhite(final Matrix a, int dim, Matrix b, double kappa);

  /**
   * Re-Uses a previously computed PCA and transforms the given matrix.
   */
  public native void usePCA(final Matrix a, Matrix b);

  /**
   * Uses a previously computed PCA and transforms the given Vector.
   */
  public native void usePCA(final Vector a, Vector b);

  /**
   * Gets the current PCA and returns it into a string.
   */
  public native String getPCA();

  /**
   * Sets the PCA from the String.
   */
  public native void setPCA(String param);

  /**
   * Gets the current PCA and returns it into a string.
   */
  public native byte[] getBinaryPCA();

  /**
   * Sets the PCA from the String.
   */
  public native void setBinaryPCA(byte[] param);

  /**
   * Creates a new PCA of the data contained in the first matrix and 
   * stores the transformed data
   * in the second matrix. Uses the LAPACK function dgeev (wrapped within
   * the lti::generalEigenVectors object), which ist very fast and 
   * numerically stable. However, all LAPACK functions are protected by
   * a Mutex, so using LAPACK may screw up your concurrencies.
   * @param a the matrix with the source data.
   * @param dim the destination dimension. If this is less or equal to zero,
   *        the dimension will be set such that the covariance matrix
   *        of the result data will be non-singular. 
   * @param b will return the transformed data.
   * @param kappa the regularization parameter
   * @return the result dimension. 
   */
  public native int makeFastPCA(final Matrix a, int dim, Matrix b, double kappa);

  /**
   * Creates a new whitening transform of the data contained in the 
   * first matrix and stores the transformed data
   * in the second matrix. Uses the LAPACK function dgeev (wrapped within
   * the lti::generalEigenVectors object), which ist very fast and 
   * numerically stable. However, all LAPACK functions are protected by
   * a Mutex, so using LAPACK may screw up your concurrencies.
   * @param a the matrix with the source data.
   * @param dim the destination dimension. If this is less or equal to zero,
   *        the dimension will be set such that the covariance matrix
   *        of the result data will be non-singular. 
   * @param b will return the transformed data.
   * @param kappa the regularization parameter
   * @return the result dimension. 
   */
  public native int makeFastWhite(final Matrix a, int dim, Matrix b, double kappa);

  // ---- sequential versions

  /**
   * Creates a new PCA of the first matrix and stores the transformed
   * data in the second matrix. The PCA here either
   * uses ltilib's Jacobi transform
   * for computing the eigenvectors, so it is slow und not numerically
   * stable. If you have LAPACK installed, better use 
   * {@link #makeFastPCA(Matrix,int,Matrix,double)}, which uses LAPACK's
   * dgeev function.
   * @param a the matrix with a chunk of source data (rowwise)
   * @return the result dimension. 
   */
  public native int makeSeqPCA(final Matrix a, int dim);

  /**
   * Creates a whitening transform  of the data contained in the 
   * first matrix and stores the transformed data
   * in the second matrix. The PCA here uses ltilib's Jacobi transform
   * for computing the eigenvectors, so it is slow und not numerically
   * stable. If you have LAPACK installed, better use 
   * {@link #makeFastWhite(Matrix,int,Matrix,double)}, which uses LAPACK's
   * dgeev function.
   * @param a the matrix with a chunk of the source data.
   * @return the result dimension. 
   */
  public native int makeSeqWhite(final Matrix a, int dim);

  /**
   * Re-Uses a previously computed PCA and transforms the given matrix.
   */
  public native void useSeqPCA(final Matrix a, Matrix b);

  /**
   * Uses a previously computed PCA and transforms the given Vector.
   */
  public native void useSeqPCA(final Vector a, Vector b);

  /**
   * Gets the current PCA and returns it into a string.
   */
  public native String getSeqPCA();

  /**
   * Sets the PCA from the String.
   */
  public native void setSeqPCA(String param);

  /**
   * Gets the current PCA and returns it into a string.
   */
  public native byte[] getBinarySeqPCA();

  /**
   * Sets the PCA from the String.
   */
  public native void setBinarySeqPCA(byte[] param);

  /**
   * Creates a new PCA of the data contained in the first matrix and 
   * stores the transformed data
   * in the second matrix. Uses the LAPACK function dgeev (wrapped within
   * the lti::generalEigenVectors object), which ist very fast and 
   * numerically stable. However, all LAPACK functions are protected by
   * a Mutex, so using LAPACK may screw up your concurrencies.
   * @param a the matrix with the source data.
   * @param dim the destination dimension. If this is less or equal to zero,
   *        the dimension will be set such that the covariance matrix
   *        of the result data will be non-singular. 
   * @return the result dimension. 
   */
  public native int makeFastSeqPCA(final Matrix a, int dim);

  /**
   * Creates a new whitening transform of the data contained in the 
   * first matrix and stores the transformed data
   * in the second matrix. Uses the LAPACK function dgeev (wrapped within
   * the lti::generalEigenVectors object), which ist very fast and 
   * numerically stable. However, all LAPACK functions are protected by
   * a Mutex, so using LAPACK may screw up your concurrencies.
   * @param a the matrix with the source data.
   * @param dim the destination dimension. If this is less or equal to zero,
   *        the dimension will be set such that the covariance matrix
   *        of the result data will be non-singular. 
   * @return the result dimension. 
   */
  public native int makeFastSeqWhite(final Matrix a, int dim);

  /**
   * Computes a square cosinus similarity measure for the 2 vectors a and b
   */
  public native double cos2Similarity(final Vector a, final Vector b);

  /**
   * Computes an euclidian-type similarity measure for the 2 vectors a and b
   */
  public native double euclidianSimilarity(final Vector a, final Vector b);


  public native void emphasizeDiagonal(Matrix a, double f);

}











