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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiLeutronFrameGrabber.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 02.01.2003
 * revisions ..: $Id: ltiLeutronFrameGrabber.h,v 1.6 2005/06/16 12:37:11 doerfler Exp $
 */

#ifndef _LTI_LEUTRON_FRAME_GRABBER_H_
#define _LTI_LEUTRON_FRAME_GRABBER_H_

#include "ltiHardwareConfig.h"
#ifdef _USE_LEUTRON_GRABBER

#include "ltiFrameGrabber.h"
#include "ltiImage.h"
#include "lvutil.h"
#include "lvdef.h"
#include "dsylib.h"
#include "grabber.h"

namespace lti {

  /**
   * Interface to all Leutron framegrabbers in the system. If more than one
   * framegrabber is installed, they will be considered all. If framegrabber
   * specific settings like monochrome VS. color or the number targets on the
   * boards vary, the minimal common set is supported. In particular if a
   * monochrome framegrabber is installed together with color framegrabbers
   * all images acquired will be monochrome.
   *
   * This implementation is rather slow because the camera is frozen while
   * grabbing and woke up afterwards, which takes about 15 ms. For a faster
   * access use the leutronMultiPlexFrameGrabber, which is not public yet.
   *
   * For some strange reason the hardware path via the high performance
   * scaler (HPS) is faster than via the binary ratio scaler (BRS).
   *
   * This class is not thread safe. If used as a singleton, thread safety 
   * should be added. It also should be tested that all initialization,
   * camera connection and camera activation methods are only called once.
   *
   * CAVEATS:
   * 1) This class is only tested with a PicPortColor framegrabber.
   *
   * BUGS:
   * 1) If two targets are enabled only the BRS path produces a useful image.
   */
  class leutronFrameGrabber : public frameGrabber {
  public:
    /**
     * the parameters for the class leutronFrameGrabber
     */
    class parameters : public frameGrabber::parameters {
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

#     ifdef _LTI_WIN32 
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
       * The target currently selected (HPS 0, BPS 1, ...). Some
       * framegrabbers like the picport color support two targets, others
       * may have even more targets. If several framegrabbers are installed
       * in the system, the number of targets is limited by the smallest
       * number of targets supported by one of the grabbers. Actually a more
       * sophisticated implementation would not need this implementation,
       * unless multi plexing.
       *
       * The framegrabbers supports two paths: high performance scaling (HPS)
       * or binary ratio scaling (BRS). On the BRS path only YUV and grey
       * formats are possible.
       * If RGB color is choosen it will be set to YUV on the BRS path. 
       * Specifying a targetIndex turns on the according field in the parameter target.
       * Default: 0.
       */
      int targetIndex;

      /**
       * A camera name known to daisy. It must be specified in lvcamera.dat.
       * For Robocup roboter use "PAL_S_CCIR" (full mode) or
       * "PAL_S_CCIR_Halbbild_50" (half mode). For Victoria use
       * "Leutron_LV2000CE" (full mode) or
       * "PAL_S_CCIR_HalbbildLV2000_50" (half mode).
       * All cameras must be of the same type. Default: PAL_S_CCIR
       */
      std::string camera;

      /**
       * The camera currently selected. The hardware supports connection
       * of 4 cameras.
       * Default: 0 (first camera).
       */
      int cameraIndex;

      /**
       * The scale factor of the image acquired by the HPS path. The maximum
       * image size
       * of the camera is detected by the framegrabber. Default: (1,1).
       */
      tpoint<float> scaleFactorHPS;

      /**
       * If true the framegrabber will auto detect the cameras. Otherwise 
       * the grabber will check whether the camera name given in camera is
       * supported and take those settings. This overrides the camera string.
       * Default: true.
       */
      bool autoDetectCamera;

      /**
       * The scale factor of the image acquired by the BRS path. The maximum
       * image size of the camera is detected by the framegrabber.
       * Default: (1,1).
       */
      point scaleFactorBRS;

      enum eWaitSyncType {Field1, Field2, Frame, VertSync, None};
      /**
       * The type synchronization with the camera: no synchronization, with
       * start of field 0, with start of field 1, with start of frame or with
       * vertical sync puls. The default is no synchronization.
       * Default: None.
       */
      eWaitSyncType waitSync;

      /**
       * Offset in grabbed image. Default value is 0. Negative values are not
       * allowed. The values refer to the original image size of the camera.
       * Default: (0,0)
       */
      point offset;

      /**
       * Scale (true) or Clip (false) the grabbed image to specified
       * dimensions. In half image mode, which has higher priority, only
       * scaling is supported. The image mode is specified implicitly by
       * the camera. Default: true.
       */
      bool scale;

      /**
       * Specifies which targets will grabbed by apply(vector<..> ..). For
       * details on the targets see the description of targetIndex.
       * Default: grap HPS-target only.
       */
      bool target[4];

      enum eColorType {RGBa, YUV, Grey};
      /**
       * Grab a RGB-alpha color image, YUV or 8-bit grey. Default: RGBa.
       */
      eColorType color;

    };

    /**
     * the optocouplers that can be set
     */
    enum eOptocouplerType {Deactivate, OPTO_0, OPTO_1, OPTO_2, OPTO_3, OPTO_4,
			   OPTO_5, OPTO_6, OPTO_7, OPTO_8, OPTO_9, OPTO_10,
			   OPTO_11, OPTO_12, OPTO_13, OPTO_14, OPTO_15,
			   OPTO_16, OPTO_17, OPTO_18, OPTO_19, OPTO_20, 
			   OPTO_21, OPTO_22, OPTO_23, OPTO_24, OPTO_25,
			   OPTO_26, OPTO_27, OPTO_28, OPTO_29, OPTO_30, 
			   OPTO_31};

    /**
     * default constructor
     */
    leutronFrameGrabber();

    /**
     * Construct a functor using the given parameters
     * @param the parameters to be used
     */
    leutronFrameGrabber(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    leutronFrameGrabber(const leutronFrameGrabber& other);

    /**
     * destructor
     */
    virtual ~leutronFrameGrabber();

    /**
     * returns the name of this type ("leutronFrameGrabber")
     */
    virtual const char* getTypeName() const;

    /**
     * Grabs a grey image.
     * operates on the given %parameter.
     * @param srcdest where the grabbed channel8 will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest);

    /**
     * Grabs an image from the current camera. 
     * operates on the given %parameter.
     * @param srcdest where the grabbed image will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest);

    /**
     * Grabs an image split in 3 separate channels. This useful if the
     * application distinguishes between chromacity (Y) and color
     * information (U,V).
     * @param band1 where the grabbed Y band will be left
     * @param band2 where the grabbed Y band will be left
     * @param band3 where the grabbed Y band will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& band1, channel8& band2, channel8& band3);

    /**
     * Sets one optocoupler or deactivates all optocouplers of the grabber that
     * has been accessed last. ToDo: think of method where you can tell the
     * grabber on which the optocoupler shall be set.
     * @param theOpto the optocoupler to be set
     * @return true on success or false otherwise.
     */
    bool setOptocoupler(eOptocouplerType theOpto);    

    /**
     * Sets or deactivates the optocouplers of the grabber that has been
     * accessed last. ToDo: think of method where you can tell the
     * grabber on which the optocouplers shall be set.
     * @param theOptos the optocouplers to be set
     * @return true on success or false otherwise.
     */
    bool setOptocoupler(const std::vector<eOptocouplerType>& theOptos);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    leutronFrameGrabber& copy(const leutronFrameGrabber& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    leutronFrameGrabber& operator=(const leutronFrameGrabber& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const leutronFrameGrabber::parameters& getParameters() const;

    /**
     * Set the paramters. Adjusts non supported values internally.
     * @param theParams the parameters to be set
     * @return true if the parameters could be set and processed by the
     *              hardware or false otherwise
     */
    bool setParameters(const leutronFrameGrabber::parameters &theParams);
    
    /**
     * Initializes and opens the framegrabber. It is not necessary to call
     * this method.
     * @return true on success or false otherwise
     */
    virtual bool init();

    /**
     * Switches the current camera. On switch, the camera has to be activated
     * which is time consuming.
     * @param cameraIndes the index of the camera from which will be grabbed
     * @return true on success or false otherwise
     */
    virtual bool setCamera(int cameraIndex);

    /**
     * Switches the target, from which is grabbed. If the target buffer is
     * not already connected to the camera(s) this is time consuming. This
     * method affects the parameters targetIndex and target[].
     * @param cameraIndes the index of the target will be grabbed
     * @return true on success or false otherwise
     */
    virtual bool setTarget(int targetIndex);

    /**
     * Get the number of connected cameras
     * @return the number of connected cameras
     */
    virtual int getNumberOfConnectedCameras();

protected:
    /**
     * returns used parameters
     */
    leutronFrameGrabber::parameters& getWritableParameters();

    /**
     * check parameters
     * @param theParams the parameters to be checked
     * @return true on success or false otherwise
     */
    virtual bool 
      checkParameters(const leutronFrameGrabber::parameters &theParams);

    /**
     * put a camera in live mode and start the acquisition
     * @return true on success or false otherwise
     */
    virtual bool startAquisition() const;

    /**
     * put a camera in freeze mode and stop the acquisition
     * @return true on success or false otherwise
     */
    virtual bool stopAquisition() const;

    /**
     * Detect and connect all cameras
     * @return true on success or false otherwise
     */
    virtual bool detectCameras();

    /**
     * Activate the current camera
     * @return true on success or false otherwise
     */
    virtual bool activateCameras();


    /**
     * Get the leutron error and write it into the status string
     * @param status the leutron status
     * @param error the prefix of the concatenated status string
     * @return true on success or false otherwise
     */
    virtual bool setDsyErrorInStatusString(STATUS& status, const char* error);

    /**
     * Initialize the roi from where the data is copied into a lti object
     * @param roi the leutron region of interest object
     * @return true on success or false otherwise
     */
    virtual bool initRoi(LvROI& roi);

    //pointers to the current grabber
    static LvGrabberNode *m_pGrabber;       
    //stores the grabber handle of each camera 
    static HGRABBER *m_pGrabberHandle;
    //stores the connector of each camera
    static HCONNECTOR *m_pConnectorHandle;  

    static int m_numConnectedCameras;
    static int m_numTargets;
    //will be used when monochrome grabbers will be supported
    static bool m_greyOnly;
    static int m_instanceCount;

    static U16BIT m_masterCameraType;   //camera on master grabber
    static U16BIT m_slaveCameraType;    //camera on slave grabber
    
    static int m_srcWidth;   //width  in the srcInfo
    static int m_srcHeight;  //height in the srcInfo

    static bool m_connect;
    static bool m_activate;
    static bool m_otherCamera;
    static bool m_turnaround;
    static U8BIT m_tgt[4];
   
    //ToDo:   Do I need a static copy of the parameters ???
    //Answer: At least which are important for the internal status,
    //        i.e. hardware settings !!!
    //        But this can wait until semaphores are added and a singleton
    //        for testing exists
 
  private:
    /**
     * set mirror mode
     */
    virtual bool turnaround();
  };
}

#endif //USE_LEUTRON_GRABBER

#endif



