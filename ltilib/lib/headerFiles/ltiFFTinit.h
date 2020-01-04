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
 * file .......: ltiFFTinit.h
 * authors ....: Stefan Syberichs, Takuyka Ooura
 * organization: LTI, RWTH Aachen
 * comment.....: inverse FFT routines based on modified code by Takuya Ooura
 *               with kind permission ooura@kurims.kyoto-u.ac.jp or
 *               ooura@mmm.t.u-tokyo.ac.jp)
 * creation ...: 28.01.2000
 * revisions ..: $Id: ltiFFTinit.h,v 1.6 2003/02/17 07:15:24 alvarado Exp $
 */

#ifndef _LTI_FFTINIT_H
#define _LTI_FFTINIT_H

namespace lti{

  /**
   * Namespace with all FFT and inverse FFT routines based on code by
   * Takuya Ooura with kind permission ooura@kurims.kyoto-u.ac.jp or
   * ooura@mmm.t.u-tokyo.ac.jp).  The code has been adapted to use C++
   * and LTI-lib conventions.  This functions will be used by the
   * functors lti::realFFT and lti::realInvFFT
   */
  namespace fft {
    // prototypes for c-functions

    /**
     * reserve memory for a n-dimensional vector of elements with type T
     * @param n1 dimension of the vector
     * @return pointer to the elements of the vector
     */
    template<class T>
    T* alloc_1d(const int& n1);

    /**
     * free memory of 1d array of elements with type T
     */
    template<class T>
    void free_1d(T* i);

    /**
     * reserve memory for a n1xn2-matrix of elementes of type T
     * @param n1 number of rows
     * @param n2 number of columns
     * @return pointer to the elements of the vector
     */
    template<class T>
    T** alloc_2d(const int& n1, const int& n2);

    /**
     * free memory of 2d array of elements with type T
     */
    template<class T>
    void free_2d(T** ii);

    /**
     * real discrete fourier transform for lti::vectors
     * @param n data length (must be power of 2)
     * @param isgn transform direction: +1 for transform and -1 for inverse.
     * @param a the input/output data.  In the frecuency domain the real
     *          parts will be stored in the even locations and the imaginary
     *          parts in the odd locations.  a[1] = real[n/2]
     * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
     *           cos/sin table
     * @param w cos/sin table.
     *3
     * w[] and ip[] will be initialized if ip[0] == 0
     */
    template<class T>
    void vrdft(int n, int isgn, vector<T>& a, int *ip, T *w);

    template<class T>
    void rdft2d(int, int, int, matrix<T>&, T *, int *, T *);

    template<class T>
    void makewt(int nw, int *ip, T *w);

    template<class T>
    void makect(int nc, int *ip, T *c);

    template<class T>
    void bitrv2(int n, int *ip, T *a);

    template<class T>
    void cftfsub(int n, T *a, T *w);

    template<class T>
    void cftbsub(int n, T *a, T *w);

    template<class T>
    void rftfsub(int n, T *a, int nc, T *c);

    template<class T>
    void rftbsub(int n, T *a, int nc, T *c);

    /**
     * real discrete fourier transform
     * @param n data length (must be power of 2)
     * @param isgn transform direction: +1 for transform and -1 for inverse.
     * @param a the input/output data.  In the frecuency domain the real
     *          parts will be stored in the even locations and the imaginary
     *          parts in the odd locations.  a[1] = real[n/2]
     * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
     *           cos/sin table
     * @param w cos/sin table.
     *
     * w[] and ip[] will be initialized if ip[0] == 0
     */
    template<class T>
    void rdft(int n, int isgn, T *a, int *ip, T *w);

    /**
     * complex direct fourier transform
     * @param n data length (must be power of 2)
     * @param isgn transform direction: +1 for transform and -1 for inverse.
     * @param a the input/output data.  In the frecuency domain the real
     *          parts will be stored in the even locations and the imaginary
     *          parts in the odd locations.  a[1] = real[n/2]
     * @param ip work area for bit reversal.  ip[0] and ip[1] are pointers to
     *           cos/sin table
     * @param w cos/sin table.
     *
     * w[] and ip[] will be initialized if ip[0] == 0
     */
    template<class T>
    void cdft(int n, int isgn, T *a, int *ip, T *w);

    template<class T>
    void cft1st(int n, T *a, T *w);

    template<class T>
    void cftmdl(int n, int l, T *a, T *w);

    template<class T>
    void bitrv2conj(int n, int *ip, T *a);
  }
}

#endif

