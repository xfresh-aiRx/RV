/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiQmfEnergy.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.5.2001
 * revisions ..: $Id: ltiQmfEnergy.h,v 1.11 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_QMF_ENERGY_H_
#define _LTI_QMF_ENERGY_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiQmf.h"
#include "ltiQmfKernels.h"
#include "ltiHistograming1D.h"
#include "ltiGlobalFeatureExtractor.h"
#include <vector>

namespace lti {
  /**
   * The qmfEnergy is a functor for extraction of texture features.
   * As described in Alvarado et. al. "Axon2 - A visual object recognition
   * system for non-rigid objects" SPPAM 2001.
   *
   * The channel will be separated using a QMF-Bank in several frecuency
   * channels.  After that the "energy" is calculated by applying the square
   * function to each pixel.  The resulting energy subchannels can be filtered
   * using the so called "powerFilter".  All channels are merged by upsampling
   * the "smaller" resolutions or alternativelly by downsampling the higher
   * resolutions.
   *
   * The resulting images can be convolved with a gaussian kernel.  The last
   * optional step for the qmfEnergy is to apply the square root to each pixel.
   *
   * From the resulting image an histogram can be generated.
   */
  class qmfEnergy : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class qmfEnergy
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

      /**
       * If true, the square root will be calculated for each pixel (or texel)
       * of the resulting fusion of subchannels.
       * Default: true
       */
      bool squareRootEnergy;

      /**
       * if true, the "small" channels will be upsampled to use the
       * higher resolutions in the energy image.  If false the higher
       * resolutions will be downsampled to fit the smallest one.
       * Default: false
       */
      bool upsample;

      /**
       * Size for the gaussian kernel used to smooth each partial frequency
       * channel.  You can disable the use of this filter setting its size
       * to 1.
       * Default: 3
       */
      int powerFilterSize;

      /**
       * Variance for the power filter used to smooth each partial frequency
       * channel.
       * Default: -1 (default gaussian variance)
       */
      double powerFilterVariance;

      /**
       */
      double fusionFilterVariance;

      /**
       * Size for the gaussian kernel used to smooth the result of merging
       * all feature channels. You can disable the use of this filter setting
       * its size to 1.
       *
       * Default: 3
       */
      int fusionFilterSize;

      /**
       * Parameters for the qmf channel transformed applied.
       * You will give here the number of levels you want to considered.
       * Please note that the given number of levels include the low-frecuency
       * band, which is ignored for the energy image computation.  So if
       * you give the value "n" for the levels, only "n-1" bands will be
       * considered.
       * Default value: see lti::qmf::parameters
       */
      qmf::parameters qmfParam;

      /**
       * Parameters for the histogram of energy elements
       * Default value:
       *  - histogram.cells = 32;
       *  - histogram.smooth = false;
       *  - histogram.kernel = gaussian kernel of size 3
       *  - histogram.mode = Interval
       *  - histogram.intervalLow = 0;
       *  - histogram.intervalHigh = 0.3;
       *  - histogram.ignoreValue = 0;
       *  - histogram.normalize = true;
       */
      histograming1D::parameters histogramParam;
    };

    /**
     * default constructor
     */
    qmfEnergy();

    /**
     * Constructor with parameters
     */
    qmfEnergy(const parameters& param);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    qmfEnergy(const qmfEnergy& other);

    /**
     * destructor
     */
    virtual ~qmfEnergy();

    /**
     * returns the name of this type ("qmfEnergy")
     */
    virtual const char* getTypeName() const;

    /**
     * extract histogram of energy image for each color channel of the
     * given image.
     * @param src image with the source data.
     * @param feature the feature vector generated by the concatenation of
     *        the feature vector for each separated color channel RGB.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,dvector& feature) const;

    /**
     * generate the energy image of the given channel
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The resulting energy image
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * generate the energy image of the given channel
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the resulting energy image be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     * extract the histogram of the energy image calculated from the given
     * channel.
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest feature vector generated as a histogram of the
     *             energy image
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,dvector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qmfEnergy& copy(const qmfEnergy& other);

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
     * get a channel for each band in the wavelet transform of the channel
     */
    void getChannels(const channel& chnl,
                     std::vector<channel>& chnls) const;

    /**
     * pad the source channel to have the proper dimensions
     */
    void padChannel(const int& levels,
                    const channel& chnl,
                    channel& padchnl) const;
  };
}

#endif

