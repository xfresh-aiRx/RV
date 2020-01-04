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
 * file .......: ltiKalmanTracker.h
 * authors ....: Joerg Zieren
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.2001
 * revisions ..: $Id: ltiKalmanTracker.h,v 1.23 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_KALMAN_TRACKER_H_
#define _LTI_KALMAN_TRACKER_H_

#include "ltiKalmanFilter.h"

namespace lti {

  /**
   * A tracker implementation making use of the class
   * kalmanFilter. It tracks a 2D point, using a system state vector
   * that contains x and y coordinate as well as x and y velocity
   * (denoted by vx and vy) and, optionally, x and y acceleration (ax and ay)
   * of this point. These state vectors correspond to the assumption of
   * constant velocity and constant acceleration, respectively. The state
   * vector is structured like this: (x,vx,y,vy),
   * or (x,vx,y,vy,ax,ay). The measured x and y coordinate are given
   * to the apply method, which returns the estimated x and y coordinates at
   * the next time step.
   *
   * These matrices (contained in the parameters) must be specified:
   *
   *   matrix<float> measurementNoiseCovariance (\f$R\f$; 2x2)<BR>
   *   matrix<float> processNoiseCovariance (\f$Q\f$; 4x4 / 6x6)
   *
   * These matrices (contained in the parameters) should be specified,
   * but are initialized to default values that should be sufficient for
   * a first try:
   *
   *   vector<float> initialSystemState (\f$\hat{x}^{-}\f$; 4 / 6)<BR>
   *   matrix<float> initialErrorCovariance (\f$P^{-}\f$ and \f$P\f$; 4x4 / 6x6)
   *
   * The parantheses contain notation and dimensionality of the matrices for
   * the constant velocity / constant acceleration case.
   */
  class kalmanTracker : public functor {
  public:

    /**
     * The parameters for the class kalmanTracker.
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Initializes all matrices/vectors (except for measurement
       * and process noise covariance) for an n-dimensional system state.
       * Currently n may be 4 or 6. Default is 4.
       */
      parameters(const int& stateDimensions = 4);

      /**
       * copy constructor
       * @param other the parameters %object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

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
       * copy the contents of a parameters %object
       * @param other the parameters %object to be copied
       * @return a reference to this parameters %object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual lti::functor::parameters* clone() const;

      /**
       * The initial system state (used to initialize the tracker)
       * (4x1 / 6x1 for constant-velocity/constant-acceleration).
       * If you want to automatically initialize the tracker with the
       * first measurement, use autoInitialize. Default value is 0. Specifying
       * an initial system state is a good idea, but is not generally required.
       */
      vector<float> initialSystemState;

      /**
       * Measurement %noise covariance matrix (R; 2x2). This matrix
       * has no default value and must be supplied!
       */
      matrix<float> measurementNoiseCovariance;

      /**
       * Process %noise covariance matrix (Q; 4x4 / 6x6 for
       * constant-velocity/constant-acceleration). This matrix
       * has no default value and must be supplied!
       */
      matrix<float> processNoiseCovariance;

      /**
       * Initial value for the estimate error covariance matrix
       * (P; 4x4 / 6x6 for constant-velocity/constant-acceleration).
       * This value is used to initialize both the a priori and the a posteriori
       * error covariance (\f$P\f$ and \f$P^{-}\f$). Depending on whether the first
       * update is a time/measurement update, the latter/former will be overwritten.
       * This matrix has a default value of 1 (unity). It should be specified
       * considering the uncertainty of the supplied initial system state.
       */
      matrix<float> initialErrorCovariance;

      /**
       * AutoInitialization means that the tracker uses the first actual
       * measurement as initial system state (i.e.\ x and y are set to
       * the first measurement, while all other state variables are set to zero).
       *
       * Default is false.
       */
      bool autoInitialize;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      virtual bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      virtual bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif
    };

    /**
     * constructor for n-dimensional system state (n may be 4 or 6).
     * default is 4.
     */
    kalmanTracker(const int& stateDimensions = 4);

    /**
     * copy constructor
     * @param other the %object to be copied
     */
    kalmanTracker(const kalmanTracker& other);

    /**
     * destructor
     */
    virtual ~kalmanTracker();

    /**
     * returns the name of this type ("kalmanTracker")
     */
    virtual const char* getTypeName() const;

    /**
     * Return the number of system state dimensions (currently either
     * 4 or 6).
     */
    int getSystemStateDimensions() const;

    /**
     * Perform a measurement update and then a time update, resulting
     * in a prediction of x and y coordinates for the next time step,
     * which are then returned.
     */
    bool apply(const float& xMeasurement,
               const float& yMeasurement,
               float& xPredictionApriori,
               float& yPredictionApriori);

    /**
     * Perform only a time update (i.e. no new measurement available), resulting
     * in a prediction of x and y coordinates for the next time step,
     * which are then returned.
     */
    bool apply(float& xPredictionApriori,
               float& yPredictionApriori);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor %object
     */
    kalmanTracker& copy(const kalmanTracker& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters(void) const;

    /**
     * set parameters of this Kalman tracker
     */
    bool updateParameters();

    /**
     * set a new measurement %noise covariance, e.g. because future
     * measurements are less/more reliable (overlapping starts/ends)
     */
    void setMeasurementNoiseCovariance(const matrix<float>& r);

    /**
     * Get the (a priori) system state estimate (\f$\hat{x}^{-}\f$).
     * This is the prediction one would normally be interested in.
     */
    const vector<float>& getEstimate() const;

    /**
     * Get the a posteriori system state estimate (\f$\hat{x}\f$).
     */
    const vector<float>& getEstimateAposteriori() const;

    /**
     * Get the a priori error covariance (\f$P^{-}\f$).
     */
    const matrix<float>& getErrorCovarianceApriori() const;

    /**
     * Get the a posteriori error covariance (\f$P\f$).
     */
    const matrix<float>& getErrorCovarianceAposteriori() const;

    /**
     * Reset the filter state (\f$x\f$) and error covariance (\f$P\f$). The
     * tracker is left in an un-initialized state.
     */
    void reset(void);

    /**
     * Tell if this tracker is initialized, i.e. if any of the apply methods
     * has been called after the creation of this object or a call to reset().
     */
    bool isInitialized() const;

  protected:
    /**
     * The kalmanFilter used by this tracker.
     */
    kalmanFilter kalman;

    /**
     * Perform time update
     */
    bool performTimeUpdate();

    /**
     * Perform measurement update
     */
    bool performMeasurementUpdate(const float& xMeasurement,
                                  const float& yMeasurement);

    /**
     * Initialization status of this tracker. Set to false after object
     * creation  or call to reset(). Set to true after the first update,
     * i.e. call to an apply method.
     */
    bool initialized;

    /**
     * This temporary variable wraps two floats (the x/y measurement)
     * in a vector.
     */
    lti::vector<float> measurement;

    /**
     * This temporary variable wraps two floats (the x/y prediction)
     * in a vector.
     */
    lti::vector<float> prediction;
  };
}

#endif

