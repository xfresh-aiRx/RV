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
 * file .......: ltiDynamicMatrix_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiDynamicMatrix_template.h,v 1.6 2003/02/17 07:16:40 alvarado Exp $
 */

namespace lti {

  // implementation of the template methods
  template <class T>
  dynamicMatrix<T>::dynamicMatrix() : mathObject() {
    clear();
    errorValue = lti::NaN;
  }

  // implementation of the template methods
  template <class T>
  dynamicMatrix<T>::dynamicMatrix(const dynamicMatrix<T>& other) 
  : mathObject() {
    copy(other);
  }


  // implementation of the template methods
  template <class T>
    dynamicMatrix<T>::~dynamicMatrix() {
    clear();
  }

  template <class T>
    bool dynamicMatrix<T>::isRowValid(const int theRow) const {
    return (theMatrix.find(theRow) != theMatrix.end());
  }

  template <class T>
    bool dynamicMatrix<T>::isColumnValid(const int theColumn) const {
    // 1. take the first row, to check this...
    typename stddynamicMatrix::const_iterator it;
    it = theMatrix.begin();

    if (it == theMatrix.end())
      return false;
    else
      return ((*it).second.find(theColumn) != (*it).second.end());
  }

  template <class T>
    T & dynamicMatrix<T>::at(const int& theRow,const int& theColumn) {
    bool valid = (isRowValid(theRow) && isColumnValid(theColumn));

    if (!valid)
      return errorValue;

    return theMatrix[theRow][theColumn];
  }

  template <class T>
    const T & dynamicMatrix<T>::at(const int& theRow,
                                   const int& theColumn) const {
    bool valid = (isRowValid(theRow) && isColumnValid(theColumn));

    if (!valid)
      return errorValue;

    return (*((*theMatrix.find(theRow)).second.find(theColumn))).second;
  }

  template <class T>
    bool dynamicMatrix<T>::addRow(const int theRow, const T initialValue) {
    if (!isRowValid(theRow)) {
      theRowIdx.push_back(theRow);
      stdIndexList::const_iterator it;
      theMatrix[theRow]; // create a row with index theRow
      for (it = theColIdx.begin(); it != theColIdx.end(); it ++) {
        theMatrix[theRow][(*it)] = initialValue;
      }
      return true;
    } else
      return false;  // the Row was already there
  }

  template <class T>
    bool dynamicMatrix<T>::addColumn(const int theColumn,
                                     const T initialValue) {
    if (!isColumnValid(theColumn)) {
      theColIdx.push_back(theColumn);
      typename stddynamicMatrix::iterator it;

      for (it = theMatrix.begin(); it != theMatrix.end(); it++) {
        ((*it).second)[theColumn] = initialValue;
      }
      return true;
    } else
      return false;
  }

  template <class T>
    bool dynamicMatrix<T>::deleteRow(const int theRow) {
    if (isRowValid(theRow)) {
      theRowIdx.remove(theRow);
      theMatrix[theRow].clear();
      theMatrix.erase(theRow);
    }
    return true;
  }

  template <class T>
    bool dynamicMatrix<T>::deleteColumn(const int theColumn) {
    if (isColumnValid(theColumn)) {
      theColIdx.remove(theColumn);
      typename stddynamicMatrix::iterator it;
      for (it = theMatrix.begin(); it != theMatrix.end(); it++) {
        ((*it).second).erase(theColumn);
      }
    }
    return true;
  }

  template <class T>
    void dynamicMatrix<T>::clear() {
    theRowIdx.clear();
    theColIdx.clear();
    theMatrix.clear();
  }

  template <class T>
    mathObject* dynamicMatrix<T>::clone() const {
    return new dynamicMatrix<T>(*this);
  }

  template <class T>
    dynamicMatrix<T>& dynamicMatrix<T>::copy(const dynamicMatrix<T>& other) {

    theRowIdx = other.theRowIdx;
    theColIdx = other.theColIdx;
    theMatrix = other.theMatrix;
    errorValue = other.errorValue;

    return *this;
  }


}

