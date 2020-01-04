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
 * file .......: ltiSortExpensive_template.h
 * authors ....: Joerg Zieren
 * organization: LTI, RWTH Aachen
 * creation ...: 24.06.2002
 * revisions ..: $Id: ltiSortExpensive_template.h,v 1.3 2003/02/17 07:16:46 alvarado Exp $
 */

namespace lti {
  template <class T, class TValue>
    sortExpensive<T,TValue>::sortExpensive() {
    }

  template <class T, class TValue>
    sortExpensive<T,TValue>::sortExpensive(const sortExpensive& other) {
    }

  template <class T, class TValue>
    sortExpensive<T,TValue>::~sortExpensive() {
    }

  template <class T, class TValue>
    void sortExpensive<T,TValue>::apply(std::list<T>& aList, void* callbackPtr,
      TValue (*computeValue)(void* callbackPtr, const T&)) {
      std::list<sortHelper> helperList;
      for (typename std::list<T>::iterator it = aList.begin();
           it != aList.end(); it++) {
        helperList.push_back(sortHelper(it,computeValue(callbackPtr,*it)));
      }
      helperList.sort();
      for (typename std::list<sortHelper>::iterator helpIt=helperList.begin();
           helpIt != helperList.end(); ++helpIt) {
        aList.splice(aList.end(),aList,(*helpIt).theIterator);
      }
    }

  // returns the name of this type
  template <class T, class TValue>
    const char* sortExpensive<T,TValue>::getTypeName() const {
      return "sortExpensive";
    }

}

