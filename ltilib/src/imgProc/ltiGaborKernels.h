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
 * file .......: ltiGaborKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiGaborKernels.h,v 1.4 2003/02/17 07:15:26 alvarado Exp $
 */

#ifndef _LTI_GABORKERNELS_H_
#define _LTI_GABORKERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {
  /** two-dimensional gabor filter kernel

      The Gabor kernels are defined by the following equation:

      \f$ f_{mn}(x,y) =
      \frac{1}{2\pi\sigma^2}\exp(-\frac{x^2+y^2}{2\sigma_m^2})
      \cos\left(\omega_mx\cos\theta_n+\omega_my\sin\theta_n \right) \f$

      The index m denotes the "band" of the filter (which variance it uses)
      and the index n the direction of the filter.

      For this implementation, the volume under the gauss function for the
      specified size will be normalized to 1.  The result will be modulated
      with the given direction and frequency.

      Due to the nature of this filter, avoid its use with unsigned byte
      channels! (no lti::channel8, only lti::channel).

      You can use an approximation of this kern as separable filter
      (see lti::sepKernel<T>::separate()), or you can use the
      lti::gaborKernelSep as a two-filter-pair separable filter.

      Example:

      \code
      // the channel to be filtered:
      lti::channel data;

      // ... initialize channel here ...

      // gabor filter kernel with 7 elements, a variance of 1.0, frequency
      // of 0.5 and 45°
      lti::gaborKernel<channel::value_type> kernel(7,1.0,0.5,45.0*Pi/180.0);

      lti::convolution filter;                        // convolution operator
      lti::convolution::parameters param;             // parameters
      param.setKernel(kernel);                        // use the gabor kernel
      filter.setParameters(param);                    // set parameters

      // filter the data and leave the result there too
      filter.apply(data);
      \endcode
   */
  template<class T>
  class gaborKernel : public kernel2D<T> {
  public:
    /** constructor
        @param size is the number of elements pro dimension (i.e. the
                    filter kern is a 'size x size' kernel!)
        @param theVariance variance of the kernel
        @param frequency the angular frequency to be modulated with
                         (in radians per line)
        @param angle the angle to modulate the gaussian function (in radians)
    */
    gaborKernel(const int& size,
                const double& theVariance,
                const double& frequency,
                const double& angle);

    /** initialize this kernel with the specified values
        @param size size of the kernel in one dimension
        @param theVariance variance of the kernel
        @param frequency the angular frequency to be modulated with
                         (in radians per line)
        @param angle the angle to modulate the gaussian function (in radians)
     */
    void generate(const int& size,
                  const double& theVariance,
                  const double& frequency,
                  const double& angle);
  };

  /** two-dimensional separable gabor filter kernel

      The Gabor kernels are defined by the following equation:

      \f$ f_{mn}(x,y) =
      \frac{1}{2\pi\sigma^2}\exp(-\frac{x^2+y^2}{2\sigma_m^2})
      \cos\left(\omega_mx\cos\theta_n+\omega_my\sin\theta_n \right) \f$

      The index m denotes the "band" of the filter (which variance it uses)
      and the index n the direction of the filter.

      This kernel can be expressed also as:
      \f$ f_{mn} = h_1(x)h_2(x)-h_3(x)h_4(x)\f$

      This two-filter-pair separation property is used in this implementation.

      Due to the nature of this filter, avoid its use with unsigned byte
      channels! (no lti::channel8, only lti::channel).

      Example:


      \code
      // the channel to be filtered:
      lti::channel data;

      // ... initialize channel here ...

      // gabor filter kernel with 7 elements, a variance of 1.0, frequency
      // of 0.5 and 45°
      lti::gaborKernelSep<channel::value_type> kernel(3,1.0,0.5,45*Pi/180.0);

      lti::convolution filter;                        // convolution operator
      lti::convolution::parameters param;             // parameters
      param.setKernel(kernel);                        // use the gauss kernel
      filter.setParameters(param);                    // set parameters

      // filter the vector and leave the result there too

      filter.apply(data);
      \endcode

      Generally, it is faster to use the separable kernel for kernel
      dimension bigger or equal han 5x5.
   */
  template<class T>
  class gaborKernelSep : public sepKernel<T> {
  public:
    /** constructor
        @param size is the number of elements pro dimension (i.e. the
                    filter kern is a 'size x size' kernel!)
        @param theVariance variance of the kernel
        @param frequency the angular frequency to be modulated with
                         (in radians per line)
        @param angle the angle to modulate the gaussian function (in radians)
    */
    gaborKernelSep(const int& size,
                   const double& theVariance,
                   const double& frequency,
                   const double& angle);

    /** initialize this kernel with the specified values
        @param size size of the kernel in one dimension
        @param theVariance variance of the kernel
        @param frequency the angular frequency to be modulated with
                         (in radians per line)
        @param angle the angle to modulate the gaussian function (in radians)
     */
    void generate(const int& size,
                  const double& theVariance,
                  const double& frequency,
                  const double& angle);
  };

}

#include "ltiGaborKernels_template.h"

#endif

