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
 * file .......: ltiGaussDist.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 21.04.99
 * revisions ..: $Id: ltiGaussDist.h,v 1.11 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_GAUSSDIST_H_
#define _LTI_GAUSSDIST_H_

#include "ltiContinuousRandDist.h"

namespace lti {
  /**
   * Normal distributed random number class.
   */
  class gaussianDistribution : public continuousRandomDistribution {
  public:

    /**
     * parameters for gaussian distribution
     */
    class parameters : public continuousRandomDistribution::parameters {
    public:
      /**
       * default constructor
       */
      parameters()
        : continuousRandomDistribution::parameters(),sigma(1.0),mu(.0) {};

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
       * standard deviation
       */
      double sigma;

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

	sigma = other.sigma;
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
	return "gaussianDistribution::parameters";
      };
    };

    /**
     * default constructor. Sets mean and standard deviation and mean.
     */
    gaussianDistribution(const double mean=0.0,
                         const double stdDeviation=1.0);

    /**
     * constructor, sets the parameters
     */
    gaussianDistribution(const parameters& theParams);

    /**
     * destructor
     */
    virtual ~gaussianDistribution() {};

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * draws a number from the distribution.
     */
    virtual double draw() const;

    /**
     * draw a number for the distribution with the given average and
     * std. deviation
     */
    double draw(const double& average, const double& stdDeviation) const;

    /**
     * return a gaussian distributed random number.
     * @param average mean value of the distribution
     * @param stdDeviation std. deviation of the distribution
     * @param result value from the distribution
     * @return true if successful, false otherwise.
     */
    bool apply(const double& average,
               const double& stdDeviation,
               double& result) const;

    /**
     * copy data of "other" functor.
     */
    gaussianDistribution& copy(const gaussianDistribution& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new gaussianDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "gaussianDistribution";
    };

  protected:
    mutable double V1, V2, S;
    mutable int phase;
  };
}

#endif

