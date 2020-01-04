
package de.rwth_aachen.techinfo.ltilib;


/** This class is a representation of a collection of functors on the
 *  C++ side of the LTI library. When created, it creates all the necessary
 *  functor objects. Each method represents a certain functor. 
 */

public class UnaryFunctors extends Functors {

  /* static class methods ---------------------------------------- */

  /** Class initialization. */
  static {
    System.loadLibrary("ltiUnaryFunctors");
  }


  /* construction/finalization ----------------------------------- */

  /** Constructor. */
  public UnaryFunctors() {
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


  /* public methods  --------------------------------------------- */


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

}
