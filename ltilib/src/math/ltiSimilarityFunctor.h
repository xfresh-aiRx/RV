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
 * file .......: ltiSimilarityFunctor.h
 * authors ....: Stefan Syberichs, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiSimilarityFunctor.h,v 1.11 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SIMILARITY_FUNCTOR_H_
#define _LTI_SIMILARITY_FUNCTOR_H_

#include "ltiLinearAlgebraFunctor.h"

namespace lti {

  /**
   * This class is the base class for all functors which compute
   * similarity measures between two vectors or matrices.
   *
   * It contains two apply functions, one for each type.
   *
   * @see distanceFunctor
   *
   * Be careful with the use of the parameters::rowWise.  It indicates
   * if the matrix should be considered as having row vectors (true) of
   * columns vectors (false).  Depending on that the computations will be
   * very different.
   */
  template <class T>
  class similarityFunctor: public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class similarityFunctor
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
    similarityFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    similarityFunctor(const similarityFunctor<T>& other);

    /**
     * destructor
     */
    virtual ~similarityFunctor();

    /**
     * returns the name of this type ("distanceFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * calculate the similarity of the vectors a and b
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the similarity of the vectors
     */
    virtual T apply(const vector<T>& a, const vector<T>& b) const = 0;

    /**
     * calculate the similarity of the matrices a and b
     * @param a the first matrix<T>
     * @param b the second matrix<T>
     * @return the similarity of the matrices
     */
    virtual T apply(const matrix<T>& a, const matrix<T>& b) const = 0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    similarityFunctor<T>& copy(const similarityFunctor<T>& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };

}

#include "ltiSimilarityFunctor_template.h"

#endif


