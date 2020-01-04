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
 * file .......: ltiHmmClassifier.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.04.2002
 * revisions ..: $Id: ltiHmmClassifier.h,v 1.14 2005/11/14 15:53:03 doerfler Exp $
 */


#ifndef _LTI_HMM_CLASSIFIER_H_
#define _LTI_HMM_CLASSIFIER_H_

#include "ltiSupervisedSequenceClassifier.h"
#include "ltiHmmTrainer.h"

namespace lti {

  /**
   * This class defines training and classification methods for Hidden Markov Models (HMM)
   * using observation sequences. The algorithms are taken from "Fundamentals of speech
   * recognition" (L. Rabiner and B.-H. Juang, Prentice Hall, 1993) and can also be found in
   * "Erkennung kontinuierlicher Gebärdensprache mit Ganzwortmodellen" (H. C. Hienz, PhD thesis,
   * Chair of Technical Computer Science, 2000). The notation in this documentation is taken
   * from the latter literature.
   *
   * In case of classification, the probability \f$P(\mathbf{O}|\lambda)\f$ of the
   * observation sequence \f$\mathbf{O}\f$ is approximated by the observation probability
   * \f$P^*(\mathbf{O}|\lambda)\f$ of the best path \f$\mathbf{q^*}\f$. The best path is found
   * with the Viterbi algorithm. Increased computational efficiency and precision are achieved
   * by using scores with \f$Score(\mathbf{X}|\lambda):=-\ln P^*(\mathbf{X}|\lambda)\f$
   * instead of probabilities. Note that the lowest score corresponds to the highest
   * probability and thus represents the best recognition result.
   *
   * <b>Note</b>: The values in the outputVector are not probabilities!
   * To match the value criteria (higher values represent better recognition results, positive values only),
   * scores are mapped to values between 0.0 and 1.0. Currently exponential, linear, and none are available.
   * <dl>
   * <dt>exponential (default)</dt>
   * <dd>\f$\left[ \frac{1.0}{score-lowestScore + 1.0} -
   * \frac{1.0}{(highestScore-lowestScore)+1.0}\right] \cdot
   * \frac{(highestScore-lowestScore)+1.0}{highestScore-lowestScore}\f$</dd>
   * <dt>linear</dt>
   * <dd>\f$value = \frac{highestScore-score}{highestScore-lowestScore}\f$</dd>
   * <dt>none</dt>
   * <dd>\f$value = score\f$</dd>
   * </dl>
   *
   */
  class hmmClassifier : public supervisedSequenceClassifier {

  public:

    // --------------------------------------------------
    // hmmClassifier::parameters
    // --------------------------------------------------

    /**
     * the parameters for the class hmmClassifier
     */
    class parameters : public classifier::parameters {
    public:


      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters %object to be copied
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
       * copy the contents of a parameters %object
       * @param other the parameters %object to be copied
       * @return a reference to this parameters %object
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
       * Parameters used for the training process.
       * Changing these parameters only affects the following training processes.
       */
      hmmTrainer::parameters hmmTrainingParameters;

      /**
       * Default hidden markov model used for the training process.
       * This can be useful for modifying attributes such as emissionScoreWeight.
       * Changing these parameters only affects the following training processes,
       * existing models are left untouched.
       */
      hiddenMarkovModel defaultModel;


      /**
       * Mapping type used for scores. Currently exponential, linear and none are available.
       * <dl>
       * <dt>exponential</dt>
       * <dd>\f$value = \frac{1.0}{score - lowestScore + 1.0}\f$</dd>
       * <dt>linear</dt>
       * <dd>\f$value = \frac{highestscore-score}{highestScore-lowestScore}\f$</dd>
       * <dt>none</dt>
       * <dd>\f$value = score\f$</dd>
       * </dl>
       */
      enum mappingType {
        exponential,  /*!< choose exponential mapping*/
        linear,       /*!< choose linear mapping*/
        none          /*!< choose no mapping */
      };

      /**
       * Specifies the mapping function to be used.
       * default: exponentialMapping
       */
      mappingType mappingFunction;

    };


    /**
     * default constructor
     */
    hmmClassifier();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    hmmClassifier(const hmmClassifier& other);

    /**
     * destructor
     */
    virtual ~hmmClassifier();

    /**
     * returns the name of this type ("hmmClassifier")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmClassifier& copy(const hmmClassifier& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    hmmClassifier& operator=(const hmmClassifier& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * write method
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

    /**
     * read method
     */
    virtual bool read(ioHandler &handler, const bool complete=true);


    //
    // training and classification methods:
    //


    /** @name Training routines
     *  These methods are needed to train Hidden Markov models.
     */
    //@{

      /**
       * Supervised sequence training.
       * The sequences in the <code>input</code> %vector are used for training
       * the %sequence %classifier. Each %sequence is associated with an id
       * given in the %vector <code>ids</code>.
       * Hidden Markov models are created for all given ids, existing models being deleted first.
       * After the training process, the outputTemplate is generated on the training set.
       * @param input %vector of training %sequences
       * @param ids the classes of the training sequences
       * @return true if successful, false otherwise.  (if false you can check
       *              the error message with getStatusString())
       */
      virtual bool train(const std::vector< sequence<dvector> > &input,
                         const ivector& ids);

      /**
       * Training of a single model.
       * The given %sequences are used to train the model of the given id.
       * If such a model already exists internally, it will be overwritten.
       * Training is performed with the training %parameters given in the parameters class.
       *
       * Note: if you train a single model this way, the output template may contain incorrect values!
       */
      bool train(std::vector< sequence<dvector> > &input, int id);

      /**
       * Deletes all models saved internally.
       */
      void reset();

    //@}


    /** @name Classification routines.
     *  These methods are used for classification.
     */
    //@{


      /**
       * Classification.
       * Classifies the %sequence of features and returns the outputVector with
       * the classification result.
       * @param observations the %sequence to be classified
       * @param result the result of the classification
       * @return false if an error occurred during classification else true
       */
      virtual bool classify(const sequence<dvector>& observations,
                            outputVector& result);

    //@}

    //@{


    /**
     * Reduced Classification.
     * Classifies the %sequence of features only with a reduced number
     * of models given in usedModelsIDNameMap.
     * Returns the outputVector with the classification result.
     * @param observations the %sequence to be classified
     * @param result the result of the classification
     * @param usedModelsIDNameMap map of hmm models given by id and name
     * @return false if an error occurred during classification else true
     */
    bool reducedClassify(const sequence<dvector>& observations,
                         outputVector& result,
                         std::map<int, std::string> usedModelsIDNameMap);


    //@}

    /** @name Direct access routines
     *  These methods are used to directly access/change the Hidden Markov models used internally.
     *  Note that changing single models may lead to incorrect output templates!
     */
    //@{

      /**
       * Query id's of trained models.
       * @return vector containing the ids of all models of this classifier.
       */
      ivector getIDs();

      /**
       * Returns a copy of the model with the given id.
       * If no such model exists, the empty default model given in parameters is returned.
       */
      hiddenMarkovModel getHMM(int id) const;

      /**
       * Set the model to be used for the given id.
       *
       * Note: the output template may contain incorrect values hereafter!
       *
       * @return true, if successful, false otherwise
       */
      bool setHMM(hiddenMarkovModel &model, int id);

      /**
       * Delete the model with the given id.
       *
       * Note: the output template may contain incorrect values after this!
       *
       * @return true, if successful, false otherwise.
       */
      bool deleteHMM(int id);

      /**
       * Set the name of a specific model (if a model with the given ID exists).
       */
      void setName(int id, const std::string &theName);

      /**
       * Generate a map ID -> name for all loaded models.
       */
      void generateIndexNameMap(std::map<int, std::string> &theMap) const;

      /** Returns the length of the shortest model. This allows to tell if
        * an observation of a certain length can be classified by this classifier. 
        * If no models are available, -1 will be returned. */
      int getShortestModelLength() const;

    //@}


  protected:

    /**
     * the models with the given ids.
     */
    std::map<int, hiddenMarkovModel> theModels;

    /**
     * feature dimension (saved here for faster access)
     */
    int featureDimension;

  };
}

#endif

