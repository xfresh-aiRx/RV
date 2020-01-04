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
 * file .......: ltiTensor_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiTensor_inline.h,v 1.4 2003/05/16 08:47:56 alvarado Exp $
 */

namespace lti {

  /*
   * returns the number of dimensions of this tensor
   */
  template<class T>
  int tensor<T>::order() const {
    return theOrder;
  };

  /*
   * get the number of cells of the dimension <code>dim</code>
   * @param dimension the index of the dimension to be checked
   * @return the number of cells of the dimension specified by
   *         <code>dim</code>
   */
  template<class T>
  const int& tensor<T>::elementsInDimension(const int& dimension) const {
    return theElementsPerDimension.at(dimension);
  }

  template<class T>
  const ivector& tensor<T>::elementsPerDimension() const {
    return theElementsPerDimension;
  }

  /*
   * returns a vector to the first element of the tensor
   * (usually every element of the vector is 0;
   */
  template<class T>
  const ivector& tensor<T>::getFirstIndex() const {
    return firstIndex;
  }

  /*
   * returns a vector to the last element of the tensor
   */
  template<class T>
  const ivector& tensor<T>::getLastIndex() const {
    return lastIndex;
  }

} // namespace lti

