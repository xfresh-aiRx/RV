/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiHmmViterbiPathSearch.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 12.9.2001
 * revisions ..: $Id: ltiHmmViterbiPathSearch.h,v 1.12 2004/06/03 09:47:40 alvarado Exp $
 */

#ifndef _LTI_HMM_VITERBI_PATH_SEARCH_H_
#define _LTI_HMM_VITERBI_PATH_SEARCH_H_

#include "ltiSequence.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiHiddenMarkovModel.h"

namespace lti {

  /**
   * This class finds and evaluates the best path through a Hidden Markov Model
   * (HMM) for a given observation sequence \f$\mathbf{O}\f$. The best path is
   * found with the Viterbi algorithm. The algorithm is taken from "Fundamentals
   * of speech recognition" (L. Rabiner and B.-H. Juang, Prentice Hall, 1993) and
   * can also be found in "Erkennung kontinuierlicher Gebärdensprache mit Ganzwortmodellen"
   * (H. C. Hienz, PhD thesis, Chair of Technical Computer Science, 2000).
   * The notation in this documentation is taken from the latter literature.
   *
   * Increased computational efficiency and precision are achieved by using 
   * scores with \f$Score(\mathbf{X}|\lambda):=-\ln P^*(\mathbf{X}|\lambda)\f$
   * instead of probabilities. Note that the lowest score corresponds to the highest
   * probability and thus represents the best recognition result.
   *
   * <b>Viterbi algorithm:</b>
   * <dl>
   * <dt>Preprocessing:</dt>
   * <dd>\f$\tilde{\pi}_i=-\ln(\pi_i)\f$</dd>
   * <dd>\f$\tilde{b}_j(O_t)=-\ln(b_j(O_t))\f$</dd>
   * <dd>\f$\tilde{a}_{ij}=-\ln(a_{ij})\f$</dd>
   * <dd>\f$1\leq t\leq T$, $1\leq j\leq N\f$</dd>
   * <dt>Initialisation:</dt>
   * <dd>\f$\tilde{\delta}_1(j)=\tilde{\pi}_i + \tilde{b}_j(O_1)\f$</dd>
   * <dd>\f$\psi_1(j)=0\f$</dd>
   * <dd>\f$1\leq j\leq N\f$</dd>
   * <dt>Recursion:</dt>
   * <dd>\f$\tilde{\delta}_t(j)=\displaystyle\min_{i}\left\{
   *     \tilde{\delta}_{t-1} + \tilde{a}_{ij} 
   *     \right\} + \tilde{b}_j(O_t)\f$</dd>
   * <dd>\f$\psi_t(j)=argmin_{i}\left\{\tilde{\delta}_{t-1} +
   * \tilde{a}_{ij}\right\}\f$</dd>
   * <dd>\f$2\leq t\leq T$, $1\leq j\leq N\f$</dd>
   * <dt>Termination:</dt>
   * <dd>\f$Score(\mathbf{X}|\lambda) = -\ln P^*(\mathbf{X}|\lambda) =
   * \displaystyle\min_{j}\left\{\tilde{\delta}_T(j)\right\}\f$</dd>
   * <dd>\f$q^*_T=argmin_{j}\left\{\tilde{\delta}_T(j)\right\}\f$</dd>
   * <dd>\f$1\leq j\leq N\f$</dd>
   * <dt>Backtracking:</dt>
   * <dd>\f$q^*_t=\psi_{t+1}(q^*_{t+1})\f$</dd>
   * <dd>\f$1\leq t<T\f$</dd>
   * </dl>
   *  
   * <dl>
   * <dt>The score of the emission probability \f$b_j(\mathbf{x})\f$ is
   * approximated by the centre of the distribution, which provides the best
   * estimation of the observation, as follows:</dt>
   * <dd>\f$-\ln(b_j(\mathbf{x})) \approx \displaystyle\min_{m} \left(
   * -\ln c_{jm}-\ln b_{jm}(\mathbf{x})\right)\f$.</dd>
   * <dt>Thereby, the score \f$-\ln c_{jm}\f$ denotes the weight scores of the
   * mixture densities \f$b_j(\mathbf{x})\f$. 
   * All components \f$b_{jm}(\mathbf{x})\f$ of \f$b_j(\mathbf{x})\f$ are
   * modelled by either laplace or gaussian distributions.</dt>
   * <dt>Laplace:</dt>
   * <dd>\f$-\ln(b_{jm}(\mathbf{x})) = \sum\limits_{k=1}^d \left\{ \left|
   * \frac{x_k - \mu_{jkm}} {\sigma_{jkm}} \right| + ln(2\sigma_{jkm})
   * \right\}\f$</dd>
   * <dt>Gaussian:</dt>
   * <dd>\f$-\ln(b_{jm}(\mathbf{x})) = \frac{1}{2} \cdot \sum\limits_{k=1}^d
   * \left\{ \left( \frac{x_k - \mu_{jkm}} {\sigma_{jkm}} \right)^2 +
   * ln(2\pi\sigma_{jkm}^2) \right\}\f$</dd>
   * </dl>
   *
   * An alternative notation for the score of the best path
   * \f$\mathbf{q}^*\f$ is
   * \f$Score(\mathbf{X}|\lambda)=-\sum^{T-1}_{t=1}{\ln(a_{q_tq_{t-1}})}-
   * \sum^{T-1}_{t=1}{\ln(b_{q_t}(\mathbf{x}_t))}\f$.
   *
   * <b>Note</b>: Each state sequence \f$\mathbf{q}\f$ always starts in the 
   * first state \f$s_1\f$ and ends in the last state \f$s_N\f$, whereas
   * \f$N\f$ denotes the number of states \f$s_j\f$.
   * Thus, the termination of the Viterbi algorithm is implemented as
   * \f$Score(\mathbf{X}|\lambda)=\tilde{\delta}_T(N)\f$.
   *
   * <b>Note</b>: The preprocessing values \f$\tilde{\pi}_i\f$, 
   * \f$\tilde{b}_j(\mathbf{x})\f$ and \f$\tilde{a}_{ij}\f$ are usually
   * estimated over a given data set by an automatic training algorithm 
   * (see hmmTrainer).
   * 
   */
  class hmmViterbiPathSearch {
  public:

    /**
     * type of an observation sequence
     */
    typedef sequence<dvector> observationSequence;

    /**
     * default constructor
     */
    hmmViterbiPathSearch();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    hmmViterbiPathSearch(const hmmViterbiPathSearch& other);

    /**
     * destructor
     */
    virtual ~hmmViterbiPathSearch();

    /**
     * returns the name of this type ("hmmViterbiPathSearch")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    hmmViterbiPathSearch& copy(const hmmViterbiPathSearch& other);



    /**
     * Performs the Viterbi path search on a given (observed) data sequence and
     * returns its score.
     * @param model the Hidden Markov Model.
     * @param srcData contains the sequence of observed vectors.
     * @result score of the best path through the model for the given sequence.
     */
    double apply(const hiddenMarkovModel &model, const observationSequence &srcData);

    /**
     * Calculates score along a given path for a given (observed) data sequence.
     * @param model the Hidden Markov Model.
     * @param srcData contains the sequence of observed vectors.
     * @param path defines the applicable state sequence.
     * @result score of the given path.
     */
    double evaluatePath(const hiddenMarkovModel &model, const observationSequence &srcData, const ivector &path);

    /**
     * Calculates score along a given path for a given (observed) data sequence,
     * and additionally selects the specified density centers of the mixture densities
     * in the visited path.
     * @param model the hidden markov model
     * @param srcData contains the data sequence.
     * @param path defines the applicable state sequence.
     * @param densityPath contains the applicable mixture density center selection.
     * @result score of the given path.
     */
    double evaluatePath(const hiddenMarkovModel &model, const observationSequence &srcData,
                       const ivector &path, const ivector &densityPath);

    /**
     * Find out lowest possible score for the given model and a sequence of the specified length.
     * @param model the hidden markov model
     * @param pathLength length of the sequence
     * @return minimum score
     */
    double minimumPathScore(const hiddenMarkovModel &model, int pathLength) const;


    /**
     * find out lowest possible score of the given model for a given path.
     * @param model the hidden markov model
     * @param stateSelection the state selection along the given path
     * @param densitySelection the density selection along the given path. if this is empty, the best densities are chosen.
     * @return the minimum score of the given path.
     */
    double minimumPathScore(const hiddenMarkovModel &model, const ivector &stateSelection, const ivector &densitySelection = ivector()) const;

    /** @name State selection logging
     *  These methods can be used to enable/disable logging and access the selection of
     *  states \f$s_j\f$ during a call of apply().
     *  Default is "disabled", for efficiency reasons. Enable only if necessary.
     */
      //@{

        /**
         * Turns logging of state selection on and off.
         * This enables to review the state matrix and the path leading to the minimal score.
         *
         * By default, this is turned off for efficiency reasons (memory).
         */
        void logStateSelection(bool enable = true);

        /**
         * @return matrix of the best states leading to the minimal score.
         * If logging of state selection is turned off or a given path was evaluated, the matrix will be empty.
         */
        const matrix<int>& getBackpointerMatrix() const;

        /**
         * @return chosen path of the last run leading to the minimal score
         * If logging of state selection is turned off or a given path was evaluated, the vector will be empty.
         */
        const vector<int>& getStatePath();

      //@}


    /** @name Density selection logging
     *  These methods can be used to enable/disable logging and access the selection of
     *  densities during a call of apply(). Density selection stands for the centre \f$m\f$
     *  of the distribution \f$b_j(\mathbf{x})\f$, which provides the best estimation of
     *  the observation and is thus selected during the Viterbi path search.
     *  Default is "disabled", for efficiency reasons. Enable only if necessary.
     */
      //@{

        /**
         * Turns logging of density selection matrix on and off.
         * This enables to review the densities selected during evaluation of a sequence.
         *
         * By default, this is turned off for efficiency reasons (memory).
         */
        void logDensitySelection(bool enable = true);

        /**
         * @return matrix of the densities used in the last pass.
         * If logging of density selection is turned off or a given path was evaluated, the matrix will be empty.
         */
        const matrix<int>& getDensitySelectionMatrix() const;

        /**
         * @return densities selected for the chosen path.
         * If logging of density selection is turned off, the vector will be empty.
         */
        const vector<int>& getDensitySelectionPath();

      //@}


    /** @name Emission score logging
     *  These methods can be used to enable/disable logging and access the emission
     *  scores \f$-\ln(b_j(\mathbf{x}))\f$ during a call of apply().
     *  Default is "disabled", for efficiency reasons. Enable only if necessary.
     */
      //@{

        /**
         * Turns logging of emission score matrix on and off.
         * This enables to analyse the emission scores generated for each step and state
         *
         * By default, this is turned off for efficiency reasons (runtime and memory).
         */
        void logEmissionScore(bool enable = true);

        /**
         * @return matrix of the emission scores generated in the last pass.
         * If logging of emission scores is turned off or a given path was evaluated, the matrix will be empty.
         */
        const matrix<double>& getEmissionScoreMatrix() const;

        /**
         * @return emission scores generated for the chosen path.
         * If logging of emission score is turned off, the vector will be empty.
         */
        const vector<double>& getEmissionScorePath();

      //@}

    /**
     * @name Routines for online classification
     */
      //@{

        /**
         * perform initial transition and add emission score, i.e. initialisation of the
         * Viterbi algorithm
         * @return last entry of result vector
         */
        double initialStep(const hiddenMarkovModel &model, const dvector &frame, dvector &result) const;

        /**
         * perform subsequent transition and add emission score, i.e. one iteration of the
         * Viterbi algorithm recursion
         * @return last entry of result vector
         */
        double subsequentStep(const hiddenMarkovModel &model, const dvector &frame, dvector &result) const;


      //@}


  private:


    /**
     * clear logged data
     */
    void reset();


    /**
     * add initial transition vector to result
     * adding is done to allow for later implementation of observation chain recognition
     */
    void initialTransition(const hiddenMarkovModel &model, dvector &result) const;

    /**
     * calculate best transitions
     */
    void subsequentTransition(const hiddenMarkovModel &model, dvector &result, ivector &predecessors) const;

    /**
     * calculate emission score for given frame and add it to the result vector
     */
    void addEmissionScore(const hiddenMarkovModel &model, const dvector &frame,
                          dvector &result, ivector &densitySelection, dvector &scoreSelection) const;

    /**
     * perform initial transition and add emission score
     * @return last entry of result vector
     */
    double initialStep(const hiddenMarkovModel &model, const dvector &frame, dvector &result,
	                    ivector &densitySelection, dvector &scoreSelection) const;

    /**
     * perform subsequent transition and add emission score
     * @return last entry of result vector
     */
    double subsequentStep(const hiddenMarkovModel &model, const dvector &frame, dvector &result,
	                       ivector &stateSelection, ivector &densitySelection, dvector &scoreSelection) const;

    // score functions

    /**
     * calculate score for given frame and density
     */
    double getDensityScore(const hiddenMarkovModel &model, const dvector &frame, const hiddenMarkovModel::singleDensity &dens) const;

    /**
     * calculate score for given frame and state, return best density
     */
    double getStateScoreAndDensity(const hiddenMarkovModel &model, const dvector &frame, const hiddenMarkovModel::mixtureDensity &state, int &dens) const;

    /**
     * gaussian Score (i.e. -ln(N(mean, variance)), where N is the normal distribution)
     */
    double gaussScore(const dvector &frame, const dvector &mean,
                     const dvector &variance, const dvector &featureWeights) const;

    /**
     * laplace Score (i.e. -ln(L(mean, variance)), where L is the laplace distribution)
     */
    double laplaceScore(const dvector &frame, const dvector &mean,
                       const dvector &variance, const dvector &featureWeights) const;

    /**
     * defines, whether state selection is logged
     */
    bool registerStateSelection;

    /**
     * backtrace matrix will be saved here
     */
    matrix<int> stateMatrix;

    /**
     * best path will be generated by getStatePath and then saved here
     */
    vector<int> statePath;


    /**
     * defines, whether density selection is logged
     */
    bool registerDensitySelection;

    /**
     * density selection matrix will be saved here
     */
    matrix<int> densityMatrix;

    /**
     * density selection path will be generated by getDensityPath and then saved here
     */
    vector<int> densityPath;


    /**
     * defines, whether emission scores are logged
     */
    bool registerEmissionScore;

    /**
     * emission score matrix will be saved here
     */
    matrix<double> emissionScoreMatrix;

    /**
     * emission score path will be generated by getEmissionScorePath and then saved here
     */
    vector<double> emissionScorePath;


//    friend class hmmTrainer;

  private:

    int stateCount;

  };

}

#endif

