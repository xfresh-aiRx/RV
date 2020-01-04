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
 * file .......: ltiSemaphore.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * revisions ..: $Id: ltiSemaphore.h,v 1.10 2005/11/24 14:24:03 doerfler Exp $
 */

#ifndef _LTI_SEMAPHORE_H_
#define _LTI_SEMAPHORE_H_

#include "ltiObject.h"

#ifndef _LTI_WIN32
#  include <semaphore.h>
#else
#  include <windows.h>
#  include <process.h>
#  define SEM_VALUE_MAX 16777215 // (2^32-1)
#endif

namespace lti {
  /** lti semaphore class for inter-thread (not inter-process!) synchronisation
      @see lti::mutex, lti::thread
   */
  class semaphore : public object {
  public:
    /// default constructor
    semaphore(const int initialValue = 1);

    /// destructor
    virtual ~semaphore();

    /**
     * wait on semaphore, i.e. decrease the value or wait if counter <= 0
     *
     * @return true if successful, false otherwise
     */
    bool wait();

    /**
     * Try to wait on semaphore, but do not block.
     *
     * @return true if value was decreased.
     */
    bool tryWait();

    /**
     * post semaphore, i.e. increase the value
     *
     * @return true if successful, false otherwise
     */
    bool post();

    /// get current value
    int getValue();

    /// reset value to initialValue
    void reset();

    /// returns the name of this type
    virtual const char* getTypeName() const;

  protected:
    void destroy();

    int initValue;

#if defined(_LTI_MACOSX)
    sem_t *osxSemaphore;
    int osxSemNumber;
    char myName[40];
#elif defined(_LTI_WIN32)
    /// the WIN32 semaphore object
    HANDLE theSemaphore;
    int counter;
#else 
    /// the posix semaphore
    sem_t theSemaphore;
# endif
  };
}

#endif

