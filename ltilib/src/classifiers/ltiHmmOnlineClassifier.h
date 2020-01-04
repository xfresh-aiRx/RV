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
 * file .......: ltiHmmOnlineClassifier.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 09.07.2002
 * revisions ..: $Id: ltiHmmOnlineClassifier.h,v 1.9 2005/11/14 15:53:03 doerfler Exp $
 */


#ifndef _LTI_HMM_ONLINE_CLASSIFIER_H_
#define _LTI_HMM_ONLINE_CLASSIFIER_H_

#include "ltiHmmViterbiPathSearch.h"
#include "ltiHmmClassifier.h"

namespace lti {

  /**
   * This class allows online classification of word chains.
   */
  class hmmOnlineClassifier : public hmmClassifier {

  public:

    // --------------------------------------------------
    // hmmOnlineClassifier::parameters
    // --------------------------------------------------

    /**
     * the parameters for the class hmmOnlineClassifier
     */
    class parameters : public hmmClassifier::parameters {
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
       * additive constant used for pruning (beam search).
       * all hypotheses that have a score greater than minScore + pruningThreshold
       * are thrown away.
       * For values lower than 0, no beam search is performed.
       * default is 1000, but this is no generally reasonable value. the threshold depends
       * strongly on the number of features and the trained data.
       */
      float pruningThreshold;

      /**
       * describes the maximum number of active hypotheses (histogram pruning).
       * if more hypotheses (i.e. paths within
       * a model) are active, the worst hypotheses are thrown away.
       * for value 0, no histogram pruning is performed.
       * this value should be adjusted depending on the number of models.
       * default is 1000.
       */
      int maxActiveHypotheses;

      /**
       * defines the number of buckets for the bucket-sort-algorithm used in histogram pruning.
       * when performing histogram pruning, one bucket-sort-run is performed and the hypotheses
       * in the buckets for the lowest scores are kept, so that there are at most maxActiveHypotheses
       * active.
       * the more buckets there are, the closer the number of surviving hypotheses will get to the
       * given maximum count. (the number of buckets has no influence on run time!)
       * default is 100.
       */
      int numberOfBuckets;

      /**
       * after the given number of timesteps, a partial trace back is performed and the calculated values
       * are saved internally.
       * this is done to keep the trace back field small and the scores low.
       * a value of 0 disables automatic trace back.
       * default is 0.
       */
      int automaticTraceBack;

    };

    /**
     * default constructor
     */
    hmmOnlineClassifier();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    hmmOnlineClassifier(const hmmOnlineClassifier& other);

    /**
     * destructor
     */
    virtual ~hmmOnlineClassifier();

    /**
     * returns the name of this type ("hmmOnlineClassifier")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmOnlineClassifier& copy(const hmmOnlineClassifier& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    hmmOnlineClassifier& operator=(const hmmOnlineClassifier& other);

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
    // online classification methods:
    //


    /** @name Online classification routines.
     *  These methods are used for online classification.
     */
      //@{


      /**
       * delete all models, hypotheses and traceback fields.
       */
      void reset();


      /**
       * Empty traceback fields and delete current hypotheses.
       * This routine must be called before a new online classification session is started.
       */
      void initialize();

      /**
       * Online classification step.
       * @param observation the observation of this timestep.
       * @return false if an error occurred during classification else true
       */
      bool classify(const dvector& observation);

    //@}


    //
    // trace back methods
    //


    /** @name trace back routines
     *  These methods are used to determine the best word sequence.
     */
      //@{


      /**
       * Perform partial traceback.
       * Find the latest timeframe left to where the word sequence is fixed, i.e. find the start of
       * a sentence where later hypotheses have no effect on. This word sequence is returned and
       * the internal traceback fields are truncated accordingly.
       * Furthermore, the internal scores of all current hypotheses are reduced.
       * @return vector describing a sequence of word ids.
       */
      ivector partialTraceBack();

      /**
       * Perform partial traceback.
       * Find the latest timeframe left to where the word sequence is fixed, i.e. find the start of
       * a sentence where later hypotheses have no effect on. This word sequence is returned and
       * the internal traceback fields are truncated accordingly.
       * Furthermore, the internal scores of all current hypotheses are reduced.
       * @param wordLength the length of each classified word is saved here.
       * @param wordEndScores the score of the word end hypotheses is saved here.
       * @return vector describing a sequence of word ids.
       */
      ivector partialTraceBack(ivector &wordLength, dvector &wordEndScores);

      /**
       * Full word sequence traceback
       * Return the best word sequence, given the assumption that the word has ended.
       * @return vector describing a sequence of word ids.
       */
      ivector fullTraceBack() const;

      /**
       * Full word sequence traceback
       * Return the best word sequence, given the assumption that the word has ended.
       * @param wordLength the length of each classified word is saved here.
       * @param wordEndScores the score of the word end hypotheses is saved here.
       * @return vector describing a sequence of word ids.
       */
      ivector fullTraceBack(ivector &wordLength, dvector &wordEndScores) const;

      /**
       * Full traceback of the best active hypothesis.
       * Return the word sequence leading to the currently best hypotheses. Note, that this hypothesis might
       * not be in a word-end-state.
       * @return vector describing a sequence of word ids.
       */
      ivector bestHypothesisTraceBack() const;

      /**
       * Full traceback of the best active hypothesis.
       * Return the word sequence leading to the currently best hypotheses. Note, that this hypothesis might
       * not be in a word-end-state.
       * @param innerState the active state of the hypothesis is saved here. the according model is
       *        given by the last entry of the return vector.
       * @param wordLength the length of each classified word is saved here.
       * @param wordEndScores the score of the word end hypotheses is saved here.
       * @return vector describing a sequence of word ids.
       */
      ivector bestHypothesisTraceBack(int &innerState, ivector &wordLength, dvector &wordEndScores) const;


      /**
       * @return number of currently active hypotheses.
       */
      int getNumberOfHypotheses() const {
        return overallActiveHypotheses;
      }


    //@}


    //
    // word error routines
    //


    /** @name word error routines
     *  These methods are used to determine the word error rate of a classified sentence.
     */
    //@{


      /**
       * Calculate the Levenshtein distance (also called edit distance).
       * The Levenshtein distance of two sentences is the minimum number of substitutions, insertions
       * and deletions needed to transform one sentence into the other.
       * @return Levenshtein distance of the given word sequences
       */
      int wordErrorCount(const ivector &wordSequence1, const ivector &wordSequence2) const;


      /**
       * The word error rate is defined as the Levenshtein distance divided by
       * the length of the correct word sequence.
       * @return word error rate
       */
      float wordErrorRate(const ivector &correctSequence, const ivector &classifiedSequence) const {
        return wordErrorCount(correctSequence, classifiedSequence) / static_cast<float>(correctSequence.size());
      }

    //@}



  private:


    /**
     * this class describes an active model
     */
    class wordHypothesis : public ioObject {

    public:

      /**
       * default constructor
       */
      wordHypothesis();

      /**
       * copy constructor
       * @param other the object to be copied
       */
      wordHypothesis(const wordHypothesis& other);

      /**
       * destructor
       */
      virtual ~wordHypothesis();

      /**
       * copy data of "other" wordHypothesis.
       * Please note that the status string will _NOT_ be copied!
       */
      wordHypothesis& copy(const wordHypothesis& other);

      /**
       * assigment operator (alias for copy(other)).
       * @param other the wordHypothesis to be copied
       * @return a reference to the actual wordHypothesis
       */
      wordHypothesis& operator=(const wordHypothesis& other);

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

  #   ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
  #   endif


      /**
       * id of the model
       */
      int modelID;

      /**
       * word length of the current hypothesis (life time)
       */
      int wordLength;

      /**
       * interim result vector
       */
      dvector resultVector;
    };




    /**
     * this class describes a traceback entry, i.e. a word end hypothesis
     */
    class wordEndHypothesis : public ioObject {

    public:

      /**
       * default constructor
       */
      wordEndHypothesis();

      /**
       * copy constructor
       * @param other the object to be copied
       */
      wordEndHypothesis(const wordEndHypothesis& other);

      /**
       * destructor
       */
      virtual ~wordEndHypothesis();

      /**
       * copy data of "other" wordEndHypothesis.
       * Please note that the status string will _NOT_ be copied!
       */
      wordEndHypothesis& copy(const wordEndHypothesis& other);

      /**
       * assigment operator (alias for copy(other)).
       * @param other the wordEndHypothesis to be copied
       * @return a reference to the actual wordEndHypothesis
       */
      wordEndHypothesis& operator=(const wordEndHypothesis& other);

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

  #   ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
  #   endif


      /**
       * id of the model
       */
      int modelID;

      /**
       * word length of the current hypothesis (life time)
       */
      int wordLength;

      /**
       * score of the model's last state
       */
      float score;
    };


    /**
     * prune one model hypothesis, adjust minScore.
     * return false if no hypotheses survive, else true.
     */
    int pruneModelHypothesis(dvector &resultVector, float &minScore, float &maxScore) const;


    /**
     * trace back from given position.
     * return word vector, word lengths and relative word scores
     */
    void traceBack(int traceBackIndex, ivector &wordIDs, ivector &wordLength, dvector &wordEndScores) const;


    /**
     * find last collective starting point for all active hypotheses
     */
    int findCollectiveStartingPoint() const;


    /**
     * reduce scores of all hypotheses by given value
     */
    void reduceScores(float value);


    /**
     * append internally saved vectors to the front of the given vectors
     */
    void appendTracebackVectors(ivector &wordIDs, ivector &wordLength, dvector &wordEndScores) const;


    /**
     * all running hypotheses are saved here
     */
    std::list<wordHypothesis> activeHypotheses;


    /**
     * traceback field containing the word end hypotheses for all time frames.
     */
    std::vector<wordEndHypothesis> traceBackField;


    /**
     * copy of model with best hypotheses.
     */
    wordHypothesis bestHypothesis;


    /**
     * temporary copy for faster access
     */
    float pruningThreshold;

    /**
     * number of active hypotheses
     */
    int overallActiveHypotheses;

    /**
     * the results of the automatic partial trace back are saved here
     */
    int timestepCounter;
    ivector partialWordHyp;
    ivector partialWordLengths;
    dvector partialWordEndScores;

  };

}

#endif

