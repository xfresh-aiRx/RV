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
 * file .......: ltiKullbackContrast.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 16.3.2001
 * revisions ..: $Id: ltiKullbackContrast.h,v 1.11 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_KULLBACK_CONTRAST_H_
#define _LTI_KULLBACK_CONTRAST_H_

#include "ltiStatisticsFunctor.h"
#include "ltiImage.h"
#include "ltiGaussianPyramid.h"

namespace lti {
  /**
   * Calculate the Kullback-Constrast for a channel, as described in:
   * Jagersand, Martin.  "Saliency Maps and Attention Selection in
   * Scale and Spatial Coordinates: An Information Theoretic
   * Approach", 5th Int. Conf. on Computer Vision, 1995, pp. 195-202
   *
   * The given channel will be analized at different resolutions
   * (given through the parameters).  The output vector contains the
   * constrast for each size, where the evaluated size corresponds to
   * 2^(vector index).
   *
   * Note that if one of the distributions contains a zero (p(x_i) = 0) and
   * q(x_i) != 0, the Kullback constrast will be infinite!
   *
   * With the parameter <code>useHistogram</code> an approximation for
   * the probability distribution of the intensity of a channel will
   * be done in form of a histogram.  Please note that this will option is
   * ignored when comparing vectors.
   */
  class kullbackContrast : public statisticsFunctor {
  public:
    /**
     * the parameters for the class kullbackContrast
     */
    class parameters : public statisticsFunctor::parameters {
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
       * Number of resolutions to be evaluated (Default 7 => 128 pixel)
       */
      int resolutions;

      /**
       * if useGaussian is true, a gaussian filter will be used, with
       * the size and variance given in gaussianSize and gaussianVariance.
       * Otherwise a squareFilter will approximate the filter pyramid.
       * Default: true
       */
      bool useGaussian;

      /**
       * Size for the gaussian kernel used in the image diffusion
       * (Default: 5)
       */
      int gaussianSize;

      /**
       * Variance for the gaussian kernel used in the image diffusion.
       * (Default: 1.0)
       */
      double gaussianVariance;

      /**
       * if false, the channel contents will be directly used as a probability
       * distribution.  If true, a histogram of the channel will be generated
       * to approximate the probability distribution.  The number of bins can
       * be specified using the parameters histogramSize
       * (Default: false)
       */
      bool useHistogram;

      /**
       * indicates how many bins the histogram will have.
       * (Default: 256)
       */
      int histogramSize;
    };

    /**
     * default constructor
     */
    kullbackContrast();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    kullbackContrast(const kullbackContrast& other);

    /**
     * destructor
     */
    virtual ~kullbackContrast();

    /**
     * returns the name of this type ("kullbackContrast")
     */
    virtual const char* getTypeName() const;

    /**
     * The Kullback contrast of the channel src will be calculated as the
     * Kullback contrast between two adjacent resolutions of the gaussian
     * pyramid for that channel, where the pixel value normalized with the sum
     * of all pixels will be considered as a probability p(x,y).  (for more
     * details see the paper:
     * Jagersand, Martin.  "Saliency Maps and Attention Selection in
     * Scale and Spatial Coordinates: An Information Theoretic  Approach",
     * 5th Int. Conf. on Computer Vision, 1995, pp. 195-202
     * operates on a copy of the given %parameters.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.  The i-th entry in
     *             the vector will contain the Kullback contrast for objects
     *             with a diameter of 2^i pixels.
     * @result a reference to the <code>dest</code>.
     */
    vector<float>& apply(const channel& src,vector<float>& dest) const;

    /**
     * The Kullback contrast of the channel src will be calculated as the
     * Kullback contrast between two adjacent resolutions of the gaussian
     * pyramid for that channel, where the pixel value normalized with the sum
     * of all pixels will be considered as a probability p(x,y).  (for more
     * details see the paper:
     * Jagersand, Martin.  "Saliency Maps and Attention Selection in
     * Scale and Spatial Coordinates: An Information Theoretic  Approach",
     * 5th Int. Conf. on Computer Vision, 1995, pp. 195-202
     * operates on a copy of the given %parameters.
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.  The i-th entry in
     *             the vector will contain the Kullback contrast for objects
     *             with a diameter of 2^i pixels.
     * @param pyr the gaussian pyramid of the given channel
     * @param pyr2 the upsampled gaussian pyramid (pyr2.size() == pyr.size()-1)
     * @result a reference to the <code>dest</code>.
     */
    vector<float>& apply(const channel& src,
                               vector<float>& dest,
                               gaussianPyramid<channel>& pyr,
                               gaussianPyramid<channel>& pyr2) const;


    /**
     * calculates the Kullback contrast between q and p, defined as
     * \f[ K[Q,P] = \int_x p(x) \log \frac{p(x)}{q(x)} dx \f]
     *
     * operates on a copy of the given %parameters.  The input channels
     * do not need to be "normalized".  This will be done here, in an
     * efficient way.
     * @param q channel with the source data q(x).
     * @param p channel with the source data p(x).
     * @param dest channel where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    float& apply(const channel& q,
                 const channel& p,
                       float& dest) const;

    /*
     * calculates a channel where the local Kullback constrast is presented
     * using some local averaging.
     */
    //channel& apply(const channel& q,
    //               const channel& p,
    //                     channel& dest) const;

    /**
     * calculates the Kullback contrast between q and p, defined as
     * \f[ K[Q,P] = \int_x p(x) \log \frac{p(x)}{q(x)} dx \f]
     *
     * operates on a copy of the given %parameters.
     * @param q vector with the source data q(x).
     * @param p vector with the source data p(x).
     * @param dest vector<float> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    float& apply(const vector<float>& q,
                 const vector<float>& p,
                       float& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kullbackContrast& copy(const kullbackContrast& other);

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
     * make an histogram of the src channel in hist.  The number of bin
     * is the number of elements in hist.
     */
    void makeHistogram(const channel& src,vector<double>& hist) const;

  };
}

#endif

