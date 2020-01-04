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
 * file .......: ltiCholeskyDecomposition.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 7.7.2003
 * revisions ..: $Id: ltiCholeskyDecomposition.h,v 1.7 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_CHOLESKY_DECOMPOSITION_H_
#define _LTI_CHOLESKY_DECOMPOSITION_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiTriangularMatrixType.h"

namespace lti {
  /**
   * This functor calculates the Cholesky decomposition of a
   * symmetric, positive-definite matrix A. The Cholesky decomposition
   * is defined as A = L * L' (with L' the transpose of L) or A = U' *
   * U. The cholesky factor L or U are lower or upper triangular
   * matrices, respectively, where L=U'. The nature of the Cholesky
   * factor can be determined by setting parameters::triangularType or
   * using the appropriate apply method. The default is Upper due to
   * shorter calculation time.
   *
   * The Cholesky decomposition can be used to solve a linear equation
   * system A*x=b by first solving L*y=b and then L'*x=y using the
   * forwardSubstitution and backSubstitution functors, respectively.
   */
  template<class T>
  class choleskyDecomposition : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class choleskyDecomposition
     */
    class parameters 
      : public linearAlgebraFunctor::parameters,
        virtual public triangularMatrixType {
    public:
      /**
       * default constructor. The triangularType is Upper by default.
       */
      parameters() : linearAlgebraFunctor::parameters() {
        
        triangularType = Upper;
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
        return "choleskyDecomposition::parameters";
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
        functor::parameters& (functor::parameters::* p_copy)
                            (const functor::parameters&) =
                            functor::parameters::copy;
        (this->*p_copy)(other);
#     endif
        triangularMatrixType::copy(other);
        
        
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
        b = b && triangularMatrixType::write(handler,false);

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
        b = b && triangularMatrixType::read(handler,false);

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
    choleskyDecomposition();

    /**
     * Construct a functor using the given parameters
     */
    choleskyDecomposition(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    choleskyDecomposition(const choleskyDecomposition& other);

    /**
     * destructor
     */
    virtual ~choleskyDecomposition();

    /**
     * returns the name of this type ("choleskyDecomposition")
     */
    virtual const char* getTypeName() const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of parameters::triangularType. The default Upper is faster.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of parameters::triangularType. The default Upper is faster.
     * @param src matrix<T> with the source data.
     * @param l matrix<T> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& l) const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of \a tType.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @param tType type of triangular matrix
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& srcdest, 
               const typename parameters::eTriangularType& tType) const;

    /**
     * The matrix given must be symmetric and positive-definite. The
     * apply method returns the upper or lower triangular matrix
     * version of the Cholosky decomposition, depending on the value
     * of \a tType.
     * @param src matrix<T> with the source data.
     * @param l matrix<T> where the result will be left.
     * @param tType type of triangular matrix
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& l,
               const typename parameters::eTriangularType& tType) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    choleskyDecomposition& copy(const choleskyDecomposition& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    choleskyDecomposition& operator=(const choleskyDecomposition& other);

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

  };
}

#endif

