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
 * file .......: ltiReadWriteLock.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 10.11.99
 * revisions ..: $Id: ltiReadWriteLock.h,v 1.7 2005/11/24 14:24:03 doerfler Exp $
 */

#ifndef _LTI_READ_WRITE_LOCK_H_
#define _LTI_READ_WRITE_LOCK_H_

#if defined(_LTI_LINUX) || defined(_LTI_MACOSX)
#include "ltiObject.h"
#include <pthread.h>

namespace lti {

  /// lti read-write synchronisation class
  class readWriteLock : public object {
  public:

    /// default constructor
    readWriteLock();

    /// destructor
    virtual ~readWriteLock();

    /**
     * wait until read-write lock becomes available and lock it for read
     * access
     */
    void lockRead() const;

    /**
     * Try to lock, but do not block.
     * Return true if locking was successful.
     */
    bool tryLockRead() const;

    /**
     * wait until read-write lock becomes available and lock it for write
     * access
     */
    void lockWrite() const;

    /**
     * Try to lock, but do not block.
     * Return true if locking was successful.
     */
    bool tryLockWrite() const;

    /// unlock read-write lock from read or write access
    void unlock() const;

    /// returns the name of this type
    virtual const char* getTypeName() const;
  protected:
    void destroy();

    mutable pthread_rwlock_t theLock;
  };
}

#endif
#endif

