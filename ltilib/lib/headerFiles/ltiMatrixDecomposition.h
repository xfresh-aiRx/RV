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
 * file .......: ltiMatrixDecomposition.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 23.06.99
 * revisions ..: $Id: ltiMatrixDecomposition.h,v 1.10 2005/11/04 09:40:24 doerfler Exp $
 */

#ifndef _LTI_MATRIX_DECOMPOSITON_H_
#define _LTI_MATRIX_DECOMPOSITON_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

namespace lti {
  /**
   * LU decomposition functor.
   * Computes the LU decomposition of a square matrix.
   *
   * The decomposition will fail if the matrix is singular or close to
   * being singular.
   *
   * @ingroup gLinearAlgebra
   */
  template<class T>
  class luDecomposition : public linearAlgebraFunctor {
    // store the last result
    matrix<T> luRef;
    vector<int> permRef;
    // this is true if there is some valid LU decomposition.
    bool haveLU;
    int pivsign;
  public:

    static const T my_epsilon;

    /// default constructor
    luDecomposition();

    /**
     * Constructs a LU decomposition for the given matrix The functor
     * internally stores the decomposition, so you can get the derived
     * parts with getL, getU and det.  The matrix itself is not
     * changed, so if you need the LU decomposition in the given
     * matrix itself, you will have to use one of the apply methods.
     * @param m the matrix that gets decomposed.
     */
    luDecomposition(const matrix<T> &m);

    /// destructor
    virtual ~luDecomposition();

    /** onPlace version of apply.
        Given a matrix theMatrix[0..n-1][0..n-1], this routine replaces it by
        the LU decomposition of a rowwise permutation of itself.
        permutation[0..n-1] is an output vector that records the row
        permutation effected by the partial pivoting. The function returns
        +/-1 depending on whether the number of row interchanges was even or
        odd, respectively. The functor internally stores a copy of the
        last decomposition, so you can use it for det and getL and getU.
        @return true, if the decomposition could be computed, false
                otherwise (typically, the matrix was singular).
    */
    bool apply(matrix<T>& theMatrix,vector<int>& permutation, bool store=true);

    /** onCopy version of apply.
        Given a matrix theMatrix[0..n-1][0..n-1], this routine returns a matrix
        decomposition[0..n-1][0..n-1] which contains the LU decomposition of a
        rowwise permutation of theMatrix. permutation[0..n-1] is an output
        vector that records the row permutation effected by the partial
        pivoting. The function returns +/-1 depending on whether the number of
        row interchanges was even or odd, respectively. The functor internally
        stores a copy of the
        last decomposition, so you can use it for det and getL and getU.
        @return true, if the decomposition could be computed, false
                otherwise (typically, the matrix was singular).
    */
    bool apply(const matrix<T>& theMatrix, matrix<T>& decompositon,
              vector<int>& permutation);

    /// copy data of "other" functor.
    luDecomposition<T>& copy(const luDecomposition<T>& other);

    /// returns a pointer to a clone of the functor.
    virtual functor* clone() const { return (new luDecomposition<T>(*this));};
    /// returns the name of this type
    virtual const char* getTypeName() const {return "luDecompositon";};

    /**
       returns a new Matrix which contains the "L" part of the last
       performed decomposition.
    */
    matrix<T> getL() const;

    /**
       returns a new Matrix which contains the "U" part of the last
       performed decomposition.
    */
    matrix<T> getU() const;

    /**
     * returns a new Matrix which contains the "L" part of the last
     * performed decomposition. This is much faster than the on
     * copy version.
     */
    void getL(matrix<T>& l) const;

    /**
     * returns a new Matrix which contains the "L" part of the last
     * performed decomposition. This is much faster than the on 
     * copy version
     */
    void getU(matrix<T>& u) const;

    /// returns the determinant of the last given matrix
    T det() const;

    /// returns the determinant of the given matrix
    T det(const matrix<T> &theMatrix);

    /// returns true if the last given matrix was regular, i.e. non-singular
    bool isRegular() const;

    /// returns true if the given matrix was regular, i.e. non-singular
    bool isRegular(const matrix<T> &theMatrix);

  };
}

#endif

