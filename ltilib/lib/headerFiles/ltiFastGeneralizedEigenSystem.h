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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiFastGeneralizedEigenSystem.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 08.05.2003
 * revisions ..: $Id: ltiFastGeneralizedEigenSystem.h,v 1.5 2005/02/02 14:17:33 doerfler Exp $
 */

#ifndef _LTI_FAST_GENERALIZED_EIGEN_SYSTEM_H_
#define _LTI_FAST_GENERALIZED_EIGEN_SYSTEM_H_

#include "ltiLapackInterface.h"

#ifdef HAVE_LAPACK

#include "ltiGeneralizedEigenSystem.h"

namespace lti {
  /**
   *  Computes all the eigenvalues, and optionally, the eigenvectors
   *  of a real generalized symmetric-definite eigenproblem, of the form
   *  A*x=(lambda)*B*x,  A*Bx=(lambda)*x,  or B*A*x=(lambda)*x.
   *  Here A and B are assumed to be symmetric and B is also
   *  positive definite.
   *
   * The type of problem as stated above can be selected via
   * parameters::problemType. When all eigenvalues/eigenvectors are
   * needed you can select the divide and conquer algorithm by setting
   * parameters::useDC to true (_SYGVD instead of _SYGV). If only the
   * n most significant eigenvalues/eigenvectors are needed set
   * parameters::dimensions to n. The LAPACK routine _SYGVX is used.
   *
   * Divide and conquer is usually faster especially for large
   * matrices but also uses more workspace.
   *
   * @ingroup lapack
   *
   * @see \ref lapack
   */
  template<class T>
  class fastGeneralizedEigenSystem : public generalizedEigenSystem<T>, lapackInterface {
  public:
    /**
     * parameter class for fastGeneralizedEigenSystem
     */
    class parameters : public generalizedEigenSystem<T>::parameters {
    public:

      /**
       * Defines different problem types for the generalized eigen system.
       */
      enum eProblemType {
        Type1 = 1, /*!< A*x = lambda*B*x */
        Type2, /*!<  A*B*x = lambda*x */
        Type3 /*!< B*A*x = lambda*x */
      };

      /**
       * default constructor
       */
      parameters()
        : generalizedEigenSystem<T>::parameters(),useDC(true) {
        problemType=Type1;
      };

      /**
       * copy constructor
       */
      parameters(const parameters& other) {
        copy(other);
      };

      /**
       * the problem type to be solved. Default is Type1.
       */
      eProblemType problemType;

      /**
       * If true the divide-and-conquer method for calculating the SVD
       * is used. This is generally faster, especially on large
       * matrices. However, it uses more temporary memory than the
       * simple method. Thus, if the computation fails due to memory
       * problems setting this parameter to false might solve the
       * problem. Default is true.
       *
       * \b Note: If dimensions is unequal to zero, ie not all
       * eigenvalues/vectors are calculated this option is not
       * available and internally set to false.
       */
      bool useDC;

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "fastGeneralizedEigenSystem::parameters";
      };

      /**
       * copy member.
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        generalizedEigenSystem<T>::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        generalizedEigenSystem<T>::parameters&
          (generalizedEigenSystem<T>::parameters::* p_copy)(const generalizedEigenSystem<T>::parameters&) =
          generalizedEigenSystem<T>::parameters::copy;
        (this->*p_copy)(other);
#endif
        useDC=other.useDC;
        problemType=other.problemType;
        

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
    fastGeneralizedEigenSystem();

    /**
     * constructor, sets the parameters
     */
    fastGeneralizedEigenSystem(const parameters& theParams);

    /**
     * constructor, sets the parameters
     */
    fastGeneralizedEigenSystem(const int dimensions);

    /**
     * destructor
     */
    virtual ~fastGeneralizedEigenSystem();

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
     * @param a the symmetric matrix A
     * @param b the symmetric, positive definite matrix B
     * @param eigenvalues  elements will contain the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     */
    virtual bool apply(const matrix<T>& a,
                       const matrix<T>& b,
                       vector<T>& eigenvalues,
                       matrix<T>& eigenvectors) const;

    /**
     * Computes eigenvalues and eigenvectors of the given matrix. The
     * functor can efficiently calculate only a few dimensions of the
     * eigenspace, taking those with the largest eigenvalues. The
     * number of dimensions is set with parameters::dimensions.
     *
     * @param a the symmetric matrix A
     * @param b the symmetric, positive definite matrix B
     * @param eigenvalues  elements will contain the eigenvalues
     */
    virtual bool apply(const matrix<T>& a,
                       const matrix<T>& b,
                       vector<T>& eigenvalues) const;


    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "fastGeneralizedEigenSystem";
    };

  private:
    // lapack routines in template form
    
    int sygv(long* itype, char* jobz, char* uplo, 
             long* n, T* a, long* lda, T* b, long* ldb, T* w,
             T* work, long* lwork,
             long* info) const;

    int sygvd(long* itype, char* jobz, char* uplo, 
              long* n, T* a, long* lda, T* b, long* ldb, T* w, 
              T* work, long* lwork, long* iwork, long *liwork,
              long* info) const;

    int sygvx(long* itype, char* jobz, char* range, char* uplo, 
              long* n, T* a, long* lda, T* b, long* ldb, 
              T* vl, T* vu, long* il, long* iu, T* abstol,
              long* m, T* w, T* z, long* ldz, 
              T* work, long* lwork, long* iwork, 
              long *ifail, long* info) const;
    
  };

}

#endif
#endif

