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
 * file .......: ltiMinimizeBasis.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 09.06.99
 * revisions ..: $Id: ltiMinimizeBasis.h,v 1.11 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_MINIMIZE_BASIS_H_
#define _LTI_MINIMIZE_BASIS_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

namespace lti {
  /**
   * minimizeBasis functor.
   *
   * This functor generates a minimum number of basis vectors to approximate
   * a given set of vectors within a given deviation.
   *
   * There is a Fast and an Exact method of computing the minimized
   * basis.  Normally the Exact method should be used, because it does
   * not only compute the exact solution and the smallest basis, but
   * may even be faster than the "Fast" method. The "Fast" method is
   * not exact and may return a basis which is larger than
   * necessary.
   *
   * @ingroup gLinearAlgebra
   */
  template<class T>
  class minimizeBasis : public linearAlgebraFunctor {
  public:
    /// minimizeBasis parameter class
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      enum eMethod {Fast,Exact};
      enum eDeviationType {Element,Vector,Matrix};
      /// default constructor
      parameters() : linearAlgebraFunctor::parameters(),
                                           deviationType(Element),maxDeviation(0),method(Exact) {};
      /// copy constructor
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {copy(other);};
      /** deviation type. Check for
          <ul>
          <li>Element: the elements of the approximating vectors must not
          deviate more than maxDeviation from their approximated
          vectors
          <li>Vector:  the l2VectorNorm of the deviation vector between the
          approximating and the approximated vector must not be
          be larger than maxDeviation
          <li>Matrix:  the l2MatrixNorm of the difference matrix between the
          approximating and the approximated set of vectors must
          not be larger than maxDeviation
          </ul>*/
      eDeviationType deviationType;
      /// maximal deviation
      double maxDeviation;
      /// apply method
      eMethod method;
      /// returns the name of this type
      virtual const char* getTypeName() const {
        return "minimizeBasis::parameters";
      };

      /// copy member.
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearAlgebraFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        deviationType = other.deviationType;
        maxDeviation = other.maxDeviation;
        method = other.method;

        return (*this);
      };
      /// returns a pointer to a clone of the parameters.
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };
    };
    /// default constructor
    minimizeBasis() 
      : linearAlgebraFunctor() {};
    /// constructor, sets the parameters
    minimizeBasis(const parameters& theParams);
    /// destructor
    virtual ~minimizeBasis() {};

    /// returns the current parameters.
    const parameters& getParameters() const;

    /** onCopy version of apply.
     * testVectors' rows should contain the vectors to be approximated.
     * newBasis' columns will contain the new basis vectors.
     * factors' rows will contain the factors to mulitply the new basis
     * vectors by to approximate the testVectors.
     */
    void apply(const matrix<T>& testVectors,
               matrix<T>& newBasis,
               matrix<T>& factors);

    /// returns a pointer to a clone of the functor.
    virtual functor* clone() const {
      return (new minimizeBasis<T>(*this));
    };

    /// returns the name of this type
    virtual const char* getTypeName() const {
      return "minimizeBasis";
    };
  };
}

#include "ltiMinimizeBasis_template.h"

#endif


