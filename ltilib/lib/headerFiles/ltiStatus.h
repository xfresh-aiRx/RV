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
 * file .......: ltiStatus.h
 * authors ....: 
 * organization: LTI, RWTH Aachen
 * creation ...: 01.07.04
 * revisions ..: $Id: ltiStatus.h,v 1.2 2005/02/02 14:09:25 doerfler Exp $
 */

#ifndef _LTI_STATUS_H_
#define _LTI_STATUS_H_

#ifdef _LTI_MSC_6
#pragma warning(disable:4786)
#endif

namespace lti {
  /**
   * Base class for all lti objects that can have a status text.
   * The original interface of this class was previously embedded
   * in the lti::functor class, but has been moved in a separate class
   * in order to allow objects other than functors to have a status.
   */
  class status {
  public:
    /**
     * default constructor
     */
    status();

    /**
     * destructor
     */
    virtual ~status();
    /**
     * return the last message set with setStatusString().  This will
     * never return 0.  If no status-string has been set yet an empty string
     * (pointer to a string with only the char(0)) will be returned.
     */
    virtual const char* getStatusString() const;

    /**
     * set a status string.
     *
     * @param msg the const string to be reported next time by
     * getStatusString(). The given string will be copied.
     * This message will be usually set within the apply methods to indicate
     * an error cause.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void setStatusString(const char* msg) const;

    /**
     * append a message to the current status string. Take care to
     * reset the status string by calling setStatusString() for each
     * call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg message to be appended to the current status string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const char* msg) const;

    /**
     * append an integer value to the current status string. Take care
     * to reset the status string by calling setStatusString() for
     * each call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg integer value to be appended to the current status
     * string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const int& msg) const;

    /**
     * append a double value to the current status string. Take care
     * to reset the status string by calling setStatusString() for
     * each call of an apply() or similar method. appendStatusString()
     * should only be used after setStatusString() has been called.
     *
     * @param msg double value to be appended to the current status
     * string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const double&  msg) const;

    /**
     * Append the status string of another %functor to this functors
     * status string. To achieve better readability of the resulting
     * message a new line is started with the other functor's name and
     * the message.
     *
     * @param other %functor whose status string is to be append to this
     *        status string.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void appendStatusString(const status& other) const;

  private:
    /**
     * the status string written with setStatusString
     */
    mutable char* statusString;

    /**
     * the empty string returned if the statusString is empty
     */
    static const char *const emptyString;

  };
}

#endif
