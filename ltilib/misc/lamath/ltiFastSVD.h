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
 * file .......: ltiFastSVD.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.06.02
 * revisions ..: $Id: ltiFastSVD.h,v 1.8 2005/11/14 15:53:03 doerfler Exp $
 */

#ifndef _LTI_FAST_SVD_H_
#define _LTI_FAST_SVD_H_

#include "ltiLapackInterface.h"

#ifdef HAVE_LAPACK

#include "ltiSVD.h"

namespace lti {
  /**
   * A fast LAPACK-based method for computation of the singular value
   * decomposition SVD. Two routines can be chosen via the parameter
   * parameters::useDC. If this is true (default) the divide and
   * conquer method _GESDD is used which is generally faster
   * especially on large data but also requires more workspace.  In
   * case memory allocation fails when using the faster version,
   * choose the simple driver _GESVD by setting parameters::useDC to
   * \a false.
   *
   * The size of the returned matrices depends on the value of
   * parameters::useMinDimensions. The input data matrix is of size
   * MxN. Let minMN=min(M,N). The vector containing the singular
   * values always has the size minMN. If the parameter is true, the
   * matrix with left singular vectors U is of size MxminMN and V
   * (right SV) is of size minMNxN. Otherwise, U is a MxM and V a NxN
   * matrix.
   *
   * \b Note: The functor returns U and V untransposed to be
   * compatible with singularValueDecomp. Both U and V can be
   * transposed by the algorithm -- which might even be faster -- by
   * setting parameters::transposeU or parameters::transposeV.
   *
   * The following are the man pages of the two LAPACK functions used:
   * -------------------------------------------------------------
   *
   * SGESVD computes the singular value decomposition (SVD) of a real
   * M-by-N matrix A, optionally computing the left and/or right
   * singular vectors. The SVD is written
   *
   * A = U * SIGMA * transpose(V)
   *
   * where SIGMA is an M-by-N matrix which is zero except for its
   * min(m,n) diago­ nal elements, U is an M-by-M orthogonal matrix,
   * and V is an N-by-N orthogo­ nal matrix.  The diagonal elements of
   * SIGMA are the singular values of A; they are real and
   * non-negative, and are returned in descending order.  The first
   * min(m,n) columns of U and V are the left and right singular
   * vectors of A.
   *
   * Note that the routine returns VT = V**T, not V.
   *
   * ---------------------------------------------------------------
   *
   * SGESDD computes the singular value decomposition (SVD) of a real
   * M-by-N matrix A, optionally computing the left and right singular
   * vectors.  If sin­ gular vectors are desired, it uses a
   * divide-and-conquer algorithm.
   *
   * The SVD is written
   *
   * A = U * SIGMA * transpose(V)
   *
   * where SIGMA is an M-by-N matrix which is zero except for its
   * min(m,n) diago­ nal elements, U is an M-by-M orthogonal matrix,
   * and V is an N-by-N orthogo­ nal matrix.  The diagonal elements of
   * SIGMA are the singular values of A; they are real and
   * non-negative, and are returned in descending order.  The first
   * min(m,n) columns of U and V are the left and right singular
   * vectors of A.
   *
   * Note that the routine returns VT = V**T, not V.
   *
   * The divide and conquer algorithm makes very mild assumptions
   * about floating point arithmetic. It will work on machines with a
   * guard digit in add/sub­ tract, or on those binary machines
   * without guard digits which subtract like the Cray X-MP, Cray
   * Y-MP, Cray C-90, or Cray-2. It could conceivably fail on
   * hexadecimal or decimal machines without guard digits, but we know
   * of none.
   *
   *
   * @ingroup lapack
   *
   * @see \ref lapack
   */
  template<class T>
  class fastSVD : public singularValueDecomp<T>, lapackInterface {
  public:
    /**
     * FastSVD parameter class. \b Note that the sort parameter
     * inherited from singularValueDecomp<T> is always true for this
     * functor, since sorting is done automatically during computation
     * of singular values and vectors.
     */
    class parameters : public singularValueDecomp<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters()
        : singularValueDecomp<T>::parameters() {
        this->sort = true;
        useDC = true;
        useMinDimensions = true;
      };

      /**
       * copy constructor
       */
      parameters(const parameters& other) {
        copy(other);
      };

      /**
       * If true the divide-and-conquer method for calculating the SVD
       * is used. This is generally faster, especially on large
       * matrices. However, it uses more temporary memory than the
       * simple method. Thus, if the computation fails due to memory
       * problems setting this parameter to false might solve the
       * problem. Default is true.
       */
      bool useDC;

      /**
       * Let the data matrix have M rows and N columns. Then, usually,
       * U will be an MxM orthogonal matrix and V an NxN orthogonal
       * matrix. However, there are only min(M,N) singular values. For
       * most applications it suffices to calculate only min(M,N) left
       * and right singular vectors, which is done when
       * useMinDimensions is true (default). All singular values are
       * calculated when false.
       */
      bool useMinDimensions;

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "fastSVD::parameters";
      };

      /**
       * copy member.
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        singularValueDecomp<T>::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        singularValueDecomp<T>::parameters&
          (singularValueDecomp<T>::parameters::* p_copy)(const singularValueDecomp<T>::parameters&) =
          singularValueDecomp<T>::parameters::copy;
        (this->*p_copy)(other);
#endif

        useDC = other.useDC;
        useMinDimensions = other.useMinDimensions;

        return (*this);
      };

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          lti::read(handler,"useDC",useDC);
          lti::read(handler,"useMinDimensions",useMinDimensions);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && singularValueDecomp<T>::parameters::read(handler,false);
#     else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool&) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"useDC",useDC);
          lti::write(handler,"useMinDimensions",useMinDimensions);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && singularValueDecomp<T>::parameters::write(handler,false);
#       else
        bool
        (functor::parameters::* p_writeMS)(ioHandler&,const bool&) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif

    };

    /**
     * default constructor
     */
    fastSVD();

    /**
     * constructor, sets the parameters
     */
    fastSVD(const parameters& theParams);

    /**
     * destructor
     */
    virtual ~fastSVD();

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * clone this functor
     */
    virtual functor* clone() const;

    /**
     * Computes singular values as well as left and right singular
     * values. The method used for calculating the SVD is set by
     * parameters::useDC. The number of singular values is set by
     * parameters::useMinDimensions. Both the \a leftSV (U) and the \a
     * rightSV (V) can be transposed by setting parameters::transposeU
     * and parameters::transposeV, respectively.
     *
     * @param theMatrix data matrix with M rows and N columns.
     * @param leftSV left singular vectors in M or min(M,N) columns.
     * @param singularValues min(M,N) singular values
     * @param rightSV right singular vectors in N or min(M,N) columns.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       matrix<T>& leftSV,
                       vector<T>& singularValues,
                       matrix<T>& rightSV) const;

    /**
     * On place version that computes singular values as well as left
     * and right singular values. The method used for calculating the
     * SVD is set by parameters::useDC. The number of singular values
     * is set by parameters::useMinDimensions. The \a rightSV (V) can
     * be transposed by setting parameters::transposeV. The parameter
     * parameters::transposeU has no effect.
     *
     * The left singular vectors are returned in \a theMatrix which
     * has M rows and N columns. If M>N only the first N left singular
     * values are returned in \a theMatrix. If M<=N all M left
     * singular values are returned in the first M columns of \a
     * theMatrix.
     *
     * @param theMatrix data matrix with M rows and N columns.
     * @param singularValues min(M,N) singular values
     * @param rightSV right singular vectors in N or min(M,N) columns.
     */
    virtual bool apply(matrix<T>& theMatrix,
                       vector<T>& singularValues,
                       matrix<T>& rightSV) const;



    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "fastSVD";
    };

  private:
    // lapack routine in template form

    int svd(char* jobu, char* jobvt,
            long* m, long* n, T* a, long* lda,
            T* s, T* u, long* ldu, T* vt, long* ldvt,
            T* work, long* lwork,
            long* info) const;

    int sdd(char* jobz, long* m, long* n, T* a, long* lda,
            T* s, T* u, long* ldu, T* vt, long* ldvt,
            T* work, long* lwork, long* iwork,
            long* info) const;
  };

}

#endif
#endif

