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
 * file .......: ltiClustering.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 29.08.2001
 * revisions ..: $Id: ltiClustering.h,v 1.11 2005/06/16 09:32:40 doerfler Exp $
 */

#include "ltiObject.h"
#include "ltiUnsupervisedClassifier.h"


#ifndef _LTI_CLUSTERING_H_
#define _LTI_CLUSTERING_H_

namespace lti {

  /**
   * Base class for all clustering algorithms. Clustering algorithms
   * can follow different training strategies as indicated by the
   * parameter clusterMode. Representations of the clusters are
   * modelled in subclasses of this class, e.g. centroidClustering.
   */
  class clustering : public unsupervisedClassifier {

  public:

    /**
     * parameters for clustering functors.
     * Provides a clusterMode which is of type eClusterMode
     */
    class parameters : public unsupervisedClassifier::parameters {

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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Different methods for clustering data using basically the
       * same algorithm. Not all clusterModes must be available for
       * all clustering algorithms. See individual documentation. The
       * different modes have the following meaning: <p>
       * <dl>
       * <dt>batch</dt>
       * <dd>All data points must be available. Clusters are only updated
       *     after all available data has been considered.</dd>
       * <dt>sequential</dt>
       * <dd>Really sequential batch. Again all data must be available
       *     but clusters are updated after consideration of each data
       *     point. The update requires the knowledge of all other or
       *     previously considered data points.</dd>
       * <dt>online</dt>
       * <dd>Consideres the current data point only. Usually involves
       *     some learning rate etc.</dd>
       * <dt>miniBatch</dt>
       * <dd>A mix of sequential/online and batch. Build small batch blocks
       *     and do batch processing with them. Usually used instead of
       *     online to lessen effect of noise.</dd>
       * </dl>
       */
      enum eClusterMode {
        batch,
        sequential,
        online,
        miniBatch
      };

      /**
       * Kind of mode used for clustering. (Default batch)
       */
      eClusterMode clusterMode;

    };

    /**
     * default constructor
     */
    clustering();

    /**
     * copy constructor
     * @param other the %object to be copied
     */
    clustering(const clustering& other);

    /**
     * destructor
     */
    virtual ~clustering();

    /**
     * returns the name of this type ("clustering")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor %object
     */
    clustering& copy(const clustering& other);

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * train the clusterer with the vectors at the rows of input
     *
     * @param input the input data
     * @return true if successful, false otherwise.
     */
    virtual bool train(const dmatrix& input) =0;

    /**
     * train the clusterer with the vectors at the rows of input,
     * and return the cluster id for each of that vectors.
     *
     * @param input the input data
     * @param ids output vector where the cluster id per input vector will
     *        be stored.
     * @return true if successful, false otherwise.
     */
    virtual bool train(const dmatrix& input,
                       ivector& ids);


  };

}

#endif

