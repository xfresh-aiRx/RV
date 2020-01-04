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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiSparseMatrix.h
 * authors ....: Bastian Ibach
 * organization: LTI, RWTH Aachen
 * creation ...: 26.04.02
 * revisions ..: $Id: ltiSparseMatrix.h,v 1.12 2005/06/16 12:37:35 doerfler Exp $
 */


#ifndef _LTI_SPARSEMATRIX_H_
#define _LTI_SPARSEMATRIX_H_

#include "ltiMathObject.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include <vector>
#include <map>
#include <iostream>

namespace lti {
  /**
   * SparseMatrix container class.
   *
   * The lti::sparseMatrix class allows the representation
   * of \e n x \e m matrices as sparse matrices (SM).
   *
   * The SM class is a container class implemented as template.
   *
   * All types defined in ltiTypes.h  except \c bool can be contained.
   *
   * A matrix is normaly defined as sparse if it contains a lot of zeros, in
   * this class you can use an arbitrary value denoted with \e sparseValue.
   *
   * Only the non-sparse values of the matrix are saved in the container
   * using the Compressed Row Storage format.
   *
   * Due to the characteristics of the storage, the method at() is here
   * a read-only one.  To write values into the matrix use the setAt() or
   * forceAt() methods.
   *
   * The Compressed Row Storage (CRS) format puts the subsequent nonsparse
   * values of the matrix rows in contiguous memory locations.
   *
   * The CRS format uses three vectors to store the non sparse values:
   * - values, type of T
   * - colIndex, type of int
   * - rowPtr, type of int
   *
   * In the \b values vector are saved the values of the non sparse elements
   * of the matrix, as they are traversed in a row-wise fashion.
   *
   * The \b colIndex vector stores the column indices of the elements
   * in the values vector.
   *
   * The \b rowPtr vector stores the position in the values vector that start a
   * new row. The vector size is equal number of rows plus one. The last value
   * in the rowPtr vector is needed to calculate the number of elements in the
   * last row of the matrix.
   *
   * Example:
   *
   * \code
   * matrix A = ( 0  2  5  0
   *              1  0  0  0
   *              0  4  0  6 )
   * \endcode
   *
   *
   * The CRS format for the matrix A is specified by the vectors given below:
   *
   * \code
   * values   = ( 2  5  1  4  6 )
   * colIndex = ( 1  2  0  1  3 )
   *
   * rowPtr   = ( 0  2  3  5 )
   * \endcode
   *
   * The \b iterators of the sparse matrices visit only the non sparse values,
   * as this is usually the required task.
   *
   * You can use the getPosition() methods to gain the index of the element
   * pointed by an iterator.  If for each non-sparse element you need to
   * compute its index, it is more efficient to avoid iterators and using the
   * data structures of the CRS directly:
   *
   * \code
   *
   * // assume you are in a template class/method of type T, or that type
   * // T is somehow defined (for example typedef float T)
   *
   * lti::point p; // the coordinates
   * lti::sparseMatrix<T> smat; 
   *
   * // ... fill the sparse matrix with data here
   *
   * const lti::ivector& rows     = smat.getRowPtr();
   * const std::vector<int>& cols = smat.getColIndex();
   * const std::vector<T>& vals   = smat.getValues();
   *
   * int i,f,l;
   * T val; 
   *
   * // for each row
   * for (p.y=0;p.y<rows.lastIdx();++p.y) {
   *   f=rows.at(p.y);       // first row element
   *   l=rows.at(p.y+1);     // last row element
   *   for (i=f;i<l;++i) { // for each element in row
   *     p.x=cols[i];
   *     val=vals[i];
   *     // the matrix element at \c p contains value \c val
   *     // here you can do whatever you need to...
   *   }
   * }
   *
   * \endcode
   *
   * @ingroup gAggregate
   * @ingroup gLinearAlgebra
   */
  template<class T>
  class sparseMatrix : public mathObject {
  public:

    /**
     * type of the contained data
     */
    typedef T value_type;

    /**
     * iterator
     * the iterator points to one element of the values vector
     * you only have access to these values, not to the sparseValues
     */
    typedef typename std::vector<T>::iterator iterator;


    /**
     * const_iterator
     * the const_iterator points to one element of the values vector
     * you only have access to these values, not to the sparseValues
     */
    typedef typename std::vector<T>::const_iterator const_iterator;


    /**
     * default constructor creates an empty sparseMatrix
     */
    sparseMatrix();

    /**
     * construct a sparse matrix of the given size and fill it
     * with the given value (i.e. use the given value as sparse value).
     */
    sparseMatrix(const int theRows,const int theCols,const T& sparseVal=T());

    /**
     * construct a sparse matrix of the given size and fill it
     * with the given value (i.e. use the given value as sparse value).
     */
    sparseMatrix(const point& size,const T& sparseVal=T());

    /**
     * copy constructor.
     *
     * create this sparseMatrix as a connected copy of another sparseMatrix
     * for this const version, the data will be always copied!
     *
     * @param other the sparseMatrix to be copied.
     */
    sparseMatrix(const sparseMatrix<T>& other);

    /**
     * destructor
     */
    virtual ~sparseMatrix();

    /**
     * resize the given sparse matrix and fill it with the given
     * value.
     * It is not possible to keep the old data when resizing
     * the matrix.
     */
    bool resize(const int rows,const int cols,const T& sparseVal);

    /**
     * resize the given sparse matrix and fill it with the given
     * value.
     * It is not possible to keep the old data when resizing
     * the matrix.
     */
    bool resize(const point& size,const T& sparseVal);

    /**
     * convert matrix to SparseMatrix
     *
     * @param srcMatrix the matrix converted to sparseMatrix
     * @param sValue the value of srcMatrix used as sparseValue
     * in the sparseMatrix
     * @return a reference to this object.
     */
    sparseMatrix<T>& castFrom(const matrix<T>& srcMatrix, const T& sValue);

    /**
     * convert matrix to SparseMatrix
     *
     * The value of the srcMatrix used as sparseValue is chosen by
     * statistical evaluation  (the most used one), which of course
     * cost some time.
     *
     * @param srcMatrix the matrix to be converted into a sparseMatrix
     * @return a reference to this object.
     */
    sparseMatrix<T>& castFrom(const matrix<T>& srcMatrix);

    /**
     * convert the other sparse matrix of type U to this sparse matrix
     * of type T
     */
    template<class U>
    sparseMatrix<T>& castFrom(const sparseMatrix<U>& other) {
      if(&other == this) { // Caution could be the same Matrix
        return(*this);
      }

      theSize = other.size();
      typename std::vector<U>::const_iterator oit;
      typename std::vector<T>::iterator it;
      values.resize(other.getValues().size());
      for (oit=other.values.begin(),it=values.begin();
           it!=values.end();
           ++it,++oit) {
        *it = static_cast<T>(*oit);
      }

      colIndex = other.getColIndex();
      rowPtr = other.getRowPtr();
      numNonSparseValues = other.getNumNonSparseValues();
      sparseValue = static_cast<T>(other.sparseValue);
      return (*this);
    }

    /**
     * convert SparseMatrix to matrix
     * @param destMatrix the lti::matrix where this sparseMatrix will be
     *        copied into.
     */
    bool castTo(matrix<T>& destMatrix);

    /**
     * copy the contents of other to this object
     * @param other the matrix to be copied
     */
    sparseMatrix<T>& copy(const sparseMatrix<T>& other);

    /**
     * iterator begin
     */
    iterator begin();

    /**
     * iterator end
     */
    iterator end();

    /**
     * const_iterator begin
     */
    const_iterator begin() const;

    /**
     * const_iterator end
     */
    const_iterator end() const;

    /**
     * access element at the given row and column
     * @param row the row of the element
     * @param col the column of the element
     * @return a reference to the matrix element
     */
    inline const T& at(const int row, const int col) const {
      return getAt(row,col);
    }

    /**
     * access element at the given row and column
     * @param row the row of the element
     * @param col the column of the element
     * @return the value of the matrix element
     */
    const T& getAt(const int row, const int col) const;

    /**
     * get a read-write reference to the given element, and in case
     * the element was a "sparse" one, insert it first (with the sparse
     * value.
     *
     * You can for example use this method, when you want to increment the
     * elements in a sparse matrix:
     * \code
     * sparseMatrix<int> sparseMat(1000,1000,0);
     * sparseMat.setAt(row,col,getAt(row,col)+1);
     * \endcode
     * is very inefficient, because the element at (row,col) must be
     * search twice.
     * On the other hand
     * \code
     * sparseMat.forceAt(row,col)++;
     * \endcode
     * makes just one access to the point and returns a reference.
     *
     * Note that if you just call forceAt without explicitelly changing
     * the value thereafter, you will have sparseElement as normal elements
     * in your matrix.  So, please use with care.  The normal way to
     * reset a value with the sparseValue is using setAt(row,col,sparseValue).
     */
    T& forceAt(const int row, const int col);

    /**
     * get a read-write reference to the given element, and in case
     * the element was a "sparse" one, insert it first.
     */
    inline T& forceAt(const point& p) {
      return forceAt(p.y,p.x);
    }

    /**
     * modify element at the given row and column
     * @param row the row of the element
     * @param col the column of the element
     * @param newValue new value for a matrix element
     * @return true if existing value is changed or
     * false if sparseValue is replaced by element
     */
    bool setAt(const int row, const int col, const T newValue);

    /**
     * modify element at point
     * @param position index of the element in sparseMatrix to be set
     * @param newValue new value for a matrix element
     * @return true if existing value is changed or false if
     * sparseValue is replaced by element
     */
    inline bool setAt(const point& position, const T newValue) {
      return setAt(position.y,position.x,newValue);
    }

    /**
     * insert element at the given row and column
     * @param iniValue the value to fill the (sub-)matrix with
     * @param fromRow the row to begin fill
     * @param fromCol the column to begin fill
     * @param toRow the last row to fill
     * @param toCol the last col to fill
     */
    void fill(const T& iniValue,
              const int& fromRow=0,
              const int& fromCol=0,
              const int& toRow=MaxInt32,
              const int& toCol=MaxInt32);

    /**
     * clear sparseMatrix to get a empty sparseMatrix
     * sparseValue is set to zero
     */
    void clear();

    /**
     * returns true if there are only "sparse values" on
     * the matrix.
     */
    bool empty() const;

    /**
     * get the position in the matrix of the iterator
     * @param iter iterator that points to a non-sparse element
     * @return point with index of the pointed element
     */
    point getPosition(const iterator& iter);

    /**
     * get the position in the matrix of the const_iterator
     * @param iter const_iterator that points to a non-sparse element
     * @return point with index of the pointed element
     */
    point getPosition(const const_iterator& iter);

    /**
     * get a copy of the row of the matrix
     * @param row the row to be accessed
     * @return a vector with the contents of the row of the matrix
     */
    lti::vector<T> getRowCopy(const int& row) const;

    /**
     * get a copy of the column of the matrix
     * @param col the column to be accessed
     * @return a vector with the contents of the column of the matrix
     */
    lti::vector<T> getColumnCopy(const int& col) const;

    /**
     * @return the name of this class: "sparseMatrix"
     */
    virtual const char* getTypeName() const;

    /**
     * create a clone of this matrix
     * @return a pointer to a copy of this matrix
     */
    virtual mathObject* clone() const;

    /**
     * assignment operator (alias for copy(other))
     *
     * @param other the matrix to be copied
     * @returns a reference to the actual matrix
     */
    sparseMatrix<T>& operator=(const sparseMatrix<T>& other);

    /**
     * compare this sparseMatrix with other
     *
     * @param other the sparseMatrix to be compared with
     * @return true if both sparseMatrices have the same SparseValue
     * the same values-, colIndex- and rowPtr-vector
     */
    bool operator==(const sparseMatrix<T> other) const;

    /**
     * returns size of the sparseMatrix
     */
    const point& size() const;

    /**
     * returns the number of non-sparse valued elements in the sparseMatrix
     */
    int getNumNonSparseValues() const;

    /**
     * return the used sparse value
     */
    T getSparseValue() const;

    /**
     * return the number of columns of the matrix
     */
    inline const int& columns() const {
      return numCols;
    }

    /**
     * return the number of columns of the matrix
     */
    inline const int& rows() const {
      return numRows;
    }

    /**
     * @name Arithmetical Operations
     */
    //@{

    /**
     * alias for add(const T value)
     *
     * @param value the value added to each element of the matrix
     * @returns a reference to the actual matrix
     */
    sparseMatrix<T>& operator+=(const T value);

    /**
     * add a constant value to this matrix and leave result in new matrix
     *
     * @param value the value added to each element of the matrix
     * @returns a new matrix with the result
     */
    sparseMatrix<T> operator+(const T value) const;

    /**
     * alias for multiply(const T value)
     *
     * @param value the constant value
     * @return a reference to the actual matrix
     */
    sparseMatrix<T>& operator*=(const T value);

    /**
     * add a constant value to all elements of the matrix
     * and leave the result here
     * @param value the constant value
     * @return return a reference to the actual matrix
     */
    sparseMatrix<T>& add(const T value);

    /**
     * devide the elements of the matrix by a constant value
     * and leave the result here
     * @param value the constant value (divisor)
     * @return return a reference to the actual matrix
     */
    sparseMatrix<T>& divide(const T value);

    /**
     * multiply the elements of the matrix by a constant value
     * and leave the result there
     * @param value the constant value
     * @return a reference to the actual matrix
     */
    sparseMatrix<T>& multiply(const T value);

    /**
     * multiply sparseMatrix with vector, write result
     * to a new vector
     *
     * @param srcVec vector to be multiplied with,
     * Its dimension must be equal to the number of columns of the matrix.
     * @param destVec vector to write result to
     * Its dimension will be equal to the number of rows of the matrix.
     */
    void multiply(const vector<T>& srcVec, vector<T>& destVec) const;

    /**
     * transpose sparseMatrix
     *
     * @return return a reference to the sparseMatrix
     */
    sparseMatrix<T> transpose();

    /**
     * get maximum value of the matrix
     * @return return a copy of this value
     */
    T maximum() const;

    /**
     * get index of maximum value of the matrix
     * @return return a copy of the index as a point
     */
    point getIndexOfMaximum() const;

    /**
     * get minimum value of the matrix
     * @return return a copy of this value
     */
    T minimum() const;

    /**
     * get index of minimum value of the matrix
     * @return return a copy of the index as a point
     */
    point getIndexOfMinimum() const;
    //@}

    /**
     * @name Input and Output
     */
    //@{

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    //@}

    /**
     * Following methods allow the access to the internal data structures,
     * which can be used to access very efficiently the non-sparse elements
     * in the rows of the matrix.
     */
    //@{
    /**
     * returns values vector
     */
    const std::vector<T>& getValues() const;

    /**
     * returns colIndex vector
     * vector of int
     */
    const std::vector<int>& getColIndex() const;

    /**
     * returns rowPtr vector
     * vector of int
     */
    const lti::vector<int>& getRowPtr() const;
    //@}

  private:

    /**
     * theSize.x is equal numColumns and theSize.y is equal numRows
     */
    point theSize;

    /**
     * number of rows of the sparseMatrix
     */
    int& numRows;

    /**
     * number of columns of the sparseMatrix
     */
    int& numCols;

    /**
     * number of non zero values
     */
    int numNonSparseValues;

    /**
     * value which is sparse, normaly this is zero
     */
    T sparseValue;

    /**
     * vector of type T, containing the non-sparse values of the sparseMatrix
     * Format: Compressed Row Storage
     */
    std::vector<T> values;

    /**
     * vector of int, containing the column indices of the non Zero values
     * must have the same size as the vector with the values
     */
    std::vector<int> colIndex;

    /**
     * vector of int, containing the position in the values vector, where
     * a new row begins
     */
    lti::vector<int> rowPtr;

    /*
     * !!! for internal use only !!
     *
     * setSparseValue, insert sparseValue at given row and column
     *
     *
    bool setSparseValue(const int& row, const int&col,
                        const T& value);
	*/

    /**
     * !!! for internal use only !!
     *
     * setSparseValue, insert sparseValue at given row and column
     *
     */
    bool setSparseValue(const int& row, const int& col);

    /**
     * !!! for internal use only !!
     *
     * fillSparseValue, fill region of sparseMatrix with sparseValue
     *
     */
    bool fillSparseValue(const int& fromRow, const int& fromCol,
                         const int& toRow, const int& toCol);

  };

}

namespace std {

  /**
   * outputs the elements of the vector on a stream
   */
  template <class T>
  ostream& operator<<(ostream& outStr, const lti::sparseMatrix<T>& sMatrix);
}


#endif

