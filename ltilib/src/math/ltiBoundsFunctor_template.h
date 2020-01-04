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
 * file .......: ltiMeansFunctor.cpp
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiBoundsFunctor_template.h,v 1.9 2004/05/03 12:20:01 alvarado Exp $
 */

//TODO: include files
#include "ltiMeansFunctor.h"

namespace lti {
  // --------------------------------------------------
  // boundsFunctor::parameters
  // --------------------------------------------------

  // see header file (MSVC bug...)

  // --------------------------------------------------
  // boundsFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  boundsFunctor<T>::boundsFunctor()
    : statisticsFunctor() {
    parameters p;
    setParameters(p);
  }

  // copy constructor
  template <class T>
  boundsFunctor<T>::boundsFunctor(const boundsFunctor& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  boundsFunctor<T>::~boundsFunctor() {
  }

  // returns the name of this type
  template <class T> const char* boundsFunctor<T>::getTypeName() const {
    return "boundsFunctor";
  }

  // copy member
  template <class T>
  boundsFunctor<T>&
    boundsFunctor<T>::copy(const boundsFunctor<T>& other) {
      statisticsFunctor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* boundsFunctor<T>::clone() const {
    return new boundsFunctor<T>(*this);
  }

  // return parameters
  template <class T>
  const typename boundsFunctor<T>::parameters&
  boundsFunctor<T>::getParameters() const {
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

  // On copy apply for type matrix<T>!
  template <class T>
  void boundsFunctor<T>::apply(const matrix<T>& src,
                               vector<T>& min,
                               vector<T>& max) const {
    vector<T> tmp;
    if (getParameters().rowWise) {
      boundsOfRows(src,min,max);
    } else {
      boundsOfColumns(src,min,max);
    }
  };

  template <class T>
  void boundsFunctor<T>::minOfRows(const matrix<T>& src,
                                   vector<T>& dest) const {
    dest.resize(src.columns(),T(0),false,false);
    vector<T> tmp;
    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      dest[i]=tmp.minimum();
    }
  }

  template <class T>
  void boundsFunctor<T>::minOfColumns(const matrix<T>& src,
                                      vector<T>& dest) const {
    dest.resize(src.rows(),T(0),false,false);
    for (int i=0; i<src.rows(); i++) {
      dest[i]=src.getRow(i).minimum();
    }
  }

  template <class T>
  void boundsFunctor<T>::maxOfRows(const matrix<T>& src,
                                   vector<T>& dest) const {
    dest.resize(src.columns(),T(0),false,false);
    vector<T> tmp;
    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      dest[i]=tmp.maximum();
    }
  }

  template <class T>
  void boundsFunctor<T>::maxOfColumns(const matrix<T>& src,
                                      vector<T>& dest) const {
    dest.resize(src.rows(),T(0),false,false);
    for (int i=0; i<src.rows(); i++) {
      dest[i]=src.getRow(i).maximum();
    }
  }


  template <class T>
  void boundsFunctor<T>::boundsOfRows(const matrix<T>& src,
                                      vector<T>& min,
                                      vector<T>& max) const {
    min.resize(src.columns(),T(0),false,false);
    max.resize(src.columns(),T(0),false,false);
    vector<T> tmp;
    for (int i=0; i<src.columns(); i++) {
      src.getColumnCopy(i,tmp);
      max[i]=tmp.maximum();
      min[i]=tmp.minimum();
    }
  }

  template <class T>
  void boundsFunctor<T>::boundsOfColumns(const matrix<T>& src,
                                         vector<T>& min,
                                         vector<T>& max) const {
    min.resize(src.rows(),T(0),false,false);
    max.resize(src.rows(),T(0),false,false);
    for (int i=0; i<src.rows(); i++) {
      min[i]=src.getRow(i).minimum();
      max[i]=src.getRow(i).maximum();
    }
  }

  template <class T>
  void boundsFunctor<T>::min(const vector<T>& a, const vector<T>& b,
                             vector<T> &result) const {
    assert(a.size() == b.size());

    result.resize(a.size());

    typename vector<T>::const_iterator ia=a.begin();
    typename vector<T>::const_iterator ib=b.begin();
    typename vector<T>::iterator ir=result.begin();
    typename vector<T>::const_iterator aend=a.end();
    while (ia != aend) {
      *ir++=lti::min(*ia++,*ib++);
    }
  }

  template <class T>
  void boundsFunctor<T>::max(const vector<T>& a, const vector<T>& b,
                             vector<T> &result) const {
    assert(a.size() == b.size());

    result.resize(a.size());

    typename vector<T>::const_iterator ia=a.begin();
    typename vector<T>::const_iterator ib=b.begin();
    typename vector<T>::iterator ir=result.begin();
    typename vector<T>::const_iterator aend=a.end();
    while (ia != aend) {
      *ir++=lti::max(*ia++,*ib++);
    }
  }

  template <class T>
  void boundsFunctor<T>::clip(vector<T>& a,
                              const vector<T>& lower,
                              const vector<T>& upper) const {
    assert(lower.size() == upper.size());
    assert(a.size() == upper.size());
    typename vector<T>::const_iterator ilow=lower.begin();
    typename vector<T>::const_iterator iup=upper.begin();
    typename vector<T>::iterator ir=a.begin();
    typename vector<T>::iterator rend=a.end();
    while (ir != rend) {
      if (*ir < *ilow) {
        *ir=*ilow;
      } else if (*ir > *iup) {
        *ir=*iup;
      }
      ir++;
      ilow++;
      iup++;
    }
  }

  template <class T>
  void boundsFunctor<T>::clip(matrix<T>& a,
                              const matrix<T>& lower,
                              const matrix<T>& upper) const {
    assert(lower.size() == upper.size());
    assert(a.size() == upper.size());
    typename matrix<T>::const_iterator ilow=lower.begin();
    typename matrix<T>::const_iterator iup=upper.begin();
    typename matrix<T>::iterator ir=a.begin();
    typename matrix<T>::iterator rend=a.end();
    while (ir != rend) {
      if (*ir < *ilow) {
        *ir=*ilow;
      } else if (*ir > *(this->ihigh)) {
        *ir=*(this->ihigh);
      }
      ir++;
      ilow++;
      this->ihigh++;
    }
  }

}


