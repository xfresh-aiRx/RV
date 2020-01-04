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
 * file .......: ltiGenericVector_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 13.06.00
 * revisions ..: $Id: ltiGenericVector_inline.h,v 1.2 2004/03/03 12:48:29 doerfler Exp $
 */

#ifndef _LTI_GENERIC_VECTOR_INLINE_H_
#define _LTI_GENERIC_VECTOR_INLINE_H_

#include "ltiAssert.h"

namespace lti {
  template<class T>
  inline bool genericVector<T>::ownsData() const {
    return ownData;
  };

  // returns the number of elements of the genericVector
  template<class T>
  inline const int& genericVector<T>::size() const {
    return vectorSize;
  };
  
  // returns first index (normally 0)
  template<class T>
  inline int genericVector<T>::firstIdx() const {
    return 0;
  };
  
  /* returns first element as a const_iterator.
     Note that you can not change the values of the genericVector
     elements when you use a const_iterator. See also begin() */
  template<class T>
  inline typename genericVector<T>::const_iterator 
  genericVector<T>::begin() const {
#   ifdef NDEBUG
    return const_iterator(theElements);
#   else
    return const_iterator(0,this);
#   endif
  };

  /* returns first element as an iterator */
  template<class T>
  inline typename genericVector<T>::iterator genericVector<T>::begin() {
#   ifdef NDEBUG
    return iterator(theElements);
#   else
    return iterator(0,this);
#   endif
  };

  // returns last index (in a genericVector this is always size()-1)
  template<class T>
  inline int genericVector<T>::lastIdx() const {
    return idxLastElement;
  };

  /* returns last index as a const iterator.
     For an example see begin() */
  template<class T>
  inline typename genericVector<T>::const_iterator 
  genericVector<T>::end() const {
#   ifdef NDEBUG
    return const_iterator(&theElements[vectorSize]);
#   else
    return const_iterator(size(),this);
#   endif
  };

  /* returns last index as an iterator
     For an example see begin() */
  template<class T>
  inline typename genericVector<T>::iterator genericVector<T>::end() {
#   ifdef NDEBUG
    return iterator(&theElements[vectorSize]);
#   else
    return iterator(size(),this);
#   endif
  };


  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericVector. It is used for inverse order
   * iteration through the genericVector using normal iterators (as opposed
   * to reverse iterators). This has the advantage that iterators
   * going from front to end and in the inverse direction are the
   * same and can thus be compared, copied etc.
   */
  template<class T>
  inline typename genericVector<T>::iterator genericVector<T>::inverseBegin() {
#   ifdef NDEBUG
    return iterator(&theElements[vectorSize-1]);
#   else
    return iterator(size()-1,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the \b last
   * valid element of the genericVector. See inverseBegin() for more details.
   */
  template<class T>
  inline typename genericVector<T>::const_iterator
  genericVector<T>::inverseBegin() const {
#   ifdef NDEBUG
    return const_iterator(&theElements[vectorSize-1]);
#   else
    return const_iterator(size()-1,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericVector. It is used to
   * mark the end for inverse order iteration through the genericVector
   * using normal iterators (as opposed to reverse iterators). This
   * has the advantage that iterators going from front to end and in
   * the inverse direction are the same and can thus be compared,
   * copied etc.
   */
  template<class T>
  inline typename genericVector<T>::iterator genericVector<T>::inverseEnd() {
#   ifdef NDEBUG
    return iterator(&theElements[-1]);
#   else
    return iterator(-1,this);
#   endif
  };

  /**
   * This method returns an iterator that points to the element \b
   * before the \b first valid element of the genericVector.
   */
  template<class T>
  inline typename genericVector<T>::const_iterator 
  genericVector<T>::inverseEnd() const {
#   ifdef NDEBUG
    return const_iterator(&theElements[-1]);
#   else
    return const_iterator(-1,this);
#   endif
  };

  template<class T>
  inline T& genericVector<T>::at(const int x) {
    assert(static_cast<unsigned int>(x)<
           static_cast<unsigned int>(vectorSize));
    assert(theElements != 0);
    return ( theElements[x] );
  }

  template<class T>
  inline const T& genericVector<T>::at(const int x) const {
    assert(static_cast<unsigned int>(x)<
           static_cast<unsigned int>(vectorSize));
    assert(theElements != 0);
    return ( theElements[x] );
  }

  // access operator (alias for at(const int& x)).
  template<class T>
  inline T& genericVector<T>::operator[](const int x) {
    return at(x);
  };

  // const access operator (alias for at(const int& x) const).
  template<class T>
  inline const T& genericVector<T>::operator[](const int x) const {
    return at(x);
  };

  /* compare this genericVector with other
     @param other the other genericVector to be compared with
     @return true if both genericVectors have the same elements and same size
  */
  template<class T>
  inline bool 
  genericVector<T>::operator==(const genericVector<T>& other) const {
    return equals(other);
  }

  /* compare this genericVector with other
     @param other the other genericVector to be compared with
     @return true if both genericVectors are different
  */
  template<class T>
  inline bool 
  genericVector<T>::operator!=(const genericVector<T>& other) const {
    return !equals(other);
  }
} // namespace lti

#endif

