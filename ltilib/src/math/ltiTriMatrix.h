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
 * file .......: ltiTriMatrix.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.99
 * revisions ..: $Id: ltiTriMatrix.h,v 1.5 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_TRIMATRIX_H_
#define _LTI_TRIMATRIX_H_

#include "ltiMathObject.h"

namespace lti {

  /**
   * triMatrix is a very simple class to optimize memory usage when using an
   * square triangular matrix with following form:
   *
   * \code
   *
   *     x x x x x x
   *     x x x x x
   *     x x x x
   *     x x x
   *     x x
   *     x
   *
   * \endcode
   *
   * @ingroup gAggregate
   */
  template <class T>
  class triMatrix : public mathObject {
    public:

    /**
     * type of the contained data
     */
    typedef T value_type;

    /**
     * return type of the size() member
     */
    typedef int size_type;

    /**
     * default constructor
     */
    triMatrix();

    /**
     * constructor with a default size.  The matrix will be a y times y
     * diagonal matrix.
     */
    triMatrix(const int y);

    /**
     * copy constructor
     */
    triMatrix(const triMatrix<T>& x);

    /**
     * destructor
     */
    virtual ~triMatrix();

    /**
     * read/write access operator.
     * @param y which row
     * @param x which column
     */
    T& at(const int y, const int x);

    /**
     * read-only access operator.
     * @param y which row
     * @param x which column
     */
    const T& at(const int y, const int x) const;

    /**
     * return a pointer to the begin of the given row
     */
    T* operator[] (const int y);

    /**
     * return a pointer to the begin of the given row
     */
    const T* operator[] (const int y) const;

    /**
     * copy the other matrix
     */
    triMatrix<T>& operator= (const triMatrix<T>& x);

    /**
     * copy the other matrix
     */
    triMatrix<T>& copy(const triMatrix<T>& x);

    /**
     * return the number of rows (or columns) of this matrix
     */
    inline int size() const {return (dimension);};

    /**
     * returns the name of this class
     */
    virtual const char* getTypeName() const {
      return "triMatrix";
    };

    /**
     * returns a copy of this object
     */
    virtual mathObject* clone() const;

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,
                       const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    protected:
    /**
     * the data of the matrix
     */
    T* mem;

    /**
     * the pointers to the rows of the matrix
     */
    T** rows;

    /**
     * the dimension of the matrix
     */
    int dimension;
  };
}

#include "ltiTriMatrix_template.h"

#endif

