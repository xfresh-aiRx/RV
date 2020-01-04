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
 * file .......: ltiConstants.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 16.05.01
 * revisions ..: $Id: ltiConstants.h,v 1.6 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_CONSTANTS_H_
#define _LTI_CONSTANTS_H_

#include "ltiObject.h"
#include "ltiTypes.h"

/**
 * \file ltiConstants.h
 * Definition of a template class containing usual mathematical and
 * physical constants.
 */

namespace lti {

  /**
   * This class is a container of some mathematical and physical constants.
   *
   * Example:
   * \code
   * constants<float>::Pi
   * \endcode
   */
  template<class T>
  class constants : public object {
  public:
    /**
     * Pi
     */
    static inline T Pi() throw() {
      return static_cast<T>(3.1415926535897932384626433832795);
    };

    /**
     * Twice Pi (2*Pi)
     */
    static inline T TwoPi() throw() {
      return static_cast<T>(3.1415926535897932384626433832795*2.0);
    };

    /**
     * Half Pi (Pi/2)
     */
    static inline T HalfPi() throw() {
      return static_cast<T>(3.1415926535897932384626433832795/2.0);
    };

    /**
     * e (basis of natural logarithm) = 2.7182818284590452353602874713526...
     */
    static inline T e() throw() {
      return static_cast<T>(2.7182818284590452353602874713526);
      // continuing with ... 62497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260
      // more at http://sources.wikipedia.org/wiki/E_to_10%2C000_places
    };

    /**
     * square root of 2 = 1.4142135623730950488016887242097...
     */
    static inline T sqrt2() throw() {
      return static_cast<T>(1.4142135623730950488016887242097);
    };

  protected:
    /**
     * Protected constructor avoids the creation of
     * an instance of this class.
     */
    constants() {};
  };
}


#endif

