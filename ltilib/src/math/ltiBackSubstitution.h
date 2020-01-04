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
 * file .......: ltiBackSubstitution.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 20.5.2003
 * revisions ..: $Id: ltiBackSubstitution.h,v 1.8 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_BACK_SUBSTITUTION_H_
#define _LTI_BACK_SUBSTITUTION_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiTriangularMatrixType.h"

namespace lti {
  /**
   * Backsubstitution. Let \a L be a lower triangular matrix and
   * \a U an upper triangular matrix. Backsubstitution solves the
   * following linear equation systems for \a x:
   *
   * U*x=b or x*L=b
   *
   * In the first case, \a x and \a b are column vectors, in the latter
   * case they are row vectors. The data %matrix can be given to
   * the functor via the parameters or directly in the apply
   * methods. There are also apply methods that calculate the
   * backsubstitution for multiple right-sides \a b. The vectors are
   * treated as before, ie they are in columns for the first case and
   * in rows for the second.
   *
   * \b Note that the column vector matrix will be transposed twice
   * and thus takes longer to compute, try to use the vector-apply
   * instead.
   *
   * @see lti::backSubstitution
   */
  template<class T>
  class backSubstitution : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class backSubstitution
     */
    class parameters 
      : public linearAlgebraFunctor::parameters, 
        virtual public triangularMatrixType {
    public:

      /**
       * default constructor. The triangularType is Upper by default.
       */
      parameters() : linearAlgebraFunctor::parameters(), theMatrix() {
        //TODO: Initialize your parameter values!
        // If you add more parameters manually, do not forget to do following:
        // 1. indicate in the default constructor the default values
        // 2. make sure that the copy member also copy your new parameters
        // 3. make sure that the read and write members also read and
        //    write your parameters
        
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
        return "backSubstitution::parameters";
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
        
        theMatrix.copy(other.theMatrix);
        
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
          
          lti::write(handler,"theMatrix", theMatrix);

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

          lti::read(handler,"theMatrix", theMatrix);
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


      /**
       * The data matrix.
       */
      matrix<T> theMatrix;

    };

    /**
     * default constructor
     */
    backSubstitution();

    /**
     * constructor, sets the data matrix and its triangular type in
     * the parameters.
     *
     * \b Note: Using this constructor, the data matrix is copied
     * twice (Once to internally put it into a parameters object and
     * then when setting the parameters). It can be better to let
     * parameters::theMatrix use the data of some matrix you have:
     *
     * \code
     * matrix<double> myData; //let this contain the data
     * backSubstitution<double>::parameters param;
     * param.theMatrix.useExternData(myData.rows(),myData.columns(),&myData.at(0,0))
     * backSubstitution<double> mySubst(param);
     * \endcode
     *
     * The fastest method is to use one of the apply-methods where the
     * data matrix is an argument.
     *
     * @param theMatrix the data matrix
     * @param tType triangularType of \a theMatrix
     */
    backSubstitution(const matrix<T>& theMatrix, 
                     const typename parameters::eTriangularType& tType=
                     parameters::Lower);

    /**
     * Construct a functor using the given parameters
     */
    backSubstitution(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    backSubstitution(const backSubstitution& other);

    /**
     * destructor
     */
    virtual ~backSubstitution();

    /**
     * returns the name of this type ("backSubstitution")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!
    

    /**
     * Performs backsubstitution with unknown \a x.  U*x=b if \a
     * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
     * triagonal. The result \a x is left in \a b. The matrix in the
     * parameters is used.
     *
     * @param b the right side of the backsubstitution. The result
     *          will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(vector<T>& b) const;

    /**
     * Performs backsubstitution with unknown \a x.  U*x=b if \a
     * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
     * triagonal. The matrix in the parameters is used.
     *
     * @param b the right side of the backsubstitution. 
     * @param x the result.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const vector<T>& b, vector<T>& x) const;

    /**
     * Performs backsubstitution with unknown \a x.  U*x=b if \a
     * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
     * triagonal. The result \a x is left in \a b. 
     *
     * @param theMatrix data matrix (L or U)
     * @param b the right side of the backsubstitution. The result
     *          will be left here too.
     * @param tType defines whether theMatrix is lower or upper triangular
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& theMatrix, vector<T>& b,
               const typename parameters::eTriangularType& tType) const;

    /**
     * Performs backsubstitution with unknown \a x.  U*x=b if \a
     * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
     * triagonal. 
     *
     * @param theMatrix data matrix (L or U)
     * @param b the right side of the backsubstitution. The result
     *          will be left here too.
     * @param x the result.
     * @param tType defines whether theMatrix is lower or upper triangular
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& theMatrix, const vector<T>& b,
               vector<T>& x, 
               const typename parameters::eTriangularType& tType) const;


    /**
     * Performs backsubstitution with unknowns \a X.  U*X=B if \a
     * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
     * triagonal. In the first case \a X and \a B contain the vectors
     * in colums, in the latter case in rows. The result \a x is left
     * in \a b. The matrix in the parameters is used.
     *
     * @param b the right side of the backsubstitution. The result
     *          will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(matrix<T>& b) const;

    /**
     * Performs backsubstitution with unknowns \a X.  U*X=B if \a
     * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
     * triagonal. In the first case \a X and \a B contain the vectors
     * in colums, in the latter case in rows. The data matrix in the
     * parameters is used.
     *
     * @param b the right side of the backsubstitution. 
     * @param x the result
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& b, matrix<T>& x) const;

    /**
     * Performs backsubstitution with unknowns \a X.  U*X=B if \a
     * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
     * triagonal. In the first case \a X and \a B contain the vectors
     * in colums, in the latter case in rows. The result \a x is left
     * in \a b.
     *
     * @param theMatrix data matrix (L or U)
     * @param b the right side of the backsubstitution. The result
     *          will be left here too.
     * @param tType defines whether theMatrix is lower or upper triangular
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& theMatrix, matrix<T>& b, 
               const typename parameters::eTriangularType& tType) const;

    /**
     * Performs backsubstitution with unknowns \a X.  U*X=B if \a
     * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
     * triagonal. In the first case \a X and \a B contain the vectors
     * in colums, in the latter case in rows.
     *
     * @param theMatrix data matrix (L or U)
     * @param b the right side of the backsubstitution. The result
     *          will be left here too.
     * @param x the result
     * @param tType defines whether theMatrix is lower or upper triangular
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<T>& theMatrix, const matrix<T>& b, 
               matrix<T>& x, 
               const typename parameters::eTriangularType& tType) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    backSubstitution& copy(const backSubstitution& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    backSubstitution& operator=(const backSubstitution& other);

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

