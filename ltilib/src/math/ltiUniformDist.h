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
 * file .......: ltiUniformDist.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.05.99
 * revisions ..: $Id: ltiUniformDist.h,v 1.13 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_UNIFORMDIST_H_
#define _LTI_UNIFORMDIST_H_

#include "ltiContinuousRandDist.h"

namespace lti {
  /**
   * Uniform distributed random number class.
   */
  class uniformDistribution : public continuousRandomDistribution {
  public:
    /**
     * parameters class for the uniform distribution
     */
    class parameters : public continuousRandomDistribution::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : lowerLimit(0),upperLimit(1) {};

      /**
       * copy constructor
       */
      parameters(const parameters& other)
        : continuousRandomDistribution::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * lower limit (inclusive)
       */
      double lowerLimit;

      /**
       * upper limit (exclusive)
       */
      double upperLimit;

      /**
       * copy data of "other" parameters
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
	// MS Visual C++ 6 is not able to compile this...
	continuousRandomDistribution::parameters::copy(other);
#else
	// ...so we have to use this workaround.
	// Conditional on that, copy may not be virtual.
	continuousRandomDistribution::parameters&
	  (continuousRandomDistribution::parameters::* p_copy)
	  (const continuousRandomDistribution::parameters&) =
	  continuousRandomDistribution::parameters::copy;
	(this->*p_copy)(other);
#endif

	lowerLimit = other.lowerLimit;
	upperLimit = other.upperLimit;

	return (*this);
      }

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const {
	return (new parameters(*this));
      };

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
	return "uniformDistribution::parameters";
      };
    };

    /**
     * default constructor.
     * Sets lower (inclusive) and upper (exclusive) limit of the distribution.
     */
    uniformDistribution(const double& lower=0.0,
                        const double& upper=1.0);

    /**
     * Constructor.
     *
     * Sets lower (inclusive) and upper (exclusive) limit of the distribution.
     * Additionally, it initializes the random number generation with the
     * given seed.  This is sometimes necessary, when the algorithms that uses
     * random number, still should behave "deterministically", in the sense
     * that the results should always be the same.
     *
     * The value of the seed follows the same rules as the C function srand.
     *
     * If you don't know what to give, just give "1".
     */
    uniformDistribution(const double& lower,
                        const double& upper,
                        const unsigned int seed);

    /**
     * constructor, sets the parameters
     */
    uniformDistribution(const parameters& theParams);

    /**
     * destructor
     */
    virtual ~uniformDistribution() {};

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Draws a uniform distributed random number between lowerLimit
     * (inclusive) and upperLimit (exclusive). Use lowerLimit>upperLimit if
     * you want to have numbers excluding the lower limit (=upperLimit) and
     * including the upper limit (=lowerLimit).
     */
    virtual double draw() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new uniformDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "uniformDistribution";
    };

  protected:
    /**
     * scaling constant for random number generator
     */
    double m;

    /**
     * shift constant
     */
    double b;
  };
}

#endif

