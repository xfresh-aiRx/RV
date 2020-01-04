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
 * file .......: ltiLine_template.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 07.02.2003
 * revisions ..: $Id: ltiLine_inline.h,v 1.2 2003/05/26 14:22:19 goenner Exp $
 */

namespace lti {

  // -------------------------------------------------------------------------
  // implementation
  // -------------------------------------------------------------------------

  template<class T>
  inline void tline<T>::set(const tpoint<T>& theStart,const tpoint<T>& theEnd){
    start=theStart;
    end=theEnd;
  }

  template<class T>
  inline void tline<T>::setStart(const tpoint<T>& theStart) {
    start = theStart;
  }

  template<class T>
  inline void tline<T>::setEnd(const tpoint<T>& theEnd) {
    end = theEnd;
  }

  template <class T>
  inline void tline<T>::invert() {
    tpoint<T> tmp(start);
    start=end;
    end=tmp;
  }

  template <class T>
  inline const tpoint<T>& tline<T>::getStart() const {
    return start;
  }

  template <class T>
  inline const tpoint<T>& tline<T>::getEnd() const {
    return end;
  }

  // --------------------------------
  // distance computation
  // --------------------------------

  template <class T>
  inline T tline<T>::distanceTo(const tpoint<T>& c) const {
    return sqrt(distanceSqr(c));
  };

  template <class T>
  inline T tline<T>::distanceSqr(const tpoint<T>& c) const {
    tpoint<T> dummy;
    return distanceSqr(c,dummy);
  }

  template <class T>
  inline T tline<T>::distanceToXPol(const tpoint<T>& c) const {
    return sqrt(distanceSqrXPol(c));
  };

  template <class T>
  inline T tline<T>::distanceSqrXPol(const tpoint<T>& c) const {
    tpoint<T> dummy;
    return distanceSqrXPol(c,dummy);
  }

  template <class T>
  inline T tline<T>::sqrLength() const {
    return start.distanceSqr(end);
  }

  // ---------------------------------------------
  // intersections
  // ---------------------------------------------

  template <class T>
  inline bool tline<T>::intersect(const tline<T>& other,
                                  const trectangle<T>& rect) {
    copy(other);
    return intersect(rect);
  }

  template <class T>
  inline bool tline<T>::getCommonLine(const tline<T>& first, const tline<T>& second) {
    copy (first);
    return getCommonLine(second);
  }

  template <class T>
  inline bool tline<T>::intersectXPol(const tline<T>& other,
                                      const trectangle<T>& rect) {
    copy(other);
    return intersectXPol(rect);
  }

  // ----------------------------------
  // implementation of shift operations
  // ---------------------------------- 
  
  template <class T>
  inline tline<T>& tline<T>::translate(const tpoint<T>& p) {
    start+=p;
    end+=p;
    return (*this);
  }

  template <class T>
  inline tline<T>& tline<T>::translate(const tline<T>& other,
                                       const tpoint<T>& p) {
    start=other.start+p;
    end  =other.end+p;
    return (*this);
  }

  template <class T>
  inline tline<T>& tline<T>::copy(const tline<T>& other) {
    start = other.start;
    end = other.end;
    return (*this);
  } 
    
  template <class T>
  inline bool tline<T>::operator==(const tline<T>& other) const {
    return ((end == other.end) && (start == other.start));
  }

  template <class T>
  inline bool tline<T>::operator!=(const tline<T>& other) const {
    return ((end != other.end) || (start != other.start));
  }

  template <class T>
  inline tline<T>& tline<T>::getOrthogonal(const tline<T>& other, double offset) {
    copy (other);
    return getOrthogonal(offset);
  }

} // namespace lti
