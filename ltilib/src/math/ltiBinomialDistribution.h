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
 * file .......: ltiBinomialDistribution.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 26.3.2002
 * revisions ..: $Id: ltiBinomialDistribution.h,v 1.10 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_BINOMIAL_DISTRIBUTION_H_
#define _LTI_BINOMIAL_DISTRIBUTION_H_

#include "ltiDiscreteRandDist.h"


namespace lti {
  /**
   * This functor implements the binomial distribution. <p>
   * \f$B(n,k,p)=\binom{n}{k}p^k(1-p)^{(n-k)}\f$  with \f$n\f$ the sample size
   * \f$p\f$ the base probability and \f$k\f$ the number of events.<p>
   * It is also possible to calculate the cumulated probability<p>
   * \f$B(x\le k)=\sum_{x=0}^k\binom{n}{x}p^x(1-p)^{(n-x)}\f$<p>
   * Furthermore, the quantile \f$q\f$ for a given confidence probability
   * \f$P_c\f$ can be calcultated. With \f$B(n,q,p)\geq P_c \wedge
   * B(n,k,p)<P_c \forall k<q\f$. <p>
   * Finally, the upper bound for base probability can be estimated with
   * given \f$n\f$, \f$k\f$ and \f$P_c\f$, with the confidence probability
   * being one-sided. I.e. the probability \f$p\f$ returned is an approximate
   * solution for \f$B(n, x\le k, p) = P_c\f$. <p>
   * Of the parameters only sampleSize needs to be set for all applications of
   * the functor. The other parameters only need to be set if necessary for the
   * desired use of the functor.
   */
  class binomialDistribution : public discreteRandomDistribution {
  public:
    /**
     * the parameters for the class binomialDistribution
     */
    class parameters : public discreteRandomDistribution::parameters {
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * The size of the sample for the binomial distribution.
       * Default 1. This parameter needs to be set for all applications of
       * the functor.
       */
      int sampleSize;

      /**
       * The number of positive events. Default 0. It only needs to be set
       * for evaluating the upper bound for the base probability if the
       * method not having this argument is used.
       * @see upperBound()
       */
      int events;

      /**
       * Here called confidence probability. Default 0.95. It is needed for
       * the calculation of the quantile() and the upperBound().
       */
      double confidence;

      /**
       * The accuracy in the iterative calculation of the upperBound(). The
       * default is 0.0001.
       */
      double accuracy;

      /**
       * The base probability of a event. Default 0.5.
       */
      double baseProbability;

    };

    /**
     * default constructor
     */
    binomialDistribution();

    /**
     * constructor. Sets the number of samples
     * @param samples the number of samples
     */
    binomialDistribution(const int& samples);

    /**
     * constructor. Set the number of samples and the base probability.
     * Optionally, the confidence probabilty can be set for calculating the
     * quantile.
     * @param samples the number of samples
     * @param baseProb base probability
     * @param conf confidence probability
     */
    binomialDistribution(const int& samples, const double& baseProb,
                         const double& conf=0.95);

    /**
     * constructor used for calculating the upperBound().
     * @param samples the number of samples
     * @param posEvents number of positiv events
     * @param conf confidence probability
     */
    binomialDistribution(const int& samples, const int& posEvents,
                         const double& conf);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    binomialDistribution(const binomialDistribution& other);

    /**
     * destructor
     */
    virtual ~binomialDistribution();

    /**
     * returns the name of this type ("binomialDistribution")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /**
     * Returns a binomial random number, i.e. zero or one. 1 is returned if
     * the event occurs, 0 if it doesn't.
     * @return binomial random number.
     */
    virtual int draw() const;

    /**
     * Returns the probability for exactly k events with the given parameters.
     * If k is out of bounds, zero is returned.
     * @param k number of positive events
     * @return probability of k events
     */
    double pdf(const int& k) const;

    /**
     * Returns the probability for base probability p with the given
     * parameters.
     * If p is out of bounds zero is returned.
     * @param p base probability of an event
     * @return probability of k events (paramters) with base probability p
     */
    double pdf(const double& p) const;

    /**
     * Returns the probability for k events and base probability p with
     * the given parameters.
     * If p is out of bounds zero is returned.
     * If k is out of bounds, zero is returned.
     * @param k number of positive events
     * @param p base probability of an event
     * @return probability of k events (paramters) with base probability p
     */
    double pdf(const int& k, const double& p) const;

    /**
     * Returns the probability for k events, base probability p and
     * number of samples n
     * If p is out of bounds zero is returned.
     * If k is out of bounds, zero is returned.
     * @param k number of positive events
     * @param p base probability of an event
     * @param n number of samples
     * @return probability of k events (paramters) with base probability p
     */
    double pdf(const int& k, const double& p, const int& n) const;

    /**
     * Returns the cumulated probability for greater equal k events
     * with the given parameters.
     * If k is out of bounds, zero is returned.
     * @param k max number of positive events
     * @return probability of up to k events
     */
    double cdf(const int& k) const;

    /**
     * Returns the cumulated probability for greater equal k events as set
     * in the parameters. The base probability in the argument is used.
     * If p is out of bounds it is truncated to 0 or 1.
     * @param p base probability of an event
     * @return probability of up to k events (parameters) with base prob p.
     */
    double cdf(const double& p) const;

    /**
     * Returns the cumulated probability for greater equal k events,
     * base probability p, and sample size n.
     *
     * If k is out of bounds, zero is returned.
     * If p is out of bounds it is truncated to 0 or 1.
     * @param k max number of positive events
     * @param p base probability of an event
     * @param n number of samples
     * @return probability of up to k events (parameters) with base prob p.  */
    double cdf(const int& k, const double& p, const int& n) const;

    /**
     * Returns the quantile for a given confidence probability. For the
     * number of samples and the base probability, the parameters are used.
     * @param confProb confidence
     * @return the quantile
     */
    int quantile(const double& confProb) const;

    /**
     * Returns an approximated upper bound for the base probability of an
     * event given the number of events. The number of samples and the
     * confidence probability are taken from the parameters. If no solution
     * can be found, a value greater than one is returned.
     * @param k number of positive events
     * @return upper bound for the base probability
     */
    double upperBound(const int& k) const;

    /**
     * Returns an approximated upper bound for the base probability of an
     * event given the number of events and the number of samples. The
     * confidence probability is taken from the parameters. If no solution
     * can be found, a value greater than one is returned.
     * @param k number of positive events
     * @param n number of samples
     * @return upper bound for the base probability
     */
    double upperBound(const int& k, const int& n) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    binomialDistribution& copy(const binomialDistribution& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    binomialDistribution& operator=(const binomialDistribution& other);

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

  };
}

#endif

