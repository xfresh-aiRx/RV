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
 * file .......: ltiErosion_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.99
 * revisions ..: $Id: ltiErosion_template.h,v 1.10 2005/11/23 13:55:58 doerfler Exp $
 */

#include <limits>

namespace lti {

  /*
   * default constructor
   */
  template<class T,class U>
    lti::erosion::accumulatorGray<T,U>::accumulatorGray() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
  void lti::erosion::accumulatorGray<T,U>::reset() {
    state = std::numeric_limits<U>::max();
  }

  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void lti::erosion::accumulatorGray<T,U>::accumulate(const T& filter,
                                                      const T& src) {
    if (filter != 0) {
      state = min(state,static_cast<U>(src)-static_cast<U>(filter));
    }
  }
  template<class T,class U>
    void lti::erosion::accumulatorGray<T,U>::accumulateZero(const T&) {
    // do nothing, filter value is zero by definition
  }

  template<class T,class U>
  void lti::erosion::accumulatorGray<T,U>::accumulateSym(const T& filter,
                                                    const T& srcL,
                                                    const T& srcR) {
    if (filter != 0) {
      const U f = static_cast<U>(filter);
      state = min(state,static_cast<U>(srcL)-f,static_cast<U>(srcR)-f);
    }
  }

  template<class T,class U>
  void lti::erosion::accumulatorGray<T,U>::accumulateASym(const T& filter,
                                                          const T& srcL,
                                                          const T& srcR) {
    if (filter != 0) {
      const U f = static_cast<U>(filter);
      state = min(state,static_cast<U>(srcL)-f,static_cast<U>(srcR)+f);
    }
  }

  template<class T,class U>
  T lti::erosion::accumulatorGray<T,U>::getResult() const {
    return (static_cast<T>(state));
  }

  template<class T,class U>
  void lti::erosion::accumulatorGray<T,U>::setNorm(const T& n) {
    norm = n;
  }


  // specialization for  T = ubyte, U = int
  template<>
  ubyte lti::erosion::accumulatorGray<ubyte, int>::getResult() const;


  //============================================================
  // Binary
  //============================================================

  template<class T,class U>
  lti::erosion::accumulatorBin<T,U>::accumulatorBin() {
    reset();
  }

  template<class T,class U>
  void lti::erosion::accumulatorBin<T,U>::reset() {
    state = static_cast<U>(norm);
  }

  template<class T,class U>
  void lti::erosion::accumulatorBin<T,U>::accumulate(const T& filter,
                                                const T& src) {
    if ((state != 0) && (filter != 0) && (src == 0)) {
      state = U(0);
    }
  }

  template<class T,class U>
  void lti::erosion::accumulatorBin<T,U>::accumulateZero(const T&) {
    // do nothing, filter value is zero by definition
  }

  template<class T,class U>
  void lti::erosion::accumulatorBin<T,U>::accumulateSym(const T& filter,
                                                   const T& srcL,
                                                   const T& srcR) {
    if ((state != 0) && (filter != 0) && ((srcL == 0) || (srcR == 0))) {
      state = U(0);
    }
  }

  template<class T,class U>
  void lti::erosion::accumulatorBin<T,U>::accumulateASym(const T& filter,
                                                    const T& srcL,
                                                    const T& srcR) {
    if ((state != 0) && (filter != 0) && ((srcL == 0) || (srcR == 0))) {
      state = U(0);
    }
  }

  template<class T,class U>
  T lti::erosion::accumulatorBin<T,U>::getResult() const {
    return static_cast<T>(state);
  }

  template<class T,class U>
  void lti::erosion::accumulatorBin<T,U>::setNorm(const T& n) {
    norm = n;
  }

}

