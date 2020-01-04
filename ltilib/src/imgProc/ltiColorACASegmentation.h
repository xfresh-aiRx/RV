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
 * file .......: ltiColorACASegmentation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 6.1.2004
 * revisions ..: $Id: ltiColorACASegmentation.h,v 1.4 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_A_C_A_SEGMENTATION_H_
#define _LTI_COLOR_A_C_A_SEGMENTATION_H_

#include "ltiImage.h"
#include "ltiSegmentation.h"
#include "ltiKMColorQuantization.h"

#include <vector>

namespace lti {
  /**
   * Color Adaptive Clustering Algorithm.
   *
   * This functor provides a segmentation algorithm based on the Adaptive
   * Clustering Algorithm (ACA) proposed by Thrasyvoulos N. Pappas in
   * his paper "An adaptive clustering algorithm for image segmentation",
   * IEEE Transactions on signal processing, vol. 40, No. 4, april 1992.
   *
   * Even if in the original version the algorithm has been applied to
   * gray valued images, the current implementation has been adapted for the
   * segmentation of color images.
   * 
   * The algorithm first generates an image pyramid for the input.  For the
   * smallest image a k-Means color clustering is done, and a labeling 
   * procedure assigns to each pixel the corresponding label.
   * 
   * Using this first segmentation proposal and the current window
   * size (initially the whole image) it generates a new estimation of the
   * expected mean values for each pixel.  With the new mean values compute the
   * a-posteriori probabilities for each pixel to belong to any of the classes
   * and compute with them a new segmentation hypothesis.
   *
   * Repeat the estimation of the expected values and the new
   * segmentation until the algorithm converges or only a small number
   * of changes has been done in the last iteration.
   *
   * Reduce the window size and begin the iterations again, until the minimum
   * window size is reached.
   *
   * Now duplicate the size of the segmentation result and use it as
   * initialization for the next iteration using the next pyramid
   * level.
   * 
   * The whole process is repeated until the highest resolution level has 
   * been analyzed. 
   *
   * Note that this algorithm is very slow.  It is implemented to
   * provide a comparison possibility with your own algorithms.
   *
   */
  class colorACASegmentation : public segmentation {
  public:
    /**
     * The parameters for the class colorACASegmentation
     */
    class parameters : public segmentation::parameters {
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
       * Number of levels in the pyramid.
       *
       * This value \b must be greater or equal one.
       *
       * Default value: 4
       */
      int levels;

      /**
       * Parameters for the kMeans quantization.
       *
       * The number of means used in the whole algorithm will be given by
       * the attribute kMColorQuantization::parameters::numberOfColors.
       *
       * Default value: all default values except numberOfColors, which is set
       *                to 16.
       */
      kMColorQuantization::parameters kMeansParam;

      /**
       * Beta factor used in the Gibbs distribution model for the spatial
       * consideration.
       *
       * Default value: 0.5
       */
      float beta;

      /**
       * A-Priori label membership.
       * 
       * If empty, all labels are equiprobable.  If not empty, for those labels
       * with an entry, this corresponds to the one-pixel clique potentials.
       *
       * Default value: empty vector
       */
      fvector alpha;

      /**
       * Standard deviation of Gaussian white distribution assumed for the
       * region noise.
       *
       * Since the value range of the data is between 0 and 255, you can 
       * consider this value as the deviation brightness expected in each
       * of the R,G and B color channels.
       *
       * Default value: 5
       */
      float sigma;

      /**
       * Convergence criterion for a \e cycle.
       *
       * After the mean values has been estimated, the new segmentation has
       * to be computed using an iterative procedure that uses the previous
       * segmentation as suggestion and continues until less than the given
       * percentage of totall image pixels change.
       *
       * Note that this value is usually much smaller than one.
       *
       * Default value: 0.0004f
       */
      float convergenceCriterion;

      /**
       * Threshold minimum.
       *
       * The real threshold is computed multiplying this value by the current
       * window width.
       *
       * If the number of pixels in a window that belong to the current label
       * is less than the computed threshold, the current pixel is marked as
       * undefined and the desition to which level it will belong will be made
       * according to spatial conditiions only (based on a MRF).
       *
       * Default value: 1.0
       */
      float tMin;

      /**
       * Maximum number of iterations with the same window size
       *
       * This parameter controls how much iteration will be done with the
       * same window size of mean-estimation + segmentation-estimation steps.
       *
       * Default value: 20
       */
      int nMax;

      /**
       * The positioning of the window centroids will be computing
       * using this overlap factor.  If zero is given, the windows
       * will be place side by side.  If one is given the windows will be
       * computed for each pixel in the image, which is very expensive.
       *
       * Pappas suggests the use of 0.5f.
       *
       * Default value: 0.5f
       */
      float windowOverlap;

      /**
       * Initial window size as percentage of the minimum of the image
       * dimensions.
       *
       * Default value: 1.0f
       */
      float firstWindowSize;

      /**
       * Absolute minimum window size.
       *
       * The window size will be reduce until it is smaller that this size.
       *
       * Default value: 7
       */
      int lastWindowSize;

      /**
       * Multiplicative window size step change.
       *
       * This values must be possitive and strictly smaller than one.
       *
       * If you set the value outsize this range, no window iteration sizes
       * will be done, and the algorithm will behave like a k-Means clustering
       * (but very slow).
       *
       * Default value 0.5
       */
      float windowSizeStep;

    };

    /**
     * Default constructor
     */
    colorACASegmentation();

    /**
     * Construct a functor using the given parameters
     */
    colorACASegmentation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    colorACASegmentation(const colorACASegmentation& other);

    /**
     * Destructor
     */
    virtual ~colorACASegmentation();

    /**
     * Returns the name of this type ("colorACASegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * Segmentates the \a srcdest image and replaces it with the 
     * estimated mean values.
     *
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(image& srcdest) const;

    /**
     * Segmentates the image \a src and compute the estimated mean
     * values.
     *
     * @param src image with the source data.
     * @param dest image with the smooth changing means per region.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,image& dest) const;

    /**
     * Segmentates the image and computes the labeled mask, with labels between
     * 0 and parameters::kMeansParam::numberOfColors - 1.  Additionally, the
     * estimated mean values will be computed.
     *
     * Note that the result differs from other segmentation approaches in the
     * fact that the functor doesn't compute a palette, since the values within
     * a region are allowed to slowly change.
     *
     * @param src image with the source data
     * @param mask imatrix with the labels for each region
     * @param means image with the estimated mean values per pixel.
     */
    bool apply(const image& src,imatrix& mask,image& means) const;

    /**
     * Segmentates the image and computes the labeled mask, with labels between
     * 0 and parameters::kMeansParam::numberOfColors - 1.  Additionally, the
     * estimated mean values will be computed.
     *
     * Note that the result differs from other segmentation approaches in the
     * fact that the functor doesn't compute a palette, since the values within
     * a region are allowed to slowly change.
     *
     * @param src image with the source data
     * @param mask imatrix with the labels for each region
     */
    bool apply(const image& src,imatrix& mask) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorACASegmentation& copy(const colorACASegmentation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorACASegmentation& operator=(const colorACASegmentation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set the parameters
     */
    bool setParameters(const functor::parameters& param);

  protected:
    /**
     * Segmentates the image and computes the labeled mask, with labels between
     * 0 and parameters::kMeansParam::numberOfColors - 1.  Additionally, the
     * estimated mean values will be computed.
     *
     * Note that the result differs from other segmentation approaches in the
     * fact that the functor doesn't compute a palette, since the values within
     * a region are allowed to slowly change.
     *
     * @param src image with the source data
     * @param mask imatrix with the labels for each region
     * @param means image with the estimated mean values per pixel.
     * @param computeMeans compute image with means
     * @return true if successful, false otherwise.
     */
    bool aca(const image& src,imatrix& mask,image& means,
             const bool computeMeans) const;

    /**
     * Initialize the segmentation using k-Means.  
     *
     * The argument \a mask represents the variable \a x in the original paper
     */
    bool kMeansInit(const image& src, imatrix& mask) const;

    /**
     * Estimates the means channel using linear interpolation of the support
     * points at the grid, using a window of the given size.
     *
     * The means image vector must have the proper size when initialized:
     * parameters::kMeansParam::numberOfColors
     */
    bool estimateMeans(const image& src,
                       const imatrix& mask,
                       std::vector<image>& means,
                       const int windowSize,
                       const int gridSize,
                       const int tMin) const;

    /**
     * Interpolate all pixels not at the grid points using bilinear
     * interpolation.
     */
    bool interpolate(image& src,
                     const int gridSize) const;

    /**
     * Estimate segmentation given the means
     */
    bool estimateSegmentation(const image& src,
                              const std::vector<image>& means,
                              const float gfactor,
                              imatrix& mask,
                              int& cycles) const;

    /**
     * Compute label.
     *
     * @param src original image
     * @param means the mean values for each label
     * @param mask initial segmentation 
     * @param p point to be evaluated
     * @param from which neighbor should be considered first
     * @param to which neighbor should be considered last
     * @param gfactor must be -1/2*sigma^2
     * @param numChanges number of changes taken in the iteration.
     * @return label that maximizes the a posteriori probability
     */
    inline int computeLabel(const image& src,
                            const std::vector<image>& means,
                            const imatrix& mask,
                            const point& p,
                            const int from,
                            const int to,
                            const float gfactor,
                            int& numChanges) const;
    /**
     * Estimate segmentation given the means
     */
    bool estimateSegmentationStep(const image& src,
                                  const std::vector<image>& means,
                                  const float gfactor,
                                  imatrix& mask,
                                  imatrix& nmask,
                                  int& numChanges) const;

    /**
     * Create an image containing the appropriate mean values
     */
    bool createMeanImage(const std::vector<image>& lmeans,
                         const imatrix mask,
                         image& means) const;

  private:
    /**
     * Constants
     *
     * One point clique constants.  This has always a length numberOfColors 
     */
    fvector alpha;

    /**
     * Constant 
     *
     * Potential for the two-point cliques.
     */
    float beta;

  };
}

#endif

