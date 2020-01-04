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
 * file .......: ltiAdjacencyGraph_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.07.2003
 * revisions ..: $Id: ltiAdjacencyGraph_template.h,v 1.7 2005/06/16 12:37:34 doerfler Exp $
 */

#undef _LTI_DEBUG
// #define _LTI_DEBUG 2

#include "ltiDebug.h"

namespace lti {

  // ----------------------------------------------------------------------
  //
  // Adjacency graph
  //
  // ----------------------------------------------------------------------

  // ---------------
  // constructors
  // ---------------

  template<class N,class W,class D,class F,class E>
  adjacencyGraph<N,W,D,F,E>::adjacencyGraph() 
    : distances(E::Invalid),firstValidNodeIndex(0),
      lastValidNodeIndex(-1),freeElements(0) {
  }
  
  template<class N,class W,class D,class F,class E>
  adjacencyGraph<N,W,D,F,E>::adjacencyGraph(const int number,
					    const node_type& nodeData)
    : distances(E::Invalid),firstValidNodeIndex(0),
      lastValidNodeIndex(-1),freeElements(0) {
    resize(number,nodeData);
  }

  template<class N,class W,class D,class F,class E>
  adjacencyGraph<N,W,D,F,E>::adjacencyGraph(const adjacencyGraph<N,W,D,F,E>& other)
    : distances(E::Invalid),firstValidNodeIndex(0),
      lastValidNodeIndex(-1),freeElements(0) {
    copy(other);
  }

  // ---------------
  // iterators
  // ---------------
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::const_iterator 
  adjacencyGraph<N,W,D,F,E>::begin() const {
    return const_iterator(firstValidNodeIndex,&nodes);
  }
  
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::iterator 
  adjacencyGraph<N,W,D,F,E>::begin() {
    return iterator(firstValidNodeIndex,&nodes);
  }

  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::const_iterator 
  adjacencyGraph<N,W,D,F,E>::end() const {
    return const_iterator(lastValidNodeIndex+1,&nodes);   
  }
  
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::iterator
  adjacencyGraph<N,W,D,F,E>::end() {
    return iterator(lastValidNodeIndex+1,&nodes);   
  }

  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::edge_iterator 
  adjacencyGraph<N,W,D,F,E>::begin(const id_type startNode) {
    return edge_iterator(adjacency[startNode].begin(),startNode);
  }

  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::const_edge_iterator 
  adjacencyGraph<N,W,D,F,E>::begin(const id_type startNode) const {
    return const_edge_iterator(adjacency[startNode].begin(),startNode);
  }

  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::edge_iterator 
  adjacencyGraph<N,W,D,F,E>::end(const id_type startNode) {
    return edge_iterator(adjacency[startNode].end(),startNode);
  }

  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::const_edge_iterator 
  adjacencyGraph<N,W,D,F,E>::end(const id_type startNode) const {
    return const_edge_iterator(adjacency[startNode].end(),startNode);
  }

  // ---------------
  // node operations
  // ---------------
  // isNodeIdValid()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::isNodeIdValid(const id_type id) const {
    return ((id >= firstValidNodeIndex) &&
            (id <= lastValidNodeIndex) &&
            (nodes[id].first >= 0)); // the first element is an
                                     // integer indicating how many
                                     // edges has a node or negative
                                     // if it has been deleted.
  }

  // getNodeData()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::node_type& 
  adjacencyGraph<N,W,D,F,E>::getNodeData(const id_type id) const {
    assert(isNodeIdValid(id));
    return nodes[id].second;
  }

  // getNodeData()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::node_type& 
  adjacencyGraph<N,W,D,F,E>::getNodeData(const id_type id) {
    assert(isNodeIdValid(id));
    return nodes[id].second;
  }

  // getNodeData()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::node_type& 
  adjacencyGraph<N,W,D,F,E>::setNodeData(const id_type id,const node_type& data) {
    return (getNodeData(id)=data);
  }

  // insertNode()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type 
  adjacencyGraph<N,W,D,F,E>::insertNode(const node_type& data) {
    if (freeElements == 0) {
      // we need to resize the vector
      // the new element is the last in the list
      lastValidNodeIndex++;
      nodes.push_back(std::make_pair(0,data)); // the node data vector
      adjacency.push_back(row_type());         // the adjacent nodes list
      return lastValidNodeIndex;
    }
    
    if (firstValidNodeIndex > lastValidNodeIndex) {
      // the vector is empty use just the first element
      firstValidNodeIndex = 0;
      lastValidNodeIndex = 0;
      freeElements--;
      nodes[lastValidNodeIndex]=std::make_pair(0,data);
      return lastValidNodeIndex;
    }

    if (firstValidNodeIndex > 0) {
      // we can try at the begin of the vector
      firstValidNodeIndex--;
      nodes[firstValidNodeIndex].first=0;
      nodes[firstValidNodeIndex].second=data;
      freeElements--;
      return firstValidNodeIndex;
    }

    if (lastValidNodeIndex < static_cast<int>(nodes.size())-1) {
      // still place to reuse at the vectors' end
      lastValidNodeIndex++;
      nodes[lastValidNodeIndex].first=0;
      nodes[lastValidNodeIndex].second=data;
      freeElements--;
      return lastValidNodeIndex;
    }
    
    // no other choice: we need to search for the first free element
    int i;
    i=firstValidNodeIndex;
    while ((i<=lastValidNodeIndex) && (nodes[i].first >= 0)) {
      ++i;
    }

    // this should always be true, but better to be cautious.
    assert(i<=lastValidNodeIndex);

    nodes[i].first=0;
    nodes[i].second=data;
    freeElements--;

    return i;
  }

  // insertNodes()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertNodes(const int number,
                                        const node_type& nodeData) {
    if (freeElements == 0) {
      // no place left, so let's insert the new data in the vector
      adjacency.resize(nodes.size()+number,row_type());
      nodes.resize(nodes.size()+number,std::make_pair(0,nodeData));
      lastValidNodeIndex = static_cast<int>(nodes.size())-1;
      return true;
    }

    int n = number;
    int i,j,k;

    if (firstValidNodeIndex > lastValidNodeIndex) {
      // the vector is empty use just the first elements
      k=min(static_cast<int>(nodes.size()),n);
      n-=k;
      for (i=0;i<k;++i) {
        nodes[i] = std::make_pair(0,nodeData);
      }
      
      firstValidNodeIndex = 0;
      lastValidNodeIndex = i;
      freeElements-=k;
    }

    // now let's try to fill the elements at the beginning
    k = min(firstValidNodeIndex,n);
    n -= k; // how many elements left..
    if (firstValidNodeIndex > 0) {
      for (i=firstValidNodeIndex-1,j=k;j>0;--j,--i) {
        // we can try at the begin of the vector
        nodes[i]=std::make_pair(0,nodeData);
      }
      firstValidNodeIndex-=k;
      freeElements-=k;
    }

    if (n>0) {
      // still something to initialize...
      k = min(freeElements,n);
      n-=k;
      if (lastValidNodeIndex < static_cast<int>(nodes.size())-1) {
        for (i=lastValidNodeIndex+1,j=k;j>0;--j,++i) {
          // we can try at the begin of the vector
          nodes[i]=std::make_pair(0,nodeData);
        }
        lastValidNodeIndex+=k;
        freeElements-=k;        
      }

      if (n>0) {
        // still something there... we need to resize!
        // no place left, so let's insert the new data in the vector
        adjacency.resize(nodes.size()+n,row_type());
        nodes.resize(nodes.size()+n,std::make_pair(0,nodeData));
        lastValidNodeIndex = static_cast<int>(nodes.size())-1;
        freeElements = 0;
      }
    }
    
    return true;
  }

  // removeNode
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::removeNode(const id_type id) {
    if (isNodeIdValid(id)) {
      edge_iterator it,eit;

      it = begin(id);
      eit = end(id);

      // remove all outgoing edges
      while(it!=eit) {
        removeEdge(it++);
      }
      nodes[id].first=-1;
      freeElements++;
      if (id == lastValidNodeIndex) {
        --lastValidNodeIndex;
      } else if (id == firstValidNodeIndex) {
        ++firstValidNodeIndex;
      }
      return true;
    }
 
    return false;
  }

  // mergeNodes()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type
  adjacencyGraph<N,W,D,F,E>::mergeNodes(const node_pair& edge) {
    return mergeNodes(edge.first,edge.second);
  }

  // mergeNodes()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type
  adjacencyGraph<N,W,D,F,E>::mergeNodes(const id_type first,
                                  const id_type second) {

    if (first == second) {
      // nothing to be done: nodes are already the same one!
      return first;
    }

    id_type l1,l2,l;
    typename row_type::iterator mit,tit,ait,bit;
    
    // keep always the smallest node id, to increase density at the lower
    // part of the matrix (save storage space)
    minmax(first,second,l2,l1);
 
    // merge both nodes, assigning to the one with the smaller index the
    // elements of the one with the higher index and updating all statistical
    // data.
    // it can occur that after some merge, already detected equivalences
    // conduce to l1==l2, in which case there is nothing to be done.
    _lti_debug2("merging " << l1 << " and " << l2 << std::endl);

    // now we need to remove the n2 node and shift all to the node n1
    getNodeData(l2) += getNodeData(l1);
    
    // get the rows in question
    row_type& row = adjacency[l2];
    row_type& oldRow = adjacency[l1];

    // -----------------------------------------------
    // remove the entries of the l1-l2 and l2-l1 edges
    // -----------------------------------------------
    if ((tit=row.find(l1)) != row.end()) {
      // edge found!
      
      // get iterator to the second complementary edge
      mit=(*tit).second.complement; 

      // remove l2->l1
      typename queue_type::index_type qelem = (*tit).second.index;
      if (distances.valid(qelem)) {
        distances.erase(qelem);
      }
      row.erase(tit);

      // remove l1->l2
      qelem = (*mit).second.index;
      if (distances.valid(qelem)) {
        distances.erase(qelem);
      }
      oldRow.erase(mit);
    }
    
    // ----------------------------------------------
    // move the neighbors of l1 to the lower label l2
    // ----------------------------------------------
    for (mit=oldRow.begin(); mit!=oldRow.end(); ++mit) {

      // get the node of the current entry
      l = (*mit).first;
      bit = (*mit).second.complement; // iterator to l->l1

      // find if label is used twice (in l1 and l2)
      if ((tit=row.find(l)) != row.end()) {
        // yes, there are two edges that need to be merged

        // ---
        // First case: remove l1->l edge

        // first merge the edge data from (l1+l2) -> l
        (*tit).second.data += (*mit).second.data;

        // now remove the edge from the queue
        if (distances.valid((*mit).second.index)) {
          distances.erase((*mit).second.index);
        }

        // delay the removal of (*mit) to avoid invalidating the iterator mit

        // ---
        // Second case: remove l->l1 edge

        row_type& lrow = adjacency[l];
        ait = (*tit).second.complement;

        nodes[l].first--; // one edge less!

        // compute the new edge info of l->(l1+l2) 
        (*ait).second.data += (*bit).second.data;
        
        if (distances.valid((*bit).second.index)) {
          // remove edge from queue
          distances.erase((*bit).second.index);
        }

        // remove the matrix node
        lrow.erase(bit);
        
      } else {
        // label l is only used in l1 => just move it

        // ---
        // First case: move l1->l to l2->l

        std::pair<typename row_type::iterator,bool> f;
        f = row.insert(std::make_pair(l,(*mit).second));

        assert(f.second); // this should always be true, otherwise, we weren't
                          // here!
        
        tit = f.first;

        nodes[l2].first++; // one edge added!

        // delay the removal of (*mit) to avoid invalidating the iterator mit

        // ---
        // Second case: remove l->l1 edge

        row_type& lrow = adjacency[l];
        f = lrow.insert(std::make_pair(l2,(*bit).second));

        assert(f.second); // this should always be true, otherwise, somewhere
                          // became the graph inconsistent!
        ait=f.first;

        // 
        if (edge_traits::Symmetric) {
          if ((l2<l) && (l<l1)) {
            // for the symmetrical case, if the label l is between l2 and l1
            // we can better use the queue entry if we switch it!
            typename queue_type::index_type tmp;
            tmp = (*tit).second.index;
            (*tit).second.index = (*ait).second.index;
            (*ait).second.index = tmp;
          }
        }

        // update the iterators
        (*ait).second.complement = tit;
        (*tit).second.complement = ait;

        // remove the matrix node
        lrow.erase(bit);
      }  
    }

    // update the weights of all outgoing and incomming edges of
    // the new merged node
    weight_type w;

    for (tit=row.begin();tit!=row.end();++tit) {
      l = (*tit).first;

      if (l < l2) {
        // first the outgoing edge
        w = theWeightFunctor(getNodeData(l2),
                             getNodeData(l),
                             (*tit).second.data);

        if (distances.valid((*tit).second.index)) {
          // valid queue index
          distances.update((*tit).second.index,w,node_pair(l2,l));
        } else {
          (*tit).second.index = distances.insert(w,node_pair(l2,l));
        }

        // now the incomming edge
        mit = (*tit).second.complement; // the corresponding incoming edge
     
        if (edge_traits::Symmetric) {
          if (distances.valid((*mit).second.index)) {          
            distances.erase((*tit).second.index);
          }
        } else {

          w = theWeightFunctor(getNodeData(l),
                               getNodeData(l2),
                               (*mit).second.data);
          
          if (distances.valid((*mit).second.index)) {
            // valid queue index
            distances.update((*mit).second.index,w,node_pair(l,l2));
          } else {
            (*tit).second.index = distances.insert(w,node_pair(l,l2));
          }

        }
        
      } else {

        if (edge_traits::Symmetric) {
          // on this side of the matrix we don't need anything, since the
          // matrix is symmetric
          if (distances.valid((*tit).second.index)) {          
            distances.erase((*tit).second.index);
          }
        } else {          
          // first the outgoing edge
          w = theWeightFunctor(getNodeData(l2),
                               getNodeData(l),
                               (*tit).second.data);
          
          if (distances.valid((*tit).second.index)) {
            // valid queue index
            distances.update((*tit).second.index,w,node_pair(l2,l));
          } else {
            (*tit).second.index = distances.insert(w,node_pair(l2,l));
          }
        }

        // now incoming edge
        mit = (*tit).second.complement; // the corresponding incoming edge

        w = theWeightFunctor(getNodeData(l),
                             getNodeData(l2),
                             (*mit).second.data);
          
        if (distances.valid((*mit).second.index)) {
          // valid queue index
          distances.update((*mit).second.index,w,node_pair(l,l2));
        } else {
          (*mit).second.index = distances.insert(w,node_pair(l,l2));
        }
      
      }
    }
    
    // remove the oldRow
    oldRow.clear();

    // remove the node
    nodes[l1].first = -1; // flag to indicate removed node!

    // we're ready
    return l2;
  }

  // topologicalMerge();
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type
  adjacencyGraph<N,W,D,F,E>::topologicalMerge(const node_pair& edge) {
    return topologicalMerge(edge.first,edge.second);
  }

  // topologicalMerge();
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type
  adjacencyGraph<N,W,D,F,E>::topologicalMerge(const id_type first,
                                              const id_type second) {
    
    if (first == second) {
      // nothing to be done: nodes are already the same one!
      return first;
    }

    int l1,l2,l;
    typename row_type::iterator mit,tit,ait,bit;

    // keep always the smallest node id, to increase density at the lower
    // part of the matrix (save storage space)
    minmax(first,second,l2,l1);
 
    // merge both nodes, assigning to the one with the smaller index the
    // elements of the one with the higher index and updating all statistical
    // data.
    // it can occur that after some merge, already detected equivalences
    // conduce to l1==l2, in which case there is nothing to be done.
    _lti_debug2("merging " << l1 << " and " << l2 << std::endl);
   
    // get the rows in question
    row_type& row = adjacency[l2];
    row_type& oldRow = adjacency[l1];

    // -----------------------------------------------
    // remove the entries of the l1-l2 and l2-l1 edges
    // -----------------------------------------------
    if ((tit=row.find(l1)) != row.end()) {
      // edge found!
      
      // get iterator to the second complementary edge
      mit=(*tit).second.complement; 

      // remove l2->l1
      typename queue_type::index_type qelem = (*tit).second.index;
      if (distances.valid(qelem)) {
        distances.erase(qelem);
      }
      row.erase(tit);

      // remove l1->l2
      qelem = (*mit).second.index;
      if (distances.valid(qelem)) {
        distances.erase(qelem);
      }
      oldRow.erase(mit);
    } 
        
    // ----------------------------------------------
    // move the neighbors of l1 to the lower label l2
    // ----------------------------------------------
    for (mit=oldRow.begin(); mit!=oldRow.end(); ++mit) {

      // get the node of the current entry
      l = (*mit).first;
      bit = (*mit).second.complement; // iterator to l->l1

      // find if label is used twice (in l1 and l2)
      if ((tit=row.find(l)) != row.end()) {
        // yes, there are two edges that need to be merged

        // ---
        // First case: remove l1->l edge

        // now remove the edge from the queue
        if (distances.valid((*mit).second.index)) {
          distances.erase((*mit).second.index);
        }

        // ---
        // Second case: remove l->l1 edge

        row_type& lrow = adjacency[l];
        ait = (*tit).second.complement;

        nodes[l].first--; // one edge less!

        // remove edge from queue
        if (distances.valid((*bit).second.index)) {
          // remove edge from queue
          distances.erase((*bit).second.index);
        }
        
        // remove the matrix node
        lrow.erase(bit);
                
      } else {
        // label l is only used in l1 => just move it

        // ---
        // First case: move l1->l to l2->l

        std::pair<typename row_type::iterator,bool> f;
        f = row.insert(std::make_pair(l,(*mit).second));

        assert(f.second); // this should always be true, otherwise, we weren't
                          // here!
        
        tit = f.first;

        nodes[l2].first++; // one edge added!

        // delay the removal of (*mit) to avoid invalidating the iterator mit

        // ---
        // Second case: remove l->l1 edge

        row_type& lrow = adjacency[l];
        f = lrow.insert(std::make_pair(l2,(*bit).second));

        assert(f.second); // this should always be true, otherwise, somewhere
                          // became the graph inconsistent!
        ait=f.first;

        // 
        if (edge_traits::Symmetric) {
          if ((l2<l) && (l<l1)) {
            // for the symmetrical case, if the label l is between l2 and l1
            // we can better use the queue entry if we switch it!
            typename queue_type::index_type tmp;
            tmp = (*tit).second.index;
            (*tit).second.index = (*ait).second.index;
            (*ait).second.index = tmp;
          }
        }

        // update the iterators
        (*ait).second.complement = tit;
        (*tit).second.complement = ait;

        // remove the matrix node
        lrow.erase(bit);
      }
    }
    
    // update the data of all outgoing and incomming edges of
    // the new merged node
    for (tit=row.begin();tit!=row.end();++tit) {
      l = (*tit).first;

      if (distances.valid((*tit).second.index)) {
        // valid queue index
        distances.setData((*tit).second.index,node_pair(l2,l));
      } 

      // now the incomming edge
      mit = (*tit).second.complement; // the corresponding incoming edge
      
      if (distances.valid((*mit).second.index)) {
        // valid queue index
        distances.setData((*mit).second.index,node_pair(l,l2));
      }
    }

    // remove the oldRow
    oldRow.clear();

    // remove the node
    nodes[l1].first = -1; // flag to indicate removed node!

    // we're ready
    return l2;
  }

  // numberEdges() 
  template<class N,class W,class D,class F,class E>
  int adjacencyGraph<N,W,D,F,E>::numberEdges(const id_type node) const {
    assert(isNodeIdValid(node));
    return nodes[node].first;
  }

  // resize
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::resize(const int number,
                                   const node_type& nodeData) {
    clear();
    return insertNodes(number,nodeData);
  }

  // getEdgeWeight()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::weight_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeWeight(const id_type a,const id_type b) const {

    if (isNodeIdValid(a) && isNodeIdValid(b)) {
      const row_type& row = adjacency[a];
      typename row_type::const_iterator it;
   
      if ( ((it=row.find(b)) != row.end()) && 
           (distances.valid((*it).second.index)) ) {
        return distances.getKey((*it).second.index);
      }
    }

    return edge_traits::Invalid;
  }

  template<class N,class W,class D,class F,class E>
  int adjacencyGraph<N,W,D,F,E>::size() const {
    int i,s(0);
    for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
      if (nodes[i].first >= 0) {
        s++;
      }
    }
    return s;
  }

  template<class N,class W,class D,class F,class E>
  int adjacencyGraph<N,W,D,F,E>::totalAdjacentNodes() const {
    int i,s(0);
    for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
      if (nodes[i].first > 0) {
        s++;
      }
    }
    return s;
  }

  template<class N,class W,class D,class F,class E>
  int adjacencyGraph<N,W,D,F,E>::totalEdges() const {
    int i,s(0);
    for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
      if (nodes[i].first > 0) {
        s+=nodes[i].first;
      }
    }
    return s;
  }


  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type 
  adjacencyGraph<N,W,D,F,E>::lastValidId() const {
    return ((lastValidNodeIndex >= firstValidNodeIndex) ? 
            lastValidNodeIndex : -1);
  }

  // getEdgeWeight()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::weight_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeWeight(const node_pair& edge) const {
    return getEdgeWeight(edge.first,edge.second);
  }

  // getEdgeWeight()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::weight_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeWeight(const edge_iterator& it) const {
    if (distances.valid(it.getEntry().index)) {
      return distances.getKey(it.getEntry().index);
    }
    return edge_traits::Invalid;
  }

  // getEdgeWeight()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::weight_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeWeight(const const_edge_iterator& it) const {  
    if (distances.valid(it.getEntry().index)) {
      return distances.getKey(it.getEntry().index);
    }
    return edge_traits::Invalid;
  }

  // updateEdgeWeight()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::updateEdgeWeight(const id_type a,
                                             const id_type b,
                                             const weight_type& weight) {

    if (isNodeIdValid(a) && isNodeIdValid(b)) {
      // update a->b
      row_type& row = adjacency[a];
      typename row_type::iterator it;
      if ((it = row.find(b)) != row.end()) {
        if (distances.valid((*it).second.index)) {
          // valid queue index
          distances.update((*it).second.index,weight);
        } else {
          (*it).second.index=distances.insert(weight,node_pair(a,b));
        }
        
        if (edge_traits::Symmetric) {
          it = (*it).second.complement;
          if (distances.valid((*it).second.index)) {
            distances.update((*it).second.index,weight);
          } else {
            (*it).second.index = distances.insert(weight,node_pair(b,a));
          }
        }
          
        return true;
      }
    }

    return false;
  }

  // updateEdgeWeight()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::updateEdgeWeight(const node_pair& a,
                                             const weight_type& weight) {
    return updateEdgeWeight(a.first,a.second,weight);
  }

  // updateEdgeWeight()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::updateEdgeWeight(const edge_iterator& it,
                                             const weight_type& weight) {
    
    if (distances.valid(it.getEntry().index)) {
      distances.update(it.getEntry().index,weight);
    } else {
      it.getRWEntry().index = distances.insert(weight,getEdge(it));
    }
    if (edge_traits::Symmetric) {
      if (distances.valid((*(it.getEntry().complement)).second.index)) {
        distances.update((*(it.getEntry().complement)).second.index,weight);
      } else {
        node_pair p = getEdge(it);
        (*(it.getEntry().complement)).second.index=
          distances.insert(weight,node_pair(p.second,p.first));
      }
    }
    return true;
  }


  // updateEdgeWeight()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::updateEdgeWeight(const id_type a,
                                             const id_type b) {

    if (isNodeIdValid(a) && isNodeIdValid(b)) {
      // update a->b
      row_type& row = adjacency[a];
      typename row_type::iterator it;
      if ((it = row.find(b)) != row.end()) {

        const weight_type weight = theWeightFunctor(nodes[a].second,
                                                     nodes[b].second,
                                                     (*it).second.data);

        if (distances.valid((*it).second.index)) {
          // valid queue index
          distances.update((*it).second.index,weight);
        } else {
          (*it).second.index=distances.insert(weight,node_pair(a,b));
        }
        
        if (edge_traits::Symmetric) {
          it = (*it).second.complement;
          if (distances.valid((*it).second.index)) {
            distances.update((*it).second.index,weight);
          } else {
            (*it).second.index = distances.insert(weight,node_pair(b,a));
          }
        }
          
        return true;
      }
    }

    return false;
  }

  // updateEdgeWeight()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::updateEdgeWeight(const node_pair& a) {
    return updateEdgeWeight(a.first,a.second);
  }

  // updateEdgeWeight()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::updateEdgeWeight(const edge_iterator& it) {

    const node_pair edge = getEdge(it);
    const weight_type weight=theWeightFunctor(nodes[edge.first].second,
                                               nodes[edge.second].second,
                                               *it);
    
    if (distances.valid(it.getEntry().index)) {
      distances.update(it.getEntry().index,weight);
    } else {
      it.getRWEntry().index = distances.insert(weight,getEdge(it));
    }
    if (edge_traits::Symmetric) {
      if (distances.valid((*(it.getEntry().complement)).second.index)) {
        distances.update((*(it.getEntry().complement)).second.index,weight);
      } else {
        (*(it.getEntry().complement)).second.index=
          distances.insert(weight,node_pair(edge.second,edge.first));
      }
    }
    return true;
  }

  // computeEdgeWeight()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::weight_type
  adjacencyGraph<N,W,D,F,E>::computeEdgeWeight(const id_type a,
                                         const id_type b) const {
    return theWeightFunctor(nodes[a].second,nodes[b].second,
                             getEdgeData(a,b));
  }

  // computeEdgeWeight()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::weight_type
  adjacencyGraph<N,W,D,F,E>::computeEdgeWeight(const node_pair& edge) const {
    return theWeightFunctor(nodes[edge.first].second,
                             nodes[edge.second].second,
                             getEdgeData(edge));
  }

  // computeEdgeWeight()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::weight_type
  adjacencyGraph<N,W,D,F,E>::computeEdgeWeight(const edge_iterator& it) const {
    const node_pair p = getEdge(it);
    return theWeightFunctor(nodes[p.first].second,nodes[p.second].second,
                             *it);
  }

  // computeEdgeWeight()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::weight_type
  adjacencyGraph<N,W,D,F,E>::computeEdgeWeight(const const_edge_iterator& it) const {
    const node_pair p = getEdge(it);
    return theWeightFunctor(nodes[p.first].second,nodes[p.second].second,
                             *it);
  }

  // setEdgeData()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::setEdgeData(const id_type a,
                                        const id_type b,
                                        const edge_data_type& data) {

    if (isNodeIdValid(a) && isNodeIdValid(b)) {
      // update a->b
      row_type& row = adjacency[a];
      typename row_type::iterator it;
      if ((it=row.find(b)) != row.end()) {
        (*it).second.data = data;

        if (edge_traits::Symmetric) {
          it = (*it).second.complement;
          (*it).second.data = data;
        }

        return true;
      } 
    }

    // edge not found?
    return false;
  }

  // setEdgeData()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::setEdgeData(const node_pair& a,
                                        const edge_data_type& data) {
    return setEdgeData(a.first,a.second,data);
  }

  // setEdgeData()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::setEdgeData(const edge_iterator& it,
                                        const edge_data_type& data) {
    it.getRWEntry().data = data;
    if (edge_traits::Symmetric) {
      (*(it.getEntry().complement)).second.data = data;
   }
    return true;
  }



  // getEdgeData()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::edge_data_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeData(const id_type a,
                                   const id_type b) const {

    if (isNodeIdValid(a) && isNodeIdValid(b)) {
      // update a->b
      const row_type& row = adjacency[a];
      typename row_type::const_iterator it;
      if ((it=row.find(b)) != row.end()) {
        return (*it).second.data;
      } 
    }

    // edge not found!
    assert(false); // in debug modus produce an assertion!
    static const edge_data_type dummy = edge_data_type();
    return dummy;
  }

  // getEdgeData()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::edge_data_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeData(const node_pair& a) const {
    return getEdgeData(a.first,a.second);
  }

  // getEdgeData()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::edge_data_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeData(const id_type a,
                                   const id_type b) {

    if (isNodeIdValid(a) && isNodeIdValid(b)) {
      // update a->b
      row_type& row = adjacency[a];
      typename row_type::iterator it;
      if ((it=row.find(b)) != row.end()) {
        return (*it).second.data;
      } 
    }

    // edge not found!
    assert(false); // in debug modus produce an assertion!
    static edge_data_type dummy;
    dummy = edge_data_type();
    return dummy;
  }

  // getEdgeData()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::edge_data_type& 
  adjacencyGraph<N,W,D,F,E>::getEdgeData(const node_pair& a) {
    return getEdgeData(a.first,a.second);
  }

  // getLowestWeightEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::getLowestWeightEdge(id_type& a,
                                                id_type& b,
                                                weight_type& weight
                                                ) const {
    if (!distances.empty()) {
      weight = distances.front().first;
      a = distances.front().second.first;
      b = distances.front().second.second;
      return true;
    }
    return false;
  }

  // getLowestWeightEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::getLowestWeightEdge(node_pair& edge,
                                                weight_type& weight
                                                ) const {
    if (!distances.empty()) {
      weight = distances.front().first;
      edge = distances.front().second;
      return true;
    }
    return false;
  }

  // getNodeId()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type 
  adjacencyGraph<N,W,D,F,E>::getNodeId(const typename entry_type::entry_iterator& it) const {
    return (*((*it).second.complement)).first;
  }

  // getOtherNode()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type 
  adjacencyGraph<N,W,D,F,E>::getOtherNode(const edge_iterator& it) const {
    return getNodeId(it.getEntry().complement);
  }
    
  // getOtherNode()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::id_type 
  adjacencyGraph<N,W,D,F,E>::getOtherNode(const const_edge_iterator& it) const {
    return getNodeId(it.getEntry().complement);
  }
  
  // getEdge()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::node_pair
  adjacencyGraph<N,W,D,F,E>::getEdge(const edge_iterator& it) const {
    return node_pair(it.getNode(),getNodeId(it.getEntry().complement));
  }

  // getEdge()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::node_pair 
  adjacencyGraph<N,W,D,F,E>::getEdge(const const_edge_iterator& it) const {
    return node_pair(it.getNode(),getNodeId(it.getEntry().complement));
  }

  // insertEdge
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertEdge(const id_type first,
                                       const id_type second,
                                       const edge_data_type& init12) {
    return insertEdge(first,second,init12,init12);
  }

  // insertEdge
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertEdge(const node_pair& nodes,
                                       const edge_data_type& init) {
    return insertEdge(nodes.first,nodes.second,init,init);
  }

  // insertEdge
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertEdge(const id_type first,
                                       const id_type second,
                                       const edge_data_type& init12,
                                       const edge_data_type& init21) {

    if ((first == second) || !isNodeIdValid(first) || !isNodeIdValid(second)) {
      // a node can never be adjacent to itself
      return false;
    }

    // compute first the weight for the desired edges (two edges!)
    const weight_type w1 = theWeightFunctor(nodes[first].second,
                                            nodes[second].second,
                                            init12);

    weight_type w2;
    if (edge_traits::Symmetric) {
      w2=w1;
    } else {
      w2 = theWeightFunctor(nodes[first].second,
                             nodes[second].second,
                             init21);
    }
    
    return insertWeightedEdgeProt(first,second,init12,w1,init21,w2);
  }

  // insertEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertEdge(const node_pair& nodes,
                                       const edge_data_type& init12,
                                       const edge_data_type& init21) {
    return insertEdge(nodes.first,nodes.second,init12,init21);
  }


  // insertEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertWeightedEdge(const id_type first,
                                               const id_type second,
                                               const edge_data_type& init12,
                                               const weight_type& w1,
                                               const edge_data_type& init21,
                                               const weight_type& w2) {

    if ((first == second) || !isNodeIdValid(first) || !isNodeIdValid(second)) {
      // a node can never be adjacent to itself
      return false;
    }

    return insertWeightedEdgeProt(first,second,init12,w1,init21,w2);
  }

  // insertEdgeProt()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertWeightedEdgeProt(const id_type first,
                                                  const id_type second,
                                                  const edge_data_type& init12,
                                                  const weight_type& w1,
                                                  const edge_data_type& init21,
                                                  const weight_type& w2) {
   
    row_type& row1 = adjacency[first];

    typename row_type::iterator it;
    if ((it=row1.find(second)) != row1.end()) {
      // edge already exists!
      return false;
    }

    row_type& row2 = adjacency[second];
    
    std::pair<typename row_type::iterator,bool> f;
    f = row1.insert(std::make_pair(second,
                       entry_type(distances.insert(w1,node_pair(first,second)),
                                  init12)));
    assert(f.second); // this should always be true, but better check!
    it=f.first;
    nodes[first].first++; // one edge added!

    typename row_type::iterator it2;
    f = row2.insert(std::make_pair(first,
                      entry_type(distances.insert(w2,node_pair(second,first)),
                                 init21)));
    assert(f.second);
    it2=f.first;
    nodes[second].first++; // one edge added!

    // update the complement iterators
    (*it).second.complement = it2;
    (*it2).second.complement = it;

    return true;
  }

  // insertEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::insertWeightedEdge(const node_pair& nodes,
                                               const edge_data_type& init12,
                                               const weight_type& w1,
                                               const edge_data_type& init21,
                                               const weight_type& w2) {
    return insertWeightedEdge(nodes.first,nodes.second,init12,w1,init21,w2);
  }



  // forceTopologicalEdge()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::edge_data_type&
  adjacencyGraph<N,W,D,F,E>::forceTopologicalEdge(const node_pair& edge) {
    return forceTopologicalEdge(edge.first,edge.second);
  }

  // forceTopologicalEdge()
  template<class N,class W,class D,class F,class E>
  typename adjacencyGraph<N,W,D,F,E>::edge_data_type&
  adjacencyGraph<N,W,D,F,E>::forceTopologicalEdge(const id_type first,
                                            const id_type second) {
   
    row_type& row1 = adjacency[first];

    typename row_type::iterator it;
    if ((it=row1.find(second)) != row1.end()) {
      // edge already exists!
      return (*it).second.data;
    }

    // edge pair didn't exist
    row_type& row2 = adjacency[second];

    // create first edge with an entry with invalid weight index and default
    // data
    std::pair<typename row_type::iterator,bool> f;
    f = row1.insert(std::make_pair(second,
                                   entry_type(-1, // invalid weight key!
                                              edge_data_type())));
    assert(f.second); // this should always be true, but better check!
    it=f.first;
    nodes[first].first++; // one edge added!

    // create second edge with an entry with invalid weight index and default
    // data
    typename row_type::iterator it2;
    f = row2.insert(std::make_pair(first,
                                   entry_type(-1, // invalid weight key!
                                              edge_data_type())));
    assert(f.second);
    it2=f.first;
    nodes[second].first++; // one edge added!

    // update the complement iterators
    (*it).second.complement = it2;
    (*it2).second.complement = it;

    return (*it).second.data;
  }

  // recomputeAllWeights()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::recomputeAllWeights() {
    distances.clear();
    // create first temporary vectors with all information
    // and reassign it to the distances structure at once: much faster!

    std::vector<weight_type> distKey;
    std::vector<node_pair> distData;

    iterator nit,enit;
    edge_iterator eit,eeit;
    weight_type w;
    node_pair p;

    for (nit=begin(),enit=end();nit!=enit;++nit) {
      for (eit=begin(nit.id()),eeit=end(nit.id());eit!=eeit;++eit) {
        p = getEdge(eit);
        if (!edge_traits::Symmetric || (p.second < p.first)) {
          eit.getRWEntry().index = static_cast<int>(distKey.size());
          w = theWeightFunctor(nodes[p.first].second,
                               nodes[p.second].second,
                               *eit);
          distKey.push_back(w);
          distData.push_back(p);
        } else {
          break; // for a symmetric matrix there is nothing else to do.
        }
      }
    }

    distances.create(distKey,distData);

    return true;
  }

  // removeEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::removeEdge(const id_type first,
                                       const id_type second) {
    
    if ((first == second) || !isNodeIdValid(first) || !isNodeIdValid(second)) {
      // a node can never be adjacent to itself
      return false;
    }

    row_type& row1 = adjacency[first];

    typename row_type::iterator it;
    
    if ((it=row1.find(second)) == row1.end()) {
      // edge not found
      return false;
    }

    row_type& row2 = adjacency[second];

    typename row_type::iterator it2 = (*it).second.complement;

    // remove entry from queue
    if (distances.valid((*it).second.index)) {
      distances.erase((*it).second.index);
    }

    // remove entry from matrix
    row1.erase(it);
    nodes[first].first--; // one edge removed!

    // remove entry from queue
    if (distances.valid((*it2).second.index)) {
      distances.erase((*it2).second.index);
    }
    // remove entry from matrix
    row2.erase(it2);
    nodes[second].first--; // one edge removed!

    return true;
  }

  // removeEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::removeEdge(const node_pair& nodes) {
    return removeEdge(nodes.first,nodes.second);
  }
  
  // removeEdge()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::removeEdge(const edge_iterator& edge) {
    node_pair e = getEdge(edge);
    return removeEdge(e.first,e.second);
  }

  // clear()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::clear() {
    adjacency.clear();
    nodes.clear();
    distances.clear();
    firstValidNodeIndex=0;
    lastValidNodeIndex=-1;
    freeElements=0;

    return true;
  };

  // empty
  template<class N,class W,class D,class F,class E>
  inline bool adjacencyGraph<N,W,D,F,E>::empty() const {
    return ((firstValidNodeIndex > lastValidNodeIndex) || (nodes.size() == 0));
  }

  // copy()
  template<class N,class W,class D,class F,class E>
  adjacencyGraph<N,W,D,F,E>& 
  adjacencyGraph<N,W,D,F,E>::copy(const adjacencyGraph<N,W,D,F,E>& other) {
    adjacency = other.adjacency;
    nodes = other.nodes;
    distances.copy(other.distances);
    firstValidNodeIndex = other.firstValidNodeIndex;
    lastValidNodeIndex = other.lastValidNodeIndex;
    freeElements = other.freeElements;
    
    theWeightFunctor = other.theWeightFunctor;

    fixEntryIterators();

    return *this;
  }

  // getWeightFunctor()
  template<class N,class W,class D,class F,class E>
  const typename adjacencyGraph<N,W,D,F,E>::weight_functor& 
  adjacencyGraph<N,W,D,F,E>::getWeightFunctor() const {
    return theWeightFunctor;
  }

  // setWeightFunctor()
  template<class N,class W,class D,class F,class E>
  void adjacencyGraph<N,W,D,F,E>::setWeightFunctor(const weight_functor& ftor){
    theWeightFunctor=ftor;
  }

  // fixEntryIterators
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::fixEntryIterators() {
    // fix only the iterators in the valid nodes
    int i,j;
    typename row_type::iterator ait,bit;
    for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
      if (nodes[i].first) {
        row_type& row = adjacency[i];
        for (ait=row.begin();ait!=row.end();++ait) {
          // update only upper-right diagonal matrix
          j = (*ait).first;
          if (j > i) {
            bit = adjacency[j].find(i);
            (*ait).second.complement = bit;
            (*bit).second.complement = ait;
          }
        }
      }
    }
    return true;
  }

  // operator=()
  template<class N,class W,class D,class F,class E>
  adjacencyGraph<N,W,D,F,E>& 
  adjacencyGraph<N,W,D,F,E>::operator=(const adjacencyGraph<N,W,D,F,E>& other){
    return copy(other);
  }
  
  // clone()
  template<class N,class W,class D,class F,class E>
  mathObject* adjacencyGraph<N,W,D,F,E>::clone() const {
    return new adjacencyGraph<N,W,D,F,E>(*this);
  }

  // getTypeName()
  template<class N,class W,class D,class F,class E>
  const char* adjacencyGraph<N,W,D,F,E>::getTypeName() const {
    return "adjacencyGraph<N,W,D,F,E>";
  };

  // checkConsistency()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::checkConsistency() const {
    if (empty()) {
      return true;
    }

    if (static_cast<int>(nodes.size()) <= lastValidNodeIndex) {
      return false;
    }

    int i;
    for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
      if (nodes[i].first < 0) {
        // node deleted -> number of edges must be zero
        if (!adjacency[i].empty()) {
          return false;
        }
      } else {
        if (static_cast<int>(adjacency[i].size()) != nodes[i].first) {
          return false;
        }
      }
    }
    
    return true;
  }

  // write()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::write(ioHandler& handler,
                                  const bool complete) const {

    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (empty()) {
      // special case for empty graph
      lti::write(handler,"size",0);
    } else {

      // 1. necessary vector size      
      lti::write(handler,"size",lastValidNodeIndex+1);

      // 2. pairs of node id + node data, to be inserted in the vector
      handler.writeBegin();             // (

      handler.writeSymbol("nodes");     //  nodes
      handler.writeKeyValueSeparator(); //       _
      
      handler.writeBegin();             //         (
      int i;
      for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
        if (nodes[i].first>=0) {
          handler.writeBegin();         //           (
          handler.write(i);             //             node_id
          handler.writeDataSeparator(); //                     _
          lti::write(handler,nodes[i].second); //                  node_data
          handler.writeEnd();           //           )
        }
      }                                 //           ... many of them ...

      handler.writeEnd();              //          )
      handler.writeEnd();              // )

      // 3. for each valid node write the row of the matrix, including
      //    for each entry the neighbor node, and the edge data.

      // store symmetrie, to read data correctly
      lti::write(handler,"symmetric",edge_traits::Symmetric);

      handler.writeBegin();             // (

      handler.writeSymbol("edges");     //  edges
      handler.writeKeyValueSeparator(); //       _
      
      handler.writeBegin();             //        (
      const_edge_iterator it,eit;
      node_pair p;

      // the symmetric graphs can be stored in much less space!
      // use only the bottom left matrix
      for (i=firstValidNodeIndex;i<=lastValidNodeIndex;++i) {
        for (it=begin(i),eit=end(i);it!=eit;++it) {
          p = getEdge(it);
          if (!edge_traits::Symmetric || (p.second < p.first)) {
            handler.writeBegin();         //      (
            // the pair
            handler.writeBegin();         //        (
            handler.write(p.first);       //         l1
            handler.writeDataSeparator(); //           _
            handler.write(p.second);      //            l2
            handler.writeEnd();           //        )
            handler.writeDataSeparator(); //           _
            lti::write(handler,getEdgeWeight(it)); //    weight
            handler.writeDataSeparator(); //                   _
            
            lti::write(handler,*it);      //                     node_data
            handler.writeEnd();           //      )   
          } else {
            break; // nothing left to do for this node
          }
        }                                 //        ... many of them ...
      }

      handler.writeEnd();                 //        ) ;; end of list of edges
      handler.writeEnd();                 // )        ;; end of edges
      
    }

    if (complete) {
      b = handler.writeEnd() && b;
    }

    return b;
  }

  // read()
  template<class N,class W,class D,class F,class E>
  bool adjacencyGraph<N,W,D,F,E>::read(ioHandler& handler,
                                 const bool complete) {
    clear();

    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    int size;
    lti::read(handler,"size",size);
    if (size>0) {
      adjacency.resize(size,row_type());
      nodes.resize(size,std::make_pair(-1,N()));
      lastValidNodeIndex = size-1;

      // 2. pairs of node id + node data, to be inserted in the vector
      handler.readBegin();              // (
      
      if (handler.trySymbol("nodes")) { // nodes
        handler.readKeyValueSeparator();//      _
        
        handler.readBegin();            //       (
        int i,j;
        for (i=0;i<size;++i) {
          if (handler.tryBegin()) {     //         (
            handler.read(j);            //           node_id
            nodes[j].first = 0; // undelete
            handler.readDataSeparator();//                   _
            lti::read(handler,nodes[j].second); //              node_data
            handler.readEnd();          //         )
            // insert the node
          }
        }                               //         ... many of them ...

        handler.readEnd();              //       )
      }

      handler.readEnd();                // )

      // 3. read the edges and their data

      // read symmetry 
      bool symmetric;
      lti::read(handler,"symmetric",symmetric);
      
      handler.readBegin();              // (

      if (handler.trySymbol("edges")) {  // edges
        handler.readKeyValueSeparator();//       _
      
        handler.readBegin();            //        (
        node_pair p;
        edge_data_type edata;
        weight_type w;

        if (symmetric) {
          // the symmetric graphs can be stored in much less space!
          // use only the upper left matrix
          bool moreData = true;
          do {
            if (handler.tryBegin()) {       //      (
        
              // the pair
              handler.readBegin();          //        (
              handler.read(p.first);        //         l1
              handler.readDataSeparator();  //           _
              handler.read(p.second);       //            l2
              handler.readEnd();            //        )

              handler.readDataSeparator();  //              _
              lti::read(handler,w);         //               weight

              handler.readDataSeparator();  //                      _
              
              lti::read(handler,edata);     //                       node_data
              if (!insertWeightedEdge(p,edata,w,edata,w)) {
                // the edge existed already!  just update
                // its data and weight
                getEdgeData(p.first,p.second)=edata;
                getEdgeData(p.second,p.first)=edata;
                updateEdgeWeight(p.first,p.second,w);
                updateEdgeWeight(p.second,p.first,w);
              }              

              handler.readEnd();            //      )
            } else {
              // no more data found! 
              // -> continue
              moreData=false; 
            }
          } while (moreData);               //        ... many of them ...
          
        } else {
          // asymmetric graphs need to store all edges
          bool moreData=true;
          do {
            if (handler.tryBegin()) {       //        (
              // the pair
              handler.readBegin();          //          (
              handler.read(p.first);        //           l1
              handler.readDataSeparator();  //             _
              handler.read(p.second);       //              l2
              handler.readEnd();            //          )
              handler.readDataSeparator();  //                _
              lti::read(handler,w);         //                 weight
              handler.readDataSeparator();  //                       _
            
              lti::read(handler,edata);     //                        node_data
              if (!insertEdge(p,edata)) {
                // node already existed
                getEdgeData(p)=edata;
              }
              updateEdgeWeight(p,w);
              handler.readEnd();           //        )
            } else {
              // no more data found! 
              // -> continue
              moreData=false;           
            }
          } while (moreData);              //        ... many of them ...
        } // end of else if symmetric

        handler.readEnd();               //        )

      } // end of if edges
          
      handler.readEnd();               // )

    }
   
    if (complete) {
      b = handler.readEnd() && b;
    }
    
    return b;
  }

} // end of namespace lti

#include "ltiUndebug.h"

