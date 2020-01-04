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
 * file .......: ltiRobustEstimator.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 6.4.2004
 * revisions ..: $Id: ltiRobustEstimator.h,v 1.5 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_ROBUST_ESTIMATOR_H_
#define _LTI_ROBUST_ESTIMATOR_H_

#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiPointSetNormalization.h"
#include "ltiPointSetNormalizationType.h"

namespace lti {

  /**
   * A parent class for estimating a transform from sets of points in a robust
   * manner. This is archived be a Monte Carlo estimator for outlier removal
   * or iterative refinement of the transform to reduce the effects of noisy
   * input data (comming up soon).
   *
   * The Monte Carlo estimators work for any transformation estimation
   * algorithms whereas iterative refinenment will be defined for least
   * squares estimators only.
   *
   * @see lti::monteCarloEstimator, lti::ransacEstimator,
   * lti::transformEstimator
   */
  class robustEstimator : public functor {
  public:
    /**
     * The parameters for the class robustEstimator
     */
    class parameters : public functor::parameters {

      //some protected attributes must be accessed by the parent class
      friend class robustEstimator;

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
       * The number of iterations that are performed.
       *
       * Default: 50.
       */
      int numIterations;

      /**
       * Sets a new normalization method.
       * @param name the normalization method to be used.
       */
      void setNormalization(ePointSetNormalizationType name);

      /**
       * Sets a new normalization method. A copy of the functor will be
       * done (so it is useless to change the parameters of the given
       * functor instance, because the internal functor will never notice
       * the changes done to its "parent").
       * @param method the normalization method to be used.
       */
      void setNormalization(const pointSetNormalization& method);

      /**
       * Is a normalization method allocated ?
       * @return true is a normalization method is allocated
       * and false otherwise.
       */
      bool existsNormalization() const;

      /**
       * Gets the normalization method. If no method is allocated
       * an exception is thrown.
       */
      const pointSetNormalization& getNormalization() const;

    protected:
 
      /**
       * The method used to normalize the data.
       *
       * Default: homography8Dof. Caution: May change in the near future.
       */
      pointSetNormalization *normalization;

    };

    /**
     * Default constructor
     */
    robustEstimator();

    /**
     * Construct a functor using the given parameters
     */
    robustEstimator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    robustEstimator(const robustEstimator& other);

    /**
     * Destructor
     */
    virtual ~robustEstimator();

    /**
     * Returns the name of this type ("robustEstimator")
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
    robustEstimator& copy(const robustEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    robustEstimator& operator=(const robustEstimator& other);

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
     * @param theParams the new normalization parameters, which will
     *                  be copied.
     * @return true on success and false otherwise, e.g. if the internal
     *         normalization method and the given parameters do not match.
     */
    virtual bool 
      setParameters(const pointSetNormalization::parameters& theParams);

    /**
     * Set the paramters of the robust estimator.
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

