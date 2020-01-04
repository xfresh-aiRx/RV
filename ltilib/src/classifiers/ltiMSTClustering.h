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
 * file .......: ltiMSTClustering.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 22.4.2003
 * revisions ..: $Id: ltiMSTClustering.h,v 1.13 2005/11/14 15:53:03 doerfler Exp $
 */

#ifndef _LTI_M_S_T_CLUSTERING_H_
#define _LTI_M_S_T_CLUSTERING_H_

#include "ltiWeightedGraph.h"
#include "ltiClustering.h"
#include "ltiKdTree.h"
#include "ltiL2Distance.h"

namespace lti {
  /**
   * This class implements a minimum spanning tree clustering.
   * 
   * First a minimum spanning tree of the input data is computed.
   * Then all edges that are unusually large compared the 
   * neighboring edges are removed. The clusters are the connected 
   * components of the minimum spanning tree after the removal of 
   * the inconsistent edges.
   *
   * As template parameter the distantor accoding to the distance measure 
   * you want to use has to be specified.
   */
  template<class U=l2SquareDistantor<dvector> >
  class MSTClustering : public clustering {
  public:
    /**
     * the parameters for the class MSTClustering
     */
    class parameters : public clustering::parameters {
    public:

      /** 
       * The different methods that can be used to compute a probability 
       * from a distance.
       */ 
      enum eProbabilityFromDistanceTypes {
        InterClusterDistanceIndependend,
        InterClusterDistanceDependend
      };

      /**
       * default constructor
       */
      parameters() : clustering::parameters() {   
        devFac = 2.0;
        nbMaxSteps = 4;
        variance = 0.333333;
        probabilityFromDistanceMode = InterClusterDistanceDependend;
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : clustering::parameters() {
        copy(other);
      }

      /**
       * destructor
       */
      ~parameters() {
      }

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "MSTClustering<U>::parameters";
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        clustering::parameters::copy(other);
# else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        clustering::parameters& (clustering::parameters::* p_copy)
          (const clustering::parameters&) =
          clustering::parameters::copy;
        (this->*p_copy)(other);
# endif

        devFac = other.devFac;
        nbMaxSteps = other.nbMaxSteps;
        variance = other.variance;
        probabilityFromDistanceMode = other.probabilityFromDistanceMode;


        return *this;
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual classifier::parameters* clone() const {
        return new parameters(*this);
      }

# ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete=true) const
# else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"devFac",devFac);
          lti::write(handler,"nbMaxSteps",nbMaxSteps);
          lti::write(handler,"variance",variance);
          if (probabilityFromDistanceMode==InterClusterDistanceIndependend) {
            lti::write(handler,"probabilityFromDistanceMode",
                       "InterClusterDistanceIndependend");
          } else {
            lti::write(handler,"probabilityFromDistanceMode",
                       "InterClusterDistanceDependend");
          }

        }

# ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && clustering::parameters::write(handler,false);
# else
        bool (clustering::parameters::* p_writeMS)(ioHandler&,const bool) const =
          clustering::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
# endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

# ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,const bool complete=true) const {
        // ...we need this workaround to cope with another really awful MSVC bug.
        return writeMS(handler,complete);
      }
# endif

# ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true)
# else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
      
          lti::read(handler,"devFac",devFac);
          lti::read(handler,"nbMaxSteps",nbMaxSteps);
          lti::read(handler,"variance",variance);
          std::string str;
          lti::read(handler,"probabilityFromDistanceMode",str);
          if (str=="InterClusterDistanceIndependend") {
            probabilityFromDistanceMode=InterClusterDistanceIndependend;
          } else {
            probabilityFromDistanceMode=InterClusterDistanceDependend;
          }
        }

# ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && clustering::parameters::read(handler,false);
# else
        bool (clustering::parameters::* p_readMS)(ioHandler&,const bool) =
          clustering::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
# endif

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

# ifdef _LTI_MSC_6
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
# endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Factor that is used to decide if an edge is inconsistent.
       * First all edges are searched that lie at most nbMaxSteps away 
       * from the current node. Then m is the mean of the weights of all 
       * these edges and sigma is the standard deviation of these edges.
       * An Edge is considered to be in consistent if is weight is bigger than 
       * mean+devFac*sigma.
       * Default is 2.0
       */
      float devFac;

      /**
       * The maximum number of edges that can be between the starting point
       * and the last edge that is considered when computed inconsistent edges.
       * Default is 4.
       */
      int nbMaxSteps;

      /** 
       * Variance used to compute a possibility from the distances 
       * for classification. 
       * While classification, the membership of feature to all clusters 
       * is computed. This done by the following: 
       * Say d is the minimal distance between the feature that is classified
       * and one of the clusters. Say also w is the the minimal distance 
       * between the current cluster the nearest neighboring cluster, 
       * then the membership is defined either 
       * \f$ e^{-\frac{d^2}{\sigma^2w^2}} \f$ or 
       * \f$ e^{-\frac{d^2}{\sigma^2}} \f$ depending on the value of 
       * probabilityFromDistanceMode
       * Default is 0.33333.
       */
      double variance;

      /** 
       * The method used to compute a probabilty from the distance between 
       * the feature that is classified and the clusters. 
       * Valid valued are: InterClusterDistanceDependend and 
       * InterClusterDistanceIndependend.
       * See parameter variance
       */ 
      eProbabilityFromDistanceTypes probabilityFromDistanceMode;
    };

    /**
     * default constructor
     */
    MSTClustering();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    MSTClustering(const MSTClustering& other);

    /**
     * destructor
     */
    virtual ~MSTClustering();

    /**
     * returns the name of this type ("MSTClustering")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    MSTClustering& copy(const MSTClustering& other);

    /**
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    MSTClustering& operator=(const MSTClustering& other);

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
     * the other train method train(const dmatrix&) and then
     * calls classify(const dvector&) to get the ids for each
     * train vector. These ids are then returned.
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
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input);

    /**
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool
      classify(const dvector& feature, outputVector& result) const;

  /**
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
   virtual bool write (ioHandler &handler, const bool complete=true) const;

  /**
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was successful
   */
  virtual bool read (ioHandler &handler, const bool complete=true);

  protected:
    /** 
     * collects the weights of all edge starting from node with id b 
     * until it reaches the given depth. a is used to make sure that 
     * the function does not run back the way same way he came from.
     */
    void collectDists(const int a,const int b,
                     std::list<std::pair<int,int> >& dists,int depth);

    /**
     * searchs for clusters in the graph after the inconsistent edges are 
     * deleted.
     */  
   void findClusters(const int from,int& curId,int& nextId,
                   weightedGraph<vector<double>,int,double>::node*& startNode,
                     vector<int>& ids,std::vector<int>& startNodes);
    /** 
     * The graph that contains the tree and the edges.
     */
   weightedGraph<vector<double>,int,double> gr1;

    /**
     * After the training process the vector contains under each id a 
     * kdTree with all points that are in the cluster with the id 
     * corresponding with the position in the vector
     */
   std::vector<kdTree<dvector,int,U >* > clusters;

    /** 
     * After the training process this map contains for each cluster 
     * with the given id the minimal distance to its nearest 
     * neighboring cluster.
     */
   std::map<int,double> minDistances;

    /** 
     * The distantor used computing distances.
     */
   U distantor;


  };
}

#include "ltiMSTClustering_template.h"

#endif

