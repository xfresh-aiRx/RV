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
 * file .......: ltiCrossCorrelationCoefficient.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 26.1.2004
 * revisions ..: $Id: ltiCrossCorrelationCoefficient.h,v 1.3 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_CROSS_CORRELATION_COEFFICIENT_H_
#define _LTI_CROSS_CORRELATION_COEFFICIENT_H_

#include "ltiImage.h"
#include "ltiPointList.h"
#include "ltiHomographyEstimatorBase.h"
#include "ltiFeatureExtractor.h"
#include "ltiTransformEstimatorType.h"

namespace lti {

  /**
   * This class computes the normalized cross correlation coefficient between
   * two matrices or vectors.
   *
   * Some applies allow to transform the points in one source using nearest
   * neighbor interpolation prior to computing the cross correlation
   * coefficient. This is only done if you specify a homographyEstimatorBase
   * in the parameters, though. The transformation is a short cut bypassing
   * classes like geometricTransform, which is useful when estimating and
   * verifying transformations (coming soon).
   *
   * CAVEAT: This class could use more iterator arithmetic inside and
   *         thus is not implemented efficiently everywhere. Some applies
   *         also are less efficient than they could be because code
   *         duplication is avoided.
   */
  class crossCorrelationCoefficient : public featureExtractor {
  public:
    /**
     * The parameters for the class crossCorrelationCoefficient
     */
    class parameters : public featureExtractor::parameters {
      
      //some protected attributes must be accessed by the parent class
      friend class crossCorrelationCoefficient;

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
       * The size of the window inside which a cross correlation is computed.
       * This parameters is only used by applies that utilize control points,
       * i.e. do not compute the cross correlation for the entire vector or
       * image. Only uneven windows are supported. Event windows are extended
       * by one.
       * Default: 10.
       */
      int window;

      /**
       * Sets a new transform estimator. The transform estimator is only used
       * by applies that pass a transform-vector. If no transform estimator
       * is specified then no transform is performed.
       * @param name the transform estimator to be used.
       */
      void setTransform(eTransformEstimatorType name);
      
      /**
       * Sets a new transform estimator. The transform estimator is only used
       * by applies that pass a transform-vector. If no transform estimator
       * is specified then no transform is performed.
       * A copy of the functor will be
       * done (so it is useless to change the parameters of the given
       * functor instance, because the internal functor will never notice
       * the changes done to its "parent").
       * @param method the transform estimator to be used.
       */
      void setTransform(const homographyEstimatorBase& method);

      /**
       * Is a transform estimator allocated ?
       * @return true is the transform estimator is allocated
       * and false otherwise.
       */
      bool existsTransform() const;
 
      /**
       * Gets the transform estimator. If no transform estiamtor is allocated
       * an exception is thrown.
       */
      const homographyEstimatorBase& getTransform() const;

    protected:
      /**
       * The transform estimator. The transform estimator is only used
       * by applies that pass a transform-vector. If no transform estimator
       * is specified then no transform is performed.
       * On default no point set is transformed.
       * Default: 0.
       */
      homographyEstimatorBase *transform;
    };

    /**
     * Default constructor
     */
    crossCorrelationCoefficient();

    /**
     * Construct a functor using the given parameters
     */
    crossCorrelationCoefficient(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    crossCorrelationCoefficient(const crossCorrelationCoefficient& other);

    /**
     * Destructor
     */
    virtual ~crossCorrelationCoefficient();

    /**
     * Returns the name of this type ("crossCorrelationCoefficient")
     */
    virtual const char* getTypeName() const;

   /**
     * @name Apply methods with one set of control points
     */
    //@{

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered.
     * If the channels are recorded from different positions it is
     * advantageous to supply a transformation vector. Please do no forget
     * to define a transformation in the parameters. On default no
     * transformation is executed. E.g. the control points and the points
     * inside the window have the same coordinates in both channels.
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param controlPoints pointList with the control points.
     * @param transf fvector with the transform by which the
     *        control points are fit inside the the second image. The
     *        window in the second image around the control points is
     *        distorted, too.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       const pointList& controlPoints,
	       const fvector& transf, float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered.
     * If the channels are recorded from different positions it is
     * advantageous to supply a transformation vector. Please do no forget
     * to define a transformation in the parameters. On default no
     * transformation is executed. E.g. the control points and the points
     * inside the window have the same coordinates in both channels.
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param controlPoints pointList with the control points.
     * @param transf fvector with the transform by which the
     *        control points are fit inside the the second image. The
     *        window in the second image around the control points is
     *        distorted, too.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       const pointList& controlPoints,
	       const fvector& transf, float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered.
     * If the channels are recorded from different positions it is
     * advantageous to supply a transformation vector. Please do no forget
     * to define a transformation in the parameters. On default no
     * transformation is executed. E.g. the control points and the points
     * inside the window have the same coordinates in both channels.
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param controlPoints pointList with the control points.
     * @param transf dvector with the transform by which the
     *        control points are fit inside the the second image. The
     *        window in the second image around the control points is
     *        distorted, too.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       const pointList& controlPoints,
	       const dvector& transf, double& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered.
     * If the channels are recorded from different positions it is
     * advantageous to supply a transformation vector. Please do no forget
     * to define a transformation in the parameters. On default no
     * transformation is executed. E.g. the control points and the points
     * inside the window have the same coordinates in both channels.
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param controlPoints pointList with the control points.
     * @param transf dvector with the transform by which the
     *        control points are fit inside the the second image. The
     *        window in the second image around the control points is
     *        distorted, too.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       const pointList& controlPoints,
	       const dvector& transf, double& dest) const;

    //@}
    /**
     * @name Apply methods with two sets of control points
     */
    //@{
 
    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered. Please specify the same number of points
     * in both point lists. In both channels the same window is used.
     * 
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param pts1 pointList with the first control points.
     * @param pts2 pointList with the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       const pointList& pts1, const pointList& pts2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered. Please specify the same number of points
     * in both point lists. In both channels the same window is used.
     * 
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param pts1 pointList with the first control points.
     * @param pts2 pointList with the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       const pointList& pts1, const pointList& pts2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered. Please specify the same number of points
     * in both point lists. In both channels the same window is used.
     * 
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param pts1 pointList with the first control points.
     * @param pts2 pointList with the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       const pointList& pts1, const pointList& pts2,
	       double& dest) const;

   /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at each control point.
     * This is useful to exlude e.g. homogenous areas. Only positions inside
     * both channels are considered. Please specify the same number of points
     * in both point lists. In both channels the same window is used.
     * 
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param pts1 pointList with the first control points.
     * @param pts2 pointList with the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       const pointList& pts1, const pointList& pts2,
	       double& dest) const;

    //@}
    /**
     * @name Apply methods with two points
     */
    //@{

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at a single control point.
     * Only positions inside both channels are considered. In both channels
     * the same window is used.
     * 
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param pt1  the first control points.
     * @param pt2  the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       const ipoint& pt1, const ipoint& pt2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at a single control point.
     * Only positions inside both channels are considered. In both channels
     * the same window is used.
     * 
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param pt1  the first control points.
     * @param pt2  the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       const ipoint& pt1, const ipoint& pt2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at a single control point.
     * Only positions inside both channels are considered. In both channels
     * the same window is used.
     * 
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param pt1  the first control points.
     * @param pt2  the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       const ipoint& pt1, const ipoint& pt2,
	       double& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering a window located at a single control point.
     * Only positions inside both channels are considered. In both channels
     * the same window is used.
     * 
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param pt1  the first control points.
     * @param pt2  the second control points.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       const ipoint& pt1, const ipoint& pt2,
	       double& dest) const;

    //@}
    /**
     * @name Apply methods for entire matrices
     */
    //@{

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering all positions inside both channels.
     * No window is used. Both channels most have the same size.
     * 
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering all positions inside both channels.
     * No window is used. Both channels most have the same size.
     * 
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering all positions inside both channels.
     * No window is used. Both channels most have the same size.
     * 
     * @param src1 channel8 with the first intensities.
     * @param src2 channel8 with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<ubyte>& src1, const matrix<ubyte>& src2,
	       double& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * channels only considering all positions inside both channels.
     * No window is used. Both channels most have the same size.
     * 
     * @param src1 channel with the first intensities.
     * @param src2 channel with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<float>& src1, const matrix<float>& src2,
	       double& dest) const;

    //@}
    /**
     * @name Apply methods for vectors
     */
    //@{

    /**
     * Computes the normalized cross correlation coefficient between two
     * vectors only considering all positions inside both vectors.
     * No window is used. Both vectors most have the same size.
     * 
     * @param src1 vector with the first intensities.
     * @param src2 vector with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<ubyte>& src1, const vector<ubyte>& src2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * vectors only considering all positions inside both vectors.
     * No window is used. Both vectors most have the same size.
     * 
     * @param src1 vector with the first intensities.
     * @param src2 vector with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */	     
    bool apply(const vector<float>& src1, const vector<float>& src2,
	       float& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * vectors only considering all positions inside both vectors.
     * No window is used. Both vectors most have the same size.
     * 
     * @param src1 vector with the first intensities.
     * @param src2 vector with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */   
    bool apply(const vector<ubyte>& src1, const vector<ubyte>& src2,
	       double& dest) const;

    /**
     * Computes the normalized cross correlation coefficient between two
     * vectors only considering all positions inside both vectors.
     * No window is used. Both vectors most have the same size.
     * 
     * @param src1 vector with the first intensities.
     * @param src2 vector with the second intensities.
     * @param dest the computed normalized cross correlation coefficient.
     * @return true if apply successful or false otherwise.
     */	     
    bool apply(const vector<float>& src1, const vector<float>& src2,
	       double& dest) const;

    //@}

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    crossCorrelationCoefficient& 
      copy(const crossCorrelationCoefficient& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    crossCorrelationCoefficient&
      operator=(const crossCorrelationCoefficient& other);

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

