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
 * file .......: ltiGenericVector.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiGenericVector_template.h,v 1.13 2005/11/24 14:21:32 doerfler Exp $
 */

// This allows to deactivate the instantiation of the generic vector
#ifndef _LTI_GENERIC_VECTOR_DONT_INSTANTIATE_REQUEST

#ifndef _LTI_GENERIC_VECTOR_TEMPLATE_H_
#define _LTI_GENERIC_VECTOR_TEMPLATE_H_

namespace lti {
  // implementation of template functions

  // -------------------------------------------------------------------
  // genericVector
  // -------------------------------------------------------------------
  // default constructor
  template<class T>
  genericVector<T>::genericVector()
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {
  }

  template<class T>
  genericVector<T>::genericVector(int theSize,const T& iniValue)
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {

    if (theSize <= 0) {
      return;
    }

    vectorSize = theSize;
    idxLastElement = theSize-1;
    ownData = true;

    theElements = new T[vectorSize];

    if (theElements == 0) { // allocation check
      throw allocException();
    }

    fill(iniValue);
  }

  template<class T>
  genericVector<T>::genericVector(int theSize,const T* data)
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {

    if (theSize <= 0) {
      return;
    }

    vectorSize = theSize;
    idxLastElement = theSize-1;
    ownData = true;

    theElements = new T[vectorSize];

    if (theElements == 0) { // allocation check
      throw allocException();
    }

    fill(data);
  }


  template<class T>
  genericVector<T>::genericVector(int theSize,
                          T* data,
                    const bool constRef)
    : vectorSize(theSize),idxLastElement(vectorSize-1),
      theElements(data),ownData(false),constReference(constRef) {

    if (theSize <= 0) {
      vectorSize = 0;
      idxLastElement = -1;
      theElements = 0;
      ownData = true;
      constReference = false;
      return;
    }

  }

  // copy constructor
  template<class T>
  genericVector<T>::genericVector(const genericVector<T>& other)
    : mathObject(),vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {

    vectorSize = other.size();
    idxLastElement = vectorSize-1;
    ownData = true;

    if (vectorSize <= 0) {
      return;
    }

    theElements = new T[vectorSize];

    if (theElements == 0) { // allocation check
      throw allocException();
    }
    // copy contents of other genericVector
    if (vectorSize>0) {
      memcpy(theElements,other.theElements,vectorSize*sizeof(T));
    }
  }

  // copy constructor
  template<class T>
  genericVector<T>::genericVector(const genericVector<T>& other, int from, int to)
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {
    copy(other, from, to);
  }

  // copy constructor
  template<class T>
  genericVector<T>::genericVector(const genericVector<T>& other, const genericVector<int>& idx)
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {
    copy(other, idx);
  }

  // pseudo-copy constructor
  template<class T>
  genericVector<T>::genericVector(const std::vector<T>& other)
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {

    vectorSize = static_cast<int>(other.size());
    idxLastElement = vectorSize-1;
    ownData = true;

    if (vectorSize <= 0) {
      return;
    }

    theElements = new T[vectorSize];

    if (theElements == 0) { // allocation check
      throw allocException();
    }

    // copy contents of other genericVector
    typename std::vector<T>::const_iterator it;
    typename genericVector<T>::iterator dit;

    for (it=other.begin(),dit=begin();it!=other.end();++it,++dit) {
      *dit = *it;
    }
  }

  template<class T>
  genericVector<T>::genericVector(bool init, int theSize)
    : vectorSize(0),idxLastElement(-1),
      theElements(0),ownData(true),constReference(0) {
    if (init) {
      resize(theSize,T(),false,true);
    } else {
      resize(theSize,T(),false,false);
    }

  }

  template<class T>
  genericVector<T>::~genericVector() {
    if (ownData) {
      delete[] theElements;
    }
    theElements = 0;
    vectorSize = 0;
    idxLastElement = -1;
  }

  template<class T>
  void genericVector<T>::useExternData(int theSize,T* data,
        bool constRef) {
    if (constReference)
      throw constReferenceException();

    vectorSize = theSize;
    idxLastElement = theSize-1;

    if (ownData) {
      delete[] theElements;
    }

    theElements = data;
    ownData = false; // do not delete data!
    constReference = constRef;
  }

  // attach external memory region to this object
  template <class T>
  void genericVector<T>::attach(int theSize, T* data) {
    useExternData(theSize,data);
    ownData = true;
  }

  // detach internal memory to an external 'receiver' genericVector
  template <class T>
  void genericVector<T>::detach(genericVector<T>& receiver) {
    // receiver should know whether he owns data, so it is not
    // accidentally deleted
    if (ownData) {
      receiver.attach(vectorSize,theElements);
    } else {
      receiver.useExternData(vectorSize,theElements);
    }
    ownData = false;
    resize(0,T(),false,false);
  }

  template <class T>
  void genericVector<T>::exchange(genericVector<T>& other) {
    swap(other);
  }

  // detach internal memory to an external 'receiver' genericVector
  template <class T>
  void genericVector<T>::swap(genericVector<T>& other) {

    if (constReference || other.constReference) {
      throw constReferenceException();
    }

    bool tmpBool;
    T* tmpTPtr;
    int tmpInt;

    if (vectorSize != other.vectorSize) {
      tmpInt = vectorSize;
      vectorSize = other.vectorSize;
      other.vectorSize = tmpInt;

      tmpInt = idxLastElement;
      idxLastElement = other.idxLastElement;
      other.idxLastElement = tmpInt;
    }

    tmpTPtr = theElements;
    theElements = other.theElements;
    other.theElements = tmpTPtr;

    tmpBool = ownData;
    ownData = other.ownData;
    other.ownData = tmpBool;
  }


  template<class T>
  void genericVector<T>::resize(int newSize,
                                const T& iniValue,
                                bool copyData,
                                bool initNew) {

    // no allocation required?
    if (newSize == vectorSize) {
      if (initNew && !copyData) {
        fill(iniValue,0,lastIdx());
      }
      return;
    }

    if (constReference) {
      throw constReferenceException();
    }

    int from = 0;

    if (newSize <= 0) {
      if (ownData) {
        delete[] theElements;
      } else {
        ownData = true;
      }

      theElements = 0;
      vectorSize = 0;
      idxLastElement = -1;

      return; // ready!!!
    }

    T* newElements = new T[newSize];

    if (newElements == 0) { // check memory allocation
      throw allocException();
    }

    if (copyData) {
      // old data should be copied
      const int top = min(newSize,size());
      if (top>0) {
        memcpy(newElements,theElements,top*sizeof(T));
      }
      from = top;
    }

    vectorSize = newSize;
    idxLastElement = newSize-1;

    if (ownData) {
      delete[] theElements;
    } else {
      ownData = true;
    }

    theElements = newElements;

    if (initNew) {
      fill(iniValue,from,lastIdx());
    }
  }

  // restore ownership
  template<class T>
  void genericVector<T>::restoreOwnership() {
    if (ownData) {
      return; // already my data!
    }

    int newSize(vectorSize);

    if (constReference) {
      throw constReferenceException();
    }

    T* newElements = new T[newSize];

    if (newElements == 0) { // check memory allocation
      throw allocException();
    }

    if (newSize>0) {
      memcpy(newElements,theElements,newSize*sizeof(T));
    }

    vectorSize = newSize;
    idxLastElement = newSize-1;

    ownData = true;

    theElements = newElements;
  }


  template<class T>
    void genericVector<T>::clear() {
    resize(0,T(),false,false);
  }

  template<class T>
    bool genericVector<T>::empty() const {
    return (size() == 0);
  }

  template<class T>
  void genericVector<T>::fill(const T& data,int from, int to) {
    int ready,wr,rest;

    // check bounds
    if (to >= size()) {
      to = lastIdx();
    }

    if (from < firstIdx()) {
      from = firstIdx();
    }

    if ((from >= size()) || (to < from)) { // nothing to do
      return;
    }

    T* ptrData;
    ptrData = &theElements[from]; // ptrData points to the first
    ready = 1;                    // element to be "filled"
    rest  = to-from;

    ptrData[0] = data;

    // fills the region with memcpy
    while (rest > 0) {
      wr = min(ready,rest);
      memcpy(&ptrData[ready],ptrData,wr*sizeof(T));
      ready += wr;
      rest  -= wr;
    }
  }

  template<class T>
  void genericVector<T>::fill(const T data[],int from, int to) {

    // check bounds
    if (to >= size()) {
      to = lastIdx();
    }

    if (from < firstIdx()) {
      from = firstIdx();
    }

    if ((from >= size()) || (to < from)) { // nothing to do
      return;
    }

    T* ptrData;
    ptrData = &theElements[from]; // ptrData points to the first
                                  // element to be "filled"

    memcpy(ptrData,data,(to-from+1)*sizeof(T));
  }


  template<class T>
  void genericVector<T>::fill(const genericVector<T>& vct,int from,
                              int to,
                              int startAt) {

    if (from < firstIdx()) {
      from = firstIdx();
    }
    if (to > lastIdx()) {
      to = lastIdx();
    }

    if ((from > to) || (startAt > vct.lastIdx())) {
      return;
    }

    int numElem = to-from+1;
    numElem = min(numElem,vct.lastIdx()-startAt+1);

    if (numElem>0) {
      memcpy(&at(from),&vct.at(startAt),numElem*sizeof(T));
    }
  }

  // copy member
  template<class T>
  genericVector<T>& genericVector<T>::copy(const genericVector& other) {

    if (&other == this) {
      return (*this); // copy of itself is ready!
    }

    if (constReference) {
      // reference is constant => only the contents will be copied
      fill(other);
    } else {
      if (ownData) {
        if (vectorSize != other.size()) {
          delete[] theElements;
          theElements = 0;
        }
      } else {
        ownData = true;
        theElements = 0;
      }

      vectorSize = other.size();
      idxLastElement = vectorSize-1;

      if (vectorSize > 0) {
        if (isNull(theElements)) {
          theElements = new T[vectorSize];

          if (isNull(theElements)) { // memory allocation check
              throw allocException();
          }
        }

        memcpy(theElements,&other.at(0),sizeof(T)*vectorSize);
      } else {
        theElements = 0;
        vectorSize = 0;
        idxLastElement = -1;
      }
    }

    return *this;
  }

  // copy member
  template<class T>
  genericVector<T>& genericVector<T>::copy(const genericVector<T>& other,
                             int from, int to) {

    int f=from;
    int t=to;

    // check bounds
    int osize=other.size();
    if (f<0) {
      f=0;
    }
    if (t>=osize) {
      t=osize-1;
    }

    if (&other == this) { // same genericVector!
      genericVector<T> tmpCopy(*this); //temporal copy of this
      copy(tmpCopy, from, to); //extract subgenericVector
      return (*this);  //return itself
    }

    //check for complete copy
    if (f==0 && t==osize-1) {
      return copy(other);
    }

    if (constReference) {
      if  (vectorSize != other.size()) {
        throw constReferenceException();
      }
    } else {
      if (ownData) {
        if (vectorSize != other.size()) {
          delete[] theElements;
          theElements = 0;
        }
      } else {
        ownData = true;
        theElements = 0;
      }
    }

    vectorSize = t-f+1;
    idxLastElement = vectorSize-1;

    if (vectorSize > 0) {
      if (isNull(theElements)) {
        theElements = new T[vectorSize];

        if (isNull(theElements)) { // memory allocation check
          throw allocException();
        }
      }

      memcpy(theElements,&other.at(f),sizeof(T)*vectorSize);

    } else {
      theElements = 0;
      vectorSize = 0;
      idxLastElement = -1;
    }

    return *this;

  }

  // copy member
  template<class T>
  genericVector<T>& genericVector<T>::copy(const genericVector& other,
                                           const genericVector<int>& idx) {

    if (&other == this) { // same genericVector!
      genericVector<T> tmpCopy(*this); //temporal copy of this
      copy(tmpCopy, idx); //extract subgenericVector
      return (*this);  //return itself
    }

    if (constReference) {
      if  (vectorSize != other.size()) {
        throw constReferenceException();
      }
    } else {
      if (ownData) {
        if (vectorSize != other.size()) {
          delete[] theElements;
          theElements = 0;
        }
      } else {
        ownData = true;
        theElements = 0;
      }
    }

    vectorSize = idx.size();
    idxLastElement = vectorSize-1;

    if (vectorSize > 0) {
      if (isNull(theElements)) {
        theElements = new T[vectorSize];

        if (isNull(theElements)) { // memory allocation check
          throw allocException();
        }
      }
      typename genericVector<T>::iterator it=begin();
      typename genericVector<T>::iterator eit=end();
      genericVector<int>::const_iterator idxIt=idx.begin();
      for (;it!=eit; ++it, ++idxIt) {
        (*it)=other.at(*idxIt);
        }

    } else {
      theElements = 0;
      vectorSize = 0;
      idxLastElement = -1;
    }

    return *this;
  }

  // creates a clone of this genericVector
  template<class T>
  mathObject* genericVector<T>::clone() const {
    genericVector<T>* tmp = new genericVector<T>(*this);
    return tmp;
  }

  /* compare this genericVector with other
     @param other the other genericVector to be compared with
     @return true if both genericVectors have the same elements and same size
  */
  template<class T>
  bool genericVector<T>::equals(const genericVector<T>& other) const {
    bool result;

    result = (size() == other.size());

    return (result &&
            (0 == memcmp(theElements,other.theElements,size()*sizeof(T))));

  }

  // applies a C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(T (*function)(T)) {
    iterator i,e;

    e = end();
    for (i=begin();i!=e;++i)
      *i=(*function)(*i);

    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                                            T (*function)(T)) {

    if (size() != other.size()) {
      resize(other.size(),T(),false,false);
    }

    const_iterator io,e;
    iterator i;

    e = other.end();
    for (i=begin(),io=other.begin();io!=e;++i,++io)
      *i=(*function)(*io);

    return (*this);
  };



  // applies a two-parameter C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                              T (*function)(const T&,const T&)) {

    assert (other.size() == size());

    iterator i1,e;
    const_iterator i2;

    e = end();
    for (i1=begin(),i2=other.begin();i1!=e;i1++,i2++) {
      *i1=(*function)(*i1,*i2);  // applies a C-Function to each element
    }

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,
                              T (*function)(T,T)) {

    assert (other.size() == size());

    iterator i1,e;
    const_iterator i2;

    e = end();
    for (i1=begin(),i2=other.begin();i1!=e;i1++,i2++) {
      *i1=(*function)(*i1,*i2);  // applies a C-Function to each element
    }

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& a,
                              const genericVector<T>& b,
                              T (*function)(const T&,const T&)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    const_iterator ia,ib,e;
    iterator it;

    e = a.end();
    for (ia=a.begin(),ib=b.begin(),it=begin();ia!=e;++ia,++ib,++it) {
      *it=(*function)(*ia,*ib);  // applies a C-Function to each element
    }

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& a,
                              const genericVector<T>& b,
                              T (*function)(T,T)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    const_iterator ia,ib,e;
    iterator it;

    e = a.end();
    for (ia=a.begin(),ib=b.begin(),it=begin();ia!=e;++ia,++ib,++it) {
      *it=(*function)(*ia,*ib);  // applies a C-Function to each element
    }

    return (*this);
  };


  // applies a C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(T (*function)(const T&)) {
    iterator i,e;

    e = end();
    for (i=begin();i!=e;++i)
      *i=(*function)(*i);  // applies a C-Function to each element


    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  genericVector<T>& genericVector<T>::apply(const genericVector<T>& other,T (*function)(const T&)) {
    if (size() != other.size()) {
      resize(other.size(),T(),false,false);
    }

    const_iterator io,e;
    iterator i;

    e = other.end();
    for (i=begin(),io=other.begin();io!=e;++i,++io)
      *i=(*function)(*io);

    return (*this);
  };

  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /**
   * write the object in the given ioHandler
   */
  template<class T>
  bool genericVector<T>::write(ioHandler& handler,const bool complete) const {

    int i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      handler.write("size",size());

      handler.writeBegin();             // Data block scope (1)
                                        //
      handler.writeSymbol("data");      //
      handler.writeKeyValueSeparator(); //
                                        //
      handler.writeBegin();             // Data             (2)
      if (size() > 0) {                 //
        for (i=0;i<lastIdx();++i) {     //
          lti::write(handler,at(i));    //
          handler.writeDataSeparator(); //
        }                               //
        lti::write(handler,at(i));      //
      }                                 //
      handler.writeEnd();               // Data             (1)
                                        //
      handler.writeEnd();               // Data block scope (0)
    }                                   //

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template<class T>
  bool genericVector<T>::read(ioHandler& handler,const bool complete) {


    int i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      handler.read("size",size);
      std::string str;

      handler.readBegin();                   // Data scope (1)
      handler.readSymbol(str);               //
                                             //
      if (str == "data") {                   //
        handler.readKeyValueSeparator();     //
        handler.readBegin();                 // Data       (2)
        resize(size,T(),false,false);        //
        if (size > 0) {                      //
          size--;                            //
          for (i=0;i<size;++i) {             // 
            lti::read(handler,at(i));        //
            handler.readKeyValueSeparator(); //
          }                                  //
          lti::read(handler,at(i));          //
        }                                    //
        handler.readEnd();                   // Data       (1)
      }                                      //
                                             //
      handler.readEnd();                     // Data scope (0)
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;

  }

} // namespace lti


#include "ltiTypes.h"

namespace std {
  // undocumented class required by the next operator in order to display
  // ubyte and byte as numbers and not as characters.
  template<class T>
  class numeric_type {
  public:
    typedef T value_type;
  };

  template<>
  class numeric_type<lti::ubyte> {
  public:
    typedef int value_type;
  };

  template<>
  class numeric_type<lti::byte> {
  public:
    typedef int value_type;
  };

  /// ASCII output of a genericVector to a std stream.
  template <class T>
  ostream& operator<<(ostream& s,const lti::genericVector<T>& v) {
    int i;

    s << "(";

    if (v.size()>0) {
      s << static_cast< typename numeric_type<T>::value_type>(v.at(0));

      for (i=1;i<v.size();++i) {
        s << " \t" << static_cast<typename numeric_type<T>::value_type>(v.at(i));
      }

      s << ")";
    } else {
      s << " )";
    }


    return s;
  }
}

#endif
#endif
