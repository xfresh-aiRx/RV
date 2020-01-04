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
 * file .......: ltiArray.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiArray_template.h,v 1.21 2005/11/24 14:21:32 doerfler Exp $
 */

#include "ltiMath.h" //for min() max()

namespace lti {
  // -------------------------------------------------------------------
  // array
  // -------------------------------------------------------------------
  // default constructor
  template<class T>
  array<T>::array()
    : vector<T>(),offset(0),
      firstArrayElement(0),lastArrayElement(-1),theElement0(0) {
  }

  template<class T>
  array<T>::array(int from,
                  int to,
                  const T& iniValue)
    : vector<T>(to-from+1,iniValue) {

    offset = -from; /* offset represent the absolute index of
                       the 0-th element.*/

    firstArrayElement = from;
    lastArrayElement  = this->size()-offset-1;
    theElement0=this->theElements + offset; // address of the element at 0
  }

  // copy constructor
  template<class T>
  array<T>::array(const vector<T>& other,int theOffset)
    : vector<T>(other),offset(theOffset) {
    firstArrayElement = -theOffset;
    lastArrayElement = this->size()-theOffset-1;
    theElement0=this->theElements + offset; // address of the element at 0
  }

  template<class T>
  array<T>::array(const array<T>& other) : vector<T>(other) {
    offset            = other.offset;
    firstArrayElement = other.firstArrayElement;
    lastArrayElement  = other.lastArrayElement;
    theElement0       = other.theElement0;
  }

  template<class T>
  array<T>::array(bool init, int from, int to)
    : vector<T>(init,to-from+1) {

    offset = -from; /* offset represent the absolute index of
                       the 0-th element.*/

    firstArrayElement = from;
    lastArrayElement  = this->size()-offset-1;
    theElement0=this->theElements + offset; // address of the element at 0
  }

  template<class T>
  array<T>::~array() {
    if (this->ownData) {
      delete[] this->theElements;
    }
    this->theElements = 0;
    this->vectorSize = 0;
    this->idxLastElement = -1;
    offset = 0;
    firstArrayElement = 0;
    lastArrayElement = -1;
    theElement0 = 0;
  }

  template<class T>
  const char* array<T>::getTypeName() const {
    return "array";
  }

  template<class T>
  void array<T>::setFirstIdx(int newIdx) {
    offset = -newIdx;
    firstArrayElement = newIdx;
    lastArrayElement = firstArrayElement+this->size()-1;
    theElement0 = this->theElements + offset;
  }

  template<class T>
  void array<T>::resize(int from,
                        int to,
                        const T& iniValue,
                        bool copyData,
                        bool initNew) {

    const int newSize = to-from+1;

    if (newSize <= 0) {
      // Wrong size! Assume this->size()==0
      if (this->ownData) {
        delete[] this->theElements;
      } else {
        this->ownData = true;
      }
      
      this->theElements = 0;
      this->vectorSize = 0;
      this->idxLastElement = -1;
      offset = 0;
      firstArrayElement = 0;
      lastArrayElement = -1;
      theElement0 = 0;
      
      return; // nothing to do!
    }

    const int oldFrom = firstIdx();
    const int oldTo   = lastIdx();
    int idxLow  = to+1;
    int idxHigh = to-2; // thus when no copying is done there is only one fill

    // if the size is unchanged
    if (newSize == this->vectorSize) {
      // no copying => fill (or not) and adjust attributes
      if (!copyData) {
        // if data is not copied but initialized fill the array and return
        if (initNew) {
          fill(iniValue,firstIdx(),lastIdx());
        }
        // if starting point is different set
        // indices and offset correctly
        if (from != oldFrom) {
          offset = -from;
          firstArrayElement = from;
          lastArrayElement = to;
          theElement0 = this->theElements + offset;
        }
        return;
      } else {  // copying required

        // if nothing is changed exit quickly
        if (from == oldFrom) {
          return;
        }

        if (oldTo >= oldFrom) {
          // use memmove for copying since areas might overlap
          idxLow  = max(oldFrom,from);
          idxHigh = min(oldTo,to);
          const int n = idxHigh - idxLow + 1;
          memmove(&this->theElements[idxLow-from],
                  &this->theElements[idxLow+offset],n*sizeof(T));
        }
      }
    } else {
      
      // allocate memory
      T* newElements = new T[newSize];
      
      if (newElements == 0) { // check memory allocation
        throw allocException();
      }

      if (copyData && (oldTo>=oldFrom)) { // old data should be copied
        // calculate the (0-based) indices of the actual memory
        // which must be copied...
        idxLow  = max(oldFrom,from);
        idxHigh = min(oldTo,to);
        const int n = idxHigh - idxLow + 1;
        memcpy(&newElements[idxLow-from],
               &this->theElements[idxLow+offset],n*sizeof(T));
      }
    
      if (this->ownData) {
        delete[] this->theElements;
      } else {
        this->ownData = true;
      }
    
      this->theElements = newElements;
      this->vectorSize = newSize;
      this->idxLastElement = newSize-1;
    
    }
    
    offset = -from;
    firstArrayElement = from;
    lastArrayElement = to;
    theElement0 = this->theElements + offset;

    if (initNew) {
      fill(iniValue,from,idxLow-1);
      fill(iniValue,idxHigh+1,to);
    }
  }

  template<class T>
  void array<T>::shift(int sh) {
    offset -= sh;
    firstArrayElement += sh;
    lastArrayElement += sh;
    theElement0 = this->theElements + offset;
  }
  

  template<class T>
  void array<T>::fill(const T& data,int from, int to) {
    int ready,wr,rest;

    // check bounds
    if (to > lastIdx()) {
      to = lastIdx();
    }

    if (from < firstIdx()) {
      from = firstIdx();
    }

    if ((from > lastIdx()) || (to < from)) { // nothing to do
      return;
    }

    T* ptrData;
    ptrData = &this->theElements[from+offset]; // ptrData points to the first
    ready = 1;                           // element to be "filled"
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
  array<T>& array<T>::copy(const vector<T>& other) {

    vector<T>::copy(other);

    offset = 0;
    firstArrayElement = 0;
    lastArrayElement  = this->size()-1;
    theElement0 = this->theElements;

    return *this;
  }

  template<class T>
    array<T>& array<T>::copy(const array<T>& other) {

    vector<T>::copy(other);

    offset = other.offset;
    firstArrayElement = other.firstArrayElement;
    lastArrayElement  = other.lastArrayElement;
    theElement0 = this->theElements + offset;

    return *this;
  }

  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<class T>
  bool array<T>::write(ioHandler& handler,const bool complete) const {

    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"offset",-firstIdx());
      vector<T>::write(handler,false);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }

  /*
   * read the object from the given ioHandler
   */
  template<class T>
  bool array<T>::read(ioHandler& handler,const bool complete) {
    int offset;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"offset",offset);
      vector<T>::read(handler,false);
      setFirstIdx(-offset);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

  // -----------------------------------------------------------------
  // Apply methods
  // -----------------------------------------------------------------


  // applies a C-Function to each element
  template<class T>
  array<T>& array<T>::apply(T (*function)(T)) {
    vector<T>::apply(*function);
    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  array<T>& array<T>::apply(T (*function)(const T&)) {
    vector<T>::apply(*function);
    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  array<T>& array<T>::apply(const array<T>& other,T (*function)(T)) {
    vector<T>::apply(other,*function);

    offset = other.offset;
    firstArrayElement = other.firstArrayElement;
    lastArrayElement  = other.lastArrayElement;
    theElement0 = this->theElements + offset;

    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  array<T>& array<T>::apply(const array<T>& other,T (*function)(const T&)) {
    vector<T>::apply(other,*function);

    offset = other.offset;
    firstArrayElement = other.firstArrayElement;
    lastArrayElement  = other.lastArrayElement;
    theElement0 = this->theElements + offset;

    return (*this);
  };


  // applies a two-parameter C-Function to each element
  template<class T>
  array<T>& array<T>::apply(const array<T>& other,
                            T (*function)(const T&,const T&)) {

    assert (other.size() == this->size());
    assert (firstArrayElement == other.firstArrayElement);

    vector<T>::apply(other,*function);

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  array<T>& array<T>::apply(const array<T>& other,
                            T (*function)(T,T)) {

    assert (other.size() == this->size());
    assert (firstArrayElement == other.firstArrayElement);

    vector<T>::apply(other,*function);

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  array<T>& array<T>::apply(const array<T>& a,
                            const array<T>& b,
                            T (*function)(const T&,const T&)) {

    assert (a.size() == b.size());
    assert (a.firstArrayElement == b.firstArrayElement);
    
    vector<T>::apply(a,b,*function);

    offset = a.offset;
    firstArrayElement = a.firstArrayElement;
    lastArrayElement  = a.lastArrayElement;
    theElement0 = this->theElements + offset;

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  array<T>& array<T>::apply(const array<T>& a,
                            const array<T>& b,
                            T (*function)(T,T)) {

    assert (a.size() == b.size());
    assert (a.firstArrayElement == b.firstArrayElement);
    
    vector<T>::apply(a,b,*function);

    offset = a.offset;
    firstArrayElement = a.firstArrayElement;
    lastArrayElement  = a.lastArrayElement;
    theElement0 = this->theElements + offset;

    return (*this);
  };
}

