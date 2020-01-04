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
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiOrientationFeature.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.5.2001
 * revisions ..: $Id: ltiOrientationFeature.h,v 1.13 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_ORIENTATION_FEATURE_H_
#define _LTI_ORIENTATION_FEATURE_H_

#include "ltiImage.h"
#include "ltiVector.h"

#include "ltiGlobalFeatureExtractor.h"

namespace lti {

  /**
   * The orientation feature takes two channels: a "relevance" channel
   * and an "orientation" channel, and with them it creates an
   * orientation histogram with the given number of cells.
   *
   * Due to the fact that the orientation is Pi cyclic (0° is the same
   * thing as 180°) the histogram created will consider the angles as being
   * between 0 and Pi (angle mod Pi).
   *
   * A relevance/orientation pair can be for example the gradient
   * magnitude and phase channels (see lti::gradientKernelX and
   * lti::cartesicToPolar) and the OGD local energy maps (lti::axOGDFeature).
   * Both methods are ready implemented in the lti::orientationMap %functor.
   *
   * \code
   *
   * lti::orientationMap omapper;
   * lti::orientationFeature ofeat;
   *
   * lti::channel chnl,mag,dir;
   * lti::vector<double> vct;
   *
   * // construct a channel with a rectangle on it:
   * chnl.resize(256,256,0.0);
   * chnl.fill(1.0,32,32,200,240);
   *
   * // extract the orientation map from the channel
   * omapper.apply(chnl,dir,mag);
   *
   * // generate the orientation feature
   * ofeat.apply(dir,mag,vct);
   *
   * \endcode
   *
   * If you need a rotation invariant feature vector see also
   * lti::shiftInvariance
   */
  class orientationFeature : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class orientationFeature
     */
    class parameters : public globalFeatureExtractor::parameters {
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
       * if true, the feature vector will be normalized to have an area of
       * one, i.e. the feature vector can be interpreted as a probability
       * distribution for the orientations.
       * If false, the resulting values depend on the values of the
       * magnitud channel.
       * Default: true
       */
      bool normalize;

      /**
       * size of the histogram.  The angles from 0 to 180(1-1/size) degrees
       * will be mapped between 0 and size-1.
       * Default value: 36 (10 degrees resolution)
       */
      int size;
    };

    /**
     * default constructor
     */
    orientationFeature();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    orientationFeature(const orientationFeature& other);

    /**
     * destructor
     */
    virtual ~orientationFeature();

    /**
     * returns the name of this type ("orientationFeature")
     */
    virtual const char* getTypeName() const;

    /**
     * generates the "weighted histogram" of the given magnitude and
     * orientations channels.
     *
     * @param orientation channel with the orientation of edges
     * @param magnitude channel with the relevance of the orientation.
     * @param dest the result vector with the histogram data
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& orientation,
               const channel& magnitude,
               dvector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    orientationFeature& copy(const orientationFeature& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

