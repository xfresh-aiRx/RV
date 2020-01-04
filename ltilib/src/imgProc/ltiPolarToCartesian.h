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
 * file .......: ltiPolarToCartesian.h
 * authors ....: Bernd Mussmann
 * organization: LTI, RWTH Aachen
 * creation ...: 4.9.2000
 * revisions ..: $Id: ltiPolarToCartesian.h,v 1.13 2005/02/02 14:14:20 doerfler Exp $
 */

#ifndef _LTI_POLAR_TO_CARTESIAN_H_
#define _LTI_POLAR_TO_CARTESIAN_H_

//include files which are needed

#include "ltiMatrix.h"
#include "ltiVector.h"
// include to parent class:
#include "ltiTransform.h"

namespace lti {
  /**
   *  Computes the elements of the cartesian coordinate system (real,imaginary) based on
   *  the parameters of the polar coordinate system (absolute value, phase).
   */
  template<class T>
  class polarToCartesian : public transform {
  public:
    /**
     * the parameters for the class polarToCartesian
     */
    class parameters : public transform::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : transform::parameters() {};

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : transform::parameters() {
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
        return "polarToCartesian::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        transform::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        transform::parameters& (transform::parameters::* p_copy)
          (const transform::parameters&) =
          transform::parameters::copy;
        (this->*p_copy)(other);
#     endif

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
    polarToCartesian();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    polarToCartesian(const polarToCartesian& other);

    /**
     * destructor
     */
    virtual ~polarToCartesian();

    /**
     * returns the name of this type ("polarToCartesian")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * If the input matrices have different size, the result will be empty
     * @param abs_r matrix<T> the absolute value source data.
     * @param ph_i matrix<T>  the phase source data.
     * @result references to abs_r (real) and
     * ph_i (imaginary).
     */
    bool apply(matrix<T>& abs_r,matrix<T>& ph_i) const;

    /**
     * operates on a copy of the given parameters.
     * If the input matrices have different size, the result will be empty
     * @param absvalue matrix<T> the absolute value source data.
     * @param phase matrix<T> the phase source data.
     * @param real reference to the target matrix which will receive the real part of the result.
     * @param imaginary reference to the target matrix which will receive the imaginary part of the result.
     */
    bool apply(const matrix<T>& absvalue,
               const matrix<T>& phase,
               matrix<T>& real,
               matrix<T>& imaginary) const;

   /**
    * operates on a copy of the given parameters.
    * @param src tpointList<T>& with the polar data the absolute in x and the angle in y.
    * @param dest this pointList gets the cartesian data
    * @param origin the origin of the coordiante system.
    *        the angles and the absolute values are relative to this point. Default is (0,0).
    */
   bool apply(const tpointList<double>& src,
                    tpointList<T> &dest, 
              const tpoint<double> origin=tpoint<double>(0.0,0.0) ) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    polarToCartesian& copy(const polarToCartesian& other);

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

#include "ltiPolarToCartesian_template.h"

#endif


