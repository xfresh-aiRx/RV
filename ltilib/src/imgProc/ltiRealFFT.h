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
 * file .......: ltiRealFFT.h
 * authors ....: Stefan Syberichs, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 06.12.99
 * revisions ..: $Id: ltiRealFFT.h,v 1.13 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_REAL_FFT_H_
#define _LTI_REAL_FFT_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiMath.h"
#include "ltiTransform.h"

namespace lti {

  /**
   * A class for FFT.
   * realFFT is a class for Fast Fourier Transforms on lti::vectors
   * and lti::channels.  The output can either be in polar or cartesic
   * format, specified by the parameter mode. The FFT on
   * channels produces two full-sized output matrices (i.e the size of
   * the input data), while the vector FFT generates only one half (!)
   * of the Fourier coefficients per dimension (real and
   * imaginary). The output data will be padded to a power of 2 (is
   * done automatically).
   *
   * In the case of channels, the DC component of the signal is in the
   * upper-left corner of the two-dimensional FFT!  The apply-methods
   * are based on fast FFT-routines written by Takuya Ooura (the original
   * code can be found
   * <a href="http://momonga.t.u-tokyo.ac.jp/~ooura/fft.html">here</a>)
   * that have been adapted for the use on lti::vectors and channels. Note
   * that the cartesic output computes faster.
   *
   * Usage:
   *  \code
   *  #include "ltiRealFFT.h"
   *  #include "ltiRealInvFFT.h"
   *
   *  lti::realFFT fft2d;       // for 2-dimensional FFT
   *  lti::realInvFFT ifft2d;   // for 2-dimensional inverse FFT
   *
   *  lti::realFFT::parameters par2d;
   *  lti::realInvFFT::parameters ipar2d;
   *
   *  par2d.mode = lti::realFFT::parameters::Polar;
   *
   *  ifft2d.setParameters(par2d);
   *  fft2d.setParameters(par2d);
   *
   *  fft2d.apply(R, re, im);       // the actual FFT
   *
   *  ifft2d.apply(re, im, back);   // inverse FFT
   *  \endcode
   */
  class realFFT : public transform {
  public:

    /**
     * Parameter class of the realFFT class.
     * the one and only parameter is the mode (i.e. Cartesic or Polar).
     * Note that the cartesic transformation is faster.
     */
    class parameters : public transform::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       */
      parameters(const parameters& other) 
        : transform::parameters() {copy(other);};

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this class
       */
      virtual const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
      /**
       * enum type for mode parameter
       */
      enum eMode{
        Cartesic, /**< cartesic coordinates */
        Polar     /**< polar coordinates */
      };

      /**
       * format of the output vectors
       * can be either cartesic or polar. Note that cartesic is faster.
       * Default: Cartesic
       */
      eMode mode;

    };

    /**
     * constructor
     */
    realFFT(void);

    /**
     * destructor
     */
    ~realFFT(void);

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
     * on-copy operator for vectors. The size of the output data is half of
     * the input data (possibly padded to power of 2).
     * The output vectors will have the dimension of the padded input divided
     * by 2 plus one.
     * @param input the real input data (constant)
     * @param realOutput the real output data (e.g. FFT: the real part)
     * @param imagOutput the imaginary output data (e.g. FFT: the imaginary
     * part)
     */
    virtual void apply(const vector<float>& input,
		       vector<float>& realOutput,
		       vector<float>& imagOutput) const;

    /**
     * on-copy operator for vectors. The size of the output data is half of
     * the input data (possibly padded to power of 2).
     * The output vectors will have the dimension of the padded input divided
     * by 2 plus one.
     * @param input the real input data (constant)
     * @param realOutput the real output data (e.g. FFT: the real part)
     * @param imagOutput the imaginary output data (e.g. FFT: the imaginary
     * part)
     */
    virtual void apply(const vector<double>& input,
		       vector<double>& realOutput,
		       vector<double>& imagOutput) const;

    /**
     * on-copy operator for channels.
     * The output is full-size (possibly padded to power of 2).
     * The DC component of the signal is in the  upper-left corner of the
     * two-dimensional FFT!
     * @param input the real input data (constant)
     * @param realOutput the real output data (e.g. FFT: the real part)
     * @param imagOutput the imaginary output data (e.g. FFT: the imaginary
     * part)
     */
    void apply(const matrix<float>& input,
                     matrix<float>& realOutput,
                     matrix<float>& imagOutput) const;

  };
} // namespace lti
#endif


