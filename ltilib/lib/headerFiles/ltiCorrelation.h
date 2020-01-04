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
 * file .......: ltiCorrelation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiCorrelation.h,v 1.18 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_CORRELATION_H_
#define _LTI_CORRELATION_H_

#include <vector>
#include "ltiFilter.h"
#include "ltiArray.h"
#include "ltiMatrix.h"
#include "ltiTypes.h"
#include "ltiLinearKernels.h"

namespace lti {
  /**
   * Correlation %functor.
   *
   * This functor convolves a %filter kernel (given in the
   * correlation::parameters) with a vector or matrix.
   *
   * @see lti::gaussKernel, lti::gaborKernel
   *
   * Example using a gaussian kernel
   *
   * \code
   * // the channel to be filtered:
   * lti::channel data,result;
   *
   * // ... initialize channel here ...
   *
   * // gauss filter kernel with dimensions 5x5, and a variance of 1.3
   * lti::gaussKernel2D<lti::channel::value_type> kernel(5,1.3);
   *
   * lti::correlation filter;                        // correlation operator
   * lti::correlation::parameters param;             // parameters
   * param.setKernel(kernel);                        // use the gauss kernel
   * filter.setParameters(param);                    // use given parameters
   *
   * // filter the channel and leave the result there too
   * filter.apply(data);
   * \endcode
   *
   * You can also create the functor with a given filter kernel:
   *
   * \code
   * lti::correlation filter(lti::gaussKernel2D<lti::channel::value_type>(5,1.3);
   * filter.apply(data); // parameters already set in the constructor!
   * \endcode
   *
   * The filter kernel can also be change, changing the parameters or with
   * the shortcut setKernel():
   *
   * \code
   * param.setKernel(anotherKernel);
   * filter.setParamters(param);
   *
   * // this is equivalent to:
   *
   * filter.setKernel(anotherKernel);
   *
   * \endcode
   */
  class correlation : public filter {
  public:
    /**
     * parameters of the lti::correlation functor
     */
    class parameters : public filter::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of other parameter object
       * @param other the parameters object to be copied
       * @return a reference to this object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of this parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * returns the kernel in use.  If it is not set yet, an
       * lti::invalidParameters exception will be thrown
       * @return a const reference to the filter kernel.
       */
      const mathObject& getKernel() const;

      /**
       * sets the filter kernel to be used.
       * A copy of the given %parameter will be made!
       * @param aKernel the filter kernel to be used
       */
      void setKernel(const mathObject& aKernel);

      /**
       * Returns the mask in use. If none is set yet, an
       * lti::invalidParametersException will be thrown.
       */
      const channel8& getMask() const;

      /**
       * Sets the maks to be used for the kernel.
       */
      void setMask(const channel8& aMask);

      /**
       * Determine whether the whole (rectangular) kernel should be convolved,
       * or only an arbitrarily shaped object within in, described by the mask.
       * The default value for this option is false, meaning the whole kernel
       * is convolved (just as one would expect).
       */
      void setUseMask(const bool b);

      /**
       * Returns the value set by setUseMask.
       */
      const bool& getUseMask() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /**
       * Specify the correlation type to be used
       */
      enum eMode {
        Classic, /*!< The correlation is defined as f(x)*k(-x), where "*"
                      specifies convolution, f(x) is the image or data where
                      the kernel k(x) must be searched
                 */
        Coefficient, /*!< Calculates the correlation coefficient defined in
                       Gonzales&Woods, 1993, "Digital Image Processing",
                       pp. 584.

                       \f$\gamma(s,t) =
                          \frac{\displaystyle\sum_{x,y}\left[f(x,y)-
                          \overline{f}(x,y)\right]
                          \left[w(x-s,y-t)-\overline{w}\right]}
                          {\left\{\displaystyle\sum_{x,y}\left[f(x,y)-
                          \overline{f}(x,y)\right]^2
                          \sum_{x,y}\left[w(x-s,y-t)-
                          \overline{w}\right]^2\right\}^{1/2}}\f$
                     */
        C1,  /*!< Calculates matching criteria C1 defined in Sonka et.al 2nd
                  Edition, pp. 191:

                  \f$C_1(u,v)=
                  \displaystyle\frac{1}
                  {\displaystyle\max_{(i,j)\in V}
                  \left|{f(i+u,j+v)-h(i,j)}\right|+1}\f$
             */
        C2,  /*!< Calculates matching criteria C2 defined in Sonka et.al 2nd
                  Edition, pp. 191:
                  \f$C_2(u,v)=
                  \displaystyle\frac{1}
                  {\left(\displaystyle\sum_{(i,j)\in V}\left|
                  f(i+u,j+v)-h(i,j)\right|+1\right)}\f$

             */
        C3  /*!< Calculates matching criteria C3 defined in Sonka et.al 2nd
              Edition, pp. 191:

              \f$C_3(u,v)=
              \displaystyle\frac{1}
              {\left(\displaystyle\sum_{(i,j)\in V}
              \left[f(i+u,j+v)-h(i,j)\right]^2+1\right)}\f$
            */
      };

      /**
       * Type of correlation (see parameters::eMode)
       */
      eMode mode;

      /**
       * pointer to the filter kernel copy
       */
      mathObject* kernel;

      /**
       * The mask that defines the object depicted in the kernel
       */
      const channel8* mask;

      /**
       * Convolve the whole kernel, or just the object defined by the mask?
       */
      bool useMask;

      /**
       * get kernel average
       */
      const double& getKernelAverage() const;

      /**
       * get kernel size
       */
      const int& getKernelSize() const;

    protected:
      /**
       * average of the kernel elements.
       * This pseudo-parameters is calculated by set kernel
       */
      double kernelAverage;

      /**
       * the number of elements of the kernel is also needed
       */
      int kernelSize;

    };

    /**
     * default constructor
     */
    correlation();

    /**
     * construct a correlation functor with a parameters set
     * which includes the given filter kernel.
     */
    correlation(const mathObject& aKernel);

    /**
     * copy constructor
     * @param other the other functor to be copied
     */
    correlation(const correlation& other);

    /**
     * destructor
     */
    virtual ~correlation();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<float>& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest dmatrix with the source data.  The result
     *                will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dmatrix& srcdest) const;


    /**
     * operates on the given %parameter.
     * @param srcdest vector<channel8::value_type> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<channel8::value_type>& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest vector<channel::value_type> with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<channel::value_type>& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest dvector with the source data.
     *                The result will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dvector& srcdest) const;


    /**
     * operates on a copy of the given parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<float>& src,matrix<float>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<channel8::value_type> with the source data.
     * @param dest vector<channel8::value_type> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<channel8::value_type>& src,
                     vector<channel8::value_type>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src vector<channel::value_type> with the source data.
     * @param dest vector<channel::value_type> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<channel::value_type>& src,
                     vector<channel::value_type>& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dvector& src,
                     dvector& dest) const;

    /**
     * copy data of "other" functor.
     */
    correlation& copy(const correlation& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut to set the filter kernel in the functor parameters.
     * The other parameters remain unchanged.
     */
    void setKernel(const mathObject& aKernel);

  private:
    /**
     * This is the accumulator class needed by the convolution helper to
     * act as a linear convolution operator for gray valued images.
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class classic {
    public:
      /**
       * Default constructor
       */
      classic();

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * set norm
       */
      inline void setNorm(const T& norm);

    protected:
      /**
       * the accumulated value
       */
      U state;

      /**
       * norm
       */
      T norm;
    };


    /**
     * This is the accumulator class needed by the correlation helper to
     * act as a correlation coefficient operator for gray valued images.
     *
     * This implements the Eq. (9.3-8) on page 584 on Gonzalez & Woods,
     * "Digital Image Processing"
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
      class coefficient {
        public:
        /**
         * Default constructor
         */
        coefficient();

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

        /**
         * Get the state of the accumulator
         */
        inline T getResult() const;

        /**
         * Reset the state of the accumulator
         */
        inline void reset();

        /**
         * set norm
         */
        inline void setNorm(const T& norm);

        /**
         * set averages of data and kernel
         */
        void setData(const T& kernelAverage,const T& kernelSize);

        protected:
        /**
         * the accumulated w-E(w)
         */
        U sumW;

        /**
         * the accumulated (w-E(w))(w-E(w))
         */
        U sumWW;

        /**
         * the accumulated f*(w-E(w))
         */
        U sumFW;

        /**
         * the accumulated f*f
         */
        U sumFF;

        /**
         * the accumulated f
         */
        U sumF;

        /**
         * norm
         */
        T norm;

        /**
         * Average of the data
         */
        T kernSize;

        /**
         * Average of the kernel
         */
        T kernAvg;
      };

    /**
     * This is the accumulator class needed by the convolution helper to
     * calculate the matching criterium C1 described in Sonka et.al.,pp.191
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class criterium1 {
    public:
      /**
       * Default constructor
       */
      criterium1();

      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);

      /**
       * Get the state of the accumulator
       */
      inline T getResult() const;

      /**
       * Reset the state of the accumulator
       */
      inline void reset();

      /**
       * set norm
       */
      inline void setNorm(const T& norm);

    protected:
      /**
       * the accumulated value
       */
      U state;

      /**
       * norm
       */
      T norm;
    };

    /**
     * This is the accumulator class needed by the convolution helper to
     * calculate the matching criterium C2 described in Sonka et.al.,pp.191
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class criterium2 : public criterium1<T,U> {
      public:
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);
	};
    /**
     * This is the accumulator class needed by the convolution helper to
     * calculate the matching criterium C3 described in Sonka et.al.,pp.191
     *
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template<class T,class U=T>
    class criterium3 : public criterium1<T,U> {
    public:
      /**
       * Accumulate the values of filter and src
	   * (srcL and srcR for symetric or asymetric kernel)
       */
      inline void accumulate(const T& filter,const T& src);
      inline void accumulateASym(const T& filter,const T& srcL,const T& srcR);
      inline void accumulateSym(const T& filter,const T& srcL,const T& srcR);

      /**
       * Accumulate the values of T(0) and src
       */
      inline void accumulateZero(const T& src);
    };

  };
  }
#include "ltiCorrelation_template.h"

#endif

