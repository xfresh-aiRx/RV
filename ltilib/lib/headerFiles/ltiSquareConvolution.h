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
 * file .......: ltiSquareConvolution.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 13.10.2000
 * revisions ..: $Id: ltiSquareConvolution.h,v 1.19 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SQUARE_CONVOLUTION_H_
#define _LTI_SQUARE_CONVOLUTION_H_

#include "ltiObject.h"
#include "ltiMatrix.h"
#include "ltiFilter.h"

namespace lti {
  /**
   * This is an efficient implementation of the convolution with a
   * square or rectangular kernel.  The template parameter T defines
   * the type of the matrix or vector to be filtered, and A represents
   * the accumulator class, which allows different modi for the
   * convolution.
   *
   * The class A must implement following members:
   *  - constructor(int)          for initialization with the value 0.
   *  - operator+=(T x)           for accumulation,
   *  - operator-=(T x)           for "deaccumulation",
   *  - operator=(T x)            for assignation of a value of type T
   *  - operator/(T x,T y) const  returns f(x)/y instead of x/y.
   *                              For example for L2 norm f(x) = sqrt(x).
   *  - operator T() const        returns the contents casted to type T
   *
   * This way, you can use any simple-types (as int, char, double...)
   * as accumulator for a linear filter, or a user-defined class for a
   * region normalization with any norm!  For an example of an accumulator
   * class see lti::chrominanzHistogram::l2accumulator<T>.
   *
   * Note that for convolving a rectangular filter kernel with a channel8,
   * you shoud use an instance squareConvolution<ubyte,int>.  If you forget
   * the "int" parameter, some quite noisy overflowing effects should be
   * expected!
   *
   * @see parameters
   */
  template<class T,class A=T>
    class squareConvolution : public filter {
      public:

      /**
       * the parameters for the class squareConvolution
       */
      class parameters : public filter::parameters {
        public:
        /**
         * default constructor to initialize a kernel of k x k size centered
         * at (0,0)
         */
        parameters(const int k = 3) : filter::parameters() {
          initSquare(k);
        }

        /**
         * copy constructor
         * @param other the parameters object to be copied
         */
        parameters(const parameters& other) 
          : filter::parameters() {
          copy(other);
        };

        /**
         * destructor
         */
        ~parameters() {};

        /**
         * returns name of this type
         */
        const char* getTypeName() const {
          return "squareConvolution::parameters";
        };

        /**
         * copy the contents of a parameters object
         * @param other the parameters object to be copied
         * @return a reference to this parameters object
         */
        parameters& copy(const parameters& other)  {
#       ifndef _LTI_MSC_6
          // MS Visual C++ 6 is not able to compile this...
          filter::parameters::copy(other);
#       else
          // ...so we have to use this workaround.
          // Conditional on that, copy may not be virtual.
          filter::parameters& (filter::parameters::* p_copy)
            (const filter::parameters&) =
            filter::parameters::copy;
          (this->*p_copy)(other);
#       endif

          kernelSize = other.kernelSize;
          kernelNorm = other.kernelNorm;

          return *this;
        };

        /**
         * returns a pointer to a clone of the parameters
         */
        virtual functor::parameters* clone() const  {
          return new parameters(*this);
        };

        /**
         * write the parameters in the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also written, otherwise only the data block will be
         *        written.
         * @return true if write was successful
         */
#     ifndef _LTI_MSC_6
        virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
        bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.writeBegin();
          }

          if (b) {
            lti::write(handler,"kernelSize",kernelSize);
            lti::write(handler,"kernelNorm",kernelNorm);
          }

#       ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && filter::parameters::write(handler,false);
#       else
          bool (modifier::parameters::* p_writeMS)(ioHandler&,
                                                   const bool) const =
            modifier::parameters::writeMS;
          b = b && (this->*p_writeMS)(handler,false);
#       endif

          if (complete) {
            b = b && handler.writeEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        virtual bool write(ioHandler& handler,
                           const bool complete = true) const {
           // ...we need this workaround to cope with another really
           // awful MSVC bug.
           return writeMS(handler,complete);
        }
#     endif

        /**
         * read the parameters from the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also read, otherwise only the data block will be read.
         * @return true if write was successful
         */
#     ifndef _LTI_MSC_6
        virtual bool read(ioHandler& handler,const bool complete = true)
#     else
        bool readMS(ioHandler& handler,const bool complete=true)
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.readBegin();
          }

          if (b) {
            lti::read(handler,"kernelSize",kernelSize);
            lti::read(handler,"kernelNorm",kernelNorm);
          }

#     ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && filter::parameters::read(handler,false);
#     else
          bool (modifier::parameters::* p_readMS)(ioHandler&,const bool) =
          modifier::parameters::readMS;
          b = b && (this->*p_readMS)(handler,false);
#     endif

          if (complete) {
            b = b && handler.readEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        virtual bool read(ioHandler& handler,const bool complete=true) {
          // ...we need this workaround to cope with another really awful MSVC
          // bug.
          return readMS(handler,complete);
        }
#      endif

        /**
         * The rectangular kernel will have the value "1/kernelNorm"
         * between the limits specified by this rectangle.  When
         * convolving a vector, the "y" coordinates of this kernelSize
         * will be ignored.
         * Default value: rectangle(-1,-1,1,1), i.e. a 3x3 kernel
         */
        rectangle kernelSize;

        /**
         * The norm is the value which will be considered as "1.0" for the
         * elements of the kernel.  The default value is 9 (for a 3x3 kernel).
         * You can consider the square filter always as a kernel with nxn
         * "ones".
         * After the convolution, the resulting image will be divide by this
         * value.  The implementation combines the convolution with this
         * normalization!
         */
        T kernelNorm;

        /**
         * Initialize the parameters for a symmetric square kernel sizeXsize,
         * where the sum of all elements equals one.
         * If the size is even, it will be "fixed" to the next odd number
         */
        void initSquare(const int size) {
          int realSize(size);
          if ((size % 2) == 0) {
            realSize++;
          }

          kernelNorm = static_cast<T>(realSize*realSize);

          realSize/=2;
          kernelSize.ul.x = -realSize;
          kernelSize.ul.y = -realSize;
          kernelSize.br.x = +realSize;
          kernelSize.br.y = +realSize;
        };

      };

      /**
       * default constructor
       */
      squareConvolution();

      /**
       * constructor to initialize the parameters with an square filter
       * of k x k size.
       */
      squareConvolution(const int k);

      /**
       * copy constructor
       * @param other the object to be copied
       */
      squareConvolution(const squareConvolution& other);

      /**
       * destructor
       */
      virtual ~squareConvolution();

      /**
       * returns the name of this type ("squareConvolution")
       */
      virtual const char* getTypeName() const;

      /**
       * operates on the given %parameter.
       * @param srcdest matrix<T> with the source data.  The result
       *                 will be left here too.
       * @return true if successful, false otherwise.
       */
      bool apply(matrix<T>& srcdest) const;

      /**
       * operates on the given %parameter.
       * @param srcdest vector<T> with the source data.  The result
       *                 will be left here too.
       * @return true if successful, false otherwise.
       */
      bool apply(vector<T>& srcdest) const;

      /**
       * operates on a copy of the given %parameters.
       * @param src matrix<T> with the source data.
       * @param dest matrix<T> where the result will be left.
       * @return true if successful, false otherwise.
       */
      bool apply(const matrix<T>& src,matrix<T>& dest) const;

      /**
       * operates on a copy of the given %parameters.
       * @param src vector<T> with the source data.
       * @param dest vector<T> where the result will be left.
       * @return true if successful, false otherwise.
       */
      bool apply(const vector<T>& src,vector<T>& dest) const;

      /**
       * copy data of "other" functor.
       * @param other the functor to be copied
       * @return a reference to this functor object
       */
      squareConvolution& copy(const squareConvolution& other);

      /**
       * returns a pointer to a clone of this functor.
       */
      virtual functor* clone() const;

      /**
       * returns used parameters
       */
      const parameters& getParameters() const;

      protected:
      /**
       * applies the vertical part of the kernel to the matrix
       * @param src matrix<T> with the source data.
       * @param dest matrix<T> where the result will be left.
       * @result a reference to the <code>dest</code>.
       */
      matrix<T>& applyCol(const matrix<T>& src,matrix<T>& dest) const;

      private:
      /**
       * calculate the real index in an vector of size srcSize for a
       * mirrored boundary
       */
      inline int getMirrorIndex(const int& x,
                                const int& srcSize,
                                bool& mirror) const;

    };



}

#include "ltiSquareConvolution_template.h"

#endif


