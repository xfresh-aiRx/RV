
package de.rwth_aachen.techinfo.ltilib;

/**
 * This is the gateway from the Java Vector class to the C++ lti::vector 
 * class. The reason for this gateway object is that Java does not seem to
 * be able to directly access a corresponding C++ class. So what this class
 * does is to create a C++ object via a C function call and pass a reference
 * to this object back to the Java object. The reference to the C++ object
 * can then be accessed by the C++ wrapper function by the JNI member access
 * methods.
 * @version $Id: Vector.java,v 1.23 2003/10/01 19:59:58 wickel Exp $
 */
public class Vector extends MathObject { 

  /* static class function ------------------------------------------ */

  /* constructors/finalization ------------------------------------- */

  /** Special constructor for creating a Java gateway object for an 
   *  existing C++ lti::vector object. The object reference is passed as
   *  parameter.
   *  @param cObject the C++ object reference
   */
  protected Vector(long cObject) {
    nativeObject=cObject;
    destroy=true;
  }

  /** Default constructor. Creates an empty vector.
   */
  public Vector() {
    openTheGate(0,0.0);
    destroy=true;
  }
  
  /** Constructor. Creates an empty vector. */
  public Vector(int size) {
    openTheGate(size,0.0);
    destroy=true;
  }

  /**
   * Constructor of the vector object. It creates a vector from the
   * array provided as argument. All elements are copied
   * into the new vector.
   *  @param data data array
   */
  public Vector(double[] data) {
    this();

    setData(data);
//      int n=data.length;
//      openTheGate(n,0.0);
//      destroy=true;
//      for (int i=0; i<n; i++) {
//        setAt(i,data[i]);
//      }
  }

  /**
   * Copy Constructor of the vector object. It creates a vector from the
   * vector provided as argument. All elements are copied
   * into the new vector.
   *  @param data old vector
   */
  public Vector(Vector data) {
    super();
    int n=data.size();
    openTheGate(n,0.0);
    destroy=true;
    copy(data);
  }

  /** Constructor. Creates a vector of the desired size and fills it with
   *  the given value. 
   */
  public Vector(int size, double iniValue) {
    openTheGate(size,iniValue);
    destroy=true;
  }
  
  /**
   * Finalizer method. Is called by the garbage collector and 
   * frees the associated C++ lti::vector object.
   */
  public void finalize() throws Throwable {
    if (destroy) {
      closeTheGate();
    }
    super.finalize();
  }

  /* public methods ------------------------------------------------ */

  /** Resizes the matrix to the desired size and defines the new elements.
   *  @param newsize the desired number of elements.
   *  @param iniValue the value to which newly allocated matrix elements
   *                  are set.
   */
  public void resize(int newsize, double iniValue) {
    _resize(nativeObject, newsize, iniValue);
  }

  /**
   * Returns the data of the Vector as a Java array. The data are
   * copied.
   * @return an array whose size equals the vector size.
   */
  public float[] getFloatArray() {
    int n=size();
    float[] data=new float[n];
    if (n > 0) {
      copyCtoJava(data,n);
    }
    return data;
  }


  /**
   * Returns the data of the Vector as a Java array. The data are
   * copied.
   * @return an array whose size equals the vector size.
   */
  public double[] getArray() {
    int n=size();
    double[] data=new double[n];
    if (n > 0) {
      copyCtoJava(data,n);
    }
    return data;
  }
  

  /**
   * Deletes the old data of the Vector and copies the new data to 
   * the C-side. The vector is resized
   * @param data array with new data. All elements are used for the new
   *        vector.
   */
  public void setData(double[] data) {
    _setData(nativeObject, data);
  }

  /**
   * Deletes the old data of the Vector and copies the new data to 
   * the C-side. The vector is resized and all elements are converted
   * to doubles.
   * @param data array with new data. All elements are used for the new
   *        vector.
   */
  public void setData(float[] data) {
    _setData(nativeObject, data);
  }


  /**
   * Determines the size of the vector.
   * @return vector dimension.
   */
  public int size() {
    return _size(nativeObject);
  }

  /**
   * Fills the entire vector with the desired value. 
   * @param iniValue the value to which the vector elements are set.
   */
  public void fill(double iniValue) {
    _fill(nativeObject, iniValue);
  }

  /**
   * Fills a part of the vector with the desired value.
   * @param iniValue the value to which the vector elements are set.
   * @param from the first vector element which is set.
   * @param to the last element which is set.
   */
  public void fill(double iniValue, int from, int to) {
    _fill(nativeObject, iniValue, from, to);
  }

  /**
   * Returns the vector element at the desired position. 
   * @param pos the position of the desired vector element.
   * @return the vector element at the given position.
   */
  public double at(int pos) {
    return _at(nativeObject, pos);
  }

  /**
   * Sets the vector element at the given position.
   * @param pos the position of the desired vector element.
   * @param x the value to which the element should be set.
   */
  public void setAt(int pos, double x) {
    _setAt(nativeObject, pos, x);
  }

  /**
   * Copies the contents of the other vector into this vector.
   * The data are copied using only C++ methods, so this is 
   * quite fast.
   * @param other the vector which is to be copied.
   */
  public void copy(Vector other) {
    _copy(nativeObject, other.nativeObject);
  }

  /**
   * Sorts the Vector in ascending order. The method used for sorting
   * is unspecified.
   */
  public void sortAscending() {
    _sortAscending(nativeObject);
  }

  /**
   * Sorts the Vector in descending order. The method used for sorting
   * is unspecified.
   */
  public void sortDescending() {
    _sortDescending(nativeObject);
  }


  /* arithmetic operations ------------------------------------------ */
  
  /**
   * Computes the dot product of this vector with the other vector.
   * The given vector needs to have the same size as this vector.
   * @param other the second operand of the dot product.
   * @return the dot product of both vectors.
   */
  public double dot(Vector other) {
    return _dot(nativeObject, other.nativeObject);
  }

  /**
   * Performs element-wise multiplication of this vector and the
   * other vector. 
   * The given vector needs to have the same size as this vector.
   * @param other the second operand of the operation.
   */
  public void emultiply(Vector other) {
    _emultiply(nativeObject, other.nativeObject);
  }

  /**
   * Performs element-wise multiplication of both given vectors.
   * Both vectors need to have the same size. The size of the
   * result vector (i.e. this) will be adapted if necessary.
   * The result is stored in this vector.
   * @param first the first operand of the operation.
   * @param other the second operand of the operation.
   */
  public void emultiply(Vector first, Vector other) {
    _emultiply(nativeObject, first.nativeObject, other.nativeObject);
  }

  /** 
   * Performs element-wise division of this vector by the
   * other vector. The other vector needs to have the same size
   * as this vector.
   * @param other the second operand of the operation.
   */
  public void edivide(Vector other) {
    _edivide(nativeObject, other.nativeObject);
  }

  /**
   * Performs element-wise division of the first given vector by
   * the second one. Both given vectors need to have the same size, 
   * otherwise something undefined will occur.
   * The result is stored in this vector.
   * @param first the first operand of the operation.
   * @param other the second operand of the operation.
   */
  public void edivide(Vector first, Vector other) {
    _edivide(nativeObject, first.nativeObject, other.nativeObject);
  }

  /**
   * Computes the sum of all vector elements. 
   * @return the sum of all vector elements.
   */
  public double sumOfElements() {
    return _sumOfElements(nativeObject);
  }

  /**
   * Computes the product of all vector elements.
   * @return the product of all vector elements.
   */
  public double productOfElements() {
    return _productOfElements(nativeObject);
  }

  /**
   * Adds another vector to this vector. The caller has to ensure that
   * the other vector has the same size as this vector. At present,
   * the JVM will crash if this condition is not met.
   * @param other the second operand of the addition.
   */
  public void add(Vector other) {
    _add(nativeObject, other.nativeObject);
  }

  /**
   * Adds another vector, multiplied by some factor, to this vector.
   * @param other the second operand of the addition.
   */
  public void addScaled(double s, Vector other) {
    _addScaled(nativeObject, s, other.nativeObject);
  }

  /**
   * Adds both vectors and stores the results in this vector. Both given
   * vectors need to have the same size.
   * @param first the first operand of the addition.
   * @param second the second operand of the addition.
   */
  public void add(Vector first, Vector second) {
    _add(nativeObject, first.nativeObject, second.nativeObject);
  }

  /** 
   * Subtracts another vector from this vector. The caller has to
   * ensure that the other vector has the same size as this vector. At
   * present, the JVM will crash if this condition is not met.
   * @param other the second operand of the subtraction
   */
  public void subtract(Vector other) {
    _subtract(nativeObject, other.nativeObject);
  }

  /** 
   * Subtracts the second from the first vector and stores the result
   * in this vector. Both given
   * vectors need to have the same size.
   * @param first the first operand of the subtraction.
   * @param second the second operand of the subtraction.
   */
  public void subtract(Vector first, Vector second) {
    _subtract(nativeObject, first.nativeObject, second.nativeObject);
  }

  /** Multiplies the elements of this vector with the given constant.
   *  @param s the scaling factor.
   */
  public void multiply(double s) {
    _multiply(nativeObject, s);
  }

  /** Divides the elements of this vector by the given constant.
   *  @param s the divisor
   */
  public void divide(double s) {
    _divide(nativeObject, s);
  }

  /** Adds the given constant to the elements of this vector.
   *  @param s the shifting factor.
   */
  public void add(double s) {
    _add(nativeObject, s);
  }

  /**
   * Compares the given vector to this vector and returns true
   * if both are equal. The given vector needs to have the same
   * size as this vector.
   * @return true if both vectors are equal (up to machine precision),
   * false if not.
   */
  public boolean equals(Vector other) {
    return _equals(nativeObject, other.nativeObject);
  }

  /**
   * Compares the given vector to this vector and returns true
   * if all elements are less than <tt>tolerance</tt> apart.
   * @param other vector which is compared to this vector.
   * @param tolerance "soft equality" parameter
   * @return true if all elements are less than "tolerance" apart
   */
  public boolean prettyCloseTo(Vector other, double tolerance) {
    return _prettyCloseTo(nativeObject, other.nativeObject, tolerance);
  }


  /**
   * Finds the largest element of this vector.
   * @return the largest element of this vector.
   */
  public double maximum() {
    return _maximum(nativeObject);
  }

  /**
   * Finds the largest Element of this vector and returns its index.
   * @return the index of the largest element of this vector.
   */
  public int getIndexOfMaximum() {
    return _getIndexOfMaximum(nativeObject);
  }

  /**
   * Finds the smallest element of this vector
   * @return the smallest element of this vector.
   */
  public double minimum() {
    return _minimum(nativeObject);
  }

  /**
   * Finds  the smallest element of this vector and returns its index.
   * Please note that "smallest" here is meant mathematically, not the
   * "smallest positive" often used in numerics libraries.
   * @return the index of the smallest element of this vector.
   */
  public int getIndexOfMinimum() {
    return _getIndexOfMinimum(nativeObject);
  }

  /**
   * Concatenates all vectors and sets this vector. to the concatenation.
   * @param source an array of source vectors. This vector will be resized
   * to source[0].size()+...+source[source.length-1].size().
   */
  public void concat(Vector[] source) {
    _concat(nativeObject, source);
  }


  /**
   * Normalizes the vector such that all elements lie between low and high.
   * @param low the lower bound of the target interval
   * @param high the higher bound of the target interval
   */
  public void normalize(double low, double high) {
    _normalize(nativeObject, low, high);
  }
  
  /**
   * Normalizes the vector such that it has the given L2-norm.
   * @param length the desired L2-norm of the vector.
   */
  public void normalize(double length) {
    _normalize(nativeObject, length);
  }

  
  /**
   * Normalize the vector such that the sum of all elements is s.
   * All elements are scaled equally.
   * @param s the desired sum.
   */
  public void normalizeSum(double s) {
    _normalizeSum(nativeObject, s);
  }

  /**
   * Returns a subvector containing the  elements whose indices
   * are contained in the index vector.
   * @param index the index vector. 
   */
  public Vector subvector(int[] index) {
    return new Vector(csubvector(index));
  }


  /* IO methods ---------------------------------------------------- */

  public String toString() {
    StringBuffer buf=new StringBuffer();
    double[] data=getArray(); 
    for (int i=0; i<data.length; i++) {
      buf.append(" ");
      buf.append(data[i]);
    }
    return buf.toString();
  }

  /* private methods ----------------------------------------------- */

  /** 
   * Opens the gateway to the C++ side of this object. Basically, it is 
   * a C function that creates the C++ lti::matrix object and returns a
   * pointer to this object to the Java side via a Java long variable. 
   * This pointer is then accessed by each native function to retrieve
   * the C++ object reference. We need to do this because the native
   * functions must not have a state (thread-safety). It is called by the
   * constructor of this object. Is sets the nativeObject attribute of
   * the parent class.
   */
  private native void openTheGate(int size, double iniValue); 

  /**
   * Closes the gateway to the C++ side of this object. Basically, it
   * estroys the C++ lti::matrix object and sets the corresponding java
   * variable to zero. It is called by the finalize() method.
   */
  private native void closeTheGate();

  /**
   * copies the C data into an Java array
   */
  private native void copyCtoJava(double[] data, int n);

  /**
   * copies the C data into an Java array
   */
  private native void copyCtoJava(float[] data, int n);

  protected native int csubvector(int[] index);


  /* protected native interface methods ------------------------------ */
  // these are not commented...

  protected static native void _resize(long nat, int newsize, double iniValue);

  protected static native void _setData(long nat, double[] data);

  protected static native void _setData(long nat, float[] data);

  protected static native int _size(long nat);

  protected static native void _fill(long nat, double iniValue);

  protected static native void _fill(long nat, double iniValue, int from, int to);

  protected static native double _at(long nat, int pos);

  protected static native void _setAt(long nat, int pos, double x);

  protected static native void _copy(long nat, long other);

  protected static native void _sortAscending(long nat);

  protected static native void _sortDescending(long nat);

  protected static native double _dot(long nat, long other);

  protected static native void _emultiply(long nat, long other);

  protected static native void _emultiply(long nat, long first, long other);

  protected static native void _edivide(long nat, long other);

  protected static native void _edivide(long nat, long first, long other);

  protected static native double _sumOfElements(long nat);

  protected static native double _productOfElements(long nat);

  protected static native void _add(long nat, long other);

  protected static native void _addScaled(long nat, double s, long other);

  protected static native void _add(long nat, long first, long second);

  protected static native void _subtract(long nat, long other);

  protected static native void _subtract(long nat, long first, long second);

  protected static native void _multiply(long nat, double s);

  protected static native void _divide(long nat, double s);

  protected static native void _add(long nat, double s);

  protected static native boolean _equals(long nat, long other);

  protected static native boolean _prettyCloseTo(long nat, long other, double tolerance);

  protected static native double _maximum(long nat);

  protected static native int _getIndexOfMaximum(long nat);

  protected static native double _minimum(long nat);

  protected static native int _getIndexOfMinimum(long nat);

  protected static native void _concat(long nat, Vector[] source);

  protected static native void _normalize(long nat, double low, double high);

  protected static native void _normalize(long nat, double length);

  protected static native void _normalizeSum(long nat, double s);
}



