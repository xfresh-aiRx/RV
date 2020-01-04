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
 * file .......: ltiQmf.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 14.12.2000
 * revisions ..: $Id: ltiQmf.h,v 1.19 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_QMF_H_
#define _LTI_QMF_H_

#include "ltiFilter.h"
#include "ltiLinearKernels.h"
#include "ltiImage.h"
#include "ltiTree.h"

namespace lti {
  /**
   * This class implements dyadic wavelet-transform as quadrature mirror
   * filters.
   * The original paper which describes this method is:
   * S. G. Mallat, "A theory for multiresolution signal decomposition: The
   * wavelet representation", In: IEEE Transactions on Pattern Analysis and
   * Machine Intelligence, 11(7), 1989, pp. 674-693.
   *
   * It can be used with separable filters only, and that's why only
   * 1D kernels are allowed in the parameters class.  (see
   * lti::haarKernel, lti::tap9Symmetric, lti::daubechies16Kernel,
   * lti::battleLemarieKernel)
   *
   * Due to the extreme high dynamic in the resulting channels, this only
   * makes sense to be applied on lti::channel's
   * The reason for this class to be a lti::filter and not a
   * lti::transform is due to the fact that this %functor is %sort of
   * special %convolution, and it shares the same %parameters that all
   * the other filters, particularly the
   * lti::modifier::parameters::boundaryType is used.
   * For the inverse transformation see lti::qmfInverse
   */
  class qmf : public filter {
  public:
    /**
     * the parameters for the class qmf
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
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
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
       *
       * Note that the expected parameter is a filter kernel and NOT the
       * wavelet coefficients.  Remember that the kernel can be constructed
       * by mirroring the wavelet coefficients.  This kernel is also the
       * low-pass kernel h(k).  The high-pass kernel g(k) will be constructed
       * using the fact that g(k)=(-1)^k h(1-k)
       */
      void setKernel(const mathObject& aKernel);

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
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
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
       * The kernel is the 1D low-pass filter, which must hold several
       * conditions:
       *
       * Let H(w) be the Fourier-Transformation of the kernel:
       *
       * |H(0)| = 1;
       * kernel[n] = O(n^-2) for n->infinity
       * |H(w)|^2 + |H(w+Pi)|^2 = 1;
       * The high-pass filter g(n) can be calculated from the kernel as:
       * g(n) = (-1)^n kernel[1-k]
       *
       * The LTI-Lib has some usual kernel predefined:
       * @see lti::tap9Symmetric, lti::battleLemarieKernel,
       *      lti::daubechies16Kernel, lti::haarKernel
       *
       * The default kernel is the 9-Tap Symetric kernel
       */
      kernel1D<float> kernel;

      /**
       * How many levels must have the result.  This value should be >= 1
       * For 1, the original channel will be returned
       * Default value: 3
       */
      int levels;
    };

    /**
     * default constructor
     */
    qmf();

    /**
     * Constructor with parameters
     */
    qmf(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    qmf(const qmf& other);

    /**
     * destructor
     */
    virtual ~qmf();

    /**
     * returns the name of this type ("qmf")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<float>& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src vector with the source data.
     * @param dest vector where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<float>& src,vector<float>& dest);

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& src,channel& dest);

    /*
     * return the result as a tree, where only the first child at each
     * level can have more children.  The second child results from the
     * low-pass at Y and high-pass at X.  The third child results from the
     * high-pass at X and Y.  The fourth child is created with the low-pass at
     * X and high-pass at Y.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    //bool apply(const channel& src,tree<channel>& dest) const;


    /**
     * generate the appropriate high-pass kernel which correponds to the
     * low-pass kernel given as argument.  Please note that a low-pass KERNEL
     * is expected an NOT the corresponding wavelet coefficients.
     * @param src the original kernel, which should be a low-pass kernel.
     * @param hpk the kernel where the resulting high-pass kernel will be
     *            left.
     */
    void generateHighPassKernel(const kernel1D<float>& src,
                                kernel1D<float>& hpk) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qmf& copy(const qmf& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * returns a vector of points with the bottom-left coordinates of
     * each band.  The first element of the vector will correspond to
     * the last valid indices of the original vector/image.  The
     * second one will be the first one divided by two, and so on.  If
     * the last apply was made for vectors, the 'y' component of the
     * points will be zero.
     */
    const vector<point>& getLastPartitioning() const;

  protected:

    vector<point> lastPartitioning;

  private:
    /**
     * convolution algorithms (private classes of lti::qmf)
     * The type T is the type of the elements of the object to be filtered
     * The (optional) type U is the type of the accumulator variable for
     * the filter.
     */
    template <class T,class U>
    class convolve {
    public:
      /**
       * default constructor
       */
      convolve();

      /**
       * default constructor
       */
      convolve(const kernel1D<T>& kern);

      /**
       * accumulates src*filter in result
       */
      inline void accumulate(const T& filter,const T& src, U& result);

      /**
       * get the result with the proper typecasting and normalization
       */
      inline T getResult(const U& result);

      /**
       * set normalization
       */
      inline void setNorm(const T& theNorm);

      /**
       * generate the appropriate high-pass kernel which correponds to the
       * low-pass kernel given as argument.  Please note that a low-pass KERNEL
       * is expected an NOT the corresponding wavelet coefficients.
       * @param src the original kernel, which should be a low-pass kernel.
       * @param hpk the kernel where the resulting high-pass kernel will be
       *            left.
       */
      void generateHighPassKernel(const kernel1D<T>& src,
                                  kernel1D<T>& hpk) const;

    protected:
      /**
       * normalization constant.  Taken from filter kernel
       */
      T norm;

      /**
       * the low-pass filter kernel
       */
      kernel1D<T> lpKernel;

      /**
       * the high-pass filter kernel
       */
      kernel1D<T> hpKernel;
    };

    /**
     * convolution algorithm for a vector * kernel1D
     * The type T is the type of the elements of the vector and kernel1D
     * The type U is the type of the accumulator variable for
     * the filter.
     */
    template <class T,class U>
      class convolve1D : public convolve<T,U> {
      public:
      /**
       * default constructor
       */
      convolve1D(const kernel1D<T>& kern);

      /**
       * destructor
       */
      ~convolve1D();

      /**
       * convolves the vector src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src vector to be filtered
       * @param kern filter kernel
       * @param dest destination vector for the result
       */
      void apply(const vector<T>& src,
                 const int& from,
                 const int& to,
                       vector<T>& dest,
                 const qmf::parameters& param);

      /**
       * convolves the columns of matrix src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src matrix to be filtered
       * @param kern filter kernel
       * @param dest destination matrix for the result
       *
       * @param allLines if true, all lines will be filtered, otherwise only
       *                 the lines needed for the downsampling specified in the
       *                 parameters will be filtered
       */
      void applyCol(const matrix<T>& src,
                    const rectangle& area,
                          matrix<T>& dest,
                    const qmf::parameters& param);

      /**
       * convolves the rows of matrix src with the filter kernel and
       * leaves the result in dest.
       *
       * Consistency checks should be done by the apply methods!
       *
       * @param src matrix to be filtered
       * @param kern filter kernel
       * @param dest destination matrix for the result
       */
      void applyRow(const matrix<T>& src,
                    const rectangle& area,
                          matrix<T>& dest,
                    const qmf::parameters& param);

      /**
       * set the initialization flag
       */
      void reinitialize();

      protected:

      /**
       * initialize boundaries flag.  If true, the flags will be initialized
       * otherwise the last calculations will be assumed to be right!
       */
      bool initialize;

      /**
       * size of the block being filtered
       */
      int blockSize;

      /**
       * position of the first element belonging to the high-pass band
       */
      int boundary;

      /**
       * @name indices
       * indices for the middle data block, where the filter kernel can be
       * applied without boundary problems
       */
      //@{
      /**
       * begin of low-pass middle block
       */
      int lbegin;

      /**
       * begin of high-pass middle block
       */
      int hbegin;

      /**
       * flag to indicate that the low-pass border was fix to fit downsampling
       * factor
       */
      bool lpfixed;

      /**
       * flag to indicate that the high-pass border was fix to fit downsampling
       * factor
       */
      bool hpfixed;

      /**
       * end of low-pass middle block
       */
      int lend;

      /**
       * end of high-pass middle block
       */
      int hend;

      /**
       * begin of middle block
       */
      int begin;

      /**
       * end of middle block
       */
      int end;
      //@}

    };


  };
}

#endif

