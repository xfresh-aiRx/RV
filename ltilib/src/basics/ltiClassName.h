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
 * file .......: ltiClassName.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 01.04.99
 * revisions ..: $Id: ltiClassName.h,v 1.6 2004/12/13 13:17:54 doerfler Exp $
 */

#ifndef _LTI_CLASSNAME_H_
#define _LTI_CLASSNAME_H_

#include "ltiObject.h"
#include <string>

namespace lti {
  /**
   * Provides methods for getting the class names of lti::objects
   */
  class className : public object {
  public:
    /**
     * default constructor
     */
    className();

    /**
     * destructor
     */
    ~className();

    /**
     * returns the name of this type
     * (obtained by using the std::typeinfo functions).
     * The returned string considers the template arguments and
     * is usually fully qualified.
     * The implementation should ensure that the returned name has
     * the following format:
     * \<namespace\>::\<classname\>::\<innerclassname\>::...
     * Example:
     * <pre>
     * lti::boundingBox<double>::parameters
     * </pre>
     * WARNING: The function returns a pointer to an internal class
     * member. So, the pointer is only valid for the lifetime of this
     * object or until the next time get is called, whatever occurs
     * first.
     */
    const char* get(const object* o) const;

    /**
     * @see get(object*)
     */
    const char* get(const object& o) const {
      return get(&o);
    }

    /**
     * returns the name of this type
     * (obtained by using the std::typeinfo functions).
     * The returned string considers the template arguments and
     * is usually fully qualified.
     * @see get(object*)
     * The name is returned in the result parameter. This is a much
     * safer method than the method returning a pointer.
     */
    void get(const object& o, std::string& result) const {
      result=get(o);
    }

    /**
     * returns the name of this type (obtained by using the
     * std::typeinfo functions).  The returned string considers the
     * template arguments and is usually fully qualified. 
     * @see get(object*)
     * The name is returned in the result parameter. This
     * is a much safer method than the method returning a pointer.
     */
    void get(const object* o, std::string& result) const {
      result=get(o);
    }
    
    /**
     * decodes a typename returned by typeid().
     * You can use this to get decoded names for objects that are
     * not derived from lti::object
     * @see get(object*)
     */
    std::string decode(const std::string& name) const;
    

  protected:
    /**
     * implementation of the native-to-readable decoding of
     * class names.
     */
    void decode(char* dest, int& dpos, const char *src, int& spos) const;

    mutable char* buffer;

  };

}

#endif






/// Unix

