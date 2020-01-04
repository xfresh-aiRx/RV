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
 * file .......: ltiGenericMatrix_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiGenericMatrix_inline.h,v 1.5 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_GENERIC_MATRIX_INLINE_H_
#define _LTI_GENERIC_MATRIX_INLINE_H_

#include "ltiMathObject.h"
#include "ltiGenericVector.h"
#include "ltiAssert.h"

namespace lti {

  template <class T>
  inline genericVector<T>* genericMatrix<T>::allocRows(const int n) {
    return new genericVector<T>[n];
  }

  /* owns this %object the data?
     returns <em>false</em> if this genericMatrix contains a reference to
     extern data.
  */
  template<class T>
  inline bool genericMatrix<T>::ownsData() const {
    return ownData;
  };

  /* store mode
     @return <code>Connected</code> if the memory is a single block or
     <code>Line</code>      if the memory of each line is allocated
     in different places!
  */
  template<class T>
  inline typename genericMatrix<T>::eStoreMode 
  genericMatrix<T>::getMode() const {
    return mode;
  };

  // number of rows of the genericMatrix
  template<class T>
  inline int genericMatrix<T>::rows() const {
    return numRows;
  };

  // number of columns of the genericMatrix
  template<class T>
  inline int genericMatrix<T>::columns() const {
    return numColumns;
  };

  // number of rows of the genericMatrix
  template<class T>
  inline int genericMatrix<T>::lastRow() const {
    return lastRowIdx;
  };

  // number of columns of the genericMatrix
  template<class T>
  inline int genericMatrix<T>::lastColumn() const {
    return lastColIdx;
  };

  /* number of "physical" rows of the matrix.
     @return If this is a <code>Connected</code> Matrix, (see getMode()),
     this member returns the same value as rows().

     If this is a <code>Line</code> Matrix, this value is bigger than
     rows().  If this was created with the copy constructor for a
     submatrix with "no copy data",
     this value will return the size of the original matrix.
  */
  template<class T>
  inline int genericMatrix<T>::metaRows() const {
    return metaNumRows;
  };

  /* number of "physical" columns of the matrix
     @return If this is a <code>Connected</code> Matrix, (see getMode()),
     this member returns the same value as columns().

     If this is a <code>Line</code> Matrix, this value is bigger than
     columns().  If this was created with the copy constructor for a
     submatrix with "no copy data",
     this value will return the size of the original matrix.
  */
  template<class T>
  inline int genericMatrix<T>::metaColumns() const {
    return metaNumColumns;
  };

  /* returns the size of the %genericMatrix in a lti::point structure.
     @return lti::point with the number of columns in its
     <code>x</code> coordinate and the number of rows in its
     <code>y</code> coordinate.
  */
  template<class T>
  inline const ipoint& genericMatrix<T>::size() const {
    return theSize;
  };

  /* returns iterator to first element of the genericMatrix
   */
  template<class T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::begin() {
#   ifdef NDEBUG
    return iterator(theElements);
#   else
    return iterator(0,this);
#   endif
  };

  /* returns const_iterator to first element of the genericMatrix
   */
  template<class T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::begin() const {
#   ifdef NDEBUG
    return const_iterator(theElements);
#   else
    return const_iterator(0,this);
#   endif
  };

  // returns iterator to the end of the genericMatrix
  template<class T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::end() {
#   ifdef NDEBUG
    return iterator(&theElements[totalSize]);
#   else
    return iterator(totalSize,this);
#   endif
  };

  // returns iterator to the end of the genericMatrix
  template<class T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::end() const {
#   ifdef NDEBUG
    return const_iterator(&theElements[totalSize]);
#   else
    return const_iterator(totalSize,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericMatrix. It is used for inverse order
   * iteration through the genericMatrix using normal iterators (as opposed
   * to reverse iterators). This has the advantage that iterators
   * going from front to end and in the inverse direction are the
   * same and can thus be compared, copied etc.
   */
  template<class T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::inverseBegin() {
#   ifdef NDEBUG
    return iterator(&theElements[totalSize-1]);
#   else
    return iterator(totalSize-1,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericMatrix. See inverseBegin() for more details.
   */
  template<class T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::inverseBegin() const {
#   ifdef NDEBUG
    return const_iterator(&theElements[totalSize-1]);
#   else
    return const_iterator(totalSize-1,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericMatrix. It is used to
   * mark the end for inverse order iteration through the genericMatrix
   * using normal iterators (as opposed to reverse iterators). This
   * has the advantage that iterators going from front to end and in
   * the inverse direction are the same and can thus be compared,
   * copied etc.
   */
  template<class T>
  inline typename genericMatrix<T>::iterator genericMatrix<T>::inverseEnd() {
#   ifdef NDEBUG
    return iterator(&theElements[-1]);
#   else
    return iterator(-1,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericMatrix.
   */
  template<class T>
  inline typename genericMatrix<T>::const_iterator 
  genericMatrix<T>::inverseEnd() const {
#   ifdef NDEBUG
    return const_iterator(&theElements[-1]);
#   else
    return const_iterator(-1,this);
#   endif
  };

  // access operators...

  /* access element at the given row and column
     @param row the row of the element
     @param col the column of the element
     @return a reference to the genericMatrix element
  */
  template<class T>
  inline T& genericMatrix<T>::at(const int row,const int col) {
    assert(static_cast<unsigned int>(row)<static_cast<unsigned int>(numRows));
    return ( (rowAddressTable[row].at(col)) );
  }

  // access operator (const)

  /* read-only access at the given row and column
     @param row the row of the element
     @param col the column of the element
     @return a reference to the genericMatrix element
  */
  template<class T>
  inline const T& genericMatrix<T>::at(const int row, const int col) const {
    assert(static_cast<unsigned int>(row)<static_cast<unsigned int>(numRows));
    return ( (rowAddressTable[row].at(col)) );
  }

  /* access element at the given position.

  With this operator the genericMatrix can be accessed as a vector,
  where the rows of the genericMatrix are concatenated.  The access
  to the genericMatrix with at(row,col) is equivalent to
  at(row*columns()+col) with this operator.

  This access operator doesn't work with lined matrices (see getMode()).

  @param pos the index of the element of the genericMatrix
  @return a reference to the genericMatrix element
  */
  template<class T>
  inline T& genericMatrix<T>::at(const int pos) {

    assert ( pos < totalSize );

    return (theElements[pos]);
  }

  /* access element at the given position.

  With this operator the genericMatrix can be accessed as a vector,
  where the rows of the genericMatrix are concatenated.  The access
  to the genericMatrix with at(row,col) is equivalent to
  at(row*columns()+col).

  This access operator doesn't work with lined matrices (see getMode()).

  @param pos the index of the element of the genericMatrix
  @return a reference to the genericMatrix element
  */
  template<class T>
  inline const T& genericMatrix<T>::at(const int pos) const {
    assert ( pos < totalSize );
    return (theElements[pos]);
  }

  /* access operator of genericMatrix element as a point in a 2D-Map
     @param p position of the element (this is equivalent to at(p.y,p.x))
     @return a reference to the genericMatrix element
  */
  template<class T>
  inline T& genericMatrix<T>::at(const ipoint& p) {
    return at(p.y,p.x);
  };

  /* const access operator of genericMatrix element as a point in a 2D-Map
     @param p position of the element (this is equivalent to at(p.y,p.x))
     @return a const reference to the vector element
  */
  template<class T>
  inline const T& genericMatrix<T>::at(const ipoint& p) const {
    return at(p.y,p.x);
  };

  /* resize member
   */
  template<class T>
  inline void genericMatrix<T>::resize(const ipoint& newDim,
                                       const T& iniValue,
                                       const bool copyData,
                                       const bool initNew) {
    resize(newDim.y,newDim.x,iniValue,copyData,initNew);
  }

  /* return genericMatrix-row as a vector.
     This method works fast, since it returns a reference to the row vector.
     The data will NOT be copied.
     @param row the row to be accessed
     @return a reference to the vector row
  */
  template<class T>
  inline genericVector<T>& genericMatrix<T>::getRow(const int row) {
    assert(static_cast<unsigned int>(row)<static_cast<unsigned int>(numRows));
    return rowAddressTable[row];
  }

  /* return genericMatrix-row as a const vector.
     This method works fast, since it returns a reference to the row vector.
     The data will NOT be copied.
     @param row the row to be accessed
     @return a const reference to the vector row
  */
  template<class T>
  inline const genericVector<T>& 
  genericMatrix<T>::getRow(const int row) const {
    assert(static_cast<unsigned int>(row)<static_cast<unsigned int>(numRows));
    return rowAddressTable[row];
  }


  // alias for getRow()
  template<class T>
  inline genericVector<T>& genericMatrix<T>::operator[](const int row) {
    return getRow(row);
  };

  // alias for getRow()
  template<class T>
  inline const genericVector<T>& 
  genericMatrix<T>::operator[](const int row) const{
    return getRow(row);
  };

  /* copy a row vector in the given parameter This method copies the
     data of a given row of the genericMatrix in the given vector.
     
     @param row the number of the row to be copied
     @param theRow the vector, where the data will be copied
     @see getRow()
  */
  template<class T>
  inline void genericMatrix<T>::getRowCopy(const int row,
                                           genericVector<T>& theRow) const {
    theRow.copy(getRow(row));
  };

  /* return genericMatrix-row as a vector.
     This method copies the data of the genericMatrix, therefore is not as
     fast as getRow()
     @param row the number of tthe row to be copied
     @return a vector with the contents of the row of the genericMatrix
  */
  template<class T>
  inline genericVector<T> genericMatrix<T>::getRowCopy(const int row) const {
    genericVector<T> tmp;
    getRowCopy(row,tmp);
    return tmp;
  };

  /* return genericMatrix-column as a vector.
     This method copies the data of the genericMatrix, therefore is not as
     fast as getRow()
     @param col the number of the column to be copied
     @return a vector with the contents of the column of the genericMatrix
  */
  template<class T>
  inline genericVector<T> genericMatrix<T>::getColumnCopy(const int col) const{
    assert(static_cast<unsigned int>(col) <
           static_cast<unsigned int>(numColumns));
    genericVector<T> tmp;
    getColumnCopy(col,tmp);
    return tmp;
  };

  /* return diagonal as a vector.
     This method copies the data of the genericMatrix.
     @return a vector with the contents of the diagonal of the genericMatrix
  */
  template<class T>
  inline genericVector<T> genericMatrix<T>::getDiagonal() const {
    genericVector<T> tmp;
    getDiagonal(tmp);
    return tmp;
  };

  /* copy the data of a vector in a given row
     @param row the row that receives the data.
     @param theRow the vector with the data to be copied
  */
  template<class T>
  inline void genericMatrix<T>::setRow(const int row,
                                       const genericVector<T>& theRow) {
    getRow(row).copy(theRow);
  };

  /* assigment operator (alias for copy(other)).
     @param other the genericMatrix to be copied
     @return a reference to the actual vector
  */
  template<class T>
  inline genericMatrix<T>& 
  genericMatrix<T>::operator=(const genericMatrix<T>& other) {
    return copy(other);
  };

  template<class T>
  inline genericMatrix<T>& 
  genericMatrix<T>::copy(const genericMatrix<T>& other,
                         const irectangle& window) {
    return copy(other,window.ul.y,window.br.y,window.ul.x,window.br.x);
  };

  template<class T>
  inline bool 
  genericMatrix<T>::operator==(const genericMatrix<T>& other) const {
    return equals(other);
  };

  /*
   * FILL Members
   */
  template<class T>
  inline void genericMatrix<T>::fill(const T& iniValue,
                                     const ipoint& from,
                                     const ipoint& to) {
    fill(iniValue,from.y,from.x,to.y,to.x);
  }

  template<class T>
  inline void genericMatrix<T>::fill(const T& iniValue,
                                     const irectangle& window) {
    fill(iniValue,window.ul.y,window.ul.x,window.br.y,window.br.x);
  }

  template<class T>
  inline void genericMatrix<T>::fill(const T data[],
                                     const ipoint& from,
                                     const ipoint& to) {
    fill(data,from.y,from.x,to.y,to.x);
  }

  template<class T>
  inline void genericMatrix<T>::fill(const T data[],const irectangle& window) {
    fill(data,window.ul.y,window.ul.x,window.br.y,window.br.x);
  }

  template<class T>
  inline void genericMatrix<T>::fill(const genericMatrix<T>& mat,
                                     const ipoint& from,
                                     const ipoint& to,
                                     const ipoint& startAt) {
    fill(mat,from.y,from.x,to.y,to.x,startAt.y,startAt.x);
  }

  template<class T>
  inline void genericMatrix<T>::fill(const genericMatrix<T>& mat,
                                     const irectangle& window,
                                     const ipoint& start) {
    fill(mat,window.ul.y,window.ul.x,window.br.y,window.br.x,start.y,start.x);
  }
}


#endif

