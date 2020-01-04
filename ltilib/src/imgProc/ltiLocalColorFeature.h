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
 * file .......: ltiLocalColorFeature.h
 * authors ....: Axel Berner, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.1.2002
 * revisions ..: $Id: ltiLocalColorFeature.h,v 1.12 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LOCAL_COLOR_FEATURE_H_
#define _LTI_LOCAL_COLOR_FEATURE_H_

#include "ltiObject.h"

#include <list>

#include "ltiImage.h"
#include "ltiArray.h"
#include "ltiLocation.h"
#include "ltiLocalFeatureExtractor.h"
#include "ltiLinearKernels.h"
#include "ltiBilinearInterpolator.h"

namespace lti {
  /**
   * This functor calculates for each location in a given list color-features.
   *
   * Each location will be considered as a circular region, that will
   * be divided in a specified number of slices.  The color feature will
   * compute for each slice the average color.
   *
   * It is also possible to compute overlapping slices, to make the feature
   * more robust against noise.
   *
   * You can compute the locations with the functor lti::axLocationSearch or
   * with lti::pyramidLocationSearch.
   * 
   * @ingroup gColor
   * @ingroup gFeatureExt
   */
  class localColorFeature : public localFeatureExtractor {
  public:
    /**
     * The parameters for the class localColorFeature
     */
    class parameters : public localFeatureExtractor::parameters {
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
       * Number of slices.
       *
       * A given location will be interpreted as a circle, that will
       * be divided in this number of slices.  The minimal allowed
       * value of slices is one, and the maximal allowed slices value
       * is 120.
       *
       * The number of dimensions of the output feature vector will be
       * "slices" or "2*slices" depending on the "overlap" parameter.
       *
       * Default value: 6
       */
      unsigned int slices;

      /**
       * Overlapping slices. 
       *
       * If true, twice the number of slices will be computed and the feature
       * vector will have 2*slices elements.  If false, single slices will be
       * computed and the number of elements of the feature vector will be
       * just 1*slices.
       *
       * Default value: true
       */
      bool overlap;

      /**
       * Normalize.
       *
       * If true, the final feature vector will be normalized so that the
       * sum of all elements equals one.
       *
       * Default value: false
       */
      bool normalize;

      /**
       * Mask radius.
       *
       * The feature computation will be made using a template mask of a fixed
       * size that is computed only once, when you set the parameters.  The
       * values in this fixed mask will be taken from the image using bilinear
       * interpolation.
       *
       * The greater the radius, the more pixels will be considered to compute
       * the mean values of each slice, but the more the time it will take
       * to compute each location.
       *
       * This value should be greater than 2 and smaller than 255
       *
       * Default value: 7
       */
      int maskRadius;
      
    };

    /**
     * Default constructor
     */
    localColorFeature();

    /**
     * Constructor with default parameters
     */
    localColorFeature(const parameters& param);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    localColorFeature(const localColorFeature& other);

    /**
     * Destructor
     */
    virtual ~localColorFeature();

    /**
     * Returns the name of this type ("localColorFeature")
     */
    virtual const char* getTypeName() const;


    /**
     * Compute the color feature for each location.
     *
     * @param src original image to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of feature vectors.  This list will have the
     *             same size as locs and contains the average color of the
     *             location and of the slices.   The structure of an element
     *             of the vector list for slices=2 and overlapping would be:
     *             rrrrggggbbbb.
     * @return true if successful, or false otherwise.  */
    bool apply(const lti::image& src,
               const std::list<location>& locs,
               std::list<dvector>& dest);

    /**
     * Compute the color feature for the given location.
     *
     * @param src original image to be analyzed.
     * @param locs location that need to be analyzed.
     * @param dest feature vector.  This vector contains the average color of
     *             the the slices.   The structure of the vector for slices=2
     *             and overlapping set to true would be:
     *             rrrrggggbbbb.
     * @return true if successful, or false otherwise.  */
    bool apply(const lti::image& src,
               const location& locs,
                     dvector& dest);

    /**
     * Slices mask
     *
     * This method returns a read-only reference to the main mask.
     * It is intended for debug purposes only.
     */
    const kernel2D<ubyte>& getMask() const;    

    /**
     * Copy data of "other" functor.
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localColorFeature& copy(const localColorFeature& other);

    /**
     * Alias for copy member
     *
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localColorFeature& operator=(const localColorFeature& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Get a read-only reference to the current parameter set.
     *
     * @return used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update parameters
     */
    virtual bool updateParameters();

  protected:

    /**
     * Simple structure used to hold a mask and the correspoinding io-points
     * of the circle.
     */
    class sliceMask {
    public:
      /**
       * Default constructor
       */
      sliceMask();

      /**
       * Constructor to generate directly the mask
       *
       * @param radius radius of the mask
       * @param angle anglewhich will be usually zero or 2*Pi/slices/2, to
       *              shift the mask one half slice from the "normal" one.
       * @param slices number of slices
       */
      sliceMask(const int radius,
                const double angle,
                const int slices);

      /**
       * Copy constructor
       */
      sliceMask(const sliceMask& other);

      /**
       * Generate this mask for the given %parameters
       */
      void generate(const int radius,
                    const double angle,
                    const int slices);

      /**
       * Copy member
       */
      sliceMask& copy(const sliceMask& other);

      /**
       * Alias for copy member
       */
      sliceMask& operator=(const sliceMask& other);

      /**
       * Get the mean values from the image at the given location considering
       * this mask.
       *
       * The vector contains first the means for all red components of the
       * slices, followed by the green components and then by the blue ones,
       */
      bool getMeans(const image& img,
                    const location& loc,
                    vector< trgbPixel<float> >& means) const;

      /**
       * Get mask
       */
      const kernel2D<ubyte>& getMask() const;

    protected:
      /**
       * Bilinear interpolation functor
       */
      bilinearInterpolator<rgbPixel> bilin;
      
      /**
       * Mask contains the index of the slice for a pixel
       */
      kernel2D<ubyte> mask;

      /**
       * Input/output points.
       *
       * The size of this "matrix" will always be the 2 x number of rows of the
       * mask, but indexed also with negative values
       */
      array<int> ioPts;

      /**
       * Number of slices used
       */
      int slices;

      /**
       * Radius of the mask
       */
      int radius;

    };

    /**
     * Slice mask used to compute the normal feature.
     */
    sliceMask normalMask;

    /**
     * Slice mask used to compute the overlapped feature
     */
    sliceMask overlapMask;
  };
}

#endif

