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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiScalarFunctor_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 20.04.99
 * revisions ..: $Id: ltiScalarFunctor_template.h,v 1.10 2005/02/02 14:16:23 doerfler Exp $
 */

namespace lti {
  // implementation of the parameters for the scalar functor

  // parameters default constructor

  template<class T>
    scalarFunctor<T>::parameters::parameters(const T& val)
    : iteratingFunctor<T>::parameters(),value(val) {
  };

  // parameters copy constructor

  template<class T>
    scalarFunctor<T>::parameters::parameters(const parameters& other) {
    copy(other);
  }

  // parameters copy member

  template<class T>
  typename scalarFunctor<T>::parameters&
  scalarFunctor<T>::parameters::copy(const parameters& other) {
    // copy members of upper classes
#   ifndef _LTI_MSC_6
    // for normal  ANSI C++
    iteratingFunctor<T>::parameters::copy(other);
#   else
    // this doesn't work with MS-VC++ 6.0 (an once again... another bug)
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    iteratingFunctor<T>::parameters&
      (iteratingFunctor<T>::parameters::* p_copy)
      (const iteratingFunctor<T>::parameters&) =
      iteratingFunctor<T>::parameters::copy;
    (this->*p_copy)(other);
#   endif
    // copy members of this class
    value = other.value;
    return (*this);
  }

  // parameters clone member

  template<class T>
    functor::parameters*
    scalarFunctor<T>::parameters::clone() const {
    return parameters(*this);
  }

  // returns name of this type

  template<class T>
    const char* scalarFunctor<T>::parameters::getTypeName() const {
    return "scalarFunctor::parameters";
  };

  // Implementation of the scalar functor members

  // default constructor

  template<class T>
  scalarFunctor<T>::scalarFunctor(const T& val) {
    parameters p;
    p.value = val;
    setParameters(p);
  }

  // constructor to set the parameters

  template<class T>
  scalarFunctor<T>::scalarFunctor(const parameters& p) {
    setParameters(p);
  }

  // returns the casted parameters

  template<class T>
  const typename scalarFunctor<T>::parameters&
  scalarFunctor<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&iteratingFunctor<T>::getParameters());
    if(params==0) throw invalidParametersException(getTypeName());
    return *params;
  }
}

