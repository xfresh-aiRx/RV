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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiserialVectorStats_template.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 21.11.2001
 * revisions ..: $Id: ltiSerialVectorStats_template.h,v 1.12 2005/06/22 08:56:36 libuda Exp $
 */

#include "ltiVarianceFunctor.h"

namespace lti {

  // --------------------------------------------------
  // serialVectorStats
  // --------------------------------------------------

  // default constructor
  template <class T>
  serialVectorStats<T>::serialVectorStats()
    : statisticsFunctor() {
    parameters defaultParameters;
    setParameters(defaultParameters);

    reset();
  }

  template <class T>
  void serialVectorStats<T>::reset() {
    nv=0;
    sumX.fill(T(0));
    sumXXT.fill(sqrType(0));
  }

  // copy constructor
  template <class T>
  serialVectorStats<T>::serialVectorStats(const serialVectorStats& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T>
  serialVectorStats<T>::~serialVectorStats() {
  }

  // returns the name of this type
  template <class T> const char* serialVectorStats<T>::getTypeName() const {
    return "serialVectorStats";
  }

  // copy member
  template <class T>
  serialVectorStats<T>&
  serialVectorStats<T>::copy(const serialVectorStats<T>& other) {
      statisticsFunctor::copy(other);
      nv=other.nv;
      sumX=other.sumX;
      sumXXT=other.sumXXT;
    return (*this);
  }

  template <class T>
  serialVectorStats<T>&
  serialVectorStats<T>::operator=(const serialVectorStats<T>& other) {
    return copy(other);
  }

  // clone member
  template <class T>
  functor* serialVectorStats<T>::clone() const {
    return new serialVectorStats<T>(*this);
  }

  // return parameters
  template <class T>
  const typename serialVectorStats<T>::parameters&
  serialVectorStats<T>::getParameters() const {
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
  void serialVectorStats<T>::apply(vector<T>& mean,
                                   vector<T>& variance) const {
    getMean(mean);
    getVariance(variance);
  };

  // On place apply for type matrix<T>!
  template <class T>
  void serialVectorStats<T>::apply(vector<T>& mean,
                                   matrix<T>& covariance) const {
    getMean(mean);
    getCovariance(covariance);
  };

  // On copy apply for type matrix<T>!
  template <class T>
  void serialVectorStats<T>::considerRows(const matrix<T>& elem) {
    int n=elem.rows();
    for (int i=0; i<n; i++) {
      consider(elem.getRow(i));
    }
  }

  template<class T>
  void serialVectorStats<T>::outer(const vector<T>& a,
                                   const vector<T>& b,
                                   matrix<sqrType>& dest) const {

    dest.resize(a.size(),b.size(),sqrType(),false,false);
    int x,y;
    for (y=0;y<dest.rows();++y) {
      const sqrType fct = a.at(y);
      for (x=0;x<dest.columns();++x) {
        dest.at(y,x)=fct*b.at(x);
      }
    }    
  }

  template<class T>
  void serialVectorStats<T>::outerAcc(const vector<T>& a,
                                      const vector<T>& b,
                                      matrix<sqrType>& dest) const {
    

    int x,y;
    for (y=0;y<dest.rows();++y) {
      const sqrType fct = a.at(y);
      for (x=0;x<dest.columns();++x) {
        dest.at(y,x)+=fct*b.at(x);
      }
    }    
  }


  template<class T>
  void serialVectorStats<T>::outerAcc(const vector<T>& a,
                                      const vector<T>& b,
                                      const int n,
                                      matrix<sqrType>& dest) const {
    

    int x,y;
    for (y=0;y<dest.rows();++y) {
      const sqrType fct = a.at(y);
      for (x=0;x<dest.columns();++x) {
        dest.at(y,x)+=(fct*b.at(x)*n);
      }
    }    
  }

  template<class T>
  void serialVectorStats<T>::diagonal(const matrix<sqrType>& mat,
                                      vector<T>& diag) const {
    diag.resize(mat.rows(),T(),false,false);
    int i;
    for (i=0;i<diag.size();++i) {
      diag.at(i) = static_cast<T>(mat.at(i,i));
    }
  }

  // On copy apply for type vector<T>!
  template <class T>
  void serialVectorStats<T>::consider(const vector<T>& elem) {
    if (sumX.size() != elem.size()) {
      sumX.resize(elem.size(),T(0),true,true);
    }
    if ( (sumXXT.rows()!=elem.size()) || (sumXXT.columns()!=elem.size()) ) {
      sumXXT.resize(elem.size(),elem.size(),sqrType(0),true,true);
    }

    sumX.add(elem);
    outerAcc(elem,elem,sumXXT);
    nv++;
  }

  // On copy apply for type vector<T>!
  template <class T>
  void serialVectorStats<T>::consider(const vector<T>& elem,const int& np) {
    if (sumX.size() != elem.size()) {
      sumX.resize(elem.size(),T(0),true,true);
    }
    if ( (sumXXT.rows()!=elem.size()) || (sumXXT.columns()!=elem.size()) ) {
      sumXXT.resize(elem.size(),elem.size(),T(0),true,true);
    }
    sumX.addScaled(static_cast<T>(np),elem);
    outerAcc(elem,elem,np,sumXXT);
    nv+=np;
  }

  template <class T>
    double serialVectorStats<T>::getN() const {
    return nv;
  }

  template <class T>
    void serialVectorStats<T>::setN(double newN) {
    if ( (nv==0)||(newN<0.0) ) {
      return;
    }
    T factor = static_cast<T>(newN/nv);
    sumX.multiply(factor);
    sumXXT.multiply(static_cast<sqrType>(factor));
    nv = newN;
  }

  template <class T>
  void serialVectorStats<T>::getMean(vector<T>& mean) const {
    mean=sumX;
    if (nv>0) {
      mean.divide(static_cast<T>(nv));
    }
  }

  template <class T>
  void serialVectorStats<T>::getVariance(vector<T>& var) const {

    vector<sqrType> tmp,diag;
    tmp.castFrom(sumX);

    const sqrType nv = static_cast<sqrType>(this->nv);

    if (getParameters().type == parameters::Empirical) {
      if (nv>1) {
        tmp.divide(nv);
        tmp.emultiply(tmp);
        sumXXT.getDiagonal(diag);
        diag.addScaled(-nv,tmp);
        diag.divide(nv-1);
        // ensure that there are no negative number.  Due to numerical errors,
        // this could happen...       
        diag.apply(lti::rectify);
        var.castFrom(diag);
      } else {
        var.resize(sumX.size(),T(0),false,true);
      }
    } else {
      if (nv>0) {
        tmp.divide(nv);
        tmp.emultiply(tmp);
        sumXXT.getDiagonal(diag);
        diag.divide(nv);
        diag.subtract(tmp);
        diag.apply(lti::rectify);
        var.castFrom(diag);
      } else {
        var.resize(sumX.size(),T(0),false,true);
      }
    }
  }

  template <class T>
  void serialVectorStats<T>::getCovariance(matrix<T>& covar) const {
    vector<sqrType> tmp;
    tmp.castFrom(sumX);
    matrix<sqrType> tmpMatrix,dcov;
    const sqrType nv = static_cast<sqrType>(this->nv);

    if (getParameters().type == parameters::Empirical) {
      if (nv>1) {
        tmp.divide(nv);
        tmpMatrix.outerProduct(tmp,tmp);
        dcov.addScaled(sumXXT,-nv,tmpMatrix);
        dcov.divide(nv-1);

        // ensure that there are no negative number.  Due to numerical errors,
        // this could happen...
        dcov.apply(lti::rectify);
        
        covar.castFrom(dcov);
        
      } else {
        covar.resize(sumX.size(),sumX.size(),T(0),false,true);
      }
    } else {
      if (nv>0) {
        tmp.divide(nv);
        tmpMatrix.outerProduct(tmp,tmp);
        dcov.divide(sumXXT,nv);       
        dcov.subtract(tmpMatrix);
        dcov.apply(lti::rectify);
        covar.castFrom(dcov);
      } else {
        covar.resize(sumX.size(),sumX.size(),T(0),false,true);
      }
    }

    if (getParameters().correlation) {
      varianceFunctor<T>::covar2corrcoef(covar);
    }

  }

  template <class T>
  void serialVectorStats<T>::getSumOfVectors(vector<T>& s) const {
    s.copy(sumX);
  }

  template <class T>
  void serialVectorStats<T>::getSumOfVecOuterProd(matrix<sqrType>& s) const {
    s.copy(sumXXT);
  }

  template <class T>
  const vector<T>& serialVectorStats<T>::getSumOfVectors() const {
    return sumX;
  }

  template <class T>
  const matrix<typename serialVectorStats<T>::sqrType>& 
  serialVectorStats<T>::getSumOfVecOuterProd() const {
    return sumXXT;
  }

  template <class T>
  bool serialVectorStats<T>::read(ioHandler& handler,
                                    const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      b=lti::read(handler, "parameters",param);
      setParameters(param);

      b=b && lti::read(handler, "sumX", sumX);
      b=b && lti::read(handler, "sumXXT", sumXXT);
      b=b && lti::read(handler, "nv", nv);

      if (complete) {
        b=b && handler.readEnd();
      }
    }
    return b;
  }

  template <class T>
  bool serialVectorStats<T>::write(ioHandler& handler,
                                     const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      b=lti::write(handler,"parameters",getParameters());

      b=b && lti::write(handler, "sumX", sumX);
      b=b && lti::write(handler, "sumXXT", sumXXT);
      b=b && lti::write(handler, "nv", nv);

      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }


};


