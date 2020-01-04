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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiLogarithm_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 20.04.99
 * revisions ..: $Id: ltiLogarithm_template.h,v 1.7 2003/04/14 08:11:08 sltilib Exp $
 */

#include "ltiMath.h"

namespace lti {
  // logarithm

  // onPlace version of apply.
  template<class T>
  vector<T>& logarithm<T>::apply(vector<T>& theObject) const {

    theObject.apply(logT);

    return theObject;
  }

  // onCopy version of apply.
  template<class T>
  vector<T>& logarithm<T>::apply(const vector<T>& theObject,
                                       vector<T>& newObject) const {
    newObject.copy(theObject);
    return apply(newObject);
  }

  // onPlace version of apply.
  template<class T>
  matrix<T>& logarithm<T>::apply(matrix<T>& theObject) const {

    int y;

    for (y=0;y<theObject.rows();y++) {
      apply(theObject.getRow(y));
    }

    return theObject;
  }

  // onCopy version of apply.
  template<class T>
  matrix<T>& logarithm<T>::apply(const matrix<T>& theObject,
                                       matrix<T>& newObject) const {
    newObject.copy(theObject);
    return apply(newObject);
  }

}

