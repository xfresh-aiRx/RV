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
 * file .......: ltiConvolution_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.99
 * revisions ..: $Id: ltiConvolution_template.h,v 1.5 2005/11/10 09:58:02 doerfler Exp $
 */

namespace lti {

  /*
   * default constructor
   */
  template<class T,class U>
    lti::convolution::accumulator<T,U>::accumulator() {
    reset();
  }

  /*
   * reset
   */
  template<class T,class U>
    void lti::convolution::accumulator<T,U>::reset() {
    state = static_cast<U>(0);
  }

  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::convolution::accumulator<T,U>::accumulate(const T& filter,
                                                        const T& src) {
    state += static_cast<U>(src*filter);
  }
  template<class T,class U>
    void lti::convolution::accumulator<T,U>::accumulateZero(const T& src) {
  }

 /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::convolution::accumulator<T,U>::accumulateSym(const T& filter,
                                                        const T& srcL,
														const T& srcR) {
    state += static_cast<U>((srcL+srcR)*filter);
  }

 /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
    void lti::convolution::accumulator<T,U>::accumulateASym(const T& filter,
                                                        const T& srcL,
														const T& srcR) {
    state += static_cast<U>((srcL-srcR)*filter);
  }

  /*
   * accumulator specialization for ubyte elements
   */
  template<>
  void lti::convolution::accumulator<ubyte,int>::accumulate(
      const ubyte& filter,
      const ubyte& src);

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T lti::convolution::accumulator<T,U>::getResult() const {
    return (static_cast<T>(state));
  }

  /*
   * specialization for integers
   */
  template<>
  int lti::convolution::accumulator<int,int>::getResult() const;

  template<>
  ubyte lti::convolution::accumulator<ubyte,int>::getResult() const;


  /*
   * set norm
   */
  template<class T,class U>
  void lti::convolution::accumulator<T,U>::setNorm(const T& n) {
    norm = n;
  }

}

