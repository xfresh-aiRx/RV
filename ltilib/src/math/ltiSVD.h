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
 * file .......: ltiSVD.h
 * authors ....: Xin Gu
 * organization: LTI, RWTH Aachen
 * creation ...: 11.01.01
 * version ....: $Id: ltiSVD.h,v 1.20 2005/11/04 09:35:59 doerfler Exp $
 */

#ifndef LTI_SINGULAR_VALUE_DECOMPOSITION
#define LTI_SINGULAR_VALUE_DECOMPOSITION

#include "ltiMath.h"
#include "ltiMatrix.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {

  /**
   * Singular Value Decomposition.
   *
   * The functor will take a matrix A and compute its singular value
   * decomposition, consisting of three matrices U, W, and V, with
   * A=U*W*V' (where V' denotes the transpose of V). U is a
   * column-orthonormal matrix, W is a diagonal matrix with the
   * singular values on the diagonal, and V is a orthonormal
   * matrix. Those columns of V whose corresponding entries in W are
   * zero are the basis of A's null space.
   *
   * You can find more theoretical information about a similar
   * algorithm in W. H. Press, S. A. Teukolsky, W. T. Vetterling, and
   * B. P. Flannery: Numerical Recipes in C, 2nd edition, Cambridge
   * University Press, 1992.
   *
   * Only instantiations of floating point types makes sense (i.e. for
   * T double or float). If you want the singular values and
   * corresponding singular vectors to be sorted, you have to set
   * parameters::sort to true.
   */
  template<class T>
  class singularValueDecomp: public linearAlgebraFunctor {
  public:

    /**
     * the parameters for the class
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        sort = false;
        transposeU = false;
        transposeV = false;       
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "singularValueDecomp::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other)  {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearAlgebraFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        sort=other.sort;
        transposeU = other.transposeU;
        transposeV = other.transposeV;

        return *this;
      };

      /**
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other) {
        copy(other);
        return *this;
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };


#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          lti::read(handler,"sort",this->sort);
          lti::read(handler,"transposeU",transposeU);
          lti::read(handler,"transposeV",transposeV);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::read(handler,false);
#     else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"sort",this->sort);
          lti::write(handler,"transposeU",transposeU);
          lti::write(handler,"transposeV",transposeV);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::write(handler,false);
#       else
        bool
        (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif


      // ---------------------------------------------------
      // the parameters
      // ---------------------------------------------------

      /**
       * If true, singular values and corresponding singular vectors
       * are rearranged so that singular values have descending
       * order. Default is false.
       */
      bool sort;

      /**
       * Specifies whether U (false) or Ut (true) is returned. It is
       * faster to return Ut. Default is false, ie U.
       */
      bool transposeU;

      /**
       * Specifies whether V (false) or Vt (true) is returned. It is
       * faster to return V. Default is false, ie V.
       */
      bool transposeV;
    };

    /**
     * default constructor
     */
    singularValueDecomp();

    /**
     * default constructor with parameters
     */
    singularValueDecomp(const parameters& params);

    /**
     * constructor. Sets parameters::sort to the given value.
     */
    singularValueDecomp(bool sort);

    /**
     * copy constructor
     */
    singularValueDecomp(const singularValueDecomp<T>& other);

    /**
     * destructor
     */
    virtual ~singularValueDecomp();

    /**
     * returns the name of this type ("singularValueDecomp")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    singularValueDecomp& copy(const singularValueDecomp& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * OnPlace version of Singular Value Decomposition. Singular Value
     * Decomposition means that a m*n-matrix A is decomposed into three
     * matrices U,W,V, such that  A = U*W*V'. U is m*n, W is a diagonal
     * matrix with n elements (which is implemented as vector), V is a
     * n*n-matrix. Note that the function returns V, not V'.
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned. If
     *            src is a m*n matrix, U will also be of size m*n
     * @param w vector<T> with the singular values, sorted descendingly
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    bool decomposition(matrix<T>& src, vector<T>& w, matrix<T>& v) const;

    /**
     * OnPlace version of Singular Value Decomposition.
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned.
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    virtual bool apply(matrix<T>& src, vector<T>& w, matrix<T>& v) const;

    /**
     * OnCopy version of Singular Value Decomposition.
     * @param src matrix<T> with the source matrix
     * @param u the U matrix
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    virtual bool apply(const matrix<T>& src, matrix<T>& u,
                       vector<T>& w, matrix<T>& v) const;

  private:
    /**
     * help method:(a^2+b^2)^0.5 without destructive underflow or overflow
     */
    T pythag(const T a,const T b) const;

    /**
     * sign
     */
    inline T sign(const T a,const T b) const {
      return (b >= T(0)) ? abs(a) : -abs(a);
    }

    /**
     * check if the given number is zero
     */
    inline bool isZero(const T x) const;

    /**
     * check if the given number is not zero
     */
    inline bool notZero(const T x) const;

    /**
     * sort singular values in w, and permute matrices u and v
     * correspondingly
     */
//     void sort(matrix<T>& u, vector<T>& w, matrix<T>& v) const;

    /**
     * swap columns i and j in both matrices, u and v
     */
//     void swapColumns(matrix<T>& u, matrix<T>& v, int& i,int& j) const;

    /**
     * Compute the dot product of a part of two matrix rows
     */
    inline T dotOfRows(const matrix<T>& data,
                       const int row1, const int row2,
                       int lowCol=0, const int highCol=MaxInt32) const {


      T sum=T(0);
      const vector<T>& rtmp1=data.getRow(row1);
      const vector<T>& rtmp2=data.getRow(row2);
      const typename vector<T>::const_iterator ie1=rtmp1.end();
      const typename vector<T>::const_iterator ie2=rtmp2.end();

      typename vector<T>::const_iterator i1=rtmp1.begin();
      typename vector<T>::const_iterator i2=rtmp2.begin();

      i1+=lowCol;
      i2+=lowCol;

      while (lowCol++ <= highCol && i1 != ie1 && i2 != ie2) {
        sum+=*i1++**i2++;
      }
      return sum;
    }

    /**
     * Compute the dot product of a part of two matrix columns
     */
    inline T dotOfColumns(const matrix<T>& data,
                          const int col1, const int col2,
                          int lowRow=0, int highRow=MaxInt32) const {
      T sum=T(0);
      highRow=min(highRow,data.rows()-1);
      while (lowRow <= highRow) {
        sum+=data.at(lowRow,col1)*data.at(lowRow,col2);
        lowRow++;
      }
      return sum;
    }


    /**
     * Compute the sum of a part of a matrix row
     */
    inline T sumOfRowPart(const matrix<T>& data,
                          const int row,
                          int lowCol=0, const int highCol=MaxInt32) const {
      T sum=T(0);
      const vector<T>& rtmp=data.getRow(row);
      const typename vector<T>::const_iterator ie=rtmp.end();

      typename vector<T>::const_iterator i=rtmp.begin();
      i+=lowCol;
      while (lowCol++ <= highCol && i != ie) {
        sum+=*i++;
      }
      return sum;
    }

    /**
     * Compute the sum of a part of a matrix column
     */
    inline T sumOfColumnPart(const matrix<T>& data,
                             const int col,
                             int lowRow=0, int highRow=MaxInt32) const {
      T sum=T(0);
      highRow=min(highRow,data.rows()-1);
      while (lowRow <= highRow) {
        sum+=data.at(lowRow,col);
        lowRow++;
      }
      return sum;
    }


    /**
     * Compute the sum of the absolute value of a part of a matrix row
     */
    inline T sumOfAbsRowPart(const matrix<T>& data,
                          const int row,
                          int lowCol=0, const int highCol=MaxInt32) const {
      T sum=T(0);
      const vector<T>& rtmp=data.getRow(row);
      const typename vector<T>::const_iterator ie=rtmp.end();

      typename vector<T>::const_iterator i=rtmp.begin();
      i+=lowCol;
      while (lowCol <= highCol && i != ie) {
        sum+=abs(*i++);
        lowCol++;
      }
      return sum;
    }

    /**
     * Compute the sum of the absolute values of a part of a matrix column
     */
    inline T sumOfAbsColumnPart(const matrix<T>& data,
                             const int col,
                             int lowRow=0, int highRow=MaxInt32) const {
      T sum=T(0);
      highRow=min(highRow,data.rows()-1);
      while (lowRow <= highRow) {
        sum+=abs(data.at(lowRow,col));
        lowRow++;
      }
      return sum;
    }

    inline void multiplyColumn(matrix<T>& data, const int col, const T factor,
                               int lowRow=0, int highRow=MaxInt32) const {
      highRow=min(highRow,data.rows()-1);
      for (int i=lowRow; i<=highRow; i++) {
        data.at(lowRow++,col)*=factor;
      }
    }

    inline void multiplyRow(matrix<T>& data, const int row, const T factor,
                            int lowCol=0, const int highCol=MaxInt32) const {
      vector<T>& rtmp=data.getRow(row);
      typename vector<T>::iterator i=rtmp.begin();
      const typename vector<T>::iterator ie=rtmp.end();
      i+=lowCol;
      while (lowCol++ <= highCol && i != ie) {
        *i++*=factor;
      }
    }

    inline void fillColumn(matrix<T>& data, const int col, const T value,
                               int lowRow=0, int highRow=MaxInt32) const {
      highRow=min(highRow,data.rows()-1);
      for (int i=lowRow; i<=highRow; i++) {
        data.at(lowRow++,col)=value;
      }
    }

    inline void fillRow(matrix<T>& data, const int row, const T value,
                        int lowCol=0, const int highCol=MaxInt32) const {
      vector<T>& rtmp=data.getRow(row);
      typename vector<T>::iterator i=rtmp.begin();
      const typename vector<T>::iterator ie=rtmp.end();
      i+=lowCol;
      while (lowCol++ <= highCol && i != ie) {
        *i++=value;
      }
    }

  };
}

#endif

