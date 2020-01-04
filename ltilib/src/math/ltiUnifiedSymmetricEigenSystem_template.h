/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiUnifiedSymmetricEigenSystem.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 18.11.2003
 * revisions ..: $Id: ltiUnifiedSymmetricEigenSystem_template.h,v 1.6 2005/11/04 15:34:55 doerfler Exp $
 */

//TODO: include files
#include "ltiUnifiedSymmetricEigenSystem.h"

#ifdef HAVE_LAPACK
#include "ltiFastEigenSystem.h"
#endif

namespace lti {
  // --------------------------------------------------
  // unifiedSymmetricEigenSystem::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // unifiedSymmetricEigenSystem
  // --------------------------------------------------

  // default constructor
  template<class T>
  unifiedSymmetricEigenSystem<T>::unifiedSymmetricEigenSystem()
    : linearAlgebraFunctor(), eigen(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template<class T>
  unifiedSymmetricEigenSystem<T>::unifiedSymmetricEigenSystem(const parameters& par)
    : linearAlgebraFunctor(), eigen(0) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  unifiedSymmetricEigenSystem<T>::unifiedSymmetricEigenSystem(const unifiedSymmetricEigenSystem<T>& other)
    : linearAlgebraFunctor(), eigen(0) {
    copy(other);
  }

  // destructor
  template<class T>
  unifiedSymmetricEigenSystem<T>::~unifiedSymmetricEigenSystem() {
    delete eigen;
  }

  // returns the name of this type
  template<class T>
  const char* unifiedSymmetricEigenSystem<T>::getTypeName() const {
    return "unifiedSymmetricEigenSystem<T>";
  }

  // copy member
  template<class T>
  unifiedSymmetricEigenSystem<T>&
  unifiedSymmetricEigenSystem<T>::copy(const unifiedSymmetricEigenSystem<T>& other) {
      linearAlgebraFunctor::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class T>
  unifiedSymmetricEigenSystem<T>&
  unifiedSymmetricEigenSystem<T>::operator=(const unifiedSymmetricEigenSystem<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* unifiedSymmetricEigenSystem<T>::clone() const {
    return new unifiedSymmetricEigenSystem<T>(*this);
  }

  // return parameters
  template<class T>
  const typename unifiedSymmetricEigenSystem<T>::parameters&
  unifiedSymmetricEigenSystem<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool unifiedSymmetricEigenSystem<T>::updateParameters() {
 
    bool b=true;
    const parameters& theParam = getParameters();

#ifdef HAVE_LAPACK
    switch (theParam.esType) {
      case parameters::Jacobi: {
        eigen=new jacobi<T>();
        typename jacobi<T>::parameters p;
        p.sort = theParam.sort;
        p.dimensions = theParam.dimensions;
        b = b && eigen->setParameters(p);
        break;
      }
      case parameters::FastEigenSystem: {
        eigen=new fastEigenSystem<T>();
        typename fastEigenSystem<T>::parameters p;
        p.sort       = theParam.sort;
        p.dimensions = theParam.dimensions;
        p.useRRR     = theParam.useRRR;
        b = b && eigen->setParameters(p);
        break;
      }
    }
#else
    switch (theParam.esType) {
      case parameters::FastEigenSystem:
        setStatusString("Illegal value of parameter esType of unifiedSymmetricEigenSystem. fastEigenSystem can only be used if LAPACK is available. Using jacobi.\n");
        b=false;
      case parameters::Jacobi: {
        eigen=new jacobi<T>();
        typename jacobi<T>::parameters p;
        p.sort = theParam.sort;
        p.dimensions = theParam.dimensions;
        b = b && eigen->setParameters(p);
        break;
      }
    }
#endif

    return b;
  }

  template<class T>
  bool unifiedSymmetricEigenSystem<T>::read(ioHandler& handler,
                                            const bool complete) {

    // have to call setParameters with the just loaded parameters to
    // ensure correct allocation of eigen
    bool b=linearAlgebraFunctor::read(handler,complete);
    b = b && setParameters(getParameters());
    return b;

  }


}

