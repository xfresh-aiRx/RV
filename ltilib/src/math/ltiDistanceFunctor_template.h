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
 * revisions ..: $Id: ltiDistanceFunctor_template.h,v 1.10 2004/05/03 12:20:01 alvarado Exp $
 */

//TODO: include files
//#include "ltiDistanceFunctor.h"

namespace lti {
  // --------------------------------------------------
  // distanceFunctor::parameters
  // --------------------------------------------------

  // see header file (again a Visual Bug...)


  // --------------------------------------------------
  // distanceFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  distanceFunctor<T>::distanceFunctor()
    : linearAlgebraFunctor(){

    parameters param;
    setParameters(param);
  }

  // copy constructor
  template <class T>
  distanceFunctor<T>::distanceFunctor(const distanceFunctor<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  distanceFunctor<T>::~distanceFunctor() {
  }

  // returns the name of this type
  template <class T> const char*
  distanceFunctor<T>::getTypeName() const {
    return "distanceFunctor";
  }

  // copy member
  template <class T> distanceFunctor<T>&
  distanceFunctor<T>::copy(const distanceFunctor<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  // clone member
//    template <class T> functor*
//    distanceFunctor<T>::clone() const {
//      return new distanceFunctor(*this);
//    }

  // return parameters
  template <class T>
  const typename distanceFunctor<T>::parameters&
  distanceFunctor<T>::getParameters() const {
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

//    // On copy apply for type vector<T>!
//    template <class T> T distanceFunctor<T>::apply(const vector<T>& a,
//                                                const vector<T>& b) const {
//      // TODO: Implement your code HERE!
//      return T(0);
//    };

//    // On copy apply for type matrix<T>!
//    template <class T> T distanceFunctor<T>::apply(const matrix<T>& a,
//                                                const matrix<T>& b) const {
//      // TODO: Implement your code HERE!

//      return T(0);
//    };

}


