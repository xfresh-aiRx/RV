/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiParetoFront.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.11.2003
 * revisions ..: $Id: ltiParetoFront.h,v 1.8 2004/07/14 09:19:39 alvarado Exp $
 */

#ifndef _LTI_PARETO_FRONT_H_
#define _LTI_PARETO_FRONT_H_

#include <list>
#include <vector>
#include <fstream>
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiContinuousRandDist.h"
#include "ltiFunctor.h"
#include "ltiProgressInfo.h"
#include "ltiLispStreamHandler.h"

namespace lti {
  /**
   * The Pareto Front functor provides an evaluation mechanism for algorithms,
   * in which the qualitiy of the algorithm cannot be restricted to one single
   * scalar measure.  It is adecuate to compare different parameterizations for
   * one or several algorithms, which are supposed to solve the same task.
   *
   * This class generates a "front" in a multidimensional fitness
   * space, which represents a tradeoff between several fitness
   * values, that each derived class explicitelly implements.
   *
   * The algorithm used here to generate the front is called PESA (Pareto
   * Envelope-based Selection Algorithm), and it is described in:
   *
   * David. W. Corne, Joshua D. Knowles and Martin J. Oates
   * The Pareto Envelope-based Selection Algorithm for Multiobjective 
   * Optimization. In: Proceedings of the International Conference on Parallel
   * Problem Solving from Nature (PPSN VI). (2000) 839-848
   *
   * A good introduction to the application of this problem for the 
   * evaluation of image processing algorithms, specifically to the evaluation
   * of segmentation is given in:
   *
   * Mark Everingham, Henk Muller and Barry Thomas, Evaluating Image
   * Segmentation Algorithms using the Pareto Front. In Proceedings of
   * the 7th European Conference on Computer Vision (ECCV2002), Part
   * IV (LNCS 2353), pages 34-48. Springer, June 2002.
   *
   * http://www.cs.bris.ac.uk/Tools/Reports/Abstracts/2002-everingham-1.html
   *
   * This is the parent class for all evaluation algorithms using PESA.  Its
   * kern administrates the evolutionary part of the algorithm and the derived
   * classes provide the adaptions necessary for each application in
   * particular, which include the conversion between functor parameters and a
   * binary chain chromosome representation, the computation of the fitness
   * measures, etc.
   *
   * The current class also provides some simple tools that simplify the
   * construction of evaluation functors.
   *
   * There is a deviation from the original paper which is more suitable for
   * the evaluation of algorithms used here.  The original algorithms
   * separates the fitness space into regular hyperboxes and computes the
   * density of individuals in each hyperbox to decide which elements will be
   * used for crossover or mutation (those boxes with the smallest density)
   * and which elements need to be removed (those with the highest density).
   * The computation of the density is in the present functor much more
   * computationally expensive, but its computation is nevertheless negligible
   * in comparison to the computation of the fitness measures for an
   * algorithm:
   *
   * The current algorithm keeps track of the bounding box of the fitness
   * space for which individuals have been created.  This bounding box is used
   * to determine the size of a Gaussian kernel, which will be used on each
   * individual to compute the influence of all other individuals in the
   * front.  This way, the selection is not strongly bounded to the selection
   * of a partitioning of the fitness space.
   *
   * For an example of an evaluation class see lti::locationSearchEvaluation
   */
  class paretoFront : public functor {
  public:
    /**
     * The parameters for the class paretoFront.
     *
     * These are the general parameters for the Pareto Front computation. 
     * For each special evaluation class, more attributes are usually added.
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool& complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool& complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool& complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool& complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Crossover probability.
       *
       * The PESA algorithms does a uniform crossover with this
       * probability Pc.  This means, with probability Pc a crossover
       * between two parents will be done, otherwise only mutation
       * will be done.
       *
       * Default value: 0.7
       */
      double crossoverProbability;

      /**
       * Initial Bit-flip mutation probability.  
       *
       * This value is usually set to 1/L, where L is the size of a 
       * chromosome, i.e. the bit-length size given to the binary parameter
       * representation. (see paretoFront::phenotypeToChromosome).
       *
       * If negative, the value used will be |mutationRate|/L. 
       * If positive, the value will be used "as is".
       *
       * Note that if positive, it only makes sense to have values between 0
       * and 1.
       *
       * This value has to be greater or equal finalMutationRate.
       *
       * Default value: -1 (i.e. 1/L will be used)
       */
      double initialMutationRate;

      /**
       * Final Bit-flip mutation probability.
       *
       * This value is usually set to 1/L, where L is the size of a 
       * chromosome, i.e. the bit-length size given to the binary parameter
       * representation. (see paretoFront::phenotypeToChromosome).
       *
       * If negative, the value used will be |mutationRate|/L. 
       * If positive, the value will be used "as is".
       *
       * Note that if positive, it only makes sense to have values between 0
       * and 1.
       *
       * This value has to be smaller or equal initialMutationRate.
       *
       * Default value: -1 (i.e. 1/L will be used)
       */
      double finalMutationRate;

      /**
       * Mutation Rate Decay Value
       *
       * It is possible to begin the evolution with a higher mutation rate
       * than in a "stable" evolution.  This supports a more random search a
       * the beginning, where nothing really good has been found.  After a few
       * steps can be however desirable to slowly reduce the mutation rate
       * into a more normal value.
       *
       * The ecuation used for the real mutation rate is:
       * (initialMutationRate-finalMutationRate)*exp(-i/d) with "i" the
       * iteration number and "d" this decay rate value.
       *
       * The smaller this value, the faster the mutation rate converges to 
       * its final value.
       *
       * This value must be strictly positive (never zero).  If you want a
       * "traditional" PESA, just set the initial and final mutation rates
       * with the same value.
       *
       * Default value: 33.38 (i.e. after 100 steps only 5\% of the
       *                       (final-initial) interval remains)
       */
      double mutationDecayRate;

      /**
       * Size of elements that constitute the Pareto Front.
       *
       * Default value: 100
       */
      int externalPopulationSize;

      /**
       * Internal population size
       *
       * Size of elements produced by each iteration through cross
       * over or mutation as candidates for the front.
       *
       * Default value: 10
       */
      int internalPopulationSize;

      /**
       * Dimensionality of the space analyzed by the Pareto Front.
       *
       * Note that this is unrelated with the parameter-space, which usually
       * has many more dimensions that the fitness space.
       *
       * Usual values are 2 or 3, since more dimensions are very difficult to
       * visualize.
       *
       * Default value: 2
       */
      int fitnessSpaceDimensionality;

      /**
       * Number of iterations.
       *
       * The process of generating an internal population and then assign the
       * best candidates to the Pareto Front is repeated a number of times
       * specified by this parameter.
       *
       * Note that the total number of evaluations for the algorithms will
       * be approximatelly this factor times internalPopulationSize.  If
       * you really want at least externalPopulationSize elements in the
       * Pareto front, you need to provide enough iterations to allow that,
       * which should be considerably greater than 
       * externalPopulationSize/internalPopulationSize, since not all
       * generated members are added to the pareto front.
       *
       * Default value: 1000
       */
      int numOfIterations;

      /**
       * Log all evaluated individuals.
       *
       * Sometimes, for documentation or debug purposes, you will want to know
       * all created individuals, even the ones not belonging to the pareto 
       * front.  Since they are usually not required, and they demand some
       * resources, it is left to you if you want to keep track of them or not.
       * 
       * Set this parameter to true, if you want to store all generated and
       * evaluated individuals, of false, if you want to save the space and
       * time required to remember them.
       *
       * Default value: false
       */
      bool logAllEvaluations;

      /**
       * Fitness space partitioning.
       *
       * The choice which individual(s) in the Pareto front should be chosen
       * for crossover or mutation is taken on a fitness-space density measure.
       * Elements will be removed from the dense locations, since there are
       * enough prototypes for those places, and for the generation of new
       * ones candidates are taken from the low-density regions.
       *
       * The bounding box for the fitness space will be computed
       * automatically as new elements are generated.
       * To determine the density at each location, a Gaussian
       * kernel will be used.  Its covariance matrix is assumed diagonal, where
       * each dimension will have as std. deviation a sixth of the length
       * obtaind dividing the interval with the given factor.
       *
       * In the original PESA paper a fixed grid was used, but this has 
       * limitations in the reachable precision.
       *
       * Default value: 32
       */
      int fitnessSpacePartition;

      /**
       * Sort result in scanning order.
       *
       * If true, the individuals of the front will be sorted in ascending
       * order of their multidimensional fitness.
       *
       * In principle, this sorting has no semantical effects about the
       * overall fitness of an individual, i.e. an individual later in the
       * list is not necessarily better than another one with a smaller index.
       * This sorting is more oriented towards drawing tasks for the Pareto
       * front.
       * 
       * According to the fitness scanning ordering, and individual A greater
       * than an individual B if
       *
       * \code
       *  (A[n-1] > B[n-1]) or 
       * ((A[n-1] == A[n-1]) and ( (A[n-2] > A[n-2]) or
       *                          ((A[n-2] == A[n-2]) and (A[n-3] > A[n-3]))
       *                           ... ))
       * \endcode
       *
       * This is the same ordering employed for lti::tpoint<T>
       *
       * Sorting will use the STL methods to sort efficiently the result. 
       * However, it is optional in case you don't care how the individuals
       * are sorted.
       *
       * Default value: false
       */
      bool sortResult;

      /**
       * @name Log options
       */
      //@{
      /**
       * Activate log.
       *
       * If true, every new individual that is inserted to the front will be
       * logged in the given file.  Later on, you can use a special apply to
       * continue the analysis of a broken progress.
       *
       * Default value: false
       */
      bool logFront;

      /**
       * Log Filename.
       *
       * Filename used for the log of patterns.  The data will be written in
       * an ASCII format using a lti::lispStreamHandler.  It will contain for
       * each individual the chromosome binary representation and the computed
       * fitness.  At the beginning it will save all parameters necessary to
       * bring this functor to a compatible state.
       *
       * Default value: "pareto.log"
       */
      std::string logFilename;
      //@}
    };

    /**
     * Default constructor
     */
    paretoFront();

    /**
     * Construct a functor using the given parameters
     */
    paretoFront(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    paretoFront(const paretoFront& other);

    /**
     * Destructor
     */
    virtual ~paretoFront();

    /**
     * Returns the name of this type ("paretoFront")
     */
    virtual const char* getTypeName() const;

    /**
     * Compute the Pareto Front
     * 
     * Since the evaluation function can get as input any kind of information,
     * it is left to the derived classes to provide alternative way of doing
     * this: other apply methods or use() methods to indicate the set of data
     * to be used.  
     * 
     * The Pareto Front will be represent by a matrix of size \e m x
     * \e n with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * @param front matrix containing for each row the multidimensional 
     *             fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<double>& front);

    /**
     * Compute the Pareto Front and the corresponding parameter objects
     * for each non-dominated point.
     * 
     * Since the evaluation function can get as input any kind of information,
     * it is left to the derived classes to provide alternative ways of doing
     * this: other apply methods or use() methods to indicate the data set
     * to be used in the algorithm evaluation.
     * 
     * The Pareto Front will be represented by a matrix of size \e m x
     * \e n with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * For each row in this matrix (an individual), the corresponding
     * parameter setting (a phenotype) is returned.  Since different
     * algorithms usually use different parameters, this can only be
     * done with dynamically allocated objects.  You need to take care of
     * them, but this class provides an utility trash() method, which deletes
     * them for you if you want to. 
     * 
     * @param front matrix containing for each row the multidimensional 
     *             fitness value.
     * @param phenotypes the parameters for each fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<double>& front,
                       std::vector<functor::parameters*>& phenotypes);


    /**
     * Compute the Pareto Front and the corresponding parameter objects
     * for each non-dominated point.
     *
     * This is a special method which reassumes a broken analysis, gaining
     * the lost information from a previously generated file.  That file
     * can be generated activating in the parameters the option logFront and
     * indicating there the name for the file.
     *
     * This method will work only if the parameters logFront and logFilename
     * are properly set (logFront in true and logFilename with the proper
     * filename).  It is not named "apply" for the simple reason that the
     * parameters will be changed to those stored in the log-file itself.
     *
     * Since the evaluation function can get as input any kind of information,
     * it is left to the derived classes to provide alternative ways of doing
     * this: other apply()-methods or use()-methods to indicate the data set
     * to be used in the algorithm evaluation.
     * 
     * The Pareto Front will be represented by a matrix of size \e m x
     * \e n with \e m parameters::externalPopulationSize and \e n
     * fitnessSpaceDimensionality.
     *
     * For each row in this matrix (an individual), the corresponding
     * parameter setting (a phenotype) is returned.  Since different
     * algorithms usually use different parameters, this can only be done with
     * dynamically allocated objects.  You need to take care of them, but this
     * class provides an handy trash() method, which deletes them for you if
     * you want to.  If you don't remove them properly you will have a huge
     * memory-leak!  Therefore, we recomend the use of trash().
     * 
     * @param front matrix containing for each row the multidimensional 
     *             fitness value.
     * @param phenotypes the parameters for each fitness value.
     *
     * @return true if apply successful or false otherwise.
     */
    virtual bool resume(matrix<double>& front,
                        std::vector<functor::parameters*>& phenotypes);


    /**
     * @name Additional Information
     */
    //@{
    /**
     * Fitness space bounding box.
     *
     * After determining the Pareto Front, you usually want to know the
     * bounding box of the fitness space that was analyzed.  This for 
     * visualization purposes among others.
     *
     * After the apply() methods you can get the bounding box calling this
     * one.
     *
     * This will always be a matrix 2 x parameters::fitnessSpaceDimensionality,
     * where the first row contains the minima for each dimension and the
     * second row the maxima.
     */
    void getAnalyzedBox(matrix<double>& bbox) const;

    /**
     * Dominated individuals.
     *
     * This method returns all points in the fitness space that were
     * analyzed while constructing the feature space.  For this method
     * to return something meaningful, you need to set in the
     * parameters the attribute logAllEvaluations to \c true.
     *
     * The resulting matrix will have the 
     * size \e n x parameters::fitnessSpaceDimensionality, where \e n is
     * the number of dominated individuals analyzed in the process.
     */
    void getDominatedIndividuals(matrix<double>& dindiv) const;

    /**
     * Get data from log
     *
     * If a log file is generated, usually you cannot read the used 
     * parameterization.  With this method you will get from the log file the
     * list of parameters and their corresponding fitness values, as if you
     * had used the corresponding apply method.
     *
     * The parameters of the current functor will change without invalidating
     * the reference.  Therefore this method is not constant.
     */
    bool getDataFromLog(const std::string& logFile,
                        matrix<double>& front,
                        std::vector<functor::parameters*>& phenotypes);

    //@}

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    paretoFront& copy(const paretoFront& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    paretoFront& operator=(const paretoFront& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Type used to represent chromosomes
     */
    typedef std::vector<bool> chromosome;

    /**
     * Delete all parameter objects in the given vector
     */
    bool trash(std::vector<functor::parameters*> phenotypes) const;


    /**
     * @name Progress Info
     *
     * Methods used to plug-in a progress visualization object.
     */
    //@{
    /**
     * set a progress %object
     *
     * A clone of the given %object will be generated.
     */
    void setProgressObject(const progressInfo& progBox);

    /**
     * remove the active progress %object
     */
    void removeProgressObject();

    /**
     * return true if a valid progressInfo %object has already been set
     */
    bool validProgressObject() const;

    /**
     * Return true if a valid progressInfo %object has already been set and
     * its detail level is greater or equal the given value.
     */
    bool validProgressObject(const int detailLevel) const;

    /**
     * get progress %object
     */
    progressInfo& getProgressObject();

    /**
     * get progress %object
     */
    const progressInfo& getProgressObject() const;

    //@}



    /**
     * @name Public methods to be reimplemented
     *
     * Following methods need to be reimplemented to evaluate specific
     * algorithms.
     */
    //@{

    /**
     * Convert a binary-chain representation of a chromosome to a valid
     * parameter object.
     *
     * There are some tools to convert standard types into some binary chains,
     * which can be used by all derived classes:
     * - binToInt()
     * - binToUInt()
     * - binToDouble()
     */
    virtual bool chromosomeToPhenotype(const chromosome& genotype,
                                       functor::parameters& phenotype) const=0;

    /**
     * Return a fresh allocated parameters for the evaluated functor, which is
     * equivalent to the given genotype.
     *
     * There are some tools to convert binary chains into standard types:
     * - intToBin()
     * - uintToBin()
     * - doubleToBin()
     */
    virtual functor::parameters* 
    chromosomeToPhenotype(const chromosome& genotype) const = 0;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool phenotypeToChromosome(const functor::parameters& phenotype,
                                       chromosome& genotype) const=0;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const = 0;

    /**
     * Evaluate Chromosome
     *
     * This method is one of the most important ones for the pareto evaluation.
     * Its task is to produce a multidimensional fitness measure for a given
     * chromosome.
     *
     * It returns true if the evaluation was successful, of false if the
     * phenotype represents some invalid parameterization.  It is highly 
     * recomended that the mutation and crossover methods are reimplemented to 
     * avoid invalid parameterizations.
     *
     * There are mainly two types of fitness measures that can be
     * analyzed with a functor of this kind: empirical goodness and
     * empirical discrepancy (Zhang).  The empirical goodness computes some
     * measure using exclusively the test data, without requiring any ground
     * truth.  The empirical discrepancy assumes the existency of ground truth
     * and provides as measure some distance between the result of an algorithm
     * and the ground truth.  Each class derived from paretoFront should
     * specify clearly which kind of fitness measures it provides.
     * 
     */
    virtual bool evaluateChromosome(const chromosome& individual,
                                    dvector& fitness) = 0; 

    //@}

    /**
     * @name Conversion Tools
     *
     * Converting between phenotypes and binary chains representing
     * chromosomes can become a tedious task.  These tools should facilitate
     * the process.
     */
    //@{
    /**
     * Convert binary chain into an integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completely read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    int binToInt(const chromosome& chain,
                 const int startBit,
                 const int bitLength,
                 int32& result) const;

    /**
     * Convert binary chain into an integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * If the given value range is higher than the range representable with
     * the given number of bits, only the lowLimit will be considered.  If
     * the read number has a higher range than the high-low one, then the 
     * modulo operation will be applied to keep the values in range.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    int binToInt(const chromosome& chain,
                 const int startBit,
                 const int bitLength,
                 const int lowLimit,
                 const int highLimit,
                 int32& result) const;


    /**
     * Convert binary chain into an unsigned integer representation.
     *
     * The lower indices in the chain represent the MSB of the number.
     * 
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins
     * @param bitLength number of bits used for this integer (never greater
     *                  than 32).
     * @param result integer value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    int binToUInt(const chromosome& chain,
                  const int startBit,
                  const int bitLength,                  
                  uint32& result) const;

    /**
     * Convert binary chain into an double floating point representation.
     *
     * The binarization of floating point values for "genetic" manipulation
     * can easily result in non-sense values.  Therefore here a specified
     * value range is quantized in the desired number of bits between 0 and
     * 31.
     *     
     * @param chain the binary chain representing a chromosome
     * @param startBit index of the bit, at which the integer begins.
     * @param bitLength number of bits used for this double number
     *                  (never greater or equal than 32).
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param result double value read from the chain
     * @return the next valid index in the chain, chain.size() if it was
     *         completelly read, or -1 if the request goes beyond the 
     *         size of the chain.
     */
    int binToDouble(const chromosome& chain,
                    const int startBit,
                    const int bitLength,
                    const double& lowLimit,
                    const double& highLimit,
                    double& result) const;

    /**
     * Convert integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    int intToBin(const int value,
                 const int startBit,
                 const int bitLength,
                 chromosome& chain) const;

    /**
     * Convert integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    int intToBin(const int value,
                 const int startBit,
                 const int bitLength,
                 const int lowLimit,
                 const int highLimit,
                 chromosome& chain) const;

    /**
     * Convert integer value in a binary change.
     *
     * @param value the value to be stored.
     * @param startBit starting index in the chain, where the representation
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param chain binary change with the resulting representation.
     * @return the next valid index in the chain, where further data can
     *         be inserted.  If there is not enough space, -2 will be returned
     */
    int uintToBin(const uint32 value,
                  const int startBit,
                  const int bitLength,
                  chromosome& chain) const;

    /**
     * Convert double value in a binary change.
     * 
     * @param value value to be quantized.
     * @param startBit starting index in the chain, where the representation 
     *                 has to be stored.
     * @param bitLength number of bits used to represent the number.  If it
     *                  is not possible, then -1 will be returned.
     * @param lowLimit lowest value (inclusive) that the stored variable
     *                 can take.
     * @param highLimit highest value (inclusive) that the stored variable
     *                  can take.
     * @param chain binary change with the resulting representation.
     */
    int doubleToBin(const double& value,
                    const int startBit,
                    const int bitLength,
                    const double& lowLimit,
                    const double& highLimit,
                    chromosome& chain) const;

    //@}


  protected:
    /**
     * Returns used parameters
     */
    parameters& getRWParameters();

    /**
     * Structure characterizing an individual
     */
    class individual {
    public:
      /**
       * Constructor
       */
      individual() : fitness(),genotype(),squeezeFactor(0.0) {}

      /**
       * An individual is here "smaller" than another one if
       * its squeezeFactor is smaller.  This is required to sort
       * the individuals after their squeeze factor, which simplifies
       * getting the smallest or greatest elements.
       */
      inline bool operator<(const individual& other) const {
        return (squeezeFactor < other.squeezeFactor);
      }

      /**
       * Fitness of the individual
       */
      dvector fitness;

      /**
       * Chromosome.
       *
       * You can get the phenotype anytime with cromosomeToPhenotype
       */
      chromosome genotype;

      /**
       * Factor computed to determine which candidates should be taken
       * for mutation or crossover.  Dense locations in the fitness space
       * get a high squeeze factor.
       */
      double squeezeFactor;
    };

    /**
     * Initialize the internal population.
     *
     * You usually don't want to reimplement this method, but
     * randomIndividual().  However, you can reimplemented it if you
     * need more that the standard generation of
     * parameters::internalPopulationSize random individuals.
     *
     * @return true if successful, false otherwise.
     */
    virtual bool initInternalPopulation(std::vector<individual>& data);

    /**
     * @name Methods to be reimplemented
     */
    //@{

    /**
     * Generate a random individual.
     *
     * You usually will need to reimplement this method to ensure that
     * the generated random individuals have a valid phenotype, i.e. that
     * the chromosome binary representation in "genotype" has an equivalent
     * parameter object for the class you are using.
     *
     * Return true if successful, false otherwise.
     */
    virtual bool randomIndividual(chromosome& genotype);

    /**
     * Mutate the given chromosome.
     *
     * This should be reimplemented to ensure that the mutation is a 
     * valid phenotype.
     *
     * The default implementation flips the bits with the probability
     * given in the parameters.
     */
    virtual bool mutate(const chromosome& parent,
                        chromosome& mutant);

    /**
     * Crossover between two chromosomes.
     * 
     * This should be reimplemented to ensure that the crossover produces a 
     * valid phenotype.
     *
     * The default implementation does a so called uniform crossover, in which
     * each pair of corresponding bits are exchanged with a probability of
     * 0.5, followed by the mutation indicated in the original PESA paper.
     */
    virtual bool crossover(const chromosome& parent1,
                           const chromosome& parent2,
                           chromosome& child);

    //@}

    /**
     * Get a random value between 0 and 1
     */
    inline double random() const {
      return crdist.draw();
    };

  private:
    /** 
     * @name PESA Algorithm
     */
    //@{
    /**
     * The PESA Algorithm
     *
     * Computes the Pareto front, which will be return as list of individuals
     * in PE.
     */
    bool pesa(std::vector<individual>& PE,const bool initFromLog=false);

    /**
     * Insert non-dominated member from PI to PE
     *
     * Return the number of elements of PI that were inserted in PE.
     */ 
    int insert(std::vector<individual>& PI,
               std::vector<individual>& PE);

    /**
     * Insert non-dominated member into PE
     */ 
    bool insert(individual& genotype,
                std::vector<individual>& PE);

    /**
     * Returns a random individual in the given population, which has
     * been selected because it had a smaller squeeze factor in a binary
     * tournament.
     */
    int binaryTournament(const std::vector<individual>& PE) const;
    
    /**
     * Return true if a>b (a dominates b) after the definition used in the
     * Pareto literature: 
     *
     * a>b <=> for all i a[i]>=b[i] and it exists one i such that a[i]>b[i]
     * 
     * The arguments a and b represent here multidimensional fitness values.
     */
    bool dominate(const dvector& a,
                  const dvector& b) const;
    
    /**
     * Shadow for the parameter mutation rate, fixed to contain only
     * values between 0 and 1
     */
    double mutationRate;
    //@}

    /**
     * LUT-based method for computing g(x) = exp(-x^2).
     * The value given is x and not x^2
     *
     * Only values between 0 and 3 will produce something, all the rest
     * produce zero as output
     */
    static const double* expLUT;
    
    /**
     * Initialize expLUT
     */
    bool initExpLUT();

    /**
     * An efficient way to compute g(x)=exp(-x^2)
     */
    inline double exp2(const double& x) const;

    /**
     * Compute the fitness distance between the given two fitness points
     */
    inline double fitnessDistance(const dvector& a,
                                  const dvector& b) const;
    
    /**
     * Log all evaluation
     */
    bool logEvaluations;

    /**
     * All individuals not belonging to the pareto front are somehow dead!
     *
     * This will be used only if logEvaluations is true
     */
    std::list<individual> deadIndividuals;

    /**
     * Functor used everywhere to generate values between 0 and 1.
     */
    continuousRandomDistribution crdist;

    /**
     * Bounding box.
     *
     * The size of this matrix will be:
     * 2 x parameters::fitnessSpaceDimensionality
     */
    dmatrix bbox;

    /**
     * Sigmas
     *
     * The fitness space grid size will be used to compute the std. deviation
     * per each axis.
     */
    dvector sigmas;

    /**
     * Initialize log.
     *
     * This method reinitializes the log.  It writes the functor parameters
     * and internal configuration.
     */
    bool initLog();

    /**
     * Make an entry for the individual in the log file (if desired).
     *
     * @return true if the logging is activated, or false if no log was
     *         desired.
     */
    bool logEntry(const individual& ind,const bool markDead=false);

    /**
     * Initialize the bounding box
     */
    void initBoundingBox(dmatrix& boundingBox) const;

    /**
     * Update bounding box considering the given fitness space point.
     *
     * @return true if there was a change in the bounding box, false if
     * given point was already within the bounding box.
     */
    bool updateBoundingBox(const dvector& pnt,
                                 dmatrix& boundingBox) const;

    /**
     * Update fitness space subdivision.
     *
     * This initializes the sigmas based on the bounding box contents.
     */
    void updateFitnessSpaceSubdivision();

    /**
     * Update density factors.
     *
     * Recompute all squeeze factors for the individuals in the 
     * external population
     */
    void updateDensityFactors(std::vector<individual>& PE);

    /**
     * Convert a chromosome into a string, to be saved in the log file
     */
    void chromosomeToString(const chromosome& genotype,std::string& str) const;

    /**
     * Convert a string into a chromosome into a string, to be loaded from
     * the log file
     */
    void stringToChromosome(const std::string& str,
                            chromosome& genotype) const;

    /**
     * current progress %object
     */
    progressInfo* progressBox;

    /**
     * Class used to compare individuals in "scanning order"
     */
    struct scanLess;

    /**
     * Output stream used to write the log
     */
    std::ofstream* logOut;

    /**
     * Lisp-Stream-Handler used for log output
     */
    lispStreamHandler olsh;

    /**
     * Copied from the parameters
     */
    bool logFront;

    /**
     * Get data from log
     *
     * This method reads the log file and create the corresponding data.
     * Since usually the logs are broken (the user breaks the execution of
     * a long computing process), this method needs to cope with broken
     * files.
     *
     * @param logFile name of the file with the log
     * @param params parameters as written in the log file
     * @param data all data found in the log file.
     * @param boundingBox bounding box of the data
     * @return true if successful.
     */
    bool getDataFromLog(const std::string& logFile,
                        parameters& params,
                        std::vector<individual>& data,
                        dmatrix& boundingBox) const;


  private:
    /**
     * Return the gray code of the given number
     */
    inline uint32 grayCode(const uint32 i) const;

    /**
     * Return the integer value corresponding to the given gray code
     */
    inline uint32 iGrayCode(const uint32 g) const;
  };
}

#endif

