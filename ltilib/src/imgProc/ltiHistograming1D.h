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
 * file .......: ltiHistograming1D.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.4.2001
 * revisions ..: $Id: ltiHistograming1D.h,v 1.12 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HISTOGRAMING1_D_H_
#define _LTI_HISTOGRAMING1_D_H_


#include "ltiGlobalFeatureExtractor.h"
#include "ltiLinearKernels.h"
#include "ltiHistogram.h"
#include "ltiVector.h"

namespace lti {
  /**
   * This class extracts a 1D histogram from a channel or channel8.
   * The result can be a lti::histogram1D or a lti::vector<double>, which is
   * the standard type for feature vectors.
   * It is possible to apply a smoothing filter to the extracted histogram
   */
  class histograming1D : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class histograming1D
     */
    class parameters : public globalFeatureExtractor::parameters {
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

      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * The smoothing kernel.
       * Default value: the default gaussian kernel of size 3
       */
      kernel1D<double> kernel;

      /**
       * smooth specify if the kernel should be apply at the end of the
       * histograming.
       * Default value: false
       */
      bool smooth;

      /**
       * Number of cells in the histogram.
       * Default: 256
       */
      int cells;

      /**
       * type to specify the mode of histogram
       */
      enum eMode {
        Normal, /**< the histogram will be calculated between 0 and 1.0 for
                    channels or between 0 and 255 for channel8s.  Values
                    under 0 will be assigned to 0 and values above 1.0 (or 255)
                    will be assigned to the <code>cells-1</code> cell.
                    The <code>ignoreValue</code> allows to ignore one specific
                    value in this interval (usually used to ignore background
                */
        Extremes, /**< the histogram will be calculated between the min and
                      max values of the input data */
        Interval  /**< the histogram will be calculated between the given
                      interval values.  Note that the given values will
                      be interpreted "as is", for example if you give as
                      interval 0-1.0, the whole channel value range will
                      be histogramed, but only two values of channel8 will
                      be considered!.  Only the values in the interval will
                      be considered, i.e.  elements outside the interval will
                      be ignored. */
      };

      /**
       * the mode of histograming (see eMode).
       * If the mode is "Normal", the outliers will be considered as part of
       * the nearest cell.  (for example if using a channel, and the value of a
       * pixel is -0.1 the cell 0 will be incremented).
       * Default value: Normal
       */
      eMode mode;

      /**
       * if true, the histogram will be normalized (to have an area of 1)
       * Default: false
       */
      bool normalize;

      /**
       * if mode==Interval, this is the lowest value considered
       * (Default 0.0f)
       */
      float intervalLow;

      /**
       * if mode==Interval, this is the highest value to be considered
       * (Default 1.0f)
       */
      float intervalHigh;

      /**
       * this value will be ignored (Default -1.0f)
       */
      float ignoreValue;
    };

    /**
     * default constructor
     */
    histograming1D();

    /**
     * Construct with given parameters
     */
    histograming1D(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    histograming1D(const histograming1D& other);

    /**
     * destructor
     */
    virtual ~histograming1D();

    /**
     * returns the name of this type ("histograming1D")
     */
    virtual const char* getTypeName() const;

    /**
     * generates feature vector for the src channel, and leaves the result
     * in the dest vector
     * @param src channel with the source data.
     * @param dest dvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel& src,dvector& dest) const;

    /**
     * generates feature vector for the src channel, and leaves the result
     * in the dest vector
     * @param src channel8 with the source data.
     * @param dest vector<double> where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel8& src,dvector& dest) const;

    /**
     * generates feature vector for the src channel, and leaves the result
     * in the dest vector
     * @param src channel with the source data.
     * @param dest dvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel& src,histogram1D& dest) const;

    /**
     * generates feature vector for the src channel, and leaves the result
     * in the dest vector
     * @param src channel8 with the source data.
     * @param dest vector<double> where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel8& src,histogram1D& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    histograming1D& copy(const histograming1D& other);

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

