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
 * file .......: ltiCosinusSimilarity_template.h
 * authors ....: Stefan Syberichs
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiCosinusSimilarity_template.h,v 1.9 2004/05/03 13:30:09 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {

  // --------------------------------------------------
  // cosinus2 similarity
  // --------------------------------------------------

  // default constructor

  template <class T>
  cosinus2Similarity<T>::cosinus2Similarity()
    : similarityFunctor<T>(){
  }

  // copy constructor

  template <class T>
  cosinus2Similarity<T>::cosinus2Similarity(const cosinus2Similarity<T>& other) {
    copy(other);
  }

  // destructor
  template <class T>
  cosinus2Similarity<T>::~cosinus2Similarity() {
  }

  // returns the name of this type
  template <class T>
  const char*  cosinus2Similarity<T>::getTypeName() const {
    return "cosinus2Similarity";
  }

  // copy member
  template <class T>
  cosinus2Similarity<T>& cosinus2Similarity<T>::copy(const cosinus2Similarity<T>& other) {
    similarityFunctor<T>::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* cosinus2Similarity<T>::clone() const {
    return new cosinus2Similarity(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On copy apply for type vector<T>!
  template <class T>
  T cosinus2Similarity<T>::apply(const vector<T>& a,
                                 const vector<T>& b) const {

    if (a.size() != b.size()) {
      return T(-1);
    };

    T denom = a.dot(a) * b.dot(b);

    T num = a.dot(b);
    num = num*num;

    return static_cast<T>(num/denom);
  }

  // On copy apply for type matrix<T>!
  template <class T>
  T cosinus2Similarity<T>::apply(const matrix<T>& a,
                                 const matrix<T>& b) const {

    if (a.size() != b.size()) {
      return T(-1);
    }

    vector<T> va,vb;
    const int size = a.rows()*a.columns();

    if (a.getMode() == matrix<T>::Connected) {
      va.useExternData(size,const_cast<T*>(&a.at(0)));
    } else {
      va.resize(size,T(),false,false);
      matrix<T> ca(a);
      va.fill(&ca.at(0,0));
    }

    if (b.getMode() == matrix<T>::Connected) {
      vb.useExternData(size,const_cast<T*>(&b.at(0)));
    } else {
      vb.resize(size,T(),false,false);
      matrix<T> cb(b);
      vb.fill(&cb.at(0,0));
    }

    return apply(va,vb);
  }

  // On copy apply for type matrix<T>!
  template <class T>
  vector<T>& cosinus2Similarity<T>::apply(const matrix<T>& a,
                                          const vector<T>& b,
                                                vector<T>& dest) const {

    if (a.columns() != b.size()) {
      dest.clear();
    }

    if (getParameters().rowWise) {
      dest.resize(a.rows(),T(),false,false);
      for (int i=0;i<a.rows();++i) {
        dest.at(i) = apply(a.getRow(i),b);
      }
    } else {
      vector<T> v;
      dest.resize(a.columns(),T(),false,false);
      for (int i=0;i<a.columns();++i) {
        a.getColumnCopy(i,v);
        dest.at(i) = apply(v,b);
      }
    }

    return dest;
  }

}

