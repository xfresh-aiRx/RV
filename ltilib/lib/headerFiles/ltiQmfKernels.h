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
 * file .......: ltiQmfKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.00
 * revisions ..: $Id: ltiQmfKernels.h,v 1.4 2003/02/17 07:15:33 alvarado Exp $
 */

#ifndef _LTI_QMF_KERNELS_H_
#define _LTI_QMF_KERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {

  /**
   * the Haar filter kernel
   *
   * Please note that this is a filter kernel and NOT wavelet
   * coefficients.  If you need the coefficients just "mirror" this
   * kernel (see lti::kernel1D<T>::mirror())
   */
  class haarKernel : public kernel1D<float> {
  public:
    /**
     * default constructor
     */
    haarKernel();
  };

  /**
   * 16 tap Daubechies filter kernel
   *
   * Please note that this is a filter kernel and NOT the wavelet
   * coefficients.  If you need the coefficients just "mirror" this
   * kernel (see lti::kernel1D<T>::mirror())
   */
  class daubechies16Kernel : public kernel1D<float> {
  public:
    /**
     * default constructor
     */
    daubechies16Kernel();
  };

  /**
   * Battle-Lemariè filter kernel
   *
   * Please note that this is a filter kernel, but due to its symmetry
   * it is equivalent to the corresponding wavelet coefficients.
   */
  class battleLemarieKernel : public kernel1D<float> {
  public:
    /**
     * default constructor
     */
    battleLemarieKernel();
  };

  /**
   * 9 tap symmetric filter kernel
   *
   * Please note that this is a filter kernel, but due to its symmetry
   * it is equivalent to the corresponding wavelet coefficients.
   */
  class tap9Symmetric : public kernel1D<float> {
  public:
    /**
     * default constructor
     */
    tap9Symmetric();
  };

}


#endif

