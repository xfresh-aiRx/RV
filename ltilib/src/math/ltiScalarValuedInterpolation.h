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
 * file .......: ltiScalarValuedInterpolation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiScalarValuedInterpolation.h,v 1.11 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_SCALARVALUEDINTERPOLATION_H_
#define _LTI_SCALARVALUEDINTERPOLATION_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiEquallySpacedSamplesInterpolator.h"

namespace lti {
  /**
   * This abstract class parents all interpolation functors, which are used
   * to interpolate the values between the pixels or elements of vectors
   * and matrices, espacially images.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class scalarValuedInterpolation : public equallySpacedSamplesInterpolator {
  public:

    /**
     * Definition of the type of the elements in the applys (given type T)
     */ 
    typedef T value_type;

    /**
     * the parameters for the class scalarValuedInterpolation
     */
    class parameters : public equallySpacedSamplesInterpolator::parameters {
    public:
      /**
       * Default constructor
       */
      parameters() : equallySpacedSamplesInterpolator::parameters() {
      };

      /**
       * Create constructor with given boundary type
       */
      parameters(const eBoundaryType btype)
        : equallySpacedSamplesInterpolator::parameters(btype) {
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : equallySpacedSamplesInterpolator::parameters() {
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
        return "scalarValuedInterpolation::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#       ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        equallySpacedSamplesInterpolator::parameters::copy(other);
#       else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        equallySpacedSamplesInterpolator::parameters& 
          (equallySpacedSamplesInterpolator::parameters::* p_copy)
          (const equallySpacedSamplesInterpolator::parameters&) =
          equallySpacedSamplesInterpolator::parameters::copy;
        (this->*p_copy)(other);
#       endif

        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was succeful
       */
      bool write(ioHandler& handler,const bool complete) const
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {

        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && equallySpacedSamplesInterpolator::parameters::write(handler,false);
#       else
        bool (equallySpacedSamplesInterpolator::parameters::* p_writeMS)(ioHandler&,
                                                 const bool) const =
          equallySpacedSamplesInterpolator::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

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
       * @return true if write was succeful
       */
      bool write(ioHandler& handler,const bool complete) const {
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
       *        be also read, otherwise only the data block will be read.
       * @return true if write was succeful
       */
      bool read(ioHandler& handler,const bool complete)
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

        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && equallySpacedSamplesInterpolator::parameters::read(handler,false);
#     else
        bool (equallySpacedSamplesInterpolator::parameters::* p_readMS)(ioHandler&,const bool) =
          equallySpacedSamplesInterpolator::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

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
       *        be also read, otherwise only the data block will be read.
       * @return true if write was succeful
       */
      bool read(ioHandler& handler,const bool complete) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
# endif
      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
      
      // no parameters yet

    };

    /**
     * default constructor
     */
    scalarValuedInterpolation();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    scalarValuedInterpolation(const scalarValuedInterpolation& other);

    /**
     * destructor
     */
    virtual ~scalarValuedInterpolation();

    /**
     * returns the name of this type ("scalarValuedInterpolation")
     */
    virtual const char* getTypeName() const;

    /**
     * all the next apply methods will return the interpolated values of the
     * given vector.
     */
    virtual bool use(const vector<T>& vct);

    /**
     * all the next apply methods will return the interpolated values of the
     * given matrix.
     */
    virtual bool use(const matrix<T>& vct);

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    virtual T apply(const vector<T>& src,const float& x) const = 0;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    virtual T apply(const float& x) const = 0;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& src,
                    const float& row,
                    const float& col) const = 0;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position p.
     *
     * @param src matrix<T> with the source data.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& src,const tpoint<float>& p) const = 0;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    virtual T apply(const float& row, const float& col) const = 0;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     *
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const tpoint<float>& p) const = 0;


    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).  This method is NOT virtual and can be used
     * if this interpolation type is used as template parameter in time
     * critical situations.
     *
     * If not reimplemented in the inherited class, the virtual apply method
     * with the same signature will be called.
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const matrix<T>& src,
                         const float& row,
                         const float& col) const {
      return apply(src,row,col);
    };

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scalarValuedInterpolation& copy(const scalarValuedInterpolation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * the vector in use
     */
    const vector<T>* theVector;

    /**
     * the matrix in use
     */
    const matrix<T>* theMatrix;

  protected:
    /**
     * @name Boundary access operators
     */
    //@{
    /**
     * Access with zero boundary
     */
    inline T zeroAt(const vector<T>& img,const int x) const;
                                  
    /**
     * Access with constant boundary
     */
    inline T cstAt(const vector<T>& img,const int x) const;

    /**
     * Access with mirrored boundary
     */
    inline T mirrAt(const vector<T>& img,const int x) const;

    /**
     * Access with periodic boundary
     */
    inline T periAt(const vector<T>& img,const int x) const;


    /**
     * Access with zero boundary
     */
    inline T zeroAt(const matrix<T>& img,const int y,const int x) const;
                                  
    /**
     * Access with constant boundary
     */
    inline T cstAt(const matrix<T>& img,const int y,const int x) const;

    /**
     * Access with mirrored boundary
     */
    inline T mirrAt(const matrix<T>& img,const int y,const int x) const;

    /**
     * Access with periodic boundary
     */
    inline T periAt(const matrix<T>& img,const int y,const int x) const;
    //@}
  };

}

#include "ltiScalarValuedInterpolation_template.h"

#endif

