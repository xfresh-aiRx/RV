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
 * file .......: ltiThresholding.h
 * authors ....: Alexandros Matsikis
 * organization: LTI, RWTH Aachen
 * creation ...: 26.04.2000
 * revisions ..: $Id: ltiThresholding.h,v 1.5 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_THRESHOLDING_H_
#define _LTI_THRESHOLDING_H_

#include "ltiImage.h"
#include "ltiTypes.h"
#include "ltiContour.h"

#include "ltiModifier.h"

namespace lti {
  /**
   * Threshold segmentation of a single channel (monochrom) image
   * or a contour. An upper and a lower threshold are used in
   * order to define the pixel-value-region where the thresholding
   * should operate.
   *
   * Attention: This class was ThresholdSegmentation, but 
   * the meaning of highThreshold and lowThreshold has been changed.
   */
  class thresholding : public modifier {
  public:
    /**
     * the parameters for the class thresholding
     */
    class parameters : public modifier::parameters {
    public:

      /**
       * Default constructor
       */
      parameters();

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
       * Specifies whheter the pixel values in the outer threshold range should 
       * be retained instead of beeing replaced by outRegionValue.
       *
       * Default: false
       */
      bool keepOutRegion;

      /**
       * Specifies whether the pixel values in the inner threshold range should 
       * be retained instead of beeing replaced by inRegionValue.
       *
       * Default: true
       */
      bool keepInRegion;

      /**
       * The value assigned to the pixels with values between the upper
       * and the lower threshold (inclusive).
       * In case of keepInRegion=true (the default value), the values of the
       * original image-pixels are acquired.
       * For channel8s this value must be between 0 and 1.0. (it is internally
       * scaled to ubyte.
       * For channels this value should be between 0 and 1.0
       *
       * Default: 1.0
       */
      float inRegionValue;

      /**
       * The value assigned to the pixels with values outside the
       * upper and the lower threshold (inclusive). In
       * case of keepOutRegion=true, the values of the
       * original image-pixels are acquired. 
       * For channel8s this value must be between 0 and 1.0. ( it is internally
       * scaled to ubyte).
       * For channels this value should be between 0 and 1.0
       *
       * Default: 0.0
       */
      float outRegionValue;

      /**
       * The upper threshold value.
       * This value should be between 0 and 1.0. 
       * For channel8s any value greater than 1 will be interpreted as 1.
       * (internally as 255)
       * The closed interval to consider a pixel-value in the region will
       * be [lowThreshold,highThreshold].
       * It must be hold highThreshold >= lowThreshold.
       *
       * Default: maximum value of float
       */
      float highThreshold;

      /**
       * The lower threshold value.
       * For channel8s this value must between 0 and 1.0. ( it is internally
       * scaled to ubyte) 
       * The closed interval to consider a pixel-value in the region will
       * be [lowThreshold,highThreshold].
       * It must be hold highThreshold >= lowThreshold
       *
       * Default: 0.5
       */
      float lowThreshold;
    };

    /**
     * default constructor
     */
    thresholding();

    /**
     * Construct a functor using the given parameters
     */
    thresholding(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    thresholding(const thresholding& other);

    /**
     * destructor
     */
    virtual ~thresholding();

    /**
     * returns the name of this type ("thresholding")
     */
    virtual const char* getTypeName() const;


    /**
     * operates on the given parameter.
     * @param srcdest channel8 with the source data.  The result
     *                         will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * segmentate the src channel and leave the result on dest
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * segmentate the src channel and leave the result on dest.
     * this apply is slower because everytime it is invoked a new Lookuptable
     * is generated
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @param lowThreshold low threshold value
     * @param highThreshold high threshold value (optional Default: 1)
     * @param keepInRegion if the inner pixels should be kept (optional Default: true)
     * @param keepOutRegion if the outer pixels should be kept (optional Default: false)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, channel8& dest, const float lowThreshold,
               const float highThreshold = 1.0f, 
               const bool keepInRegion = true,
               const bool keepOutRegion = false) const;

    /**
     * segmentates a channel on place
     * @param src channel with the source data. The result will be
     *             left here as well.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& src) const;

    /**
     * generates a mask for the channel src.
     * @param src channel with the source data.
     * @param dest channel where the resulting mask will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,channel8& dest) const;

    /**
     * segmentate the channel src, and leave the result in dest
     * @param src channel with the source data.
     * @param dest channel where the resulting mask will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;


   /** 
     * segmentate the src channel and leave the result on dest.
     * @param src channel with the source data
     * @param dest channel where the results will be left
     * @param lowThreshold low threshold value
     * @param highThreshold high threshold value (optional Default: 1.0f)
     * @param keepInRegion if the inner pixels should be kept (optional Default: true)
     * @param keepOutRegion if the outer pixels should be kept (optional Default: false)
     * @param inRegionValue if keepInRegion is false the inner pixels are replaced with this value 
     * (optional Default: 1.0f)
     * @param outRegionValue if keepOutRegion is false the outer pixels are replaced with this value 
     * (optional Default: 0.0f)
     */
    bool apply(const channel& src,channel& dest,
               const float lowThreshold, const float highThreshold=1.0f,
               const bool keepInRegion=true, const bool keepOutRegion=false,
               const float inRegionValue=1.0f, const float outRegionValue=0.0f) const;

    /**
     * segmentate the channel src, and generate an area-points-list on for
     * the given dest-contour
     * @param src channel8 with the source data.
     * @param dest the area point-list where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,areaPoints& dest) const;

    /**
     * segmentate the channel src, and generate an area-points-list on for
     * the given dest-contour
     * @param src channel with the source data.
     * @param dest the area point list where the result will be left.  Only the area
     *             points will be generated.  If you need another contour
     *             representation you can use the castFrom methods of the other
     *             point list types (see lti::borderPoints, lti::ioPoints)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,areaPoints& dest) const;

    /**
     * segmentate the src channel, and consider only the area points given
     * by the "comp" list.
     * @param src channel8 with the source data.
     * @param comp the list of points that should be segmented
     * @param dest point list where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
                      const areaPoints& comp, areaPoints& dest) const;


    /**
     * segmentate the src channel, and consider only the area points given
     * by the "comp" point list .
     * @param src channel with the source data.
     * @param comp the list of points that should be segmented
     * @param dest point list where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
                      const areaPoints& comp, areaPoints& dest) const;

    /**
     * copy data of "other" functor.
     */
    thresholding& copy(const thresholding& other);

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

   /**
    * Method to init the lookuptables.
    * The lookuptables will be used to accelerate the segmentation of channel8 data.
    * Usually they are initialized within invocation of setParameters() but also
    * in applies to channel8 which contains Thresholds as parameters.
    * @param lut where to store the look up table
    * @param mask where to store the mask table
    * @param lowThreshold low threshold value
    * @param highThreshold high threshold value
    * @param keepInRegion whether the inner pixels of the original channel8 should be kept
    * @param keepOutRegion whether the outer pixels of the original channel8 should be kept
    * @param inRegionValue if keepInRegion is false, the pixel will be overwritten with this value
    * @param outRegionValue if keepOutRegion is false, the pixel will be overwritten with this value
    */
    bool initLut(ubyte *lut,ubyte *mask, 
                 const float lowThreshold, const float highThreshold,
                 const bool keepInRegion, const bool keepOutRegion,
                 const float inRegionValue, const float outRegionValue) const;

  };
}

#endif

