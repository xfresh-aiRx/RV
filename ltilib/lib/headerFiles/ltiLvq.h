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
 * file .......: ltiRbf.h
 * authors ....: Peter Doerfler, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 11.8.2000
 * revisions ..: $Id: ltiLvq.h,v 1.13 2003/10/18 04:04:07 sltilib Exp $
 */

#ifndef _LTI_LVQ_H_
#define _LTI_LVQ_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiClassifier.h"
#include "ltiSupervisedInstanceClassifier.h"
#include "ltiStdLayer.h"

namespace lti {
  /**
   * LVQ-Net
   */
  class lvq : public supervisedInstanceClassifier {
  public:

    // --------------------------------------------------
    // lvq::parameters
    // --------------------------------------------------

    /**
     * the parameters for the class classifier
     */
    class parameters : public supervisedInstanceClassifier::parameters {
    public:

      /**
       * enumeration to specify the network initialization types.
       */
      enum eLvqInit {
        LvqRand,   /*!< random initialization of the vector code */
        LvqMaxDist /*!< initialization with the maximum distance */
      };

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
       * Alias for copy.
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      inline parameters& operator=(const parameters& other) {
        return copy(other);
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual classifier::parameters* clone() const;

      // --------------
      // the parameters
      // --------------

      /**
       * learn rate for LVQ1 or OLVQ1
       */
      double learnRate1;

      /**
       * learn rate for LVQ3 or OLVQ3
       */
      double learnRate2;

      /**
       * learn rate factor
       * used in LVQ3 and OLVQ3 to change the learn rate
       */
      double learnRateFactor;

      /**
       * window size
       */
      double windowSize;

      /**
       * number of neurons per class
       */
      int nbNeuronsPerClass;

      /**
       * number of presentations for LVQ1 or OLVQ1
       */
      int nbPresentations1;

      /**
       * number of presentations for LVQ3 or OLVQ3
       */
      int nbPresentations2;

      /**
       * the norm type (L1-, L2-norm)
       */
      eNormType norm;

      /**
       * specify the way the codebook vectors should be initialized
       */
      eLvqInit initType;

      /**
       * if true, the OLVQ1 algorithm will be used for the first presentation
       * set.  Otherwise the LVQ1 will be used.
       */
      bool flagOlvq1;

      /**
       * if true, the OLVQ3 algorithm will be used for the second presentation
       * set.  Otherwise the LVQ3 will be used.
       */
      bool flagOlvq3;

      /**
       * sigma factor
       */
      double sigmaFactor;

      /**
       * generate training statistics
       */
      bool doStatistics;

      /**
       * name of the file where the statistics will be saved
       */
      std::string statisticsFilename;

      /**
       * file name for the network
       */
      std::string netFilename;

      /**
       * has something to do with the statistics... (ask P. Doerfler)
       */
      bool doTrain2;

      /**
       * if true, the "best" network will be saved (see also correctVs3Best)
       */
      bool saveBest;

      /**
       * if saveBest is true, and correctVs3Best is also true, the best
       * net with the correct result will be saved.  If correctVs3Best is
       * false, the best "three-best" network will be saved.  If saveBest
       * is false, this parameter will be ignored.
       */
      bool correctVs3Best;
    };

    // --------------------------------------------------
    // lvq::layer
    // --------------------------------------------------

    /**
     * lvq layer
     */
    class layer : public stdLayer {
    public:
      /**
       * constructor
       */
      layer(const std::string& theName = "LVQ Layer");

      /**
       * destructor
       */
      virtual ~layer() {};

      /**
       * Find sigmas for weighting excitation of layer with exp()
       * @param nbNeurCl number of neurons per class
       * @param sigFac   sigma factor
       * @param sigma the resulting vector will be left here!
       */
      void findSigmas(const int& nbNeurCl,
                      const double& sigFac,
                      dvector& sigma);

    };

    // --------------------------------------------------
    // lvq::trainFunctor
    // --------------------------------------------------

    /**
     * base class of training functors for the lvq networks
     */
    class trainFunctor : public stdLayer::trainFunctor {
    public:
      /**
       * constructor
       * @param theProp the distance propagation functor
       */
      trainFunctor(const stdLayer::distancePropFunctor& theProp)
        : prop(theProp), firstUse(true) {};

      /**
       * set the learn rate factor
       */
      virtual void setLearnFactor(const double& lrFac);

      /**
       * set the window size
       */
      virtual void setWindowSize(const double& winSize);

    protected:
      /**
       * distance propagation functor
       */
      stdLayer::distancePropFunctor prop;

      /**
       * delta vector
       */
      dvector delta;

      /**
       * delta2 vector
       */
      dvector delta2;

      /**
       * calls vector
       */
      ivector calls;

      /**
       * lerning rate
       */
      double learnFactor;

      /**
       * window size
       */
      double windowSize;

      /**
       * first use of the functor
       */
      bool firstUse;

      /**
       * get the indices of the two minimum values of the vector
       * min1 <= min2
       */
      void twoMinIndex(const dvector& vct,int& min1,int& min2);

    };

    /**
     * LVQ1
     */
    class lvq1TrainFunctor : public trainFunctor {
    public:
      /**
       * constructor
       * @param theProp the distance propagation functor
       */
      lvq1TrainFunctor(stdLayer::distancePropFunctor& theProp)
        : trainFunctor(theProp) {};

      /**
       * apply operator
       */
      virtual bool operator()(const dvector& input,
                              dmatrix& weights,
                              dvector& outLayer,
                              const ivector& outID,
                              const int& trainID,
                              bool& modified);
    };

    /**
     * OLVQ1
     */
    class olvq1TrainFunctor : public trainFunctor {
    public:
      /**
       * constructor
       */
      olvq1TrainFunctor(stdLayer::distancePropFunctor& theProp)
        : trainFunctor(theProp) {};

      /**
       * set the learn rate
       */
      void setLearnRate(const double &theLr);

      /**
       * apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);

    protected:
      /**
       * learning rate vector
       */
      dvector lr;
    };

    /**
     * LVQ3
     */
    class lvq3TrainFunctor : public trainFunctor {
    public:
      /**
       * constructor
       */
      lvq3TrainFunctor(stdLayer::distancePropFunctor& theProp)
        : trainFunctor(theProp) {};

      /**
       * apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);


    };

    /**
     * OLVQ3
     */
    class olvq3TrainFunctor : public trainFunctor {
    public:
      /**
       * constructor
       */
      olvq3TrainFunctor(stdLayer::distancePropFunctor& theProp)
        : trainFunctor(theProp) {};

      /**
       * set the learn rate
       */
      void setLearnRate(const double &lRate);

      /**
       * apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);
    protected:
      /**
       * learning rate vector
       */
      dvector lr;

    };

    /**
     * LVQ4
     */
    class lvq4TrainFunctor : public trainFunctor {
    public:
      /**
       * constructor
       */
      lvq4TrainFunctor(stdLayer::distancePropFunctor& theProp)
        : trainFunctor(theProp) {};

      /**
       * apply operator
       */
      bool operator()(const dvector& input,
                      dmatrix& weights,
                      dvector& outLayer,
                      const ivector& outID,
                      const int& trainID,
                      bool& modified);
    };

    // --------------------------------------------------
    // lvq::initFunctor
    // --------------------------------------------------

    /**
     * random initialization
     */
    class randInitFunctor : public stdLayer::initFunctor {
    public:
      /**
       * LVQ ANNs are best initialized with the training data
       * therefore the somewhat bulky constructor
       */
      randInitFunctor(const dmatrix& theFeatures,
                      const ivector& theTrainIDs,
                      const ivector& theNbViewsObj,
                      const int& theNbObj,
                      const int& theNbNeurObj);

      bool operator()(dmatrix& weights, ivector& outID);

    protected:
      /**
       * reference matrix to the features
       */
      const dmatrix& features;

      /**
       * reference vector to the train IDs
       */
      const ivector& trainIDs;

      /**
       * number of views per object
       */
      const ivector& nbViewsObj;

      /**
       * number of objects
       */
      const int& nbObj;

      /**
       * number of neuron per object
       */
      const int& nbNeurObj;
    };

    /**
     * maximum distance initialization
     */
    class maxDistInitFunctor : public stdLayer::initFunctor {
    public:
      /**
       * LVQ ANNs are best initialized with the training data
       * therefore the somewhat bulky constructor
       */
      maxDistInitFunctor(const dmatrix& theFeatures,
                         const ivector& theTrainIDs,
                         const ivector& theNbViewsObj,
                         const int& theNbObj,
                         const int& theNbNeurObj,
                         const eNormType& theNorm);

      bool operator()(dmatrix& weights, ivector& outID);

    protected:
      /**
       * reference to the matrix with the feature vectors
       */
      const dmatrix& features;

      /**
       * vector with the train IDs
       */
      const ivector& trainIDs;

      /**
       * vector with the number of views per objectd
       */
      const ivector& nbViewsObj;

      /**
       * number of objects
       */
      const int& nbObj;

      /**
       * number of neurons per object
       */
      const int& nbNeurObj;

      /**
       * norm
       */
      const eNormType& norm;
    };

    // --------------------------------------------------
    // lvq
    // --------------------------------------------------

    /**
     * Constructor
     */
    lvq();

    /**
     * Copy constructor
     */
    lvq(const lvq& other);


    /**
     * Destructor
     */
    ~lvq();


    /**
     * Supervised training.  (LVQ)
     * The vectors in the <code>input</code> sequence
     * must be trained using as "known" classes the values given in
     * <code>ids</code>.
     * @param input the sequence of input vectors
     * @param ids the output classes ids for the input vectors.
     */
    bool train(const dmatrix& input,
               const ivector& ids);

    /**
     * Propagation.
     * Propagates feature vector through the net
     */
    bool classify(const dvector& featurem, outputVector& result) const;

    /**
     * dimension of the output layer
     */
    inline int getDimOutputLayer() const;

    /**
     * Gets the test-data for training statistics
     */
    void setTestSet(const dmatrix& input,
                    const ivector& ids);

    /**
     * Gets the disjunct training data for object probabilities
     */
    void setTrainSet2(const dmatrix& input,
                      const ivector& ids);

    /**
     * Calculate object probabilities with given features
     */
    void calcObjProbs(const ivector& ids,
                      const dmatrix& feats,
                      stdLayer::distancePropFunctor& distProp);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lvq& copy(const lvq& other);

    /**
     * Alias for copy.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    inline lvq& operator=(const lvq& other) {
      return copy(other);
    }

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


#ifdef _INCLUDE_DEPRECATED
    /**
     * Load network. Gets network information from a '.lvq' file
     */
//      bool load(std::istream& instream);

    /**
     * Save network. Saves relevant information in a '.lvq' file
     */
//      bool save(std::ostream& outstream);
#endif

  protected:

    /**
     * returns used parameters
     */
//  parameters& getParameters();

  private:
    /**
     * lvq layer functor
     */
    layer lvqLayer;

    /**
     * pointer to the used distance propagation functor
     */
    stdLayer::distancePropFunctor* distProp;

    /**
     * linear activation functor
     */
    layer::linearActFunctor linearAct;

    /**
     * pointer to the used gaussian activation functor instance
     */
    stdLayer::gaussActFunctor* gaussAct;

    /**
     * classification statistics functor for training data
     */
//      classifyStatFunctor trainStat;

    /**
     * classification statistics functor for test data
     */
//      classifyStatFunctor testStat;

    /**
     * input size
     */
    int sizeIn;

    /**
     * number of objects
     */
    int nbObj;

    /**
     * the code-book vectors
     */
    dmatrix features;

    /**
     * sigma vector
     */
    dvector sigma;

    /**
     * internal id to real id
     */
    ivector internToReal;

    /**
     * real to intern id
     */
    std::map<int,int> realToIntern;


    /**
     * vector with IDs
     */
    ivector trainID;

    /**
     * second set of training feature vectors
     */
    dmatrix train2Features;

    /**
     * IDs for the second training set
     */
    ivector train2IDs;


    /**
     * set of test feature vectors
     */
    dmatrix testFeatures;

    /**
     * IDs for the test feature set
     */
    ivector testIDs;

    /**
     * generate statistics for the given feature set
     * @param stat statistics functor
     * @param ids IDs for the feature vector set
     * @param feats feature vector set.  Each row of the matrix is a feature
     *              vector
     * @param save if true the statistics will be saved.
     */
//      void statistics(classifyStatFunctor& stat,
//                      const ivector& ids,
//                      const dmatrix& feats,
//                      const bool save);
  };

  /**
   * return the dimensions of the output layer
   */
  int lvq::getDimOutputLayer() const {
    return nbObj*getParameters().nbNeuronsPerClass;
  }
}

#endif //_LTI_LVQ_H_

