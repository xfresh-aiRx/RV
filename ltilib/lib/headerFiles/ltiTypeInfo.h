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
 * file .......: ltiTypeInfo.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 16.05.01
 * revisions ..: $Id: ltiTypeInfo.h,v 1.12 2003/11/10 06:49:54 alvarado Exp $
 */

#ifndef _LTI_TYPE_INFO_H_
#define _LTI_TYPE_INFO_H_

#include "ltiObject.h"
#include "ltiTypes.h"
#include <limits>

// Visual define this as macros
#undef min
#undef max

/**
 * \file ltiTypeInfo.h
 * Definition of some template functions that give information
 * about its template type.
 */

/*
 * namespace lti
 */
namespace lti {

  /**
   * This class allows to ask some information required in the
   * LTI-Lib for some types.
   * Note that the members are static, and you do not need any
   * instance of the class to get the type information.
   *
   * For more information about a type you can also use the 
   * std::numeric_limits type of the Standard Template Library.
   *
   * Example:
   * \code
   * if (typeInfo<double>::isFloatingPointType()) {
   *   cout << "double is a floating point type" << endl;
   * } else {
   *   cout << "this is a really weird case!" << endl;
   * }
   * \endcode
   */
  template<class T>
  class typeInfo : public object {
  public:
    /**
     * Type suggested for accumulation of current type elements
     * (for example int for ubyte).
     */
    typedef T accumulation_type;

    /**
     * Type suggested to accumulate the square of values of the current
     * type
     */
    typedef T square_accumulation_type;

    /**
     * suggest a norm for the given type.
     * Usually 255 is used for ubyte, 127 for byte, 65535 for all other
     * integer types and 1.0 for the floating point types.
     */
    static T suggestedNorm() throw() {return 65535;}

    /**
     * return true if the type T is a floating point type.
     */
    static bool isFloatingPointType() throw() {return false;}

    /**
     * Return a const char* with the name of the type
     */
    static const char* name() throw() {return "";}

    /**
     * The difference of this minimum with the std::numeric_limits<T>::min() is
     * that here the minimum value is returned for floating point types and
     * fixed point types.  The STL method returns for floating point values
     * the minimal representable value above zero.  For max() you can use
     * the standard version
     */
    static T min() throw() {return std::numeric_limits<T>::min();}
  };

  // ------------------------
  // template specializations
  // ------------------------

  template<>
  class typeInfo<ubyte> : public object {
  public:
    typedef int accumulation_type;
    typedef int square_accumulation_type;
    static ubyte suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "ubyte";};
    static ubyte min() throw() {return std::numeric_limits<ubyte>::min();}
  };

  template<>
  class typeInfo<byte> : public object {
  public:
    typedef int accumulation_type;
    typedef int square_accumulation_type;
    static byte suggestedNorm() throw() {return 127;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "byte";}
    static byte min() throw() {return std::numeric_limits<byte>::min();}
  };

  template<>
  class typeInfo<short int> : public object {
  public:
    typedef int accumulation_type;
    typedef double square_accumulation_type;
    static short int suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "short int";}
    static short int min() throw() {
      return std::numeric_limits<short int>::min();
    }
  };

  template<>
  class typeInfo<unsigned short int> : public object {
  public:
    typedef int accumulation_type;
    typedef double square_accumulation_type;
    static unsigned short int suggestedNorm() throw() {return 255;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "unsigned short int";}
    static unsigned short int min() throw() {
      return std::numeric_limits<unsigned short int>::min();
    }
  };

  template<>
  class typeInfo<int> : public object {
  public:
    typedef int accumulation_type;
    typedef double square_accumulation_type;
    static int suggestedNorm() throw() {return 65535;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "int";}
    static int min() throw() {return std::numeric_limits<int>::min();}
  };

  template<>
  class typeInfo<unsigned int> : public object {
  public:
    typedef int accumulation_type;
    typedef double square_accumulation_type;
    static unsigned int suggestedNorm() throw() {return 65535;}
    static bool isFloatingPointType() throw() {return false;}
    static const char* name() throw() {return "unsigned int";}
    static unsigned int min() throw() {
      return std::numeric_limits<unsigned int>::min();
    }
  };

  template<>
  class typeInfo<float> : public object {
  public:
    typedef float accumulation_type;
    typedef double square_accumulation_type;
    static float suggestedNorm() throw() {return 1.0f;}
    static bool isFloatingPointType() throw() {return true;}
    static const char* name() throw() {return "float";};
    static float min() throw() {return -std::numeric_limits<float>::max();}
  };

  template<>
  class typeInfo<double> : public object {
  public:
    typedef double accumulation_type;
    typedef double square_accumulation_type;
    static double suggestedNorm() throw() {return 1.0;}
    static bool isFloatingPointType() throw() {return true;}
    static const char* name() throw() {return "double";};
    static double min() throw() {return -std::numeric_limits<double>::max();}
  };
}


#endif

