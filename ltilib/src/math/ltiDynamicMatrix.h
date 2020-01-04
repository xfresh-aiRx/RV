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
 * file .......: ltiDynamicMatrix.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiDynamicMatrix.h,v 1.9 2003/02/17 07:16:40 alvarado Exp $
 */

#ifndef _LTI_DYNAMIC_MATRIX_H_
#define _LTI_DYNAMIC_MATRIX_H_

#include <assert.h>
#include <iostream>
#include <map>
#include <list>

#include "ltiMathObject.h"
#include "ltiMath.h"
#include "ltiPoint.h"

namespace lti {
  /**
   * This template class allows the use of matrices which change its
   * dimensions without losing any data.  They are base on the
   * STL-maps.  The intention of this class is to provide a base
   * data-structure for the generation of classification statistics
   * (confusion-matrices).  This is a VERY SLOW class, but highly
   * confortable for dynamically growing matrices.
   *
   * @ingroup gAggregate
   */
  template <class T>
  class dynamicMatrix : public mathObject {
    public:
    /**
     * default constructor (create an empty matrix)
     */
    dynamicMatrix();

    /**
     * copy constructor
     */
    dynamicMatrix(const dynamicMatrix<T>& other);

    /**
     * destructor
     */
    ~dynamicMatrix();

    /**
     * get number of rows actually being stored
     */
    inline unsigned int rows() const { return theRowIdx.size();};

    /**
     * get the number of columns actually being stored
     */
    inline unsigned int columns() const { return theColIdx.size();};

    /**
     * Return type of the size() member
     */
    typedef point size_type;

    /**
     * return the size of the matrix (number of used rows and columns) as
     * a point.  The "x" member contains the used columns and "y" the used
     * rows.
     */
    inline size_type size() const {return point();}

    /**
     * check if the given row exist on the matrix
     */
    bool isRowValid(const int theRow) const;

    /**
     * check if the given matrix exist on the matrix
     */
    bool isColumnValid(const int theColumn) const;

    /**
     * insert a row with index "theRow" on the matrix, and initialize
     * its contents with "initialValue"
     */
    bool addRow(const int theRow,const T initialValue = T());

    /**
     * insert a column with index "theColumn" on the matrix, and initialize
     * its contents with "initialValue"
     */
    bool addColumn(const int theColumn,const T initialValue = T());

    /**
     * delete the given row if it exists
     */
    bool deleteRow(const int theRow);

    /**
     * delete the given column if it exists
     */
    bool deleteColumn(const int theColumn);

    /**
     * read-write access operator
     */
    inline T& at(const int& theRow,const int& theColumn);

    /**
     * read only access operator
     */
    inline const T & at(const int& theRow,const int& theColumn) const;

    /**
     * assignment operator
     */
    inline dynamicMatrix& operator=(const dynamicMatrix<T>& theMatrix) {
      return copy(theMatrix);
    };

    /**
     * copy operator
     */
    inline dynamicMatrix& copy(const dynamicMatrix<T>& theMatrix);

    /**
     * clone member
     */
    mathObject* clone() const;

    /**
     * the value given here will be return if the content of a non existent
     * cell is requested
     */
    void setErrorValue(T error) {errorValue = error;};

    /**
     * clear the contents of the matrix
     */
    void clear();

    /**
     * list of integers, used to indicate the used indices in the matrix
     */
    typedef std::list<int> stdIndexList;

    /**
     * returns a list of integers with the used row indices
     */
    const stdIndexList& getUsedRows() const {return theRowIdx;};

    /**
     * returns a list of integers with the used column indices
     */
    const stdIndexList& getUsedColumns() const {return theColIdx;};

  protected:
    /**
     * type of a "dynamic"-row
     */
    typedef std::map<int,T> stdRowMap;

    /**
     * the matrix-data type
     */
    typedef std::map<int,stdRowMap> stddynamicMatrix;

    /**
     * the list of row indices
     */
    stdIndexList theRowIdx;

    /**
     * the list of column indices
     */
    stdIndexList theColIdx;

    /**
     * the matrix data
     */
    stddynamicMatrix theMatrix;

    /**
     * value used when unexistent cells are accessed.
     */
    T errorValue;
  };


}

#include "ltiDynamicMatrix_template.h"

#endif

