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
 * file .......: ltiDistanceFunctor_template.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 28.6.2000
 * revisions ..: $Id: ltiKernelFunctor_template.h,v 1.9 2005/02/02 14:09:59 doerfler Exp $
 */

//TODO: include files
//#include "ltiDistanceFunctor.h"

namespace lti {
  // --------------------------------------------------
  // kernelFunctor::parameters
  // --------------------------------------------------

#if 0
  // This code is now in the header file, because MSVC6 cannot compile
  // and link it properly

  // default constructor
  template <class T>
  kernelFunctor<T>::parameters::parameters()
    : functor::parameters() {

  }

  // copy constructor
  template <class T>
  kernelFunctor<T>::parameters::parameters(const parameters& other)
    : functor::parameters() {
    copy(other);
  }

  // destructor
  template <class T>
  kernelFunctor<T>::parameters::~parameters() {
  }

  // get type name
  template <class T> const char*
  kernelFunctor<T>::parameters::getTypeName() const {
    return "kernelFunctor::parameters";
  }

  // copy member

  template <class T>
  kernelFunctor<T>::parameters&
  kernelFunctor<T>::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    functor::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    functor::parameters&
      (functor::parameters::* p_copy)
      (const functor::parameters&) =
      functor::parameters::copy;
    (this->*p_copy)(other);
# endif

    return *this;
  }

  // clone member
  template <class T> functor::parameters*
  kernelFunctor<T>::parameters::clone() const {
    return new parameters(*this);
  }
#endif

  // --------------------------------------------------
  // kernelFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  kernelFunctor<T>::kernelFunctor()
    : functor(){
  }

  // copy constructor
  template <class T>
  kernelFunctor<T>::kernelFunctor(const kernelFunctor<T>& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class T>
  kernelFunctor<T>::~kernelFunctor() {
  }

  // returns the name of this type
  template <class T> const char*
  kernelFunctor<T>::getTypeName() const {
    return "kernelFunctor";
  }

  // copy member
  template <class T> kernelFunctor<T>&
  kernelFunctor<T>::copy(const kernelFunctor<T>& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  template <class T> functor*
  kernelFunctor<T>::clone() const {
    return 0;
  }

  // return parameters
  template <class T>
  const typename kernelFunctor<T>::parameters&
  kernelFunctor<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type vector<T>!
  //template <class T> T kernelFunctor<T>::apply(const vector<T>& a,
  //                                            const vector<T>& b) const {
    // TODO: Implement your code HERE!
  //  return T(0);
  //};


}


