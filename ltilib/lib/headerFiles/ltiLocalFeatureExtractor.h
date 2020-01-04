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
 * file .......: ltiLocalFeatureExtractor.h
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 5.4.2004
 * revisions ..: $Id: ltiLocalFeatureExtractor.h,v 1.5 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LOCAL_FEATURE_EXTRACTOR_H_
#define _LTI_LOCAL_FEATURE_EXTRACTOR_H_

//TODO: include only those files which are needed in this header!!

#include <list>

#include "ltiImage.h"
#include "ltiLocation.h"
#include "ltiFeatureExtractor.h"
#include "ltiArray.h"
#include "ltiBilinearInterpolator.h"

namespace lti {
  /**
   * The local feature extractors
   * are functors which extract information from the given images or channels
   * at the given locations. This class is parent class for all implemented
   * localfeature extractors.
   */
  class localFeatureExtractor : virtual public featureExtractor {
  public:
    /**
     * The parameters for the class localFeatureExtractor
     */
    class parameters : virtual public featureExtractor::parameters {
    public:

      /**
       * enumeration of sampling methods
       */
      enum eSamplingMethod {
        FixedRaySampling, /**< Sampling method which uses circles and rays. 
                           *  The pixel values at intersections are returned.
                           */
        FixedGridSampling /**< Sampling method which uses a fixed circular
                           * grid.
                           */
      };

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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      

    };

    /**
     * Default constructor
     */
    localFeatureExtractor();

    /**
     * Construct a functor using the given parameters
     */
    localFeatureExtractor(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    localFeatureExtractor(const localFeatureExtractor& other);

    /**
     * Destructor
     */
    virtual ~localFeatureExtractor();

    /**
     * Returns the name of this type ("localFeatureExtractor")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!
    
    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localFeatureExtractor& copy(const localFeatureExtractor& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localFeatureExtractor& operator=(const localFeatureExtractor& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


    // Applies :
    /**
     * Compute the local feature for each location.
     * The standard implementation iterates through the lists and
     * invokes apply(channel&,const location,dvector)
     * for each location.
     *
     * @param src original channel to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of feature vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::channel& src,
                       const std::list<location>& locs,
                       std::list<dvector>& dest) const;

    /**
     * Compute the local feature for each location.
     * The stanard implementation casts the channel8 to a channel
     * and invokes the apply for channel.
     *
     * @param src original channel8 to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of feature vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::channel8& src,
                       const std::list<location>& locs,
                       std::list<dvector>& dest) const;
    /**
     * Compute the local feature for each location.
     * Not implemented.
     *
     * @param src list of channels be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of feature vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const std::list<channel>& src,
                       const std::list<location>& locs,
                       std::list<dvector>& dest) const;

    /**
     * Compute the local feature for each location.
     * The standard implementation iterates through the lists and
     * invokes apply(image&,const location,dvector)
     * for each location.
     *
     * @param src original image to be analyzed.
     * @param locs locations that need to be analyzed.
     * @param dest list of feature vectors.  
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::image& src,
                       const std::list<location>& locs,
                       std::list<dvector>& dest) const;
  
     /**
     * Compute the local feature for the given location.
     * The standard implementation splits the image in three
     * channels red,green,blue and concatenates the resulting
     * vectors of each apply(channel&,const location, devector)
     * in this order.
     *
     * @param src original image to be analyzed.
     * @param locs location that need to be analyzed.
     * @param dest feature vector. 
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::image& src,
                       const location& locs,
                       dvector& dest) const;

     /**
     * Compute the local feature for the given location.
     * Not implemented.
     *
     * @param src original channel to be analyzed.
     * @param locs location that need to be analyzed.
     * @param dest feature vector. 
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::channel& src,
                       const location& locs,
                       dvector& dest) const;
   
 
  protected:

    /**
     * This helper function extracts a sequence of circles around the given
     * location. 
     * @param chan the input channel
     * @param loc the location where extraction should start
     * @param dest the output vector
     * @param numRays how many points are extracted from each circle
     * @param numCircles how many circles are used
     * @param scale the radius will be scaled with this factor (Default: 1)
     * @param useOrientation whether the angle of the location is used 
     * (Default: true)
     * @param includeCenter wheter the location itself should be in the
     * results (Default: 1)
     * @param useCircleOrder wheter the order of the points is along the circles
     * or along the rays. The data in the output vector is from 
     * the center to the outer and each circle is sampled clockwise. 
     * (Default: true)
     */
    bool fixedRaySampling(const channel& chan, const location& loc, 
                          fvector& dest, int numRays,
                          int numCircles, float scale = 1.0f,
                          bool useOrientation = true,
                          bool includeCenter = true,
                          bool useCircleOrder = true) const;


    /**
     * This helper function extracts a sequence of circles around the given
     * locations. The order of the points in the output vectors is from 
     * the center to the outer and each circle is sampled clockwise.
     * @param chan the input channel
     * @param locs the locations where extraction should start
     * @param dests the output vectors
     * @param numRays how many points are extracted from each circle
     * @param numCircles how many circles are used
     * @param scale the radius will be scaled with this factor (Default: 1)
     * @param useOrientation whether the angle of the location is used 
     * as orientation (Default: true)
     * @param includeCenter wheter the location itself should be in the
     * results (Default: 1)
     * @param useCircleOrder wheter the order of the points is along the 
     * circles
     * or along the rays. The data in the output vector is from 
     * the center to the outer and each circle is sampled clockwise. 
     * (Default: true)
     */
    bool fixedRaySampling(const channel& chan, 
                          const std::list<location>& locs, 
                          std::list<fvector>& dests, int numRays,
                          int numCircles, float scale = 1.0f,
                          bool useOrientation = true,
                          bool includeCenter = true,
                          bool useCircleOrder = true) const;
   /**
    * This helper function extracts pixels from a given channel. The coordinates
    * of this pixels are calculated from the given circPoints. 
    * (see initCircle())
    * @param chan the input channel
    * @param loc the location where extraction should start 
    * (center of the circle)
    * @param dest the output vector
    * @param circPoints mask used to calculate pixel coordinates
    * @param scale the radius will be scaled by this factor (Default: 1)
    * @param useOrientation whether the angle of the location is used as 
    * orientation (Default: true)
    */
    bool fixedGridSampling(const channel& chan, const location& loc,
                           fvector& dest, const array<int>& circPoints,
                           float scale = 1.0f,
                           bool useOrientation = true) const;

   /**
    * This helper function extracts pixels from a given channel. The coordinates
    * of this pixels are calculated from the given circPoints. 
    * (see initCircle())
    * @param chan the input channel
    * @param locs the locations where extraction should start 
    * (center of the circle)
    * @param dests the output vectors
    * @param circPoints mask used to calculate pixel coordinates
    * @param scale the radius will be scaled by this factor (Default: 1)
    * @param useOrientation whether the angle of the location is used as 
    * orientation (Default: true)
    */
    bool fixedGridSampling(const channel& chan, const std::list<location>& locs,
                           std::list<fvector>& dests, const array<int>& circPoints,
                           float scale = 1.0f,
                           bool useOrientation = true) const;

    /** This helper function creates a circular mask with the specified 
     * radius to use with fixedGridSampling.
     * @param radius which is used for the computed circle
     * @param circPoints the resulting mask will be left here
     */
    bool initCircle(int radius, array<int>& circPoints) const;

   /** 
    * Interpolater which can be used in child classes.
    */
    bilinearInterpolator<float> interpol;


  };
}

#endif

