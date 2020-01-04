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
 * file .......: ltiQuickMedian_template.h
 * authors ....: Guy Wafo
 * organization: LTI, RWTH Aachen
 * creation ...: 27.3.2001
 * revisions ..: $Id: ltiQuickMedian_template.h,v 1.9 2004/05/03 12:20:02 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // quickMedian
  // --------------------------------------------------

  // default constructor
  template <class T>
  quickMedian<T>::quickMedian()
    : functor(){
    parameters param;
    setParameters(param);
  }

  // default constructor
  template <class T>
  quickMedian<T>::quickMedian(const typename parameters::eEvenCase evenCase)
    : functor(){
    parameters param;
    param.evenCase = evenCase;
    setParameters(param);
  }

  // copy constructor
  template <class T>
  quickMedian<T>::quickMedian(const quickMedian& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class T>
  quickMedian<T>::~quickMedian() {
  }

  // returns the name of this type
  template <class T>
  const char* quickMedian<T>::getTypeName() const {
    return "quickMedian";
  }

  // copy member
  template <class T>
  quickMedian<T>& quickMedian<T>::copy(const quickMedian<T>& other) {
      functor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* quickMedian<T>::clone() const {
    return new quickMedian(*this);
  }

  // return parameters
  template <class T>
  const typename quickMedian<T>::parameters&
  quickMedian<T>::getParameters() const {
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
  T quickMedian<T>::apply(matrix<T>& srcdest) const {
    vector<T> tmpvct;
    tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                         &srcdest.at(0,0),
                         true);

    return apply(tmpvct);
  };

  // On copy apply for type matrix<T>!
  template <class T>
  T& quickMedian<T>::apply(const matrix<T>& src, T& dest) const {
    matrix<T> tmp(src);
    dest = apply(tmp);
    return dest;
  };

  // On place apply for type vector<T>!
  template <class T>
  T quickMedian<T>::apply(vector<T>& srcdest) const {
      int medianPos;
      const parameters& param = getParameters();

      if ((srcdest.size() % 2) != 0) {
          // odd
          medianPos = srcdest.size()/2;
      } else {
          // even
          if (param.evenCase == parameters::TakeLower) {
              medianPos = srcdest.size()/2 - 1 ;
          } else {
              medianPos = srcdest.size()/2 ;
          }
      }

    return findMedian(srcdest,0,srcdest.lastIdx(),medianPos);
  };

  // On place apply for type vector<T>!
  template <class T>
  T& quickMedian<T>::apply(const vector<T>& src,T& dest) const {
    vector<T> tmp(src);
    dest = apply(tmp);
    return dest;
  };

  // On place apply for type vector<T>!
  template <class T>
  T& quickMedian<T>::apply(const vector<T>& src,vector<T>& dest,
                           T& median) const {
    dest.copy(src);
    median = apply(dest);
    return median;
  };

  // On place apply for type vector<T>!
  template <class T>
  T quickMedian<T>::apply(std::vector<T>& srcdest) const {
    int medianPos;
    const parameters& param = getParameters();
    
    if ((srcdest.size() % 2) != 0) {
      // odd
      medianPos = srcdest.size()/2;
    } else {
      // even
      if (param.evenCase == parameters::TakeLower) {
        medianPos = srcdest.size()/2 - 1 ;
      } else {
        medianPos = srcdest.size()/2 ;
      }
    }
    
    return findMedian(srcdest,0,srcdest.lastIdx(),medianPos);
  };

  // On place apply for type vector<T>!
  template <class T>
  T& quickMedian<T>::apply(const std::vector<T>& src,T& dest) const {
    vector<T> tmp(src);
    dest = apply(tmp);
    return dest;
  };

  // On place apply for type vector<T>!
  template <class T>
  T& quickMedian<T>::apply(const std::vector<T>& src,std::vector<T>& dest,
                           T& median) const {
    dest.copy(src);
    median = apply(dest);
    return median;
  };



  template<class T>
  T  quickMedian<T>::findMedian(vector<T>& vct,
                                const int& begin,
                                const int& end,
                                const int& medianPos) const {
    int pivot;

    pivot=partition(vct,begin,end);
    if(pivot==medianPos) {
      return vct[pivot];
    } else if(pivot>medianPos) {
      return findMedian(vct,begin,pivot-1,medianPos);
    } else if(pivot<medianPos) {
      return findMedian(vct,pivot+1,end,medianPos);
    }

    return T();
  }

  // partition algorithm (see quickSort)
  template<class T>
  int quickMedian<T>::partition(vector<T>& vct,
                                const int& begin,
                                const int& end) const {
    
    int i,j;
    T v,t;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct.at(++i) < v);
      while ( (--j >= 0) && (v < vct.at(j)) ) ;
      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;
    }
    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    return i;
  }

  template<class T>
  T  quickMedian<T>::findMedian(std::vector<T>& vct,
                                const int& begin,
                                const int& end,
                                const int& medianPos) const {
    int pivot;

    pivot=partition(vct,begin,end);
    if(pivot==medianPos) {
      return vct[pivot];
    } else if(pivot>medianPos) {
      return findMedian(vct,begin,pivot-1,medianPos);
    } else if(pivot<medianPos) {
      return findMedian(vct,pivot+1,end,medianPos);
    }

    return T();
  }

  // partition algorithm (see quickSort)
  template<class T>
  int quickMedian<T>::partition(std::vector<T>& vct,
                                const int& begin,
                                const int& end) const {
    
    int i,j;
    T v,t;

    v = vct[end];
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct[++i] < v);
      while ( (--j >= 0) && (v < vct[j]) ) ;
      if ( i >= j ) {
        break ;
      }
      t=vct[i];
      vct[i]=vct[j];
      vct[j]=t;
    }
    t=vct[i];
    vct[i]=vct[end];
    vct[end]=t;

    return i;
  }

  // --------------------------------------------------
  // quickMedian2
  // --------------------------------------------------

  // default constructor
  template <class T,class U>
  quickMedian2<T,U>::quickMedian2()
    : quickMedian<T>() {
  }

  // copy constructor
  template <class T,class U>
  quickMedian2<T,U>::quickMedian2(const quickMedian2& other)
    : quickMedian<T>() {
    copy(other);
  }

  // destructor
  template <class T,class U>
  quickMedian2<T,U>::~quickMedian2() {
  }

  // returns the name of this type
  template <class T,class U>
  const char* quickMedian2<T,U>::getTypeName() const {
    return "quickMedian2";
  }

  // clone member
  template <class T,class U>
  functor* quickMedian2<T,U>::clone() const {
    return new quickMedian2(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type vector<T>!
  template <class T,class U>
  T quickMedian2<T,U>::apply(vector<T>& keys,vector<U>& data) const {
    assert(keys.size()==data.size());

    int medianPos;
    const parameters& param = this->getParameters();
    
    if ((keys.size() % 2) != 0) {
      // odd
      medianPos = keys.size()/2;
    } else {
      // even
      if (param.evenCase == parameters::TakeLower) {
        medianPos = keys.size()/2 - 1 ;
      } else {
        medianPos = keys.size()/2 ;
      }
    }
    
    return findMedian(keys,data,0,keys.lastIdx(),medianPos);
  };

  template <class T,class U>
  T quickMedian2<T,U>::findMedian(vector<T>& vct,
                                  vector<U>& data,
                                  const int& begin,
                                  const int& end,
                                  const int& medianPos) const {
    int pivot;

    pivot=partition(vct,data,begin,end);
    if(pivot==medianPos) {
      return vct[pivot];
    } else if(pivot>medianPos) {
      return findMedian(vct,data,begin,pivot-1,medianPos);
    } else if(pivot<medianPos) {
      return findMedian(vct,data,pivot+1,end,medianPos);
    }

    return T();
  }

  // partition algorithm (see quickSort)
  template <class T,class U>
  int quickMedian2<T,U>::partition(vector<T>& vct,
                                   vector<U>& data,
                                   const int& begin,
                                   const int& end) const {
    
    int i,j;
    T v,t;
    U d;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct.at(++i) < v);
      while ( (--j >= 0) && (v < vct.at(j)) ) ;
      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;

      d=data.at(i);
      data.at(i)=data.at(j);
      data.at(j)=d;
    }
    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    d=data.at(i);
    data.at(i)=data.at(end);
    data.at(end)=d;

    return i;
  }

}
