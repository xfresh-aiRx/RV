/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2004
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
 * file .......: ltiBrightRGB.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 28.2.2002
 * revisions ..: $Id: ltiBrightRGB.h,v 1.12 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BRIGHT_R_G_B_H_
#define _LTI_BRIGHT_R_G_B_H_

#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * This functor splits an image into its three channels (R.G,b) and
   * then calculates the Minimum, Maximum, Average, Median or Average
   * and Variance over their values, depending on the parameter type.
   *
   * It is possible to define a "transparent color" which will be
   * ignored by the calculation.
   */
  class brightRGB : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class brightRGB
     */
    class parameters : public globalFeatureExtractor::parameters {
    public:

      /**
       * Specification of the filter type used on each channel to
       * calculate the feature %vector. For all modes the transparent
       * color is not taken into account.
       * @see getMin()
       * @see getMax()
       * @see getAverage()
       * @see getMedian()
       */
      enum eBRGBFilterType {
        Minimum=0, /*!< find the minimum value of each channel */
        Maximum, /*!< find the maximum value of each channel */
        Average, /*!< find the average value of each channel */
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
       * define if there is a transparent color
       * i.e. is there a color to be ignored by the calculation
       * default value is true.
       */
      bool transparent;

      /**
       * color which will ignored by the calculation. Default value is
       * lti::Black.
       */
      rgbPixel transColor;
    };

    /**
     * default constructor
     */
    brightRGB();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    brightRGB(const brightRGB& other);

    /**
     * destructor
     */
    virtual ~brightRGB();

    /**
     * returns the name of this type ("brightRGB")
     */
    virtual const char* getTypeName() const;

    /**
     * Calculates the Minimum, Maximum, Average or Median of each channel
     * of the given image, depending on the value of the parameter type.
     * @param src image which is analysed by the calculation rule
     * @param dest the result value per channel
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,dvector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    brightRGB& copy(const brightRGB& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    brightRGB& operator=(const brightRGB& other);

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
     * Calculates the minimum value of each channel of the image.
     * If the %parameter transparent is true, the %transparent %color
     * defined by the %parameter transColor is not taken into account.
     * @param img image to be evaluated
     * @param dest vector with the minimum value of all not transparent points
     *             between R, G and B.
     * @return true if apply successful or false otherwise.
     */
    bool getMin(const image& img,dvector& dest) const;

   /**
     * Calculates the maximum value of each channel of the image.
     * If the %parameter transparent is true, the %transparent %color
     * defined by the %parameter transColor is not taken into account.
     * @param img image to be evaluated
     * @param dest vector with the maximum value of all not transparent points
     * between R, G and B.
     * @return true if apply successful or false otherwise.
     */
    bool getMax(const image& img,dvector& dest) const;

    /**
     * Calculates the average value of each channel of the image.
     * If the %parameter transparent is true, the %transparent %color
     * defined by the %parameter transColor is not taken into account.
     * @param img image to be evaluated
     * @param dest vector with the average value of all not
     * transparent points between R, G and B.
     * @return true if apply successful or false otherwise.
     */
    bool getAverage(const image& img,dvector& dest) const;

   /**
     * Calculates the median of all values of each channel of the image.
     * If the %parameter transparent is true, the %transparent %color
     * defined by the %parameter transColor is not taken into account.
     * @param img image to be evaluated
     * @param dest vector with the average value of all not transparent points
     *             between R, G and B.
     * @return true if apply successful or false otherwise.
     */
    bool getMedian(const image& img,dvector& dest) const;
  };
}

#endif

