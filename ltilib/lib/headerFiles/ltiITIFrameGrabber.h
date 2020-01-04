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
 * file .......: ltiFrameGrabber.h
 * authors ....: Peter Mathes
 * organization: LTI, RWTH Aachen
 * creation ...: 13.08.99
 * revisions ..: $Id: ltiITIFrameGrabber.h,v 1.12 2005/02/02 14:15:18 doerfler Exp $
 */

#ifndef _LTI_ITI_FRAMEGRABBER_H_
#define _LTI_ITI_FRAMEGRABBER_H_

#ifdef _USE_ITI_FRAME_GRABBER

#ifdef _LTI_MSC_6

#include "ltiFrameGrabber.h"

// ------- ITI includes -------

#include <itxcore.h>
#include <icp.h>
#include <ams.h>
#include <amc1.h>

// ----------------------------

namespace lti {

  /**
   * iti frame grabber class (Windows ONLY!).
   *
   * class for grabbing frames from cameras and converting them to lti::image
   * format.
   *
   * Don't forget to uncomment the line "#define _USE_ITI_FRAME_GRABBER" in
   * the file
   * ltiHardwareConfig.h before compiling!
   *
   * You must also include the file "itxco10.lib" in your settings (unter
   * <code>Project->Settings->Link->General->Library modules</code>)
   */
  class itiFrameGrabber : public frameGrabber {
  public:
    /**
     * frameGrabber parameters class
     */
    class parameters : public frameGrabber::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters( const parameters& other ) 
        : frameGrabber::parameters() {
        copy( other );
      };

      /**
       * copy member
       */
      parameters& copy( const parameters& other );

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns the type name
       */
      virtual const char* getTypeName() const;

      // ------------- The Parameters ---------------

      /**
       * path and filename of the grabber configuration file
       */
      std::string configFile;

      /**
       * image data transfer mode
       */
      enum eCameraMode {
        FullImageRectangular = 0, /*!< full image rectangular */
        FullImageSquare,          /*!< full image square */
        QuarterImageFiltered,     /*!< quarter image filtered */
        QuarterImageSampled       /*!< quarter image sampled */
      };

      /**
       * specify the appearance of the transfered image
       */
      eCameraMode mode;

      /**
       * voltage reference, negative, red, green and blue values
       */
      trgbPixel<double> referenceNegative;

      /**
       * voltage reference, positive, red, green and blue values
       */
      trgbPixel<double> referencePositive;

      /**
       * if true, a low pass filter (hardware) will be used
       */
      bool lowPassFilterOn;
    };

    /**
     * default constructor
     */
    itiFrameGrabber(const bool& initialize=true);

    /**
     * copy constructor
     */
    itiFrameGrabber(const itiFrameGrabber& other);

    /**
     * constructor, sets the parameters
     */
    itiFrameGrabber(const parameters& theParam);

    /**
     * destructor
     */
    virtual ~itiFrameGrabber( void );

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * load an (color) image from the grabber
     */
    virtual image& apply(image& theImage);

    /**
     * load a grey value channel from the
     */
    virtual channel8& apply(channel8& theChannel);

    /**
     * copy member
     */
    itiFrameGrabber& copy(const itiFrameGrabber& other);

    /**
     * clone member
     */
    functor* clone() const;

    /**
     * check if the frame grabber is active
     */
    bool isActive() const;

    /**
     * initialize frame grabber
     */
    void init();

    // error codes
    static const short NoError;
    static const short Error;
    static const short NoGrabber;


  protected:
    /**
     * true if the frame grabber has been successfully initialized
     */
    bool active;

    /** capture quarter image
        @return true if everything is ok, false if error
     */
    bool captureQuarter(image& theImage);

    /** capture the full image, and extract a square from it
        @return true if everything is ok, false if error
     */
    bool captureFullSquare(image& theImage);

    /** capture the full rectangular image
        @return true if everything is ok, false if error
     */
    bool captureFull(image& theImage);

    /** cut to square and decimate the buffer
        @return true if everything is ok, false if error
     */
    bool cutToSquareAndDecimate(image& theImage);

    /** take a picture!
        @return true if everything is ok, false if error
    */
    bool snap();

  private:
    /**
     * Abstract class for the hardware modules
     */
    class itiModule {
    public:
      virtual ~itiModule( void );

      virtual short setRGB( void ) = 0;
      virtual short setBGR( void ) = 0;
      virtual short setHSI( void );

      virtual short lowpassOn( void ) = 0;
      virtual short lowpassOff( void ) = 0;

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
      virtual short setRedRef( double neg, double pos ) = 0;

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
      virtual short setGreenRef( double neg, double pos ) = 0;

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
      virtual short setBlueRef( double neg, double pos ) = 0;

      virtual short getRedRef( double &neg, double &pos ) = 0;
      virtual short getGreenRef( double &neg, double &pos ) = 0;
      virtual short getBlueRef( double &neg, double &pos ) = 0;

      virtual short selectPort(int port) = 0;
      virtual short getPort(int &port) = 0;

      bool    isAMCLR( void );
      bool    isAMSTD( void );
    };

    /**
     * a newer model of the frame grabber card
     */
    class  itiAMStd : public itiModule {
    public:
      itiAMStd( MODCNF* theIcpmod, MODCNF* theModule );
      virtual ~itiAMStd( void );

	    virtual short setRGB( void );
	    virtual short setBGR( void );

	    virtual short lowpassOn( void );
	    virtual short lowpassOff( void );

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
	    virtual short setRedRef( double neg, double pos );

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
	    virtual short setGreenRef( double neg, double pos );

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
	    virtual short setBlueRef( double neg, double pos );

      /** get the voltage references values of the ADC (in Volt)
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
       */
      virtual short getRedRef( double &neg, double &pos );

      /** get the voltage references values of the ADC (in Volt)
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
       */
    	virtual short getGreenRef( double &neg, double &pos );

      /** get the voltage references values of the ADC (in Volt)
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
       */
    	virtual short getBlueRef( double &neg, double &pos );

	    virtual short selectPort(int port);
	    virtual short getPort(int &port);

    protected:
      MODCNF*     icpmod;
      MODCNF*     module;
      AMS_CAMCNF* camera;
    private:
      static const double maxVoltage;
      static const double minVoltage;
      static const double minDiffVoltage;
    };

    /**
     * An older model of the frame grabber card
     */
    class  itiAMClr : public itiModule {
    public:
      itiAMClr( MODCNF* theIcpmod, MODCNF* theModule );
      virtual ~itiAMClr( void );

	    virtual short setRGB( void );
	    virtual short setBGR( void );
      virtual short setHSI( void );

	    virtual short lowpassOn( void );
	    virtual short lowpassOff( void );

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
	    virtual short setRedRef( double neg, double pos );

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
	    virtual short setGreenRef( double neg, double pos );

      /** set reference value for the ADC (in Volt)
          0.0<=neg<=1.476 0.7<=pos<=1.97 pos>=neg
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
        */
	    virtual short setBlueRef( double neg, double pos );

      /** get the voltage references values of the ADC (in Volt)
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
       */
	    virtual short getRedRef( double &neg, double &pos );

      /** get the voltage references values of the ADC (in Volt)
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
       */
	    virtual short getGreenRef( double &neg, double &pos );

      /** get the voltage references values of the ADC (in Volt)
          @param neg negative reference voltage (in volt)
          @param pos positive reference voltage (in volt)
          @return returns a Error if something was wrong, NoError otherwise.
       */
      virtual short getBlueRef( double &neg, double &pos );

	    virtual short selectPort(int port);
	    virtual short getPort(int &port);

    protected:
      MODCNF* icpmod;
      MODCNF* module;
      AMC1_MATRIX *rgbmat, *hsimat, *bgrmat;
	    AMC1_CAMCNF *camera;
    };

    /**
     * generate an hardware interface module for the correct iti frame
     * grabber card.
     */
    class itiFactory {
    public:
      itiModule* generateModule( MODCNF* icpmod );
    };

    /**
     * buffer for a half image
     */
    char*       halfPict;


    MODCNF*     icpmod;

    /**
     * the module (the instance depends on the existent frame grabber)
     */
    itiModule*  module;

    /**
     * creates the module with the correct type
     */
    itiFactory  moduleFactory;

    /**
     * id of the grabbed frame
     */
    short frameID;

    uint16      xCam;
    uint16      yCam;

    uint32      firstFieldStart;
    uint32      secondFieldStart;
  };
}

#endif // _LTI_MSC_6
#endif // _USE_ITI_FRAME_GRABBER
#endif // #ifdef _LTI_ITI_FRAMEGRABBER_H_


