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
 * file .......: ltiSquare.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 16.04.99
 * revisions ..: $Id: ltiSquare.h,v 1.4 2003/02/17 07:16:46 alvarado Exp $
 */

#ifndef _LTI_SQUARE_H_
#define _LTI_SQUARE_H_

#include "ltiIteratingFunctor.h"

namespace lti {
  /** square functor class.
      apply sets each element of the T-object to it's square value.*/
  template<class T>
    class square : public iteratingFunctor<T> {
    public:
    /// default constructor
    square() {};

    /// destructor
    virtual ~square() {};

    /** onPlace version of apply.
        applies the functor's function to <code>theObject</code>. */
    virtual vector<T>& apply(vector<T>& theObject) const;

    /** onCopy version of apply.
        applies the functor's function to the copy of <code>theObject</code>,
        <code>newObject</code>.
    */
    virtual vector<T>& apply(const vector<T>& theObject,
                                   vector<T>& newObject) const;

    /** onPlace version of apply.
        applies the functor's function to <code>theObject</code>. */
    virtual matrix<T>& apply(matrix<T>& theObject) const;

    /** onCopy version of apply.
        applies the functor's function to the copy of <code>theObject</code>,
        <code>newObject</code>.
    */
    virtual matrix<T>& apply(const matrix<T>& theObject,
                                   matrix<T>& newObject) const;

    /// returns a pointer to a clone of the functor.
    virtual functor* clone() const { return (new square(*this));};

    /// returns the name of this type
    virtual const char* getTypeName() const {return "square";};
  };
}

#include "ltiSquare_template.h"

#endif

