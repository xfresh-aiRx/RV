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
 * file .......: ltiHmmTrainer.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 7.1.2002
 * revisions ..: $Id: ltiHmmTrainer.h,v 1.17 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_HMM_TRAINER_H_
#define _LTI_HMM_TRAINER_H_

#include <vector>

#include "ltiFunctor.h"
#include "ltiHmmViterbiPathSearch.h"

namespace lti {
  /**
   * Hidden Markov Model (HMM) trainer.
   * This class creates a hiddenMarkovModel from a given training set of
   * feature vector sequences.
   *
   * By default a bakis-topology is chosen (0-1-2 transitions), but this
   * can be changed by redefining minJump and maxJump.
   *
   * The Viterbi training algorithm, aka segmental K-means, and a mixture
   * density estimator are used to determine the HMM %parameters, starting
   * from the user-defined %parameters.
   *
   * See: B.-H. Juang and L. Rabiner, "The segmentation K-means algorithm for
   * estimating %parameters of hidden markov models", IEEE ASSP Magazine,
   * no.9, vol.38, pp.1639-1641,1990.
   *
   * <b>Note</b>: for computational efficiency probabilities p are not
   * explicitly computed but represented by <b>scores</b>
   * (~ -ln(p)). Note that a high score corresponds to a low probability,
   * i.e. it actually resembles a cost.
   */
  class hmmTrainer : public functor {
  public:

    /**
     * the parameters for the class hmmTrainer
     */
    class parameters : public functor::parameters {
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
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Pooling modes
       */
      enum poolingType {
        noPooling,   /*!< don't pool variances */
        statePooling,/*!< pool variances within one state */
        modelPooling /*!< pool variances of a dimension for all states */
      };

      /**
       * Estimator types:
       */
      enum estimatorType {
        estimMaximumLikelihood, /*!< select estimator for given score
                                  function (default) */
        estimStandardDeviation, /*!< standard deviation (gaussian
                                  maximum-likelihood) */
        estimMeanDeviation,     /*!< mean absolute deviation
                                  (laplacian maximum-likelihood) */
        estimMeanDeviationRoot  /*!< root of mean absolute deviation
                                  (laplacian maximum-likelihood) */
      };

      /**
       * special definitions for numberOfStates
       */
      enum {
        minimumSequenceLength = 0, /*!< uses the minimum length of all
                                     sequences as number of states */
        averageSequenceLength = -1 /*!< uses the average length of all
                                     sequences as number of states */
      };


      /** @name Training termination
       *  These members are the training iteration control %parameters.
       *  <b>For experienced users!</b>
       */
      //@{

        /**
         * Maximum number of training iterations.
         *
         * Default is 0 (iterate, until convergenceThreshold is reached).
         */
        int maxIterations;

        /**
         * Estimation of the HMM %parameters is finished, when score
         * enhancement within one iteration is below or equal to
         * convergence threshold for given training data.
         *
         * Default is 0.0 (iterate, while model score is improving)
         */
        double convergenceThreshold;

      //@}


    /** @name HMM attributes
     *  These members are the model attributes.
     */
      //@{

        /**
         * Number of states of the model to be generated.
         *
         * A value of <b>minimumSequenceLength</b> (0) indicates to
         * pick the size of the shortest observation sequence,
         * <b>averageSequenceLength</b> (-1) picks the average size of
         * all considered observation sequences.
         *
         * Default: minimumSequenceLength (0)
         */
        int numberOfStates;

        /**
         * Minimum relative forward jump within HMM.
         *
         * Default: 0 , i.e. loop to current state
         */
        int minimumJump;

        /**
         * Maximum relative forward jump within HMM.
         *
         * Default: 2
         */
        int maximumJump;

        /**
         * Vector of initial scores.
         * If the size of this vector differs from numberOfStates, the default
         * initialScore is used:
         *
         * <code>
         * <TABLE>
         *  <TR> <TD>            </TD><TD>state 0 </TD><TD>other states</TD> </TR>
         *  <TR> <TD><b>score</b></TD><TD><b>0</b></TD><TD><b>oo</b>   </TD> </TR>
         *  <TR> <TD>(prob)      </TD><TD>(1)     </TD><TD>(0)         </TD> </TR>
         * </TABLE>
         * </code>
         *
         * Remember to specify the initialScore as <b>scores</b>
         * (i.e. ~ -ln(p)). Note that the initialScore vector doesn't have
         * to be normalized.
         */
        dvector initialScore;
      //@}


    /** @name Emission density
     *
     *  %Parameters defining the kind of emission density and the
     *  estimator to use.
     */
      //@{

        /**
         * Density function type to be used for the emission scores.
         *
         * Default: laplaceScore
         */
        hiddenMarkovModel::scoreType scoreFunction;

        /**
         * Method for estimating the scaling factor (ie spread) of the
         * density functions.
         *
         * Default: estimMaximumLikelihood
         */
        estimatorType estimatorFunction;

        /**
         * Minimum scaling factor. This is required to avoid very
         * narrow and high density functions, which are obtained, when
         * variation in data is too little.
         *
         * Default is 0.1
         */
        double minScalingFactor;

      //@}

    /** @name Splitting (mixture densities)
     *  These members can be adjusted to change the splitting process, where
     *  the density functions are successively divided into smaller ones to
     *  describe the actual distribution better.
     *
     *  <b>For intermediate users!</b>
     */
      //@{

        /**
         * Maximum allowed number of densities per state.
         *
         * Default = 5
         */
        int maxDensities;

        /**
         * Number of iterations between two split processes.
         *
         * A value of 0 will reassign observations until no further
         * changes occur (nope, not yet!).
         *
         * The default value is 5 (split densities at every 5th iteration).
         */
        int reassignmentIterations;

        /**
         * Number of iteration, where splitting is taken into
         * consideration for the first time.
         *
         * By default, this is set to 0.
         */
        int firstSplit;

        /**
         * Pooling means to compute a common scaling factor (spread,
         * variance) for several densities. You can select to have no
         * pooling, state pooling or model pooling.
         *
         * Default: noPooling
         */
        poolingType pooling;

        /**
         * Minimum number of observations assigned to a density to be
         * splittable.
         *
         * Default: 3
         */
        int minFramesForSplit;

      //@}


    };

    /**
     * default constructor
     */
    hmmTrainer();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    hmmTrainer(const hmmTrainer& other);

    /**
     * destructor
     */
    virtual ~hmmTrainer();

    /**
     * returns the name of this type ("hmmTrainer")
     */
    virtual const char* getTypeName() const;

    /**
     * Discard sequences considered so far.
     */
    void reset();

    /**
     * Consider this sequence for computing the hidden markov model.
     * The sequence is added to an internal data store.
     */
    void consider(const sequence<dvector> &theSeq);

    /**
     * Consider multiple sequences for computing the hidden markov model.
     * The sequences are added to an internal data store.
     */
    void consider(const std::vector< sequence<dvector> > &theSequences);

    /**
     * Generate hiddenMarkovModel from the data in the internal data store,
     * that has been filled with the consider() members so far.
     */
    void apply(hiddenMarkovModel &model);

    /**
     * Get the progress of the total score over training data. The returned
     * vector contains the scores of each iteration as a result of the training
     * (apply()).
     */
    dvector getScoreProgressSequence() {
      return scoreSequence;
    }

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmTrainer& copy(const hmmTrainer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmTrainer& operator=(const hmmTrainer& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


  private:

    /**
     * this private class defines an assignment of frames to a density
     * overall scores of the density are also saved here
     */
    class densityAssignment {
    public:

      /**
       * pointer to the frames (observations) assigned to this density
       */
      std::vector<const dvector *> frames;

      /**
       * calculated density center is written here.
       */
      hiddenMarkovModel::singleDensity center;

      /**
       * average score for this density
       */
      double score;
    };

    /**
     * assignment of densities to a state
     */
    typedef std::vector<densityAssignment> stateAssignment;

    /**
     * improve given model with respect to the training sequences
     * (might become public)
     */
    double improveModel(hiddenMarkovModel &model, bool split);

    /**
     * change state number, assign frames to state/density in a linear fashion
     */
    void initializeAssignments();

    /**
     * clears all assignments of densities/frames (does not change the
     * number of states or densities)
     */
    void clearAssignments();

    /**
     * initialize the model
     */
    void initializeModel(hiddenMarkovModel &model);

    /**
     * copy parameters and modify them if necessary
     */
    void copyParameters();

    /**
     * calculate transition matrix, i.e. do smoothing and -log()
     */
    void calculateTransitionMatrix(matrix<double> &transitionScore);

    /**
     * test model on training data, return average score
     */
    double evaluateModel(const hiddenMarkovModel &model);

    /**
     * create model from the current transitions and assignments
     */
    void createModel(hiddenMarkovModel &model);

    /**
     * calculate mean values and variances
     */
    void calculateDensities();

    /**
     * calculate single density
     */
    void calculateThisDensity(densityAssignment &theDensity);

    /**
     * calculate transition scores from the viterbi search
     * return achieved score
     */
    double considerTrainingSequence(const hiddenMarkovModel &lastModel,
                                    const sequence<dvector> &theSeq);

    /**
     * divide emission scores by number of assigned frames
     * (i.e. number of emissions)
     */
    void normalizeEmissionScores();

    /**
     * check density scores and split if appropriate
     * return true, if new densities were actually created
     */
    bool splitDensities();

    /**
     * split one density center by disturbance of the center
     */
    void splitThisDensity(densityAssignment &thisDensity,
                          densityAssignment &newDensity);

    /**
     * delete empty densities and states
     */
    void garbageCollection();

    /**
     * delete state with the given number
     */
    void deleteState(const int stateNumber);

    /**
     * delete state with the given number
     */
    void deleteDensity(const int stateNumber, const int densityCenter);

    /**
     * viterbi path search
     */
    hmmViterbiPathSearch viterbi;

    /**
     * training data will be saved here
     */
    sequence< sequence<dvector> > theTrainingData;

    /**
     * assignments of all states and densities
     * assignedFrames[state][density]
     */
    std::vector<stateAssignment> assignedFrames;

    /**
     * score sequence is saved here by apply
     */
    dvector scoreSequence;



    /**
     * length of the shortest sequence will be saved here
     */
    int minSequenceLength;

    /**
     * total number of frames in all considered sequences is saved
     * here to quickly calculate the average sequence length, if
     * needed.
     */
    int totalFramesInSequences;

    /**
     * temporary copy of parameters
     */
    parameters params;

    /**
     * feature dimension.
     * will be saved here by the first call to consider.
     * sequences will then be checked for consecutive calls to consider.
     */
    int featureDimension;

    /**
     * feature weights
     */
    dvector featureWeights;

    /**
     * transition frequency
     * for every transition from state i to j, trans[i][j] is increased.
     * calculateTransitionMatrix creates score matrix from this matrix
     */
    matrix<int> transitionFrequency;


    // calculation of mean and scaling factor (i.e. deviation etc.)

    /**
     * calculate mean
     */
    void calculateDensityCenter(const std::vector<const dvector *> &frames,
                                      dvector &mean) const;

    /**
     * reset scaling factor calculation
     */
    void resetScaleCalculation();

    /**
     * scaling factor calculation: take vector into consideration
     */
    void considerScale(const dvector &mean, dvector vec);

    /**
     * calculate scaling factor from considered vectors
     */
    void calculateScale(dvector &deviation);

    // scale calculation variables
    dvector scaleVector;
    int scaleConsidered;

  };
}

#endif

