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
 * file .......: ltiOrientationMap.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 26.5.2000
 * revisions ..: $Id: ltiOrientationMap.h,v 1.14 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_ORIENTATION_MAP_H_
#define _LTI_ORIENTATION_MAP_H_

#include "ltiTransform.h"

namespace lti {
  /**
   * Generate an orientation map of a given channel
   *
   * There are two modes for this functor:
   *  - The first one is based on the gradient approximation of the
   *    channel values, which will consider the whole frequency spectrum
   *    for the orientation estimation.
   *
   *  - The second mode is base on some characteristics of the first order
   *    oriented gaussian derivative to calculate the direction with the
   *    maximal energy.  The use of this steerable filters allows the
   *    consideration of just one frequency band.  It is much slower than the
   *    gradient mode.
   *
   * The result of the analysis are two channels:
   *
   * In the gradient mode the first resulting channel contains the
   * angle (in radians) of the local gradient.
   * The second channel contains the magnitude of the gradient.
   *
   * In the ogd mode the first channel will contain the
   * angle (in radians) with the local maximal energy, and
   * the second channel contains a relevance-ratio of the obtained direction.
   *
   * Note that the return angle is always perpendicular to the edge direction.
   *
   * The angular value lies between -Pi and Pi.
   * 
   * \warning:  This functor is too old, maybe you are just looking for the
   *            lti::gradientFunctor in the polar coordinates mode.
   *
   * @see orientationMap::parameters
   */
  class orientationMap : public transform {
  public:
    /**
     * the parameters for the class orientationMap
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
       * the possible modes for the orientation map
       */
      enum eMode {
        Ogd,      /*!< Oriented gaussian derivative */
        Gradient  /*!< Gradient mode */
      };

      /**
       * mode for the orientation map.  It can be Ogd or Gradient.
       *
       * Default value: Gradient
       */
      eMode mode;

      /**
       * size of the used filter.
       *
       * This specify the size of the region being used to calculate
       * the direction.  In "Gradient"-mode this is the size of the
       * gradient approximation kernel.
       */
      int size;

      /**
       * variance of the used ogd filter
       */
      double variance;

      /**
       * size of the low pass filter used as integrator
       *
       * To calculate the direction, a local-region from
       * the ogd-filtered channel must be evaluated.  This parameter
       * specify the dimensions of this region.
       */
      int localFilterSize;

      /**
       * variance of the low pass filter used as integrator
       */
      double localFilterVariance;
    };

    /**
     * default constructor
     */
    orientationMap();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    orientationMap(const orientationMap& other);

    /**
     * destructor
     */
    virtual ~orientationMap();

    /**
     * returns the name of this type ("orientationMap")
     */
    virtual const char* getTypeName() const;

    /**
     * generate the orientation map
     * @param src channel with the source data.
     * @param direction channel where the direction will be left.
     *        The values will be between -Pi and Pi.
     * @param relevance channel with values between 0 and 1, which
     *        specify how relevant the direction information is.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& src,
                     channel& direction,
                     channel& relevance) const;


    /**
     * generate the orientation map
     * @param src channel8 with the source data.
     * @param direction channel8 where the direction will be left.
     *                  The values will be between -Pi and Pi.
     * @param relevance channel8 with values between 0 and 1, which
     *                  specify how relevant the direction information is.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel8& src,
                     channel8& direction,
                     channel8& relevance) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    orientationMap& copy(const orientationMap& other);

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
     * generate the orientation map
     * @param src channel with the source data.
     * @param direction channel where the direction will be left.
     *        The values will be between -Pi and Pi.
     * @param relevance channel with values between 0 and 1, which
     *        specify how relevant the direction information is.
     * @result a reference to the <code>direction</code>.
     */
    channel& ogdMap(const channel& src,
                          channel& direction,
                          channel& relevance) const;

    /**
     * generate the orientation map
     * @param src channel with the source data.
     * @param direction channel where the direction will be left.
     *        The values will be between -Pi and Pi.
     * @param relevance channel with values between 0 and 1, which
     *        specify how relevant the direction information is.
     * @result a reference to the <code>direction</code>.
     */
    channel& gradientMap(const channel& src,
                               channel& direction,
                               channel& relevance) const;

  };
}

#endif

