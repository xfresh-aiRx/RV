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
 * file .......: ltiSerialStatsExtFunctor_template.h
 * authors ....: Jochen Wickel,Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.12.2002
 * revisions ..: $Id: ltiSerialStatsExtFunctor_template.h,v 1.6 2004/05/03 12:20:02 alvarado Exp $
 */

// MS Visual could define these as macros, so just let's undefine them!
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace lti {

  // --------------------------------------------------
  // serialStatsExtFunctor
  // --------------------------------------------------

  // default constructor
  template <class T,class U>
  serialStatsExtFunctor<T,U>::serialStatsExtFunctor()
    : statisticsFunctor() {
    parameters defaultParameters;
    setParameters(defaultParameters);

    reset();
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::reset() {
    sumOfElements=U(0);
    sumOfSquares=U(0);
    n=0;
    nv=0;
    sumOfVectors.fill(U(0));
    sumOfVecSq.fill(U(0));
    // ensure that the extremes have the biggest values (or smallest) in
    // order to force the first consider() to update the respective values
    if (std::numeric_limits<U>::is_integer) {
      theMin = std::numeric_limits<U>::max();
      theMax = std::numeric_limits<U>::min();
    } else {
      theMin = std::numeric_limits<U>::max();
      theMax = -theMin;
    }
  }

  // copy constructor
  template <class T,class U>
  serialStatsExtFunctor<T,U>::serialStatsExtFunctor(const serialStatsExtFunctor& other)
    : statisticsFunctor() {
    copy(other);
  }

  // destructor
  template <class T,class U>
  serialStatsExtFunctor<T,U>::~serialStatsExtFunctor() {
  }

  // returns the name of this type
  template <class T,class U>
  const char* serialStatsExtFunctor<T,U>::getTypeName() const {
    return "serialStatsExtFunctor";
  }

  // copy member
  template <class T,class U>
  serialStatsExtFunctor<T,U>&
  serialStatsExtFunctor<T,U>::copy(const serialStatsExtFunctor<T,U>& other) {
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
  functor* serialStatsExtFunctor<T,U>::clone() const {
    return new serialStatsExtFunctor<T,U>(*this);
  }

  // return parameters
  template <class T,class U>
  const typename serialStatsExtFunctor<T,U>::parameters&
  serialStatsExtFunctor<T,U>::getParameters() const {
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
  void serialStatsExtFunctor<T,U>::apply(U& mean, U& variance,
                                         U& min, U& max) const {
    mean=getMean();
    variance=getVariance();
    min=getMin();
    max=getMax();
  };

  // On place apply for type vector<T>!
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::apply(vector<U>& mean,
                                         vector<U>& variance,
                                         vector<U>& min,
                                         vector<U>& max) const {
    getMean(mean);
    getVariance(variance);
    getMin(min);
    getMax(max);
  };

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::consider(const T& elem) {
    sumOfElements+=static_cast<U>(elem);
    sumOfSquares+=static_cast<U>(elem)*static_cast<U>(elem);
    theMin=min(theMin,static_cast<U>(elem));
    theMax=max(theMax,static_cast<U>(elem));
    n++;
  }

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::consider(const T& elem,const int& np) {
    sumOfElements+=(static_cast<U>(elem)*np);
    sumOfSquares+=(static_cast<U>(elem)*static_cast<U>(elem)*np);
    theMin=min(theMin,static_cast<U>(elem));
    theMax=max(theMax,static_cast<U>(elem));
    n+=np;
  }

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::consider(const vector<T>& elem) {
    if (sumOfVectors.size() != elem.size()) {
      sumOfVectors.resize(elem.size(),U(0),true,true);
      sumOfVecSq.resize(elem.size(),U(0),true,true);
    }
    vector<U> tmp;
    tmp.castFrom(elem);

    theMins.apply(tmp,min);
    theMaxs.apply(tmp,max);

    sumOfVectors.add(tmp);
    tmp.emultiply(tmp);
    sumOfVecSq.add(tmp);

    nv++;
  }

  // On copy apply for type vector<T>!
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::consider(const vector<T>& elem,const int& np) {
    if (sumOfVectors.size() != elem.size()) {
      sumOfVectors.resize(elem.size(),U(0),true,true);
      sumOfVecSq.resize(elem.size(),U(0),true,true);
    }
    vector<U> tmp;
    tmp.castFrom(elem);

    theMins.apply(tmp,min);
    theMaxs.apply(tmp,max);

    sumOfVectors.addScaled(np,tmp);
    tmp.emultiply(tmp);
    sumOfVecSq.addScaled(np,tmp);

    nv+=np;
  }


  /*
   * Consider all elements of the given vector
   */
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::considerElements(const vector<T> &elements) {
    typename vector<T>::const_iterator it,eit;
    for (it=elements.begin(),eit=elements.end();
         it!=eit;
         ++it) {
      sumOfElements+=static_cast<U>(*it);
      sumOfSquares+=(static_cast<U>(*it)*static_cast<U>(*it));
      theMin = min(theMin,static_cast<U>(*it));
      theMax = max(theMax,static_cast<U>(*it));
      n++;
    }
  }

  /*
   * Consider all elements of the given matrix
   */
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::considerElements(const matrix<T> &elements) {
    int y;
    for (y=0;y<elements.rows();++y) {
      typename vector<T>::const_iterator it,eit;
      for (it=elements.getRow(y).begin(),eit=elements.getRow(y).end();
           it!=eit;
           ++it) {
        sumOfElements+=static_cast<U>(*it);
        sumOfSquares+=(static_cast<U>(*it)*static_cast<U>(*it));
        theMin = min(theMin,static_cast<U>(*it));
        theMax = max(theMax,static_cast<U>(*it));
        n++;
      }
    }
  }

  /*
   * Consider all elements of the given matrix
   */
  template <class T,class U>
  void serialStatsExtFunctor<T,U>::considerElements(const matrix<T> &elements,
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
        theMin = min(theMin,static_cast<U>(*it));
        theMax = max(theMax,static_cast<U>(*it));

        ++n;
        ++it;
      }
    }
  }

  /*
   * Consider all rows of the given matrix as vectors
   */
  template <class T,class U>
    void serialStatsExtFunctor<T,U>::considerRows(const matrix<T> &elements) {
    int y;
    for (y=0;y<elements.rows();++y) {
      consider(elements.getRow(y));
    }
  }

  template <class T,class U>
  U serialStatsExtFunctor<T,U>::getMean() const {
    if (n>0) {
      return sumOfElements/n;
    } else {
      return T(0);
    }
  }

  template <class T,class U>
  U serialStatsExtFunctor<T,U>::getVariance() const {

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
  U serialStatsExtFunctor<T,U>::getMin() const {
    return theMin;
  }

  template <class T,class U>
  U serialStatsExtFunctor<T,U>::getMax() const {
    return theMax;
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::getMin(vector<U>& m) const {
    m.copy(theMins);
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::getMax(vector<U>& m) const {
    m.copy(theMaxs);
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::getMean(vector<U>& mean) const {
    mean=sumOfVectors;
    if (nv>0) {
      mean.divide(nv);
    }
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::getVariance(vector<U>& var) const {
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
  U serialStatsExtFunctor<T,U>::getSumOfElements() const {
    return sumOfElements;
  }

  /*
   * The sum of the squares of of all elements that have been
   * considered so far.
   */
  template <class T,class U>
  U serialStatsExtFunctor<T,U>::getSumOfSquares() const {
    return sumOfSquares;
  }

  /*
   * return the number of elements considered so far
   */
  template <class T,class U>
  int serialStatsExtFunctor<T,U>::getN() const {
    return n;
  }

  /*
   * return the number of vectors considered so far
   */
  template <class T,class U>
  int serialStatsExtFunctor<T,U>::getNVectors() const {
    return nv;
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::getSumOfVectors(vector<U>& s) const {
    s.copy(sumOfVectors);
  }

  template <class T,class U>
  void serialStatsExtFunctor<T,U>::getSumOfVectorSquares(vector<U>& s) const {
    s.copy(sumOfVecSq);
  }

  template <class T,class U>
  const vector<U>& serialStatsExtFunctor<T,U>::getSumOfVectors() const {
    return sumOfVectors;
  }

  template <class T,class U>
  const vector<U>& serialStatsExtFunctor<T,U>::getSumOfVectorSquares() const {
    return sumOfVecSq;
  }

};

