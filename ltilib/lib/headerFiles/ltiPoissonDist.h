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
 * file .......: ltiPoissonDist.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 21.04.99
 * revisions ..: $Id: ltiPoissonDist.h,v 1.10 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_POISSONDIST_H_
#define _LTI_POISSONDIST_H_

#include "ltiContinuousRandDist.h"

namespace lti {
  /**
   * Poisson distributed random number class.
   */
  class poissonDistribution : public continuousRandomDistribution {
  public:
    /**
     * parameters class for the poisson distribution
     */
    class parameters : public continuousRandomDistribution::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : mu(1) {};

      /**
       * copy constructor
       */
      parameters(const parameters& other) 
        : continuousRandomDistribution::parameters() {copy(other);};

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * mean
       */
      double mu;

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

	mu = other.mu;

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
	return "poissonDistribution::parameters";
      };
    };

    /**
     * default constructor.
     */
    poissonDistribution() : continuousRandomDistribution() {};

    /**
     * constructor, sets mean.
     */
    poissonDistribution(const double mean);

    /**
     * constructor, sets the parameters.
     */
    poissonDistribution(const parameters& theParams);

    /**
     * destructor
     */
    virtual ~poissonDistribution() {};

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * returns as a floating-point number an integer value that is a
     * random deviate drawn from a Poisson distribution of the given mean.
     */
    virtual double draw() const;

    /**
     * copy data of "other" functor.
     */
    poissonDistribution& copy(const poissonDistribution& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new poissonDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "poissonDistribution";
    };

  protected:
    mutable double sq, alxm, g, oldm;
  };
}

#endif


