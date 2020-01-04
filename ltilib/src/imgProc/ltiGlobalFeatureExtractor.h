/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004
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
 * file .......: ltiFeatureExtractor.h
 * authors ....: Pablo Alvarado, Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 9.8.2000
 * revisions ..: $Id: ltiGlobalFeatureExtractor.h,
 */

#ifndef _LTI_GLOBAL_FEATURE_EXTRACTOR_H_
#define _LTI_GLOBAL_FEATURE_EXTRACTOR_H_

#include "ltiObject.h"
#include "ltiImage.h"
#include "ltiFeatureExtractor.h"

namespace lti {
  /**
   * The feature extractors are functors which extract information from
   * the given images or channels, and leave this information in the form
   * of vectors of doubles.  This class is the parent class for all
   * implemented feature extractors.
   */
  class globalFeatureExtractor: virtual public featureExtractor {
  public:
    /**
     * the parameters for the class globalFeatureExtractor
     */
    class parameters : virtual public featureExtractor::parameters {
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
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

    };

    /**
     * default constructor
     */
    globalFeatureExtractor();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    globalFeatureExtractor(const globalFeatureExtractor& other);

    /**
     * destructor
     */
    virtual ~globalFeatureExtractor();

    /**
     * returns the name of this type ("globalFeatureExtractor")
     */
    virtual const char* getTypeName() const;

    /**
     * generates feature vector for the src channel, and leaves the result
     * in the dest vector
     * @param src channel with the source data.
     * @param dest dvector where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual bool apply(const channel& src,dvector& dest) const;

    /**
     * generates feature vector for the src channel, and leaves the result
     * in the dest vector
     * @param src channel8 with the source data.
     * @param dest vector<double> where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual bool apply(const channel8& src,dvector& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    virtual bool apply(const image& src,dvector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    globalFeatureExtractor& copy(const globalFeatureExtractor& other);

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

