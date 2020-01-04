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
 * file .......: ltiObjectFactory.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 28.6.2000
 * revisions ..: $Id: ltiObjectFactory.h,v 1.5 2003/05/05 06:09:29 alvarado Exp $
 */

#ifndef _LTI_OBJECT_FACTORY
#define _LTI_OBJECT_FACTORY

#include "ltiObject.h"
#include "ltiMutex.h"
#include "ltiClassName.h"
#include <map>
#include <string>

namespace lti {

  /**
   * This class defines a factory for objects.
   * The instantiation of this class will fail if the template
   * argument T is not a class which defines the clone() method.
   * Usually, you will use an instance of this class as a class variable
   * (C jargon: static class member).
   * This class is thread-safe.
   *
   * Please note that the C-type lists used in the constructor must be
   * null-terminated.
   */
  template <class T>
  class objectFactory: public object {

  public:
    /**
     * Constructor. Creates an object factory which is able to
     * handle the types whose names and prototypes are given in these
     * two tables. <code>names</code> is an array of (char*)  which must be
     * terminated by a null pointer. <code>objs</code> is a corresponding array
     * of object pointers, and must also be null-terminated.
     * Example:
     * \code
     * const char* names[]={ "object1", "object2", "object3", 0 };
     * const obj_type* prototypes[] = {
     *   new object1(), new object2(), new object3(), 0 };
     * objectFactor<obj_type> t(names,prototypes);
     * \endcode
     * @param names array of pointers to const char buffers. It must be
     *              null-terminated
     * @param objs array of pointers to const objects.
     *             It must be null-terminated.
     */
    objectFactory(const char** names,
                  const T* const* objs);

    /**
     * Constructor.
     * Creates an object factory which is able to
     * handle the types whose prototypes are given in this
     * table. The parameter is a null-terminated array of
     * pointers.
     *
     * The names of the prototypes are determined via the
     * className functor.
     *
     * Example:
     *
     * \code
     * const obj_type* prototypes[] = {
     *   new object1(), new object2(), new object3(), 0 };
     * objectFactor<obj_type> t(prototypes);
     * \endcode
     * @param objs array of pointers to const objects.  It must be
     *             null-terminated.
     */
    objectFactory(const T* const* objs);

    /**
     * Destructor.
     */
    virtual ~objectFactory();

    /**
     * returns the name of this type ("objectFactory")
     */
    virtual const char* getTypeName() const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual T* newInstance(const char *name) const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual T* newInstance(const std::string& name) const {
      return newInstance(name.c_str());
    }

  private:
    // locks the table for concurrent accesses
    mutable mutex lock;

    // definition for the map from class names to prototypes
    // this previously used a const string as key, which makes sense,
    // but unfortunately is not ANSI compliant
    typedef std::map<std::string, const T*> classMap;

    // the the map from class names to prototypes
    classMap objMap;

    // the functor which gets the default class names
    className cn;

  };

}

#include "ltiObjectFactory_template.h"

#endif

