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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiSchieleCrowley6DFeature.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 27.3.2002
 * revisions ..: $Id: ltiSchieleCrowley6DFeature.h,v 1.11 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SCHIELE_CROWLEY6_D_FEATURE_H_
#define _LTI_SCHIELE_CROWLEY6_D_FEATURE_H_


#include "ltiObject.h"

#include <list>

#include "ltiContour.h"
#include "ltiImage.h"
#include "ltiLocalFeatureExtractor.h"
#include "ltiGlobalFeatureExtractor.h"
#include "ltiLocation.h"

namespace lti {
  /**
   * This functor create huge lists of local features, that can be used
   * to train the multidimensional receptive field histograms of Schiele
   * and Crowley (see lti::shClassifier)
   *
   * Each vector will be usually 6 dimensional, where the even dimensions will
   * correspond to the pixel values of gaussian derivatives in
   * x-direction and the odd dimensions will correspond to the y-direction.
   *
   * Each pair of dimensions will correspond to one resolution or scale, where
   * the std. deviation of the gaussians between the levels will be related
   * by a factor 2.
   *
   * More information on the features and classification approach can be
   * taken from the original paper:
   *
   * B. Schiele and J. Crowley.  Recognition without correspondence using
   * multidimensional receptive field histograms.  Technical Report No. 453
   * MIT Medial Laboratory, Perceptial Computing Section.
   */
  class schieleCrowley6DFeature : public localFeatureExtractor,
                                  public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class schieleCrowley6DFeature
     */
    class parameters : public localFeatureExtractor::parameters,
                       public globalFeatureExtractor::parameters {
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

      /**
       * Standard deviation for the first resolution
       *
       * Default value: 2
       */
      double sigma;

      /**
       * Number of resolutions used.  Schiele and Crowley suggest 3
       *
       * Default value: 3
       */
      int numResolutions;

    };

    /**
     * default constructor
     */
    schieleCrowley6DFeature();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    schieleCrowley6DFeature(const schieleCrowley6DFeature& other);

    /**
     * destructor
     */
    virtual ~schieleCrowley6DFeature();

    /**
     * returns the name of this type ("schieleCrowley6DFeature")
     */
    virtual const char* getTypeName() const;

    /** @name slow apply methods
     *
     *  Use these methods only when you need the src channel once.  To
     *  extract feature for different regions of the same src channel, see
     *  for example apply(channel8&,std::list<dvector>&)
     */
    //@{

    /**
     * Get a vector for all non-zero pixels in the mask, using the gaussian
     * derivatives of the given channel.
     * @param src original channel with the source data
     * @param mask only the pixels in src, for which the respective pixel in
     *             the mask is different than zero will be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const channel8& mask,
               std::list<dvector>& vcts) const;


    /**
     * Get a vector for all pixels in the area points.
     * @param src original channel with the source data
     * @param mask only the pixels in the mask (areaPoints list) will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const areaPoints& mask,
               std::list<dvector>& vcts) const;

    /**
     * Get a vector for each location in the locations list.
     * @param src original channel with the source data
     * @param mask only the central pixels in the locations will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const std::list<location>& mask,
               std::list<dvector>& vcts) const;

    /**
     * Get a vector for each non-zero pixel in the mask.
     * @param src original channel with the source data
     * @param angle reorient the gaussian derivatives with this angle
     * @param mask only the pixels in src, for which the respective pixel in
     *             the mask is different than zero will be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const double angle,
               const channel8& mask,
               std::list<dvector>& vcts) const;

    /**
     * Get a vector for each pixel in the areaPoint.
     * @param src original channel with the source data
     * @param angle reorient the gaussian derivatives with this angle
     * @param mask only the pixels in the mask (areaPoints list) will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const double angle,
               const areaPoints& mask,
               std::list<dvector>& vcts) const;

    /**
     * Get a vector for each location in the locations list.
     * @param src original channel with the source data
     * @param angle reorient the gaussian derivatives with this angle
     * @param mask only the central pixels of the locations will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const double angle,
               const std::list<location>& mask,
               std::list<dvector>& vcts) const;

    //@}

    /**
     * @name fast apply methods.
     *
     * All these apply methods use the internal computed gaussian derivatives.
     * Before calling these methods you need therefore to specify your channel
     * with the method use().
     *
     * This approach is very fast when extracting different feature sets for
     * the same image (for example, feature sets for each object in the image)
     *
     * In this case you specify once with use() which channel you want to
     * use, and after that, you can call the apply method for each
     * object found in the image.  This way, the expensive filters will be
     * applied just one, and the feature extraction is faster.
     */
    //@{

    /**
     * indicates the channel to be used.  This will allow a pre-computation
     * of the gaussian derivatives, that allow a faster analysis of image
     * regions.
     */
    bool use(const channel& chnl);

    /**
     * Before calling this method, you need to specify your channel with
     * use(const channel&)
     * @param mask only the pixels in src, for which the respective pixel in
     *             the mask is different than zero will be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& mask,
               std::list<dvector>& vcts) const;

    /**
     * Before calling this method, you need to specify your channel with
     * use(const channel&)
     * @param mask only the pixels in the mask (areaPoints list) will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const areaPoints& mask,
               std::list<dvector>& vcts) const;

    /**
     * Before calling this method, you need to specify your channel with
     * use(const channel&)
     * @param mask only the central pixels in the locations will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const std::list<location>& mask,
               std::list<dvector>& vcts) const;

    /**
     * Before calling this method, you need to specify your channel with
     * use(const channel&)
     * @param angle reorient the gaussian derivatives with this angle
     * @param mask only the pixels in src, for which the respective pixel in
     *             the mask is different than zero will be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const double angle,
               const channel8& mask,
               std::list<dvector>& vcts) const;

    /**
     * Before calling this method, you need to specify your channel with
     * use(const channel&)
     * @param angle reorient the gaussian derivatives with this angle
     * @param mask only the pixels in the mask (areaPoints list) will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const double angle,
               const areaPoints& mask,
               std::list<dvector>& vcts) const;

    /**
     * Before calling this method, you need to specify your channel with
     * use(const channel&)
     * @param angle reorient the gaussian derivatives with this angle
     * @param mask only the central pixels of the locations will
     *             be considered.
     * @param vcts the list of all feature vectors for all masked pixels
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const double angle,
               const std::list<location>& mask,
               std::list<dvector>& vcts) const;

    //@}

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    schieleCrowley6DFeature& copy(const schieleCrowley6DFeature& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    schieleCrowley6DFeature& operator=(const schieleCrowley6DFeature& other);

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
     * get the basis channels for the channel chnl, assuming a
     * gaussian with std. deviation sigma.
     */
    void getBasisChannels(const channel& chnl,
                          const double sigma,
                          channel& Dx,
                          channel& Dy) const;

    /**
     * the set of channels internally used.
     */
    channel* chnls;

  };
}

#endif

