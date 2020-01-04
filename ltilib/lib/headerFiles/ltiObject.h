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
 * file .......: ltiObject.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.04.99
 * revisions ..: $Id: ltiObject.h,v 1.12 2005/02/02 14:09:25 doerfler Exp $
 */

#ifndef _LTI_OBJECT_H_
#define _LTI_OBJECT_H_

#include "ltiMacroSymbols.h"

#ifdef _LTI_MSC_6
#pragma warning(disable:4786)
#endif

namespace lti {
  /**
   * Base class for all lti objects
   */
  class object {
  public:
    /**
     * default constructor
     */
    object();

    /**
     * destructor
     */
    virtual ~object();

    /**
     * returns the name of this class. Each class has to
     * overload this function and return its name. The returned
     * string is system-independent; however, template classes
     * do not consider the template argument.
     */
    virtual const char* getTypeName() const;

  protected:
    /**
     * this flag indicates if any lti-object has been constructed
     */
    static bool ltiLibInitialized;

    /**
     * member which initialize everything for the ltilib
     */
    void initialize();
  };

  inline bool isNull(const void* p) {return p == 0;};
  inline bool notNull(const void* p) {return p != 0;};

}

#endif
