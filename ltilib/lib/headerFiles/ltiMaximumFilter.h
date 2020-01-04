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
 * file .......: ltiMaximumFilter.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 08.01.2002
 * revisions ..: $Id: ltiMaximumFilter.h,v 1.12 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_MAXIMUM_FILTER_H_
#define _LTI_MAXIMUM_FILTER_H_

#include "ltiObject.h"
#include "ltiMatrix.h"
#include "ltiFilter.h"
#include <cstring>

namespace lti {
  /**
   * This is an efficient implementation of the non-linear maximum filter.
   * It assigns each pixel the maximal value in the region defined by
   * the parameter kernelSize.
   *
   * The template parameter T defines the type of the matrix or vector
   * to be filtered.
   *
   * An instance of this class cannot be used from different threads (not
   * thread-save).  If you have multiple threads, use simply one functor for
   * each thread.
   *
   * @see parameters
   */
  template<class T>
  class maximumFilter : public filter {
  public:

    /**
     * the parameters for the class maximumFilter
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
        return "maximumFilter::parameters";
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
       * The rectangular kernel defines the region around the actual pixel
       * in which the maximum value will be determined.
       *
       * Default value: rectangle(-1,-1,1,1), i.e. a 3x3 kernel
       */
      rectangle kernelSize;

      /**
       * Initialize the parameters for a symmetric square kernel sizeXsize.
       * If the size is even, it will be "fixed" to the next odd number
       */
      void initSquare(const int size) {
        int realSize(size);
        if ((size % 2) == 0) {
          realSize++;
        }

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
    maximumFilter();

    /**
     * constructor with a set of parameters
     */
    maximumFilter(const parameters& par);

    /**
     * constructor to initialize the parameters with an square filter
     * of k x k size.
     */
    maximumFilter(const int k);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    maximumFilter(const maximumFilter& other);

    /**
     * destructor
     */
    virtual ~maximumFilter();

    /**
     * returns the name of this type ("maximumFilter")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(matrix<T>& srcdest);

    /**
     * operates on the given %parameter.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(vector<T>& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const matrix<T>& src,matrix<T>& dest);

    /**
     * operates on a copy of the given %parameters.
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const vector<T>& src,vector<T>& dest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    maximumFilter& copy(const maximumFilter& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut to set the parameters with the given kernel size
     */
    bool setKernelSize(const rectangle& ksize);

    /**
     * shortcut to set the parameters with a square kernel (see also
     * parameters::initSquare)
     */
    bool setSquareKernel(const int& ksize);

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();


  protected:
    /**
     * applies the vertical part of the kernel to the matrix
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    bool applyCol(const matrix<T>& src,matrix<T>& dest);

    /**
     * calculate the real index in an vector of size srcSize for a
     * mirrored boundary
     */
    inline int getMirrorIndex(const int& x,
                              const int& srcSize,
                              bool& mirror) const;

    /**
     * Sorted elements.  The memory is administrated by setParameters
     * and the destructor.
     */
    T* rawBuffer;

    /**
     * pointer to the buffer withing rawBuffer (there are elements after
     * and before used to check boundary conditions
     */
    T* buffer;


    /**
     * insert the element "elem" in the buffer of size "size" pointed by
     * the attribute buffer. The last element of the buffer is supposed
     * to be uninitialized.
     */
    void insert(const int size,const T elem);

    /**
     * insert the element "elem" in the given buffer of size "size"
     */
    void insdel(const int size,const T insElem,const T delElem);
  };



}
#endif


