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
 * file .......: ltiHomographyEstimatorBase.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 25.3.2004
 * revisions ..: $Id: ltiHomographyEstimatorBase.h,v 1.7 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HOMOGRAPHY_ESTIMATOR_BASE_H_
#define _LTI_HOMOGRAPHY_ESTIMATOR_BASE_H_


#include "ltiTransformEstimator.h"
#include "ltiPoint.h"
#include "ltiMatrix.h"
#include "ltiHTypes.h"

namespace lti {
  /**
   * A parent class for estimating a transform that can be converted to a
   * homography. For example all transform estimators, whose result may
   * be used by geometricTransform, should be derived from this class. These
   * classes must provide an interface to convert their result to hMatrix3D.
   *
   * The results of all homography estimators can be improved and verified
   * by registering the image planes and computing a similarity measure,
   * lti::homographyVerification. To allow efficient computations inside
   * the homographyVerification, please implement a tranform-method.
   *
   * The robust estimatorBase performs data normalization on request, whereas
   * the derived classes do not.
   *
   * If you derive a class please remember to support the paramter for
   * returning a squared error measure (e.g. squared residual) instead of
   * the error (e.g. residual), and thus spare taking the square root.
   *
   * @see lti::transformEstimator, lti::homography8DOFEstimator
   */
  class homographyEstimatorBase : public transformEstimator {
  public:
    /**
     * The parameters for the class homographyEstimatorBase
     */
    class parameters : public transformEstimator::parameters {
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
    };

    /**
     * Default constructor
     */
    homographyEstimatorBase();

    /**
     * Construct a functor using the given parameters
     */
    homographyEstimatorBase(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    homographyEstimatorBase(const homographyEstimatorBase& other);

    /**
     * Destructor
     */
    virtual ~homographyEstimatorBase();

    /**
     * Returns the name of this type ("homographyEstimatorBase")
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
     * @param src matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *  
     * @param src matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if apply successful or false otherwise.
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
     * @param src matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if apply successful or false otherwise.
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
     * @param src matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
                       fvector& dest, fvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a homography from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimatorBases use a random sampling
     * approach. Some estimatorBases consider all input points and weight them
     * according to their deviation from the homography computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated homography.
     * @return true if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest) const = 0;

    /**
     * Estimates a homography from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<fpoint> with the point sets. All points of the same
     *             image stand in a row. The correspondences in another image
     *             stand in the according columns.
     * @param dest fvector the estimated homography.
     * @param error fvector containing the deviation of each point from the
     *              estimated homography. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error) const = 0;

    /**
     * Estimates a homography from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimatorBases using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated homography.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a homography from the supplied point sets, whereby only
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
     * @param dest fvector the estimated homography.
     * @param error fvector containing the deviation of each point from the
     *              estimated homography. Usually this is the residual or
     *              elementwise squared residual. All correspondences are
     *              considered, discarding the valid indices.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
                       fvector& dest, fvector& error,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a homography from the supplied point sets, where all
     * points are considered. Please implement efficient code using
     * iterators here. Not all robust estimatorBases use a random sampling
     * approach. Some estimatorBases consider all input points and weight them
     * according to their deviation from the homography computed at the prior
     * iteration.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated homography.
     * @return true if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest) const = 0;

    /**
     * Estimates a homography from the supplied point sets, where all
     * points are considered. Usually this method calls the apply without
     * the residual first, and then computes the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated homography.
     * @param error dvector containing the deviation of each point from the
     *              estimated homography. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest, dvector& error) const = 0;

    /**
     * Estimates a homography from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimatorBases using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src  matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated homography.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the homography could be estimated and false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
                       dvector& dest,
                       const ivector& indices,
                       int numCorrespondences) const = 0;

    /**
     * Estimates a homography from the supplied point sets, whereby only
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
     * @param dest dvector the estimated homography.
     * @param error dvector containing the deviation of each point from the
     *              estimated homography. Usually this is the residual or
     *              elementwise squared residual.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if the homography could be estimated and false otherwise.
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
     * the homography.
     */
    virtual int minNumberCorrespondences() const = 0;

    /**
     * Returns the mininum dimension of a correspondence,
     * e.g. the minimum dimension of a correspondence pair is 2.
     * Each derived homography estimatorBase only works on correspondences of
     * priori defined dimensions.
     */
    virtual int minCorrespondenceDimension() const = 0;

    /**
     * Returns the maximum dimension of a correspondence,
     * e.g. the maximum dimension of a correspondence pair is 2, whereas
     * homographyEstimatorBase running on n-tuples may allow an infinite number.
     * Each derived homography estimatorBase only works on correspondences of
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
    bool denormalize(fvector& srcdest,
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
    bool denormalize(dvector& srcdest,
                     const vector<dpoint>& scale,
                     const vector<dpoint>& shift) const = 0;

   /**
     * Converts the estimated vector into a hMatrix3D<float>, which is e.g.
     * used by geometricTransform.
     * @param src fvector the estimated transform.
     * @param dest hMatrix3D<float> the estimated transform as a hMatrix3D.
     *
     * @return true if successful or false otherwise.
     */
    virtual bool convert(const fvector& src, hMatrix3D<float>& dest) const = 0;
  
    /**
     * Converts the estimated vector into a hMatrix3D<float>, which is e.g.
     * used by geometricTransform.
     * @param src dvector the estimated transform.
     * @param dest hMatrix3D<float> the estimated transform as a hMatrix3D.
     *
     * @return true if successful or false otherwise.
     */
    virtual bool convert(const dvector& src, hMatrix3D<float>& dest) const = 0;
    
    /* Todo ?????
     * Converts the estimated vector into a matrix.
     *
     * @param src fvector the estimated transform.
     * @param dest fmatrix the estimated transform as a matrix.
     *
     * @return true if successful or false otherwise.
     */
    //virtual bool convert(const fvector& src, fmatrix& dest) const = 0;

    /* Todo ?????
     * Converts the estimated vector into a matrix.
     *
     * @param src dvector the estimated transform.
     * @param dest dmatrix the estimated transform as a matrix.
     *
     * @return true if successful or false otherwise.
     */
    //virtual bool convert(const dvector& src, dmatrix& dest) const = 0;

    /**
     * This is a fast short cut used by the special robust estimator
     * homographyVerification in the main loop.
     * Please implement this method inline.
     * @param pt fpoint the point to be transformed
     * @param transf fvector with the transformation, which e.g. was computed
     *               by an apply.
     * @return the transformed point
     */
    virtual fpoint transform(const fpoint& pt, 
                             const fvector& transf) const = 0;
 
    /**
     * This is a fast short cut used in the special robust estimator
     * homographyVerification in the main loop.
     * Please implement this method inline.
     * @param pt fpoint the point to be transformed
     * @param transf fvector with the transformation, which e.g. was computed
     *               by an apply.
     * @return the transformed point
     */
    virtual dpoint transform(const dpoint& pt, 
                             const dvector& transf) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    homographyEstimatorBase& copy(const homographyEstimatorBase& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    homographyEstimatorBase& operator=(const homographyEstimatorBase& other);

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

