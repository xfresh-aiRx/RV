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
 * file .......: ltiGtkServer.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 11.11.2001
 * revisions ..: $Id: ltiGtkServer.h,v 1.4 2003/02/17 07:17:11 alvarado Exp $
 */

#ifndef _LTI_GTK_SERVER_H_
#define _LTI_GTK_SERVER_H_

#include "ltiConfig.h"

#ifdef HAVE_GTK

#include <gtk/gtk.h>

#include "ltiThread.h"
#include "ltiSemaphore.h"
#include "ltiMutex.h"

namespace lti {

  /**
   * Server to administrate gtk GUI.
   */
  class gtkServer : public object {
  public:
    /**
     * default constructor
     */
    gtkServer();

    /**
     * destructor
     */
    virtual ~gtkServer();

    /**
     * start the server.
     * @return true if successful or if already started, false if
     * something went wrong.
     */
    bool start();

    /**
     * stop the server.
     *
     * It will not be possible to restart the server after this.  You can
     * stop the server before the end of the application, to close all
     * GTK widgets and its main loop.
     * @return true if successful, of false otherwise.
     */
    static bool shutdown();

    /**
     * return true if the gtk-server is active (alive) or false otherwise
     */
    static bool isAlive();

  protected:
    /**
     * An instance of this class will be the GUI server object, called
     * gtkServer
     */
    class guiThread : public thread {
    public:
      /**
       * default constructor
       */
      guiThread();

      /**
       * destructor
       */
      virtual ~guiThread();

      /**
       * wait until the initialization of the gui toolkit
       * is ready
       */
      void start();

      /**
       * stop the gui-thread.  Once stopped, it cannot be restarted due
       * to a GDK limitation :-(
       */
      virtual void stop();

    protected:
      /**
       * the job to be independently executed in a new thread
       */
      virtual void run();

      /**
       * clean up the run() method
       */
      virtual void cleanUp();

      /**
       * initializes GUI toolkit
       */
      static void toolkitInit();

      /**
       * enters event handling routine of the toolkit
       */
      static void toolkitMainLoop();

      /**
       * wait until initialization of toolkit is ready
       */
      static void waitUntilInitReady();

      /**
       * semaphore used to indicate if initialization is ready
       */
      static semaphore initReady;

      /**
       * timeout gtk-callback function, called as first when the
       * main-loop has been initialized!
       */
      static gint initializationTimeout(gpointer data);

      /**
       * Stop the gtk-application
       */
      static gint applicationEnd(gpointer data);

    };

    /**
     * the gui server (just one for all gui elements...)
     */
    static guiThread theThread;

    /**
     * flag to indicate if server thread is already on
     */
    static bool alreadyStarted;

    /**
     * mutex to protect flag alreadyStarted
     */
    static mutex lock;

    /**
     * static method called at the end of the application, to ensure
     * the proper finalization of the GTK main loop.
     */
    static void toolkitAbort();
  };

}

#endif
#endif

