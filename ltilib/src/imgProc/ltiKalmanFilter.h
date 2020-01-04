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
 * file .......: ltiKalmanFilter.h
 * authors ....: Joerg Zieren
 * organization: LTI, RWTH Aachen
 * creation ...: 26.3.2001
 * revisions ..: $Id: ltiKalmanFilter.h,v 1.20 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_KALMAN_FILTER_H_
#define _LTI_KALMAN_FILTER_H_

#include "ltiIoHandler.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiMatrixInversion.h"
#include "ltiObject.h"

namespace lti {
  /**
   * A discrete linear Kalman %filter implementation. Kalman filters
   * can be used to estimate the state of a dynamic system from noisy
   * measurement data.
   *
   * This implementation uses (at some places) the nomenclature from
   * the following tech report:
   *
   * Theory: "An Introduction to the Kalmen Filter" by Greg Welch and
   * Gary Bishop, Department of Computer Science, University of North
   * Carolina at Chapel Hill.
   * http://www.cs.unc.edu/~welch/kalman/kalmanIntro.html */
  class kalmanFilter : public functor {
  public:

    /**
     * The parameters for the class kalmanFilter.
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor. All initial values (vectors and matrices)
       * are initialized empty, i.e. they have 0 or 0x0 dimensions.
       */
      parameters();

      /**
       * This constructor creates all initial values (vectors and
       * matrices) with matching dimensions, containing only zeros.
       */
      parameters(int systemDimension,
                 int measurementDimension,
                 int controlDimension);

      /**
       * copy constructor
       * @param other the parameters object to be copied
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
      virtual functor::parameters* clone() const;

      /**
       * Check the consistency of the parameters. The parameters are
       * consistent if certain dimensions match each other, e.g. the
       * dynamicsMatrix must be square and must have as many rows as
       * the system state.
       * Additional consistency checks are performed for each
       * measurement and control vector.
       */
      bool consistent() const;

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

      // --------------
      // the parameters
      // --------------

      /**
       * Initial system state (x). This value is used either for
       * \f$\hat{x}^{-}\f$ or \f$\hat{x}\f$, depending on whether the
       * first update is a measurement or a time update (respectively).
       */
      vector<float> initialSystemState;

      /**
       * matrix that describes system dynamics (A)
       */
      matrix<float> dynamicsMatrix;

      /**
       * matrix that describes influence of control input on
       * system state (rarely used at LTI) (B)
       */
      matrix<float> controlMatrix;

      /**
       * matrix that relates the system state to the
       * (expected) measurement (H)
       */
      matrix<float> measurementMatrix;

      /**
       * measurement %noise covariance (R)
       */
      matrix<float> measurementNoiseCovariance;

      /**
       * process %noise covariance (Q)
       */
      matrix<float> processNoiseCovariance;

      /**
       * Initial estimate error covariance (P). This value is used either for
       * \f$P^{-}\f$ or \f$P\f$, depending on whether the
       * first update is a measurement or a time update (respectively).
       */
      matrix<float> initialErrorCovariance;
    };

    /**
     * Default constructor.
     */
    kalmanFilter();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    kalmanFilter(const kalmanFilter& other);

    /**
     * destructor
     */
    virtual ~kalmanFilter();

    /**
     * returns the name of this type ("kalmanFilter")
     */
    virtual const char* getTypeName() const;

    /**
     * Perform a measurement update using the given measurement, followed
     * by a time update with a control vector of zero. This call results
     * in a new prediction available via getEstimate.
     */
    bool apply(const lti::vector<float>& measurement);

    /**
     * Perform a measurement update using the given measurement, followed
     * by a time update with the given control. This call results
     * in a new (a priori) prediction available via getEstimate.
     */
    bool apply(const lti::vector<float>& measurement,
               const lti::vector<float>& control);

    /**
     * Perform a time update without control input. This call
     * results in a new a priori prediction available via
     * getEstimate.
     */
    bool apply();

    /**
     * Perform a measurement update using the given measurement. This call
     * results in a new a posteriori prediction available via
     * getEstimateAposteriori. This method is identical to
     * apply(const lti::vector<float>&).
     */
    bool applyMeasurementUpdate(const lti::vector<float>& measurement);

    /**
     * Perform a time update using the given control. This call
     * results in a new a priori prediction available via
     * getEstimate.
     */
    bool applyTimeUpdate(const lti::vector<float>& control);

    /**
     * Perform a time update without control input. This call
     * results in a new a priori prediction available via
     * getEstimate.
     */
    bool applyTimeUpdate();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kalmanFilter& copy(const kalmanFilter& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Get the (a priori) system state estimate (\f$\hat{x}^{-}\f$).
     * This is the prediction one would normally be interested in.
     */
    const vector<float>& getEstimate() const;

    /**
     * Get the a posteriori system state estimate (\f$\hat{x}\f$).
     * This value is primarily of internal use and not of interest
     * to the user.
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
     * Reset the filter state (\f$x\f$) and error covariance (\f$P\f$)
     * to the initial values stored in the parameters.
     */
    void reset();

  protected:

    // vectors and matrices that describe this Kalman filter

    /** @name Internal State
     *  These are the internal state members of the Kalman filter, which
     *  are copied to by calling the copy() member
     */
    //@{

      /**
       * system state prior to measurement update (\f$\hat{x}^{-}\f$)
       */
      vector<float> systemStateApriori;

      /**
       * system state after measurement update (\f$\hat{x}\f$)
       */
      vector<float> systemStateAposteriori;

      /**
       * matrix of current Kalman gain (K)
       */
      matrix<float> kalmanGainMatrix;

      /**
       * matrix of a priori estimate error covariance (\f$P^{-}\f$)
       */
      matrix<float> errorCovarianceApriori;

      /**
       * matrix of a posteriore estimate error covariance (P)
       */
      matrix<float> errorCovarianceAposteriori;

    //@}

    /** @name Temporary
     */
    //@{
	    /** Temporary matrices and vectors
       */
      matrix<float> tempMatrix;
      matrix<float> tempMatrix2;
      vector<float> tempVector;
      vector<float> tempVector2;

	    /**
	     * matrixInversion used to invert matrices
	     */
	    matrixInversion<float> myMatrixInvertor;
    //@}


    /**
     * Perform time update (with control input). Rows of controlVector
     * must match columns of controlMatrix.
     */
    bool timeUpdate(const vector<float>& controlVector);

    /**
     * Perform time update (no control input)
     */
    bool timeUpdate();

    /**
     * Perform measurement update. Rows of measurementVector must
     * match rows of measurementMatrix.
     */
    bool measurementUpdate(const vector<float>& measurementVector);
  };
}

#endif

