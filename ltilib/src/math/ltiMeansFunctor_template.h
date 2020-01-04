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
 * revisions ..: $Id: ltiMeansFunctor_template.h,v 1.16 2004/05/03 12:20:01 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // meansFunctor
  // --------------------------------------------------

  // default constructor
  template <class T>
  meansFunctor<T>::meansFunctor()
    : statisticsFunctor(){
    parameters tmp;
    setParameters(tmp);
  }

  // copy constructor
  template <class T>
  meansFunctor<T>::meansFunctor(const meansFunctor& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  meansFunctor<T>::~meansFunctor() {
  }

  // returns the name of this type
  template <class T> const char* meansFunctor<T>::getTypeName() const {
    return "meansFunctor";
  }

  // copy member
  template <class T>
  meansFunctor<T>&
    meansFunctor<T>::copy(const meansFunctor<T>& other) {
      statisticsFunctor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* meansFunctor<T>::clone() const {
    return new meansFunctor<T>(*this);
  }

  // return parameters
  template <class T>
  const typename meansFunctor<T>::parameters&
    meansFunctor<T>::getParameters() const {
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
  T meansFunctor<T>::apply(const vector<T>& src) const {
    T sum;
    return apply(src,sum);
  };

  // On place apply for type vector<T>!
  template <class T>
  T meansFunctor<T>::geometricMean(const vector<T>& src) const {
    T sum;
    return geometricMean(src,sum);
  };

  // On copy apply for type vector<T>!
  template <class T>
  T meansFunctor<T>::arithmeticMean(const vector<T>& src) const {
    T tmp;
    arithmeticMean(src,tmp);
    return tmp;
  }

  // On copy apply for type vector<T>!
  template <class T>
  T& meansFunctor<T>::apply(const vector<T>& src, T& dest) const {
    if (getParameters().geometric) {
      return geometricMean(src,dest);
    } else {
      return arithmeticMean(src,dest);
    }
  }


  // On copy apply for type vector<T>!
  template <class T>
  T& meansFunctor<T>::arithmeticMean(const vector<T>& src, T& dest) const {
    dest=T(0);

    for (typename vector<T>::const_iterator i=src.begin();
         i != src.end(); i++) {
      dest+=*i;
    }
    dest/=src.size();
    return dest;
  };


  // On copy apply for type vector<T>!
  template <class T>
  T& meansFunctor<T>::geometricMean(const vector<T>& src, T& dest) const {
    dest=T(1);
    typename vector<T>::const_iterator i=src.begin();
    typename vector<T>::const_iterator e=src.end();

    for (; i!=e; ++i) {
      dest*=(*i);
    }
    dest=abs(dest);
    dest=static_cast<T>(lti::pow(static_cast<double>(dest),1.0/src.size()));
    return dest;
  }


  // On copy apply for type matrix<T>!
  template <class T>
  vector<T>& meansFunctor<T>::apply(const matrix<T>& src,vector<T>& dest) const {
    vector<T> tmp;
    if (getParameters().rowWise) {
      meanOfRows(src,dest);
    } else {
      meanOfColumns(src,dest);
    }
    return dest;
  };

  template <class T>
  void meansFunctor<T>::meanOfRows(const matrix<T>& src, vector<T>& dest) const {
    if (getParameters().geometric) {
      geometricMeanOfRows(src,dest);
    } else {
      arithmeticMeanOfRows(src,dest);
    }
  }

  template <class T>
  void meansFunctor<T>::meanOfColumns(const matrix<T>& src, vector<T>& dest) const {
    if (getParameters().geometric) {
      geometricMeanOfColumns(src,dest);
    } else {
      arithmeticMeanOfColumns(src,dest);
    }
  }

  template <class T>
  void meansFunctor<T>::arithmeticMeanOfRows(const matrix<T>& src, vector<T>& dest) const {
    dest.resize(src.columns(),T(0),false,true);
    for (int i=0; i<src.rows(); i++) {
      dest.add(src.getRow(i));
    }
    dest.divide(static_cast<T>(src.rows()));
  }

  template <class T>
  void meansFunctor<T>::geometricMeanOfRows(const matrix<T>& src, vector<T>& dest) const {
    dest.resize(src.columns(),T(1),false,true);
    const int n=src.rows();
    for (int i=0; i<n; i++) {
      dest.emultiply(src.getRow(i));
    }
    typename vector<T>::iterator j=dest.begin();
    typename vector<T>::iterator e=dest.end();
    for (; j!=e; ++j) {
      *j=abs(*j);
      *j=static_cast<T>(pow(static_cast<double>(*j),1.0/n));
    }
  }

  template <class T>
  void meansFunctor<T>::arithmeticMeanOfColumns(const matrix<T>& src,
                                                vector<T>& dest) const {
    dest.resize(src.rows(),T(0),false,false);
    vector<T> tmp;
    for (int i=0; i<src.rows(); i++) {
      dest.at(i)=src.getRow(i).sumOfElements();
    }
    dest.divide(static_cast<T>(src.columns()));
  }

  template <class T>
  void meansFunctor<T>::geometricMeanOfColumns(const matrix<T>& src,
                                               vector<T>& dest) const {
    dest.resize(src.rows(),T(1),false,true);
    const int n=src.rows();
    int i;

    typename vector<T>::const_iterator j;
    typename vector<T>::const_iterator e;
    
    for (i=0; i<n; i++) {
      const vector<T>& tmp=src.getRow(i);
      j=tmp.begin();
      e=tmp.end();
      for (;j!=e;++j) {
        dest.at(i)*=(*j);
      }
      dest.at(i)=abs(dest.at(i));
      dest.at(i)=static_cast<T>(lti::pow(static_cast<double>(dest.at(i)), 1./src.columns()));
    }
  }

}


