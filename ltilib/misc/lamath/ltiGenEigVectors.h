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
 * file .......: ltiGenEigVectors.h
 * authors ....: Xin Gu
 * organization: LTI, RWTH Aachen
 * creation ...: 16.06.99
 * revisions ..: $Id: ltiGenEigVectors.h,v 1.14 2005/02/02 14:17:33 doerfler Exp $
 */

#ifndef _LTI_GENERAL_EIGEN_VECTORS_H_
#define _LTI_GENERAL_EIGEN_VECTORS_H_

#include "ltiLapackInterface.h"

#ifdef HAVE_LAPACK

#include "ltiEigenSystem.h"

namespace lti {
  /**
   * Generic eigenvalues and eigenvectors computations.
   *
   * Use LAPack functions to compute the eigenvalues and eigenvectors of
   * a matrix.
   *
   * @ingroup lapack
   *
   * @see \ref lapack
   */
  template<class T>
  class generalEigenVectors : public eigenSystem<T>, lapackInterface {
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
        : eigenSystem<T>::parameters(),leftVector(false) {
      };

      /**
       * copy constructor
       */
      parameters(const parameters& other) {
        copy(other);
      };

      /**
       * computing the left eigenvectors of the matrix. Default is
       * false, meaning that right eigenvectors are computed.
       */
      bool leftVector;

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "generalEigenVectors::parameters";
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
        leftVector=other.leftVector;

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
    generalEigenVectors();

    /**
     * constructor, sets the parameters
     */
    generalEigenVectors(const parameters& theParams);

    /**
     * constructor, sets the parameters
     */
    generalEigenVectors(const bool left);

    /**
     * destructor
     */
    virtual ~generalEigenVectors();

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * clone this functor
     */
    virtual functor* clone() const;

    /**
     * Computes left or right eigenvalues and eigenvectors of the
     * given matrix. Which kind will be computed depends on the
     * value of getParameters().leftVector.
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param iEigenvalues  elements will contain the imaginary part of
     *                     the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     * @return false if \a theMatrix contains illegal values like NaN. 
     *         All return arguments are cleared. See lti::validator.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector<T>& eigenvalues,
                       vector<T>& iEigenvalues,
                       matrix<T>& eigenvectors) const;

    /**
     * Computes left or right eigenvalues and eigenvectors of the
     * given matrix. Which kind will be computed depends on the
     * value of getParameters().leftVector. WARNING: This apply method
     * ignores imaginary parts of the eigenvalues. So if the eigenvalues
     * are in fact complex, you get wrong results.
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     * @return false if \a theMatrix contains illegal values like NaN. 
     *         All return arguments are cleared. See lti::validator.
     */
    virtual bool apply(const matrix<T>& theMatrix,
                       vector<T>& eigenvalues,
                       matrix<T>& eigenvectors) const;

    /**
     * Computes left eigenvalues and eigenvectors of the
     * given matrix. "Left" means that eigenvalues e and eigenvectors v
     * will obey the following law: v'A = ev' (v' meaning transpose of v)
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param iEigenvalues elements will contain the imaginary part of
     *                     the eigenvalues
     * @param eigenvectors columns will contain the eigenvectors
     *                     corresponding to the eigenvalues
     * @return false if \a theMatrix contains illegal values like NaN. 
     *         All return arguments are cleared. See lti::validator.
     */
    bool leftEigenVectors(const matrix<T>& theMatrix,
                       vector<T>& eigenvalues,
                       vector<T>& iEigenvalues,
                       matrix<T>& eigenvectors) const;

    /**
     * Computes right eigenvalues and eigenvectors of the
     * given matrix. "Right" means that eigenvalues e and eigenvectors v
     * will obey the following law: Av = ev
     * @param theMatrix    matrix whose eigenvectors are to be computed
     * @param eigenvalues  elements will contain the eigenvalues
     * @param iEigenvalues elements will contain the imaginary part of
     *                     the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     * @return false if \a theMatrix contains illegal values like NaN. 
     *         All return arguments are cleared. See lti::validator.
     */
    bool rightEigenVectors(const matrix<T>& theMatrix,
                           vector<T>& eigenvalues,
                           vector<T>& iEigenvalues,
                           matrix<T>& eigenvectors) const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "generalEigenVectors";
    };

  private:
    // lapack routine in template form
    int geev(char *, char *, long *, T *, long *, T*, T*, T*,
             long*,  T*, long*, T*, long*, long*) const;

  };

}

#endif
#endif

