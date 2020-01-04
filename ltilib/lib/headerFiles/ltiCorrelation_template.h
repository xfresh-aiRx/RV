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
 * file .......: ltiCorrelation_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.99
 * revisions ..: $Id: ltiCorrelation_template.h,v 1.7 2004/08/24 13:44:04 doerfler Exp $
 */

namespace lti {

  /* --------------------------------------------------------
   * CLASSIC
   * --------------------------------------------------------
   */

  /*
   * default constructor
   */
  template<class T,class U>
    lti::correlation::classic<T,U>::classic() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
    void lti::correlation::classic<T,U>::reset() {
    state = static_cast<U>(0);
  }

  /*
   * classic (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::correlation::classic<T,U>::accumulate(const T& filter,
                                                        const T& src) {
    state += static_cast<U>(src*filter);
  }
  template<class T,class U>
    void lti::correlation::classic<T,U>::accumulateZero(const T& src) {
  }

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::classic<T,U>::accumulateSym(const T& filter,
										const T& srcL,const T& srcR) {
    state += static_cast<U>((srcL+srcR)*filter);
  }
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::classic<T,U>::accumulateASym(const T& filter,
										const T& srcL,const T& srcR) {
    state += static_cast<U>((srcL-srcR)*filter);
  }

  /*
   * classic specialization for ubyte elements
   */
  template<>
  void lti::correlation::classic<ubyte,int>::accumulate(
      const ubyte& filter,
      const ubyte& src);

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T lti::correlation::classic<T,U>::getResult() const {
    return (static_cast<T>(state));
  }

  /*
   * specialization for integers
   */
  template<>
  int lti::correlation::classic<int,int>::getResult() const;

  template<>
  ubyte lti::correlation::classic<ubyte,int>::getResult() const;


  /*
   * set norm
   */
  template<class T,class U>
  void lti::correlation::classic<T,U>::setNorm(const T& n) {
    norm = n;
  }



  /* --------------------------------------------------------
   * COEFFICIENT
   * --------------------------------------------------------
   */


  /*
   * default constructor
   */
  template<class T,class U>
    lti::correlation::coefficient<T,U>::coefficient() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
    void lti::correlation::coefficient<T,U>::reset() {
    sumW=sumWW=sumFW=sumFF=sumF=static_cast<U>(0);
  }

  /*
   * coefficient (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::correlation::coefficient<T,U>::accumulate(const T& filter,
                                                        const T& src) {

    U tmp = static_cast<U>(filter-kernAvg);
    sumW+=tmp;
    sumWW+=(tmp*tmp);
    sumFW+=(src*tmp);
    sumFF+=(src*src);
    sumF+=src;
  }
  template<class T,class U>
    void lti::correlation::coefficient<T,U>::accumulateZero(const T& src) {

    U tmp = static_cast<U>(-kernAvg);
    sumW+=tmp;
    sumWW+=(tmp*tmp);
    sumFW+=(src*tmp);
    sumFF+=(src*src);
    sumF+=src;
  }

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::coefficient<T,U>::accumulateSym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::coefficient<T,U>::accumulateASym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

  /*
   * coefficient specialization for ubyte elements
   */
  template<>
  void lti::correlation::coefficient<ubyte,int>::accumulate(
      const ubyte& filter,
      const ubyte& src);

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T lti::correlation::coefficient<T,U>::getResult() const {
    U tmp = (sumFF-(sumF*sumF/kernSize))*sumWW;
    if (tmp>0)
      return static_cast<T>((sumFW-(sumF*sumW/kernSize))/sqrt(tmp));
    else
      return 0;
  }

  template<class T,class U>
  void lti::correlation::coefficient<T,U>::setData(const T& kernelAverage,
                                                   const T& kernelSize)  {
    kernSize = kernelSize;
    kernAvg = kernelAverage;
  }


  /*
   * specialization for integers
   */
  template<>
  int lti::correlation::coefficient<int,int>::getResult() const;

  template<>
  ubyte lti::correlation::coefficient<ubyte,int>::getResult() const;

  /*
   * set norm
   */
  template<class T,class U>
  void
    lti::correlation::coefficient<T,U>::setNorm(const T& n) {
    norm = n;
  }


  /* --------------------------------------------------------
   * MATCHING CRITERIUM 1
   * --------------------------------------------------------
   */

  /*
   * default constructor
   */
  template<class T,class U>
    lti::correlation::criterium1<T,U>::criterium1() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
    void lti::correlation::criterium1<T,U>::reset() {
    state = static_cast<U>(0);
  }

  /*
   * criterium1 (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::correlation::criterium1<T,U>::accumulate(const T& filter,
                                                       const T& src) {
    if (src>filter) {
      state = max(state,static_cast<U>(src-filter));
    } else {
      state = max(state,static_cast<U>(filter-src));
    }
  }
  template<class T,class U>
    void lti::correlation::criterium1<T,U>::accumulateZero(const T& src) {
      accumulate(T(0),src);
  }

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::criterium1<T,U>::accumulateSym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::criterium1<T,U>::accumulateASym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T lti::correlation::criterium1<T,U>::getResult() const {
    return (static_cast<T>(static_cast<U>(1)/(state+static_cast<U>(1))));
  }

  /*
   * specialization for integers
   */
  template<>
  int lti::correlation::criterium1<int,int>::getResult() const;

  template<>
  ubyte lti::correlation::criterium1<ubyte,int>::getResult() const;

  /*
   * set norm
   */
  template<class T,class U>
  void lti::correlation::criterium1<T,U>::setNorm(const T& n) {
    norm = n;
  }

  /* --------------------------------------------------------
   * MATCHING CRITERIUM 2
   * --------------------------------------------------------
   */

  /*
   * criterium2 (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::correlation::criterium2<T,U>::accumulate(const T& filter,
                                                       const T& src) {
    if (filter>src) {
      this->state += static_cast<U>(filter-src);
    } else {
      this->state += static_cast<U>(src-filter);
    }

  }
  template<class T,class U>
    void lti::correlation::criterium2<T,U>::accumulateZero(const T& src) {
    accumulate(T(0),src);
  }

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::criterium2<T,U>::accumulateSym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::criterium2<T,U>::accumulateASym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

  /* --------------------------------------------------------
   * MATCHING CRITERIUM 3
   * --------------------------------------------------------
   */

  /*
   * criterium3 (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::correlation::criterium3<T,U>::accumulate(const T& filter,
                                                       const T& src) {
    U tmp = static_cast<U>(filter-src);
    this->state += tmp*tmp;
  }
  template<class T,class U>
    void lti::correlation::criterium3<T,U>::accumulateZero(const T& src) {
    this->state += static_cast<U>(src*src);
  }

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::criterium3<T,U>::accumulateSym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(filter,srcR);
  }
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
  template<class T,class U>
    void lti::correlation::criterium3<T,U>::accumulateASym(const T& filter,
										const T& srcL,const T& srcR) {
    accumulate(filter,srcL);
    accumulate(-filter,srcR);
  }

}

