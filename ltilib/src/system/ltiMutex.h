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
 * file .......: ltiMutex.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * revisions ..: $Id: ltiMutex.h,v 1.6 2003/10/30 10:03:41 alvarado Exp $
 */

#ifndef _LTI_MUTEX_H_
#define _LTI_MUTEX_H_

#include "ltiObject.h"

#ifndef _LTI_WIN32
#  include <pthread.h> // for unix/linux systems (posix threads!)
#else
#  include <windows.h>
#  include <process.h>
#endif

namespace lti {
  /** lti mutex class

    This object can be used to protect critical sections on
    multithreaded applications.  The same thread should NOT try
    to lock the mutex more than once.  The behavior of this will
    depend on the operating system:  on linux/unix the thread will
    be locked forever (posix standard); on windows, the thread
    will count how many lock have been done, but it will not be
    blocked by the later locks!

    @see lti::semaphore, lti::thread
	*/
  class mutex : public object {
  public:
    /// default constructor
    mutex();
    /// destructor
    virtual ~mutex();
    /// wait until lock for mutex becomes available and lock it
    void lock();
    /** Try to lock mutex, but do not block.
        @return true if locking was successful. */
    bool tryLock();
    /// unlock mutex
    void unlock();

    /// returns the name of this type
    virtual const char* getTypeName() const;
  protected:
    void destroy();

# ifndef _LTI_WIN32
    /// the posix mutex object
    pthread_mutex_t theMutex;
# else
  	/// the WIN32 mutex objects
    HANDLE theMutex;
# endif
  };
}

#endif

