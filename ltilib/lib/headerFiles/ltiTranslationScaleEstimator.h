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
 * file .......: ltiTranslationScaleEstimator.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 14.4.2004
 * revisions ..: $Id: ltiTranslationScaleEstimator.h,v 1.2 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_TRANSLATION_SCALE_ESTIMATOR_H_
#define _LTI_TRANSLATION_SCALE_ESTIMATOR_H_

#include "ltiHomographyEstimatorBase.h"
#include "ltiHTypes.h"

namespace lti {

  /**
   * This class estimates a translation and common scale for the x and y
   * coordinates, i.e. the following transformation:
   * \code
   * x_new = s * x + tx
   * y_new = s * y + ty
   * \endcode
   * The resulting least squares problem is solved by a cholesky
   * decomposition.
   *
   * Such a transformation can be used to correct distortion errors
   * when the camera rotation and translation is known a priori,
   * but errorprone.
   */
  class translationScaleEstimator : public homographyEstimatorBase {
  public:
    /**
     * The parameters for the class translationScaleEstimator
     */
    class parameters : public homographyEstimatorBase::parameters {
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
    translationScaleEstimator();

    /**
     * Construct a functor using the given parameters
     */
    translationScaleEstimator(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    translationScaleEstimator(const translationScaleEstimator& other);

    /**
     * Destructor
     */
    virtual ~translationScaleEstimator();

    /**
     * Returns the name of this type ("translationScaleEstimator")
     */
    virtual const char* getTypeName() const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. 
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
		       fvector& dest) const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. This method calls the apply without
     * the residual first, and then computes the residual.
     * @param src  std::vector<vector<fpoint> > with the point sets.
     * @param indeces ivector with the indices of the relevant points.
     * @param dest fvector the estimated transform.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *  
     * @param src matrix<ipoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<ipoint>& src,
		       fvector& dest, fvector& error) const;

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
		       int numCorrespondences) const;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This
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
		       int numCorrespondences) const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
		       fvector& dest) const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. This method calls the apply without
     * the residual first, and then computes the residual.
     * @param src  std::vector<vector<fpoint> > with the point sets.
     * @param indeces ivector with the indices of the relevant points.
     * @param dest fvector the estimated transform.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *  
     * @param src matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param error fvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
		       fvector& dest, fvector& error) const;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src matrix<fpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest fvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<fpoint>& src,
		       fvector& dest,
		       const ivector& indices,
		       int numCorrespondences) const;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src matrix<fpoint> with the point sets. All points of the same
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
    virtual bool apply(const matrix<fpoint>& src,
		       fvector& dest, fvector& error,
		       const ivector& indices,
		       int numCorrespondences) const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
		       dvector& dest) const;

    /**
     * Estimates a transform from the supplied point sets, where all
     * points are considered. This method calls the apply without
     * the residual first, and then computes the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *    
     * @param src matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param error dvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
		       dvector& dest, dvector& error) const;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This method
     * is used by robost estimators using a monte carlo approach.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
		       dvector& dest,
		       const ivector& indices,
		       int numCorrespondences) const;

    /**
     * Estimates a transform from the supplied point sets, whereby only
     * the points specified in the index vector are considered. This
     * method calls the apply without the residual first, and then computes
     * the residual.
     *
     * All points of one point set give a matrix row, whereas all elements
     * of a specifec correspondence stand in a matrix column.
     *
     * @param src matrix<dpoint> with the point sets. All points of the same
     *    image stand in a row. The correspondences in another image stand in
     *    the according columns.
     * @param dest dvector the estimated transform.
     * @param error dvector containing the deviation of each point from the
     *              estimated transform. Usually this is the residual or
     *              elementwise squared residual.
     * @param indices ivector with the indices of the relevant points.
     * @param numCorrespondences the first numCorrespondences indices are
     *                            considered.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<dpoint>& src,
		       dvector& dest, dvector& error,
		       const ivector& indices,
		       int numCorrespondences) const;

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
				    fvector& dest) const;

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
				    dvector& dest) const;

    /**
     * Returns the minimum number of correspondences required to estimate
     * the transform.
     */
    virtual int minNumberCorrespondences() const;

    /**
     * Returns the mininum dimension of a correspondence,
     * e.g. the minimum dimension of a correspondence pair is 2.
     * Each derived transform estimator only works on correspondences of
     * priori defined dimensions.
     */
    virtual int minCorrespondenceDimension() const;

    /**
     * Returns the maximum dimension of a correspondence,
     * e.g. the maximum dimension of a correspondence pair is 2, whereas
     * transformEstimator running on n-tuples may allow an infinite number.
     * Each derived transform estimator only works on correspondences of
     * priori defined dimensions.
     */
    virtual int maxCorrespondenceDimension() const;

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
		     const vector<fpoint>& shift) const;

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
		     const vector<dpoint>& shift) const;

    /**
     * Converts the estimated vector into a hMatrix3D<float>, which is e.g.
     * used by geometricTransform.
     * @param src fvector the estimated transform.
     * @param dest hMatrix3D<float> the estimated transform as a hMatrix3D.
     *
     * @return true if successful or false otherwise.
     */
    bool convert(const fvector& src, hMatrix3D<float>& dest) const;
  
    /**
     * Converts the estimated vector into a hMatrix3D<float>, which is e.g.
     * used by geometricTransform.
     * @param src dvector the estimated transform.
     * @param dest hMatrix3D<float> the estimated transform as a hMatrix3D.
     *
     * @return true if successful or false otherwise.
     */
    bool convert(const dvector& src, hMatrix3D<float>& dest) const;
    
    /**
     * Converts the estimated vector into a 3x3 matrix.
     *
     * @param src fvector the estimated transform.
     * @param dest fmatrix the estimated transform as a matrix.
     *
     * @return true if successful or false otherwise.
     */
    bool convert(const fvector& src, fmatrix& dest) const;

    /**
     * Converts the estimated vector into a 3x3 matrix.
     *
     * @param src dvector the estimated transform.
     * @param dest dmatrix the estimated transform as a matrix.
     *
     * @return true if successful or false otherwise.
     */
    bool convert(const dvector& src, dmatrix& dest) const;

    /**
     * This is a fast short cut used by the special robust estimator
     * homographyVerification in the main loop.
     * @param pt fpoint the point to be transformed
     * @param transf fvector with the transformation, which e.g. was computed
     *               by an apply.
     * @return the transformed point
     */
    fpoint transform(const fpoint& pt, 
		     const fvector& transf) const;
    
    /**
     * This is a fast short cut used by the special robust estimator
     * homographyVerification in the main loop.
     * @param pt fpoint the point to be transformed
     * @param transf fvector with the transformation, which e.g. was computed
     *               by an apply.
     * @return the transformed point
     */
    dpoint transform(const dpoint& pt, 
		     const dvector& transf) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    translationScaleEstimator& copy(const translationScaleEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    translationScaleEstimator& operator=(const translationScaleEstimator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };

  // -------------------------------------------------------------------
  // The transform-methods! no t-helper due to efficiency here
  // -------------------------------------------------------------------

  inline fpoint translationScaleEstimator
    ::transform(const fpoint& pt, const fvector& transf) const {
    
    fpoint res;
    fvector::const_iterator it ( transf.begin() );
    res.x = *it * pt.x;
    res.y = *it * pt.y;
    res.x += *(++it);
    res.y += *(++it);
 
    return res;
  }
  
  inline dpoint  translationScaleEstimator
    ::transform(const dpoint& pt, const dvector& transf) const {
   
    dpoint res;
    dvector::const_iterator it ( transf.begin() );
    res.x = *it * pt.x;
    res.y = *it * pt.y;
    res.x += *(++it);
    res.y += *(++it);
    
    return res;
  }
}

#endif

