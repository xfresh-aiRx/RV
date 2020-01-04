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
 * file .......: ltiThread.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 03.11.99
 * revisions ..: $Id: ltiThread.h,v 1.8 2005/06/16 12:37:49 doerfler Exp $
 */

#ifndef _LTI_THREAD_H_
#define _LTI_THREAD_H_

#include "ltiObject.h"
#include "ltiMutex.h"
#include "ltiSemaphore.h"

#ifndef _LTI_WIN32
#  include <pthread.h>
#else
#  include <winbase.h>
#endif

namespace lti {

  /**
   * lti thread class.
   * representiation of a system thread (currently a posix thread for Unix
   * systems, and WIN32 thread for windows systems)
   *
   * If you need a thread, just inherit from this class and reimplement the
   * member run:
   * \code
   * class myThread : public thread {
   * protected:
   *   void run() {
   *     // your thread's job here!
   *   }
   * };
   *
   * ...
   *
   * // somewhere else in your code:
   *
   * myThread aThread;
   * aThread.start(); // do your threads job...
   * // continue with other things (your thread runs in parallel...)
   * \endcode
   */
  class thread : public object {
  public:
    /**
     * default constructor
     */
    thread();

    /**
     * destructor.
     * If the thread is still executing while destroying the thread object,
     * the thread will be forced to stop. Use join() to wait for the thread
     * to terminate itself.
     */
    virtual ~thread();

    /**
     * start thread
     */
    virtual void start();

    /**
     * forces the thread to stop executing.
     * overload this function to cleanup things run leaves after
     * cancellation.
     */
    virtual void stop();

    /**
     * wait for thread termination
     */
    virtual void join();

    /**
     * test if the thread is alive
     */
    bool isAlive() const;

    /**
     *  returns whether the thread object represents the called thread
     *  i.e. true if called within the run() method.
     */
    bool representsCalledThread() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

  protected:
    /**
     * method to be called when starting the thread;
     * contains the job the thread has to execute
     */
    virtual void run() = 0;

    /**
     * method to be called when finishing regulary or cancelling the thread;
     * something like a destructor for the run() method
     */
    virtual void cleanUp() {};

  private:
    static void clean(void* threadObject);
    bool alive;
    static mutex startMutex;
    // semaphore used to join called thread with the calling one
    semaphore suspendSem;

# ifndef _LTI_WIN32 // UNIX/LINUX
    static void* execute(void* threadObject);
    pthread_t theThread;
# else          // WINDOWS
    static void execute(void* threadObject);
    // windows handle of the thread
    HANDLE theThread;

    // id of the called thread
    unsigned int calledThreadId;

# endif
  };

}

#endif

