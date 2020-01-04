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
 * revisions ..: $Id: ltiQuickPartialSort_template.h,v 1.2 2004/05/03 13:30:10 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // quickPartialSort
  // --------------------------------------------------

  // default constructor
  template <class T>
  quickPartialSort<T>::quickPartialSort()
    : functor(){
    parameters param;
    setParameters(param);
  }

  // copy constructor
  template <class T>
  quickPartialSort<T>::quickPartialSort(const quickPartialSort& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class T>
  quickPartialSort<T>::~quickPartialSort() {
  }

  // returns the name of this type
  template <class T>
  const char* quickPartialSort<T>::getTypeName() const {
    return "quickPartialSort";
  }

  // copy member
  template <class T>
  quickPartialSort<T>& quickPartialSort<T>
  ::copy(const quickPartialSort<T>& other) {
    functor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* quickPartialSort<T>::clone() const {
    return new quickPartialSort(*this);
  }

  // return parameters
  template <class T>
  const typename quickPartialSort<T>::parameters&
  quickPartialSort<T>::getParameters() const {
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

  template <class T>
  bool quickPartialSort<T>::apply(matrix<T>& srcdest) const {
    return apply(getParameters().pivotIndex,srcdest);
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const matrix<T>& src, 
                                        matrix<T>& dest) const {
    return apply(getParameters().pivotIndex,src,dest);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(vector<T>& srcdest) const {
    return apply(getParameters().pivotIndex,srcdest);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const vector<T>& src,
                                        vector<T>& dest) const {
    return apply(getParameters().pivotIndex,src,dest);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(std::vector<T>& srcdest) const {
    return apply(getParameters().pivotIndex,srcdest);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const std::vector<T>& src,
                                  std::vector<T>& dest) const {
    return apply(getParameters().pivotIndex,src,dest);
  };

  // On place apply for type matrix<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const int pivotIndex,
                                  matrix<T>& srcdest) const {
    vector<T> tmpvct;
    tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                         &srcdest.at(0,0),
                         true);

    return apply(pivotIndex,tmpvct);
  };

  // On copy apply for type matrix<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const int pivotIndex,
                                  const matrix<T>& src, 
                                        matrix<T>& dest) const {
    dest.copy(src);
    return apply(dest);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const int pivotIndex,
                                  vector<T>& srcdest) const {
    if ((pivotIndex < 0) ||
        (pivotIndex >= srcdest.size())) {
      setStatusString("Pivot index out of range.  Check the vector size");
      return false;
    }

    if (pivotIndex == 0) {
      // just want to get the smallest element
      int i = srcdest.getIndexOfMinimum();
      exchange(srcdest.at(i),srcdest.at(0));
    } if (pivotIndex == srcdest.lastIdx()) {
      // just want to get the greatest element
      int i = srcdest.getIndexOfMaximum();
      exchange(srcdest.at(i),srcdest.at(srcdest.lastIdx()));      
    } else {    
      findMedian(srcdest,0,srcdest.lastIdx(),pivotIndex);
    }

    return true;
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const int pivotIndex,
                                  const vector<T>& src,
                                        vector<T>& dest) const {
    dest.copy(src);
    return apply(pivotIndex,dest);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const int pivotIndex,
                                  std::vector<T>& srcdest) const {
    if ((pivotIndex < 0) ||
        (pivotIndex >= static_cast<int>(srcdest.size()))) {
      setStatusString("Pivot index out of range.  Check the vector size");
      return false;
    }

    if (pivotIndex == 0) {
      typename std::vector<T>::iterator it;
      it = std::min_element(srcdest.begin(),srcdest.end());
      exchange(srcdest[0],*it);
    } else if (pivotIndex == static_cast<int>(srcdest.size()-1)) {
      typename std::vector<T>::iterator it;
      it = std::max_element(srcdest.begin(),srcdest.end());
      exchange(srcdest[srcdest.size()-1],*it);
    } else {
      findMedian(srcdest,0,srcdest.size()-1,pivotIndex);
    }

    return true;
  };

  // On place apply for type vector<T>!
  template <class T>
  bool quickPartialSort<T>::apply(const int pivotIndex,
                                  const std::vector<T>& src,
                                  std::vector<T>& dest) const {
    dest = src;
    return apply(pivotIndex,dest);
  };

  template<class T>
  T quickPartialSort<T>::findMedian(vector<T>& vct,
                                const int& begin,
                                const int& end,
                                const int& medianPos) const {
    int pivot;

    pivot=partition(vct,begin,end);

    if (pivot==medianPos) {
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
  int quickPartialSort<T>::partition(vector<T>& vct,
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
  T  quickPartialSort<T>::findMedian(std::vector<T>& vct,
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
  int quickPartialSort<T>::partition(std::vector<T>& vct,
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
  // quickPartialSort2
  // --------------------------------------------------

  // default constructor
  template <class T,class U>
  quickPartialSort2<T,U>::quickPartialSort2()
    : quickPartialSort<T>() {
  }

  // copy constructor
  template <class T,class U>
  quickPartialSort2<T,U>::quickPartialSort2(const quickPartialSort2& other)
    : quickPartialSort<T>() {
    copy(other);
  }

  // destructor
  template <class T,class U>
  quickPartialSort2<T,U>::~quickPartialSort2() {
  }

  // returns the name of this type
  template <class T,class U>
  const char* quickPartialSort2<T,U>::getTypeName() const {
    return "quickPartialSort2";
  }

  // clone member
  template <class T,class U>
  functor* quickPartialSort2<T,U>::clone() const {
    return new quickPartialSort2(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type vector<T>!
  template <class T,class U>
  bool quickPartialSort2<T,U>::apply(const int pivotIndex,
                                     vector<T>& keys,
                                     vector<U>& data) const {
    assert(keys.size()==data.size());
    
    if (pivotIndex == 0) {
      // just want to get the smallest element
      int i = keys.getIndexOfMinimum();
      exchange(keys.at(i),keys.at(0));
      exchange(data.at(i),data.at(0));
    } if (pivotIndex == srcdest.lastIdx()) {
      // just want to get the greatest element
      int i = keys.getIndexOfMaximum();
      exchange(keys.at(i),keys.at(keys.lastIdx()));
      exchange(data.at(i),data.at(data.lastIdx()));
    } else {    
      findMedian(keys,data,0,keys.lastIdx(),pivotIndex);
    }

    return true;
  };

  // On place apply for type vector<T>!
  template <class T,class U>
  bool quickPartialSort2<T,U>::apply(const int pivotIndex,
                                     std::vector<T>& keys,
                                     vector<U>& data) const {
    assert(keys.size()==data.size());

    if (pivotIndex == 0) {
      typename std::vector<T>::iterator it;
      it = std::min_element(keys.begin(),keys.end());
      exchange(keys[0],*it);
      exchange(data[0],data[it-keys.begin()]);
    } else if (pivotIndex == srcdest.lastIdx()) {
      typename std::vector<T>::iterator it;
      it = std::max_element(keys.begin(),keys.end());
      exchange(keys[keys.size()-1],*it);
      exchange(data[data.lastIdx()],data[it-keys.begin()]);
    } else {    
      findMedian(keys,data,0,keys.lastIdx(),pivotIndex);
    }
    return true;
  };

  template <class T,class U>
  T quickPartialSort2<T,U>::findMedian(vector<T>& vct,
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
  int quickPartialSort2<T,U>::partition(vector<T>& vct,
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


  template <class T,class U>
  T quickPartialSort2<T,U>::findMedian(std::vector<T>& vct,
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
  int quickPartialSort2<T,U>::partition(std::vector<T>& vct,
                                        vector<U>& data,
                                        const int& begin,
                                        const int& end) const {
    
    int i,j;
    T v,t;
    U d;

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

      d=data.at(i);
      data.at(i)=data.at(j);
      data.at(j)=d;
    }

    t=vct[i];
    vct[i]=vct[end];
    vct[end]=t;

    d=data.at(i);
    data.at(i)=data.at(end);
    data.at(end)=d;

    return i;
  }


}


