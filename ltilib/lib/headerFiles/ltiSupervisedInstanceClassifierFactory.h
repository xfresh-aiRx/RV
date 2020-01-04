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
 * file .......: ltiSupervisedInstanceClassifierFactory.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2002
 * revisions ..: $Iog: ltiSupervisedInstanceClassifierFactory.h,v $
 */

#ifndef _LTI_SUPERVISED_INSTANCE_CLASSIFIER_FACTORY
#define _LTI_SUPERVISED_INSTANCE_CLASSIFIER_FACTORY

#include "ltiSupervisedInstanceClassifier.h"
#include "ltiObjectFactory.h"

namespace lti {

  /**
   * This class defines a factory for classifiers.
   *
   */
  class supervisedInstanceClassifierFactory: public object {

  public:
    /**
     * Constructor.
     */
    supervisedInstanceClassifierFactory();

    /**
     * Destructor.
     */
    virtual ~supervisedInstanceClassifierFactory();

    /**
     * returns the name of this type ("supervisedInstanceClassifierFactory")
     */
    virtual const char* getTypeName() const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual supervisedInstanceClassifier* newInstance(const char *name) const;

    /**
     * Creates a new instance of the class whose name is given
     * as parameter, if this class is known to the factory.
     * Otherwise, it returns null.
     * @param name name of the to-be instantiated class.
     * @return a new instance.
     */
    virtual supervisedInstanceClassifier* newInstance(const std::string& name) const {
      return newInstance(name.c_str());
    }

  private:

    /**
     * supervisedInstanceClassifiers used in the factory
     */
    static const supervisedInstanceClassifier *const  classifiers[];

    /**
     * the real object factory
     */
    static objectFactory<supervisedInstanceClassifier> factory;

  };


}

#endif
