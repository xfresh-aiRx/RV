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
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiAxOGDFeature.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.5.2001
 * revisions ..: $Id: ltiAxOGDFeature.h,v 1.16 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_AX_O_G_D_FEATURE_H_
#define _LTI_AX_O_G_D_FEATURE_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiOgdKernels.h"
#include "ltiOgdFilter.h"
#include "ltiLocation.h"
#include <list>
#include <map>
#include <vector>

#include "ltiLocalFeatureExtractor.h"
#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * OGD based texture feature.
   *
   * Functor for texture feature extraction that uses the steerability
   * property of the oriented gaussian derivatives (OGD) to generate rotation
   * invariant feature vectors.
   *
   * The description of this feature can be found on Alvarado, P.,
   * Doerfler, P. and Wickel, J. "Axon2 - A visual object recognition
   * system for non-rigid objects" SPPRA July, 2001
   * (http://www.techinfo.rwth-aachen.de/Veroeffentlichungen/V003_2001.pdf)
   *
   * The basic idea is to compute the "energy" of a region (a location for
   * local features or the whole image for global features) as a steerable
   * function.
   *
   * This energy is computed for different "power" channels, which are the
   * result of convolving the images with OGD filters of a specific order.
   *
   * In some sense, the first order OGD compute some "edge" energy, while
   * second order OGD compute some "line" energy.
   */
  class axOGDFeature : public localFeatureExtractor,
                       public globalFeatureExtractor {
  public:
    /**
     * The parameters for the class axOGDFeature
     */
    class parameters : public localFeatureExtractor::parameters,
                       public globalFeatureExtractor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
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
       * Use Gaussian kernel.
       *
       * If gaussian is true, for the local features a gaussian kernel
       * is used, with the given size (windowSize) and variance.  If
       * false, a square kernel is used, with the given windowSize.
       * Default: true
       */
      bool gaussian;

      /**
       * Window size for the extraction of local features. 
       *
       * This is the size of the gaussian kernel is "gaussian" is true, or the
       * size of the square window, if a faster square kernel is chosen.
       * Default : 13 (13x13 window)
       */
      int windowSize;

      /**
       * If "gaussian" is true, this value gives the variance to be used.
       * Default: -1 (i.e. use default variance for the given windowSize
       */
      double windowVariance;

      /**
       * Variance of the base gaussian used in the OGDs
       * Default: 2.0
       */
      double ogdVariance;

      /**
       * Specify if a second set of levels should be generated, using a
       * standard deviation equal to the one of the original set multiplied
       * with a factor sqrt(2).  This allows a better resolution of the
       * "resolution"-axis.
       * Default: true
       */
      bool voices;

      /**
       * Size of the ODG filter kernels used.
       * Default: 13
       */
      int ogdSize;

      /**
       * Order of the derivative being used.
       * The valid range by now is 1 or 2.
       * Default: 2
       */
      int ogdOrder;

      /**
       * Number of levels used.
       * Default 4
       */
      int levels;

      /**
       * If true, the feature vector will contain also phase information
       * which is NOT rotation invariant.
       * Default: false
       */
      bool considerPhase;

      /**
       * If true, the angles in the thetaX channels will be fixed to
       * be perpendicular to the edges.  If false, these angles will be keeped
       * to represent the phase of the energy components as described in the
       * original paper.
       *
       * Note that this parameter only changes the content of the
       * channels thetaX returned by apply(const channel&,channel&,channel&)
       * and apply(const channel&,channel&,channel&,channel&,channel&) methods.
       *
       * Default value: true
       */
      bool computeEdgeOrientation;
    };

    /**
     * Default constructor
     */
    axOGDFeature();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    axOGDFeature(const axOGDFeature& other);

    /**
     * Destructor
     */
    virtual ~axOGDFeature();

    /**
     * Returns the name of this type ("axOGDFeature")
     */
    virtual const char* getTypeName() const;

    /**
     * Indicates which channel to use in the local feature extraction.
     * Some precalculation will be done, and using the
     * apply(const point&, dvector&) member, the feature vector of a specific
     * position can be computed in a faster way.
     *
     * After calling this method the corresponding monochromatic features will
     * be extracted.
     */
    virtual bool use(const channel& src);

    /**
     * Indicates which channels to use in the local feature extraction.
     * Some precalculation will be done, and using the
     * apply(const point&, dvector&) member, the feature vector of a specific
     * position can be computed in a faster way.
     *
     * After calling this method the corresponding opponent-color features will
     * be extracted.
     */
    virtual bool use(const channel& center,const channel& surround);

    /**
     * Extract the local feature at the position pos and leave the result
     * in the given vector.  It depends on the last used "use" member, if
     * the feature vector is an "opponent-color" feature or a
     * monochromatic feature.
     *
     * @param pos the position at the given image.
     * @param dest the feature vector.
     * @return true if sucessful, false otherwise
     */
    virtual bool apply(const point& pos,dvector& dest) const;

    /**
     * Extracts the OGD rotation invariant feature from the given gray
     * valued image (a channel)
     * @param src channel with the source data.
     * @param dest the feature vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,dvector& dest) const;

    /**
     * Extract the OGD features from the red, green and blue components of
     * the given image and concatenate the results.
     * @param src image with the source data.
     * @param dest the feature vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,dvector& dest) const;

    /**
     * Extracts the first order OGD coefficients for the given window
     * size and variance (if gaussian).  The "levels" parameter will be
     * ignored, and only the first level will be calculated.  This method
     * is used by other feature extractors.
     *
     * To obtain the angle othogonal to the edges, you need to divide
     * <code>theta1</code> by 2.  This can automatically be done if you
     * specify it in the parameters.  You can use a1 and theta1/2 for the
     * lti::orientationFeature.
     */
    bool apply(const channel& src,
               channel& a0,channel& a1,channel& theta1) const;

    /**
     * Extracts the second order OGD coefficients for the given windows
     * size and variance (if gaussian).  The "levels" parameter will be
     * ignored, and only the first level will be calculated.  This method
     * is used by other feature extractors
     *
     * To obtain the angle othogonal to the edges, you need to divide
     * <code>theta1</code> by 2.  You can use a1 and theta1/2 for the
     * lti::orientationFeature.
     */
    bool apply(const channel& src,
               channel& a0,channel& a1,channel& a2,
               channel& theta1,channel& theta2) const;

    /**
     * Extracts the OGD rotation invariant opponent color feature from the
     * given color channels.  Center and Surround must have the same sizes.
     * @param center the center channel.
     * @param sround the surround channel.
     * @param dest the feature vector
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& center,
               const channel& sround,
               dvector& dest) const;


    /**
     * Extracts the OGD features for each given location.  The radius
     * of each location is used to determine which scale should be used
     * in the generation of each feature vector.
     * @param src original channel to be analyzed.
     * @param locs the locations that need to be analyzed.
     * @param dest the list of feature vectors.  This list will have the same
     *             size as locs.  The feature vector at position p will
     *             correspond to the location at position p.
     * @return true if successful, of false otherwise.
     */
    bool apply(const channel& src,
               const std::list<location>& locs,
               std::list<dvector>& dest);

    /**
     * Extracts the OGD features for each given location.  The image will
     * be splitted in its red, green and blue components, and the feature
     * vectors will be concatenated.
     * The radius of each location is used to determine which scale
     * should be used in the generation of each feature vector.
     * @param src original channel to be analyzed.
     * @param locs the locations that need to be analyzed.
     * @param dest the list of feature vectors.  This list will have the same
     *             size as locs.  The feature vector at position p will
     *             correspond to the location at position p.
     * @return true if successful, of false otherwise.
     */
    bool apply(const image& src,
               const std::list<location>& locs,
               std::list<dvector>& dest);


    /**
     * Extracts the opponent color OGD features for each given
     * location.  The radius of each location is used to determine
     * which scale should be used in the generation of each feature
     * vector.
     *
     * @param center original channel considered as "center"
     * @param surround channel considered as surround.  This must have
     *                 the same size as center.
     * @param locs the locations that need to be analyzed.
     * @param dest the list of feature vectors.  This list will have the same
     *             size as locs.  The feature vector at position p will
     *             correspond to the location at position p.
     * @return true if successful, of false otherwise.  */
    bool apply(const channel& center,
               const channel& surround,
               const std::list<location>& locs,
               std::list<dvector>& dest);

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    axOGDFeature& copy(const axOGDFeature& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * Extracts the first order OGD coefficients for the given windows
     * size and variance (if gaussian).  The "levels" parameter will be
     * ignored, and only the first level will be calculated.  This method
     * is used by other feature extractors.
     *
     * To obtain the angle othogonal to the edges, you need to divide
     * <code>theta1</code> by 2.  You can use a1 and theta1/2 for the
     * lti::orientationFeature.
     */
    bool apply(const channel& src,
               const bool& voices,
               const int& level,
               channel& a0,channel& a1,channel& theta1) const;

    /**
     * Extracts the second order OGD coefficients for the given windows
     * size and variance (if gaussian).  The "levels" parameter will be
     * ignored, and only the first level will be calculated.  This method
     * is used by other feature extractors
     *
     * To obtain the angle othogonal to the edges, you need to divide
     * <code>theta1</code> by 2.  You can use a1 and theta1/2 for the
     * lti::orientationFeature.
     */
    bool apply(const channel& src,
               const bool& voices,
               const int& level,
               channel& a0,channel& a1,channel& a2,
               channel& theta1,channel& theta2) const;


    /**
     * Generate the basis power channels.
     * If voice is true, the power correspond to a "voice" channel, and
     * the filter parameters must be adjusted.
     */
    void generatePowerBasisOgd1(const channel& chnl,
                                const bool voice,
                                channel& p11,
                                channel& p12,
                                channel& p22) const;

    /**
     * Generate the basis power channels for the opponent color feature,
     * where the first given channel corresponds to the center and the second
     * to the surround.
     */
    void generatePowerBasisOgd1(const channel& center,
                                const channel& sround,
                                const bool voice,
                                channel& pc1,
                                channel& pc2,
                                channel& ps1,
                                channel& ps2) const;

    /**
     * Generate the basis power channels of a gray-valued image using
     * second order OGD.
     */
    void generatePowerBasisOgd2(const channel& chnl,
                                const bool voice,
                                channel& p11,
                                channel& p12,
                                channel& p13,
                                channel& p22,
                                channel& p23,
                                channel& p33) const;

    /**
     * Calculate the global descriptors using the first order ogd basis
     * power channels
     */
    void globalOgd1(const channel& p11,
                    const channel& p12,
                    const channel& p22,
                    double& a0,
                    double& a1,
                    double& theta1) const;

    /**
     * Calculate the local descriptors using the first order ogd basis
     * power channels
     */
    void localOgd1(const point& pos,
                   const imatrix& ioPts,
                   const channel& p11,
                   const channel& p12,
                   const channel& p22,
                   double& a0,
                   double& a1,
                   double& theta1) const;

    /**
     * Calculate the global descriptors using the first order ogd basis
     * power channels for the opponent color feature
     */
    void globalOgd1(const channel& center,
                    const channel& sround,
                    const bool voice,
                    double& ac0,
                    double& ac1,
                    double& thetac1,
                    double& as0,
                    double& as1,
                    double& thetas1,
                    double& ao0,
                    double& ao1,
                    double& thetao1) const;


    /**
     * Calculate the global descriptors using the first order ogd basis
     * power channels for the opponent color feature
     */
    void localOgd1(const point& pos,
                   const imatrix& ioPts,
                   const channel& pc1,
                   const channel& pc2,
                   const channel& ps1,
                   const channel& ps2,
                   double& ac0,
                   double& ac1,
                   double& thetac1,
                   double& as0,
                   double& as1,
                   double& thetas1,
                   double& ao0,
                   double& ao1,
                   double& thetao1) const;

    /**
     * Calculate the global descriptors using the second order ogd basis
     * power channels
     */
    void globalOgd2(const channel& p11,
                    const channel& p12,
                    const channel& p13,
                    const channel& p22,
                    const channel& p23,
                    const channel& p33,
                    double& a0,
                    double& a1,
                    double& a2,
                    double& theta1,
                    double& theta2) const;

    /**
     * Calculate the global descriptors using the second order ogd basis
     * power channels
     */
    void localOgd2(const point& pos,
                   const imatrix& ioPts,
                   const channel& p11,
                   const channel& p12,
                   const channel& p13,
                   const channel& p22,
                   const channel& p23,
                   const channel& p33,
                   double& a0,
                   double& a1,
                   double& a2,
                   double& theta1,
                   double& theta2) const;

    /**
     * Check the location list for used radii, and initialize
     * the region io-points required.
     *
     */
    int analyzeLocations(const std::list<location>& locs,
                                std::map<float,int>& radToIdx,
                                std::map<float,int>& radToScale,
                                std::vector<imatrix>& ioPts);

    /**
     * Get the io points for the radial location of the given radius.
     * This is used by the apply methods which analyse only given
     * locations, to take only the necessary data into consideration.
     */
    void getRadialIoPoints(const double& radius,
                                 imatrix& ioPts);


  protected:
    /**
     * This channels contain the term a0-a2 and theta2
     */
    std::vector<channel> theChannels;

    /**
     * The degree of the OGD used to extract the channels
     */
    int ogdOrder;

    /**
     * The number of levels included in theChannels
     */
    int numLevels;

  };
}

#endif

