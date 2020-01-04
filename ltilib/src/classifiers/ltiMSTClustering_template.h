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
 * file .......: ltiMSTClustering_template.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 22.4.2003
 * revisions ..: $Id: ltiMSTClustering_template.h,v 1.4 2005/11/14 15:53:03 doerfler Exp $
 */


#include "ltiMinimumSpanningTreeOfKeyValuetype.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"
#include "ltiSTLIoInterface.h"

namespace lti {
  // --------------------------------------------------
  // MSTClustering::parameters
  // --------------------------------------------------


  // --------------------------------------------------
  // MSTClustering
  // --------------------------------------------------

  // default constructor
  template<class U>
  MSTClustering<U>::MSTClustering()
    : clustering() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

  }

  // copy constructor
  template<class U>
  MSTClustering<U>::MSTClustering(const MSTClustering& other)
    : clustering() {
    copy(other);
  }

  // destructor
  template<class U>
  MSTClustering<U>::~MSTClustering() {
  }

  // returns the name of this type
  template<class U>
  const char* MSTClustering<U>::getTypeName() const {
    return "MSTClustering<U>";
  }

  // copy member
  template<class U>
  MSTClustering<U>& MSTClustering<U>::copy(const MSTClustering& other) {
      clustering::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class U>
  MSTClustering<U>& MSTClustering<U>::operator=(const MSTClustering& other) {
    return (copy(other));
  }

  // clone member
  template<class U>
  classifier* MSTClustering<U>::clone() const {
    return new MSTClustering(*this);
  }

  // return parameters
  template<class U>
  const typename MSTClustering<U>::parameters&
  MSTClustering<U>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&classifier::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The train-methods!
  // -------------------------------------------------------------------

  // Calls the same method of the superclass.
  template<class U>
  bool MSTClustering<U>::train(const dmatrix& input, ivector& ids) {

    train(input);

    int nbData=input.rows();

    // resize and clear ids
    ids.resize(nbData, 0, false);

    classifier::outputVector out;

    for (int i=0; i<nbData; i++) {
      if (classify(input.getRow(i),out)) {
        ids.at(i)=out.getWinnerUnit();
      } else {
        ids.at(i)=classifier::noObject;
      }
    }

    return true;
  }

  template<class U>
  void MSTClustering<U>::collectDists(const int a,const int b,
                             std::list<std::pair<int,int> >& dists,int depth) {
    typedef std::pair<int,int> edge;
    std::map<int,weightedGraph<vector<double>,int,double>::node* > nodes;
    nodes=gr1.getNodes();

    std::list<int> tmpVec=nodes[b]->getConnections();
    std::list<int>::iterator it;
    depth--;        
    for (it=tmpVec.begin(); it!=tmpVec.end(); it++) {
      // if the iterator doen't point to the id that was visited before
      if (*it!=a) {
        dists.push_back(edge(b,*it));
      }
      if (depth!=0) {
        collectDists(b,*it,dists,depth);
      }
    }
  };
    
  template<class U>
  void MSTClustering<U>::findClusters(const int from,int& curId,int& nextId,
                   weightedGraph<vector<double>,int,double>::node* & startNode,
                   vector<int>& ids,std::vector<int>& startNodes){

    std::map<int,weightedGraph<vector<double>,int,double>::node* > nodes;
    std::map<std::pair<int,int>,double> edges;
    
    edges=gr1.getEdges();
    nodes=gr1.getNodes();

    std::list<int> cons=startNode->getConnections();
    std::list<int>::iterator it;
    int tmp1;
    int myId=startNode->getId();
    ids.at(myId)=curId;
    for (it=cons.begin(); it!=cons.end(); it++) {
      if (*it!=from) {
        if (edges[std::pair<int,int>(myId,*it)]==-1.) {
          gr1.removeEdge(myId,*it);
//           edgesToDelete.push_back(std::pair<int,int>(myId,*it);
          tmp1=nextId;
          nextId++;
          startNodes.push_back(nodes[*it]->getId());
          findClusters(myId,tmp1,nextId,nodes[*it],ids,startNodes);
        } else {
          findClusters(myId,curId,nextId,nodes[*it],ids,startNodes);
        }
      }
    }
  }

  template<class U>
  bool MSTClustering<U>::train(const dmatrix& data) {

    typedef vector<double> keytype;
    typedef std::pair<int,int> edge;
    typedef weightedGraph<keytype,int,double>::node* nodePtr;
    gr1.clear();
    parameters param=getParameters();
    int nbMaxSteps=param.nbMaxSteps;
    double devFac=param.devFac;
    ivector tmpIds(data.rows());
    int i;
    bool ok=true;
    // find the minimum spanning tree
    minimumSpanningTreeOfKeyValuetype<keytype,int,U> minTree;
    minTree.apply(data,tmpIds,gr1);
    
    std::map<int,nodePtr> nodes=gr1.getNodes();
    std::map<edge,double> edges=gr1.getEdges();
 
    std::map<int,nodePtr>::const_iterator it;
    std::list<edge>::const_iterator edgeIt;

    std::list<edge> tmpEdges;
    // later the edges that will be deleted are placed in this list 
    // because they can´t be deleted at once.
    std::list<edge> toDelete;

    // functors for computation of mean and standard deviation
    meansFunctor<double> mF;
    varianceFunctor<double> vF;
    double tmp,deviation,maxLength;
    tmp=0.;
    for (it=nodes.begin(); it!=nodes.end(); it++) {
      tmpEdges.clear();
      // collect all distances starting at the node pointed by it until the 
      // nbMaxSteps depth
      // this function returns the edges that were on the way in tmpEdges
      collectDists(it->first,it->first,tmpEdges,nbMaxSteps);
      dvector dists(static_cast<int>(tmpEdges.size()));
      i=0;
      // now collect all the distances from edges
      for (edgeIt=tmpEdges.begin(); edgeIt!=tmpEdges.end(); edgeIt++) {
        dists.at(i)=edges[*edgeIt];
        i++;
      }
      tmp=mF.apply(dists); // the mean of the points
      deviation=sqrt(vF.apply(dists)); // the standard deviation
      maxLength=tmp+devFac*deviation; // compute maximal length
      for (edgeIt=tmpEdges.begin(); edgeIt!=tmpEdges.end(); edgeIt++) {
        // if a edge was too long mark it for deleting
        // the edge can not be deleted at once because the length of this 
        // edge might be used later for computing the mean and the standard 
        // deviation
        if (edges[*edgeIt]>maxLength) {
          toDelete.push_back(*edgeIt);
        }
      }
    }

    // set all weight of all edges that will be deleted to -1
    for (edgeIt=toDelete.begin(); edgeIt!=toDelete.end(); edgeIt++) {
      gr1.setWeight(edgeIt->first,edgeIt->second,-1.);
    }
      
    int curId=0;
    int nextId=1;
    vector<int> ids(static_cast<int>(nodes.size()));
    std::vector<int> startNodes;
    startNodes.push_back(0);
    // find all Clusters that remain when removing the inconsistent edges.
    findClusters(0,curId,nextId,nodes.begin()->second,ids,startNodes);

    int nbClusters = ids.maximum()+1;

    // contruct one kdTree for each remaining cluster.
    // These kdTrees are saved and later used for classifing the features.
    kdTree<keytype,int,U>* tmpTree;
    for (i=0; i<nbClusters; i++) {
      tmpTree = new kdTree<keytype,int,U>;
      clusters.push_back(tmpTree);
    }
    for (i=0; i<ids.size(); i++) {
      clusters[ids.at(i)]->add(nodes[i]->getPoint(),ids.at(i));
    }
    typename std::vector<kdTree<keytype,int,U>* >::iterator itCluster;
    for (itCluster=clusters.begin();itCluster!=clusters.end(); itCluster++) {
      (*itCluster)->build();
    }

    std::vector<int>::iterator itNode,itNode2;

    // compute all minimal distances between all clusters. This information
    // is used during the classification process.
    if (param.probabilityFromDistanceMode==
        parameters::InterClusterDistanceDependend) {
      bool conv=false;
      dvector a,b;
      int clusterIdA,clusterIdB,counter;
      typename kdTree<keytype,int,U>::element elem,elem2;
      
      for (itNode=startNodes.begin(); itNode!=startNodes.end(); itNode++) {
        for (itNode2=itNode+1; itNode2!=startNodes.end(); itNode2++) {
          a=nodes[*itNode]->getPoint();
          clusterIdA=ids.at(*itNode);
          b=nodes[*itNode2]->getPoint();
          clusterIdB=ids.at(*itNode2);
          conv=false;
          counter=0;
          while (!conv && counter<10) {
            // search nearest point in cluster idB from point a
            clusters[clusterIdB]->searchNearest(a,elem);
            clusters[clusterIdA]->searchNearest(b,elem2);
            if (elem2.point==a && elem.point==b) {
              conv=true;
            } else {
              a=elem2.point;
              b=elem.point;
            }
            counter++;
          }
          
          // test if the current distance is smaller than the previous ones.
          // If so, this is the new smallest one.
          tmp = distantor(a,b);
          if (tmp<minDistances[clusterIdA] || minDistances[clusterIdA]==0) {
            minDistances[clusterIdA]=tmp;
          }
          minDistances[clusterIdB]=tmp;
        }
      }
    }

    //     Put the id information into the result object
    //     Each cluster has the id of its position in the matrix
    ivector tids(nbClusters);
    for (i=0; i<nbClusters; i++) {
      tids.at(i)=i;
    }
    outTemplate=outputTemplate(tids);

    return ok;
  };


  // -------------------------------------------------------------------
  // The classify-method!
  // -------------------------------------------------------------------


  template<class U>
  bool MSTClustering<U>::classify(const dvector& feature, 
                               outputVector& result) const {
    typename std::vector<kdTree<dvector,int,U>* >::const_iterator itCluster;
    typename kdTree<dvector,int,U>::element elem;
    parameters param=getParameters();
    bool ok=true;
    dvector dists(static_cast<int>(clusters.size()));
    int i=0;
    double tmp=0.;
    double variance=param.variance;
    double distance=0.;
    if (param.probabilityFromDistanceMode==
        parameters::InterClusterDistanceDependend) {
      for (itCluster=clusters.begin();
           itCluster!=clusters.end(); 
           itCluster++, i++) {
        (*itCluster)->searchNearest(feature,elem);
        tmp = distantor(elem.point,feature);
        distance=minDistances.find(elem.data)->second;
        dists.at(i)=exp(-tmp*tmp/(variance*
                                  distance*
                                  distance));
      }
    } else {
      for (itCluster=clusters.begin();
           itCluster!=clusters.end(); 
           itCluster++, i++) {
        (*itCluster)->searchNearest(feature,elem);
        tmp = distantor(elem.point,feature);
        dists.at(i)=exp(-tmp*tmp/variance);
      }
    }

    ok = ok && outTemplate.apply(dists,result);
    result.setWinnerAtMax();
    return ok;
  }

  template<class U>
  bool MSTClustering<U>::write (ioHandler &handler, 
                                const bool complete) const {
    bool b(true);

    if (complete) {
      b = handler.writeBegin();
    }

    // write the standard data (output and parameters)
    b = b && clustering::write(handler,false);

    int nbClusters=static_cast<int>(clusters.size());
    typename std::vector<kdTree<vector<double>,int,U >* >::const_iterator 
      clusterIt;

    b = b && lti::write(handler,"minDistances",minDistances);
    b = b && lti::write(handler,"nbClusters",nbClusters);
    for (clusterIt=clusters.begin(); clusterIt!=clusters.end(); clusterIt++){
        b = b && (*clusterIt)->write(handler,false);       
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }


  template<class U>
  bool MSTClustering<U>::read(ioHandler &handler, const bool complete) {
    bool b(true);

    if (complete) {
      b = handler.readBegin();
    }

    // read the standard data (output and parameters)
    b = b && clustering::read(handler,false);

    int nbClusters,i;
    kdTree<dvector,int,U>* tmp;
    clusters.clear();
    
    b = b && lti::read(handler,"minDistances",minDistances);
    b = b && lti::read(handler,"nbClusters",nbClusters);

    for (i=1; i<nbClusters; i++) {
        tmp = new kdTree<dvector,int,U>();
        b = b && tmp->read(handler,false);
        clusters.push_back(tmp);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;

  }


}

