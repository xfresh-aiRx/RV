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
 * file .......: ltiSortExpensive.h
 * authors ....: Joerg Zieren
 * organization: LTI, RWTH Aachen
 * creation ...: 24.06.2002
 * revisions ..: $Id: ltiSortExpensive.h,v 1.4 2003/02/17 07:16:46 alvarado Exp $
 */

#ifndef _LTI_SORT_EXPENSIVE_H_
#define _LTI_SORT_EXPENSIVE_H_

#include <ltiObject.h>
#include <list>

namespace lti {
  /**
   * Sorts elements in a list that are computationally expensive to compare.
   * You need to provide a pointer to a class member function that performs the
   * expensive computation (e.g. area computation). This class sorts the list
   * while performing the expensive computation exactly once for each element.
   */
  template <class T, class TValue>
  class sortExpensive : public lti::object {
  protected:
    /** Helper class for sorting. */
    class sortHelper {
    public:
      typename std::list<T>::iterator theIterator;
      TValue value;
      sortHelper(const typename std::list<T>::iterator& it,
                 const TValue& val) : theIterator(it),value(val) {};
      bool operator<(const sortHelper& other) {
        return value < other.value;
      };
    };

  public:
    /** Constructor */
    sortExpensive();
    /** Copy constructor (same as sortExpensive() - no memory.) */
    sortExpensive(const sortExpensive& other);

    /**
     * Destructor
     */
    virtual ~sortExpensive();

    /**
     * Sort the elements in the given list. Requires a function
     * pointer that computes a double value for an object of type
     * T. The function is assumed to be a class member function,
     * therefore a pointer to an instance of that class
     * (<code>callbackPtr</code>) is required (see
     * <code>www.function-pointer.org</code>). See ltiObjectsFromMask
     * for a usage example.
     */
    void apply(std::list<T>& aList, void* callbackPtr,
               TValue (*computeValue)(void* callbackPtr, const T&));

    /** returns the name of this type */
    virtual const char* getTypeName() const;
  };
}

#include "ltiSortExpensive_template.h"

#endif

