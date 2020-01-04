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
 * file .......: ltiFuzzyCMeans.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 5.2.2002
 * revisions ..: $Id: ltiFuzzyCMeans.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_FUZZY_C_MEANS_H_
#define _LTI_FUZZY_C_MEANS_H_


#include "ltiVector.h"
#include "ltiMatrix.h"

#include "ltiCentroidClustering.h"

namespace lti {
  /**
   *  this class implements the fuzzy C Means clustering algorithm.
   *  <p> the class arbitrary picks a given number of points as centroids.
   *  Between these centroids and the given points is a degree of membership
   *  calculated. According to these memberships new centroids are calculated.
   *  This is done until the convergence criterion is met. <p> The convergence
   *  criterion could be either the the maximum number of allowed iterations or
   *  the distance between the matrix of centroids of the last two iterations
   *  <B> the classify method is not implemented, instead the classify method
   *  of superclass is used which does not regard the memberships of the points
   *  to the centroids </B>
   */
  class fuzzyCMeans : public centroidClustering {
  public:
    /**
     * the parameters for the class fuzzyCMeans
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
       * bias the algorithm either towards hard clustering (nearby 1) or
       * fuzzy clustering (bigger 1); this parameter must be bigger than 1
       */
      double fuzzifier;

      /**
       * norm in which the distances are measured, valid options are L1 and L2
       */
      eDistanceMeasure norm;

      /**
       * the algorithm terminates if the distance between the new centroids
       * and the old centroids is smaller than epsilon
       */
      double epsilon;

      /**
       * maximum number of Iteration before the algorithm terminates
       */
      int maxIterations;

      /**
       * the number of clusters the algorithm creates
       */
      int nbOfClusters;

    };

    /**
     * default constructor
     */
    fuzzyCMeans();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fuzzyCMeans(const fuzzyCMeans& other);

    /**
     * destructor
     */
    virtual ~fuzzyCMeans();

    /**
     * returns the name of this type ("fuzzyCMeans")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    fuzzyCMeans& copy(const fuzzyCMeans& other);

    /**
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    fuzzyCMeans& operator=(const fuzzyCMeans& other);

    /**
     * returns a pointer to a clone of this clustering.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set the parameters of the classifier, calls setParameters of the
     * superclass and tests if the parameter norm is valid. If not a status
     * string is set.
     */
    bool setParameters(const classifier::parameters& theParams);

    /**
     * Unsupervised training.  The vectors in the <code>input</code>
     * matrix will be put into groups according to the training
     * algorithm.  Additionally, an integer indicating the class each
     * point belongs to is returned. <p> By default this method uses
     * the other train method train(const dmatrix&) and then
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

  };
}

#endif

