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
 * file .......: ltiArray.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiArray.h,v 1.18 2005/11/24 14:21:32 doerfler Exp $
 */

#ifndef _LTI_ARRAY_H_
#define _LTI_ARRAY_H_

#include "ltiVector.h"
#include "ltiAssert.h"

namespace lti {
  /**
   * Array class.
   *
   * A n-dimensional array indexed between \a from and \a to, where
   * <em>to-from+1 = n</em>.  The indices \a from and \a to can be negative,
   * but \a from must be lower or equal \a to.
   *
   * All inherited arithmetical operations still need to be reviewed, since
   * the vector methods will only check if the size \a n of the array is
   * correct.  Use these operations at your own risk.  It is safer if you use
   * the on-place version of the arithmetical methods, since they won't change
   * the index range of the array. Otherwise, the range will be change to
   * [0,n-1].
   * 
   * @ingroup gAggregate
   */
  template<class T> class array : public vector<T> {
  public:
    /**
     * default constructor creates an empty array;
     */
    array();

    /**
     * constructor.
     * creates an array indexed between \p from and \p to
     * and initializes it with \p iniValue
     */
    array(int from,int to,const T& iniValue = T());


    /**
     * copy constructor.
     * copy contents of \p other array in this array.
     */
    array(const array<T>& other);

    /**
     * copy constructor.
     * copy contents of \p other vector in this array.
     */
    array(const vector<T>& other,int theOffset=0);

    /**
     * If \a init is true this constructor is equivalent to calling
     * array(int theSize), and thus initializing
     * all elements with T(). However, in some cases the elements need
     * not be initialized during construction, since complex
     * initializion is required. Especially for large arrays, the
     * unnecessary constructor initialization is very time consuming.
     *
     * If \a init is false, memory is allocated but no initialization
     * takes place.
     *
     * @param init initialize matrix or not
     * @param from first index
     * @param to last index
     */
    array(bool init, int from, int to);


    /**
     * destructor
     */
    virtual ~array();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * change the dimension and/or range of the array.
     *
     * If the new dimension of the array (to-from+1) is different from
     * the old one or if this array doesn't own the data, new memory
     * is allocated.
     *
     * If \p copyData is true the data is copied according to the
     * indices, e.g. if the old array is (-2, 2) and contains the
     * values 0 to 4 and a resize(0,4,10) is performed the new array
     * will be [2, 3, 4, 10, 10]. If the indices don't overlap no
     * copying takes place.
     *
     * To adjust the range of an array, i.e. the data contained and
     * the size stays the same, but the start and end indices change,
     * use shift(int).
     *
     * If the new size is not equal to the old size (i.e. to-from+1),
     * the array always owns the data afterwards (i.e. new memory is
     * allocated) even if it didn't own the data before. Otherwise the
     * ownership remains unchanged. You can use restoreOwnership() if
     * you just want to own the data.
     */
    void resize(int from,
		int to,
		const T& iniValue = T(),
		bool copyData = true,
		bool initNew = true);

    /**
     * Shift all indices of the array by the given value \p
     * sh. Negative values result in a left shift, positive values in
     * a right shift.
     *
     * An array with range (-2,2) will have range (0,4) after shift(2)
     * is performed.
     */
    void shift(int sh);

    /**
     * fills the array with value \p data between \p from
     * and \p to.
     */
    void fill(const T& data, int from = MinInt32,
	      int to = MaxInt32);

    /**
     * returns first index.  the returned value is equal to \p from by
     * construction or resizing.
     */
    inline int firstIdx() const {return firstArrayElement;};

    /**
     * change the first index of the array, without changing the content
     */
    void setFirstIdx(int newIdx);

    /**
     * returns last index.
     * the returned value is equal to \p to by contruction or
     * resizing
     */
    inline int lastIdx() const {return lastArrayElement;};

    /**
     * access operator.
     * access element x with firstIdx() <= x <= lastIdx().
     */
    inline T& at(int x) {
      assert ( (firstIdx() <= x) && (x <= lastIdx()) );
      assert ( this->theElements != 0 );
      return ( theElement0[x] );
    };

    /**
     * const access operator.
     * access element x with firstIdx() <= x <= lastIdx().
     */
    inline const T& at(int x) const {
      assert ( (firstIdx() <= x) && (x <= lastIdx()) );
      assert ( this->theElements != 0 );
      return ( theElement0[x] );
    };

    /**
     * access operator (alias for at(x)).
     */
    inline T& operator[](int x) {return at(x);};

    /**
     * const access operator (alias for at(x))
     */
    inline const T& operator[](int x) const {return at(x);};

    /**
     * assigment operator.
     * copy content of vector \p other
     */
    array<T>& copy(const vector<T>& other);

    /**
     * assigment operator.
     * copy content of array \p other
     */
    array<T>& copy(const array<T>& other);

    /**
     * assigment operator. (alias for copy(other).
     */
    array<T>& operator=(const vector<T>& other) {return copy(other);};

    /**
     * assigment operator. (alias for copy(other).
     */
    array<T>& operator=(const array<T>& other)  {return copy(other);};

    /**
     * cast and copy the elements of the "other" array
     */
    template<class U>
    array<T>& castFrom(const array<U>& other) {

      vector<T>::castFrom(other);

      offset = -other.firstIdx();
      firstArrayElement = other.firstIdx();
      lastArrayElement  = other.lastIdx();
      theElement0 = this->theElements + offset;

      return (*this);
    }


    /**
     * @name Apply Methods
     */
    //@{

    /**
     * applies a C-function to each element of the array.
     * 
     * In the following example, %array \a vct is initialized with
     * 4.0. After applying \a sqrt(), all elements of \a vct are 2.0.
     * \code
     * array<float> vct(4,4.0);
     * vct.apply(sqrt);
     * \endcode
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(T (*function)(T));

    /**
     * applies a C-function to each element of the other array and leaves
     * the result here.
     * @param other the source array
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(T));

    /**
     * applies a C-function to each element of the array.
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(T (*function)(const T&));

    /**
     * applies a C-function to each element the other array and
     * leaves the result here.
     * @param other the array with the source data
     * @param function a pointer to a C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(const T&));

    /**
     * a two-parameter C-function receives the i-th elements of this
     * and the given array and the result will be left in this
     * array.  Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     * @param other the second array to be considered (the first
     *              array will be this object!)
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(const T&,const T&));

    /**
     * a two-parameter C-function receives the i-th elements of this
     * and the given array and the result will be left in this
     * array.  Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     * @param other the second array to be considered (the first
     *              array will be this object!)
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& other,T (*function)(T,T));

    /**
     * a two-parameter C-function receives the i-th elements of the
     * given arrays and leaves the result here.
     * Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     *
     * The following example uses lti::min as function. The arrays \a
     * a and \a b contain the values [1,2,3,4] and [4,3,2,1],
     * respectively. After applying the function, %array \a c
     * contains the values [1,2,2,1].
     * \code 
     * iarray a,b,c;
     * int i=0;
     * for (i=0; i<4; ++i) {
     *   a.at(i)=i+1;
     *   b.at(i)=4-i;
     * }
     * c.apply(a,b,lti::min);
     * \endcode
     * @param a the first array
     * @param b the second array
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& a,
                     const array<T>& b,
                     T (*function)(const T&,const T&));

    /**
     * a two-parameter C-function receives the i-th elements of the
     * given arrays and leaves the result here.
     * Note that both arrays MUST have the same size!
     * If both arrays have different size, the function will throw an
     * assertion without changing anything!
     * @param a the first array
     * @param b the second array
     * @param function a pointer to a two parameters C-function
     * @return a reference to the actual array
     */
    array<T>& apply(const array<T>& a,
                     const array<T>& b,
                     T (*function)(T,T));

    //@}


    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);


  protected:
    /**
     * offset for the 0-th element
     */
    int offset;

    /**
     * index of the first element of the array
     */
    int firstArrayElement;

    /**
     * index of the last element of the array.
     *
     * if the array is casted to a vector, the "lastIdx()" should return
     * size()-1 instead of the array lastIdx!.  That's the reason why
     * we use a new member for the last index in the array!
     */
    int lastArrayElement;

    /**
     * A pointer to the element at index 0
     * (This allows a faster access than adding the offset each time)
     */
    T* theElement0;
  };

}

#endif

