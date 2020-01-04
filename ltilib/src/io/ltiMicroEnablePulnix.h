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
 * file .......: ltiMicroEnablePulnix.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 13.08.99
 * revisions ..: $Id: ltiMicroEnablePulnix.h,v 1.11 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_MICRO_ENABLE_H_
#define _LTI_MICRO_ENABLE_H_


#include "ltiFrameGrabber.h"
#include "ltiSemaphore.h"
#include "ltiSerial.h"

#ifdef _USE_MICRO_ENABLE_FRAME_GRABBER

// ------- Silicon Software Includes -------

#if __linux__

// this is required by silicon software os_type.h file:
#ifndef LINUX
#define LINUX
#endif

#endif

#include <menable.h>
#include <fgrab.h>

// ----------------------------

namespace lti {

  /**
   * Silicon Software channel link frame grabber (Linux version!).
   *
   * class for grabbing frames from cameras and converting them to lti::image
   * format.
   *
   * Don't forget to uncomment the line
   * "#define _USE_MICRO_ENABLE_FRAME_GRABBER" in the file ltiHardwareConfig.h
   * before compiling!
   *
   * For Linux it is important to use the correct extra-path variables in
   * the Makefile.  These variables point to the include files and
   * libraries required by the MicroEnable hardware.  The variables are:
   * \code
   * EXTRAINCLUDEPATH = -I/usr/src/menable/include
   * EXTRALIBPATH = -L/usr/src/menable/lib
   * EXTRALIBS =  -lpulnixchanneltmc6700 -lmenable
   * \endcode
   *
   * where you should replace <code>/usr/src/menable</code> with the correct
   * path to your MicroEnable directory.
   *
   * Errors:
   * The error 2080 usually means that the variable HAP_PATH has not been
   * set yet!
   *
   */
  class microEnablePulnix : public frameGrabber {
  public:

    class bankType;

    /**
     * frameGrabber parameters class
     */
    class parameters : public frameGrabber::parameters {
      friend class microEnablePulnix;
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters( const parameters& other );

      /**
       * destructor
       */
      ~parameters();

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

      /**
       * read the parameters from the given ioHandler
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

      /**
       * read the parameters from the camera,
       * assuming that the serial port of the pulnix camera is
       * connected at the given serial port.  This will take a while!
       * You can choose if you want to read all parameter sets or the
       * current bank only.
       *
       * To send the parameters to the camera use the method setParameters()
       * of the microEnablePulnix functor.
       *
       * @param serialPort the serial port used to communicate with
       * the camera.  (Com1, Com2, ...)
       * @param allBanks if true, all parameter banks will be read.  Otherwise
       *                 only the current bank will be read.
       * @return true if successful, false otherwise
       */
      virtual bool download(const serial::parameters::ePortType& serialPort,
                            const bool& allBanks = true);


      // ------------- The Parameters ---------------

      /**
       * Which serial port should be used to communicate with the camera?
       */
      serial::parameters::ePortType serialPort;

      /**
       * The four parameter banks
       */
      bankType* bank;

      /**
       * The parameter-bank number to be used.
       * This must be a value between 0 and 3.  Only the last two bits of
       * the given value will be considered.
       * Default value: 3
       */
      int currentBank;

      /**
       * Amount of memory in bytes to be allocated on the host to
       * communicate with the grabber (Default 8MB=8*1024*1024).
       *
       * This parameter is consider just once, at the initialization.
       * After that, it is not possible to change the buffer size.
       */
      int bufferSize;

      /**
       * dimensions of the grabbed window (default 640x480)
       */
      point windowSize;

      /**
       * offset of the grabbed window (default 104x40)
       */
      point windowOffset;

      /**
       * Possible shutter modes
       */
      enum eShutterMode {
        SMUninitialized = -1, /**< uninitialized shutter mode */
        DirectM, /**< Direct shutter specification for the manual mode.
                  *   The shutter value to be used is specified in the
                  *   parameter <code>shutterSpeed</code>
                  */
        DirectA, /**< Direct shutter specification for the asynchronous mode.
                  *   The shutter value to be used is specified in the
                  *   parameter <code>shutterSpeed</code>
                  */
        Manual,  /**< Use one of the shutter values specified in the
                  *   manual shutter table.  The entry in that table is
                  *   given in the parameter <code>shutterEntry</code>.
                  */
        Async,   /**< Use one of the shutter values specified in the
                  *   async shutter table.  The entry in that table is
                  *   given in the parameter <code>shutterEntry</code>.
                  */
      };

      /**
       * modes for edge enhancement
       */
      enum eEdgeEnhancement {
        EEUninitialized=-1, /**< uninitialized
                             */
        Off=0x00,           /**< No edge enhancement
                             */
        Horizontal=0x04,    /**< Horizontal edge enhancement
                             */
        Vertical=0x08,      /**< Vertical edge enhancement
                             */
        Both=0x0C           /**< Horizontal and vertical edge enhancement
                             */
      };

      /**
       * white balance mode
       */
      enum eWhiteBalanceType {
        WBUninitialized=-1, /**< uninitialized
                             */
        WBManual, /**< Manual mode.  You need this mode to adjust the
                   *   blue and red gains.
                   *   In the Pulnix documentation, this mode is
                   *   refered as "External" mode
                   */
        WBAuto    /**< Automatic White Balance.
                   *   In the Pulnix documentation, this mode is
                   *   refered as "Internal" mode
                   */
      };

    protected:

      /**
       * send the parameters to the camera,
       * assuming that the serial port of the pulnix camera is
       * connected to the given serial port.  This will upload only
       * those parameter that have changed!
       * You can choose if you want to send only the current bank (default) or
       * if you want to send all parameters banks.
       * @param serialPort the serial port used to communicate with
       * the camera.  (Com1, Com2, ...)
       * @param allBanks if true, all parameters sets will be send to the
       *                 camera.  If false, only the current bank will be send.
       * @param force if true, everything will be senden, ignoring the
       *              content of the "camera shadow"
       * @return true if successful, false otherwise
       */
      virtual bool upload(const serial::parameters::ePortType& serialPort,
                          const bool allBanks = false,
                          const bool force = false) const;

      /**
       * dimensions of the grabbed window
       *
       * These are the real values in the camera/frame grabber, used
       * to detect if the table needs to be resend.
       */
      static point camWindowSize;

      /**
       * offset of the grabbed window
       *
       * These are the real values in the camera/frame grabber, used
       * to detect if the table needs to be resend.
       */
      static point camWindowOffset;

      /**
       * current bank in the camera
       */
      static int camCurrentBank;
    };

    /**
     * The Pulnix Camera TMC6700 can handle up to four parameters sets.
     * This class provides the information for one bank.  The parameter class
     * contains four of this banks.
     */
    class bankType : public functor::parameters {
    public:
      /**
       * default constructor.
       */
      bankType();

      /**
       * copy constructor.
       *
       * This constructor copies everything (including the bankNumber).
       */
      bankType( const bankType& other );

      /**
       * destructor
       */
      virtual ~bankType();

      /**
       * Set the bank identification number for this parameter set
       */
      void setBankNumber(const int& bank);

      /**
       * Get the bank identification number for this parameter set
       */
      const int& getBankNumber() const;

      /**
       * copy member.
       *
       * Please note that the bankNumber will NOT be copied.  This allows
       * to copy one bank into another without needing to reset the bank
       * number.
       */
      bankType& copy( const bankType& other );

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns the type name
       */
      virtual const char* getTypeName() const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in a parameter-bank of the camera,
       * assuming that the serial port of the pulnix camera is
       * connected to the given serial port.  This will upload only
       * those parameter that have changed!
       * @param serialPort the serial port used to communicate with
       *                   the camera.  (Com1, Com2, ...)
       * @param force flag to indicate if everything needs to be send (true)
       *        or if only the changed stuff needs to be send (false)
       * @return true if successful, false otherwise
       */
      virtual bool
      upload(const serial::parameters::ePortType& serialPort,
             const bool force = false) const;

      /**
       * activate this bank in the camera
       * @param serialPort the serial port used to communicate with
       * the camera.  (Com1, Com2, ...)
       * @return true if successful, false otherwise
       */
      virtual bool
      activate(const serial::parameters::ePortType& serialPort) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

      /**
       * read the parameters from the parameter-bank in the camera,
       * assuming that the serial port of the pulnix camera is
       * connected at the given serial port.  This will take a while!
       * @param serialPort the serial port used to communicate with
       * the camera.  (Com1, Com2, ...)
       * @return true if successful, false otherwise
       */
      virtual bool download(const serial::parameters::ePortType& serialPort);

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

      /**
       * return the "real" values in the camera (or values out of range
       * if the information is not available).
       */
      const bankType& getCameraValues() const;

      // ------------- The Parameters ---------------

      /**
       * gamma correction (valid values between 0.0 and 1.0)
       * Only two values are understand by the Pulnix camera: 0.45f and 1.0f.
       * The given value will be rounded to one of these two valid values.
       *
       * Default value: 0.45f
       */
      float gamma;

      /**
       * gain
       * valid values between 0 and 1.0
       *
       * Default value: 0.3569f
       */
      float gain;

      /**
       * Brightness.
       * These "brightness" value is achieved using the
       * "advanced DSP gain".  Valid values are from -1.0 to 1.0
       * Default value 0.0;
       */
      float brightness;

      /**
       * shutter speed.
       * Only used if the shutterMode is DirectM or DirectA.  The value
       * has no time units.  It just means a long exposure time (1.0)
       * or a very short exposure time (0.0).
       *
       * HARDWARE NOTES: The values 0.0 - 1.0 will be mapped to the
       * from hardware expected values 1024-525 respectively.  If
       * other hardware values are required you can give them directly
       * here.  I.e. values between 0.0 and 1.0 (inclusive) will be
       * mapped to the given hardware values.  Values greater or equal
       * 2.0 and less than 65536.0 will be directly sended to the
       * camera.  This behaviour is also applied for the values in the
       * shutter tables.
       *
       * Default value: 0.2557f */
      float shutterSpeed;

      /**
       * shutter mode.
       * Specifies which shutter mode should be used (@see eShutterMode)
       * Default: DirectM
       */
      ::lti::microEnablePulnix::parameters::eShutterMode shutterMode;

      /**
       * specify which entry of the manual or async shutter tables should
       * be used (for the Manual or Async shutter modes).
       * The value must be between 0 and 9.
       * Default: 0;
       */
      int shutterEntry;

      /**
       * color matrix.  This 3x3 matrix is used internally by the
       * Pulnix camera to transform the RGB values readed from the CCD
       * chip into another "color space".  The values of the elements
       * must be between -2.96875 and 3.96875.  An extra condition is
       * that the sum of the elements of one row should add to one.
       * Default value: identity matrix */
      fmatrix colorMatrix;

      /**
       * edgeEnhancement mode
       * Default value: off
       */
      ::lti::microEnablePulnix::parameters::eEdgeEnhancement
          edgeEnhancementMode;

      /**
       * level of edge enhancement.  It must be 1 or 2.
       * Default: Level 1
       */
      int edgeEnhancementLevel;


      /**
       * white balance
       *
       * Default value: Manual
       */
      ::lti::microEnablePulnix::parameters::eWhiteBalanceType whiteBalance;

      /**
       * Red gain.
       *
       * To see the effect of this gain factor you need to set the white
       * balance mode into "Manual".
       *
       * Valid values between  -1.0 and 1.0
       *
       * Default value: -0.1732
       */
      float redGain;

      /**
       * Blue gain
       *
       * Valid values between -1.0 and 1.0
       *
       * To see the effect of this gain factor you need to set the white
       * balance mode into "Manual".
       *
       * Default value: -0.2913
       */
      float blueGain;

      /**
       * offset for the analog to digital converter (ADC)
       * Valid values: 200-250
       * Default value: 233
       */
      int adcOffset;

      /**
       * reference top value for the analog to digital converter (ADC)
       * Valid values: 0-255
       * Default value: 225
       */
      int adcReferenceTop;

      /**
       * offset for each color channel.
       * The value for each color must be between -128..127
       */
      trgbPixel<int> rgbOffset;

      /**
       * The Manual Shutter Table contains 10 entries, which can be
       * used in the shutter Manual Mode giving an entry in
       * shutterEntry field of the respective bank.  Each entry of the
       * table contains a value between 0.0 and 1.0. The values have
       * no time units.  They just means a long exposure time (1.0) or
       * a very short exposure time (0.0).  This vector must have 10
       * elements
       */
      fvector manualShutterTable;

      /**
       * The Async Shutter Table contains 10 entries, which can be
       * used in the shutter Asynchronous Mode giving an entry in
       * shutterEntry field of the respective bank.  Each entry of the
       * table contains a value between 0.0 and 1.0. The values have
       * no time units.  They just means a long exposure time (1.0) or
       * a very short exposure time (0.0).  This vector must have 10
       * elements
       */
      fvector asyncShutterTable;

    protected:
      /**
       * serial parameters.
       *
       * The Pulnix Camera expects 9600baud, no parity, 1 stop bit and
       * 8 bits per char.
       */
      mutable serial::parameters serialParameters;

      /**
       * protected initializer to create an "uninitialized" bank object
       */
      void uninitialize();

      /**
       * the bank number in the camera, which is represented by this bank
       * Valid numbers from 0 to 3
       */
      int bankNumber;

      /**
       * this bank contains the values written in the camera, or out-of-range
       * values if they are not read/written yet.
       */
      mutable bankType* cameraValues;

      /**
       * LUT for bit reversion
       */
      static const int* bitReversionLUT;

      /**
       * initialize LUT for bit reversion
       */
      void initBitReversionLUT();

      /**
       * revert bits
       */
      const int& revert(const ubyte& b) const;

    private:
      static bankType* allCameraValues;
    };

    /**
     * default constructor
     */
    microEnablePulnix(const bool& initialize=true);

    /**
     * copy constructor
     */
    microEnablePulnix(const microEnablePulnix& other);

    /**
     * constructor, sets the parameters
     */
    microEnablePulnix(const parameters& theParam);

    /**
     * destructor
     */
    virtual ~microEnablePulnix( void );

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
    virtual bool apply(image& theImage);

    /**
     * load a grey value channel from the
     */
    virtual bool apply(channel8& theChannel);

    /**
     * copy member
     */
    microEnablePulnix& copy(const microEnablePulnix& other);

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
    bool init();

    /**
     * set functor's parameters.  This will set the frame grabber parameters
     * and will send the wished configuration to the camera.
     *
     * You can also load the actual parameter sets in the camera with
     * the method of the parameters "download".
     *
     * @return true if successful, false otherwise.
     */
    virtual bool setParameters(const functor::parameters& theParam);

  protected:
    /**
     * Return the error message reported by the microEnable hardware.
     */
    std::string getErrorMessage();

    /**
     * the initialized frame grabber object
     */
    static Fg_Info* fg;

    /**
     * memory block reserved for frame-grabber communication
     */
    static ubyte* frameBuffer;

    /**
     * contains the initialization status
     */
    static bool active;

    /**
     * number of existen instances of the grabber
     */
    static int instances;

    /**
     * allocate the buffer memory
     */
    bool allocMemory();

    /**
     * deallocate the buffer memory
     */
    bool freeMemory();

    /**
     * free the frame graber resource
     */
    bool freeGrabber();

    /**
     * activate hardware parameters
     */
    bool activateParameters();

    /**
     * get an image from the frame grabber
     */
    bool grab(image& theImage);

    /**
     * get a channel8 from the frame grabber
     */
    bool grab(channel8& theChannel);

    /**
     * convert the frame buffer into an image.
     * theImage MUST have the correct size when this function is called!
     */
    static void bufferToImage(image& theImage, const bool turnaround);

    /**
     * convert the frame buffer into a channel8
     * theChannel MUST have the correct size when this function is called!
     */
    static void bufferToChannel(channel8& theChannel, const bool turnaround);

    /**
     * the last grabbed image.
     */
    static image* lastImage;

  };
}

#endif // _USE_MICRO_ENABLE_FRAME_GRABBER
#endif // #ifdef _LTI_MICRO_ENABLE_H_

