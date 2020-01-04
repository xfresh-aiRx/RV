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
 * file .......: ltiFeatureSaliencyIK.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2000
 * revisions ..: $Id: ltiFeatureSaliencyIK.h,v 1.15 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_FEATURE_SALIENCY_IK_H_
#define _LTI_FEATURE_SALIENCY_IK_H_

#include "ltiObject.h"
#include "ltiImage.h"
#include "ltiSaliency.h"
#include "ltiGaussianPyramid.h"

namespace lti {
  /**
   * Feature Saliency for color images.
   *
   * This class allows the generation of a Saliency Map from a color
   * image based on the description in L. Itti et.at. "A Model of Saliency-
   * Based Visual Attention for Rapid Scene Analysis", IEEE Trans. on
   * Pattern Analysis and Machine Intelligence, Vol. 20, No. 11,
   * November 1998
   */
  class featureSaliencyIK : public saliency {
  public:
    /**
     * the parameters for the class featureSaliencyIK
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

      // --------------------------------------------
      // the parameters
      // --------------------------------------------

      /**
       * Number of scales to be used in the multiresolutional analysis.
       * (Default value: 8)
       */
      int scales;

      /**
       * the saliency map will be generated with the given scale resolution.
       * The value must be between 0 and scales-1 (default 0)
       */
      int mapScale;

      /**
       * factor of the maximum intensity, which determine the threshold under
       * which all color information will be neglected (default 0.05 for 5%)
       */
      double colorThreshold;

      /**
       * mininum scale index to be checked in the conspicuity maps (default 1)
       */
      int minScale;

      /**
       * maximum scale index to be checked in the conspicuity maps (default 3)
       */
      int maxScale;

      /**
       * minimum delta for the inter-scale comparision (default 3)
       */
      int minDeltaScale;

      /**
       * maximum delta for the inter-scale comparition (default 4)
       * Please note that maxDeltaScale + maxScale must be less than scales
       */
      int maxDeltaScale;

      /**
       * gabor kernel size (default 9 (means 9x9 kernel))
       */
      int gaborKernelSize;

      /**
       * number of iterations to select the saliency regions (default 3).
       * (this allows an approximation of the suggestion in Itti and Koch
       *  "A saliency-based search mechanism for overt and covert shifts of
       *   visual attention" Elsevier 2000 (www.elsevier.com/locate/visres)
       */
      int smoothingIterations;
    };

    /**
     * default constructor
     */
    featureSaliencyIK();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    featureSaliencyIK(const featureSaliencyIK& other);

    /**
     * destructor
     */
    virtual ~featureSaliencyIK();

    /**
     * returns the name of this type ("featureSaliencyIK")
     */
    virtual const char* getTypeName() const;

    /**
     * extract the saliency map of the image and leave it on dest
     * @param src image with the source data.
     * @param dest saliency map will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,channel& dest) const;

    /**
     * extract the conspicuity maps of intensity, color and orientation from
     * the given color image
     * @param src image with the source data.
     * @param conspIntensity conspicuity map for intensity
     * @param conspColor conspicuity map for color
     * @param conspOrientation conspicuity map for orientation
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,
                     channel& conspIntensity,
                     channel& conspColor,
                     channel& conspOrientation) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    featureSaliencyIK& copy(const featureSaliencyIK& other);

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
     * calculate the after Itti & Koch required color channels:
     *
     * With
     * r', g' and b' the original color channels
     * I = (r'+g'+b')/3
     * r = r'/I
     * g = g'/I
     * b = b'/I
     *
     * the red, green, blue and yellow broadly tuned color channels
     * are defined as:
     *
     * R = r-(g+b)/2;         // negative values are set to zero!
     * G = g-(r+b)/2;         // negative values are set to zero!
     * B = b-(r+g)/2;         // negative values are set to zero!
     * Y = (r+g)/2-|r-g|/2-b; // negative values are set to zero!
     *
     */
    void getColorChannels(const image& img,
                          channel& RG,
                          channel& BY,
                          channel& I) const;

    /**
     * returns the number of pyramids required to represent all reconstructed
     * scales needed by the conspicuity maps
     */
    int getNumberPyramids() const;

    /**
     * return the index of the pyramid that contains the scaling of s to c
     */
    int getIndex(const int& c,const int& s) const;

    /**
     * return the used scales of the pyramid with index idx
     */
    void getLimits(const int& idx, int& from, int& to) const;

    /**
     * generate the upsampling pyramids required for the given pyramid
     */
    void getUpscaledPyramids(const gaussianPyramid<channel>& src,
                           std::vector<gaussianPyramid<channel> >& usp) const;


    /**
     * modified subtration (-) of two inter-resolutional channels...
     * This does an element-wise subtraction and absolute value:
     * result = | a - b |
     * The member assumes "connected" channels (not lined!)
     */
    void subtAndNorm(const channel& a,const channel& b,channel& result) const;

    /**
     * modified addition (+) of two inter-resolutional channels...
     * This does an element-wise subtraction and absolute value:
     * result = | a + b |
     * The member assumes "connected" channels (not lined!)
     */
    void addAndNorm(const channel& a,const channel& b,channel& result) const;

    /**
     * accumulate the normed channel c in the accumulator-channel acc
     */
    void normAndAcc(const channel& c, channel& acc) const;

    /**
     * normalize
     */
    void normalize(channel& chnl) const;


    /**
     * contrast channel is define as I(c,s)=|I(c) (-) I(s)|
     */
    void conspicuityI(const gaussianPyramid<channel>& pyr,channel& mapI) const;

    /**
     * opponent colors conspicuity C(c,s)=N(RG(c,s))+N(BY(c,s))
     * the given pyramid must contain the subtraction of two color channels,
     * i.e. R-G or B-Y
     */
    void conspicuityC(const gaussianPyramid<channel>& pRG,
                      const gaussianPyramid<channel>& pBY,
                      channel& mapI) const;


    /**
     * orientation conspicuity.  The gaussian pyramids must contain
     * gabor pyramids for the angles 0, 45, 90 and 135.  (Note that gabor
     * pyramids are a sort gaussian pyramids for the upsampling reconstruction)
     */
    void conspicuityO(const gaussianPyramid<channel>& p00,
                      const gaussianPyramid<channel>& p45,
                      const gaussianPyramid<channel>& p90,
                      const gaussianPyramid<channel>& p135,
                      channel& mapI) const;


    /**
     * add the scales in the given pyramid at the given scale, and leave
     * the result in the given channel.  The size of the channel must be
     * properly set before calling this function!
     */
    void add(const gaussianPyramid<channel>& pyr,
             const int& pyrIndex,
             const int& scale,
             channel& result) const;

    /**
     * iterative normalization
     */
    void itNorm(const int& steps,channel& chnl) const;

  };

}

#endif

