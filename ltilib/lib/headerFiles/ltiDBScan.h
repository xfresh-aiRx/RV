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
 * revisions ..: $Id: ltiDBScan.h,v 1.11 2005/10/10 15:19:35 libuda Exp $
 */

#ifndef _LTI_D_B_SCAN_H_
#define _LTI_D_B_SCAN_H_

//TODO: include only those files which are needed in this header!!

// TODO: Check this include to parent class: 
#include "ltiClustering.h"
#include "ltiKdTree.h"
#include "ltiL2Distance.h"

// std / stl includes
#include <list>

 #undef _LTI_DEBUG
//#define _LTI_DEBUG 3 // comment out this line to remove all debug info.
 #include "ltiDebug.h"

namespace lti {

  /**
   * A class providing the DBScan for clustering
   *
   * DBScan is a density-based clustering algorithm, which is designed to find
   * clusters of abitrary shape. 
   *
   * Using one point the algorithm searches for minPts in the eps-neighborhood
   * of this point. If not enough points are found, they are classified as
   * "NOISE" (Value = 0) else it tries to expand this found cluster by using
   * each found point for another search and assigns the current cluster ID to
   * each point.  If then not enough points are found an new cluster is
   * created using an not jet classified point.  The clustering finishes if
   * all points are clussified, that means either having a cluster ID or
   * marked as "NOISE".
   *
   * The original algorithm uses a R*-Tree as internal data struckture, this
   * implementation uses the lti::kdTree.  The trees provide an efficient
   * method for searching points in an eps-neighborhood.
   *
   * DBScan requires three parameters:
   *
   * - \e buckedSize the number of elements stored in each leaf of the
   *   kd-Tree. The default value is 2.
   * - \e minPts the minimum number of points which have to be found in the
   *   eps neighborhood. The default value is 4.
   * - \e eps <b>IMPORTANT:</b> The size of the eps-neighborhood used for
   *   searching.  This parameter <b>MUST</b> be set by the user. The default
   *   value is 0 and if it isn't change, the algorithms returns false.
   *
   * The eps is the critical parameter of this algorithm, because the number
   * of clusters depends on it.  A good value would be the density of the
   * least dense cluster. But it is very hard to get this information on
   * advance.  Normally one does not know the distribution of the points in
   * the space. If no cluster is found, all points are marked as noise!
   *
   * The type T describes the distance measure used in the internal data 
   * structure. The Default value is l2SquareDistance
   * 
   * The original DBScan algorithm was introduced in:
   *
   * Martin Ester, Hans-Peter Kriegel, Joerg Sander, Xiaowei Xu A
   * Density-Based Algorithm for Discovering Clusters in Large Spatial
   * Databases with Noise. Proceedings of 2nd International Conference on
   * Knowledge Discovery and Data Mining (KDD-96)
   *
   * URL: http://www.dbs.informatik.uni-muenchen.de/Publikationen/Papers/
   * KDD-96.final.frame.ps
   */

  template <class T=l2SquareDistantor<vector<double> > >
  class DBScan : public clustering {
  public: 
    
    /**
     * the parameters for the class DBScan
     */
    class parameters : public clustering::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : clustering::parameters(){
        eps = double(1.0);
        minPts = int(6);
        buckedSize = int(4);
        distanceMeasure = L2SquareDistance;
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other)
        : clustering::parameters() {
        copy(other);
      }

      /**
       * destructor
       */
      virtual ~parameters(){}

      /**
       * returns name of this type
       */
      const char* getTypeName() const{
	  return "DBScan:parameters";
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other){
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        clustering::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        clustering::parameters& (clustering::parameters::* p_copy)
          (const clustering::parameters&) =
          clustering::parameters::copy;
        (this->*p_copy)(other);
#    endif
        eps = other.eps;
        minPts = other.minPts;
	buckedSize = other.buckedSize;
        distanceMeasure = other.distanceMeasure;
        return *this;
      };


      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other){
	  return copy(other);
      };


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual classifier::parameters* clone() const{
	  return new parameters(*this);
      };

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
# ifndef _LTI_MSC_6
      bool write(ioHandler& handler,const bool complete=true) const
# else
      bool writeMS(ioHandler& handler,const bool complete=true) const
# endif
        {
          bool b = true;
          if (complete) {
            b = handler.writeBegin();
          }
          
          if (b) {
            lti::write(handler,"eps",eps);
            lti::write(handler,"minPts",minPts);
            lti::write(handler,"buckedSize",buckedSize);
            if (distanceMeasure==L1Distance) {
              lti::write(handler,"distanceMeasure","L1Distance");
            } else if (distanceMeasure==L2Distance) {
              lti::write(handler,"distanceMeasure","L2Distance");
            } else {
              lti::write(handler,"distanceMeasure","L2SquareDistance");
            }
          }
          
#     ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not able to
          // compile...
          b = b && clustering::parameters::write(handler,false);
#     else
          bool (clustering::parameters::* p_writeMS)(ioHandler&,
                                                    const bool) const =
	    clustering::parameters::writeMS;
          b = b && (this->*p_writeMS)(handler,false);
#     endif
          
          if (complete) {
            b = b && handler.writeEnd();
          }
          
          return b;
        }
      
#     ifdef _LTI_MSC_6
      virtual bool write(ioHandler& handler,
                         const bool complete = true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif



      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
#     ifndef _LTI_MSC_6
      virtual bool read(ioHandler& handler,const bool complete = true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
          lti::read(handler,"eps",eps);
          lti::read(handler,"minPts",minPts);
	  lti::read(handler,"buckedSize",buckedSize);
          std::string tmpStr;
          lti::read(handler,"distanceMeasure",tmpStr);
          if (tmpStr=="L1Distance") {
            distanceMeasure=L1Distance;
          } else if (tmpStr=="L2Distance") {
            distanceMeasure=L2Distance;
          } else {
            distanceMeasure=L2SquareDistance;
          }

        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && clustering::parameters::read(handler,false);
#     else
        bool (clustering::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          clustering::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#      endif



//#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
//      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
//      bool writeMS(ioHandler& handler,const bool complete=true) const;
//#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      enum eDistanceType {
        L1Distance,
        L2Distance,
        L2SquareDistance
      };
      
      /**
       * Define eps region for a point, where neighbour points must be in
       */
      double eps;
      
      /**
       * Number of points which have to be in the eps-neighbourhood
       * otherwise the points are defined as noise
       */
      int minPts;
      
      /**
       * number of elements stored in each leaf of the kd-tree,
       * which is used als internal datastructure of DBScan
       */
      int buckedSize;

      /**
       * The kind of distance measurement that is used for computing the
       * distances between the data points.
       * Values are: l1Distance, l2Distance, l2SquareDistance
       */
      eDistanceType distanceMeasure;

    };
      
    //--------------------------------------------------------
    //       class DBSCan
    //--------------------------------------------------------

    /**
     * default constructor
     * reads and sets the parameters
     */
    DBScan();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    DBScan(const DBScan& other);

    /**
     * destructor
     */
    virtual ~DBScan();

    /**
     * returns the name of this type ("DBScan")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    DBScan& copy(const DBScan& other);

    /**
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    DBScan& operator=(const DBScan& other);

    /**
     * returns a pointer to a clone of this clustering.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Unsupervised training.  The vectors in the <code>input</code>
     * matrix will be put into groups according to the training
     * algorithm.  Additionally, an integer indicating the class each
     * point belongs to is returned. <p> By default this method uses
     * the other train method train(const dmatrix&) and then calls
     * classify(const dvector&,outputVector&) to get the ids for each
     * train vector. These ids are then returned.
     * Train will return false, if no cluster is found and all points are 
     * marked as noise!
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       ivector& ids);

    /**
     * Unsupervised training.
     * The vectors in the <code>input</code> matrix
     * will be clustered using each specific method.
     * Train will return false, if no cluster is found.
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input);

    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.
  
    /** classify
     * @param feature vector to be classified
     * @param result result as described above
     * @return true if successful, false otherwise
     */
    virtual bool classify(const dvector& feature, outputVector& result) const;
    


#ifdef _LTI_DEBUG
    /**
     * showClustering for 2D points
     */
    void showClustering(typename kdTree< vector<double>,int,T >::node* nPtr,
                        kdTree< vector<double>,int,T >& Tree);

#endif

  protected:

    // typedefs
    typedef kdTree< vector<double>,int,T > dKdTree;

    typedef typename dKdTree::node dNode;

    typedef typename dKdTree::element dElement;
    
    /**
     * DBScan algorithm working with the kdTree as data struckture
     * @param setOfPoints a kd-Tree containing the data to be clustered
     */
    virtual bool dbScan(dKdTree& setOfPoints);

    /**
     * The dbScan algorithm. 
     * walks rekursively through the tree, if one point ist found, the
     * clustering is started
     */
    void cluster(dNode* nPtr, 
                 dKdTree* setOfPoints);
    
    /**
     * expandCluster tries to expand the cluster found, with respect of
     * eps, minPts, density-reachability and desity-connectivity
     * @param setOfPoints pointer to the used kd-Tree
     * @param point one point/element of the data stored in the kd-Tree
     * @param clusterID the ID of the current cluster
     * returns true if the cluster could be expanded
     */
    bool expandCluster(dKdTree* setOfPoints, 
		       dElement* point,
		       int clusterID);
    
    /**
     * check if elements of node have been clustered, if the node is a leaf
     * returns true if all elements are clusterd, else false
     */
    bool allElementsClustered(dNode* nPtr);
    
    /**
     * eps neighbourhood
     */
    double eps;
    
    /**
     * minPts
     */
    int minPts;
    
    /**
     * clusterID
     */
    int clusterID;

    /**
     * kd-tree used for effective search on elements
     * internal used data strukture
     */
    dKdTree setOfPoints;

    /**
     * bucked-size of the kd-tree
     * number of elements stored in one leaf
     * default value: 2
     */
    int buckedSize;

  private:

    /**
     * iterate over setOfPoints and add all points belonging to one
     * cluster to the same kd-Tree in kdClusterVec
     *
     */
    void buildKdClusterVec(dNode* nPtr);
    

    /**
     * vector of kd-trees, each kd-tree represents one cluster
     * used for classification of points
     * the vector is initialized after the clustering
     */
    std::vector<dKdTree*> kdClusterVec;

    /**
     * vector of kdTree::elements one element of each cluster
     */
    std::vector<dElement*> startElements;

    /**
     * minimal distance to other clusters for each cluster
     * the vector index corresponds to the clusterID-1
     */
    vector<double> minDistances;
  };
}
   
#include "ltiUndebug.h"


#endif

		
