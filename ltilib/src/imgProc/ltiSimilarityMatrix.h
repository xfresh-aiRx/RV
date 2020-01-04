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
 * file .......: ltiSimilarityMatrix.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 18.3.2002
 * revisions ..: $Id: ltiSimilarityMatrix.h,v 1.13 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SIMILARITY_MATRIX_H_
#define _LTI_SIMILARITY_MATRIX_H_

#include "ltiContour.h"
#include "ltiImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * Given a set of connected image regions (coded in a labeled mask
   * or std::list of area point lists) and the original color image,
   * this functor will compute a similarity measure between the
   * objects, and will code them in a similarity matrix.
   *
   * The first approach (BorderBased) will compute the sum of the
   * square root of the difference of the colors at the borders between two
   * both objects.  The second approach (AreaBased) will compute the
   * difference of the mean colors for each object.
   *
   * See also \ref segmPage
   */
  class similarityMatrix : public functor {
  public:
    /**
     * the parameters for the class similarityMatrix
     */
    class parameters : public functor::parameters {
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
       * type to specify which kind of color similarity should be
       * computed.
       */
      enum eModeType {
        BorderBased, /**< compute the similarity measure using the mean
                      *   value of square of the color differences at the
                      *   border between two objects
                      */
        AreaBased    /**< compute the similarity measure using the magnitude
                      *   of the difference of the mean colors of each object
                      */
      };

      /**
       * Color similarity modus (see eModusType)
       *
       * Default value: BorderBased
       */
      eModeType mode;

      /**
       * function pointer used when evaluating the distance between two
       * colors.
       *
       * Default value: lti::similarityMatrix::chromaticity2
       */
      double (*distFunction)(const rgbPixel&,const rgbPixel&);

    };

    /**
     * default constructor
     */
    similarityMatrix();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    similarityMatrix(const similarityMatrix& other);

    /**
     * destructor
     */
    virtual ~similarityMatrix();

    /**
     * returns the name of this type ("similarityMatrix")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param img original image.  All points in the list of areaPoints must
     *        lie inside the image, or an exception will be thrown.
     * @param nObj number of objects in the mask (without background).
     *             Is expected here in order to
     *             reduce its computation from the mask.  This value should be
     *             equal to the maximum value in the mask, i.e. the number of
     *             elements of the area points list used to generate the mask.
     * @param lmask label mask computed from the area points
     * @param colSimMat color similarity matrix
     * @param geoSimMat geometry similarity measure
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const int& nObj,
	       const imatrix& lmask,
               dmatrix& colSimMat,
               dmatrix& geoSimMat) const;


    /**
     * operates on the given %parameter.
     * @param img original image.  All points in the list of areaPoints must
     *        lie inside the image, or an exception will be thrown.
     * @param lapts list of area points.
     * @param colSimMat color similarity matrix
     * @param geoSimMat geometry similarity measure
     * @param lmask label mask computed from the area points
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& img,
               const std::list<areaPoints>& lapts,
               dmatrix& colSimMat,
               dmatrix& geoSimMat,
               imatrix& lmask) const;


    /**
     * computes for the given mask which labels are used in neighbor objects
     * for the given label.
     * @param nObj number of objects in the mask (without background).
     *             Is expected here in order to
     *             reduce its computation from the mask.  This value should be
     *             equal to the maximum value in the mask, i.e. the number of
     *             elements of the area points list used to generate the mask.
     * @param lmask label mask computed from the area points
     * @param label label for which the neighbors need to be found
     * @param neighborLabels resulting vector
     */
    bool neighbors(const int& nObj,
                   const imatrix& lmask,
                   const int& label,
                   ivector& neighborLabels) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    similarityMatrix& copy(const similarityMatrix& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    similarityMatrix& operator=(const similarityMatrix& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * @name Color Distances
     */
    //@{

    /**
     * This measure compares the differences between the color channels
     * (red-green)^2, (red-blue)^2 and (green-blue)^2 for each pixel,
     * and then the square of the differences between these values for
     * a and b.
     */
    static double euclideanLLL2(const rgbPixel& a,
				const rgbPixel& b);
    /**
     * square of the euclidean distance in RGB color space, i.e.
     * this returns a.distanceSqr(b)
     */
    static double euclideanRGB2(const rgbPixel& a,
                                const rgbPixel& b);

    /**
     * euclidean distance in the rg color space, ignoring the
     * intensity channel.
     */
    static double chromaticity2(const rgbPixel& a,
                                const rgbPixel& b);

    /**
     * distance of the pixels in the HSV color space.  The value
     * channel is ignored.  The angular distance of hue is always normed
     * from 0 to 255.  The Saturation value is also normed.
     *
     * Also usefull to ignore shadow based differences.
     */
    static double hueSaturation(const rgbPixel& a,
                                const rgbPixel& b);

    /**
     * distance of the pixels in the HSV color space, where only the hue
     * channel is considered.  The angular distance of hue is always normed
     * from 0 to 255.
     *
     * Also usefull to ignore shadow based differences.
     */
    static double hue(const rgbPixel& a,
                      const rgbPixel& b);

    /**
     * returns always 1.0
     * Usefull to create a matrix to know which areas are neighbors
     */
    static double neighbor(const rgbPixel& a,
			   const rgbPixel& b);

    //@}

  protected:

    /**
     * create a mask from the area points.
     * The mask must have the correct dimensions before calling this method.
     */
    void createMask(const std::list<areaPoints>& apts,
                    imatrix& mask) const;

    /**
     * create the border similarity matrix.
     * The size of the similarity matrix must be set before calling this
     * method.
     *
     * @param img original color image
     * @param mask labeled segmentation mask, created with createMask()
     * @param colBorderSimMat border color similarity mask, the size must be
     *                        set previously
     * @param colAreaSimMat area color similarity
     * @param geoSimMat geomatrical similarity measure
     */
    void similarity(const image& img,
                    const imatrix& mask,
                    dmatrix& colBorderSimMat,
                    dmatrix& colAreaSimMat,
                    dmatrix& geoSimMat) const;

  };
}

#endif


