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
 * file .......: ltiSort.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.8.2000
 * revisions ..: $Id: ltiSort.h,v 1.21 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SORT_H_
#define _LTI_SORT_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiPerformanceConfig.h"

namespace lti {

  /**
   * Sort vectors.
   *
   * This class is used to sort the elements of a given vector or matrix.
   *
   * The sort::parameters::order specify if the elements should be sorted in
   * ascending or descending order.
   *
   * This functor requires that the type T accept the operator<.
   *
   * @see lti::scramble, lti::sort2, lti::quickPartialSort
   *
   * The functor uses the well-known quick-sort algorithm to sort the elements
   * of the vector.  Since the overhead of the recursion makes at some point
   * the quick-sort more innefficient than simpler algorithms, you can specify
   * in the parameters for which size the vectors should be sorted with 
   * the bubble-sort algorithm.
   *
   * The quick-sort is not "stable", this means that elements with the same
   * key value can change their positions in the vector.
   *
   * You should also revise the STL algorithms std::sort() if you are using 
   * containers of the STL.
   */
  template <class T>
  class sort : public functor {
  public:

    /**
     * The parameters for the class sort
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Type used in the specification of the sorting order
       */
      enum eOrder {
        Ascending, /*!< ascending order  */
        Descending /*!< descending order */
      };

      /**
       * Default constructor
       */
      parameters()
        : functor::parameters() {

        thresholdForBubble = int(_LTI_PERFORMANCE_SORT_STOP_QUICKSORT);
        order = Ascending;
      };

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : functor::parameters() {
        copy(other);
      };

      /**
       * Destructor
       */
      ~parameters() {};

      /**
       * Returns name of this type
       */
      const char* getTypeName() const {
        return "sort<T>::parameters";
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

        thresholdForBubble = other.thresholdForBubble;
        order = other.order;

        return *this;
      };

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          lti::read(handler,"thresholdForBubble",thresholdForBubble);

          std::string str;
          lti::read(handler,"order",str);
          if (str == "Ascending") {
            order = Ascending;
          } else {
            order = Descending;
          }
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && functor::parameters::read(handler,false);
#     else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"thresholdForBubble",thresholdForBubble);
          if (order == Ascending) {
            lti::write(handler,"order","Ascending");
          } else {
            lti::write(handler,"order","Descending");
          }
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && functor::parameters::write(handler,false);
#       else
        bool (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif

      // ---------------------------------------------------------
      //  Parameters
      // ---------------------------------------------------------


      /**
       * For vector/matrices with size smaller than this value, a bubble sort
       * will be used (this way is faster)!
       *
       * The default value is usually 10, but if you configured your
       * system for performance this value could change.
       *
       * The best value can be found in the ltiPerformanceConfig.h file,
       * under _LTI_PERFORMANCE_SORT_STOP_QUICKSORT.
       *
       * Default value: 10 or better.
       */
      int thresholdForBubble;

      /**
       * Order of the sorting
       * the default value is ascending order
       */
      eOrder order;
    };

    /**
     * Default constructor
     */
    sort(const bool& descendingOrder = false);

    /**
     * Construct with given parameters
     */
    sort(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    sort(const sort<T>& other);

    /**
     * Destructor
     */
    virtual ~sort();

    /**
     * Returns the name of this type ("sort")
     */
    virtual const char* getTypeName() const;

    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the matrix at the left will
     * be sorted into the matrix at the right side:
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise
     */
    virtual bool apply(matrix<T>& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise
     */
    virtual bool apply(vector<T>& srcdest) const;

    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the matrix at the left will
     * be sorted into the matrix at the right side:
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if successful, false otherwise
     */
    virtual bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if successful, false otherwise
     */
    virtual bool apply(const vector<T>& src,vector<T>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sort& copy(const sort& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set parameters
     */
    bool updateParameters();

  protected:
    void quicksort(vector<T>& vct,
                   const int& begin,
                   const int& end) const;
    int partitionAsc(vector<T>& vct,
                     const int& begin,
                     const int& end) const;
    void insertionsortAsc(vector<T>& vct,
                          const int& begin,
                          const int& end) const;
    int partitionDesc(vector<T>& vct,
                      const int& begin,
                      const int& end) const;
    void insertionsortDesc(vector<T>& vct,
                           const int& begin,
                           const int& end) const;

    /**
     * @name Shadows of the parameters to avoid a function access
     */
    //@{
    int thresholdForBubble;
    typename parameters::eOrder order;
    //@}
      
  };

  /**
   * Sort two vectors, using the first one as key.
   *
   * This class is used to sort the elements of two given vectors or
   * matrices.  The first one (of type T) contains always the keys to
   * be used by sorting, and the second (of type U) one will be sorted
   * accordingly to first one. 
   *
   * For example, if you have a second vector of integers, which was
   * initialized with the indices (0 for the first element, 1 for the second
   * and so on), at the end you can use this sorted vector to know which
   * position has an element of the first vector after sorting:
   *
   * \code
   * // the key vector
   * const float fdata[] = {3.2, 1.5, 4.2, 2.0};
   * lti::vector<float> a(4,fdata);
   *
   * // the data vector
   * const int idata[] = {0,1,2,3};
   * lti::vector<int> idx(4,idata);
   *
   * // sorting object:
   * sort2<float,int> sorter;
   *
   * // sort the vector a, and accordingly the vector b
   * sorter.apply(a,b);
   *
   * // after this you will get:
   * // a = 1.5, 2.0, 3.2, 4.2
   * // b = 1  , 3  , 0  , 2
   * \endcode
   *
   * The sort2::parameters inherit from sort::parameters, and therefore you can
   * also here specify the sorting order and the threshold for applying bubble-
   * sort.
   *
   * This functor requires that the type T accept the operator<.
   *
   * @see lti::scramble, lti::sort, lti::quickPartialSort2
   */
  template <class T,class U = int>
  class sort2 : public sort<T> {
  public:
    /**
     * The parameters for the class sort
     */
    class parameters : public sort<T>::parameters {
    public:
      /**
       * Type used in the specification of the sorting order
       * (used when sorting the rows or columns of a matrix using as keys
       *  the values of a vector)
       */
      enum eWhichVectors {
        Columns, /*!< sort the columns of the matrix  */
        Rows     /*!< sort the rows of the matrix     */
      };

      /**
       * Default constructor
       */
      parameters()
        : sort<T>::parameters() {

        whichVectors = Rows;
      };

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : sort<T>::parameters() {
        copy(other);
      };

      /**
       * Destructor
       */
      ~parameters() {};

      /**
       * Returns name of this type
       */
      const char* getTypeName() const {
        return "sort2<T>::parameters";
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        sort<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        sort<T>::parameters& (sort<T>::parameters::* p_copy)
          (const sort<T>::parameters&) = sort<T>::parameters::copy;
        (this->*p_copy)(other);
#     endif

        whichVectors = other.whichVectors;

        return *this;
      };

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          std::string str;
          lti::read(handler,"whichVectors",str);
          if (str == "Columns") {
            whichVectors = Columns;
          } else {
            whichVectors = Rows;
          }
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && sort<T>::parameters::read(handler,false);
#     else
        bool (sort<T>::parameters::* p_readMS)(ioHandler&,const bool) =
          sort<T>::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          if (whichVectors == Columns) {
            lti::write(handler,"whichVectors","Columns");
          } else {
            lti::write(handler,"whichVectors","Rows");
          }
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && sort<T>::parameters::write(handler,false);
#       else
        bool (sort<T>::parameters::* p_writeMS)(ioHandler&,const bool) const =
          sort<T>::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * Write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif

      // ---------------------------------------------------------
      //  Parameters
      // ---------------------------------------------------------


      /**
       * Specify if in the apply(vector<T>,matrix<U>) the rows or the columns
       * of the matrix should be sorted.
       * The default value is Rows
       */
      eWhichVectors whichVectors;

    };


    /**
     * Default constructor.
     * @param descendingOrder if true, the vector/matrix will be sorted
     *                        in descending order.  If false, in ascending
     *                        order.
     * @param sortRows used in apply(vector,matrix).  If true, then the
     *                        rows of the matrix are sorted using vector as
     *                        a key.  Otherwise the columns of the matrix
     *                        will be sorted.
     */
    sort2(const bool& descendingOrder=false,const bool& sortRows=true);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    sort2(const sort2<T,U>& other);

    /**
     * Construct with given parameters
     */
    sort2(const parameters& par);

    /**
     * Destructor
     */
    virtual ~sort2();

    /**
     * Returns the name of this type ("sort2")
     */
    virtual const char* getTypeName() const;

    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the key matrix at the left will
     * be sorted into the matrix at the right side:
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param key matrix<T> with the key data.  The result will be
     * left here too.
     * @param srcdest matrix<U> with the data that will be sorted
     * according to the key data
     * @return true if successful, false otherwise
     */
    bool apply(matrix<T>& key,matrix<U>& srcdest) const;

    /**
     * Operates on the given parameter.
     * @param key vector<T> with the key data.  The result
     *                      will be left here too.
     * @param srcdest vector<U> with the data that will be sorted according
     *                          to the key data
     * @return true if successful, false otherwise
     */
    bool apply(vector<T>& key,vector<U>& srcdest) const;

    /**
     * Sort the rows of the matrix, using as key the elements of the given
     * vector.  For example, the matrix at the left side will be sorted in
     * the matrix at the right side with the key-vector (5 2 3):
     *
     * \code
     *
     *  | 2 8 3 |         | 1 4 5 |
     *  | 1 4 5 |  --->   | 7 9 6 |
     *  | 7 9 6 |         | 2 8 3 |
     *
     * \endcode
     *
     * The number of rows of the matrix must be equal to the number of
     * elements in the key vector.
     *
     * @param key vector<T> with the key data.  The result
     *                      will be left here too.
     * @param srcdest matrix<U> with the rows that will be sorted according
     *                          to the key data
     * @return true if successful, false otherwise
     */
    bool apply(vector<T>& key,matrix<U>& srcdest) const;


    /**
     * Sort all the elements of the matrix.  The elements will be
     * ordered row-wise.  For example, the key matrix at the left will
     * be sorted into the matrix at the right side:
     *
     * \code
     *
     *  | 2 8 3 |         | 1 2 3 |
     *  | 1 4 5 |  --->   | 4 5 6 |
     *  | 7 9 6 |         | 7 8 9 |
     *
     * \endcode
     * @param key matrix<T> with the key data.
     * @param src matrix<U> with the source data.
     * @param destkey matrix<T> where the sorted keys will be left.
     * @param dest matrix<U> where the sorted data (using the key) will
     *                       be left.
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& key, const matrix<U>& src,
                     matrix<T>& destkey,matrix<U>& dest) const;

    /**
     * Operates on a copy of the given parameters.
     * @param key vector<T> with the key data.
     * @param src vector<U> with the source data.
     * @param destkey vector<T> where the sorted keys will be left.
     * @param dest vector<U> where the sorted data (using the key) will
     *                       be left.
     * @return true if successful, false otherwise
     */
    bool apply(const vector<T>& key,const vector<U>& src,
                     vector<T>& destkey, vector<U>& dest) const;

    /**
     * Sort the rows of the matrix, using as key the elements of the given
     * vector.  For example, the matrix at the left side will be sorted in
     * the matrix at the right side with the key-vector (5 2 3):
     *
     * \code
     *
     *  | 2 8 3 |         | 1 4 5 |
     *  | 1 4 5 |  --->   | 7 9 6 |
     *  | 7 9 6 |         | 2 8 3 |
     *
     * \endcode
     *
     * The number of rows of the matrix must be equal to the number of
     * elements in the key vector.
     *
     * @param key vector<T> with the key data.
     * @param src matrix<U> with the rows that will be sorted according
     *                          to the key data
     * @param destkey the sorted key-vector
     * @param dest the matrix with sorted rows.
     * @return true if successful, false otherwise
     */
    bool apply(const vector<T>& key,const matrix<U>& src,
                     vector<T>& destkey, matrix<U>& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sort2& copy(const sort2& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    void quicksort(vector<T>& vct,vector<U>& vct2,
                   const int& begin,
                   const int& end) const;
    int partitionAsc(vector<T>& vct,vector<U>& vct2,
                     const int& begin,
                     const int& end) const;
    void insertionsortAsc(vector<T>& vct,vector<U>& vct2,
                          const int& begin,
                          const int& end) const;
    int partitionDesc(vector<T>& vct,vector<U>& vct2,
                      const int& begin,
                      const int& end) const;
    void insertionsortDesc(vector<T>& vct,vector<U>& vct2,
                           const int& begin,
                           const int& end) const;
    void reorder(const ivector& indices,
                 const matrix<U>& src,
                       matrix<U>& dest) const;
  };


}

#include "ltiSort_template.h"

#endif


