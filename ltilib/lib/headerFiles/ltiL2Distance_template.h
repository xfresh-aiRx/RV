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
 * file .......: ltiL2Distance_template.h
 * authors ....: Jochen Wickel, Pablo Alvarado, Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 28.6.2000
 * revisions ..: $Id: ltiL2Distance_template.h,v 1.6 2004/05/03 12:20:01 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {

  // --------------------------------------------------
  // l2Distance
  // --------------------------------------------------

  // default constructor
  template <class T>
  l2Distance<T>::l2Distance()
    : distanceFunctor<T>() {
  }

  // copy constructor
  template <class T>
  l2Distance<T>::l2Distance(const l2Distance& other)
    : distanceFunctor<T>() {
    copy(other);
  }

  // destructor
  template <class T>
  l2Distance<T>::~l2Distance() {
  }

  // returns the name of this type
  template <class T>
  const char* l2Distance<T>::getTypeName() const {
    return "l2Distance";
  }

  // copy member
  template <class T>
  l2Distance<T>& l2Distance<T>::copy(const l2Distance<T>& other) {
    distanceFunctor<T>::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* l2Distance<T>::clone() const {
    return new l2Distance(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // norm of vector
  template <class T>
  bool l2Distance<T>::apply(const vector<T>& v, T& norm) const {

    norm=T(0);
    typename vector<T>::const_iterator it, end;
    end=v.end();
    for (it=v.begin(); it!=end; ++it) {
      norm+=(*it)*(*it);
    }
    norm=sqrt(norm);
    return true;
  }

  // norm of vector
  template <class T>
  T l2Distance<T>::apply(const vector<T>& v) const {

    T tmp;
    apply (v, tmp);

    return tmp;
  }

  // norms of matrix rows or columns
  template <class T>
  bool l2Distance<T>::apply(const matrix<T>& m, vector<T>& norms) const {

    int i=0;
    const int r=m.rows();
    const int c=m.columns();
    const parameters& param=this->getParameters();
    typename vector<T>::const_iterator it;
    typename vector<T>::const_iterator ite;
    typename vector<T>::iterator nit;
    typename vector<T>::iterator nite;

    if (param.rowWise) {
      norms.resize(r, T(0),false, true);
      nit = norms.begin();
      nite = norms.end();
      for ( ; nit!=nite; ++nit) {
        it = m.getRow(i).begin();
        ite = m.getRow(i).end();
        for ( ; it!=ite; ++it) {
          *nit+=(*it)*(*it);
        }
        *nit=sqrt(*nit);
      }
    } else {
      norms.resize(c, T(0),false, true);
      for (i=0; i<r; ++i) {
        nit = norms.begin();
        it = m.getRow(i).begin();
        ite = m.getRow(i).end();
        for ( ; it!=ite; ++it, ++nit) {
          *nit+=(*it)*(*it);
        }
      }
      norms.apply(sqrt);
    }

    return true;
  }

  // vector norm of matrix
  template <class T>
  bool l2Distance<T>::apply(const matrix<T>& m, T& norm) const {

    typename vector<T>::const_iterator it;
    typename vector<T>::const_iterator end;
    norm=T(0);

    for (int i=0;i<m.rows();++i) {
      const vector<T>& v = m.getRow(i);
      for (it=v.begin(),end=v.end();it!=end;++it) {
        norm+=(*it)*(*it);
      }
    }
    norm=sqrt(norm);

    return true;
  }

  // vector norm of matrix
  template <class T>
  T l2Distance<T>::apply(const matrix<T>& m) const {

    T tmp;
    apply(m, tmp);

    return tmp;
  }

  // On copy apply for type vector<T>!
  template <class T>
  T l2Distance<T>::apply(const vector<T>& a,
                         const vector<T>& b) const {

    T tmp;
    if (apply(a, b, tmp)) {
      return tmp;
    } else {
      return T(-1);
    }

  }

  // On copy apply for type vector<T>!
  template <class T>
  bool l2Distance<T>::apply(const vector<T>& a,
                            const vector<T>& b,
                            T& dist) const {

    assert (a.size() == b.size());

    typename vector<T>::const_iterator ait,bit,eit;

    ait = a.begin();
    bit = b.begin();
    eit = a.end();

    T tmp;
    dist=T(0);
    while (ait != eit) {
      tmp = (*ait)-(*bit);
      dist += tmp*tmp;
      ++ait;
      ++bit;
    }
    dist=sqrt(dist);

    return true;
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool l2Distance<T>::apply(const matrix<T>& a,
                            const matrix<T>& b,
                               T& dist) const {

    assert (a.size() == b.size());

    dist=T(0);
    typename vector<T>::const_iterator ait, bit, eit;

    T tmp;
    for (int i=0;i<a.rows();++i) {
      ait=a.getRow(i).begin();
      eit=a.getRow(i).end();
      bit=b.getRow(i).begin();
      for (; ait!=eit; ++ait, ++bit) {
        tmp=(*ait)-(*bit);
        dist+=tmp*tmp;
      }
    }
    dist=sqrt(dist);

    return true;
  }

  // On copy apply for type matrix<T>!
  template <class T>
  T l2Distance<T>::apply(const matrix<T>& a,
                         const matrix<T>& b) const {

    T tmp;
    bool ok=apply(a, b, tmp);
    if (ok) {
      return tmp;
    } else {
      return T(-1);
    }

  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool l2Distance<T>::apply(const matrix<T>& a,
                            const vector<T>& b,
                            vector<T>& dest) const {
    const parameters& param=this->getParameters();
    assert((param.rowWise && (a.columns()==b.size())) ||
	   (!param.rowWise && (a.rows()==b.size())));
    

    int i=0;
    const int r=a.rows();
    const int c=a.columns();

    if (param.rowWise) {
      dest.resize(r,T(),false);
      typename vector<T>::iterator it=dest.begin();

      for (i=0; i < r; i++, ++it) {
        *it = apply(a.getRow(i),b);
      }
    } else {
      dest.resize(c,T(),false);
      T tmp;
      typename vector<T>::const_iterator bit=b.begin();
      typename vector<T>::const_iterator ait;
      typename vector<T>::const_iterator aite;
      typename vector<T>::iterator dit;

      for (i=0; i<r; i++, ++bit) {
        const vector<T>& aRow=a.getRow(i);
        ait=aRow.begin();
        aite=aRow.end();
        dit=dest.begin();
        for ( ; ait!=aite; ++ait, ++dit) {
          tmp=*ait-*bit;
          *dit += tmp*tmp;
        }
      }
      
      dest.apply(sqrt);
    }

    return true;
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool l2Distance<T>::apply(const matrix<T>& a,
                            const matrix<T>& b,
                                  vector<T>& dest) const {

    assert(a.size() == b.size());

    const parameters& param=this->getParameters();
    const int r=a.rows();
    int i=0;
    
    if (param.rowWise) {
      dest.resize(r,T(),false,false);
      
      for (i=0; i < r; i++) {
        dest.at(i) = apply(a.getRow(i),b.getRow(i));
      }
    } else {
      dest.resize(a.columns(),T(0),false,true);
      T tmp;
      
      typename vector<T>::const_iterator bit;
      typename vector<T>::const_iterator ait;
      typename vector<T>::const_iterator aite;
      typename vector<T>::iterator dit;
      
      for (i=0; i < r; i++) {
        bit=b.getRow(i).begin();
        const vector<T>& aRow=a.getRow(i);
        ait=aRow.begin();
        aite=aRow.end();
        dit=dest.begin();
        for (; ait!=aite; ++ait, ++bit, ++dit) {
          tmp=*ait-*bit;
          *dit+=tmp*tmp;
        }
      }
      dest.apply(sqrt);
    }
    return true;
  }
  
}

