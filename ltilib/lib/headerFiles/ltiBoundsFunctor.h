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
 * file .......: ltiMeansFunctor.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiBoundsFunctor.h,v 1.15 2005/02/02 14:16:22 doerfler Exp $
 */

#ifndef _LTI_BOUNDS_FUNCTOR_H_
#define _LTI_BOUNDS_FUNCTOR_H_

#include "ltiStatisticsFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

// because of a stupid implementation of the C library in Visual C++, we
// have to undefine min and max
#undef min
#undef max

namespace lti {
  /**
   * Boudaries of a hyperbox enclosing all points of a n-dimensional space.

   * This functor computes for each "dimension" of points in a n-dimensional
   * space the extrema (i.e. minimum and maximum values).  This way,
   * it computes the boundaries of a hyperbox where all points are contained.
   *
   * The points are given in form of vectors.  If a matrix is given, the
   * parameter parameters::rowWise indicates if the vectors are taken from
   * the rows (true) or from the columns (false) of the matrix.
   *
   * Several methods besides the standard apply() are provided in order to
   * \e clip points into a given hyperbox.
   */
  template <class T> class boundsFunctor : public statisticsFunctor {
  public:
    /**
     * the parameters for the class boundsFunctor
     */
    class parameters : public statisticsFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : statisticsFunctor::parameters() {

        rowWise=true;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : statisticsFunctor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "boundsFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        statisticsFunctor::parameters::copy(other);
# else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        statisticsFunctor::parameters& (statisticsFunctor::parameters::* p_copy)
          (const statisticsFunctor::parameters&) =
          statisticsFunctor::parameters::copy;
          (this->*p_copy)(other);
# endif

        rowWise=other.rowWise;

        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * If this flag is true, the mean computation will be row-wise,
       * i.e.  the computation will use the rows of the matrix as row-vectors.
       * This is much faster than column-wise, since data do not have to be
       * copied.
       */
      bool rowWise;
    };

    /**
     * default constructor
     */
    boundsFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    boundsFunctor(const boundsFunctor& other);

    /**
     * destructor
     */
    virtual ~boundsFunctor();

    /**
     * returns the name of this type ("boundsFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * The result of this function depends on the value of
     * parameters.rowWise. If this parameter is true, the functor
     * will compute a vector, whose elements contain each the mean
     * of one column of the matrix (the computes the mean of the rows,
     * where each row is a data point in n-dimensional space.
     * if rowWise is false, the result vector contains the mean
     * of the columns of the matrix (each column a data point).
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum vector of the matrix will be
     *                      left
     * @param max vector<T> where the maximum vector of the matrix will be
     *                      left
     */
    void apply(const matrix<T>& src, vector<T>& min, vector<T>& max) const;

    /**
     * This function will compute a vector, whose elements contain
     * each the minimum of one column of the matrix (this computes the
     * n-dimensional equivalent of a lower-left corner of the bounding
     * box of a data set, where each row is a data point in
     * n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.  */
    void minOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * Alias for minOfRows
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    inline void minOfRowVectors(const matrix<T>& src, vector<T>& dest) const {
      minOfRows(src,dest);
    }

    /**
     * This function
     * will compute a vector, whose elements contain each the maximum
     * of one column of the matrix (this computes the n-dimensional
     * equivalent of an upper-right corner of the bounding box of
     * a data set, where each row is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void maxOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * Alias for maxOfRows
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    inline void maxOfRowVectors(const matrix<T>& src, vector<T>& dest) const {
      maxOfRows(src,dest);
    }

    /**
     * This function
     * will compute a vector, whose elements contain each the minimum
     * of one row of the matrix (this computes the n-dimensional
     * equivalent of a lower-left corner of the bounding box of
     * a data set, where each column is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void minOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * Alias for minOfColumns
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    inline void minOfColumnVectors(const matrix<T>& src,
                                         vector<T>& dest) const {
      minOfColumns(src,dest);
    }

    /**
     * This function
     * will compute a vector, whose elements contain each the maximum
     * of one row of the matrix (this computes the n-dimensional
     * equivalent of an upper-right corner of the bounding box of
     * a data set, where each column is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void maxOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * Alias for maxOfColumns
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    inline void maxOfColumnVectors(const matrix<T>& src,
                                         vector<T>& dest) const {
      maxOfColumns(src,dest);
    }

    /**
     * This function is a combination of minOfRows and maxOfRows
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum will be left.
     * @param max vector<T> where the maximum will be left.
     */
    void boundsOfRows(const matrix<T>& src, vector<T>& min,
                      vector<T>& max) const;

    /**
     * Alias for boundsOfRows
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum will be left.
     * @param max vector<T> where the maximum will be left.
     */
    inline void boundsOfRowVectors(const matrix<T>& src, vector<T>& min,
                      vector<T>& max) const {
      boundsOfRows(src,min,max);
    }

    /**
     * This function is a combination of minOfColumns and maxOfColumns
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum will be left.
     * @param max vector<T> where the maximum will be left.
     */
    void boundsOfColumns(const matrix<T>& src,
                         vector<T>& min, vector<T>& max) const;

    /**
     * Alias for boundsOfColumns
     * @param src matrix<T> with the source data.
     * @param min vector<T> where the minimum will be left.
     * @param max vector<T> where the maximum will be left.
     */
    inline void boundsOfColumnVectors(const matrix<T>& src, vector<T>& min,
                      vector<T>& max) const {
      boundsOfColumns(src,min,max);
    }

    /**
     * Computes a vector which contains the minimum elements of a and b
     * @param a vector<T> first operand
     * @param b vector<T> second operand
     * @param dest vector<T> which will receive the element-wise minimum
     *          of both operands
     */
    void min(const vector<T> &a, const vector<T> &b,
             vector<T>& dest) const;

    /**
     * Computes a vector which contains the maximum elements of a and b
     * @param a vector<T> first operand
     * @param b vector<T> second operand
     * @param dest vector<T> which will receive the element-wise maximum
     *          of both operands
     */
    void max(const vector<T> &a, const vector<T> &b,
             vector<T>& dest) const;


    /**
     * Clips the values of the vector a to lowerBounds and upperBounds.
     * For each vector element a[i],
     * if lowerBounds[i] <= a[i] <= higherBounds[i],
     * the value of a[i] remains unchanged. If a[i] < lowerBounds[i],
     * a[i] will be set to lowerBounds[i]. Analagous with upperBounds.
     * @param a vector<T> to be clipped.
     * @param lowerBounds vector<T> which contains the smallest permitted
     *                    elements.
     * @param upperBounds vector<T> which contains the largest permitted
     *                    elements.
     */
    void clip(vector<T> &a,
              const vector<T>& lowerBounds,
              const vector<T>& upperBounds) const;

    /**
     * Clips the values of the matrix a to lowerBounds and upperBounds.
     * For each matrix element a[i][j],
     * if lowerBounds[i][j] <= a[i][j] <= higherBounds[i][j],
     * the value of a[i][j] remains unchanged. If a[i][j] < lowerBounds[i][j],
     * a[i][j] will be set to lowerBounds[i][j]. Analagous with upperBounds.
     * @param a matrix<T> to be clipped.
     * @param lowerBounds matrix<T> which contains the smallest permitted
     *                    elements.
     * @param upperBounds matrix<T> which contains the largest permitted
     *                    elements.
     */
    void clip(matrix<T> &a,
              const matrix<T>& lowerBounds,
              const matrix<T>& upperBounds) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    boundsFunctor& copy(const boundsFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#include "ltiBoundsFunctor_template.h"

#endif


