
package de.rwth_aachen.techinfo.ltilib;

/**
 * This is the gateway from the Java Vector class to the C++ lti::vector 
 * class. The reason for this gateway object is that Java does not seem to
 * be able to directly access a corresponding C++ class. So what this class
 * does is to create a C++ object via a C function call and pass a reference
 * to this object back to the Java object. The reference to the C++ object
 * can then be accessed by the C++ wrapper function by the JNI member access
 * methods.
 * @version $Id: RectLocation.java,v 1.7 2003/06/13 12:32:21 wickel Exp $
 */
public class RectLocation extends LibBase { 

  /* static class function ------------------------------------------ */

//   static {
//     System.loadLibrary("ltiInterface");
//   }

  /* constructors/finalization ------------------------------------- */

  /** Special constructor for creating a Java gateway object for an 
   *  existing C++ lti::vector object. The object reference is passed as
   *  parameter.
   *  @param cObject the C++ object reference
   */
  protected RectLocation(long cObject) {
    nativeObject=cObject;
    destroy=true;
  }

  /** Default constructor. Creates an empty vector.
   */
  public RectLocation() {
    openTheGate();
    destroy=true;
  }
  
  /**
   * Constructor of the vector object. It creates a vector from the
   * array provided as argument. All elements are copied
   * into the new vector.
   *  @param x the x-coordinate of the center of the new location
   *  @param y the y-coordinate of the center of the new location
   *  @param angle the orientation of the new location
   *  @param maxLength the length of the longer side of the area
   *  @param minLength the length of the shorter side of the area
   */
  public RectLocation(float x, float y, 
          float angle, float maxLength, float minLength) {
    this();

    setData(x,y,angle,maxLength,minLength);
  }

  /**
   * Copy Constructor of the vector object. It creates a vector from the
   * vector provided as argument. All elements are copied
   * into the new vector.
   *  @param data old vector
   */
  public RectLocation(RectLocation data) {
    super();
    openTheGate();
    destroy=true;
    copy(data);
  }

  /**
   * Finalizer method. 
   */
  public void finalize() throws Throwable {
    if (destroy) {
      closeTheGate();
    }
    super.finalize();
  }

  /* public methods ------------------------------------------------ */

  /**
   * Deletes the old data of the RectLocation and copies the new data to 
   * the C-side.
   * @param data new data
   */
  public native void setData(float x, float y, 
           float angle, float maxLength, float minLength); 


  /**
   * copies the contents of the other rectangular locationinto this one.
   *  @param other the RectLocation to be copied.
   */
  public native void copy(RectLocation other);

  /**
   * Check if the given point can be considered within the rectLocation.
   */
  public native boolean contains(int x, int y);

  /**
   * Check if the given point can be considered within the rectLocation.
   */
  public native boolean contains(float x, float y);
  
  /**
   * returns the square of the distance between the borders of two
   * locations or zero if they overlap or if one of the locations
   * lies inside the other one.  
   */
  public native float distance2(RectLocation other);

  /**
   * returns the square of the distance between the borders of two
   * locations or zero if they overlap or if one of the locations
   * lies inside the other one.
   *
   * @param other the other rectLocation to be compared with
   * @param pt point in the border of this location with the
   *           smallest distance. (x first element, y second element) 
   * @param po point in the border of this location with the
   *           smallest distance. (x first element, y second element)
   */
  public native float distance2(RectLocation other,
        float[] pt,
        float[] po);
    
  /**
   * Get the area of this location (maxLength*minLength)
   */
  public native float getArea();


  /**
   * Get the angle of this location.
   */
  public native float getAngle();

  /**
   * Get the axes lengths of this location. The given array must
   * have at least two elements when the function is called.
   * The first element is set to the maximum length, the second
   * one to the minimum length.
   */
  public native void getAxes(float[] xy);
  
  /**
   * Get the position of the center of this location. The given array must
   * have at least two elements when the function is called.
   * The first element is set to the x coordinate, the second
   * one to the y coordinate.
   */
  public native void getPosition(float[] xy);
  
  

  /* IO methods ---------------------------------------------------- */

  public String toString() {
//      StringBuffer buf=new StringBuffer();
//      double[] data=getArray(); 
//      for (int i=0; i<data.length; i++) {
//        buf.append(" ");
//        buf.append(data[i]);
//      }
//      return buf.toString();
    return "TODO";
  }

  /* private methods ----------------------------------------------- */

  /** 
   * Opens the gateway to the C++ side of this object. Basically, it is 
   * a C function that creates the C++ lti::rectLocation object and returns a
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
   * destroys the C++ lti::rectLocation object and sets the corresponding java
   * variable to zero. It is called by the finalize() method.
   */
  private native void closeTheGate();

  /**
   * copies the C data into an Java array
   *
   * The array must have 5 elements before calling this method.
   * data[0] will be the x position,
   * data[1] the y position,
   * data[2] the angle
   * data[3] maxLength and
   * data[4] minLength
   */
  private native void copyCtoJava(float[] data);


}









