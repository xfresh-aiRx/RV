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
 * file .......: ltiCartesianToPolar.h
 * authors ....: Bernd Mussmann
 * organization: LTI, RWTH Aachen
 * creation ...: 18.8.2000
 * revisions ..: $Id: ltiCartesianToPolar.h,v 1.18 2005/02/02 14:14:16 doerfler Exp $
 */

#ifndef _LTI_CARTESIAN_TO_POLAR_H_
#define _LTI_CARTESIAN_TO_POLAR_H_

//include files which are needed

#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiPointList.h"
#include "ltiArctanLUT.h"
// include parent class:
#include "ltiTransform.h"

namespace lti {
  /**
   *  Computes the elements of the polar coordinate system
   *  (value,phase) based on the parameters of the cartesian
   *  coordinate system (x,y).
   */
  template<class T>
  class cartesianToPolar : public transform {
    public:
    /**
     * value type of the used matrices
     */
    typedef T value_type;

    /**
     * the parameters for the class cartesianToPolar
     */
    class parameters : public transform::parameters {
      public:
        /**
         * default constructor
         */
        parameters(): transform::parameters() {
          // Initialize default parameter values
          useLUT = false;
        };

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
          return "cartesianToPolar::parameters";
        };

        /**
         * copy the contents of a parameters object
         * @param other the parameters object to be copied
         * @return a reference to this parameters object
         */
        parameters& copy(const parameters& other)  {
#       ifndef _LTI_MSC_6
          // MS Visual C++ 6 is not able to compile this...
          transform::parameters::copy(other);
#       else
          // ...so we have to use this workaround.
          // Conditional on that, copy may not be virtual.
          transform::parameters& (transform::parameters::* p_copy)
            (const transform::parameters&) =
            transform::parameters::copy;
          (this->*p_copy)(other);
#       endif

          useLUT = other.useLUT;

          return *this;
        };

        /**
         * returns a pointer to a clone of the parameters
         */
        virtual functor::parameters* clone() const {
          return new parameters(*this);
        };

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

     /**
      * useLUT specifies if arctanLUT is used to calculate atan values
      *
      * Default Value: false;
      */
      bool useLUT;

    };

    /**
     * default constructor
     */
    cartesianToPolar();

    /**
     * constructor with parameters
     */
    cartesianToPolar(const parameters& thePars);

    /** 
     * constructor which uses the arctan lookup table if its argument is true
     */
    cartesianToPolar(bool useLUT);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    cartesianToPolar(const cartesianToPolar& other);

    /**
     * destructor
     */
    virtual ~cartesianToPolar();

    /**
     * returns the name of this type ("cartesianToPolar")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * @param r_abs the real part of the source data
     *              (magnitude will be left here).
     * @param i_ph  the imaginary part of the source data
     *              (phase will be left here).
     */
    bool apply(matrix<T>& r_abs ,matrix<T>& i_ph);

    /**
     * operates on the given parameter.
     * @param r_abs the real part of the source data
     *              (magnitude will be left here).
     * @param i_ph  the imaginary part of the source data
     *              (phase will be left here).
     */
    bool apply(vector<T>& r_abs ,vector<T>& i_ph);

    /**
     * operates on a copy of the given parameters.
     * If the source matrices have different sizes, the returned data
     * will be empty.
     * @param real matrix<T> with the real source data.
     * @param imaginary matrix<T> the imaginary source data.
     * @param absvalue absolute value of the input data.
     * @param phase phase of the input data.
     */
    bool apply(const matrix<T>& real,
               const matrix<T>& imaginary,
               matrix<T>& absvalue,
               matrix<T>& phase);

    /**
     * operates on a copy of the given parameters.
     * If the source matrices have different sizes, the returned data
     * will be empty.
     * @param real vector with the real part of the source data.
     * @param imaginary vector the imaginary part of the source data.
     * @param absvalue absolute value of the input data.
     * @param phase phase of the input data.
     */
    bool apply(const vector<T>& real,
               const vector<T>& imaginary,
               vector<T>& absvalue,
               vector<T>& phase);

    /**
     * operates on a copy of the given parameters.
     * When this apply is invoked the Arctan Lookup Table is not used
     * regardless of the useLUT parameter.
     * @param src list of points with the cartesian data.
     * @param dest resulting list, in which the points contains in their
     *             member x the magnitude (or radius) and the angle in the y.
     * @param origin optional parameter which gives the position considered
     *               as origin for the cartesic-polar conversion
     */
    bool apply(const tpointList<T>& src,
                     tpointList<double> &dest,
               const tpoint<double> origin=tpoint<double>(0.0,0.0) ) const;


    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cartesianToPolar& copy(const cartesianToPolar& other);

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


