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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiMinimumSpanningTree_template.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.4.2003
 * revisions ..: $Id: ltiMinimumSpanningTree_template.h,v 1.12 2005/08/05 12:37:09 libuda Exp $
 */

#include "ltiKdTree.h"
#include <vector>
#include "ltiBoundsFunctor.h"

namespace lti {
  // --------------------------------------------------
  // minimumSpanningTree::parameters
  // --------------------------------------------------


  // --------------------------------------------------
  // minimumSpanningTree
  // --------------------------------------------------

  // default constructor
  template <class K, class V, class Distantor>
  minimumSpanningTree<K,V,Distantor>::minimumSpanningTree()
    : functor(){
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template <class K, class V, class Distantor>
  minimumSpanningTree<K,V,Distantor>::minimumSpanningTree(const parameters& par)
    : functor() {
    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <class K, class V, class Distantor>
  minimumSpanningTree<K,V,Distantor>::minimumSpanningTree(const minimumSpanningTree& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class K, class V, class Distantor>
  minimumSpanningTree<K,V,Distantor>::~minimumSpanningTree() {
  }

  // returns the name of this type
  template <class K, class V, class Distantor>
  const char* minimumSpanningTree<K,V,Distantor>::getTypeName() const {
    return "minimumSpanningTree";
  }

  // copy member
  template <class K, class V, class Distantor>
  minimumSpanningTree<K,V,Distantor>&
  minimumSpanningTree<K,V,Distantor>::copy(const minimumSpanningTree& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  template <class K, class V, class Distantor>
  minimumSpanningTree<K,V,Distantor>&
  minimumSpanningTree<K,V,Distantor>::operator=(const minimumSpanningTree<K,V,Distantor>& other) {
    return (copy(other));
  }


  // clone member
  template <class K, class V, class Distantor>
  functor* minimumSpanningTree<K,V,Distantor>::clone() const {
    return new minimumSpanningTree(*this);
  }

  // return parameters
  template <class K, class V, class Distantor>
  const typename minimumSpanningTree<K,V,Distantor>::parameters&
  minimumSpanningTree<K,V,Distantor>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  template <class K, class V, class Distantor>
  bool minimumSpanningTree<K,V,Distantor>::buildGraph(
                                                      const std::vector<K>& src, const std::vector<V>& datas,
                                                      weightedGraph<K,V,distance_type>& graph) const {

    bool ok(true);
    typedef kdTree<K,int,Distantor> kdTreeType;
    typedef typename kdTreeType::element elementsType;
    std::list< elementsType* > foundElements;
    typename std::list< elementsType* >::iterator elemIt;
    int nbPoints(static_cast<int>(src.size())),i,j,tmp;
    kdTreeType myKDTree;
    int nbNeighbors=getParameters().nbNeighbors;

    // add all points and build the kdTree
    for (i=0; i<nbPoints; i++) {
      //       myKDTree.add(src.at(i),i);
      //       graph.insertNode(src.at(i));
      myKDTree.add(src[i],i);
      graph.insertNode(datas[i],src[i]);
    }
    myKDTree.build();
    
    // belongsToTree contains for each point, the id of the tree it belongs 
    // to. It is initialized with each point in its own tree. 
    // Later the trees are merged. A point and all its neighbors that are 
    // found by the kdTree are combined into the same tree.
    ivector belongsToTree(nbPoints);
    for (i=0; i<nbPoints; i++) {
      belongsToTree.at(i) = i;
    }
    
    // the ids of rows, that a point is connected to.
    ivector ids(nbNeighbors);
    // the number of the tree, that a point point belongs to.
    ivector treeNb(nbNeighbors);
    // A vector that contains in each row a vector with the ids of the
    // connected point.
    
    // search for each point nbNeighbors neighbors
    for (i=0; i<nbPoints; i++) {
      myKDTree.searchNearest(nbNeighbors,src[i],foundElements);
      j=0;
      // save the ids of the found neighbors to the id vector
      // and the compute the distance of this edges in edges.
      for (elemIt=foundElements.begin(); 
           elemIt!=foundElements.end(); elemIt++) {
        ids.at(j) = (*elemIt)->data;
        tmp = (*elemIt)->data;
        // collect all tree numbers from the found points to merge them
        // to one tree whose id is the minimal id of all found ids.
        treeNb.at(j) = belongsToTree.at((*elemIt)->data);
        if (i!=tmp) {
          graph.insertEdge(i,tmp,distantor(src[i],src[tmp]));
        }
        j++;
      }
      // merge all found points into the same tree
      for (j=0; j<nbNeighbors; j++) {
        belongsToTree.at(ids.at(j)) = treeNb.minimum();
      }
    }
    
    // connect all not connected parts
    // To do that, a forest of kdTrees is build. Each kdTree contains
    // all points of a seperated cluster. Then the shortest connection
    // between all clusters is searched and inserted into the edges matrix
    // and the connections vector.
    
    // test if there are differnt trees left
    bool allConnected(false);
    if (belongsToTree.minimum() == belongsToTree.maximum() ) {
      allConnected = true;
    }
       
    if (!allConnected) {
      std::list<int> pl;
      std::list<int>::iterator plIt;
      kdTreeType *kdTreePtr;
      // map the tree number to the tree
      std::map<int,kdTreeType*> kdForest;
      typename std::map<int,kdTreeType*>::iterator kdForestIt;
      // sort all points into the corresponding kdTree
      for (i=0; i<nbPoints; i++) {
        if (kdForest.find(belongsToTree.at(i))==kdForest.end()) {
          kdTreePtr = new kdTreeType;
          kdForest[belongsToTree.at(i)]=kdTreePtr;
          pl.push_back(i);
        }
        kdForest[belongsToTree.at(i)]->add(src[i],i);
      }
      // build up all trees
      for (kdForestIt=kdForest.begin();
           kdForestIt!=kdForest.end(); kdForestIt++) {
        kdForestIt->second->build();
      }
      // p contains one point of each tree.
      // ptt contains the number of the different trees
      //       matrix<typename K::value_type> p(kdForest.size(),src.columns());
      std::vector<K> p(kdForest.size());
      vector<int> ptt(static_cast<int>(kdForest.size()));
      i=0;
      for (plIt=pl.begin(); plIt!=pl.end(); plIt++) {
        //         p.setRow(i,src.at(*plIt));
        p[i]=src[*plIt]; //.setRow(i,src.at(*plIt));
        ptt.at(i)=belongsToTree.at(*plIt);
        i++;
      }
      elementsType elem1,elem2;
      K old1,old2;
      bool conv(false);
      // now find the shortest connection between each tree.
      // The points in p are used as starting points. 
      //       for (i=0; i<p.rows(); i++) {
      //         for (j=i+1; j<p.rows(); j++) {
      for (i=0; i<static_cast<int>(p.size()); i++) {
        for (j=i+1; j<static_cast<int>(p.size()); j++) {
          old1=p[i];
          kdForest[ptt.at(j)]->searchNearest(old1,elem2);
          old2=elem2.point;
          while (!conv) {
            kdForest[ptt.at(i)]->searchNearest(old2,elem1);
            kdForest[ptt.at(j)]->searchNearest(old1,elem2);
            if (elem2.point==old2 && elem1.point==old1) {
              conv = true;
            } else {
              old2=elem2.point;
              old1=elem1.point;
            }
          }
          // insert the weight of the edge into edges matrix
          graph.insertEdge(elem1.data,elem2.data,distantor(old1,old2));
          
          conv = false;
        }
      }
    }
    return ok;
  }

  template <class K, class V, class Distantor>
  bool minimumSpanningTree<K,V,Distantor>::apply(
                                                 const weightedGraph<K,V,distance_type>& src,
                                                 weightedGraph<K,V,distance_type>& dest) const {

    bool ok(true);
    dest.clear();
    dest.copy(src);
    apply(dest);
    return ok;
  }

  template <class K, class V, class Distantor>
  bool minimumSpanningTree<K,V,Distantor>::apply(const std::vector<K>& src,
                                                 const std::vector<V>& datas,
                                                 weightedGraph<K,V,distance_type>& graph) const{

    bool ok;
    if (getParameters().useCompleteGraph) {
      ok = buildCompleteGraph(src,datas,graph);
    } else {
      ok = buildGraph(src,datas,graph);
    }
    ok = ok && apply(graph);
    return ok;
  }

  template <class K, class V, class Distantor>
  bool minimumSpanningTree<K,V,Distantor>::apply(weightedGraph<K,V,
                                                 distance_type>& srcDest) const {
    bool ok=true;
    typedef std::pair<int,int> edge;
    
    std::map<int, node_type*> nodes=srcDest.getNodes();
    std::map<edge,distance_type> edges=srcDest.getEdges();
    
    std::list<int>::iterator it;
    typename std::map<int, node_type*>::iterator nIt;
    std::map<int,std::list<int>* > tmpList;
    std::list<int> *t;
    for (nIt=nodes.begin(); nIt!=nodes.end(); nIt++) {
      t = new std::list<int>(nIt->second->getConnections());
      tmpList[nIt->first]=t;
    }
    srcDest.clearEdges();
    int nbNodes=static_cast<int>(nodes.size());
    int i;
    // making out the start edge
    // list with all ids, that are included into the minimum spanning tree
    std::list<int> inTree;
    // a element corresponding to a key is true if the key has already 
    // been inserted into the tree
    std::map<int,bool> inTreeMap;
    for(i=0; i<nbNodes; i++) {
      inTreeMap[i]=false;
    }

    std::list<int>::iterator listIntIt;
    // insert the first point into the list. We always start with the 
    // first point given in src, because it is not important with which 
    // point we started for the minimum spanning tree
    inTree.push_back(nodes.begin()->first);
    inTreeMap[nodes.begin()->first]=true;
    key_value_type old1,old2;
    point min;
    
    key_value_type max=std::numeric_limits<key_value_type>::max();
    old1=max;
    std::pair<int,int> tmpPair;
    
    // insert one edge in each interation to the list. So i must go from 
    // i to nbPoints-1, because a minimum spanning tree has nbPoints-1 edges.
    for (i=0; i<nbNodes-1; i++) { 
      for (listIntIt=inTree.begin(); listIntIt!=inTree.end(); listIntIt++) {
        for (it=tmpList[(*listIntIt)]->begin(); 
             it!=tmpList[(*listIntIt)]->end(); it++) {
          if (!inTreeMap[*it]) {
            tmpPair.first=*listIntIt;
            tmpPair.second=*it;
            old2=edges[tmpPair];
            if(old2<old1) {
              min.x=(*listIntIt);
              min.y=*it;
              old1=old2;
            }
          }
        }
      }
      // insert the found edge into the minimum spanning tree
      srcDest.insertEdge(min.x,min.y,old1);
      // mark the inserted point
      if (inTreeMap[min.x]) {
        inTree.push_back(min.y);
        inTreeMap[min.y]=true;
      } else {
        inTree.push_back(min.x);
        inTreeMap[min.x]=true;
      };
      old1=max;    

    }
 
    return ok;
  }

  template <class K, class V, class Distantor>
  bool minimumSpanningTree<K,V,Distantor>::buildCompleteGraph(
                                                              const std::vector<K>& src,const std::vector<V>& datas,
                                                              weightedGraph<K,V,distance_type>& graph) const {

    // BUILD UP MINIMUM SPANNING TREE
    bool ok(true);
    int i,j;
    int nbPoints=static_cast<int>(src.size());
    graph.clear();
    for (i=0; i<nbPoints; i++) {
      graph.insertNode(datas[i],src[i]);
    }
    for (i=0; i<nbPoints; i++) {
      for (j=i+1; j<nbPoints; j++) {
        graph.insertEdge(i,j,distantor(src[i],src[j]));
      }
    }
    return ok;
  };

}

