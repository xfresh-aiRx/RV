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
 */

#ifndef _LTI_FRAMEGRABBER_H_
#define _LTI_FRAMEGRABBER_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiHardwareConfig.h"

namespace lti {
  /**
   * frame grabber class.
   * class for grabbing frames from cameras and
   * converting them to ltiImage format.
   */
  class frameGrabber : public functor {
  public:
    /**
     * frameGrabber parameters class
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : functor::parameters(),turnaround(false) {
      };

      /**
       * copy constructor
       */
      parameters( const parameters& other ) 
        : functor::parameters() {
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


      /**
       * If true, the grabber will flip the image before it is
       * returned to the caller. This is useful for upside-down mounted
       * cameras.
       */
      bool turnaround;

    };

    /**
     * class for frame grabber exceptions
     */
    class frameGrabberException : public lti::exception {
    public:
      /**
       * default constructor
       */
      frameGrabberException(const char* msg = "frame grabber exception")
        : lti::exception(msg) {
      };
    };

    /**
     * default constructor
     */
    frameGrabber() 
      : functor() {};

    /**
     * copy constructor
     */
    frameGrabber(const frameGrabber& other);

    /**
     * constructor, sets the parameters
     */
    frameGrabber(const parameters& theParam);

    /**
     * destructor
     */
    virtual ~frameGrabber( void );

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
    virtual bool apply(image& theImage)=0;

    /**
     * load a grey value channel from the
     */
    virtual bool apply(channel8& theChannel)=0;

    /**
     * load a grey value channel from the
     */
    virtual bool apply(channel& theChannel);

    /**
     * check if the frame grabber / camera system is active
     */
    virtual bool isActive() const;

    /**
     * initialize frame grabber
     * @return true if successful, false otherwise
     */
    virtual bool init() = 0;

    /**
     * copy member
     */
    frameGrabber& copy(const frameGrabber& other);
  };
}

#endif


