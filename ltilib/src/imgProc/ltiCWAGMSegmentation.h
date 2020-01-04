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
 * file .......: ltiCWAGMSegmentation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.6.2003
 * revisions ..: $Id: ltiCWAGMSegmentation.h,v 1.2 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CWAGM_SEGMENTATION_H_
#define _LTI_CWAGM_SEGMENTATION_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifdef _LTI_MSC_6

#pragma message("Insufficient C++ Template Support for lti::regionGraphMeans.")
#pragma message("You need a newer compiler")

#else


#include "ltiSegmentation.h"
#include "ltiMedianFilter.h"
#include "ltiColorContrastGradient.h"
#include "ltiSplitImage.h"
#include "ltiWatershedSegmentation.h"
#include "ltiRegionGraphMeans.h"

#include <map>
#include <vector>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti {

  // ------------------------------------------------------------------------
  //
  // Color Watershed - Adjacency Graph Merge
  //
  // ------------------------------------------------------------------------


  /**
   * CWAGM Segmentation
   *
   * CWAGM means Color Watershed - Adjacency Graph Merge.  It is a
   * segmentation algortihms based on low level information introduced in
   * P. Alvarado "Segmentation of color images for interactive 3D object
   * retrieval".  Ph.D. Thesis. RWTH-Aachen, 2004.
   * http://www.bth.rwth-aachen.de/job/disslist.pl
   *
   * This watershed-based concept is based on the ideas of Haris et.al.
   * for a segmentation concept of gray-valued image, extended to color images.
   * It uses (optionally) a median filtering to reduce the noise in each color
   * channel, computes an over-segmentation with the watershed transformation
   * and merge the regions to minimize the mean square error of a piece-wise
   * constant image approximation.
   */
  class cwagmSegmentation : public segmentation {
  public:

    /**
     * the parameters for the class cwagmSegmentation
     */
    class parameters : public segmentation::parameters {
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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * @name Preprocessing
       */
      //@{
      /**
       * A median filter with following parameters is used to denoise the image
       *
       * If you don't want any pre-smoothing, indicate a kernel size of 1 or
       * zero and nothing will be done.
       *
       * Default value: medianParam::kernelSize = 3;
       *                medianParam::boundaryType = Constant;
       */
      medianFilter::parameters medianParam;

      /**
       * Enhance contrast
       *
       * If set to \a true, the dynamic range of the channels will be
       * optimized.
       *
       * Since the RGB channels will be linearly expanded to map
       * min(R,G,B)-max(R,G,B) to the 0-255 range, the colors of the image
       * will not be severely altered.
       *
       * Default value: true
       */
      bool enhanceContrast;
      //@}

      /**
       * @name Split-Stage with Watersheds
       */
      //@{
      /**
       * The colorSplitter specifies the name of the color space splitting
       * functor used to extract the color components of the image, which are
       * employed to analyze the edges of the image.
       *
       * Default value: "lti::splitImageToXYZ"
       */
      std::string colorSplitter;

      /**
       * Parameters for the computations of the color contrast gradient
       *
       * The result is also the input for the watershed algorithm.
       */
      colorContrastGradient::parameters colorContrastParam;

      /**
       * Watershed transform parameters.
       *
       * Default: Rainfall method is selected as default and a 8-Neighborhood.
       *          The threshold is set to 255 to activate the automatic 
       *          threshold detection.
       */
      watershedSegmentation::parameters watershedParam;

      /**
       * To compute which threshold should be used in the watershed
       * segmentation, the accumulative histogram of gradient values will be
       * used as a probability distribution.  It will be assumed that the 
       * probability for a gradient value to be relevant must be greater 
       * than the given value.
       *
       * If you set a value close to zero, almost any gradient value will be
       * detected as edge, and an oversegmentation will be produced.
       *
       * If you set a value close to one, then only very strong gradient
       * values will be considered, causing an undersegmentation.
       * 
       * The automatic threshold computation will be done only if the user set
       * the watershedParam.threshold to 255.
       *
       * The value range must be between 0.0f and 1.0f
       * 
       * Default value: 0.45f
       */
      float minProbForWatershedThreshold;
      //@}
      
      /**
       * Merging parameters for the graph representation.
       *
       * Watersheds over-segment images.  The high number of regions detected
       * slows down the rest of the algorithms.  Therefore a first "merging"
       * stage is done, using an adaption of the distance measure suggested by
       * Haris et.al. to also use color images.  All adjacent regions with
       * distances under the given threshold will be merged.
       *
       * A mergeThreshold value close to zero means that only very similar
       * regions have to be merged, while a large value indicates to be much
       * more tolerant while merging and many regions will be merged.
       *
       * The threshold indicated within this parameter objects indicate a
       * square distance between mean values in the color space weighted by
       * the area of the regions, so that this value will be between 0 and
       * about 1000000, depending on the regions detected.  You will usually
       * give values between 0 and 2000
       *
       * Default: harisRegionMergeParam::mergeThreshold = 1
       *          harisRegionMergeParam::minRegionNumber = 10
       */
      regionGraphColorHaris::parameters harisRegionMergeParam;
    };

    /**
     * default constructor
     */
    cwagmSegmentation();

    /**
     * Construct a functor using the given parameters
     */
    cwagmSegmentation(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    cwagmSegmentation(const cwagmSegmentation& other);

    /**
     * destructor
     */
    virtual ~cwagmSegmentation();

    /**
     * returns the name of this type ("cwagmSegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @param regionSizes sizes of each region in the \a regions mask.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     imatrix& regions,
                     ivector& regionSizes);

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param preproc preprocessed image.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @param regionSizes sizes of each region in the \a regions mask.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     image& preproc,
                     imatrix& regions,
                     ivector& regionSizes);


    /**
     * Segmentate the image \a src and return many internal partial results
     * that can be used in further processing units.
     *
     * Note that there are several segmentation modes (like color ACA or
     * mean-shift segmentation) which do not require the gradient computation.
     * These modes will leave the given \a gradMag and \a gradArg untouched.
     * This way, the gradients won't be unnecessarily computed and you can
     * detect if they are computed or not: just give empty channels and if
     * they are still empty after calling this method, you will have to
     * compute them, if you need to.
     *
     * @param src image with the source data.
     * @param preproc resulting preprocessed image.
     * @param regions labeled mask as imatrix, where the result will be left.
     * @param regionSizes sizes of each region in the \a regions mask.
     * @param gradMag magnitude of the color gradient
     * @param gradArg orientation of the color gradient
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     image& preproc,
                     imatrix& regions,
                     ivector& regionSizes,
                     channel& gradMag,
                     channel& gradArg);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cwagmSegmentation& copy(const cwagmSegmentation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cwagmSegmentation& operator=(const cwagmSegmentation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor's parameters.
     *
     * Update the internal state of the functor according to the parameters.
     * This special case the functor used to split the image into its color
     * componentes is initialized here.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();
    
  protected:

    /**
     * Preprocess
     * Denoise and Enhance Contrast
     */
    bool preprocess(const image& src,image& dest) const;

    /**
     * Split and merge with watersheds.
     *
     * @param orig original color image
     * @param preproc preprocessed image
     * @param regions labeld mask with initial partition
     * @param regionSizes number of pixels per region
     * @param gradM magnitude of the color gradient
     * @param gradA orientation of the color gradient
     * @param gradHisto histogram of the gradient values
     * @param maxGrad maximum gradient value which is also the last value in
     *                the histogram.
     */
    bool worker(const image& orig,
                const image& preproc,
                imatrix& regions,
                ivector& regionSizes,
                channel& gradM,
                channel& gradA,
                vector<float>& gradHisto,
                float& maxGrad);

    /**
     * The simplest contrast enhancement consists in mapping the input value
     * range of each channel into 0 to 255, each channel independently of
     * the others.
     *
     * src MUST be connected.  (see matrix<T>::mode())
     */
    bool enhanceContrast(image& src) const;
   
    /**
     * @name local functors
     */
    //@{
    /**
     * median filter
     */
    medianFilter median;

    /**
     * Watershed segmentation functor
     */
    watershedSegmentation watershed;

    /**
     * pointer to splitter object
     */
    splitImage* splitter;

    /**
     * color contrast gradient functor used in edge detection
     */
    colorContrastGradient colorContrast;

    /**
     * Haris region merging method (well, my adaption of it)
     */
    regionGraphColorHaris haris;
    //@}

  };
}

#endif

#include "ltiUndebug.h"
#endif
