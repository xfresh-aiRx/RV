/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiCamera.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 28.8.2003
 * revisions ..: $Id: ltiCamera.h,v 1.6 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_CAMERA_H_
#define _LTI_CAMERA_H_

#include "ltiFunctor.h"

namespace lti {
  /**
   * Parent class for all cameras with control over the lens system, including
   * zooming. Cameras without zoom shall set the maxZoom parameter to 0. When 
   * deriving a camera, please add all values (which can be set) as parameters
   * to allow construction with application specific settings. During operation
   * value setting by a specific method is more efficient as by setParameters.
   * Camera constants, e.g. the max. and min. focus, should be defined as constant
   * parameters.
   *
   * Please note that the camera may take a little while until a focus, zoom, etc.
   * position is reached. Thus the values in the parameters and those returned by
   * the get-methods may differ.
   *
   * The parameter functionality was reimplemented, following the functor's concept,
   * because of a problem with multiple inheritance, which occures when deriving
   * classed which are e.g. both a camera and a panTiltUnit.
   */
  class camera : public virtual functor {
  public:
    /**
     * the parameters for the class camera
     */
    class parameters : public virtual functor::parameters {
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
       * Mode of determing the whiteBalance. Default: Auto, i.e. true.
       * Derived classes may distinguish between more than two modes by
       * adding a whiteBalanceMode-parameter. If a class does not distinguish between
       * further modes false refers to manual white balance, i.e. direct control
       * by the red and blue gain values. Default: true.
       */
      bool autoWhiteBalance;

      /**
       * automatic determination of the global gain by the camera (true) or
       * manual setting by the user (false). Default: true.
       */
      bool autoGain;
      
      /**
       * the global gain in dB. Default: maxGain.
       */
      float gain;

      /**
       * the red gain in dB. Default: maxRBGain.
       */
      float redGain;

      /**
       * the green gain in dB. Default: maxRBGain.
       */
      float blueGain;

      /**
       * the min. possible gain in dB
       */
      static const float minGain;

      /**
       * the max. possible gain in dB
       */
      static const float maxGain;
     
      /**
       * the min. possible red or blue gain in dB
       */
      static const float minRBGain;

      /**
       * the max. possible red or blue gain in dB
       */
      static const float maxRBGain;

      /**
       * if true the shutter speed is set automatically, otherwise it is
       * controlled manually. Default: true.
       */
      bool autoShutter;

      /**
       * the shutter speed in 1/s. Default: minShutterSpeed.
       */
      float shutterSpeed;

      /**
       * the minimum shutter speed of the device
       */
      static const float minShutterSpeed;

      /**
       * the maximum shutter speed
       */
      static const float maxShutterSpeed;

      /**
       * auto focus (true) or manual focus (false). Default: true.
       */
      bool autoFocus;

      /**
       * focus value which the camera shall use in meter. Default: maxFocus.
       */
      float focus;

      /**
       * the minimum focus in meter
       */
      static const float minFocus;

      /**
       * the maximum focus in meter
       */
      static const float maxFocus;

      /**
       * Zoom value which the camera shall use. Default: 0.
       */
      float zoom;

      /**
       * The maximum possible zoom value. The zoom ranges from 0 (no zoom) to
       * max. zoom.
       */
      static const float maxZoom;
    };

    /**
     * default constructor
     */
    camera();

    /**
     * Construct a functor using the given parameters
     */
    camera(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    camera(const camera& other);

    /**
     * destructor
     */
    virtual ~camera();

    /**
     * returns the name of this type ("camera")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    camera& copy(const camera& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    camera& operator=(const camera& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //-------------------------
    // the camera functionality
    //-------------------------

    /**
     * turn on automatic white balance
     * @return true on success
     */
    virtual bool autoWhiteBalanceOn() = 0;

    /**
     * turn on automatic gain
     * @return true on success
     */
    virtual bool autoGainOn() = 0;

    /**
     * set the global gain. Turns off automatic gain,
     * i.e. sets it to manual mode.
     * @param value the gain value
     * @return true on success
     */
    virtual bool setGain(float value) = 0;

    /**
     * set the red gain
     * @param value the red gain value
     * @return true on success
     */
    virtual bool setRedGain(float value) = 0;

    /**
     * set the blue gain
     * @param value the blue gain value 
     * @return true on success
     */
    virtual bool setBlueGain(float value) = 0;
    
    /**
     * turn on automatic choice of the shutter speed
     * @return true on success
     */
    virtual bool autoShutterSpeedOn() = 0;

    /**
     * set the shutter speed. Turn off automatic shutter.
     * @param value the shutter speed
     * @return true on success
     */
    virtual bool setShutterSpeed(float value) = 0;

    /**
     * set the focus. Turns off autofocus.
     * @param value the focus value
     * @return true on success
     */
    virtual bool setFocus(float value) = 0;

    /**
     * set the focus to infiniy. Turns off autofocus.
     * @return true on success
     */
    virtual bool setFocusToInfinity() = 0;

    /**
     * turn on auto focus
     * @return true on success
     */
    virtual bool autoFocusOn() = 0; 

    /**
     * @return the current focus
     */
    virtual float getFocus() const = 0;

    /**
     * set the zoom which the camera shall use next.
     * @param value the zoom value
     * @return true on success
     */
    virtual bool setZoom(float value) = 0;

    /**
     * @return the current zoom.
     */
    virtual float getZoom() const = 0;

    /**
     * Get the focus and zoom values from a single time instant.
     * @param focus the current focus value
     * @param zoom the current zoom value
     * @return true on success
     */
    virtual bool getLensSetting(float& focus, float& zoom) const = 0;

  };
}

#endif

