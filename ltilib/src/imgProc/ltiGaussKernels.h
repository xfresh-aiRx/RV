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
 * file .......: ltiGaussKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiGaussKernels.h,v 1.9 2005/04/27 15:14:30 doerfler Exp $
 */

#ifndef _LTI_GAUSSKERNELS_H_
#define _LTI_GAUSSKERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {
  /**
   * one-dimensional filter kernel
   *
   *  This class will create a one-dimensional gaussian filter kernel.
   *
   *  The area under the filter will be normalized to one.
   *
   *  The one dimesional kernel is calculated with following equation:
   *
   *  \f$ g(x) = \frac{1}{\sigma \sqrt{2\pi}} \exp{-\frac{x^2}{2\sigma^2}}\f$
   *
   *  Example:
   *
   *  \code
   *  // the vector to be filtered:
   *  lti::vector<channel::value_type> data;
   *
   *  // ... initialize vector here ...
   *
   *  // gaussian filter kernel with 3 elements, and a variance of 0.72
   *  lti::gaussKernel1D<channel::value_type> kernel(3,0.72);
   *
   *  lti::convolution filter;                        // convolution operator
   *  lti::convolution::parameters param;             // parameters
   *  param.setKernel(kernel);                        // use the gauss kernel
   *  filter.setParameters(param);                    // set parameters
   *
   *  // filter the vector and leave the result there too
   *
   *  filter.apply(data);
   *  \endcode
   */
  template<class T>
  class gaussKernel1D : public kernel1D<T> {
  public:
    /**
     * constructor
     * @param size size of the kernel in one dimension
     * @param variance variance of the kernel.  If this argument is
     *        ommited or a negative value is given, the variance will
     *        be calculated such that the value at the index floor(size/2)
     *        is 1/(1+floor(size/2)) times the value at index 0.
     *        For example, for size==3, the value at 1 will be 1/2 the value
     *        at 0.
     *        Hence a 3 taps kernel will contain the elements (1/4,1/2,1/4).
     */
    gaussKernel1D(const int& size = 3,
                  const double& variance = -1.0);

    /** initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     * @param theVariance variance of the kernel. If this argument is
     *        ommited or a negative value is given, the variance will
     *        be calculated such that the value at the index floor(size/2)
     *        is 1/(1+floor(size/2)) times the value at index 0.
     *        For example, for size==3, the value at 1 will be 1/2 the value
     *        at 0.
     *        Hence a 3 taps kernel will contain the elements (1/4,1/2,1/4).
     */
    void generate(const int& size,
                  const double& theVariance=-1.0);

    /**
     * Returns the actual variance used for generating the
     * kernel. This is helpful if a negative value was given in the
     * generation.
     */
    const double& getActualVariance() const;
  protected:
    double variance;
  };

  /** 
   * Two-dimensional Gaussian filter kernel
   *
   * Gaussian kernels are separable, and will be created this way!
   * (@see  gaussKernel1D)
   *
   * You can create a "real" 2D kernel with the following code
   *
   * \code
   * lti::gaussKernel2D<float> gauss(5); // a kernel 5x5 with default variance
   * lti::kernel2D<float>      kern;     // a 2D kernel;
   * 
   * kern.castFrom(gauss);
   * \endcode
   *
   * but note that the convolution of this kernel with a channel is less
   * efficient than convolving its separable version.
   *
   * To convolve this filter with a channel follow the next example
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data;
   *
   * // ... initialize channel here ...
   *
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::channel::value_type> kernel(5,1.3);
   *
   * lti::convolution filter;                        // convolution operator
   * lti::convolution::parameters param;             // parameters
   * param.setKernel(kernel);                        // use the gauss kernel
   * filter.setParameters(param);                    // set parameters
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   * 
   * \endcode
   *
   * You can also use following shortcut, if you can use the default
   * boundary type for the convolution:
   *
   * \code
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::channel::value_type> kernel(5,1.3);
   *
   * lti::convolution filter(kernel);                // convolution operator
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   *
   * \endcode
   */
  template<class T>
  class gaussKernel2D : public sepKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     * @param theVariance variance of the kernel.  If negative, a default
     *             value from the given size will be computed 
     *             (see lti::gaussKernel1D<T>)
     */
    gaussKernel2D(const int& size = 3,
                  const double& theVariance =  1.4426950409);

    /** 
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     * @param theVariance variance of the kernel.  If negative, a default
     *                    value from the given size will be computed 
     *                    (see lti::gaussKernel1D<T>)
     */
    void generate(const int& size,
                  const double& theVariance);

    /**
     * Returns the actual variance used for generating the
     * kernel. This is helpful if a negative value was given in the
     * generation.
     */
    const double& getActualVariance() const;
  protected:
    double variance;
  };
}

#include "ltiGaussKernels_template.h"

#endif

