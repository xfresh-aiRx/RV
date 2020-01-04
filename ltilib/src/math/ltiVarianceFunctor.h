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
 * file .......: ltiVarianceFunctor.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.9.2000
 * revisions ..: $Id: ltiVarianceFunctor.h,v 1.20 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_VARIANCE_FUNCTOR_H_
#define _LTI_VARIANCE_FUNCTOR_H_

#include "ltiStatisticsFunctor.h"
#include "ltiMeansFunctor.h"

namespace lti {
  /**
   * Functor which computes several kinds of variances for
   * vectors and matrices.
   *
   * Be careful with the terms used here.  If you have following matrix
   *
   * \code
   * |  5  6  10  2 |
   * |  1 -2   4  4 |
   * |  3  2   1 -3 |
   * \endcode
   *
   * The (empirical) varianceOfRows means the variance of all rows,
   * i.e. a row vector containing the sum of the square of difference
   * between each row vector and the mean vector of all rows, divided
   * by the number of rows minus 1.  This is (4 8 21 13).
   *
   * The (empirical) varianceOfColumns means the variance of all columns,
   * i.e. a column vector containing the sum of the square of difference
   * between each column vector and the mean vector of all columns, divided
   * by the number of columns minus 1.  This is (8.667 4.667 2)^T.
   *
   * The parameter parameters::rowWise indicates that the matrix will be
   * seen as a set of row vectors (if true), or column vectors (false).
   */
  template <class T> class varianceFunctor : public statisticsFunctor {
  public:
    /**
     * the parameters for the class varianceFunctor
     */
    class parameters : public statisticsFunctor::parameters {
    public:
      /**
       * Type of the variance
       */
      enum eVarianceType {
        Empirical=0, /**< Empirical Variance:
                      *   \f[ \frac{1}{n-1} \sum_i^n (x_i - \mu)^2 \f]
                      */
        Maxlikely=1  /**< Maximum Likelihood Variance:
                      *   \f[ \frac{1}{n} \sum_i^n (x_i - \mu)^2 \f]
                      */
      };

      /**
       * default constructor
       */
      parameters()
        : statisticsFunctor::parameters() {
        rowWise=true;
        type=Empirical;
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
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "varianceFunctor::parameters";
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

        rowWise=other.rowWise;
        type=other.type;
        correlation=other.correlation;

        return *this;
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
            lti::write(handler,"rowWise",rowWise);
            if (type == Empirical) {
              lti::write(handler,"type","Empirical");
            } else {
              lti::write(handler,"type","Maxlikely");
            }
            lti::write(handler,"correlation",correlation);

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
            lti::read(handler,"rowWise",rowWise);

            std::string str;

            lti::read(handler,"type",str);
            if (str == "Empirical") {
              type = Empirical;
            } else {
              type = Maxlikely;
            }

            lti::read(handler,"correlation",correlation);
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
#      endif


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * If this flag is true, the variance computation will be row-wise, i.e.
       * the result will contain a sum of the rows. This is much faster
       * than column-wise, since data do not have to be copied.
       *
       * Default value: true
       */
      bool rowWise;

      /**
       * The type of the variance computation. If type == empirical,
       * the empirical variance or covariance matrix is computed
       * (division by number of samples minus 1), otherwise, the
       * maximum likelihood estimator is computed (division by
       * number of samples).
       *
       * Default value: Empirical
       */
      eVarianceType type;

      /**
       * If this flag is true, the covariance matrix is normalized
       * to contain the correlation coefficients instead of the
       * covariances.
       *
       * Default value: false
       */
      bool correlation;

    };

    /**
     * default constructor */
    varianceFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    varianceFunctor(const varianceFunctor& other);

    /**
     * destructor
     */
    virtual ~varianceFunctor();

    /**
     * returns the name of this type ("varianceFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * returns the variance of the current vector.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    T apply(const vector<T>& srcdest) const;

    /**
     * computes the variance of the given vector.
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    T &apply(const vector<T>& src, T& dest) const;

    /**
     * The result of this function depends on the value of
     * parameters.rowWise. If this parameter is true, the functor
     * will compute a vector, whose elements contain each the variance
     * of one column of the matrix (the computes the variance of the rows,
     * where each row is a data point in n-dimensional space.
     * if rowWise is false, the result vector contains the variance
     * of the columns of the matrix (each column a data point).
     * @param src matrix<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    vector<T>& apply(const matrix<T>& src, vector<T>& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the variance
     * of one column of the matrix (the computes the variance of the rows,
     * where each row is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest vector<T> where the result will be left.
     */
    void varianceOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * The result vector contains the variances
     * of the columns of the matrix (each column a data point).
     * @param src matrix<T> with the source data.
     * @param dest vector<T> where the result will be left.
     */
    void varianceOfColumns(const matrix<T>& src, vector<T>& dest) const;


    /**
     * The result is the covariance of these two vectors. The vectors are
     * regarded as series of outcomes of a twodimensional random variable.
     * This function computes the covariance of both dimensions of the
     * variable.
     * @param a vector<T> with dimension one of the random variable.
     * @param b vector<T> with dimension two of the random variable.
     * @return covariance between a and b
     */
    T covariance(const vector<T>& a, const vector<T>& b) const;

    /**
     * This function
     * will compute a matrix which contains the covariance matrix
     * of the data points in the first matrix, if each row constitutes
     * a data point in n-dimensional space
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void covarianceMatrixOfRows(const matrix<T> &src, matrix<T>& dest) const;

    /**
     * This function
     * will compute a matrix which contains the covariance matrix
     * of the data points in the first matrix, if each row constitutes
     * a data point in n-dimensional space
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param mu mean value of the data.
     */
    void covarianceMatrixOfRows(const matrix<T>& src,
                                matrix<T>& dest,
                                vector<T>& mu) const;

    /**
     * This function
     * will compute a matrix which contains the covariance matrix
     * of the data points in the first matrix, if each row constitutes
     * a data point in n-dimensional space. This is an older, slow
     * version which has been left here, because there are a few
     * rare cases when it is still somewhat faster than the new version.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void slowCovarianceMatrixOfRows(const matrix<T> &src, matrix<T>& dest) const;

    /**
     * This function
     * will compute a matrix which contains the covariance matrix
     * of the data points in the first matrix, if each column constitutes
     * a data point in n-dimensional space
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void covarianceMatrixOfColumns(const matrix<T> &src,matrix<T>& dest) const;

    /**
     * This function
     * will compute a matrix which contains the covariance matrix
     * of the data points in the first matrix, if each column constitutes
     * a data point in n-dimensional space
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param mu mean value of the data.
     */
    void covarianceMatrixOfColumns(const matrix<T> &src,
                                   matrix<T>& dest,
                                   vector<T>& mu) const;


    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    varianceFunctor& copy(const varianceFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Converts a covariance matrix into a correlation coefficient matrix.
     */
    static void covar2corrcoef(matrix<T>& cv);

  private:
    meansFunctor<T> mean;

  };
}


#endif


