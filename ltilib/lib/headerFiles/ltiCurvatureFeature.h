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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiCurvatureFeature.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 5.12.2001
 * revisions ..: $Id: ltiCurvatureFeature.h,v 1.10 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_CURVATURE_FEATURE_H_
#define _LTI_CURVATURE_FEATURE_H_


#include "ltiImage.h"
#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * The curvature feature is similar to the lti::orientationFeature,
   * but instead of creating a histogram of the pixel orientations, a
   * histogram of the curvature of a pixel will be done, considering
   * also the "relevance" channel when constructing it.  All values in
   * the relevance channel smaller than the
   * <code>relevanceThreshold</code> will be ignored (will be marked
   * with zero curvature).
   *
   * The curvature is defined as the derivative of the tangent of a given
   * contour.  This will be approximated here using the differences of
   * orientation between the neighbour pixels.
   */
  class curvatureFeature : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class curvatureFeature
     */
    class parameters : public globalFeatureExtractor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

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
       * Size for the gaussian kernel used to smooth the orientation
       * channel.  If you do not want to smooth the angles, use a
       * gaussian size of 0 or 1.
       *
       * Default value: 3
       */
      int gaussianSize;

      /**
       * Variance for the gaussian filter used to smooth the
       * orientation channel.  If you do not want to smooth the
       * angles, use a gaussian size of 0 or 1.
       *
       * Default value: -1 (i.e. let the constructor of the gaussian
       *                         kernel choose a variance value
       *                         appropriate for the kernel size)
       */
      float gaussianVariance;

      /**
       * Value used to get the orientation of the neighbour pixels
       */
      float deltaLength;

      /**
       * Minimal relevance value needed to consider a pixel in the
       * computations.
       */
      float relevanceThreshold;

      /**
       * if true, the feature vector will be normalized to have an area of
       * one, i.e. the feature vector can be interpreted as a probability
       * distribution for the orientations.
       * If false, the resulting values depend on the values of the
       * magnitud channel.
       * Default: true
       */
      bool normalize;

      /**
       * lower limit for the histogram.  All curvature values below this
       * threshold will be ignored.  This value MUST be lower than the
       * upperLimit.
       *
       * Valid value range: -Pi/2 .. Pi/2
       *
       * Default value: -Pi/2
       */
      float lowerLimit;

      /**
       * upper limit for the histogram.  All curvature values above this
       * threshold will be ignored.  This value MUST be higher than the
       * lowerLimit
       *
       * Valid value range: -Pi/2 .. Pi/2
       *
       * Default value: Pi/2
       */
      float upperLimit;

      /**
       * size of the histogram.  The curvature values from -lowerLimit
       * to upperLimit will be mapped between 0 and size-1.
       *
       * Default value: 64
       */
      int size;
    };

    /**
     * default constructor
     */
    curvatureFeature();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    curvatureFeature(const curvatureFeature& other);

    /**
     * destructor
     */
    virtual ~curvatureFeature();

    /**
     * returns the name of this type ("curvatureFeature")
     */
    virtual const char* getTypeName() const;

    /**
     * compute the feature vector
     * @param orientation channel with the angle of the tangent for all pixels
     * @param relevance channel with a "relevance"-degree for the orientation
     *                  channel.  Values of 0 mean, the angle in the
     *                  orientation channel has to be ignored, and 1 that
     *                  the value in the orientation channel is reliable.
     * @param dest feature vector.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& orientation,
               const channel& relevance,
                     dvector& dest) const;

    /**
     * Computes a curvature channel from the given orientation and relevance
     * channels.
     *
     * @param orientation channel with the angle of the tangent for all pixels
     * @param relevance channel with a "relevance"-degree for the orientation
     *                  channel.  Values of 0 mean, the angle in the
     *                  orientation channel has to be ignored, and 1 that
     *                  the value in the orientation channel is reliable.
     * @param dest channel with the curvature value computed for each pixel.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& orientation,
               const channel& relevance,
                     channel& dest) const;

    /**
     * Computes a curvature channel and the corresponding feature vector
     * from the given orientation and relevance channels.
     *
     * @param orientation channel with the angle of the tangent for all pixels
     * @param relevance channel with a "relevance"-degree for the orientation
     *                  channel.  Values of 0 mean, the angle in the
     *                  orientation channel has to be ignored, and 1 that
     *                  the value in the orientation channel is reliable.
     * @param dest channel with the curvature value computed for each pixel.
     * @param vct feature vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& orientation,
               const channel& relevance,
                     channel& dest,
                     dvector& vct) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    curvatureFeature& copy(const curvatureFeature& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    curvatureFeature& operator=(const curvatureFeature& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

