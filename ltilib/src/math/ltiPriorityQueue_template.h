/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiPriorityQueue_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.07.2003
 * revisions ..: $Id: ltiPriorityQueue_template.h,v 1.12 2005/06/16 12:37:35 doerfler Exp $
 */

#include "ltiSort.h"

namespace lti {
  

  template<class T,class U>
  priorityQueue<T,U>::priorityQueue(const T& inv) 
    : mathObject(),invalid(inv) {
    clear();
  }

  template<class T,class U>
  priorityQueue<T,U>::priorityQueue(const priorityQueue<T,U>& other) 
    : mathObject() {
    copy(other);
  }

  template<class T,class U>
  priorityQueue<T,U>::~priorityQueue() {
    clear();
  }

  template<class T,class U>
  const char* priorityQueue<T,U>::getTypeName() const {
    return "priorityQueue<T,U>";
  }

  template<class T,class U>
  mathObject* priorityQueue<T,U>::clone() const {
    return new priorityQueue<T,U>(*this);
  }

  template<class T,class U>
  priorityQueue<T,U>& 
  priorityQueue<T,U>::copy(const priorityQueue<T,U>& other) {
    invalid     = other.invalid;
    data        = other.data;
    numElements = other.numElements;
    stackPtr    = other.stackPtr;
    numRemoved  = other.numRemoved;
    idToIndex   = other.idToIndex;
    indexToId   = other.indexToId;
    return *this;
  }

  template<class T,class U>
  priorityQueue<T,U>& 
  priorityQueue<T,U>::operator=(const priorityQueue<T,U>& other) {
    return copy(other);
  }

  template<class T,class U>
  void priorityQueue<T,U>::clear() {
    data.clear();
    numElements=0;
    stackPtr=0;
    numRemoved = 0;
    idToIndex.clear();
  }

  template<class T,class U>
  void priorityQueue<T,U>::pop() {
    assert(numElements>0);

    _lti_debug3("PQ: pop(), still left " << numElements << " -> ");
    erase(indexToId[stackPtr]);
  }
  
  template<class T,class U>
  bool priorityQueue<T,U>::empty() const {
    return (numElements == 0);
  }

  template<class T,class U>
  const std::pair<T,U>& priorityQueue<T,U>::front() const {
    return data[stackPtr];
  }

  template<class T,class U>
  void priorityQueue<T,U>::getData(std::vector< std::pair<T,U> >& d) const {
    d.clear();
    typename std::vector< std::pair<T,U> >::const_iterator it;
    for (it=data.begin()+stackPtr;it!=data.end();++it) {
      if ((*it).first != invalid) {
        d.push_back(*it);
      }
    }
    
  }

  template<class T,class U>
  bool priorityQueue<T,U>::valid(const index_type id) const {
    return ((static_cast<unsigned int>(id) < idToIndex.size()) &&
            (data[idToIndex[id]].first != invalid));
  }

  template<class T,class U>
  const T& priorityQueue<T,U>::getKey(const index_type id) const {
    if (static_cast<unsigned int>(id) < idToIndex.size()) {
      return data[idToIndex[id]].first;
    } else {
      return invalid;
    }
  }

  template<class T,class U>
  const U& priorityQueue<T,U>::getData(const index_type id) const {
    if (static_cast<unsigned int>(id) < idToIndex.size()) {
      return data[idToIndex[id]].second;
    } else {
      static const U dummyData = U();
      return dummyData;
    }
  }

  template<class T,class U>
  bool priorityQueue<T,U>::setData(const index_type id,
                                   const U& newData) {
    if (static_cast<unsigned int>(id) < idToIndex.size()) {
      data[idToIndex[id]].second = newData;
      return true;
    }
    return false;
  }

  /* --------------
   * Insert method
   * --------------
   */
  template<class T,class U>
  typename priorityQueue<T,U>::index_type
  priorityQueue<T,U>::insert(const T& key,const U& d) {

    _lti_debug4("PQ::insert key " << key << ", data " << d);
    
    int i(stackPtr),lastFree(i-1),j;
    int id,nid;
    int pre(i); // number of "free" spaces before i

    // compute position where the new element should be inserted
    while ((i<static_cast<int>(data.size())) && ((data[i].first < key) ||
                                                 (data[i].first == invalid))) {
      if (data[i].first == invalid) {
        lastFree=i;
        ++pre;
      }
      ++i;
    }

    _lti_debug4(" at idx " << i);

    // new element should be at exactly position i

    // if pre>0, we can avoid the resizing of the whole vectors and just
    // shift the elements before i, updating of course the equivalence vectors

    if (pre>0) {
      _lti_debug4(" shift to left." << std::endl);
      _lti_debug4("    stackPtr: " << stackPtr << std::endl);
      _lti_debug4("    lastFree: " << lastFree << std::endl);
      // just shift to the left

      // reuse an old label
      nid = indexToId[lastFree];

      j=lastFree;
      
      if (!(data[i].first < key)) {
        i--;
      }
      
      const int last = min(i,static_cast<int>(data.size())-1);
      while(j<last) {
        data[j]=data[j+1];
        id = ( indexToId[j] = indexToId[j+1] );
        idToIndex[id]--;
        ++j;
      }
      
      idToIndex[nid]=last;
      indexToId[last]=nid;
      data[last]=std::make_pair(key,d);
      numRemoved--; // one removed element has been reused
      
      if (lastFree < stackPtr) {
        stackPtr--;
      }
    
    } else {
      _lti_debug4(" shift to right." << std::endl);

      // pre == 0 here!
      // shift to the right.  This maybe requires increasing the vector size!
      // numRemoved - pre should be the number of "free" spaces on the right
      // side
      if (numRemoved <= 0) {
        if (i==static_cast<int>(data.size())) {
          // element must be inserted at the end.
          data.push_back(std::make_pair(key,d));
          // new id will be the new index in the data vector!
          id = static_cast<int>(data.size())-1;
          indexToId.push_back(id);
          idToIndex.push_back(id);
          numElements++;

#       if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
          checkConsistency();
#       endif    

          return id;
        }

        // it is required to resize all vectors
        id = indexToId[data.size()-1];
        indexToId.push_back(id);        
        idToIndex[id]++;

        data.push_back(data[data.size()-1]);
        j=static_cast<int>(data.size())-2;

        // now shift elements
        while (j>i) {
          data[j]=data[j-1];
          id = indexToId[j] = indexToId[j-1];
          idToIndex[id]++;
          j--;
        };

        // now we can insert the new elements
        idToIndex.push_back(i);
        nid=indexToId[i]=static_cast<int>(idToIndex.size())-1;
        data[i]=std::make_pair(key,d);
      } else {
        // there should be a free element on the right side.  So we just need
        // to shift until this element is reached!
        
        // find the removed element
        j=i;
        while ((j<static_cast<int>(data.size())) &&
               (data[j].first != invalid)) {
          ++j;
        }

        if (j==static_cast<int>(data.size())) {
          // hmmm, we shouldn't get here at all! but we just insert the
          // element at the end.

          _lti_debug4("WARNING!! We should not get here at all!\n");

          data.push_back(std::make_pair(key,d));
          // new id will be the new index in the data vector!
          id = static_cast<int>(data.size())-1;
          indexToId.push_back(id);
          idToIndex.push_back(id);
          numElements++;

#       if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
          checkConsistency();
#       endif    

          return id;
        }

        // reuse the freed id:
        nid = indexToId[j];

        while (j>i) {
          // shift the elements
          data[j]=data[j-1];
          id = indexToId[j] = indexToId[j-1];
          idToIndex[id]++;
          j--;
        }

        // now we can insert the new elements
        idToIndex[nid]=i;
        indexToId[i]=nid;
        data[i]=std::make_pair(key,d);
        numRemoved--; // one removed element has been reused!    
      }
    }

    numElements++;

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
    checkConsistency();
#endif    

    return nid;
  }
  
  template<class T,class U>
  void priorityQueue<T,U>::erase(const index_type id) {

    if (id<0) {
      return;
    }

    int i = idToIndex[id];

    _lti_debug4("PQ::erase id " << id << ", idx " << i 
                << ", key=" << data[i].first
                << ", data=" << data[i].second << std::endl);

    if (i==stackPtr) { // deleting top of the queue requires updating the stack
                       // pointer

      // make sure the stackPtr points to a valid element
      stackPtr++;
      while ((stackPtr < static_cast<int>(data.size())) && 
             (data[stackPtr].first == invalid)) {
        ++stackPtr;
      }
      
      if (stackPtr == static_cast<int>(data.size())) {
        // oh! it seems the whole thing is empty! so we clear it completelly
        clear();
        return;
      }
    }

    if (data[i].first != invalid) {
      data[i].first = invalid;
      numElements--;
      numRemoved++;
    }

  }

  template<class T,class U>
  bool priorityQueue<T,U>::checkConsistency() const {

    bool res = true;
    if ((numElements+numRemoved) != static_cast<int>(data.size())) {
      res = false;
    }

    if (stackPtr > numRemoved) {
      res = false;
    }

    // check idToIndex indexToId consistency:
    int i,id;
    for (i=0;i<static_cast<int>(indexToId.size());++i) {
      id = indexToId[i];
      if (idToIndex[id] != i) {
        res = false;
      }
    }

    // check queue consistency (data must be sorted!)
    T last=data[stackPtr].first;
    for (i=stackPtr;i<static_cast<int>(data.size());++i) {
      if (data[i].first != invalid) {
        if (data[i].first<last) {
          _lti_debug2("Priority queue inconsistency at idx " << i << 
                      " last value: " << last <<
                      " this value: " << data[i].first << 
                      " stackPtr: " << stackPtr << std::endl);
          last = data[i].first;
          res=false;
        }
      }
    }

    return res;
  }

  template<class T,class U>
  void priorityQueue<T,U>::update(const index_type id,
                                  const T& newKey) {

    int idx = idToIndex[id];
    int j,i(idx);

    _lti_debug4("PQ::update id " << id << ", new key " << newKey << 
                " (old key " << data[idx].first << ")" << 
                " idx: " << idx << " -> ");

    assert(data[idx].first != invalid);

    if (data[idx].first < newKey) { // new key is greater than old one
      // try to shift to the right
      std::pair<T,U> elem(newKey,data[i].second); // remember the old data

      const int last = static_cast<int>(data.size())-1;
      while ((i<last) && ((data[i+1].first == invalid) || 
                          (data[i+1].first < newKey))) {
        data[i]=data[i+1];
        j=indexToId[i]=indexToId[i+1];
        idToIndex[j]--;
        ++i;
      } 

      data[i]=elem;
      idToIndex[id]=i;
      indexToId[i]=id;

      // the stack pointer may require some updating
      while (data[stackPtr].first == invalid) {
        stackPtr++;
      }

    } else if (newKey < data[idx].first) {
      // try to shift to the left
      std::pair<T,U> elem(newKey,data[i].second); // remember the old data

      while ((i>stackPtr) && ((data[i-1].first == invalid) || 
                              (newKey<data[i-1].first))) {

        data[i]=data[i-1];
        j=indexToId[i]=indexToId[i-1];
        idToIndex[j]++;        
        --i;
      }

      data[i]=elem;
      idToIndex[id]=i;
      indexToId[i]=id;
    }

    _lti_debug4(i << std::endl);

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
    checkConsistency();
#endif
  }

  template<class T,class U>
  void priorityQueue<T,U>::update(const index_type id,
                                  const T& newKey,
                                  const U& dat) {

    update(id,newKey);
    data[idToIndex[id]].second = dat;
  }

  template<class T,class U>
  bool priorityQueue<T,U>::write(ioHandler& handler,
                                 const bool complete) const {

    std::vector< std::pair<T,U> > d;
    getData(d);

    return lti::write(handler,d,complete);
  }

  template<class T,class U>
  bool priorityQueue<T,U>::read(ioHandler& handler,
                                const bool complete) {
    std::vector< std::pair<T,U> > d;
    if (lti::read(handler,d,complete)) {
      create(d);
      return true;
    }
    return false;
  }


  template<class T,class U>
  bool priorityQueue<T,U>::writeAll(ioHandler& handler,
                                    const bool complete) const {

    if (complete) {
      handler.writeBegin();
    }

    lti::write(handler,"invalid",invalid);
    lti::write(handler,"data",data);
    lti::write(handler,"idToIndex",idToIndex);
    lti::write(handler,"indexToId",indexToId);
    lti::write(handler,"numElements",numElements);
    lti::write(handler,"numRemoved",numRemoved);
    lti::write(handler,"stackPtr",stackPtr);

    if (complete) {
      handler.writeEnd();
    }

    return true;
  }

  template<class T,class U>
  bool priorityQueue<T,U>::readAll(ioHandler& handler,
                                   const bool complete) {

    if (complete) {
      handler.readBegin();
    }

    lti::read(handler,"invalid",invalid);
    lti::read(handler,"data",data);
    lti::read(handler,"idToIndex",idToIndex);
    lti::read(handler,"indexToId",indexToId);
    lti::read(handler,"numElements",numElements);
    lti::read(handler,"numRemoved",numRemoved);
    lti::read(handler,"stackPtr",stackPtr);

    if (complete) {
      handler.readEnd();
    }

    return true;
  }

  template<class T,class U>
  void priorityQueue<T,U>::create(const std::vector< std::pair<T,U> >& da) {
    
    // first than all, clear all old data
    clear();
    
    vector<T> lkey;
    vector<index_type> index;

    lkey.resize(static_cast<int>(data.size()),T(),false,false);
    index.resize(static_cast<int>(lkey.size()),0,false,false);
    index_type i;
    for (i=0;i<lkey.size();++i) {
      lkey.at(i)=data[i].first;
      index.at(i)=i;
    }
  
    sort2<T,int> sorter;
    sorter.apply(lkey,index);

    numElements = lkey.size();
    numRemoved = 0;
    stackPtr = 0;

    idToIndex.resize(lkey.size());
    indexToId.resize(lkey.size());
    data.resize(lkey.size());

    for (i=0;i<lkey.size();++i) {
      data[i].first          = lkey.at(i);
      data[i].second         = da[index.at(i)].second;
      indexToId[i]           = index.at(i);
      idToIndex[index.at(i)] = i;
    }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 4)
    _lti_debug4("Creation consistency ");
    if (checkConsistency()) {
      _lti_debug4("ok\n");
    } else {
      _lti_debug4("failed\n");
    }
#endif    
  }

  template<class T,class U>
  void priorityQueue<T,U>::create(const std::vector<T>& key,
                                  const std::vector<U>& da) {

    assert(key.size() == da.size());

    _lti_debug("PQC0\n");

    // first than all, clear all old data
    clear();
    
    _lti_debug("PQC1\n");

    vector<T> lkey;
    vector<int> index;
    lkey.resize(static_cast<int>(key.size()),T(),false,false); 
    index.resize(lkey.size(),0,false,false);
    int i;
    for (i=0;i<index.size();++i) {
      lkey.at(i)=key[i];
      index.at(i)=i;
    }

    _lti_debug("PQC2\n");

    sort2<T,int> sorter;
    sorter.apply(lkey,index);

    _lti_debug("PQC3\n");

    numElements = lkey.size();
    numRemoved = 0;
    stackPtr = 0;

    idToIndex.resize(lkey.size());
    indexToId.resize(lkey.size());
    data.resize(lkey.size());

    _lti_debug("PQC4\n");

    for (i=0;i<lkey.size();++i) {
      data[i].first          = lkey.at(i);
      data[i].second         = da[index.at(i)];
      indexToId[i]           = index.at(i);
      idToIndex[index.at(i)] = i;
    }

#if defined(_LTI_DEBUG) && (_LTI_DEBUG >= 2)
    _lti_debug("Creation consistency ");
    if (checkConsistency()) {
      _lti_debug("ok\n");
    } else {
      _lti_debug("failed\n");
    }
#endif    
    
  }

}
