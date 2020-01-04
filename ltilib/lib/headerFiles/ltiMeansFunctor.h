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
 * revisions ..: $Id: ltiMeansFunctor.h,v 1.16 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_MEANS_FUNCTOR_H_
#define _LTI_MEANS_FUNCTOR_H_

#include "ltiStatisticsFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Functor which computes several kinds of means for
   * vectors and matrices.
   *
   * Be careful with the terms used here.  If you have following matrix
   *
   * \code
   * |  5  6  10  3 |
   * |  1 -2   4  5 |
   * |  3  2   1 -2 |
   * \endcode
   *
   * The meanOfRows means the mean of all rows, i.e. a row vector containing
   * (for the arithmetical mean) the sum of all row vectors divided by the
   * number of rows.  This is (3 2 5 2).
   *
   * The meanOfColumns means the mean of all columns, i.e. a column vector
   * containing (for the arithmetical mean) the sum of all column vectors
   * divided by the number of columns.  This is (6 2 1)^T.
   *
   * The parameter parameters::rowWise indicates that the matrix will be
   * seen as a set of row vectors (if true), or column vectors (false).
   *
   * @ingroup gStats
   */
  template <class T> class meansFunctor : public statisticsFunctor {
  public:
    /**
     * the parameters for the class meansFunctor
     */
    class parameters : public statisticsFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters(): statisticsFunctor::parameters() {
        rowWise=true;
        geometric=false;
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
        return "meansFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        statisticsFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        statisticsFunctor::parameters& (statisticsFunctor::parameters::* p_copy)
          (const statisticsFunctor::parameters&) =
          statisticsFunctor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        rowWise=other.rowWise;
        geometric=other.geometric;

        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * If this flag is true, the mean computation will be row-wise, i.e.
       * the result will contain a sum of the rows. This is much faster
       * than column-wise, since data do not have to be copied.
       *
       * Default value: true
       */
      bool rowWise;

      /**
       * If this flag is true, the functor will compute the geometric
       * mean of the elements, i.e. pow(e1*e2*...*en,1/n)
       *
       * Default value: false
       */
      bool geometric;
    };

    /**
     * default constructor
     */
    meansFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    meansFunctor(const meansFunctor& other);

    /**
     * destructor
     */
    virtual ~meansFunctor();

    /**
     * returns the name of this type ("meansFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * returns the mean of the given vector. The type of the mean
     * (arithmetic or geometric) is determined by the parameter
     * parameters::geometric
     * @param src vector<T> with the source data.
     * @return the result.
     */
    T apply(const vector<T>& src) const;

    /**
     * returns the geometric mean of the given vector. For convenience
     * we take the square root of the absolute value of the product of
     * all elements.
     * @param src vector<T> with the source data.
     * @return the geometric mean of the elements of src.
     */
    T geometricMean(const vector<T>& src) const;

    /**
     * returns the arithmetic mean of the given vector.
     * @param src vector<T> with the source data.
     * @return the arithmetic mean of the elements of src.
     */
    T arithmeticMean(const vector<T>& src) const;

    /**
     * computes the mean of the given vector. The type of the mean
     * (arithmetic or geometric) is determined by the parameter
     * parameters::geometric. For geometric mean we take the square
     * root of the absolute value of the product of all elements.
     * @param src vector<T> with the source data.
     * @param dest T where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    T &apply(const vector<T>& src, T& dest) const;

    /**
     * computes the arithmetic mean of the given vector.
     * @param src vector<T> with the source data.
     * @param dest T where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    T &arithmeticMean(const vector<T>& src, T& dest) const;

    /**
     * computes the geometric mean of the given vector. For convenience
     * we take the square root of the absolute value of the product of
     * all elements.
     * @param src vector<T> with the source data.
     * @param dest T where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    T &geometricMean(const vector<T>& src, T& dest) const;

    /**
     * The result of this function depends on the value of
     * parameters::rowWise. If this parameter is true, the functor
     * will compute a vector, whose elements contain each the mean of
     * one column of the matrix (the computes the mean of the rows,
     * where each row is a data point in n-dimensional space.  if
     * rowWise is false, the result vector contains the mean of the
     * columns of the matrix (each column a data point).  The type of
     * the mean (arithmetic or geometric) is determined by the value
     * of parameters::geometric. For the geometric mean we take the
     * square root of the absolute value of the product of all
     * elements.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    vector<T>& apply(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the mean
     * of one column of the matrix (the method computes the mean of the rows,
     * where each row is a data point in n-dimensional space).
     * The type of the mean (arithmetic or geometric) is determined by
     * the value of parameters::geometric. For the geometric mean we take the
     * square root of the absolute value of the product of all
     * elements.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void meanOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the geometric
     * mean
     * of one column of the matrix (the method computes the mean of the rows,
     * where each row is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void geometricMeanOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function will compute a vector, whose elements contain
     * each the arithmetic mean of one column of the matrix (the
     * method computes the mean of the rows, where each row is a data
     * point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void arithmeticMeanOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * The result vector contains the mean of the columns of the
     * matrix (each column a data point).  The type of the mean
     * (arithmetic or geometric) is determined by the value of
     * parameters::geometric. For the geometric mean we take the
     * square root of the absolute value of the product of all
     * elements.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void meanOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * The result vector contains the arithmetic mean
     * of the columns of the matrix (each column a data point).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void arithmeticMeanOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * The result vector contains the geometric mean of the columns of
     * the matrix (each column a data point). For convenience we take
     * the square root of the absolute value of the product of all
     * elements.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void geometricMeanOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    meansFunctor& copy(const meansFunctor& other);

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

#endif


