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
 * file .......: ltiTriangularKernels.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiTriangularKernels.h,v 1.3 2003/02/17 07:15:35 alvarado Exp $
 */

#ifndef _LTI_TRIANGULARKERNELS_H_
#define _LTI_TRIANGULARKERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {
  /**
   *  one-dimensional filter kernel
   *
   *  This class will create a one-dimensional triangular filter kernel.
   *
   *  The area under the filter will be normalized to one.
   *
   *  The one dimesional kernel is calculated with the following equation:
   *
   *  \f$ g(n) = 1 - \frac{n}{| \lfloor size/2 \rfloor |} \f$
   *
   *  with
   *
   *  \f$ n = -\lceil size/2 \rceil  ~ \cdots ~ +(\lceil size/2 \rceil -1) \f$
   *
   *  Example:
   *
   *  \code
   *  // the vector to be filtered:
   *  lti::vector<channel::value_type> data;
   *
   *  // ... initialize vector here ...
   *
   *  // triangular filter kernel with 3 elements with n = -1,0,+1
   *  lti::triangularKernel1D<channel::value_type> aKernel(3);
   *
   *  // triangular filter kernel with 4 elements with n = -2,-1,0,+1
   *  lti::triangularKernel1D<channel::value_type> anotherKernel(4);
   *
   *  lti::convolution filter;                        // convolution operator
   *  lti::convolution::parameters param;             // parameters
   *  param.setKernel(aKernel);                       // use the triangular kernel
   *  filter.setParameters(param);                    // set parameters
   *
   *  // filter the vector and leave the result there too
   *
   *  filter.apply(data);
   *  \endcode
   */
  template<class T>
  class triangularKernel1D : public kernel1D<T> {
  public:
    /**
     * constructor
     * @param size size of the kernel in one dimension (default 5)
     */
    triangularKernel1D(const int& size = 5);

    /**
     * initialize this kernel with the specified value
     * @param size size of the kernel in one dimension
     */
    void generate(const int& size);
  };

}

#include "ltiTriangularKernels_template.h"

#endif

