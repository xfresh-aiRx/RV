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
 * file .......: ltiStdLayer.h
 * authors ....: Peter Doerfler, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 11.8.2000
 * revisions ..: $Id: ltiStdLayer.h,v 1.13 2005/06/16 09:32:40 doerfler Exp $
 */


#ifndef _LTI_STD_LAYER_H_
#define _LTI_STD_LAYER_H_

#ifndef _INCLUDE_DEPRECATED
#define _INCLUDE_DEPRECATED
#endif

#include <string>
#include <iostream>

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiObject.h"

namespace lti {

  /**
   * used for flags indicating usage of L1 or L2 norm for
   * propagation
   */
  enum eNormType {
    L1distance, /*!< L1-norm (sum of the absolut values) */
    L2distance  /*!< L2-norm (square root of the sum of the squares)*/
  };

  /**
   * This abstract class implements a standard neural network layer.
   *
   * This class contains the weights matrix, the vector with the
   * output values, the class id for each output element, the
   * dimensionality of the input vector and output layer (which must
   * be consistent with the dimensions of the weights matrix and the
   * output vector), the name of the layer (an ASCII string) and a learn
   * rate value.
   *
   * The enclosed classes are supponsed to modify this values.  There are
   * some initialization, activation, propagation and training functors.
   */
  class stdLayer : public object {
  public:

    // ----------------------------------------------
    // initialization functor
    // ----------------------------------------------

    /**
     * This class and all it's derived classes should initialize
     * the network layer
     */
    class initFunctor : public object {
    public:
      /**
       * default constructor
       */
      initFunctor() 
        : object() {};

      /**
       * destructor
       */
      virtual ~initFunctor() {};

      /**
       * initialization operator
       */
      virtual bool operator()(dmatrix& weights,ivector& outID) = 0;

    protected:

      /**
       * Returns the unique IDs in a vector
       */
      ivector uniqueIDs(const ivector& allIDs) const;

      /**
       * Returns all train vectors of the given class
       * @param allVectors the training vectors are in the rows of this matrix
       * @param allIDs IDs for the training vectors
       * @param nbViews number of views per object
       * @param id the ID for which the features are returned
       */
      dmatrix classVectors(const dmatrix& allVectors,
                           const ivector& allIDs,
                           int nbViews,
                           int id) const;
    };

    // ----------------------------------------------
    // activation functor
    // ----------------------------------------------

    /**
     * Parent class for all activation function functors
     */
    class activationFunctor : public object {
    public:
      /**
       * the functor operator
       */
      virtual bool operator()(dvector& output) const = 0;
    };

    /**
     * a linear activation function
     */
    class linearActFunctor : public activationFunctor {
    public:
      /**
       * the functor operator
       */
      virtual bool operator()(dvector& output) const {return true;};
    };

    /**
     * a gaussian activation function
     */
    class gaussActFunctor : public activationFunctor {
    protected:
      double mu,sigma;
      const dvector* sigmaVec;

    public:
      /**
       * constructor for a gaussian activation function with
       * mean value theMu and std. deviation theSigma
       */
      gaussActFunctor(const double& theMu,
                      const double& theSigma)
        : mu(theMu),sigma(theSigma),sigmaVec(0) {};

      /**
       * constructor for a gaussian activation function with
       * mean value theMu and std. deviation vector theSigma
       */
      gaussActFunctor(const double& theMu,
                      const dvector& theSigmaVec)
        : mu(theMu),sigma(-1),sigmaVec(&theSigmaVec) {};


      /**
       * functor operator
       */
      bool operator() (dvector& output) const;
    };

    /**
     * a sigmoid activation function
     */
    class sigmoidActFunctor : public activationFunctor {
    protected:
      double slope;

    public:
      sigmoidActFunctor(const double& theSlope):
        slope(theSlope) {};

      sigmoidActFunctor() :
        slope(1.0) {};

      bool operator () (dvector& output) const;
    };

    // ----------------------------------------------
    // propagation functor
    // ----------------------------------------------
    /**
     * Parent class for all propagation functors
     * All propagation functors use a given activation function
     * to calculate the output layer vector using a given input
     * vector and a given weights matrix
     */
    class propagationFunctor : public object {
    public:

      /**
       * defines a linear activation function
       */
      propagationFunctor(const activationFunctor& theAct)
        : act(theAct) {};

      /**
       * the functor operator
       * @param input the input vector
       * @param weights the weights matrix of the layer
       * @param outLayer the output vector
       */
      virtual bool operator() (const dvector& input,
                               const dmatrix& weights,
                               dvector& outLayer) const = 0;
    protected:
      const activationFunctor& act;
    };

    /**
     * Distance propagation functor
     */
    class distancePropFunctor : public propagationFunctor {
    public:

      /**
       * Default constructor
       * For Propagation L1 and L2 norm can be used
       */
      distancePropFunctor(const activationFunctor& theAct,
                          eNormType theNorm = L2distance)
        : propagationFunctor(theAct),norm(theNorm) {};

      /**
       * The functor operator
       * @param input the input vector
       * @param weights the weights matrix of the layer
       * @param outLayer the output vector
       */
      bool operator() (const dvector& input,
                       const dmatrix& weights,
                             dvector& outLayer) const;
    private:
      eNormType norm;
    };

    /**
     * Dot-Product propagation functor
     */
    class dotPropFunctor : public propagationFunctor {
    public:
      /**
       * Default constructor
       */
      dotPropFunctor(const activationFunctor& theAct)
        : propagationFunctor(theAct) {};

      /**
       * The functor operator
       * @param input the input vector
       * @param weights the weights matrix of the layer
       * @param outLayer the output vector
       */
      bool operator() (const dvector& input,
                       const dmatrix& weights,
                             dvector& outLayer) const;
    };

    /**
     * training functor
     */
    class trainFunctor : public object {
    public:
      /**
       * default constructor
       * Both neccessary since there are nets which are trained
       * with an overall net error, not a layer error
       */
      trainFunctor() 
        : object() {};

      /**
       * destructor
       */
      virtual ~trainFunctor() {};

      /**
       * set the learn rate
       */
      virtual void setLearnRate(const double & lr);


      /**
       * the functor operator for supervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param outID the class identification for each output neuron
       * @param trainID the class ID for the presented vector
       * @param netError the error of the layer
       * @param modified indicates whether the weights were modified
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              const ivector& outID,
                              const int& trainID,
                              dvector& netError,
                              bool& modified) {
        return false; // throw exception("training functor not implemented");
      };

      /**
       * the functor operator for unsupervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param netError the error of the layer
       * @param modified indicates whether the weights were modified
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              dvector& netError,
                              bool& modified) {
        return false; // throw exception("training functor not implemented");
      };

      /**
       * the functor operator for supervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param outID the class identification for each output neuron
       * @param trainID the class ID for the presented vector
       * @param modified indicates whether the weights were modified
       * @return true if successful, false otherwise
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              const ivector& outID,
                              const int& trainID,
                              bool& modified) {
        return false; // throw exception("training functor not implemented");
      };

      /**
       * the functor operator for unsupervised training requires
       * following parameters:
       * @param input the input vector
       * @param weights the weights matrix to be changed
       * @param outLayer the output layer vector
       * @param modified indicates whether the weights were modified
       * @return true if successful, false otherwise
       */
      virtual bool operator ()(const dvector& input,
                               dmatrix& weights,
                               dvector& outLayer,
                               bool& modified) {
        return false; // throw exception("training functor not implemented");
      };

    protected:
      double learnRate;
    };

    // ----------------------------------------------
    // standard layer
    // ----------------------------------------------

    /**
     * default constructor
     */
    stdLayer(const std::string& theName);

    /**
     * copy constructor
     */
    stdLayer(const stdLayer& other);

    /**
     * destructor
     */
    virtual ~stdLayer();

    /**
     * copy member
     */
    stdLayer& copy(const stdLayer& other);

    /**
     * initialize the weights
     */
    virtual void initWeights(initFunctor& initFunc);

    /**
     * train the network layer (supervised) with the given input vector
     *
     * Does one step of training, i.e. one feature vector 'input' of class
     * 'trainID' is learned by the layer as specified in the trainFunctor
     *
     * @return false if an error occurred, otherwise true
     */
    virtual bool train(const dvector& input,
                       const int& trainID,
                       trainFunctor& trainFunc);

    /**
     * train the network layer (unsupervised) with the given input vector
     *
     * @return false if an error occurred, otherwise true
     */
    virtual bool train(const dvector& input,trainFunctor& trainFunc);

    /**
     * propagate the vector through the network
     *
     * @return false if an error occurred, otherwise true
     */
    virtual bool propagate(const dvector& input,
                           propagationFunctor& prop,
                           dvector& output) const;

    /**
     * get the output vector
     */
//      inline const dvector& getOutput() const;

    /**
     * get the class IDs for output vector elements
     */
    inline const ivector& getOutID() const;

    /**
     * set the size of the input and output vectors
     * @param in size of the input vector
     * @param out size of the output vector
     */
    void setSize(const int& in, const int& out);


    /**
     * write the standard layer in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the standard layer from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * return the last message set with setStatusString().  This will
     * never return 0.  If no status-string has been set yet an empty string
     * (pointer to a string with only the char(0)) will be returned.
     */
    virtual const char* getStatusString() const;

    /**
     * set a status string.
     *
     * @param msg the const string to be reported next time by
     * getStatusString().  The given string will be copied
     * This message will be usually set within the apply methods to indicate
     * an error cause.
     *
     * Note that the change of the status string is not considered as
     * a change in the functor status.
     */
    virtual void setStatusString(const char* msg) const;


    /**
     * get the weights of the layer
     */
    inline const dmatrix& getWeights();

    /**
     * set the learn rate
     */
    inline void setLearnRate(const double& theLR);


    /**
     * These methods should only be used by the Axiom Java interface
     */
    //@{

    /**
     * Set the weights of the layer to the given matrix
     */
    void setWeights(const dmatrix& weights);

    /**
     * Set the IDs of the layer to the given vector
     */
    void setIDs(const ivector& ids);

    //@}

#ifdef _INCLUDE_DEPRECATED


    /**
     * @name deprecated methods
     * Deprecated methods
     */
    //@{

    /**
     * write the standard layer in the given stream (ascii)
     * \deprecated Use write() instead
     */
    virtual bool save(std::ostream& out) const;

    /**
     * read the standard layer from the given stream (ascii)
     * \deprecated Use read() instead
     */
    virtual bool load(std::istream& in);

    /**
     * write the standard layer in the given stream (binary)
     * \deprecated Use write() instead
     */
    virtual bool saveBinary(std::ostream& out) const;

    /**
     * read the standard layer from the given stream (binary)
     * \deprecated Use read() instead
     */
    virtual bool loadBinary(std::istream& in);
    //@}
#endif

  protected:
    /**
     * weights matrix
     */
    dmatrix weights;

    /**
     * output layer
     */
//      dvector outLayer;

    /**
     * Class ID of output node
     */
    ivector outID;

    /**
     * dimensionality of the input vector
     */
    int sizeIn;

    /**
     * dimensionality of the output vector
     */
    int sizeOut;

    /**
     * name of a layer
     */
    std::string layerName;

    /**
     * learn rate
     */
    double learnRate;

    /**
     * the status string written with setStatusString
     */
    mutable char* statusString;

    /**
     * the empty string returned if the statusString is empty
     */
    static const char *const emptyString;
  };

  /**
   * @name Storable interface for classifiere classes
   *  Members for the storable interface
   */

  //@{
  /**
   * read the objectProb from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   */
  bool read(ioHandler& handler,
            stdLayer& p,
            const bool complete=true);

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   */
  bool write(ioHandler& handler,
             const stdLayer& p,
             const bool complete=true);
  //@}
  // --------------------------------------
  // inline - methods implementation
  // --------------------------------------

//    const dvector& stdLayer::getOutput() const {
//      return outLayer;
//    }

  const ivector& stdLayer::getOutID() const {
    return outID;
  }

  void stdLayer::setLearnRate(const double& theLR) {
    learnRate=theLR;
  }

  const dmatrix& stdLayer::getWeights() {
    return weights;
  }
}

#endif //_LTI_STD_LAYER_H_


