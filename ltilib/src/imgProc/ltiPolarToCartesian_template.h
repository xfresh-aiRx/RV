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
 * file .......: ltiPolarToCartesian.cpp
 * authors ....: Bernd Mussmann
 * organization: LTI, RWTH Aachen
 * creation ...: 4.9.2000
 * revisions ..: $Id: ltiPolarToCartesian_template.h,v 1.14 2005/10/28 15:56:42 doerfler Exp $
 */

#include "ltiMath.h"

namespace lti {
  // --------------------------------------------------
  // polarToCartesian::parameters
  // --------------------------------------------------

  // implementation in header file due to MS VC++ bug

  // --------------------------------------------------
  // polarToCartesian
  // --------------------------------------------------

  // default constructor
  template<class T>
  polarToCartesian<T>::polarToCartesian()
    : transform(){
  }

  // copy constructor
  template<class T>
  polarToCartesian<T>::polarToCartesian(const polarToCartesian& other)
    : transform() {
    copy(other);
  }

  // destructor
  template<class T>
  polarToCartesian<T>::~polarToCartesian() {
  }

  // returns the name of this type
  template<class T>
  const char* polarToCartesian<T>::getTypeName() const {
    return "polarToCartesian";
  }

  // copy member
  template<class T>
  polarToCartesian<T>&
    polarToCartesian<T>::copy(const polarToCartesian& other) {
      transform::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* polarToCartesian<T>::clone() const {
    return new polarToCartesian(*this);
  }

  // return parameters
  template<class T>
  const typename polarToCartesian<T>::parameters&
  polarToCartesian<T>::getParameters() const {
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
  template<class T>
  bool polarToCartesian<T>::apply(matrix<T>& abs_r,matrix<T>& ph_i) const {

    lti::matrix<T> temp1,temp2;
    bool rc = apply(abs_r,ph_i,temp1,temp2);
    temp1.detach(abs_r);
    temp2.detach(ph_i);
    return rc;
  }

  // On copy apply for type matrix<T>!
  template<class T>
  bool polarToCartesian<T>::apply(const matrix<T>& absvalue,
                                  const matrix<T>& phase,
                                  matrix<T>& real,
                                  matrix<T>& imaginary) const {

    if (absvalue.size() != phase.size()) {
      real.clear();
      imaginary.clear();
      return false;
    }

    // creation and initialzation of new matrix
    real.resize(absvalue.size(),T(),false,false);
    imaginary.resize(absvalue.size(),T(),false,false);

    typename lti::matrix<T>::eStoreMode Mode=absvalue.getMode();
    typename lti::matrix<T>::eStoreMode Mode2=phase.getMode();
    //if one matrix mode not the connected mode then
    // the computation will be in the line mode
    if(Mode != Mode2) Mode = lti::matrix<T>::Line;

    T sina, cosa;

    // switch if the Matrix in the line or connected mode
    switch (Mode){
      //Matrix in the line mode
      case lti::matrix<T>::Line: {
        const int n=absvalue.rows();
        //Iterators for line Mode
        typename vector<T>::const_iterator ab,ph,eab;
        typename matrix<T>::iterator re,im;

        re = real.begin();
        im = imaginary.begin();
        //computing of the absolute value and the phase
        for (int y=0;y<n;y++) {
          for (ab=absvalue.getRow(y).begin(),
                 eab=absvalue.getRow(y).end(),ph=phase.getRow(y).begin();
              ab!=eab;
              re++,im++,ab++,ph++){
            sincos(*ph, sina, cosa);
            *re = (*ab)*cosa;
            *im = (*ab)*sina;
		      }
        }
        break;
      }
      //Matrix in the connected mode
      case lti::matrix<T>::Connected: {
        //Iterators for connected Mode
        typename matrix<T>::const_iterator ab,ph,eab;
        typename matrix<T>::iterator re,im;

        re = real.begin();
        im = imaginary.begin();
         //computing of the complexe number (real part and imaginary part)
        for (ab=absvalue.begin(),eab=absvalue.end(),ph=phase.begin();
             ab!=eab;
             re++,im++,ab++,ph++){
          sincos(*ph, sina, cosa);
          *re = (*ab)*cosa;
          *im = (*ab)*sina;
        }

        break;
      }
      default: {
        return false;
        break;
      }
    }
    return true;
  }

  template<class T>
  bool polarToCartesian<T>::apply(const tpointList<double>& src,
				  tpointList<T> &dest, 
				  const tpoint<double> origin) const {
    dest.clear();

    tpoint<T> tmp;
    tpoint<double> zp;
    tpointList<double>::const_iterator itc;
    double sina, cosa;
    for(itc=src.begin(); itc != src.end(); itc++){
      sincos((*itc).y, sina, cosa);
      condRoundCastTo(origin.x+cosa*(*itc).x,tmp.x);
      condRoundCastTo(origin.y+sina*(*itc).x,tmp.y);
      dest.push_back(tmp);
    }
    return true;
  }
}


