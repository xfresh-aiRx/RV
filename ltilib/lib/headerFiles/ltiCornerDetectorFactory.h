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
 * file .......: ltiCornerDetectorFactory.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2002
 * revisions ..: $Id: ltiCornerDetectorFactory.h,v 1.2 2003/02/17 07:15:18 alvarado Exp $
 */

#ifndef _LTI_CORNER_DETECTOR_FACTORY
#define _LTI_CORNER_DETECTOR_FACTORY

#include "ltiCornerDetector.h"
#include "ltiObjectFactory.h"

namespace lti {

  /**
   * This class defines a factory for corner detector functors.
   *
   * Depending on the way the library was compiled, the susanCorners functor
   * will be included or not.  The applications should check if this functor
   * returns a valid instance (not null pointer).
   */
  class cornerDetectorFactory: public object {

  public:
    /**
     * Constructor.
     */
    cornerDetectorFactory();

    /**
     * Destructor.
     */
    virtual ~cornerDetectorFactory();

    /**
     * returns the name of this type ("cornerDetectorFactory")
     */
    virtual const char* getTypeName() const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual cornerDetector* newInstance(const char *name) const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual cornerDetector* newInstance(const std::string& name) const {
      return newInstance(name.c_str());
    }

  private:

    /**
     * corner detectors used in the factory
     */
    static const cornerDetector *const  cornerDetectors[];

    /**
     * the real object factory
     */
    static objectFactory<cornerDetector> factory;

  };


}

#endif

