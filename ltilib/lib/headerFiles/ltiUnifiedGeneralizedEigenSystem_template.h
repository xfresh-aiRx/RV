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
 * file .......: ltiUnifiedGeneralizedEigenSystem.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 21.11.2003
 * revisions ..: $Id: ltiUnifiedGeneralizedEigenSystem_template.h,v 1.5 2005/06/16 12:37:35 doerfler Exp $
 */

//TODO: include files
#include "ltiUnifiedGeneralizedEigenSystem.h"

#ifdef HAVE_LAPACK
#include "ltiFastGeneralizedEigenSystem.h"
#endif

namespace lti {
  // --------------------------------------------------
  // unifiedGeneralizedEigenSystem::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // unifiedGeneralizedEigenSystem
  // --------------------------------------------------

  // default constructor
  template<class T>
  unifiedGeneralizedEigenSystem<T>::unifiedGeneralizedEigenSystem()
    : linearAlgebraFunctor(), eigen(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template<class T>
  unifiedGeneralizedEigenSystem<T>::unifiedGeneralizedEigenSystem(const parameters& par)
    : linearAlgebraFunctor(), eigen(0) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  unifiedGeneralizedEigenSystem<T>::unifiedGeneralizedEigenSystem(const unifiedGeneralizedEigenSystem<T>& other)
    : linearAlgebraFunctor(), eigen(0) {
    copy(other);
  }

  // destructor
  template<class T>
  unifiedGeneralizedEigenSystem<T>::~unifiedGeneralizedEigenSystem() {
    delete eigen;
  }

  // returns the name of this type
  template<class T>
  const char* unifiedGeneralizedEigenSystem<T>::getTypeName() const {
    return "unifiedGeneralizedEigenSystem<T>";
  }

  // copy member
  template<class T>
  unifiedGeneralizedEigenSystem<T>&
  unifiedGeneralizedEigenSystem<T>::copy(const unifiedGeneralizedEigenSystem<T>& other) {
    linearAlgebraFunctor::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  template<class T>
  unifiedGeneralizedEigenSystem<T>&
  unifiedGeneralizedEigenSystem<T>::operator=(const unifiedGeneralizedEigenSystem<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* unifiedGeneralizedEigenSystem<T>::clone() const {
    return new unifiedGeneralizedEigenSystem<T>(*this);
  }

  // return parameters
  template<class T>
  const typename unifiedGeneralizedEigenSystem<T>::parameters&
  unifiedGeneralizedEigenSystem<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool unifiedGeneralizedEigenSystem<T>::updateParameters() {

    bool b=true;
    const parameters& theParam = getParameters();

#ifdef HAVE_LAPACK
    switch (theParam.esType) {
      case parameters::GeneralizedEigenSystem: {
        eigen=new generalizedEigenSystem<T>();
        typename generalizedEigenSystem<T>::parameters p;
        p.sort = theParam.sort;
        p.dimensions = theParam.dimensions;
        b = b && eigen->setParameters(p);
        break;
      }
      case parameters::FastGeneralizedEigenSystem: {
        eigen=new fastGeneralizedEigenSystem<T>();
        typename fastGeneralizedEigenSystem<T>::parameters p;
        p.sort = theParam.sort;
        p.dimensions = theParam.dimensions;
        p.useDC = theParam.useDC;
        p.problemType = fastGeneralizedEigenSystem<T>::parameters::Type1;
        b = b && eigen->setParameters(p);
        break;
      }
    }
#else
    switch (theParam.esType) {
      case parameters::FastGeneralizedEigenSystem:
        setStatusString("Illegal value of parameter esType of unifiedGeneralizedEigenSystem. fastGeneralizedEigenSystem can only be used if LAPACK is available. Using generalizedEigenSystem.\n");
        b=false;
      case parameters::GeneralizedEigenSystem: {
        eigen=new generalizedEigenSystem<T>();
        typename generalizedEigenSystem<T>::parameters p;
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
  bool unifiedGeneralizedEigenSystem<T>::read(ioHandler& handler,
                                              const bool complete) {

    // have to call setParameters with the just loaded parameters to
    // ensure correct allocation of eigen
    bool b=linearAlgebraFunctor::read(handler,complete);
    b = b && setParameters(getParameters());
    return b;

  }


}

