/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiMonteCarloEstimator.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 6.4.2004
 */

#ifndef _LTI_MONTE_CARLO_ESTIMATOR_H_
#define _LTI_MONTE_CARLO_ESTIMATOR_H_

#include "ltiRobustEstimator.h"
#include "ltiTransformEstimator.h"
#include "ltiTransformEstimatorType.h"

namespace lti {

  /**
   * A parent class for estimating a transform from point sets while
   * detecting and discarding outliers. To archive this a random set of
   * samples is drawn from the input data to estimate their transform. Common
   * Monte Carlo estimators are e.g. RANSAC and least median of squares
   * algorithm.
   *
   * @see lti::ransacEstimator, lti::transformEstimator
   */
  class monteCarloEstimator : public robustEstimator {
  public:
    /**
     * The parameters for the class monteCarloEstimator
     */
    class parameters : public robustEstimator::parameters {

      //some protected attributes must be accessed by the parent class
      friend class monteCarloEstimator;

    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
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
       * The maximum error for a single correspondence or the averaged
       * size of the residual. Depending on the parameter settings the
       * monteCarloEstimator normalizes the correspondences. This maximum error
       * is not adjusted to the scaled data automatically. Thus supplying a
       * meaningful value is left to the user.
       *
       * Default: .8.
       */
      double maxError;

      /**
       * Sets a new estimated transform.
       * @param name the transform to be estimated.
       */
      void setTransform(eTransformEstimatorType name);

      /**
       * Sets a new estimated transform. A copy of the functor will be
       * done (so it is useless to change the parameters of the given
       * functor instance, because the internal functor will never notice
       * the changes done to its "parent").
       * @param estimator the transform to be estimated.
       */
      void setTransform(const transformEstimator& estimator);

      /**
       * Is a transform estimator allocated ?
       * @return true is a transform estimator is allocated
       * and false otherwise.
       */
      bool existsTransform() const;

      /**
       * Gets the transform to be estimated. If no estimator is allocated
       * an exception is thrown.
       */
      const transformEstimator& getTransform() const;

    protected:
      /**
       * The transform to be estimated.
       *
       * Default: homography8Dof. Caution: May change in the near future.
       */
      transformEstimator *transform;
    };

    /**
     * Default constructor
     */
    monteCarloEstimator();

    /**
     * Construct a functor using the given parameters
     */
    monteCarloEstimator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    monteCarloEstimator(const monteCarloEstimator& other);

    /**
     * Destructor
     */
    virtual ~monteCarloEstimator();

    /**
     * Returns the name of this type ("monteCarloEstimator")
     */
    virtual const char* getTypeName() const;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets.
     * @param dest fector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual, also a
     *              if a squared residual is computed inside the transform
     *              estimator.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets.
     * @param dest dvector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets.
     * @param dest dvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual, also a
     *              if a squared residual is computed inside the transform
     *              estimator.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    monteCarloEstimator& copy(const monteCarloEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    monteCarloEstimator& operator=(const monteCarloEstimator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set the paramters of the transform estimator used internally.
     * @param theParams the new transform estimator parameters, which will
     *                  be copied.
     * @return true on success and false otherwise, e.g. if the internal
     *         transform estimator and the given parameters do not match.
     */
    virtual bool 
      setParameters(const transformEstimator::parameters& theParams);

    /**
     * Set the paramters of the transform estimator used internally.
     * @param theParams the new normalization parameters, which will
     *                  be copied.
     * @return true on success and false otherwise, e.g. if the internal
     *         normalization method and the given parameters do not match.
     */
    virtual bool 
      setParameters(const pointSetNormalization::parameters& theParams);

    /**
     * Set the paramters of the Monte Carlo estimator.
     * @param theParams the new parameters, which will be copied.
     * @return true on success and false otherwise.
     */
    bool setParameters(const parameters& theParams);

  protected:
    /**
     * Returns used parameters.
     */
    parameters& getWritableParameters();

  };
}

#endif

