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
 * file .......: ltiFeatureSaliencyAx.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 23.2.2001
 * revisions ..: $Id: ltiFeatureSaliencyAx.h,v 1.9 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_FEATURE_SALIENCY_AX_H_
#define _LTI_FEATURE_SALIENCY_AX_H_

#include "ltiSaliency.h"
#include "ltiPyramid.h"
#include "ltiImage.h"
#include <list>

namespace lti {
  /**
   * Feature saliency map for AXIOM.
   *
   * The feature saliency map generate a channel, where the pixel
   * values represent "saliency" factors, i.e. how conspicous is the
   * pixel respect its neighbourhood.
   *
   * This implementation has the same principles of the Itti and Koch
   * saliency map, but the implementation is different, to allow a
   * faster calculation of the conspicous regions, interesting for the
   * object recognition.
   *
   * The name Ax is related with the Project AXIOM at the Chair of
   * Technical Computer Science (LTI), for which this functor was
   * developed.
   */
  class featureSaliencyAx : public saliency {
  public:
    /**
     * the parameters for the class featureSaliencyAx
     */
    class parameters : public saliency::parameters {
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * This is the maximal number of levels to be considered in the
       * multiresolutional analysis of the image.
       *
       * The color/intensity/orientation analysis will be
       * done only in the "important" resolutions detected by the
       * Kullback-Contrast analysis, which consider the   Default value: 256
       */
      int levels;

      /**
       * to calculate which resolutions are important a histogram can
       * be used as probability distribution to evaluate the
       * similarity, or the channel itself can be somehow considered
       * as a probability distribution.  If useHistogram is true
       * (default), the histogram will be used.
       */
      bool useHistogram;

      /**
       * calculate the conspicuity map based on the intensity
       */
      bool doIntensity;

      /**
       * calculate the conspicuity map based on color
       */
      bool doColor;

      /**
       * calculate the conspicuity map based on orientation
       */
      bool doOrientation;
    };

    /**
     * default constructor
     */
    featureSaliencyAx();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    featureSaliencyAx(const featureSaliencyAx& other);

    /**
     * destructor
     */
    virtual ~featureSaliencyAx();

    /**
     * returns the name of this type ("featureSaliencyAx")
     */
    virtual const char* getTypeName() const;

    /**
     * generate the saliency map for the given image
     * @param src image with the source data.
     * @param map the resulting saliency map
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,channel& map);

    /**
     * generate the saliency map for the given image
     * @param src image with the source data.
     * @param conspIntensity the conspicuity map based on intensity
     * @param conspColor conspicuity map based on color
     * @param conspOrientation conspicuity map based on orientation
     * @param map the resulting saliency map
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,
               channel& conspIntensity,
               channel& conspColor,
               channel& conspOrientation,
               channel& map);

    /**
     * get the most important resolutions used in the last apply method
     */
    const std::list<int>& getResolutions() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    featureSaliencyAx& copy(const featureSaliencyAx& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * return a mask with the local maxima of a channel
     */
    void normalize(channel& chnl) const;


  protected:

    /**
     * generate conspicuity map for intensity contrast
     * This method uses edge detection for the saliency.
     */
    void intensityConsp(const channel& intensity,
                        channel& iconsp) const;

    /**
     * generate color conspicuity map
     */
    void colorConsp(const channel& red,
                    const channel& green,
                    const channel& blue,
                    channel& cconsp) const;

    /**
     * generate orientation conspicuity map
     */
    void orientationConsp(const channel& intensity,
                          channel& oconsp) const;

    /**
     * generate a pyramid using square filters with the resolutions i specified
     * in the resolutions-list and the next ones (i and i+1)
     */
    void squarePyramid(const channel& src,
                       pyramid<channel>& pyram) const;

    /**
     * generate a pyramid using square filters with only the resolutions in
     * the list
     */
    void squarePyramid0(const channel& src,
                       pyramid<channel>& pyram) const;
    /**
     * generate a pyramid using square filters with only the resolutions
     * following the ones in the list.
     */
    void squarePyramid1(const channel& src,
                       pyramid<channel>& pyram) const;

    /**
     * list with the "most important" resolutions for the given image.
     */
    std::list<int> resolutions;

    /**
     * resolution with the highest kullback
     */
    int resWithHighestKullback;


  };
}

#endif

