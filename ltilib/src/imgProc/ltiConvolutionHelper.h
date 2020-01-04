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
 * file .......: ltiConvolutionHelper.h
 * authors ....: Pablo Alvarado, Bodo Hoffmanna
 * organization: LTI, RWTH Aachen
 * creation ...: 19.7.2000
 * revisions ..: $Id: ltiConvolutionHelper.h,v 1.13 2005/10/20 13:23:03 doerfler Exp $
 */

#ifndef _LTI_CONVOLUTION_HELPER_H_
#define _LTI_CONVOLUTION_HELPER_H_

#include "ltiObject.h"
#include "ltiMath.h"
#include "ltiImage.h"
#include "ltiLinearKernels.h"
#include "ltiModifier.h"
#include "ltiMorphology.h"
#include "ltiContour.h"
#include "ltiKernelType.h"

namespace lti {
  /**
   * pre-convolution algorithm for a vector * kernel1D
   * The type T is the type of the elements of the vector and kernel1D
   * The class A determines how the values should be accumulated.
   * The class A must have following  methods:
   *
   *  - accumulate(T kernelElement,T source)
   *    accumulate the result of applying some function to one element
   *    of the kernel and one element of the source channel.
   *  - accumulateSym(T kernelElement,T source1, T source2)
   *    accumulate the result of applying some function to one element of a
   *    symmetric kernel and the corresponding elements of the source
   *    channel.
   *  - accumulateASym(T kernelElement,T source1, T source2)
   *    accumulate the result of applying some function to one element of a
   *    anti-symmetric kernel (e.g. a gradient) and the corresponding elements
   *    of the source channel.
   *  - accumulateZero(T source) 
   *    accumulate the result of applying some function with kernel value
   *    zero to the source channel. For efficient implementation of 
   *    accumulateASym() where middle kernel value is zero by definition.
   *    For filters this is an empty function, however, not for others.
   *  - getResult()
   *    returns the accumulated value
   *  - reset()
   *    reset the accumulated value
   *  - setNorm()
   *    set the normalization constant used by the getResult() method
   *
   * Examples of the accumulator class can be found in:
   * lti::dilation::accumulatorGray, lti::dilation::accumulatorBin,
   * lti::erosion::accumulatorGray, lti::erosion::accumulatorBin,
   * lti::convolution::accumulator
   *
   * The implementation of these methods allows the efficient use
   * of this class for the implementation of convolution, dilation and
   * erosion.
   *
   * Please note that this class is intended to be used just as a helper class.
   * The correctness of the parameters and input data must be ensured by the
   * "helped" classes before these methods are called.
   *
   */
  template <class T,class A>
  class convHelper1D {
  public:
    /**
     * default constructor
     */
    convHelper1D();

    /**
     * destructor
     */
    ~convHelper1D();

    /**
     * try to get a Kernel from the parameters.
     * If it is not possible to get it, return false;
     * This will not copy the kernel, it just will keep a reference to it!
     * @param kern the mathObject with the kernel.
     * @return true if kernel could be set and false otherwise
     */
    bool setKernel(const mathObject* kern);

    /**
     * convHelpers the vector src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src vector to be filtered
     * @param dest destination vector for the result
     * @param param parameters with the boundaryType to be used
     */
    void apply(const vector<T>& src,
                     vector<T>& dest,
               const modifier::parameters& param);

    /**
     * convHelpers the columns of matrix src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param param parameters with the boundaryType to be used
     */
    void applyCol(const matrix<T>& src,
                        matrix<T>& dest,
                  const modifier::parameters& param);

    /**
     * convHelpers the rows of matrix src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest destination matrix for the result
     * @param param parameters with the boundaryType to be used
     */
    void applyRow(const matrix<T>& src,
                        matrix<T>& dest,
                  const modifier::parameters& param);

    /**
     * returns a reference to the accumulator object being used
     */
    A& getAccumulator();

    /**
     * returns a reference to the accumulator object being used
     */
    const A& getAccumulator() const;

  protected:
    const kernel1D<T>* kernel;
    A lresult;
    eKernelType kernelType;

    private:
    kernel1D<T>* tmpKernel;
    

    /**
     * Does the work of apply() if the kernel is larger than the src.
     */
    void applyLargeFilter(const vector<T>& src,
                          vector<T>& dest,
                          const modifier::parameters& param);

    /**
     * Does the work of applyCol() if the kernel is larger than the src.
     */
    void applyLargeFilterCol(const matrix<T>& src,
                             matrix<T>& dest,
                             const modifier::parameters& param);
    
      
  };

  /**
   * pre-convolution algorithm for a matrix * kernel2D
   * The type T is the type of the elements of the vector an kernel1D
   * The class A determines how the values should be accumulated.
   * The class A must have following methods:
   *
   *  - accumulate(T kernelElement,T source)
   *    accumulate the result of applying some function to one element
   *    of the kernel and one element of the source channel.
   *  - accumulateSym(T kernelElement,T source1, T source2)
   *    accumulate the result of applying some function to one element of a
   *    symmetric kernel and the corresponding elements of the source
   *    channel.
   *  - accumulateASym(T kernelElement,T source1, T source2)
   *    accumulate the result of applying some function to one element of a
   *    anti-symmetric kernel (e.g. a gradient) and the corresponding elements
   *    of the source channel.
   *  - accumulateZero(T source) 
   *    accumulate the result of applying some function with kernel value
   *    zero to the source channel. For efficient implementation of 
   *    accumulateASym() where middle kernel value is zero by definition.
   *    For filters this is an empty function, however, not for others.
   *  - getResult()
   *    returns the accumulated value
   *  - reset()
   *    reset the accumulated value
   *  - setNorm()
   *    set the normalization constant used by the getResult() method
   *
   * Examples of the accumulator class can be found in:
   * lti::dilation::accumulatorGray, lti::dilation::accumulatorBin,
   * lti::erosion::accumulatorGray, lti::erosion::accumulatorBin,
   * lti::convolution::accumulator
   *
   * The implementation of these methods allows the efficient use
   * of this class for the implementation of convolution, dilation and
   * erosion.
   *
   * Please note that this class is intended to be used just as a helper class.
   * The correctness of the parameters and input data must be ensured by the
   * "helped" classes before these methods are called.
   *
   */
  template <class T,class A>
    class convHelper2D  {
    public:
    /**
     * default constructor
     */
    convHelper2D();

    /**
     * destructor
     */
    ~convHelper2D();

    /**
     * try to get a Kernel from the parameters.
     * If it is not possible to get it, return false;
     * This will not copy the kernel, it just will keep a reference to it!
     * @param kern the mathObject with the kernel.
     * @return true if kernel could be set and false otherwise
     */
    bool setKernel(const mathObject* kern);

    /**
     * get kernel type
     */
    inline bool isKernelSeparable() const;

    /**
     * convHelpers the matrix src with the filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest result
     * @param param parameters with the boundaryType to be used
     */
    void apply(const matrix<T>& src,
               matrix<T>& dest,
               const modifier::parameters& param);

    /**
     * convHelpers the matrix src with the separable filter kernel and
     * leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest result
     * @param param parameters with the boundaryType to be used
     */
    void applySep(const matrix<T>& src,
                  matrix<T>& dest,
                  const modifier::parameters& param);
    /**
     * convolves the matrix src with the object depicted in the filter
     * kernel and described by the mask. Leaves the result in dest.
     *
     * Consistency checks should be done by the apply methods!
     *
     * @param src matrix to be filtered
     * @param dest result
     * @param param parameters that determine e.g. border behaviour
     */
    void applyMask(const matrix<T>& src,
                    matrix<T>& dest,
                    const modifier::parameters& param);

    /**
     * Set the mask to be used with the kernel.
     */
    void setMask(const channel8& mask);

    /**
     * Returns a pointer to the current mask.
     */
    const channel8* getMask() const;

    /**
     * returns a reference to the accumulator object being used
     */
    A& getAccumulator();

    /**
     * returns a reference to the accumulator object being used
     */
    const A& getAccumulator() const;

  protected:
    const kernel2D<T>* kernel;
    const sepKernel<T>* sKernel;
    const channel8* mask;

    A lresult;
  private:
    kernel2D<T>* tmpKernel;
    sepKernel<T>* tmpSKernel;
  };
}

#include "ltiConvolutionHelper_template.h"

#endif

