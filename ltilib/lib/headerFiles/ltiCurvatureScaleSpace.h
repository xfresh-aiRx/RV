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
 * file .......: ltiCurvatureScaleSpace.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 24.6.2001
 * revisions ..: $Id: ltiCurvatureScaleSpace.h,v 1.18 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_CURVATURE_SCALE_SPACE_H_
#define _LTI_CURVATURE_SCALE_SPACE_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiContour.h"
#include "ltiLinearKernels.h"
#include "ltiTransform.h"
#include "ltiConvolution.h"
#include "ltiGaussKernels.h"

namespace lti {

  /**
   * Curvature Scale Space Representation (CSS)
   *
   * This functor extracts from a boundary (borderPoints) the CSS
   * representation as described in Mokhtarian, F. et. al "Robust and
   * efficient shape indexing through curvature scale space", British
   * Machine Vision Conference, 1996
   *
   * The apply methods will check if there are enough points in the
   * input parameters to compute the css.  If there are not enough points,
   * nothing will be done (see the apply methods for more details).
   */
  class curvatureScaleSpace : public transform {
  public:
    /**
     * the parameters for the class curvatureScaleSpace
     */
    class parameters : public transform::parameters {
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
       * if geometricDelta is true, the sigma increments will be multiplicative
       * i.e. sigma(1) = deltaSigma*sigma(0).  If false, arithmetical
       * increments will be done, (sigma(1) = deltaSigma + sigma(0)
       * Please note that for the geometricDelta, the deltaSigma should
       * be greater than one to get usefull results!
       *
       * Default value: false
       */
      bool geometricDelta;

      /**
       * Step for the std. deviation to generate the CSS-image.
       * This value will be added at each step to the previous sigma (if
       * geometricDelta is false) or will be multiplied with the last sigma
       * (if geometricDelta is true).  For the last case please ensure that
       * this value is greater than one, otherwise the results will be
       * unpredictable.
       *
       * Default value: 0.1
       */
      double deltaSigma;

      /**
       * The first std. deviation for the gaussian kernel used.
       *
       * Default value: 1.0
       */
      double startSigma;

      /**
       * Size for the first kernel being used.  Note that this is the
       * first kernel size.  As the std. deviation increases, the size of
       * the kernel will be updated automatically.
       * Default value: 9
       */
      int gaussianSize;

      /**
       * Normalized length size.  This will be the number of columns
       * of the resulting CSS image.  If negative, the length will not be
       * normalized and the number of columns of the resulting image will be
       * equal to the number of elements of the borderPoint-list given in the
       * apply method.
       *
       * Default value: 128
       */
      int normLength;

      /**
       * The sigmaAxisBlock specifies how many sigma steps will have a
       * block.  The result image will have an integer number of blocks,
       * which means, the number of rows of the resulting channel will be
       * a multiple of this value.
       * Default value: 128
       */
      int sigmaAxisBlock;

      /**
       * pixel value in the CSS image for a positive-negative zero
       * crossing.
       *
       * If you want to extract features, please ensure that this value is
       * different than negPosCross, otherwise the features could be wrong.
       *
       * (Default: 0)
       */
      ubyte posNegCross;

      /**
       * pixel value in the CSS image for a negative-positive zero
       * crossing.
       *
       * If you want to extract features, please ensure that this value is
       * different than posNegCross, otherwise the features could be wrong.
       *
       * (Default: 255)
       */
      ubyte negPosCross;

      /**
       * value for non-zero crossing (background of the CSS image)
       * (Default: 128)
       */
      ubyte nonCross;

      /**
       * If false, the CSS will be computed evolving the x and y components
       * of the border points using the derived gauss kernels.  If true,
       * the components will be first low-pass filtered and after that the
       * resulting vectors will be derived.  This second method is much faster
       * but for the geometricDelta mode and for small sigma regions it is
       * not precise.  If you need this functor for feature extraction, the
       * fast method will provide you with the maxima of the CSS "blobs" with
       * enough precision.
       * (Default value: true)
       */
      bool fastMethod;
    };

    /**
     * indices for the columns of the feature-vectors in the matrix
     */
    enum eFeatureIndices {
      MaxY = 0, /**< maximum sigma value of the blob */
      MaxX,     /**< position x (length index) of the blob maximum */
      Width,    /**< average width of the blob */
      Offset    /**< diference between the average width position and the
                     position x of the maximum */
    };

    /**
     * default constructor
     */
    curvatureScaleSpace();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    curvatureScaleSpace(const curvatureScaleSpace& other);

    /**
     * destructor
     */
    virtual ~curvatureScaleSpace();

    /**
     * returns the name of this type ("curvatureScaleSpace")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param src list of boundary points.  If this list has less than 9
     *            points (the smallest "circle" has 8 points), false will
     *            be returned with the respective message in the status string
     * @param dest the CSS image will be left here.
     * @param maxSigma the maximum sigma in the CSS image.
     * @param maxRow the row corresponding to the maximum sigma.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
                     channel8& dest,
                     float& maxSigma,
                     int& maxRow) const;

    /**
     * operates on the given %parameter.
     * @param src list of boundary points.  If this list has less than 9
     *            points (the smallest "circle" has 8 points), false will
     *            be returned with the respective message in the status string
     * @param dest the CSS image will be left here.
     * @param rows a list of points p, where p.x contains the number of
     *             blobs crossing the row of \a dest with index p.y.
     *             In a "clean" CSS representation the number of blobs has
     *             to decrease or be equal with increasing index p.y.  If you
     *             detect here the opposite effect is because there is too much
     *             noise in the high-resolution levels (small variance of
     *             Gaussians).
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
                     channel8& dest,
                     pointList& rows) const;


    /**
     * operates on the given %parameter.
     * @param src list of boundary points.  If this list has less than 9
     *            points (the smallest "circle" has 8 points), false will
     *            be returned with the respective message in the status string
     * @param dest the CSS image will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
                     channel8& dest) const;

    /**
     * operates on the given %parameter.  This member computes the CSS-image
     * and the feature vectors.  Each row of the matrix will correspond to
     * a feature vector which contains:
     * - Column 0: (alias for 0: MaxY) the value of the maximum of the blob.
     * - Column 1: (alias for 1: MaxX) the x-position of maximum of the blob.
     * - Column 2: (alias for 2: Width) the average width
     * - Column 3: (alias for 3: Offset) the offset between the x-position of
     *                                   the maximum of the blob and the center
     *                                   of the average width.
     *
     * @param src list of boundary points.  If this list has less than 9
     *            points (the smallest "circle" has 8 points), false will
     *            be returned with the respective message in the status string
     * @param css the CSS image will be left here.
     * @param vcts the feature vectors packed in a matrix.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
                     channel8& css,
                     dmatrix& vcts) const;

    /**
     * operates on the given %parameter.  This member computes the CSS-image
     * and the feature vectors.  Each row of the matrix will correspond to
     * a feature vector which contains:
     * - Column 0: (alias for 0: MaxY) the value of the maximum of the blob.
     * - Column 1: (alias for 1: MaxX) the x-position of maximum of the blob.
     * - Column 2: (alias for 2: Width) the average width
     * - Column 3: (alias for 3: Offset) the offset between the x-position of
     *                                   the maximum of the blob and the center
     *                                   of the average width.
     *
     * @param src list of boundary points.  If this list has less than 9
     *            points (the smallest "circle" has 8 points), false will
     *            be returned with the respective message in the status string
     * @param css the CSS image will be left here.
     * @param vcts the feature vectors packed in a matrix.
     * @param rows a list of points p, where p.x contains the number of
     *             blobs crossing the row of \a dest with index p.y.
     *             In a "clean" CSS representation the number of blobs has
     *             to decrease or be equal with increasing index p.y.  If you
     *             detect here the opposite effect is because there is too much
     *             noise in the high-resolution levels (small variance of
     *             Gaussians).
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
                     channel8& css,
                     dmatrix& vcts,
                     pointList& rows) const;

    /**
     * operates on the given %parameter.
     * This member computes the CSS-image and the feature vectors.
     * Each row of the matrix will correspond to a feature vector
     * which contains:
     * - Column 0: (alias for 0: MaxY) the value of the maximum of the blob.
     * - Column 1: (alias for 1: MaxX) the x-position of maximum of the blob.
     * - Column 2: (alias for 2: Width) the average width
     * - Column 3: (alias for 3: Offset) the offset between the x-position of
     *                                   the maximum of the blob and the center
     *                                   of the average width.
     *
     * @param src list of boundary points.  If this list has less than 9
     *            points (the smallest "circle" has 8 points), false will
     *            be returned with the respective message in the status string
     * @param vcts feature vectors.  Each row corresponds to a CSS "blob"
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src,
                     dmatrix& vcts) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    curvatureScaleSpace& copy(const curvatureScaleSpace& other);

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
     * extract the x and y components of the borderPoints object and
     * resize (normalize the length) of the vectors if required.
     */
    bool borderToXY(const borderPoints& src,
                          vector<float>& theX,
                          vector<float>& theY) const;


    /**
     * calculate the gaussian first and second derivative kernels.
     * The size is calcultated considering the parameters object.
     */
    void calcKernels(const float& sigma,
                           kernel1D<float>& first,
                           kernel1D<float>& second) const;

    /**
     * calculate the curvature using the derivatives
     */
    int curvature(const float& sigma,
                  const vector<float>& theX,
                  const vector<float>& theY,
                        vector<float>& curv,
                        vector<ubyte>& zeroCross) const;

    /**
     * calculate the gaussian first and second derivative kernels.
     * The size of the kernel is recomputed considering the parameters object.
     * The sigma term will be updated to the new one.
     */
    void calcKernels(float& sigma,
                     gaussKernel1D<float>& gaussian) const;

    /**
     * calculate the curvature using the derivatives
     */
    int curvature(const gaussKernel1D<float>& gaussian,
                        vector<float>& theX,
                        vector<float>& theY,
                        vector<float>& curv,
                        vector<ubyte>& zeroCross,
                  const int& lastZC) const;

    /**
     * operates on the given %parameter.
     * @param src list of boundary points.
     * @param dest the CSS image will be left here.
     * @param maxSigma the maximum sigma in the CSS image.
     * @param maxRow the row corresponding to the maximum sigma.
     * @param rows a list of points, where x contains the number of
     *             maxima of the row with the index given by y.
     * @return true if apply successful or false otherwise.
     */
    bool cssOriginal(const borderPoints& src,
                           channel8& dest,
                           float& maxSigma,
                           int& maxRow,
                           pointList& rows) const;

    /**
     * operates on the given %parameter.
     * @param src list of boundary points.
     * @param dest modified CSS image will be left here.
     * @param maxSigma the maximum sigma in the CSS image.
     * @param maxRow the row corresponding to the maximum sigma.
     * @param rows a list of points, where x contains the number of
     *             maxima of the row with the index given in the point's y.
     * @return true if apply successful or false otherwise.
     */
    bool cssFast(const borderPoints& src,
                 channel8& dest,
                 float& maxSigma,
                 int& maxRow,
                 pointList& rows) const;

    void derive(const vector<float>& theX,
                      vector<float>& xu,
                      vector<float>& xuu) const;



    /**
     * calculate the zero crossings.  The output vector MUST have the
     * same size as the input vector.
     *
     * This member returns the number of zero crossings detected.
     */
    int zeroCross(const vector<float>& curv,
                        vector<ubyte>& dest,
                        const ubyte& posNeg,
                        const ubyte& negPos) const;

    /**
     * extract the blobs from the css image (only the first 127 blobs will
     * be extracted)
     */
    int extractBlobs(const channel8& css,
                     const pointList& rows,
                           matrix<int>& blobs,
                           dmatrix& features) const;

  };
}

#endif
