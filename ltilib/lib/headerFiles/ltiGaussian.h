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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiGaussian.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 18.3.2002
 * revisions ..: $Id: ltiGaussian.h,v 1.13 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_GAUSSIAN_H_
#define _LTI_GAUSSIAN_H_

#include "ltiMath.h"
#include "ltiMathFunction.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * This function evaluates a multivariate gaussian with the form:
   * \f[ g(\mathbf{x}) = \frac{1}{(2\pi)^{n/2} |\boldsymbol{\Sigma}|^{1/2}}
   * \cdot \exp\left(-\frac{1}{2}(\mathbf{x}-\boldsymbol{\mu})^T
   * \boldsymbol{\Sigma}^{-1}(\mathbf{x}-\boldsymbol{\mu})\right)\f]
   *
   * The template value T is the type of the container elements (double or
   * float).
   */
  template<class T>
  class gaussian : public mathFunction {
  public:
    /**
     * the parameters for the class gaussian
     */
    class parameters : public mathFunction::parameters {
    public:
      /**
       * default constructor
       */
      parameters() 
        : mathFunction::parameters() {
        mean = vector<T>(3,T(0));
        covariance = matrix<T>(3,3,T(0));
        covariance.setIdentity();
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other)
        : mathFunction::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "gaussian::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        mathFunction::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        mathFunction::parameters& (mathFunction::parameters::* p_copy)
          (const mathFunction::parameters&) =
          mathFunction::parameters::copy;
        (this->*p_copy)(other);
#     endif

        mean.copy(other.mean);
        covariance.copy(other.covariance);

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
      virtual bool write(ioHandler& handler,
                         const bool complete) const
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,
                   const bool complete) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {

          lti::write(handler,"mean",mean);
          lti::write(handler,"covariance",covariance);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && mathFunction::parameters::write(handler,false);
#     else
        bool
        (mathFunction::parameters::* p_writeMS)(ioHandler&,
                                                const bool) const =
          mathFunction::parameters::writeMS;
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
      virtual bool write(ioHandler& handler,
                         const bool complete) const {
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
      virtual bool read(ioHandler& handler,
                        const bool complete)
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,
                  const bool complete)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {

          lti::read(handler,"mean",mean);
          lti::read(handler,"covariance",covariance);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && mathFunction::parameters::read(handler,false);
#     else
        bool (mathFunction::parameters::* p_readMS)(ioHandler&,const bool) =
          mathFunction::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

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
      virtual bool read(ioHandler& handler,
                        const bool complete) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      /**
       * Set the mean and covariance
       */
      void setMeanAndCovariance(const vector<T>& mu,
                                const matrix<T>& covar) {
        mean.copy(mu);
        covariance.copy(covar);
      }

      /**
       * For the one dimensional case, you can set the proper mean and
       * variance values using this method, which resizes properly the
       * mean and covariance attributes
       */
      void setMeanAndVariance(const T mu,const T sigma2) {
        mean.resize(1,mu,false,true);
        covariance.resize(1,1,sigma2,false,true);
      }

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Mean vector.  It must have the same dimension than the presented
       * vectors.
       *
       * Default value:  vector<T>(3,T(0))
       */
      vector<T> mean;

      /**
       * Covariance matrix.  Square matrix with the same number of
       * rows (or columns) than the dimension of the mean vector.
       *
       * Default value: 3x3 identity matrix
       */
      matrix<T> covariance;

    };

    /**
     * default constructor
     */
    gaussian();

    /**
     * Construct a 1D gaussian with the given mean and variance
     *
     * Note that the second parameter is the variance and not the standard
     * deviation.
     */
    gaussian(const T mean, const T variance);

    /**
     * construct a n-dimensional gaussian with the given mean vector and
     * covariance matrix.
     */
    gaussian(const vector<T>& mean, const matrix<T>& covar);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    gaussian(const gaussian<T>& other);

    /**
     * destructor
     */
    virtual ~gaussian();

    /**
     * returns the name of this type ("gaussian")
     */
    virtual const char* getTypeName() const;

    /**
     * computes the gaussian for the given vector, and leave the result
     * in the dest parameter.
     * @param src vector<T> with the source data.
     * @param dest function value
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<T>& src,T& dest) const;

    /**
     * alias for apply
     * @param vct vector<T> with the source data.
     * @return function value or negative if error occurred
     */
    T operator()(const vector<T>& vct);

    /**
     * Computes the gaussian for the given scalar, and leave the result
     * in the dest parameter.
     *
     * This is a shortcut for the 1D case.  If the mean and covariance
     * given in the parameters have more the one dimension, the result
     * will not be valid.
     * @param src input scalar
     * @param dest function value
     * @return true if apply successful or false otherwise.
     */
    bool apply(const T src,T& dest) const;

    /**
     * Alias for apply
     * @param x input scalar
     * @return function value
     */
    T operator()(const T x);

    /**
     * Shortcut to set the mean vector and covariance matrix
     */
    void setMeanAndCovariance(const vector<T>& mean,const matrix<T>& covar);

    /**
     * Shortcut to set a one dimensional gaussian with the mean value and
     * the variance.
     */
    void setMeanAndVariance(const T mean,const T variance);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gaussian& copy(const gaussian& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gaussian& operator=(const gaussian& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  protected:
    /**
     * inverse of covariance matrix
     */
    matrix<T> invCovariance;

    /**
     * mean vector
     */
    vector<T> meanVct;

    /**
     * determinant of the covariance matrix
     */
    T norm;

    /**
     * inverse of -2 times the variance for the 1D case
     */
    T invVarianceH;

    /**
     * mean value for the 1D case
     */
    T mean;

  };
}

#endif


