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
 * file .......: ltiPCA.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiRegularizedPCA.h,v 1.7 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_REG_PCA_H_
#define _LTI_REG_PCA_H_

#include "ltiPCA.h"

namespace lti {
  /**
   * Almost a functor for computing a principal component analysis.
   *
   * It receives a set of input vectors in form of a matrix (each row of
   * the matrix corresponds to an input vector), which will be transformed
   * with PCA.
   *
   * The first time you use the apply()-method, the transformation
   * matrix will be computed.  You can use this transformation matrix
   * with other data sets using the transform() methods.
   *
   * Please note that the eigenvector matrices will contain the
   * eigenvector in the COLUMNS and not in the rows, as could be
   * expected.  This avoids the requirement of transposing matrices in
   * the vector transformations (see also lti::eigenSystem<T>).
   *
   * The only difference to PCA is that this receives
   * an additional parameter kappa which is used in dimensionality
   * reductions. If kappa is greater than zero, we keep the dimensionality
   * of the original space and set the insignificant eigenvalues to the
   * value of the smallest significant one times kappa.
   */
  template <class T>
  class regularizedPCA : public principalComponents<T> {
  public:
    /**
     * the parameters for the class regularizedPCA
     */
    class parameters : public principalComponents<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters()
        : principalComponents<T>::parameters() {
        kappa=T(1);
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) {
        copy(other);
      }


      /**
       * destructor
       */
      virtual ~parameters() {
      }

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "regularizedPCA::parameters";
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        principalComponents<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        kappa=other.kappa;
        return *this;
      }


      /**
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other) {
        copy(other);
        return *this;
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          b=b && lti::read(handler, "kappa", kappa);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && principalComponents<T>::parameters::read(handler,false);
#     else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler, "kappa", kappa);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && principalComponents<T>::parameters::write(handler,false);
#       else
        bool
        (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif

      /**
       * The kappa parameter. Kappa controls the factor by which the
       * last significant eigenvalue is scaled to yield a replacement
       * for the insignificant eigenvalues.
       */
      T kappa;

    };

    /**
     * default constructor
     */
    regularizedPCA();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regularizedPCA(const regularizedPCA& other);

    /**
     * destructor
     */
    virtual ~regularizedPCA();

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

  protected:
    /**
     * Determines the intrinsic dimensionality of the data set if the
     * user specify autoDim, otherwise return parameters::resultDim.
     * The member usedDimensionality will be set with the returned value
     */
    int checkDim();

  };

  template <class T>
  bool read(ioHandler& handler,
            regularizedPCA<T>& pca,
            const bool complete=true) {
    return pca.read(handler,complete);
  }


  template <class T>
  bool write(ioHandler& handler,
             const regularizedPCA<T>& pca,
             const bool complete=true) {
    return pca.write(handler,complete);
  }

}

#include "ltiRegularizedPCA_template.h"

#endif

