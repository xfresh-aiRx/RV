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
 * file .......: ltiTensor_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiTensor_template.h,v 1.3 2005/06/16 12:37:35 doerfler Exp $
 */

#include "ltiTensor.h"

namespace lti {

  /*
   * default constructor creates an empty tensor;
   */
  template<class T>
  tensor<T>::tensor()
    : mathObject(),totalNumberOfElements(0) {
  }

  /*
   * create a tensor of the given order.
   *
   * Each dimension will have the given number of elements, i.e. the tensor
   * will have \f$elements^order\f$ elements.
   *
   * @param order the order of the tensor.
   * @param elements the number of elements per dimension.
   */
  template<class T>
  tensor<T>::tensor(const int& order,const int& elements)
    : mathObject(), totalNumberOfElements(0) {
    resize(order,elements);
  }

  /*
   * create a tensor with the given order.
   *
   * @param order the order of the tensor
   * @param elements a vector with the number of element per dimension
   */
  template<class T>
  tensor<T>::tensor(const int& order,const ivector& elements)
    : mathObject(), totalNumberOfElements(0) {
    resize(order,elements);
  }

  /*
   * create this tensor as a copy of another tensor
   * @param other the tensor to be copied.
   */
  template<class T>
  tensor<T>::tensor(const tensor<T>& other)
    : mathObject() {
    copy(other);
  }

  /*
   * destructor
   */
  template<class T>
  tensor<T>::~tensor() {
  }

  /*
   * change order and element number of the tensor.  All data will
   * be lost!
   *
   * @param order the new order of the tensor
   * @param elements      the number of elements per dimension
   *
   */
  template<class T>
  void tensor<T>::resize(const int& order,
                         const int& elements,
                         const T& initValue,
                         const bool& copyData,
                         const bool& initData) {
    // a temporary order vector
    ivector tmp(order,elements);
    // resize
    resize(order,tmp,initValue,copyData,initData);
  }

  /*
   * change order and element number of the tensor.  All data will
   * be lost!
   *
   * @param order the new order of the tensor
   * @param elements      the number of elements per dimension
   *
   */
  template<class T>
  void tensor<T>::resize(const int& order,
                         const ivector& elements,
                         const T& initValue,
                         const bool& copyData,
                         const bool& initData) {

    if (order == 0) {
      // zero order tensor is a scalar value:
      totalNumberOfElements = 1;
      theOrder = 0;
      theElements.resize(1,initValue,copyData,initData);
      theElementsPerDimension.resize(1,1,false,true);
      firstIndex.resize(1,0,false,true);
      lastIndex.resize(1,0,false,true);
      return;
    }
    // should this tensor be cleaned up?
    else if ((order < 0) || (elements.size() < 1)) {
      totalNumberOfElements = 0;
      theOrder = -1;
      theElements.clear();
      theElementsPerDimension.clear();
      firstIndex.clear();
      lastIndex.clear();
      return;
    }

    // determine how many elements the new tensor should have
    int i,totalElements;

    theElementsPerDimension.resize(order);
    theElementsPerDimension.at(0)=elements.at(0);

    for (totalElements=elements.at(0),i=1;
         i<order;
         i++) {
      totalElements *= elements.at(i % elements.size());
      theElementsPerDimension.at(i)=elements.at(i % elements.size());
    }

    // resize and initialize the vector
    theElements.resize(totalElements,initValue,copyData,initData);
    totalNumberOfElements = totalElements;

    theOrder = order;
    firstIndex.resize(order,0);
    lastIndex.copy(theElementsPerDimension);
    lastIndex.add(-1); // subtract 1 from all elements
  }

  template<class T>
  void tensor<T>::clear() {
    resize(-1,0,T(),false,false);
  }

  /*
   * initialize all elements of the tensor with 0 (or another specified
   * number).
   */
  template<class T>
  void tensor<T>::fill(const T& value) {
    theElements.fill(value);
  }

  /*
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
   *   lti::tensor hist(1,10); // 1D-tensor with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  template<class T>
  void tensor<T>::fill(const T& iniValue,
                       const ivector& from,
                       const ivector& to) {

    if (theOrder == 0) {
      theElements.at(0) = iniValue;
      return;
    }

    ivector first;
    ivector last;

    if (from.size() == 0) {
      first.copy(firstIndex);
    } else {
      first.copy(from);
    }

    if (to.size() == 0) {
      last.copy(lastIndex);
    } else {
      last.copy(to);
    }

    if ((first.size() != last.size()) || (first.size() != theOrder)) {
      // invalid order
      return;
    }

    // can I fill everything?
    if (first.equals(firstIndex) && last.equals(lastIndex)) {
      theElements.fill(iniValue);
      return;
    }

    int i,mi,ma;

    // check and correct the boundaries
    for (i=0;i<theOrder;i++) {
      mi = min(first.at(i),last.at(i));
      ma = max(first.at(i),last.at(i));

      first.at(i)= max( mi, firstIndex.at(i) );
      last.at(i) = min( ma, lastIndex.at(i)  );
    }

    ivector idx(first); // the index! this will iterate on the tensor
                        // hyperbox

    bool ready = false;

    // to check if index at the end, we just need to check the last
    // dimension of the index
    while ( !ready ) {

      // set the element value
      at(idx) = iniValue;

      // idx update
      i = 0;
      while (i<theOrder) {
        idx.at(i)++;
        if (idx.at(i)>last.at(i)) {
          idx.at(i)=first.at(i);
          i++;
          if (i==theOrder) {
            ready = true;
          }
        } else {
          i=theOrder; // ready flag!
        }
      }
    }
  }

  /*
   * read-only access to the element x of the tensor
   * @param x index of the tensor element to be accessed.  It should
   *          be between getFirstIndex() and getLastIndex()
   * @return the number of entries in the given element
   */
  template<class T>
  const T& tensor<T>::at(const ivector& x) const {
    int i,idx;

    for (idx=0,i=theOrder-1;i>=0;i--) {
      assert ((x.at(i)>=0) && (x.at(i)<theElementsPerDimension.at(i)));
      idx*=theElementsPerDimension.at(i);
      idx+=x.at(i);
    }

    return theElements.at(idx);
  }

  /*
   * access element x of the tensor
   * @param x index of the tensor element to be accessed.  It should
   *          be between getFirstIndex() and getLastIndex()
   * @return the number of entries in the given element
   */
  template<class T>
  T& tensor<T>::at(const ivector& x) {
    int i,idx;

    for (idx=0,i=theOrder-1;i>=0;i--) {
      assert ((x.at(i)>=0) && (x.at(i)<theElementsPerDimension.at(i)));
      idx*=theElementsPerDimension.at(i);
      idx+=x.at(i);
    }

    return theElements.at(idx);
  }

  // access operator for zero order tensors

  template<class T>
  const T& tensor<T>::at() const {
    assert(theOrder == 0);
    return theElements.at(0);
  }

  template<class T>
  T& tensor<T>::at() {
    assert(theOrder == 0);
    return theElements.at(0);
  }

  // access operator for first order tensors

  template<class T>
  const T& tensor<T>::at(const int& x) const {
    assert(theOrder == 1);
    return theElements.at(x);
  }

  template<class T>
  T& tensor<T>::at(const int& x) {
    assert(theOrder == 1);
    return theElements.at(x);
  }

  // access operator for second order tensors

  template<class T>
  const T& tensor<T>::at(const int& x,const int& y) const {
    assert(theOrder == 2);
    return theElements.at(y*theElementsPerDimension.at(0)+x);
  }

  template<class T>
  T& tensor<T>::at(const int& x,const int& y) {
    assert(theOrder == 2);
    return theElements.at(y*theElementsPerDimension.at(0)+x);
  }

  // access operator for second order tensors

  template<class T>
  const T& tensor<T>::at(const int& x,const int& y,const int& z) const {
    assert(theOrder == 3);
    return theElements.at((z*theElementsPerDimension.at(1)+y)*
                          theElementsPerDimension.at(0)+x);
  }

  template<class T>
  T& tensor<T>::at(const int& x,const int& y,const int& z) {
    assert(theOrder == 3);
    return theElements.at((z*theElementsPerDimension.at(1)+y)*
                          theElementsPerDimension.at(0)+x);
  }


  /*
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
  template<class T>
    void tensor<T>::detach(tensor<T>& receiver) {

    receiver.totalNumberOfElements = totalNumberOfElements;
    totalNumberOfElements = 0;

    receiver.theOrder = theOrder;
    theOrder = -1;

    theElements.detach(receiver.theElements);
    theElementsPerDimension.detach(receiver.theElementsPerDimension);
    firstIndex.detach(receiver.firstIndex);
    lastIndex.detach(receiver.lastIndex);
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source tensor to be copied.
   * @return a reference to this object
   */
  template<class T>
  tensor<T>& tensor<T>::copy(const tensor<T>& other) {

    totalNumberOfElements = other.totalNumberOfElements;
    theOrder = other.theOrder;
    theElements.copy(other.theElements);
    theElementsPerDimension.copy(other.theElementsPerDimension);
    firstIndex.copy(other.firstIndex);
    lastIndex.copy(other.lastIndex);

    return *this;
  }

  /*
   * create a clone of this tensor
   * @return a pointer to a copy of this tensor
   */
  template<class T>
  mathObject* tensor<T>::clone() const {
    return new tensor<T>(*this);
  }

  /*
   * compare this tensor with another one.
   *
   * @param other the other tensor to be compared with
   * @return true if both tensors have the same elements and same size
   */
  template<class T>
  bool tensor<T>::equals(const tensor<T>& other) const {
    return (( totalNumberOfElements == other.totalNumberOfElements) &&
            ( theOrder == other.theOrder) &&
            ( theElements.equals(other.theElements) ) &&
            ( theElementsPerDimension.equals(other.theElementsPerDimension)));
  }

  /*
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
  template<class T>
  bool tensor<T>::prettyCloseTo(const tensor<T>& other,
                                const T& tolerance) const {
    return (( totalNumberOfElements == other.totalNumberOfElements) &&
            ( theOrder == other.theOrder) &&
            ( theElements.prettyCloseTo(other.theElements,tolerance )) &&
            ( theElementsPerDimension.equals(other.theElementsPerDimension)));
  }

  /*
   * applies a C-function to each element of the tensor.
   * @param function a pointer to a C-function
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::apply(T (*function)(T)) {
    theElements.apply(function);
    return (*this);
  }

  /*
   * applies a C-function to each element of the tensor.
   * @param function a pointer to a C-function
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::apply(T (*function)(const T&)) {
    theElements.apply(function);
    return (*this);
  }

  /*
   * Elementwise multiplication.
   * Each element of this tensor will be multiplied with the elements
   * of the other tensor and the result will be left in this %object!
   *
   * Both tensors are first normalized, then multiplied, and the number of
   * entries is after the multiplication 1!  After this multiplication, this
   * tensor cannot be anymore interpreted as a tensor, but as a
   * combined probabilty distribution.  You can use setNumberOfEntries() to
   * change this fact under your own risk (the semathical meaning of that is
   * left to you!).
   *
   * The returned tensor is this %object!
   *
   * @param other the other tensor to be multiplied with
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::emultiply(const tensor<T>& other) {
    assert ( totalNumberOfElements == other.totalNumberOfElements );
    assert ( theOrder == other.theOrder);
    theElements.emultiply(other.theElements);
    return *this;
  }

  /*
   * Elementwise multiplication.
   * This tensor will contain the elementwise multiplication of the
   * elements in <code>first</code> and <code>second</code>.
   *
   * Both tensors are first normalized, then multiplied, and the number of
   * entries is after the multiplication 1!  After this multiplication, this
   * tensor cannot be anymore interpreted as a tensor, but as a
   * combined probabilty distribution.  You can use setNumberOfEntries() to
   * change this fact under your own risk (the semathical meaning of that is
   * left to you!).
   *
   * @param first the first tensor
   * @param second the second tensor will be multiplied with the
   *               first tensor
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::emultiply(const tensor<T>& first,
                                  const tensor<T>& second) {
    assert ( first.totalNumberOfElements == second.totalNumberOfElements);
    assert ( first.theOrder              == second.theOrder);

    theElements.emultiply(first.theElements,second.theElements);

    return *this;
  }

  /*
   * Add another tensor of the same type and same dimension and
   * leave the result in this %object.  The number of entries of both
   * tensors are also added.
   *
   * @param other the other tensor to be added with
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::add(const tensor<T>& other) {
    assert ( totalNumberOfElements == other.totalNumberOfElements );
    assert ( theOrder     == other.theOrder);

    theElements.add(other.theElements);

    return *this;
  }

  /*
   * Add two tensor and leave the result in this %object.
   * @param first the first tensor.  The number of entries of both
   * tensors are also added.
   * @param second the second tensor will be added with the first
   *               tensor
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::add(const tensor<T>& first,const tensor<T>& second) {
    assert ( first.totalNumberOfElements == second.totalNumberOfElements);
    assert ( first.theOrder     == second.theOrder);

    theElements.add(first.theElements,second.theElements);

    return *this;
  }

  /*
   * Subtracts another tensor of the same type and same dimension
   * and leaves the result in this %object
   *
   * @param other will be substracted from this tensor
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::subtract(const tensor<T>& other) {
    assert ( totalNumberOfElements == other.totalNumberOfElements );
    assert ( theOrder     == other.theOrder);

    theElements.subtract(other.theElements);

    return *this;
  }

  /*
   * Subtracts two tensors and leaves the result in this %object.
   * @param first the first tensor
   * @param second the second tensor will be substracted from the
   *                   first tensor
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::subtract(const tensor<T>& first,
                                 const tensor<T>& second) {
    assert ( first.totalNumberOfElements == second.totalNumberOfElements);
    assert ( first.theOrder     == second.theOrder);

    theElements.add(first.theElements,second.theElements);

    return *this;
  }

  /*
   * Multiply this tensor with a constant.
   * Returns this tensor.  The total number of entries will also be
   * updated. Note that if you use this operation, the number of entries
   * could be false at the end due to numerical instabilities.
   * @see setNumberOfEntries()
   *
   * @param cst constant scalar to be multiplied with
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::multiply(const T& cst) {
    theElements.multiply(cst);

    return *this;
  }

  /*
   * Multiply the other %tensor with a constant and leave the result here.
   * Returns a reference to this tensor.
   * Note that if you use this operation, the number of entries
   * could be false at the end due to numerical instabilities.
   * @see setNumberOfEntries()
   *
   * @param other the other tensor to be multiplied with the
   *              constant value
   * @param cst constant scalar to be multiplied with the other tensor.
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::multiply(const tensor<T>& other,const T& cst) {
    theElements.multiply(other.theElements,cst);
    return *this;
  }

  /*
   * Divide this tensor with a constant.
   * Returns this tensor.  @see setNumberOfEntries()
   *
   * @param cst the elements of the tensor will be divided with this
   *            constant
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::divide(const T& cst) {
    theElements.divide(cst);

    return *this;
  }

  /*
   * Divide the other tensor with a constant and leave the result here.
   * Returns a reference to this tensor.
   * @param other the tensor to be divide by the constant value
   * @param cst the elements of the tensor will be divided with this
   *            constant
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::divide(const tensor<T>& other,const T& cst) {
    theElements.divide(other.theElements,cst);
    return *this;
  }

  /*
   * Add constant to this tensor.  This tensor is changed.
   * Returns this tensor.
   * @param cst constant scala to be added with each element
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::add(const T& cst) {
    theElements.add(cst);
    return *this;
  }

  /*
   * Add constant to the other tensor and leave the result here.
   * Returns a reference to this tensor.
   * @param other the oder tensor
   * @param cst constant scala to be added with each element of the other
   *            tensor
   * @return a reference to the actual tensor
   */
  template<class T>
  tensor<T>& tensor<T>::add(const tensor<T>& other,const T& cst) {
    theElements.add(cst);
    return *this;
  }

  /**
   * write the object in the given ioHandler
   */
  template<class T>
  bool tensor<T>::write(ioHandler& handler,const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
      lti::write(handler,"totalNumberOfElements",totalNumberOfElements);
      lti::write(handler,"order",theOrder);
      lti::write(handler,"data",theElements);
      lti::write(handler,"elementsPerDimension",theElementsPerDimension);
      lti::write(handler,"firstIndex",firstIndex);
      lti::write(handler,"lastIndex",lastIndex);
    }

    if (complete) b = handler.writeEnd();
    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template<class T>
  bool tensor<T>::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
      lti::read(handler,"totalNumberOfElements",totalNumberOfElements);
      lti::read(handler,"order",theOrder);
      lti::read(handler,"data",theElements);
      lti::read(handler,"elementsPerDimension",theElementsPerDimension);
      lti::read(handler,"firstIndex",firstIndex);
      lti::read(handler,"lastIndex",lastIndex);
    }

    if (complete) b = handler.readEnd();
    return b;
  }


} // namespace lti

/// outputs the elements of the tensor on a stream
namespace std {
  template <class T>
    ostream& operator<<(ostream& s,const lti::tensor<T>& v) {

    lti::ivector idx(v.getFirstIndex()); // the index! this will iterate on
                                         // the tensor hyperbox

    const lti::ivector& first = v.getFirstIndex();
    const lti::ivector& last = v.getLastIndex();

    // index of the last dimension
    const int dims = v.order();
    int i,j;
    bool ready = false;

    // string with spaces!
    char* spc = new char[dims+1];
    for (i=0;i<dims;i++) {
      spc[i]=' ';
    }
    spc[dims]=0;

    j = dims;

    // to check if index at the end, we just need to check the last
    // dimension of the index
    while ( !ready ) {

      // j indicates how many "(" must be written

      if (j>0) {
        s << std::endl;

        for (i=dims;i>0;i--) {
          if (i>j)
            s << ' ';
          else
            s << '(';
        }
      }

      s << v.at(idx) << "\t ";

      // idx update
      i = j = 0;

      while (i<dims) {
        idx.at(i)++;
        if (idx.at(i)>last.at(i)) {
          s << ")";
          j++;
          idx.at(i)=first.at(i);
          i++;
          if (i==dims) {
            ready = true;
          }
        } else {
          i=dims; // ready flag!
        }
      }

      // j indicates how many ")" were written.
    }

    return s;
  }
}


