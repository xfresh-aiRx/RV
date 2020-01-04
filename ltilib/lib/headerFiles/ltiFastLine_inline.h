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
 * revisions ..: $Id: ltiFastLine_inline.h,v 1.3 2004/05/03 12:20:01 alvarado Exp $
 */

namespace lti {

  // -------------------------------------------------------------------------
  // implementation
  // -------------------------------------------------------------------------

  template<class T>
  inline void tfastLine<T>::updateRequest() {
    uptodate=false;
  };
  
  template<class T>
  inline void tfastLine<T>::ensureCorrectSlope() const {
    if (!uptodate) {
      delta=this->end-this->start;
      if (invSlope=(abs(delta.y)>abs(delta.x))) {
        normSlope=static_cast<float>(delta.x)/static_cast<float>(delta.y);
      } else {
        normSlope=static_cast<float>(delta.y)/static_cast<float>(delta.x);
      }
      uptodate=true;
    }
  };

  // ----------------------------------------------------------
  // Line attributes
  // ----------------------------------------------------------

  template<class T>
  inline void tfastLine<T>::updateSlope() {
    ensureCorrectSlope();
  }

  template<class T>
  inline const tpoint<T>& tfastLine<T>::getDelta() const {
    ensureCorrectSlope();
    return delta;
  }

  template<class T>
  inline bool tfastLine<T>::getNormalizedSlope(float& slope) const {
    ensureCorrectSlope();
    slope=normSlope;
    return !invSlope;
  };

  template<class T>
  inline void tfastLine<T>::getLineAttributes(tpoint<T>& deltaLine,
                                              float& nslope,
                                              bool&  normalSlope) const {
    ensureCorrectSlope();
    deltaLine=delta;
    nslope=normSlope;
    normalSlope=!invSlope;
  };

  template<class T>
  inline void tfastLine<T>::set(const tpoint<T>& theStart,
                                const tpoint<T>& theEnd) {
    tline<T>::set(theStart,theEnd);
    updateRequest();
  }

  template<class T>
  inline void tfastLine<T>::setStart(const tpoint<T>& theStart) {
    tline<T>::setStart(theStart);
    updateRequest();
  }

  template<class T>
  inline void tfastLine<T>::setEnd(const tpoint<T>& theEnd) {
    tline<T>::setEnd(theEnd);
    updateRequest();
  }

  template <class T>
  inline void tfastLine<T>::invert() {
    tline<T>::invert();
    updateRequest();
  }

  // --------------------------------
  // distance computation
  // --------------------------------

  template <class T>
  inline T tfastLine<T>::distanceToXPol(const tpoint<T>& c) const {
    return sqrt(distanceSqrXPol(c));
  };

  template <class T>
  inline T tfastLine<T>::distanceToXPol(const tpoint<T>& c,
                                              tpoint<T>& p) const {
    return sqrt(distanceSqrXPol(c,p));
  };

  // ---------------------------------------------
  // intersections
  // ---------------------------------------------

  template <class T>
  inline bool tfastLine<T>::intersect(const tfastLine<T>& other,
                                      const trectangle<T>& rect) {
    copy(other);
    return intersect(rect);
  }

  template <class T>
  inline bool tfastLine<T>::intersectXPol(const tfastLine<T>& other,
                                          const trectangle<T>& rect) {
    copy(other);
    return intersectXPol(rect);
  }

  // ----------------------------------
  // implementation of shift operations
  // ---------------------------------- 
  
  template <class T>
  inline tfastLine<T>& tfastLine<T>::translate(const tpoint<T>& p) {
    tline<T>::translate(p);
    updateRequest();
    return (*this);
  }

  template <class T>
  inline tfastLine<T>& tfastLine<T>::translate(const tline<T>& other,
                                               const tpoint<T>& p) {
    tline<T>::translate(other,p);
    updateRequest();
    return (*this);
  }

  template <class T>
  inline tfastLine<T>& tfastLine<T>::copy(const tline<T>& other) {
    tline<T>::copy(other);
    updateRequest();
    return (*this);
  } 
    
  template <class T>
  inline tfastLine<T>& tfastLine<T>::copy(const tfastLine<T>& other) {
    tline<T>::copy(other);
    delta = other.delta;
    invSlope=other.invSlope;
    normSlope=other.normSlope;
    uptodate=other.uptodate;
    return (*this);
  } 

  template <class T>
  inline tfastLine<T>& tfastLine<T>::operator=(const tfastLine<T>& other) {
    return copy(other);
  }

  template <class T>
  inline tfastLine<T>& tfastLine<T>::operator=(const tline<T>& other) {
    return copy(other);
  }

} // namespace lti
