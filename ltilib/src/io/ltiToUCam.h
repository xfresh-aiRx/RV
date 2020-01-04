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
 * file .......: ltiToUCam.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 24.04.01
 * revisions ..: $Id: ltiToUCam.h,v 1.17 2005/11/24 05:02:05 sltilib Exp $
 */

#ifndef _LTI_TOUCAM_H_
#define _LTI_TOUCAM_H_

#include "ltiFrameGrabber.h"

#ifdef _USE_PHILIPS_TOUCAM

#if __linux__

#include <string>

#undef None

namespace lti {

  /**
   * Philips USB ToUCam "frame grabber".
   *
   * This class allows capturing images from the USB camera ToUCam and
   * other compatible web-cams (see
   * <a href="http://www.smcc.demon.nl/webcam/">here</a>).  Even if
   * there is no physical frame-grabber, this class provides the
   * standard frameGrabber interface to simplify its use.
   *
   * At this time, only an implementation for Linux is provided.
   *
   * Don't forget to uncomment the line "#define _USE_PHILIPS_TOUCAM"
   * in the file ltiHardwareConfig.h before compiling, or at least
   * ensure that this symbol is defined.  If you are using the LTI-Lib
   * Makefiles, you can also define that symbol in the
   * <code>ltilib/linux/extrapaths.mk</code> file.
   *
   * Please ensure that the file pwc-ioctl.h is in the standard include path.
   * This file is usually included in your Linux kernel sources
   * (/usr/src/linux/drivers/usb).
   * In order to work, the kernel module pwcx-i386.o is also required.
   * Both files can be found at http://www.smcc.demon.nl/webcam/.
   * To automatically load this compression module, you may want to put it
   * in your /etc/modules.conf:
   *
   * \code
   * post-install pwc /sbin/insmod --force /lib/modules/usb/pwcx-i386.o >/dev/null 2>&1
   * \endcode
   *
   * where /lib/modules/usb should be replaced by the correct path.  The file
   * name also differs and is something like pwcx-2.4.20.o or a newer kernel
   * version.
   *
   * If you want to add more functionality to this camera functor you can
   * visit the programmer's API
   * <a href="http://www.smcc.demon.nl/webcam/api.html">here</a>.
   * And the Video4Linux API can be found
   * <a href="http://roadrunner.swansea.linux.org.uk/v4lapi.shtml">here</a>.
   *
   * \section Building ToUCam Kernel Modules
   *
   * Since there is no further direct support for the philips webcams
   * in the linux kernel, here some useful tips how to build the
   * required modules for newer kernel versions.
   *
   * You will require the kernel sources and headers and the files hopefully
   * still <a href="http://www.smcc.demon.nl/webcam">here</a>.
   * -# copy all pwc*.[hc] of the kernel into the directory
   *    <code>/usr/src/linux/drivers/usb/media/</code>, or wherever you have 
   *    your kernel sources.
   * -# edit the Makefile and Kconfig files in usb/media.  In Kconfig add a
   *    block
   *    \code
   *       config USB_PWC
   *        tristate "USB Philips Cameras"
   *        depends on USB && VIDEO_DEV
   *        ---help---
   *          Say Y or M here if you want to use one of these Philips & OEM
   *            webcams:
   *            * Philips PCA645, PCA646
   *            * Philips PCVC675, PCVC680, PCVC690
   *            * Philips PCVC720/40, PCVC730, PCVC740, PCVC750
   *            * Askey VC010
   *            * Logitech QuickCam Pro 3000, 4000, 'Zoom', 'Notebook Pro' 
   *              and 'Orbit'/'Sphere'
   *            * Samsung MPC-C10, MPC-C30
   *            * Creative Webcam 5, Pro Ex
   *            * SOTEC Afina Eye
   *            * Visionite VCS-UC300, VCS-UM100
   *
   *          The PCA635, PCVC665 and PCVC720/20 are not supported by
   *          this driver and never will be, but the 665 and 720/20 are
   *          supported by other drivers.
   *
   *          This driver has an optional plugin (called PWCX), which
   *          is distributed as a binary module only. It contains code
   *          that * allow you to use higher resolutions and
   *          framerates but may * not be distributed as source. But
   *          even without this plugin * you can these cams for most
   *          applications.
   *
   *          See <file:Documentation/usb/philips.txt> for more information and
   *          installation instructions.
   *
   *          The built-in microphone is enabled by selecting USB
   *          Audio support.
   *
   *          This driver uses the Video For Linux API. You must say Y
   *          or M to "Video For Linux" (under Character Devices) to
   *          use this driver.  
   *
   *          Information on this API and pointers to "v4l" programs
   *          may be found at <file:Documentation/video4linux/API.html>.
   *
   *          To compile this driver as a module, choose M here: the module
   *          will be called pwc.
   *
   *       config USB_PWCX
   *        tristate "PWCX decompreso module for USB Philips Cameras"
   *        depends on USB_PWC
   *    \endcode
   *
   * For the Makefile, you have to add at the beginning
   *
   * \code
   *   pwc-objs	 := pwc-if.o pwc-misc.o pwc-ctrl.o pwc-uncompress.o
   *   pwcx-objs := pwcx-glue.o libpwcx.a
   * \endcode
   * 
   * and later on
   *
   * \code
   *   obj-$(CONFIG_USB_PWC) += pwc.o
   *   obj-$(CONFIG_USB_PWCX) += pwcx.o
   * \endcode
   *
   * When your finished with this editing, you can call
   * \code
   *   make xconfig
   * \endcode
   * to start the configuration of the kernel.  Select the module for
   * the Philips cameras or edit manually the file at
   * <code>/usr/src/linux/arch/i386/defconfig</code> to include the lines
   * 
   * \code
   *   CONFIG_USB_PWC=y
   *   CONFIG_USB_PWCX=y
   * \endcode
   *
   * After calling <code>make modules</code> you can copy the newly created
   * modules <code>pwc*.ko</code> into the corresponding directory in
   * <code>/lib/modules/kernel/drivers/usb/media/</code>. Before you
   * can load the modules with <code>modprobe</code> you have to recreate the
   * module dependencies with <code>depmod</code>.
   */
  class toUCam : public frameGrabber {
  public:
    /**
     * frameGrabber parameters class
     */
    class parameters : public frameGrabber::parameters {
    public:
      /**
       * constructor to generate an invalid set of parameters (all values out
       * of range).
       * if valid is false, the parameter set will be invalid, otherwise
       * a default set will be generated.
       */
      parameters(const bool valid=true);

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
       * the image size
       *
       * The following values are accepted by the camera: 128x96, 160x120,
       * 176x144, 320x240, 352x288 and 640x480.
       *
       * Default value: 320x240
       *
       * Following is an excerpt of the original driver pages:
       *
       * Resolution chart
       *
       * Below is a table that shows the possible resolutions and
       * framerates for the webcams. On the left, all possible
       * physical resolutions are listed; at the top, the various
       * framerates for the cameras.
       *
       * If a cell is blank, it means the combination of resolution
       * and framerate is not directly supported by the webcam. If an
       * unsupported image size is requested by a program, the first
       * smaller image size is selected and padded with a black or
       * gray border.
       *
       * A green cell means the combination is supported uncompressed;
       * a red cell indicates that combination is only available with
       * the decompressor module, <b>pwcx</b>.
       * \htmlonly
         <table border="1">
         <tr>
           <th rowspan="2" valign="bottom">Size</th>
           <th colspan="8">PCA 645/646 &amp; VC010</th>
           <th rowspan="8" width="15">&nbsp;</td>
           <th colspan="6">PCVC 675/680/690</th>
           <th rowspan="8" width="15">&nbsp;</th>
           <th colspan="6">PCVC 730/740</th>
         </tr>

         <tr>
           <th width="30">3.75</th>
           <th width="30">5</th>
           <th width="30">7.5</th>
           <th width="30">10</th>
           <th width="30">12</th>
           <th width="30">15</th>
           <th width="30">20</th>
           <th width="30">24</th>
           <th width="30">5</th>
           <th width="30">10</th>
           <th width="30">15</th>
           <th width="30">20</th>
           <th width="30">25</th>
           <th width="30">30</th>
           <th width="30">5</th>
           <th width="30">10</th>
           <th width="30">15</th>
           <th width="30">20</th>
           <th width="30">25</th>
           <th width="30">30</th>
         </tr>
         <tr>
           <th>sQCIF<br>128x96</th>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         <tr>
           <th>QSIF<br>160x120</th>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
         </tr>
         <tr>
           <th>QCIF<br>176x144</th>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         <tr>
           <th>SIF<br>320x240</th>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
         </tr>
         <tr>
           <th>CIF<br>352x288</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#00cc00">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         <tr>
           <th>VGA<br>640x480</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td bgcolor="#cc0000">&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
           <td>&nbsp;</td>
         </tr>
         </table>
         \endhtmlonly

       * (Yes, VGA resolutions are limited to 15 fps: there isn't
       * enough bandwidth available on the USB bus to squeeze through
       * more, even with compression).
       *
       * The Philips driver now supports any image size between the
       * minimum and maximum size. For image sizes that are not an
       * 'integral' image size directly supported by the camera, the
       * image is now padded with a gray or black border, depending on
       * the palette.
       *
       * Note: the driver only pads the image; it selects a format
       * that is smaller or equal to the requested size; it does not
       * crop an image that is only a bit larger than the requested
       * current image.
       */
      point size;

      /**
       * image brightness (valid values between 0.0 and 1.0)
       *
       * Default value: 0.5
       */
      float brightness;

      /**
       * image hue (valid values between 0.0 and 1.0)
       * This value doesn't seem to be used by the ToUCams, but it
       * belongs to the V4L standard...
       *
       * Default value: 0.5
       */
      float hue;

      /**
       * image color saturation (valid values between 0.0 and 1.0)
       *
       * Default value: 0.5
       */
      float saturation;

      /**
       * image contrast (valid values between 0.0 and 1.0)
       *
       * Default value: 0.5
       */
      float contrast;

      /**
       * gamma correction (valid values between 0.0 and 1.0)
       *
       * Default value: 0.5
       */
      float gamma;

      /**
       * frame rate
       * This value shoud be between 0 and 60.
       * If 0 is given, the last frame rate will be used again.
       * Not all combination of frameRate and size are allowed.  For more
       * details see parameters::size;
       *
       * Default value:
       */
      int frameRate;

      /**
       * snapshot mode
       * Default value:
       */
      bool snapshotMode;

      /**
       * the image compression rate
       */
      enum eCompressionType {
        None   = 0, /**< no compression will be used  */
        Low    = 1, /**< low compression              */
        Medium = 2, /**< middle compression rate      */
        High   = 3, /**< high compression             */
        Invalid = 4 /**< flag to denote invalid value */

      };

      /**
       * Set compression preference.
       *
       * The Philips webcams use losy compression techniques to get the
       * images across the (narrow) USB bus. With this parameter
       * you can specify if you like no compression, low, medium or
       * high compression. Higher compression means less bandwidth is
       * used, but it could introduce artefacts in the image.
       *
       * Default value: Medium
       */
      eCompressionType compression;

      /**
       * automatic gain control
       *
       * Default value: false
       */
      bool agc;

      /**
       * gain
       * valid values between 0.0 and 1.0
       *
       * Default value: 0.75
       */
      float gain;

      /**
       * automatic shutter speed
       *
       * Default value: false
       */
      bool autoShutter;

      /**
       * shutter speed.
       * Only used if autoShutter is false.
       * The value has no time units.  It just means a long exposure time (1.0)
       * or a very short exposure time (0.0).
       *
       * Default value: 0.75
       */
      float shutterSpeed;

      /**
       * white balance mode
       */
      enum eWhiteBalanceType {
        InvalidWB   = -1, /**< Invalid value */
        Indoor      =  0, /**< Indoor lighting  */
        Outdoor     =  1, /**< Outdoor lighting */
        Fluorescent =  2, /**< Fluorescent lighting  */
        Manual      =  3, /**< Manual mode.  You need this mode to adjust the
                               blue and red gains */
        Auto        =  4  /**< Automatic White Balance */
      };

      /**
       * white balance
       *
       * Default value: Outdoor
       */
      eWhiteBalanceType whiteBalance;

      /**
       * Red gain.
       *
       * To see the effect of this gain factor you need to set the white
       * balance mode into "Manual".
       *
       * Valid values between 0.0 and 1.0
       *
       * Default value: 0.5
       */
      float redGain;

      /**
       * Blue gain
       *
       * Valid values between 0.0 and 1.0
       *
       * To see the effect of this gain factor you need to set the white
       * balance mode into "Manual".
       *
       * Default value: 0.5
       */
      float blueGain;

      /**
       * LED
       */
      int led;

      /**
       * This string denotes the name of the device used.  For Linux it
       * could be for example /dev/video or /dev/video0.  There is a special
       * constructor that allows you to give this value directly.
       *
       * Default value: for Linux: /dev/video
       *                for Windows: not implemented yet.
       */
      std::string device;
    };

    /**
     * default constructor
     */
    toUCam(const bool initialize=true);

    /**
     * Constructor with initialization for the given device
     */
    toUCam(const std::string& device,const bool initialize=true);

    /**
     * copy constructor
     */
    toUCam(const toUCam& other);

    /**
     * constructor, sets the parameters
     */
    toUCam(const parameters& theParam);

    /**
     * destructor
     */
    virtual ~toUCam( void );

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * setParameters overload to allow changing the camera attributes
     */
    virtual bool setParameters(const functor::parameters& theParam);

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * load an (color) image from the camera
     */
    bool apply(image& theImage);

    /**
     * load a grey value channel from the camera
     */
    bool apply(channel8& theChannel);

    /**
     * copy member
     */
    toUCam& copy(const toUCam& other);

    /**
     * clone member
     */
    functor* clone() const;

    /**
     * check if the frame grabber has been initialized
     */
    bool isInitialized() const;

    /**
     * initialize frame grabber
     */
    bool init();

  protected:
    /**
     * the actual capture size for the images
     */
    point size;

    /**
     * flag indicates if the camera interface has been initialized
     */
    bool initialized;

    /**
     * name of the camera being used
     */
    std::string camName;

    /**
     * the camera file handle.
     * this is valid only if <code>initialized</code> is true
     */
    int cameraHndl;

    /**
     * type to specify which kind of images (gray or color)
     * was grabbed the last time
     */
    enum eLastGrabbedType {
      NotUsedYet, /**< no grabbing yet       */
      Gray,       /**< gray channel grabbed  */
      Color       /**< color channel grabbed */
    };

    /**
     * last grabbed type
     */
    eLastGrabbedType lastGrabbed;

    /**
     * set the camera parameters
     */
    bool setCameraParameters();

    /**
     * last used parameter values (used to check if a value needs to be
     * changed).  These are needed to spare some time sending the new
     * parameters to the camera
     */
    parameters lastUsedParams;

    /**
     * look up table to accelerate computation of saturation
     */
    static const ubyte* lutSaturation;

    /**
     * look up tables to accelerate conversion Luv -> RGB
     */
    //@{
    static const int* lutUg;
    static const int* lutUb;
    static const int* lutVg;
    static const int* lutVr;
    //@}

    /**
     * initialize the Look-Up-Tables
     */
    void initializeLUTs();
  };
}
#endif // __linux__
#endif // _USE_PHILIPS_TOUCAM
#endif // #ifdef _LTI_TOUCAM_H_


