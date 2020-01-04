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
 * file .......: ltiPyramid_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiPyramid_template.h,v 1.4 2003/02/17 07:15:33 alvarado Exp $
 */

#include "ltiPyramid.h"


namespace lti {

  /*
   * default constructor creates an empty pyramid;
   */
  template <class T>
  pyramid<T>::pyramid()
    : mathObject(), resolutions(0) {
  }

  /*
   * create a pyramid of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the pyramid
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the pyramid.
   * @param cells the number of cells per dimension.
   */
  template <class T>
  pyramid<T>::pyramid(const int& theScales)
    : mathObject(), resolutions(theScales) {
    resize(theScales,false);
  }

  /*
   * create this pyramid as a copy of another pyramid
   * @param other the pyramid to be copied.
   */
  template <class T>
  pyramid<T>::pyramid(const pyramid<T>& other)
    : mathObject() {
    copy(other);
  }

  /*
   * destructor
   */
  template <class T>
  pyramid<T>::~pyramid() {
  }

  /*
   * change dimensionality and cell number of the pyramid.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the pyramid
   * @param cells      the number of cells per dimension
   *
   */
  template <class T>
  void pyramid<T>::resize(const int& theResolutions,
                          const bool& copyData) {
    if (!copyData) {
      thePyramid.resize(0);
    }

    resolutions = theResolutions;
    thePyramid.resize(resolutions);
  }

  template <class T>
  void pyramid<T>::clear() {
    resize(0);
  }

  /**
   * append one element to the pyramid
   *
   * @param theElement Element that will be appended to the pyramid
   */
  template <class T>
  void pyramid<T>::append(const T& theElement) {

    thePyramid.push_back(theElement);
    resolutions = size();
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source pyramid to be copied.
   * @return a reference to this object
   */
  template <class T>
  pyramid<T>& pyramid<T>::copy(const pyramid<T>& other) {

    thePyramid = other.thePyramid;
    resolutions = other.resolutions;

    return *this;
  }

  /*
   * create a clone of this pyramid
   * @return a pointer to a copy of this pyramid
   */
  template <class T>
  mathObject* pyramid<T>::clone() const {
    return new pyramid(*this);
  }

  /*
   * compare this pyramid with another one.
   *
   * @param other the other pyramid to be compared with
   * @return true if both pyramids have the same elements and same size
   */
  template <class T>
  bool pyramid<T>::equals(const pyramid& other) const {

    bool result = (size() == other.size());
    const_iterator it,it2;

    for (it=begin(),it2=other.begin();
         result && (it!=end());
         ++it,++it2) {
      result = result && ((*it)==(*it2));
    }

    return (result);
  }

  /*
   * compare this pyramid with another one.
   *
   * @param other the other pyramid to be compared with
   * @return true if both pyramids have the same elements and same size
   */
  template <class T>
  bool pyramid<T>::compareResolutions(const pyramid& other) const {

    bool result = (size() == other.size());
    const_iterator it,it2;

    for (it=begin(),it2=other.begin();
         result && (it!=end());
         ++it,++it2) {
      result = result && ((*it).size()==(*it2).size());
    }

    return (result);
  }

}


