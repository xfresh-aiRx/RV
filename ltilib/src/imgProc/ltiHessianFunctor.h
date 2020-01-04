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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiHessianFunctor.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.3.2004
 * revisions ..: $Id: ltiHessianFunctor.h,v 1.5 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HESSIAN_FUNCTOR_H_
#define _LTI_HESSIAN_FUNCTOR_H_



#include "ltiImage.h"
#include "ltiConvolution.h"
#include "ltiTransform.h"

namespace lti {
  /**
   * The hessianFunctor calculates three channels from an input channel L:
   *
   * Lxx, Lxy and Lyy which are the second derivative channels.
   *
   * The make up the Hessian of each pixel of L like this:
   *
   *          / Lxx(i,j)  Lxy(i,j) \
   * H(i,j)= |                      |
   *          \ Lxy(i,j)  Lyy(i,j) /
   *
   * See parameters::eKernelType for available kernels.
   *
   * Corresponding gradient kernels can be found in ltiGradientKernels.h
   */
  class hessianFunctor : public transform {
  public:
    /**
     * The parameters for the class hessianFunctor
     */
    class parameters : public transform::parameters {
    public:

      /**
       * Possible types of kernels for the computation of the second derivative
       */
      enum eKernelType {
        Ando,     /**<
                   * convolution of the appropriate optimal gradient kernels
                   * by Ando (see lti::andoKernelXX)
                   */

        Sobel,    /**
                   * convolution of Sobel kernels leads to 5x5
                   * separable kernels (see lti::sobleKernelXX):
                   * - XX and YY:
                   * /code
                   * [ 1  0 -2  0  1]  [ 1  4  6  4  1]
                   * /endcode
                   * - XY both:
                   * /code
                   * [ 1  2  0 -2 -1]
                   * /endcode
                   */
        
        Prewitt,  /**
                   * convolution of Prewitt kernels leads to 5x5
                   * separable kernels (see lti::prewittKernelXX):
                   * - XX and YY:
                   * /code
                   * [ 1  0 -2  0  1]  [ 1  2  3  2  1]
                   * /endcode
                   * - XY both:
                   * /code
                   * [ 1  1  0 -1 -1]
                   * /endcode
                   */
        
        Harris,   /**
                   * Harris has 1D kernels of length nine for XX and
                   * YY and a separable 5x5 kernel for XY (see
                   * lti::harrisKernelXX):
                   * - XX and YY:
                   * /code
                   * [ 4  4  1 -4 -10 -4  1  4  4]  []
                   * /endcode
                   * - XY both:
                   * /code
                   * [-2 -1  0  1  2]
                   * /endcode
                   */

        Robinson, /**
                   * convolution of Robinson kernels leads to 5x5
                   * NON-separable kernels (see lti::robinsonKernelXX):
                   * - XX and YY: 
                   * /code
                   * +-              -+
                   * |  1  2 -1 -2  1 |
                   * |  2 -2 -8  2  2 |
                   * |  3  0  0  0  3 |
                   * |  2 -2 -8  2  2 |
                   * |  1  2 -1 -2  1 |
                   * +-              -+
                   * /endcode
                   * - XY:        
                   * /code
                   * +-              -+
                   * |  1  2  1  0 -1 |
                   * |  2 -2 -4  0 -2 |
                   * |  1 -4  4  0 -1 |
                   * |  0  0  0  6  0 |
                   * | -1 -2 -1  0 -1 |
                   * +-              -+
                   * /endcode
                   */

        Kirsch,   /**
                   * convolution of Kirsch kernels leads to 5x5
                   * NON-separable kernels (see lti::kirschKernelXX):
                   * - XX and YY:
                   * /code
                   * +-                  -+
                   * |  9  18 -21 -30  25 |
                   * | 18  18 -51 -30  50 |
                   * | 27  36 -72 -60  75 |
                   * | 18  18 -51 -30  50 |
                   * |  9  18 -21 -30  25 |
                   * +-                  -+
                   * /endcode
                   * - XY:
                   * /code
                   * +-                   -+            
                   * |   9  18   3  -6 -15 |
                   * |  18  18 -12  -6 -30 |
                   * |   3 -12  -8   4  -5 |
                   * |  -6  -6   4  34  10 |
                   * | -15 -30  -5  10  25 |
                   * +-                   -+
                   * /endcode
                   */
 
        OGD2,     /**
                   * Oriented Gaussian Derivative Kernels order 2. See
                   * lti::ogd2Kernel for description. Remember to set
                   * kernelSize and kernelVariance appropriately.
                   */

        Hessian,  /**
                   * Kernels developed by Pablo Alvarado. See
                   * lti::hessianKernelXX for details.
                   */

        Classic   /**
                   * Classic kernels for second derivatives. Note that
                   * no actual kernels but a much faster manual
                   * implementation is used. The same is used for the
                   * XY direction of Hessian since the kernel is
                   * equal. Further, the 'same' kernels are obtained
                   * by convolution of lti::robertsKernelX and Y and a
                   * rotation of \f$\pi/4\f$. These are the kernels:
                   * - XX and YY are 1D:
                   * /code
                   * [1 -2  1]
                   * /endcode
                   * - XY is 3x3 separable:
                   * /code
                   * +-        -+
                   * |  1  0 -1 |
                   * |  0  0  0 |
                   * | -1  0  1 |
                   * +-        -+
                   * /endcode
                   */
      };

      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Sets the kernel type used to calculate the second
       * derivatives. Most kernels can be found in
       * ltiSecondDerivativeKernels.h
       */
      eKernelType kernelType;

      /**
       * This parameter sets the size of the kernel for:
       * - the andoKernels (see andoKernelXX). Possible values are 5,
       *   7, and 9.
       * - ogd2Kernel, any value is possible 
       *
       * Default: 5
       */
      int kernelSize;

      /**
       * The variance for kernels that use the Gaussian. Value -1 sets
       * the variance appropriately for the given kernelSize. (see
       * e.g. ogd2Kernels)
       *
       * Default: -1
       */
      float kernelVariance;

    };

    /**
     * Default constructor
     */
    hessianFunctor();

    /**
     * Construct a functor using the given parameters
     */
    hessianFunctor(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    hessianFunctor(const hessianFunctor& other);

    /**
     * Destructor
     */
    virtual ~hessianFunctor();

    /**
     * Returns the name of this type ("hessianFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * Calculated the second derivatives of the given channel which
     * together form the components of the Hessian matrix of each
     * pixel.
     *
     * @param src channel with the source data.
     * @param xx second derivative in xx direction.
     * @param xy second derivative in xy direction.
     * @param yy second derivative in yy direction.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               channel& xx, channel& xy, channel& yy) const;

    /**
     * Calculated the second derivatives of the given channel8 which
     * together form the components of the Hessian matrix of each
     * pixel. \b Note that the channel8 is converted to a channel
     * before calculation.
     *
     * @param src channel8 with the source data.
     * @param xx second derivative in xx direction.
     * @param xy second derivative in xy direction.
     * @param yy second derivative in yy direction.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
               channel& xx, channel& xy, channel& yy) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hessianFunctor& copy(const hessianFunctor& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hessianFunctor& operator=(const hessianFunctor& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Set parameters. Especially, set the correct kernels.
     */
    virtual bool updateParameters();

  protected:

    /**
     * Used for convolution in x or xx direction
     */
    convolution convXX;

    /**
     * Used for convolution in y or yy direction
     */
    convolution convYY;

    /**
     * Used for convolution in xy direction
     */
    convolution convXY;

    /**
     * Fast calculation of the Hessian with classic kernels [1 -2 1]
     * for XX and YY and [1 0 -1; 0 0 0; -1 0 1] for XY.
     */
    bool classicHessian(const channel& src,
                        channel& xx, channel& xy, channel& yy) const;

    /**
     * Fast computation of the second derivative in XY direction using
     * the kernel:
     * \code
     *  1  0 -1
     *  0  0  0
     * -1  0  1
     * \endcode
     * This is used for parameters::Hessian since it is much faster
     * than convolution with a very sparse kernel.
     */
    bool classicXY(const channel& src, channel& xy) const;

  };
}

#endif

