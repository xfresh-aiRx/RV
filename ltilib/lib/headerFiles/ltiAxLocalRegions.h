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
 * file .......: ltiAxLocalRegions.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 7.1.2002
 * revisions ..: $Id: ltiAxLocalRegions.h,v 1.11 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_AX_LOCAL_REGIONS_H_
#define _LTI_AX_LOCAL_REGIONS_H_

#include "ltiImage.h"
#include "ltiMath.h"
#include "ltiTransform.h"
#include "ltiLocation.h"
#include "ltiGaussianPyramid.h"
#include <list>
#include <map>

namespace lti {
  /**
   * This functor is based on concepts described in
   * Lowe, D.G. "Object Recognition from Local Scale-Invariant Features",
   * Proc. of the International Conference on Computer Vision, Corfu, Greece
   * Sept. 1999. pp. 1150-1157.  This paper can be found at
   *
   * http://www.cs.ubc.ca/spider/lowe/papers/iccv99.pdf
   *
   * It uses the DoG to detect relevant locations, and depending on the
   * scale at which the location was found, a corresponding radius will be
   * assigned.  The orientation of the location will be computed using a small
   * orientation histogram on the gradient of the image.
   *
   * The location detection is done somehow different as in Lowe's
   * paper, in order to get a result much faster (but maybe with more
   * locations as necessary):
   *
   * -# For the given channel, a gaussian pyramid is build, where the
   *    differences between the std. deviation of the gaussians for the
   *    different levels are sqrt(2) instead of 2.
   * -# The DoG (differences of gaussians) between the levels of the pyramid
   *    are computed.
   * -# Compute the local maxima of each level.  If their values are greater
   *    than the given threshold multiplied by the maximum value in the image,
   *    then consider that region as a valid interesting location and insert
   *    it in the locations list.
   * -# For each location in the list, compute its orientation using
   *    the maximal value in a orientation histogram computed for a window
   *    of the size given in the parameters.
   */
  class axLocalRegions : public transform {
  public:
    /**
     * the parameters for the class axLocalRegions
     */
    class parameters : public transform::parameters {
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
       * The gaussian used as window for the orientation histograms will
       * have a std. deviation equal to this factor multiplied with the
       * std. deviation of the location scale.
       *
       * Default value: 3
       */
      double orientationWindowFactor;

      /**
       * Number of bins used in the orientation histogram.
       *
       * Default value 32
       */
      int binsInOrientationHistogram;

      /**
       * first level to consider.  Must be greater 0 and smaller or equal than
       * lastLevel.
       *
       * Default: 2
       */
      int firstLevel;

      /**
       * last level to consider.  Must be greater or equal than
       * firstLevel
       *
       * Default: 6
       */
      int lastLevel;

      /**
       * Saliency threshold values per level.  The elements of the
       * vector should be between 0 and 1.  The element of the vector
       * with the same index as a any given level of the internally
       * computed pyramid will be multiplied with the maximum value in
       * that level.  Only those locations which surpass this value
       * are then considered as relevant and inserted in the locations
       * list.  The size of the vector should be at least lastLevel+1.
       * Otherwise, the last value will be taken for all the remaining
       * levels.
       *
       * Default value: vector of size 1 with the value 0.1,
       *
       */
      vector<float> saliencyThreshold;

      /**
       * To detect the local maxima, a lti::maximumFilter is used.  The size
       * of the kernel used for the firstLevel is the one given here.  It
       * should be an odd number.
       *
       * Default value: 5
       */
      int firstMaximumFilterSize;

      /**
       * The size of the used kernel can be reduced for each higher level
       * in the pyramid.  This value should be less or equal 1.0.  Internally,
       * the kernel value used will be always rounded to an odd integer number
       * greater or equal 3.
       *
       * Default value: 1.0, i.e. do not change the filter size.
       */
      float maximumFilterAdaption;
    };

    /**
     * default constructor
     */
    axLocalRegions();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    axLocalRegions(const axLocalRegions& other);

    /**
     * destructor
     */
    virtual ~axLocalRegions();

    /**
     * returns the name of this type ("axLocalRegions")
     */
    virtual const char* getTypeName() const;

    /**
     * extracts the most relevant locations from the given channel, and return
     * them as list of lti::location objects
     * @param src channel with the source data.
     * @param locs list of relevant locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,std::list<location>& locs) const;

    /**
     * extracts the most relevant locations from the given channel, and return
     * them in a map of lists of lti::location objects, the map first key
     * is the radius of the locations.
     *
     * You can iterate in the lists of locations with the same radius as
     * follows:
     *
     * \code
     * std::map<float,std::list<location> > locs; // map of lists of locations
     * channel chnl;                              // channel to be analyzed
     * draw<float> drawer;                        // draw on channel
     * drawer.use(chnl);                          // using chnl as canvas
     *
     * // ... load or create the channel to be analyzed
     *
     * axLocalRegions localizer;   // our local regions functor
     * localizer.apply(chnl,locs); // get the local regions
     *
     * std::map<float,std::list<location> >::const_iterator locsIt;
     * std::list<location>::const_iterator it;
     *
     * for (locsIt=locs.begin();locsIt!=locs.end();++locsIt) {
     *   cout << "Locations for radius " << (*locsIt).first << endl;
     *
     *   // do something with each location at this level:
     *   for (it=(*locsIt).second.begin();
     *        it!=(*locsIt).second.end();
     *        ++it) {
     *     // for example, draw the location:
     *    drawer.set(*it);
     *   }
     * }
     *
     * \endcode
     *
     * @param src channel with the source data.
     * @param locs map of relevant locations
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               std::map<float,std::list<location> >& locs) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    axLocalRegions& copy(const axLocalRegions& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    axLocalRegions& operator=(const axLocalRegions& other);

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
     * with the final DoG pyramid, compute the location list
     */
    bool pyramidToLocs(const channel& src,
                       const pyramid<channel>& pyr,
                             std::list<location>& locs) const;

  private:
    /**
     * returns true if i is an even number
     */
    inline bool even(const int i) const;

    /**
     * return the gaussian with sigma = 2
     */
    inline double gaussian(const float dy,
                           const float dx,
                           const double& s2) const;

  };
}

#endif

