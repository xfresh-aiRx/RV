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
 * file .......: ltiMaskFunctors.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 08.10.99
 * revisions ..: $Id: ltiMaskFunctors.h,v 1.8 2004/11/22 15:45:51 sltilib Exp $
 */

#ifndef _LTI_MASKFUNCTORS_H_
#define _LTI_MASKFUNCTORS_H_

#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiTypeInfo.h"

namespace lti {
  /**
   * The mask functor is a colection of boolean operations used in the
   * processing of mask images or matrices.  All members are used in derived
   * classes, which provide the LTI-Lib standard interface.
   * The template parameter is the type of the matrices to be operated and NOT
   * the whole matrix type.  For example:
   * \code
   * lti::maskFunctor<channel8::value_type> masker;
   * channel8 a,b,result;
   * ...
   * masker.multiply(a,b,result);
   * \endcode
   * @see lti::maskMultiply, lti::maskAlgebraicSum, lti::maskOr, lti::maskAnd,
   *      lti::maskNot, lti::maskInvert
   */
  template<class T>
  class maskFunctor : public functor {
    public:
    /**
     * constructor
     */
    maskFunctor();

    /**
     * copy constructor
     */
    maskFunctor(const maskFunctor<T>& other) : functor() {copy(other);};

    /**
     * destructor
     */
    virtual ~maskFunctor();

    /**
     * returns the name of this type
     */
    const char* getTypeName() const {return "maskFunctor";};

    /**
     * clones this object
     */
    functor* clone() const {
      return (new maskFunctor(*this));
    };

    /**
     * copy
     */
    void copy(const maskFunctor<T>& other);

    /**
     * multiplies the normalized first mask with the second and leaves
     * the result on the second mask.  The size of m2 will not be
     * modified (i.e. only the common submatrix will be considered!!!)
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return a reference to m2
     */
    matrix<T>& multiply(const matrix<T>& m1,matrix<T>& m2);

    /**
     * multiplies the normalized first mask with the second and leaves
     * the result on the third mask.  The size of 'result' is as big as the
     * biggest matrix.
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return a reference to result
     */
    matrix<T>& multiply(const matrix<T>& m1,
			const matrix<T>& m2,
			matrix<T>& result);

    /**
     * calculates the algebraic sum for each element of the normalized
     * matrixes.  The algebraic sum of <em>a</em> and <em>b</em> is
     * defined as follows: <pre> s = 1 - (1-a)(1-b).  </pre> This is
     * some sort of OR operator.  The size of m1 will not be
     * modified (i.e. only the common submatrix will be considered)
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return a reference to m2
     */
    matrix<T>& algebraicSum(const matrix<T>& m1,matrix<T>& m2);

    /**
     * calculates the algebraic sum for each element of the matrixes.
     * The algebraic sum of a and b is defined as follows:
     * <pre> s = 1 - (1-a)(1-b). </pre>
     * This is used in the implementation of OR.
     * The size of <em>result</em> is as big as the biggest matrix.
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return a reference to result
     */
    matrix<T>& algebraicSum(const matrix<T>& m1,
			    const matrix<T>& m2,
			    matrix<T>& result);

    /**
     * the m2 pixel will contain the norm value (see suggestedNorm()) if any
     * of both mask's pixels is not null
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return a reference to m2
     */
    matrix<T>& maskOr(const matrix<T>& m1,matrix<T>& m2);

    /**
     * the result pixel will contain the norm value (see
     * suggestedNorm()) if any of both mask's pixels is not null
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return a reference to result
     */
    matrix<T>& maskOr(const matrix<T>& m1,
                      const matrix<T>& m2,
                      matrix<T>& result);

    /**
     * the m2 pixel will contain the norm value (see suggestedNorm()) if both
     * mask's pixels are not zero, otherwise the value is zero.
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return a reference to m2
     */
    matrix<T>& maskAnd(const matrix<T>& m1,matrix<T>& m2);

    /**
     * the result pixel will contain the norm value (see
     * suggestedNorm()) if both mask's pixels are not zero, otherwise
     * the value is zero.
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return a reference to result
     */
    matrix<T>& maskAnd(const matrix<T>& m1,
                       const matrix<T>& m2,
                             matrix<T>& result);

    /**
     * the value of the mask will be zero if the original value is not zero and
     * viceversa.
     *
     * @param m matrix to be inverted.  The result will be left here too.
     * @return a reference to m
     */
    matrix<T>& maskNot(matrix<T>& m);

    /**
     * the result of the mask will be zero if the original value is
     * not zero and viceversa.
     *
     * @param m1 first matrix
     * @param result result container
     * @return a reference to result
     */
    matrix<T>& maskNot(const matrix<T>& m1,
                             matrix<T>& result);

    /**
     * inverts the values in the matrix m, where "inversion" means the
     * suggestedNorm() minus the original matrix entry value.
     *
     * @param m matrix to be inverted.  The result is left here too.
     * @return a reference to the matrix m
     */
    matrix<T>& invert(matrix<T>& m);

    /**
     * inverts the values in the matrix m, where "inversion" means the
     * suggestedNorm() minus the original matrix entry value.
     *
     * @param m1 matrix to be inverted.
     * @param result the result is left here.
     * @return a reference to the matrix result.
     */
    matrix<T>& invert(const matrix<T>& m1,
                      matrix<T>& result);

    /**
     * apply the given function at each pixel of m1 and leave the
     * result in the <code>result</code> matrix.
     *
     * @param m1 the first matrix
     * @param result the resulting matrix.  The size of this matrix will be
     *        the size of m1
     * @param function the function to be applied.  The first parameter will
     *        be the element value of the first matrix, the second
     *        parameter will be usually a normalization factor.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& m1,
                     matrix<T>& result,
               T (*function)(const T&,
                             const T&)) const;

    /**
     * apply the given function at each pixel of srcdest and leave the
     * result there too.
     *
     * @param srcdest the source and destination matrix
     * @param function the function to be applied.  The first parameter will
     *        be the element value of the first matrix, the second parameter
     *        will be usually a normalization factor.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<T>& srcdest,
                          T (*function)(const T&,const T&)) const;

    /**
     * apply the given function at each element of m1 and m2 and leave the
     * result in the <code>result</code> matrix.
     * @param m1 the first matrix
     * @param m2 the second matrix
     * @param result the resulting matrix.  The size of this matrix will be
     *        the smallest matrix which can contain both matrices m1 and m2.
     *        For those parts covered only by one of the matrices, the assumed
     *        value for the other one will be zero.
     * @param function the function to be applied.  The first parameter will
     *        be the element value of the first matrix, the second
     *        parameter the element value of the second matrix and the
     *        third parameter will be usually a normalization factor.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& m1,
               const matrix<T>& m2,
                     matrix<T>& result,
                     T (*function)(const T&,const T&,const T&)) const;

    /**
     * apply the given function at each element of m1 and srcdest and
     * leave the result in the <code>srcdest</code> matrix.
     * @param m1 the first matrix
     * @param srcdest the second matrix and also the resulting matrix.
     *                Its size won't be changed. For those parts covered only
     *                by one of the matrices, the assumed
     *                value for the other one will be zero.
     * @param function the function to be applied.  The first parameter will
     *        be the element value of the first matrix, the second
     *        parameter the element value of the second matrix and the
     *        third parameter will be usually a normalization factor.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& m1,
                     matrix<T>& srcdest,
                     T (*function)(const T&,const T&,const T&)) const;

    /**
     * this "dummy" apply member is just an interface for the subclasses
     * which will follow the standard LTI-Lib approach.  This doesn't do
     * anything at all...
     *
     * @param m1 the first matrix to operate with
     * @param m2 the second matrix to operate with
     * @param result the matrix where the result will be left
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<T>& m1,
                       const matrix<T>& m2,
                             matrix<T>& result) const {
      throw functor::invalidMethodException();
      return false;
    }

    /**
     * this "dummy" apply member is just an interface for the subclasses
     * which will follow the standard LTI-Lib approach.  This doesn't do
     * anything at all...
     *
     * @param m1 the first matrix to operate with
     * @param result the matrix where the result will be left
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const matrix<T>& m1,
                             matrix<T>& result) const {
      throw functor::invalidMethodException();
      return false;
    }

  protected:
    /**
     * calculates a*b/norm;
     *
     * @param a first parameter
     * @param b second parameter
     * @param norm norm factor used
     * @return a*b/norm
     */
    static T multiplyNorm(const T& a,const T& b,const T& norm) {
      return a*b/norm;
    }

    /**
     * calculates a*b;
     *
     * @param a first parameter
     * @param b second parameter
     * @param norm norm factor used
     * @return a*b
     */
    static T multiplyNoNorm(const T& a,const T& b,const T& norm) {
      return a*b;
    }

    /**
     * calculates the algebraic sum = norm - (norm-a)*(norm-b)/norm
     *
     * @param a first parameter
     * @param b second parameter
     * @param norm norm factor used
     * @return norm - (norm-a)*(norm-b)/norm
     */
    static T algebraicSumNorm(const T& a,const T& b,const T& norm) {
      return norm - static_cast<T>((norm-a)*(norm-b)/norm);
    }

    /**
     * calculates algebraic sum = norm - (norm-a)*(norm-b)/norm
     *
     * @param a first parameter
     * @param b second parameter
     * @param norm norm factor used
     * @return norm - (norm-a)*(norm-b)/norm
     */
    static T algebraicSumNoNorm(const T& a,const T& b,const T& norm) {
      return norm - static_cast<T>((norm-a)*(norm-b));
    }

    /**
     * calculates (a or b)
     *
     * @param a first parameter
     * @param b second parameter
     * @param norm norm factor used
     * @return a or b
     */
    static T orNorm(const T& a,const T& b,const T& norm) {
      return (((a != 0) || (b != 0))) ? norm : static_cast<T>(0);
    }

    /**
     * calculates (a and b)
     *
     * @param a first parameter
     * @param b second parameter
     * @param norm norm factor used
     * @return a and b
     */
    static T andNorm(const T& a,const T& b,const T& norm) {
      return (((a != 0) && (b != 0))) ? norm : static_cast<T>(0);
    }

    /**
     * calculates (not a)
     *
     * @param a first parameter
     * @param norm norm factor used
     * @return not a
     */
    static T notMaskNorm(const T& a,const T& norm) {
      return (a == 0) ? norm : static_cast<T>(0);
    }

    /**
     * calculates norm-a;
     *
     * @param a first parameter
     * @param norm norm factor used
     * @return norm-a
     */
    static T invertNorm(const T& a,const T& norm) {
      return norm-a;
    }

    /**
     * a static member can be one of following things
     */
    enum eFunctionType {
      Black,    /**< if a parameter is zero, the result is zero */
      NoEffect, /**< if a parameter is zero, the result will be the
                     other value */
      White,    /**< if the result is zero, the result will be the norm */
      Unknown   /**< unknown masking effect!*/
    };

    /**
     * check a function.  if par==0 the first parameter will be set to zero
     * otherwise the second parameters will be set to zero.
     *
     * @param function the function to be checked
     * @param par flag used.
     * @return the function type
     */
    eFunctionType check(T (*function)(const T&,const T&,const T&),
                        const int& par) const;

  };


  /**
   * alias with standard LTI-Lib interface for mask-multiply member of the
   * maskFunctor
   */
  template<class T>
  class maskMultiply : public maskFunctor<T> {
  public:
    /**
     * apply on place (see also lti::maskFunctor::multiply())
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1, matrix<T>& m2) {
      multiply(m1,m2);
      return true;
    };

    /**
     * apply on copy (see also lti::maskFunctor::multiply())
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1,
               const matrix<T>& m2,
		     matrix<T>& result) {
      multiply(m1,m2,result);
      return true;
    };
  };

  /**
   * alias with standard LTI-Lib interface for mask-algebraicSum member of the
   * maskFunctor
   */
  template<class T>
  class maskAlgebraicSum : public maskFunctor<T> {
  public:
    /**
     * apply on place (see also lti::maskFunctor::algebraicSum())
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1, matrix<T>& m2) {
      algebraicSum(m1,m2);
      return true;
    };

    /**
     * apply on copy (see also lti::maskFunctor::algebraicSum())
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1,
		     const matrix<T>& m2,
		     matrix<T>& result) {
      algebraicSum(m1,m2,result);
      return true;
    };
  };

  /**
   * alias with standard LTI-interface for maskOr member of the
   * maskFunctor
   */
  template<class T>
  class maskOr : public maskFunctor<T> {
  public:
    /**
     * apply on place (see also lti::maskFunctor::maskOr())
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1, matrix<T>& m2) {
      maskOr(m1,m2);
      return true;
    };

    /**
     * apply on copy (see also lti::maskFunctor::maskOr())
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1,
               const matrix<T>& m2,
		     matrix<T>& result) {
      maskOr(m1,m2,result);
      return true;
    };
  };

  /**
   * alias with standard LTI-Lib interface for maskAnd member of the
   * maskFunctor
   */
  template<class T>
  class maskAnd : public maskFunctor<T> {
  public:
    /**
     * apply on place (see also lti::maskFunctor::maskAnd())
     *
     * @param m1 first matrix
     * @param m2 second matrix and result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1, matrix<T>& m2) {
      maskAnd(m1,m2);
      return true;
    };

    /**
     * apply on copy (see also lti::maskFunctor::maskAnd())
     *
     * @param m1 first matrix
     * @param m2 second matrix
     * @param result result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1,
               const matrix<T>& m2,
		     matrix<T>& result) {
      maskAnd(m1,m2,result);
      return true;
    };
  };

  /**
   * alias with standard LTI-Lib interface for maskNot member of the
   * maskFunctor
   */
  template<class T>
  class maskNot : public maskFunctor<T> {
  public:
    /**
     * apply on place (see also lti::maskFunctor::maskNot())
     *
     * @param m1 source matrix and result
     * @return true if successful, false otherwise
     */
    bool apply(matrix<T>& m1) {
      maskFunctor<T>::maskNot(m1);
      return true;
    };

    /**
     * apply on copy (see also lti::maskFunctor::maskNot())
     *
     * @param m1 first matrix
     * @param result result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1,
		     matrix<T>& result) {
      maskFunctor<T>::maskNot(m1,result);
      return true;
    };
  };

  /**
   * alias with standard LTI-Lib interface for invert member of the
   * maskFunctor
   */
  template<class T>
  class maskInvert : public maskFunctor<T> {
  public:
    /**
     * apply on place
     * apply on place (see also lti::maskFunctor::maskNot())
     *
     * @param m1 source matrix and result
     * @return true if successful, false otherwise
     */
    bool apply(matrix<T>& m1) {
      invert(m1);
      return true;
    };

    /**
     * apply on copy
     *
     * @param m1 first matrix
     * @param result result container
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& m1,
		     matrix<T>& result) {
      invert(m1,result);
      return true;
    };
  };

}


#endif

