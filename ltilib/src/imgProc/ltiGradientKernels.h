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
 * file .......: ltiGradientKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 11.07.00
 * revisions ..: $Id: ltiGradientKernels.h,v 1.10 2004/03/24 11:30:08 doerfler Exp $
 */

/**
 * \file ltiGradientKernels.h Defines several linear kernels for the
 * computation of the gradient, like the Ando kernels, and other classical
 * ones like Sobel, Prewitt, etc.
 */ 

#ifndef _LTI_GRADIENTKERNELS_H_
#define _LTI_GRADIENTKERNELS_H_

#include "ltiLinearKernels.h"
#include <limits>

namespace lti {
  /**
   * Two-dimensional separable filter kernels for the gradient approximation
   *
   * This class will create a two-dimensional gradient filter kernel.
   *
   * There are three posibilities for the kernel dimensions 3x3, 4x4 or 5x5.
   * The approximation is done using the suggestions of Ando in its paper
   * "Consistent Gradient Operators" (IEEE PAMI, Vol. 22, No. 3, March 2000)
   *
   * These suggested kernels are separable and are of course implemented that
   * way (much faster!)
   *
   * This object will correspond to the derivative respect the x-Axis.
   * For the y-Axis just exchange the 1D filter kernels, or use the
   * lti::gradientKernelY.
   *
   * The template parameter T should be the same as the value_type of
   * the matrix or channel to be filtered (channel::value_type, double, etc.)
   *
   * It is recomended to use this kernels with floating point matrices
   * or channels, due to precision aspects.
   *
   * @ingroup gLinearFilters
   */
  template<class T>
  class gradientKernelX : public sepKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             The valid values are 3, 4 or 5.  If an invalid value is
     *             given, an empty kernel will be created.
     */
    gradientKernelX(const int& size = 3);

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     */
    void generate(const int& size);
  };

  /**
   * Two-dimensional separable filter kernels for the gradient approximation
   *
   * This class will create a two-dimensional gradientian filter kernel.
   *
   * There are three posibilities for the kernel dimensions 3x3, 4x4 or 5x5.
   * The approximation is done using the suggestions of Ando in its paper
   * "Consistent Gradient Operators" (IEEE PAMI, Vol. 22, No. 3, March 2000)
   *
   * These suggested kernels are separable and are of course implemented that
   * way (much faster!)
   *
   * This object will correspond to the derivative respect the y-Axis.
   * For the x-Axis just exchange the 1D filter kernels, or use the
   * lti::gradientKernelX .
   *
   * The template parameter T should be the same as the value_type of
   * the matrix or channel to be filtered (channel::value_type, double, etc.)
   *
   * It is recomended to use this kernels with floating point matrices
   * or channels, due to precision aspects.
   *
   * @ingroup gLinearFilters
   */
  template<class T>
  class gradientKernelY : public sepKernel<T> {
  public:
    /**
     * constructor
     *
     * @param size is the dimension of the one dimensional part
     *             (i.e. the filter kern is a size x size kernel!)  The valid
     *             values are 3, 4 or 5.  If an invalid value is given, an
     *             empty kernel will be created.
     */
    gradientKernelY(const int& size = 3);

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension.
     */
    void generate(const int& size);
  };

  /**
   * Sobel Kernel X
   *
   * Define as a 3x3 separable kernel:
   *
   * \code
   * -1  0  1
   * -2  0  2
   * -1  0  1
   * \endcode
   */
  template<class T>
  class sobelKernelX : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/4.
     */
    sobelKernelX(const bool normalized = false);
  };

  /**
   * Sobel Kernel Y
   *
   * Defined as a 3x3 separable kernel:
   *
   * \code
   * -1 -2 -1
   *  0  0  0
   *  1  2  1
   * \endcode
   */
  template<class T>
  class sobelKernelY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/4.
     */
    sobelKernelY(const bool normalized = false);
  };

  /**
   * Prewitt Kernel X
   *
   * Define as a 3x3 separable kernel:
   *
   * \code
   * -1  0  1
   * -1  0  1
   * -1  0  1
   * \endcode
   */
  template<class T>
  class prewittKernelX : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/3.
     */
    prewittKernelX(const bool normalized = false);
  };

  /**
   * Prewitt Kernel Y
   *
   * Defined as a 3x3 separable kernel:
   *
   * \code
   * -1 -1 -1
   *  0  0  0
   *  1  1  1
   * \endcode
   */
  template<class T>
  class prewittKernelY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the gradient axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/3.
     */
    prewittKernelY(const bool normalized = false);
  };

  /**
   * Harris Kernel X
   *
   * This is the kernel used for the famed Harris Corner Detector
   * introduced in:
   *
   * C. Harris and M. Stephens, "A Combined Corner and Edge Detector",
   * in Proc. 4th Alvey Vision Conference, pp. 147-151, 1988
   *
   * Define as a 1x5 separable kernel:
   *
   * \code
   * -2 -1  0  1 2
   * \endcode
   */
  template<class T>
  class harrisKernelX : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other gradient kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelX();
  };

  /**
   * Harris Kernel Y
   *
   * This is the kernel used for the famed Harris Corner Detector
   * introduced in:
   *
   * C. Harris and M. Stephens, "A Combined Corner and Edge Detector",
   * in Proc. 4th Alvey Vision Conference, pp. 147-151, 1988
   *
   * Define as a 5x1 separable kernel:
   *
   * \code
   * [-2 -1  0  1 2] transposed
   * \endcode
   */
  template<class T>
  class harrisKernelY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other gradient kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelY();
  };

  /**
   * Robinson Kernel X
   *
   * Define as a 3x3 2D (non-separable) kernel:
   *
   * \code
   * -1   1  1
   * -1  -2  1
   * -1   1  1
   * \endcode
   */
  template<class T>
  class robinsonKernelX : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelX();
  };

  /**
   * Robinson Kernel Y
   *
   * Defined as a 3x3 2D (non separable) kernel:
   *
   * \code
   * -1 -1 -1
   *  1 -2  1
   *  1  1  1
   * \endcode
   */
  template<class T>
  class robinsonKernelY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelY();
  };

  /**
   * Kirsch Kernel X
   *
   * Defined as a 3x3 2D (non-separable) kernel:
   *
   * \code
   * -5   3  3
   * -5   0  3
   * -5   3  3
   * \endcode
   */
  template<class T>
  class kirschKernelX : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelX();
  };

  /**
   * Kirsch Kernel Y
   *
   * Defined as a 3x3 2D (non-separable) kernel:
   *
   * \code
   * -5 -5 -5
   *  3  0  3
   *  3  3  3
   * \endcode
   */
  template<class T>
  class kirschKernelY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelY();
  };

}

#include "ltiGradientKernels_template.h"

#endif

