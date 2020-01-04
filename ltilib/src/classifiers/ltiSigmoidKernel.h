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
 * file .......: ltiSigmoidKernel.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 05.11.2001
 * revisions ..: $Id: ltiSigmoidKernel.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_SIGMOID_KERNEL
#define _LTI_SIGMOID_KERNEL

#include "ltiKernelFunctor.h"

namespace lti {

  /**
   * This class defines a "sigmoid" kernel functor.  It is used in
   * Support Vector Machines (lti::svm) to simulate a multilayer
   * perceptron.
   *
   * Here is \f$ K(A,B) = \tanh\left(\kappa (A \cdot B) + \theta
   * \right)\f$.
   * The terms \f$\kappa\f$ (<code>kappa</code>) and \f$\theta\f$
   * (<code>theta</code>) are given through the parameters object, and only
   * some of values satisfy the Mercer's theorem.
   */
  class sigmoidKernel: public kernelFunctor<double> {

  public:
    /**
     * the parameters for the class distanceFunctor
     */
    class parameters : public kernelFunctor<double>::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

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

      /**
       * Slope of sigmoid function.
       */
      double kappa;

      /**
       * Offset of sigmoid function.
       */
      double theta;
    };

    /**
     * default constructor
     */
    sigmoidKernel();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    sigmoidKernel(const sigmoidKernel& other);

    /**
     * returns the name of this type ("sigmoidKernel")
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

