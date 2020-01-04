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
 * file .......: ltiEntropyFunctor.h
 * authors ....: Stefan Syberichs
 * organization: LTI, RWTH Aachen
 * creation ...: 5.2.2001
 * revisions ..: $Id: ltiEntropyFunctor.h,v 1.12 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_ENTROPY_FUNCTOR_H_
#define _LTI_ENTROPY_FUNCTOR_H_

#include "ltiStatisticsFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Functor which computes entropy for vectors and matrices.  The
   * template parameter T corresponds to the value type of the objects
   * to be evaluated.  For example, to calculate the entropy of a
   * vector<double> you will need an entropyFunctor<double>.
   *
   * This functor assumes that the rows (or columns) of matrix contain
   * probability distributions, i.e. the sum of the rows (column) elements
   * must be 1.0.  The entropy for the row will be defined as the sum of
   * p(x)*ln(p(x)) for all x, where p(x) are the elements of the vector, row
   * or column.
   */
  template <class T> class entropyFunctor : public statisticsFunctor {
  public:
    /**
     * the parameters for the class entropyFunctor
     */
    class parameters : public statisticsFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : statisticsFunctor::parameters() {
        rowWise=true;
        normalize = false;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : statisticsFunctor::parameters() {
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
        return "entropyFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        statisticsFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        statisticsFunctor::parameters&
          (statisticsFunctor::parameters::* p_copy)
          (const statisticsFunctor::parameters&) =
          statisticsFunctor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        rowWise=other.rowWise;
        normalize=other.normalize;

        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        
        if (b) {
          lti::write(handler,"rowWise",rowWise);
          lti::write(handler,"normalize",normalize);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && statisticsFunctor::parameters::write(handler,false);
#     else
        bool (statisticsFunctor::parameters::* p_writeMS)(ioHandler&,
                                                          const bool) const =
          statisticsFunctor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        
        if (b) {
          lti::read(handler,"rowWise",rowWise);
          lti::read(handler,"normalize",normalize);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && statisticsFunctor::parameters::read(handler,false);
#       else
        bool (statisticsFunctor::parameters::* p_readMS)(ioHandler&,
                                                         const bool) =
          statisticsFunctor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      /**
       * If this flag is true, the entropy computation will be row-wise, i.e.
       * the result will contain the entropy of the rows. This is much faster
       * than column-wise, since data do not have to be copied.
       * Default value: true
       */
      bool rowWise;

      /**
       * if false, it will be assume that the matrices or vectors are
       * already normalized, and can be considered directly as probability
       * distributions (which sum is 1.0).  If true, the vectors and matrices
       * will be "normalized" first, in an efficient way.
       * Default value: false
       */
      bool normalize;
    };

    /**
     * default constructor
     */
    entropyFunctor();

    /**
     * constructor with parameter preinitialization
     */
    entropyFunctor(const bool& normalize,const bool& rowWise=true);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    entropyFunctor(const entropyFunctor& other);

    /**
     * destructor
     */
    virtual ~entropyFunctor();

    /**
     * returns the name of this type ("entropyFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * returns the entropy of the current vector.
     *
     * The entropy is defined as the sum for all vector elements of
     * p(x)*ln(p(x)), with p(x) = srcdest.at(x). Warning: Elements that 
     * are less than zero are ignored.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    T apply(const vector<T>& srcdest) const;

    /**
     * computes the entropy of the given vector.
     *
     * The entropy is defined as the sum for all vector elements of
     * p(x)*ln(p(x)), with p(x) = srcdest.at(x). Warning: Elements that 
     * are less than zero are ignored.
     *
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    T &apply(const vector<T>& src, T& dest) const;

    /**
     * The result of this function depends on the value of
     * parameters.rowWise. If this parameter is true, the functor
     * will compute a vector, whose elements contain each the entropy
     * of one column of the matrix (this computes the entropy of the rows,
     * where each row is a data point in n-dimensional space.
     * if rowWise is false, the result vector contains the entropy
     * of the columns of the matrix (each column a data point).
     *
     * The entropy of a row or column is defined as the sum for all
     * its elements of p(x)*log2(p(x)), with p(x) = elements of the row or
     * column.
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    vector<T>& apply(const matrix<T>& src, vector<T>& dest) const;

    /**
     * computes the entropy of the given matrix.  Consider the matrix
     * as a two-dimensional probability distribution.
     *
     * The entropy is defined as the sum for all vector elements of
     * p(x,y)*ln(p(x,y)), with p(x,y) = src.at(x,y).
     *
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    T &apply(const matrix<T>& src, T& dest) const;

    /**
     * This function
     * will compute a vector, whose elements contain each the entropy
     * of one column of the matrix (this computes the entropy of the rows,
     * where each row is a data point in n-dimensional space).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void entropyOfRows(const matrix<T>& src, vector<T>& dest) const;

    /**
     * The result vector contains the entropy
     * of the columns of the matrix (each column a data point).
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     */
    void entropyOfColumns(const matrix<T>& src, vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    entropyFunctor& copy(const entropyFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

