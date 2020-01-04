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
 * file .......: ltiLaplacianKernel.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.2003
 * revisions ..: $Id: ltiHessianKernels.h,v 1.1 2003/11/24 06:47:16 alvarado Exp $
 */

#ifndef _LTI_HESSIAN_KERNELS_H_
#define _LTI_HESSIAN_KERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {
  /**
   * Two-dimensional kernels to compute the Hessian coefficients.
   *
   * The Hessian coefficients at a pixel correspond to the second derivatives.
   * There are for images three cases: 
   * - \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   * - \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   * - \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   *
   * The 3x3 is the kernels to compute this are:
   * - For \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   * \code
   *  0.1  -0.2   0.1
   *  0.3  -0.6   0.3
   *  0.1  -0.2   0.1
   * \endcode
   *
   * - For \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   * \code
   *  0.1   0.3   0.1
   * -0.2  -0.6  -0.2
   *  0.1   0.3   0.1
   * \endcode
   *
   * - For \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   * \code
   *  0.125  0  -0.125
   *  0      0   0
   * -0.125  0   0.125
   * \endcode
   *
   * These kernels are separable and therefore implemented that way.
   *
   * @ingroup gLinearFilters
   * 
   * This kernel implements \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   */
  class hessianKernelXX : public sepKernel<float> {
  public:
    /**
     * Constructor
     */
    hessianKernelXX();

    /**
     * Initialize this kernel.
     */
    void generate();
  };

  /**
   * Two-dimensional kernels to compute the Hessian coefficients.
   *
   * The Hessian coefficients at a pixel correspond to the second derivatives.
   * There are for images three cases: 
   * - \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   * - \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   * - \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   *
   * The 3x3 is the kernels to compute this are:
   * - For \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   * \code
   *  0.1  -0.2   0.1
   *  0.3  -0.6   0.3
   *  0.1  -0.2   0.1
   * \endcode
   *
   * - For \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   * \code
   *  0.1   0.3   0.1
   * -0.2  -0.6  -0.2
   *  0.1   0.3   0.1
   * \endcode
   *
   * - For \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   * \code
   *  0.125  0  -0.125
   *  0      0   0
   * -0.125  0   0.125
   * \endcode
   *
   * These kernels are separable and therefore implemented that way.
   *
   * @ingroup gLinearFilters
   *
   * This kernel implements \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   */
  class hessianKernelYY : public sepKernel<float> {
  public:
    /**
     * Constructor
     */
    hessianKernelYY();

    /**
     * Initialize this kernel.
     */
    void generate();
  };


  /**
   * Two-dimensional kernels to compute the Hessian coefficients.
   *
   * The Hessian coefficients at a pixel correspond to the second derivatives.
   * There are for images three cases: 
   * - \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   * - \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   * - \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   *
   * The 3x3 is the kernels to compute this are:
   * - For \f$\frac{\partial^2 f(x,y)}{\partial x^2}\f$
   * \code
   *  0.1  -0.2   0.1
   *  0.3  -0.6   0.3
   *  0.1  -0.2   0.1
   * \endcode
   *
   * - For \f$\frac{\partial^2 f(x,y)}{\partial y^2}\f$
   * \code
   *  0.1   0.3   0.1
   * -0.2  -0.6  -0.2
   *  0.1   0.3   0.1
   * \endcode
   *
   * - For \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   * \code
   *  0.125  0  -0.125
   *  0      0   0
   * -0.125  0   0.125
   * \endcode
   *
   * These kernels are separable and therefore implemented that way.
   *
   * @ingroup gLinearFilters
   *
   * This kernel implements 
   * \f$\frac{\partial^2 f(x,y)}{\partial x \partial y}\f$
   */
  class hessianKernelXY : public sepKernel<float> {
  public:
    /**
     * Constructor
     */
    hessianKernelXY();

    /**
     * Initialize this kernel.
     */
    void generate();
  };




}
#endif

