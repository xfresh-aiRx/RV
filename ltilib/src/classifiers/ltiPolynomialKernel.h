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
 * file .......: ltiPolynomialKernel.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 05.11.2001
 * revisions ..: $Id: ltiPolynomialKernel.h,v 1.10 2005/06/24 09:41:00 haehnel Exp $
 */

#ifndef _LTI_POLYNOMIAL_KERNEL
#define _LTI_POLYNOMIAL_KERNEL

#include "ltiKernelFunctor.h"

namespace lti {

  /**
   * This class defines a polynomial kernel functor.  It is defined
   * as \f$ K(A,B) = \left( 1 + A \cdot B \right)^d\f$ with A and B vectors and
   * d an integer given in the parameters.
   */
  class polynomialKernel: public kernelFunctor<double> {

  public:
    /**
     * the parameters for the class kernelFunctor
     */
    class parameters : public kernelFunctor<double>::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * construct a parameter object for a polynomial kernel using the
       * given degree
       */
      parameters(const int degree);

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /** Degree of the polynomial kernel
      */
      int d;
    };

    /**
     * default constructor
     */
    polynomialKernel();

    /**
     * Constructor to create a polynomial kernel of the given degree.
     */
    polynomialKernel(const int degree);


    /**
     * copy constructor
     * @param other the object to be copied
     */
    polynomialKernel(const polynomialKernel& other);

    /**
     * returns the name of this type ("polynomialKernel")
     */
    virtual const char* getTypeName() const;

    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the kernel function value of the vectors
     */
    virtual double apply(const vector<double>& a, const vector<double>& b) const;

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

#endif

