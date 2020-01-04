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
 * file .......: ltiThresholdSegmentation.h
 * authors ....: Alexandros Matsikis
 * organization: LTI, RWTH Aachen
 * creation ...: 26.04.2000
 * revisions ..: $Id: ltiThresholdSegmentation.h,v 1.24 2005/06/28 10:13:07 canzler Exp $
 */

/**
 * \file ltiThresholdSegmentation.h Deprecated.  Use
 * ltiThresholding.h instead
 */

#include "ltiMacroSymbols.h"

#ifndef _LTI_IGNORE_THRESH_DEP_WARNING
#ifdef _LTI_MSC_VER
#pragma message("ltiThresholdSegmentation.h deprecated, use ltiThresholding.h instead!")
#else
#warning "ltiThresholdSegmentation.h deprecated, use ltiThresholding.h instead!"
#endif
#endif

#ifndef _LTI_THRESHOLD_SEGMENTATION_H_
#define _LTI_THRESHOLD_SEGMENTATION_H_

#include "ltiImage.h"
#include "ltiTypes.h"
#include "ltiContour.h"

#include "ltiSegmentation.h"

namespace lti {
  /**
   * Threshold segmentation of a single channel (monochrom) image
   * or a contour. An upper and a lower threshold are used in
   * order to define the pixel-value-region where the thresholding
   * should operate.
   */
  class thresholdSegmentation : public segmentation {
  public:
    /**
     * the parameters for the class thresholdSegmentation
     */
    class parameters : public segmentation::parameters {
    public:
      /**
       * default constructor
       * Per default, the threshold values will be defined in the value
       * range of channel8 (0 to 255).  If you want to work with channels
       * than specify in the constructor of the parameters (false), this
       * way the value range of the thresholds will lay between 0 and 1.0
       */
      parameters(const bool& useChannel8 = true);

      /**
       * copy constructor
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
       * copy member
       */
      parameters& copy(const parameters& other);

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
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /**
       * Constant value used to specify that the channel pixel values
       * should be retained in the segmented image, instead of generating
       * a mask, with only two values.
       */
      static const float Original;

      /**
       * The value assigned to the pixels with values between the upper
       * and the lower threshold (inclusive) or "Original".
       * In case of Original (the default value), the values of the
       * original image-pixels are acquired.
       * For channel8s this value must be between 0 and 255.
       * For channels this value should be between 0 and 1.0
       */
      float inRegionValue;

      /**
       * The value assigned to the pixels with values outside the
       * upper and the lower threshold (inclusive) or "Original". In
       * case of Original, the values of the
       * original image-pixels are acquired. (Default: 0.0)
       * For channel8s this value must be between 0 and 255.
       * For channels this value should be between 0 and 1.0
       */
      float outRegionValue;

      /**
       * The upper threshold value.
       * Default value: 255 for channel8 or 1.0 for channels (see constructor
       * parameters(const bool)).
       * Note that this value will be interpreted "as is" by the apply methods,
       * i.e., if you say the highThreshold is 0.8 (default value for channels)
       * and you apply the functor to a channel8, than eevery pixel will be
       * considered to lay out of the region!
       * The closed interval to consider a pixel-value in the region will
       * be [lowThreshold,highThreshold].
       * It must be hold highThreshold >= lowThreshold
       */
      float highThreshold;

      /**
       * The lower threshold value.
       * Default value: 0
       * Note that this value will be interpreted "as is" by the apply methods,
       * i.e., if you say the lowThreshold is 127, then for a "standard"
       * channel, which usually contains values <= 1.0, everything will lay
       * out of the region!
       * The closed interval to consider a pixel-value in the region will
       * be [lowThreshold,highThreshold].
       * It must be hold highThreshold >= lowThreshold
       */
      float lowThreshold;
    };

    /**
     * default constructor
     */
    thresholdSegmentation();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    thresholdSegmentation(const thresholdSegmentation& other);

    /**
     * destructor
     */
    virtual ~thresholdSegmentation();

    /**
     * returns the name of this type ("thresholdSegmentation")
     */
    virtual const char* getTypeName() const;


    /**
     * operates on the given parameter.
     * @param srcdest channel8 with the source data.  The result
     *                         will be left here too.
     * @return a reference to the <code>srcdest</code>.
     */
    channel8& apply(channel8& srcdest) const;

    /**
     * segmentate the src channel and leave the result on dest
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    channel8& apply(const channel8& src,channel8& dest) const;

    /**
     * segmentates a channel on place
     * @param src channel with the source data. The result will be
     *             left here as well.
     * @return a reference to the <code>dest</code>.
     */
    channel& apply(channel& src) const;

    /**
     * generates a mask for the channel src.
     * If the parameters specified "Original" for the output values, the
     * values between 0.0 and 1.0 will be linearly mapped between 0 and 255.
     * @param src channel with the source data.
     * @param dest channel where the resulting mask will be left.
     * @return a reference to the <code>dest</code>.
     */
    channel8& apply(const channel& src,channel8& dest) const;

    /**
     * segmentate the channel src, and leave the result in dest
     * @param src channel with the source data.
     * @param dest channel where the resulting mask will be left.
     * @return a reference to the <code>dest</code>.
     */
    channel& apply(const channel& src,channel& dest) const;

    /**
     * segmentate the channel src, and generate an area-points-list on for
     * the given dest-contour
     * @param src channel8 with the source data.
     * @param dest the area point-list where the result will be left.
     * @return a reference to the <code>dest</code>. contour is of IO type
     */
    areaPoints& apply(const channel8& src,areaPoints& dest) const;

    /**
     * segmentate the channel src, and generate an area-points-list on for
     * the given dest-contour
     * @param src channel with the source data.
     * @param dest the area point list where the result will be left.  Only the area
     *             points will be generated.  If you need another contour
     *             representation you can use the castFrom methods of the other
     *             point list types (see lti::borderPoints, lti::ioPoints)
     * @return a reference to the <code>dest</code>. contour is of IO type
     */
    areaPoints& apply(const channel& src,areaPoints& dest) const;

    /**
     * segmentate the src channel, and consider only the area points given
     * by the "comp" list.
     * @param src channel8 with the source data.
     * @param comp the list of points that should be segmented
     * @param dest point list where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    areaPoints& apply(const channel8& src,
                      const areaPoints& comp, areaPoints& dest) const;


    /**
     * segmentate the src channel, and consider only the area points given
     * by the "comp" point list .
     * @param src channel with the source data.
     * @param comp the list of points that should be segmented
     * @param dest point list where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    areaPoints& apply(const channel& src,
                      const areaPoints& comp, areaPoints& dest) const;

    /**
     * copy data of "other" functor.
     */
    thresholdSegmentation& copy(const thresholdSegmentation& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * sets the functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     */
    virtual bool updateParameters();

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * This look up table will be used to accelerate the segmentation of
     * channel8 data.  It contains the value to be assigned to the
     * destination channel
     * It will be generated within the member setParameters
     */
    ubyte lut[256];

    /**
     * This look up table will be used to accelerate the segmentation of
     * channel8 data.  It contains 0 if the value must be considered as
     * background or 1 otherwise.
     * It will be generated within the member setParameters
     */
    ubyte mask[256];

  };
}

#endif

