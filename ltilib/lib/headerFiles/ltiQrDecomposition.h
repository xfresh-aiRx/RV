/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiQrDecomposition.h
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 26.1.2004
 * revisions ..: $Id: ltiQrDecomposition.h,v 1.9 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_QR_DECOMPOSITION_H_
#define _LTI_QR_DECOMPOSITION_H_


#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiPerformanceConfig.h"

#ifdef HAVE_LAPACK
#include "ltiLapackInterface.h"
#endif

namespace lti {
  /**
   * This functor computes a QRDecomposition of a given rectangular 
   * m x n Matrix A of the Form:
   *    
   *  A = Q * R
   *
   * Where R is an upper triangular m x m Matrix and Q is a 
   * m x n orthogonal matrix.
   * Transpose of Q muliplied with Q itself is the identity Matrix.
   *
   * If LAPACK is not used or not available, A <b>must</b> be of full rang!
   *
   * \code
   * matrix<float> src(3,3);
   * float data[] =  {1,2,3,4,5,6,7,8,9};
   * src.fill(data);
   * matrix<float> q,r;
   *
   * qrDecomposition<float> qrd;
   * qrd.apply(src,q,r);
   *
   * matrix<float> result;
   * result.multiply(q,r);
   *
   * std::cout << "Q:\n" << q << "\n";
   * std::cout << "R:\n" << r << "\n";
   * // should be identical to src
   * std::cout << "A = Q * R:\n"<< result << "\n";
   *
   * \endcode
   */
  template <class T>
#ifdef HAVE_LAPACK
  class qrDecomposition : public linearAlgebraFunctor, lapackInterface {
#else
  class qrDecomposition : public linearAlgebraFunctor {
#endif
  public:
    /**
     * The parameters for the class qrDecomposition
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * Default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        useLapack = true;
        performanceTweakThresholdForTranspose = 
          _LTI_PERFORMANCE_QR_DECOMPOSITION;
      };

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {
        copy(other);
      };

      /**
       * Destructor
       */
      ~parameters() {
      };

      /**
       * Returns name of this type
       */
      const char* getTypeName() const {
        return "qrDecomposition<T>::parameters";
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearAlgebraFunctor::parameters::copy(other);
# else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        linearAlgebraFunctor::parameters& (linearAlgebraFunctor::parameters::* p_copy)
          (const linearAlgebraFunctor::parameters&) =
          linearAlgebraFunctor::parameters::copy;
        (this->*p_copy)(other);
# endif
        
        useLapack = other.useLapack;
        performanceTweakThresholdForTranspose = 
          other.performanceTweakThresholdForTranspose;
        
        return *this;
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };


      /**
       * Returns a pointer to a clone of the parameters
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
          
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && linearAlgebraFunctor::parameters::write(handler,false);
#     else
        bool (linearAlgebraFunctor::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          linearAlgebraFunctor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif
        b = b && lti::write(handler, "useLapack", useLapack);
        b = b && lti::write(handler, "performanceTweakThresholdForTranspose",
                            performanceTweakThresholdForTranspose);

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
          
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::read(handler,false);
#       else
        bool (linearAlgebraFunctor::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          linearAlgebraFunctor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif
        b = b && lti::read(handler, "useLapack", useLapack);
        b = b && lti::read(handler, "performanceTweakThresholdForTranspose",
                           performanceTweakThresholdForTranspose);

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
       * specifies if the lapack routine is used (if available) 
       * for qr-decomposition.
       * If LAPACK is not used or not available, A <b>must</b> be of full rang!
       *
       * Default: true */
      bool useLapack;

      /**
       * Specifies above which matrix size, the matrix is internally transposed.
       * This parameter takes only effect if useLapack is false or LAPACK is
       * not available.
       * Due to memory access isues, the algorithm works a lot faster 
       * especially for large matrices.
       * Both, collumns and rows are checked against this value. So a value of 50
       * means that the matrix must have at least 51 rows <b>and</b> at least 51 
       * columns to be transposed.
       *
       *
       * Default: _LTI_PERFORMANCE_QR_DECOMPOSITION in ltiPerformanceConfig.h
       */
      int performanceTweakThresholdForTranspose;

    };

    /**
     * Default constructor
     */
    qrDecomposition();

    /**
     * Construct a functor using the given parameters
     */
    qrDecomposition(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    qrDecomposition(const qrDecomposition& other);

    /**
     * Destructor
     */
    virtual ~qrDecomposition();

    /**
     * Returns the name of this type ("qrDecomposition")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!
    

    /**
     * operates on the given %parameter.
     * @param srcdest matrix<T> with the source data.  The resulting Q
     *                 will be left here too.
     * @param r matrix<T> where R will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest, matrix<T>& r) const;


    /**
     * operates on a copy of the given %parameters.
     * @param src matrix<T> with the source data.
     * @param q matrix<T> where Q will be left.
     * @param r matrix<T> where R will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src, matrix<T>& q, matrix<T>& r) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qrDecomposition& copy(const qrDecomposition& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    qrDecomposition& operator=(const qrDecomposition& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  private:

    /* 
     * transpose the given matrix perform a column based algorithm and
     * transpose result again.
     * r must be initiated before invokation
     */
    bool transApply(matrix<T>& srcdest, matrix<T>& r) const;

    
#ifdef HAVE_LAPACK	 
	// LAPACK routines in template form

    /*  SGEQRF computes a QR factorization of a real M-by-N matrix A:
     *  A = Q * R. */
    int geqrf(long* rows, long* cols, T* a, long* lda, T* tau,
               T* work, long* lwork, long* info) const;


    /*  SORGQR generates an M-by-N real matrix Q with orthonormal columns,
     *  which is defined as the first N columns of a product of K elementary
     *  reflectors of order M
     *
     *        Q  =  H(1) H(2) . . . H(k)
     *
     *  as returned by SGEQRF. */
    int orgqr(long* rows, long* cols, long* k,T* a, long* lda,
               T* tau, T* work, long* lwork, long* info) const;
#endif


  };
}

#endif

