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
 * file .......: ltiHistogramEqualization.h
 * authors ....: Thomas Erger
 * organization: LTI, RWTH Aachen
 * creation ...: 25.7.2000
 * revisions ..: $Id: ltiHistogramEqualization.h,v 1.15 2005/06/29 13:22:18 doerfler Exp $
 */

#ifndef _LTI_HISTOGRAM_EQUALIZATION_H_
#define _LTI_HISTOGRAM_EQUALIZATION_H_

#include "ltiObject.h"
#include "ltiModifier.h"

namespace lti {
  /**
   *  HistogramEqualization equalizes the values of the given channel,
   *  channel8, vector<ubyte> or fvector within the given input range and
   *  maps these equalized values to the given output range.
   *
   *  The algorithm used can be found in Sonka et. al "Image Processing,
   *  Analysis and Machine Vision" 2nd Edition, PWS Publishing, pp. 60-61
   *
   *  Example using HistogramEqualization
   *  \code
   *  // The channel to be equalized and the result channel
   *  lti::channel src,dest;
   *
   *  // ... initialize channel src here ...
   *
   *  // set parameters and construct the functor
   *  lti::histogramEqualization::parameters param;  // parameters
   *  param.cells = 256;           // cells
   *  param.lowerInputLimit=0.0;   // equalizes the dark values
   *  param.upperInputLimit=0.2;   //
   *  param.lowerOutputLimit=0.6;  // renders the equalized values lighter
   *  param.upperOutputLimit=1.0;  //
   *  lti::histogramEqualization equalizer(param);          // functor
   *
   *  // equalizes the values within input range, maps them to the output range
   *  // and returns the partly equalized channel in dest.
   *  equalizer.apply(src,dest);
   *  \endcode
   */
  class histogramEqualization : public modifier {
  public:
    /**
     * the parameters for the class histogramEqualization
     */
    class parameters : public modifier::parameters {
    public:
      /**
       * default constructor
       *
       * @param upperLimit upper value used for the input and output upper
       *                   limits.  It is 1.0f as default, but if you plan
       *                   to equalize channel8 objects, you should use 255
       */
      parameters(const float upperLimit=1.0f);

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
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * the least value equalized.  Use values in the same input
       * range as the input parameter, i.e. values between 0.0 and 1.0
       * for channel equalization or values between 0 and 255 for
       * channel8 equalization. Default 0.0
       */
      float lowerInputLimit;

      /**
       * the least value the equalized values are mapped to.
       *
       * Use values in the same input range as the input parameter,
       * i.e. values between 0.0 and 1.0 for channel equalization or
       * values between 0 and 255 for channel8 equalization.
       * Default 0.0
       */
      float lowerOutputLimit;

      /**
       * the number of cells, the histogram uses for equalization
       *
       * Default 256
       */
      int cells;

      /**
       * the highest value equalized
       *
       * Use values in the same input range as the input parameter,
       * i.e. values between 0.0 and 1.0 for channel equalization or
       * values between 0 and 255 for channel8 equalization
       * Default 1.0
       */
      float upperInputLimit;

      /**
       * the highest value the equalized values are mapped to
       *
       * Use values in the same input range as the input parameter,
       * i.e. values between 0.0 and 1.0 for channel equalization or
       * values between 0 and 255 for channel8 equalization
       * Default 1.0
       */
      float upperOutputLimit;
    };

    /**
     * default constructor
     *
     * @param upperLimit upper value used for the input and output upper
     *                   limits.  It is 1.0f as default, but if you plan
     *                   to equalize channel8 objects, you should use 255
     */
    histogramEqualization(const float upperLimit=1.0f);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    histogramEqualization(const histogramEqualization& other);

    /**
     * destructor
     */
    virtual ~histogramEqualization();

    /**
     * returns the name of this type ("histogramEqualization")
     * @return true if ok, false otherwise.
     */
    virtual const char* getTypeName() const;

    /**
     * equalizes the values of the given channel
     * @return true if ok, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * equalizes the values of the given channel8
     * @return true if ok, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * equalizes the values of the given fvector
     * @return true if ok, false otherwise.
     */
    bool apply(fvector& srcdest) const;

    /**
     * equalizes the values of the given vector<ubyte>
     * @return true if ok, false otherwise.
     */
    bool apply(vector<ubyte>& srcdest) const;

    /**
     * returns the dest channel with equalized values of the src
     * channel
     * @param src the input data
     * @param dest the channel where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     * returns the dest channel8 with equalized values of the src
     * channel8
     * @param src the input data
     * @param dest the channel8 where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * returns the dest fvector with equalized values of the src fvector
     * @param src the input data
     * @param dest the fvector where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const fvector& src,fvector& dest) const;

    /**
     * equalize the src vector and write result in dest
     * @param src the input data
     * @param dest the vector where the result will be left
     * @return true if ok, false otherwise.
     */
    bool apply(const vector<ubyte>& src,vector<ubyte>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    histogramEqualization& copy(const histogramEqualization& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };

}

#endif

