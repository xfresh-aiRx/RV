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
 * file .......: ltiTransformEstimator.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 25.3.2004
 * revisions ..: $Id: ltiTransformEstimator.h,v 1.8 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_TRANSFORM_ESTIMATOR_H_
#define _LTI_TRANSFORM_ESTIMATOR_H_


#include "ltiFunctor.h"
#include "ltiPoint.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * A parent class for estimating a transform from sets of points.
   *
   * For automtic outlier detection please use one of the
   * Monte Carlo estimators, which call a transform estimator in their main
   * loop. The Monte Carlo estimators perform data normalization on request,
   * whereas this class does not.
   *
   * If you derive a class please remember to support the paramter for
   * returning a squared error measure (e.g. squared residual) instead of
   * the error (e.g. residual), and thus spare taking the square root.
   *
   * @see  lti::homography8DOFEstimator, lti::homography9DOFEstimator,
   * lti::fMatrixEstimator, lti::monteCarloEstimator
   *
   * Also planned: 
   * - estimating a homography together with a radial distortion
   */
  class transformEstimator : public functor {
  public:
    /**
     * The parameters for the class transformEstimator
     */
    class parameters : public functor::parameters {
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
       * For runtime efficiency a squared error (e.g. elementwise squared
       * residual) may be computed, which spares taking the square root.
       *
       * Default: false.
       */
      bool computeSqError;
    };

    /**
     * Default constructor
     */
    transformEstimator();

    /**
     * Construct a functor using the given parameters
     */
    transformEstimator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    transformEstimator(const transformEstimator& other);

    /**
     * Destructor
     */
    virtual ~transformEstimator();

    /**
     * Returns the name of this type ("transformEstimator")
     */
    virtual const char* getTypeName() const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimators use a random sampling
     * approach. Some estimators consider all input points and weight them
     * according to their deviation from the transform computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *             image stand in a row. The correspondences in another image
     *             stand in the according columns.  All points of one point
     *             set give a matrix row, whereas all elements of a specific
     *             correspondence stand in a matrix column.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest, fvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. Usually this
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual. All correspondences are
     *              considered, discarding the valid indices.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest, fvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimators use a random sampling
     * approach. Some estimators consider all input points and weight them
     * according to their deviation from the transform computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *             image stand in a row. The correspondences in another image 
     *             stand in the according columns.
     *             All points of one point set give a matrix row, whereas all
     *             elements of a specific correspondence stand in a matrix
     *             column.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. Usually this
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual. All correspondences are
     *              considered, discarding the valid indices.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimators use a random sampling
     * approach. Some estimators consider all input points and weight them
     * according to their deviation from the transform computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param error dvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param indices which rows of the src matrix have to be considered.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. Usually this
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param error dvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the transform could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Compute the residual for the given correspondences and transformation
     * vector.
     * @param src matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param transform fvector with the transformation
     * @param dest      fvector with the residual
     * @return true on success and false otherwise.
     */
    virtual bool computeResidual(const matrix<fpoint >& src,
                                    const fvector& transform,
                                    fvector& dest) const = 0;

    /**
     * Compute the residual for the given correspondences and transformation
     * vector.
     * @param src matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param transform dvector with the transformation
     * @param dest      dvector with the residual
     * @return true on success and false otherwise.
     */
    virtual bool computeResidual(const matrix<dpoint >& src,
                                    const dvector& transform,
                                    dvector& dest) const = 0;
    
    /**
     * Returns the minimum number of correspondences required to estimate
     * the transform.
     */
    virtual int minNumberCorrespondences() const = 0;

    /**
     * Returns the mininum dimension of a correspondence,
     * e.g. the minimum dimension of a correspondence pair is 2.
     * Each derived transform estimator only works on correspondences of
     * priori defined dimensions.
     */
    virtual int minCorrespondenceDimension() const = 0;

    /**
     * Returns the maximum dimension of a correspondence,
     * e.g. the maximum dimension of a correspondence pair is 2, whereas
     * transformEstimator running on n-tuples may allow an infinite number.
     * Each derived transform estimator only works on correspondences of
     * priori defined dimensions.
     */
    virtual int maxCorrespondenceDimension() const = 0;

    /**
     * A transform estimated on normalized data usually differs from
     * the transform of the original data. Considering the normalization
     * performed this methods computes the transform which applies to the
     * original data.
     * @param srcdest the normalized transform. The result will be left here
     *     too.
     * @param scale a vector containing the scale applied to each point set.
     * @param shift a vector containing the shift of each scaled point set.
     */
    virtual bool denormalize(fvector& srcdest,
                             const vector<fpoint>& scale,
                             const vector<fpoint>& shift) const = 0;
    
    /**
     * A transform estimated on normalized data usually differs from
     * the transform of the original data. Considering the normalization
     * performed this methods computes the transform which applies to the
     * original data.
     * @param srcdest the normalized transform. The result will be left here
     *     too.
     * @param scale a vector containing the scale applied to each point set.
     * @param shift a vector containing the shift of each scaled point set.
     */
    virtual bool denormalize(dvector& srcdest,
                             const vector<dpoint>& scale,
                             const vector<dpoint>& shift) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    transformEstimator& copy(const transformEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    transformEstimator& operator=(const transformEstimator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

