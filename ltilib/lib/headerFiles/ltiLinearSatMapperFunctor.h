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
 * file .......: ltiLinearSatMapperFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 10.7.2000
 * revisions ..: $Id: ltiLinearSatMapperFunctor.h,v 1.12 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_LINEAR_SAT_MAPPER_FUNCTOR_H_
#define _LTI_LINEAR_SAT_MAPPER_FUNCTOR_H_

#include "ltiObject.h"
#include "ltiMapperFunctor.h"

namespace lti {

  /**
   * This %functor maps a %vector space of type Tin into another %vector
   * space of type Tout.  The dimensionality and boundaries of both spaces
   * can be specified through the lti::mapperFunctor::parameters
   * class.
   *
   * The mapping-function is linear.  If an input %point is outer bounds, the
   * output %point will be corrected to the next valid dimension.
   *
   * For the following example a chromaticity %histogram will be created with
   * 10 cells for the red and 16 cells for the green channel
   *
   * \code
   *   lti::image img; // the image to be analysed
   *   // ...          // load or create the image to be analysed
   *   lti::splitImageTorgI splitter;
   *   lti::channel r,g,I;
   *
   *   splitter.apply(r,g,I); // get chromaticity channels
   *
   *   lti::histogram2D histo(10,16);  // a two dimensional histogram
   *   // the mapper functor
   *   lti::linearSatMapperFunctor<float> mapper;
   *   lti::linearSatMapperFunctor<float>::parameters mapParam;
   *
   *   mapParam.generateFrom(histo);
   *   mapParam.lowerLimitsInput.fill(0.0f);
   *   mapParam.upperLimitsInput.fill(1.0f);
   *
   *   histo.setParameters(mapParam);
   *
   *   lti::point p,q;
   *
   *   // for each pixel in the channels
   *   for (p.y=0;p.y<r.rows();p.y++) {
   *     for (p.x=0;p.x<g.rows();p.x++) {
   *       histo.put(mapper(r.at(p),g.at(p),q));
   *     }
   *   }
   * \endcode
   */
  template <class Tin, class Tout=int>
    class linearSatMapperFunctor : public mapperFunctor<Tin,Tout> {
    public:
    /**
     * the parameters for the class linearSatMapperFunctor
     */
    class parameters : public mapperFunctor<Tin,Tout>::parameters {
      public:
      /**
       * default constructor
       */
      parameters() : mapperFunctor<Tin,Tout>::parameters() {
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other){
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
        return "linearSatMapperFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        mapperFunctor<Tin,Tout>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        mapperFunctor<Tin,Tout>::parameters&
          (mapperFunctor<Tin,Tout>::parameters::* p_copy)
          (const mapperFunctor<Tin,Tout>::parameters&) =
          mapperFunctor<Tin,Tout>::parameters::copy;
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
    linearSatMapperFunctor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    linearSatMapperFunctor(const linearSatMapperFunctor& other);

    /**
     * destructor
     */
    virtual ~linearSatMapperFunctor();

    /**
     * returns the name of this type ("linearSatMapperFunctor")
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
     * For the two dimensional case, maps a two dimensional integer vector in
     * another integer vector, both coded as points.
     * @param src the source point.
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
    linearSatMapperFunctor& copy(const linearSatMapperFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set the parameters and check them for correctness
     */
    virtual bool updateParameters();

  protected:
    /**
     * slopes of the mapping lines.
     *
     * These are calculated when setParameters
     * is called
     */
    fvector slope;

    /**
     * offset of the mapping lines.
     *
     * These are calculated when setParameters
     * is called
     */
    fvector b;

  };
}

#include "ltiLinearSatMapperFunctor_template.h"

#endif

