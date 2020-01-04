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
 * file .......: ltiTransformEstimatorFactory.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 18.4.2004
 * revisions ..: $Id: ltiTransformEstimatorFactory.h,v 1.1 2004/04/30 18:00:23 goenner Exp $
 */

#ifndef _LTI_TRANSFORM_ESTIMATOR_FACTORY
#define _LTI_TRANSFORM_ESTIMATOR_FACTORY

#include "ltiTransformEstimatorType.h"
#include "ltiTransformEstimator.h"
#include "ltiObjectFactory.h"

namespace lti {

  /**
   * This class defines a factory for transform estimator functors.
   *
   * The applications should check if this functor
   * returns a valid instance (not null pointer).
   */
  class transformEstimatorFactory: public object {

  public:
    /**
     * Constructor.
     */
    transformEstimatorFactory();

    /**
     * Destructor.
     */
    virtual ~transformEstimatorFactory();

    /**
     * returns the name of this type ("transformEstimatorFactory")
     */
    virtual const char* getTypeName() const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual transformEstimator* newInstance(const char *name) const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual transformEstimator* newInstance(const std::string& name) const {
      return newInstance(name.c_str());
    }

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name type of the to-be instantiated class.
     * @return a new instance.
     */
    virtual transformEstimator* 
      newInstance(eTransformEstimatorType name) const;

  private:

    /**
     * transform estimators used in the factory
     */
    static const transformEstimator *const  transformEstimators[];

    /**
     * the real object factory
     */
    static objectFactory<transformEstimator> factory;

  };


}

#endif

