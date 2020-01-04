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
 * file .......: ltiMorphology.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.7.2000
 * revisions ..: $Id: ltiMorphology.h,v 1.7 2004/05/03 12:18:38 alvarado Exp $
 */

#ifndef _LTI_MORPHOLOGY_H_
#define _LTI_MORPHOLOGY_H_

#include "ltiObject.h"
#include "ltiModifier.h"

namespace lti {
  /**
   *  Base class for all morphological operators
   */
  class morphology : public modifier {
  public:
    /**
     * the parameters for the class morphology
     */
    class parameters : public modifier::parameters {
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
    morphology();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    morphology(const morphology& other);

    /**
     * destructor
     */
    virtual ~morphology();

    /**
     * returns the name of this type ("morphology")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(channel& srcdest) const = 0;

    /**
     * operates on the given parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(channel8& srcdest) const = 0;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const = 0;

    /**
     * operates on a copy of the given parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const = 0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    morphology& copy(const morphology& other);

    /*
     * returns a pointer to a clone of this functor.
     */
    //virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

