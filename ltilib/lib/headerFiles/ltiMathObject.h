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
 * file .......: ltiMathObject.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.04.99
 * revisions ..: $Id: ltiMathObject.h,v 1.13 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_MATHOBJECT_H_
#define _LTI_MATHOBJECT_H_

// #include "ltiObject.h"
// #include <iostream>
// #include "ltiAssert.h"
// #include "ltiException.h"
#include "ltiIoObject.h"
// #include "ltiTypes.h"
#include "ltiIoHandler.h"
// #include "ltiMath.h"

namespace lti {

  /**
   * Base object for the math-classes.
   *
   * Derived classes must provide:
   * - The clone() method
   * - The read(ioHandler,const bool complete) method
   * - The write(ioHandler,const bool complete) const method
   *
   * It is recommended that the derived classes define the type value_type,
   * but it is not absolutely necessary.
   */
  class mathObject : public ioObject {
  public:
    /**
     * returns the name of this class
     */
    virtual const char* getTypeName() const {
      return "mathObject";
    };

    /**
     * returns a copy of this object
     */
    virtual mathObject* clone() const = 0;

    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,
                       const bool complete = true) const;

    /**
     * read the object from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);
  };

  /**
   * write the mathObject in the given ioHandler.
   * The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool write(ioHandler& handler,const mathObject& mo,
             const bool complete = true);

  /**
   * read the mathObject from the given ioHandler.
   * The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool read(ioHandler& handler,mathObject& mo,
             const bool complete = true);
}

#endif

