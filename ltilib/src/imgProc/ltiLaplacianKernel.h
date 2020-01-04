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
 * revisions ..: $Id: ltiLaplacianKernel.h,v 1.2 2003/11/24 06:53:40 alvarado Exp $
 */

#ifndef _LTI_LAPLACIAN_KERNELS_H_
#define _LTI_LAPLACIAN_KERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {
  /**
   * Two-dimensional kernels to compute the Laplacian
   *
   * The Laplacian is defined as:
   *
   * \f[\nabla^2f = \frac{\partial^2f}{\partial x^2} + 
   *                \frac{\partial^2f}{\partial y^2}\f]
   * 
   * This class will create a two-dimensional filter kernel that can be 
   * applied with lti::convolution to any channel, in order to compute
   * the Laplacian of that channel.  
   *
   * Please note that only kernels of "float" are provided, since ubyte
   * channels cannot represent negative values.
   *
   * There are three posibilities for the kernel dimensions 3x3, 5x5 or 9x9.
   *
   * The 3x3 is the standard kernel:
   * \code
   *  0    1    0
   *  1   -4    1
   *  0    1    0
   * \endcode
   *
   * There is another 3x3 Laplacian kernel based on a polynomial quadratic
   * least-square-error surface passing through the 3x3 grid (this can
   * generated giving the size "2" (from quadratic) ):
   * \code
   *  0.2   0.1    0.2
   *  0.1  -1.2    0.1
   *  0.2   0.1    0.2
   * \endcode
   *
   * We designed kernel with following approach:
   *
   * - Approximate the 3x3 with a quadratic surface that passes exactly
   *   through the middle point of the grid.
   *   The quadratic function has the form:
   *   q(x,y) = a*x^2 + b*x*y + c*y^2 + d*x + e*y + f
   * - Assume, without loss of generality, that the center point has 
   *   coordinates (0,0). 
   *   Since the surface goes through the middle point we got the term f.
   * - We have now five coefficients to be determined with eight values of
   *   the neighbors.  We can solve this problem with standard SVD methods.
   * - With the SVD we can compute the terms for a, b, c, d and e that
   *   minimize the quadratic error, in terms of a matrix that does not depend
   *   on the values of the grid itself.
   * - Since the Hessian matrix for the surface is composed of the a,c and b/2
   *   terms, and the Hessian matrix represents the second order derivatives
   *   of the surface, we can use this values directly to compute our kernel.
   *
   * The 5x5 and 9x9 kernels are based on the gradient operators
   * defined by Ando in its paper "Consistent Gradient Operators"
   * (IEEE PAMI, Vol. 22, No. 3, March 2000).  With this gradients, the 
   * operators for the second derivatives are computed and added.
   * From the 3x3 gradient kernel is generated the 5x5 Laplacian one, and from
   * the 5x5 kernel the 9x9 Laplacian one.
   *
   * These kernels are not directly separable and therefore implemented as
   * two-dimensional ones.  You can approximate the separability with the
   * separate() methods of the sepKernel<T> class.
   *
   * @ingroup gLinearFilters
   */
  class laplacianKernel : public kernel2D<float> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             The valid values are 2, 3, 5 or 9.  If an invalid value is
     *             given, an empty kernel will be created.
     *             Note that "2" generates the 3x3 "quadratic" kernel
     *             approximation.
     */
    laplacianKernel(const int size = 3);

    /**
     * Initialize this kernel with the specified values.
     *
     * @param size size of the kernel in one dimension. The valid
     *             values are 3, 5 or 9.  If an invalid value is given, an 
     *             empty kernel will be created.
     */
    void generate(const int size);
  };
}
#endif

