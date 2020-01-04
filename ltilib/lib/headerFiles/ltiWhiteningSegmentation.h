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
 * file .......: ltiWhiteningSegmentation.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 14.2.2002
 * revisions ..: $Id: ltiWhiteningSegmentation.h,v 1.15 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_WHITENING_SEGMENTATION_H_
#define _LTI_WHITENING_SEGMENTATION_H_

#include "ltiImage.h"
#include "ltiPCA.h"
#include "ltiSegmentation.h"
#include "ltiKMeansSegmentation.h"
#include "ltiPointList.h"

namespace lti {
  /**
   * The whitening segmentation is an extention of the k-Means based
   * segmentation (see lti::kMeansSegmentation).  Internally, it
   * computes the k-Means segmentation of the given image, and the
   * k-Means segmentation of an image resulting from mapping linearly
   * each RGB pixel in the original image into another color space
   * determined by the given pca transformation functor.  The mapping is
   * clipped using a sigmoid function to keep all values in the valid
   * RGB space (0..255 for each component).  The last stage removes the
   * smallest regions assigning their pixels to the neighbor most similar ones.
   *
   * The PCA can be obtained using from example the
   * lti::colorModelSelector, which is usually configured to returned
   * a skin color model.  The PCA is used to make a "color zoom" into
   * the skin color to detect the borders between skin colored objects
   * and real skin.
   *
   * The two color quantization results are combined into the final one,
   * multiplying the labels of the second segmentation by the number of
   * labels of the first quantization, and adding the first quantization.
   *
   * \warning The RGB color space used is the unit cube (red, green
   * and blue components between zero and one) and not the 8-bit based
   * color cube, so, if you use lti::colorModelEstimator or
   * lti::computePalette to compute a statistic color model be sure
   * that you norm the estimated mean and covariance accordingly:
   * mean.divide(255) and covariance.divide(255*255)!  For an example
   * see transformImage()
   */
  class whiteningSegmentation : public segmentation {
  public:
    /**
     * the parameters for the class whiteningSegmentation
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
       * @return true if write was succe    quantColorsTransformed = 15;
ssful
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
       * parameters for the color quantization of the original image
       *
       * Default value: kernelSize=5, quantColors=16, smoothFilter=KNearest
       */
      kMeansSegmentation::parameters quantNormal;

      /**
       * parameters for the color quantization of the transformed
       * (color zoomed) image.
       *
       * Default value: kernelSize=5, quantColors=15, smoothFilter=KNearest
       */
      kMeansSegmentation::parameters quantTransformed;

      /**
       * minimum number of pixel for a region.
       *
       * Default value: 12
       */
      int minRegionSize;

    };

    /**
     * default constructor
     */
    whiteningSegmentation();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    whiteningSegmentation(const whiteningSegmentation& other);

    /**
     * destructor
     */
    virtual ~whiteningSegmentation();

    /**
     * returns the name of this type ("whiteningSegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * attaches all pixel given by the freePoints list
     * to a neighbour region (same label) by checking the
     * color distances (freePoint to all its neighbours)
     * @param src original image
     * @param freePoints list of points to be reassigned
     * @param imgMap the image regions map
     */
    void attachPoints(const image& src,
		      pointList& freePoints,
		      imatrix& imgMap) const;

    /**
     * removes small regions from label image imgMap.
     * All pixels belong to a region which consist less than
     * thresh pixels are attached to a neighbour region.
     * @param src original image
     * @param thresh minimal valid value for the size of a region
     * @param imgMap the image regions map
     */
    void removeSmallRegions(const image& src,
			    const int& thresh,
			    imatrix& imgMap) const;

    /**
     * whitening transformation of the src image in the dest image
     * using the given pca transformation functor.
     *
     * A sigmoid clipping will be taken to limit the result to valid
     * values in the RGB color space. This operation can be considered
     * as color zooming.
     *
     * For the given pca functor, it will be expected that the color
     * axes have been normalized from 0 to 1 (instead of 0 to 255).
     *
     * @see transformImage(const drgbPixel&,const dmatrix&,const image&,image&)
     *
     * @param pca functor with the linear transformation matrix (pca).
     * @param src image
     * @param dest color zoomed image
     *
     * Example:
     * \code
     *
     * // Example for color zoom
     *
     * lti::image img;    // original color imaged
     * lti::image zoomed; // color zoomed image
     * lti::imatrix mask; // labeled mask, result of a segmentation algorithm
     *
     * ... // get the image and the mask with any algorithm...
     *
     * lti::computePalette compPal;  // functor used to get a complete palette
     *                               // or just a palette entry of an image
     *                               // given a mask.
     * lti::fmatrix covariance;    // covariance matrix of the color
     * lti::fvector mean;          // mean color
     * int n;                      // how many pixels were considered in the
     *                             // computation of mean and covariance.
     *
     * // compute mean color and covariance matrix for the pixels with
     * // label "0" (usually background)
     * compPal.apply(img,mask,0,mean,covariance,n);
     *
     * // for the color zooming, the mean and covariance need to be
     * // given for the unit cube color space, instead of the 8-bit valued
     * // color space.
     * mean.divide(255);
     * covariance.divide(255*255);
     *
     * // principal components functor used to transform the pixel colors
     * lti::principalComponents<float> pca;
     * lti::principalComponents<float>::parameters pcaPar;
     *
     * pcaPar.whitening = true; // whitening transform is a requirement!
     * pcaPar.resultDim = 3;    // after the transform, each pixel must
     *                          // still have three dimensions!
     * pcaPar.autoDim = false;  // do not try to reduce the dimensionality
     *
     * pca.setParameters(pcaPar); // specify to use the given parameters
     * pca.setCovarianceAndMean(covariance,mean); // and the computed stats.
     *
     * // compute the color zoomed image
     * lti::whiteningSegmentation wseg;
     * wseg.transformImage(pca,img,zoomed);
     *
     * \endcode
     */
    bool transformImage(const principalComponents<float>& pca,
			const image& src,
			image& dest) const;


    /**
     * whitening transformation of the src image in the dest image
     * using the given statistical parameter to create a pca-whitening
     * transformation.
     *
     * A sigmoid clipping will be taken to limit the result to valid
     * values in the RGB color space. This operation can be considered
     * as color zooming.
     *
     * The given parameters of mean and covariance can be computed
     * directly with functors like the colorModelEstimator and
     * computePalette (for this last one some type conversions would
     * be required)
     *
     * @see transformImage(const principalComponents<float>&,const image&,
     *                     image&)
     *
     * @param mean drgbPixel with the mean color, that will be
     *             projected into rgbPixel(128,128,128).  The coordinates of
     *             the mean should be between 0 and 255.
     * @param covar a 3x3 dmatrix with the covariance matrix of the colors.
     * @param src source image
     * @param dest color zoomed image
     *
     * Example:
     * \code
     *
     * // Example for color zoom
     *
     * lti::image img;    // original color imaged
     * lti::image zoomed; // color zoomed image
     *
     * ... // get the image and the mask with any algorithm...
     *
     * lti::colorModelEstimator colEst; // functor used to get a color model
     * lti::dmatrix covariance;    // covariance matrix of the color
     * lti::drgbPixel mean;        // mean color
     *
     * // compute mean color and covariance matrix for the pixels in the given
     * // image.
     * colEst.consider(img);
     * colEst.apply(mean,covariance);
     *
     * // compute the color zoomed image
     * lti::whiteningSegmentation wseg;
     * wseg.transformImage(mean,covariance,img,zoomed);
     *
     * \endcode
     */
    bool transformImage(const drgbPixel& mean,
                        const dmatrix& covar,
			const image& src,
			image& dest) const;


    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param pca functor responsible for the color zooming
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
	       const principalComponents<float>& pca,
	       imatrix& dest) const;

    /**
     * operates on a copy of the given %parameters.  You can get the
     * mean and covariance matrix (which should be computed in a RGB
     * color space with each color between 0 and 255) using the
     * lti::colorModelEstimator functor.
     *
     * @param src image with the source data.
     * @param mean mean color used for the pca/whitening transformation
     * @param covar covariance matrix used for the pca/whitening transformation
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
	       const drgbPixel& mean,
               const dmatrix& covar,
                     imatrix& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    whiteningSegmentation& copy(const whiteningSegmentation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    whiteningSegmentation& operator=(const whiteningSegmentation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif

