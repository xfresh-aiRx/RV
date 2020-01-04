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
 * file .......: ltiMeansFunctor.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiSerialStatsFunctor.h,v 1.17 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SERIALSTATS_FUNCTOR_H_
#define _LTI_SERIALSTATS_FUNCTOR_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiStatisticsFunctor.h"

namespace lti {
  /**
   * Functor which computes several kinds of means for
   * vectors and matrices.  It does NOT compute the extremes (min and max) of
   * the considered values, which is the main difference with his brother
   * functor lti::serialStatsFunctorExt
   *
   * The first template parameter T describes the type of the vectors
   * and matrices to be analysed.  The second parameter U (usually the same
   * type as T) specify the type of the statistics.
   *
   * For example, if you want to compute the mean and variance of the
   * elements {2,5,6,7,9} with double precision, you can use following code:
   *
   * \code
   *
   * lti::serialStatsFunctor<double> stats;
   * double theAverage,theVariance;
   * double data[] = {2,5,6,7,9};
   * lti::vector vct(5,data);
   *
   * stats.considerElements(vct);
   * stats.apply(theAverage,theVariance);
   * cout << "Average: " << theAverage << " Variance: " << theVariance << endl;
   *
   * \endcode
   *
   * It is also possible to compute the average/variance for each component
   * withing lti::vectors.

   */
  template <class T,class U=T>
  class serialStatsFunctor : public statisticsFunctor {
  public:
    /**
     * the parameters for the class meansFunctor
     */
    class parameters : public statisticsFunctor::parameters {
    public:

      /**
       * Type of the variance
       */
      enum eVarianceType {
        Empirical=0, /**< Empirical */
        Maxlikely=1  /**< Maxlikely */
      };

      /**
       * default constructor
       */
      parameters() : statisticsFunctor::parameters() {
        type = Empirical;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other)
        : statisticsFunctor::parameters() {
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
        return "serialStatsFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#       ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        statisticsFunctor::parameters::copy(other);
#       else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        statisticsFunctor::parameters&
          (statisticsFunctor::parameters::* p_copy)
          (const statisticsFunctor::parameters&) =
          statisticsFunctor::parameters::copy;
        (this->*p_copy)(other);
#      endif

        type=other.type;

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
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * workaround for MS-VC bug.  Never use this method directly,
       * use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {
          if (type == Empirical) {
            lti::write(handler,"type","Empirical");
          } else {
            lti::write(handler,"type","Maxlikely");
          }
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && statisticsFunctor::parameters::write(handler,false);
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
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,
                         const bool complete = true) const {
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
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete = true)
#     else
      /**
       * workaround for MS-VC bug.  Never use this method directly,
       * use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
          std::string str;

          lti::read(handler,"type",str);
          if (str == "Empirical") {
            type = Empirical;
          } else {
            type = Maxlikely;
          }
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && statisticsFunctor::parameters::read(handler,false);
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


      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * The type of the variance computation. If type == empirical,
       * the empirical variance or covariance matrix is computed
       * (division by number of samples minus 1), otherwise, the
       * maximum likelihood estimator is computed (division by
       * number of samples)
       */
      eVarianceType type;

    };

    /**
     * default constructor
     */
    serialStatsFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    serialStatsFunctor(const serialStatsFunctor& other);

    /**
     * destructor
     */
    virtual ~serialStatsFunctor();

    /**
     * returns the name of this type ("serialStatsFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * returns mean and variance of the data elements considered so far.
     * @param mean the result, where the mean will be put
     * @param variance the result, where the variable will be put
     */
    void apply(U& mean, U& variance) const;

    /**
     * returns mean and variance vectors of the data vectors considered so far.
     * @param mean the result, where the mean will be put
     * @param variance the result, where the variable will be put
     */
    void apply(vector<U>& mean, vector<U>& variance) const;

    /**
     * This function considers the given element for computing the
     * mean and variance.
     */
    void consider(const T &element);

    /**
     * This function considers the given element for computing the
     * mean and variance as if it were presented n times.
     */
    void consider(const T &element,const int& n);

    /**
     * This function considers the given vector for computing the
     * mean and variance.
     */
    void consider(const vector<T> &element);

    /**
     * This function considers the given vector for computing the
     * mean and variance, as if it were presented n times.
     */
    void consider(const vector<T> &element,const int& n);

    /**
     * Consider also all elements of the given vector
     */
    void considerElements(const vector<T> &elements);

    /**
     * Consider also all elements of the given matrix
     */
    void considerElements(const matrix<T> &elements);

    /**
     * Consider also all elements of the given matrix which lie within
     * the given rectangle.
     */
    void considerElements(const matrix<T> &elements,const rectangle& rect);

    /**
     * Consider all rows of the given matrix as vectors
     */
    void considerRows(const matrix<T> &elements);

    /**
     * The mean of all elements that have been considered so far.
     */
    U getMean() const;

    /**
     * The variance of all elements that have been considered so far.
     */
    U getVariance() const;

    /**
     * The sum of all elements that have been considered so far.
     */
    U getSumOfElements() const;

    /**
     * The sum of the squares of of all elements that have been
     * considered so far.
     */
    U getSumOfSquares() const;

    /**
     * return the number of elements considered so far
     */
    int getN() const;

    /**
     * return the number of vectors considered so far
     */
    int getNVectors() const;

    /**
     * The mean of all vectors that have been considered so far.
     */
    void getMean(vector<U> &m) const;

    /**
     * The variance of all vectors that have been considered so far.
     */
    void getVariance(vector<U> &v) const;

    /**
     * The sum of all vectors that have been considered so far.
     * The values will be copied into the given vector.
     */
    void getSumOfVectors(vector<U>& s) const;

    /**
     * The sum of the squares of all vectors that have been considered
     * so far.
     * The values will be copied into the given vector.
     */
    void getSumOfVectorSquares(vector<U>& s) const;

    /**
     * The sum of all vectors that have been considered so far.
     * @return a read-only reference to the internal data
     */
    const vector<U>& getSumOfVectors() const;

    /**
     * The sum of the squares of all vectors that have been considered
     * so far.
     * @return a read-only reference to the internal data
     */
    const vector<U>& getSumOfVectorSquares() const;

    /**
     * Reset the series, i.e. discard all considered elements.
     */
    void reset();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    serialStatsFunctor& copy(const serialStatsFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:
    U sumOfElements;
    U sumOfSquares;
    vector<U> sumOfVectors;
    vector<U> sumOfVecSq;
    int n;  // number of elements considered so far
    int nv; // number of vector considered so far!
  };
}

#include "ltiSerialStatsFunctor_template.h"

#endif

