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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiComplex.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 6.4.2003
 * revisions ..: $Id: ltiComplex.h,v 1.12 2005/02/02 14:16:22 doerfler Exp $
 */

/**
 * \file ltiComplex.h
 * The standard \c complex header provide by the different compilers and 
 * library versions different functionality.  For the LTI-Lib this file
 * provides a unified implementation.
 */

#ifndef _LTI_COMPLEX_H
#define _LTI_COMPLEX_H

#ifdef _LTI_MSC_6
#pragma warning(disable:4275) // warnings for complex<T> 
                              // disabled (dll stuff in 
                              // MS implementation)
#endif

#include "ltiMath.h"
#include <complex>
#include <iostream>

#ifdef _LTI_GNUC_2
namespace std {
  typedef ios ios_base;
}
#endif

namespace lti {    
  /**
   * Class to represent and manipulate complex numbers.
   *
   * The complex numbers contain a real and an imaginary part, which can
   * be accessed through the methods real() and imag().
   *
   * The interface of this class is similar to the one provided by the
   * Standard C++ library.  It has being included in the LTI-Lib because 
   * different library versions do not provide the same functionality.
   *
   * Note that many of the provided mathematical operations make only
   * sense if the type used for the components of the complex number
   * is double or float.
   */
  template<typename T>
  class complex {
  public:
    typedef T value_type;
    
    /**
     * default constructor.
     */
    complex(const T& = T(), const T & = T());
    
    /**
     * copy constructor
     */
    template<class U>
    complex(const complex<U>& other) 
      : realPart(static_cast<T>(other.realPart)),
        imagPart(static_cast<T>(other.imagPart)) {	
    }
    
    /**
     * cast a std::complex to an lti::complex of the same type
     */
    complex(const std::complex<T>& other);

    /**
     * cast a lti::complex into a std::complex of the same type
     */
    operator std::complex<T>() {
      return std::complex<T>(real(),imag());
    }

    /**
     * return the real component of the complex number
     */
    T real() const;
    
    /**
     * return the imaginary component of the complex number
     */
    T imag() const;
    
    /**
     * copy a real number into this complex one, making the imaginary part
     * zero.
     */
    complex<T>& operator=(const T& other);
    
    /**
     * add a real number to this complex one.
     */     
    complex<T>& operator+=(const T& other);
    
    /**
     * subtract a real number to this complex one.
     */     
    complex<T>& operator-=(const T& other);
    
    /**
     * multiply this complex number with a real one.
     */     
    complex<T>& operator*=(const T& other);
    
    /**
     * divide this complex number by a real one.
     */     
    complex<T>& operator/=(const T& other);
    
    /**
     * copy another complex number into this one
     */
    template<typename U>
    complex<T>& operator=(const complex<U>& other) {
      realPart = other.realPart;
      imagPart = other.imagPart;
      return (*this);
    };
    
    /**
     * add another complex number
     */
    template<typename U>
    complex<T>& operator+=(const complex<U>& other) {
      realPart+=static_cast<T>(other.realPart);
      imagPart+=static_cast<T>(other.imagPart);
      return (*this);
    }
    
    /**
     * subtract another complex number
     */
    template<typename U>
    complex<T>& operator-=(const complex<U>& other) {
      realPart-=static_cast<T>(other.realPart);
      imagPart-=static_cast<T>(other.imagPart);
      return (*this);
    }
    
    /**
     * multiply with another complex number
     */
    template<typename U>
    complex<T>& operator*=(const complex<U>& other) {
      const T re = realPart * other.real() - imagPart * other.imag();
      imagPart = realPart * other.imag() + imagPart * other.real();
      realPart = re;
      return *this;
    }
    
    /**
     * divide with another complex number
     */
    template<typename U>
    complex<T>& operator/=(const complex<U>& other) {
      const T norm = other.real()*other.real() + other.imag()*other.imag();
      const T re = (realPart * other.real() + imagPart * other.imag())/norm;
      imagPart = (imagPart * other.real() - realPart * other.imag())/norm;
      realPart = re;
      return *this;
    }

  private:
    /**
       * real component
       */
    T realPart;
    
    /**
       * imaginary component
       */
    T imagPart;
  };

  // -------------------------------------------------------------------------
  // implementation
  // -------------------------------------------------------------------------

  template<typename T>
  inline T complex<T>::real() const {
    return realPart; 
  }
  
  template<typename T>
  inline T complex<T>::imag() const {
    return imagPart; 
  }
  
  template<typename T>
  inline complex<T>::complex(const T& re, const T& im)
    : realPart(re), imagPart(im) { }

  template<typename T>
    inline complex<T>::complex(const std::complex<T>& other)
    : realPart(other.real()), imagPart(other.imag()) { }
  
  template<typename T>
  complex<T>& complex<T>::operator=(const T& other) {
    realPart = other;
    imagPart = T();
    return *this;
  } 

  template<typename T>
  inline complex<T>& complex<T>::operator+=(const T& other) {
    realPart += other;
    return *this;
  }
  
  template<typename T>
  inline complex<T>& complex<T>::operator-=(const T& other) {
    realPart -= other;
    return *this;
  }

  template<typename T>
  complex<T>&  complex<T>::operator*=(const T& other) {
    realPart *= other;
    imagPart *= other;
    return *this;
  }

  template<typename T>
  complex<T>& complex<T>::operator/=(const T& other) {
    realPart /= other;
    imagPart /= other;
    return *this;
  }




  // -------------------------------------------------------------------------
  // global operators
  // -------------------------------------------------------------------------
  /**
   * @name operators for complex numbers
   */
  //@{
    
  /**
   * add two complex numbers
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) += b; 
  }

  /**
   * add a complex number with a real number
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a, const T& b) {
    return complex<T>(a) += b; 
  }

  /**
   * add a real number with a complex number
   */
  template<typename T>
  inline complex<T> operator+(const T& a, const complex<T>& b) {
    return complex<T>(b) += a; 
  }

  /**
   * subtract two complex numbers
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) -= b; 
  }
    
  /**
   * subtract a real number from a complex one
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a, const T& b) {
    return complex<T>(a) -= b; 
  }

  /**
   * subtract a complex number from a real one
   */
  template<typename T>
  inline complex<T> operator-(const T& a, const complex<T>& b) {
    return complex<T>(a) -= b;
  }

  /**
   * multiply two complex numbers
   */
  template<typename T>
  inline complex<T> operator*(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) *= b; 
  }

  /**
   * multiply a complex number with a real one
   */
  template<typename T>
  inline complex<T> operator*(const complex<T>& a, const T& b) {
    return complex<T>(a) *= b; 
  }

  /**
   * multiply a real number with a complex one
   */
  template<typename T>
  inline complex<T> operator*(const T& a, const complex<T>& b) {
    return complex<T>(b) *= a;
  }

  /**
   * divide two complex numbers
   */
  template<typename T>
  inline complex<T> operator/(const complex<T>& a, const complex<T>& b) {
    return complex<T>(a) /= b; 
  }
    
  /**
   * divide a complex number by a real one
   */
  template<typename T>
  inline complex<T> operator/(const complex<T>& a, const T& b) {
    return complex<T>(a) /= b; 
  }

  /**
   * divide a real number by a complex one
   */
  template<typename T>
  inline complex<T> operator/(const T& a, const complex<T>& b) {
    return complex<T>(a) /= b; 
  }

  /**
   * plus sign a complex number
   */
  template<typename T>
  inline complex<T> operator+(const complex<T>& a) {
    return a;
  }

  /**
   * minus sign a complex number
   */
  template<typename T>
  inline complex<T> operator-(const complex<T>& a) {
    return complex<T>(-a.real(), -a.imag());
  }

  /**
   * compare if two complex numbers are equal
   */
  template<typename T>
  inline bool operator==(const complex<T>& a, const complex<T>& b) {
    return (a.real() == b.real()) && (a.imag() == b.imag());
  }

  /**
   * compare if two complex numbers differ
   */
  template<typename T>
  inline bool operator==(const complex<T>& a, const T& b) {
    return (a.real() == b) && (a.imag() == T());
  }

  /**
   * compare if a real number and a complex number are the same
   */
  template<typename T>
  inline bool operator==(const T& a, const complex<T>& b) {
    return (a == b.real()) && (T() == b.imag()); 
  }

  /**
   * compare if two complex numbers differ
   */
  template<typename T>
  inline bool operator!=(const complex<T>& a, const complex<T>& b) {
    return (a.real() != b.real()) || (a.imag() != b.imag());
  }

  /**
   * compare if a complex number and a real one differ
   */
  template<typename T>
  inline bool operator!=(const complex<T>& a, const T& b) {
    return (a.real() != b) || (a.imag() != T());
  }

  /**
   * compare if a real number and a complex one differ
   */
  template<typename T>
  inline bool operator!=(const T& a, const complex<T>& b) {
    return (a != b.real()) || (T() != b.imag());
  }

  /**
   * a complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const complex<T>& a,const complex<T>& b) {
    return ((a.real()<b.real()) || 
            ((a.real() == b.real()) && (a.imag() < b.imag())));
  }

  /**
   * a complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const complex<T>& a,const T& b) {
    return ((a.real()<b) || 
            ((a.real() == b) && (a.imag() < T())));
  }

  /**
   * a complex number is "less than" another one if its real part is
   * smaller, or when both real parts are identical, if its imaginary
   * part is smaller
   */
  template<typename T>
  inline bool operator<(const T& a,const complex<T>& b) {
    return ((a<b.real()) || 
            ((a == b.real()) && (T() < b.imag())));
  }

  /**
   * a complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const complex<T>& a,const complex<T>& b) {
    return ((a.real()>b.real()) || 
            ((a.real() == b.real()) && (a.imag() > b.imag())));
  }

  /**
   * a complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const complex<T>& a,const T& b) {
    return ((a.real()>b) || 
            ((a.real() == b) && (a.imag() > T())));
  }

  /**
   * a complex number is "greater than" another one if its real part is
   * greater, or when both real parts are identical, if its imaginary
   * part is greater
   */
  template<typename T>
  inline bool operator>(const T& a,const complex<T>& b) {
    return ((a > b.real()) || 
            ( (a == b.real()) && ( b.imag() < T(0) ) ));
  }

  /**
   * get the real part of a complex number
   */
  template<typename T>
  inline T real(const complex<T>& cn) {
    return cn.real(); 
  }
    
  /**
   * get the imaginary part of a complex number
   */
  template<typename T>
  inline T imag(const complex<T>& cn) {
    return cn.imag(); 
  }

  /**
   * get the absolute value of a complex number
   */
  template<typename T>
  inline T abs(const complex<T>& cn) {
    T a = cn.real();
    T b = cn.imag();

    const T tmp = max(abs(a), abs(b));
    if (tmp == T()) { // zero?
      return tmp;
    }

    a /= tmp; // normalize for more numerical stability
    b /= tmp;
    return tmp * sqrt(a * a + b * b);
  }

  /**
   * get the argument (angle or phase) of a complex number
   */
  template<typename T>
  inline T arg(const complex<T>& cn) {
    return atan2(cn.imag(), cn.real());
  }


  /**
   * return the square magnitude of the given complex number
   */ 
  template<typename T>
  inline T norm(const complex<T>& cn) {
    const T a(cn.real),b(cn.imag());
    return (a*a+b*b);
  }

  /**
   * convert the given polar values into a complex number
   *
   * @param radius magnitude of the number
   * @param angle  angle of the number
   * 
   * @return Equivalent complex number to the polar one
   */
  template<typename T>
  inline complex<T> polar(const T& radius, const T& angle) {
    return complex<T>(radius * cos(angle), radius * sin(angle)); 
  }

  /**
   * return the conjugate of the complex number
   */
  template<typename T>
  inline complex<T> conj(const complex<T>& cn) {
    return complex<T>(cn.real(), -cn.imag()); 
  }
  
  /**
   * cosine of a complex number
   */
  template<typename T>
  inline complex<T> cos(const complex<T>& cn) {
    const T a = cn.real();
    const T b = cn.imag();
    return complex<T>(cos(a) * std::cosh(b), -sin(a) * std::sinh(b));
  }

  /**
   * hyperbolic cosine of a complex number
   */
  template<typename T>
  inline complex<T> cosh(const complex<T>& cn) {
    const T a = cn.real();
    const T b = cn.imag();
    return complex<T>(std::cosh(a) * cos(b), std::sinh(a) * sin(b));
  }

  template<typename T>
  inline complex<T> acos(const complex<T>& cn) {
    complex<T> tmp(log(cn+sqrt(cn*cn-T(1))));
    return complex<T>(tmp.imag(),-tmp.real());;
  }

  /**
   * exponetial of a complex number
   */
  template<typename T>
  inline complex<T> exp(const complex<T>& cn) {
    return polar(exp(cn.real()), cn.imag()); 
  }

  /**
   * natural logarithm of a complex number (base \e e)
   */
  template<typename T>
  inline complex<T> log(const complex<T>& cn) {
    return complex<T>(log(abs(cn)), arg(cn)); 
  }

  /**
   * logarithm base 10 of a complex number
   */
  template<typename T>
  inline complex<T> log10(const complex<T>& cn) {
    return log(cn) / log(T(10.0)); 
  }

  /**
   * sine of a complex number
   */
  template<typename T>
  inline complex<T> sin(const complex<T>& cn) {
    const T a = cn.real();
    const T b = cn.imag();
    return complex<T>(sin(a) * std::cosh(b), cos(a) * std::sinh(b)); 
  }
  
  /**
   * hyperbolic sine of a complex number
   */
  template<typename T>
  inline complex<T> sinh(const complex<T>& cn) {
    const T a = cn.real();
    const T  b = cn.imag();
    return complex<T>(std::sinh(a) * cos(b), std::cosh(a) * sin(b));
  }

  template<typename T>
  inline complex<T> asin(const complex<T>& cn) {
    complex<T> tmp(log(complex<T>(-cn.imag(),cn.real())+sqrt(1-cn*cn)));
    return complex<T>(tmp.imag(),-tmp.real());;
  }

  /**
   * square root of a complex number.
   *
   * A there are always two solutions for sqrt(x+iy), this method provides
   * the first one consisting in sqrt(m*e^(i*a))=sqrt(m)*e^(i*a/2)
   */
  template<typename T>
  complex<T> sqrt(const complex<T>& cn) {
    T a = cn.real();
    T b = cn.imag();

    if (a == T()) {
      T other = sqrt(abs(b) / 2);
      return complex<T>(other, b < T() ? -other : other);
    }
    else {
      T other = sqrt(2 * (abs(cn) + abs(a)));
      T tmp = other / 2;
      return (a > T()) ? 
        complex<T>(tmp, b / other)
        : complex<T>(abs(b) / other, b < T() ? -tmp : tmp);
    }
  }

  /**
   * cube root of a complex number
   *
   * A there are always two solutions for sqrt(x+iy), this method provides
   * the first one consisting in sqrt(m*e^(i*a))=sqrt(m)*e^(i*a/2)
   */
  template<typename T>
  inline complex<T> cbrt(const complex<T>& cn) {
    const T m = abs(cn);
    const T a = arg(cn);
    return polar(pow(double(m),1.0/3.0),m/3.0);
  }

  /**
   * cube root of a complex number (alias for cbrt())
   */
  template<typename T>
  inline complex<T> cubeRoot(const complex<T>& cn) {
    return cbrt(cn);
  }  
  
  /**
   * tangent of a complex number
   */
  template<typename T>
  inline complex<T> tan(const complex<T>& cn) {
    return sin(cn) / cos(cn);
  }

  /**
   * hyperbolic tangent of a complex number
   */
  template<typename T>
  inline complex<T> tanh(const complex<T>& cn) {
    return std::sinh(cn) / std::cosh(cn);
  }

  /**
   * hyperbolic arc tangent of a complex number
   */
  template<typename T>
  inline complex<T> atan(const complex<T>& cn) {
    return log((complex<T>(1)+complex<T>(-cn.real(),cn.imag()))/
               (complex<T>(1)-complex<T>(-cn.real(),cn.imag())))/
      (complex<T>(0,2));
  }

  /**
   * complex number to the power of \a b, with \b real
   */
  template<typename T>
  inline complex<T> pow(const complex<T>& a, const T& b) {
    return exp(b * log(a));
  }

  /**
   * complex number to the power of \a b, with \b complex
   */
  template<typename T>
  inline complex<T> pow(const complex<T>& a, const complex<T>& b) {
    return exp(b * log(a));
  }

  /**
   * real number to the power of \a b, with \b complex
   */
  template<typename T>
  inline complex<T> pow(const T& a, const complex<T>& b) {
    return exp(b * log(a));
  }

  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  template <typename T>
  bool read(ioHandler& handler,complex<T>& p,const bool complete=true) {
    bool b(true);
    
    if (complete) {
      b = b && handler.readBegin();
    }

    T re,im;
    b = b && handler.read(re);
    b = b && handler.readDataSeparator();
    b = b && handler.read(im);

    p=complex<T>(re,im);
    
    if (complete) {
      b = b && handler.readEnd();
    }
    
    return b;
  };

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<typename T>
  bool write(ioHandler& handler,const complex<T>& p,const bool complete=true) {
    bool b(true);
    
    if (complete) {
      b = b && handler.writeBegin();
    }
    
    b = b && handler.write(p.real());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.imag());
    
    if (complete) {
      b = b && handler.writeEnd();
    }
    
    return b;
  };

} // end namespace lti

namespace std {

  template<typename T>
  inline istream& operator>>(istream& in, lti::complex<T>& a) {
    T re, im;
    char ch;
    in >> ch;
    if (ch == '(') {
      in >> re >> ch;
      if (ch == ',') {
        in >> im >> ch;
        if (ch == ')') {
          a = lti::complex<T>(re, im);
        } else {
          in.setstate(ios_base::failbit);
        }
      }
      else if (ch == ')') {
        a = lti::complex<T>(re, T(0));
      } else {
        in.setstate(ios_base::failbit);
      }
    }
    else {
      in.putback(ch);
      in >> re;
      a = lti::complex<T>(re, T(0));
    }
    return in;
  }

  template<typename T>
  inline ostream& operator<<(ostream& out, const lti::complex<T>& a) {
    out << '(' << a.real() << ',' << a.imag() << ')';
    return out;
  }

} // namespace std

#endif 
