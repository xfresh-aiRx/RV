
package de.rwth_aachen.techinfo.ltilib;

/**
 * This class is a representation of a collection of functors on the
 * C++ side of the LTI library. When created, it creates all the necessary
 * functor objects. Each method represents a certain functor. 
 * $Id: StatFunctors.java,v 1.20 2003/10/04 12:02:51 wickel Exp $
 */

public class StatFunctors extends Functors {

  /* static class methods ---------------------------------------- */

  /** Class initialization. */
//   static {
//     System.loadLibrary("ltiInterface");
//   }


  /* construction/finalization ----------------------------------- */

  /** Constructor. */
  public StatFunctors() {
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
   * Computes the mean of the rows of theMatrix. Each row is taken
   * as a data point in n-dim. space. The result vector contains
   * the n-dimensional mean of all row vectors.
   * @param theMatrix the data matrix 
   * @param theResult the mean of all rows of the matrix
   */
  public native void meanOfRows(Matrix theMatrix, Vector theResult);

  
  /**
   * Computes the mean of the columns of theMatrix. Each column is taken
   * as a data point in n-dim. space. The result vector contains
   * the n-dimensional mean of all row vectors.
   * @param theMatrix the data matrix 
   * @param theResult the mean of all columns of the matrix
   */
  public native void meanOfColumns(Matrix theMatrix, Vector theResult);

  /**
   * Computes the mean of the vector's elements.
   */
  public native double meanOfVector(Vector theVector);

  /**
   * Computes the geometric mean of the vector's elements.
   */
  public native double geometricMeanOfVector(Vector theVector);


  /**
   * Computes the variance of the rows of theMatrix. Each row is taken
   * as a data point in n-dim. space. The result vector contains
   * in element i the variance of the i-th element of all rows.
   * @param theMatrix the data matrix 
   * @param theResult the mean of all rows of the matrix
   */
  public native void varianceOfRows(Matrix theMatrix, Vector theResult);

  /**
   * Fill the given vector with numbers drawn from a gaussian distribution
   * with the given parameters (mean and variance)
   * @param mu the mean of the gaussian distribution
   * @param sigma the variance of the gaussian distribution
   * @param theResult vector which will contain the random values
   */
  public native void fillGaussVector(double mu, double sigma,
                                     Vector theResult);
  
  /**
   * Computes the mean of the columns of theMatrix. Each column is taken
   * as a data point in n-dim. space. The result vector contains
   * in element i the variance of the i-th element of all columns.
   * @param theMatrix the data matrix 
   * @param theResult the mean of all columns of the matrix
   */
  public native void varianceOfColumns(Matrix theMatrix, Vector theResult);

  /**
   * Computes the covariance matrix of the rows of theMatrix. 
   * Each row is taken
   * as a data point in n-dim. space. The result matrix contains
   * the covariance matrix of the data matrix.
   * @param theMatrix the data matrix 
   * @param theResult the covariance matrix (returned)
   */
  public native void covarianceMatrixOfRows(Matrix theMatrix, 
                                            Matrix theResult);

  /**
   * Computes the mean of the columns of theMatrix. Each column is taken
   * as a data point in n-dim. space. The result vector contains
   * the covariance matrix of the data matrix.
   * @param theMatrix the data matrix 
   * @param theResult the covariance matrix (returned)
   */
  public native void covarianceMatrixOfColumns(Matrix theMatrix, 
                                               Matrix theResult);



  /**
   * Computes the covariance between two vectors a and b.
   * The result is the covariance of these two vectors. The vectors are
   * regarded as series of outcomes of a twodimensional random variable. 
   * This function computes the covariance of both dimensions of the 
   * variable.
   * @param a vector<T> with dimension one of the random variable.
   * @param b vector<T> with dimension two of the random variable.
   * @return covariance between a and b
   */
  public native double covariance(Vector a, Vector b);


  /**
   * Computes the bounding vectors of the rows of theMatrix.
   * @param theMatrix the data matrix
   * @param min the vector containing the minimum (returned)
   * @param max the vector containing the maximum (returned)
   */
  public native void boundsOfRows(Matrix theMatrix, Vector min, Vector max);
  
  /**
   * Computes the bounding vectors of the columns of theMatrix.
   * @param theMatrix the data matrix
   * @param min the vector containing the minimum (returned)
   * @param max the vector containing the maximum (returned)
   */
  public native void boundsOfColumns(Matrix theMatrix, 
                                     Vector min, Vector max);
  
  /**
   * Computes a vector which contains the minimum elements of a and b
   * @param a vector<T> first operand
   * @param b vector<T> second operand
   * @param result vector<T> which will receive the element-wise minimum 
   *          of both operands (returned)
   */
  public native void min(final Vector a, final Vector b, Vector result);
  
  /**
   * Computes a vector which contains the maximum elements of a and b
   * @param a vector<T> first operand
   * @param b vector<T> second operand
   * @param result vector<T> which will receive the element-wise maximum 
   *          of both operands (returned)
   */
  public native void max(final Vector a, final Vector b, Vector result);
  

  
  /**
   * Computes the entropy of the elements of the given vector, where the
   * elements are interpreted as probabilities (e.g. a discrete probability
   * distribution).
   * @param v vector<T> the given vector
   */
  public native double entropy(final Vector v);


  /**
   * Computes a local LDA of one class. The class data points are given in
   * the first matrix. The second matrix is the between-class scatter
   * matrix of the whole data set (including all classes). 
   * The transformed data is returned in the last matrix.
   * It returns the dimension of the result space.
   */
  public native int makeClassLDA(final Matrix data, Matrix Sb, int dim, Matrix transformedData);

  /**
   * Computes the between-class scatter matrix for a later use with 
   * {@link #makeClassLDA(Matrix,Matrix,int,Matrix) makeClassLDA}.
   */
  public native void computeSb(final Matrix a, final Vector labels, Matrix Sb);

  /**
   * Creates a new LDA of the first matrix and stored the transformation
   * in the second matrix.
   * It returns the dimension of the result space.
   */
  public native int makeLDA(final Matrix a, final Vector labels, int dim, Matrix b);

  /**
   * Re-Uses a previously computed LDA and transforms the given matrix.
   */
  public native void useLDA(final Matrix a, Matrix b);

  /**
   * Uses a previously computed LDA and transforms the given Vector.
   */
  public native void useLDA(final Vector a, Vector b);

  /**
   * Gets the current PCA and returns it into a string.
   */
  public native String getLDA();

  /**
   * Sets the PCA from the String.
   */
  public native void setLDA(String param);


  /**
   * Function for considering a data matrix of one class 
   * with the serial LDA functor. The dimension can be set by using the
   * useSeqLDA function.
   */
  public native int makeSeqLDA(final Matrix a);

  /**
   * Re-Uses a previously computed LDA and transforms the given matrix.
   */
  public native void useSeqLDA(final Matrix a, int dim, Matrix b);

  /**
   * Uses a previously computed LDA and transforms the given Vector.
   */
  public native void useSeqLDA(final Vector a, int dim, Vector b);

  /**
   * Gets the current PCA and returns it into a string.
   */
  public native String getSeqLDA();

  /**
   * Sets the PCA from the String.
   */
  public native void setSeqLDA(String param);

  /**
   * Computes a mixture model fitting the data.
   */
  public native boolean fitMixtureModel(Matrix data, Vector[] means, 
                                        Matrix[] sigmas, Vector alpha,
                                        double lambda, int iter);

  /**
   * Returns the status message of the last operation.
   * This is only meaningful if the last operation failed, ie. returned
   * false.
   */
  public native String getStatus();

}











