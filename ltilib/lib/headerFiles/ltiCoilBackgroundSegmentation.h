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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiCoilBackgroundSegmentation.h
 * authors ....: Helmuth Euler
 * organization: LTI, RWTH Aachen
 * creation ...: 29.4.2003
 */

#ifndef _LTI_COILBACKGROUND_SEGMENTATION_H_
#define _LTI_COILBACKGROUND_SEGMENTATION_H_

#include "ltiImage.h"
#include "ltiSegmentation.h"
#include "ltiKMColorQuantization.h"
#include "ltiSparseMatrix.h"

namespace lti {
  /**
   * The coilBackgroundSegmentation functor tries to detect which pixels 
   * belong to the background and which not. It is quite slow, so it should be
   * used mainly for static applications where the mask has to be generated
   * only once and without any matter of time.
   *
   * For proper functionality there are some assumtions/requirements:
   *
   * - The background has to be almost homogeneous. (The functor works best 
   *   with black background, but this is not necessary.)
   * - There is a high probability that the border of the image
   *   contains only background pixels and the object overlaps the middle
   *   of the image.
   *
   *
   * The algorithm works as follows:
   *
   * At first the colors are quantized into regions using the k-means
   * algorithm.  Then two masks are generated, one representing the
   * object, the other one the background. These masks serve as a
   * basis for a probability estimation which region belongs to object
   * or background. After that the masks grow considering these
   * results, neighbourhoods and covariance matrices.  This iteration
   * is repeated "iteration"-times and finally generates a channel
   * containing 0 for background and 255 for the object.
   *
   * At last some after-treatment is done: Pixels constituting weak
   * links are deleted and if there is more than one object estimated,
   * only the largest one is kept.  In order to improve results, the
   * whole algorithm is reiterated up to "maxNumberBestOfN"-times,
   * erasing pixels in the final mask which are not estimated as
   * object every time. In the best case these shrinking lasts until
   * the final mask doesn´t change any more.
   * 
   * See the lti::coilBackgroundSegmentation::parameters for more information.
   */
  class coilBackgroundSegmentation : public segmentation {
  public:
    /**
     * the parameters for the class coilBackgroundSegmentation
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
       * The number of iterations the object- and background-masks take to grow
       * outside and inside the object.
       *
       * Default value: 10
       */
      int iterations;

      /**
       * The number of masks created. From them the final mask will be
       * generated with the value zero where not all masks were nonzero.
       * 4 is enough for a good result, but values >= 100 will generate 
       * the best possible solution (high values will cause a long time
       * to do the segmentation).
       *
       * Minimum value: 2
       * Default value: 100
       */
      int maxNumberBestOfN;

      /**
       * The factor the first object- and background-mask is stretchend.
       * Values > 1 should be used if the image is larger than 128*128
       * pixels in order to get a sharper segmentation around the object.
       *
       * Minimum value: 1
       * Default value: 1
       */
      float maskFactor;

      /**
       * The factor the threshold decinding between object and
       * background is stretched.  Lower values will cause better
       * results with objects containing many dark spots.  Higher
       * values prevent 'noise' outside the object.
       *
       * Default value: 1.30
       */
      double thresholdFactor;

      /**
      * The mean background color to be expected in order to improve
      * results by estimating with another covariance matrix.  If the
      * backgroundcolor is unknown use meanBackgroundColor = -1 to
      * disable.  Otherwise use only values between 0 and 255.  (See
      * also: covarMatrixBackground.)
      * 
      * Default value: 0 (black)
      */
      int meanBackgroundColor;

      /**
      * The size and shape of the additional covariace matrix.
      * Only affecting the result if meanBackgroundColor >= 0.
      * (See also: meanBackgroundColor)
      *
      * Default value: 100 on the main diagonal, 0 otherwise
      */
      dmatrix covarMatrixBackground;
    };

    /**
     * default constructor
     */
    coilBackgroundSegmentation();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    coilBackgroundSegmentation(const coilBackgroundSegmentation& other);

    /**
     * destructor
     */
    virtual ~coilBackgroundSegmentation();

    /**
     * returns the name of this type ("coilBackgroundSegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * generates a mask with the value 255 where the object is estimated
     * and 0 for the background.
     *
     * @param src image with the source data
     * @param result the resulting mask
     * @return true if apply successful or false otherwise
     */
    bool apply(const image& src,channel8& result);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    coilBackgroundSegmentation& copy(const coilBackgroundSegmentation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    coilBackgroundSegmentation& 
    operator=(const coilBackgroundSegmentation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * type combining the region with its mean color, probability
     * and relation to background and object
     */
    struct TmeanProbability {
      float sumProbability;         /// sum of the regionwide probability
      int appearanceCounter;        /// number of pixels in this region
      float meanProbability;        /// mean probability for the region
      int sumColorRed;              /// sum of the regionwide red-value
      int sumColorGreen;            /// sum of the regionwide green-value
      int sumColorBlue;             /// sum of the regionwide blue-value
      rgbPixel meanColor;           /// mean color for the region
      bool labelToObjectVector;     /// true if region belongs to object
      bool labelToBackgroundVector; /// true if region belongs to background
    };

    /**
     * delete such positions in the mask
     * \code
     *    ** **        *****
     *    ** **        *****
     *    **x**          x
     *    ** **        *****
     *    ** **        *****
     * \endcode
     * @param ch8 the channel8 to be optimized
     */
    void deleteWeakLinks(channel8& ch8) const;

    /**
     * calculating the inverted covariance matrix under consideration of 
     * the object/background mask
     * @param img the image to be considered
     * @param mask the mask which pixels from img should be used
     * @param mean the meanvalue (center) of the covarianve matrix is returned 
     *        here
     * @param covarMatrix the resulting matrix is returned here
     * @return true if succesfull
     */
    bool getInvertedCovarMatrixForMask(const image& img, 
                                       channel8& mask, 
                                       trgbPixel<double>& mean, 
                                       dmatrix& covarMatrix) const;

    /**
     * checking if a given color is close enough to the covariance matrix
     * @param mean the center of the covarianve matrix is considered here
     * @param covarMatrix the covariance matrix to be considered
     * @param xMinusM pixel with the color to be checked
     * @return true if succesfull
     */
    bool checkColorReasonable(const trgbPixel<double>& mean, 
                              const dmatrix& covarMatrix,
                              const rgbPixel& xMinusM) const;

    /**
     * building the background mask under consideration of
     * neighbourhoods, covariance matrix and probabilities
     *
     * @param img the image to be considered
     * @param ch8MaskChannel the mask estimating the background
     * @param intSparseMat matix containing which regions are neighbours
     * @param meanProbabilityList a list containing mean probability, color and
     *        relation towards background and object for each region
     * @param newLabelsMatrix matrix containing the labelnumber for each pixel
     * @param converged true if no change took place
     * @return true if succesfull
     */
    bool growOutsideObject(const image& img, 
                           channel8& ch8MaskChannel, 
                           sparseMatrix<int>& intSparseMat,
                           std::vector<TmeanProbability>& meanProbabilityList,
                           matrix<int>& newLabelsMatrix,
                           bool& converged) const;
    
    /**
     * building the object mask under consideration of neighbourhoods,
     * covariance matrix and probabilities
     *
     * @param img the image to be considered
     * @param ch8MaskChannel the mask estimating the object
     * @param intSparseMat matix containing which labels are related
     * @param meanProbabilityList a list containing mean probability, color and
     *        relation towards background and object for each region
     * @param newLabelsMatrix matrix containing the labelnumber for each pixel
     * @param converged true if no change took place
     * @return true if succesfull
     */
    bool growInsideObject(const image& img,
                          channel8& ch8MaskChannel,
                          sparseMatrix<int>& intSparseMat,
                          std::vector<TmeanProbability>& meanProbabilityList,
                          matrix<int>& newLabelsMatrix,
                          bool& converged) const;
  };
}

#endif

