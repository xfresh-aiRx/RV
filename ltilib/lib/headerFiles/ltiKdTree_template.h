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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiKdTree.cpp
 * authors ....: Frederik Lange, Jens Rietzschel, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 9.1.2003
 * revisions ..: $Id: ltiKdTree_template.h,v 1.27 2005/06/23 15:09:16 doerfler Exp $
 */

// MS VC++ defines min and max somewhere.  This conflicts with the LTI-Lib
// definition of min and max as functions, and with the std::numeric_limits,
// which have methods min and max.  So let's get rid of this awful old C
// stuff:
#undef min
#undef max

#include <limits>

#undef _LTI_DEBUG
// #define _LTI_DEBUG 3
#include "ltiDebug.h"

#ifdef _LTI_DEBUG

#include "ltiLispStreamHandler.h"

// a lisp stream handler used to output some elements
static lti::lispStreamHandler _lti_debug_lsh(std::cerr);

#endif

namespace lti {

  // --------------------------------------------------
  // kdTree
  // --------------------------------------------------

  // default constructor
  template<class T,class D,class U>
  kdTree<T,D,U>::kdTree()
    : mathObject(),status(),root(0),levels(0),
      numElements(0),numAddedElements(0),numLeaves(0) {
  }

  // copy constructor
  template<class T,class D,class U>
  kdTree<T,D,U>::kdTree(const kdTree& other) 
    : mathObject(),status(),root(0),levels(0),
      numElements(0),numAddedElements(0),numLeaves(0) {
    copy(other);
  }

  // destructor
  template<class T,class D,class U>
  kdTree<T,D,U>::~kdTree() {
    clear();
  }

  // returns the name of this type
  template<class T,class D,class U>
  const char* kdTree<T,D,U>::getTypeName() const {
    return "kdTree";
  }

  // copy member
  template<class T,class D,class U>
  bool kdTree<T,D,U>::empty() const {
    return (isNull(root));
  }

  /*
   * get the number of elements added to the tree
   */
  template<typename T,typename D,class U>
  int kdTree<T,D,U>::getNumberOfAddedElements() const {
    return numAddedElements;
  }

  template<typename T,typename D,class U>
  int kdTree<T,D,U>::size() const {
    return numElements;    
  }
  
  /*
   * get the number of leaves in the tree
   */
  template<class T,class D,class U>
  int kdTree<T,D,U>::getNumberOfLeaves() const {
    if (isNull(root)) {
      return 0;
    } else {
      return numLeaves;
    }
  }

  /*
   * get the number of levels of the tree
   */
  template<class T,class D,class U>
  int kdTree<T,D,U>::getLevels() const {
    if (isNull(root)) {
      return 0;
    } else {
      return levels;
    }
  }

  /*
   *  get pointer to root node
   */
  template<class T, class D, class U>
  typename kdTree<T,D,U>::node* kdTree<T,D,U>::getRoot(){
      return root;
  }


  // copy member
  template<class T,class D,class U>
  kdTree<T,D,U>&
  kdTree<T,D,U>::copy(const kdTree& other) {
    mathObject::copy(other);

    if (notNull(root)) {

      if (!other.empty()) {
        root->copy(*other.root);
      } else {
        clear();
      }

    } else {

      if (!other.empty()) {
        root = other.root->clone();
      }

    }

    // we also have to copy the elements added since the last call to
    // build() or rebuild()
    if (!other.points.empty()) {
      typename std::list<element*>::const_iterator it 
        = other.points.begin();
      const typename std::list<element*>::const_iterator ite 
        = other.points.end();
      while (it!=ite) {
        points.push_back((*it)->clone());
        ++it;
      }
    }


    levels = other.levels;
    numElements = other.numElements;
    numAddedElements = other.numAddedElements;
    numLeaves = other.numLeaves;

    // copy totalBounds and re-init the other bounds stuff
    totalBounds.copy(other.totalBounds);
    boundsBox.copy(totalBounds);
    boundsMin.useExternData(boundsBox.columns(),&boundsBox.at(0,0));
    boundsMax.useExternData(boundsBox.columns(),&boundsBox.at(1,0));

    return (*this);
  }

  // alias for copy member
  template<class T,class D,class U>
  kdTree<T,D,U>&
  kdTree<T,D,U>::operator=(const kdTree& other) {
    return (copy(other));
  }

  // clone member
  template<class T,class D,class U>
  mathObject* kdTree<T,D,U>::clone() const {
    return new kdTree(*this);
  }

  // clear member
  template<class T,class D,class U>
  void kdTree<T,D,U>::clear() {
    // delete all nodes
    delete root;
    root=0;

    // delete unassigned data nodes
    typename std::list<element*>::iterator it,eit;
    for (it=points.begin(),eit=points.end();it!=eit;++it) {
      delete (*it);
      (*it)=0;
    }
    points.clear();

    levels=0;
    numElements=0;
    numLeaves=0;
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::write(ioHandler &handler, 
                          const bool complete) const {
    bool b=true;

    if (complete) {
      b = handler.writeBegin();
    }

    // 
    b = b && lti::write(handler,"numElements",numElements);
    b = b && lti::write(handler,"numLeaves",numLeaves);
    b = b && lti::write(handler,"levels",levels);
    b = b && lti::write(handler,"pointDim",totalBounds.columns());

    // write the nodes
    b = b && handler.writeBegin(); // nodes           ----|
    b = b && handler.writeSymbol("nodes");       //       |
    b = b && handler.writeKeyValueSeparator();   //       |
                                   //                     |
    b = b && handler.writeBegin(); // data in the nodes   |
    if (notNull(root)) {                         //  |    |
      b = b && root->write(handler,false);       //  |    |
    }                                            //  |    |
                                                 //  |    |
    b = b && handler.writeEnd();   // data in the nodes   |
    b = b && handler.writeEnd();   // nodes           ____|

    if (complete) {
      b = handler.writeEnd();
    }
    
    return b;
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::read(ioHandler &handler, const bool complete) {
    bool b = true;
    int dim;

    clear(); // remove root and all related stuff

    if (complete) {
      b = handler.readBegin();
    }

    // 
    b = b && lti::read(handler,"numElements",numElements);
    b = b && lti::read(handler,"numLeaves",numLeaves);
    b = b && lti::read(handler,"levels",levels);
    b = b && lti::read(handler,"pointDim",dim);

    // read the nodes
    b = b && handler.readBegin();             // nodes         -----|   
    b = b && handler.trySymbol("nodes");      //                    |
    b = b && handler.readKeyValueSeparator(); //                    |
                                              //                    |
    b = b && handler.readBegin();             // data in the nodes  |
    if (!handler.tryEnd()) {                  //                 |  |
      root = new node();                      //                 |  |
      b = b && root->read(handler,false);     //                 |  |
      b = b && handler.readEnd();             // data in the nodes  |
    }                                         //                    |
                                              //                    |
    b = b && handler.readEnd();               // nodes         _____|

    if (complete) {
      b = handler.readEnd() && b;
    }

    // restore the sizes for the rest of the internal data

    if (numElements > 0) {
      totalBounds.resize(2,dim,value_type(),false,false);

      if (std::numeric_limits<value_type>::is_integer) {
        // initialize the mins with max
        totalBounds.getRow(1).fill(std::numeric_limits<value_type>::max());
        
        value_type tmp = std::numeric_limits<value_type>::min();
        
        if (tmp == 0) {
          // initialize the maxs with min --> unsigned types 
          totalBounds.getRow(0).fill(tmp);
        } else {
          // initialize the maxs with min
          // don't use the absolute minimum:  it cannot be negated and can
          // produce arithmetic errors
          totalBounds.getRow(0).fill(-std::numeric_limits<value_type>::max());
        }
      

      } else {
        // floating point types
        value_type tmp = sqrt(std::numeric_limits<value_type>::max());
        // initialize the mins with max
        totalBounds.getRow(1).fill(tmp);
        // initialize the maxs with min
        totalBounds.getRow(0).fill(-tmp);
      }

      // resize the temporal bounds and indicate memory sharing...
      boundsBox.copy(totalBounds);
      boundsMin.useExternData(boundsBox.columns(),&boundsBox.at(0,0));
      boundsMax.useExternData(boundsBox.columns(),&boundsBox.at(1,0));
    }

    return b;
  }
  
  template<class T,class D,class U>
  void kdTree<T,D,U>::add(const T& point,const D& data) {
    element* e = new element(point,data);
    points.push_back(e);
    ++numElements;
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::rebuild(const int bucketSize) {
    if (bucketSize<1) {
      return false;
    }

    if (points.empty()) {
      if (isNull(root)) {
        setStatusString("no data added to kdTree when trying to rebuild\n");
        return false;
      } else {
        //all we could do is build the tree again, which we wont
        return true;
      }
    }

    // check whether there are already elements in the tree.
    // if yes get them out and include in the rebuild of the tree.
    if (!isNull(root)) {
      std::list<element*> tmp;
      getDataInSubtree(root, tmp);
      typename std::list<element*>::const_iterator it=tmp.begin();
      typename std::list<element*>::const_iterator ite=tmp.end();
      for (; it!=ite; ++it) {
        points.push_back((*it)->clone());
      }
      delete root;
      root=0;
      numAddedElements+=numElements;
    }

    return build(bucketSize);
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::build(const int bucketSize) {
    if (bucketSize<1) {
      return false;
    }

    delete root;
    root=0;
    if (points.empty()) {
      return false;
    }

    root=new node;
    const int dim = points.front()->size();
    numLeaves=0;
    root->add(points);
    root->subdivide(bucketSize,levels,numLeaves);
    // we must delete the points vector, otherwise its elements will
    // be deleted twice
    points.clear();

    // initialize the bounds
    // this is somehow tricky.  The distances will be manipulated as square
    // values of euclidean distances.  For integer values, this can easily
    // cause overflows in the variable contents when building the square if
    // the absolute max and min values for the types are used.  So, the
    // square roots of those values should be used instead! (they are still
    // big enough)
    totalBounds.resize(2,dim,value_type(),false,false);
    // initialize the maxs with min
    if (std::numeric_limits<value_type>::is_integer) {
      // initialize the mins with max
      totalBounds.getRow(1).fill(std::numeric_limits<value_type>::max());

      value_type tmp = std::numeric_limits<value_type>::min();
      
      if (tmp == 0) {
        // initialize the maxs with min --> unsigned types 
        totalBounds.getRow(0).fill(tmp);
      } else {
        // initialize the maxs with min
        // don't use the absolute minimum:  it cannot be negated and can
        // produce arithmetic errors
        totalBounds.getRow(0).fill(-std::numeric_limits<value_type>::max());
      }
      

    } else {
      // floating point types
      value_type tmp = sqrt(std::numeric_limits<value_type>::max());
      // initialize the mins with max
      totalBounds.getRow(1).fill(tmp);
      // initialize the maxs with min
      totalBounds.getRow(0).fill(-tmp);
    }

    // resize the temporal bounds and indicate memory sharing...
    boundsBox.copy(totalBounds);
    boundsMin.useExternData(boundsBox.columns(),&boundsBox.at(0,0));
    boundsMax.useExternData(boundsBox.columns(),&boundsBox.at(1,0));

    return true;
  }


  template<class T,class D,class U>
  void kdTree<T,D,U>::getDataInSubtree(node* nodePtr,
                                     std::list<element*>& data) const {
    // if node contains points, append them at the end of this->points
    if (!(nodePtr->points.empty())) {
      data.insert(data.end(),nodePtr->points.begin(),nodePtr->points.end());
    }
    
    // get data from the left child
    if (notNull(nodePtr->left)) {
      getDataInSubtree(nodePtr->left,data);
    }

    // get data from the right child
    if (notNull(nodePtr->right)) {
      getDataInSubtree(nodePtr->right,data);
    }
  }


  /*
   * search for an element with exactly the given position in the
   * hyperspace.
   *
   * This is the classical search in a kd-tree, that assumes that 
   * a leaf node contains exactly the given point.
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false.
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchExactly(const T& key,element& elem) const {
    if (notNull(root)) {
      return searchExactly(root,key,elem);
    }
    return false;
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchExactly(const node* nptr,
                                  const T& key,element& elem) const {
    if (nptr->isLeaf()) {
      // linearly search for the key in the contained points.
      return nptr->getPoint(key,elem);
    } else {
      if ((key[nptr->splitDim]<nptr->partition) && (notNull(nptr->left))) {
        // if at all there, the key must be on the left subtree
        return searchExactly(nptr->left,key,elem);
      } 
      else if ((key[nptr->splitDim]>nptr->partition) && 
               (notNull(nptr->right))) {
        // if at all there, the key must be on the right subtree
        return searchExactly(nptr->right,key,elem);
      } 
      else if (key[nptr->splitDim]==nptr->partition) {
        // the key can be in any subtree, so we need to look for it everywhere
        if (notNull(nptr->left) && (searchExactly(nptr->left,key,elem))) {
          return true;
        }
        
        if (notNull(nptr->right) && (searchExactly(nptr->right,key,elem))) {
          return true;
        }
      }
   
    }
    return false;
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchExactly(const T& key,
                                  std::list<element>& elem) const {
    elem.clear();
    if (notNull(root)) {
      return searchExactly(root,key,elem);
    }
    return false;
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchExactly(const node* nptr,
                                    const T& key,
                                    std::list<element>& elem) const {
    if (nptr->isLeaf()) {
      // linearly search for the key in the contained points.
      return nptr->getPoint(key,elem);
    } else {
      if ((key[nptr->splitDim]<nptr->partition) && (notNull(nptr->left))) {
        // if at all there, the key must be on the left subtree
        return searchExactly(nptr->left,key,elem);
      } 
      else if ((key[nptr->splitDim]>nptr->partition) && 
               (notNull(nptr->right))) {
        // if at all there, the key must be on the right subtree
        return searchExactly(nptr->right,key,elem);
      } 
      else if (key[nptr->splitDim]==nptr->partition) {
        // the key can be in any subtree, so we need to look for it everywhere
        bool res(false);
        if (notNull(nptr->left)) {
          res = searchExactly(nptr->left,key,elem);
        }
        
        if (notNull(nptr->right)) {
          // first search the other node, and after that compute res...
          res = searchExactly(nptr->right,key,elem) || res;
        }
        return res;
      }   
    }
    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,element*& elem) const {
    elem = 0;
    if (notNull(root)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to boundsBox.fill(totalBounds), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=&totalBounds.at(0,0),pe=&totalBounds.at(1,boundsMax.lastIdx()),
             p=&boundsMin.at(0);pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<acc_type,element*> neigh(std::numeric_limits<acc_type>::max(),
                                         (element*)(0));
      searchNearest(root,key,neigh);
      if (notNull(neigh.second)) {
        elem=neigh.second;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,
                                    element*& elem,
                                    acc_type& dist) const {
    elem = 0;
    if (notNull(root)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to boundsBox.fill(totalBounds), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=&totalBounds.at(0,0),pe=&totalBounds.at(1,boundsMax.lastIdx()),
             p=&boundsMin.at(0);pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<acc_type,element*> neigh(std::numeric_limits<acc_type>::max(),
                                         (element*)(0));
      searchNearest(root,key,neigh);
      if (notNull(neigh.second)) {
        elem=neigh.second;
        dist=neigh.first;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,element& elem) const {

    if (notNull(root)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to boundsBox.fill(totalBounds), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=&totalBounds.at(0,0),pe=&totalBounds.at(1,boundsMax.lastIdx()),
             p=&boundsMin.at(0);pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<acc_type,element*> neigh(std::numeric_limits<acc_type>::max(),
                                         (element*)(0));
      searchNearest(root,key,neigh);
      if (notNull(neigh.second)) {
        elem=*(neigh.second);
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,
                                    element& elem,
                                    acc_type& dist) const {

    if (notNull(root)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to boundsBox.fill(totalBounds), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=&totalBounds.at(0,0),pe=&totalBounds.at(1,boundsMax.lastIdx()),
             p=&boundsMin.at(0);pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<acc_type,element*> neigh(std::numeric_limits<acc_type>::max(),
                                         (element*)(0));
      searchNearest(root,key,neigh);
      if (notNull(neigh.second)) {
        elem=*(neigh.second);
        dist=  neigh.first;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest element in the tree to the given key point.
   *
   * If found, the contents of the element will by copied in the 
   * elem parameters and true will be returned.   Otherwise false will
   * be returned.
   * @param key point in the n-dimensional space you are looking for.
   * @param elem the contents of the found point will be left here.
   * @return true if key was found, otherwise false (i.e. tree empty).
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const T& key,D& data) const {

    if (notNull(root)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to boundsBox.fill(totalBounds), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=&totalBounds.at(0,0),pe=&totalBounds.at(1,boundsMax.lastIdx()),
             p=&boundsMin.at(0);pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      // clear old data
      std::pair<acc_type,element*> neigh(std::numeric_limits<acc_type>::max(),
                                         (element*)(0));
      searchNearest(root,key,neigh);
      if (notNull(neigh.second)) {
        data=(neigh.second)->data;
        return true; // one neighbor is always found
      }
    }

    return false;
  }

  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const int k,
                                  const T& key,
                                  std::list<element*>& neighbors) const {

    // clear all data
    neighbors.clear();

    if (notNull(root) && (k<=numElements)) {
      matrix<value_type> bounds(totalBounds);

      // clear old data
      std::multimap<acc_type,element*> neigh;
      int nsize(0);

      while (nsize<k) {
        neigh.insert(std::make_pair(std::numeric_limits<acc_type>::max(),
                                    (element*)(0)));
        ++nsize;
      }

      searchNearest(root,k,key,bounds,neigh);
      typename std::multimap<acc_type,element*>::iterator it;
      for (it=neigh.begin();it!=neigh.end();++it) {
        if (notNull((*it).second)) {
          neighbors.push_back((*it).second);
        } else {
          return false;
        }
      }
      return true; // if k neighbors found, then everything is ok!
  
    }

    return false;
  }

  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchNearest(const int k,
                                  const T& key,
                             std::multimap<acc_type,element*>& neigh) const {

    // clear old data
    neigh.clear();

    if (notNull(root) && (k<=numElements)) {
      matrix<value_type> bounds(totalBounds);

      int nsize(0);
      while (nsize<k) {
        neigh.insert(std::make_pair(std::numeric_limits<acc_type>::max(),
                                    (element*)(0)));
        ++nsize;
      }

      return searchNearest(root,k,key,bounds,neigh);
    }

    return false;
  }

  /*
   *check if the given key point and the given bounding box
   */ 
  template<class T,class D,class U>
  inline bool
  kdTree<T,D,U>::checkHypersphereWithinBounds(const T& key,
                                            const matrix<value_type>& bounds,
                                            const acc_type& dist) const {

    _lti_debug4("  Check sphere within bounds. " << std::endl <<
                "    key   : " << key << std::endl <<
                "    bounds: " << bounds << std::endl <<
                "    dist  : " << dist << std::endl);

    value_type k;
    int i(key.size()-1);
    const vector<value_type>& boundsMin = bounds.getRow(0);
    const vector<value_type>& boundsMax = bounds.getRow(1);
    while (i>=0) {
      if ( (distantor.component(k=key[i],boundsMin.at(i)) <= dist) ||
           (distantor.component(boundsMax.at(i),k) <= dist) ) {
        _lti_debug4("false." << std::endl << std::endl);
        return false;
      }
      --i;
    }
    _lti_debug4("true." << std::endl << std::endl);
    return true;
  };

 
  /*
   * Search for elements in dist neighbourhood of the given key poin
   */
  template <class T, class D, class U>
  bool kdTree<T,D,U>::searchWithin(const T& key,
                                   const acc_type& dist,
                                   std::list<element*>& elems) const {
    elems.clear();
    if (notNull(root)){
      matrix<value_type> bounds(totalBounds);
      return searchWithin(root, key, dist*dist, bounds, elems);
    }
    return false;

  }
 
  /*
   * Search for elements in dist neighbourhood of the given key poin
   */
  template <class T, class D, class U>
  bool kdTree<T,D,U>::searchWithin(const T& key,
                                   const acc_type& dist,
                           std::multimap<acc_type,element*>& neighbors) const {
    neighbors.clear();
    if (notNull(root)){
      matrix<value_type> bounds(totalBounds);
      return searchWithin(root, key, dist*dist, bounds, neighbors);
    }
    return false;
  }

  /*
   * check if the bounds overlap the hypersphere.  It was called in
   * the original paper "bounds overlap ball".
   */
  template<class T,class D,class U>
  inline bool 
  kdTree<T,D,U>::checkBoundsOverlapHypersphere(const T& key,
                                              const matrix<value_type>& bounds,
                                              const acc_type& dist) const {

    _lti_debug4("  Check bounds overlap sphere. " << std::endl <<
                "    key   : " << key << std::endl <<
                "    bounds: " << bounds << std::endl <<
                "    dist  : " << dist << std::endl);

    acc_type sum(0);
    int i(key.size()-1);
    const vector<value_type>& boundsMin = bounds.getRow(0);
    const vector<value_type>& boundsMax = bounds.getRow(1);
    value_type k;
    while (i>=0) {
      k = key[i];
      if (k < boundsMin.at(i)) {
        // lower than low boundary
        distantor.accumulate(boundsMin.at(i),k,sum);
        if (distantor.accGreaterThan(sum,dist)) {
          _lti_debug4("false." << std::endl << std::endl);
          return false;
        }
      } else if (k > boundsMax.at(i)) {
        // higher than high boundary
        distantor.accumulate(k,boundsMax.at(i),sum);
        if (distantor.accGreaterThan(sum,dist)) {
          _lti_debug4("false." << std::endl << std::endl);
          return false;
        }
      }
      --i;
    }

    _lti_debug4("true." << std::endl << std::endl);
    return true;
  }
  
  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D,class U>
  bool 
  kdTree<T,D,U>::searchNearest(const node* nptr,
                             const int k,
                             const T& key,
                             matrix<value_type>& bounds,
                           std::multimap<acc_type,element*>& neighbors) const {
    
    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
      // examine records in bucket, and update the neighbors map if necessary
      typename node::points_type::const_iterator it,eit;
      acc_type d;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if neighbors have too few elements or if the farest
        // nearest element have a greater distance than the actual
        // element then insert the elements in the map
        if ((neighbors.rbegin())->first > 
            (d=distantor(key,(*it)->point))) {
          
          // delete the last one
          typename std::multimap<acc_type,element*>::iterator mit;
          mit = neighbors.end();
          --mit;
#         if defined(_LTI_DEBUG) && (_LTI_DEBUG > 3)
          if (notNull((*mit).second)) {
            _lti_debug3("Deleting " << ((*mit).second)->point << std::endl);
          } else {
            _lti_debug3("Deleting dummy entry" << std::endl);
          }
#         endif
          neighbors.erase(mit);

          _lti_debug3("Inserting " << (*it)->point << std::endl);
          // insert one element
          neighbors.insert(std::make_pair(d,(*it)));
        }
      }

      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key,
                                           bounds,
                                           (neighbors.rbegin())->first));
    } // end if leaf

    value_type tmp;
    const int& d=nptr->splitDim;
    const value_type& p = (nptr->partition);
    value_type& boxMin = bounds.at(0,d);
    value_type& boxMax = bounds.at(1,d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug3("Searching closer son left" << std::endl);
      if (searchNearest(nptr->left,k,key,bounds,neighbors)) {
        _lti_debug3("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,
                                        bounds,
                                        (neighbors.rbegin())->first)) {
        _lti_debug3("Searching farther son right" << std::endl);
        if (searchNearest(nptr->right,k,key,bounds,neighbors)) {
          boxMin=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } else {

      // ------------
      //  right child
      // ------------
      
      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug3("Searching closer son right" << std::endl);
      if (searchNearest(nptr->right,k,key,bounds,neighbors)) {
        boxMin=tmp;
        _lti_debug3("done." << std::endl);
        return true;
      } 

      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key,
                                        bounds,
                                        (neighbors.rbegin())->first)) {

        _lti_debug3("Searching farther son left" << std::endl);
        if (searchNearest(nptr->left,k,key,bounds,neighbors)) {
          boxMax=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;       
    }
    
    _lti_debug3("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,
                                         bounds,
                                         (neighbors.rbegin())->first));
  }
  

  /*
   * search for the nearest element in the tree to the given key point.
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D,class U>
  bool 
  kdTree<T,D,U>::searchNearest(const node* nptr,
                             const T& key,
                             std::pair<acc_type,element*>& neighbors) const {

    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
      // examine records in bucket, and update the neighbors map if necessary
      typename node::points_type::const_iterator it,eit;
      acc_type d;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if neighbors have too few elements or if the farest
        // nearest element have a greater distance than the actual
        // element then insert the elements in the map
        if (neighbors.first > (d=distantor(key,(*it)->point))) {
          
          _lti_debug3("Updating " << (*it)->point << std::endl);
          // insert one element
          neighbors.first=d;
          neighbors.second=(*it);
        }
      }

      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key,
                                           boundsBox,
                                           neighbors.first));
    } // end if leaf

    // boundsBox is a matrix containing both vectors boundsMax and boundsMin
    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;

    // save some time avoiding too many accesses to boundsMax/boundsMin
    value_type& boxMax = boundsMax.at(d);
    value_type& boxMin = boundsMin.at(d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug3("Searching closer son left" << std::endl);
      if (searchNearest(nptr->left,key,neighbors)) {
        _lti_debug3("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,
                                        boundsBox,
                                        neighbors.first)) {
        _lti_debug3("Searching farther son right" << std::endl);
        if (searchNearest(nptr->right,key,neighbors)) {
          boxMin=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } 

    // ------------
    //  right child
    // ------------

    if (key[d] >= p) {

      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug3("Searching closer son right" << std::endl);
      if (searchNearest(nptr->right,key,neighbors)) {
        boxMin=tmp;
        _lti_debug3("done." << std::endl);
        return true;
      } 
      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key,
                                        boundsBox,
                                        neighbors.first)) {

        _lti_debug3("Searching farther son left" << std::endl);
        if (searchNearest(nptr->left,key,neighbors)) {
          boxMax=tmp;
          _lti_debug3("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;
    }
    
    _lti_debug3("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,
                                         boundsBox,
                                         neighbors.first));
  }


  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D, class U>
  bool kdTree<T,D,U>::searchWithin(const node* nptr,
                                   const T& key,
                                   const acc_type& dist,
                                   matrix<value_type>& bounds,
                                   std::list<element*>& elems) const {
    
    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
     
      // examine records in bucket, and update the elements list if necessary
      typename node::points_type::const_iterator it,eit;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if point is within dist add to list of elements
        if (dist >= distanceSqr(key,(*it)->point)) {
          elems.push_back(*it);
        }
      }
      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key, bounds,dist)); 
    } // end if leaf


    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;
    value_type& boxMin = bounds.at(0,d);
    value_type& boxMax = bounds.at(1,d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug("Searching closer son left" << std::endl);
      if (searchWithin(nptr->left,key,dist,bounds,elems)) {
        _lti_debug("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,bounds,dist)) {
        _lti_debug("Searching farther son right" << std::endl);
        if (searchWithin(nptr->right,key,dist,bounds,elems)) {
          boxMin=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } else {

      // ------------
      //  right child
      // ------------
      
      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug("Searching closer son right" << std::endl);
      if (searchWithin(nptr->right,key,dist,bounds,elems)) {
        boxMin=tmp;
        _lti_debug("done." << std::endl);
        return true;
      } 

      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key, bounds,dist)) {

        _lti_debug("Searching farther son left" << std::endl);
        if (searchWithin(nptr->left,key,dist,bounds,elems)) {
          boxMax=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;       
    }
    
    _lti_debug("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,bounds,dist));
  }

  /*
   * search for the nearest k elements in the tree to the given key point.
   * @param k number of elements you are looking for
   * @param key the point in the n-dimensional space you are looking for
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D, class U>
  bool kdTree<T,D,U>::searchWithin(const node* nptr,
                                   const T& key,
                                   const acc_type& dist,
                                   matrix<value_type>& bounds,
                               std::multimap<acc_type,element*>& neigh) const {
    
    // if this is a leaf, elements should be inserted in neighbors...
    if (nptr->isLeaf()) {
     
      // examine records in bucket, and update the elements list if necessary
      typename node::points_type::const_iterator it,eit;
      acc_type d;

      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        // if point is within dist add to list of elements
        if (dist >= (d=distanceSqr(key,(*it)->point))) {
          neigh.insert(std::make_pair(d,(*it)));
        }
      }
      // check "hypersphere within bounds"
      return (checkHypersphereWithinBounds(key, bounds,dist)); 
    } // end if leaf


    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;
    value_type& boxMin = bounds.at(0,d);
    value_type& boxMax = bounds.at(1,d);

    // ------------
    //  left child
    // ------------

    if (key[d] <= p) {
      // recursive call on closer son
      tmp=boxMax;
      boxMax=p;
      _lti_debug("Searching closer son left" << std::endl);
      if (searchWithin(nptr->left,key,dist,bounds,neigh)) {
        _lti_debug("done." << std::endl);
        boxMax=tmp;
        return true;
      } 
      boxMax=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMin;
      boxMin=p;
      if (checkBoundsOverlapHypersphere(key,bounds,dist)) {
        _lti_debug("Searching farther son right" << std::endl);
        if (searchWithin(nptr->right,key,dist,bounds,neigh)) {
          boxMin=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMin=tmp;

    } else {

      // ------------
      //  right child
      // ------------
      
      // recursive call on closer son
      tmp=boxMin;
      boxMin=p;
      _lti_debug("Searching closer son right" << std::endl);
      if (searchWithin(nptr->right,key,dist,bounds,neigh)) {
        boxMin=tmp;
        _lti_debug("done." << std::endl);
        return true;
      } 

      boxMin=tmp;

      // recursive call on farther son, if necessary
      tmp=boxMax;
      boxMax=p;
      if (checkBoundsOverlapHypersphere(key, bounds,dist)) {

        _lti_debug("Searching farther son left" << std::endl);
        if (searchWithin(nptr->left,key,dist,bounds,neigh)) {
          boxMax=tmp;
          _lti_debug("done." << std::endl);
          return true;
        }
      }
      boxMax=tmp;       
    }
    
    _lti_debug("Return or terminate" << std::endl);
    // see if we should return or terminate
    return (checkHypersphereWithinBounds(key,bounds,dist));
  }



  /*
   * search the best-bin-first algorithm of Beis and Lowe.  It search
   * for the approximate k nearest neighbors.
   * @param k number of elements you are looking for.
   * @param key the point in the n-dimensional space you are looking for.
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @param emax is the maximal number of visits allowed for leaf nodes.
   *             (in the original paper is called \f$E_{max}\f$).
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchBestBinFirst(const int k,
                                       const T& key,
                                       const int emax,
                                       std::list<element*>& neighbors) const {


    // clear all data
    neighbors.clear();

    if (notNull(root) && (numElements>=k)) {

      // clear old data
      std::multimap<acc_type,element*> neigh;
      int nsize(0);

      while (nsize<k) {
        neigh.insert(std::make_pair(std::numeric_limits<acc_type>::max(),
                                    (element*)(0)));
        ++nsize;
      }

      if (searchBestBinFirst(root,k,key,neigh,emax)) {
        typename std::multimap<acc_type,element*>::iterator it;
        for (it=neigh.begin();it!=neigh.end();++it) {
          neighbors.push_back((*it).second);
        }
        return true; // if k neighbors found, then everything is ok!
      }
    }

    return false;
  }

  /*
   * search the best-bin-first algorithm of Beis and Lowe.  It search
   * for the approximate k nearest neighbors.
   * @param k number of elements you are looking for.
   * @param key the point in the n-dimensional space you are looking for.
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @param emax is the maximal number of visits allowed for leaf nodes.
   *             (in the original paper is called \f$E_{max}\f$).
   * @return true if k elements were found, or false, if the tree contains
   *             less than k elements
   */
  template<class T,class D,class U>
  bool
  kdTree<T,D,U>::searchBestBinFirst(const int k,
                                  const T& key,
                                  const int emax,
                                  std::multimap<acc_type,element*>& neighbors
                                  ) const{
    
    // clear all data
    neighbors.clear();

    if (notNull(root) && (numElements>=k)) {
      // clear old data
      for (int i=0;i<k;++i) {
        neighbors.insert(std::make_pair(std::numeric_limits<acc_type>::max(),
                                        (element*)(0)));
      }

      return searchBestBinFirst(root,k,key,neighbors,emax);
    }

    return false;
  }




  /*
   * get the square of the Euclidean distance of the given point to
   * the node's hyperbox.
   *
   * If the point lies withing the hyperbox, the distance will be zero.
   *
   * @param indexPoint the point to be compared with the hyperbox.
   * @param na hyperbox representing the node's covered subspace.
   *           The first row represents the minimum and the second the
   *           maximum values.  It must be na.columns()==indexPoint.size()
   */
  template<class T,class D,class U>
  inline typename kdTree<T,D,U>::acc_type
  kdTree<T,D,U>::minDistancePointToBox(const T& indexPoint,
                                       const matrix<value_type>& na) const {
    
    acc_type dist(0);
    value_type tmp;

    const vector<value_type>& boxMin = na.getRow(0);
    const vector<value_type>& boxMax = na.getRow(1);
    
    for (int i = indexPoint.size()-1; i >= 0; --i) {
      if ((tmp=indexPoint[i]) > boxMax.at(i)) {
        distantor.accumulate(tmp,boxMax.at(i),dist);
      } else if (tmp < boxMin.at(i)) {
        distantor.accumulate(tmp,boxMin.at(i),dist);
      }
    }
    
    return distantor.computeDistance(dist);
  }
  

  /*
   * search the best-bin-first algorithm of Beis and Lowe.  It search
   * for the approximate k nearest neighbors.
   * @param k number of elements you are looking for.
   * @param key the point in the n-dimensional space you are looking for.
   * @param neighbors contains the pointers to the found elements.  You
   *                  should NEVER delete these elements.
   * @return true if k elements were found, or false, if the tree contains
   *         less than k elements
   */
  template<class T,class D,class U>
  bool 
  kdTree<T,D,U>::searchBestBinFirst(const node* root,
                                  const int k,
                                  const T& key,
                                  std::multimap<acc_type,element*>& neighbors,
                                  const int emax
                                  ) const {

    // priority queue
    typedef
      std::multimap<acc_type,std::pair< node*,matrix<value_type> > > qtype;
    qtype pqueue;
    typename qtype::iterator qit;

    matrix<value_type> bounds(totalBounds);

    const int maxVisits = min(emax,numLeaves);// ensure termination
    int nodeVisits(0);                        // number of leaf nodes visited
    int elems(0);                             // number of elements inserted in
                                              // neighbors
    const node* nptr = root;

    while ( notNull(nptr) && ((elems<k) || (nodeVisits<maxVisits)) ) {
      // if this is a leaf, elements should be inserted in neighbors...
      if (nptr->isLeaf()) {
        // examine records in bucket, and update the neighbors map if necessary
        typename node::points_type::const_iterator it,eit;
        acc_type d;
        
        for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
          // if neighbors have too few elements or if the farest
          // nearest element have a greater distance than the actual
          // element then insert the elements in the map
          if ( (neighbors.rbegin())->first > 
               (d=distantor(key,(*it)->point)) ) {
            
            // delete the last one
            typename std::multimap<acc_type,element*>::iterator mit;
            mit = neighbors.end();
            --mit;
#           if defined(_LTI_DEBUG) && (_LTI_DEBUG > 3)
            if (notNull((*mit).second)) {
              _lti_debug3("Deleting " << ((*mit).second)->point << std::endl);
            } else {
              _lti_debug3("Deleting dummy entry" << std::endl);
            }
#           endif
            neighbors.erase(mit);
            
            _lti_debug3("Inserting " << (*it)->point << std::endl);
            // insert one element
            neighbors.insert(std::make_pair(d,(*it)));
            ++elems;
          }
        }
        
        nodeVisits++;

        // check "hypersphere within bounds"
        if (checkHypersphereWithinBounds(key,
                                         bounds,
                                         (neighbors.rbegin())->first)) {
          return true;
        }

        // get the next node to be analyzed from the priority queue
        if (pqueue.empty()) {
          nptr=0;
        } else {
          qit = pqueue.begin();
          (*qit).second.second.detach(bounds);
          nptr = (*qit).second.first;
          pqueue.erase(qit);
        }

      } // end if leaf
      else {
        value_type tmp;
        const int& d = (nptr->splitDim);
        const value_type& p = (nptr->partition);
        value_type& boxMin = bounds.at(0,d);
        value_type& boxMax = bounds.at(1,d);
        
        // ------------
        //  left child
        // ------------
        
        if (key[d] <= p) {
          // insert in the priority queue the distance to the
          // farest node
          tmp=boxMin;
          boxMin=p;
          pqueue.insert(std::make_pair(minDistancePointToBox(key,bounds),
                                       std::make_pair(nptr->right,bounds)));
          
          boxMin=tmp;
          
          // call on closer son
          boxMax=p;
          nptr = nptr->left;
        } else {
          
          // ------------
          //  right child
          // ------------
          
          // insert in the priority queue the distance to the
          // farest node
          tmp=boxMax;
          boxMax=p;
          pqueue.insert(std::make_pair(minDistancePointToBox(key,bounds),
                                       std::make_pair(nptr->left,bounds)));
          
          boxMax=tmp;
          
          // call on closer son
          boxMin=p;
          nptr = nptr->right;
        }
      }
        
    } // end of while
    
    return true;
  }

  // ----------------------------------------
  // Search range
  // ----------------------------------------
  

  template<class T,class D,class U>
  inline bool kdTree<T,D,U>::withinBox(const T& boxMin,
                                     const T& boxMax,
                                     const T& key) const {
    const int size = key.size();
    int i;
    for (i=0;i<size;++i) {
      const typename T::value_type k=key[i];
      if ( (k<boxMin[i]) || (k>boxMax[i]) ) {
        return false;
      }
    }
    return true;
  }

  template<class T,class D,class U>
  inline bool kdTree<T,D,U>::withinBox(const matrix<value_type>& bbox,
                                     const T& boxMin,
                                     const T& boxMax) const {
    const int size = bbox.columns();
    int i;
    const vector<value_type>& low  = bbox.getRow(0);
    const vector<value_type>& high = bbox.getRow(1);
    for (i=0;i<size;++i) {
      if ( (boxMin[i]<low.at(i)) || (boxMax[i]>high.at(i)) ) {
        return false;
      }
    }
    return true;
  }

  /*
   * Search for all points lying within the given hyperbox.
   * @param nptr pointer to the node root of the subtree to be evaluated
   * @param bbox matrix specifying the hyperbox.  Its first row specify the
   *             minimum values at each dimension of the box, and the second
   *             row the maximum values.  The number of columns of the matrix
   *             must be equal as the number of dimensions of the points 
   *             stored in the kd-Tree.
   * @param neighbors list of pointer to the elements found until now.
   * @return true if any element was found.
   */
  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchRange(const T& boxMin,
                                const T& boxMax,
                                std::list<element*>& neighbors) const {

    _lti_debug3("Search In Range: from " << boxMin << 
                " to " << boxMax << std::endl);

    neighbors.clear();

    // if this is a leaf, elements should be inserted in neighbors...
    if (notNull(root)) {
      // the fastest contents copy between the two matrices is required here!
      // without resize (to avoid new memory allocation).
      // use pointer arithmetic.
      // is is equivalent to boundsBox.fill(totalBounds), but avoiding
      // many (here) unnecessary boundary checks.
      const value_type *pt,*pe;
      value_type *p;
      for (pt=&totalBounds.at(0,0),pe=&totalBounds.at(1,boundsMax.lastIdx()),
             p=&boundsMin.at(0);pt<=pe;++pt,++p) {
        (*p)=(*pt);
      }

      searchRange(root,boxMin,boxMax,neighbors);
    }

    return !neighbors.empty();
  }

  template<class T,class D,class U>
  bool kdTree<T,D,U>::searchRange(const node* nptr,
                                const T& boxMin,
                                const T& boxMax,
                                std::list<element*>& neighbors) const {

    if (nptr->isLeaf()) {
      typename node::points_type::const_iterator it,eit;

      _lti_debug3("Leaf node bounds:\n" << boundsBox << endl);

      // examine records in bucket, and update the neighbors map if necessary
      for (it=nptr->points.begin(),eit=nptr->points.end();it!=eit;++it) {
        if (withinBox(boxMin,boxMax,(*it)->point)) {
          _lti_debug3("Inserting (once) " << (*it)->point << endl);
          neighbors.push_back(*it);
        }
      }
      return withinBox(boundsBox,boxMin,boxMax);  // false means, still need to
                                                  // search somewhere else
      
    } // end is leaf.
    
    // boundsBox is a matrix containing both vectors boundsMax and boundsMin
    value_type tmp;
    const int& d = nptr->splitDim;
    const value_type& p = nptr->partition;

    // save some time avoiding too many accesses to boundsMax/boundsMin
    value_type& nodeBoxMax = boundsMax.at(d);
    value_type& nodeBoxMin = boundsMin.at(d);

    // ------------
    //  left child
    // ------------
  
    if (boxMin[d] <= p) {
      tmp=nodeBoxMax;
      nodeBoxMax=p;
      _lti_debug3("Searching left child" << std::endl);
      if (searchRange(nptr->left,boxMin,boxMax,neighbors)) {
        _lti_debug3("done." << std::endl);
        nodeBoxMax=tmp;
        return true;
      }
      nodeBoxMax=tmp;
    }

    // ------------
    //  right child
    // ------------

    if (boxMax[d] >= p) {
      tmp=nodeBoxMin;
      nodeBoxMin=p;
      _lti_debug3("Searching right child" << std::endl);
      if (searchRange(nptr->right,boxMin,boxMax,neighbors)) {
        _lti_debug3("done." << std::endl);
        nodeBoxMin=tmp;
        return true;        
      }
      nodeBoxMin=tmp;
    }

    return withinBox(boundsBox,boxMin,boxMax);
  }

}

#include "ltiUndebug.h"
