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
 * file .......: ltiFireWireDCAM.h
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 21.10.2004
 * revisions ..: $Id: ltiFireWireDCAM.h,v 1.5 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_FIRE_WIRE_D_C_A_M_H_
#define _LTI_FIRE_WIRE_D_C_A_M_H_

#ifdef _USE_FIRE_WIRE_DCAM

#include "ltiFrameGrabber.h"
#include <string>

#include <libraw1394/raw1394.h>
#include <libdc1394/dc1394_control.h>
#include "ltiMergeYUVToImage.h"
#include "ltiBayerDemosaicing.h"


namespace lti {
  /**
   *  Functor to interact with a standard FireWire DCAM (aka IIDC v1.3) camera
   *  via libdc1394 (linux).
   *
   *  System requirements:
   *    - linux kernel modules ieee1394, ohci1394, raw1394 and video1394
   *    - libdc1394 (>= 1.0.0)
   *    - libraw1394 (>= 0.9.0)
   *
   *  Installation:
   *    -# compile or install above kernel modules for your kernel
   *    -# create video devicefiles for video1394
   *       (e.g. /dev/video1394)
   *    -# compile or install libdc1394 and libraw1394
   *    -# edit extrapaths.mk in the linux directory of your ltilib 
   *       (example can be found in extrapaths_template.mk)
   *    -# compile ltilib
   *
   *  Usage:
   *    -# all kernel modules must be loaded in order to make the functor work
   *
   *  \b Note: invoking init() dumps the &parameters stored in the camera
   *     into the local parameters object. If you want to set custom 
   *     camera parameters (brightness, gain, etc.) you have to set them
   *     \b after a init() has been invoked.
   * 
   * Example:
   *  \code
   *    // viewer to display taken shot
   *    viewer view("FireWireDCAM");
   * 
   *    fireWireDCAM::parameters camPar;
   *    fireWireDCAM cam;
   *    // will be preserved by init()
   *    camPar.deviceFile = "/dev/video1394/0";
   *    camPar.oneShot = true;
   *
   *    // won't have any effect, as this is overwritten after init()
   *    camPar.brightness = 22;
   *
   *    // as camera is not yet initialized nothing is written into the camera 
   *    // itself
   *    cam.setParameters(camPar);
   *
   *    // initializes camera and dumps camera parameters into the parameters
   *    // object of the camera functor
   *    cam.init();
   *
   *    // get parameters out of camera functor
   *    camPar = cam.getParameters();
   * 
   *    // now the changes will take effect
   *    camPar.brightness = 22;
   *
   *    // as the camera is initialized now, the new parameters will be written
   *    // into the camera
   *    cam.setParameters(camPar);
   *    // take picture
   *    image img;
   *    cam.apply(img);
   *
   *    // show image
   *    view.show(img);
   *
   *  \endcode
   *
   *  Not yet implemented:
   *  - format7 (scalable image)
   *  - color modes: Mono16, RGB, YUV444
   *  - camera features: exposure, sharpness, hue, saturation, gamma
   *  - camera mechanical features: zoom, pan, tilt, focus, iris, 
   *    optical filter
   *
   *  Other things to do:
   *  - get feature constraints
   *  - possibly use own enum for features
   *  - it would be much better to use a wrapper class instead of including
   *    raw1394.h and dc1394_control.h directly (namespace)
   *  - rewrite conversion from YUV to RGB (with a choice to use interpolation)
   *  - better error handling (e.g. remember setup_capture failures etc..)
   *  - camera selection (node/port) to support multiple cameras / FW cards
   */
  class fireWireDCAM : public frameGrabber {
  public:
    /**
     * The parameters for the class fireWireDCAM
     */
    class parameters : public frameGrabber::parameters {
    public:
      

      /* Enumeration of camera modes */
      enum eMode {
        // modes in format0
        Mode_160x120_YUV444=64,
        Mode_320x240_YUV422,
        Mode_640x480_YUV411,
        Mode_640x480_YUV422,
        Mode_640x480_RGB,
        Mode_640x480_Mono,
        Mode_640x480_Mono16,

        // modes in format1
        Mode_800x600_YUV422= 96,
        Mode_800x600_RGB,
        Mode_800x600_Mono,
        Mode_1024x768_YUV422,
        Mode_1024x768_RGB,
        Mode_1024x768_Mono,
        Mode_800x600_Mono16,
        Mode_1024x768_Mono16,

        // Modes in format2
        Mode_1280x960_YUV422= 128,
        Mode_1280x960_RGB,
        Mode_1280x960_Mono,
        Mode_1600x1200_YUV422,
        Mode_1600x1200_RGB,
        Mode_1600x1200_Mono,
        Mode_1280x960_Mono16,
        Mode_1600x1200_Mono16,

        // modes for Format6
        Mode_EXIF = 256,

        // modes for Format7
        Mode_format7_0= 288,
        Mode_format7_1,
        Mode_format7_2,
        Mode_format7_3,
        Mode_format7_4,
        Mode_format7_5,
        Mode_format7_6,
        Mode_format7_7
      };

      /**
       * enumeration of framerates
       */
      enum eFramerate {
        Framerate_1_875 = 32,
        Framerate_3_75,
        Framerate_7_5,
        Framerate_15,
        Framerate_30,
        Framerate_60
      };
 
      /**
       * enumeration of color modes for format7
       */
      enum eFormat7ColorMode {
        Mono8 = 320,
        YUV411,
        YUV422,
        YUV444,
        RGB8,
        Mono16,
        RGB16        
      };


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
       * Mode in which the camera should operate.
       * The corresponding image format will be determined
       * automatically in updateParameters().
       *
       * Default: Mode_640x480_YUV422
       */
      eMode mode;

      /**
       * When format7 is used this determines the color mode of the
       * camera.
       *
       * Default: YUV422
       */
      eFormat7ColorMode format7ColorMode;

      /**
       * Desired framerate
       *
       */
      eFramerate framerate;
            
      /**
       * Gain of the camera, usually applied on all colors.
       */
      unsigned int gain;

      /**
       * Whether auto calibration of redGain and blueGain should be used.
       *
       * Default: true
       */
      bool autoWhiteBalance;

      /**
       * Whether auto calibration of shutter speed should be used.
       *
       * Default: true
       */
      bool autoShutter;

      /**
       * Gain of the camera, which is applied on red pixels.
       */
      unsigned int redGain;

      /**
       * Influences shutter speed of the camera, higher values
       * usually mean lower shutter speed.
       */
      unsigned int shutter;

      /**
       * Overall brightness.
       */
      unsigned int brightness;

      /**
       * Gain of the camera, which is applied on blue pixels.
       */
      unsigned int blueGain;

      /**
       * Whether auto calibration of gain should be used.
       *
       * Default: true
       */
      bool autoGain;

      /**
       * Whether one shot mode should be used.
       * In oneShot mode the computer advises the camera to shot
       * one picture and transfer it to the computer. 
       * If oneShot is false, the camera will transfer pictures
       * all the time, thus using a lot of fireWire bandwith.
       * The latter mode should be faster and recommended if high
       * framerates are desired.
       *
       * Default: true
       */
      bool oneShot;

      /**
       * DMA buffer size in frames
       * 
       * Default: 2
       */
      int dmaBufferSize;

      /**
       * Whether frames will be dropped to keep down latency.
       *
       * Default: true
       */
      bool dropFrames;

      /**
       * Device file for video capture.
       *
       * Default: "/dev/video1394/0"
       */
      std::string deviceFile;


      /**
       * Method which should be used to do demosaicing in raw (mono)
       * modes.
       */      
      bayerDemosaicing::eBayerMethod bayerMethod;

      /**
       * Bayer pattern used in demosaicing functor.
       */
      bayerDemosaicing::eBayerPattern bayerPattern;

    protected:
      /**
       * write mode parameter
       */
      bool writeMode(ioHandler& handler) const;
      
      /**
       * read mode parameter
       */
      bool readMode(ioHandler& handler);

      /**
       * write framerate parameter
       */
      bool writeFramerate(ioHandler& handler) const;

      /**
       * read framerate parameter
       */
      bool readFramerate(ioHandler& handler);


      
    };

    /**
     * Default constructor
     */
    fireWireDCAM();


    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fireWireDCAM(const fireWireDCAM& other);

    /**
     * Destructor
     */
    virtual ~fireWireDCAM();

    /**
     * Returns the name of this type ("fireWireDCAM")
     */
    virtual const char* getTypeName() const;

    
    /**
     * Gets one frame from camera.
     * @param dest image the result will be left here.
     * @return true if apply successful or false otherwise. 
     */
    bool apply(image& dest);

    /**
     * Gets one frame from camera.
     * @param dest channel8 the result be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& dest);


    /**
     * Grabs a raw frame from camera.
     * @param dest channle8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool grabRaw(matrix<ubyte>& dest);


    /**
     * write parameters in camera
     */
    bool updateParameters();


    /**
     * Set brightness register in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setBrightness(const unsigned int brightness) const;

    /**
     * Get brightness value directly out of camera.
     * @return true if operation was successfull, false otherwise
     */ 
    bool getBrightness(unsigned int& brightness) const;


    /**
     * Set shutter register in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setShutter(const unsigned int shutter) const;

    /**
     * Get shutter value directly out of camera.
     * @return true if operation was successfull, false otherwise
     */ 
    bool getShutter(unsigned int& shutter) const;

    /**
     * Set shutter to auto operation in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setShutterAuto(const bool value) const;

//     bool setSharpness(const unsigned int sharpness) const;
//     bool getSharpness(unsigned int& sharpness) const;


    /**
     * Set redGain and blueGain registers in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setWhiteBalance(const unsigned int blueGain, 
                         const unsigned int redGain) const;    
    
    /**
     * Get blueGain and redGain values directly out of camera.
     * @return true if operation was successfull, false otherwise
     */ 
    bool getWhiteBalance(unsigned int& blueGain,
                         unsigned int& redGain) const;

    /**
     * Set white balance to auto operation in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setWhiteBalanceAuto(const bool value) const;


    /**
     * Set gain register in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setGain(const unsigned int gain) const;

    /**
     * Get gain value directly out of camera.
     * @return true if operation was successfull, false otherwise
     */ 
    bool getGain(unsigned int& gain) const;

    /**
     * Set gain to auto operation in camera directly.
     * \b Note: This doesn't effect the local parameters object!
     * @return true if operation was successfull, false otherwise
     */
    bool setGainAuto(const bool value) const;
    
    /**
     * Get current video mode directly out of camera.
     * @return true if operation was successfull, false otherwise
     */ 
    bool getVideoMode(parameters::eMode& mode) const;

    /**
     * Get current frame rate directly out of camera.
     * @return true if operation was successfull, false otherwise
     */
    bool getFramerate(parameters::eFramerate& framerate) const;


    /**
     * Initializes the camera and dumps the parameters.
     * \b Important: the local parameters object will be overwritten with
     * the %parameters stored in the camera (e.g. factory defaults).
     * Only oneShot, dropFrames, deviceFile, dmaBufferSize, bayerPattern
     * and bayerMethod are preserved.
     * @return true if operation was successfull, false otherwise
     */
    bool init();


    /**
     * Member function that prints informations about camera features on
     * stdout.
     * @return true if operation was successfull, false otherwise
     */
    bool printFeatures() const;

    /**
     * Read the actual hardware settings of the camera into a parameter
     * object.
     * \b Note: the %parameters deviceFile, oneShot, dropFrames will
     * be copied from the local parameters instance
     * @param par the parameters object in which the gained information
     * will be stored
     * @return true if operation was successfull, false otherwise
     */
    bool dumpParameters(parameters &par);
      

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fireWireDCAM& copy(const fireWireDCAM& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fireWireDCAM& operator=(const fireWireDCAM& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    /** internal enumeration of color modes **/
    enum eColorMode {
      Mono8,
//      Mono16,  // not yet implemented
//      YUV444,  // not yet implemented
      YUV411,
      YUV422,
//      RGB8,   // not yet implemented
//      RGB16   // not yet implemented
    };

    
    /** Enumeration of camera image formats */
    enum eFormat {
      VGA = 384, /*< FORMAT_VGA_NONCOMPRESSED */
      SVGA1,       /*< FORMAT_SVGA_NONCOMPRESSED_1 */
      SVGA2,       /*< FORAMT_SVGA_NONCOMPRESSED_2 */
      /* 3 reserved formats */
      StillImage = 390,
      ScalableImage        
    };

    
    /**
     * colormode used for conversion to rgb image 
     */
    eColorMode colorMode_;

    /** 
     * image format in which the camere should be set 
     */
    eFormat format_;
    
    /**
     * saves what features are available
     */
    dc1394_feature_set features_;

    /**
     * FireWire handle for low level communication
     */
    raw1394handle_t handle_;

    /**
     * just a buffer channel8
     */
    channel8 ch8_;

    /**
     * capture handle with the camera node, buffer etc.
     */
    dc1394_cameracapture camera_;

    /**
     * remember if camera is initialized
     *
     * Default: false
     */
    bool init_;

    /**
     * if a setup capture succeeded
     */
    bool setupCapture_;

    /**
     * fireWire port where the camere will be searched
     * Should usually be 0 unless there are multiple fireWire
     * cards in the system
     *
     * Default: 0
     */
    int port_;

    
    /**
     * merger for YUV to RGB conversion
     */
    mergeYUVToImage merger_;

    /**
     * demosaicing functor for Mono to RGB conversion
     */
    bayerDemosaicing bayer_;

    /**
     * converts YUV411 to RGB
     */
    void yuv411toRGB(image& srcdest) const;

    /**
     * converts YUV422 to RGB
     */
    void yuv422toRGB(image& srcdest) const;


    /**
     * check features_ if a feature is available
     */
    bool isFeatureAvailable(const unsigned int featureid) const;

    /**
     * ask driver if a feature is set to auto 
     */
    bool isFeatureAuto(const unsigned int featureid) const;

    /**
     * set a feature auto
     */
    bool setFeatureAuto(const unsigned int featureid, 
                        const bool value) const;

    /**
     * just capture a frame through libdc1394
     */
    bool captureFrame();
  

  private:
    /**
     * This constructor should \b not be implemented.
     * As init() has to be invoked before setting custom %parameters
     * implementing this constructor may be misleading.
     */
    fireWireDCAM(const parameters& par);



  };
}

#endif

#endif

