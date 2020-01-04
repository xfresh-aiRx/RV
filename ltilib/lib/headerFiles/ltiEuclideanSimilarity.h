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
 * file .......: ltiEuclideanSimilarity.h
 * authors ....: Stefan Syberichs, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiEuclideanSimilarity.h,v 1.7 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_EUCLID_SIMILARITY_H_
#define _LTI_EUCLID_SIMILARITY_H_

#include "ltiSimilarityFunctor.h"

namespace lti {

  /**
   * Euclidean Similarity Measure
   *
   * This class computes the similarity of two vectors or matrices
   * by means of a measure based on the euclidean distance of the two vectors.
   *
   */
  template <class T>
  class euclideanSimilarity : public similarityFunctor<T> {
  public:
    /**
     * the parameters for the class similarityFunctor
     */
    class parameters : public similarityFunctor<T>::parameters {
    public:
      /**
       * Types of similarity measures based on the euclidean distance
       */
      enum eEuclideanSimilarityType {
        Inverse,     /**< Based on the inverse of the distance
                      *   (1/(1+|a-b|^2))
                      */
        Exponential  /** Based on the exponential exp(-|a-b|^2) */
      };

      /**
       * default constructor
       */
      parameters() : similarityFunctor<T>::parameters() {
        rowWise=true;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) {
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
        similarityFunctor<T>::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
        similarityFunctor<T>::parameters&
          (similarityFunctor<T>::parameters::* p_copy)
          (const similarityFunctor<T>::parameters&) =
          similarityFunctor<T>::parameters::copy;
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

        switch(type) {
          case Inverse:
            b = b && lti::write(handler, "type", "Inverse");
            break;
          case Exponential:
            b = b && lti::write(handler, "type", "Exponential");
            break;
          default:
            b = b && lti::write(handler, "type", "Inverse");
        }

        if (complete) {
          b = b && handler.writeEnd();
        }


#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && similarityFunctor<T>::parameters::write(handler,false);
#     else
        bool (similarityFunctor<T>::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          similarityFunctor<T>::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

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

        std::string str;

        b = b && lti::read(handler, "type", str);
        if (str == "Exponential") {
          type = Exponential;
        } else {
          type = Inverse;
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && similarityFunctor<T>::parameters::read(handler,false);
#     else
        bool (similarityFunctor<T>::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          similarityFunctor<T>::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

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
       * determine how the euclidean distance is used.
       *
       * Default value: Inverse
       */
      eEuclideanSimilarityType type;
    };

    /**
     * default constructor
     */
    euclideanSimilarity();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    euclideanSimilarity(const euclideanSimilarity<T>& other);

    /**
     * destructor
     */
    virtual ~euclideanSimilarity();

    /**
     * returns the name of this type ("l1Similarity")
     */
    virtual const char* getTypeName() const;

    /**
     * calculates the a similarity of the vectors a and b
     * based on their euclidean distance.
     * If both vectors have different sizes, the returned value will be
     * negative!
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the cos2-type of similarity of a and b
     */
    T apply(const vector<T>& a, const vector<T>& b) const;

    /**
     * is not (yet) defined for matrices.
     @return always null
     */
    T apply(const matrix<T>& a, const matrix<T>& b) const;

    /**
     * Is not (yet) defined for matrices.
     * @return always null
     */
    vector<T>& apply(const matrix<T>& a,
                     const vector<T>& b,
                           vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    euclideanSimilarity& copy(const euclideanSimilarity& other);

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

#include "ltiEuclideanSimilarity_template.h"

#endif

