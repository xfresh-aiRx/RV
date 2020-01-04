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
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 21.11.2001
 * revisions ..: $Id: ltiSerialVectorStats.h,v 1.16 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SERIALVECTORSTATS_H_
#define _LTI_SERIALVECTORSTATS_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiStatisticsFunctor.h"
#include "ltiTypeInfo.h"

namespace lti {
  /**
   * Functor which computes the statistics of a series of vectors.
   * It delivers the mean vector and the covariance matrix, or
   * alternatively the variance vector under the assumption, that
   * the components of the vectors are independent from each other.
   *
   * If the empirical computation type is selected, the variances
   * resp. covariances will be normalized with (N-1). Otherwise
   * with N.
   *
   * An example (maximum-likelihood case, i.e. normalizing with N):
   *
   * Assume we have three vectors {3,4},{1,3},{2,2}
   *
   * The mean vector is {2,3}
   *
   * The covariance matrix, which is always symetric, is:
   *
   * \code
   * | 2/3  1/3  |
   * | 1/3  2/3  |
   * \endcode
   *
   * The variance vector is {2/3,2/3}, i.e. the diagonal elements
   * of the covariance matrix.
   */
  template <class T>
  class serialVectorStats : public statisticsFunctor {
  public:
    /**
     * the parameters for the class serialVectorStats
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
        correlation=false;
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
        return "serialVectorStats::parameters";
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
        correlation=other.correlation;

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
          lti::write(handler, "correlation", correlation);
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
          lti::read(handler, "correlation", correlation);
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
       * number of samples). 
       *
       * Default is Empirical.
       */
      eVarianceType type;

      /**
       * If true, the covariance function computes the correlation
       * coefficient instead.
       *
       * Default value: false
       */
      bool correlation;
    };

    typedef typename typeInfo<T>::square_accumulation_type sqrType;

    /**
     * default constructor
     */
    serialVectorStats();
    
    /**
     * copy constructor
     * @param other the object to be copied
     */
    serialVectorStats(const serialVectorStats& other);

    /**
     * destructor
     */
    virtual ~serialVectorStats();

    /**
     * returns the name of this type ("serialVectorStats")
     */
    virtual const char* getTypeName() const;

    /**
     * returns mean and variance vectors of the data vectors considered so far.
     * @param mean the result, where the mean will be put
     * @param variance the result, where the variance will be put
     */
    void apply(vector<T>& mean, vector<T>& variance) const;

    /**
     * returns mean vector and covariance matrix of the data vectors
     * considered so far.
     *
     * @param mean the result, where the mean will be put
     * @param covariance the matrix, where the covariances will be put
     */
    void apply(vector<T>& mean, matrix<T>& covariance) const;

    /**
     * This function considers the row vectors of the given matrix
     * for computing the mean and variance.
     */
    void considerRows(const matrix<T> &element);

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
     * return the number of elements considered so far
     */
    double getN() const;

    /**
     * set the number of elements considered so far
     */
    void setN(double newN);

    /**
     * The mean of all vectors that have been considered so far.
     */
    void getMean(vector<T> &mean) const;

    /**
     * The variance of all vectors that have been considered so far.
     */
    void getVariance(vector<T> &var) const;

    /**
     * The covariance of all vectors that have been considered so far.
     */
    void getCovariance(matrix<T> &covar) const;

    /**
     * The sum of all vectors that have been considered so far.
     * The values will be copied into the given vector.
     */
    void getSumOfVectors(vector<T>& s) const;

    /**
     * The sum of the squares of all vectors that have been considered
     * so far.
     * The values will be copied into the given vector.
     */
    void getSumOfVecOuterProd(matrix<sqrType>& s) const;

    /**
     * The sum of all vectors that have been considered so far.
     * @return a read-only reference to the internal data
     */
    const vector<T>& getSumOfVectors() const;

    /**
     * The sum of the outer products of all vectors that
     * have been considered so far. (i.e. sum(vec*vecTransposed))
     * @return a read-only reference to the internal data
     */
    const matrix<sqrType>& getSumOfVecOuterProd() const;

    /**
     * Reset the series, i.e. discard all considered elements.
     */
    void reset();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    serialVectorStats& copy(const serialVectorStats& other);

    /**
     * Alias for copy.
     */
    serialVectorStats& operator=(const serialVectorStats& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Reads this functor from the given handler.
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * Writes this functor to the given handler.
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

  protected:
    /**
     * compute the outer product of two vector<T> and leave the result
     * in a matrix<sqrType>
     */
    void outer(const vector<T>& a,
               const vector<T>& b,
               matrix<sqrType>& dest) const;


    /**
     * accumulate the outer product of a and b in the matrix dest
     * The matrix dest MUST have the correct size before calling this method.
     */
    void outerAcc(const vector<T>& a,
                  const vector<T>& b,
                  matrix<sqrType>& dest) const;

    /**
     * accumulate n times the outer product of a and b in the matrix dest
     * The matrix dest MUST have the correct size before calling this method.
     */
    void outerAcc(const vector<T>& a,
                  const vector<T>& b,
                  const int n,
                  matrix<sqrType>& dest) const;
    
    /**
     * get diagonal vector of the given matrix, but casted into the lower type
     */
    void diagonal(const matrix<sqrType>& mat,vector<T>& diag) const;

  private:
    /**
     * sum of vectors
     */
    vector<T> sumX;   

    /**
     * sum of outer products of vectors
     */
    matrix<sqrType> sumXXT;

    /**
     * number of vector considered so far!
     */
    double nv; 
    
  };

  template <class T>
  bool read(ioHandler& handler,
            serialVectorStats<T>& pca,
            const bool complete=true) {
    return pca.read(handler,complete);
  }


  template <class T>
  bool write(ioHandler& handler,
             const serialVectorStats<T>& pca,
             const bool complete=true) {
    return pca.write(handler,complete);
  }

}

#include "ltiSerialVectorStats_template.h"

#endif


