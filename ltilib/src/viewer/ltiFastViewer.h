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
 * file .......: ltiViewer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.4.2000
 * revisions ..: $Id: ltiFastViewer.h,v 1.17 2005/11/24 14:24:03 doerfler Exp $
 */

#ifndef _LTI_FAST_VIEWER_H_
#define _LTI_FAST_VIEWER_H_

#ifdef _LTI_WIN32
#include "ltiViewer.h"
#else
#include "ltiViewerBase.h"
#include "ltiImage.h"
#include "ltiException.h"
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/extensions/XShm.h>
#include <sys/shm.h>
#include <X11/keysym.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#endif

#ifdef _LTI_MACOSX
// this doesn't get defined on OS X with _XOPEN_SOURCE/_POSIX_C_SOURCE set
extern int getdomainname (char*, int);
#endif

namespace lti {

#ifdef _LTI_WIN32
  typedef viewer fastViewer;
#else
  /**
   * Use objects of this class to display your data in a window.
   *
   * At this moment this works on Linux only!!  For windows the GTK viewer
   * will be used instead...
   *
   * This viewer works only on true-color modes of the X-Server.  So please
   * set your configuration to 24 or 32 bit-depth.  If your program crashes,
   * remember to catch all std::exception and lti::exception, and ask for
   * a reason with their what() method.
   *
   * The use of the lti::fastViewer is very simple.  Each viewer administrates
   * one window, and this window will exist as long as the viewer object
   * exists.  This means, if you destroy the viewer-object, the window will
   * be automatically destroyed.
   *
   * If used in the localhost, a shared memory block will be used to transfer
   * the data between the application and the window.  If used through the
   * X-Protocoll then this mechanism cannot be used and "normal" memory blocks
   * will serve for the data transfer.
   *
   * To use the viewer see following example:
   *
   * \code
   *
   * #include "ltiImage.h"
   * #include "ltiSplitImage.h"
   * #include "ltiFastViewer.h"
   * #include "ltiBMPFunctor.h"
   *
   * ...
   *
   * lti::image img;                // an image
   * lti::channel chnl;             // a channel
   * lti::splitImageTorgI splitter; // a functor to get the intensity channel
   *
   * lti::loadBMP loader;
   *
   * loader.load("yourImage.bmp",img); // load the image in the given file
   * splitter.getIntensity(img,chnl);  // get the intensity channel of the img
   *
   * lti::fastViewer view1,view2;
   *
   * view1.show(img);                  // open a window and show the image
   * view2.show(chnl);                 // open a new window and show the
   *                                   // intensity channel.
   *
   * ...
   *
   * // until the end of the execution block the images will be displayed.
   * // When view1 and view2 are destroyed, their windows will disappear!
   *
   * \endcode
   *
   */
  class fastViewer : public viewerBase {
  public:
    /**
     * the parameters for the class viewer
     */
    class parameters : public viewerBase::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * copy data of "other" parameters
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

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
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------
      // the parameters
      // ------------------------

      /**
       * top left corner of the viewer window. Default is (0,0)
       */
      point topleft;

      /**
       * If true, a borderless window is created.
       */
      bool noBorder;
    };

    /**
     * default constructor
     * @param winName title of the viewer window
     */
    fastViewer(const char* winName = "lti::fastViewer");

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fastViewer(const fastViewer& other);

    /**
     * destructor
     */
    virtual ~fastViewer();

    /**
     * returns the name of this type ("fastViewer")
     */
    virtual const char* getTypeName() const;

    /**
     * Sets the position of the window. This method will have an
     * effect only if the window has not yet been opened.
     */
    void setPosition(const point& pos);

    /**
     * shows an lti::image
     * @param data the object to be shown.
     */
    bool show(const image& data);

    /**
     * shows a 8-bit channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel8& data);

    /**
     * shows a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const channel& data);

    /**
     * shows a channel or matrix of float
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<float>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<double>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<float>& data);

    /**
     * shows a vector of double
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const vector<int>& data);

    /**
     * shows a matrix of doubles as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<double>& data);

    /**
     * shows a matrix of integers as a channel
     * @param data the object to be shown.
     * @return true if successful, false otherwise.
     */
    virtual bool show(const matrix<int>& data);

    /**
     * hides the display window
     */
    bool hide();

    /**
     * copy data of "other" functor.
     */
    fastViewer& copy(const fastViewer& other);

    /**
     * copy data of "other" functor.
     */
    fastViewer& operator=(const fastViewer& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual viewerBase* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set the parameters to be used
     */
    virtual bool setParameters(const viewerBase::parameters& param);


  protected:
    /**
     * x-stuff structure
     */
    struct display_info_s {
      display_info_s();
      Display* display;
      GC       gc;
      Window   win;
      int      width;
      int      height;
      int      depth;
      XImage  *shmimage;
    };

    /**
     * window manager stuff
     */
    struct winman_info_s {
      winman_info_s();
      Atom motif_wm_hints;
    };

    winman_info_s wm_info;

    /**
     * The useShareMemory variable indicates if the faster data transfer
     * through the MIT XShm extention should take place or not.
     * This member is automatically initialized in the contructor, depending
     * on where the X-Server is located.
     */
    bool useShareMemory;

    /**
     * shared memory info block
     */
    XShmSegmentInfo shminfo;

    /**
     * XWindows data
     */
    display_info_s display_info;

    /**
     * window title
     */
    std::string title;

    /**
     * the actual image being displayed
     */
    image data;

    /**
     * the data being displayed if a remote server is in use
     */
    char* remoteData;

    /**
     * initialize window
     */
    void createWindow();

    /**
     * destroy window
     */
    void destroyWindow();

    /**
     * create XImage
     */
    void createImage(const image& img);

    /**
     * destroy XImage
     */
    void destroyImage();
  };
#endif



}

#endif

