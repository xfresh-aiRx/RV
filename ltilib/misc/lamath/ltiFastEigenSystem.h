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
 * file .......: ltiFastEigenSystem.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.04.02
 * revisions ..: $Id: ltiFastEigenSystem.h,v 1.6 2005/11/04 15:34:55 doerfler Exp $
 */

#ifndef _LTI_FAST_EIGEN_SYSTEM_H_
#define _LTI_FAST_EIGEN_SYSTEM_H_

#include "ltiLapackInterface.h"

#ifdef HAVE_LAPACK

#include "ltiEigenSystem.h"

namespace lti {
  /**
   * A fast LAPACK-based method for computation of (some) eigenvector
   * and eigenvalues. Depending on the parameters useRRR (relatively
   * robust representation), either xSYEVR (true) or xSYEVX (false)
   * are used. The latter is default since RRR goes into infinite
   * loops for some data. \b Note however that switching useRRR to
   * true is usually much faster (a simple test yielded 25% speed-up).
   *
   * Part of the man page of xSYEVR:
   *
   * DSYEVR (SSYEVR) computes selected eigenvalues and, optionally,
   * eigenvectors of a real symmetric tridiagonal matrix T.
   * Eigenvalues and eigenvectors can be selected by specifying either
   * a range of values or a range of indices for the desired
   * eigenvalues.
   *
   * Whenever possible, DSYEVR calls DSTEGR to compute the
   * eigenspectrum using Relatively Robust Representations.  DSTEGR
   * computes eigenvalues by the dqds algorithm, while orthogonal
   * eigenvectors are com­ puted from various "good" L D L^T
   * representations (also known as Relatively Robust
   * Representations). Gram-Schmidt orthogonalization is avoided as
   * far as possible.  More specifically, the various steps of the
   * algorithm are as fol­ lows. For the i-th unreduced block of T,
   *
   * (a) Compute T - sigma_i = L_i D_i L_i^T, such that L_i D_i L_i^T
   *     is a relatively robust representation,
   *
   * (b) Compute the eigenvalues, lambda_j, of L_i D_i L_i^T to
   *     high relative accuracy by the dqds algorithm,
   *
   * (c) If there is a cluster of close eigenvalues,
   *     "choose" sigma_i close to the cluster, and go to step (a),
   *
   * (d) Given the approximate eigenvalue lambda_j of
   *     L_i D_i L_i^T, compute the corresponding eigenvector by
   *     forming a rank-revealing twisted factorization.
   *
   * The desired accuracy of the output can be specified by the input
   * parameter ABSTOL.
   *
   * For more details, see "A new O(n^2) algorithm for the symmetric
   * tridiagonal eigenvalue/eigenvector problem", by Inderjit Dhillon,
   * Computer Science Division Technical Report No. UCB//CSD-97-971,
   * UC Berkeley, May 1997.
   *
   *
   * Part of the man page of xSYEVX:
   *
   * SSYEVX computes selected eigenvalues and, optionally, eigenvectors
   * of a real symmetric matrix A.  Eigenvalues and eigenvectors can be
   * selected by specifying either a range of values or a range of indices
   * for the desired eigenvalues.
   *
   * @ingroup lapack
   *
   * @see \ref lapack
   */
  template<class T>
  class fastEigenSystem : public eigenSystem<T>, lapackInterface {
  public:
    /**
     * eigenSystem parameter class
     */
    class parameters : public eigenSystem<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters()
        : eigenSystem<T>::parameters(),dimensions(0), useRRR(false) {
      };

      /**
       * copy constructor
       */
      parameters(const parameters& other) {
        copy(other);
      };

      /**
       * number of dimensions calculated. The default is zero. In this
       * case, all eigenvectors and eigenvalues are calculated.
       */
      int dimensions;

      /**
       * use relatively robust representation (RRR i.e. xSYEVR) or not
       * (i.e. xSYEVX)
       *
       * Default: false
       */
      bool useRRR;

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "fastEigenSystem::parameters";
      };

      /**
       * copy member.
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        eigenSystem<T>::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        eigenSystem<T>::parameters&
          (eigenSystem<T>::parameters::* p_copy)(const eigenSystem<T>::parameters&) =
          eigenSystem<T>::parameters::copy;
        (this->*p_copy)(other);
#endif
        dimensions = other.dimensions;
        useRRR     = other.useRRR; 

        return (*this);
      };

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };
    };

    /**
     * default constructor
     */
    fastEigenSystem();

    /**
     * constructor, sets the parameters
     */
    fastEigenSystem(const parameters& theParams);

    /**
     * constructor, sets the parameters
     */
    fastEigenSystem(const int dimensions);

    /**
     * destructor
     */
    virtual ~fastEigenSystem();

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * clone this functor
     */
    virtual functor* clone() const;

    /**
     * Computes eigenvalues and eigenvectors of the given matrix. The
     * functor can efficiently calculate only a few dimensions of the
     * eigenspace, taking those with the largest eigenvalues. The
     * number of dimensions is set with parameters::dimensions.
     *
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector<T>& eigenvalues,
                       matrix<T>& eigenvectors) const;


    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "fastEigenSystem";
    };

  private:
    // lapack routine in template form

    int evr(char* jobz, char* range, char* uplo,
            long* n, T* a, long* lda,
            T* vl, T* vu,
            long* il, long* iu,
            T* abstol,
            long* m, T* w,
            T* z, long* ldz, long* isuppz,
            T* work, long* lwork,
            long* iwork, long* liwork,
            long* info) const;

    int evx(char* jobz, char* range, char* uplo,
            long* n, T* a, long* lda,
            T* vl, T* vu,
            long* il, long* iu,
            T* abstol,
            long* m, T* w,
            T* z, long* ldz,
            T* work, long* lwork,
            long* iwork, long* ifail,
            long* info) const;

  };

}

#endif
#endif

