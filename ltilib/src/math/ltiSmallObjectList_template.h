/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiSmallObjectList_template.h
 * authors ....: Gustavo Quiros
 * organization: LTI, RWTH Aachen
 * creation ...: 12.12.03
 * revisions ..: $Id: 
 */

namespace lti {

  /*
   * Erases the element at position pos, and returns an iterator pointing
   * to the next element after pos.
   */
  template <typename T>
  typename smallObjectList<T>::iterator
  smallObjectList<T>::erase(iterator pos){
    node *theNode = pos.pos;
    if(theNode == first){
      pop_front();
      return iterator(this,first);
    }
    else if(theNode == last){
      pop_back();
      return iterator(this,0);
    }
    else{
      theNode->prev->next = theNode->next;
      theNode->next->prev = theNode->prev;
      iterator it(this,theNode->next);
      --count;
      theHeap.deallocate(theNode);
      return it;
    }
  }

  /*
   * Erases the elements between first and last, and returns an iterator
   * pointing to the next element after last.
   */
  template <typename T>
  typename smallObjectList<T>::iterator 
  smallObjectList<T>::erase(iterator first,iterator last){
    while(first != last){
      first.pos = erase(first).pos;
    }
    return first;
  }

  /*
   * Inserts the range [first, last) before pos, and returns an iterator
   * pointing after the last inserted element.
   */
  template <typename T>
  typename smallObjectList<T>::iterator
  smallObjectList<T>::insert(iterator pos, 
                             const_iterator first, 
                             const_iterator last){
    while(first != last){
      pos.pos = insert(pos,*first).pos;
      ++first;
    }
    return pos;
  }

  /*
   * Inserts n copies of x before pos, and returns an iterator pointing 
   * after the last inserted element.
   */
  template <typename T>
  typename smallObjectList<T>::iterator
  smallObjectList<T>::insert(iterator pos, size_type n, const T& x){
    for(size_type i = 0; i < n; ++i){
      pos.pos = insert(pos,x).pos;
    }
    return pos;
  }

  /*
   * Inserts x before pos, and returns an iterator pointing after the
   * inserted element.
   */
  template <typename T>
  typename smallObjectList<T>::iterator 
  smallObjectList<T>::insert(iterator pos, const T& x) {
    node *next = pos.pos;
    if(next == first){
      push_front(x);
      return iterator(this,first->next);
    }
    else if(next == 0){
      push_back(x);
      return iterator(this,0);
    }
    else{
      // count is > 1
      node *newNode = theHeap.allocate();
      newNode->obj = x;
      newNode->prev = next->prev;
      newNode->next = next;
      newNode->prev->next = newNode;
      next->prev = newNode;
      ++count;
      return pos;
    }
  }

  /**
   * Removes the first occurrence of x from the list.
   */
  template <typename T>
  void smallObjectList<T>::remove(const T& x) {
    if (first->obj == x) {
      pop_front();
      return;
    }
    if (last->obj == x) {
      pop_back();
      return;
    }
    node *nod = first->next;
    while (nod != last) {
      if (nod->obj == x) {
        nod->prev->next = nod->next;
        nod->next->prev = nod->prev;
        --count;
        theHeap.deallocate(nod);
        return;
      }
      nod = nod->next;
    }
  }

  
  /*
   * Inserts x at the beginning of the list.
   */
  template <typename T>
  void smallObjectList<T>::push_front(const T& x){
    node *newNode = theHeap.allocate();
    newNode->obj = x;
    newNode->next = first;
    newNode->prev = 0;
    if(count > 0){
      first->prev = newNode;
    }
    else{
      // List was empty
      last = newNode;
    }
    first = newNode;
    ++count;
  }
  
  /*
   * Inserts x at the end of the list.
   */
  template <typename T>
  void smallObjectList<T>::push_back(const T& x){
    node *newNode = theHeap.allocate();
    newNode->obj = x;
    newNode->next = 0;
    if(count > 0){
      newNode->prev = last;
      last->next = newNode;
    }
    else{
      // List was empty
      first = newNode;
      newNode->prev = 0;
    }
    last = newNode;
    ++count;
  }
  
  /*
   * Removes the first element from the list.
   */
  template <typename T>
  void smallObjectList<T>::pop_front(){
    if(count > 0){
      node *newFirst = first->next;
      if(count == 1){
        last = 0;
      }
      else{
        newFirst->prev = 0;
      }
      theHeap.deallocate(first);
      first = newFirst;
      --count;
    }
  }
  
  /*
   * Removes the last element from the list.
   */
  template <typename T>
  void smallObjectList<T>::pop_back(){
    if(count > 0){
      node *newLast = last->prev;
      if(count == 1){
        first = 0;
      }
      else{
        newLast->next = 0;
      }
      theHeap.deallocate(last);
      last = newLast;
      --count;
    }
  }
  
  /**
   * Inserts all elements from the given list before the given position,
   * and removes them from the given list. This is a constant
   * time operation.
   */
  template <typename T>
  void smallObjectList<T>::splice(iterator position, smallObjectList<T>& l){
    if(position.pos == 0){
      last->next = l.first;
      last = l.last;
    }
    else if(position.pos == first){
      l.last->next = first;
      first = l.first;
    }
    else{
      position.pos->prev->next = l.first;
      position.pos->prev = l.last;
    }
    l.count = 0; 
    l.first = l.last = 0;
  }

  /**
   * Assignment operator. Clears this list, and copies the contents
   * of the given list.
   */
  template <typename T>
  smallObjectList<T>& smallObjectList<T>::operator = (const smallObjectList<T>& l) {
    clear();
    for(const_iterator i = l.begin(), e = l.end(); i != e; ++i){
      push_back(*i);
    }
    return *this;
  }

  /**
   * Sorts this list according to the < operator.
   */
  template <typename T>
  void smallObjectList<T>::sort(){
    quicksort(begin(),--end());
  }

  /**
   * Implementation of quicksort
   */
  template <typename T>
  void smallObjectList<T>::quicksort(iterator front, iterator back){
    // Nothing to sort, return
    if(front == back) return;    
    iterator f = front, b = back;
    T pivot = *f;
    while(f != b){
      while(pivot < *b && f != b) --b;
      if(f != b) { *f = *b; ++f; }
      while(!(pivot < *f) && f != b) ++f;
      if(f != b) { *b = *f; --b; }
    }
    *f = pivot;
    if(f != front) quicksort(front,--f);
    if(b != back) quicksort(++b,back);
  }
    
  /**
   * Constructor for segment. Initializes the list of free nodes.
   */
  template <typename T>
  smallObjectList<T>::segment::segment()
    : firstFree(&nodes[0]), next(0), prev(0), nodeCount(0) {
    for(int i = 0;
        i < _LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE - 1;
        ++i){
      nodes[i].next = &nodes[i+1];
    }
    nodes[_LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE - 1].next = 0;
  }

  /**
   * Destructor for segment. Restores the next and prev pointers of the prev
   * and next node in the list.
   */
  template <typename T>
  smallObjectList<T>::segment::~segment() {
    if(prev) prev->next = next;
    if(next) next->prev = prev;
  }
  
  /**
   * Allocates a new node in the heap
   */
  template <typename T>
  typename smallObjectList<T>::node *smallObjectList<T>::heap::allocate(){
    if(!recentAlloc->firstFree){
      // The recent alloc segment is full
      if(objectCount < segmentCount * _LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE){
        // We know that there must be at least one free node in some segment,
        // so look for it.
        // First try the recentDealloc
        if(recentDealloc->firstFree){
          recentAlloc = recentDealloc;
        }
        else {
          // Search bidirectionally
          segment *up = recentAlloc->next;
          segment *down = recentAlloc->prev;
          recentAlloc = 0;
          while(true){
            if(up){
              if(up->firstFree){ recentAlloc = up; break; }
              up = up->next;
            }
            else {
              // Only down now
              while(down){
                if(down->firstFree){ recentAlloc = down; break; }
                down = down->prev;
              }
              break;
            }
            if(down){
              if(down->firstFree){ recentAlloc = down; break; }
              down = down->prev;
            }
            else {
              // Only up now
              while(up){
                if(up->firstFree){ recentAlloc = up; break; }
                up = up->next;
              }
              break;
            }
          }
          if(!recentAlloc){
            // We didn't find the free node, this should never happen!
            throw exception("Fatal: Free node not found when allocating (smallObjectList).");
          }
        }
      }
      else{
        // All segments are full, so add a new one
        recentAlloc = new segment;
        recentAlloc->next = first;
        first->prev = recentAlloc;
        first = recentAlloc;
        ++segmentCount;
      }
    }
    ++objectCount;
    return recentAlloc->grab();
  }
  
  /**
   * Frees the given node from the heap
   */
  template <typename T>
  void smallObjectList<T>::heap::deallocate(node *n){
    if(!recentDealloc->contains(n)){
      // Node not in recentDealloc, find segment that contains it
      if(recentAlloc->contains(n)){
        recentDealloc = recentAlloc;
      }
      else{
        // Search bidirectionally
        segment *up = recentDealloc->next;
        segment *down = recentDealloc->prev;
        recentDealloc = 0;
        while(true){
          if(up){
            if(up->contains(n)){ recentDealloc = up; break; }
            up = up->next;
          }
          else {
            // Only down now
            while(down){
              if(down->contains(n)){ recentDealloc = down; break; }
              down = down->prev;
            }
            break;
          }
          if(down){
            if(down->contains(n)){ recentDealloc = down; break; }
            down = down->prev;
          }
          else {
            // Only up now
            while(up){
              if(up->contains(n)){ recentDealloc = up; break; }
              up = up->next;
            }
            break;
          }
        }
        if(!recentDealloc){
          // Node not found, this should never happen!!!
          throw exception("Fatal: Node not found when deallocating (smallObjectList).");
        }
      }
    }
    recentDealloc->release(n);
    --objectCount;
    if(recentDealloc->nodeCount == 0 && recentDealloc != first){
      segment *s = recentDealloc;
      recentDealloc = recentDealloc->prev;
      delete s;
      --segmentCount;
    }
  }
  
}

