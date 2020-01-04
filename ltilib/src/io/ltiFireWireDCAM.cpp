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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiFireWireDCAM.cpp
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 21.10.2004
 * revisions ..: $Id: ltiFireWireDCAM.cpp,v 1.5 2005/06/16 12:37:11 doerfler Exp $
 */

#ifdef _USE_FIRE_WIRE_DCAM

#include "ltiFireWireDCAM.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"



namespace lti {
  

  // --------------------------------------------------
  // fireWireDCAM::parameters
  // --------------------------------------------------

  // default constructor
  fireWireDCAM::parameters::parameters()
    : frameGrabber::parameters() {
    //TODO: Initialize your parameter values!
    // If you add more parameters manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new parameters
    // 3. make sure that the read and write members also read and
    //    write your parameters
    
    gain = 0;
    autoWhiteBalance = true;
    autoShutter = true;
    redGain = 0;
    shutter = 0;
    brightness = 0;
    blueGain = 0;
    autoGain = true;
    oneShot = true;
    dmaBufferSize = 2;
    dropFrames = true;
    deviceFile = "/dev/video1394/0";
    mode = Mode_640x480_YUV422;
  }

  // copy constructor
  fireWireDCAM::parameters::parameters(const parameters& other)
    : frameGrabber::parameters() {
    copy(other);
  }

  // destructor
  fireWireDCAM::parameters::~parameters() {

  }

  // get type name
  const char* fireWireDCAM::parameters::getTypeName() const {
    return "fireWireDCAM::parameters";
  }

  // copy member

  fireWireDCAM::parameters&
    fireWireDCAM::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    frameGrabber::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    frameGrabber::parameters& (frameGrabber::parameters::* p_copy)
      (const frameGrabber::parameters&) =
      frameGrabber::parameters::copy;
    (this->*p_copy)(other);
# endif

    
    gain = other.gain;
    autoWhiteBalance = other.autoWhiteBalance;
    autoShutter = other.autoShutter;
    redGain = other.redGain;
    shutter = other.shutter;
    brightness = other.brightness;
    blueGain = other.blueGain;
    autoGain = other.autoGain;
    oneShot = other.oneShot;
    dmaBufferSize = other.dmaBufferSize;
    dropFrames = other.dropFrames;
    deviceFile = other.deviceFile;
    bayerMethod = other.bayerMethod;
    bayerPattern = other.bayerPattern;    
    mode = other.mode;
    framerate = other.framerate;
    return *this;
  }
    
  // alias for copy member
    fireWireDCAM::parameters&
    fireWireDCAM::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  functor::parameters* fireWireDCAM::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool fireWireDCAM::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool fireWireDCAM::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      
      lti::write(handler,"gain",gain);
      lti::write(handler,"autoWhiteBalance",autoWhiteBalance);
      lti::write(handler,"autoShutter",autoShutter);
      lti::write(handler,"redGain",redGain);
      lti::write(handler,"shutter",shutter);
      lti::write(handler,"brightness",brightness);
      lti::write(handler,"blueGain",blueGain);
      lti::write(handler,"autoGain",autoGain);
      lti::write(handler,"oneShot",oneShot);
      lti::write(handler,"dmaBufferSize",dmaBufferSize);
      lti::write(handler,"dropFrames",dropFrames);
      lti::write(handler,"deviceFile",deviceFile);
      lti::write(handler,"bayerMethod",bayerMethod);
      lti::write(handler,"bayerPattern",bayerPattern);
      writeMode(handler);
      writeFramerate(handler);


    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && frameGrabber::parameters::write(handler,false);
# else
    bool (frameGrabber::parameters::* p_writeMS)(ioHandler&,const bool) const =
      frameGrabber::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  // write framerate
  bool fireWireDCAM::parameters::writeFramerate(ioHandler& handler) const {
    switch (framerate) {
      case (Framerate_1_875): {
        lti::write(handler,"framerate","Framerate_1_875");
        break;
      }
      case (Framerate_3_75): {
        lti::write(handler,"framerate","Framerate_3_75");
        break;
      }
      case (Framerate_7_5): {
        lti::write(handler,"framerate","Framerate_7_5");
        break;
      }
      case (Framerate_15): {
        lti::write(handler,"framerate","Framerate_15");
        break;
      }
      case (Framerate_30): {
        lti::write(handler,"framerate","Framerate_30");
        break;
      }
      case (Framerate_60): {
        lti::write(handler,"framerate","Framerate_60");
        break;
      }
    }
    return true;
  }
  
  // write mode
  bool fireWireDCAM::parameters::writeMode(ioHandler& handler) const {
    // write mode
    switch (mode) {
      // modes in format0
      case (Mode_160x120_YUV444): {
        lti::write(handler,"mode","Mode_160x120_YUV444");
        break;
      }
      case (Mode_320x240_YUV422): {
        lti::write(handler,"mode","Mode_320x240_YUV422");
        break;
      }
      case (Mode_640x480_YUV411): {
        lti::write(handler,"mode","Mode_640x480_YUV411");
        break;
      }
      case (Mode_640x480_YUV422): {
        lti::write(handler,"mode","Mode_640x480_YUV422");
        break;
      }
      case (Mode_640x480_RGB): {
        lti::write(handler,"mode","Mode_640x480_RGB");
        break;
      }
      case (Mode_640x480_Mono): {
        lti::write(handler,"mode","Mode_640x480_Mono");
        break;
      }
      case (Mode_640x480_Mono16): {
        lti::write(handler,"mode","Mode_640x480_Mono16");
        break;
      }

      // modes in format1
      case (Mode_800x600_YUV422): {
        lti::write(handler,"mode","Mode_800x600_YUV422");
        break;
      }
      case (Mode_800x600_RGB): {
        lti::write(handler,"mode","Mode_800x600_RGB");
        break;
      }
      case (Mode_800x600_Mono): {
        lti::write(handler,"mode","Mode_800x600_Mono");
        break;
      }
      case (Mode_1024x768_YUV422): {
        lti::write(handler,"mode","Mode_1024x768_YUV422");
        break;
      }
      case (Mode_1024x768_RGB): {
        lti::write(handler,"mode","Mode_1024x768_RGB");
        break;
      }
      case (Mode_1024x768_Mono): {
        lti::write(handler,"mode","Mode_1024x768_Mono");
        break;
      }
      case (Mode_800x600_Mono16): {
        lti::write(handler,"mode","Mode_800x600_Mono16");
        break;
      }
      case (Mode_1024x768_Mono16): {
        lti::write(handler,"mode","Mode_1024x768_Mono16");
        break;
      }

      // Modes in format2
      case (Mode_1280x960_YUV422): {
        lti::write(handler,"mode","Mode_1280x960_YUV422");
        break;
      }
      case (Mode_1280x960_RGB): {
        lti::write(handler,"mode","Mode_1280x960_RGB");
        break;
      }
      case (Mode_1280x960_Mono): {
        lti::write(handler,"mode","Mode_1280x960_Mono");
        break;
      }
      case (Mode_1600x1200_YUV422): {
        lti::write(handler,"mode","Mode_1600x1200_YUV422");
        break;
      }
      case (Mode_1600x1200_RGB): {
        lti::write(handler,"mode","Mode_1600x1200_RGB");
        break;
      }
      case (Mode_1600x1200_Mono): {
        lti::write(handler,"mode","Mode_1600x1200_Mono");
        break;
      }
      case (Mode_1280x960_Mono16): {
        lti::write(handler,"mode","Mode_1280x960_Mono16");
        break;
      }
      case (Mode_1600x1200_Mono16): {
        lti::write(handler,"mode","Mode_1600x1200_Mono16");
        break;
      }

      // modes for Format6
      case (Mode_EXIF): {
        lti::write(handler,"mode","Mode_EXIF");
        break;
      }

      // modes for Format7
      case (Mode_format7_0): {
        lti::write(handler,"mode","Mode_format7_0");
        break;
      }
      case (Mode_format7_1): {
        lti::write(handler,"mode","Mode_format7_1");
        break;
      }
      case (Mode_format7_2): {
        lti::write(handler,"mode","Mode_format7_2");
        break;
      }
      case (Mode_format7_3): {
        lti::write(handler,"mode","Mode_format7_3");
        break;
      }
      case (Mode_format7_4): {
        lti::write(handler,"mode","Mode_format7_4");
        break;
      }
      case (Mode_format7_5): {
        lti::write(handler,"mode","Mode_format7_5");
        break;
      }
      case (Mode_format7_6): {
        lti::write(handler,"mode","Mode_format7_6");
        break;
      }
      case (Mode_format7_7): {
        lti::write(handler,"mode","Mode_format7_7");
        break;
      }
    }
    return true;
  }

  bool fireWireDCAM::parameters::readFramerate(ioHandler& handler) {
    std::string str;
    lti::read(handler,"framerate",str);
    if ("Framerate_1_875"==str) {
      framerate = Framerate_1_875;
    } else if ("Framerate_3_75"==str) {
      framerate = Framerate_3_75;
    } else if ("Framerate_7_5"==str) {
      framerate = Framerate_7_5;
    } else if ("Framerate_15"==str) {
      framerate = Framerate_15;
    } else if ("Framerate_30"==str) {
      framerate = Framerate_30;
    } else if ("Framerate_30"==str) {
      framerate = Framerate_30;
    } else {
      // default fallback
      framerate = Framerate_1_875;
      return false;
    }
    return true;
  }

  bool fireWireDCAM::parameters::readMode(ioHandler& handler) {
    std::string str;
    lti::read(handler,"mode",str);
    return true;
  }

   

# ifdef _LTI_MSC_6
  bool fireWireDCAM::parameters::write(ioHandler& handler,
                                       const bool complete) const {
    // ...we need this workaround to cope with another really awful MSVC bug.
    return writeMS(handler,complete);
  }
# endif

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if read was succeful
   */
# ifndef _LTI_MSC_6
  bool fireWireDCAM::parameters::read(ioHandler& handler,
                                        const bool complete)
# else
  bool fireWireDCAM::parameters::readMS(ioHandler& handler,
                                          const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      
      lti::read(handler,"gain",gain);
      lti::read(handler,"autoWhiteBalance",autoWhiteBalance);
      lti::read(handler,"autoShutter",autoShutter);
      lti::read(handler,"redGain",redGain);
      lti::read(handler,"shutter",shutter);
      lti::read(handler,"brightness",brightness);
      lti::read(handler,"blueGain",blueGain);
      lti::read(handler,"autoGain",autoGain);
      lti::read(handler,"oneShot",oneShot);
      lti::read(handler,"dmaBufferSize",dmaBufferSize);
      lti::read(handler,"dropFrames",dropFrames);
      lti::read(handler,"deviceFile",deviceFile);
      lti::read(handler,"bayerMethod",bayerMethod);
      lti::read(handler,"bayerPattern",bayerPattern);
      readFramerate(handler);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && frameGrabber::parameters::read(handler,false);
# else
    bool (frameGrabber::parameters::* p_readMS)(ioHandler&,const bool) =
      frameGrabber::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool fireWireDCAM::parameters::read(ioHandler& handler,
                                        const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // --------------------------------------------------
  // fireWireDCAM
  // --------------------------------------------------

  // default constructor
  fireWireDCAM::fireWireDCAM()
    : frameGrabber(){

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.
    // for now only cameras on port 0 are recognized

    init_ = false;
    port_ = 0;
    setupCapture_ = false;
    
   
    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);

  }

  // this constructor must not be implemented!!!
  fireWireDCAM::fireWireDCAM(const parameters& par)
    : frameGrabber() {
    throw invalidMethodException("use default constructor instead");
  }


  // copy constructor
  fireWireDCAM::fireWireDCAM(const fireWireDCAM& other)
    : frameGrabber() {
    copy(other);
  }

  // destructor
  fireWireDCAM::~fireWireDCAM() {

    if (setupCapture_) {
      dc1394_dma_release_camera(handle_,&camera_);
    }
    // destroy handle
    if (init_) {
      dc1394_destroy_handle(handle_);
    }
  }

  // returns the name of this type
  const char* fireWireDCAM::getTypeName() const {
    return "fireWireDCAM";
  }

  // copy member
  fireWireDCAM&
    fireWireDCAM::copy(const fireWireDCAM& other) {
      frameGrabber::copy(other);

      init_ = other.init_;
      port_ = other.port_;
      setupCapture_ = other.setupCapture_;

    return (*this);
  }

  // alias for copy member
  fireWireDCAM&
    fireWireDCAM::operator=(const fireWireDCAM& other) {
    return (copy(other));
  }


  // clone member
  functor* fireWireDCAM::clone() const {
    return new fireWireDCAM(*this);
  }

  // return parameters
  const fireWireDCAM::parameters&
    fireWireDCAM::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }


  
  // only updateParameters when camera is initialized
  bool fireWireDCAM::updateParameters() {
    bool b = true;

    // camera must be initialized
    if (init_) {

      const parameters& par = getParameters();
      
      _lti_debug("setting parameters in camera...\n");

      // brightness
      if (isFeatureAvailable(FEATURE_BRIGHTNESS)) {
        b = b && setBrightness(par.brightness);
      }
      

      // white balance
      if (isFeatureAvailable(FEATURE_WHITE_BALANCE)) {
        b = b && setWhiteBalance(par.blueGain,par.redGain);
        setWhiteBalanceAuto(par.autoWhiteBalance);
      }

      // shutter
      if (isFeatureAvailable(FEATURE_SHUTTER)) {
        b = b && setShutter(par.shutter);
        setShutterAuto(par.autoShutter);
      }

      // gain
      if (isFeatureAvailable(FEATURE_GAIN)) {
        b = b && setGain(par.gain);
        setGainAuto(par.autoGain);
      }

      if (b) {
        // determine color mode and format
        switch (par.mode) {
          // YUV 444
          case (parameters::Mode_160x120_YUV444): {
            setStatusString("sorry, color mode YUV444 not yet supported\n");
            b = false;
            break;
          }

          // YUV 422
          case (parameters::Mode_320x240_YUV422):
          case (parameters::Mode_640x480_YUV422): {
            colorMode_ = YUV422;
            format_ = VGA;
            break;
          }
          case (parameters::Mode_800x600_YUV422):
          case (parameters::Mode_1024x768_YUV422): {
            colorMode_ = YUV422;
            format_ = SVGA1;
            break;
          }
          case (parameters::Mode_1280x960_YUV422):
          case (parameters::Mode_1600x1200_YUV422): {
            colorMode_ = YUV422;
            format_ = SVGA2;
            break;
          }

          // YUV 411
          case (parameters::Mode_640x480_YUV411): {
            colorMode_ = YUV411;
            format_ = VGA;
            break;
          }

          // Mono8
          case (parameters::Mode_640x480_Mono): {
            colorMode_ = Mono8;
            format_ = VGA;
            break;
          }
          case (parameters::Mode_800x600_Mono):
          case (parameters::Mode_1024x768_Mono): {
            colorMode_ = Mono8;
            format_ = SVGA1;
            break;
          }
          case (parameters::Mode_1280x960_Mono):
          case (parameters::Mode_1600x1200_Mono): {
            colorMode_ = Mono8;
            format_ = SVGA2;
            break;
          }

          // Mono16
          case (parameters::Mode_640x480_Mono16): {
            format_ = VGA;
            setStatusString("sorry, color mode Mono16 not yet supported\n");
            b = false;
            break;
          }
          case (parameters::Mode_800x600_Mono16):
            case (parameters::Mode_1024x768_Mono16): {
            format_ = SVGA1;
            setStatusString("sorry, color mode Mono16 not yet supported\n");
            b = false;
            break;
          }
          case (parameters::Mode_1280x960_Mono16):
          case (parameters::Mode_1600x1200_Mono16): {
            format_ = SVGA2;
            setStatusString("sorry, color mode Mono16 not yet supported\n");
            b = false;
            break;
          }

          // RGB
          case (parameters::Mode_640x480_RGB):
          case (parameters::Mode_800x600_RGB):
          case (parameters::Mode_1024x768_RGB):
          case (parameters::Mode_1280x960_RGB):
          case (parameters::Mode_1600x1200_RGB): {
            setStatusString("sorry, color mode RGB not yet supported\n");
            b = false;
            break;
          }

          // format6
          case (parameters::Mode_EXIF): {
            setStatusString("sorry, format6 not yet supported\n");
            b = false;
            break;
          }

          // format7
          case (parameters::Mode_format7_0):
          case (parameters::Mode_format7_1):
          case (parameters::Mode_format7_2):
          case (parameters::Mode_format7_3):
          case (parameters::Mode_format7_4):
          case (parameters::Mode_format7_5):
          case (parameters::Mode_format7_6):
          case (parameters::Mode_format7_7): {
            setStatusString("sorry, format7 not yet supported\n");
            b = false;
            break;
          }

          default: {
            setStatusString("sorry, unknown format\n");
            b = false;
            break;
          }

        }
      }
      
      if (setupCapture_) {
        _lti_debug("release old camera structure...\n");
        dc1394_dma_release_camera(handle_,&camera_);
        setupCapture_ = false;
      }
      
      
      _lti_debug("setup capture...\n");
      int drop;
      if (par.dropFrames) {
        drop = 1;
      } else {
        drop = 0;
      }
      _lti_debug(" format: "<<format_<<"\n");
      _lti_debug(" mode:   "<<par.mode<<"\n");
      if (dc1394_dma_setup_capture(handle_,camera_.node,0,
                                   format_,
                                   par.mode,
                                   SPEED_400,
                                   par.framerate,
                                   par.dmaBufferSize,
                                   drop,
                                   par.deviceFile.data(),
                                   &camera_)==DC1394_SUCCESS) {
        setupCapture_ = true;
      } else {
        _lti_debug("unable to setup capture\n");
        b = false;
      } 
      
      // stop transmission
      dc1394_stop_iso_transmission(handle_,camera_.node);

      // if oneShot mode is not set turn on transmission
      if (!(par.oneShot)) {
        _lti_debug("start iso-transmission...\n");
        if (dc1394_start_iso_transmission(handle_,camera_.node)
            !=DC1394_SUCCESS) {
          _lti_debug("unable to start iso transmission\n");
          b = false;
        }        
      }
      
    }
    return b;
  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // helper functio to capture a frame 
  bool fireWireDCAM::captureFrame() {
    // is camera initialized?
    if (!init_) {
      bool b = init();
      if (!b) {
        setStatusString("Failed to initialize camera.");
        return false;
      }
    }
    // is there a camera handle
    if (!setupCapture_) {
      // there should be a statusstring from updateParameters()
      return false;
    }

    const parameters& par = getParameters();
    if (par.oneShot==true) {      
      _lti_debug("set into one shot mode...\n");
      if (dc1394_set_one_shot(handle_,camera_.node)!=DC1394_SUCCESS) {
        _lti_debug("unable to enter one shot mode...\n");
        setStatusString("unable to enter one shot mode.\n");
        return false;
      }
    }

    _lti_debug("last dma buffer: "<<camera_.dma_last_buffer<<"\n");
    _lti_debug("dma buffer_size: "<<camera_.num_dma_buffers<<"\n");
    // if all dma buffers are used release buffer:
//     if (camera_.dma_last_buffer+1==camera_.num_dma_buffers) {
//       _lti_debug("release buffer...\n");
//       dc1394_dma_done_with_buffer(&camera_);
//     }


    _lti_debug("capture single-frame...\n");
    if (dc1394_dma_single_capture(&camera_)!=DC1394_SUCCESS) {
      _lti_debug("unable to do dma_single_capture\n");
      setStatusString("Unable to capture single frame. Check camera setup.\n");
      return false;
    }
    return true;
  }

  
  // On place apply for type image!
  bool fireWireDCAM::apply(image& srcdest) {
    // capture frame
    if (!(captureFrame())) {
      // statusString is set in captureFrame();
      return false;
    }

    // determine how the conversion should be done
    switch (colorMode_) {
      case (YUV411) : {
        _lti_debug("convert frame YUV411 -> RGB...\n");
        yuv411toRGB(srcdest);
        break;
      }
      case (YUV422) : {
        _lti_debug("convert frame YUV422 -> RGB...\n");
        yuv422toRGB(srcdest);
        break;
      }
      case (Mono8) : {
        _lti_debug("bayer conversion of frame...\n");
        const int rows = camera_.frame_height;
        const int cols = camera_.frame_width;
        ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
        ch8_.useExternData(rows,cols,sPtr);
        bayer_.apply(ch8_,srcdest);
        break;
      }
    }
  
    /*  channel8 ch8;
    _lti_debug("copy frame...\n");
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    ch8.resize(rows,cols,ubyte(),false,false);
    ubyte* ptr = reinterpret_cast<ubyte*>(&ch8.at(0,0));
    memcpy(ptr,camera_.capture_buffer,sizeof(ubyte)*rows*cols);
    */

    // _lti_debug("convert frame...\n");
    // yuv422toRGB(srcdest);
    //bayer_.apply(ch8,srcdest);
    _lti_debug("release buffer...\n");
    dc1394_dma_done_with_buffer(&camera_);
    
    return true;
  }

  bool fireWireDCAM::apply(channel8& dest) {
    image tmp;
    if (apply(tmp)) {
      dest.castFrom(tmp);
    } else {
      return false;
    }
    
    return true;
  }

  bool fireWireDCAM::grabRaw(matrix<ubyte>& dest) {
    // nothing done when not in mono mode
    if (colorMode_!=Mono8) {
      setStatusString(
        "Error: to grab raw images the camera colormode must be mono8\n");
      return false;
    }

    // capture frame
    if (!(captureFrame())) {
      // statusString is set in captureFrame();
      return false;
    } 

    // just resize and copy data
    _lti_debug("copy frame...\n");
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.resize(rows,cols,ubyte(),false,false);
    ubyte* ptr = reinterpret_cast<ubyte*>(&dest.at(0,0));
    memcpy(ptr,camera_.capture_buffer,sizeof(ubyte)*rows*cols);


    // never forget to release the buffer ;-)
    _lti_debug("release buffer...\n");
    dc1394_dma_done_with_buffer(&camera_);

    return true;
  }


  // simple conversion from yuv422 to rgb, NO interpolation is done
  void fireWireDCAM::yuv422toRGB(image& dest) const {
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.resize(rows,cols,rgbPixel(),false,false);



   // 4 pixels are stored in 8 byte
    const int elemSize = 8;

    // iterator for destination pixels
    image::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
    // iterator for capture buffer
    ubyte* iPtr;
    // end of capture buffer
    ubyte* ePtr = sPtr + ((rows*cols)/4)*elemSize;
    
    for (iPtr = sPtr; iPtr != ePtr;) {
      // byte structure is uyvyuyvyuyvy
      const ubyte u1 = *(iPtr++);
      const ubyte y1 = *(iPtr++);
      const ubyte v1 = *(iPtr++);
      const ubyte y2 = *(iPtr++);
      // 1st pixel
      merger_.apply(y1,u1,v1,*(itd++));
      // 2nd pixel
      merger_.apply(y2,u1,v1,*(itd++));      
    }
  }

  // simple conversion from yuv411 to rgb, NO interpolation is done
  void fireWireDCAM::yuv411toRGB(image& dest) const {
    int rows = camera_.frame_height;
    int cols = camera_.frame_width;
    dest.resize(rows,cols,rgbPixel(),false,false);



   // 4 pixels are stored in 6 byte
    const int elemSize = 6;

    // iterator for destination pixels
    image::iterator itd = dest.begin();

    ubyte* sPtr = reinterpret_cast<ubyte*>(camera_.capture_buffer);
    // iterator for capture buffer
    ubyte* iPtr;
    // end of capture buffer
    ubyte* ePtr = sPtr + ((rows*cols)/4)*elemSize;
    
    for (iPtr = sPtr; iPtr != ePtr;) {
      // byte structure is uyyvyyuyyvyy
      const ubyte u1 = *(iPtr++);
      const ubyte y1 = *(iPtr++);
      const ubyte y2 = *(iPtr++);
      const ubyte v1 = *(iPtr++);
      const ubyte y3 = *(iPtr++);
      const ubyte y4 = *(iPtr++);
      // 1st pixel
      merger_.apply(y1,u1,v1,*(itd++));
      // 2nd pixel
      merger_.apply(y2,u1,v1,*(itd++));      
      // 3rd pixel
      merger_.apply(y3,u1,v1,*(itd++));
      // 4th pixel
      merger_.apply(y4,u1,v1,*(itd++));      

    }
  }

  bool fireWireDCAM::dumpParameters(parameters& par) {
    // copy some parameters from local parameter object
    const parameters& oldPar = getParameters();
    par.deviceFile = oldPar.deviceFile;
    par.dropFrames = oldPar.dropFrames;
    par.oneShot = oldPar.oneShot;
    par.bayerMethod = oldPar.bayerMethod;
    par.bayerPattern = oldPar.bayerPattern;
    par.dmaBufferSize = oldPar.dmaBufferSize;
    
    
    bool b = true;;
    _lti_debug("reading parameters from camera...\n");

    // videomode
    b = b && getVideoMode(par.mode);
    _lti_debug(" videomode is: "<<par.mode<<"\n");

    // framerate
    b = b && getFramerate(par.framerate);
    _lti_debug(" framerate is: "<<par.framerate<<"\n");

    // brightness
    if (isFeatureAvailable(FEATURE_BRIGHTNESS)) {
      b = b && getBrightness(par.brightness);     
      _lti_debug(" brightness is: "<<par.brightness<<"\n");
    } else {
      par.brightness = 0;
      _lti_debug(" brightness is not available\n");
    }
    
    // white balance (redGain/blueGain)
    if (isFeatureAvailable(FEATURE_WHITE_BALANCE)) {
      b = b && getWhiteBalance(par.blueGain,par.redGain);
      if (isFeatureAuto(FEATURE_WHITE_BALANCE)) {
        par.autoWhiteBalance = true;
      } else {
        par.autoWhiteBalance = false;
      }
      _lti_debug(" white balance is: "<<par.blueGain<<" (blue), "
                 <<par.redGain<< " (red); auto is: "
                 <<par.autoWhiteBalance<<"\n");
    } else {
      par.redGain = 0; par.blueGain = 0;
      _lti_debug(" white balance is not available\n");
    }
    
    // shutter
    if (isFeatureAvailable(FEATURE_SHUTTER)) {
      b = b && getShutter(par.shutter);     
      if (isFeatureAuto(FEATURE_SHUTTER)) {
        par.autoShutter = true;
      } else {
        par.autoShutter = false;
      }
      _lti_debug(" shutter is: "<<par.shutter<<"; auto is: "
                 << par.autoShutter<<"\n");
    } else {
      par.shutter = 0;
      _lti_debug(" shutter setting is not available\n");
    }

    // gain
    if (isFeatureAvailable(FEATURE_GAIN)) {
      b = b && getGain(par.gain);     
      if (isFeatureAuto(FEATURE_GAIN)) {
        par.autoGain = true;
      } else {
        par.autoGain = false;
      }
      _lti_debug(" gain is: "<<par.gain<<"; auto is: "
                 << par.autoGain<<"\n");
    } else {
      par.gain = 0;
      _lti_debug(" gain setting is not available\n");
    }

    return b;
  }

  // protected internal helper
  bool fireWireDCAM::isFeatureAvailable(const unsigned int featureId) const {
    int i;
    for (i=0;i<NUM_FEATURES;i++) {
      if (features_.feature[i].feature_id == featureId) {
        if (features_.feature[i].available == DC1394_TRUE) {
          return true;
        } else {
          return false;
        }
      }
    }
    return false;
  }
 
  // protected internal helper
  bool fireWireDCAM::isFeatureAuto(const unsigned int featureId) const {
    dc1394bool_t isAuto;
    if (dc1394_is_feature_auto(handle_,
                               camera_.node,
                               featureId,&isAuto) != DC1394_SUCCESS) {
      // return false if an error occurs
      _lti_debug("unable to ask if feature "<<featureId<<" is auto\n");
      return false;
    }
    if (isAuto==DC1394_TRUE) {
      return true;
    } else {
      return false;
    }
  }

  // protected internal helper
  bool fireWireDCAM::setFeatureAuto(const unsigned int featureId, 
                                    const bool val) const {
    unsigned int ival;
    if (val) {
      ival = 1;
    } else {
      ival = 0;
    }
    bool b=(dc1394_auto_on_off(handle_,camera_.node,
                               featureId,ival) == DC1394_SUCCESS);
    return b;
  }


  // print actual feature set for debug purpose
  bool fireWireDCAM::printFeatures() const {
    dc1394_feature_set features;
    if (dc1394_get_camera_feature_set(handle_,camera_.node,
                                      &features)!=DC1394_SUCCESS) {
      return false;
    }
    dc1394_print_feature_set(&features);
    return true;
  }

  // init camera
  bool fireWireDCAM::init() {
//    const parameters& par = getParameters();
    _lti_debug("init camera...\n");
    bool b = true;
    init_ = false;

    // create handle on port_ (0 is first FW card)
    handle_ = dc1394_create_handle(port_);
    if (NULL==handle_) {
      _lti_debug(" Unable to create raw1394 handle, kernel modules loaded?\n");
      setStatusString("Unable to create raw1394 handle, kernel modules loaded?\n");
      return false;
    }

    //get cameras on port_
    nodeid_t * cameraNodes = NULL;
    int numCams=0;
    cameraNodes = dc1394_get_camera_nodes(handle_,&numCams,0);
    _lti_debug(" Found "<<numCams<< " cameras.\n");
    if (0==numCams) {
      setStatusString("no camera found.\n");
      return false;
    }

    // for now use the first camera 
    camera_.node=cameraNodes[0];
    
#ifdef _LTI_DEBUG
    // get camerainfo for debugging
    dc1394_camerainfo cameraInfo;
    dc1394_get_camera_info(handle_,camera_.node,&cameraInfo);
    _lti_debug(" Using "<<cameraInfo.model<<" ("<<cameraInfo.vendor
               <<") on Node "<< cameraInfo.id << ".\n");
#endif


    b = b && (dc1394_init_camera(handle_,camera_.node)==DC1394_SUCCESS);
//    getfeatures
    b = b && (dc1394_get_camera_feature_set(handle_,camera_.node,
                                            &features_));
#ifdef _LTI_DEBUG
//    _lti_debug("features available:\n");
//    printFeatures();
#endif
    if (b) { 
      init_ = true;
      // dump parameters
      parameters par;
      dumpParameters(par);
      setParameters(par);
//      updateParameters();
    } else {
      setStatusString("Init failed. Check driver and camera connection.\n");
      init_ = false;
    }
    return b;
  }
    

  
  // brightness
  bool fireWireDCAM::setBrightness(const unsigned int brightness) const {
    bool b = (dc1394_set_brightness(handle_,camera_.node, brightness)
              ==DC1394_SUCCESS);
    return b;    
  }

  bool fireWireDCAM::getBrightness(unsigned int &brightness) const {
    bool b = (dc1394_get_brightness(handle_,camera_.node, &brightness)
              ==DC1394_SUCCESS);
    return b;
  }

//   bool fireWireDCAM::setBrightnessAuto(bool auto) const {
//     return setFeatureAuto(FEATURE_BRIGHTNESS,auto);
//   }

  // shutter
  bool fireWireDCAM::setShutter(const unsigned int shutter) const {
    bool b = (dc1394_set_shutter(handle_,camera_.node, shutter)
              ==DC1394_SUCCESS);
    return b;    
  }

  bool fireWireDCAM::getShutter(unsigned int &shutter) const {
    bool b = (dc1394_get_shutter(handle_,camera_.node, &shutter)
              ==DC1394_SUCCESS);
    return b;
  }

  bool fireWireDCAM::setShutterAuto(bool value) const {
    return setFeatureAuto(FEATURE_SHUTTER,value);
  }

  // gain
  bool fireWireDCAM::setGain(const unsigned int gain) const {
    bool b = (dc1394_set_gain(handle_,camera_.node, gain)
              ==DC1394_SUCCESS);
    return b;    
  }

  bool fireWireDCAM::getGain(unsigned int &gain) const {
    bool b = (dc1394_get_gain(handle_,camera_.node, &gain)
              ==DC1394_SUCCESS);
    return b;
  }

  bool fireWireDCAM::setGainAuto(bool value) const {
    return setFeatureAuto(FEATURE_GAIN,value);
  }


  // white balance
  bool fireWireDCAM::setWhiteBalance(const unsigned int blueGain,
                                     const unsigned int redGain) const {
    bool b = (dc1394_set_white_balance(handle_,camera_.node,
                                       blueGain,redGain)
              ==DC1394_SUCCESS);
    return b;    
  }

  bool fireWireDCAM::getWhiteBalance(unsigned int &blueGain,
                                     unsigned int &redGain) const {
    bool b = (dc1394_get_white_balance(handle_,camera_.node, 
                                       &blueGain,&redGain)
              ==DC1394_SUCCESS);
    return b;
  }

  bool fireWireDCAM::setWhiteBalanceAuto(bool value) const {
    return setFeatureAuto(FEATURE_WHITE_BALANCE,value);
  }


  bool fireWireDCAM::getFramerate(parameters::eFramerate& rate) const {
    unsigned int irate;
    bool b = true;
    b = b && (dc1394_get_video_framerate(handle_,camera_.node,&irate)
              ==DC1394_SUCCESS);
    if (b) {
      // same enumeration values
      rate = static_cast<parameters::eFramerate>(irate);
    }
    return b;
  }

  bool fireWireDCAM::getVideoMode(parameters::eMode &mode) const {
    unsigned int imode;
    unsigned int iformat;
    bool b = true;
    b = b && (dc1394_get_video_format(handle_,camera_.node,&iformat)
              ==DC1394_SUCCESS);
    b = b && (dc1394_get_video_mode(handle_,camera_.node,&imode)
              ==DC1394_SUCCESS);
    
    if (b) {
      switch (iformat) {
        case (FORMAT_VGA_NONCOMPRESSED):
        case (FORMAT_SVGA_NONCOMPRESSED_1):
        case (FORMAT_SVGA_NONCOMPRESSED_2): {
          // same enumeration values
          mode = static_cast<parameters::eMode>(imode);
          break;
        }
        default: {
          // unsupported mode
          mode = static_cast<parameters::eMode>(imode);
//          b = false;
          break;
        }
      }
    }
    return b;
  }


  


}

#include "ltiUndebug.h"

#endif //USE_FIRE_WIRE_DCAM
