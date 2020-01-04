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
 * file .......: ltiSmallObjectList.h
 * authors ....: Gustavo Quiros
 * organization: LTI, RWTH Aachen
 * creation ...: 12.12.03
 * revisions ..: $Id: 
 */

#ifndef _LTI_SMALL_OBJECT_LIST_H_
#define _LTI_SMALL_OBJECT_LIST_H_

#include "ltiObject.h"
#include "ltiIoObject.h"
#include "ltiTypes.h"
#include "ltiException.h"
#include "ltiAssert.h"

/**
 * Performance macro, temporarily here
 * The segment size of the heap. Indicates the amount of nodes
 * that will be allocated at once when there is no more available
 * memory.
 */
#define _LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE 100

namespace lti {
  
  /**
   * smallObjectList template class.
   *
   * The %lti::smallObjectList is an efficient implementation
   * of a (double) linked list for small data types. It maintains a
   * common heap for all lists of the same contained type. It should
   * serve, in many cases, as a drop-in replacement for std::list. 
   */

  template <typename T>
  class smallObjectList : public ioObject {
    
    struct node;
    class heap;

  public:
    
    class iterator;
    class const_iterator;
    friend class iterator;
    friend class const_iterator;

    /**
     * The type used to store the size of this list.
     */
    typedef unsigned int size_type;

    /**
     * reference type (allows read and write operations)
     * The use of the reference classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef T& reference;

    /**
     * const_reference type (allows read-only operations)
     * The use of the reference classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef const T& const_reference;
    
    /**
     * pointer type (allows read and write operations)
     * The use of the pointer classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef T* pointer;
    
    /**
     * const_pointer type (allows read-only operations)
     * The use of the pointer classes is similar to the
     * references of the STL (Standard Template Library).
     */
    typedef const T* const_pointer;

    /**
     * Default constructor. Creates an empty smallObjectList.
     */
    smallObjectList() :
      theHeap(heap::instance()),
      count(0), first(0), last(0){
    }
    
    /**
     * Copy constructor. Creates a smallObjectList with the same
     * contents as the given list.
     */
    smallObjectList(const smallObjectList& l)
      : theHeap(heap::instance()),
        count(0), first(0), last(0) {
      operator = (l);
    }

    /**
     * Destructor
     */
    ~smallObjectList() { clear(); }

    /**
     * Returns the number of elements in the list.
     */
    size_type size() const { return count; }

    /**
     * Returns true if the list has no elements, false otherwise.
     */
    bool empty() const { return count == 0; }

    /**
     * Empties the list.
     */
    void clear() { while(count > 0) { pop_back(); } }
    
    /**
     * Returns an iterator pointing to the first element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    iterator begin() { return iterator(this,first); }

    /**
     * Returns an iterator pointing after the last element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    iterator end() { return iterator(this,0); }
    
    /**
     * Returns a const_iterator pointing to the first element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    const_iterator begin() const { return const_iterator(this,first); }
    
    /**
     * Returns a const_iterator pointing after the last element of the list.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    const_iterator end() const { return const_iterator(this,0); }
    
    /**
     * Erases the element at position pos, and returns an iterator pointing
     * to the next element after pos.
     */
    iterator erase(iterator pos);
    
    /**
     * Erases the elements between first and last, and returns an iterator
     * pointing to the next element after last.
     */
    iterator erase(iterator first, iterator last);
    
    /**
     * Inserts the range [first, last) before pos, and returns an iterator
     * pointing after the last inserted element.
     */
    iterator insert(iterator pos, const_iterator first, const_iterator last);

    /**
     * Inserts n copies of x before pos, and returns an iterator pointing 
     * after the last inserted element.
     */
    iterator insert(iterator pos, size_type n, const T& x);
    
    /**
     * Inserts x before pos, and returns an iterator pointing after the
     * inserted element.
     */
    iterator insert(iterator pos, const T& x);

    /**
     * Removes the first instance of T found in the list. If the value
     * \a x is not in the list, the list remains unchanged.
     * @param x value to be removed from the list
     */
    void remove(const T& x);

    /**
     * Inserts x at the beginning of the list.
     */
    void push_front(const T& x);
    
    /**
     * Inserts x at the end of the list.
     */
    void push_back(const T& x);
    
    /**
     * Removes the first element from the list.
     */
    void pop_front();
    
    /**
     * Removes the last element from the list.
     */
    void pop_back();
    
    /**
     * Returns a reference to the first element of the list.
     */
    reference front() { return *begin(); }
    
    /**
     * Returns a const_reference to the first element of the list.
     */
    const_reference front() const { return *begin(); }
    
    /**
     * Returns a reference to the last element of the list.
     */
    reference back() { return *(--end()); }
    
    /**
     * Returns a const_reference to the last element of the list.
     */
    const_reference back() const { return *(--end()); }

    /**
     * Sorts this list according to the < operator.
     */
    void sort();

    /**
     * Swaps the contents of this list with the given list.
     */
    void swap(smallObjectList<T>& l){
      std::swap(count,l.count);
      std::swap(first,l.first);
      std::swap(last,l.last);
    }

    /**
     * Inserts all elements from the given list before the given position,
     * and removes them from the given list. This is a constant
     * time operation.
     */
    void splice(iterator position, smallObjectList<T>& l);
    
    /**
     * Assignment operator. Clears this list, and copies the contents
     * of the given list.
     */
    smallObjectList<T>& operator = (const smallObjectList<T>& l);
    
    /**
     * iterator class (allows read and write operations)
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library).
     */
    class iterator {
      
    public:
      
      /**
       * Iterator traits. These are required by the algorithms
       * of the STL.
       */
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef T value_type;
      typedef T difference_type;
      typedef T* pointer;
      typedef T& reference;

    private:

      friend class smallObjectList<T>;
      friend class const_iterator;
      
      /**
       * Pointer to the list being iterated.
       */
      smallObjectList *theList; 

      /**
       * Pointer to the current position.
       */
      node *pos;
 
    protected:

      /**
       * Creates an iterator for the given list, at the given position.
       */
      iterator(smallObjectList *l, node *p)
        : theList(l),
          pos(p) {
      }
      
    public:

      /**
       * Creates an uninitialized iterator.
       */
      iterator() : theList(0), pos(0) {}

      /**
       * Copy constructor. Creates a copy of the given iterator.
       */
      iterator(const iterator& i)
        : theList(i.theList),
          pos(i.pos) {
      }

      /**
       * Returns true if both iterators are at the same position on the
       * same list, false otherwise.
       */
      bool operator == (const iterator& i) const {
        return pos == i.pos;
      }
      
      /**
       * Returns false if both iterators are at the same position on the
       * same list, true otherwise.
       */
      bool operator != (const iterator& i) const {
        return pos != i.pos;
      }
      
      /**
       * Returns a reference to the current element.
       */
      reference operator * () const {
        return pos->obj;
      }

      /**
       * Overwrites the current element with the given element.
       */
      reference operator * (T obj) const {
        return pos->obj = obj;
      }

      /**
       * Returns a pointer to the current element.
       */
      pointer operator -> () const {
        return &(*pos.obj);
      }
      
      /**
       * Moves forward one element in the list, and returns itself.
       */
      iterator& operator ++ () {
        pos = pos->next;
        return *this;
      }
      
      /**
       * Moves forward one element in the list, and returns a copy of itself
       * before the move.
       */
      iterator operator ++ (int) { 
        iterator tmp(*this);
        ++*this;
        return tmp;
      }

      /**
       * Moves backward one element in the list, and returns itself.
       */
      iterator& operator -- () {
        pos = ((pos == 0) ? theList->last : pos->prev);
        return *this;
      }
      
      /**
       * Moves backward one element in the list, and returns a copy of itself
       * before the move.
       */
      iterator operator -- (int) { 
        iterator tmp(*this);
        --*this;
        return tmp;
      }

    };

    /**
     * const_iterator class (allows read-only operations)
     * The use of the iterator classes is similar to the iterators of
     * the STL (Standard Template Library).
     */
    class const_iterator {
  
    public:

      /**
       * Iterator traits. These are required by the algorithms
       * of the STL.
       */
      typedef std::bidirectional_iterator_tag iterator_category;
      typedef T value_type;
      typedef T difference_type;
      typedef T* pointer;
      typedef T& reference;

    private:
      
      friend class smallObjectList<T>;
      
      /**
       * Pointer to the list being iterated.
       */
      smallObjectList const *theList; 

      /**
       * Pointer to the current position.
       */
      node *pos;
      
    protected:
      
      /**
       * Creates an const_iterator for the given list, at the given position.
       */
      const_iterator(const smallObjectList *l, node *p)
        : theList(l),
          pos(p) {
      }
      
    public:
      
      /**
       * Creates an uninitialized const_iterator.
       */      
      const_iterator() : theList(0), pos(0) {}
      
      /**
       * Copy constructor. Creates a copy of the given const_iterator.
       */
      const_iterator(const const_iterator& i)
        : theList(i.theList),
          pos(i.pos) {
      }
      
      /**
       * Copy constructor. Creates a copy of the given iterator.
       */
      const_iterator(const iterator& i)
        : theList(i.theList),
          pos(i.pos) {
      }

      /**
       * Returns true if both iterators are at the same position on the
       * same list, false otherwise.
       */
      bool operator == (const const_iterator& i) const {
        return pos == i.pos;
      }

      /**
       * Returns false if both iterators are at the same position on the
       * same list, true otherwise.
       */
      bool operator != (const const_iterator& i) const {
        return pos != i.pos;
      }
      
      /**
       * Returns a const_reference to the current element.
       */
      const_reference operator * () const {
        return pos->obj;
      }

      /**
       * Returns a const_pointer to the current element.
       */
      const_pointer operator -> () const {
        return &(pos->obj);
      }
      
      /**
       * Moves forward one element in the list, and returns itself.
       */
      const_iterator& operator ++ () {
        pos = pos->next;
        return *this;
      }
      
      /**
       * Moves forward one element in the list, and returns a copy of itself
       * before the move.
       */
      const_iterator operator ++ (int) { 
        const_iterator tmp(*this);
        ++*this;
        return tmp;
      }

      /**
       * Moves backward one element in the list, and returns itself.
       */
      const_iterator& operator -- () {
        pos = ((pos == 0) ? theList->last : pos->prev);
        return *this;
      }

      /**
       * Moves backward one element in the list, and returns a copy of itself
       * before the move.
       */
      const_iterator operator -- (int) { 
        const_iterator tmp(*this);
        --*this;
        return tmp;
      }

    };

  private:
    
    /**
     * node structure. Represents a node of the linked list.
     */
    struct node {
      
      /**
       * The stored element.
       */
      T obj;

      /**
       * Pointer to the previous node.
       */
      node *prev;

      /**
       * Pointer to the next node.
       */
      node *next;

    };
    
    /**
     * A segment of the heap, used to allocate memory for nodes. Since
     * these nodes are used for a linked list, the unused nodes are
     * kept in a (singly) linked list of free nodes.
     */
    class segment {

      friend class heap;
      
      /**
       * The array of nodes contained in this segment
       */
      node nodes[_LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE];

      /**
       * A pointer to the first free node of the segment
       */
      node *firstFree;
      
      /**
       * A pointer to the next segment in the heap
       */
      segment *next;

      /**
       * A pointer to the previous segment in the heap
       */
      segment *prev;
      
      /**
       * The number of nodes currently allocated in this segment
       */
      short int nodeCount;

      /**
       * Constructor. Initializes the list of free nodes.
       */
      segment();

      /**
       * Destructor
       */
      ~segment();

      /**
       * Determines if the given node is contained in this segment, that is,
       * if the pointer points within the inner node array.
       */
      inline bool contains(node *node) {
        return
          (node >= &nodes[0]) &&
          (node < &nodes[_LTI_PERFORMANCE_SMALLOBJECTLIST_HEAP_SEGMENT_SIZE]);
      }

      /**
       * 'Grabs' (allocates) a node in this segment.
       */
      inline node* grab(){
        ++nodeCount;
        node *n = firstFree;
        firstFree = firstFree->next;
        return n;
      }

      /**
       * Releases the given node in this segment
       */
      inline void release(node *n){
        --nodeCount;
        n->next = firstFree;
        firstFree = n;
      }
      
    };
    
    /**
     * A heap of nodes. Stores nodes in dynamically allocated
     * segments.
     */
    class heap {
      
      /**
       * A pointer to the first segment in the list of segments
       */
      segment *first;
      
      /**
       * A pointer to the segment most recently used for allocation
       */
      segment *recentAlloc;

      /**
       * A pointer to the segment most recently used for deallocation
       */
      segment *recentDealloc;

      /**
       * The total object count contained in this heap
       */
      unsigned long int objectCount;

      /**
       * The number of segments in this heap
       */
      unsigned long int segmentCount;

      /**
       * Constructor. Segment lists are initially empty.
       */
      heap() : first(new segment), recentAlloc(first), recentDealloc(first),
               objectCount(0), segmentCount(1) {
      }
      
      /**
       * Destructor. Frees the lists of segments
       */
      ~heap(){
        segment *s, *n;
        s = first;
        while(s){
          n = s->next;
          delete s;
          s = n;
        }
      }
      
      /**
       * Disable the copy constructor
       */
      heap(const heap&);

      /**
       * Disable the assignment operator
       */
      heap& operator = (const heap&);

    public:
      
      /**
       * The singleton instance accessor
       */
      static heap& instance(){
        static heap theInstance;
        return theInstance;
      }
      
      /**
       * Allocates a new node in the heap
       */
      node *allocate();
      
      /**
       * Frees the given node from the heap
       */
      void deallocate(node *n);
      
    };
    
    /**
     * The heap of nodes, used for memory allocation and management.
     */
    heap& theHeap;
    
    /**
     * The counter of stored elements in the list. It stores the size of
     * the list.
     */
    size_type count;

    /**
     * The pointer to the first element of the list. 
     */
    node *first;

    /**
     * The pointer to the last element of the list. 
     */
    node *last;
    
    /**
     * Quicksort implementation used by sort().
     */
    void quicksort(iterator front, iterator back);
       
  };
  
}

#include "ltiSmallObjectList_template.h"

#endif
