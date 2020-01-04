
package de.rwth_aachen.techinfo.ltilib;

import java.lang.*;

/** 
 * This is the gateway from the Java Matrix class to the C++ lti::matrix 
 * class. The reason for this gateway object is that Java does not seem to
 * be able to directly access a corresponding C++ class. So what this class
 * does is to create a C++ object via a C function call and pass a reference
 * to this object back to the Java object. The reference to the C++ object
 * can then be accessed by the C++ wrapper function by the JNI member access
 * methods.
 * @version $Id: Matrix.java,v 1.23 2003/10/01 19:59:58 wickel Exp $
 */

public class Matrix extends MathObject { 

  /* static class function ------------------------------------------ */

//   static {
//     loadNativeLibrary();
//   }

  /* constructors/finalization -------------------------------------- */
  
  /** Constructor of the matrix object. It creates an empty matrix. 
   */
  public Matrix() {
    super();
    openTheGate(0,0,0.0);
    destroy=true;
  }

  /**
   * Copy Constructor of the matrix object. It creates a copy of
   * the other matrix. 
   */
  public Matrix(final Matrix other) {
    super();
    openTheGate(other.rows(),other.columns(),0.0);
    destroy=true;
    copy(other);
  }


  /**
   * a special constructor for creating a Java gateway object for a
   * C++ lti::matrix object handle.
   */
  protected Matrix(long cObject) {
    nativeObject=cObject;
    destroy=true;
  }

  /**
   * Constructor of the matrix object. It creates a matrix of the desired 
   * size. The matrix elements are initialized to zero.
   * @param rows number of rows of the matrix.
   * @param cols number of columns of the matrix.
   */
  public Matrix(int rows, int cols) {
    super();
    openTheGate(rows,cols,0.0);
    destroy=true;
  }

  /**
   * Constructor of the matrix object. It creates a matrix from the
   * two-dimensional array provided as argument. All elements are copied
   * into the new matrix.
   *  @param data data array
   */
  public Matrix(double[][] data) {
    this();
    setData(data);

//      int rows=data.length;
//      int columns;
//      if (rows > 0) {
//        columns=data[0].length;
//      } else {
//        columns=0;
//      }
//      openTheGate(rows,columns,0.0);
//      destroy=true;
//      for (int i=0; i<rows; i++) {
//        for (int j=0; j<columns; j++) {
//          setAt(i,j,data[i][j]);
//        }
//      }
  }

  /** Constructor of the matrix object. It creates a matrix of the desired
   *  size and initializes it to the desired value.
   *  @param rows number of rows of the matrix.
   *  @param cols number of columns of the matrix.
   *  @param iniValue value to which each matrix element is initialized.
   */
  public Matrix(int rows, int cols, double iniValue) {
    super();
    openTheGate(rows,cols,iniValue);
    destroy=true;
  }

  /**
   * Destroys the matrix object on the C++ side. It is called implicitly
   * by the destructor. 
   */
  public void finalize() throws Throwable {
    if (destroy) {
      closeTheGate();
    }
    super.finalize();
  }

  /* public methods ------------------------------------------------ */

  /**
   * Sets the lti::matrix elements to data
   * @param data the new matrix elements. All rows of the array must
   * have the same length.
   */
  public void setData(double[][] data) {
    _setData(nativeObject, data);
  }

  /**
   * Sets the lti::matrix elements to data
   * @param data the new matrix elements. All rows of the array must
   * have the same length. The data will be converted to doubles internally.
   */
  public void setData(float[][] data) {
    _setData(nativeObject, data);
  }

  /**
   * Returns the number of rows of the matrix.
   * @return the number of rows of the matrix.
   */
  public int rows() {
    return _rows(nativeObject);
  }

  /**
   * Returns the number of columns of the matrix.
   * @return the number of columns of the matrix.
   */
  public int columns() {
    return _columns(nativeObject);
  }

  /**
   * Resizes the matrix to the desired size and defines the new elements.
   * @param newRows the desired number of rows.
   * @param newCols the desired number of columns.
   * @param iniValue the value to which newly allocated matrix elements
   *                 are set.
   */
  public void resize(int newRows, int newCols, double iniValue) {
    _resize(nativeObject, newRows, newCols, iniValue);
  }

  /**
   * Fills a part of the matrix with the desired value.
   * @param iniValue the value that is to be written into the submatrix.
   * @param fromRow  the first row of the submatrix.
   * @param fromColumn the first column of the submatrix.
   * @param toRow the last row of the submatrix.
   * @param toColumn   the last column of the submatrix.
   */
  public void fill(double iniValue, int fromRow, int fromColumn, int toRow, int toColumn) {
    _fill(nativeObject, iniValue, fromRow, fromColumn, toRow, toColumn);
  }


  /** 
   * Fills the entire matrix with a value. 
   * @param iniValue the value that is to be written into the matrix.
   */
  public void fill(double iniValue) {
    _fill(nativeObject, iniValue);
  }

  /** 
   * Returns the element of the matrix at the desired position.
   * @param row the row position of the desired element.
   * @param col the column position of the desired element.
   * @return the value of the matrix at the given position.
   */
  public double at(int row, int col) {
    return _at(nativeObject, row, col);
  }

  /**
   * Sets the element of the matrix at the desired position.
   * @param row the row position of the desired element.
   * @param col the column position of the desired element.
   * @param value the desired value of the matrix at the given position.
   */
  public void setAt(int row, int col, double value) {
    _setAt(nativeObject, row, col, value);
  }

  /** 
   * Returns the minimum of all elements in the matrix.
   * @return the smallest value contained in the matrix.
   */
  public double minimum() {
    return _minimum(nativeObject);
  }

  /** 
   * Returns the minimum of all elements in the matrix.
   * In the given array, which must be two-dimensional, the
   * index of the minimum element is returned.
   * @return the smallest value contained in the matrix.
   */
  public double minimum(int[] index) {
    return _minimum(nativeObject, index);
  }

  /** 
   * Returns the maximum of all elements in the matrix.
   * @return the largest value contained in the matrix.
   */
  public double maximum() {
    return _maximum(nativeObject);
  }

  /** 
   * Returns the maximum of all elements in the matrix.
   * In the given array, which must have two elements, the
   * index of the maximum element is returned: index[0] will
   * receive the column index, index[1] the row index.
   * @return the largest value contained in the matrix.
   */
  public double maximum(int[] index) {
    return _maximum(nativeObject, index);
  }

  /** 
   * Returns the trace of the matrix.
   * @return the sum of the diagonal elements of the matrix
   */
  public double trace() {
    return _trace(nativeObject);
  }

  /** 
   * Return a new vector with the contents of the desired row.
   * @param row the row number.
   * @return a new vector with the contents of the row.
   */
  public Vector getRow(int row) {
    return new Vector(_getRow(nativeObject,row));
  }

  /**
   * return a new matrix with the contents of the desired rows.
   * @param rows the row numbers.
   * @return a new matrix with the contents of the row.
   */
  public Matrix getRows(int[] rows) {
    return new Matrix(_getRows(nativeObject,rows));
  }

  /** 
   * Return a new matrix with the contents of the desired columns.
   * @param cols the column indices.
   * @return a new matrix with the contents of the columns.
   */
  public Matrix getColumns(int[] cols) {
    return new Matrix(_getColumns(nativeObject,cols));
  }


  /**
   * Returns a new 2dim double array with the contents of the matrix.
   * @return a new array with the contents of the matrix.
   */
  public double[][] getArray() {
    int r=rows();
    int c=columns();
    double[][] data=new double[r][c];
    if (r > 0 && c > 0) {
      copyCtoJava(data,r,c);
    }
    return data;
  }

  /**
   * Returns a new 2dim double array with the contents of the matrix.
   * @return a new array with the contents of the matrix.
   */
  public float[][] getFloatArray() {
    int r=rows();
    int c=columns();
    float[][] data=new float[r][c];
    if (r > 0 && c > 0) {
      copyCtoJava(data,r,c);
    }
    return data;
  }

  /** return a new vector with the contents of the desired column.
   *  @param column the column number.
   *  @return a new vector with the contents of the column.
   */
  public Vector getColumn(int column) {
    return new Vector(_getColumn(nativeObject,column));
  }

  /**
   * Copies the diagonal elemnts of the matrix into the given vector.
   * @param result the vector which will contan the diagonal elements.
   */
  public void getDiagonal(Vector result) {
    _getDiagonal(nativeObject, result.nativeObject);
  }

  /** set the desired row to the contents of the other vector. 
   *  @param row the row which should be set.
   *  @param theRow the vector which is to be copied into the matrix.
   */
  public void setRow(int row, Vector theRow) {
    _setRow(nativeObject, row, theRow.nativeObject);
  }

  /** set the desired row to the contents of the other vector. 
   *  @param row the row which should be set.
   *  @param theRow the array which is to be copied into the matrix.
   */
  public void setRow(int row, double[] theRow) {
    for (int i=0; i<columns(); i++) {
      setAt(row,i,theRow[i]);
    }
  }

  /** set the desired column to the contents of the other vector. 
   *  @param col the column which should be set.
   *  @param theColumn the vector which is to be copied into the matrix.
   */
  public void setColumn(int col, Vector theColumn) {
    _setColumn(nativeObject, col, theColumn.nativeObject);
  }

  /** set the desired column to the contents of the other vector. 
   *  @param col the column which should be set.
   *  @param theColumn the vector which is to be copied into the matrix.
   */
  public void setColumn(int col, double[] theColumn) {
    for (int i=0; i<rows(); i++) {
      setAt(i,col,theColumn[i]);
    }
  }

  /** Copy the contents of the desired sub-matrix into this matrix.
   *  @param other the matrix from which the elements should be taken.
   *  @param fromRow the first row of the sub-matrix.
   *  @param toRow   the last row of the sub-matrix.
   *  @param fromColumn the first column of the sub-matrix.
   *  @param toColumn the last column of the sub-matrix.
   */
  public void copy(Matrix other, int fromRow, int toRow, int fromColumn, int toColumn) {
    _copy(nativeObject, other.nativeObject, fromRow, toRow, fromColumn, toColumn);
  }
  
  /** Copy the contents of the other matrix into this matrix. 
   *  @param other the matrix which is to be copied.
   */
  public void copy(Matrix other) {
    _copy(nativeObject, other.nativeObject);
  }

  /** Compares the matrix to the other matrix. Return true if both are equal.
   *  @param other the Matrix which is to be compared.
   *  @return true if both matrices are equal.
   */
  public boolean compare(Matrix other) {
    return _compare(nativeObject, other.nativeObject);
  }


  /* arithmetic operations ----------------------------------------- */

  /** Performs element-wise multiplication of this matrix and the
   *  other matrix. 
   *  @param other the second operand of the operation.
   */
  public void emultiply(Matrix other) {
    _emultiply(nativeObject, other.nativeObject);
  }

  /** Performs element-wise multiplication of both given matrices.
   *  The result is stored in this matrix.
   *  @param first the first operand of the operation.
   *  @param other the second operand of the operation.
   */
  public void emultiply(Matrix first, Matrix other) {
    _emultiply(nativeObject, first.nativeObject, other.nativeObject);
  }

  /** Adds another matrix to this matrix.
   *  @param other the second operand of the addition.
   */
  public void add(Matrix other) {
    _add(nativeObject, other.nativeObject);
  }

  /** Adds both matrices and stores the results in this matrix.
   *  @param first the first operand of the addition.
   *  @param second the second operand of the addition.
   */
  public void add(Matrix first, Matrix second) {
    _add(nativeObject, first.nativeObject, second.nativeObject);
  }

  /** Adds the given constant to the elements of this matrix.
   *  @param s the shifting factor.
   */
  public void add(double s) {
    _add(nativeObject, s);
  }

  /** Subtracts another Matrix from this Matrix.
   *  @param other the second operand of the subtraction
   */
  public void subtract(Matrix other) {
    _subtract(nativeObject, other.nativeObject);
  }

  /** Subtracts the second from the first matrix and stores the result
      in this matrix.
   *  @param first the first operand of the subtraction.
   *  @param second the second operand of the subtraction.
   */
  public void subtract(Matrix first, Matrix second) {
    _subtract(nativeObject, first.nativeObject, second.nativeObject);
  }

  /** Multiplies the elements of this matrix with the given constant.
   *  @param s the scaling factor.
   */
  public void multiply(double s) {
    _multiply(nativeObject, s);
  }
 
  /**
   * Multiplies this matrix with the argument, leaving the result in this
   * Matrix.
   * @param other Matrix to be multiplied with this matrix
   */
  public void multiply(Matrix other) {
    _multiply(nativeObject, other.nativeObject);
  }

  /**
   * Multiplies the two matrices and leaves the result in this matrix.
   * @param first first factor
   * @param second second factor
   */
  public void multiply(Matrix first, Matrix second) {
    _multiply(nativeObject, first.nativeObject, second.nativeObject);
  }

  /**
   * Multiplies this matrix with the given vector <b>other</b> and returns the
   * result in the argument <b>result</b>.
   * @param other vector to be multiplied
   * @param result result of the multiplication
   */
  public void multiply(Vector other, Vector result) {
    _multiplyV(nativeObject, other.nativeObject, result.nativeObject);
  }

  /**
   * Multiplies the given vector <b>other</b> with this matrix and returns the
   * result in the argument <b>result</b>.
   * @param other vector to be multiplied
   * @param result result of the multiplication
   */
  public void leftMultiply(Vector other, Vector result) {
    _leftMultiply(nativeObject, other.nativeObject, result.nativeObject);
  }

  /**
   * Multiplies the given vector <b>srcDest</b> with this matrix and returns the
   * result in the argument <b>srcDest</b>.
   * @param srcDest argument and result of the multiplication
   */
  public void leftMultiply(Vector srcDest) {
    _leftMultiply(nativeObject, srcDest.nativeObject);
  }

  /**
   * Calculates the outer product of two vectors and leaves the result in this
   * matrix.
   * @param first first factor
   * @param second second factor
   */
  public void outerProduct(Vector first, Vector second) {
    _outerProduct(nativeObject, first.nativeObject, second.nativeObject);
  }
 
  /** Transpose this matrix.
   */
  public void transpose() {
    _transpose(nativeObject);
  }

  /**
   * Compares the given vector to this vector and returns true
   * if both are equal
   */
  public boolean equals(Matrix other) {
    return _equals(nativeObject, other.nativeObject);
  }

  /**
   * Compares the given vector to this vector and returns true
   * if all elements are less than <tt>tolerance</tt> apart
   */
  public boolean prettyCloseTo(Matrix other, double tolerance) {
    return _prettyCloseTo(nativeObject, other.nativeObject, tolerance);
  }

  
  /* IO methods ---------------------------------------------------- */

  public String toString() {
    double[][] data=getArray();
    StringBuffer buf=new StringBuffer();

    for (int i=0; i<data.length; i++) {
      for (int j=0; j<data[i].length; j++) {
        buf.append(" ");
        buf.append(data[i][j]);
      }
    }
    return buf.toString();
  }
  
 
  /* private methods ----------------------------------------------- */

  /** Internal function for calling the lti::matrix::getRow function.
   *  @return a holder variable for the reference of the C++ object. 
   */ 
  protected native long _getRow(long nat, int row);

  /** Internal function for calling the lti::matrix::getColumn function.
   *  @return a holder variable for the reference of the C++ object. 
   */ 
  protected native long _getColumn(long nat, int column);

  /**
   * Internal function for creating a submatric containing only the
   * given rows.
   *  @return a holder variable for the reference of the C++ object. 
   */ 
  protected native long _getRows(long nat, int[] rows);

  /** 
   * Internal function for creating a submatrix containing only
   * the given columns.
   * @return a holder variable for the reference of the C++ object. 
   */ 
  protected native long _getColumns(long nat, int[] column);

  /** Opens the gateway to the C++ side of this object. Basically, it is 
   *  a C function that creates the C++ lti::matrix object and returns a
   *  pointer to this object to the Java side via a Java long variable. 
   *  This pointer is then accessed by each native function to retrieve
   *  the C++ object reference. We need to do this because the native
   *  functions must not have a state (thread-safety). It is called by the
   *  constructor of this object. Is sets the nativeObject attribute of
   *  the parent class.
   */
  private native void openTheGate(int rows, int cols, double iniValue); 

  /** Closes the gateway to the C++ side of this object. Basically, it
   *  destroys the C++ lti::matrix object and sets the corresponding java
   *  variable to zero. It is called by the finalize() method.
   */
  private native void closeTheGate();

  /**
   * Copies the C matrix data into the given array
   */
  private native void copyCtoJava(double[][] data, int rows, 
                                         int columns);

  /**
   * Copies the C matrix data into the given array
   */
  private native void copyCtoJava(float[][] data, int rows, 
                                         int columns);

  /* protected native interface methods ------------------------------ */

  protected native void _setData(long nat, double[][] data);

  protected native void _setData(long nat, float[][] data);

  protected native int _rows(long nat);

  protected native int _columns(long nat);

  protected native void _resize(long nat, int newRows, int newCols, double iniValue);

  protected native void _fill(long nat, double iniValue, int fromRow, int fromColumn, int toRow, int toColumn);

  protected native void _fill(long nat, double iniValue);

  protected native double _at(long nat, int row, int col);

  protected native void _setAt(long nat, int row, int col, double value);

  protected native double _minimum(long nat);

  protected native double _minimum(long nat, int[] index);

  protected native double _maximum(long nat);

  protected native double _maximum(long nat, int[] index);

  protected native double _trace(long nat);

  protected native void _getDiagonal(long nat, long v_result);

  protected native void _setRow(long nat, int row, long v_theRow);

  protected native void _setColumn(long nat, int col, long v_theColumn);

  protected native void _copy(long nat, long m_other, int fromRow, int toRow, int fromColumn, int toColumn);

  protected native void _copy(long nat, long m_other);

  protected native boolean _compare(long nat, long m_other);

  protected native void _emultiply(long nat, long m_other);

  protected native void _emultiply(long nat, long m_first, long m_other);

  protected native void _add(long nat, long m_other);

  protected native void _add(long nat, long m_first, long m_second);

  protected native void _add(long nat, double s);

  protected native void _subtract(long nat, long m_other);

  protected native void _subtract(long nat, long m_first, long m_second);

  protected native void _multiply(long nat, double s);

  protected native void _multiply(long nat, long m_other);

  protected native void _multiply(long nat, long m_first, long m_second);

  protected native void _multiplyV(long nat, long v_other, long v_result);

  protected native void _leftMultiply(long nat, long v_other, long v_result);

  protected native void _leftMultiply(long nat, long v_srcDest);

  protected native void _outerProduct(long nat, long v_first, long v_second);

  protected native void _transpose(long nat);

  protected native boolean _equals(long nat, long m_other);

  protected native boolean _prettyCloseTo(long nat, long m_other, double tolerance);
}


