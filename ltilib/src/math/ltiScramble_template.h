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
 * file .......: ltiScramble.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.8.2000
 * revisions ..: $Id: ltiScramble_template.h,v 1.12 2004/05/03 12:20:02 alvarado Exp $
 */

#include <ctime>
#include "ltiMath.h"

namespace lti {
  // --------------------------------------------------
  // scramble<T>::parameters
  // --------------------------------------------------

  // implementation in header, due to MS VC++ bug

  // --------------------------------------------------
  // scramble
  // --------------------------------------------------

  // default constructor
  template <class T>
  scramble<T>::scramble()
    : functor(), randomizer() {
  }

  // copy constructor
  template <class T>
  scramble<T>::scramble(const scramble& other)
    : functor(), randomizer() {
    copy(other);
  }

  // destructor
  template <class T>
  scramble<T>::~scramble() {
  }

  // returns the name of this type
  template <class T>
  const char* scramble<T>::getTypeName() const {
    return "scramble";
  }

  // copy member
  template <class T>
  scramble<T>& scramble<T>::copy(const scramble<T>& other) {
      functor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* scramble<T>::clone() const {
    return new scramble(*this);
  }

  // return parameters
  template <class T>
  const typename scramble<T>::parameters&
  scramble<T>::getParameters() const {
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


  // On place apply for type matrix<T>!
  template <class T>
  bool scramble<T>::apply(matrix<T>& srcdest) const {
    vector<T> tmpvct;
    tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                         &srcdest.at(0,0),
                         true);

    return apply(tmpvct);

  };

  // On place apply for type vector<T>!
  template <class T>
  bool scramble<T>::apply(vector<T>& srcdest) const {

    int i,j;
    int k;
    const int sz=srcdest.size();

    for (k=0; k<sz; k++) {

      i=static_cast<int>(randomizer.draw()*sz);
      j=static_cast<int>(randomizer.draw()*sz);

      exchange(srcdest.at(i),srcdest.at(j));
    }

    return true;
  };

  // On copy apply for type matrix<T>!
  template <class T>
  bool scramble<T>::apply(const matrix<T>& src,matrix<T>& dest) const {

    dest.copy(src);
    return apply(dest);

  };

  // On copy apply for type vector<T>!
  template <class T>
  bool scramble<T>::apply(const vector<T>& src,vector<T>& dest) const {

    dest.copy(src);
    return apply(dest);
  };

}


