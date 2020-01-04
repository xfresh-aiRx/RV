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
 * file .......: ltiMapperFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 10.7.2000
 * revisions ..: $Id: ltiMapperFunctor.h,v 1.11 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_MAPPER_FUNCTOR_H_
#define _LTI_MAPPER_FUNCTOR_H_

#include "ltiObject.h"
#include "ltiFunctor.h"
#include "ltiHistogram.h"

namespace lti {

  /**
   * This class maps a vector in a n-dimensional space into another vector
   * in a m-dimensional space.  The main use of the "mappers" is to
   * map a n-dimensional space in the integers cell space required by the
   * histograms.
   *
   * The type of the input space is <code>Tin</code>, and the type of the
   * output space is <code>Tout</code> (default value <code>int</code>.
   *
   * This is an abstract class.  For an example see lti::linearMapper
   *
   * There are only on-copy apply-members.
   */
  template <class Tin,class Tout=int>
  class mapperFunctor : public functor {
  public:
    /**
     * the parameters for the class mapperFunctor
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : functor::parameters() {
      };

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
      const char* getTypeName() const  {
        return "mapperFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
# endif

        lowerLimitsInput.copy(other.lowerLimitsInput);
        lowerLimitsOutput.copy(other.lowerLimitsOutput);
        upperLimitsInput.copy(other.upperLimitsInput);
        upperLimitsOutput.copy(other.upperLimitsOutput);

        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * use the description of a histogram to initialize
       * the parameter members.
       *
       * The size of the four limits-vectors will be changed to the
       * dimensionality of the histogram, and the output range will
       * be set to the valid range of the histogram.  The input intervall
       * will be left uninitialized!  (normally you will use fill to set
       * the values you want here: see lti::ltiLinearMapperFunctor for an
       * example)
       */
      void generateFrom(const histogram& histo)  {
        lowerLimitsInput.resize(histo.dimensions(),Tin(0.0),false);
        upperLimitsInput.resize(histo.dimensions(),Tout(1.0),false);
        lowerLimitsOutput.castFrom(histo.getFirstCell());
        upperLimitsOutput.castFrom(histo.getLastCell());
      };

      /**
       * a vector with the input dimensionality, which contains the
       * lower bounds of the input vector space.
       */
      vector<Tin> lowerLimitsInput;

      /**
       * a vector with the input dimensionality, which contains the
       * upper bounds of the input vector space
       */
      vector<Tin> upperLimitsInput;

      /**
       * a vector with the output dimensionality, which contains the
       * lower bounds of the output vector space.
       */
      vector<Tout> lowerLimitsOutput;

      /**
       * a vector with the output dimensionality, which contains the
       * upper bounds of the output vector space
       */
      vector<Tout> upperLimitsOutput;
    };

    /**
     * default constructor
     */
    mapperFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    mapperFunctor(const mapperFunctor& other);

    /**
     * destructor
     */
    virtual ~mapperFunctor();

    /**
     * returns the name of this type ("mapperFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * Map the input vector src in the output vector dest.
     * @param src vector<Tin> with the source data.
     * @param dest vector<Tout> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual vector<Tout>& apply(const vector<Tin>& src,
                                      vector<Tout>& dest) const;

    /**
     * For the two dimensional case, maps a two dimensional vector in a
     * integer vector coded as a point
     * @param src ivector with the source data.
     * @param dest point where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    point& apply(const vector<Tin>& src,point& dest) const;

    /**
     * For the two dimensional case, maps a two dimensional vector in a
     * integer vector coded as a point
     * @param src0 the first element of the input vector
     * @param src1 the second element of the input vector
     * @param dest point where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    point& apply(const Tin& src0,const Tin& src1,
                 point& dest) const;


    /**
     * For the two dimensional case, maps a two dimensional integer vector in a
     * integer vector coded both as points
     * @param src the original point
     * @param dest point where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    point& apply(const point& src,point& dest) const;

    /**
     * For the one dimensional case, maps the input vector into a one
     * dimensional vector of type Tout.
     * @param src int with the source data.
     * @param dest int where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    Tout& apply(const vector<Tin>& src,Tout& dest) const;

    /**
     * For the one dimensional case, maps a one dimensional vector in a
     * output vector
     * @param src one-dimensional vector with the source data.
     * @param dest one-dimensional vector where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    Tout& apply(const Tin& src,Tout& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mapperFunctor& copy(const mapperFunctor& other);

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

#include "ltiMapperFunctor_template.h"

#endif

