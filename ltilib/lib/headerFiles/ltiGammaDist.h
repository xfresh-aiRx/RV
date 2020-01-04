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
 * file .......: ltiGammaDist.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 21.04.99
 * revisions ..: $Id: ltiGammaDist.h,v 1.9 2005/02/02 14:16:23 doerfler Exp $
 */
#ifndef _LTI_GAMMADIST_H_
#define _LTI_GAMMADIST_H_

#include "ltiContinuousRandDist.h"

namespace lti {
  /**
   * Gamma distributed random number class.
   */
  class gammaDistribution : public continuousRandomDistribution {
  public:
    /**
     * parameters for the gamma distribution
     */
    class parameters : public continuousRandomDistribution::parameters {
    public:

      /**
       * default constructor
       */
      parameters() : continuousRandomDistribution::parameters(),order(1) {};

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
       * the order
       */
      int order;

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
          (continuousRandomDistribution::parameters::*p_copy)(const continuousRandomDistribution::parameters&) =
          continuousRandomDistribution::parameters::copy;
        (this->*p_copy)(other);
#endif

        order = other.order;

        return (*this);
      };

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
        return "gammaDistribution::parameters";
      };
    };

    /**
     * default constructor.
     */
    gammaDistribution() : continuousRandomDistribution() {};

    /**
     * constructor, sets the parameters
     */
    gammaDistribution(const parameters& theParams);

    /**
     * constructor, sets order theOrder
     */
    gammaDistribution(const int theOrder);

    /**
     * destructor
     */
    virtual ~gammaDistribution() {};

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * gamma distribution. Returns a deviate distributed  as a gamma
     * distribution of the given integer order, i.e. a waiting time to the
     * (theOrder)th event in a Poisson process of unit mean.
     */
    virtual double draw() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new gammaDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "gammaDistribution";
    };
  };
}

#endif

