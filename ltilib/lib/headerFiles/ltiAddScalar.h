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
 * file .......: ltiAddScalar.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 20.04.99
 * revisions ..: $Id: ltiAddScalar.h,v 1.5 2003/02/17 07:16:39 alvarado Exp $
 */

#ifndef _LTI_ADDSCALAR_H_
#define _LTI_ADDSCALAR_H_

#include "ltiScalarFunctor.h"

namespace lti {
  /**
   * addScalar functor class.
   * Apply adds the given scalar value to each element of the MathObj.
   */
  template<class T>
  class addScalar : public scalarFunctor<T> {
    public:
    typedef typename scalarFunctor<T>::parameters parameters;

    /// default constructor
    addScalar() {};

    /// constructor, sets the scalar value.
    addScalar(const T& theValue) : scalarFunctor<T>(theValue) {};

    /// constructor, sets the parameters, i.e. the scalar value.
    addScalar(const parameters& theParams)
      : scalarFunctor<T>(theParams) {};

    /// destructor
    virtual ~addScalar();

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
    virtual functor* clone() const;

    /// returns the name of this type
    virtual const char* getTypeName() const;
  };
}

#include "ltiAddScalar_template.h"

#endif
