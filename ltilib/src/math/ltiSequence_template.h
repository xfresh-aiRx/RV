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
 * file .......: ltiSequence.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.06.00
 * revisions ..: $Id: ltiSequence_template.h,v 1.9 2005/08/08 17:20:43 libuda Exp $
 */

// The global lti::write methods for
// STL types 'vector', 'list' and 'map'
#include "ltiSTLIoInterface.h"

namespace lti {
  // implementation of template functions

  // -------------------------------------------------------------------
  // sequence
  // -------------------------------------------------------------------

  // returns the number of elements of the sequence
  template<class T>
  int sequence<T>::size() const {
    return static_cast<int>(theSequence.size());
  };

  // returns first index (normally 0)
  template<class T>
  int sequence<T>::firstIdx() const {
    return 0;
  };

  /* returns first element as a const_iterator.
     Note that you can not change the values of the sequence
     elements when you use a const_iterator. See also begin() */
  template<class T>
  typename sequence<T>::const_iterator sequence<T>::begin() const {
    return theSequence.begin();
  };

  /* returns first element as an iterator */
  template<class T>
  typename sequence<T>::iterator sequence<T>::begin() {
    return theSequence.begin();
  };

  // returns last index (in a sequence this is always size()-1)
  template<class T>
  int sequence<T>::lastIdx() const {
    return size()-1;
  };

  /* returns last index as a const iterator.
      For an example see begin() */
  template<class T>
  typename sequence<T>::const_iterator sequence<T>::end() const {
    return theSequence.end();
  };

  /* returns last index as an iterator
      For an example see begin() */
  template<class T>
  typename sequence<T>::iterator sequence<T>::end() {
    return theSequence.end();
  };

  template<class T>
  T& sequence<T>::at(const int& x) {
    assert ( x < size());
    return ( theSequence[x] );
  }

  template<class T>
  const T& sequence<T>::at(const int& x) const {
    assert ( x < size());
    return ( theSequence[x] );
  }

  // access operator (alias for at(const int& x)).
  template<class T>
  T& sequence<T>::operator[](const int& x) {
    assert ( x < size());
    return ( theSequence[x] );
  };

  // const access operator (alias for at(const int& x) const).
  template<class T>
  const T& sequence<T>::operator[](const int& x) const {
    assert ( x < size());
    return ( theSequence[x] );
  };

  /* compare this sequence with other
     @param other the other sequence to be compared with
     @return true if both sequences have the same elements and same size
  */
  template<class T>
  bool sequence<T>::operator==(const sequence<T>& other) const {
    return equals(other);
  }

  // default constructor
  template<class T>
  sequence<T>::sequence() : ioObject()  {
  }

  template<class T>
  sequence<T>::sequence(const int& theSize) : ioObject() {

    std::vector<T> vct(theSize);
    theSequence=vct;

  }

  // copy constructor
  template<class T>
  sequence<T>::sequence(const sequence& other) : ioObject() {

    theSequence=other.theSequence;
  }

  // destructor
  template<class T>
  sequence<T>::~sequence() {
    theSequence.clear();
  }

  // append
  template<class T>
  void sequence<T>::append(const T& theElement){

    theSequence.push_back(theElement);
  }

  // resize
  template<class T>
  void sequence<T>::resize(const int& newSize,
                           const T& iniValue,
                           const bool& copyData,
                           const bool& initNew) {

    if (newSize == size()) {
      if (initNew && !copyData)
          fill(iniValue,0,lastIdx());
      return;
    }

    if (newSize <= 0) {
      theSequence.clear();
      return; // ready!!!
    }

    int i,from;

    from = size();

    if (newSize > size()) {
      theSequence.reserve(newSize);
      for (i=size();i<newSize;i++) {
        theSequence.push_back(iniValue);
      }
    } else {
      for (i=newSize;i<from;i++) {
        theSequence.pop_back();
      }
    }

    if (!copyData) {
      from = 0;
    }

    if (initNew)
      fill(iniValue,from,lastIdx());
  }

  // fill with a constant value
  template<class T>
  void sequence<T>::fill(const T& data,const int& f, const int& t) {
    int from,to,i;
    from  = f;
    to    = t;

    // check bounds
    if (to >= size())
      to = lastIdx();

    if (from < firstIdx())
      from = firstIdx();

    if ((from >= size()) || (to < from)) // nothing to do
      return;

    for (i=from;i<=to;i++) {
      theSequence[i]=data;
    }
  }

  // fill with another sequence
  template<class T>
  void sequence<T>::fill(const sequence<T>& seq,const int& from,
           const int& to,
           const int& startAt) {
    int f,t,numElem,i,j;

    f = from;
    t = to;

    if (f < firstIdx())
      f = firstIdx();
    if (t > lastIdx())
      t = lastIdx();

    if ((f > t) || (startAt > seq.lastIdx()))
      return;

    numElem = t-f+1;
    numElem = min(numElem,seq.lastIdx()-startAt+1);

    for (i=f,j=startAt;numElem>0;numElem--,i++,j++) {
      theSequence[i]=seq.at(j);
    }
  }

  // copy member
  template<class T>
  sequence<T>& sequence<T>::copy(const sequence& other) {
    theSequence = other.theSequence;
    return *this;
  }

  // creates a clone of this sequence
  template<class T>
  object* sequence<T>::clone() const {
    sequence<T>* tmp = new sequence<T>(*this);
    return tmp;
  }

  /* compare this sequence with other
     @param other the other sequence to be compared with
     @return true if both sequences have the same elements and same size
  */
  template<class T>
  bool sequence<T>::equals(const sequence<T>& other) const {

    bool result;

    result = (size() == other.size());
    int i;

    for (i = 0; i < size(); i++) {
      result = result && (theSequence[i]==other.theSequence[i]);
    }

    return result;
  }

  // multiply sequence with a constant (alias).
  template<class T>
  sequence<T>& sequence<T>::concatenate(const sequence<T>& other) {
    theSequence.reserve(size()+other.size());
    const_iterator it,e;

    for (it=other.begin(),e=other.end();it!=e;it++) {
      theSequence.push_back(*it);
    }

    return *this;
  }

  // read the sequence the given ioHandler. The complete flag indicates
  // whether the enclosing begin and end should also be read
  template<class T>
  bool sequence<T>::read(ioHandler& handler,const bool complete) {

    return lti::read(handler,theSequence,complete);

  };


  // write the sequence the given ioHandler. The complete flag indicates
  // if the enclosing begin and end should be also be written or not
  template<class T>
  bool sequence<T>::write(ioHandler& handler,const bool complete) const {

    return lti::write(handler,theSequence,complete);

  };

} // namespace lti

/*
// ASCII output of a vector to a std stream.
template <class T>
std::ostream& operator<<(std::ostream& s,const lti::sequence<T>& v) {
  int i;

  for (i=v.firstIdx();i<v.size();i++) {
    s << "--- [" << i << "] ---:" << std::endl;
    s << v.at(i) << std::endl;
  }

  return s;
}
*/

