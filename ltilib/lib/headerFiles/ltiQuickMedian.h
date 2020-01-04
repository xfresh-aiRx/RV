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
 * file .......: ltiQuickMedian.h
 * authors ....: Guy Wafo
 * organization: LTI, RWTH Aachen
 * creation ...: 27.3.2001
 * revisions ..: $Id: ltiQuickMedian.h,v 1.13 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_QUICKMEDIAN_H_
#define _LTI_QUICKMEDIAN_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * This class is used to extract the median of the elements of a
   * given vector or matrix.  The median is defined as the element at
   * the middle position of the sorted vector.  The algorithm used is
   * based on the quick sort.
   * For vectors (or matrices) with an even number n of elements, the
   * median will be the element at (n/2) or (n/2)-1 depending on the
   * parameter settings.
   *
   * The type of the vector elements (T) must accept the operators \<
   * and \>.
   *
   * @ingroup gStats
   */
  template <class T>
  class quickMedian : public functor {
  public:

    /**
     * The parameters for the class quickMedian
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters()
        : functor::parameters() {
          evenCase = TakeLower;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : functor::parameters() {
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
        return "quickMedian<T>::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
         (const functor::parameters&) =
         functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        evenCase = other.evenCase;

        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * This enum specifies how to consider the case of vectors with
       * an even number of elements.
       */
      enum eEvenCase {
        TakeLower, /**< take the element with the lower index, i.e. N/2-1 with
                    *   N the number of elements of the vector
                    */
        TakeHigher /**< take the element with the higher index, i.e. N/2 with
                    *   N the number of elements of the vector
                    */
      };

      /**
       * Specifies how to consider the case when the number of elements of
       * the vector is even (see eEvenCase).
       *
       * This parameter has no effect for odd-sized vectors.
       *
       * Default value: TakeLower
       */
      eEvenCase evenCase;
    };

    /**
     * Default constructor
     */
    quickMedian();

    /**
     * Constructor with indicator what to do for even-sized vectors
     */
    quickMedian(const typename parameters::eEvenCase evenCase);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickMedian(const quickMedian<T>& other);

    /**
     * destructor
     */
    virtual ~quickMedian();

    /**
     * returns the name of this type ("quickMedian")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given matrix, which WILL BE modified.
     * The resulting matrix contain the elements less or equal than the median
     * for the indexes (x,y) such that (x+y*rows() < rows()*columns()/2),
     * and higher or equal otherwise.
     *
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return the median of the matrix
     */
    T apply(matrix<T>& srcdest) const;

    /**
     * calculates the median of the given matrix, which is NOT modified.
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     * @param src matrix<T> with the source data.
     * @param dest the median value of the given matrix.
     * @return the median of the matrix (as reference to the dest parameter)
     */
    T& apply(const matrix<T>& src,T& dest) const;

    /**
     * operates on the given parameter.
     * The resulting vector contains the elements less or equal than the median
     * for the indexes <code>x</code> such that <code>x < size()/2</code>,
     * and higher or equal otherwise.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return a reference to the <code>srcdest</code>.
     */
    T apply(vector<T>& srcdest) const;

    /**
     * operates on the given parameter.
     * @param src vector<T> with the source data.
     * @param dest the median value
     * @return a reference to the <code>dest</code> variable.
     */
    T& apply(const vector<T>& src, T& dest) const;

    /**
     * operates on the given parameter.
     * @param src vector<T> with the source data.
     * @param dest the partially sorted vector.  The elements at the
     *             first half of the vector are less or equal than the median
     *             and on the other half greater or equal.
     * @param median  the median value
     * @return a reference to the <code>median</code> variable.
     */
    T& apply(const vector<T>& src, vector<T>& dest,T& median) const;

    /**
     * operates on the given parameter.
     * The resulting vector contains the elements less or equal than the median
     * for the indexes <code>x</code> such that <code>x < size()/2</code>,
     * and higher or equal otherwise.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return the median value.
     */
    T apply(std::vector<T>& srcdest) const;

    /**
     * operates on the given parameter.
     * @param src vector<T> with the source data.
     * @param dest the median value
     * @return a reference to the <code>dest</code> variable.
     */
    T& apply(const std::vector<T>& src, T& dest) const;

    /**
     * operates on the given parameter.
     * @param src vector<T> with the source data.
     * @param dest the partially sorted vector.  The elements at the
     *             first half of the vector are less or equal than the median
     *             and on the other half greater or equal.
     * @param median  the median value
     * @return a reference to the <code>median</code> variable.
     */
    T& apply(const std::vector<T>& src, std::vector<T>& dest,T& median) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    quickMedian& copy(const quickMedian& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * this method calculates the median in a recursively form
     */
    T  findMedian(vector<T>& vct,
                  const int& begin,
                  const int& end,
                  const int& medianPos) const;
    
    /**
     * this method chooses a pivot-value and ensures that lower values lie
     * at the left and higher values at the right of its final position, which
     * will be returned.
     */
    int partition(vector<T>& vct,
                  const int& begin,
                  const int& end) const;

    /**
     * this method calculates the median in a recursively form
     */
    T  findMedian(std::vector<T>& vct,
                  const int& begin,
                  const int& end,
                  const int& medianPos) const;
    
    /**
     * this method chooses a pivot-value and ensures that lower values lie
     * at the left and higher values at the right of its final position, which
     * will be returned.
     */
    int partition(std::vector<T>& vct,
                  const int& begin,
                  const int& end) const;

  };

  /**
   * This class is used to extract the median of the elements of a
   * given vector or matrix, partitioning at the same time a second
   * vector.  The median is defined as the element at the middle
   * position of the sorted vector.  The algorithm used is based on
   * the quick sort.
   *
   * The difference with the lti::quickMedian functor is that you can
   * "sort" a second vector, which might contain for example the indices
   * of the elements.  This way, you can easily find out, which elements of the
   * original vector are under the median, and which ones above.
   *
   * @see lti::quickMedian, lti::sort, lti::sort2
   *
   * For vectors (or matrices) with an even number n of elements, the
   * median will be the element at (n/2) or (n/2)-1 depending on the
   * parameter settings.
   *
   * The type of the vector elements (T) must accept the operators <
   * and >.
   *
   * @ingroup gStats
   */
  template <class T,class U>
  class quickMedian2 : public quickMedian<T> {
  public:
    
    typedef typename quickMedian<T>::parameters parameters;

    /**
     * default constructor
     */
    quickMedian2();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickMedian2(const quickMedian2<T,U>& other);

    /**
     * destructor
     */
    virtual ~quickMedian2();

    /**
     * returns the name of this type ("quickMedian2")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * The resulting keys vector contains the elements less or equal than the
     * median for the indexes <code>x</code> such that 
     * <code>x < size()/2</code>, and higher or equal otherwise.
     *
     * Both vectors must have the same size.
     *
     * @param keys vector<T> with the key data.  The median of this data
     *             is partially sorted while looking for the median.
     * @param data vector<U> with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     *             
     * @return a the median value of keys.
     */
    T apply(vector<T>& keys,vector<U>& data) const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

  protected:
    /**
     * this method calculates the median in a recursively form
     */
    T  findMedian(vector<T>& vct,
                  vector<U>& data,
                  const int& begin,
                  const int& end,
                  const int& medianPos) const;
    
    /**
     * this method chooses a pivot-value and ensures that lower values lie
     * at the left and higher values at the right of its final position, which
     * will be returned.
     */
    int partition(vector<T>& vct,
                  vector<U>& data,
                  const int& begin,
                  const int& end) const;
  };



}

#include "ltiQuickMedian_template.h"

#endif


