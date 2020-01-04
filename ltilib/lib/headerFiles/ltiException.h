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
 * file .......: ltiException.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 07.04.99
 * revisions ..: $Id: ltiException.h,v 1.6 2003/02/17 07:13:53 alvarado Exp $
 */

#ifndef _LTI_EXCEPTION_H
#define _LTI_EXCEPTION_H

#include "ltiObject.h"
#include <exception>
#include <string>

namespace lti {
  /**
   * Base class for all lti-library exceptions
   */
  class exception
    : public object,public std::exception {
    public:
    /**
     * constructor.
     * @param excName name of the exception.  These string will be copied and
     *                can be accessed with the "what()" method
     */
    exception(const char* excName = "exception");

    /**
     * constructor.
     * @param excName name of the exception.  These string will be copied and
     *                can be accessed with the "what()" method
     */
    exception(const std::string& excName);

    /**
     * copy constructor
     */
    exception(const exception& other);

    /**
     * destructor
     */
    virtual ~exception() throw ();

    /**
     * name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * name of the exception
     */
    virtual const char* what() const throw ();

    /**
     * copy member
     */
    inline exception& copy(const exception& other) {
      // call the standard copy member:
      (*dynamic_cast<std::exception*>(this))=other;
      exceptionName = other.what();
      return (*this);
    };

    /**
     * clone this object
     */
    virtual exception* clone() const;

    /**
     * alias for copy operator!
     */
    inline exception& operator=(const exception& other) {
      return copy(other);
    };

    protected:
    /**
     * the exception message.
     * This string will be returned with the method what()
     */
    std::string exceptionName;
  };

  /**
   * allocException is thrown if a memory allocation problem occurs
   */
  class allocException : public exception  {
  public:
    allocException() : exception("memory allocation error") {};
    virtual const char* getTypeName() const;
  };
}

#endif

