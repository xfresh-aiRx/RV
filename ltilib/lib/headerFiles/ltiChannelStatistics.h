/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004
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
 * file .......: ltiChannelStatistics.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 28.3.2002
 * revisions ..: $Id: ltiChannelStatistics.h,v 1.15 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CHANNEL_STATISTICS
#define _LTI_CHANNEL_STATISTICS

#include "ltiGlobalFeatureExtractor.h"
#include "ltiContour.h"
#include "ltiColors.h"
#include "ltiSplitImage.h"

namespace lti {
  /**
   * This functor splits an image into three channels of a color space
   * set in the parameters (see parameters::setSplitter() ). From
   * these channels it then calculates the Minimum, Maximum, Average,
   * Median or Average and Variance over their values, depending on
   * the parameter type.
   *
   * It is possible to define a "transparent color" which will be
   * ignored by the calculation.
   */
  class channelStatistics : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class channelStatistics
     */
    class parameters : public globalFeatureExtractor::parameters {
    public:

      /**
       * Specification of the filter type used on each channel to
       * calculate the feature %vector.
       * @see apply()
       * @see getMin()
       * @see getMax()
       * @see getAverage()
       * @see getGaussian()
       * @see getMedian()
       */
      enum eBRGBFilterType {
        Minimum=0, /*!< find the minimum value of each channel */
        Maximum, /*!< find the maximum value of each channel */
        Average, /*!< find the average value of each channel */
        Gaussian, /*!< find the mean and variance of each channel */
        Median /*!< find the median of all values of each channel */
      };

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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * Kind of calculation rule used to get the values. Default value
       * is Average
       */
      eBRGBFilterType type;

      /**
       * Indicates whether the %parameters own the splitImage pointer or not.
       * If they do, the pointer will be deleted in the destructor, if they
       * don't, the class using the channelStatistics is responsible for
       * deleting the the pointer. Default is true.<p>
       * When channelStatistics is read from a file, this parameter is set
       * to true, since a new pointer is generated.
       */
      bool ownsSplitter;

      /**
       * The color which is ignored when the statistics is built.
       * This works only with the unmasked apply methods. If a mask
       * or an areapoint object is given, these are used instead and 
       * this parameter is ignored.
       */
      rgbColor ignoreColor;

      /**
       * set the splitImage %functor to be used.  This will delete the
       * actual splitter (if this %object owns it), before assigning it
       * to the actual parameter pointer.
       *
       * @param theSplitter a pointer to the new functor to be used
       * @param own if true, this parameter object will take control of
       *            the memory deallocation of the object.  If false,
       *            this %object will never delete the %functor.
       */
      void setSplitter(splitImage* theSplitter,const bool own = true);

      /**
       * get a reference to the splitter object
       */
      const splitImage& getSplitter() const;

    protected:
      /**
       * The splitImage %functor used for selecting the channels. Default
       * is splitImageToRGB.
       */
      splitImage* splitter;
    };

    /**
     * default constructor
     */
    channelStatistics();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    channelStatistics(const channelStatistics& other);

    /**
     * destructor
     */
    virtual ~channelStatistics();

    /**
     * returns the name of this type ("channelStatistics")
     */
    virtual const char* getTypeName() const;

    /**
     * Calculates statics for each channel of the part of the given image
     * indicated by the areaPoints. The kind of channels used depends on the
     * parameter splitter which is a pointer of splitImage. The different
     * statistics are chosen through the parameter type: <p>
     * <table>
     * <tr>
     * <td>Minimum</td><td>Minimum of each channel</td><td>3 dimensions</td>
     * </tr><tr>
     * <td>Maximum</td><td>Maximum of each channel</td><td>3 dimensions</td>
     * </tr><tr>
     * <td>Average</td><td>Average value of each channel</td>
     * <td>3 dimensions</td>
     * </tr><tr>
     * <td>Gaussian</td><td>First the mean values of each channel</td>
     * <td>6 dimensions</td>
     * <td></td><td>then the variances</td><td></td>
     * </tr><tr>
     * <td>Median</td><td>Median value of each channel</td>
     * <td>3 dimensions</td>
     * </tr></table>
     * @param src image for feature extraction.
     * @param obj area of the image that contains the %object
     * @param dest the feature %vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src, const areaPoints& obj, dvector& dest) const;

    /**
     * Calculates statics for each channel of the given image.
     * The kind of channels used depends on the
     * parameter splitter which is a pointer of splitImage. 
     * Points which have the color param.ignoreColor are ignored.
     * @param src image for feature extraction.
     * @param dest the feature %vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src, dvector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    channelStatistics& copy(const channelStatistics& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    channelStatistics& operator=(const channelStatistics& other);

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
     * Calculates the minimum value of the points in obj of the given
     * %channel .
     * @param ch channel to be evaluated
     * @param obj list of points belonging to the %object
     * @param dest the minimum value
     * @return true if apply successful or false otherwise.
     */
    bool getMin(const channel& ch, const areaPoints& obj, double& dest) const;

   /**
     * Calculates the maximum value of the points in obj of the given
     * %channel .
     * @param ch channel to be evaluated
     * @param obj list of points belonging to the %object
     * @param dest the maximum value
     * @return true if apply successful or false otherwise.
     */
    bool getMax(const channel& ch, const areaPoints& obj, double& dest) const;

    /**
     * Calculates the average value of the points in obj of the given
     * %channel .
     * @param ch channel to be evaluated
     * @param obj list of points belonging to the %object
     * @param dest the average value
     * @return true if apply successful or false otherwise.
     */
    bool getAverage(const channel& ch, const areaPoints& obj,
                    double& dest) const;

    /**
     * Calculates the Gaussian, ie the mean and variance, of the points in
     * obj of the given %channel .
     * @param ch channel to be evaluated
     * @param obj list of points belonging to the %object
     * @param m the mean value
     * @param v the variance
     * @return true if apply successful or false otherwise.
     */
    bool getGaussian(const channel& ch, const areaPoints& obj,
                     double& m, double& v) const;

   /**
     * Calculates the median of all values of the points in obj of the
     * given %channel .
     * @param ch channel to be evaluated
     * @param obj list of points belonging to the %object
     * @param dest the median of all values
     * @return true if apply successful or false otherwise.
     */
    bool getMedian(const channel& ch, const areaPoints& obj,
                   double& dest) const;


    /**
     * Calculates the minimum value of the points of the given
     * %channel for which the mask entry is true.
     * @param ch channel to be evaluated
     * @param mask flag mask which tells for each point if it should
     *             be included in the statistics.
     * @param dest the minimum value
     * @return true if apply successful or false otherwise.
     */
    bool getMin(const channel& ch, 
                const channel8& mask, double& dest) const;

    /**
     * Calculates the maximum value of the points of the given
     * %channel for which the mask entry is true.
     * @param ch channel to be evaluated
     * @param mask flag mask which tells for each point if it should
     *             be included in the statistics.
     * @param dest the minimum value
     * @return true if apply successful or false otherwise.
     */
    bool getMax(const channel& ch,
                const channel8& mask, double& dest) const;

    /**
     * Calculates the average value of the points of the given
     * %channel for which the mask entry is true.
     * @param ch channel to be evaluated
     * @param mask flag mask which tells for each point if it should
     *             be included in the statistics.
     * @param dest the minimum value
     * @return true if apply successful or false otherwise.
     */
    bool getAverage(const channel& ch, const channel8& mask,
                    double& dest) const;

    /**
     * Calculates a gaussian distribution of the points of the given
     * %channel for which the mask entry is true.
     * @param ch channel to be evaluated
     * @param mask flag mask which tells for each point if it should
     *             be included in the statistics.
     * @param m the mean of the computed distribution
     * @param v the variance of the computed distribution
     * @return true if apply successful or false otherwise.
     */
    bool getGaussian(const channel& ch, const channel8& mask,
                     double& m, double& v) const;

    /**
     * Calculates the median of the points of the given
     * %channel for which the mask entry is true.
     * @param ch channel to be evaluated
     * @param mask flag mask which tells for each point if it should
     *             be included in the statistics.
     * @param dest the minimum value
     * @return true if apply successful or false otherwise.
     */
    bool getMedian(const channel& ch, const channel8& mask,
                   double& dest) const;


  };
}

#endif

