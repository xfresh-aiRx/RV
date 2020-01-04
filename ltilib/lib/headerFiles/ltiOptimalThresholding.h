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
 * file .......: ltioptimalThresholding.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 13.12.2000
 * revisions ..: $Id: ltiOptimalThresholding.h,v 1.14 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_OPTIMAL_THRESHOLD_H_
#define _LTI_OPTIMAL_THRESHOLD_H_

#include "ltiImage.h"

#include "ltiThresholding.h"

namespace lti {
  /**
   * Threshold segmentation of a single channel with the optimal
   * threshold value.  The optimal threshold value is calculated
   * iteratively according to Sonky, Hlavac, Boyle: "Image Processing,
   * Analysis and Machine Vision", p.129)
   */
  class optimalThresholding : public thresholding {
  public:

    /**
     * the parameters for the class optimalThresholding
     */
    class parameters : public thresholding::parameters {
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
       * precision of threshold determination.
       * iteration stops, when the changes are smaller or equal than
       * the given precision valid precision values range from 0.0
       * (inclusive) to 1.0.  when using channel8, the values are
       * linearly mapped to 0..255.  default precision = 0.003915
       * (which is < 1/255 and > 1/256)
       */
      float precision;

    };

    /**
     * default constructor
     */
    optimalThresholding();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    optimalThresholding(const optimalThresholding& other);

    /**
     * destructor
     */
    virtual ~optimalThresholding();

    /**
     * returns the name of this type ("optimalThresholding")
     */
    virtual const char* getTypeName() const;

    /**
     * segmentates a channel on place
     * lowThreshold is set to the optimal threshold value,
     * highThreshold is set to 1.0 and thresholding is applied
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise
     */
    bool apply(channel& srcdest);

    /**
     * segmentates a channel on place
     * lowThreshold is set to the optimal threshold value,
     * highThreshold is set to 1.0 and thresholding is applied
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise
     */
    bool apply(channel8& srcdest);

    /**
     * segmentate the channel src, and leave the result in dest
     * lowThreshold is set to the optimal threshold value,
     * highThreshold is set to 1.0 and thresholding is applied
     * @param src channel8 with the source data.
     * @param dest The segmentated data
     *                 will be left here.
     * @return true if apply successful or false otherwise
     */
    bool apply(const channel& src,channel& dest);

    /**
     * segmentate the channel src, and leave the result in dest.
     * lowThreshold is set to the optimal threshold value,
     * highThreshold is set to 1.0 and thresholding is applied
     * @param src channel8 with the source data.
     * @param dest channel8 where the segmentated data will be left.
     * @return true if apply successful or false otherwise
     */
    bool apply(const channel8& src,channel8& dest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    optimalThresholding& copy(const optimalThresholding& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * calculates the optimal thresholding value for a given channel
     */
    float getThreshold(const channel& src) const;

    /**
     * calculates the optimal thresholding value for a given channel
     */
    int getThreshold(const channel8& src) const;

    /**
     * calculates the optimal thresholding value for the given histogram
     * @param histo the vector containing a 1D histogram
     * @return the index in the histogram which corresponds to the optimal
     *         threshold.
     */
    int getThreshold(const vector<float>& histo) const;

    /**
     * calculates the optimal thresholding value for the given histogram
     * @param histo the vector containing a 1D histogram
     * @return the index in the histogram which corresponds to the optimal
     *         threshold.
     */
    int getThreshold(const vector<double>& histo) const;

    /**
     * calculates the optimal thresholding value for the given histogram
     * @param histo the vector containing a 1D histogram
     * @return the index in the histogram which corresponds to the optimal
     *         threshold.
     */
    int getThreshold(const vector<int>& histo) const;

  };
}

#endif

