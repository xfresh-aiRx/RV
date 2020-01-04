/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiSecondDerivativeKernels.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 18.03.2004
 * revisions ..: $Id: ltiSecondDerivativeKernels.h,v 1.1 2004/03/24 10:34:20 doerfler Exp $
 */

/**
 * \file ltiSecondDerivativeKernels.h Defines several linear kernels
 * for the computation of the second derivative. Each kernel type has
 * three classes for derivatives xx, xy, and yy. They are derived
 * from kernels defined in ltiGradientKernels.h. Examples are the Ando
 * kernels, and other classical ones like Sobel, Prewitt, etc.
 */ 

#ifndef _LTI_SECONDDERIVATIVEKERNELS_H_
#define _LTI_SECONDDERIVATIVEKERNELS_H_

#include "ltiLinearKernels.h"
#include <limits>

namespace lti {
  /**
   * Two-dimensional separable filter kernels for the second
   * derivative approximation. They are found by convolution of two
   * optimal gradient kernels as introduced by Ando. For more details
   * see lti::gradientKernelX.
   *
   * Kernel sizes 5x5, 7x7 and 9x9 are available.
   *
   * This object will correspond to the second derivative with respect
   * to the x-Axis.  For the y-Axis just exchange the 1D filter
   * kernels, or use the lti::andoKernelYY.
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
  class andoKernelXX : public sepKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             The valid values are 5, 7 or 9.  If an invalid value is
     *             given, an empty kernel will be created.
     */
    andoKernelXX(const int& size = 5);

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     */
    void generate(const int& size);
  };

  /**
   * Two-dimensional separable filter kernels for the second
   * derivative approximation. They are found by convolution of two
   * optimal gradient kernels as introduced by Ando. For more details
   * see lti::gradientKernelX.
   *
   * Kernel sizes 5x5, 7x7 and 9x9 are available.
   *
   * This object will correspond to the second derivative in xy direction.
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
  class andoKernelXY : public sepKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             The valid values are 5, 7 or 9.  If an invalid value is
     *             given, an empty kernel will be created.
     */
    andoKernelXY(const int& size = 5);

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     */
    void generate(const int& size);
  };

  /**
   * Two-dimensional separable filter kernels for the second
   * derivative approximation. They are found by convolution of two
   * optimal gradient kernels as introduced by Ando. For more details
   * see lti::gradientKernelY.
   *
   * Kernel sizes 5x5, 7x7 and 9x9 are available.
   *
   * This object will correspond to the second derivative with respect
   * to the y-Axis.  For the x-Axis just exchange the 1D filter
   * kernels, or use the lti::andoKernelXX.
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
  class andoKernelYY : public sepKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             The valid values are 5, 7 or 9.  If an invalid value is
     *             given, an empty kernel will be created.
     */
    andoKernelYY(const int& size = 5);

    /**
     * initialize this kernel with the specified values
     * @param size size of the kernel in one dimension
     */
    void generate(const int& size);
  };


  /**
   * Sobel Kernel XX
   *
   * Define as a 5x5 separable kernel:
   *
   * \code
   *  1  0  -2  0  1
   *  4  0  -8  0  4
   *  8  0 -12  0  8
   *  4  0  -8  0  4
   *  1  0  -2  0  1
   * \endcode
   */
  template<class T>
  class sobelKernelXX : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the derivative axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/16.
     */
    sobelKernelXX(const bool normalized = false);
  };

  /**
   * Sobel Kernel XY
   *
   * Defined as a 5x5 separable kernel:
   *
   * \code
   *  1  2  0 -2 -1
   *  2  4  0 -4 -2
   *  0  0  0  0  0
   * -2 -4  0  4  2
   * -1 -2  0  2  1
   * \endcode
   */
  template<class T>
  class sobelKernelXY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * This kernel does not need to be normalized.
     */
    sobelKernelXY();
  };

  /**
   * Sobel Kernel YY
   *
   * Define as a 5x5 separable kernel:
   *
   * \code
   *  1  4   8  4  1
   *  0  0   0  0  0
   * -2 -8 -12 -8 -2
   *  0  0   0  0  0
   *  1  4   8  4  1
   * \endcode
   */
  template<class T>
  class sobelKernelYY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the derivative axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/16.
     */
    sobelKernelYY(const bool normalized = false);
  };

  /**
   * Prewitt Kernel XX
   *
   * Define as a 5x5 separable kernel:
   *
   * \code
   * 1  0 -2  0  1
   * 2  0 -4  0  2
   * 3  0 -6  0  3
   * 2  0 -4  0  2
   * 1  0 -2  0  1
   * \endcode
   */
  template<class T>
  class prewittKernelXX : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the derivative axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/9.
     */
    prewittKernelXX(const bool normalized = false);
  };

  /**
   * Prewitt Kernel XY
   *
   * Defined as a 5x5 separable kernel:
   *
   * \code
   *  1  1  0 -1 -1
   *  1  1  0 -1 -1
   *  0  0  0  0  0
   * -1 -1  0  1  1
   * -1 -1  0  1  1
   * \endcode
   */
  template<class T>
  class prewittKernelXY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * Normalization is not necessary for this kernel.
     */
    prewittKernelXY();
  };

  /**
   * Prewitt Kernel YY
   *
   * Define as a 5x5 separable kernel:
   *
   * \code
   *  1  2  3  2  1
   *  0  0  0  0  0
   * -2 -4 -6 -4 -2 
   *  0  0  0  0  0
   *  1  2  3  2  1
   * \endcode
   */
  template<class T>
  class prewittKernelYY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * @param normalized if true, the kernel will be normalized so that
     *        in the low-pass axis (perpendicular to the derivative axis)
     *        the mean value remains the same, i.e. the kernel will be 
     *        multiplied by 1/9.
     */
    prewittKernelYY(const bool normalized = false);
  };

  /**
   * Harris Kernel XX
   *
   * Define as a 1x9 separable kernel:
   *
   * \code
   * 4  4  1 -4 -10 -4  1  4  4
   * \endcode
   */
  template<class T>
  class harrisKernelXX : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other second derivative kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelXX();
  };

  /**
   * Harris Kernel XY
   *
   * Define as a 5x5 separable kernel:
   *
   * \code
   *  4  2  0 -2 -4
   *  2  1  0 -1 -2
   *  0  0  0  0  0
   * -2 -1  0  1  2
   * -4 -2  0  2  4
   * \endcode
   */
  template<class T>
  class harrisKernelXY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other second derivative kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelXY();
  };

  /**
   * Harris Kernel YY
   *
   * Define as a 9x1 separable kernel:
   *
   * \code
   * [4  4  1 -4 -10 -4  1  4  4] transposed
   * \endcode
   */
  template<class T>
  class harrisKernelYY : public sepKernel<T> {
  public:
    /**
     * Constructor
     *
     * There is no normalization option as with other derivative kernels
     * since the harris kernel does not have a low-pass component.
     */
    harrisKernelYY();
  };

  /**
   * Robinson Kernel XX
   *
   * Define as a 5x5 2D (non-separable) kernel:
   *
   * \code
   * 1  2 -1 -2  1
   * 2 -2 -8  2  2
   * 3  0  0  0  3
   * 2 -2 -8  2  2
   * 1  2 -1 -2  1
   * \endcode
   */
  template<class T>
  class robinsonKernelXX : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelXX();
  };

  /**
   * Robinson Kernel XY
   *
   * Defined as a 5x5 2D (non separable) kernel:
   *
   * \code
   *  1  2  1  0 -1
   *  2 -2 -4  0 -2
   *  1 -4  4  0 -1
   *  0  0  0  6  0
   * -1 -2 -1  0 -1
   * \endcode
   */
  template<class T>
  class robinsonKernelXY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelXY();
  };

  /**
   * Robinson Kernel YY
   *
   * Define as a 5x5 2D (non-separable) kernel:
   *
   * \code
   *  1  2  3  2  1
   *  2 -2  0 -2  2
   * -1 -8  0 -8 -1
   * -2  2  0  2 -2
   *  1  2  3  2  1
   * \endcode
   */
  template<class T>
  class robinsonKernelYY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    robinsonKernelYY();
  };

  /**
   * Kirsch Kernel XX
   *
   * Define as a 5x5 2D (non-separable) kernel:
   *
   * \code
   *  9  18 -21 -30  25
   * 18  18 -51 -30  50
   * 27  36 -72 -60  75
   * 18  18 -51 -30  50
   *  9  18 -21 -30  25
   * \endcode
   */
  template<class T>
  class kirschKernelXX : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelXX();
  };

  /**
   * Kirsch Kernel XY
   *
   * Define as a 5x5 2D (non-separable) kernel:
   *
   * \code
   *   9  18   3  -6 -15
   *  18  18 -12  -6 -30
   *   3 -12  -8   4  -5
   *  -6  -6   4  34  10
   * -15 -30  -5  10  25
   * \endcode
   */
  template<class T>
  class kirschKernelXY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelXY();
  };

  /**
   * Kirsch Kernel YY
   *
   * Define as a 5x5 2D (non-separable) kernel:
   *
   * \code
   *   9  18  27  18   9
   *  18  18  36  18  18
   * -21 -51 -72 -51 -21
   *  18  18  36  18  18
   *   9  18  27  18   9
   * \endcode
   */
  template<class T>
  class kirschKernelYY : public kernel2D<T> {
  public:
    /**
     * Constructor
     */
    kirschKernelYY();
  };
}

#include "ltiSecondDerivativeKernels_template.h"

#endif

