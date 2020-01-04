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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiSymmetricMatrixInversion.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 2.8.2003
 * revisions ..: $Id: ltiSymmetricMatrixInversion.h,v 1.6 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SYMMETRIC_MATRIX_INVERSION_H_
#define _LTI_SYMMETRIC_MATRIX_INVERSION_H_


#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"
#include "ltiCholeskyDecomposition.h"

namespace lti {
  /**
   * Functor for inversion of symmetric, positive definite
   * matrices. The functor uses the choleskyDecomposition for
   * inversion unless the size of the matrix is smaller than or equal
   * to four. In that case the cofactor method is used which can
   * handle matrices that are not positiv definite but requires them
   * to be non-singular.
   *
   * @see lti::matrixInversion
   *
   * @ingroup gLinearAlgebra
   */
  template<class T>
  class symmetricMatrixInversion : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class symmetricMatrixInversion
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        //TODO: Initialize your parameter values!
        // If you add more parameters manually, do not forget to do following:
        // 1. indicate in the default constructor the default values
        // 2. make sure that the copy member also copy your new parameters
        // 3. make sure that the read and write members also read and
        //    write your parameters
        
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : linearAlgebraFunctor::parameters() {
        copy(other);
      }
      
      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "symmetricMatrixInversion::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearAlgebraFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        linearAlgebraFunctor::parameters& (linearAlgebraFunctor::parameters::* p_copy)
                            (const linearAlgebraFunctor::parameters&) =
                            linearAlgebraFunctor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        
        
        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        
        if (b) {
          
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && linearAlgebraFunctor::parameters::write(handler,false);
#     else
        bool (linearAlgebraFunctor::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          linearAlgebraFunctor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        
        if (b) {
          
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::read(handler,false);
#       else
        bool (linearAlgebraFunctor::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          linearAlgebraFunctor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      

    };

    /**
     * default constructor
     */
    symmetricMatrixInversion();

    /**
     * Construct a functor using the given parameters
     */
    symmetricMatrixInversion(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    symmetricMatrixInversion(const symmetricMatrixInversion& other);

    /**
     * destructor
     */
    virtual ~symmetricMatrixInversion();

    /**
     * returns the name of this type ("symmetricMatrixInversion")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!
    
    /**
     * Inverts the symmetric matrix \a srcdest. If the dimension of \a
     * srcdest ist greater than 4 it must be positive definite as
     * well.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Inverts the symmetric matrix \a src and leaves the result in \a
     * dest. If the dimension of \a src ist greater than 4 it must
     * be positive definite as well.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    symmetricMatrixInversion& copy(const symmetricMatrixInversion& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    symmetricMatrixInversion& operator=(const symmetricMatrixInversion& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  protected:

    /**
     * invert the matrix \a m using the cholesky decomposition
     */
    bool choleskyInversion(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * inverts a 2x2 matrix
     */
    bool invert2(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * inverts a 3x3 matrix
     */
    bool invert3(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * inverts a 4x4 matrix
     */
    bool invert4(const matrix<T>& m, matrix<T>& inv) const;

    /**
     * Cholesky Decomposition functor.
     */
    choleskyDecomposition<T> chol;
  };
}

#endif

