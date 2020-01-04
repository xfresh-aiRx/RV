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
 * file .......: ltiScramble.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.8.2000
 * revisions ..: $Id: ltiScramble.h,v 1.10 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_SCRAMBLE_H_
#define _LTI_SCRAMBLE_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiFunctor.h"
#include "ltiContinuousRandDist.h"

namespace lti {
  /**
   * This class is used to scramble the elements of a given vector or
   * matrix. To this end two random indices of the vector or matrix
   * are chosen and their contents swapped. This operation is
   * performed N times, with N the number of elements.
   */
  template <class T>
  class scramble : public functor {
  public:
    /**
     * the parameters for the class scramble (empty)
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() 
        : functor::parameters() {};

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : functor::parameters() {
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
        return "scramble<T>::parameters";
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#       ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
          functor::parameters::copy(other);
#       else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
          (this->*p_copy)(other);
#       endif
        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };
    };

    /**
     * default constructor
     */
    scramble();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    scramble(const scramble& other);

    /**
     * destructor
     */
    virtual ~scramble();

    /**
     * returns the name of this type ("scramble")
     */
    virtual const char* getTypeName() const;

    /**
     * Scramble the elements of \a srcdest.
     * @param srcdest matrix<T> with the source data.  The result
     *                 will be left here too.
     * @return true if operation was successfull
     */
    bool apply(matrix<T>& srcdest) const;

    /**
     * Scramble the elements of \a srcdest.
     * @param srcdest vector<T> with the source data.  The result
     *                 will be left here too.
     * @return true if operation was successfull
     */
    bool apply(vector<T>& srcdest) const;

    /**
     * Makes a scrambled copy from \a src to \a dest.
     * @param src matrix<T> with the source data.
     * @param dest matrix<T> where the result will be left.
     * @return true if operation was successfull
     */
    bool apply(const matrix<T>& src,matrix<T>& dest) const;

    /**
     * Makes a scrambled copy from \a src to \a dest.
     * @param src vector<T> with the source data.
     * @param dest vector<T> where the result will be left.
     * @return a reference to the <code>dest</code>.
     */
    bool apply(const vector<T>& src,vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    scramble& copy(const scramble& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    
    continuousRandomDistribution randomizer;

  };

}
#include "ltiScramble_template.h"

#endif


