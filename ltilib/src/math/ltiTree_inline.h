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
 * file .......: ltiTree_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 23.11.00
 * revisions ..: $Id: ltiTree_inline.h,v 1.8 2003/06/06 09:09:20 goenner Exp $
 */

#ifndef _LTI_TREE_INLINE_H_
#define _LTI_TREE_INLINE_H_

namespace lti {

  // returns the number of elements of the tree
  template<class T>
    inline int tree<T>::size() const {
    return theNodeManager.size();
  };

  // returns the number of elements of the tree
  template<class T>
    inline int tree<T>::height() const {
    if (isNull(theRoot)) {
      return 0;
    } else {
      return theRoot->height();
    }
  };

  /* returns first element as a const_iterator.
     Note that you can not change the values of the tree
     elements when you use a const_iterator. See also begin() */
  template<class T>
  inline typename tree<T>::const_iterator tree<T>::begin() const {
    return const_iterator(*theRoot,theNodeManager);
  };

  /* returns first element as an iterator */
  template<class T>
  inline typename tree<T>::iterator tree<T>::begin() {
    return iterator(*theRoot,theNodeManager);
  };

  /* returns last index as a const iterator.
      For an example see begin() */
  template<class T>
  inline typename tree<T>::const_iterator tree<T>::end() const {
    return const_iterator(*((node*)(0)),theNodeManager);
  };

  /* returns last index as an iterator
      For an example see begin() */
  template<class T>
  inline typename tree<T>::iterator tree<T>::end() {
    return iterator(*((node*)(0)),theNodeManager);
  };

  /* returns iterator to the given node */
  template<class T>
  inline typename tree<T>::iterator tree<T>::getIteratorTo(node& aNode) {
    return iterator(aNode,theNodeManager);
  };

  /* returns iterator to the given node */
  template<class T>
  inline typename tree<T>::const_iterator
  tree<T>::getIteratorTo(const node& aNode) const {
    return const_iterator(aNode,theNodeManager);
  };

  /* compare this tree with other
     @param other the other tree to be compared with
     @return true if both trees have the same elements and same size
  */
  template<class T>
    inline bool tree<T>::operator==(const tree<T>& other) const {
    return equals(other);
  }

} // namespace lti

#endif

 
