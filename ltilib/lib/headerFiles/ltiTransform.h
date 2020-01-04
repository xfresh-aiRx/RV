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
 * file .......: ltiTransform.h
 * authors ....: Stefan Syberichs, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 06.12.99
 * revisions ..: $Id: ltiTransform.h,v 1.5 2004/05/03 12:18:40 alvarado Exp $
 */


#ifndef _LTI_TRANSFORM_H_
#define _LTI_TRANSFORM_H_

#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiMath.h"


namespace lti {

  /**
   * A base class for integral transformations.
   * Transform is the base class for integral transformations like DFFT, DCT
   * and others. The class has no dedicated parameters.
   */
  class transform : public functor {
  public:

    /**
     * transform Functor parameters class.
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : functor::parameters() {};

      /**
       * copy constructor.
       * @param other the parameter object to copy
       */
      parameters(const parameters& other) : functor::parameters() {
        copy(other);
      };

      /**  copy member function.
       * @param other the parameter object to copy
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const;

      /**
       * returns name of this type
       */
      virtual const char* getTypeName() const;
    };

    /**
     * default constructor
     */
    transform() 
      : functor() {};

    /**
     * constructor, sets the parameters
     * @param theParam parameter object of type lti::transform::parameters.
     */
    transform(const parameters& theParam);

    /**
     * destructor
     */
    virtual ~transform() {};

    /**
     * get the current parameters
     */
    const parameters& getParameters() const;

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;
  };

} // namespace lti
#endif


