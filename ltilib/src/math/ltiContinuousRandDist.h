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
 * file .......: ltiRandDist.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 21.04.99
 * revisions ..: $Id: ltiContinuousRandDist.h,v 1.8 2005/02/02 14:16:22 doerfler Exp $
 */

#ifndef _LTI_CONTINUOUSRANDDIST_H_
#define _LTI_CONTINUOUSRANDDIST_H_

#include "ltiRandDist.h"

namespace lti {
    /**
     * Base random number class.
     *
     *
     */
  class continuousRandomDistribution : public randomDistribution {
  public:

    /**
     * parameters of the random distribution
     */
    class parameters : public randomDistribution::parameters {
    public:
      /**
       * copy data of "other" parameters
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        randomDistribution::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        randomDistribution::parameters&
          (randomDistribution::parameters::*p_copy)(const randomDistribution::parameters&) =
          randomDistribution::parameters::copy;
        (this->*p_copy)(other);
#endif
        
        return *this;
      };
      
    };

    /**
     * default constructor.
     *
     * Initializes the pseudo-random number generator using system time.
     * If reInit is false, the generator is initialized only when
     * instantiating the first continuousRandomDistribution object.
     */
    continuousRandomDistribution(bool reInit=false);

    /**
     * this constructor initializes the pseudo-random number generator
     * using the given value
     */
    continuousRandomDistribution(const unsigned int theValue);

    /**
     * destructor
     */
    virtual ~continuousRandomDistribution();

    /**
     * draws a number from the distribution. For this class the values
     * are distributed uniformly in [0;1).  This method always (for
     * any distribution) returns a value between 0.0 (inclusive) and
     * 1.0 (exclusive).
     */
    inline virtual double draw() const {
      return random();
    }

    /**
     * draws a number from the specified distribution using the virtual
     * function draw().
     */
    template<class T>
    bool apply(T& theObject) const {
      theObject = T(draw());
      return true;
    };

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new continuousRandomDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "continuousRandomDistribution";
    };

    /**
     * copy member
     */
    continuousRandomDistribution& 
    copy(const continuousRandomDistribution& other);

  };
}

#endif

