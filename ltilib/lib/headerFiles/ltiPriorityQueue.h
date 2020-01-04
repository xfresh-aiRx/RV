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
 * file .......: ltiPriorityQueue.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 24.08.2003
 * revisions ..: $Id: ltiPriorityQueue.h,v 1.4 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_PRIORITY_QUEUE_H_
#define _LTI_PRIORITY_QUEUE_H_

#include "ltiMathObject.h"
#include "ltiSTLIoInterface.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {


  /**
   * Simple priority queue data class.
   *
   * The main difference with the std::priority_queue is that when you insert
   * an element in the queue, you get an identification token (positive
   * integer value), that will uniquely identify the element in the queue as
   * long as the element exists.  This is used to access the specific element
   * independently of its position in the queue, allowing to change its key or
   * data, changing its positions, but still keeping a valid reference to it.
   *
   * The queue is always sorted in increasing order of the key.  This means
   * that the smallest key is always on the top of the queue.
   *
   * Removing elements of the queue (pop() or erase()) are O(1) operations.
   * Inserting an elements is a O(n) operation.  Therefore, a specialized
   * method to create the queue at once (create()) is provided, which will
   * build the queue in O(n*log(n)), instead of the O(n^2) required inserting
   * the elements one by one.
   *
   * The type T is the type for the key.  The queue is sorted with
   * respect to this key.
   *
   * The type U is the type of the data contained in each element.  
   */
  template<class T,class U>
  class priorityQueue : public mathObject {
  public:
    
    /**
     * index type used to reference elements in the queue
     */
    typedef int index_type;
    
    /**
     * constructor
     * @param inv key used to indicate an invalid queue entry.  Entries
     *            are for example invalid, if they were removed or "poped"
     *            from the queue.
     */
    priorityQueue(const T& inv);
    
    /**
     * copy constructor
     */
    priorityQueue(const priorityQueue<T,U>& other);

    /**
     * destructor
     */
    virtual ~priorityQueue();
    
    /**
     * insert a new node with the given key and data and return
     * a pointer to the created node
     * @param key key of the element
     * @param data data of the element
     */
    index_type insert(const T& key,const U& data);
    
    /**
     * remove the node with the given id
     */
    void erase(const index_type id);
    
    /**
     * update the key of an element without changing its id
     */
    void update(const index_type id,
                const T& newKey);
    
    /**
     * update the key of an element without changing its id
     */
    void update(const index_type id,
                const T& newKey,
                const U& data);
    
    /**
     * Check if the given id is valid
     */
    bool valid(const index_type id) const;
    
    /**
     * Get the key for the given index id.  You must ensure first that the
     * id is valid
     */
    const T& getKey(const index_type id) const;

    /**
     * Get the data for the given index id.  You must ensure first that the
     * id is valid
     */
    const U& getData(const index_type id) const;

    /**
     * Change the data content of an element indexed by the given id.
     * You must first ensure that the id is valid.
     * @param id identification key for the element to be modified.
     * @param newData new data to be written in the element
     * @return true if successful, false otherwise.
     */
    bool setData(const index_type id,const U& newData);

    /**
     * return true if the queue is empty
     */
    bool empty() const;
    
    /**
     * create the queue using the given data.  The ids will be the original
     * element index
     */
    void create(const std::vector<T>& key,
                const std::vector<U>& data);
    
    /**
     * create the queue using the given data.  The ids will be the original
     * element index
     */
    void create(const std::vector< std::pair<T,U> >& data);

    /**
     * return reference to element at the front
     */
    const std::pair<T,U>& front() const;
    
    /**
     * copy all data in the priority queue into the given std::vector.
     */
    void getData(std::vector< std::pair<T,U> >& d) const;
    
    /**
     * remove the first element of the queue
     */
    void pop();
    
    /**
     * remove all elements from the priority queue
     */
    void clear();
    
    /**
     * copy the priority queue
     * @param other the priority queue to be copied.
     * @return a reference to the current object
     */
    priorityQueue<T,U>& copy(const priorityQueue<T,U>& other);

    /**
     * copy the priority queue
     * @param other the priority queue to be copied.
     * @return a reference to the current object
     */
    priorityQueue<T,U>& operator=(const priorityQueue<T,U>& other);

    /**
     * returns a copy of this object
     */
    virtual mathObject* clone() const;

    /**
     * returns name of this type
     */
    const char* getTypeName() const;

    /**
     * write the priority queue
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;
    
    /**
     * read the priority queue
     */
    virtual bool read(ioHandler& handler,const bool complete=true);
    
    /**
     * write all attributes of the class
     *
     * To serialize a priority queue, not all data is necessary, but
     * for debugging purposes you might want to have all internals written.
     * This method writes everything.
     */
    virtual bool writeAll(ioHandler& handler,const bool complete=true) const;
    
    /**
     * read the output of the writeAll method.
     */
    virtual bool readAll(ioHandler& handler,const bool complete=true);

  protected:
    
    /**
     * debugging method to check the consistency of the priority queue
     */
    bool checkConsistency() const;
    
    /**
     * code used to indicate a removed element'
     */
    T invalid;
    
    /**
     * the real data.  Some of its elements can be removed, so the real
     * number of elements is NOT data.size() but numElements.  The data
     * here is sorted.  Because the indices of the elements will change
     * each time an element is inserted/removed, an equivalences vector
     * will keep track of the element movements.
     */
    std::vector< std::pair<T,U> > data;
    
    /**
     * number of elements in data
     */
    int numElements;
    
    /**
     * top of the stack.  This pointer MUST always point to the first valid 
     * element!
     */
    int stackPtr;
    
    /**
     * number of elements removed until now
     */
    int numRemoved;
    
    /**
     * get the real element position in the data vector given the
     * original index.
     */
    std::vector<int> idToIndex;
    
    /**
     * get the real element position in the data vector given the
     * original index.
     */
    std::vector<int> indexToId;
  };
  

}

#include "ltiPriorityQueue_template.h"
#include "ltiUndebug.h"
#endif
