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
 * file .......: ltiLogarithm.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 16.04.99
 * revisions ..: $Id: ltiLogarithm.h,v 1.7 2003/04/14 08:11:08 sltilib Exp $
 */

#ifndef _LTI_LOGARITHM_H_
#define _LTI_LOGARITHM_H_

#include "ltiIteratingFunctor.h"
#include "ltiMath.h"

namespace lti {
  /** logarithm functor class.
      apply sets each element of the T-object to it's logarithm.*/
  template<class T>
  class logarithm : public iteratingFunctor<T> {
  public:
    /// default constructor
    logarithm() {};
    /// destructor
    virtual ~logarithm() {};

    /** onPlace version of apply.
	applies the functor's function to <em>theObject</em>. */
    virtual vector<T>& apply(vector<T>& theObject) const;

    /** onCopy version of apply.
	applies the functor's function to the copy of <em>theObject</em>,
	<em>newObject</em>. */
    virtual vector<T>& apply(const vector<T>& theObject,
                                   vector<T>& newObject) const;

    /** onPlace version of apply.
	applies the functor's function to <em>theObject</em>. */
    virtual matrix<T>& apply(matrix<T>& theObject) const;

    /** onCopy version of apply.
	applies the functor's function to the copy of <em>theObject</em>,
	<em>newObject</em>. */
    virtual matrix<T>& apply(const matrix<T>& theObject,
                                   matrix<T>& newObject) const;

    /// returns a pointer to a clone of the functor.
    virtual functor* clone() const { return (new logarithm(*this));};

    /// returns the name of this type
    virtual const char* getTypeName() const {return "logarithm";};

  protected:
    inline static T logT(const T& value);
  };

  template<class T>
    inline T logarithm<T>::logT(const T& value) {
    return static_cast<T>(log(static_cast<double>(value)));
  }

}

#include "ltiLogarithm_template.h"

#endif

