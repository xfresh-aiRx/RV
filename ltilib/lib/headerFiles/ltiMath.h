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
 * file .......: ltiMath.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.99
 * revisions ..: $Id: ltiMath.h,v 1.59 2005/11/14 15:56:51 doerfler Exp $
 */

#ifndef _LTI_MATH_H_
#define _LTI_MATH_H_

#include "ltiObject.h"
#include "ltiTypes.h"
#include "ltiSinCos.h"
#include <cmath>
#include <limits>

/**
 * \file ltiMath.h
 * Definition of some usually used global functions like min, max, round, etc.
 */

/*
 * MS Visual C++ 6.0 and old GCC versions do not provide the standard
 * cmath interface of functions for several types in the namespace std.
 * For this compilers a cumbersome implementation must be provided.
 */
#undef _LTI_MATH_H_STYLE
#if (defined _LTI_MSC_6) || (defined _LTI_GNUC_2 )
#define _LTI_MATH_H_STYLE 1
#endif

// on newer gcc's isnan and others are sometimes only available in the
// namespace std. Examples are mingw and possibly gcc on Mac. 
// Pull them in here
#if defined(_LTI_GNUC_3) && !defined(_LTI_ICC) 
  using std::isnan;
  using std::isinf;
  using std::isfinite;
#endif

#ifndef _LTI_MATH_H_STYLE
#include <algorithm>
#endif

# ifdef min
#   undef min
# endif

# ifdef max
#   undef max
# endif

namespace lti {

  /**
   * Returns true if \a a is close to \a b, i.e. if \a abs(a-b) <= \a epsilon.
   *
   * This is always better than testing equality with a==b for
   * floating point types. For default \a epsilon integers are tested
   * for equality.
   *
   * \b Note: Do not used for unsigned types T.
   * 
   * @param a first value
   * @param b second value
   * @param epsilon max difference between \a a and \a b
   *                         , default std::numeric_limits<T>::epsilon()
   * @returns true if distance between \a a and \a b is <= \a epsilon 
   *
   * @ingroup gBasicMathFunc
   */
  template <class T>
  inline bool closeTo(const T& a, const T& b, 
                      const T epsilon=std::numeric_limits<T>::epsilon()) {
    const T diff=a-b;
    return ((diff<=epsilon) && (diff>=-epsilon));
  }

  
  /**
   * Returns true if \a a is close to T(0), i.e. if abs(a) <= \a epsilon.
   *
   * This is always better than testing equality with a==0. for
   * floating point types. For default \a epsilon integers are tested
   * for ==0 automagically.
   *
   * \b Note: Do not used for unsigned types T.
   *
   * @param a value
   * @param epsilon maximum absolute value of \a a 
   *                         , default std::numeric_limits<T>::epsilon()
   * @returns true if absolute value of \a a is <= \a epsilon 
   *
   * @ingroup gBasicMathFunc
   */
  template <class T>
  inline bool closeToZero(const T& a,
                          const T epsilon=std::numeric_limits<T>::epsilon()) {
    return ((a<=epsilon) && (a>=-epsilon));
  }


  /**
   * round any float type mathematicly
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T round(T x) {
    if( x >= static_cast<T>(0) ) {
      return static_cast<T>(static_cast<long>(x + 0.5));
    }

    return static_cast<T>(static_cast<long>(x - 0.5));
  }

  /**
   * round any float type mathematicly and return an integer
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline int iround(T x) {
    if( x >= 0 ) {
      return static_cast<int>(x + 0.5);
    }

    return static_cast<int>(x - 0.5);
  }

  /**
   * round any float type mathematicly and return an long integer
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline long lround(T x) {
    if( x >= 0 ) {
      return static_cast<long>(x + 0.5);
    }

    return static_cast<long>(x - 0.5);
  }

  /**
   * Conditional rounding with cast.
   *
   * Template function that rounds only if the desired output type is
   * an integer type.  Otherwise, just a static cast will be done.
   *
   * It would be nice to have this in a static_cast or dynamic_cast
   * similar syntax, but, as usuall, Visual C++ do not support ANSI
   * C++ template as it should (the bug that make this imposible is
   * documented as Q240871)
   *
   * Usage: 
   * T a;
   * condRoundCastTo(from,to)
   *
   * @param from original data with type F
   * @param to destination data, will contain \a from rounded if it is
   *           of a floating point type and \a T is an integer type. 
   *           Otherwise the types will just be casted.
   */
  template<class T,class F>
  inline void condRoundCastTo(const F from,T& to) {
    to=static_cast<T>(from);
  }

  // specializations
  template<>
  inline void condRoundCastTo<int,float>(const float x,int& dest) {
    dest=iround(x);
  }

  template<>
  inline void condRoundCastTo<int,double>(const double x,int& dest) {
    dest=iround(x);
  }

  template<>
  inline void condRoundCastTo<ubyte,float>(const float x,ubyte& dest) {
    dest=static_cast<ubyte>(iround(x));
  }

  template<>
  inline void condRoundCastTo<ubyte,double>(const double x,ubyte& dest) {
    dest=static_cast<ubyte>(iround(x));
  }

  template<>
  inline void condRoundCastTo<short,float>(const float x,short& dest) {
    dest=static_cast<short>(iround(x));
  }

  template<>
  inline void condRoundCastTo<short,double>(const double x,short& dest) {
    dest=static_cast<short>(iround(x));
  }

  template<>
  inline void condRoundCastTo<unsigned int,float>(const float x,
                                                  unsigned int& dest) {
    dest=static_cast<unsigned int>(iround(x));
  }

  template<>
  inline void condRoundCastTo<unsigned int,double>(const double x,
                                                   unsigned int& dest) {
    dest=static_cast<unsigned int>(iround(x));
  }

  template<>
  inline void condRoundCastTo<byte,float>(const float x,byte& dest) {
    dest=static_cast<byte>(iround(x));
  }

  template<>
  inline void condRoundCastTo<byte,double>(const double x,byte& dest) {
    dest=static_cast<byte>(iround(x));
  }

  template<>
  inline void condRoundCastTo<unsigned short,float>(const float x,
                                                    unsigned short& dest) {
    dest=static_cast<unsigned short>(iround(x));
  }

  template<>
  inline void condRoundCastTo<unsigned short,double>(const double x,
                                                     unsigned short& dest) {
    dest=static_cast<unsigned short>(iround(x));
  }

  // end of condRoundCastTo specializations

  /**
   * Exchange the content of the two given variables.  It works for all
   * types T that properly define the copy constructor and operator=().
   *
   * \deprecated Please use swap instead
   *
   * @param a first variable
   * @param b second variable
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline void exchange(T& a,T& b) {
    T tmp(a);
    a=b;
    b=tmp;
  }

  /**
   * Get the minimum of x and y
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T min(const T x, const T y) {
    return ((x<y) ? x : y);
  };

  /**
   * Get the maximum of x and y
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T max(const T x, const T y) {
    return ((x<y) ? y : x);
  };

  /**
   * Get the minimum of x, y and z
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T min(const T x, const T y, const T z) {
    return (min(x,min(y,z)));
  };

  /**
   * Get the maximum of x, y and z
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T max(const T x, const T y, const T z) {
    return (max(x,max(y,z)));
  };

  /**
   * Store the minimum of x and y in theMin and the maximum in theMax
   */
  template<class T>
  inline void minmax(const T x, const T y, T& theMin, T& theMax) {
    if (x<y) {
      theMin=x;
      theMax=y;
    } else {
      theMin=y;
      theMax=x;
    }
  }

  /**
   * Ensure that the minimum of the two given values is stored in theMin and
   * the maximum in theMax.
   */
  template<class T>
  inline void minmax(T& theMin, T& theMax) {
    if (theMax < theMin) {
      exchange(theMin,theMax);
    }
  }

  /**
   * Constant Pi (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  const double Pi = 3.1415926535897932; //3.1415926535897932384626433832795;

  /**
   * Constant Not a Number (NaN) (doble precision)
   *
   * @ingroup gBasicMathFunc
   */
  const double NaN = log(-1.0);

  /**
   * Constant Infinity (Inf) (double precision)
   *
   * @ingroup gBasicMathFunc
   */
  const double Inf = tan(Pi/2.0);

  /**
   * Returns the value ln( gamma(x) ) for xx>0
   *
   * @ingroup gBasicMathFunc
   */
  template<class T> T lnGamma(const T& x);

  /**
   * Returns the factorial of x 
   *
   * Note that the returned type is always the same as the input type.  So,
   * if you require very large faculty values, just cast the input to double.
   *
   * @ingroup gBasicMathFunc
   */
  double factorial(const int x);

  /**
   * Returns the natural logarithm of the factorial of x 
   *
   * @ingroup gBasicMathFunc
   */
  double lnFactorial(const int x);

  /**
   * Returns the binomial coefficient defined by
   * \f[ \begin{pmatrix} n \\ k \end{pmatrix} \frac{n!}{k!(n-k)!} 
   * \quad 0 \leq k \leq n \f]
   *
   * @ingroup gBasicMathFunc
   */
  double binomial(const int n,const int k);

  /**
   * Convert the given angle in degrees to an angle in radians
   */
  inline double degToRad(const double& deg) {
    static const double factor = Pi/180.0;
    return deg*factor;
  }

  /**
   * Convert the given angle in degrees to an angle in radians
   */
  inline float degToRad(const float& deg) {
    static const float factor = static_cast<float>(Pi/180.0);
    return deg*factor;
  }

  /**
   * Convert the given angle in degrees to an angle in radians
   */
  inline double degToRad(const int& deg) {
    static const double factor = Pi/180.0;
    return deg*factor;
  }


  /**
   * Convert the given angle in radians to an angle in degrees
   */
  inline double radToDeg(const double& rad) {
    static const double factor = 180.0/Pi;
    return rad*factor;
  }

  /**
   * Convert the given angle in radians to an angle in degrees
   */
  inline float radToDeg(const float& rad) {
    static const float factor = static_cast<float>(180.0/Pi);
    return rad*factor;
  }

  /**
   * absolute value for signed bytes
   *
   * @ingroup gBasicMathFunc
   */
  inline byte abs(byte x) {
    return ((x>=0) ? x : -x);
  }

  /**
   * absolute value for integers
   *
   * @ingroup gBasicMathFunc
   */
  inline int abs(int x) {
    return ((x>=0) ? x : -x);
  }

  /**
   * absolute value for any type
   *
   * @ingroup gBasicMathFunc
   */
  inline float abs(float x) {
#ifdef _LTI_MATH_H_STYLE
    return ((x>=0) ? x : -x);
#else
    return std::abs(x);
#endif
  }

  /**
   * absolute value for any type
   *
   * @ingroup gBasicMathFunc
   */
  inline double abs(const double& x) {
#ifdef _LTI_MATH_H_STYLE
    return ((x>=0) ? x : -x);
#else
    return std::abs(x);
#endif
  }

  /**
   * absolute difference for the given values
   * equals if (x>y) then (x-y) else (y-x).
   * 
   * Note that with complex numbers it does not return the real abs value.
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T absdiff(const T& x,const T& y) {
    return ((x>y) ? (x-y) : (y-x));
  }

  /**
   * Compute the reciprocal of the given value \e x, defined as 1/x.
   */
  inline double reciprocal(const double& x) {
    return 1.0/x;
  }

  /**
   * Compute the reciprocal of the given value \e x, defined as 1/x.
   */
  inline float reciprocal(const float& x) {
    return 1.0f/x;
  }

  /**
   * Compute 1 minus the given value.
   *
   * This function is usually employed to revert a probability value
   */
  inline double oneMinus(const double& x) {
    return 1.0-x;
  }

  /**
   * Compute 1 minus the given value
   *
   * This function is usually employed to revert a probability value
   */
  inline float oneMinus(const float& x) {
    return 1.0f-x;
  }

  /**
   * Compute 1 minus the given value
   */
  inline int oneMinus(const int& x) {
    return 1-x;
  }

  /**
   * rectify is 0 if x<0 or x otherwise
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T rectify(const T& x) {
    return ((x>=0) ? x : 0);
  }

  /**
   * square (x*x)
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T sqr(const T& x) {
    return (x*x);
  }

  /**
   * square root of integer type.  Equals floor(sqrt(x))
   *
   * @ingroup gBasicMathFunc
   */
  inline int sqrt(int x) {
    return static_cast<int>(::sqrt(static_cast<double>(x)));
  }

  /**
   * square root
   *
   * @ingroup gBasicMathFunc
   */
  inline unsigned int sqrt(unsigned int x) {
    return static_cast<unsigned int>(::sqrt(static_cast<double>(x)));
  }

  /**
   * square root
   *
   * @ingroup gBasicMathFunc
   */
  inline float sqrt(float x) {
#ifdef _LTI_MATH_H_STYLE
    // MS Visual does not support std namespace functions, which
    // can be optimized for the different types
    return static_cast<float>(::sqrt(x));
#else
    // use std method optimized for float
    return static_cast<float>(std::sqrt(x));
#endif
  }

  /**
   * square root
   *
   * @ingroup gBasicMathFunc
   */
  inline double sqrt(const double& x) {
#ifdef _LTI_MATH_H_STYLE
    // MS Visual does not support std namespace functions, which
    // can be optimized for the different types
    return static_cast<double>(::sqrt(x));
#else
    // use std method optimized for double
    return static_cast<double>(std::sqrt(x));
#endif
  }

  /**
   * square root of rectified value, i.e. returns 0 if x<=0 or sqrt(x)
   * otherwise.
   */
  template<class T>
  inline T sqrtrect(const T x) {
    if (x<=static_cast<T>(0)) {
      return static_cast<T>(0);
    }
    return sqrt(x);
  }

  // other standard functions used

  /**
   * Calculate the sine and cosine values of \p angle in one step if
   * the setup allows it.
   *
   * @param angle the angle
   * @param sval sine of the \p angle
   * @param cval cosine of the \p angle
   */
  template <class T>
  inline void sincos(T angle, T& sval, T& cval) {
    sincos_impl(static_cast<double>(angle),
                static_cast<double&>(sval),
                static_cast<double&>(cval));
  }

  // overload for float
  inline void sincos(float angle, float& sval, float& cval) {
    sincosf_impl(angle,sval,cval);
  }

    

#ifdef _LTI_MATH_H_STYLE
  /**
   * Exchange the content of the two given variables.  It works for all
   * types T that properly define the copy constructor and operator=().
   *
   * @param a first variable
   * @param b second variable
   *
   * At the end \a b contains the previous contents of \a a, which at the
   * same time contains the previous contents of \a b.
   * 
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline void swap(T& a,T& b) {
    T tmp(a);
    a=b;
    b=tmp;
  }

  /**
   * sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline float sin(float x) {
    return static_cast<float>(::sin(x));
  }

  /**
   * sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double sin(const double& x) {
    return ::sin(x);
  }

  /**
   * cosinus
   *
   * @ingroup gBasicMathFunc
   */
  inline float cos(float x) {
    return static_cast<float>(::cos(x));
  }

  /**
   * cosinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double cos(const double& x) {
    return ::cos(x);
  }

  /**
   * tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float tan(float x) {
    return static_cast<float>(::tan(x));
  }

  /**
   * tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double tan(const double& x) {
    return ::tan(x);
  }

  /**
   * arc sine
   *
   * @ingroup gBasicMathFunc
   */
  inline float asin(float x) {
    return static_cast<float>(::asin(x));
  }

  /**
   * arc sine
   *
   * @ingroup gBasicMathFunc
   */
  inline double asin(const double& x) {
    return ::asin(x);
  }

  /**
   * arc cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline float acos(float x) {
    return static_cast<float>(::acos(x));
  }

  /**
   * arc cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline double acos(const double& x) {
    return ::acos(x);
  }


  /**
   * arc tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float atan(float x) {
    return static_cast<float>(::atan(x));
  }

  /**
   * arc tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double atan(const double& x) {
    return ::atan(x);
  }

  /**
   * arc tangent of y/x
   *
   * @ingroup gBasicMathFunc
   */
  inline float atan2(float y,float x) {
    return static_cast<float>(::atan2(y,x));
  }

  /**
   * arc tangent of y/x
   *
   * @ingroup gBasicMathFunc
   */
  inline double atan2(const double& y,const double& x) {
    return ::atan2(y,x);
  }

  /**
   * hyperbolic sine
   *
   * @ingroup gBasicMathFunc
   */
  inline double sinh(const double& x) {
    return ::sinh(x);
  }

  /**
   * hyperbolic sinus
   *
   * @ingroup gBasicMathFunc
   */
  inline double sinh(float x) {
    return static_cast<float>(::sinh(x));
  }

  /**
   * hyperbolic cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline float cosh(float x) {
    return static_cast<float>(::cosh(x));
  }

  /**
   * hyperbolic cosine
   *
   * @ingroup gBasicMathFunc
   */
  inline double cosh(const double& x) {
    return ::cosh(x);
  }

  /**
   * hyperbolic tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline float tanh(float x) {
    return static_cast<float>(::tanh(x));
  }

  /**
   * hyperbolic tangent
   *
   * @ingroup gBasicMathFunc
   */
  inline double tanh(const double& x) {
    return ::tanh(x);
  }

  /**
   * natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline float log(float x) {
    return static_cast<float>(::log(x));
  }

  /**
   * natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline double log(const double& x) {
    return ::log(x);
  }

  /**
   * natural logarithm
   *
   * @ingroup gBasicMathFunc
   */
  inline double log(int x) {
    return ::log(static_cast<double>(x));
  }

  /**
   * logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline float log10(float x) {
    return static_cast<float>(::log10(x));
  }

  /**
   * logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline double log10(const double& x) {
    return ::log10(x);
  }

  /**
   * logarithm base 10
   *
   * @ingroup gBasicMathFunc
   */
  inline double log10(int x) {
    return ::log10(static_cast<double>(x));
  }

  /**
   * natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline float exp(float x) {
    return static_cast<float>(::exp(x));
  }

  /**
   * natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline double exp(const double& x) {
    return ::exp(x);
  }

  /**
   * natural exponential
   *
   * @ingroup gBasicMathFunc
   */
  inline double exp(int x) {
    return ::exp(static_cast<double>(x));
  }

  /**
   * power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double pow(const double& x, const double& y) {
    return ::pow(x,y);
  }

  /**
   * power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float pow(float x,float y) {
    return static_cast<float>(::pow(x,y));
  }

  /**
   * power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline double pow(const double& x, int y) {
    return ::pow(x,y);
  }

  /**
   * power function, computes \f$x^y\f$
   *
   * @ingroup gBasicMathFunc
   */
  inline float pow(float x,int y) {
    return static_cast<float>(::pow(x,y));
  }

#else
  // methods optimized for different types
  using std::swap;
  using std::sin;
  using std::cos;
  using std::tan;
  using std::asin;
  using std::acos;
  using std::atan;
  using std::atan2;
  using std::sinh;
  using std::cosh;
  using std::tanh;
  using std::log;
  using std::log10;
  using std::exp;
  using std::pow;
#endif

  /**
   * sigmoid for floats
   *
   * @ingroup gBasicMathFunc
   */
  inline float sigmoid(const float x) {
    return static_cast<float>(1.0/(1.0+exp(-x)));
  }

  /**
   * sigmoid for doubles
   *
   * @ingroup gBasicMathFunc
   */
  inline double sigmoid(const double& x){
    return 1.0/(1.0+exp(-x));
  }

  /**
   * sigmoid for ints
   *
   * @ingroup gBasicMathFunc
   */
  inline double sigmoid(const int x){
    return 1.0/(1.0+exp(-static_cast<double>(x)));
  }

  /**
   * signum of x (for x==0 -> signum = 1) (@see signum0)
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T signum(T x) {
    return ((x>=T()) ? T(1) : T(-1));
  }

  /**
   * signum of x (for x==0 -> signum = 0) (@see signum)
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline T signum0(T x) {
    return ((x>0) ? T(1) : ((x<0) ? T(-1) : T(0)));
  }

  /**
   * Return true if the given number is an even number. 
   *
   * For floating point types, this function will return true only if the
   * number is \e exacltly an integer and it is even.
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline bool even(const T x) {
    return ((x & static_cast<T>(1)) == 0); 
  }

  template<>
  inline bool even(const double x) {
    const int64 i = static_cast<int64>(x);
    return ((static_cast<double>(i) == x) && even(i));
  }

  template<>
  inline bool even(const float x) {
    const int i = static_cast<int>(x);
    return ((static_cast<float>(i) == x) && even(i));
  }


  /**
   * Return true if the given number is an odd number.
   *
   * For floating point types, this function will return true only if the
   * number is \e exacltly an integer and it is odd.
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  inline bool odd(const T x) {
    return ((x & static_cast<T>(1)) != 0); 
  }

  template<>
  inline bool odd(const double x) {
    const int64 i = static_cast<int64>(x);
    return ((static_cast<double>(i) == x) && odd(i));
  }

  template<>
  inline bool odd(const float x) {
    const int i = static_cast<int>(x);
    return ((static_cast<float>(i) == x) && odd(i));
  }

  /**
   * Compute the incomplete beta function.
   *
   * @ingroup gBasicMathFunc
   */
  template<class T>
  T betai(const T& a, const T& b, const T& x);
  
  /**
   * Compute the coefficients required for the incomplete beta function.
   * Internally we compute with double precision.
   */
  double betacf(const double& a, const double& b, const double& x);
  
}


#include "ltiMath_template.h"

#endif
