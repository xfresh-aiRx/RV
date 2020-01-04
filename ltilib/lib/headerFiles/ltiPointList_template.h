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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiPointList_template.h
 * authors ....: Rüdiger Weiler, Gustavo Quiros, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 26.4.2001
 * revisions ..: $Id: ltiPointList_template.h,v 1.17 2005/06/16 12:35:43 doerfler Exp $
 */

#include "ltiMath.h"

namespace lti {
  // implementation of functions

  // -------------------------------------------------------------------
  // pointList
  // -------------------------------------------------------------------

  // default constructor
  template<class T>
  tpointList<T>::tpointList() : ioObject() {
  }

  // copy constructor
  template<class T>
  tpointList<T>::tpointList(const tpointList<T>& other) : ioObject() {
    copy(other);
  }

  /*
   * create this pointList as a copy of a list_type of tpoint<T>
   * @param other the pointList to be copied.
   */
  template<class T>
  tpointList<T>::tpointList(const list_type& other) {
    castFrom(other);
  }

  // destructor
  template<class T>
  tpointList<T>::~tpointList(){
      thePointList.clear();
  }


  // returns the number of elements of the pointList
  template<class T>
  int tpointList<T>::size() const {
    return thePointList.size();
  }


  // compares the size of this list with the size of the other point list
  // and returns true if this list has fewer points than the other one.
  template<class T>
  bool tpointList<T>::operator<(const tpointList<T>& other) const {
    return size()<other.size();
  }

  // compares the size of this list with the size of the other point list
  // and returns true if this list has more points than the other one.
  template<class T>
  bool tpointList<T>::operator>(const tpointList<T>& other) const {
    return size()>other.size();
  }

  // returns first element as a const_iterator.
  // Note that you can not change the values of the pointList
  // elements when you use a const_iterator. See also begin()
  template<class T>
  typename tpointList<T>::const_iterator tpointList<T>::begin() const {
    return thePointList.begin();
  }


  // returns first element as an iterator
  template<class T>
  typename tpointList<T>::iterator tpointList<T>::begin() {
    return thePointList.begin();
  }

  // returns last element as a const iterator.
  // For an example see begin()
  template<class T>
  typename tpointList<T>::const_iterator tpointList<T>::end() const {
    return thePointList.end();
  }

  // returns last element as an iterator
  // For an example see begin()
  template<class T>
  typename tpointList<T>::iterator tpointList<T>::end() {
    return thePointList.end();
  }

  // deletes all points from list and leaves empty pointList.
  template<class T>
  void tpointList<T>::clear(){
    thePointList.clear();
  }

  // erases point, which is denoted by it. Returns iterator to next element.
  template<class T>
  typename tpointList<T>::iterator
  tpointList<T>::erase(const typename tpointList<T>::iterator& it){
    return thePointList.erase(it);
  }

  // erases points between first and last. Returns iterator to next element.
  template<class T>
  typename tpointList<T>::iterator
  tpointList<T>::erase(const typename tpointList<T>::iterator& first,
                       const typename tpointList<T>::iterator& last){
    return thePointList.erase(first,last);
  }

  // inserts points at position denoted by it.
  // Returns iterator to inserted element.
  template<class T>
  typename tpointList<T>::iterator
  tpointList<T>::insert(const typename tpointList<T>::iterator& it,
                        const tpoint<T>& thePoint){
    return thePointList.insert(it,thePoint);
  }

  // inserts points at position denoted by it.
  // Returns iterator to inserted element.
  template<class T>
  typename tpointList<T>::iterator
  tpointList<T>::insert(const typename tpointList<T>::iterator& it,
                        const int& n,
                        const tpoint<T>& thePoint) {
    typename tpointList<T>::iterator itTemp=it;
    itTemp--;
    thePointList.insert(it,n,thePoint);
    itTemp++;
    return itTemp;
  }

  // inserts the elements between first and last at position denoted by it.
  template<class T>
  typename tpointList<T>::iterator
  tpointList<T>::insert(const typename tpointList<T>::iterator& it,
                        typename tpointList<T>::const_iterator first,
                        typename tpointList<T>::const_iterator last) {
    typename tpointList<T>::iterator itTemp=it;
    itTemp--;
    thePointList.insert(it,first,last);
    itTemp++;
    return itTemp;
  }

  // inserts element at begin of pointList
  template<class T>
  void tpointList<T>::push_front(const tpoint<T>& thePoint){
    thePointList.push_front(thePoint);
  }

  // removes element at begin of pointList
  template<class T>
  void tpointList<T>::pop_front(){
    thePointList.pop_front();
  }

  // inserts element at end of pointList
  template<class T>
  void tpointList<T>::push_back(const tpoint<T>& thePoint){
    thePointList.push_back(thePoint);
  }

  // removes element at end of pointList
  template<class T>
  void tpointList<T>::pop_back(){
    thePointList.pop_back();
  }

  // returns a reference to the first element
  template<class T>
  typename tpointList<T>::reference tpointList<T>::front() {
    return thePointList.front();
  }

  // returns a const reference to the first element
  template<class T>
  typename tpointList<T>::const_reference tpointList<T>::front() const {
    return thePointList.front();
  }

  // returns a reference to the last element
  template<class T>
  typename tpointList<T>::reference tpointList<T>::back() {
    return thePointList.back();
  }

  // returns a const reference to the last element
  template<class T>
  typename tpointList<T>::const_reference tpointList<T>::back() const {
    return thePointList.back();
  }

  // assigment operator.
  // copy the contents of <code>other</code> in this %object.
  // @param other the source pointList to be copied.
  template<class T>
  tpointList<T>& tpointList<T>::copy(const tpointList<T>& other){
    thePointList = other.thePointList;
    boundary.copy(other.getBoundary());
    return *this;
  }


  // assigment operator.
  // copy the contents of <code>other</code> in this %object.
  // @param other the source pointList to be copied.
  template<class T>
  tpointList<T>& tpointList<T>::operator=(const tpointList<T>& other){
    return copy(other);
  }

  // assigment operator.
  // copy the contents of <code>other</code> in this %object.
  // @param other the source pointList to be copied.
  template<class T>
  tpointList<T>& tpointList<T>::castFrom(const list_type& other){
    thePointList = other;
    return *this;
  }

  // compare this pointList with other
  // @param other the other pointList to be compared with
  // @return true if both pointLists have the same elements and same size
  template<class T>
  bool tpointList<T>::operator==(const tpointList<T>& other) const {
    return equals(other);
  }

  // return true if the list is empty
  template<class T>
  bool tpointList<T>::empty() const {
    return thePointList.empty();
  }

  // creates a clone of this pointList
  template<class T>
  object* tpointList<T>::clone() const {
    tpointList<T>* tmp = new tpointList<T>(*this);
    return tmp;
  }

  // compare this pointList with other
  // @param other the other pointList to be compared with
  // @return true if both pointList have the same elements and same size
  template<class T>
  bool tpointList<T>::equals(const tpointList<T>& other) const {
    bool result;
    if ((size() == other.size()) && (boundary == other.boundary)) {
      typename tpointList<T>::const_iterator it = thePointList.begin();
      typename tpointList<T>::const_iterator itOther = 
        other.thePointList.begin();
      typename tpointList<T>::const_iterator itEnd = thePointList.end();
      result = true;
      while ( (it!=itEnd) && (result == true) ) {
        result = result && ((*it)==(*itOther));
        it++;
        itOther++;
      }
      return result;
    }
    else {
      return false;
    }
  return true;
  }

  template<class T>
  const trectangle<T> tpointList<T>::computeBoundary() const {

    trectangle<T> rect;

    if (empty()) {
      rect=trectangle<T>(0,0,0,0);
      return rect;
    }

    typename tpointList<T>::const_iterator it;

    it = begin();
    rect.ul = *it;
    rect.br = *it;

    for (++it;it!=end();++it) {
      rect.ul.x = min(rect.ul.x,(*it).x);
      rect.br.x = max(rect.br.x,(*it).x);
      rect.ul.y = min(rect.ul.y,(*it).y);
      rect.br.y = max(rect.br.y,(*it).y);
    }

    return rect;
  }

  template<class T>
  const trectangle<T>& tpointList<T>::updateBoundary() {
    boundary = computeBoundary();
    return boundary;
  }

  template<class T>
  void tpointList<T>::splice(const iterator& pos,
                             tpointList<T>& other) {
    // This has to be done
    thePointList.splice(pos,other.thePointList);
    
    // FIXME: (when smallPointList becomes its splice
    // this is not what splice should do (this invalids all iterators), but
    // 
    //insert(pos,other.begin(),other.end());
    //other.clear();
  }

  // read the vector from the given ioHandler.  The complete flag indicates
  // whether the enclosing begin and end should also be read
  template<class T>
  bool tpointList<T>::read(ioHandler& handler,const bool complete) {

    int i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      handler.read("size",size);
      std::string str;

      int level = handler.getLevel();
      tpoint<T> aPoint;

      handler.readBegin();
      handler.readSymbol(str);

      if (str == "data") {
        handler.readKeyValueSeparator();
        handler.readBegin();
        clear();

        if (size > 0) {
          size--; // read all but the last one
          for (i=0;i<size;++i) {
            lti::read(handler,aPoint);
            push_back(aPoint);
            handler.readKeyValueSeparator();
          }
          lti::read(handler,aPoint);
          push_back(aPoint);
        }
      }

      while (handler.readEnd() && (handler.getLevel() > level));

    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  };

  // write the vector in the given ioHandler.  The complete flag indicates
  // if the enclosing begin and end should be also be written or not
  template<class T>
  bool tpointList<T>::write(ioHandler& handler,const bool complete) const {

    int i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      handler.write("size",size());

      handler.writeBegin();
      handler.writeSymbol(std::string("data"));
      handler.writeKeyValueSeparator();
      handler.writeBegin();
      if (size() > 0) {
        const_iterator it;
        const int last = size()-1;
        it = begin();
        for (i=0;i<last;++i,++it) {
          lti::write(handler,*it);
          handler.writeDataSeparator();
        }
        lti::write(handler,*it);
      }
      handler.writeEnd();
      handler.writeEnd();
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  };

} // namespace lti

/*
// ASCII output of a pointList to a std stream.
  template<class T>
  std::ostream& operator<<(std::ostream& s,const lti::tpointList<T>& v) {

     lti::tpointList<T>::const_iterator it;

     for (it=v.begin();it!=v.end();it++) {
       s << "(" << (*it).x << "," << (*it).y << ") ";
     }

    return s;
  }
*/

