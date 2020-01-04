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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiKMeansClustering.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 05.10.2001
 * revisions ..: $Id: ltiKMeansClustering.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#include "ltiCentroidClustering.h"


#ifndef _LTI_KMEANSCLUSTERING_H_
#define _LTI_KMEANSCLUSTERING_H_

namespace lti {

  /**
   * This class implements two versions of k-means-clustering: batch
   * and sequential. <p> Both methods are initailized by drawing
   * numberOfClusters training points and assigning them as
   * centroids. The batch version continues by labeling each training
   * point with the centroid it belongs to and then calculating new
   * centroids as mean average of the data belonging to each
   * cluster. This is continued until a convergence criterion is
   * met. <p> The sequential algorithm performs one step of batch
   * training. After that training points are analyzed in random
   * order. Whenever one data point switches clusters the centroids of
   * the old and new cluster of that data point are
   * recalculated. Calculation is also stoped by a convergence
   * criterion. <p> For both methods the convergence criterion is that
   * for one run through the training set none of the training points
   * switched clusters.
   */
  class kMeansClustering : public centroidClustering {

  public:

    /**
     * Parameters for kMeansClustering. Only batch and sequential are allowed
     * as values for clusterMode. Only other parameter is numberOfClusters.
     */
    class parameters : public centroidClustering::parameters {

    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters %object to be copied
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
       * copy the contents of a parameters %object
       * @param other the parameters %object to be copied
       * @return a reference to this parameters %object
       */
      parameters& copy(const parameters& other);

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

      /** the number of clusters (default 2)*/
      int numberOfClusters;
    };



    /**
     * default constructor
     */
    kMeansClustering();

    /**
     * copy constructor
     * @param other the %object to be copied
     */
    kMeansClustering(const kMeansClustering& other);

    /**
     * destructor
     */
    virtual ~kMeansClustering();

    /**
     * returns the name of this type ("kMeansClustering")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor %object
     */
    kMeansClustering& copy(const kMeansClustering& other);

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Performs batch or sequential training according to
     * the value of parameters::clusterMode.
     * @param data the training points
     */
    virtual bool train(const dmatrix& data);

    /** calls centroidClustering::train(const dmatrix&, ivector&) */
    virtual bool train(const dmatrix& input,
                       ivector& ids);

  protected:

    /**
     * returns current parameters. (non const! -> protected)
     */
//  parameters& getParameters() {return *params;};

    /**
     * Performs batch k-means clustering, see class description.
     */
    bool trainBatch(const dmatrix& data);

    /**
     * Performs sequential k-means clustering, see class description.
     */
    bool trainSequential(const dmatrix& data);

  };

}

#endif

