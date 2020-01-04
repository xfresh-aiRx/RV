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
 * file .......: ltiPanTiltUnit.h
 * authors ....: Claudia Goenner (modified by Christian Hein)
 * organization: LTI, RWTH Aachen
 * creation ...: 28.8.2003 (Modification 28.09.2004 & 30.03.05) 
 * revisions ..: $Id: ltiPanTiltUnit.h,v 1.8 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_PANTILTUNIT_H_
#define _LTI_PANTILTUNIT_H_

#include "ltiFunctor.h"
#include "ltiPoint.h"

namespace lti {
  /**
   * Parent class for all pan tilt units.
   *
   * Inherited classes should use the parameter \a angleFormat to
   * allow passing the pan and tilt values in either degrees or
   * radians.  Please add all values (which can be set) as parameters
   * to allow construction with application specific settings.  During
   * operation, value settings using a specific method is more efficient as
   * by setParameters.  Pan tilt constants, e.g. the max. pan and
   * tilt, should be defined as constant static parameters.
   *
   * Please note that the camera may take a little while until a pan
   * or tilt position is reached. Thus the values in the parameters
   * and those returned by the get-methods may differ.
   *
   * The parameter functionality was reimplemented, following the
   * functor's concept, because of a problem with multiple
   * inheritance, which occures when deriving classed which are both a
   * camera and a panTiltUnit.
   */
  class panTiltUnit : public virtual functor {
  public:
    /**
     * the parameters for the class panTiltUnit
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
       * constructor
       * @param minPan
	   * @param maxPan
	   * @param minTilt
	   * @param maxTilt
       */
      parameters(float minPan, float maxPan, float minTilt,float maxTilt);

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
       * angle formats
       */
      enum eAngleFormatType {Degrees, Radiant};
  
      /**
       * Specifies if the pan and tilt values are measured in degrees or radiant.
       * Default: Radiant.
       */
      eAngleFormatType angleFormat;

      /**
       * Pan value to which the pan tilt device shall move. Default: 0.
       */
      float pan;

      /**
       * Tilt value to which the pan tilt device shall move. Default: 0.
       */
      float tilt;

      /**
       * Specifies whether the pan tilt device shall move to a relative (true)
       * or absolute Position (false). This is here to ensure a well defined 
       * behavior. If a specific pan tilt device does not support the one or 
       * the other, catch it in setParameters(). This way any application
       * can be informed about a relative or absolute movement.
       */
      bool relativeMovement;

      /**
       * The pan tilt unit can physically move between minPan and
       * maxPan.  This values are always specified in radiants and
       * automatically converted by the getMinPan() / getMaxPan() method 
       * if the angle format is set to degrees.
       */
      const float minPan;
      
      const float maxPan;
      
      /**
       * The pan tilt unit can physically move between minTilt and
       * maxTilt.  This values are always specified in radiants and
       * automatically converted by the getMinTilt() / getMaxTilt() method 
       * if the angle format is set to degrees.
       */
     
      const float minTilt;
      
      const float maxTilt;
    
   };

    /**
     * default constructor
     */
    panTiltUnit();

    /**
     * Construct a functor using the given parameters
     */
    panTiltUnit(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    panTiltUnit(const panTiltUnit& other);

    /**
     * destructor
     */
    virtual ~panTiltUnit();

    /**
     * returns the name of this type ("panTiltUnit")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    panTiltUnit& copy(const panTiltUnit& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    panTiltUnit& operator=(const panTiltUnit& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //----------------------
    // pan tilt unit methods
    //----------------------

    /**
     * Reset the pan tilt device. 
     *
     * This method resets internal pan and tilt registers.  Some
     * devices need resets from time to time to avoid accumulation
     * errors.  After succesfull execution the pan tilt unit should be
     * in the same position as before the call. During execution the
     * pan tilt unit may move.  @return true on success
     */
    virtual bool reset() = 0;

    /**
     * stop the movement
     * @return true on success
     */
    virtual bool stopPanTilt() = 0;

    /** 
     * set the pan and tilt position. Usually pan and tilt are sent to
     * the device in a single package.
     * @param pan the pan position the device will move to
     * @param tilt the pan position the device will move to
     * @return true on success
     */
    virtual bool setPanTilt(float pan, float tilt) = 0;

    /** 
     * set the pan and tilt position. Usually pan and tilt are sent to
     * the device in a single package.
     * @param pos the pan tilt position the device will move to
     * @return true on success
     */
    inline bool setPanTilt(const tpoint<float> &pos) {
      return setPanTilt(pos.x, pos.y);
    };

    /** 
     * set the pan position
     * @param pan the pan position the device will move to
     * @return true on success
     */
    virtual bool setPan(float pan) = 0;

    /** 
     * set the tilt position
     * @param tilt the pan position the device will move to
     * @return true on success
     */
    virtual bool setTilt(float tilt) = 0;

    /**
     * get the current pan and tilt position. The combined
     * get-methods guarantee pan and tilt positions of the
     * same time instant.
     * @param pan the pan position which the device is in
     * @param tilt the tilt position which the device is in
     * @return true on success
     */
    virtual bool getPanTilt(float& pan, float& tilt) const = 0;

    /**
     * get the current pan and tilt position. The combined
     * get-method guarantees pan and tilt positions of the
     * same time instant.
     * @return the current pan and tilt position
     */
    inline tpoint<float> getPanTilt() const {
      tpoint<float> tmp;
      getPanTilt(tmp.x, tmp.y);
      return tmp;
    };

    /**
     * @return the current pan position
     */
    virtual float getPan() const = 0;

    /**
     * @return the current tilt position
     */
    virtual float getTilt() const = 0;

    /**
     * get the min. supported pan and tilt positions as specified by
     * angleFormat
     *
     * @param minPan the min. pan position
     * @param minTilt the min. tilt position
     * @return true on success
     */
    virtual bool getMinPanTilt(float& minPan, float& minTilt) const;

    /**
     * @return the min. supported pan and tilt positions as specified
     * by angleFormat
     */
    inline tpoint<float> getMinPanTilt() const {
      tpoint<float> tmp;
      getMinPanTilt(tmp.x, tmp.y);
      return tmp;
    };
    
    /**
     * get the max. supported pan and tilt positions as specified by
     * angleFormat
     *
     * @param maxPan the max. pan position
     * @param maxTilt the max. tilt position
     * @return true on success
     */
    virtual bool getMaxPanTilt(float& maxPan, float& maxTilt) const;

    /**
     * @return the max. supported pan and tilt positions as specified
     * by angleFormat
     */
    inline tpoint<float> getMaxPanTilt() const {
      tpoint<float> tmp;
      getMaxPanTilt(tmp.x, tmp.y);
      return tmp;
    };
 
    /**
     * @return the min. supported pan position as specified by angleFormat
     */
    virtual float getMinPan() const;
    
    /**
     * @return the max. supported pan position as specified by angleFormat
     */
    virtual float getMaxPan() const;
    
    /**
     * @return the min. supported tilt position as specified by angleFormat
     */
    virtual float getMinTilt() const;
    
    /**
     * @return the max. supported tilt position as specified by angleFormat
     */
    virtual float getMaxTilt() const;

  };
}

#endif

