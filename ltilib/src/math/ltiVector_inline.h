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
 * file .......: ltiVector_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 13.06.00
 * revisions ..: $Id: ltiVector_inline.h,v 1.12 2003/11/07 07:31:34 alvarado Exp $
 */

#ifndef _LTI_VECTOR_INLINE_H_
#define _LTI_VECTOR_INLINE_H_

namespace lti {

  /*
    Divide this vector with a constant.  This vector will changed!
    Returns this vector.
    synonym for divide(const T cst).
    @param cst the elements of the vector will be divided with this
    constant
    @return a reference to the actual vector
  */
  template<class T>
  inline vector<T>& vector<T>::edivide(const T cst) {
    return divide(cst);
  }

  /*
    Divide the other vector with a constant and leave the result here.
    Returns a reference to this vector. <p>
    synonym for divide(const vector<T>& other,const T cst).
    @param other the vector to be divide by the constant value
    @param cst the elements of the vector will be divided with this
    constant
    @return a reference to the actual vector
  */
  template<class T>
  inline vector<T>& vector<T>::edivide(const vector<T>& other,const T cst) {
    return divide(other, cst);
  }

} // namespace lti

#endif

