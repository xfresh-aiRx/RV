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
 * file .......: ltiFilledUpsampling.h
 * authors ....: Pablo Alvarado, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 12.5.2000
 * revisions ..: $Id: ltiFilledUpsampling.h,v 1.17 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_FILLEDUPSAMPLING_H_
#define _LTI_FILLEDUPSAMPLING_H_

#include "ltiObject.h"
#include "ltiFilter.h"

namespace lti {
  /**
   * filledUpsampling takes some image or channel and transforms each
   * pixel to a filled square. This could also be obtained with
   * the upsampling functor and an appropriate kernel, but this one
   * is much faster, since it omits all that convolution stuff by default.
   * However, convolution can still be performed by supplying an
   * external kernel (by calling setKernel).
   */
  class filledUpsampling : public filter {
  public:
    /// the parameters for the class upsampling
    class parameters : public filter::parameters {
    public:
      /// default constructor
      parameters();

      /** copy constructor
          @param other the parameters object to be copied
      */
      parameters(const parameters& other);

      /// destructor
      ~parameters();

      /// returns name of this type
      const char* getTypeName() const;

      /** copy the contents of a parameters object
          @param other the parameters object to be copied
          @return a reference to this parameters object
      */
      parameters& copy(const parameters& other);

      /// returns a pointer to a clone of the parameters
      virtual functor::parameters* clone() const;

      /** returns the kernel in use.  If it is not set yet, a dummy kernel
          will be returned
          @return a const reference to the filter kernel.
      */
      const mathObject& getKernel() const;

      /**
       * sets the filter kernel to be used.
       * A copy of the given parameter will be made!
       * This parameter is optional.  If not given the "normal" filled
       * upsampling will be done.  If given, after the filled-upsampling
       * the resulting image will be convolved with the kernel.
       * @param aKernel the filter kernel to be used
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

      /// interpolation filter
      mathObject* kernel;

      /// upsampling factor
      point factor;
    };

    /**
     * default constructor
     */
    filledUpsampling();

    /**
     * constructor to give directly the scaling factor
     */
    filledUpsampling(const point& factor);

    /**
     * constructor to give directly the scaling factor
     */
    filledUpsampling(const int& factor);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    filledUpsampling(const filledUpsampling& other);

    /// destructor
    virtual ~filledUpsampling();

    /// returns the name of this type ("filledUpsampling")
    virtual const char* getTypeName() const;

    /** operates on the given parameter.
        @param srcdest channel8 with the source data.  The result
                       will be left here too.
        @return true if successful, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /** operates on the given parameter.
        @param srcdest channel with the source data.  The result
                       will be left here too.
        @return true if successful, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest image with the source data.  The result
     *                      will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(image& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest dmatrix with the source data.  The result
     *                      will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(dmatrix& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest image with the source data.  The result
     *                      will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(imatrix& srcdest) const;

    /** operates on the given parameter.
        @param srcdest vector<channel8::value_type> with the source data.
                       The result will be left here too.
        @return true if successful, false otherwise.
     */
    bool apply(vector<channel8::value_type>& srcdest) const;

    /** operates on the given parameter.
        @param srcdest vector<channel::value_type> with the source data.
                       The result will be left here too.
        @return true if successful, false otherwise.
     */
    bool apply(vector<channel::value_type>& srcdest) const;

    /** operates on a copy of the given parameters.
        @param src channel8 with the source data.
        @param dest channel8 where the result will be left.
        @return true if successful, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /** operates on a copy of the given parameters.
        @param src channel with the source data.
        @param dest channel where the result will be left.
        @return true if successful, false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const image& src,image& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src dmatrix with the source data.
     * @param dest dmatrix where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const dmatrix& src,dmatrix& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const imatrix& src,imatrix& dest) const;

    /** operates on a copy of the given parameters.
        @param src vector<channel8::value_type> with the source data.
        @param dest vector<channel8::value_type> where the result will be left.
        @return true if successful, false otherwise.
     */
    bool apply(const vector<channel8::value_type>& src,
                     vector<channel8::value_type>& dest) const;

    /** operates on a copy of the given parameters.
        @param src vector<channel::value_type> with the source data.
        @param dest vector<channel::value_type> where the result will be left.
        @return true if successful, false otherwise.
     */
    bool apply(const vector<channel::value_type>& src,
                     vector<channel::value_type>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     *  @return a reference to this functor object
     */
    filledUpsampling& copy(const filledUpsampling& other);

    /**
     * returns a pointer to a clone of this functor.
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
     * this template class does the real upsampling job
     */
    template <class T>
    class genericUpsampler {
    public:
      /**
       * upsampling of a vector
       */
      bool apply(const int& factor,
                 const vector<T>& src,
                 vector<T>& dest);

      /**
       * upsampling of a matrix
       */
      bool apply(const point& factor,
                 const matrix<T>& src,
                 matrix<T>& dest);

    };
  };
}

#endif

