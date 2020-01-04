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
 * file .......: ltiSimilarityFunctor_template.h
 * authors ....: Stefan Syberichs, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiSimilarityFunctor_template.h,v 1.8 2004/05/03 13:30:10 alvarado Exp $
 */

//TODO: include files
//#include "ltiSmilarityFunctor.h"

namespace lti {
  // --------------------------------------------------
  // similarityFunctor::parameters
  // --------------------------------------------------

  // implementation in header file due to MS-VC++ bug

  // --------------------------------------------------
  // similarityFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  similarityFunctor<T>::similarityFunctor()
    : linearAlgebraFunctor(){
  }

  // copy constructor
  template <class T>
  similarityFunctor<T>::similarityFunctor(const similarityFunctor<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  similarityFunctor<T>::~similarityFunctor() {
  }

  // returns the name of this type
  template <class T> const char*
  similarityFunctor<T>::getTypeName() const {
    return "similarityFunctor";
  }

  // copy member
  template <class T> similarityFunctor<T>&
  similarityFunctor<T>::copy(const similarityFunctor<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  // return parameters
  template <class T> const typename similarityFunctor<T>::parameters&
    similarityFunctor<T>::getParameters() const {
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

  // pure virtual
}


