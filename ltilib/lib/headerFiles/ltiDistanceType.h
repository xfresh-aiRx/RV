/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiDistanceType.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 11.05.2003
 * revisions ..: $Id: ltiDistanceType.h,v 1.3 2003/11/06 14:25:20 sltilib Exp $
 */

#ifndef _LTI_DISTANCE_TYPE_H_
#define _LTI_DISTANCE_TYPE_H_

#include "ltiObject.h"
#include "ltiTypeInfo.h"

/**
 * \file ltiDistanceType.h This file contains the class distanceType<T>
 * which specifies three types that are recommended for the result of
 * a distance calculation between two instances of T. The three types are:
 * - \a distance_type is used when a simple distance is calculated
 *   between two instances of T which does not involve accumulation of
 *   squares or typical floating point operations.
 * - \a square_distance_type is used for the result of distances
 *   that involve the square of the type T.
 * - \a fp_distance_type is used for distances that involve floating
 *   point operations such as the sqrt or log functions.
 */

/**
 * namespace lti
 */
namespace lti {

  /**
   * This class defines the appropriate return types when calculating
   * the distance between to values of type T. These can be simple
   * types like int or double or lti types like vector<T>, rgbPixel
   * etc. There are three different distance types:
   * - \a distance_type is used when a simple distance is calculated
   *   between two instances of T which does not involve accumulation of
   *   squares or typical floating point operations.
   * - \a square_distance_type is used for the result of distances
   *   that involve the square of the type T.
   * - \a fp_distance_type is used for distances that involve floating
   *   point operations such as the sqrt or log functions.
   */
  template<class T>
  class distanceType : public object {
  public:
    /**
     * Suggested distance type for distances that use simple
     * accumulation of type elements (for example l1Distantor).
     */
    typedef typename distanceType<typename T::value_type>::distance_type
    distance_type;

    /**
     * Suggested distance type for distances that use
     * accumulation of squares of type elements (for example l2SqrDistantor).
     */
    typedef typename distanceType<typename T::value_type>::square_distance_type
    square_distance_type;

    /**
     * Suggested distance type for distances that use accumulation of
     * type elements or their squares and use a typical floating point (fp)
     * operation like sqrt or log on that. (for example l2Distantor).
     */
    typedef typename distanceType<typename T::value_type>::fp_distance_type
    fp_distance_type;
  };

  // ------------------------
  // template specializations
  // ------------------------

#ifndef _LTI_MSC_6

  // This is the way is should be

  template<class T>
  class distanceType< tpoint<T> > : public object {
  public:
    typedef T distance_type;
    typedef T square_distance_type;
    typedef double fp_distance_type;
  };

  template<class T>
  class distanceType< tpoint3D<T> > : public object {
  public:
    typedef T distance_type;
    typedef T square_distance_type;
    typedef double fp_distance_type;
  };

  template<class T>
  class distanceType< trgbPixel<T> > : public object {
  public:
    typedef typename distanceType<T>::distance_type distance_type;
    typedef typename distanceType<T>::distance_type square_distance_type;
    typedef double fp_distance_type;
  };

#else

  // Endless code duplication for MSC 6
  // keeping it to the most typical instantiations

  template<>
  class distanceType<point> : public object {
  public:
    typedef int distance_type;
    typedef int square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType< tpoint<float> > : public object {
  public:
    typedef float distance_type;
    typedef float square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<dpoint> : public object {
  public:
    typedef double distance_type;
    typedef double square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<point3D> : public object {
  public:
    typedef int distance_type;
    typedef int square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType< tpoint3D<float> > : public object {
  public:
    typedef float distance_type;
    typedef float square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<dpoint3D> : public object {
  public:
    typedef double distance_type;
    typedef double square_distance_type;
    typedef double fp_distance_type;
  };
  
  template<>
  class distanceType< trgbPixel<int> > : public object {
  public:
    typedef int distance_type;
    typedef int square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType< trgbPixel<float> > : public object {
  public:
    typedef float distance_type;
    typedef float square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType< trgbPixel<double> > : public object {
  public:
    typedef double distance_type;
    typedef double square_distance_type;
    typedef double fp_distance_type;
  };


#endif

  template<>
  class distanceType<ubyte> : public object {
  public:
    typedef typeInfo<ubyte>::accumulation_type distance_type;
    typedef typeInfo<ubyte>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<byte> : public object {
  public:
    typedef typeInfo<byte>::accumulation_type distance_type;
    typedef typeInfo<byte>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<short int> : public object {
  public:
    typedef typeInfo<short int>::accumulation_type distance_type;
    typedef typeInfo<short int>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<unsigned short int> : public object {
  public:
    typedef typeInfo<unsigned short int>::accumulation_type distance_type;
    typedef typeInfo<unsigned short int>::square_accumulation_type 
    square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<int> : public object {
  public:
    typedef typeInfo<int>::accumulation_type distance_type;
    typedef typeInfo<int>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<unsigned int> : public object {
  public:
    typedef typeInfo<unsigned int>::accumulation_type distance_type;
    typedef typeInfo<unsigned int>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<float> : public object {
  public:
    typedef typeInfo<float>::accumulation_type distance_type;
    typedef typeInfo<float>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };

  template<>
  class distanceType<double> : public object {
  public:
    typedef typeInfo<double>::accumulation_type distance_type;
    typedef typeInfo<double>::square_accumulation_type square_distance_type;
    typedef double fp_distance_type;
  };


}


#endif

