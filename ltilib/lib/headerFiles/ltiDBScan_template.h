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
 * file .......: ltiDBScan.h
 * authors ....: Bastian Ibach
 * organization: LTI, RWTH Aachen
 * creation ...: 28.4.2003
 * revisions ..: $Id: ltiDBScan_template.h,v 1.8 2005/10/10 15:19:35 libuda Exp $
 */

//TODO: include only those files which are needed in this header!!

// TODO: Check this include to parent class: 
#include "ltiClustering.h"
#include "ltiKdTree.h"
#include "ltiL2Distance.h"
#include "ltiL1Distance.h"


// std / stl includes
#include <list>

// debug includes
#undef _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"


namespace lti {
    //--------------------------------------------------------
    //       class DBSCan
    //--------------------------------------------------------

    /*
     * default constructor
     */
    template<class T>
    DBScan<T>::DBScan():clustering(){

      // create an instance of the parameters with the default values
      parameters defaultParameters;
      // set the default parameters
      setParameters(defaultParameters);

	
    }

    /*
     * copy constructor
     * @param other the object to be copied
     */
    template<class T>
    DBScan<T>::DBScan(const DBScan& other):clustering() {
    }

    /*
     * destructor
     */
    template<class T>
//    virtual  DBScan<T>::~DBScan(){
    DBScan<T>::~DBScan()
    {
      // delete kd-Trees in kdClusterVec
      typename std::vector<dKdTree*>::iterator it,eit;
      for (it=kdClusterVec.begin(),eit=kdClusterVec.end();it!=eit;++it) {
        delete (*it);
        (*it)=0;
      }
      kdClusterVec.clear();
    }

    /*
     * returns the name of this type ("DBScan")
     */
    template<class T>
//    virtual const char*  DBScan<T>::getTypeName() const{
      const char*  DBScan<T>::getTypeName() const{
	    return "DBScan";
    }

    /*
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */ 
    template<class T>
    DBScan<T>&  DBScan<T>::copy(const DBScan<T>& other){
	    clustering::copy(other);
	    return (*this);
    }

    /*
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    template<class T>
    DBScan<T>&  DBScan<T>::operator=(const DBScan<T>& other){
	    return (copy(other));
    }

    /*
     * returns a pointer to a clone of this clustering.
     */
    template<class T>
//    virtual classifier* DBScan<T>::clone() const{
    classifier* DBScan<T>::clone() const{
	    return new DBScan<T>(*this);
    }

    /*
     * returns used parameters
     */
    template<class T>
    const typename DBScan<T>::parameters& DBScan<T>::getParameters() const{
	    const parameters* par = 
	    dynamic_cast<const parameters*>(&classifier::getParameters());
	    if(isNull(par)) {
	      throw invalidParametersException(getTypeName());
	    }
	    return *par;
    }

    /**
     * Unsupervised training.  The vectors in the <code>input</code>
     * matrix will be put into groups according to the training
     * algorithm.  Additionally, an integer indicating the class each
     * point belongs to is returned. 
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    template<class T>
    bool DBScan<T>::train(const dmatrix& input, ivector& ids){
      // set size of ids to input.rows()
      ids.resize(input.rows());
      
      bool b;
      b=train(input);
      // if clustering fails return else build ids
      if(b==false) return b;

      // building ids result vector
      dElement elem;
      for(int i=0; i < input.rows(); ++i){
        b=setOfPoints.searchExactly(input.getRow(i),elem);
        ids[i]=elem.data;
      }
      return b;
    }

    /**
     * Unsupervised training.
     * The vectors in the <code>input</code> matrix
     * will be clustered using each specific method.
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    template<class T>
    bool DBScan<T>::train(const dmatrix& input){
      // read parameters
      parameters param=getParameters();
      eps = param.eps;
      minPts = param.minPts;
      buckedSize = param.buckedSize;

      bool retValue;
      // clear setOfPoints kd-tree and kdClusterVec;
      setOfPoints.clear();
      kdClusterVec.clear();
      // build kd-Tree 
      for(int i=0; i < input.rows(); ++i){
	      setOfPoints.add(input.getRowCopy(i), -10);
      }
      setOfPoints.build(buckedSize);
      
      if(eps > 0){
	      // call clustering funktion
	      retValue = (dbScan(setOfPoints));
        if(!retValue)
          return false;

	      // compute minimal distances between all clusters. This information
	      // is used during the classification process
	      // kdClusterVec = vector of kdTrees for each cluster one tree
	      typename std::vector<dElement* >::iterator itEl, itEl2;
	      dElement elem, elem2;
	      vector<double> pointA, pointB;
	      bool conv=false;
	      int counter, clusterIDA, clusterIDB;
              double tmp=0.0;
              minDistances.resize(clusterID-1);

	      for(itEl = startElements.begin(); itEl!=startElements.end(); itEl++){
                if (isNull(*itEl)) continue;
	        for(itEl2 = itEl+1; itEl2!=startElements.end(); itEl2++){
                  if (isNull(*itEl2)) continue;
	          pointA = (*itEl)->point;
	          clusterIDA = (*itEl)->data;
	          pointB = (*itEl2)->point;
	          clusterIDB = (*itEl2)->data;
	          conv=false;
	          counter=0;
	          while(!conv && counter<10){
	            kdClusterVec[clusterIDA]->searchNearest(pointB,elem);
	            kdClusterVec[clusterIDB]->searchNearest(pointA,elem2);
	            if(elem.point == pointB && elem2.point==pointA){
		      conv=true;
	            }else{
		      pointA=elem.point;
		      pointB=elem2.point;
	      }
	      counter++;
	    }
	    // calculate distance between the found points
	    // determin distance mesure
	    switch (param.distanceMeasure) {
	      case parameters::L1Distance:
		      tmp = cityBlockDistance(pointA,pointB);
		      break;
	      case parameters::L2Distance:
		      tmp = sqrt(distanceSqr(pointA,pointB));
		      break;
	      case parameters::L2SquareDistance:
		      tmp = distanceSqr(pointA,pointB);
		      break;
	      default:
		      break;
      }
            if (tmp<minDistances[clusterIDA] || minDistances[clusterIDA]==0){
              minDistances[clusterIDA]=tmp;
            }
            minDistances[clusterIDB]=tmp;

          }
        }
        return retValue;
      }
      else return false;
    }

    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.

    /*
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    template<class T>
    bool DBScan<T>::classify(const dvector& feature, outputVector& result) const{
      typename std::vector<dKdTree*>::const_iterator itCluster;
      dElement elem;
      const parameters& param=getParameters();
      bool ok=true;
      dvector dists(kdClusterVec.size());
      int i=0;
      // should be a parameter
      const double lambda=0.333333333;
      double tmp=0;
      for(itCluster=kdClusterVec.begin();
          itCluster!=kdClusterVec.end();
          itCluster++, i++) {
        (*itCluster)->searchNearest(feature,elem);
        switch (param.distanceMeasure){
            case parameters::L1Distance:
              tmp = cityBlockDistance(elem.point,feature);
              break;
            case parameters::L2Distance: 
              tmp = sqrt(distanceSqr(elem.point,feature));
              break;
            case parameters::L2SquareDistance:
              tmp = distanceSqr(elem.point,feature);
              break;
            default: 
              break;
        }
        double distance=minDistances[elem.data-1];
        dists.at(i)=exp(-tmp*tmp/(lambda*distance*distance));
      }
      ok = ok && outTemplate.apply(dists,result);
      result.setWinnerAtMax();
      return ok;
    }
  
    /*
     * DBScan algorithm with kdTree as datastucture
     */ 

    template<class T>
    bool DBScan<T>::dbScan(dKdTree& setOfPoints){
      // set clusterID to one, for first cluster
	    clusterID = 1;
      int i;

	    //walk through kdTree
	    cluster(setOfPoints.getRoot(), &setOfPoints);
      if(clusterID==1)
        return false;

      // build kd-tree for each cluster, they are stored in
      // kdClusterVec
        _lti_debug2("resize kdClusterVec\n");


      // contruct one kdTree for each remaining cluster.
      // These kdTrees are saved and later used for classifing the features.
      dKdTree* tmpTree;
      for(i=0; i<clusterID; i++) {
        tmpTree = new dKdTree;
        kdClusterVec.push_back(tmpTree);
      }

 	    _lti_debug2("Building kd-Tree for each cluster...\n");
	    buildKdClusterVec(setOfPoints.getRoot());

      _lti_debug2("Build kd-Tree using method build...");
	    // build kd-Trees in kdClusterVec
	    for(i=0; i<clusterID; ++i){
	      kdClusterVec[i]->build(2);
	    }
	    _lti_debug1("done\n");

	    // TODO what return value for DBScan???
	    // how to implement returnvalue bool????
	    return true;

    }


    // protected member funktions

    template <class T>
    void DBScan<T>::cluster(dNode* nPtr, 
                            dKdTree* setOfPoints){
	// one point of all elements
      dElement* point;
      // if node isLeaf, iterate over all elements of the leave
      // if they are not allready proccessed
//	if(nPtr->isLeaf() && !allElementsClustered(nPtr)){
      if(nPtr->isLeaf()){
        _lti_debug("Node is leaf !\n");
        typename std::vector<dElement* >::iterator eIt;
        for(eIt = nPtr->points.begin(); 
            eIt < nPtr->points.end(); eIt++){
          // get point of the leaf
          point = *eIt;
          // if point is unclassified, find points in eps neighborhood
          // using fkt. expandCluster
          if(point->data == -10){ // magic value for unclassified
            _lti_debug("expanding clusters ... \n");
            if(expandCluster(setOfPoints, point, clusterID)){
              _lti_debug( "Finished cluster \n" << clusterID << "\n");
              clusterID++;
            }
          }
        }
      }
      else {
        // got to child node
        // check if it exist if so, check if all elements of this
        // leaf have allready been proccessed
        if(nPtr->left != 0){
          cluster(nPtr->left, setOfPoints);
        }
        if(nPtr->right != 0){
          cluster(nPtr->right, setOfPoints);
        }
      }
    }

    /*
     * expandCluster, tries to expand the cluster found, with respect of
     * eps, minPts, density-reachability and desity-connectivity
     */
    template<class T>
    bool DBScan<T>::expandCluster(dKdTree* setOfPoints,
				  dElement* point,
				  int clusterID){
      
      // helper variable for deleting current point
      bool foundPoint = false;
      std::list<dElement* > seedsList;
      // get points in the eps neighborhood of point
      setOfPoints->searchWithin(point->point, eps, seedsList);
      if (seedsList.size() < (unsigned)minPts) {
        _lti_debug("not enough point in eps region\n");
        // change ClID of point (public attribute) or memberfunction
        // for kd-Tree clusterID = data
        point->data=0; // magic value for noise
        return false;
      } else {
        // all points in seeds are densityreachable from point seeds.
        // change clusterID for all points in seedsmap to clusterID
        _lti_debug("All points in seeds densityreachable, changing CID..\n"
                   << "Size of seeds: " << seedsList.size() << std::endl
                   << "Used Cluster ID: " << clusterID << std::endl);
        typename std::list<dElement* >::iterator itSeeds, itTmp;
        
        itSeeds = seedsList.begin();
        // iterate over seedsList
        // work arround for < operatore which is not 
        // implemented in dNode 
        // and dElement
        _lti_debug("Iterate over seedsList... \n");
        for(unsigned int c=0; c < seedsList.size(); ++c){
          // change cluster ID for points in seedsList
          (*itSeeds)->data=clusterID;
          // remove point form seeds list Part 1
          if(point->data == (*itSeeds)->data &&
             point->point == (*itSeeds)->point){
            itTmp = itSeeds;
            foundPoint = true;
          }
          _lti_debug("New clusterID: "<<(*itSeeds)->data <<"\n");
          itSeeds++;
        }
        _lti_debug("done\n");
        // remove point from seeds list Part 2
        if(foundPoint){
          _lti_debug2("Removing point from seedsList\n");
          seedsList.erase(itTmp);
        }
        //}
        
        // helper for next part of algorithm
        // dElement* resultPoint;
        std::list<dElement* > results;
        // workarround for pop_front which does not work
        typename std::list<dElement* >::iterator itP, itResults, itResults_end;
        _lti_debug1("Using points in seedsList as new seeds...\n");
        while(!seedsList.empty()){
          // iterator pointing to first element of list
          // redefined each time because we delete element
          itP = seedsList.begin();
          setOfPoints->searchWithin((*itP)->point, eps, results);
          _lti_debug("Found " << results.size() << " points in eps " 
                      << "neighborhood\n");
          if(results.size() >= (unsigned)minPts)
          {
            for(itResults=results.begin(),itResults_end=results.end();itResults!=itResults_end;++itResults)
            {
              if((*itResults)->data == 0 || (*itResults)->data == -10)
              {
                // if unclassified insert into seedsList
                if((*itResults)->data == -10)
                {
                  _lti_debug2( "itR point size: " << (*itResults)->point.size() <<"\n");
                  seedsList.push_back(*itResults);
                  _lti_debug2("Inserted point in seedsList " << " Size now: " << static_cast<int>(seedsList.size()) <<"\n");
                }
                // change cluster ID of result point
                (*itResults)->data = clusterID;
              }
            } // end for results
          }

          //if(results.size() >= (unsigned)minPts){
          //  itR = results.begin();
          //  // iterate over results
          //  _lti_debug( "Iterate over results Map ...\n");
          //  // check if points in result are allready classified or noise
          //  // if not add the point to the seeds map
          //  for(unsigned int r=0; r<results.size(); ++r){
          //    resultPoint = *itR;
          //    itR++;
          //    // check if point is noise or unclassified
          //    if(resultPoint->data == 0 ||
          //       resultPoint->data == -10){
          //      // if unclassified insert into seedsList
          //      if(resultPoint->data == -10){
          //        seedsList.push_back(*itR);
          //        _lti_debug2("Inserted point in seedsList "
          //                    << "New Size now: " << seedsList.size() <<"\n");
          //      }
          //      // change cluster ID of result point
          //      resultPoint->data = clusterID;
          //    }
          //    
          //  } // end for results
          //}
          results.clear();
          // delete used element of seedsList
          seedsList.erase(itP);
        } // end while seedsList
        return true;
        
      }
      return false; //never reached;
    }
  
    

    /*
     * check if elements of node/leaf have already been clustered
     */
    template <class T>
    bool DBScan<T>::allElementsClustered(dNode* nPtr){
	if(nPtr->isLeaf()){
	    typename std::vector<dElement* >::iterator eIt;
	    bool retValue;
	    retValue = true;
	    for(eIt = nPtr->points.begin(); eIt < nPtr->points.end(); eIt++){
		if((*eIt)->data == -10) retValue = false;
	    }
	    return retValue;
	}else{
	    return false;
	}
    }

#ifdef _LTI_DEBUG
    /*
     * showClustering points
     */
    template <class T>
    void DBScan<T>::showClustering(dNode* nPtr, 
                                   dKdTree& Tree){
	// one point of all elements
	dElement* elem;
	// if node isLeaf, iterate over all elements of the leave
	if(nPtr->isLeaf()){
	    typename std::vector<dElement* >::iterator eIt;
	    for(eIt = nPtr->points.begin(); 
		eIt < nPtr->points.end(); eIt++){
		// get one point of the leaf
		elem = *eIt;
		std::cout << "Point (";
		typename vector<double>::iterator itV;
		for(itV = (elem->point).begin();itV < (elem->point).end(); itV++){
		    std::cout <<"| "<< *itV << "\t";
		}
		std::cout << ") ClusterID :";
		std::cout << elem->data << std::endl;
	    }
	}
	else {
	    if(nPtr->left != 0){
		showClustering(nPtr->left, Tree);
		if(nPtr->right !=0)
		    showClustering(nPtr->right, Tree);
	    }
	    else{
		if(nPtr->right != 0){
		    showClustering(nPtr->right, Tree);
		    if(nPtr->left !=0)
			showClustering(nPtr->right, Tree);
		}
	    }
		
	}
    }
#endif

    /*
     * iterate over setOfPoints and add all points belonging to one
     * cluster to the same kd-Tree in kdClusterVec
     *
     */
    template <class T>
    void DBScan<T>::buildKdClusterVec(dNode* nPtr){
	    // one point of all elements
	    dElement* elem;
      startElements.resize(clusterID-1);
	    // if node isLeaf, iterate over all elements of the leave
	    if(nPtr->isLeaf()){
	      typename std::vector<dElement* >::iterator eIt;
	      for(eIt = nPtr->points.begin(); eIt < nPtr->points.end(); eIt++){
		        // get one point of the leaf
		        elem = *eIt;
		        kdClusterVec[(elem->data)]->add(elem->point,elem->data);
		        // save one element of each cluster into a vector (startElements)
		        // this information is needed to calculate the minimum
		        // distance between the clusters
		        if(startElements[(elem->data)] == 0){
		          startElements[(elem->data)] = elem;
		        }
        }
	    }
	    else {
	      if(nPtr->left != 0){
	        buildKdClusterVec(nPtr->left);
	        if(nPtr->right !=0)
	          buildKdClusterVec(nPtr->right);
	      }
	      else{
	        if(nPtr->right != 0){
	          buildKdClusterVec(nPtr->right);
	          if(nPtr->left !=0)
		          buildKdClusterVec(nPtr->right);
	        }
	      }
	    }
    }    
}

#include "ltiUndebug.h"
