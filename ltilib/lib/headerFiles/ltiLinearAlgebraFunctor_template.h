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
 * file .......: ltiLinearAlgebraFunctor_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 22.06.99
 * revisions ..: $Id: ltiLinearAlgebraFunctor_template.h,v 1.5 2003/02/17 07:16:43 alvarado Exp $
 */

namespace lti {
  //--- scalarProduct ---
  //---------------------

  // onCopy version of apply.
  template<class T,class U>
  T& scalarProduct<T,U>::apply(const vector<T>& mult1,
                               const vector<U>& mult2,
                               T& result) const {
    int i;

    result = 0;
    const int minSize=min(mult1.size(),mult2.size());
    for(i=0;i<minSize;i++) {
      result += mult1[i]*mult2[i];
    }

    return result;
  }


}

