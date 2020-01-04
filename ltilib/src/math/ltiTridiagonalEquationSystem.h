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
 * file .......: ltiTridiagonalEquationSystem.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 15.04.2003
 * revisions ..: $Id: ltiTridiagonalEquationSystem.h,v 1.5 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_TRIDIAGONAL_EQUATION_SYSTEM_H_
#define _LTI_TRIDIAGONAL_EQUATION_SYSTEM_H_

#include "ltiEquationSystem.h"

namespace lti {

  /**
   * Solves a special kind of linear equation system, where only the
   * main diagonal and the first lower and upper sub-diagonals are
   * non-zero. The solution can be found in O(n) time. Let the system
   * be \f$A\cdot x=r\f$ with
   * \f[
   * \begin{bmatrix}
   * b_1 & c_1 & 0 & 0 & \cdots & & & & \\
   * a_2 & b_2 & c_2 & 0 & \cdots & & & & \vdots \\
   * 0 & & & & \cdots & & & & 0 \\
   * \vdots & & & & \cdots & 0 & a_{n-1} & b_{n-1} & c_{n-1}\\
   * & & & & \cdots & 0 & 0 & a_n & b_n
   * \end{bmatrix} \cdot \begin{bmatrix}
   * x_1\\ x_2 \\ \vdots \\ x_{n-1}\\ x_n
   * \end{bmatrix} = \begin{bmatrix}
   * r_1\\ r_2\\ \vdots\\ r_{n-1}\\ r_n \end{bmatrix}
   * \f]
   *
   * The diagonals a, b, and c can be stored in the parameters of this
   * functor or given directly via the appropriate apply method.
   * 
   * \b Note: The algorithm does not perform pivoting and can thus
   * fail. In this case the apply methods return false. Use
   * luSolution or qrSolution instead.
   *
   * \b Note: The first element of the main diagonal b must not be 0!
   */
  template<class T>
  class tridiagonalEquationSystem : public linearEquationSystemSolutionMethod<T> {
  public:
    /**
     * tridiagonalEquationSystem parameter class
     */
    class parameters : public linearEquationSystemSolutionMethod<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters()
        : linearEquationSystemSolutionMethod<T>::parameters() {
        a.clear(); b.clear(); c.clear(); 
      };

     /**
       * copy constructor
       */
      parameters(const parameters& other)
        : linearEquationSystemSolutionMethod<T>::parameters() {
        copy(other);
      };

      /**
       * lower sub-diagonal
       */
      vector<T> a;

      /**
       * main diagonal
       */
      vector<T> b;

      /**
       * upper sub-diagonal
       */
      vector<T> c;
      
      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "tridiagonalEquationSystem::parameters";
      };

      /**
       * copy member.
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearEquationSystemSolutionMethod<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
          (const functor::parameters&) = functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        a.copy(other.a);
        b.copy(other.b);
        c.copy(other.c);

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
    tridiagonalEquationSystem() : linearEquationSystemSolutionMethod<T>() {};

    /**
     * constructor, sets the parameters
     */
    tridiagonalEquationSystem(const parameters& theParams);

    /**
     * constructor, sets the diagonals
     * @param a lower sub-diagonal
     * @param b main diagonal
     * @param c upper sub-diagonal
     */
    tridiagonalEquationSystem(const vector<T> a, const vector<T> b, 
                              const vector<T> c);

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     */
    tridiagonalEquationSystem& copy(const tridiagonalEquationSystem& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new tridiagonalEquationSystem<T>(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "tridiagonalEquationSystem";
    };

    /**
     * onPlace version of apply.
     *
     * Uses the diagonals from the parameters to calculate a
     * solution. \a r also contains the result.
     * @param r right side of the equation as input and solution of the 
     *          equation system as output
     */
    bool apply(vector<T>& r);

    /** onCopy version of apply.
     *
     * Uses the diagonals from the parameters to calculate a
     * solution.
     * @param r right side of the equation
     * @param x solution of the equation system
     */
    bool apply(const vector<T>& r,vector<T>& x);

    /**
     * Uses the given diagonals instead of those in the parameters to
     * calculate the solution.
     * @param a lower sub-diagonal
     * @param b main diagonal
     * @param c upper sub-diagonal
     * @param r right side of the equation
     * @param x solution of the equation system
     */
    bool apply(const vector<T>& a, const vector<T>& b, const vector<T>& c,
               const vector<T>& r, vector<T>& x);

  };

}

#endif
