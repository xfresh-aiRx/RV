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
 * file .......: ltiSaliency.h
 * authors ....: Stefan Syberichs
 * organization: LTI, RWTH Aachen
 * creation ...: 17.04.2000
 * revisions ..: $Id: ltiSaliency.h,v 1.9 2005/02/02 14:14:20 doerfler Exp $
 */

#ifndef _LTI_SALIENCY_H_
#define _LTI_SALIENCY_H_

#include "ltiImage.h"
#include "ltiModifier.h"

namespace lti {


  /**
   * base class for all saliency modifiers.
   */
  class saliency : public modifier {
  public:
    /**
     * %parameters class for lti::saliency objects.
     */
    class parameters : public modifier::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : modifier::parameters() {};

      /**
       * copy constructor
       */
      parameters(const parameters& other) 
        : modifier::parameters() {
        copy(other);
      };

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "saliency::parameters";
      };

      /**
       * copy member
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        modifier::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        modifier::parameters& (modifier::parameters::* p_copy)
          (const modifier::parameters&) = modifier::parameters::copy;
        (this->*p_copy)(other);
#      endif

        return (*this);
      };

      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };

    }; // end of parameters class

    //virtual channel&  apply(const image& src, channel& dest) const=0;

  };
}; // namespace lti

#endif


