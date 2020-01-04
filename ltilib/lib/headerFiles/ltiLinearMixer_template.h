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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiLinearMixer.cpp
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 21.1.2002
 * revisions ..: $Id: ltiLinearMixer_template.h,v 1.8 2005/02/02 14:16:23 doerfler Exp $
 */

//TODO: include files
//#include "ltiLinearMixer.h"

namespace lti {

  // --------------------------------------------------
  // linearMixer
  // --------------------------------------------------

  // default constructor
  template <class T>
  linearMixer<T>::linearMixer()
    : functor(){

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.


  }

  // copy constructor
  template <class T>
  linearMixer<T>::linearMixer(const linearMixer<T>& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class T>
  linearMixer<T>::~linearMixer() {
  }

  // returns the name of this type
  template <class T>
  const char* linearMixer<T>::getTypeName() const {
    return "linearMixer";
  }

  // copy member
  template <class T>
  linearMixer<T>& linearMixer<T>::copy(const linearMixer<T>& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  template <class T>
  linearMixer<T>&  linearMixer<T>::operator=(const linearMixer<T>& other) {
    return (copy(other));
  }


  // clone member
  template <class T>
  functor* linearMixer<T>::clone() const {
    return new linearMixer(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  /* On-Copy Apply method for the mixing functor.
   * The functor takes the mixing matrix and the source matrices.
   * The it mixes the matrices.
   */
  template <class T>
  bool linearMixer<T>::apply(const matrix<T>& mixer,
                             const std::vector<const matrix<T>*>& src,
                             const std::vector<matrix<T>*>& dest) const {
    const int n=mixer.rows();
    const int m=mixer.columns();

    int i;
    for (i=0; i<n; i++) {
      dest[i]->resize(src[0]->size());
    }

    for (i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
        typename matrix<T>::const_iterator mi=src[j]->begin();
        typename matrix<T>::iterator di=dest[i]->begin();
        while (mi != src[j]->end()) {
          *di+=*mi*mixer.at(i,j);
          mi++;
          di++;
        }

      }
    }

    return true;
  }


  /**
   * On-Copy Apply method for the mixing functor.
   * The functor takes the mixing matrix and the source matrices.
   * The it mixes the matrices.
   */
  template <class T>
  bool linearMixer<T>::apply(const matrix<T>& mixer,
                             const std::vector<const vector<T>*>& src,
                             const std::vector<vector<T>*>& dest
#ifdef _LTI_MSC_6
       // see header file for the reason for this flag
       , bool msbug
#endif
       ) const {
    const int n=mixer.rows();
    const int m=mixer.columns();

    int i,j;
    for (i=0; i<n; i++) {
      dest[i]->resize(src[0]->size());
    }

    for (i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
        typename vector<T>::const_iterator mi=src[j]->begin();
        typename vector<T>::iterator di=dest[i]->begin();
        while (mi != src[j]->end()) {
          *di+=*mi*mixer.at(i,j);
          mi++;
          di++;
        }
      }
    }
    return true;
  }


}


