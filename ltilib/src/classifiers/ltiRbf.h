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
 * revisions ..: $Id: ltiRbf.h,v 1.27 2005/06/16 09:32:40 doerfler Exp $
 */


#ifndef _LTI_RBF_H_
#define _LTI_RBF_H_

#include "ltiObject.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiStdLayer.h"
#include "ltiClassifier.h"
#include "ltiSupervisedInstanceClassifier.h"
#include "ltiLvq.h"

namespace lti {

  /**
   * class which implements RBF networks.
   */
  class rbf : public supervisedInstanceClassifier {
  public:

    // ----------------------------------------------------
    // rbf::parameters
    // ----------------------------------------------------

    /**
     * parameters class for the RBF-Networks
     */
    class parameters: public supervisedInstanceClassifier::parameters {

    public:
      /**
       * type to specify the kind of initialization for the networks
       */
      enum eLvqInit {
        LvqRand,   /*!< random initialization of the vector code */
        LvqMaxDist /*!< initialization with the maximum distance */
      };

      /**
       * The LVQ training algorithm
       */
      enum eLvqTrainType {
        NO_LVQ=-1, /*!< do not use LVQ */
        LVQ1,      /*!< use LVQ1       */
        OLVQ1,     /*!< use OLVQ1      */
        LVQ3,      /*!< use LVQ3       */
        OLVQ3,     /*!< use OLVQ3      */
        LVQ4       /*!< use LVQ4       */
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
       * Alias for "copy".
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

      // --------------
      // the parameters
      // --------------

      /**
       * @name classifier parameters
       */
      //@{

      /**
       * trainNewNet
       *
       * Default value: false
       */
//        bool trainNewNet;

      /**
       * convertNet
       *
       * Default value: false
       */
//        bool convertNet;

      /**
       * doTrain2
       *
       * Default value: false
       */
      bool doTrain2;

      /**
       * loadNet
       *
       * Default value: true
       */
//        bool loadNet;

      /**
       * generate statistics?
       *
       * Default value: false
       */
//        bool doStatistics;

      /**
       * Save/Load in binary mode
       *
       * Default value: true
       */
//        bool binary;

      /**
       * Save/Load in ASCII mode
       *
       * Default value: false
       */
//        bool ascii;

      /**
       * convert binary file to ascii file
       *
       * Default value: true
       */
//        bool convertBinToAscii;

      /**
       * noObjectProbs
       *
       * Default value: false
       */
//        bool noObjectProbs;

      /**
       * classifyStat
       *
       * Default value: true
       */
//        bool classifyStat;

      /**
       * mseStat
       *
       * Default value: false
       */
//        bool mseStat;

      /**
       * mseSave
       *
       * Default value: false
       */
//        bool mseSave;

      /**
       * firstBestSave
       *
       * Default value: false
       */
//        bool firstBestSave;

      /**
       * threeBestSave
       *
       * Default value: true
       */
//        bool threeBestSave;

      /**
       * include the training set in the pattern set for the training
       * statistics
       *
       * Default value: false
       */
//        bool alsoTrain;

      /**
       * nbPresentations1
       *
       * Default value: 0
       */
      int nbPresentations1;

      /**
       * nbPresentations2
       *
       * Default value: 20
       */
      int nbPresentations2;

      /**
       * nbHiddenNeurons
       *
       * Default value: 1
       */
      int nbHiddenNeurons;

      /**
       * learnRate1
       *
       * Default value: 0.3
       */
      double learnRate1;

      /**
       * learnRate2
       *
       * Default value: 0.1
       */
      double learnRate2;

      /**
       * learnFactor
       *
       * Default value: 0.3
       */
      double learnFactor;

      /**
       * windowSize
       *
       * Default value: 0.2
       */
      double windowSize;

      /**
       * sigma
       *
       * Default value: 0.0
       */
      double sigma;

      /**
       * sigmaFactor
       *
       * Default value: 1.6
       */
      double sigmaFactor;

      /**
       * lambda
       *
       * Default value: 0.0
       */
      double lambda;

      /**
       * this factor determines the value of the sigmoid function that
       * will be used as threshold for a correct classification.
       *
       * Default value: 0.99
       */
      double high;

      /**
       * norm
       *
       * Default value: L2distance
       */
      eNormType norm;

      /**
       * lvqInitType
       *
       * Default value: rbf::parameters::LvqMaxDist
       */
      eLvqInit lvqInitType;

      /**
       * specify the type for the first LVQ training
       * (usually LVQ1 or OLVQ1)
       *
       * Default value: OLVQ1
       */
      eLvqTrainType lvqType1;

      /**
       * specify the type for the second LVQ training
       * (usually LVQ3 or OLVQ3)
       *
       * Default value: OLVQ3
       */
      eLvqTrainType lvqType2;

      /**
       * netTrainFile
       *
       * Default value: ""
       */
//        std::string netTrainFile;

      /**
       * netTrainFile2
       *
       * Default value: ""
       */
//        std::string netTrainFile2;

      /**
       * netTestFile
       *
       * Default value: ""
       */
//        std::string netTestFile;

      /**
       * statisticsFile
       *
       * Default value: ""
       */
//        std::string statisticsFile;

      /**
       * netFilename
       *
       * Default value: "net.rbf"
       */
//        std::string netFilename;

      /**
       * netFilename2
       *
       * Default value: ""
       */
//        std::string netFilename2;
      //@}
    };

    // ----------------------------------------------------
    // rbf::initFunctors
    // ----------------------------------------------------

    /**
     * initialization functor for rbf
     */
    class initFunctor1 : public stdLayer::initFunctor {
    public:
      /**
       * default constructor
       * @param theFeatures the matrix with the training patterns (each row
       *                    a pattern)
       * @param theTrainIDs the numerical ids for each pattern in theFeatures
       * @param theNbViewsObj the number of patterns per class
       * @param theNbObj      the number of classes (or objects)
       * @param theNbNeurObj  the number of neurons per class
       * @param theLvqInit initialization method vor the LVQ training
       * @param norm the used distance (L1distance or L2distance)
       */
      initFunctor1(const dmatrix& theFeatures,
                   const ivector& theTrainIDs,
                   const ivector& theNbViewsObj,
                   const int& theNbObj,
                   const int& theNbNeurObj,
                   const parameters::eLvqInit& theLvqInit,
                   const eNormType& norm);

      /**
       * destructor
       */
      ~initFunctor1();

      /**
       * apply operator
       */
      bool operator() (dmatrix& weights, ivector& outID);

      /**
       * set the train parameters
       * @param theTrainType specify training for the LVQ initialization
       * @param theLearnRate
       * @param theLrFac the learn factor
       * @param theWinSize the window size
       */
      void setTrainParams(const parameters::eLvqTrainType& theTrainType,
                          const double& theLearnRate,
                          const double& theLrFac=0.,
                          const double& theWinSize=0.);

    protected:
      /**
       * a reference to the training patterns
       */
      const dmatrix& features;

      /**
       * a reference to the numerical ids of the training patterns
       */
      const ivector& trainIDs;
      /**
       * a reference to the number of views per object
       */
      const ivector& nbViewsObj;

      /**
       * number of objects
       */
      int nbObj;

      /**
       * number of neurons per object
       */
      int nbNeurObj;

      /**
       * learn rate
       */
      double learnRate;

      /**
       * learn factor
       */
      double lrFac;

      /**
       * window size
       */
      double winSize;

      parameters::eLvqInit lvqInit;
      parameters::eLvqTrainType lvqType;
      eNormType norm;

      ivector locTrainIDs;
      ivector locNbViewsObj;

      int locNbObj;
      int locNbNeurObj;
      stdLayer::linearActFunctor linear;
      stdLayer::distancePropFunctor distProp;
      lvq::trainFunctor* trainer;
      bool changed;

      ivector index;

      dvector dummy;

      int step;

      /**
       * flag that indicates whether the weights were changed at all during
       * the last epoche. If not no further 'training' is performed.
       */
      bool modified;
    };

    /**
     * initialization functor for rbf networks
     */
    class initFunctor2 : public stdLayer::initFunctor {
    public:
      /**
       * constructor
       */
      initFunctor2(dmatrix& theH,
                   dmatrix& theHtilde,
                   const double& theLambda,
                   const ivector& theTrainIDs);
      /**
       * apply operator
       */
      bool operator()(dmatrix& weights, ivector& outID);

    protected:
      dmatrix& H;
      dmatrix& Htilde;
      const ivector& trainIDs;
      double lambda;
    };

    // ----------------------------------------------------
    // rbf::layer1
    // ----------------------------------------------------

    /**
     * layer 1 for rbf networks
     */
    class layer1 : public stdLayer {
    public:
      /**
       * constructor for the first layer
       */
      layer1(std::string theName) : stdLayer(theName) {};

      /**
       * destructor
       */
      virtual ~layer1() {};

      /**
       * train layer 1
       */
      bool train(const dvector& input,
                 const int& trainID,
                 trainFunctor& trainFunc);

      /**
       * get matrix H
       */
      void getH(const dmatrix& features,
                stdLayer::propagationFunctor& prop,
                dmatrix& H);

      /**
       * get matrix H~
       */
      void getHtilde(stdLayer::propagationFunctor& prop,dmatrix& Htilde);

      /**
       * calculate sigma-factors
       */
      void findSigmas(const double& sigmaFac, dvector& sigmas);

      /**
       * set training parameters
       */
      void setTrainParams(initFunctor1& initFunctor,
                          const parameters::eLvqTrainType& theTrainType,
                          const double& theLearnRate,
                          const double& theLrFac=0.,
                          const double& theWinSize=0.);


    };

    // ----------------------------------------------------
    // rbf::layer2
    // ----------------------------------------------------

    /**
     * layer 2 for rbf networks
     */
    class layer2 : public stdLayer {
    public:
      /**
       * constructor
       */
      layer2(std::string theName) : stdLayer(theName) {};
      /**
       * destructor
       */
      virtual ~layer2() {};

      /**
       * train layer 2
       */
      bool train(const dvector& input,
                 const int& trainID,
                 trainFunctor& trainFunc);
    };



    // ----------------------------------------------------
    // rbf
    // ----------------------------------------------------

    /**
     * Constructor
     */
    rbf();

    /**
     * Copy constructor
     */
    rbf(const rbf& other);

    /**
     * Create with the given parameters
     */
    rbf(const parameters& param);

    /**
     * Destructor
     */
    ~rbf();

    /**
     * get type name
     */
    const char* getTypeName() const;

    /**
     * Training. Mainly organization of data and
     * training algorithms
     */
    bool train(const dmatrix& input,
               const ivector& ids);

    /**
     * Propagates a feature vector through the network. The winner is
     * set to the unit with the maximum value. If an error occurs, the
     * %outputVectors recect flag is set to true and false is
     * returned.
     */
    bool classify(const lti::dvector& feature, outputVector& result) const;

    /**
     * Get dimensions of the output layer
     */
    inline int getDimOutputLayer() const;

    /**
     * object probabilities are not used
     */
//      void noObjProbs();

    /**
     * Get the test-data for training statistics
     */
    void setTestSet(const dmatrix& input,
                    const ivector& ids);

    /**
     * Gets the disjunct training data for object probabilities
     */
    void setTrainSet2(const dmatrix& input,
                      const ivector& ids);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    rbf& copy(const rbf& other);

    /**
     * Alias for copy.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    inline rbf& operator=(const rbf& other) {
      return copy(other);
    }

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual classifier* clone() const;

    /**
     * @name Methods to access inner data.
     * Should not be used for anything but Axioms or others JNI interface
     * to use this class.
     */
    //@{

    /**
     * Returns the weights of layer1
     */
    const dmatrix& getWeights1();

    /**
     * Returns the sigmas of layer1
     */
    const dvector& getSigmas1();

    /**
     * Returns the weights of layer2
     */
    const dmatrix& getWeights2();

    /**
     * Returns the IDs of each neuron of the network
     */
    const ivector& getIds();

    /**
     * Set the sigmas for layer 1
     */
    void setSigmas(const dvector& s);

    /**
     * Set data of layer1
     */
    void setLayer1Data(const dmatrix& weights);

    /**
     * Set data of layer2
     */
    void setLayer2Data(const dmatrix& weights, const ivector& ids);

    /**
     * Set the size and init functors after data is transmitted to layers
     * and sigmas are set.
     */
    void initNet(const int& inSize, const int& outSize);


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

#ifdef _INCLUDE_DEPRECATED
    /**
     * Load information about the network from a '.rbf' file
     * \deprecated Use read() instead
     */
//      bool load(std::istream& instream);

    /**
     * Save information about the network in a '.rbf' file
     * \deprecated Use write() instead
     */
//      bool save(std::ostream& outstream);

    /**
     * Load information about the network from a binary '.rbf' file
     * \deprecated Use read() instead
     */
//      bool loadBinary(const char * fname);

    /**
     * Save information about the network in a binary '.rbf' file
     * \deprecated Use write() instead
     */
//      bool saveBinary(const char * fname);
#endif // deprecated

  protected:

    /**
     * returns used parameters
     */
//  parameters& getParameters();

    layer1 rbf1;
    layer2 rbf2;

//      output lvqNnoutput;

    int sizeIn;
    int sizeOut;

    ivector trainID;
    ivector internToReal;
    std::map<int,int> realToIntern;
    dmatrix features;

    dmatrix train2Features;
    ivector train2Ids;

    dvector sigmas;

    dmatrix testFeatures;
    ivector testIDs;
    ivector trainStatIDs;

    stdLayer::distancePropFunctor* distProp;
    stdLayer::dotPropFunctor* dotProp;
    stdLayer::gaussActFunctor* gaussAct;
    stdLayer::sigmoidActFunctor* sigmoidAct;

//      statFunctor* trainStat;
//      statFunctor* testStat;
//      statFunctor* lvqStat;

    std::map<int,dvector> targetVec;

    /**
     * Calculate object probabilities with given features
     */
    bool calcObjProbs(ivector& ids,
                      dmatrix& feats,
                      stdLayer::distancePropFunctor& distProp);

//      void initStatistics();
//      void statistics(statFunctor& stat,
//                      ivector& ids,
//                      dmatrix& feats,
//                      bool saveMe);
  };


  int rbf::getDimOutputLayer() const {
    return sizeOut;
  }

}

#endif //_LTI_RBF_H_

