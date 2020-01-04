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
 * file .......: ltiRansacEstimator.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 14.4.2004
 * revisions ..: $Id: ltiRansacEstimator.h,v 1.4 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_RANSAC_ESTIMATOR_H_
#define _LTI_RANSAC_ESTIMATOR_H_

#include "ltiMonteCarloEstimator.h"

namespace lti {

  /**
   * This class estimates a transform using the Ransac algorithm. The
   * Ransac algorithm maximizes the number of inliers. At each
   * iteration a subset of points/correspondences is drawn from which the
   * transform is computed.
   *
   * Theoretically the Ransac algorithm copes with up to 90% outliers. It is
   * advised though, to verify the estimated transform and repeat the
   * ransac estimation in case of a bad estimation. For the homography
   * estimators this may be performed by the homographyVerfication
   * (coming up soon).
   *
   */
  class ransacEstimator : public monteCarloEstimator {
  public:
    /**
     * The parameters for the class ransacEstimator
     */
    class parameters : public monteCarloEstimator::parameters {
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
       * Adjust the degree of contamination after each successfull guess.
       * The contamination is only decreased and never increased.
       * This parameter effects the number of iterations performed.
       * The functor will always terminate after at most the maximum iterations
       * specified in the parameters, though.
       *
       * If adaptive contamination is on, the applies return true even if the
       * detected inliers suggest a contamination above the parametrized
       * contamination.
       *
       * Default: false.
       */
      bool adaptiveContamination;

      /**
       * The number of correspondences drawn at each trial to estimate the
       * transform. Literature advises to use the minimum number
       * correspondences that are required which is proved optimal under a
       * statistical context.
       *
       * Default: true.
       */
      int numCorrespondencesPerTrial;

      /**
       * If true the algorithm stops as soon as a transformation is found,
       * whose number of outliers is below the expected contamination. The
       * expected contamination is supplied as a parameter and not changed
       * adaptively.
       *
       * Default: true.
       */
      bool checkStop;

      /**
       * The expected degree of contamination.
       *
       * Default: .5
       */
      float contamination;

      /**
       * The number of trials in adaptive mode depends on the estimated
       * contamination and the confidence, under which the result is correct.
       *
       * Default: .99
       */
      float confidence;

      /**
       * If true the minimum required number of points/correspondences is
       * used no matter which value numCorrespondencesPerTrial has.
       *
       * Default: true.
       */
      bool useMinCorrespondences;

    };

    /**
     * Default constructor
     */
    ransacEstimator();

    /**
     * Construct a functor using the given parameters
     */
    ransacEstimator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    ransacEstimator(const ransacEstimator& other);

    /**
     * Destructor
     */
    virtual ~ransacEstimator();

    /**
     * Returns the name of this type ("ransacEstimator")
     */
    virtual const char* getTypeName() const;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  std::vector<vector<dpoint> > with the point sets.
     * @param dest fvector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest) const;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  std::vector<vector<dpoint> > with the point sets.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual, also a
     *              if a squared residual is computed inside the transform
     *              estimator.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error) const;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  std::vector<vector<dpoint> > with the point sets.
     * @param dest dvector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest) const;

    /**
     * Estimates a transform from the supplied point sets.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  std::vector<vector<dpoint> > with the point sets.
     * @param dest dvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual, also a
     *              if a squared residual is computed inside the transform
     *              estimator.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ransacEstimator& copy(const ransacEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ransacEstimator& operator=(const ransacEstimator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
    
  };
}

#endif

