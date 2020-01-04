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
 * file .......: ltiMatrixInversion.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 07.10.01
 * revisions ..: $Id: ltiMatrixInversion.h,v 1.16 2005/11/04 09:42:22 doerfler Exp $
 */


#ifndef _LTI_MATRIX_INVERSION_H_
#define _LTI_MATRIX_INVERSION_H_

#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   * Matrix inversion functor.
   *
   * Computes the inverse of a matrix using LU decomposition.
   * Only use this functor if the inverse of a matrix is needed explicity!
   * To solve an equation system
   * Ax=b or a set of equation systems AX=B it is more efficient to use the
   * LU solution method (solve Ax=b resp. AX[i]=B[i]) directly than
   * inverting A and multiplying the result by b resp. B!
   *
   * If the parameter value method is set to SVD, a singular value
   * decomposition is used to invert the matrix, instead of the LU
   * decomposition method.
   *
   * The apply() methods return false if the matrix is singular or
   * close to being singular. This is defined by checking that the
   * smalles singular value is greater than
   * std::numeric_limits<T>::epsilon() for SVD and internally in the
   * LU decomposition.
   *
   * For small (2x2,3x3 or 4x4) symmetric matrices you can also use
   * lti::symmetricMatrixInversion.
   *
   * @ingroup gLinearAlgebra
   */
  template <class T>
  class matrixInversion : public linearAlgebraFunctor {
  public:
    /**
     * %parameters class for lti::matrixInversion %functor
     */
    class parameters: public linearAlgebraFunctor::parameters {

    public:
      /**
       * Default constructor
       */
      parameters(void) : linearAlgebraFunctor::parameters() {
        method=LUD;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "matrixInversion::parameters";
      };

      /**
       * copy member
       *
       * @param other the parameters object to be copied
       * @return a reference to this object
       */
      parameters& copy(const parameters& other) {
#       ifndef _LTI_MSC_6
          // MS Visual C++ 6 is not able to compile this...
          linearAlgebraFunctor::parameters::copy(other);
#       else
          // ...so we have to use this workaround.
          // Conditional on that, copy may not be virtual.
          functor::parameters&
            (functor::parameters::* p_copy)
            (const functor::parameters&) =
            functor::parameters::copy;
          (this->*p_copy)(other);
#       endif

        method=other.method;

        return *this;
      };

      /**
       * return a cloned instance of this one.
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

        /**
         * write the parameters in the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also written, otherwise only the data block will be
         *        written.
         * @return true if write was successful
         */
#     ifndef _LTI_MSC_6
        virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
        bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.writeBegin();
          }

          if (b) {
            if (method == LUD) {
              lti::write(handler,"method","LUD");
            } else {
              lti::write(handler,"method","SVD");
            }
          }

#       ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && linearAlgebraFunctor::parameters::write(handler,false);
#       else
          bool (functor::parameters::* p_writeMS)(ioHandler&,
                                                  const bool) const =
            functor::parameters::writeMS;
          b = b && (this->*p_writeMS)(handler,false);
#       endif

          if (complete) {
            b = b && handler.writeEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        virtual bool write(ioHandler& handler,
                           const bool complete = true) const {
           // ...we need this workaround to cope with another really
           // awful MSVC bug.
           return writeMS(handler,complete);
        }
#     endif

        /**
         * read the parameters from the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also read, otherwise only the data block will be read.
         * @return true if write was successful
         */
#     ifndef _LTI_MSC_6
        virtual bool read(ioHandler& handler,const bool complete = true)
#     else
        bool readMS(ioHandler& handler,const bool complete=true)
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.readBegin();
          }

          if (b) {
            std::string str;
            lti::read(handler,"method",str);
            if (str == "LUD") {
              method = LUD;
            } else {
              method = SVD;
            }
          }

#     ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && linearAlgebraFunctor::parameters::read(handler,false);
#     else
          bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
          b = b && (this->*p_readMS)(handler,false);
#     endif

          if (complete) {
            b = b && handler.readEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        virtual bool read(ioHandler& handler,const bool complete=true) {
          // ...we need this workaround to cope with another really awful MSVC
          // bug.
          return readMS(handler,complete);
        }
#      endif

      // ------------------------------------------------------------------
      // The Parameters
      // ------------------------------------------------------------------

      /**
       * Available algorithms for matrix inversion
       */
      enum algorithmType {
        LUD = 0, //!< LU-Decomposition
        SVD = 1  //!< Singular Value Decomposition
      };

      /**
       * Algorithm to be used in the matrix inversion
       *
       * Default value: LUD
       */
      algorithmType method;
    };

    /**
     * default constructor
     */
    matrixInversion();

    /**
     * destructor
     */
    virtual ~matrixInversion() {};

    /**
     * onCopy version of apply.
     *
     * @param theMatrix matrix to be inverted
     * @param theInverse inverted matrix
     * @return true if inversion was possible, false otherwise.
     */
    bool apply(const matrix<T>& theMatrix,
                     matrix<T>& theInverse) const;

    /**
     * onPlace version of apply.
     *
     * @param theMatrix matrix to be inverted.  The result will be left here
     *                  too.
     * @return true if inversion was possible, false otherwise.
     */
    bool apply(matrix<T>& theMatrix) const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new matrixInversion<T>(*this));
    };

    /**
     * change the used parameters in order to used the
     * LU-decomposition method for matrix inversion.
     */
    void useLUD();

    /**
     * change the used parameters in order to used the singular value
     * decomposition method for matrix inversion.
     */
    void useSVD();

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {return "matrixInversion";};

    /**
     * get a constant reference to the actual used parameters.
     */
    const parameters& getParameters() const;

  protected:
    /**
     * smallest value of type T, which can be added to 0 in order to produce
     * a number different than zero.
     */
    static const T my_epsilon;
  };
}

#endif


