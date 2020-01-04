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

 
/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiPyramidLocationSearch.h
 * authors ....: Frederik Lange, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 18.03.2003
 * revisions ..: $Id: ltiPyramidLocationSearch.h,v 1.10 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_PYRAMID_LOCATION_SEARCH_H_
#define _LTI_PYRAMID_LOCATION_SEARCH_H_

#include "ltiImage.h"
#include "ltiMath.h"
#include "ltiTransform.h"
#include "ltiLocation.h"
#include "ltiScaleSpacePyramid.h"
#include "ltiGradientFunctor.h"

#include <list>
#include <vector>
#include <map>

using std::list;
using std::vector;

namespace lti {
  /**
   * Search for salient locations in the scale space of a channel.
   *
   * The detected locations can be used to extract local features.
   *
   * For more information see:
   * David G. Lowe "Distinctive Image Features from Scale-Invariant Keypoints"
   * June, 2003
   *
   * This functor is more robust but also computationally more expensive than
   * its predecessor lti::axLocalRegions.
   *
   * The method is not 100% any of the variants published by Lowe, but it
   * got his name since the general principle is the one explain in the 
   * previously mentioned paper. 
   * 
   * One of the major differences with Lowe's approach is the use of a
   * scale-space pyramid where all levels has their own scale.  Lowe uses
   * the same size for all level within an octave.  The Gaussian smoothing is
   * done here only for the first level, and thereafter only the one required
   * to keep the downsampling valid.
   *
   * @see lti::axLocalRegions, lti::locationSelector,
   *      lti::localColorFeature, lti::axOGDFeature
   */
  class pyramidLocationSearch : public transform {
  public:
    /**
     * The parameters for the class pyramidLocationSearch
     */
    class parameters : public transform::parameters {
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
       * Read the parameters from the given ioHandler
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
       * @name Scale-space pyramid options
       */
      //@{
      /**
       * Number of levels in the pyramid
       *
       * Default value: 15 (five octaves with the default levelScaleFactor)
       */
      int numLevels;

      /**
       * Lowe suggests to upsample the first level by a factor of two using
       * bilinear interpolation. 
       *
       * Here you can given the number of upsampled levels you want to
       * have in your pyramid.  Using the default levelScaleFactor you need
       * to set here 3 in order to get a first level upsampled a scaling
       * factor of exaclty 2.
       *
       * In general, the size of the first level will be the reciprocal of
       * levelScaleFactor to the power of upsampleFirstLevel, multiplied by
       * the size of the original channel.
       *
       * If you want to use the original channel as the highest
       * resolution level or the pyramid, set this parameter to \a 0.
       *
       * This value must be 0 or positive.
       *
       * Usually, the locations returned by the apply methods contain
       * coordinates in the original image. By setting the parameter
       * locationsAtLevelZero to \a true the location coordinates
       * relate to the channel at level \a 0 in the channel pyramid
       * (see getChannelPyramid()). This can be useful when the
       * location search is part of a feature extraction. The level \a
       * 0 channel is either the original image or the upsampled image
       * depending on the value or upsampleFirstLevel.
       *
       * Default value: 1 
       */
      int upsampleFirstLevel;

      /**
       * Parameters used in the pyramid.
       *
       * Lowe suggests to use scaling factors of the form 2^(-1/n),
       * meaning that one octave will be divided by an integer number n of
       * level.  His experiments suggest that the optimal value for n is 3.
       * The default value for this factor is 2^(-1/3) = 0.793700526.
       *
       * The scaling factor (factor) is set to 
       */
      scaleSpacePyramid<channel>::parameters pyramidParameters;

      /**
       * Set to \a true to enable subpixel interpolation in the determination
       * of scale space maxima.
       *
       * Default value: true
       */
      bool interpolateMaxima;

      /**
       * Set to true if only locations have to be accepted which have a 
       * saliency bigger than its upper and lower levels.
       *
       * Default value: false
       */
      bool checkInterLevelMaxima;

      /**
       * Value used for the smoothing kernel of only the first level.
       *
       * This corresponds to the variance of the kernel, and not to the
       * standard deviation.
       *
       * If you don't want any smoothing, set this value to zero.  The size
       * of the kernel will be computed automatically to have at the first
       * and last tabs values less than 5 percent of the middle tab.
       *
       * Default value: Lowe suggestion of 1.6^2 = 2.56 (kernel size 5)
       */
      float smoothingGaussianVariance;

      /**
       * Usually, the locations returned by the apply methods contain
       * coordinates in the original image. By setting the parameter
       * locationsAtLevelZero to \a true the location coordinates
       * relate to the channel at level \a 0 in the channel pyramid
       * (see getChannelPyramid()) instead. This can be useful when the
       * location search is part of a feature extraction. The level \a
       * 0 channel is either the original image or the upsampled image
       * depending on the value or upsampleFirstLevel.
       *
       * Default \a false
       *
       * \b NOTE: Usually, you will not want this option to be
       * activated. Be sure to fully understand the consequences
       * before you do.
       */
      bool locationsAtLevelZero;
      //@}

      /**
       * @name Maxima detection
       */
      //@{
      /**
       * The size of the neighborhood to search for maxima in one level.
       *
       * The accepted values are 3,5 or 7 (meaning 3x3, 5x5 or 7x7
       * neighborhoods).
       *
       * Default value: 3
       */
      int spatialMaximumNeighborhoodSize;

      /**
       * Modes available for the peak detection
       */
      enum ePeakSelectionMode {
        GradientMagnitude, /**< Magnitude of the gradient */
        Laplacian,         /**< Laplacian, using an lti::laplacianKernel */
        Harris,            /**< Harris corner detector */
        DoG                /**< Difference of Gaussians.  See doGLevelDistance
                                for more information.*/
      };

      /**
       *  Method for peak selection:
       *  - GradientMagnitude 
       *  - Laplacian
       *  - Harris
       *  - DoG (Default and best)
       *
       * Default value: DoG
       */
      ePeakSelectionMode peakSelectionMode;

      /**
       * Scale distance for the difference of Gaussians.
       *
       * The Difference of Gaussians (DoG) peak detection mode needs to know
       * which levels it has to compare.  The default implementation compares
       * two adjacent levels, but this is not mandatory.  You can specify here
       * any integer greater or equal 1.
       *
       * The computation is done convolving each level with a precomputed
       * kernel, that considers this value and the doGVariance, so that the
       * computation times will be proportionally increasing with greater
       * values here and in doGVariance.
       *
       * The kernel \e K used will be:
       *
       * \f[ K(x,y) = G(x,y,k\sigma) - G(x,y,\sigma) \f]
       *
       * where \e G is a Gaussian function and \e k represents this
       * doGLevelDistance parameter.
       *
       * Default value: 1
       */
      int doGLevelDistance;

      /**
       * Variance for the Gaussian of the current level when using
       * the DoG peak detection mode.
       *
       * The size of the final kernel used can be indicated explicitely or
       * computed automatically depending on the value of doGKernelSize.
       *
       * Default value: 1.6*1.6
       */
      float doGKernelVariance;

      /**
       * Size of the kernel used to compute the DoG.
       *
       * If you set here a negative value, a kernel will be computed with a
       * size such that the taps of a Gaussian kernel with
       * variance doGLevelDistance*doGKernelVariance will be no greater than
       * 5\% of the the kernel tap at the position zero.
       *
       * If you don't want automatic computation of the kernel size, you
       * should set and odd value greater or equal 3.
       *
       * Default value: -1 (e.g. automatic size computation)
       */
      int doGKernelSize;

      /**
       * Minimum relative value for the allowed for a location to be salient.
       *
       * If the Laplacian pyramid (or its equivalent in other
       * peakDetectionMode) contains a value less than saliencyThreshold
       * times the maximum value at the corresponding pyramid level, then
       * that value will be ignored.
       *
       * If the number of levels in the pyramid is greater than the size
       * of these vector, to all "additional" levels, the last valid value
       * of the vector will be used.  This means, if the given vector is
       * of size one, then for all levels the same threshold will be used.
       *
       * Default value: vector<float>(1,0.1f), i.e. all levels will use 0.1f
       */
      vector<float> saliencyThreshold;

      /**
       * Edgeness Threshold.
       *
       * The edgeness is defined here as the ratio between the largest and the
       * smallest eigenvalues of the Hessian matrix.  This means, the smallest
       * possible edgeness value is one, when both eigenvalues are equal,
       * denoting a point with no definite edge direction, being a good
       * candidate for a stable salient point.  The larger the value, the
       * greater the probability that the salient value is an egde made 
       * salient by noise.
       *
       * Set the value closer to one to filter out edge point or larger to
       * tolerate edge induced locations.
       *
       * Default value 10;
       */
      float edgenessThreshold;

      /**
       * Density filter.
       *
       * There are many applications where a global saliency threshold for the
       * whole image is not appropriate.  In those cases, you usually set the
       * saliency threshold very low, so that in dark image regions some
       * locations can also be detected.
       *
       * The density filter activates a "local" saliency check.  First, all
       * locations are computed considering the saliencyThreshold only.  Then,
       * all locations "vote" with their saliency value in a window of radius
       * densityKernelRadius.  Voting means here, to assign all pixels in the
       * window the maximum value between the value they already had and the
       * saliency of the location.
       *
       * Only those locations will survive, which have a saliency value
       * greater or equal than the final "accumulated" saliency multiplied
       * with the corresponding level densityThreshold
       *
       * Default value: false
       */
      bool densityFilter;

      /**
       * Density Kernel Radius
       *
       * Radius of a circular region around a location for density check.  The
       * greater the number, the more competitive the saliency of a location
       * must be, in order to survive.
       *
       * Default value: 3
       */
      int densityKernelRadius;

      /**
       * Minimum relative value for the allowed for a location to be salient.
       *
       * A location will be considered salient only if the value at the final
       * local saliency estimation map multiplied by the corresponding
       * densityThreshold is smaller or equal the location saliency.
       *
       * If the number of levels in the pyramid is greater than the size
       * of these vector, to all "additional" levels, the last valid value
       * of the vector will be used.  This means, if the given vector is
       * of size one, then for all levels the same threshold will be used.
       *
       * Default value: vector<float>(1,0.75f), i.e. all levels will use 0.75f
       */
      fvector densityThreshold;

      //@}

      /**
       * @name Orientation detection
       */
      //@{
      /**
       * Radius of the orientation window
       *
       * The orientation will be analyzed at the nearest level corresponding
       * to the radius of a location.  The window used will have a size of
       * (1+2*orientationWindowRadius)^2.
       *
       * Default value: 3
       */
      int orientationWindowRadius;

      /**
       * Orientation sigma factor
       *
       * The values in the window are weighted with a Gaussian having as
       * standard deviation (sigma) the orientation window radius divided
       * by this factor.
       *
       * This value must always be greater than zero, or an unpredictible
       * behaviour must be expected.
       *
       * Default value: 2
       */
      float orientationSigmaFactor;

      /**
       * The resolution of the histogram for the determination of the
       * orientation of the location
       *
       * Default value: 36
       */
      int binsInOrientationHistogram;

      /**
       * Threshold used to accept multiple orientations.
       *
       * To compute the orientation of a location, a window with
       * orientationWindowRadius is used to compute an orientation histogram.
       * The greatest peak in this histogram determines the dominant
       * orientation.  However, if there are other peaks with a value equal
       * the greatest peak times this factor, then locations with their 
       * orientation will also be inserted in the location list.
       *
       * Set to 1.0 to detect only the maximum or maxima (if there are several
       * bins sharing the same maximal value), or greater than 1 if you want
       * only one maximum.  A value of zero or less will include a location
       * for all angles detected in the image (a much as
       * binsInOrientationHistogram).
       *
       * Default value: 0.8
       */
      float multiOrientationThreshold;

      /**
       * Average orientation
       *
       * If this value is set to true, all entries with values greater
       * than multiOrientationThreshold times the maximum entry will
       * be used to compute one single average value for the orientation.  Note
       * that no multi-oriented locations will be detected, but only one.
       *
       * If is set to false, the entries are used directly as orientations, as
       * Lowe suggested.
       *
       * Default value: false
       */
      bool averageOrientation;

      //@}

      /**
       * @name Location properties
       */
      //@{
      /**
       * This value indicates the scaling factor for a location radius.
       * 
       * In other words, this value will be multiplied with the corresponding
       * pixel radius of a scaling factor to obtain the location radius.
       *
       * Remember that the pixel radius at level 0 is 0.5;
       *
       * Default value: 7
       */
      float locationRelativeRadius;
      //@}
    };

    /**
     * Default constructor
     */
    pyramidLocationSearch();

    /**
     * Construct a functor using the given parameters
     */
    pyramidLocationSearch(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    pyramidLocationSearch(const pyramidLocationSearch& other);

    /**
     * Destructor
     */
    virtual ~pyramidLocationSearch();

    /**
     * Returns the name of this type ("pyramidLocationSearch")
     */
    virtual const char* getTypeName() const;
  
    /**
     * @name Apply methods
     */
    //@{

    /**
     * Extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects.
     *
     * @param src channel with the source data.
     * @param locs list of relevant locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               std::list<location>& locs);

    /**
     * Extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects.
     *
     * @param src channel with the source data.
     * @param locs list of relevant locations
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               std::list<location>& locs,
               int& totalLocs);

    /**
     * Extracts the most relevant locations from the given channel,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param src channel with the source data.
     * @param locs map of lists of relevant locations per level
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,std::map<int,std::list<location> >& locs);

    /**
     * Extracts the most relevant locations from the given channel,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param src channel with the source data.
     * @param locs map of lists of relevant locations per level
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               std::map<int,std::list<location> >& locs,
               int& totalLocs);


    /**
     * Extracts the most relevant locations from both channels, and return
     * them as list of lti::location objects.
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param locs list of relevant locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,
               const channel& c2,
               std::list<location>& locs);

    /**
     * Extracts the most relevant locations from both channels, and return
     * them as list of lti::location objects.
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param locs list of relevant locations
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,
               const channel& c2,
               std::list<location>& locs,
               int& totalLocs);

    /**
     * Extracts the most relevant locations from both channels,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param locs map of lists of relevant locations per level
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,const channel& c2,
               std::map<int,std::list<location> >& locs);

    /**
     * Extracts the most relevant locations from both channels,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param locs map of lists of relevant locations per level
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,const channel& c2,
               std::map<int,std::list<location> >& locs,
               int& totalLocs);

    /**
     * Extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects.
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param c3 third color or other feature component
     * @param locs list of relevant locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,const channel& c2,const channel& c3,
               std::list<location>& locs);

    /**
     * Extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects.
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param c3 third color or other feature component
     * @param locs list of relevant locations
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,const channel& c2,const channel& c3,
               std::list<location>& locs,
               int& totalLocs);

    /**
     * Extracts the most relevant locations from the given channel,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param c3 third color or other feature component
     * @param locs map of lists of relevant locations per level
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,const channel& c2,const channel& c3,
               std::map<int,std::list<location> >& locs);

    /**
     * Extracts the most relevant locations from the given channel,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param c1 first color or other feature component
     * @param c2 second color or other feature component
     * @param c3 third color or other feature component
     * @param locs map of lists of relevant locations per level
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,const channel& c2,const channel& c3,
               std::map<int,std::list<location> >& locs,
               int& totalLocs);

    //@}

#if 0
    /**
     * @name Apply masked methods.
     *
     * Some times, you know that some part of the images will contain
     * locations although the contrast or saliency is very low, while other
     * sections should be "suppressed", independently of their saliency values.
     *
     * This family of apply methods allow you to specify a mask (lti::channel)
     * that specify the expected maximal saliency.
     *
     * The threshold that a location's saliency value has to surpass is
     * the one used in the normal apply() methods multiplied by the value
     * in the corresponding pixel of the given mask.  Thus, the pixels masked
     * with zero will produce all possible locations; the pixels masked with
     * one will behave as in the normal applies; pixels with values greater
     * than one will suppress locations (compared with the normal case).
     *
     * The simplest mask contains a strongly smoothed intensity channel of
     * the image, normalize to have as maximum value 1.0.  Additionaly, if
     * a background segmentation is available, those values can be set to
     * std::numeric_limits<float>::max() in order to suppress all background
     * locations.
     *
     * The mask is always the last argument.  Otherwise, they could not be
     * distinguished from the normal apply methods.
     */
    //@{
    /**
     * Extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects.
     *
     * @param src channel with the source data.
     * @param locs list of relevant locations
     * @param mask saliency suppression mask
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,              
               std::list<location>& locs,
               const channel& mask);

    /**
     * Extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects.
     *
     * @param src channel with the source data.
     * @param mask saliency suppression mask
     * @param locs list of relevant locations
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,               
               std::list<location>& locs,
               int& totalLocs,
               const channel& mask);
    
    /**
     * Extracts the most relevant locations from the given channel,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param src channel with the source data.
     * @param mask saliency suppression mask
     * @param locs map of lists of relevant locations per level
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               std::map<int,std::list<location> >& locs,
               const channel& mask);

    /**
     * Extracts the most relevant locations from the given channel,
     * and return them as map where the key equals the level in which
     * the locations where found and the value is a list of
     * lti::location objects in that level.
     *
     * \b Note: This apply is faster than 
     * apply(const channel&,std::list<location>&).
     *
     * @param src channel with the source data.
     * @param locs map of lists of relevant locations per level
     * @param totalLocs total number of detected locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               std::map<int,std::list<location> >& locs,
               int& totalLocs,
               const channel& mask);

    //@}

#endif
      
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pyramidLocationSearch& copy(const pyramidLocationSearch& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pyramidLocationSearch& operator=(const pyramidLocationSearch& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const pyramidLocationSearch::parameters& getParameters() const;

    /**
     * @name Read-only access to the created pyramids in an apply method.
     *
     * This methods are used not only for debugging purposes, but also
     * to save some time with the computation of a channel pyramid, which
     * is usually employed in the computation of features.
     */
    //@{
    /**
     * Return the saliency pyramid of the last channel of the last apply
     *
     * Note that the multi-channel apply-methods use only one pyramid for
     * all channels, and therefore only the last one is kept.
     */
    const scaleSpacePyramid<channel>& getSaliencyPyramid() const;

    /**
     * Return the orientation pyramid of the last channel of the last apply
     *
     * Note that the multi-channel apply-methods use only one pyramid for
     * all channels, and therefore only the last one is kept.
     */
    const scaleSpacePyramid<channel>& getOrientationPyramid() const;

    /**
     * Return the input channel pyramid of the last channel of the last apply
     *
     * Note that the multi-channel apply-methods use only one pyramid for
     * all channels, and therefore only the last one is kept.
     */
    const scaleSpacePyramid<channel>& getChannelPyramid() const;

    //@}


  protected:

    /**
     * @name Saliency Methods
     */
    //@{
    /**
     * Applies the edge and/or corner extraction to the source souce channel
     * based on Laplace-Operator
     * 
     */
    void applyLaplacian(const channel& src, channel& dest) const ;

    /**
     * Extracts the magnitude and angle of the gradient for the given
     * for the given channel.
     *
     * @param src original channel
     * @param mag magnitude of the gradient
     * @param angle angle of the gradient.
     */
    void applyMag(const channel& src,
                  channel& mag,
                  channel& angle) const;

    /**
     * Applies the Harris cornerness extraction to the source souce channel
     * this is a measure for the curvature at a pixel
     */
    void applyCornerness(const channel& src, channel& dest) const;

    /**
     * Applies a DoG kernel to the given channel
     */
    void applyDoG(const channel& src, channel& dest) const;
    //@}
    
    /**
     * The required pyramids
     */
    //@{
    /**
     * Pyramid of original channel.
     */
    scaleSpacePyramid<channel> iPyr;

    /**
     * Pyramid of Laplacians, DoG, Harris cornerness, or gradient magnitude,
     * depending on the mode selected in the parameters.
     */
    scaleSpacePyramid<channel> lapPyr;

    /**
     * Vector containing the maxima at each level of the pyramid
     */
    fvector lapMax;

    /**
     * Pyramid containing the angles of the gradient, used to compute
     * the rotation of each feature.
     */
    scaleSpacePyramid<channel> argPyr;

    /**
     * Pyramid containing the magnitude of the gradient
     */
    scaleSpacePyramid<channel> magPyr;

    /**
     * Pyramid with maxima detected in the lapPyr.  
     *
     * This pyramid will only be initialized if the spatial maximum
     * neighborhood is greater than 3.
     */
    scaleSpacePyramid<channel> maxPyr;
    
    /**
     * First level factor
     *
     * If the pyramid was upsampled, this is the factor with which the
     * first level was created.
     */
    double firstLevelFactor;
    //@}

    /**
     * Functor used to compute gradients
     */
    gradientFunctor gradient;

    /**
     * Compute the doGKernel as wished by the user.
     */
    bool computeDoGKernel();

    /**
     * Kernel used to compute the DoG
     */
    sepKernel<float> doGKernel;

    /**
     * @name Apply helper methods
     */
    //@{
    /**
     * Compute the pyramids
     */
    bool computePyramids(const channel& src);

    /**
     * Compute the position of the locations for each level, based on
     * the pyramids
     */
    bool
    computeLocationsPosition(std::map< int, std::list<location> >& locMap);

    /**
     * Compute the orientation of each location in each level.
     *
     * The total number of locations is also computed
     */
    bool
    computeLocationOrientation(std::map< int, std::list<location> >& locMap,
                               int& totalLocs);
    //@}

    /**
     * Compute the maximum for the given three values, assuming that the
     * middle value has an offset of zero and using a quadratic function 
     * that passes through all three points.
     *
     * The returned value \a x will be between -1.0 and 1.0 if a maximum 
     * was detected, or will remain untouched if no maximum was found.  If
     * a maximum is found, the function returns true.
     */
    inline bool qmaximum(const float ym1,const float y0,const float y1,
                         float& x) const;

    /**
     * Compute the angle given an index.  It will use the neighbors to
     * compute the position of the maximum.
     */
    float getAngle(const vector<float>& angles,
                   const int idx) const;

    /**
     * Compute the edgeness ratio. 
     *
     * The value computed here corresponds to the Tr(H)^2/Det(H), with
     * the Hessian matrix H.
     *
     * This method expects x and y to be greater 1 and to be not larger than
     * 1 minus the last valid index bei each coordinate.
     */
    float edgenessRatio(const channel& chnl,
                        const int y,
                        const int x) const;
    

    /**
     * Transport all data in the second map into the first one, without
     * copying it.
     * 
     * At the end of the operation the second map will be empty.
     */
    void concatLocsMap(std::map<int,std::list<location> >& src1dest,
                       std::map<int,std::list<location> >& src2) const;

    /**
     * Density filter
     *
     * If the saliency threshold is too low, too many locations will be
     * detected.  The density filter chooses the most salient locations such
     * that in the radio of each level no more than 
     *
     */
    bool densityFilter(std::map<int,std::list<location> >& srcDest) const;
  };
}

#endif
