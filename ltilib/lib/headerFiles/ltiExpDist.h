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
 * file .......: ltiExpDist.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 21.04.99
 * revisions ..: $Id: ltiExpDist.h,v 1.6 2003/02/17 07:16:40 alvarado Exp $
 */

#ifndef _LTI_EXPDIST_H_
#define _LTI_EXPDIST_H_

#include "ltiContinuousRandDist.h"

namespace lti {
  /**
   * Exponentially distributed random number class
   *
   * see for example: Press, Vetterling, Teukolsky, Flannery
   *                  Numerical Recipes in C, 2nd edition
   *                  Cambridge University Press, 1992
   *                  p. 287
   */
  class exponentialDistribution : public continuousRandomDistribution {
  public:
    /**
     * default constructor.
     */
    exponentialDistribution():continuousRandomDistribution() {};

    /**
     * destructor
     */
    virtual ~exponentialDistribution() {};

    /**
     * returns an exponentially distributed, positive, random deviate of
     * unit mean.
     */
    virtual double draw() const;

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new exponentialDistribution(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "exponentialDistribution";
    };
  };
}

#endif

