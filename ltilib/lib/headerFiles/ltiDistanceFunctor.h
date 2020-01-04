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
 * file .......: ltiDistanceFunctor.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 28.6.2000
 * revisions ..: $Id: ltiDistanceFunctor.h,v 1.14 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_DISTANCE_FUNCTOR_H_
#define _LTI_DISTANCE_FUNCTOR_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMath.h"

namespace lti {

  /**
   * This class is the base class for all functors which compute
   * distances between two vectors or matrices as well as norms of
   * vectors and matrices.
   *
   * @see similarityFunctor
   *
   * Be careful with the use of the parameters::rowWise.  It indicates
   * if the matrix should be considered as having row vectors (true) of
   * columns vectors (false).  Depending on that the computations will be
   * very different.
   */
  template <class T>
  class distanceFunctor: public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class distanceFunctor
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        rowWise=true;
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
      virtual ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "distanceFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
        linearAlgebraFunctor::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
          (this->*p_copy)(other);
# endif

        rowWise=other.rowWise;

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

        b = b && lti::write(handler, "rowWise", rowWise);

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

        b = b && lti::read(handler, "rowWise", rowWise);

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

      /**
       * determine whether norms and distances between the row vectors in
       * a matrix (true) or the column vectors in it (false) should
       * be calculated.
       *
       * Default value: true
       */
      bool rowWise;

    };

    /**
     * default constructor
     */
    distanceFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    distanceFunctor(const distanceFunctor<T>& other);

    /**
     * destructor
     */
    virtual ~distanceFunctor();

    /**
     * returns the name of this type ("distanceFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * calculate the norm of vector v.
     * @param v the vector<T>
     * @param norm the norm of the vector
     * @return false on error
     */
    virtual bool apply(const vector<T>& v, T& norm) const = 0;

    /**
     * calculate the norm of vector v
     * @param v the vector<T>
     * @return the norm of the vector
     */
    virtual T apply(const vector<T>& v) const =0;

    /**
     * calculate the norms of rows or columns of the matrix
     * @param m the matrix<T>
     * @param norms the norms of the rows/columns
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, vector<T>& norms) const=0;

    /**
     * calculate something like the norm of the matrix: the matrix
     * is seen as a vector.
     * @param m the matrix<T>
     * @param norm the 'norm' of the matrix
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, T& norm) const=0;

    /**
     * calculate something like the norm of the matrix: the matrix
     * is seen as a vector.
     * @param m the matrix<T>
     * @return the 'norm' of the matrix
     */
    virtual T apply(const matrix<T>& m) const=0;

    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dist the distance between the vectors
     * @return false on error -> see status string
     */
    virtual bool apply(const vector<T>& a, const vector<T>& b,
                       T& dist) const=0;

    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the distance between the vectors
     */
    virtual T apply(const vector<T>& a, const vector<T>& b) const=0;

    /**
     * calculate the distances between the rows or columns of the
     * matrices a and b, determined by the parameters rowWise.
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @param dists the distances between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       vector<T>& dists) const=0;

    /**
     * Calculate the distance between each row or column of m
     * depending on the value of rowWise and the vector v.
     * @param m the matrix<T>
     * @param v the vector to be compared with
     * @param dest the vector with the distances to the vector v
     * @return false on error
     */
    virtual bool apply(const matrix<T>& m, const vector<T>& v,
                       vector<T>& dest) const=0;

    /**
     * calculate something like the distance between the matrices a and b:
     * both matrices are seen as vectors.
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @param dist the 'distance' between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<T>& a, const matrix<T>& b,
                       T& dist) const=0;

    /**
     * calculate something like the distance between the matrices a and b:
     * both matrices are seen as vectors.
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @return the 'distance' between the matrices
     */
    virtual T apply(const matrix<T>& a, const matrix<T>& b) const=0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    distanceFunctor<T>& copy(const distanceFunctor<T>& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const=0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };

}

#include "ltiDistanceFunctor_template.h"

#endif


