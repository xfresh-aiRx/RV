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
 * file .......: ltiGenericInterpolator.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiGenericInterpolator.h,v 1.5 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GENERIC_INTERPOLATOR_H_
#define _LTI_GENERIC_INTERPOLATOR_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiScalarValuedInterpolation.h"

namespace lti {

  /**
   * This functor use a generic interpolation concept based on look-up-tables 
   * for the interpolation kernels.
   *
   * The LUT allow to considerably accelerate the computation times, but a
   * little bit precision is lost due to the unavoidable quantization present
   * in the LUT.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class genericInterpolator : public scalarValuedInterpolation<T> {
  public:
    /**
     * The parameters for the class genericInterpolator
     */
    class parameters : public scalarValuedInterpolation<T>::parameters {
    public:
      /**
       * Types for interpolation kernels.
       */
      enum eInterpolationKernelType {
        BilinearKernel, /**< Bilinear interpolation kernel.
                         * The neighborhood size is always 2x2.
                         * This implementation
                         * is slower than the optimized version in the functor
                         * lti::bilinearInterpolation, but is provided for
                         * completeness.
                         */
        BicubicKernel,  /**< Bicubic interpolation.
                         * The neighborhood size is always 4x4.
                         * The kernel provided here is after Sonka et.al. 
                         * pp. 67
                         *
                         * \f[ h(x)=\begin{cases}
                         * 1-2|x|^2+|x|^3 & \text{for $0\leq|x|\leq 1$} \\
                         * 4-8|x|+5|x|^2-|x|^3 & \text{for $1\leq|x|\leq 2$}\\
                         * 0 & \text{otherwise}
                         * \end{cases} \f]
                         */
        GenericKernel   /**< Generic interpolation.
                         * The generic interpolation uses the kernelLUT given
                         * in the parameters.
                         */
      };

      /**
       * default constructor
       */
      parameters() : scalarValuedInterpolation<T>::parameters() {
        kernelType = eInterpolationKernelType(BicubicKernel);
        numSamplesPerInterval = int(128);
        this->boundaryType = Constant;
        kernelLUT.clear();
      };

      /**
       * Constructor to set explicitelly the desired boundaryType
       */
      parameters(const eBoundaryType btype) 
        : scalarValuedInterpolation<T>::parameters(btype) {
        kernelType = eInterpolationKernelType(BicubicKernel);
        numSamplesPerInterval = int(256);
        this->boundaryType = Constant;
        kernelLUT.clear();
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : scalarValuedInterpolation<T>::parameters() {
        copy(other);
      }
      
      /**
       * destructor
       */
      ~parameters() {
        kernelLUT.clear();
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "genericInterpolator::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        scalarValuedInterpolation<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        scalarValuedInterpolation<T>::parameters&
          (scalarValuedInterpolation<T>::parameters::* p_copy)
          (const scalarValuedInterpolation<T>::parameters&) =
          scalarValuedInterpolation<T>::parameters::copy;
        (this->*p_copy)(other);
#     endif
        
        kernelType = other.kernelType;
        numSamplesPerInterval = other.numSamplesPerInterval;
        kernelLUT.copy(other.kernelLUT);
        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        
        if (b) {
          switch (kernelType) {
            case BilinearKernel:
              lti::write(handler,"kernelType","BilinearKernel");
              break;
            case BicubicKernel:
              lti::write(handler,"kernelType","BicubicKernel");
              break;
            case GenericKernel:
              lti::write(handler,"kernelType","GenericKernel");
              break;
            default:
              lti::write(handler,"kernelType","Unknown");
              b=false;
          }
          
          lti::write(handler,"numSamplesPerInterval",numSamplesPerInterval);
          lti::write(handler,"kernelLUT",kernelLUT);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && scalarValuedInterpolation<T>::parameters::write(handler,
                                                                 false);
#     else
        bool (scalarValuedInterpolation<T>::parameters::*
              p_writeMS)(ioHandler&,
                         const bool) const =
          scalarValuedInterpolation<T>::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        
        if (b) {
          std::string str;
          
          lti::read(handler,"kernelType",str);
          if (str.find("ilinear") != std::string::npos) {
            kernelType = BilinearKernel;
          } else if (str.find("icubic") != std::string::npos) {
            kernelType = BicubicKernel;            
          } else if (str.find("eneric") != std::string::npos) {
            kernelType = GenericKernel;
          } else {
            handler.setStatusString("Unknown interpolator kernel type");
            b=false;
          }

          lti::read(handler,"numSamplesPerInterval",numSamplesPerInterval);
          lti::read(handler,"kernelLUT",kernelLUT);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && scalarValuedInterpolation<T>::parameters::read(handler,false);
#       else
        bool (scalarValuedInterpolation<T>::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          scalarValuedInterpolation<T>::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Interpolation type to be used.
       *
       * Default: Bicubic
       */
      eInterpolationKernelType kernelType;

      /**
       * Number of samples in the interpolation table for a unit interval.
       * 
       * The total number of elements in the LUT will be this value multiplied
       * by the number of unit intervals of the interpolation kernel.
       *
       * Default value: 256
       */
      int numSamplesPerInterval;

      /**
       * Generic interpolation kernel.
       * 
       * If the kernelType is GenericKernel then this kernel LUT will be used.
       * To be a valid kernel, this vector must have a size \a n times
       * numSamplesPerInterval, with \a n even.  If the validity of the
       * kernel LUT cannot be confirmed, a Bilinear Kernel will be used
       * instead, but setParameters will return false.
       *
       * Default value: empty kernel
       */
      fvector kernelLUT;
    };

    /**
     * default constructor
     */
    genericInterpolator();

    /**
     * Construct an interpolator with the given parameters
     */
    genericInterpolator(const parameters& params);

    /**
     * Construct an interpolator with the given boundary type
     */
    genericInterpolator(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    genericInterpolator(const genericInterpolator<T>& other);

    /**
     * destructor
     */
    virtual ~genericInterpolator();

    /**
     * returns the name of this type ("genericInterpolator")
     */
    virtual const char* getTypeName() const;

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    T apply(const vector<T>& src,const float& x) const;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    T apply(const float& x) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    T apply(const matrix<T>& src,
            const float& row,
            const float& col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position p.
     *
     * @param src matrix<T> with the source data.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    T apply(const matrix<T>& src,const tpoint<float>& p) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    T apply(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     *
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    T apply(const tpoint<float>& p) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).  This method is not virtual and can be used
     * if this interpolation type is used as template parameter in time
     * critical situations
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    T interpolate(const matrix<T>& src,
                  const float row,
                  const float col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const float row,
                         const float col) const {
      return interpolate(*this->theMatrix,row,col);
    };

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * This method does not check if the given coordinates and the rest of
     * used points in the src matrix lie within the valid range.  This is
     * left to you.  Please consider that for the generic interpolation
     * not only the point(trunc(col),trunc(row)) is used, but a nxn 
     * neighborhood centered such that (col,row) is nearest to the center.
     *
     * This method is not virtual and can be used in time critical situations,
     * where you prefer to take the boundary control by yourself.
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const matrix<T>& src,
                              const float row,
                              const float col);

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const float row,
                              const float col);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    genericInterpolator& copy(const genericInterpolator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * returns used parameters
     */
    virtual bool updateParameters();

    /**
     * Return a read only reference to the last computed LUT.
     *
     * This method is mainly used for debugging purposes.
     */
    inline const vector<float>& getLUT() const;

    /**
     * Compute the generic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fx fractional term between 0.0f (inclusive) and 1.0f (exclusive)
     * @param data pointer to an array with the support samples.  It has at
     *             least \a n elements, with \a n the number of unit intervals
     *             in the interpolation kernel.
     *             Their position will be assumed to be from 
     *             -(n/2-1) to n/2, so that the
     *             interval 0..1 lies exaclty in the middle.
     *
     * @return interpolated value between the three values
     */
    inline T compute(const float fx,const T* data) const;
    
    /**
     * Compute the generic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between 0.0 and 1.0
     * @param fx fractional term between 0.0 and 1.0
     * @param data pointer to an array of arrays with the support samples.
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float fy,
                     const float fx,
                     const T** data) const;

  protected:
    /**
     * The interpolation kernel is traversed in revers order than
     * the data.  To save the first computation time, we store the
     * sample index for the first element of the last interval
     */
    int lastIntervalFirstSample;

    /**
     * Number of intervals really used.
     *
     * The value given in the parameters might be ignored depending on the
     * chosen kernel type.
     */
    int numberOfIntervals;

    /**
     * This is the number that need to be substracted  to a position point
     * to get the first valid support sample.
     * It is usually numberOfIntervals/2 - 1;
     */
    int firstSampleOffset;
     
    /**
     * Interval size taken from the parameters numSamplesPerInterval;
     */
    int intervalSize;

    /**
     * @name Buffers
     */
    /**
     * For the two dimensional interpolation, this vector will administrate
     * the memory array required.
     */
    vector<T> column;

    /**
     * This pointer will always point to the first element of column
     */
    T* firstElemColumn;

    /**
     * This is a small window that can be used to save temporarily the
     * sample supporting points
     */
    mutable matrix<T> buffer;

    /**
     * This pointer to arrays is initialized to have the proper size, but
     * you have to set the second pointers by yourself
     */
    mutable const T** rows;

    /**
     * This is initialized to point a the buffer's rows.
     */
    const T** bufferRows;

    //@}

    /**
     * @name Kernel initialization routines
     */
    //@{
    /**
     * Interpolation kernel.
     *
     * This is initialized by the initLUT
     */
    vector<float> interpolationKernel;

    /**
     * Initialize the interpolation kernel LUT based on the parameters
     * settings.
     *
     * The LUT has only positive entries, but represents the interval from
     * -halfSize+1 to halfSize.  No use of the symmetrical properties of the
     * kernel are exploited to improve efficiency, since less comparisons are
     * required.
     */
    bool initLUT();

    /**
     * Initialization of the bilinear kernel.
     * Two unit intervals wlll be initialized.
     */
    bool initBilinearLUT();

    /**
     * Initialization of the bicubic kernel.
     * Four unit intervals wlll be initialized.
     * The kernel provided here is after Sonka et.al. page 67:
     * \code
     *           /     
     *          |  1-2|x|^2 + |x|^3           for 0<=x<=1
     *  h(x) = <   4-8|x|^2 + 5|x|^2 - |x|^3  for 1<x<=2
     *          |  0                          otherwise 
     *           \
     * \endcode
     */
    bool initBicubicLUT();

    /**
     * Generic kernel initialization.
     * 
     * This method checks that the kernelLUT in the parameters is a valid
     * kernel and sets all required attributes.
     */
    bool initGenericLUT();

    //@}
  };
}

#include "ltiGenericInterpolator_inline.h"

#endif

