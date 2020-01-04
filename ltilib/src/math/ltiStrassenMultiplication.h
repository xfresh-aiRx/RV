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


/*
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiStrassenMultiplication.h
 * authors ....: Xin Gu
 * organization: LTI, RWTH Aachen
 * creation ...: 17.12.00
 * revisions ..: $Id: ltiStrassenMultiplication.h,v 1.6 2004/05/03 13:30:10 alvarado Exp $
 * description.:
 * The Program implements the Winograd's variant of Strassen'algorithm
 * The matrix multiplication is represented in the divide-and-conquer form
 * There are 15 matrix additions and 7 matrix multiplications.
 */

#ifndef _LTI_STRASSEN_MULTIPLICATAION_H_
#define _LTI_STRASSEN_MULTIPLICATAION_H_

#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   * Implementation for the addtion forms:
   *
   * @ingroup gLinearAlgebra
   */
  template<class T> class strassenMultiplication: public linearAlgebraFunctor {
  public:

    /**
     * default constructor
     */
    strassenMultiplication();
    /**
     * copy constructor
     */
    strassenMultiplication(const strassenMultiplication<T>& other);


    /**
     * destructor
     */
    virtual ~strassenMultiplication();

    /**
     * multiple funktion for the matrixmultiplcation under the
     * strassen's algorithm
     */
    matrix<T>& multiply(const matrix<T>& one,
                       const matrix<T>& other,
                       matrix<T>& result);

    /**
     * Handling of highly rectangular matrices
     */
    bool multiplyHighlyRectangularMatrix(const matrix<T>& one,
                                         const matrix<T>& other,
                                         matrix<T>& product);


    /// onCopy version of apply.
    inline matrix<T>& apply(const matrix<T>& mult1,
                            const matrix<T>& mult2, 
                                  matrix<T>& result) {
      return multiply(mult1,mult2,result);
    }

    /// returns a pointer to a clone of the functor.
    virtual functor* clone() const;
    /// returns the name of this type
    virtual const char* getTypeName() const;

  private:
    /**
     * test funktion for the odd number
     */
    inline bool isodd(const int& num) { return (num%2 != 0); };

  };
}
#include "ltiStrassenMultiplication_template.h"

#endif

