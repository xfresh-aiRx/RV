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
 * file .......: ltiPseudoInverseMP.h
 * authors ....: Peter Dörfler
 * organization: LTI, RWTH Aachen
 * creation ...: 6.10.2000
 * revisions ..: $Id: ltiPseudoInverseMP.h,v 1.12 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_PSEUDO_INVERSE_M_P_H_
#define _LTI_PSEUDO_INVERSE_M_P_H_

#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   * This functor calculates the Moore-Penrose Pseudo-Inverse of a
   * matrix.  It is used for solving an over-determined linear
   * equation system according to LMS. The pseudo-inverse of \f$H\f$ is
   * defined as<p>
   * \f$ H^+= (H^t\dot H)^{-1}\dot H^t \f$<p>
   * For some applications smoothing of this solution is desired. This is
   * achieved through a smoothing Matrix \f$\tilde{H}\f$ and a (small)
   * factor \f$\lambda\f$:<p>
   * \f$ H^+= (H^t\dot H + \lambda\tilde{H})^{-1}\dot H^t \f$<p>
   *
   * @ingroup gLinearAlgebra
   */
  template<class T>
  class pseudoInverseMP : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class pseudoInverseMP
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      // The code must be included here due to (once again)
      // another bug of the Visual C++

      /**
       * default constructor
       */
      parameters()
        : linearAlgebraFunctor::parameters(),lambda(0.01) {};

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {copy(other);};

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "pseudoInverseMP<T>::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this ANSI C++...
        linearAlgebraFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
        (const functor::parameters&) =
        functor::parameters::copy;
        (this->*p_copy)(other);
#     endif
        lambda = other.lambda;
        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * the smoothing factor
       */
      double lambda;
    };

    /**
     * default constructor
     */
    pseudoInverseMP();

    /**
     * Creates a new PseudoInverse %functor with a smoothing factor
     * given as the argument
     */
    pseudoInverseMP(const double& lambda);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    pseudoInverseMP(const pseudoInverseMP<T>& other);

    /**
     * destructor
     */
    virtual ~pseudoInverseMP();

    /**
     * returns the name of this type ("pseudoInverseMP")
     */
    virtual const char* getTypeName() const;

    /**
     * Calculates the Moore-Penrose pseudo-inverse <b>without</b> smoothing.
     * @param src matrix<T> to be inverted
     * @param dest Moore-Penrose pseudo-inverse of src
     * @result a reference to <code>dest</code>.
     */
    matrix<T>& apply(const matrix<T>& src,matrix<T>& dest) const;

    matrix<T>& applyS(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Calculates the Moore-Penrose pseudo-inverse <b>with</b> smoothing.
     * Do not forget to set parameters::lambda to the desired value.
     * @param src matrix<T> to be inverted
     * @param smooth matrix<T> used for smoothing
     * @param dest Moore-Penrose pseudo-inverse of src
     * @result a reference to <code>dest</code>.
     */
    matrix<T>& apply(const matrix<T>& src,
                     const matrix<T>& smooth,
                     matrix<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pseudoInverseMP<T>& copy(const pseudoInverseMP<T>& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#include "ltiPseudoInverseMP_template.h"

#endif


