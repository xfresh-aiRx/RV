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
 * file .......: ltiScalarFunctor.h
 * authors ....: Thomas Rusert, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 20.04.99
 * revisions ..: $Id: ltiScalarFunctor.h,v 1.5 2003/02/17 07:16:45 alvarado Exp $
 */

#ifndef _LTI_SCALARFUNCTOR_H_
#define _LTI_SCALARFUNCTOR_H_

#include "ltiIteratingFunctor.h"

namespace lti {

  /**
   * Base class for all scalar functors. \\
   * These functors have similar functionality as unaryFunctors, but
   * instead giving the functor object to the 'apply'-Method of the
   * mathObject (e.g. matrix or vector), it receives the mathObject as
   * argument in its 'apply'-method.  Scalar functors are usually
   * faster than unaryFunctors, because for the last ones a call to a
   * virtual function is done for each element in the mathObject.
   * This function call is in the scalar functors not requiered.
   */
  template<class T>
  class scalarFunctor : public iteratingFunctor<T> {
  public:
    /**
     * scalar value parameter needed in scalar functors
     */
    class parameters : public iteratingFunctor<T>::parameters {
    public:
      /// default constructor
      parameters(const T& val = T());
      /// copy constructor
      parameters(const parameters& other);
      /// copy member
      parameters& copy(const parameters& other);
      /// returns pointer to a clone of this object
      virtual functor::parameters* clone() const;
      /// name of this type
      virtual const char* getTypeName() const;

      /// scalar value
      T value;
    };

    /// default constructor
    scalarFunctor(const T& val = T());
    /// constructor to set the parameters
    scalarFunctor(const parameters& p);
    /// return the casted parametes of this functor
    const parameters& getParameters() const;

  };

}

#include "ltiScalarFunctor_template.h"

#endif

