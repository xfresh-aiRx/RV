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
 * file .......: ltiSonyEviD100P.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 28.8.2003
 * revisions ..: $Id: ltiSonyEviD100P.h,v 1.8 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_SONYEVID100P_H_
#define _LTI_SONYEIVD100P_H_

#include "ltiCamera.h"
#include "ltiPanTiltUnit.h"
#include "ltiSerial.h"
#include "ltiMutex.h"
#include <string>


namespace lti {
  /**
   * Access to the Sony Evi D100P camera and pan tilt unit. So far only a
   * single device on the serial port and asynchronous behauvior are
   * supported.
   *
   * In asynchronous mode a returned true of the set-methods means that the
   * command has been successfully sent to the device. About the proper
   * execution by the device nothing is known at that time. Furthermore the
   * sony protocoll does not tell which commands have been completed
   * successfully or caused an error. It is unclear, if the order of sent
   * commands and received status messages correspond.
   *
   * If a command is successfully sent to the device, but not executed
   * properly the parameters do not resemble the current status of the
   * device. Therefore all called methods send a command to the device, no
   * matter whether the device should already be in that status or not.
   */
  class sonyEviD100P : public camera, public panTiltUnit {
    /*-----------------------------------------
     * Poll Helper
     *----------------------------------------*/
  private:
    /**
     * Polls the Sony Evi D100P camera. This class performs all communication
     * to the camera, guaranteeing thread safety. It has a buffer for all
     * information send by the camera, i.e. pan and tilt position. In order
     * to keep the buffers up to date, inquiries should be send on a regular
     * basis, e.g. by a deduced class with a thread running in a loop to allow
     * asynchronous communication.
     *
     * Please note that after construction all inquired values are set to their
     * default, no matter whether this resembles the actual state of the sony
     * or not. The buffers will be updated as soon as the first inquiry has
     * been sent and received. 
     */
    class poll : public object {
    public:

      /**
       * Constructor.
       * @param serialPortName The serial port, to which the camera is connected,
       *                       e.g. Com1 (default)
       * @param waitForInit the msec which will be waited for feedback from
       *                    the device, e.g. 100 (default)
       */
      poll(serial::parameters::ePortType port = serial::parameters::Com1,
	   int waitForInit=100);

      /**
       * Destructor.
       */
      ~poll();
      
      /**
       * @return the name of this type
       */
      virtual const char* getTypeName () const;
      
      /**
       * check returned messages and update the buffers
       */
      virtual void processMessages();
      
      /**
       * send inquiry commands to the device
       * @return true on success
       */
      virtual bool inquiry();
      
      /**
       * Registration. Call this method for each copied pointer.
       */
      virtual void enroll();
      
      /**
       * SignOff a pointer.
       * @return the number of objects that are still registered.
       */
      virtual int signOff();

      /**
       * Send a command to the device.
       * @param description the command in a format well readable to humans
       * @param command the hex command which will be send to the device
       * @return true on success
       */
      virtual bool send(const std::string& description, vector<ubyte>& command);
      
      /**
       * Get the current pan and tilt position (of the same time instant).
       * @param pan the current pan position in an internal format
       * @param tilt the current tilt position in an internal format
       */
      virtual void getPanTilt(float& pan, float& tilt) const;

      /**
       * Get the current pan position.
       * @return the current pan position in an internal format
       */
      virtual int getPan() const;

      /**
       * Get the current tilt position.
       * @return the current tilt position in an internal format
       */
      virtual int getTilt() const;

      /**
       * Set the pan and tilt position, i.e. update the buffers
       * @param pan the current pan position in an internal format
       * @param tilt the current tilt position in an internal format
       */
      virtual void setPanTilt(int pan, int tilt);

      /**
       * Get the current lens setting.
       * @param focus the current focus in an internal format
       * @param zoom the current zoom in an internal format
       */
      virtual void getLensSetting(int& focus, float& zoom) const;
      
      /**
       * Get the current zoom.
       * @return the current zoom in an internal format
       */
      virtual int getZoom()  const;

      /**
       * Get the current focus.
       * @return the current focus in an internal format
       */
      virtual int getFocus()  const;

      /**
       * Set the current lens setting.
       * @param zoom the current zoom in an internal format
       * @param focus the current focus in an internal format
       * @param focusNearLimit the current focusNearLimit in an internal format
       */
      virtual void setLensSetting(int zoom, int focus, int focusNearLimit);

      /**
       * Get the current focus near limit position. 
       * @return the focus near limit position in an internal format
       */
      virtual int getFocusNearLimit() const;
      
      /**
       * Remember the current focus mode
       * @param mode the focus mode (auto focus (1), manual focus (0)).
       */
      virtual void rememberFocusMode(bool mode);

      /**
       * @return the current focus mode (auto focus (1), manual focus (0)).
       */
      virtual bool getFocusMode() const;
      
      /**
       * Turn on/off polling for lens values. This slows down the pan/tilt
       * position rate.
       * @param on poll the lens system if true, do not otherwise
       */
      virtual void pollLens(bool on);
 
     /**
       * Poll for pan tilt positions. If this slows down the lens system is not
       * tested yet.
       * @param on poll for pan and tilt position if true, do not otherwise
       */
      virtual void pollPanTilt(bool on);
    
      /*
       * init the camera network and get the camera address
       * @return true on success
       */
      bool initCamNetwork();
 
    private: // No copies
      
      poll ( const poll & rhs );
      
      poll & operator = ( const poll & rhs );
      
    private:
      //the serial port to which the camera is connected
      serial m_serialPort;    //not static because there are several serial ports in the system
      
      //the camera address - currently only one camera per port is provided
      unsigned char m_camAddress;
      unsigned char m_recAddress;
      bool m_ifClear;
      int m_waitForInit;

      //latest values
      int m_panPos;
      int m_tiltPos;
      int m_zoomPos;
      int m_focusPos;
      int m_focusNearLimitPos;
      bool m_autoFocus;
      //bool m_focusing;
      //bool m_zooming;
      
      int m_numPosInquiries;    //counts pan tilt position inquiries on the way
      int m_numBlockInquiries;  //counts block inquiries (lens system) on the way
      int m_numPosCalls;  //counts when no inquiry are send (too much traffic)
      int m_numLensCalls; //counts when no inquiry are send (too much traffic)

      bool m_pollLens;              //poll lens values 
      bool m_pollPanTilt;           //pan tilt positions 
    
      //buffer of received messages
      //max. possible number of bytes is 15, the end flag 0xFF is not stored
      ubyte m_recBuffer[15];
      //current length of read bytes
      int m_length;
      
      //mutex for thread safety
      //1) for registration/signing off
      //2) for sending/reading commands via the serial port (communication)
      //3) for writing/reading pan tilt positions or lens settings (write)
      //4) for inquiry/processMessages (max. 3 inquiries may be on the way)
      mutex m_registrateMutex;
      mutex m_communicateMutex;
      mutable mutex m_bufferMutex;
      mutex m_inquiryMutex;

      //object count
      int m_referenceCount;
    };
    
    //ToDo: Deduce class for asynchronous polling 
    //Remark: A class with signaling (synchronous) only make sense, if I guarantee
    //        that the the answer belongs too a specific inquiry. Then only
    //        one inquiry may be on the way (instead of three). The sony
    //        also should not be moving to ensure that the inquired values
    //        fit to the grabbed image. Solution: Add blocking getNextPanTilt(...),
    //        getNextLensSetting(...) which also checks if the sony moves and has
    //        a timeout.

    /*---------------------------------------------------------------------------
     * the spy class: This is here to ensure registration 
     *--------------------------------------------------------------------------*/
  public: 
    /**
     * If an user wants to send inquiries to the device and process the answers explicitly,
     * he may obtain a reference to this class.
     */
    class externPoll : public object {
      
      public:
      /**
       * constructor
       */
      externPoll();
      
      /**
       * copy constructor
       * @param other the class to be copied
       */
      externPoll(const externPoll& other);
    
      /**
       * destructor
       */
      ~externPoll();

      /**
       * copy data of "other" functor.
       * @param other the class to be copied
       * @return a reference to this functor object
       */
      externPoll& copy(const externPoll& other);

      /**
       * alias for copy member
       * @param other the class to be copied
       * @return a reference to this functor object
       */
      externPoll& operator=(const externPoll& other);

      /**
       * Attaches the class which does the actual inquiry of the device.
       * @param poller the poller-object which does the inquiry
       */
      virtual void attachPoller(poll *poller);

      /**
       * @return the name of this type
       */
      virtual const char* getTypeName () const;

      /**
       * check returned messages and update the buffers. 
       * @return true on success, false otherwise.
       */
      virtual bool processMessages() const;
      
      /**
       * send inquiry commands to the device.
       * @return true on success, false otherwise.
       */
      virtual bool inquiry() const;

      private:
      mutable poll *m_poller;
    };

    /*-----------------------------------------
     * Parameters
     *----------------------------------------*/
  public:
 
    //ToDo: Add a parameter which determines the poller (when there is a choice)
    //      The poller may only be changed if 1 registration (1 sony)
    //      otherwise copied instanced do not have a poller anymore
    /**
     * the parameters for the class sonyEviD100P
     *
     * CAVEAUTS: Continuous movement to the newest pan tilt position only works
     *           in absolute mode. When moving in relative coordinates it is hard to
     *           figure out, where the camera is moving too, i.e. which command is
     *           executed. This restriction only applies if a new pan-tilt-command
     *           is issued before the old pan-tilt-command is completed.
     */
    class parameters : public camera::parameters, public panTiltUnit::parameters {
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
       * The possible poll-modes: Manual (the user is responsible for polling
       * the device). Further modes could be added, e.g. asynchronous polling,
       * where the class splits off a seperate poll-thread which updates its
       * internal buffers a regular basis, or a synchronous behavior, where each
       * command blocks till its success.
       */
      enum ePollerType { ManualPoll };
 
      /**
       * the type of requested polling behavior.
       */
      ePollerType poller;

      /**
       * Turn on/off polling for lens values. This slows down the pan/tilt
       * position rate. If true lens system inquiries will be send. Default: false.
       */
      bool pollLens;

      /**
       * Poll for pan tilt positions. Whether this slows down the lens system is not
       * tested yet. If true pan tilt inquiries will be send. Default: true.
       */
      bool pollPanTilt;
      
      /**
       * the time in ms which the constructor waits for feedback from the device.
       * If this time intervall is too short, the feedback will be received later
       * by the method processMessages(). Commands sent to the camera might return
       * an error, though, due to missing initialization information.
       * Default: 200 ms.
       */
      int waitForInit;

      //---------------
      // pan tilt unit
      //---------------

      /**
       * The pan speed with which the pan Tilt unit shall move in degrees/s.
       * Default: max. speed.
       */
      float panSpeed;

      /**
       * the minimum pan speed, i.e. 2 degrees/s
       */
      static const float minPanSpeed;

      /**
       * the maximum pan speed, i.e. 300 degrees/s
       */
      static const float maxPanSpeed;

      /**
       * The tilt speed with which the pan tilt unit shall move in degrees/s.
       * Default: max. speed.
       */
      float tiltSpeed;
  
      /**
       * the minimum tilt speed, i.e. 2 degrees/s 
       */
      static const float minTiltSpeed;    

      /**
       * the maximum tilt speed, i.e. 125 degrees/s 
       */
      static const float maxTiltSpeed;

      //--------
      // camera
      //--------
      
      /**
       * the white balance modes supported by the sony Evi D100:
       * auto tracing (3000 - 7500K), extended auto tracing (2000 - 10000 K),
       * indoor (3200 K), outdor (5800 K), triggering one push and manual.
       */
      enum eWhiteBalanceType { Auto, ExtendedAuto, Indoor, Outdoor, OnePush, Manual };

      /**
       * a specialisation of auto or manual white balance. Thereby Auto,
       * ExtendedAuto, Indoor and Outdoor are considered as automatic modes,
       * i.e the autoWhiteBalance parameter is true. Whereas OnePush and
       * Manual require the autoWhiteBalance to be false. On conflicts between
       * the two parameters the boolean autoWhiteBalance has higher priority,
       * and the whiteBalanceMode will be changed to Auto or Manual respectively.
       * Default: Auto.
       */
      eWhiteBalanceType whiteBalanceMode;

      /**
       * If true the iris will be set automatically, otherwise manual controll is
       * enabled. Default: true.
       */
      bool autoIris;

      /**
       * the possible iris openings.
       */
      enum eIrisType { Close, F28, F22, F19, F16, F14, F11, F9_6, F8, F6_8, F5_6,
		       F4_8, F4, F3_4, F2_8, F2_4, F2, F1_8 };
      
      /**
       * the desired iris setting if in manual mode. Default: F11.
       */
      eIrisType iris;

      /**
       * If true, both gain and iris are adjusted using an internal algorithm,
       * according to a brighness level set by the user. The brightness mode is
       * considered a special combined automatic iris and white balance mode.
       * Thus the parameters autoWhiteBalance and autoIris must be true. Otherwise
       * this flag has no effect. Default: false.
       */
      bool enableBrightness;

      /**
       * The brightness level ranging from 0 to 23. Default: 6
       */
      int brightness;

      /**
       * the maximum brightness level, i.e. 23.
       */
      static const int maxBrightness;

      /**
       * Value of the exposure compensation. Exposure compensation is a function
       * which offsets the internal reference brightness level by steps of 1.5 dB.
       * Default: 0
       */
      int exposureCompensation;

      /**
       * the minimum exposure compensation, i.e. -7.
       */
      static const int minExposureCompensation;

      /**
       * the maximum exposure compensation, i.e. 7.
       */
      static const int maxExposureCompensation;

      /**
       * If the background is too bright or too dark when shooting in automatic
       * exposure mode, backlight compensation will make the subject appear clearer.
       * If true backlight compensation is enabled.
       */
      bool enableBackLightCompensation;

      /**
       * Aperture controll is a function which adjusts the enhancement of edges
       * of objects in the picture. Ranges from 0 (no enhancement) to 15. Default: 5.
       */
      int aperture;

      /**
       * the maximum aperture enhancement, i.e. 15
       */
      static const int maxAperture;

      /**
       * The sony allows several picture effects:
       * Pastel (gradient in the first band and UV in the second and third),
       * NegArt (negative/positive reversal),
       * Sepia, Monochrome,
       * Solarize (simple mean shift algorithm),
       * Mosaic (coarse resolution),
       * Slim (vertical stretch),
       * Stretch (horizontal stretch),
       * and Color.
       */
      enum ePictureEffectType { Pastel, NegArt, Sepia, Monochrome, Solarize, Mosaic,
				Slim, Stretch, Color };

      /**
       * the picture effect, see above. Default: Color.
       */
      ePictureEffectType pictureEffect;

      /**
       * The sony allows some digital effects, which are executed via the field memory.
       * Still: moving image on still image,
       * Flash: Continuous still image,
       * Lumi: Moving images on a binaried still image,
       * Trail: Afterimage lag of moving subject,
       * None: no digital effect.
       */
      enum eDigitalEffectType { Still, Flash, Lumi, Trail, None };

      /**
       * the digital effect, see above. Default: None.
       */
      eDigitalEffectType digitalEffect;

      /**
       * Digital effect level: 0 to 24 (flash, trail) and 0 t0 32 (still, lumi). 
       * Default: 0.
       */
      int digitalEffectLevel;

      /**
       * The maximum level of the still effect;
       */
      static const int maxStillLevel;

      /**
       * The maximum level of the flash effect;
       */
      static const int maxFlashLevel; 
      
      /**
       * The maximum level of the lumi effect;
       */
      static const int maxLumiLevel;   
      
      /**
       * The maximum level of the trail effect;
       */
      static const int maxTrailLevel;
 
      /**
       * If true the auto focus has a high sensitiviy, otherwise the sensitivity is low.
       * Default: 0.
       */
      bool highAutoFocusSensitivity;

      /**
       * the focus near limit in meter. Default: 0.
       */
      float focusNearLimit;

      /**
       * the focus speed. Default: max. speed
       */
      int focusSpeed;

      /**
       * the mininum focus speed, i.e. 0
       */
      static const int minFocusSpeed;

      /**
       * the maximum focus speed, i.e. 7
       */
      static const int maxFocusSpeed;

      /**
       * the maximum optical zoom, i.e. 10
       */
      static const float maxOpticalZoom;

      /**
       * Enable or disable the digital zoom.
       */
      bool digitalZoom;

      /**
       * the speed of the zoom. Default: max. speed
       */
      int zoomSpeed;

      /**
       * the minimum zoom speed, i.e. 0
       */
      static const float minZoomSpeed;

      /**
       * the maximum zoom speed, i.e. 7
       */
      static const float maxZoomSpeed;
 
    };

    /**
     * default constructor
     */
    sonyEviD100P();

    /**
     * Construct a functor using the given parameters
     */
    sonyEviD100P(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    sonyEviD100P(const sonyEviD100P& other);

    /**
     * destructor
     */
    virtual ~sonyEviD100P();

    /**
     * returns the name of this type ("sonyEviD100P")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sonyEviD100P& copy(const sonyEviD100P& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sonyEviD100P& operator=(const sonyEviD100P& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set the parameters and inform the hardware about changes
     * @param par the parameters to be used
     * @return true if successful, false otherwise
     */
    virtual bool setParameters(const functor::parameters& par);

    //---------------------
    // the pan tilt methods
    //---------------------

    /**
     * Reset the pan tilt device. This method resets internal pan and tilt
     * registers.  Some devices need resets from time to time to avoid
     * accumulation errors.  After succesfull execution the pan tilt unit
     * should be in the same position as before the call. During execution the
     * pan tilt unit may move.
     *
     * @return true on success
     */
    virtual bool reset();

    /**
     * stop the movement
     * @return true on success
     */
    virtual bool stopPanTilt();

    /** 
     * set the pan and tilt position. Usually pan and tilt are sent to
     * the device in a single package. The pan tilt moves with maximum
     * speed.
     * @param pan the pan position the device will move to
     * @param tilt the pan position the device will move to
     * @return true on success
     */
    virtual bool setPanTilt(float pan, float tilt);

    /**
     * set the pan and tilt position including pan and tilt speeds.
     * @param pan the pan position the device will move to
     * @param tilt the pan position the device will move to
     * @param panSpeed the speed in pan direction
     * @param tiltSpeed the speed in tilt direction
     * @return true on success
     */
    virtual bool setPanTilt(float pan, float tilt,
			    float panSpeed, float tiltSpeed); 

    /** 
     * set the pan position
     * @param pan the pan position the device will move to
     * @return true on success
     */
    virtual bool setPan(float pan);

    /** 
     * set the tilt position
     * @param tilt the tilt position the device will move to
     * @return true on success
     */
    virtual bool setTilt(float tilt);

    /**
     * get the current pan and tilt position. The combined
     * get-methods guarantee pan and tilt positions of the
     * same time instant.
     * @param pan the pan position which the device is in
     * @param tilt the tilt position which the device is in
     * @return true on success
     */
    virtual bool getPanTilt(float& pan, float& tilt) const;

    /**
     * @return the current pan position
     */
    virtual float getPan() const;

    /**
     * @return the current tilt position
     */
    virtual float getTilt() const;

    //-------------------
    // the camera methods
    //-------------------

    /**
     * turn on automatic exposure (AE) mode.
     * The iris and gain are set automatically, the
     * shutter speed is fixed to 1/50 s. Disables brightness.
     * @return true on success
     */
    virtual bool autoExposureOn();
    
    /**
     * turn automatic gain and iris control. The shutter will be
     * set manually. Disables brightness.
     * @return true on success
     */
    virtual bool shutterPriorityOn();

    /**
     * turn on manual gain, shutter and iris. Disables brightness.
     * @return true on success
     */
    virtual bool manualExposureOn();

    /**
     * turn automatic shutter and gain control. The iris will be
     * set manually. Disables brightness.
     * @return true on success
     */
    virtual bool irisPriorityOn();

    /**
     * turn automatic shutter and iris control. The gain will be
     * set manually. Disables brightness.
     * @return true on success
     */
    virtual bool gainPriorityOn();

    /**
     * turn on automatic shutter only. The gain and iris will
     * be set manually. Disables brightness.
     * @return true on success
     */
    virtual bool onlyAutoShutterOn();

    /**
     * turn on automatic gain only. The shutter and iris will
     * be set manually. Disables brightness.
     * @return true on success
     */
    virtual bool onlyAutoGainOn();

    /**
     * turn on automatic iris only. The shutter and gain will
     * be set manually. Disables brightness.
     * @return true on success
     */
    virtual bool onlyAutoIrisOn();

    //----------
    // WB + gain
    //----------

    /**
     * Trigger the white balance one push, assuming that a white
     * object occupies at least half the image. The value will be
     * fixed till the next trigger. This is a special manual white
     * balance mode, i.e. the autoWhiteBalance-parameter is false.
     * @return true on success
     */
    virtual bool triggerWhiteBalance();
  
    /**
     * The red and blue gain values will be determined
     * on call of the method triggerWhiteBalance().
     * @return true on success
     */
    virtual bool onePushWhiteBalanceOn();
  
    /**
     * Extend the auto white balance tracing to 2000 to 10000 K.
     * This is a special auto white balance mode.
     * @return true on success
     */
    virtual bool extendedAutoWhiteBalanceOn();

    /**
     * Fix the white balance to 3200 K.
     * This is a special auto white balance mode.
     * @return true on success
     */
    virtual bool outdoorWhiteBalanceOn();

    /**
     * Fix the white balance to 5800 K.
     * This is a special auto white balance mode.
     * @return true on success
     */
    virtual bool indoorWhiteBalanceOn();

    /**
     * turn on automatic white balance (3000 to 7500 K)
     * @return true on success
     */
    virtual bool autoWhiteBalanceOn();

    /**
     * turn on manual white balance
     * @return true on success
     */
    virtual bool manualWhiteBalanceOn();  

    /**
     * turn on automatic gain. Disables brightness.
     * @return true on success
     */
    virtual bool autoGainOn();

    /**
     * set the global gain. Turns off automatic gain if the
     * according parameter is true.
     * @param value the gain value
     * @return true on success
     */
    virtual bool setGain(float value);

    /**
     * set the red gain. Switched to manual white balance unless
     * the parameters already indicate that state.
     * @param value the red gain value
     * @return true on success
     */
    virtual bool setRedGain(float value);

    /**
     * set the blue gain. Switched to manual white balance unless
     * the parameters already indicate that state.
     * @param value the blue gain value
     * @return true on success
     */
    virtual bool setBlueGain(float value);

    //---------------
    // shutter speed
    //---------------
    /**
     * turn on automatic choice of the shutter speed.
     * The shutter changes automatically depending onf the brightness of
     * the subject. Disables brightness.
     * @return true on success
     */
    virtual bool autoShutterSpeedOn();

    /**
     * set the shutter speed. Turns off automatic shutter if the according
     * parameter is true.
     * @param value the shutter speed
     * @return true on success
     */
    virtual bool setShutterSpeed(float value);

    //-------
    // iris
    //-------
    /**
     * turn on automatic choice of the iris. Disables brightness.
     * @return true on success
     */
    virtual bool autoIrisOn();

    /**
     * set the iris. Turns off automatic iris if the according
     * parameter is true.
     * @param value the iris setting
     * @return true on success
     */
    virtual bool setIris(parameters::eIrisType value);

    //-----------------
    // brightness & co
    //-----------------
    /**
     * Turn of the brightness mode. The bright controll function adjusts both gain
     * and iris depending on the brightness level. It only works if automatic
     * iris and gain are turned on. Otherwise false is returned.
     * @return true on success
     */
    virtual bool brightnessOn();

    /**
     * Set the brightness level. Enables the brightness mode if its
     * parameter is turned off.
     * Turns on auto white balance and auto iris.
     * @param value the brightness value
     * @return true on success
     */
    virtual bool setBrightness(int value);

    /**
     * set an offset to the internal reference brightness level
     * @param value the degree of exposure compensation
     * @return true on success
     */    
    virtual bool setExposureCompensation(int value);

    //-----------------
    // special effects
    //-----------------
    
    /**
     * If the background is too bright or too dark when shooting in automatic
     * exposure mode, back light compensation will make the subject appear clearer.
     * @return true on success
     */
    virtual bool backLightCompensationOn();
    
    /**
     * Turn off the back light compensation
     * @return true on success
     */ 
    virtual bool backLightCompensationOff();

    /**
     * Aperture controll is a function which adjusts the enhancement of edges
     * of objects in the picture. Ranges from 0 (no enhancement) to 15.
     * param value the aperture
     * @return true on success
     */
    virtual bool setAperture(int value);
   
    /**
     * set the picture effect, see parameters documentation for details
     * param effect the picture effect
     * @return true on success
     */
    virtual bool setPictureEffect(parameters::ePictureEffectType effect);

    /**
     * set the digital effect, see parameters documentation for details
     * param effect the digital effect
     * @return true on success
     */  
    virtual bool setDigitalEffect(parameters::eDigitalEffectType effect);

    /**
     * Digital effect level: 00 (flash) to 24 (trail) and 00 (still) t0 32
     * (lumi). 
     * param value the level of the digital effect
     * @return true on success
     */
    virtual bool setDigitalEffectLevel(int value);  

    //-------
    // focus
    //-------
    /**
     * Enable the highly sensible auto focus.
     * @return true on success
     */
    virtual bool highAutoFocusSensibilityOn();

    /**
     * Enable the low sensible auto focus.
     * @return true on success
     */
    virtual bool lowAutoFocusSensibilityOn(); 

    /**
     * set the focus. Turns off autofocus if the
     * parameters indicate that it is on.
     * @param value the focus value
     * @return true on success
     */
    virtual bool setFocus(float value);

    /**
     * Set the focus to its maximum value.
     *
     * Turns off autofocus if the parameters indicate that it is on.
     *
     * @param speed the focusing speed
     *
     * @return true on success
     */
    virtual bool setFarFocus(int speed = 7);

    /**
     * Set the focus to its minimum value.
     *
     * Turns off autofocus if the parameters indicate that it is on.
     *
     * @param speed the focusing speed
     * @return true on success
     */
    virtual bool setNearFocus(int speed = 7);

    /**
     * Stop the focus at its current position. 
     *
     * The focus position in the parameter set is not updated, because
     * the actual focus position is unknown yet due to the delay of
     * the inquiries and their answers.  Turns off autofocus if the
     * parameters indicate that it is on.
     *
     * @return true on success
     */
    virtual bool stopFocus();  

    /**
     * Set the focus to infiniy.
     *
     * Turns off autofocus if the parameters indicate that it is on.
     *
     * @return true on success
     */
    virtual bool setFocusToInfinity();
    
    /**
     * Turn on manual focus
     *
     * @return true on success
     */
    virtual bool manualFocusOn();
    
    /**
     * Turn on auto focus
     *
     * @return true on success
     */
    virtual bool autoFocusOn(); 

    /**
     * Trigger the focus one push.
     *
     * Turns off auto focus.  The actual focus value is unknown yet
     * and thus the parameter's focus is not updated.
     *
     * @return true on success
     */
    virtual bool triggerFocus();
    
    /**
     * @return the current focus
     */
    virtual float getFocus() const;

    //---------
    // zoom
    //---------

    /**
     * Stop the zoom at its current position. The zoom
     * position in the parameter set is not updated, because
     * the actual zoom position is unknown yet due to the delay
     * of the inquiries and their answers.
     * @return true on success
     */
    virtual bool stopZoom();

    /**
     * set the zoom to its minimum value.
     * @param speed the zooming speed
     * @return true on success
     */
    virtual bool setWideZoom(int speed = 7);

    /**
     * set the zoom to its maximum optical value.
     * @param speed the zooming speed
     * @return true on success
     */    
    virtual bool setTeleZoom(int speed = 7);

    /*
     * Turn on the digital zoom.
     * @return true on success
     */    
    virtual bool digitalZoomOn();

   /*
     * Turn off the digital zoom.
     * @return true on success
     */    
    virtual bool digitalZoomOff();

    /**
     * set the zoom. Returns true on success. If the digital zoom is
     * disabled only values up to 10 are allowed.
     * Otherwise a 4 times digital zoom is combined with the 10 times
     * optical zoom resulting in a maximal possible zoom factor of 40.
     * @param value the zoom value
     * @return true on success
     */
    virtual bool setZoom(float value);

    /**
     * @return the current zoom
     */
    virtual float getZoom() const;

    /**
     * Get the focus and zoom values from a single time instant.
     * @param focus the current focus value
     * @param zoom the current zoom value
     * @return true on success
     */
    virtual bool getLensSetting(float& focus, float& zoom) const;

    //-------------------
    // poll/trigger stuff
    //-------------------

    /**
     * Get a reference to the poller. This may be usefull
     * if the user wants to send and process inquiries himself.
     * return a reference to the extern poller
     */
    virtual const externPoll& getPoller();

    /**
     * Check wether the camera was initialized completely on construction.
     * A complete initialization requires feedback from the hardware. Because
     * this driver only waits a limited amount of time for the feedback, it
     * does not always perform a complete initialization. In this case
     * the hardware still works, but the desired parameters have not been
     * sent to the hardware yet, e.g. the camera might not have the expected
     * zoom. It is up to the user to recall setParameters() after an incomplete
     * initialization.
     */
    virtual bool completeInit();

    /**
     * Turn on polling the lens system.
     * @return true on success
     */
    virtual bool pollLensOn();
      
    /**
     * Turn off polling the lens system.
     * @return true on success
     */
    virtual bool pollLensOff();

    /**
     * Turn on polling the pan tilt positions.
     * @return true on success
     */
    virtual bool pollPanTiltOn();

    /**
     * Turn off polling the pan tilt positions.
     * @return true on success
     */
    virtual bool pollPanTiltOff();
    
  protected:
    /** get a pointer to the parameters */
    parameters& getWritableParameters();

    /** initialize device */
    bool init();

    /** initialize Set*Position command string */
    bool initPosCommand(vector<ubyte>& command, float pan, float tilt, 
			float panSpeed, float tiltSpeed);

    /** Convert focus from distance in m to internal value */
    int distance2Focus (float& distance) const;

    /** Convert focus from internal value to distance in m */
    float focus2Distance (int focus) const;

    /** the poller, which is not static because we may have different serial ports
     * and thus different cameras */
    poll *m_poller;
    /** the user access to the poller whith limited operations */
    externPoll m_externPoller;

    /** false if the constructor did not have enough time to init the camera */
    bool m_completeInit;

    /** for conversion between degrees/radiant and sony-format */
    static const float m_tiltFactor;
    static const float m_panFactor;
  };
}

#endif

