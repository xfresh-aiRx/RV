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
 * file .......: ltiEntropyFunctor.cpp
 * authors ....: Stefan Syberichs
 * organization: LTI, RWTH Aachen
 * creation ...: 5.3.2001
 * revisions ..: $Id: ltiEntropyFunctor_template.h,v 1.12 2004/05/03 12:20:01 alvarado Exp $
 */


#include "ltiMath.h"

namespace lti {
  // --------------------------------------------------
  // entropyFunctor::parameters
  // --------------------------------------------------

  // implementation in header file due to MS VC++ 6.0 bug

  // --------------------------------------------------
  // entropyFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  entropyFunctor<T>::entropyFunctor()
    : statisticsFunctor() {

    parameters defaultParam;
    setParameters(defaultParam);

  }

  // constructor
  template <class T>
  entropyFunctor<T>::entropyFunctor(const bool& normalize,
                                    const bool& rowWise)
    : statisticsFunctor() {

    parameters defaultParam;

    defaultParam.normalize = normalize;
    defaultParam.rowWise = rowWise;

    setParameters(defaultParam);
  }

  // copy constructor
  template <class T>
  entropyFunctor<T>::entropyFunctor(const entropyFunctor& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  entropyFunctor<T>::~entropyFunctor() {
  }

  // returns the name of this type
  template <class T> const char* entropyFunctor<T>::getTypeName() const {
    return "entropyFunctor";
  }

  // copy member
  template <class T>
  entropyFunctor<T>&
    entropyFunctor<T>::copy(const entropyFunctor<T>& other) {
      statisticsFunctor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* entropyFunctor<T>::clone() const {
    return new entropyFunctor<T>(*this);
  }

  // return parameters
  template <class T>
  const typename entropyFunctor<T>::parameters&
    entropyFunctor<T>::getParameters() const {
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

  // On place apply for type vector<T>!
  template <class T>
  T entropyFunctor<T>::apply(const vector<T>& src) const {

    T sum;
    return apply(src,sum);
  };

  // On copy apply for type vector<T>!
  template <class T>
  T& entropyFunctor<T>::apply(const vector<T>& src, T& dest) const {

    const parameters& param = getParameters();

    dest=T(0);
    T p;

    if (!param.normalize) {

      for (typename vector<T>::const_iterator i=src.begin(); i != src.end(); i++) {
        p = *i;
        if (p > static_cast<T>(0)) {
          dest-=(p * log(p));
        }
      }
    } else {

      T k(0);
      for (typename vector<T>::const_iterator i=src.begin(); i != src.end(); i++) {
        p = *i;
        if (p > static_cast<T>(0)) {
          k+=p;
          dest-=(p * log(p));
        }
      }

      dest = ((dest/k)+log(k));
    }

    return dest;
  };

  // On copy apply for type matrix<T>!
  template <class T>
  vector<T>& entropyFunctor<T>::apply(const matrix<T>& src,
                                            vector<T>& dest) const {
    vector<T> tmp;
    if (getParameters().rowWise) {
      entropyOfRows(src,dest);
    } else {
      entropyOfColumns(src,dest);
    }

    return dest;
  };

  template <class T>
  void entropyFunctor<T>::entropyOfRows(const matrix<T>& src,
                                              vector<T>& dest) const {
    dest.resize(src.rows(),T(0),false,false);

    for (int i=0; i<src.rows(); i++) {
      dest[i] = apply(src.getRowCopy(i));
    }
  }

  template <class T>
  void entropyFunctor<T>::entropyOfColumns(const matrix<T>& src,
                                                 vector<T>& dest) const {

    const parameters& param = getParameters();

    dest.resize(src.columns(),T(0),false,true);

    typename vector<T>::const_iterator it,eit;
    typename vector<T>::iterator dit;

    if (!param.normalize) {

      for (int j=0;j<src.rows();++j) {

        const vector<T>& vct = src.getRow(j);

        for (it=vct.begin(),eit=vct.end(),dit=dest.begin();
             it!=eit;
             ++it,++dit) {
          if ((*it) > 0) {
            (*dit)-=((*it) * log((*it)));
          }
        }

      }

    } else {

      vector<T> k(src.rows(),T(0));
      typename vector<T>::iterator kit,edit;

      for (int j=0;j<src.rows();++j) {

        const vector<T>& vct = src.getRow(j);

        for (it=vct.begin(),eit=vct.end(),dit=dest.begin(),kit=k.begin();
             it!=eit;
             ++it,++dit,++kit) {
          if ((*it) > 0) {
            (*dit)-=((*it) * log((*it)));
            (*kit)+=(*it);
          }
        }

      }

      for (dit=dest.begin(),kit=k.begin(),edit=dest.end();
           dit!=edit;
           ++dit,++kit) {

        (*dit) = (((*dit)/(*kit)) + log(*kit));
      }

    }
  }

  template <class T>
  T& entropyFunctor<T>::apply(const matrix<T>& src,
                                    T& dest) const {

    const parameters& param = getParameters();

    dest = T(0);
    typename vector<T>::const_iterator it,eit;

    if (!param.normalize) {

      for (int j=0;j<src.rows();++j) {

        const vector<T>& vct = src.getRow(j);

        for (it=vct.begin(),eit=vct.end();
             it!=eit;
             ++it) {
          if ((*it) > 0) {
            dest-=((*it) * log((*it)));
          }
        }

      }

    } else {
      T k = T(0);

      for (int j=0;j<src.rows();++j) {

        const vector<T>& vct = src.getRow(j);

        for (it=vct.begin(),eit=vct.end();
             it!=eit;
             ++it) {
          if ((*it) > 0) {
            dest-=((*it) * log((*it)));
            k+=(*it);
          }
        }

      }

      dest = ((dest/k) + log(k));

    }

    return dest;
  }

}


