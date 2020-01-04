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
 * file .......: ltiQuickCam.h
 * authors ....: Peter Mathes
 * organization: LTI, RWTH Aachen
 * creation ...: 13.08.99
 * revisions ..: $Id: ltiQuickCam.h,v 1.13 2004/05/03 13:30:10 alvarado Exp $
 */

#ifndef _LTI_QUICKCAM_H_
#define _LTI_QUICKCAM_H_

#include "ltiHardwareConfig.h"

#ifdef _USE_QUICKCAM_GRABBER

#include "ltiFrameGrabber.h"
#include "ltiImage.h"

// Unix systems implementation
#ifdef __linux__

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <camera.h>
#include <imager.h>
#include <rcfile.h>

namespace lti {

  /**
   * class to access the quick cam camera system (for LINUX only!)
   */
  class quickCam : public frameGrabber {
  public:

    /**
     * parameters for the quick-cam functor
     */
    class parameters : public frameGrabber::parameters  {
      public:

      /**
       * constructor
       */
      parameters();

      /**
       * copy member
       */
      parameters& copy( const parameters& other );

      /**
       * copy constructor
       */
      parameters( const parameters& other ) 
        : frameGrabber::parameters() {
        copy( other );
      };

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this class
       */
      virtual const char* getTypeName() const;

      // the parameters ---------------------------------------------

      /**
       * which port to find the camera on (0 = probe 0x378, 0x278, and 0x3bc)
       */
      int cameraPort;

      /** which detection mode to use (see camera.h for details)
           - 0 = no detection
           - 1 = okay if either succeeds    (default)
           - 2 = try Patrick's scheme only
           - 3 = try Kenny's scheme only
           - 4 = okay only if both schemes succeed
           - 2 and 3 are faster than 1, but are less likely to succeed.
           - Is 4 even worth having?  :)
       */
      int detectionMode;

      /** automatically adjust brightness and color balance on startup?
          - 1=yes,
          - 0=no
       */
      int autoAdjust;

      /**
       * use 24 or 32 bits per pixel?
       */
      int bpp;

      /**
       * use 1:1, 2:1, or 4:1 decimation bits per pixel?
       */
      int decimation;

      /** set it if you want auto-despeckling. Despeckling can cause a very
          small amount of blurring in some pictures. (Better than the speckles,
          I promise :)
          - 1=yes,
          - 0=no
       */
      int despeckleMode;

      int x, y;
    };

    /**
     * default constuctor
     */
    quickCam();

    /**
     * construct, set parameters and initialize frame grabber
     */
    quickCam(const quickCam::parameters& theParams);

    /**
     * destructor
     */
    virtual ~quickCam( void );

    /**
     * returns the current parameters
     */
    const quickCam::parameters& quickCam::getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName( void ) const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    bool setParameters( const functor::parameters& theParams );

    /**
     * inits the cam
     * @return true if successful, false otherwise
     */
    bool init();

    /**
     * check if the quick cam is active
     */
    bool isActive() const;


    /**
     * get frame
     */
    bool apply( image& theImage );

    bool apply(channel8& theChannel);

    bool apply(channel& theChannel);

  private:

    // which detection mode to use (see camera.h for details)
    //   0 = no detection
    //   1 = okay if either succeeds    (default)
    //   2 = try Patrick's scheme only
    //   3 = try Kenny's scheme only
    //   4 = okay only if both schemes succeed
    // 2 and 3 are faster than 1, but are less likely to succeed.  Is 4 even
    // worth having?  :)
    int detectionMode;

    // which port to find the camera on (0 = probe 0x378, 0x278, and 0x3bc)
    // int cameraPort;
    //
    // automatically adjust brightness and color balance on startup?
    //  1=yes, 0=no
    // int auto_adjust;
    //
    // use 24 or 32 bits per pixel?
    // int bpp;
    //
    // use 1:1, 2:1, or 4:1 decimation bits per pixel?
    // int decimation;
    //
    // set it if you want auto-despeckling. Despeckling can cause a very
    //  small amount of blurring in some pictures. (Better than the speckles,
    // I promise :)   1=yes, 0=no
    // int despeckle_mode;
    //
    int upperBound, lowerBound;

    camera_t *camera;

    /**
     * frame size x
     */
    int xSize;

    /**
     * frame width
     */
    int width;

    /**
     * frame size y
     */
    int ySize;

    /**
     * frame height
     */
    int  height;

  };
}  //namespace lti

#endif  // Unix systems

#endif  // _USE_QUICKCAM_GRABBER

#endif	// _LTI_QUICKCAM_H_


