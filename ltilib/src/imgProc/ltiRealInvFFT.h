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
 * file .......: ltiRealInvFFT.h
 * authors ....: Stefan Syberichs, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 06.12.99
 * revisions ..: $Id: ltiRealInvFFT.h,v 1.7 2003/02/17 07:15:33 alvarado Exp $
 */


#ifndef _LTI_REAL_INV_FFT_H_
#define _LTI_REAL_INV_FFT_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiMath.h"
#include "ltiTransform.h"
#include "ltiRealFFT.h"

namespace lti {

  /**
   * A class for inverse FFT.
   * realFFT is a class for Fast Fourier Transforms on lti::vectors
   * and lti::matrix<float>.  The input can either be in polar or cartesic
   * format, specified by the parameter inputMode. The FFT on matrix<float>
   * works full-sized input matrices (i.e the size of the output
   * data), while the vector FFT works only one half (!) of the
   * Fourier coefficients per dimension (real and imaginary). Note
   * that cartesic input data computes faster!  The apply-methods are
   * based on fast inverse FFT-routines written by Takuya Ooura (the original
   * code can be found
   * <a href="http://momonga.t.u-tokyo.ac.jp/~ooura/fft.html">here</a>)
   * that have been adapted for the use on lti::vectors and matrix<float>.
   * Note that the cartesic output is faster !
   * Usage:
   *
   *  \code
   *  #include "ltiRealInvFFT.h"
   *  #include "ltiRealFFT.h"
   *
   *  lti::realFFT fft2d;       // for 2-dimensional FFT
   *  lti::realInvFFT ifft2d;   // for 2-dimensional inverse FFT
   *
   *  lti::realFFT::parameters par2d;
   *
   *  par2d.mode = lti::realFFT::parameters::Polar;
   *
   *  ifft2d.setParameters(par2d);
   *  fft2d.setParameters(par2d);
   *
   *
   *  fft2d.apply(R, re, im);       // the actual FFT
   *
   *  ifft2d.apply(re, im, back);   // inverse FFT
   *  \endcode
   */
  class realInvFFT : public transform {
  public:

    /**
     * Parameter class of the realInvFFT class (are compatible with the
     * parameters of the realFFT functor).
     */
    typedef realFFT::parameters parameters;

    /**
     * constructor
     */
    realInvFFT(void);

    /**
     * detsructor
     */
    ~realInvFFT(void);

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * on-copy method vor vectors. The input data has the half the size of
     * the output data (i.e. only the positive coefficients are used).
     * @param realInput the real part of the Fourier coefficients
     *                  (Polar or Cartesic), size (n/2)+1
     * @param imagInput the imaginary part of the Fourier coefficients
     *                  (Polar or Cartesic), size (n/2)+1
     * @param realOutput the real inverse-computed signal, size n
     */
    void apply(const vector<float>& realInput,
               const vector<float>& imagInput,
                     vector<float>& realOutput) const;

    /**
     * on-copy method vor vectors. The input data has the half the size of
     * the output data (i.e. only the positive coefficients are used).
     * @param realInput the real part of the Fourier coefficients
     *                  (Polar or Cartesic), size (n/2)+1
     * @param imagInput the imaginary part of the Fourier coefficients
     *                  (Polar or Cartesic), size (n/2)+1
     * @param realOutput the real inverse-computed signal, size n
     */
    void apply(const vector<double>& realInput,
               const vector<double>& imagInput,
                     vector<double>& realOutput) const;

    /**
     * on-copy method for matrix<float>. The input data has the half the
     * size of the output data (i.e. only the positive coefficients are used).
     * Note that the  DC component of the signal is in the upper-left corner
     * of the two-dimensional FFT. (corresponds to the FFT output)
     * @param realInput the real part of the Fourier coefficients
     *                  (Polar or Cartesic), size n
     * @param imagInput the imaginary part of the Fourier coefficients
     *                  (Polar or Cartesic), size n
     * @param realOutput the real inverse-computed signal, size n
     */
    void apply(const matrix<float>& realInput,
	       const matrix<float>& imagInput,
                     matrix<float>& realOutput) const;

  };

} // namespace lti
#endif

