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
 * file .......: ltiSort_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.8.2000
 * revisions ..: $Id: ltiSort_template.h,v 1.19 2004/11/16 15:05:15 zieren Exp $
 */

namespace lti {

  // --------------------------------------------------
  // sort
  // --------------------------------------------------

  // default constructor
  template <class T>
  sort<T>::sort(const bool& descendingOrder)
    : functor(){
    parameters param;
    if (descendingOrder) {
      param.order = sort<T>::parameters::Descending;
    } else {
      param.order = sort<T>::parameters::Ascending;
    }
    setParameters(param);
  }


  // default constructor
  template <class T>
  sort<T>::sort(const parameters& par)
    : functor() {
    setParameters(par);
  }


  // copy constructor
  template <class T>
  sort<T>::sort(const sort& other)
    : functor()  {
    copy(other);
  }

  // destructor
  template <class T>
  sort<T>::~sort() {
  }

  // returns the name of this type
  template <class T>
  const char* sort<T>::getTypeName() const {
    return "sort";
  }

  // copy member
  template <class T>
  sort<T>& sort<T>::copy(const sort<T>& other) {
      functor::copy(other);
    return (*this);
  }

  // clone member
  template <class T>
  functor* sort<T>::clone() const {
    return new sort(*this);
  }

  // return parameters
  template <class T>
  const typename sort<T>::parameters&
  sort<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool sort<T>::updateParameters() {
    const parameters& p = getParameters();
    thresholdForBubble = p.thresholdForBubble;
    order = p.order;
    return true;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<T>!
  template <class T>
  bool sort<T>::apply(matrix<T>& srcdest) const {
    vector<T> tmpvct;
    tmpvct.useExternData(srcdest.rows()*srcdest.columns(),
                         &srcdest.at(0,0),
                         true);

    return apply(tmpvct);
  };

  // On place apply for type vector<T>!
  template <class T>
  bool sort<T>::apply(vector<T>& srcdest) const {

    quicksort(srcdest,0,srcdest.lastIdx());

    return true;
  };

  // On copy apply for type matrix<T>!
  template <class T>
  bool sort<T>::apply(const matrix<T>& src,matrix<T>& dest) const {

    dest.copy(src);
    return apply(dest);
  };

  // On copy apply for type vector<T>!
  template <class T>
  bool sort<T>::apply(const vector<T>& src,vector<T>& dest) const {

    dest.copy(src);
    return apply(dest);
  };

  template<class T>
  void sort<T>::insertionsortAsc(vector<T>& vct,
                                 const int& begin,
                                 const int& end) const {
    T a;
    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(a < vct.at(i))) {
          break;
        }
        vct.at(i+1)=vct.at(i);
      }
      vct.at(i+1)=a;
    }
  }

  template<class T>
  void sort<T>::insertionsortDesc(vector<T>& vct,
                                  const int& begin,
                                  const int& end) const {
    T a;
    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(vct.at(i) < a)) {
          break;
        }
        vct.at(i+1)=vct.at(i);
      }
      vct.at(i+1)=a;
    }
  }

  template<class T>
  void sort<T>::quicksort(vector<T>& vct,
                          const int& begin,
                          const int& end) const {

    int i;

    if (order == parameters::Ascending) {

      if ((end-begin) > thresholdForBubble) {
        i = partitionAsc(vct,begin,end);
        quicksort(vct,begin,i-1);
        quicksort(vct,i+1,end);
      } else {
        insertionsortAsc(vct,begin,end);
      }
    } else { // Descending order

      if (end-begin > thresholdForBubble) {
        i = partitionDesc(vct,begin,end);
        quicksort(vct,begin,i-1);
        quicksort(vct,i+1,end);
      } else {
        insertionsortDesc(vct,begin,end);
      }
    }
  }

  template<class T>
  int sort<T>::partitionDesc(vector<T>& vct,
                             const int& begin,
                             const int& end) const {

    int i,j;
    T v,t;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (v < vct.at(++i) && i < end);
      while ( (--j >= 0) && (vct.at(j) < v) ) ;
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
  int sort<T>::partitionAsc(vector<T>& vct,
                            const int& begin,
                            const int& end) const {

    int i,j;
    T v,t;

    v = vct.at(end);
    i = begin-1;
    j = end;
    for(;;) {
      while (vct.at(++i) < v && i < end);
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

  // -----------------------------------------------------------------
  // sort2
  // -----------------------------------------------------------------

  // default constructor
  template <class T,class U>
  sort2<T,U>::sort2(const bool& descendingOrder,const bool& sortRows) {
    parameters par;

    if (descendingOrder) {
      par.order = sort2<T,U>::parameters::Descending;
    } else {
      par.order = sort2<T,U>::parameters::Ascending;
    }

    if (sortRows) {
      par.whichVectors = sort2<T,U>::parameters::Rows;
    } else {
      par.whichVectors = sort2<T,U>::parameters::Columns;
    }

    setParameters(par);
  }

  // default constructor
  template <class T,class U>
  sort2<T,U>::sort2(const parameters& par)
    : sort<T>(par) {
  }


  // copy constructor
  template <class T,class U>
  sort2<T,U>::sort2(const sort2& other)
    : sort<T>() {
    copy(other);
  }

  // destructor
  template <class T,class U>
  sort2<T,U>::~sort2() {
  }

  // returns the name of this type
  template <class T,class U>
  const char* sort2<T,U>::getTypeName() const {
    return "sort2";
  }

  // copy member
  template <class T,class U>
  sort2<T,U>& sort2<T,U>::copy(const sort2<T,U>& other) {
    sort<T>::copy(other);
    return (*this);
  }

  // clone member
  template <class T,class U>
  functor* sort2<T,U>::clone() const {
    return new sort2(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<T>!
  template <class T,class U>
  bool sort2<T,U>::apply(matrix<T>& key,matrix<U>& srcdest) const {
    if (key.size() != srcdest.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    vector<T> tmpvct;
    tmpvct.useExternData(key.rows()*key.columns(),
                         &key.at(0,0),
                         true);

    vector<U> tmpvct2;
    tmpvct2.useExternData(srcdest.rows()*srcdest.columns(),
                          &srcdest.at(0,0),
                          true);

    return apply(tmpvct,tmpvct2);
  };

  // On place apply for type vector<T>!
  template <class T,class U>
    bool sort2<T,U>::apply(vector<T>& key,vector<U>& srcdest) const {
    if (key.size() != srcdest.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    quicksort(key,srcdest,0,srcdest.lastIdx());

    return true;
  };


  template <class T,class U>
  void sort2<T,U>::reorder(const vector<int>& indices,
                           const matrix<U>& src,
                                 matrix<U>& dest) const {
    const parameters& param = getParameters();
    int i,j;

    dest.resize(src.size(),U(),false,false);

    if (param.whichVectors == sort2<T,U>::parameters::Rows) {
      for (i=0;i<indices.size();++i) {
        dest.getRow(i).copy(src.getRow(indices.at(i)));
      }
    } else {
      // reorder the columns
      for (i=0;i<indices.size();++i) {
        const int idx = indices.at(i);
        for (j=0;j<dest.rows();++j) {
          dest.at(j,i) = src.at(j,idx);
        }
      }
    }
  }

  template <class T,class U>
  bool sort2<T,U>::apply(vector<T>& key, matrix<U>& srcdest) const {
    if (key.size() != srcdest.rows()) {
      this->setStatusString("Vector size must be equal to the number of rows");
      return false;
    }

    int i;
    ivector indices;
    indices.resize(key.size(),0,false,false);

    for (i=0;i<indices.size();++i) {
      indices.at(i) = i;
    }

    sort2<T,int> sorter(getParameters().order == parameters::Descending);

    if (sorter.apply(key,indices)) {
      matrix<U> tmp;
      reorder(indices,srcdest,tmp);
      tmp.detach(srcdest);
    } else {
      return false;
    }

    return true;
  };

  template <class T,class U>
  bool sort2<T,U>::apply(const vector<T>& key, const matrix<U>& src,
                               vector<T>& keydest, matrix<U>& dest) const {
    if (key.size() != src.rows()) {
      this->setStatusString("Vector size must be equal to the number of rows");
      return false;
    }

    keydest.copy(key);

    int i;
    ivector indices;
    indices.resize(key.size(),0,false,false);

    for (i=0;i<indices.size();++i) {
      indices.at(i) = i;
    }

    sort2<T,int> sorter(getParameters().order ==
                        sort<T>::parameters::Descending);

    if (sorter.apply(keydest,indices)) {
      reorder(indices,src,dest);
    } else {
      return false;
    }

    return true;
  };

  // On copy apply for type matrix<T>!
  template <class T,class U>
  bool sort2<T,U>::apply(const matrix<T>& key,const matrix<U>& src,
                               matrix<T>& keydest,matrix<U>& dest) const {
    if (key.size() != src.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    keydest.copy(key);
    dest.copy(src);

    return apply(keydest,dest);
  };

  // On copy apply for type vector<T>!
  template <class T,class U>
  bool sort2<T,U>::apply(const vector<T>& key,const vector<U>& src,
                               vector<T>& keydest,vector<U>& dest) const {
    if (key.size() != src.size()) {
      this->setStatusString("Key and Value data must have the same size");
      return false;
    }

    keydest.copy(key);
    dest.copy(src);

    return apply(keydest,dest);
  };

  // use one vector to sort the other one

  template<class T,class U>
    void sort2<T,U>::insertionsortAsc(vector<T>& vct,
                                      vector<U>& vct2,
                                      const int& begin,
                                      const int& end) const {
    T a;
    U a2;

    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      a2 = vct2.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(a < vct.at(i))) {
          break;
        }
        vct.at(i+1)=vct.at(i);
        vct2.at(i+1)=vct2.at(i);
      }
      vct.at(i+1)=a;
      vct2.at(i+1)=a2;
    }
  }

  template<class T,class U>
    void sort2<T,U>::insertionsortDesc(vector<T>& vct,
                                       vector<U>& vct2,
                                       const int& begin,
                                       const int& end) const {
    T a;
    U a2;
    int i,j;
    for (j=begin+1;j<=end;j++) {
      a = vct.at(j);
      a2 = vct2.at(j);
      for (i=j-1;i>=begin;i--) {
        if (!(vct.at(i) < a)) {
          break;
        }
        vct.at(i+1)=vct.at(i);
        vct2.at(i+1)=vct2.at(i);
      }
      vct.at(i+1)=a;
      vct2.at(i+1)=a2;
    }
  }

  template<class T,class U>
    void sort2<T,U>::quicksort(vector<T>& vct,
                               vector<U>& vct2,
                               const int& begin,
                               const int& end) const {

    int i;

    if (this->order == parameters::Ascending) {

      if ((end-begin) > this->thresholdForBubble) {
        i = partitionAsc(vct,vct2,begin,end);
        quicksort(vct,vct2,begin,i-1);
        quicksort(vct,vct2,i+1,end);
      } else {
        insertionsortAsc(vct,vct2,begin,end);
      }

    } else {
      if (end-begin > this->thresholdForBubble) {
        i = partitionDesc(vct,vct2,begin,end);
        quicksort(vct,vct2,begin,i-1);
        quicksort(vct,vct2,i+1,end);
      } else {
        insertionsortDesc(vct,vct2,begin,end);
      }
    }
  }

  template<class T,class U>
    int sort2<T,U>::partitionDesc(vector<T>& vct,
                                  vector<U>& vct2,
                                  const int& begin,
                                  const int& end) const {

    int i,j;
    T v,t;
    U t2;

    v = vct.at(end);

    i = begin-1 ;
    j = end ;
    for(;;) {
      while (v < vct.at(++i) && i < end);
      while ( (--j >= 0) && (vct.at(j) < v) ) ;
      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;

      t2=vct2.at(i);
      vct2.at(i)=vct2.at(j);
      vct2.at(j)=t2;

    }

    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    t2=vct2.at(i);
    vct2.at(i)=vct2.at(end);
    vct2.at(end)=t2;

    return i;
  }

  template<class T,class U>
    int sort2<T,U>::partitionAsc(vector<T>& vct,
                                 vector<U>& vct2,
                                 const int& begin,
                                 const int& end) const {

    int i,j;
    T v,t;
    U t2;

    v = vct.at(end);
    i = begin-1 ;
    j = end ;
    for(;;) {
      while (vct.at(++i) < v && i < end);
      while ( (--j >= 0) && (v < vct.at(j)) ) ;
      if ( i >= j ) {
        break ;
      }
      t=vct.at(i);
      vct.at(i)=vct.at(j);
      vct.at(j)=t;

      t2=vct2.at(i);
      vct2.at(i)=vct2.at(j);
      vct2.at(j)=t2;
    }
    t=vct.at(i);
    vct.at(i)=vct.at(end);
    vct.at(end)=t;

    t2=vct2.at(i);
    vct2.at(i)=vct2.at(end);
    vct2.at(end)=t2;

    return i;
  }

  // return parameters
  template <class T,class U>
  const typename sort2<T,U>::parameters&
  sort2<T,U>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *par;
  }

}


