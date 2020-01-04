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
 * file .......: ltiPCA.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiPCA.h,v 1.43 2005/06/24 09:41:00 haehnel Exp $
 */

#ifndef _LTI_P_C_A_H_
#define _LTI_P_C_A_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiEigenSystem.h"
#include "ltiSVD.h"
#include "ltiIoHandler.h"

#ifdef HAVE_LAPACK
#include "ltiFastEigenSystem.h"
#include "ltiFastSVD.h"
#endif

namespace lti {
  /**
   * Functor for computing a principal component analysis.
   *
   * It receives a set of input vectors in form of a matrix (each row of
   * the matrix corresponds to an input vector), which will be transformed
   * with PCA.
   *
   * The first time you use the apply()-method, the transformation
   * matrix will be computed.  You can use this transformation matrix
   * with other data sets using the transform() methods.
   *
   * Please note that the eigenvector matrices will contain the
   * eigenvector in the COLUMNS and not in the rows, as could be
   * expected.  This avoids the requirement of transposing matrices in
   * the vector transformations (see also lti::eigenSystem<T>).
   *
   * For large data matrices is is advisable to use singular value
   * decomposition instead of an eigensystem for the PCA. The
   * operation will usually be faster and using less memory. To do so
   * set parameters::useSVD to true and set parameters::svd to
   * singularValueDecomp<T> or one of its subclasses (eg. fastSVD<T>
   * if you are using LAPACK)
   *
   * @ingroup gLinearAlgebra
   */
  template <class T>
  class principalComponents : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class principalComponents
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() 
        : linearAlgebraFunctor::parameters() {
        resultDim=3;
        autoDim=false;
        useCorrelation=false;
        whitening=false;
        relevance=T(100000);
        centerData=true;
        useSVD=false;
        svd=0;
#ifdef HAVE_LAPACK
        eigen=new fastEigenSystem<T>;
#else
        eigen=new jacobi<T>;
#endif
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {
        eigen = 0;
        svd=0;
        copy(other);
      }


      /**
       * destructor
       */
      ~parameters(){
        delete eigen;
      }

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "principalComponents::parameters";
      }

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
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        delete eigen;
        eigen = 0;
        delete svd;
        svd = 0;

        resultDim=other.resultDim;
        autoDim=other.autoDim;
        useCorrelation=other.useCorrelation;
        whitening=other.whitening;
        relevance=other.relevance;
        centerData=other.centerData;
        if (other.eigen!=0) {
          eigen=dynamic_cast<eigenSystem<T>*>(other.eigen->clone());
        } else {
          eigen = 0;
        }
        useSVD=other.useSVD;
        if (other.svd!=0) {
          svd=dynamic_cast<singularValueDecomp<T>*>(other.svd->clone());
        } else {
          svd = 0;
        }
        return *this;
      }


      /**
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other) {
        copy(other);
        return *this;
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          lti::read(handler,"dim",resultDim);
          lti::read(handler,"auto",autoDim);
          lti::read(handler,"corr",useCorrelation);
          lti::read(handler,"white",whitening);
          lti::read(handler, "relevance", relevance);
          lti::read(handler, "centerData", centerData);
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
          b=b && handler.readEnd();
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
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"dim",resultDim);
          lti::write(handler,"auto",autoDim);
          lti::write(handler,"corr",useCorrelation);
          lti::write(handler,"white",whitening);
          lti::write(handler, "relevance", relevance);
          lti::write(handler, "centerData", centerData);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::write(handler,false);
#       else
        bool
        (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif


      /**
       * This is the dimension of the reduced vectors.
       *
       * Default value: 3
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

      /**
       * This flag determines if the functor should use the
       * correlation coefficient matrix (flag is true) for eigenvector
       * computation or the covariance matrix (flag is false).
       *
       * The default is false.
       */
      bool useCorrelation;

      /**
       * This flag determines if the functor should perform a
       * whitening transform of the data. Whitening means that
       * 1. A PCA is performed, using the covariance matrix for
       *    eigenvector computation
       * 2. A scaling of the transformed data by the inverse of the
       *    square root of the eigenvalues.
       *
       * You should set useCorrelation to false if you use whitening.
       *
       * The default is false.
       */
      bool whitening;

      /**
       * The eigensystem functor used for computing eigenvectors.
       *
       * Default with LAPACK: lti::fastEigenSystem<T>
       * Default without LAPACK: lti::jacobi<T>
       */
      eigenSystem<T> *eigen;

      /**
       * The factor which determines relevant eigenvectors. An eigenvector
       * is considered relevant if its eigenvalue is at least as large
       * as the largest eigenvalue divided by this number. Usually,
       * it takes values between 1e4 and 1e6.
       *
       * Default: T(100000)
       */
      T relevance;

      /**
       * This flag denotes if the transformed data should be
       * centered around zero. This is the usual behaviour of
       * the PCA, but for some special operations it may be
       * necessary to disable this. If the flag is false,
       * the mean of the transformed data is moved to the transformed
       * mean of the source data.
       *
       * Default: true
       */
      bool centerData;

      /**
       * When true singular value decomposition instead of eigensystem
       * solution is used to calculate the eigenvectors and
       * eigenvalues. This can be much faster and less memory consuming.
       * Default is false.
       */
      bool useSVD;

      /**
       * Singular value decomposition to be used when useSVD is true.
       *
       * Default is null. If LAPACK is used and data matrices are
       * large consider using lti::fastSVD<T>.
       */
      singularValueDecomp<T> *svd;
    };

    /**
     * default constructor
     * @param createDefaultParams if true (default) a default parameters
     *                            object will be created.
     */
    principalComponents(const bool createDefaultParams = true);

    /**
     * default constructor with parameters
     * 
     */
    principalComponents(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    principalComponents(const principalComponents& other);

    /**
     * destructor
     */
    virtual ~principalComponents();

    /**
     * returns the name of this type ("principalComponents")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes the principal components of the data matrix
     * and transforms it according to the new coordinate system.
     * The result is the transformed matrix.
     * Data and result must not be references to the same matrix.
     * Data points are expected to be in the rows of the data matrix.
     *
     * This method uses the eigenvector functor given in the parameters.
     * By default, it uses ltilib's own jacobi functor. However, you can
     * speed it up considerably by using the eigensystem functor in
     * the lamath directory.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the principal components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param data matrix<T> with the source data.
     * @param result matrix<T> with the result data.
     * @return true if the PCA could be computed, false otherwise
     */
    virtual bool apply(const matrix<T>& data, matrix<T>& result);

    /**
     * On-Place version of the transformation.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the principal components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param srcdest matrix<T> with the source data, which will also contain
     *        the result.
     * @return a reference to <code>srcdest</code>.
     */
    virtual bool apply(matrix<T>& srcdest);

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix. (this is an alias for the transform() method)
     */
    virtual inline bool apply(const vector<T> &src, vector<T>& result) {
      return transform(src,result);
    }

    /**
     * Pass the covariance matrix and the mean values directly
     * to the functor to generate the transform matrix.
     *
     * If you know the mean and covariance of your data, you can use this
     * method to speed up the computations of the transformation matrix.
     * Otherwise, just call one of the apply() methods with your data vectors
     * in the rows of the matrix.  The covariance and mean vectors will be
     * computed there automatically.
     */
    bool setCovarianceAndMean(const matrix<T>& coVar,
                              const vector<T>& meanVec);

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     * @param src the data vector
     * @param result the vector which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    virtual bool transform(const vector<T> &src, vector<T>& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     * @param src the data matrix
     * @param result the matrix which will receive the transformed data
     * @return true if successful, false otherwise.
     */
    virtual bool transform(const matrix<T> &src, matrix<T>& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     * @param srcdest the data matrix.  The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool transform(matrix<T> &srcdest) const;

    /**
     * Compute the transformation matrix.  Similar to the apply() method,
     * but it does not transform the given data (this saves some time).
     *
     * @param src the matrix with the input data to be analysed.
     * @return true if transformation matrix could be computed, false otherwise
     */
    virtual bool computeTransformMatrix(const matrix<T>& src);

    /**
     * Alias for computeTransformMatrix()
     */
    virtual bool train(const matrix<T>& src);

    /**
     * Reconstructs a data vector \c dest from the given coefficients
     * \c coeff, using the transformMatrix found by
     * computeTransformMatrix() or apply() and the appropriate offset.
     * @param coeff PCA coefficients for reconstruction.
     * @param dest reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const vector<T>& coeff, vector<T>& dest) const;

    /**
     * Reconstructs a set of data vectors \c dest from the given
     * coefficients \c coeff, using the transformMatrix found by
     * computeTransformMatrix() or apply() and the appropriate
     * offset. As usual \c coeff as well as \c dest contain one data
     * vector per row.
     * @param coeff each row contains PCA coefficients for reconstruction.
     * @param dest each row is one reconstructed data vector.
     * @return true if reconstruction was successful
     */
    virtual bool reconstruct(const matrix<T>& coeff, matrix<T>& dest) const;

    /**
     * Returns the previously computed transform matrix.
     * @param result the matrix which will receive the transformation
     *        matrix.
     * @return true if the matrix could be computed, false otherwise.
     */
    virtual bool getTransformMatrix(matrix<T>& result) const;

    /**
     * Returns the previously computed transform matrix.
     * @return a const reference to the last computed or used transformation
     *           matrix.
     */
    virtual const matrix<T>& getTransformMatrix() const;

    /**
     * Returns the previously computed offset vector, which corresponds to the
     * mean of the data.
     *
     * @param result the offset vector will be written here.
     * @return true if the matrix could be computed, false otherwise.
     */
    virtual bool getOffsetVector(vector<T>& result) const;

    /**
     * Returns the previously computed offset vector, which corresponds to the
     * mean of the data.
     *
     * @return a const reference to the last computed or used offset vector.
     */
    virtual const vector<T>& getOffsetVector() const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     * @param result the vector which will receive the eigenvalues.
     * @return true if the values could be obtained, false otherwise.
     */
    virtual bool getEigenValues(vector<T>& result) const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvalues
     */
    virtual const vector<T>& getEigenValues() const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     * @param result the matrix which will receive the eigenvectors.
     *        Each column of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    virtual bool getEigenVectors(matrix<T>& result) const;

    /**
     * Returns the previously computed eigenvectors of the covariance
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
    virtual bool getEigenVectorsInRows(matrix<T>& result) const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     * @return a const reference to the last computed eigenvectors
     */
    virtual const matrix<T>& getEigenVectors() const;

    /**
     * Set the dimension to which the vectors should be reduced. This is just considered when 
     * computing the transformation matrix. After this matrix is determined the (destination) 
     * dimension is fixed and just can be changed by recalculating the transformation matrix.
     */
    virtual void setDimension(int k);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    principalComponents& copy(const principalComponents& other);

    /**
     * Alias for copy.
     */
    principalComponents& operator=(const principalComponents& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor's parameters.
     *
     * This member initializes some internal data according to the values in
     * the parameters.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Reads this functor from the given handler.
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * Writes this functor to the given handler.
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

    /**
     * Number of dimensions considered in the transformation
     * @return the number of dimensions used for the transformation.  It
     *         is always less or equal the number of dimensions of the input
     *         vectors.
     */
    int getUsedDimension() const {
      return usedDimensionality;
    }

  protected:
    /**
     * Determines the intrinsic dimensionality of the data set if the
     * user specify autoDim, otherwise return parameters::resultDim.
     * The member usedDimensionality will be set with the returned value
     */
    int checkDim();

    /**
     * Resets all private members to size 0. Used when an error occurs
     * in the calculation of the transform matrix.
     */
    void reset();

  protected:
    matrix<T> orderedEigVec;
    matrix<T> transformMatrix;
    vector<T> eigValues;

    vector<T> offset;
    vector<T> transformedOffset;
    vector<T> scale;
    vector<T> whiteScale;

    /**
     * dimensionality being used.
     */
    int usedDimensionality;
  };

  template <class T>
  bool read(ioHandler& handler,
            principalComponents<T>& pca,
            const bool complete=true) {
    return pca.read(handler,complete);
  }


  template <class T>
  bool write(ioHandler& handler,
             const principalComponents<T>& pca,
             const bool complete=true) {
    return pca.write(handler,complete);
  }

}

#endif


