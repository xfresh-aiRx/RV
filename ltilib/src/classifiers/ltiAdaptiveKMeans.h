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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiAdaptiveKMeans.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 28.2.2002
 * revisions ..: $Id: ltiAdaptiveKMeans.h,v 1.8 2005/06/16 09:32:39 doerfler Exp $
 */

#ifndef _LTI_ADAPTIVE_K_MEANS_H_
#define _LTI_ADAPTIVE_K_MEANS_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiCentroidClustering.h"

namespace lti {

  /**
   *  this class implements a k-means clustering that determines the number of
   *  clusters by itself <p> The algorithm is based on Kothari R., Pitts D.,
   *  1998. On finding the number of clusters. Pattern Recognition Letters 20,
   *  405-416. <p> The algorithm determines the number of cluster in the given
   *  data and performs a k means clustering the the detected number of
   *  clusters on it. When the algorithm is executed, is does a lot of k means
   *  clusterings, with an additional term that brings the cluster centers
   *  together. Clusters that have too few points are deleted and clusters
   *  that are two close to each other are combined, according to the actual
   *  neighborhood. The neighborhood is varied between the values of the
   *  parameters minNeighborhood and maxNeighborhood, in as many steps as
   *  parameter nbNeighborhoods. If parameter detectNeighborhood is true,
   *  the paramters minNeighborhood and maxNeighborhood are set by the
   *  algorithm.The Parameter minNumberOfPoints is the minimal number of
   *  points that must belong to a cluster, otherwise the cluster will be
   *  deleted. maxIterations is the maximum number of Iterations before the
   *  clusters are viewed to be stable. maxDistance is the maximal distance
   *  that each component of the centroids can move before they are viewed as
   *  stable. If every component moves in one iteration less than this
   *  parameter, the centroids are set to be stable.
   *  The Parameter nbClusters is the number of clusters the
   *  algorithm starts with. It has to be bigger than the real number
   *  clusters. If the learnRate is too big, so that the centroids don't
   *  converge, the algorithm halves the learn rate, restarts and sets a
   *  statusString.
   */
  class adaptiveKMeans : public centroidClustering {
  public:
    /**
     * the parameters for the class adaptiveKMeans
     */
    class parameters : public centroidClustering::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual classifier::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------


      /**
       * minimum neighborhood
       */
      double minNeighborhood;

      /**
       * maximum neighborhood
       */
      double maxNeighborhood;

      /**
       * the learn rate
       */
      double learnRate;

      /**
       * the increment of the neighborhood
       */
      int nbNeighborhoods;

      /**
       * minimum number of points in one cluster. If a cluster has less than
       * this the cluster will be deleted
       */
      double minNumberOfPoints;

      /**
       * number of clusters to start with
       */
      int nbClusters;

      /**
       * if true the parameters minNeighborhood, maxNeighborhood, increment
       * and maxDistance will be automaticly be detected
       */
      bool detectNeighborhood;

      /**
       * maximum number of iterations before the next neighborhood considered
       */
      int maxIterations;

      /**
       * if every component of the centroids moves less than this parameter
       * the centroids are stable
       */
      double maxDistance;

    };

    /**
     * default constructor
     */
    adaptiveKMeans();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    adaptiveKMeans(const adaptiveKMeans& other);

    /**
     * destructor
     */
    virtual ~adaptiveKMeans();

    /**
     * returns the name of this type ("adaptiveKMeans")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    adaptiveKMeans& copy(const adaptiveKMeans& other);

    /**
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    adaptiveKMeans& operator=(const adaptiveKMeans& other);

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
     * the other train method and then
     * calls classify(const dvector&) to get the ids for each
     * trainvector. These ids are then returned.
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
//    virtual bool
//    classify(const dvector& feature, outputVector& result) const;

      virtual const ivector& getClusterSpreading() const;

  protected:
      ivector clusterSpreading;
  };
}

#endif

