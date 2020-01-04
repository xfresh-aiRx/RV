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
 * file .......: ltiSplitImageFactory.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 04.06.2003
 * revisions ..: $Id: ltiSplitImageFactory.h,v 1.2 2003/11/14 14:15:20 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_FACTORY
#define _LTI_SPLIT_IMAGE_FACTORY

#include "ltiSplitImage.h"
#include "ltiObjectFactory.h"

namespace lti {

  /**
   * This class defines a factory for functors that split color images in
   * their color components.
   */
  class splitImageFactory: public object {

  public:
    /**
     * Constructor.
     */
    splitImageFactory();

    /**
     * Destructor.
     */
    virtual ~splitImageFactory();

    /**
     * returns the name of this type ("splitImageFactory")
     */
    virtual const char* getTypeName() const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual splitImage* newInstance(const char *name) const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual splitImage* newInstance(const std::string& name) const {
      return newInstance(name.c_str());
    }

  private:

    /**
     * edge detectors used in the factory
     */
    static const splitImage *const splitImages[];

    /**
     * the real object factory
     */
    static objectFactory<splitImage> factory;

  };


}

#endif

