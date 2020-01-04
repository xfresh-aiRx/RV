
package de.rwth_aachen.techinfo.ltilib;


/**
 * This class is a representation of a collection of functors on the
 * C++ side of the LTI library. When created, it creates all the necessary
 * functor objects. Each method represents a certain functor. 
 * @version $Id: IteratingFunctors.java,v 1.4 2001/03/15 15:14:23 wickel Exp $
 */

public class IteratingFunctors extends Functors {

  /* static class methods ---------------------------------------- */

  /** Class initialization. */
  static {
    System.loadLibrary("ltiIteratingFunctors");
  }


  /* construction/finalization ----------------------------------- */

  /** Constructor. */
  public IteratingFunctors() {
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

  /** Opens the gateway to the C++ side of this object. Basically, it is 
   *  a C function that creates the C++ lti::functorcollection object and 
   *  returns a
   *  pointer to this object to the Java side via a Java long variable. 
   *  This pointer is then accessed by each native function to retrieve
   *  the C++ object reference. We need to do this because the native
   *  functions must not have a state (thread-safety). It is called by the
   *  constructor of this object. Is sets the nativeObject attribute of
   *  the parent class.
   */
  private native void openTheGate(); 


  /** Closes the gateway to the C++ side of this object. Basically, it
   *  destroys the C++ lti::functorcollection object and sets the
   *  corresponding java
   *  variable to zero. It is called by the finalize() method.
   */
  private native void closeTheGate();


  /* public methods  --------------------------------------------- */


  /* elementwise absolute value */

  /**
   * Replaces each element of the Matrix with its absolute value.
   * @param ma Matrix
   * @see #absElem(Matrix, Matrix)
   * @see #absElem(Vector)
   * @see #absElem(Vector, Vector)
   */
  public native void absElem(Matrix ma);

  /**
   * Replaces each element of the Vector with its absolute value.
   * @param v Vector
   * @see #absElem(Vector, Vector)
   * @see #absElem(Matrix)
   * @see #absElem(Matrix, Matrix)
   */
  public native void absElem(Vector v);

  /**
   * Returns a Matrix whose elements are absolute value of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param ma original Matrix
   * @param res Matrix containing the absolut values 
   * @see #absElem(Matrix)
   * @see #absElem(Vector)
   * @see #absElem(Vector, Vector)
   */
  public native void absElem(Matrix ma, Matrix res);

  /**
   * Returns a Vector whose elements are absolute value of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param v original Vector
   * @param res Vector containing the absolut values 
   * @see #absElem(Vector, Vector)
   * @see #absElem(Matrix)
   * @see #absElem(Matrix, Matrix)
   */
  public native void absElem(Vector v, Vector res);


  /* elementwise logarithm */

  /**
   * Replaces each element of the Matrix with its logarithm.
   * @param ma Matrix
   * @see #logElem(Matrix, Matrix)
   * @see #logElem(Vector)
   * @see #logElem(Vector, Vector)
   */
  public native void logElem(Matrix ma);

  /**
   * Replaces each element of the Vector with its logarithm.
   * @param v Vector
   * @see #logElem(Vector, Vector)
   * @see #logElem(Matrix)
   * @see #logElem(Matrix, Matrix)
   */
  public native void logElem(Vector v);

  /**
   * Returns a Matrix whose elements are the logarithm of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param ma original Matrix
   * @param res Matrix containing the absolut values 
   * @see #logElem(Matrix)
   * @see #logElem(Vector)
   * @see #logElem(Vector, Vector)
   */
  public native void logElem(Matrix ma, Matrix res);

  /**
   * Returns a Vector whose elements are the logarithm of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param v original Vector
   * @param res Vector containing the absolut values 
   * @see #logElem(Vector, Vector)
   * @see #logElem(Matrix)
   * @see #logElem(Matrix, Matrix)
   */
  public native void logElem(Vector v, Vector res);


  /* elementwise square */

  /**
   * Replaces each element of the Matrix with its square.
   * @param ma Matrix
   * @see #squareElem(Matrix, Matrix)
   * @see #squareElem(Vector)
   * @see #squareElem(Vector, Vector)
   */
  public native void squareElem(Matrix ma);

  /**
   * Replaces each element of the Vector with its square.
   * @param v Vector
   * @see #squareElem(Vector, Vector)
   * @see #squareElem(Matrix)
   * @see #squareElem(Matrix, Matrix)
   */
  public native void squareElem(Vector v);

  /**
   * Returns a Matrix whose elements are the square of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param ma original Matrix
   * @param res Matrix containing the absolut values 
   * @see #squareElem(Matrix)
   * @see #squareElem(Vector)
   * @see #squareElem(Vector, Vector)
   */
  public native void squareElem(Matrix ma, Matrix res);

  /**
   * Returns a Vector whose elements are the square of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param v original Vector
   * @param res Vector containing the absolut values 
   * @see #squareElem(Vector, Vector)
   * @see #squareElem(Matrix)
   * @see #squareElem(Matrix, Matrix)
   */
  public native void squareElem(Vector v, Vector res);


  /* elementwise square root */

  /**
   * Replaces each element of the Matrix with its square root.
   * @param ma Matrix
   * @see #sqrtElem(Matrix, Matrix)
   * @see #sqrtElem(Vector)
   * @see #sqrtElem(Vector, Vector)
   */
  public native void sqrtElem(Matrix ma);

  /**
   * Replaces each element of the Vector with its square root.
   * @param v Vector
   * @see #sqrtElem(Vector, Vector)
   * @see #sqrtElem(Matrix)
   * @see #sqrtElem(Matrix, Matrix)
   */
  public native void sqrtElem(Vector v);

  /**
   * Returns a Matrix whose elements are the square root of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param ma original Matrix
   * @param res Matrix containing the absolut values 
   * @see #sqrtElem(Matrix)
   * @see #sqrtElem(Vector)
   * @see #sqrtElem(Vector, Vector)
   */
  public native void sqrtElem(Matrix ma, Matrix res);

  /**
   * Returns a Vector whose elements are the square root of the elements of 
   * <b>ma</b> in <b>res</b>.
   * @param v original Vector
   * @param res Vector containing the absolut values 
   * @see #sqrtElem(Vector, Vector)
   * @see #sqrtElem(Matrix)
   * @see #sqrtElem(Matrix, Matrix)
   */
  public native void sqrtElem(Vector v, Vector res);

}








