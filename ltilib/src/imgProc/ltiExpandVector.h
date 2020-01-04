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
 * file .......: ltiExpandVector.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 22.3.2001
 * revisions ..: $Id: ltiExpandVector.h,v 1.9 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_EXPAND_VECTOR_H_
#define _LTI_EXPAND_VECTOR_H_

#include "ltiFunctor.h"
#include "ltiVector.h"

namespace lti {
  /**
   * This class is used to map a function f(x) represented by a vector
   * into another scale f(g(x)), where g(x) can be given.  g(x) must
   * be monotonically increasing.
   * Linear interpolation will be used between the "samples".
   */
  class expandVector : public functor {
  public:
    /**
     * the parameters for the class expandVector
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * This function pointer specifies the coordinate mapping function.
       * The default value is \f$g(x)=2^\lfloor x \rfloor -1\f$.
       * (see intPow2m())
       */
      double (*function)(double);
    };

    /**
     * this function evaluates (2^floor(x))-1
     */
    static double intPow2m(double x);

    /**
     * default constructor.  Initialize with default parameters
     */
    expandVector();

    /**
     * constructor, with the wished default mapping function
     */
    expandVector(double (*function)(double));

    /**
     * copy constructor
     * @param other the object to be copied
     */
    expandVector(const expandVector& other);

    /**
     * destructor
     */
    virtual ~expandVector();

    /**
     * returns the name of this type ("expandVector")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest vector<float> with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    vector<float>& apply(vector<float>& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest vector<double> with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    vector<double>& apply(vector<double>& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src vector<float> with the source data.
     * @param dest vector<float> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    vector<float>& apply(const vector<float>& src,vector<float>& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src vector<double> with the source data.
     * @param dest vector<double> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    vector<double>& apply(const vector<double>& src,
                          vector<double>& dest) const;

    /**
     * short cut for apply and parameters.  This function "draw" the
     * input vector on the output vector, using the given function,
     * i.e.  out(g(x)) = in(x).  Between the samples g(x) and g(x+1)
     * linear interpolation will be used.
     * @param in input vector
     * @param out output vector
     * @param g mapping function
     */
    vector<double>& draw(const vector<double>& in,
                               vector<double>& out,
                               double (*g)(double) = intPow2m) const;

    /**
     * short cut for apply and parameters.  This function "draw" the
     * input vector on the output vector, using the given function,
     * i.e.  out(g(x)) = in(x).  Between the samples g(x) and g(x+1)
     * linear interpolation will be used.
     * @param in input vector
     * @param out output vector
     * @param g mapping function
     */
    vector<float>& draw(const vector<float>& in,
                               vector<float>& out,
                               double (*g)(double) = intPow2m) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    expandVector& copy(const expandVector& other);

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

