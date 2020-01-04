/*
 * Copyright (C) 1998 - 2005
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
 * file .......: ltiLoweGradientFeature.h
 * authors ....: Frederik Lange, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.11.2003
 * revisions ..: $Id: ltiLoweGradientFeature.h,v 1.3 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LOWE_GRADIENT_FEATURE_H_
#define _LTI_LOWE_GRADIENT_FEATURE_H_


#include "ltiVector.h"
#include "ltiLocation.h"
#include "ltiScaleSpacePyramid.h"
#include "ltiFeatureExtractor.h"
#include "ltiColorContrastGradient.h"
#include "ltiBilinearInterpolator.h"
#include <list>

namespace lti {
  /**
   * Local feature based on the gradient distribution at the level of the 
   * location.
   *
   * This local descriptor based was developed by David Lowe (see his
   * paper "Distinctive Image Features from Scale-Invariant
   * Keypoints", 2003).
   *
   * Lowe proposed also proposed 1999 a similar local descriptor called
   * "SIFT", but he changed it in his later work to used the information of
   * one resolution only.
   *
   * It computes histograms of the gradient orientation for several
   * parts of the region delimited by a location, where the weights of
   * each sample are determined by the magnitude of the gradient and the
   * distance to the center of the location.
   *
   * The location is divided in each axis by a given integer number \e h
   * (parameters::numHistograms).  This results in a total of \e h x \e h 
   * histograms and each one of them will have \e n x \e n samples, where
   * \e n represents parameters::sampleRegionSize.
   *
   * It can compute features for monochrome images and for color images, where
   * the color feature is just the concatenation of the three monochromatic
   * RGB ones.
   */
  class loweGradientFeature : public featureExtractor {
  public:
    /**
     * The parameters for the class loweGradientFeature
     */
    class parameters : public featureExtractor::parameters {
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
       * Sigma factor.
       *
       * The samples at the center of the location will a stronger
       * weighting than the ones farther from that center.  The weight is
       * computed as exp(-x^2/(2*sigma^2)), where x is the distance to the
       * location center.
       *
       * Default value: 3.5
       */
      float sigma;

      /**
       * Location partition.
       *
       * Each dimension of the location will be split in the given number of
       * parts.  The total number of histograms computed will be 
       * the square of the given value.
       *
       * Default value: 4 (i.e. a total of 16 histograms will be computed).
       */
      int locationPartition;

      /**
       * Histogram Partition
       *
       * Each histogram computed will use a number of samples equal to the
       * square of this value.
       *
       * Default value: 4 (i.e. a total of 16 samples will be used to compute
       *                   the histogram).
       */
      int histogramPartition;

      /**
       * Number of Bins in the orientation histogram.
       *
       * Default value: 8 (i.e. only 45 degree steps will be considered)
       */
      int orientationBins;

      /**
       * Parameters for gradient computation.
       *
       * Please ensure that the mode is "Polar" (the default).  Otherwise the
       * results will make no sense.
       *
       * Default value: colorContrastGradient::parameters()
       */
      colorContrastGradient::parameters gradientParam;

      /**
       * Total number of levels in the pyramid.
       *
       * This should be the same value used for the location extraction stage.
       *
       * Default value: 15
       */
      int pyramidLevels;

      /**
       * Parameters used to construct the pyramid.
       *
       * For better results please ensure that this parameter set corresponds
       * to the ones used in the location extraction.
       *
       * Default value: scaleSpacePyramid::parameters()
       */
      scaleSpacePyramid<channel>::parameters pyramidParam;

      /**
       * Location relative radius.
       *
       * This parameter is necessary to gain the information which
       * location belongs to which level.  
       * 
       * You should use here exactly the same value given to the
       * location selector functor.  Since the real radius of a
       * pixel at level zero is 0.5, you must ensure that the
       * used radius for a location at level zero divide by the factor given
       * here results in 0.5.
       *
       * Default value: 7
       */
      float locationRelativeRadius;

      /**
       * Normalize histogram.
       *
       * If set to true, each histogram will be normalized, representing a
       * probability distribution.
       *
       * Default value: true
       */
      bool normalize;

      /**
       * Cut threshold.
       *
       * If \a normalize is set to true, then all histogram values greater
       * to this value will be clipped also to this value.  This is supposed
       * to help compensating illumination changes.  If you don't want to clip,
       * just set this value to 1.0f.
       *
       * After all values greater than cutThreshold have been clipped, the
       * resulting vector will be normalized again.
       *
       * Default value: 0.2f
       */
      float cutThreshold;
      
    };

    /**
     * Default constructor
     */
    loweGradientFeature();

    /**
     * Construct a functor using the given parameters
     */
    loweGradientFeature(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    loweGradientFeature(const loweGradientFeature& other);

    /**
     * Destructor
     */
    virtual ~loweGradientFeature();

    /**
     * Returns the name of this type ("loweGradientFeature")
     */
    virtual const char* getTypeName() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loweGradientFeature& copy(const loweGradientFeature& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    loweGradientFeature& operator=(const loweGradientFeature& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    virtual bool updateParameters();

    // --------------------

    /**
     * Extracts gradient features at the locations for a gray-valued
     * input channel.
     */
    bool apply(const channel &src, 
               const std::list<location> &locs, 
                     std::list<dvector> &vcts);

    /**
     * Extracts the color features at the loctions
     */
    bool apply(const image &src,
               const std::list<location> &locs, 
                     std::list<dvector> &vcts);
    
    /**
     * Extract features based on a previously generated pyramid set.
     *
     * The decision if you want color or gray features is taken at the time
     * of the pyramid generation.
     *
     * @see generate()
     */
    bool apply(const std::list<location> &locs,
                     std::list<dvector> &vcts);


    /**
     * Compute the gradient feature for the given location.
     *
     * You must call first the method generate() to indicate which image
     * needs to be analyzed.  (Otherwise it would take too much time if you
     * need to extract several features from the same image, and the pyramid
     * would have to be generated each time!)
     *
     * @param locs location that need to be analyzed.
     * @param dest feature vector.
     * @return true if successful, or false otherwise.  
     */
    bool apply(const location& locs,
                     dvector& dest);

    /**
     * Generate and internal pyramidal representation for the given color
     * image, which can be later analyzed with the respective apply method.
     */
    bool generate(const image &src);

    /**
     * Generate and internal pyramidal representation for the given gray valued
     * channel, which can be later analyzed with the respective apply method.
     */
    bool generate(const channel &src);

    /**
     * @name Debug methods.
     */
    //@{
    /**
     * Return read-only reference to the histogram mask
     */
    const imatrix& getHistogramMask() const;

    /**
     * Return the weight mask
     */
    const matrix<float>& getWeightMask() const;

    /**
     * Return the gradient magnitude pyramid
     */
    const scaleSpacePyramid<channel>& getMagnitudePyramid() const;

    /**
     * Return the gradient orientation pyramid
     */
    const scaleSpacePyramid<channel>& getAnglePyramid() const;

    //@}


  protected:
    
    /**
     * the pyramids
     */
    scaleSpacePyramid<channel> pyrMag, pyrArg;

    /**
     * Compute the histogram for a given location
     */
    bool computeHistograms(const int level,
                           const tpoint<float>& pos,
                           const float angle,
                           matrix<double>& hists) const;

    /**
     * @name LUT and Masks
     */

    //@{

    /**
     * Initialize LUT and masks used to accelerate feature computation
     */
    bool initMasks();
    
    /**
     * Mask contains the index of the corresponding histogram for 
     * each entry.
     */
    matrix<int> hmask;

    /**
     * Mask contains the weight for each entry in the histogram.
     */
    matrix<float> weight;

    /**
     * Position of the location center in the mask
     */
    tpoint<float> maskCenter;
    
    /**
     * Total number of histograms computer per location.
     */
    int numHistograms;

    /**
     * Input/output points.
     *
     * The size of this "matrix" will always be the 2 x number of rows of the
     * mask, but indexed also with negative values. 
     *
     * The row 0 contains the in points and the row 1 the out points.
     */
    matrix<int> ioPts;
    
    //@}

  };
}

#endif

