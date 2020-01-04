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
 * file .......: ltiSvm.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.2001
 * revisions ..: $Id: ltiSVM.h,v 1.18 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_SVM_H_
#define _LTI_SVM_H_

#include "ltiSupervisedInstanceClassifier.h"
#include "ltiObjectFactory.h"
#include "ltiKernelFunctor.h"
#include "ltiLinearKernel.h"
#include "ltiClassName.h"

namespace lti {
  /**
   * Implements a support vector machine (SVM). The SVM is trained
   * using the SMO algorithm (sequential minimal optimization) by
   * J.C. Platt (presented at NIPS 1999).
   *
   * The kernel function is determined by an %object of type
   * kernelFunctor<double>. svm contains an inner class called
   * linearKernel which is the default. If your application requires another
   * kernel function, you have to find one in another place.
   */
  class svm : public supervisedInstanceClassifier {
  public:

    /**
     * the parameters for the class svm
     */
    class parameters : public supervisedInstanceClassifier::parameters {
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
      virtual ~parameters();

      /**
       * returns name of this type.
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual classifier::parameters* clone() const;

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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * The number of support vectors that must be used.
       *
       * Default value: 0
       */
      int nSupport;

      /**
       * The "C" parameter. The meaning is explained in the standard SVM
       * literature; the boiled down version is that C controls the
       * generalization ability of the SVM. The larger C, the more
       * accurate the recognition on the training set. On the other hand,
       * the generalization performance is better with a smaller C.
       *
       * Default value: 1
       */
      double C;

      /**
       * The kernel function.  Try to use the kernel setting methods in order
       * to ensure a consistent memory managment of the pointed instance.
       * @see setKernel(),attachKernel(),useExternalKernel()
       *
       * Default value: "linearKernel"
       */
      kernelFunctor<double>* kernel;

      /**
       * The initial threshold of the SVM.
       *
       * Default value: 1
       */
      double bias;

      /**
       * The tolerance for detecting a violation of the KKT conditions.
       *
       * Default value: 1e-3
       */
      double tolerance;

      /**
       * Epsilon for detecting change in alpha value.
       *
       * Default value: 1e-12
       */
      double epsilon;

      /**
       * Is true if the result should be normalized to sum 1
       *
       * Default value: false
       */
      bool sumToOne;

      /*
       * This parameter value is not needed anymore: If a kernel
       * vector is to be used, it has to be passed to the train
       * method anyway, so we can always know for sure, if there
       * is a single kernel type or a kernel for each class.
       *
       * If this flag is true, the kernel given in this object is
       * ignored; instead, the caller must provide with the
       * method lti::svm::buildKernelVector a map, which assigns to each
       * class id a specific kernel function.
       * If false, just one kernel for all used SVM will be used.
       *
       * \warning The kernel vector functionality is not ready yet, so
       * use always false here!
       *
       * Default value: false
       *
      bool useKernelVector;
      */

      /**
       * If this flag is true, the training data are normalized to
       * mean zero and variance one before training the svm.
       * This means that a copy of the training data is required.
       * The transformation is stored and also used for testing.
       * However, it is essential for SVM training that the data have
       * low magnitudes. If the data have a variance of 100, the
       * training simply may fail.
       * The default value is false.
       */
      bool normalizeData;

      /**
       * The normal SVM algorithm uses a one-against-all scheme for
       * multi-class classification. However, as is pointed out in
       * ..., this is suboptimal in some cases. Therefore, you can
       * use a combination of one-against-one classifiers (n*(n-1)/2
       * for n classes).
       * Default: false.
       */
      bool usePairwise;

    private:
      /**
       * array of objects needed by the functor factory
       */
      static const kernelFunctor<double>* kernelArray[];

      /**
       * kernel functor factory
       */
      static objectFactory<kernelFunctor<double> > kfa;

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
    svm();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    svm(const svm& other);

    /**
     * destructor
     */
    virtual ~svm();

    /**
     * returns the name of this type ("svm")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" clustering.
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    svm& copy(const svm& other);

    /**
     * alias for copy member
     * @param other the clustering to be copied
     * @return a reference to this clustering object
     */
    svm& operator=(const svm& other);

    /**
     * returns a pointer to a clone of this clustering.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Supervised training.  The vectors in the <code>input</code> matrix
     * are arranged row-wise, i.e. each row contains one data vector.
     * The <code>ids</code> vector contains the class label for each row.
     * This functor implements the
     * <a href="http://www.research.microsoft.com/~jplatt/smo.html">SMO algorithm</a> by
     * <a href="http://www.research.microsoft.com/~jplatt">J.C. Platt</a>.
     * All classes are modeled using the same kernel function, given
     * in the parameters object.
     *
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids);

    /**
     * Supervised training.  The vectors in the <code>input</code> matrix
     * are arranged row-wise, i.e. each row contains one data vector.
     * The <code>ids</code> vector contains the class label for each row.
     * This functor implements the
     * <a href="http://www.research.microsoft.com/~jplatt/smo.html">SMO algorithm</a> by
     * <a href="http://www.research.microsoft.com/~jplatt">J.C. Platt</a>.
     * Each class is modeled using a separate kernel function given by
     * the <code>kernels</code> map. This map consists of assignments
     * of each class label to a kernelFunctor object. Care must be taken
     * that the kernel functor object ist not deallocated while the
     * SVM functor is in use.
     *
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @param ids vector of class ids for each input point
     * @param kernels map of kernel functions. Each functor uses
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids,
                       const std::map<int,kernelFunctor<double>*>& kernels);

    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.

    /*
     * Classification.
     * Classifies the feature and returns the output %object with
     * the classification result.
     * @param feature pattern to be classified
     * @return result of the classifications as a classifier::output
     */
/*      virtual const output& classify(const dvector& feature); */


    /**
     * Classification.
     * Classifies the feature and returns the outputVector %object with
     * the classification result.
     * <p><b>NOTE:</b> This method is NOT really const. Although the main
     * members of the svm are not changed some state variables used for
     * efficiency are. Thus, it is not save to use the same instance of the
     * svm in two different threads.
     * @param feature pattern to be classified
     * @param result of the classifications as a classifier::outputVector
     * @return true if the classification has been successful
     */
    virtual bool classify(const dvector& feature, outputVector& result) const;

    //void classify(const dvector& feature, dvector& result) const;

    /**
     * Returns the alpha matrix of the classifier. The matrix contains
     * one row for each class. Each row contains the alpha values of a
     * SVM that distinguishes this class from all other classes.
     */
    void getAlphas(dmatrix& result) const {
      result=alpha;
    }

    /**
     * Returns the bias vector of the classifier. The vector contains
     * the bias value for each class. Each element is the bias value
     * of a SVM that distinguishes this class from all other classes.  
     */
    void getBiases(dvector& result) const {
      result=bias;
    }

    /**
     * Sets a new kernel function. The kernel which is passed here will be
     * copied, and the parameters instance will control the memory
     * management.
     */
    void setKernel(const kernelFunctor<double>& k);

    /**
     * Sets a new kernel function. The kernel which is passed here as an
     * argument must have been allocated with new; it must not be
     * a local variable. On destruction of the parameters object,
     * the kernel will be deleted, i.e. the parameters instance will take
     * control over the memory management of the kernel functor.
     */
    void attachKernel(kernelFunctor<double>* k);

    /**
     * Sets a new kernel function. The kernel which is passed here as an
     * argument is not deleted by the parameters object, the caller
     * must ensure that there are no memory leaks.
     */
    void useExternalKernel(kernelFunctor<double>* k);


    /**
     * write the classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

  protected:
    void buildIdMaps(const ivector& ids);

    void buildKernelVector(const std::map<int,kernelFunctor<double>* >& ks);

    void buildKernelVector();

    virtual bool genericTrain(const dmatrix& input,
                              const ivector& ids);

    /**
     * Copy the given ids to srcIds and build the target matrix.
     */
    void makeTargets(const ivector& ids);

    /**
     * Rebuild the target matrix from srcIds.
     */
    void rebuildTargets();

    ivector srcIds;

  private:

    bool takeStep(const int& i1, const int& i2);

    void updateBiasError(const double& deltab);

    void setAlpha(const int &k, const double& a);

    bool examineExample(const int& i2);

    double objectiveFunction();

    inline double outputFunction(const dvector& x,
                                 const int& currentClass ) const;

    void fillErrorCache();

    void defineOutputTemplate();

    bool genericNormTrain(const dmatrix& input, const ivector& ids);


    // the number of classes
    int nClasses;

    // the alpha matrix, one for each class
    // each row contains the alpha values for the corresponding
    // class.
    dmatrix alpha;

    // the target values for each training vector
    // each row contains the target vector for one class
    const dmatrix* target;

    // this matrix contains all training vectors.
    // each row contains one vector
    const dmatrix* trainData;

    // parameters, can be taken from parameter object
    double epsilon;

    // system epsilon, i.e. the smallest number that can be added to zero
    // and returns something different than zero for the type double.
    const double syseps;

    // SVM constant C that controls how strong the patterns must
    // be separable to allow a successful training.  If C is very large,
    // the training patterns must be linear separable in the feature space.
    double C;

    double tolerance;

    // is modified during training, has to be saved
    dvector bias;

    // this vector contains one kernel functor per class
    std::vector<kernelFunctor<double>*> kernels;

    // map from external id to internal id, used for training
    std::map<int,int> idMap;
    // map from internal to external id, used for training
    std::map<int,int> rIdMap;

    // this is a temporary variable during training
    dvector errorCache;

    // the sum of the elements of the errorCache
    double errorSum;

    // temporary variables for training the SVM
    double y2,e2,alph2;

    // temporary variables for training
    int currentClass;
    dvector* currentAlpha;
    const dvector* currentTarget;


    // the transformation for normalizing the data
    dvector offset;
    //dvector scale;
    double scale;
  };

  // --------------------------------------
  // inline - methods implementation
  // --------------------------------------

  inline double svm::outputFunction(const dvector& x,
                                    const int& currentClass ) const {

    double u=0;
    const dvector* currentAlpha=&alpha.getRow(currentClass);
    const dvector* currentTarget=&target->getRow(currentClass);
    for (int i=0; i<currentAlpha->size(); i++) {
      if (currentAlpha->at(i) > 0) {
        u+=currentAlpha->at(i)*currentTarget->at(i)*kernels[currentClass]->apply(trainData->getRow(i),x);
      }
    }
    u-=bias[currentClass];
    return u;
  }

}

#endif

