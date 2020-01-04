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
 * file .......: ltiNoise.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 06.05.99
 * revisions ..: $Id: ltiNoise.h,v 1.11 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_NOISE_H_
#define _LTI_NOISE_H_

#include "ltiIteratingFunctor.h"
#include "ltiContinuousRandDist.h"
#include <limits>

namespace lti {
  /** Noise class.
      Apply puts noise of the specified distribution on the elements of
      the given MathObject. */
  template<class T>
  class noise : public iteratingFunctor<T> {
  public:
    /// scalarParameter class
    class parameters : public iteratingFunctor<T>::parameters {
    public:
      /// lower limit of the type T
      T lowerLimit;

      /// upper limit of the type T
      T upperLimit;

      /// default constructor
      parameters() : iteratingFunctor<T>::parameters() {
        distribution=0;
        lowerLimit=(std::numeric_limits<T>::min)();
        upperLimit=(std::numeric_limits<T>::max)();
      };

      /// copy constructor
      parameters(const parameters& other)
        : iteratingFunctor<T>::parameters() {
        copy(other);
      };

      /// destructor
      ~parameters() {
        delete distribution;
        distribution = 0;
      };

      /// sets the noise distribution
      void setDistribution(const continuousRandomDistribution& theDistribution) {
        delete distribution;
        distribution =
          dynamic_cast<continuousRandomDistribution*>(theDistribution.clone());
      };

      /// returns current noise distribution.
      const continuousRandomDistribution& getDistribution() const {
        if (isNull(distribution)) {
          throw functor::invalidParametersException(getTypeName());
        }
        return *distribution;
      }

      /// copy data of "other" parameters
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        iteratingFunctor<T>::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        iteratingFunctor<T>::parameters&
          (iteratingFunctor<T>::parameters::* p_copy)
          (const iteratingFunctor<T>::parameters&) =
          iteratingFunctor<T>::parameters::copy;
        (this->*p_copy)(other);
#endif

        distribution =
          dynamic_cast<continuousRandomDistribution*>(other.getDistribution().clone());
        lowerLimit=other.lowerLimit;
        upperLimit=other.upperLimit;

        return (*this);
      };

      /// returns a pointer to a clone of the parameters.
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };

      /// returns the name of this type
      virtual const char* getTypeName() const {
        return "noise::parameters";
      };

    protected:
      /// the random distribution to be used.
      continuousRandomDistribution* distribution;
    };

    /// default constructor
    noise() {};

    /// constructor, sets the parameters, i.e. the noise distribution.
    noise(const parameters& theParams);

    /// constructor, sets the noise distribution
    noise(const continuousRandomDistribution& theDistribution);

    /// destructor
    virtual ~noise() {};

    /// returns the current parameters.
    const parameters& getParameters() const;

    /** onPlace version of apply.
        applies the functor's function to 'theObject'. */
    virtual vector<T>& apply(vector<T>& theObject) const;

    /** onCopy version of apply.
        applies the functor's function to the copy of 'theObject',
        'newObject'. */
    virtual vector<T>& apply(const vector<T>& theObject,
                             vector<T>& newObject) const;

    /** onPlace version of apply.
        applies the functor's function to 'theObject'. */
    virtual matrix<T>& apply(matrix<T>& theObject) const;

    /** onCopy version of apply.
        applies the functor's function to the copy of 'theObject',
        'newObject'. */
    virtual matrix<T>& apply(const matrix<T>& theObject,
                             matrix<T>& newObject) const;

    /// returns a pointer to a clone of the functor.
    virtual functor* clone() const { return (new noise<T>(*this));};

    /// returns the name of this type
    virtual const char* getTypeName() const {return "noise";};
  };
}

#endif

