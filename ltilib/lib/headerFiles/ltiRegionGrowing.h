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
 * file .......: ltiRegionGrowing.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.9.2000
 * revisions ..: $Id: ltiRegionGrowing.h,v 1.17 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_REGION_GROWING_H_
#define _LTI_REGION_GROWING_H_

#include "ltiObject.h"
#include "ltiTypes.h"
#include "ltiSegmentation.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiImage.h"
#include "ltiMath.h"

namespace lti {
  /**
   * This class is used to segmentate an image with a regular
   * background.  It uses a set of seed pixels (given in the
   * parameters) to start the region growing.  To check the
   * similarity, a comparition is done between the pixel value at the
   * candidate position and the average value at the
   * seed position.  If the difference of these values is lower than a given
   * threshold, the pixel will be considered as part of the growing
   * region.  It is possible to get automatically values for these thresholds
   * if a known background region is given in the parameters.
   *
   * Example:
   *
   * \code
   * lti::loadBMP loader;
   *
   * lti::image img;     // the color image to be segmented
   * lti::channel8 chnl; // the segmentation mask will be left here
   *
   * loader.load("img/leo040801_00_019.bmp",img); // read an image
   *
   * lti::regionGrowing segmenter;         // segmentation functor and
   * lti::regionGrowing::parameters param; // its parameters
   *
   * param.useGaussKernel = true;            // use a gaussian kernel 5x5
   * param.localStatisticsKernelSize = 5;
   * param.localStatisticsKernelVariance = -1;
   *
   * param.mode = lti::regionGrowing::parameters::UseGivenThresholds;
   * param.averageThresholds = trgbPixel<float>(0.05,0.05,0.05);
   * param.edgesThreshold = 0.04;
   *
   * // do not use all corners as seed-points, just the three following...
   * param.seedPoints.resize(3,tpoint<float>(0,0));
   * param.seedPoints.at(1).y = 1.0f;
   * param.seedPoints.at(2).x = 1.0f;
   * param.seedPoints.at(2).y = 1.0f;
   *
   * // indicate the segmentation functor the user-parameters
   * segmenter.setParameters(param);
   *
   * // segmentate the image and leave the segmentation in the channel
   * segmenter.apply(img,chnl);
   *
   * view1.show(img);  // show original image
   * view2.show(chnl); // show the segmentation mask
   *
   * \endcode
   *
   * @ingroup gSegmentation
   */
  class regionGrowing : public segmentation {
  public:
    /**
     * The parameters for the class regionGrowing
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
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      // -----------------------------------------------------------------

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * Write the parameters in the given ioHandler
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
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /**
       * Types for the region growing modi
       */
      enum eMode {
        UseGivenThresholds,    /*!< Use the thresholds specified in the
                                    parameters */
        GetThresholdsRelative, /*!< Use the relative patchPosition to obtain
                                    the threshold values */
        GetThresholdsAbsolute  /*!< Use the absolute patchPosition to obtain
                                    the threshold values */
      };

      /**
       * Types of "lightness" for the background
       */
      enum eBackgroundType {
        Dark,   /*!< The background has only dark colors */
        Medium, /*!< The background has colors with medium intensity */
        Light   /*!< The background is white */
      };

      /**
       * Type (brightness) of the background.  The default value is "Dark".
       */
      eBackgroundType backgroundType;

      /**
       * If this variable is "true", the local region will be specified
       * by a gassian filter with the given (local region) size and variance.
       * If "false", a rectangular filter will be used.  The default value is
       * "false"
       */
      bool useGaussKernel;

      /**
       * Size of the filter kernel used to specify a local region.
       * The default value is 5
       */
      int localStatisticsKernelSize;

      /**
       * Variance of the filter kernel used to specify a local region
       * (used only if useGaussKernel is true).  The default value is
       * "-1", which meaning is described in gaussKernel
       */
      double localStatisticsKernelVariance;

      /**
       * Size of a gaussian kernel used to low-pass-filter the final
       * segmentated mask.
       */
      int smoothingKernelSize;

      /**
       * Smoothing threshold.  After the first segmentation, all pixels
       * will be assigned to the background or the object.  The smoothing
       * filter will generate at the borders of the objects values between
       * 0 (background) and 1 (object).  This value will determine which values
       * will be considered as object.
       *
       * The default value is 0.6
       */
      float smoothingThreshold;

      /**
       * Specify the position in the image or channel, where the
       * values for the "background" can be taken.
       *
       * If the mode is GetThresholdsRelative, the values of the
       * coordinates must be between 0.0 and 1.0.  The absolute
       * coordinates will be calculated multiplying this values with
       * the size of the image to be segmented.
       *
       * If the mode is GetThresholdsAbsolute, this parameters should
       * contain the absolute coordinates for the background patch.
       */
      trectangle<float> patchPosition;

      /**
       * Specify how to obtain the threshold values to considered a pixel
       * similar to the seed or not.
       */
      eMode mode;

      /**
       * The scale factor allows a faster calculation for the segmentation
       * mask, but with a lower boundary detection accuracy.
       */
      int scaleFactor;

      /**
       * Used by the UseGivenThresholds-mode for the segmentation of a
       * channel or channel8.  This value MUST be between 0.0 and 1.0
       * (The channel8 values will be divided by 255 before the comparitions)
       */
      float averageThreshold;

      /**
       * Used by the UseGivenThresholds-mode for the segmentation of a
       * color image.  Each component value must be between 0.0 and 1.0
       *
       */
      trgbPixel<float> averageThresholds;

      /**
       * Used by the UseGivenThresholds-mode for the segmentation of a
       * channel or channel8.  This value must be between 0.0 and 1.0.
       * (The channel8 values will be divided by 255 before the comparitions)
       *
       */
      float edgesThreshold;

      /**
       * A list with the relative positions for the starting seed points
       * can be specified here.  The valid values for each coordinate are
       * between 0.0f and 1.0f.  The default values are the four corners
       * (i.e. (0,0),(0,1),(1,0),(1,1) )
       */
      vector< tpoint<float> > seedPoints;
    };

    /**
     * Default constructor
     */
    regionGrowing();

    /**
     * Default constructor with parameters
     */
    regionGrowing(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    regionGrowing(const regionGrowing& other);

    /**
     * Destructor
     */
    virtual ~regionGrowing();

    /**
     * Returns the name of this type ("regionGrowing")
     */
    virtual const char* getTypeName() const;

    /**
     * Operates on the given parameter.
     *
     * The background-pixels will be set to 0,128 or 255 depending on
     * the parameter "backgroundType".
     *
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return a reference to the <code>srcdest</code>.
     */
    channel8& apply(channel8& srcdest) const;

    /**
     * Operates on the given parameter.
     *
     * The background-pixels will be set to 0, 0.5 or 1.0 depending on
     * the parameter "backgroundType".
     *
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return a reference to the <code>srcdest</code>.
     */
    channel& apply(channel& srcdest) const;

    /**
     * Operates on the given parameter.
     *
     * The background-pixels will be set to black, grey or white depending on
     * the parameter "backgroundType".
     *
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return a reference to the <code>srcdest</code>.
     */
    image& apply(image& srcdest) const;

    /**
     * Operates on a copy of the given %parameters.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the segmentation mask will be left.
     *             This mask contains 0 for a background point and
     *             (usually) 255 for an object point.
     * @return a reference to the <code>dest</code>.
     */
    channel8& apply(const channel8& src,channel8& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel8 where the segmentation mask will be left.
     *             This mask contains 0 for a background point and
     *             (usually) 255 for an object point.
     * @return a reference to the <code>dest</code>.
     */
    channel8& apply(const channel& src,channel8& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest channel8 where the segmentation mask will be left.
     *             This mask contains 0 for a background point and
     *             (usually) 255 for an object point.
     * @return a reference to the <code>dest</code>.
     */
    channel8& apply(const image& src,channel8& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionGrowing& copy(const regionGrowing& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    static float sqroot(const float& x) {
      return static_cast<float>(sqrt(x));
    };

    static int sqroot(const int& x) {
      return static_cast<int>(sqrt(x));
    };

    /**
     * The pixel at position "neighbour" will be compared with the average
     * value corresponding to the position "seed".
     *
     * The Codition for similarity proof if the absolute value of
     * the difference between the channel value at "neighbour" and the
     * local average on the "seed"-position is lower than the given
     * avrgThreshold
     *
     */
    inline bool similar(const ubyte& neighbour,
                        const ubyte& avrgAtSeed,
                        const int& avrgThreshold) const {
      return (abs(int(neighbour)-int(avrgAtSeed))<=avrgThreshold);
    }

    /**
     * The pixel at position "neighbour" will be compared with the average
     * value corresponding to the position "seed".
     *
     * The Codition for similarity proof if the absolute value of
     * the difference between the channel value at "neighbour" and the
     * local average on the "seed"-position is lower than the given
     * avrgThreshold
     *
     */
    inline bool similar(const float& neighbour,
                        const float& avrgAtSeed,
                        const float& avrgThreshold) const {
      return (fabs(neighbour - avrgAtSeed)<=avrgThreshold);
    }

    /**
     * All pixels at img for which the mask is 0 will be set to the given
     * value.  The mask and img MUST have the same size!
     */
    template <class T>
    void mask(matrix<T>& img,const channel8& msk,const T& background) const {
      channel8::const_iterator cit,ce;
      typename matrix<T>::iterator it;
      for (cit=msk.begin(),ce=msk.end(),it=img.begin();
           cit!=ce;
           cit++,it++) {
        if ((*cit) == 0) {
          (*it)=background;
        }
      }
    }

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest the mask will be 0 for background pixels
     * @return a reference to the <code>dest</code>.
     */
    channel8& segmentate(const image& src,channel8& dest) const;

    /**
     * Operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest the mask will be 0 for background pixels
     * @return a reference to the <code>dest</code>.
     */
    channel8& segmentate(const channel& src,channel8& dest) const;
  };
}

#endif

