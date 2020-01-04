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
 * revisions ..: $Id: ltiSerialPCA.h,v 1.12 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SERIAL_PCA_H_
#define _LTI_SERIAL_PCA_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiEigenSystem.h"
#include "ltiIoHandler.h"
#include "ltiSerialVectorStats.h"

namespace lti {
  /**
   * Functor for sequentially computing a principal component analysis.
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
   * @ingroup gLinearAlgebra
   */
  template <class T>
  class serialPCA : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class serialPCA
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

        eigen=new jacobi<T>;
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {
        eigen = 0;
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
        return "serialPCA::parameters";
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

        resultDim=other.resultDim;
        autoDim=other.autoDim;
        useCorrelation=other.useCorrelation;
        whitening=other.whitening;
        eigen=dynamic_cast<eigenSystem<T>*>(other.eigen->clone());
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
       * The eigensystem functor used for computing eigenvectors
       */
      eigenSystem<T> *eigen;

    };

    /**
     * default constructor
     */
    serialPCA();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    serialPCA(const serialPCA& other);

    /**
     * destructor
     */
    virtual ~serialPCA();

    /**
     * returns the name of this type ("serialPCA")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes the principal components of the data matrix
     * and transforms it according to the new coordinate system.
     * The result is the transformed matrix.
     * data and result must not be references to the same matrix.
     * This method uses the eigenvector functor given in the parameters.
     * By default, it uses ltilib's own jacobi functor. However, you can
     * speed it up considerably by using the eigensystem functor in
     * the lamath directory.
     *
     * @param data matrix<T> with the source data.
     * @return true if the PCA could be computed, false otherwise
     */
    bool consider(const matrix<T>& data);

    /**
     * Computes the principal components of the previously
     * considered data points and applies it to the given data
     * matrix. The result is the transformed matrix.
     * data and result must not be references to the same matrix.
     * This method uses the eigenvector functor given in the parameters.
     * By default, it uses ltilib's own jacobi functor. However, you can
     * speed it up considerably by using the eigensystem functor in
     * the lamath directory.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> with the result data.
     * @return true if the PCA could be computed, false otherwise
     */
    bool apply(const matrix<T>& src, matrix<T>& dest);


    /**
     * On-Place version of the transformation.
     * @param srcdest matrix<T> with the source data, which will also contain
     *        the result.
     * @return a reference to <code>srcdest</code>.
     */
    bool apply(matrix<T>& srcdest);

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     */
    inline bool apply(const vector<T> &src, vector<T>& result) {
      return transform(src,result);
    }

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     * @param src the data vector
     * @param result the vector which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    bool transform(const vector<T> &src, vector<T>& result);

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     * @param src the data matrix
     * @param result the matrix which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    bool transform(const matrix<T> &src, matrix<T>& result);

    /**
     * Returns the previously computed transform matrix.
     * @param result the matrix which will receive the transformation
     *        matrix.
     * @return true if the matrix could be computed, false otherwise.
     */
    bool getTransformMatrix(matrix<T>& result) const;

    /**
     * Returns the previously computed transform matrix.
     * @return a const reference to the last computed or used transformation
     *           matrix.
     */
    const matrix<T>& getTransformMatrix() const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     * @param result the vector which will receive the eigenvalues.
     * @return true if the values could be obtained, false otherwise.
     */
    bool getEigenValues(vector<T>& result) const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvalues
     */
    const vector<T>& getEigenValues() const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     * @param result the matrix which will receive the eigenvectors.
     *        Each column of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    bool getEigenVectors(matrix<T>& result) const;

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
    bool getEigenVectorsInRows(matrix<T>& result) const;


    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     * @return a const reference to the last computed eigenvectors
     */
    const matrix<T>& getEigenVectors() const;

    /**
     * Set the dimension to which the vectors should be reduced.
     */
    void setDimension(int k);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    serialPCA& copy(const serialPCA& other);

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
     * Initialize some internal data according to the parameters.
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


  private:

    bool buildTransform();

    matrix<T> orderedEigVec;
    matrix<T> transformMatrix;
    vector<T> eigValues;

    vector<T> offset;
    vector<T> scale;
    vector<T> whiteScale;

    // this is used for computing
    serialVectorStats<T> vstat;

    bool isValid;

    /**
     * dimensionality being used.
     */
    int usedDimensionality;
  };

  template <class T>
  bool read(ioHandler& handler,
            serialPCA<T>& pca,
            const bool complete=true) {
    return pca.read(handler,complete);
  }


  template <class T>
  bool write(ioHandler& handler,
             const serialPCA<T>& pca,
             const bool complete=true) {
    return pca.write(handler,complete);
  }

}

#endif


