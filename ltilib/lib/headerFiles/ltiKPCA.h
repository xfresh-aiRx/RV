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
 * file .......: ltiKPCA.h
 * authors ....: Pablo Alvarado Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiKPCA.h,v 1.10 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_K_P_C_A_H_
#define _LTI_K_P_C_A_H_

#include "ltiPCA.h"
#include "ltiEigenSystem.h"
#include "ltiIoHandler.h"
#include "ltiKernelFunctor.h"
#include "ltiObjectFactory.h"

namespace lti {
  /**
   * Functor for computing a kernel principal component analysis.
   *
   * It receives a set of input vectors in form of a matrix (each row of
   * the matrix corresponds to an input vector), which will be transformed
   * with KPCA.
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
   * The Kernel PCA is described in Schoelkopf, B., Smola, A. and
   * Mueller, K. "Nonlinear Component Analysis as a Kernel Eigenvalue
   * Problem", Neural Computation, vol. 10, no. 5, pp. 1299-1319
   */
  class kernelPCA: public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class principalComponents
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
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /**
       * Sets a new kernel function.  A copy of the kernel will be
       * done (so it is useless to change the parameters of the given
       * kernel instance, because the internal kernel will never
       * notice the changes done to its "parent").
       */
      void setKernel(const kernelFunctor<double>& k);

      /**
       * Sets a new kernel function. The kernel which is passed here as an
       * argument must have been allocated with new; it must not be
       * a local variable. On destruction of the parameters object,
       * the kernel will be deleted, i.e. this parameters instance will be
       * responsible for the memory managment of the object.
       */
      void attachKernel(kernelFunctor<double>* k);

      /**
       * Sets a new kernel function. The kernel which is passed here as an
       * argument is not deleted by the parameters object, the caller
       * must ensure that there are no memory leaks.
       */
      void useExternalKernel(kernelFunctor<double>* k);

      /**
       * create a new kernel function, using its class name. A pointer to the
       * kernel function is returned, so that the caller may modify
       * the kernel's parameters.
       */
      kernelFunctor<double>* createKernel(const std::string& name) const;

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * The kernel function.  Try to use the kernel setting methods in order
       * to ensure a consistent memory managment of the pointed instance.
       * @see setKernel(),attachKernel(),useExternalKernel()
       *
       * Default value: "linearKernel"
       */
      kernelFunctor<double>* kernel;

      /**
       * This is the maximal dimension of the reduced vectors.
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
       * The eigensystem functor used for computing eigenvectors.
       *
       * Default value: lti::jacobi<double>
       */
      eigenSystem<double> *eigen;

      /**
       * This flag determines if the functor should perform a
       * whitening transform of the data. Whitening means that
       * 1. A KPCA is performed
       * 2. A scaling of the transformed data by the inverse of the
       *    square root of the eigenvalues.
       *
       * The default is false.
       */
      bool whitening;


      /**
       * The factor which determines relevant eigenvectors. An eigenvector
       * is considered relevant if its eigenvalue is at least as large
       * as the largest eigenvalue divided by this number. Usually,
       * it takes values between 1e4 and 1e6.
       */
      double relevance;

    private:
      /**
       * array of objects needed by the functor factory
       */
      static const kernelFunctor<double>* kernelArray[];

      /**
       * kernel functor factory
       */
      static objectFactory< kernelFunctor<double> > kfa;

      /**
       * class name object used to detect the type name of an object instance.
       */
      className cn;

      /**
       * Flag used to inidicate if the local kernel functor must be deleted
       * by this object or not (just pointing to some external instance...)
       */
      bool destroyKernel;

    };

    /**
     * default constructor
     */
    kernelPCA();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    kernelPCA(const kernelPCA& other);

    /**
     * destructor
     */
    virtual ~kernelPCA();

    /**
     * returns the name of this type ("kernelPCA")
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
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the principal components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param data dmatrix with the source data.
     * @param result dmatrix with the result data.
     * @return true if the KPCA could be computed, false otherwise
     */
    virtual bool apply(const dmatrix& data, dmatrix& result);

    /**
     * On-Place version of the transformation.
     *
     * If you don't need to transform the input data, and just want to
     * use the input matrix to compute the principal components you
     * can use the method computeTransformMatrix().  If you just need
     * to transform the data, without computing the transformation matrix, you
     * can use the method transform().
     *
     * @param srcdest dmatrix with the source data, which will also contain
     *        the result.
     * @return a reference to <code>srcdest</code>.
     */
    virtual bool apply(dmatrix& srcdest);

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix. (this is an alias for the transform() method)
     */
    inline bool apply(const dvector &src, dvector& result) {
      return transform(src,result);
    }

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     * @param src the data vector
     * @param result the vector which will receive the transformed data
     * @return a reference to <code>result</code>
     */
    virtual bool transform(const dvector &src, dvector& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     * @param src the data matrix
     * @param result the matrix which will receive the transformed data
     * @return true if successful, false otherwise.
     */
    virtual bool transform(const dmatrix &src, dmatrix& result) const;

    /**
     * Transform an entire matrix according to a previously computed
     * transformation matrix. Unfortunately, we must choose a name
     * different from apply.
     * @param srcdest the data matrix.  The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool transform(dmatrix &srcdest) const;

    /**
     * Compute the transformation matrix.  Similar to the apply() method,
     * but it does not transform the given data (this saves some time).
     *
     * @param src the matrix with the input data to be analysed.
     * @return true if transformation matrix could be computed, false otherwise
     */
    virtual bool computeTransformMatrix(const dmatrix& src);

    /**
     * Alias for computeTransformMatrix()
     */
    virtual bool train(const dmatrix& src);

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     * @param result the vector which will receive the eigenvalues.
     * @return true if the values could be obtained, false otherwise.
     */
    virtual bool getEigenValues(dvector& result) const;

    /**
     * Returns the previously computed eigenvalues of the covariance
     * matrix.
     *
     * @return a const reference to the last computed eigenvalues
     */
    virtual const dvector& getEigenValues() const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.  Each ROW (as opposite to PCA) will contain the eigenvectors.
     *
     * @param result the matrix which will receive the eigenvectors.
     *        Each row of the matrix contains one eigenvector.
     * @return true if the vectors could be obtained, false otherwise
     */
    virtual bool getEigenVectors(dmatrix& result) const;

    /**
     * Returns the previously computed eigenvectors of the covariance
     * matrix.
     * @return a const reference to the last computed eigenvectors.
     *         Each row of the matrix contains one eigenvector.
     */
    virtual const dmatrix& getEigenVectors() const;

    /**
     * Set the dimension to which the vectors should be reduced.
     */
    virtual void setDimension(int k);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kernelPCA& copy(const kernelPCA& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
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
     * compute kernel matrix.  The data will be centered.
     */
    bool computeKernelMatrix(const dmatrix& src,dmatrix& kmat);

    /**
     * compute test kernel matrix.  The data will be centered.
     * It will be assumed that the source data (srcData) contains the
     * right data
     */
    bool computeTestKernelMatrix(const dmatrix& src,dmatrix& kmat) const;

    /**
     * compute test kernel vector.  The data will be centered.
     * It will be assumed that the source data (srcData) contains the
     * right data
     */
    bool computeTestKernelVector(const dvector& src,dvector& kvct) const;

  private:

    /**
     * Kernel PCA requires the original data set
     */
    dmatrix srcData;

    /**
     * Mean of rows of srcData
     */
    dvector Kunit;

    /**
     * Mean of columns of srcData
     */
    dvector unitK;

    /**
     * Mean of all values in the srcData
     */
    double unitKunit;

    /**
     * ordered set of eigenvectors (sorted with decreasing order of
     * eigenvalues).
     */
    dmatrix orderedEigVec;

    /**
     * the transformation matrix will contain just a subset of
     * the orderedEigVec matrix.
     */
    dmatrix transformMatrix;

    /**
     * ordered eigenvalues (in decreasing order)
     */
    dvector eigValues;

    /**
     * scaling factors for the whitening transformation
     */
    dvector whiteScale;

    /**
     * dimensionality being used.
     */
    int usedDimensionality;
  };

  bool read(ioHandler& handler,
            kernelPCA& kpca,
            const bool complete=true);


  bool write(ioHandler& handler,
             const kernelPCA& kpca,
             const bool complete=true);

}

#endif

