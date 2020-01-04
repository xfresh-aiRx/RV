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
 * file .......: ltiTensor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiTensor.h,v 1.7 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_TENSOR_H_
#define _LTI_TENSOR_H_

#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   *  Tensor template class.
   *
   * The lti::tensor template container class allows the generation
   * multidimensional tables.  If you only need first or second order
   * tensors, please use the lti::vector and lti::matrix instead (they
   * are more efficient!).
   *
   * The elements of the tensor will be indexed in each dimension \e i
   * between 0 an \f$n_i-1\f$.
   *
   *  Example:
   *  \code
   *    lti::tensor<float> hist(3,10); // creates a 3rd order tensor with 10
   *                                   // elements pro dimension.  The total
   *                                   // number of elements is 10^3=1000
   *  \endcode
   *
   * To read the content of a tensor element use the access operator
   * at().
   *
   * For tensors with order 0, the number of elements per dimension will be
   * ignored.
   *
   * @ingroup gAggregate
   */
  template<class T>
  class tensor : public mathObject {
  public:
    /**
     *  The iterator is equivalent to a lti::vector<T>::iterator
     */
    typedef typename vector<T>::iterator iterator;

    /**
     *  The const_iterator is equivalent to a lti::vector<T>::const_iterator
     */
    typedef typename vector<T>::const_iterator const_iterator;

    /**
     * default constructor creates an empty tensor.
     */
    tensor();

    /**
     * create a tensor of the given order.
     *
     * Each "dimension" will have the given number of elements, i.e. the tensor
     * will have \f$elements^{dimensions}\f$ elements.
     *
     * For tensors with order 0, the number of elements per dimension will be
     * ignored.
     *
     * @param order the order of the tensor.
     * @param elements the number of elements per dimension.
     */
    tensor(const int& order,const int& elements);

    /**
     * create a tensor of the given order.
     *
     * Each "dimension" \e i will have the number of elements indicated in
     * the \e i-th element of the vector <code>elements</code>.
     *
     * If the order differs from the size of the given vector, the
     * number of elements for the dimension \e i will be given by
     * \f$dim_i = elements[i \, mod \, elements.size()]\f$.
     *
     * This means, if you want a 6th-order tensor, and your elements-vector
     * has only three elements [10,15,5], the number of elements per dimension
     * will be [10,15,5,10,15,5]
     *
     * For tensors with order 0, the number of elements per dimension will be
     * ignored.
     *
     * @param order the order of the tensor
     * @param elements a vector with the number of elements per dimension
     */
    tensor(const int& order,const ivector& elements);

    /**
     * create this tensor as a copy of another tensor
     * @param other the tensor to be copied.
     */
    tensor(const tensor<T>& other);

    /**
     * destructor
     */
    virtual ~tensor();

    /**
     * returns the name of this class: "tensor"
     */
    const char* getTypeName() const {return "tensor";};

    /**
     * returns the order of this tensor
     */
    inline int order() const;

    /**
     * get the number of elements of the dimension <code>dim</code>
     * @param dimension the index of the dimension to be checked
     * @return the number of elements of the dimension specified by
     *         <code>dim</code>
     */
    inline const int& elementsInDimension(const int& dimension) const;

    /**
     * get the number of elements per dimension
     */
    inline const ivector& elementsPerDimension() const;

    /**
     * returns a vector with the index of the first element of the tensor
     * (usually every element of the vector is 0)
     */
    inline const ivector& getFirstIndex() const;

    /**
     * returns a vector with the index to the last element of the tensor
     * (usually is the value returned by elementsPerDimension() minus (1,1,..1)
     */
    inline const ivector& getLastIndex() const;

    /**
     * returns an iterator pointing to the first element.
     * Note that you can not change the values of the tensor
     * elements when you use a const_iterator. See also begin()
     */
    inline const_iterator begin() const {
      return theElements.begin();
    };

    /**
     * returns an iterator pointing to the first element of the tensor.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     *
     * If you need to iterate on all elements of the tensor, you can
     * use following code:
     * \code
     *   int tmp,accu;                      // a temporal variable
     *   lti::tensor<float> myTensor(3,10); // a 3rd order tensor with
     *                                      // 10 elements/dim. (of type float)
     *   lti::tensor<float>::iterator it;   // an iterator
     *
     *   for (it=myTensor.begin();it!=myTensor.end();it++) {
     *     tmp = *it;                       // tmp has value of element pointed
     *                                      // by the iterator.
     *     accu += tmp;
     *     (*it) = accu;                    // change the value in the tensor.
     *   }
     * \endcode
     *
     * Please note that if you define <code>it</code> as a const_iterator,
     * you can not make something like <code>*it=accu</code>.
     */
    inline iterator begin() {
      return theElements.begin();
    }

    /**
     * returns last index as a const iterator.
     * For an example see begin()
     */
    inline const_iterator end() const {
      return theElements.end();
    }

    /**
     * returns last index as an iterator
     * For an example see begin()
     */
    inline iterator end() {
      return theElements.end();
    };

    /**
     * change the order and number of elements per dimension of the
     * tensor.  All data will be lost!
     *
     * For tensors with order 0, the number of elements per dimension will be
     * ignored.
     *
     * @param order the new order of the tensor
     * @param elements the number of elements per dimension
     * @param initValue value to be copied at each element of the tensor
     *        if the initData parameter is true
     * @param copyData copy the old data into the new tensor (WARNING: Not
     *        implemented yet! (i.e. only false is correctly interpreted))
     * @param initData initialize all data in the tensor with the value
     *        specified in initValue.
     */
    void resize(const int& order,
                const int& elements,
                const T& initValue = T(),
                const bool& copyData = false,
                const bool& initData = true);

    /**
     * change the order and number of elements per dimension of the
     * tensor.  All data will be lost!
     *
     * For tensors with order 0, the number of elements per dimension will be
     * ignored.
     *
     * @param order the new order of the tensor
     * @param elements the number of elements per dimension
     * @param initValue value to be copied at each element of the tensor
     *        if the initData parameter is true
     * @param copyData copy the old data into the new tensor (WARNING: Not
     *        implemented yet! (i.e. only false is correctly interpreted)
     * @param initData initialize all data in the tensor with the value
     *        specified in initValue.
     */
    void resize(const int& order,
                const ivector& elements,
                const T& initValue = T(),
                const bool& copyData = false,
                const bool& initData = true);

    /**
     * transfer the data of this object into the receiver, leaving this
     * vector empty and the receiver as if a copy were done.
     *
     * This function makes a "memory block transfusion" to another
     * tensor. It is a very efficient way to make a copy of this
     * tensor, if you don't need the source data anymore!
     *
     * At the end of the detachment, this tensor will be empty.
     *
     * @param receiver the tensor that will receive the data
     */
    void detach(tensor<T>& receiver);

    /**
     * equivalent to resize(-1,0);
     */
    void clear();

    /**
     * initialize all elements of the tensor with 0 (or another specified
     * number).
     */
    void fill(const T& value = T(0));

    /**
     * fills the tensor elements with <code>iniValue</code> between
     * the n-dimensional points <code>from</code> and <code>to</code>.
     * @param iniValue the elements will be initialized with this
     *                 value.
     * @param from     first element index
     * @param to       last element index
     *
     * If <code>from</code> or <code>to</code> are out of bounds,
     * they will be (internaly) adjusted to a correct value.
     *
     * Example:
     * \code
     *   lti::tensor<float> t(1,10);    // first order tensor with 10 elements
     *   t.clear();
     *   t.fill(9.0f,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
     * \endcode
     */
    void fill(const T& iniValue,
              const ivector& from,
              const ivector& to);

    /**
     * read-only access to the element x of the tensor
     * @param x index of the tensor element to be accessed.  It should
     *          be between getFirstIndex() and getLastIndex()
     * @return a read-only reference to the element at index <code>x</code>.
     */
    const T& at(const ivector& x) const;

    /**
     * access element x of the tensor
     * @param x index of the tensor element to be accessed.  It should
     *          be between getFirstIndex() and getLastIndex()
     * @return a reference to the element at index <code>x</code>.
     */
    T& at(const ivector& x);

    /**
     * overload of the at() operator for zero-order tensors.
     *
     * This method is provided as a shortcut to avoid creating an empty vector
     * to access the zero-order tensor.  If used with tensors of orders other
     * than zero, an assertion will be thrown (in debug mode) or an undefined
     * behaviour must be expected (in release mode).
     */
    T& at();

    /**
     * overload of the at() operator for zero-order tensors.
     *
     * This method is provided as a shortcut to avoid creating an empty vector
     * to access the zero-order tensor.  If used with tensors of orders other
     * than zero, an assertion will be thrown (in debug mode) or an undefined
     * behaviour must be expected (in release mode).
     */
    const T& at() const;

    /**
     * overload of the at() operator for first-order tensors.
     *
     * This method is provided as a shortcut to avoid creating a vector
     * with only one element to access the first-order tensor.  If used
     * with tensors of orders other than one, an assertion will be
     * thrown (in debug mode) or an undefined behaviour must be
     * expected (in release mode).
     *
     * @param x equivalent to the first element of an index vector.
     */
    T& at(const int& x);

    /**
     * overload of the at() operator for first-order tensors.
     *
     * This method is provided as a shortcut to avoid creating a vector
     * with only one element to access the first-order tensor.  If used
     * with tensors of orders other than one, an assertion will be
     * thrown (in debug mode) or an undefined behaviour must be
     * expected (in release mode).
     *
     * @param x equivalent to the first element of an index vector.
     */
    const T& at(const int& x) const;

    /**
     * overload of the at() operator for second-order tensors.
     *
     * This method is provided as a shortcut to avoid creating a vector
     * with only two elements to access the second-order tensor.  If used
     * with tensors of orders other than two, an assertion will be
     * thrown (in debug mode) or an undefined behaviour must be
     * expected (in release mode).
     *
     * @param x equivalent to the first element of an index vector.
     * @param y equivalent to the second element of an index vector.
     */
    T& at(const int& x,const int& y);

    /**
     * overload of the at() operator for second-order tensors.
     *
     * This method is provided as a shortcut to avoid creating a vector
     * with only two elements to access the second-order tensor.  If used
     * with tensors of orders other than two, an assertion will be
     * thrown (in debug mode) or an undefined behaviour must be
     * expected (in release mode).
     *
     * @param x equivalent to the first element of an index vector.
     * @param y equivalent to the second element of an index vector.
     */
    const T& at(const int& x,const int& y) const;

    /**
     * overload of the at() operator for third-order tensors.
     *
     * This method is provided as a shortcut to avoid creating a vector
     * with only three elements to access the third-order tensor.  If used
     * with tensors of orders other than three, an assertion will be
     * thrown (in debug mode) or an undefined behaviour must be
     * expected (in release mode).
     *
     * @param x equivalent to the first element of an index vector.
     * @param y equivalent to the second element of an index vector.
     * @param z equivalent to the third element of an index vector.
     */
    T& at(const int& x,const int& y,const int& z);

    /**
     * overload of the at() operator for third-order tensors.
     *
     * This method is provided as a shortcut to avoid creating a vector
     * with only three elements to access the third-order tensor.  If used
     * with tensors of orders other than three, an assertion will be
     * thrown (in debug mode) or an undefined behaviour must be
     * expected (in release mode).
     *
     * @param x equivalent to the first element of an index vector.
     * @param y equivalent to the second element of an index vector.
     * @param z equivalent to the third element of an index vector.
     */
    const T& at(const int& x,const int& y,const int& z) const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source tensor to be copied.
     * @return a reference to this object
     */
    tensor<T>& copy(const tensor<T>& other);

    /**
     * copy the <code>other</code> tensor by casting each of its elements
     * @param other The tensor to be casted
     */
    template<class U>
    tensor<T>& castFrom(const tensor<U>& other) {
      resize(other.dimensions(), other.elementsPerDimension());

      typename tensor<U>::const_iterator otherIt = other.begin();
      typename tensor<U>::const_iterator endIt = other.end();
      iterator thisIt = begin();

      for(;otherIt != endIt;++otherIt,++thisIt) {
        *thisIt = static_cast<T>(*otherIt);
      }

      return (*this);
    };

    /**
     * create a clone of this tensor
     * @return a pointer to a copy of this tensor
     */
    virtual mathObject* clone() const;

    /**
     * compare this tensor with another one.
     *
     * @param other the other tensor to be compared with
     * @return true if both tensors have the same elements and same size
     */
    bool equals(const tensor<T>& other) const;

    /** compare this tensor with other
     *
     * @param other the other tensor to be compared with
     * @return true if both tensors have the same elements and same size
     */
    inline bool operator==(const tensor<T>& other) const {
      return equals(other);
    };

    /**
     * compare this tensor with another one, and use the given tolerance to
     * determine if the value of each element of the other tensor
     * approximately equals the values of the actual tensor elements.
     *
     * An element \e x is approximately equal to another element \e y
     * with a tolerance \e t, if following equation holds:
     * <i>x</i>-t < <i>y</i> < <i>x</i>+t
     *
     * @param other the other tensor to be compared with
     * @param tolerance the tolerance to be used
     *
     * @return true if both tensors are approximatly equal
     */
    bool prettyCloseTo(const tensor<T>& other,
                       const T& tolerance) const;

    /**
     * assigment operator (alias for copy(other)).
     * @param other the tensor to be copied
     * @return a reference to the actual tensor
     */
    inline tensor<T>& operator=(const tensor<T>& other) {return copy(other);};

    /**
     * applies a C-function to each element of the tensor.
     * @param function a pointer to a C-function
     * @return a reference to the actual tensor
     */
    tensor<T>& apply(T (*function)(T));

    /**
     * applies a C-function to each element of the tensor.
     * @param function a pointer to a C-function
     * @return a reference to the actual tensor
     */
    tensor<T>& apply(T (*function)(const T&));

    /**
     * Elementwise multiplication.
     * Each element of this tensor will be multiplied with the elements
     * of the other tensor and the result will be left in this %object!
     *
     * @param other the other tensor to be multiplied with
     * @return a reference to the actual tensor
     */
    tensor<T>& emultiply(const tensor<T>& other);

    /**
     * Elementwise multiplication.
     * This tensor will contain the elementwise multiplication of the
     * elements in <code>first</code> and <code>second</code>.
     *
     * @param first the first tensor
     * @param second the second tensor will be multiplied with the
     *               first tensor
     * @return a reference to the actual tensor
     */
    tensor<T>& emultiply(const tensor<T>& first,
                         const tensor<T>& second);

    /**
     * Add another tensor of the same type and same order and
     * leave the result in this %object.
     *
     * @param other the other tensor to be added with
     * @return a reference to the actual tensor
     */
    tensor<T>& add(const tensor<T>& other);

    /**
     * Add two tensor and leave the result in this %object.
     *
     * @param first the first tensor.
     * @param second the second tensor will be added with the first
     *               tensor
     * @return a reference to the actual tensor
     */
    tensor<T>& add(const tensor<T>& first,const tensor<T>& second);

    /**
     * Alias for add(const tensor& other)
     */
    inline tensor<T>& operator+=(const tensor<T>& other) {
      return add(other);
    };

    /**
     * Subtracts another tensor of the same type and same order
     * and leaves the result in this %object
     *
     * @param other will be substracted from this tensor
     * @return a reference to the actual tensor
     */
    tensor<T>& subtract(const tensor<T>& other);

    /**
     * Subtracts two tensors and leaves the result in this %object.
     * @param first the first tensor
     * @param second the second tensor will be substracted from the
     *                   first tensor
     * @return a reference to the actual tensor
     */
    tensor<T>& subtract(const tensor<T>& first,
                        const tensor<T>& second);

    /**
     * Alias for substract(const tensor& other)
     */
    inline tensor<T>& operator-=(const tensor<T>& other) {
      return subtract(other);
    };

    /**
     * Multiply this tensor with a constant.
     * Returns this tensor.
     *
     * @param cst constant scalar to be multiplied with
     * @return a reference to the actual tensor
     */
    tensor<T>& multiply(const T& cst);

    /**
     * Multiply the other %tensor with a constant and leave the result here.
     * Returns a reference to this tensor.
     *
     * @param other the other tensor to be multiplied with the
     *              constant value
     * @param cst constant scalar to be multiplied with the other tensor.
     * @return a reference to the actual tensor
     */
    tensor<T>& multiply(const tensor<T>& other,const T& cst);

    /**
     * alias for multiply(const T& cst)
     * @param cst constant scalar to be multiplied with
     * @return a reference to the actual tensor
     */
    inline tensor<T>& operator*=(const T& cst) {
      return multiply(cst);
    };

    /**
     * Divide this tensor with a constant.
     * Returns this tensor.
     *
     * @param cst the elements of the tensor will be divided with this
     *            constant
     * @return a reference to the actual tensor
     */
    tensor<T>& divide(const T& cst);

    /**
     * Divide the other tensor with a constant and leave the result here.
     * Returns a reference to this tensor.
     *
     * @param other the tensor to be divide by the constant value
     * @param cst the elements of the tensor will be divided with this
     *            constant
     * @return a reference to the actual tensor
    */
    tensor<T>& divide(const tensor<T>& other,const T& cst);

    /**
     * Add constant to this tensor.  This tensor is changed.
     * Returns this tensor.
     * @param cst constant scala to be added with each element
     * @return a reference to the actual tensor
     */
    tensor<T>& add(const T& cst);

    /**
     * Alias for add(const T& cst)
     */
    tensor<T>& operator+=(const T& cst) {
      return add(cst);
    }

    /**
     * Add constant to the other tensor and leave the result here.
     * Returns a reference to this tensor.
     * @param other the oder tensor
     * @param cst constant scala to be added with each element of the other
     *            tensor
     * @return a reference to the actual tensor
     */
    tensor<T>& add(const tensor<T>& other,const T& cst);

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
     * the total number of elements
     */
    int totalNumberOfElements;

    /**
     * the dimensionality of this tensor
     */
    int theOrder;

    /**
     * the data of the tensor
     */
    vector<T> theElements;

    /**
     * number of elements
     */
    ivector theElementsPerDimension;

    /**
     * a vector with the right dimension initialized with 0
     */
    ivector firstIndex;

    /**
     * a vector with the right dimension initialized with the
     * number of elements - 1 per dimension
     */
    ivector lastIndex;

  };

  /** @name Storable interface
   *  Members for the storable interface
   */
  //@{
  /**
   * read the matrix from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be read
   */
  template <class T>
  bool read(ioHandler& handler,tensor<T>& hist,const bool complete=true) {
    return hist.read(handler,complete);
  } // immediate implementation to avoid MSVC++ bug!!

  /**
   * write the matrix in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  template <class T>
  bool write(ioHandler& handler,
             const tensor<T>& hist,
             const bool complete=true) {
    return hist.write(handler,complete);
  } // immediate implementation to avoid MSVC++ bug!!
  //@}

}

namespace std {
  /// outputs the elements of the tensor on a stream
  template <class T>
    ostream& operator<<(ostream& s,const lti::tensor<T>& v);
}

#include "ltiTensor_inline.h"


#endif

