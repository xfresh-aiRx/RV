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
 * file .......: ltiMatrix_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiMatrix_inline.h,v 1.23 2004/05/03 12:20:01 alvarado Exp $
 */

#ifndef _LTI_MATRIX_INLINE_H_
#define _LTI_MATRIX_INLINE_H_

#include "ltiMathObject.h"
#include "ltiVector.h"

namespace lti {

  template <class T>
  inline genericVector<T>* matrix<T>::allocRows(const int n) {
    return new vector<T>[n];
  }


  /* return matrix-row as a vector.
      This method works fast, since it returns a reference to the row vector.
      The data will NOT be copied.
      @param row the row to be accessed
      @return a reference to the vector row
  */
  template<class T>
  inline vector<T>& matrix<T>::getRow(const int row) {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(this->numRows));
    return static_cast<vector<T>*>(this->rowAddressTable)[row];
  }

  /* return matrix-row as a const vector.
      This method works fast, since it returns a reference to the row vector.
      The data will NOT be copied.
      @param row the row to be accessed
      @return a const reference to the vector row
  */
  template<class T>
  inline const vector<T>& matrix<T>::getRow(const int row) const {
    assert(static_cast<unsigned int>(row) <
           static_cast<unsigned int>(this->numRows));
    return static_cast<vector<T>*>(this->rowAddressTable)[row];
  }


  // alias for getRow()
  template<class T>
  inline vector<T>& matrix<T>::operator[](const int row) {
    return getRow(row);
  };

  // alias for getRow()
  template<class T>
  inline const vector<T>& matrix<T>::operator[](const int row) const{
    return getRow(row);
  };

  /* return matrix-row as a vector.
     This method copies the data of the matrix, therefore is not as
     fast as getRow()
     @param row the number of tthe row to be copied
     @return a vector with the contents of the row of the matrix
  */
  template<class T>
  inline vector<T> matrix<T>::getRowCopy(const int row) const {
    vector<T> tmp;
    getRowCopy(row,tmp);
    return tmp;
  };

  template<class T>
  inline void matrix<T>::getRowCopy(const int row,
				    vector<T>& theRow) const {
    genericMatrix<T>::getRowCopy(row,theRow);
  };


  /* return matrix-column as a vector.
     This method copies the data of the matrix, therefore is not as
     fast as getRow()
     @param col the number of the column to be copied
     @return a vector with the contents of the column of the matrix
  */
  template<class T>
  inline vector<T> matrix<T>::getColumnCopy(const int col) const {
    assert(static_cast<unsigned int>(col) <
           static_cast<unsigned int>(this->numColumns));
    vector<T> tmp;
    getColumnCopy(col,tmp);
    return tmp;
  };

  template<class T>
  inline void matrix<T>::getColumnCopy(const int col,
				       vector<T>& theCol) const {
    genericMatrix<T>::getColumnCopy(col,theCol);
  };

  /* return diagonal as a vector.
     This method copies the data of the matrix.
     @return a vector with the contents of the diagonal of the matrix
  */
  template<class T>
  inline vector<T> matrix<T>::getDiagonal() const {
    vector<T> tmp;
    getDiagonal(tmp);
    return tmp;
  };

  /* return diagonal as a vector.
     This method copies the data of the matrix.
     @return a vector with the contents of the diagonal of the matrix
  */
  template<class T>
  inline void matrix<T>::getDiagonal(vector<T>& diag) const {
    genericMatrix<T>::getDiagonal(diag);
  }

  // copy an external matrix
  template <class T>
  inline matrix<T>& matrix<T>::copy(const matrix<T>& other) {
    genericMatrix<T>::copy(other);
    return *this;
  }


  // copy an external matrix
  template <class T>
  inline matrix<T>& matrix<T>::copy(const matrix<T>& other,
                                    const int fromRow,const int toRow,
                                    const int fromCol,const int toCol) {
    genericMatrix<T>::copy(other,fromRow,toRow,fromCol,toCol);
    return (*this);
  }

  // copy an external matrix
  template <class T>
  inline matrix<T>& matrix<T>::copy(const matrix<T>& other,
                                    const vector<int>& idx,
                                    bool rowWise) {
    genericMatrix<T>::copy(other,idx,rowWise);
    return (*this);
  }

  /* assigment operator (alias for copy(other)).
     @param other the matrix to be copied
     @return a reference to the actual vector
  */
  template<class T>
  inline matrix<T>& matrix<T>::operator=(const matrix<T>& other) {
    return copy(other);
  };

  template<class T>
  inline matrix<T>& matrix<T>::copy(const matrix<T>& other,
                                    const irectangle& window) {
    return copy(other,window.ul.y,window.br.y,window.ul.x,window.br.x);
  };

}


#endif

