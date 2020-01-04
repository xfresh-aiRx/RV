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
 * file .......: ltiSerialStatsFunctor_template.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiSerialStatsFunctor_template.h,v 1.15 2004/05/03 12:20:02 alvarado Exp $
 */


namespace lti {

  // --------------------------------------------------
  // serialStatsFunctor
  // --------------------------------------------------

  // default constructor
  template <class T,class U>
  serialStatsFunctor<T,U>::serialStatsFunctor()
    : statisticsFunctor() {
    parameters defaultParameters;
    setParameters(defaultParameters);

    reset();
  }

  template <class T,class U>
  void serialStatsFunctor<T,U>::reset() {
    sumOfElements=U(0);
    sumOfSquares=U(0);
    n=0;
    nv=0;
    sumOfVectors.fill(U(0));
    sumOfVecSq.fill(U(0));
  }

  // copy constructor
  template <class T,class U>
  serialStatsFunctor<T,U>::serialStatsFunctor(const serialStatsFunctor& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T,class U>
  serialStatsFunctor<T,U>::~serialStatsFunctor() {
  }

  // returns the name of this type
  template <class T,class U>
  const char* serialStatsFunctor<T,U>::getTypeName() const {
    return "serialStatsFunctor";
  }

  // copy member
  template <class T,class U>
  serialStatsFunctor<T,U>&
  serialStatsFunctor<T,U>::copy(const serialStatsFunctor<T,U>& other) {
    statisticsFunctor::copy(other);
    n=other.n;
    nv=other.nv;
    sumOfElements=other.sumOfElements;
    sumOfSquares=other.sumOfSquares;
    sumOfVectors=other.sumOfVectors;
    sumOfVecSq=other.sumOfVecSq;
    return (*this);
  }

  // clone member
  template <class T,class U>
  functor* serialStatsFunctor<T,U>::clone() const {
    return new serialStatsFunctor<T,U>(*this);
  }

  // return parameters
  template <class T,class U>
  const typename serialStatsFunctor<T,U>::parameters&
  serialStatsFunctor<T,U>::getParameters() const {
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
  template <class T,class U>
  void serialStatsFunctor<T,U>::apply(U& mean, U& variance) const {
    mean=getMean();
    variance=getVariance();
  };

  // On place apply for type vector<T>!
  template <class T,class U>
  void serialStatsFunctor<T,U>::apply(vector<U>& mean,
                                      vector<U>& variance) const {
    getMean(mean);
    getVariance(variance);
  };

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsFunctor<T,U>::consider(const T& elem) {
    sumOfElements+=static_cast<U>(elem);
    sumOfSquares+=static_cast<U>(elem)*static_cast<U>(elem);
    n++;
  }

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsFunctor<T,U>::consider(const T& elem,const int& np) {
    sumOfElements+=(static_cast<U>(elem)*np);
    sumOfSquares+=(static_cast<U>(elem)*static_cast<U>(elem)*np);
    n+=np;
  }

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsFunctor<T,U>::consider(const vector<T>& elem) {
    if (sumOfVectors.size() != elem.size()) {
      sumOfVectors.resize(elem.size(),U(0),true,true);
      sumOfVecSq.resize(elem.size(),U(0),true,true);
    }
    vector<U> tmp;
    tmp.castFrom(elem);

    sumOfVectors.add(tmp);
    tmp.emultiply(tmp);
    sumOfVecSq.add(tmp);
    nv++;
  }

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsFunctor<T,U>::consider(const vector<T>& elem,const int& np) {
    if (sumOfVectors.size() != elem.size()) {
      sumOfVectors.resize(elem.size(),U(0),true,true);
      sumOfVecSq.resize(elem.size(),U(0),true,true);
    }
    vector<U> tmp;
    tmp.castFrom(elem);

    sumOfVectors.addScaled(np,tmp);
    tmp.emultiply(tmp);
    sumOfVecSq.addScaled(np,tmp);
    nv+=np;
  }


  /*
   * Consider all elements of the given vector
   */
  template <class T,class U>
  void serialStatsFunctor<T,U>::considerElements(const vector<T> &elements) {
    typename vector<T>::const_iterator it,eit;
    for (it=elements.begin(),eit=elements.end();
         it!=eit;
         ++it) {
      sumOfElements+=static_cast<U>(*it);
      sumOfSquares+=(static_cast<U>(*it)*static_cast<U>(*it));
      n++;
    }
  }

  /*
   * Consider all elements of the given matrix
   */
  template <class T,class U>
  void serialStatsFunctor<T,U>::considerElements(const matrix<T> &elements) {
    int y;
    for (y=0;y<elements.rows();++y) {
      typename vector<T>::const_iterator it,eit;
      for (it=elements.getRow(y).begin(),eit=elements.getRow(y).end();
           it!=eit;
           ++it) {
        sumOfElements+=static_cast<U>(*it);
        sumOfSquares+=(static_cast<U>(*it)*static_cast<U>(*it));
        n++;
      }
    }
  }

  /*
   * Consider all elements of the given matrix
   */
  template <class T,class U>
  void serialStatsFunctor<T,U>::considerElements(const matrix<T> &elements,
                                                 const rectangle& rect) {
    
    rectangle wnd(rect),origWnd(point(0,0),elements.size());
    origWnd.br.x--;
    origWnd.br.y--;

    wnd.ensureConsistency();
    wnd.intersect(origWnd);

    int y;
    for (y=wnd.ul.y;y<=wnd.br.y;++y) {
      typename vector<T>::const_iterator it,eit;
      eit = it = elements.getRow(y).begin();
      it += wnd.ul.x;
      eit += wnd.br.x;

      while (it!=eit) {
        sumOfElements+=static_cast<U>(*it);
        sumOfSquares+=(static_cast<U>(*it)*static_cast<U>(*it));
        ++n;
        ++it;
      }
    }
  }

  /*
   * Consider all rows of the given matrix as vectors
   */
  template <class T,class U>
    void serialStatsFunctor<T,U>::considerRows(const matrix<T> &elements) {
    int y;
    for (y=0;y<elements.rows();++y) {
      consider(elements.getRow(y));
    }
  }

  template <class T,class U>
  U serialStatsFunctor<T,U>::getMean() const {
    if (n>0) {
      return sumOfElements/n;
    } else {
      return T(0);
    }
  }

  template <class T,class U>
  U serialStatsFunctor<T,U>::getVariance() const {

    if (getParameters().type == parameters::Empirical) {
      if (n>1) {
        U tmp=(sumOfElements/n);
        tmp =  (sumOfSquares-U(n)*(tmp*tmp))/(n-1);
        return (tmp > U(0)) ? tmp : U(0);
      } else {
        return U(0);
      }
    } else {
      if (n>0) {
        U tmp=(sumOfElements/n);
        tmp =  (sumOfSquares/U(n))-(tmp*tmp);
        return (tmp > U(0)) ? tmp : U(0);
      } else {
        return U(0);
      }
    }
  }

  template <class T,class U>
  void serialStatsFunctor<T,U>::getMean(vector<U>& mean) const {
    mean=sumOfVectors;
    if (nv>0) {
      mean.divide(nv);
    }
  }

  template <class T,class U>
  void serialStatsFunctor<T,U>::getVariance(vector<U>& var) const {
    vector<U> tmp(sumOfVectors);
    if (getParameters().type == parameters::Empirical) {
      if (nv>1) {
        tmp.divide(nv);
        tmp.emultiply(tmp);
        var.copy(sumOfVecSq);
        var.addScaled(-nv,tmp);
        var.divide(nv-1);
        // ensure that there are no negative number.  Due to numerical errors,
        // this could happen...
        var.apply(lti::rectify);
      } else {
        var.resize(sumOfVectors.size(),U(0),false,true);
      }
    } else {
      if (nv>0) {
        tmp.divide(nv);
        tmp.emultiply(tmp);
        var.copy(sumOfVecSq);
        var.divide(nv);
        var.subtract(tmp);
        var.apply(lti::rectify);
      } else {
        var.resize(sumOfVectors.size(),U(0),false,true);
      }
    }
  }

  /*
   * The sum of all elements that have been considered so far.
   */
  template <class T,class U>
  U serialStatsFunctor<T,U>::getSumOfElements() const {
    return sumOfElements;
  }

  /*
   * The sum of the squares of of all elements that have been
   * considered so far.
   */
  template <class T,class U>
  U serialStatsFunctor<T,U>::getSumOfSquares() const {
    return sumOfSquares;
  }

  /*
   * return the number of elements considered so far
   */
  template <class T,class U>
  int serialStatsFunctor<T,U>::getN() const {
    return n;
  }

  /*
   * return the number of vectors considered so far
   */
  template <class T,class U>
  int serialStatsFunctor<T,U>::getNVectors() const {
    return nv;
  }

  template <class T,class U>
  void serialStatsFunctor<T,U>::getSumOfVectors(vector<U>& s) const {
    s.copy(sumOfVectors);
  }

  template <class T,class U>
  void serialStatsFunctor<T,U>::getSumOfVectorSquares(vector<U>& s) const {
    s.copy(sumOfVecSq);
  }

  template <class T,class U>
  const vector<U>& serialStatsFunctor<T,U>::getSumOfVectors() const {
    return sumOfVectors;
  }

  template <class T,class U>
  const vector<U>& serialStatsFunctor<T,U>::getSumOfVectorSquares() const {
    return sumOfVecSq;
  }

};

