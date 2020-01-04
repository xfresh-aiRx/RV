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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiMLP.h
 * authors ....: Bastian Ibach, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 13.8.2002
 * revisions ..: $Id: ltiMLP.h,v 1.12 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_M_L_P_H_
#define _LTI_M_L_P_H_

#include "ltiSupervisedInstanceClassifier.h"
#include "ltiVector.h"
#include "ltiObjectFactory.h"
#include <vector>

namespace lti {

  /**
   * Multi-layer perceptrons
   *
   * This class implements multi-layer neural networks using different
   * training methods.
   *
   * A number of layers between 1 and 3 is allowed.
   *
   * Training methods implemented at this time are:
   * - generalized delta-rule (steepest descent) with and without momentum,
   * - conjugate gradients.
   *
   * Following example shows how to use this sort of classifier:
   *
   * \code
   *    double inData[] = {-1,-1,
   *                       -1, 0,
   *                       -1,+1,
   *                       +0,+1,
   *                       +1,+1,
   *                       +1,+0,
   *                       +1,-1,
   *                       +0,-1,
   *                       +0,+0};
   *    lti::dmatrix inputs(9,2,inData);          // training vectors
   *
   *    int idsData[] = {1,0,1,0,1,0,1,0,1}; // and the respective ids
   *    lti::ivector ids(9,idsData);
   *
   *    lti::MLP ann;  // our artificial neural network
   *
   *    lti::MLP::parameters param;
   *    lti::MLP::sigmoidFunctor sigmoid(1);
   *    param.setLayers(4,sigmoid);  // two layers and four hidden units.
   *    param.trainingMode = lti::MLP::parameters::ConjugateGradients;
   *    param.maxNumberOfEpochs=200;
   *    ann.setParameters(param);
   *
   *    // we want to see some info while training
   *    streamProgressInfo prog(std::cout);
   *    ann.setProgressObject(prog);
   *
   *    // train the network
   *    ann.train(inputs,ids);
   *
   *    // let us save our network for future use
   *    // in the file called mlp.dat
   *    std::ofstream out("mlp.dat");
   *    lti::lispStreamHandler lsh(out);
   *
   *    // save the network
   *    ann.write(lsh);
   *    // close the file
   *    out.close();
   *
   *    // show some results with the same training set:
   *
   *    lti::MLP::outputVector outv; // here we will get some
   *                                 // classification results
   *    cout << endl << "Results: " << endl;
   *
   *    int i,id;
   *    for (i=0;i<inputs.rows();++i) {
   *      ann.classify(inputs.getRow(i),outv);
   *      cout << "Input " << inputs.getRow(i) << " \tOutput: ";
   *      outv.getId(outv.getWinnerUnit(),id);
   *      cout << id;
   *      if (id != ids.at(i)) {
   *        cout << " <- should be " << ids.at(i);
   *      }
   *      cout << endl;
   *    }
   * \endcode
   *
   * Better display for the classification of 2D problems can be
   * generated using the functor lti::classifier2DVisualizer.
   *
   */
  class MLP : public supervisedInstanceClassifier {
  public:

    // ----------------------------------------------
    // activation functor
    // ----------------------------------------------

    /**
     * Parent class for all activation function functors
     */
    class activationFunctor : public object {
    public:
      /**
       * the functor operator.  Operates on place, applying the function
       * to each element of the vector
       */
      virtual bool apply(dvector& output) const = 0;

      /**
       * the functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const =  0;

      /**
       * the derivative of the functor
       */
      virtual bool deriv(dvector& output) const = 0;

      /**
       * return a copy of this functor
       */
      virtual activationFunctor* clone() const = 0;

      /**
       * return value used to represent "true" or "on"
       */
      virtual const double& onValue() const = 0;

      /**
       * return value used to represent "false" or "off"
       */
      virtual const double& offValue() const = 0;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const {
        bool b = true;
        if (complete) {
          b = b && handler.writeBegin();
          b = b && handler.writeEnd();
        }
        return b;
      }

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        bool b = true;
        if (complete) {
          b = b && handler.readBegin();
          b = b && handler.readEnd();
        }
        return b;
      }
    };

    /**
     * a linear activation function
     */
    class linearActFunctor : public activationFunctor {
    public:
      /**
       * the functor operator
       */
      virtual bool apply(dvector& output) const {return true;};

      /**
       * the functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const {
        output.copy(src);
        return true;
      };

      /**
       * the derivative of the functor
       */
      virtual bool deriv(dvector& output) const {
        output.fill(1.0);
        return true;
      };

      /**
       * return value used to represent "true" or "on"
       */
      virtual const double& onValue() const {
        static const double theOnValue = +1.0;
        return theOnValue;
      };

      /**
       * return value used to represent "false" or "off"
       */
      virtual const double& offValue() const {
        static const double theOffValue = -1.0;
        return theOffValue;
      };

      /**
       * return a copy of this functor
       */
      virtual activationFunctor* clone() const {return new linearActFunctor;};
    };

    /**
     * a sign activation function (1.0 if input 0 or positive, -1.0 otherwise)
     */
    class signFunctor : public activationFunctor {
    public:
      /**
       * the functor operator
       */
      virtual bool apply(dvector& output) const {
        dvector::iterator it,eit;
        for (it=output.begin(),eit=output.end();it!=eit;++it) {
          (*it) = (*it) >= 0 ? 1.0 : -1.0;
        }
        return true;
      };

      /**
       * the functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const {
        dvector::const_iterator it,eit;
        dvector::iterator oit;
        output.resize(src.size(),0.0,false,false);
        for (it=src.begin(),eit=src.end(),oit=output.begin();
             it!=eit;
             ++it,++oit) {
          (*oit) = (*it) >= 0 ? 1.0 : -1.0;
        }
        return true;
      };

      /**
       * the derivative of the functor (will return 1.0 to allow learning)
       */
      virtual bool deriv(dvector& output) const {
        output.fill(1.0);
        return true;
      };

      /**
       * return a copy of this functor
       */
      virtual activationFunctor* clone() const {return new signFunctor;};

      /**
       * return value used to represent "true" or "on"
       */
      virtual const double& onValue() const {
        static const double theOnValue = +1.0;
        return theOnValue;
      };

      /**
       * return value used to represent "false" or "off"
       */
      virtual const double& offValue() const {
        static const double theOffValue = -1.0;
        return theOffValue;
      };
    };

    /**
     * a sigmoid activation function
     */
    class sigmoidFunctor : public activationFunctor {
    protected:
      double slope;
    public:
      /**
       * constructor using explicit sigmoid slope
       */
      sigmoidFunctor(const double& theSlope):
        slope(theSlope) {};

      /**
       * constructor of a sigmoid with slope 1.0
       */
      sigmoidFunctor() : slope(1.0) {};

      /**
       * the functor operator
       */
      virtual bool apply(dvector& output) const {
        dvector::iterator it,eit;
        for (it=output.begin(),eit=output.end();it!=eit;++it) {
          (*it) = 1.0/(1.0+exp(-(*it)*slope));
        }
        return true;
      };

      /**
       * the functor operator.  Operates on copy
       */
      virtual bool apply(const dvector& src, dvector& output) const {
        dvector::const_iterator it,eit;
        dvector::iterator oit;
        output.resize(src.size(),0.0,false,false);
        for (it=src.begin(),eit=src.end(),oit=output.begin();
             it!=eit;
             ++it,++oit) {
          (*oit) = 1.0/(1.0+exp(-(*it)*slope));

        }
        return true;
      };

      /**
       * the derivative of the functor
       */
      virtual bool deriv(dvector& output) const {
        dvector::iterator it,eit;
        for (it=output.begin(),eit=output.end();it!=eit;++it) {
          (*it) = 1.0/(1.0+exp(-(*it)*slope));
          (*it) = (*it)*(1.0-(*it))*slope;
        }
        return true;
      };

      /**
       * return a copy of this functor
       */
      virtual activationFunctor* clone() const {
        return new sigmoidFunctor(slope);
      };

      /**
       * return value used to represent "true" or "on"
       */
      virtual const double& onValue() const {
        static const double theOnValue = +1.0;
        return theOnValue;
      };

      /**
       * return value used to represent "false" or "off"
       */
      virtual const double& offValue() const {
        static const double theOffValue = 0.0;
        return theOffValue;
      };

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const {
        bool b = true;
        if (complete) {
          b = b && handler.writeBegin();
        }
        b = b && lti::write(handler,"slope",slope);
        if (complete) {
          b = b && handler.writeEnd();
        }
        return b;
      }

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        bool b = true;
        if (complete) {
          b = b && handler.readBegin();
        }
        b = b && lti::read(handler,"slope",slope);
        if (complete) {
          b = b && handler.readEnd();
        }
        return b;
      }
    };

    /**
     * the parameters for the class MLP
     */
    class parameters : public supervisedInstanceClassifier::parameters {
    private:
      /**
       * activations functors
       */
      static const activationFunctor *const  activationFunctors[];

      /**
       * factory to create activation functors
       */
      static objectFactory<activationFunctor> objFactory;

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
       * initialize the parameters to create a MLP with two layers
       * with the given number of hidden units (the number of
       * input and output units is determined in the training stage).
       *
       * @param hidden number of hidden units
       * @param activ activation function to be used in all units
       *
       */
      bool setLayers(const int hidden,
                     const activationFunctor& activ);

      /**
       * initialize the parameters to create a MLP with one sigle layer
       * (the number of input and output units is determined in the training 
       * stage).
       *
       * @param activ activation function to be used in all units
       */
      bool setLayers(const activationFunctor& activ);

      /**
       * set the activation functor for a given layer.  The current number of
       * layers is determined by the size of the \a hiddenUnits attribute.
       *
       * @param layer number of layer
       * @param aFct activationFunctor
       * @return true if successful, or false otherwise (usually, the
       * layer number is invalid
       */
      bool setLayerActivation(const int layer, const activationFunctor& aFct);


      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      enum eTrainingType {
        SteepestDescent, /**< Generalized Delta-Rule.  Just use the gradient
                          *   and the learnrate
                          */
        ConjugateGradients /**< Conjugate Gradients */
      };

      /**
       * Traning mode to be used.
       *
       * Default value: ConjugateGradients
       */
      eTrainingType trainingMode;

      /**
       * If true, an epoch (all the training data) will be presented before a
       * weigh adaption is taken.  Otherwise just training point is considered
       * to adapt the weights.  For the conjugate gradient method this mode
       * is ignored (assumed true).
       *
       * Default value: true
       */
      bool batchMode;

      /**
       * Value for the momentum used in the steepest descent methods.
       * Should be between 0.0 and 1.0.
       *
       * Default value: 0.0 (no momentum)
       */
      double momentum;

      /**
       * Number of units in the hidden layers.
       *
       * The size of this vector determines indirectly the number of
       * layers for the network.  It corresponds to is the number of
       * layers minus one, which means that the total number of layers
       * for the network is equal to the size of this vector plus one.
       *
       * Default value: [4] (i.e. a 2 layer network with 4 units in the hidden
       *                     layer)
       */
      ivector hiddenUnits;

      /**
       * Learning rate for the steepest descent method.
       */
      float learnrate;

      /**
       * Maximal number of epochs (number of presentations of the entire
       * training set)
       *
       * Default: 500
       */
      int maxNumberOfEpochs;

      /**
       * If this error value (or lower) is reached, the training is stopped.
       * For the conjugate gradients method, the algorithm is stopped if
       * the magnitude of the gradient is smaller than this value multiplied
       * by the magnitude of the initial gradient.
       *
       * Default value: 0.005
       */
      double stopError;

      /**
       * Activation functors (per layer).  The objects pointed by these
       * elements will be deleted when the parameters are delete.
       *
       * Default value: sigmoids
       */
      std::vector<activationFunctor*> activationFunctions;
    };

    /**
     * default constructor
     */
    MLP();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    MLP(const MLP& other);

    /**
     * destructor
     */
    virtual ~MLP();

    /**
     * returns the name of this type ("MLP")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    MLP& copy(const MLP& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    MLP& operator=(const MLP& other);

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Supervised training.
     * The vectors in the <code>input</code> matrix
     * must be trained using as "known" classes the values given in
     * <code>ids</code>.
     * @param input the matrix with input vectors (each row is a training
     *              vector)
     * @param ids the output classes ids for the input vectors.
     * @return true if successful, false otherwise.  (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids);

    /**
     * Supervised training.
     *
     * This method, used for debugging purposes mainly, initialize the
     * weights with the values given.
     *
     * The vectors in the \a input matrix must be trained using as "known"
     * classes the values given in \a ids.
     *
     * @param weights this vector is used to initialize the weights.  Must
     *                be consistent with the parameters.
     * @param input the matrix with input vectors (each row is a training
     *              vector)
     * @param ids the output classes ids for the input vectors.
     * @return true if successful, false otherwise.  (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dvector& weights,
                       const dmatrix& input,
                       const ivector& ids);

    //TODO Check whether you really need a new classify method.
    // In some cases the superclasses method will suffice. Then just
    // delete the declaration and its implementation stump.

    /**
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool classify(const dvector& feature, outputVector& result) const;

    /**
     * write the rbf classifier in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the rbf classifier from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /*
     * Undocumented function for debug purposes only.  It display the
     * internal weight matrices
     */
    bool prettyPrint();

    /**
     * Return a reference to the internal weights vector.
     * Used mainly for debugging purposes.
     */
    const dvector& getWeights() const;


    //TODO: comment the attributes of your classifier
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  private:

    // used variables

    /**
     * vector for saving the weights of the network
     */
    vector<double> weights;

    /**
     * indices for the begining of each layer in the weights vector
     */
    ivector layerIndex;

    /**
     * weights as matrix, to simplify propagation.
     * The matrices share the memory with weights.
     *
     * All matrices are initialized in initWeights()
     */
    std::vector< dmatrix > matWeights;

    /**
     * number of input units (determined by the size of the training data)
     */
    int inputs;

    /**
     * number of outputs (determined by training)
     */
    int outputs;

    /**
     * check how many outputs are required for the given
     * (external) ids, and set the outputs attribute.
     */
    void checkHowManyOutputs(const ivector& ids);


    /**
     * unitsOut, save output of each unit
     * each layer ist one element of the vector
     * first layer has index 0
     */
    mutable std::vector< dvector > unitsOut;

    /**
     * net value at each layer
     */
    mutable std::vector < dvector > unitsNet;

    /**
     * Value for "off" at the output layer (it depends on the activation
     * functor of the output layer.
     */
    double off;

    /**
     * Value for "on" at the output layer (it depends on the activation
     * functor of the output layer.
     */
    double on;

    /**
     * initialize weights with random values.
     * This method assumes that the attributes inputs and outputs are correct.
     *
     * @param keepWeightVals if true, the contents of weights won't be
     *                       changed (if they are incompatible with the
     *                       actual parameters, false will be returned).
     *                       if false, the values of the weights will be
     *                       initialized with random values between
     *                       "from" and "to".
     * @param from lower value for random values
     * @param to higher value for random values
     * @return true if successful, false otherwise.
     */
    bool initWeights(const bool keepWeightVals = false,
                     const double& from = -1.0,
                     const double& to = +1.0);


    /**
     * Given the weights vector, update the vector of matrices so that
     * each matrix uses its respective memory block in the vector.
     */
    void updateWeightIndices(dvector& theWeights,
                             std::vector<dmatrix>& theWMats) const;


    /**
     * accumulated error for one epoch
     */
    double totalError;

    /**
     * Error norm
     */
    double errorNorm;

    /**
     * calculate all outputs for all network units.  The result will
     * be left in the unitsOut attribute.
     *
     * @param input vector
     */
    bool propagate(const dvector& input) const;

    /**
     * calculate all outputs for all network units.
     *
     * @param input input vector
     * @param mWeights weights in matrix form
     * @param uNet preliminary results for each unit with the "net" values
     * @param uOut output for all units (f(net)).
     * @return true if successful, false otherwise.
     */
    bool propagate(const dvector& input,
                   const std::vector<dmatrix>& mWeights,
                   std::vector<dvector>& uNet,
                   std::vector<dvector>& uOut) const;


    /**
     * compute the error using the last propagated input and the given
     * pattern
     */
    bool computeActualError(const int id,double& error) const;

    /**
     * compute the error using the given output units vector
     */
    bool computePatternError(const int id,
                             const dvector& outUnits,
                             double& error) const;

    /**
     * compute the error of the given weights for the whole training set.
     */
    bool computeTotalError(const std::vector<dmatrix>& mWeights,
                           const dmatrix& inputs,
                           const ivector& ids,
                           double& totalError) const;

    /**
     * compute mat*vct' where vct' is a vector with one additional element
     * (1.0) at the beginning of vct.
     */
    bool biasMultiply(const dmatrix& mat,
                      const dvector& vct,
                      dvector& res) const;

    /**
     * calculate the negative gradient of error surface using
     * back-propagation algorithm
     *
     * @param input input vector
     * @param outputId desired output.  This value must be between 0 and
     *                 the number of output elements-1.
     * @param grad computed gradient of the error surface
     * @return true if successful, or false otherwise.
     */
    bool calcGradient(const dvector& input,
                      const int outputId,
                      dvector& grad);

    /**
     * calculate negative gradient of error surface using
     * back-propagation algorithm for all patterns in an epoch.
     *
     * @param input input vector
     * @param outputId desired output.  This value must be between 0 and
     *                 the number of output elements-1.
     * @param grad computed gradient of the error surface
     * @return true if successful, or false otherwise.
     */
    bool calcGradient(const dmatrix& inputs,
                      const ivector& ids,
                      dvector& grad);


    /**
     * train the network with steepest descent method (batch mode)
     * Weights must be initialized previously
     */
    bool trainSteepestBatch(const dmatrix& inputs,
                            const ivector& internalIds);


    /**
     * train the network with steepest descent method (sequential mode)
     * Weights must be initialized previously
     */
    bool trainSteepestSequential(const dmatrix& inputs,
                                 const ivector& internalIds);


    /**
     * train the network with steepest descent method (batch mode)
     * Weights must be initialized previously
     */
    bool trainConjugateGradients(const dmatrix& inputs,
                                 const ivector& internalIds);

    /**
     * compute the error norm, with which all displayed error values are
     * normalized.  This will allow to compare easily different classifiers.
     */
    bool computeErrorNorm(const ivector& internIds);

    /**
     * line search computes the eta scalar factor at which the error
     * is minimized.  It begins at the actual weight and follows the given
     * direction.
     */
    bool lineSearch(const dmatrix& inputs,
                    const ivector& ids,
                    const dvector& direction,
                    double& eta,
                    dvector& newWeights) const;
  };


  /**
   * write the functor::parameters in the given ioHandler.
   * The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool write(ioHandler& handler,const MLP::activationFunctor& p,
             const bool complete = true);

  /**
   * read the functor::parameters from the given ioHandler.
   * The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool read(ioHandler& handler,MLP::activationFunctor& p,
            const bool complete = true);

}

#endif

