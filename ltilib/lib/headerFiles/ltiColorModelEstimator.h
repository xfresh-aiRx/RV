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
 * file .......: ltiColorModelEstimator.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.3.2002
 * revisions ..: $Id: ltiColorModelEstimator.h,v 1.16 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_MODEL_ESTIMATOR_H_
#define _LTI_COLOR_MODEL_ESTIMATOR_H_

#include <list>
#include "ltiImage.h"
#include "ltiHistogram.h"

#include "ltiStatisticsFunctor.h"

namespace lti {

  /**
   * This functor is used to create three-dimensional histograms for
   * the colors of images.  The histograms are used as color models in
   * functors like lti::colorModelSelector and lti::probabilityMap.
   *
   * It is possible to provide whole images (usefull to create histograms
   * of backgrounds, if they can be captured as a whole image), or
   * images with corresponding masks, that allow to consider only those
   * pixels with a mask value different than zero.
   *
   * It is also possible to provide directly a list of images.
   *
   * This functor generates also simple statistics for a multivariate gaussian
   * model for the color (mean color and covariance matrix)
   */
  class colorModelEstimator : public statisticsFunctor {
  public:
    /**
     * The parameters for the class colorModelEstimator
     */
    class parameters : public statisticsFunctor::parameters {
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
       * Dimensions for the 3D color model.
       *
       * Default: (32,32,32)
       */
      rgbPixel histogramDimensions;

    };

    /**
     * Default constructor
     */
    colorModelEstimator();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    colorModelEstimator(const colorModelEstimator& other);

    /**
     * Destructor
     */
    virtual ~colorModelEstimator();

    /**
     * Returns the name of this type ("colorModelEstimator")
     */
    virtual const char* getTypeName() const;

    /**
     * Reset the internal histogram to consider new elements.
     * The internal model will be empty, but with the correct size.
     */
    bool reset();

    /**
     * Consider a pixel for the model.
     * @param pix pixel to be considered
     * @param times how many times should the pixel be considered
     * @return true if successful or false otherwise.
     */
    bool consider(const rgbPixel& pix,const int times=1);

    /**
     * Consider a pixel for the model.
     *
     * This more time consuming consider method will not only add 1 to
     * the "winner" bin, but will also distribute the "1" vote using 
     * tri-linear interpolation among the seven neighbors.
     *
     * @param pix pixel to be considered
     * @param times how many times should the pixel be considered
     * @return true if successful or false otherwise.
     */
    bool smoothConsider(const rgbPixel& pix,const int times=1);

    /**
     * Consider a three dimensional pixel value given its three components.
     * It is assumed that the value range of all channels lies between 0.0 and
     * 1.0.
     * @param c1 first color component.
     * @param c2 second color component.
     * @param c3 third color component.
     * @param times number of times the given color point must be consider.
     */
    bool consider(const float& c1,const float& c2,const float& c3,
                  const int times=1);

    /**
     * Consider a pixel for the model.
     * @param pal the color palette to be considered
     * @return true if successful or false otherwise.
     */
    bool consider(const palette& pal);

    /**
     * Consider all pixels in the image.
     * @param img image with colors to be considered
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img);

    /**
     * Consider all pixels defined by the correspondent elements in the three
     * given channels.
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size.
     *
     * @param img image with colors to be considered
     * @param mask channel8 with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const channel8& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size.
     *
     * @param img image with colors to be considered
     * @param mask imatrix with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const imatrix& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @param mask channel8 with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3,
                  const channel8& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask is not zero.  The mask and the image must
     * have the same size.
     *
     * @param c1 first channel.
     * @param c2 second channel.
     * @param c3 third channel.
     * @param mask imatrix with the mask.  A value of zero means, that the
     *             corresponding pixel in the image must be ignored.
     * @return true if successful or false otherwise.
     */
    bool consider(const channel& c1,
                  const channel& c2,
                  const channel& c3,
                  const imatrix& mask);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask contains the value given by label.
     * The mask and the image must have the same size.
     *
     * This is seldom used.  See lti::computePalette, maybe that is
     * what you are looking for!
     *
     * @param img image with colors to be considered
     * @param mask channel8 with the mask.  
     * @param label only those pixel whose value in the mask are equal to
     *              this parameters will be considered.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const channel8& mask,const ubyte label);

    /**
     * Consider the pixels in the image, for which the corresponding
     * pixel in the mask contains the value given by label.
     * The mask and the image must have the same size.
     *
     * This is seldom used.  See lti::computePalette, maybe that is
     * what you are looking for!
     *
     * @param img image with colors to be considered
     * @param mask imatrix with the mask.  
     * @param label only those pixel whose value in the mask are equal to
     *              this parameters will be considered.
     * @return true if successful or false otherwise.
     */
    bool consider(const image& img,const imatrix& mask,const int label);


    /**
     * Consider all the images in the given list.
     *
     * @param imgs list of images to be considered
     * @return true if successful or false otherwise.
     */
    bool consider(const std::list<image>& imgs);

    /**
     * Consider all the images in the given list.  The mask list must have
     * the same size, and the corresponding images must also have the same
     * size.
     *
     * @param imgs list of images to be considered.
     * @param masks list of all masks.
     * @return true if successful or false otherwise.
     */
    bool consider(const std::list<image>& imgs,
                  const std::list<channel8>& masks);

    /**
     * Get a constant reference to the internal model
     */
    const thistogram<double>& getColorModel();

    /**
     * Get a copy of the internal model in the given %parameter.
     * @param model the color model (as 3D histogram) computed until now
     * @return true if apply successful or false otherwise.
     */
    bool apply(thistogram<double>& model) const;

    /**
     * Get the mean color and covariance matrix of the internal model
     * @param mean the mean color
     * @param covar covariance matrix of the color (3x3 matrix)
     * @return true if apply successful, false otherwise.
     */
    bool apply(trgbPixel<double>& mean,
               dmatrix& covar) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorModelEstimator& copy(const colorModelEstimator& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorModelEstimator& operator=(const colorModelEstimator& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     *
     * This overloading will initialize the size of the internal color model
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * The color model used.  If empty, it is assumed to be uninitialized.
     */
    thistogram<double> theModel;

    /**
     * Sum of colors until now
     */
    trgbPixel<double> sum;

    /**
     * Sum of outer-product of the colors until now (required for the
     * computation of the covariance matrix.
     */
    dmatrix sumOuter;

    /**
     * Number of pixels considered until now
     */
    int n;

    /**
     * Outer product of two colors.  The matrix MUST be 3x3 before calling
     * this method.
     */
    void outer(const trgbPixel<double>& a,
               const trgbPixel<double>& b,
               dmatrix& mat) const;

  private:
    /**
     * Iterator for dmatrices
     */
    dmatrix::iterator omit;

    /**
     * Index vector.  Attribute to reduce creation time...
     */
    ivector index;

    /**
     * Dimensions of the histogram (red)
     */
    int hdimRed;

    /**
     * Dimensions of the histogram (green)
     */
    int hdimGreen;

    /**
     * Dimensions of the histogram (blue)
     */
    int hdimBlue;

    /**
     * Equivalent to hdimRed-1;
     */
    int lastIdxRed;

    /**
     * Equivalent to hdimGreen-1;
     */
    int lastIdxGreen;

    /**
     * Equivalent to hdimBlue-1;
     */
    int lastIdxBlue;
  };
}

#endif

