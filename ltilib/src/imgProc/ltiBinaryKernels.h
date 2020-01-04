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
 * file .......: ltiBinaryKernels.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 15.08.00
 * revisions ..: $Id: ltiBinaryKernels.h,v 1.7 2005/11/10 15:56:04 doerfler Exp $
 */

#ifndef _LTI_BINARYKERNELS_H_
#define _LTI_BINARYKERNELS_H_

#include "ltiLinearKernels.h"

namespace lti {

  /**
   * cityBlock filter kernel
   *
   * This binary filter kernel (values only 0 or \p onValue) is used
   * as a structuring element for erosion/dilation operations.
   *
   * The onValue is important if e.g. erosion::parameters::Gray is
   * used. For Binary mode the exact value of onValue is not
   * relevant. Default is typeInfo<T>::suggestedNorm()/255, i.e. 1 for
   * ubyte and 0.003921 for float.
   *
   * The possible sizes are all odd values greater/equal 3.
   *
   * The kernel norm is set to the typeInfo<T>::suggestedNorm() so the
   * result is the expected black and white image.
   *
   * An example of a city block kernel of size seven:
   *
   * \code
   *          - - - 1 - - -
   *          - - 1 1 1 - -
   *          - 1 1 1 1 1 -
   *          1 1 1 1 1 1 1
   *          - 1 1 1 1 1 -
   *          - - 1 1 1 - -
   *          - - - 1 - - -
   * \endcode
   */
  template<class T=float>
  class cityBlockKernel : public kernel2D<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    cityBlockKernel(const int& size = 3,
                    const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));

    /**
     * initialize this kernel with the specified values
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    void generate(const int& size,
                  const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));
  };

  /**
   * chessboard filter kernel or square filter kernel
   *
   * This binary filter kernel (values only 0 or 1) is used as a structuring
   * element for erosion/dilation operations
   *
   * The onValue is important if e.g. erosion::parameters::Gray is
   * used. For Binary mode the exact value of onValue is not
   * relevant. Default is typeInfo<T>::suggestedNorm()/255, i.e. 1 for
   * ubyte and 0.003921 for float.
   *
   * The possible sizes are all odd values greater/equal 3.
   *
   * The kernel norm is set to the typeInfo<T>::suggestedNorm() so the
   * result is the expected black and white image.
   *
   * An example of a chessboard kernel of size seven:
   *
   * \code
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   * \endcode
   */
  template<class T=float>
  class chessBoardKernel : public sepKernel<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    chessBoardKernel(const int& size = 3,
                     const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));

    /**
     * initialize this kernel with the specified values
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    void generate(const int& size,
                  const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));
  };

  /**
   * octagonal filter kernel
   *
   * This binary filter kernel (values only 0 or 1) is used as a structuring
   * element for erosion/dilation operations
   *
   * The onValue is important if e.g. erosion::parameters::Gray is
   * used. For Binary mode the exact value of onValue is not
   * relevant. Default is typeInfo<T>::suggestedNorm()/255, i.e. 1 for
   * ubyte and 0.003921 for float.
   *
   * The possible sizes are all odd values greater/equal 3.
   *
   * The kernel norm is set to the typeInfo<T>::suggestedNorm() so the
   * result is the expected black and white image.
   *
   * An example of an octagonal kernel of size seven:
   *
   * \code
   *          - - 1 1 1 - -
   *          - 1 1 1 1 1 -
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          1 1 1 1 1 1 1
   *          - 1 1 1 1 1 -
   *          - - 1 1 1 - -
   * \endcode
   */
  template<class T=float>
  class octagonalKernel : public kernel2D<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    octagonalKernel(const int& size = 3,
                    const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));

    /**
     * initialize this kernel with the specified values
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    void generate(const int& size,
                  const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));
  };

  /**
   * euclidean filter kernel
   *
   * This binary filter kernel (values only 0 or 1) is used as a structuring
   * element for erosion/dilation operations. It is equivalent to a circle.
   *
   * The onValue is important if e.g. erosion::parameters::Gray is
   * used. For Binary mode the exact value of onValue is not
   * relevant. Default is typeInfo<T>::suggestedNorm()/255, i.e. 1 for
   * ubyte and 0.003921 for float.
   *
   * The possible sizes are all odd values greater/equal 3.
   *
   * The kernel norm is set to the typeInfo<T>::suggestedNorm() so the
   * result is the expected black and white image.
   *
   * An example of an euclidean kernel of size nine:
   *
   * \code
   *          - - - - 1 - - - -
   *          - - 1 1 1 1 1 - -
   *          - 1 1 1 1 1 1 1 -
   *          - 1 1 1 1 1 1 1 -
   *          1 1 1 1 1 1 1 1 1
   *          - 1 1 1 1 1 1 1 -
   *          - 1 1 1 1 1 1 1 -
   *          - - 1 1 1 1 1 - -
   *          - - - - 1 - - - -
   * \endcode
   */
  template<class T=float>
  class euclideanKernel : public kernel2D<T> {
  public:
    /**
     * constructor
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    euclideanKernel(const int& size = 3,
                    const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));

    /**
     * initialize this kernel with the specified values
     * @param size is the dimension of the one dimensional part (i.e. the
     *             filter kern is a size x size kernel!)
     *             This must be an odd value greater/equal 3.  If another
     *             value is given, the next valid odd value will be assumed.
     * @param onValue value different from zero in the kernel.
     */
    void generate(const int& size,
                  const T& onValue=lti::typeInfo<T>::suggestedNorm()/T(255));
  };

}

#include "ltiBinaryKernels_template.h"

#endif

