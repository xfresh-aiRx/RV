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
 * file .......: ltiKernelFunctor.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 28.6.2000
 * revisions ..: $Id: ltiKernelFunctor.h,v 1.10 2005/02/02 14:09:59 doerfler Exp $
 */

#ifndef _LTI_KERNEL_FUNCTOR
#define _LTI_KERNEL_FUNCTOR

#include "ltiFunctor.h"
#include "ltiVector.h"

namespace lti {

  /**
   * This class defines a kernel functor. A kernel is sort of an
   * inner product, but needs not be linear. It takes two vectors
   * and returns a scalar. The most simple kernel is the canonical
   * scalar product.
   *
   * @see linearKernel, sigmoidKernel, radialKernel, polynomialKernel
   *
   * For more information on kernel machines see
   * <A HREF="http://www.kernel-machines.org">here</A>.
   */
  template <class T>
  class kernelFunctor: public functor {

  public:
    /**
     * the parameters for the class distanceFunctor
     */
    class parameters : public functor::parameters {
    public:

      /**
       * default constructor
       */
      parameters() : functor::parameters() {
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : functor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "kernelFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
       // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
# else
       // ...so we have to use this workaround.
       // Conditional on that, copy may not be virtual.
       functor::parameters&
         (functor::parameters::* p_copy)
         (const functor::parameters&) =
         functor::parameters::copy;
       (this->*p_copy)(other);
# endif
       return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };
    };

    /**
     * default constructor
     */
    kernelFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    kernelFunctor(const kernelFunctor<T>& other);

    /**
     * destructor
     */
    virtual ~kernelFunctor();

    /**
     * returns the name of this type ("kernelFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the kernel function value of the vectors
     */
    virtual T apply(const vector<T>& a, const vector<T>& b) const=0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kernelFunctor<T>& copy(const kernelFunctor<T>& other);

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

#include "ltiKernelFunctor_template.h"

#endif


