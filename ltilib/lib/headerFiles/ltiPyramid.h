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
 * file .......: ltiPyramid.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.00
 * revisions ..: $Id: ltiPyramid.h,v 1.7 2003/11/19 15:31:50 alvarado Exp $
 */

#ifndef _LTI_PYRAMID_H_
#define _LTI_PYRAMID_H_

#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {
  /**
   * Pyramid class.
   *
   * The lti::pyramid class is the parent class for the multiresolutional
   * pyramids.  For example lti::gaussianPyramid
   *
   * It contains some objects of type T (e.g. channel, channel8 or image),
   * which must implement the type T::size_type and the member size().  Note
   * that pyramids of pyramids are allowed.
   *
   */
  template <class T>
  class pyramid : public mathObject {
  public:

    /**
     * The iterator is equivalent to a lti::fvector::iterator
     */
    typedef typename std::vector<T>::iterator iterator;

    /**
     * The const_iterator is equivalent to a lti::fvector::const_iterator
     */
    typedef typename std::vector<T>::const_iterator const_iterator;

    /**
     * Default constructor creates an empty pyramid;
     */
    pyramid();

    /**
     * Create a pyramid with the given number of resolutions or scales
     *
     * @param resolutions the number of resolutions or scales in the pyramid
     */
    pyramid(const int& resolutions);

    /**
     * Create this pyramid as a copy of another pyramid
     * @param other the pyramid to be copied.
     */
    pyramid(const pyramid& other);

    /**
     * Destructor
     */
    virtual ~pyramid();

    /**
     * Returns the name of this class: "pyramid"
     */
    const char* getTypeName() const {return "pyramid";};

    /**
     * The return type of the size() member
     */
    typedef int size_type;

    /**
     * Returns the number of resolutions of this pyramid
     */
    inline int size() const;

    /**
     * Returns true if the size of this pyramid is zero.
     */
    inline bool empty() const;

    /**
     * Returns an iterator pointing to the first element.
     * Note that you can not change the values of the pyramid
     * elements when you use a const_iterator. See also begin()
     */
    inline const_iterator begin() const;

    /**
     * Returns an iterator pointing to the first element.
     * The use of the interators is similar to the iterators of the
     * Standard Template Library (STL).
     */
    inline iterator begin();

    /**
     * Returns last index as a const iterator.
     * For an example see begin()
     */
    inline const_iterator end() const;

    /**
     * Returns last index as an iterator
     * For an example see begin()
     */
    inline iterator end();

    /**
     * Change the number of resolutions of the pyramid
     *
     * @param resolutions the new number of resolutions of the pyramid
     * @param copyData if true (default), the old data will be keeped.
     *                 If false, all data will be lost.
     */
    virtual void resize(const int& resolutions,
			const bool& copyData=true);

    /**
     * Append one element to the pyramid
     *
     * @param theElement Element that will be appended to the pyramid
     */
    void append(const T& theElement);

    /**
     * Equivalent to resize(0);
     */
    void clear();

    /**
     * Read-only access to the element x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline const T& at(const int& x) const;

    /**
     * Access element x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline T& at(const int& x);

    /**
     * Read-only access to the element x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline const T& operator[](const int& x) const;

    /**
     * Access element x of the pyramid
     * @param x index of the pyramid element to be accessed.  It should
     *          be between 0 and size()-1
     * @return the number of entries in the given cell
     */
    inline T& operator[](const int& x);


    /**
     * Assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source pyramid to be copied.
     * @return a reference to this object
     */
    pyramid<T>& copy(const pyramid<T>& other);

    /**
     * Create a clone of this pyramid
     * @return a pointer to a copy of this pyramid
     */
    virtual mathObject* clone() const;

    /**
     * Compare this pyramid with another one.
     *
     * @param other the other pyramid to be compared with
     * @return true if both pyramids have the same elements and same size
     */
    bool equals(const pyramid& other) const;

    /**
     * Compare if the resolutions of this and the other pyramid are the same.
     * @param other the pyramid to be compared with.
     * @return true if both pyramids and its elements have the same size
     */
    bool compareResolutions(const pyramid& other) const;

    /**
     * Compare this pyramid with another one
     *
     * @param other the other pyramid to be compared with
     * @return true if both pyramids have the same elements and same size
     */
    inline bool operator==(const pyramid& other) const {
      return equals(other);
    };

    /**
     * Assigment operator (alias for copy(other)).
     * @param other the pyramid to be copied
     * @return a reference to the actual pyramid
     */
    inline pyramid& operator=(const pyramid& other) {return copy(other);};

  protected:
    /**
     * The data of the pyramid
     */
    std::vector<T> thePyramid;

    int resolutions;
  };

} // namespace lti

#include "ltiPyramid_inline.h"
#include "ltiPyramid_template.h"

#endif

