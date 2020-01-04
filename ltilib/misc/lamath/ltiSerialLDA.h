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
 * file .......: ltiLDA.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiSerialLDA.h,v 1.11 2005/04/07 12:34:23 doerfler Exp $
 */

#ifndef _LTI_SERIAL_LDA_H_
#define _LTI_SERIAL_LDA_H_

#ifdef HAVE_LAPACK

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrixInversion.h"
#include "ltiGenEigVectors.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"


namespace lti {
  /**
   * Functor for iteratively computing a linear discriminant analysis.
   * The functor is used by subsequent calls of consider(),
   * which is given the data of another class each time.
   * Then, the first time apply() is called, the LDA is computed
   * and can be used. It is possible to mix calls to consider and
   * apply.
   *
   * It uses functors that require the LAPack.
   *
   * @ingroup lapack
   *
   * @see \ref lapack
   *
   */
  template <class T>
  class serialLDA : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class serialLDA
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      //TODO: comment the parameters of your functor

      bool read(ioHandler &handler, const bool complete=true);
      bool write(ioHandler &handler, const bool complete=true) const;

      /**
       * This is the dimension of the reduced vectors.
       */
      int resultDim;

      /**
       * This flag determines, if the functor should determine a
       * maximum allowable dimension itself. "Maximum dimension" means
       * that the dimension is equal to the number of eigenvalues of
       * the covariance matrix which are larger than zero.
       *
       * Default value: false
       */
      bool autoDim;
    };

    /**
     * default constructor
     */
    serialLDA();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    serialLDA(const serialLDA& other);

    /**
     * destructor
     */
    virtual ~serialLDA();

    /**
     * returns the name of this type ("serialLDA")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /*
     * operates on the given %parameter.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @param labels ivector for each row of the source matrix, this
     *        vector contains the class label.
     * @result true if the LDA could be computed, false otherwise
     *
    bool apply(matrix<T>& srcdest, const ivector& labels) const;
    */

    /**
     * Adds a single class to the lda. This can be used as an
     * iterative version of the apply method. Class labels are
     * determined automatically.
     *
     * @param src matrix<T> with the source data.
     * @result true if the data could be considered, false otherwise
    */
    bool consider(const matrix<T>& src);

    /*
     * Computes a  linear discriminant analysis for the data given in
     * src. This matrix is assumed to contain the data points for
     * one class. The computed transformation is stored internally
     * and can be used just as a global transformation. The main
     * purpose of this method is to be used for LDA classifiers,
     * which compute class-wise LDA transforms and classify by
     * subsequently applying the LDA's of all classes to the vector, and
     * then choosing the class whose mean is closest to the presented
     * vector. This method can be used to compute the required LDAs.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @param labels matrix<T> which is the between-class
     *        scatter matrix of the entire population.
     * @result true if the LDA could be computed, false otherwise
    *
    bool apply(const matrix<T>& src, matrix<T>& dest,
               const matrix<T>& Sb);
    */

    /**
     * Uses a previously computed transform in order to reduce
     * the given data set's dimensionality according to the
     * resultDim parameter.
     * @param src matrix<T> with the source data.
     * @param result matrix<T> with the result data.
     * @result true if the LDA could be applied, false otherwise
     */
    bool apply(const matrix<T>&src, matrix<T>& result);

    /**
     * Uses a previously computed transform in order to reduce
     * the given data set's dimensionality according to the
     * resultDim parameter.
     * @param src matrix<T> with the source data.
     * @param result matrix<T> with the result data.
     * @result true if the LDA could be applied, false otherwise
     */
    bool apply(const vector<T>&src, vector<T>& result);

    /**
     * On-Place version of the transformation.
     * @param srcdest matrix<T> with the source data, which will also contain
     *        the result.
     * @result true if the LDA could be applied, false otherwise
     */
    bool apply(matrix<T>& srcdest);



    /**
     * Set the dimension to which the vectors should be reduced.
     */
    void setDimension(int k);

    /**
     * Returns the previously computed eigenvalues of the scatter
     * matrix.
     * @param result the vector which will receive the eigenvalues.
     * @return true if the values could be obtained, false otherwise.
     */
    bool getEigenValues(vector<T>& result) const;

    /**
     * Returns the previously computed eigenvalues of the scatter
     * matrix.
     * @return true if the values could be obtained, false otherwise.
     */
    const vector<T>& getEigenValues() const;

    /**
     * Returns the previously computed eigenvectors of the scatter
     * matrix.
     * @param result the matrix which will receive the eigenvectors.
     *        Each column of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    bool getEigenVectors(matrix<T>& result) const;

    /**
     * Returns the previously computed eigenvectors of the scatter
     * matrix.
     *
     * This method will call the normal getEigenVectors() methods and
     * after that will transpose the obtained matrix, i.e. it is faster
     * to get the eigenvectors in the columns.
     *
     * @param result the matrix which will receive the eigenvectors.
     *        Each row of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    bool getEigenVectorsInRows(matrix<T>& result) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    serialLDA& copy(const serialLDA& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Computes the between-class scatter matrix Sb.
     */
//     bool computeSb(const matrix<T>& src, matrix<T>& Sb,
//                    const ivector& labels) const;

    bool read(ioHandler &handler, const bool complete=true);
    bool write(ioHandler &handler, const bool complete=true) const;

    int getUsedDimension() const {
      return usedDimensionality;
    }

    bool getTransformMatrix(matrix<T>& result) const;
    const matrix<T>& getTransformMatrix() const;
    const matrix<T>& getEigenVectors() const;

  protected:
    /*
    void find(const ivector &data, int value, ivector &result) const;

    void submatrix(const matrix<T>& data,const ivector &index,
                   matrix<T>& result) const;
    */

    /**
     * Determines the intrinsic dimensionality of the data set if the
     * user specify autoDim, otherwise return parameters::resultDim.
     * The member usedDimensionality will be set with the returned value
     */
    int checkDim();

    bool buildTransform();

  private:
    // number of classes considered so far
    int classes;
    // data points considered so far
    int elements;

    // the sum of class covariance matrices so far
    matrix<T> Sw;

    // the between-class covariance matrix so far
    matrix<T> Sb;

    // the sum of all data vectors so far
    vector<T> sumOfElements;

    // the mean vectors of all classes so far
    std::vector< vector<T> > classMeans;
    // the cardinality of all classes so far
    std::vector<int> classCounts;

    // is true if the eigenvectors are valid for all the accumulated data
    bool isValid;

    // the current LDA result, it is computed when the first
    // apply is called.
    matrix<T> orderedEigVec;
    matrix<T> transformMatrix;
    vector<T> eigValues;

    // functors for doing the computations
    matrixInversion<T> inv;
    varianceFunctor<T> var;
    varianceFunctor<T> mlvar;
    meansFunctor<T> mean;
    generalEigenVectors<T> eig;

    /**
     * dimensionality being used.
     */
    int usedDimensionality;
  };


  template <class T>
  bool read(ioHandler& handler, serialLDA<T>& pca, const bool complete=true) {
    return pca.read(handler,complete);
  }


  template <class T>
  bool write(ioHandler& handler, const serialLDA<T>& pca, const bool complete=true) {
    return pca.write(handler,complete);
  }

  template <class T>
    bool read(ioHandler& handler, typename serialLDA<T>::parameters& p, const bool complete=true) {
    return p.read(handler,complete);
  }

  template <class T>
    bool write(ioHandler& handler, typename serialLDA<T>::parameters& p, const bool complete=true) {
    return p.write(handler,complete);
  }


}

#endif
#endif
