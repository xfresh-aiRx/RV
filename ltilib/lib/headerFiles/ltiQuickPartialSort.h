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
 * file .......: ltiQuickPartialSort.h
 * authors ....: Guy Wafo
 * organization: LTI, RWTH Aachen
 * creation ...: 27.3.2001
 * revisions ..: $Id: ltiQuickPartialSort.h,v 1.3 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_QUICKMEDIAN_H_
#define _LTI_QUICKMEDIAN_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include <algorithm>

namespace lti {

  /**
   * Quick partial sort.
   * 
   * This class is used to extract the n smallest elements of a
   * vector, without requiring to complete the sort.  Since the
   * apply method expects the index of the element you want for sure at
   * its destination if the vector were completelly sorted, to get the
   * first n elements you need to give the apply method "n-1".
   *
   * It is usually employed when you just required, for example, the
   * three smallest elements of a vector.  In this case you would
   * apply(2,vct), since "2" is the index of the third element of
   * the vector.  
   *
   * A major difference to the STL method std::partial_sort is that this
   * functor don't even sorts the elements at the begining, it just ensures
   * that they are smaller or equal the element at the given position, and
   * that the elements after the final one at the pivot position are greater
   * or equal the pivot element.  In this sense, this corresponds to the
   * std::partition algorithm, but here we use the LTI-Lib functor-based
   * interface.
   *
   * The type of the vector elements T must accept the operator< and
   * operator=.
   *
   * Note that this functor always sorts in increasing order.
   * If you need the n greatest elements, then just give as pivot-index
   * vct.size()-n, and the last elements are the ones you are looking for.
   *
   * @see lti::sort,lti::quickMedian,lti::quickPartialSort2
   *
   * You may also be interested in the std::nth_element and std::partial_sort
   * functions of the \<algorithms\> file, which can be better if you are
   * dealing with STL structures or if you really want to sort the beginning of
   * your vector.  Since these standard functions are templates, you can also
   * use them with lti::vectors.
   *
   * @ingroup gStats
   */
  template <class T>
  class quickPartialSort : public functor {
  public:

    /**
     * The parameters for the class quickPartialSort
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters() : functor::parameters() {
        pivotIndex = int(2);
      };

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : functor::parameters() {
        copy(other);
      }
      
      /**
       * Destructor
       */
      ~parameters() {
      };

      /**
       * Returns name of this type
       */
      const char* getTypeName() const {
        return "quickPartialSort::parameters";
      };

      /**
       * Copy the contents of a parameters object
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

        
        pivotIndex = other.pivotIndex;
        
        return *this;
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        
        if (b) {
          
          lti::write(handler,"pivotIndex",pivotIndex);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && functor::parameters::write(handler,false);
#     else
        bool (functor::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


#     ifndef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if read was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        
        if (b) {
          
          lti::read(handler,"pivotIndex",pivotIndex);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && functor::parameters::read(handler,false);
#       else
        bool (functor::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if read was successful
       */
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Specifies the position in the vectors that will have its own
       * place, if the whole vector where sorted.  This means,
       * all first "pivotIndex" elements in the output vectors will
       * be smaller or equal the rest of the elements.
       *
       * Default value: 2
       */
      int pivotIndex;

    };

    /**
     * default constructor
     */
    quickPartialSort();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickPartialSort(const quickPartialSort<T>& other);

    /**
     * destructor
     */
    virtual ~quickPartialSort();

    /**
     * returns the name of this type ("quickPartialSort")
     */
    virtual const char* getTypeName() const;

    /**
     * The given matrix is used as source and destination.
     * 
     * The matrix will be considered as a vector, where the rows of the matrix
     * are simply concatenated.  The first parameters::pivotIndex elements of
     * the resulting matrix will be smaller or equall than the rest of
     * the elements.
     *
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Partially sorts given matrix, which is NOT modified.
     *
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     * @param src matrix<T> with the source data.
     * @param dest the median value of the given matrix.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Operates on the given parameter.
     *
     * The resulting vector has its first parameters::pivotIndex elements of
     * smaller or equall than the rest of the elements.
     *
     * @param srcdest vector<T> with the source data.  The result will
     *        be left here too.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(vector<T>& srcdest) const;

    /**
     * Partially sorts the input vector and leaves the result in the
     * destination vector.
     *
     * @param src vector<T> with the source data.
     * @param dest the partially sorted vector.  The first pivotIndex elements
     *             are less or equal than the rest of the elements of the 
     *             vector.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<T>& src, vector<T>& dest) const;

    /**
     * Partially sorts the vector on-place.
     *
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(std::vector<T>& srcdest) const;

    /**
     * operates on the given parameter.
     * @param src vector<T> with the source data.
     * @param dest the partially sorted vector.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const std::vector<T>& src, std::vector<T>& dest) const;

    /**
     * The given matrix is used as source and destination.
     * 
     * The matrix will be considered as a vector, where the rows of the matrix
     * are simply concatenated.  The first parameters::pivotIndex elements of
     * the resulting matrix will be smaller or equall than the rest of
     * the elements.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,matrix<T>& srcdest) const;

    /**
     * Partially sorts given matrix, which is NOT modified.
     *
     * The elements of the matrix will be considered as part of a vector
     * with "columns()" times "rows()" elements.
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param src matrix<T> with the source data.
     * @param dest the median value of the given matrix.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,
               const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Operates on the given parameter.
     *
     * The resulting vector has its first pivotIndex elements of
     * smaller or equall than the rest of the elements.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param srcdest vector<T> with the source data.  The result will
     *        be left here too.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,vector<T>& srcdest) const;

    /**
     * Partially sorts the input vector and leaves the result in the
     * destination vector.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param src vector<T> with the source data.
     * @param dest the partially sorted vector.  The first pivotIndex elements
     *             are less or equal than the rest of the elements of the 
     *             vector.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,
               const vector<T>& src, vector<T>& dest) const;

    /**
     * Partially sorts the vector on-place.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,std::vector<T>& srcdest) const;

    /**
     * Partially sorts the vector on-copy.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param src vector<T> with the source data.
     * @param dest the partially sorted vector.
     *
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,
               const std::vector<T>& src, std::vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    quickPartialSort& copy(const quickPartialSort& other);

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
   * Quick partial sort with colateral effects.
   * 
   * This class is used to extract the n smallest elements of a
   * vector, without requiring to complete the sort, while a second
   * vector is sorted exactly the same way.  
   *
   * It is usually employed when you just required, for example, the
   * three smallest elements of a vector.  
   *
   * If the so-called pivot-index is equal to vct.size()/2 (where vct is the
   * vector you want to partially sort), then this functor is equivalent
   * to the quickMedian.
   *
   * The type of the vector elements T must accept the operator< and
   * operator=.
   *
   * Note that this functor always sorts in increasing order.
   * If you need the n greatest elements, then just give as pivot-index
   * vct.size()-n, and the last elements are the ones you are looking for.
   *
   * @see lti::quickMedian2, lti::sort2, lti::quickPartialSort
   *
   * @ingroup gStats
   */
  template <class T,class U>
  class quickPartialSort2 : public quickPartialSort<T> {
  public:
    
    typedef typename quickPartialSort<T>::parameters parameters;

    /**
     * default constructor
     */
    quickPartialSort2();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    quickPartialSort2(const quickPartialSort2<T,U>& other);

    /**
     * destructor
     */
    virtual ~quickPartialSort2();

    /**
     * returns the name of this type ("quickPartialSort2")
     */
    virtual const char* getTypeName() const;

    /**
     * Partially sort the vector \a keys and reorder the elements of \a data
     * accordingly.
     *
     * Both vectors must have the same size.
     *
     * @param keys vector<T> with the key data.  This vector will
     *             be partially sorted.
     * @param data vector<U> with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     *             
     * @return true if successful, false otherwise.
     */
    bool apply(vector<T>& keys,vector<U>& data) const;

    /**
     * Partially sort the vector \a keys and reorder the elements of \a data
     * accordingly.
     *
     * Both vectors must have the same size.
     *
     * @param keys vector<T> with the key data.  This vector will
     *             be partially sorted.
     * @param data vector<U> with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     *             
     * @return true if successful, false otherwise.
     */
    bool apply(std::vector<T>& keys,vector<U>& data) const;

    /**
     * Partially sort the vector \a keys and reorder the elements of \a data
     * accordingly.
     *
     * Both vectors must have the same size.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param keys vector<T> with the key data.  This vector will
     *             be partially sorted.
     * @param data vector<U> with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     *             
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,
               vector<T>& keys,vector<U>& data) const;

    /**
     * Partially sort the vector \a keys and reorder the elements of \a data
     * accordingly.
     *
     * Both vectors must have the same size.
     *
     * @param pivotIndex pivot position on the given vector.  The result
     *                   has the first "pivotIndex" elements smaller or
     *                   equal the rest of the vector elements.
     * @param keys vector<T> with the key data.  This vector will
     *             be partially sorted.
     * @param data vector<U> with data to be sorted the same way as the keys.
     *             You can for example use a ivector initialized with the
     *             index values (i.e. data(i)=i), so that after the apply
     *             method you can check which elements are below the median
     *             and which above.
     *             
     * @return true if successful, false otherwise.
     */
    bool apply(const int pivotIndex,
               std::vector<T>& keys,vector<U>& data) const;

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

    /**
     * this method calculates the median in a recursively form
     */
    T  findMedian(std::vector<T>& vct,
                  vector<U>& data,
                  const int& begin,
                  const int& end,
                  const int& medianPos) const;
    
    /**
     * this method chooses a pivot-value and ensures that lower values lie
     * at the left and higher values at the right of its final position, which
     * will be returned.
     */
    int partition(std::vector<T>& vct,
                  vector<U>& data,
                  const int& begin,
                  const int& end) const;

  };



}

#include "ltiQuickPartialSort_template.h"

#endif


