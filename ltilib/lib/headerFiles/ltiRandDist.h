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
 * revisions ..: $Id: ltiRandDist.h,v 1.16 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_RANDDIST_H_
#define _LTI_RANDDIST_H_

#include "ltiFunctor.h"

namespace lti {
  /**
   * Base random number class.
   *
   * You can use an instance of this class to initialize the random
   * number generator.
   */
  class randomDistribution : public functor {
  public:

    /**
     * parameters of the random distribution
     */
    class parameters : public functor::parameters {
    public:
      /**
       * copy data of "other" parameters
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::*p_copy)(const functor::parameters&) =
          functor::parameters::copy;
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
     * instantiating the first randomDistribution object.
     */
    randomDistribution(bool reInit=false);

    /**
     * this constructor initializes the pseudo-random number generator
     * using the given value
     */
    randomDistribution(const unsigned int theValue);

    /**
     * destructor
     */
    virtual ~randomDistribution();

    /**
     * re-initializes the random number generator using system time
     */
    void init() const;

    /**
     * re-initializes the random number generator using the given value
     */
    void init(const unsigned int theValue) const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new randomDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {return "randomDistribution";};

    /**
     * copy member
     */
    randomDistribution& copy(const randomDistribution& other);

  protected:
    /**
     * This is a wrapper for the rand() function that returns a value
     * between 0.0 (inclusive) and 1.0 (exclusive).
     */
    inline double random() const {
      static const double randmax = (RAND_MAX+1.0);
      return rand()/randmax;
    }
    
  private:
    static bool initialized;
  };
}

#endif

