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
 * file .......: ltiOgdKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiOgdKernels.h,v 1.6 2004/03/26 13:58:59 doerfler Exp $
 */

#ifndef _LTI_OGDKERNELS_H_
#define _LTI_OGDKERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {

  /** 
   * First order Oriented Gaussian Derivative
   *
   * OGD kernels are separable (one filter pair per basis kernel)!
   *
   * You can create a "real" 2D kernel with the following code
   *
   * \code
   * lti::ogd1Kernel<float> ogd(5); // a kernel 5x5 with default variance
   *                                 // and angle
   * lti::kernel2D<float>   kern;    // a 2D kernel;
   *
   * kern.outerProduct(ogd.getRowFilter(0),ogd.getColFilter(0));
   *
   * \endcode
   *
   * The first basis filter of the first order OGD is:
   *
   * \f$ b_0(x,y) = -g(x,y) \frac{x}{\sigma^2}\f$
   *
   * and the second:
   *
   * \f$ b_1(x,y) = -g(x,y) \frac{y}{\sigma^2}\f$
   *
   * The used interpolation functions are \f$ \cos\theta\f$ and
   * \f$ \sin\theta\f$.
   *
   * If you need to calculate several channels with different directions,
   * calculate first the basis images and then use lti::ogd1Filter to
   * combine them.  This way is faster as to convolve each time the channel
   * with the separable kernel.
   */
  template<class T>
  class ogd1Kernel : public sepKernel<T> {
  public:
    /** 
     * Default constructor
     *
     * Creates an empty kernel.  To initialize use generate() or
     * generateBasisKernel().
     */
    ogd1Kernel();

    /** 
     * Constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param theVariance variance of the kernel
     * @param theAngle used angle (in radians)
     */
    ogd1Kernel(const int& size,
               const double& theVariance =  1.4426950409,
               const double& theAngle = 0.0);

    /** 
     * Initialize this kernel with the specified values
     * @param size is the dimension of the one dimensional part (i.e. the
     *              kernel has dimensions size x size !)
     * @param theVariance variance of the kernel
     * @param theAngle used angle (in radians)
     */
    void generate(const int& size,
                  const double& theVariance,
                  const double& theAngle);

    /** 
     * Initialize this kernel with the specified values
     * @param whichBasis the steerable filter basis (for ogd1 must be 0 or 1)
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param theVariance variance of the kernel
     */
    void generateBasisKernel(const int& whichBasis,
                             const int& size,
                             const double& theVariance);

  protected:
    double variance;
    double angle;
  };

  /**
   * Second order Oriented Gaussian Derivative
   *
   * OGD kernels are separable! (one filter pair per basis kernel)
   *
   * The first basis filter of the second order OGD is:
   *
   * \f$ b_0(x,y) = g(x,y) \frac{x^2 - \sigma^2}{\sigma^4}\f$
   *
   * and the second:
   *
   * \f$ b_1(x,y) = g(x,y) \frac{y^2 - \sigma^2}{\sigma^4}\f$
   *
   * and the third:
   *
   * \f$ b_2(x,y) = g(x,y) \frac{xy}{\sigma^4}\f$
   *
   * The used interpolation functions are \f$ \cos^2\theta\f$,
   * \f$ \sin^2\theta\f$ and \f$2\cos\theta \sin\theta\f$.
   */
  template<class T>
  class ogd2Kernel : public sepKernel<T> {
  public:
    /** 
     * Default constructor.
     *
     * Creates an empty kernel.  To initialize use generate() or
     * generateBasisKernel().
     */
    ogd2Kernel();

    /**
     * Constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param theVariance variance of the kernel
     * @param theAngle used angle (in radians)
     */
    ogd2Kernel(const int& size,
               const double& theVariance =  1.4426950409,
               const double& theAngle = 0.0);

    /**
     * Initialize this kernel with the specified values
     * @param size is the dimension of the one dimensional part (i.e. the
     *             kernel has dimensions size x size !)
     * @param theVariance variance of the kernel
     * @param theAngle used angle (in radians)
     */
    void generate(const int& size,
                  const double& theVariance,
                  const double& theAngle);

    /*

     * Initialize this kernel with the specified values

     *

     * - 0: angle is    0, corresponds to derivative in XX direction

     * - 1: angle is Pi/2, corresponds to derivative in YY direction

     * - 2: angle is Pi/4, corresponds to derivative in XY direction

     *

     * @param whichBasis the steerable filter basis

     *                   (for ogd2 must be 0, 1 or 2)

     * @param size is the dimension of the one dimensional part (i.e. the

     *             kernel has dimensions size x size !

     * @param theVariance variance of the kernel

     */
    void generateBasisKernel(const int& whichBasis,
                             const int& size,
                             const double& theVariance);

  protected:
    double variance;
    double angle;
  };

}

#include "ltiOgdKernels_template.h"

#endif

