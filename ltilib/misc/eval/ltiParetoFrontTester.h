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
 * revisions ..: $Id: ltiParetoFrontTester.h,v 1.2 2004/05/03 12:20:57 alvarado Exp $
 */

#ifndef _LTI_PARETO_FRONT_TESTER_H_
#define _LTI_PARETO_FRONT_TESTER_H_

#include "ltiParetoFront.h"

namespace lti {

  /**
   * A dummy functor that is evaluated with the paretoFront
   */
  class dummyFunctor : public functor {
  public:
    /**
     * The parameters for the class dummyFunctor
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
       * The time the functor takes is given by this parameter.
       */
      float linear;

      /**
       * A second quality measure is given by this.
       */
      float quadratic;

    };

    /**
     * Default constructor
     */
    dummyFunctor();

    /**
     * Construct a functor using the given parameters
     */
    dummyFunctor(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    dummyFunctor(const dummyFunctor& other);

    /**
     * Destructor
     */
    virtual ~dummyFunctor();

    /**
     * Returns the name of this type ("dummyFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src double with the source data.
     * @param dest double where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const double& src,double& dest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dummyFunctor& copy(const dummyFunctor& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    dummyFunctor& operator=(const dummyFunctor& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;
  };
  
  // -------------------------------------------------------------------------
  //
  // P A R E T O   T E S T E R
  //
  // -------------------------------------------------------------------------



  /**
   * The Pareto Front Tester functor is a simple example of an evaluation
   * framework using the Pareto Front class. 
   */
  class paretoFrontTester : public paretoFront {
  public:

    /**
     * Default constructor
     */
    paretoFrontTester();

    /**
     * Construct a functor using the given parameters
     */
    paretoFrontTester(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    paretoFrontTester(const paretoFrontTester& other);

    /**
     * Destructor
     */
    virtual ~paretoFrontTester();

    /**
     * Returns the name of this type ("paretoFrontTester")
     */
    virtual const char* getTypeName() const;

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * @name Public methods to be reimplemented
     */
    //@{

    /**
     * Convert a binary-chain representation of a chromosome to a valid
     * parameter object.
     */
    virtual bool chromosomeToPhenotype(const chromosome& genotype,
                                       functor::parameters& phenotype) const;

    /**
     * Return a fresh allocated parameters for the evaluated functor, which is
     * equivalent to the given genotype.
     */
    virtual functor::parameters* 
    chromosomeToPhenotype(const chromosome& genotype) const;

    /**
     * Convert a valid parameters object (phenotype) into binary-chain
     * representation of a chromosome.
     */
    virtual bool phenotypeToChromosome(const functor::parameters& phenotype,
                                       chromosome& genotype) const;

    /**
     * Return the length in bits for a chromosome.
     *
     * This method needs to be reimplemented, in order to get some 
     * default implementations to work.
     */
    virtual int getChromosomeSize() const;

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
     * analyzed with this functor of this kind: empirical goodness and
     * empirical discrepancy (Zhang).  The empirical goodness computes some
     * measure using exclusively the test data, without requiring any ground
     * truth.  The empirical discrepancy assumes the existency of ground truth
     * and provides as measure some distance between the result of an algorithm
     * and the ground truth.  Each class derived from paretoFrontTester should
     * specify clearly which kind of fitness measures it provides.
     * 
     */
    virtual bool evaluateChromosome(const chromosome& individual,
                                    dvector& fitness); 

    //@}

  protected:
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
     * The default implementation does a so called uniform crossover, in
     * which each pair of corresponding bits are exchanged with a 
     * probability of 0.5.
     */
    virtual bool crossover(const chromosome& parent1,
                           const chromosome& parent2,
                           chromosome& child);

    //@}

  };
}

#endif

