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
 * file .......: ltiLinearAlgebraFunctor.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 01.06.99
 * revisions ..: $Id: ltiLinearAlgebraFunctor.h,v 1.13 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_LINEAR_ALGEBRA_FUNCTOR_H_
#define _LTI_LINEAR_ALGEBRA_FUNCTOR_H_

#include "ltiFunctor.h"
#include "ltiException.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Base class for all lti linear algebra functors.
   *
   * @ingroup gLinearAlgebra
   */
  class linearAlgebraFunctor : public functor {
  public:
    /**
     * singular matrix exception
     */
    class singularMatrixException : public exception {
    public:
      /**
       * Default constructor
       */
      singularMatrixException()
        : exception("a matrix singularity was encountered") {}
      /**
       * Return the name of this class
       */
      virtual const char* getTypeName() const {
        return "linearAlgebraFunctor::singularMatrixException";
      };
    };
    /**
     * incompatible dimensions exception
     */
    class incompatibleDimensionsException : public exception {
    public:
      /**
       * Default constructor
       */
      incompatibleDimensionsException()
        : exception("MathObject dimensions do not match") {};
      /**
       * Return the name of this class
       */
      virtual const char* getTypeName() const {
        return "linearAlgebraFunctor::incompatibleDimensionsException";
      };
    };

    /**
     * the parameters for the linear algebra functors
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
      parameters(const parameters& other) : functor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      virtual ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "linearAlgebraFunctor::parameters";
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
        functor::parameters& (functor::parameters::* p_copy)
          (const functor::parameters&) = functor::parameters::copy;
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

# ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete) const
# else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,
                   const bool complete) const
# endif
      {
        bool b(true);

        if (complete) {
          b = b && handler.writeBegin();
        }

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

# ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete) const {
       // ...we need this workaround to cope with another really awful MSVC
       // bug.
        return writeMS(handler,complete);
      }
# endif


# ifndef _LTI_MSC_6
      /**
       * read the parametersfrom the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,
                const bool complete)
# else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,
                  const bool complete)
# endif
      {
        bool b(true);

        if (complete) {
          b = b && handler.readBegin();
        }

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

# ifdef _LTI_MSC_6
      /**
       * read the parametersfrom the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,
                const bool complete) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
# endif

    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "linearAlgebraFunctor";
    };
  };

  /**
   * Dot Product of two vectors.
   *
   * computes the dot product of two vectors (elementtypes T and U) and
   * returns the product (type T!)
   *
   * @ingroup gLinearAlgebra
   */
  template<class T,class U=T>
  class scalarProduct : public linearAlgebraFunctor {
    public:
    /**
     * default constructor
     */
    scalarProduct()
      : linearAlgebraFunctor() {};
    /**
     * destructor
     */
    virtual ~scalarProduct() {};

    /**
     * onCopy version of apply.
     */
    T& apply(const vector<T>& mult1,
             const vector<U>& mult2,
                   T& result) const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new scalarProduct<T,U>(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {return "scalarProduct";};
  };

}

#include "ltiLinearAlgebraFunctor_template.h"

#endif

