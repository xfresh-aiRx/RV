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
 * file .......: ltiTriangularKernels_template.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 16.05.00
 * revisions ..: $Id: ltiTriangularKernels_template.h,v 1.7 2004/05/03 12:19:02 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {
  // 1D filter kernel

  // constructor
  template<class T>
    triangularKernel1D<T>::triangularKernel1D(const int& size)
    : kernel1D<T>() {

    generate(size);
  }

  // generate filter
  template<class T>
  void  triangularKernel1D<T>::generate(const int& size) {

    int i;
    this->norm = typeInfo<T>::suggestedNorm();
    double snorm = double(this->norm);

    double accu;
    int range = static_cast<int>(size/2);

    resize(-range,size-range-1,T(0),false,false);

    accu=0;
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(snorm*(1.0f-
                                        static_cast<float>(abs(i))/range));
      accu+=this->at(i);
    }
    accu /= snorm;
    // make sure the sum of all elements is one!
    for (i=this->firstIdx();i<=this->lastIdx();i++) {
      this->at(i)=static_cast<T>(this->at(i)/accu);
    }

  }


}

