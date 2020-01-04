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
 * file .......: ltiLocalMoments.h
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 28.4.2004
 * revisions ..: $Id: ltiLocalMoments.h,v 1.3 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LOCAL_MOMENTS_H_
#define _LTI_LOCAL_MOMENTS_H_

//TODO: include only those files which are needed in this header!!

#include <list>
#include "ltiImage.h"
#include "ltiArray.h"
#include "ltiLocalFeatureExtractor.h"

namespace lti {
  /**
   * This local feature extractor can be used to estimate the 
   * standardized moments (Mean, Variance, Skew and Kurtosis) 
   * of a certain circular area around the given location.
   *
   * For Variance, Skew and Kurtosis unbiased estimators are used.
   *
   * \code
   * #include "ltiLocalMoments.h"
   *
   * lti::channel src;
   * lti::point p(200,200);
   * lti::location(p,0,5);
   * lti::dvector dest;
   * 
   * lti::localMoments lM;
   * lM.apply(src,loc,dest);
   *
   * // per default all those moments are calculated:
   * std::cout << "Mean ="<<dest[0]<<"\n";
   * std::cout << "Variance = "<<dest[1]<<"\n";
   * std::cout << "Skew = "<<dest[2]<<"\n";
   * std::cout << "Kurtosis = "<<dest[3]<<"\n";
   * \endcode
   *
   * @ingroup gFeatureExtr
   */
  class localMoments : public localFeatureExtractor {
  public:
    /**
     * The parameters for the class localMoments
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
       * Which method is used to extract pixels of the original image.
       *
       * Default: FixedRaySampling
       */
      eSamplingMethod samplingMethod;

      /**
       * If this is true, the mean of the location pixels is 
       * calculated and stored in the result vector.
       *
       * The values in each result vector are ordered by ascending complexity.
       * (Mean,Variance,Skew,Kurtosis) 
       *
       * Default: true
       */
      bool calculateMean;
      /**
       * If this is true, the variance of the location pixels is 
       * calculated and stored in the result vector.
       *
       * The values in each result vector are ordered by ascending complexity.
       * (Mean,Variance,Skew,Kurtosis) 
       *
       * Default: true
       */
      bool calculateVariance;
      /**
       * If this is true, the skew of the location pixels is 
       * calculated and stored in the result vector.
       *
       * The values in each result vector are ordered by ascending complexity.
       * (Mean,Variance,Skew,Kurtosis) 
       *
       * Default: true
       */
      bool calculateSkew;
      /**
       * If this is true, the kurtosis of the location pixels is 
       * calculated and stored in the result vector.
       *
       * The values in each result vector are ordered by ascending complexity.
       * (Mean,Variance,Skew,Kurtosis) 
       *
       * Default: true
       */
      bool calculateKurtosis;

      /**
       * Factor used to scale the radius of the given locations.
       *
       * Default: 1.0f
       */
      float scale;

      /**
       * Radius of the fixed grid used for fixedGridSampling().
       * This parameter has no effect if FixedRaySampling is used.
       *     
       * Default: 3    
       */
      int gridRadius;

      /**
       * Number of Circles used for fixedRaySampling().
       * This parameter has no effect if FixedGridSampling is used.
       *
       * Default: 3
       */
      int numCircles;

      /**
       * Number of Rays used for fixedRaySampling().
       * This parameter has no effect if FixedGridSampling is used.
       *
       * Default: 24
       */
      int numRays;


    };

    /**
     * Default constructor
     */
    localMoments();

    /**
     * Construct a functor using the given parameters
     */
    localMoments(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    localMoments(const localMoments& other);

    /**
     * Destructor
     */
    virtual ~localMoments();

    /**
     * Returns the name of this type ("localMoments")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    virtual bool setParameters(const parameters& theParam);    

    /**
     * Compute the local feature for the given location.
     *
     * @param src original channel to be analyzed.
     * @param loc location that need to be analyzed.
     * @param dest feature vector. 
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::channel& src,
                       const location& loc,
                       dvector& dest) const;

    /**
     * Compute the local feature for the given location for each color channel
     * of the given image.
     * The resulting vectors are concatinated in this order: red,green,blue.
     *
     * @param src original image to be analyzed.
     * @param loc location that need to be analyzed.
     * @param dest feature vector. 
     * @return true if successful, or false otherwise.  
     */
    virtual bool apply(const lti::image& src,
                       const location& loc,
                       dvector& dest) const;

    /** 
     * Compute the local features for the given locations for each 
     * color channel of the given image.
     * The resulting vectors are concatinated in this order: red,green,blue.
     *     
     * @param src original channel to be analyzed.
     * @param locs list of locations that need to be analyzed.
     * @param dests list of feature vectors. 
     * @return true if successful, or false otherwise. 
     */   
    virtual bool apply(const lti::image& src,
                       const std::list<location>& locs,
                       std::list<dvector>& dests) const;

   /** 
    * Compute the local features for the given locations.
    *
    * @param src original channel to be analyzed.
    * @param locs locations that need to be analyzed.
    * @param dests feature vectors. 
    * @return true if successful, or false otherwise. 
    */
    virtual bool apply(const lti::channel& src,
                       const std::list<location>& locs,
                       std::list<dvector>& dests) const;


    /**
     * Returns whether the onPlace version of apply is faster or
     * the onCopy version.
     */
//     virtual functor::eFasterApply theFaster() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localMoments& copy(const localMoments& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    localMoments& operator=(const localMoments& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  protected:
    /**
     * Internal array for circular mask of fixedGridSampling
     */
    array<int> circPoints;

  };
}

#endif

