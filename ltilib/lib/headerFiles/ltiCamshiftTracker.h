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
 * file .......: ltiCamshiftTracker.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 8.6.2001
 * revisions ..: $Id: ltiCamshiftTracker.h,v 1.19 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CAMSHIFT_TRACKER_H_
#define _LTI_CAMSHIFT_TRACKER_H_

// include files which are needed in this header!!
#include "ltiException.h"
#include "ltiGaussKernels.h"
#include "ltiMath.h"

// include to parent class:
#include "ltiModifier.h"

namespace lti {
  /**
   * This is the implementation of the CAMSHIFT (Continuously Adaptive
   * Mean Shift) Tracking algorithm as described in: G.Bradski,
   * "Computer Vision Face Tracking For Use in a Perceptual User
   * Interface", IEEE Workshop on Applic.Comp.Vis.,1998.
   *
   * It tracks a rectangular search window in a channel/channel8,
   * which must contain probability information of relevant image
   * content (e.g. a skinProbabilityMap).  It uses an iterative
   * gradient ascent algorithm, that finds the peak, which is the
   * closest to the specified window.
   *
   * Use this by calling an apply on subsequent images and thereby
   * passing the last result.  Nevertheless, it is possible to control
   * rectangle size and position manually.
   *
   * After an apply you can get Information about the distribution inside the
   * search window (e.g. orientation). Use the getXXX() methods to do so.
   */
  class camshiftTracker : public modifier {
  public:
    /**
     * the parameters for the class camshiftTracker
     */
    class parameters : public modifier::parameters {
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
	     * Kernel Type.
       */
      enum eKernelType {
        Constant,  /**< All data gets equal weight (default)*/
        Gaussian   /**< Data in the center of search window gets more weight */
      };

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

      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * In order to let the tracker adapt the window size, you have
       * to specify a ratio of height to width, that has to be kept
       * constant. Otherwise the window size will not be changed by
       * the tracker (this is the default, which means 0.0).
       *
       * height=sizeRatio*width
       */
      double sizeRatio;

      /**
       * This is meant for dealing with missing or insufficient data
       * within the search window. If zeroth moment within search
       * window doesn't exceed this quota of the maximum possible
       * zeroth moment, then the tracker simply doesn't react.
       *
       * Set this to a value between 0.0 and 1.0 for 0% resp. 100%
       * (although 100% is not a reasonable value)
       *
       * Default is 0.0
       */
      double threshold;

      /**
       * This indicates, how the Data inside the search window should
       * be weighted.  
       *
       * Default is "Constant".
       *
       * Please see eKernelType for further information.
       */
      eKernelType kernelType;

    };

    /**
     * default constructor
     */
    camshiftTracker();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    camshiftTracker(const camshiftTracker& other);

    /**
     * destructor
     */
    virtual ~camshiftTracker();

    /**
     * returns the name of this type ("camshiftTracker")
     */
    virtual const char* getTypeName() const;

    /**
     * Centers the given search window on the nearest probability peak.
     *
     * Tracking is realised by using this rectangle in the next call
     * of this function.  Information about the distribution inside
     * the search window is stored and can be retrieved by using the
     * getXXXX() methods of this functor.
     *
     * @param src channel with the source data. Should represent a
     *            probability map with values between 0.0 and 1.0
     * @param window the tracking window. Result will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,rectangle& window);

    /**
     * Centers the given search window on the nearest probability peak.
     *
     * Tracking is realised by using this rectangle in the next call
     * of this function.  Information about the distribution inside
     * the search window is stored and can be retrieved by using the
     * getXXXX() methods of this functor.
     *
     * @param src channel8 with the source data.  Should represent a
     *            probability map (prob(0)=0.0 and prob(255)=1.0)
     * @param window the tracking window. Result will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,rectangle& window);

    /**
     * Resets the internal state but not the parameters
     */
    bool reset();

    /**
     * Tells, if tracker is initialized
     */
    bool isInitialized();

    /**
     * Tells, if last tracking attempt delivered valid data
     */
    bool isValid();

    /**
     * Returns Main orientation (-Pi/2 to +Pi/2) of current internal
     * distribution state
     */
    double getOrientation();

    /**
     * Returns length of current internal distribution state
     */
    double getLength();

    /**
     * Returns width of current internal distribution state
     */
    double getWidth();

    /**
     * Returns center of current internal distribution state
     */
    tpoint<float> getCenter();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    camshiftTracker& copy(const camshiftTracker& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:
    /**
     * Struct for storing the tracker data after call of any apply method
     */
    class trackerState {
      public:
        trackerState();
        ~trackerState();

        // methods
        void clear();
        trackerState& copy(const trackerState& other);
        trackerState& operator=(const trackerState& other);

        // Moments
        double M00, M10, M01, M11, M20, M02;
        // relative Center positions and size
        double xc, yc, s;
        // Center position in absolute coordinates
        tpoint<float> absCenter;
    };

    /**
     * shifts and clips rect to fit into given canvas
     */
    bool correctRect(trectangle<int>& rect, const trectangle<int>& canvas) const;

    /**
     * calculates moments in given src
     */
    bool getTrackerState(const channel& src);

    /**
     * stores results of last call of one apply method
     */
    trackerState td;

    /**
     * indicates if the tracker is initialized
     */
    bool initialized;

    /**
     * indicates if last tracking attempt delivered valid data
     */
    bool valid;
  };
}

#endif

