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
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiGenericInterpolator_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiGenericInterpolator_inline.h,v 1.3 2004/05/03 12:18:12 alvarado Exp $
 */

namespace lti {

  //
  // Private class used to get the smallest floating point accumulation type
  // that can be safetly used.
  //
  template<class T>
  struct giMinimalFPType {
    typedef float fp_type;
  };

  template<>
  struct giMinimalFPType<double> {
    typedef double fp_type;
  };
  
  
  // --------------------------------------------
  // implementation of some inline helper methods
  // --------------------------------------------
  
  template<class T>
  inline T genericInterpolator<T>::compute(const float fx,
                                           const T* y) const {

    typedef typename giMinimalFPType<T>::fp_type fp_type;

    int i=static_cast<int>(fx*intervalSize)+lastIntervalFirstSample;
    fp_type acc = static_cast<fp_type>((*y++)*interpolationKernel.at(i));
    i-=intervalSize;
    while (i>0) {
      acc += static_cast<fp_type>((*y++)*interpolationKernel.at(i));
      i-=intervalSize;
    }

    return static_cast<T>(acc);
  }

  template<>
  inline rgbPixel 
  genericInterpolator<rgbPixel>::compute(const float fx,
                                         const rgbPixel* y) const {

    int i=static_cast<int>(fx*intervalSize)+lastIntervalFirstSample;
    trgbPixel<float> tmp;
    trgbPixel<float> acc(*y++);
    acc*=interpolationKernel.at(i);
    i-=intervalSize;
    while (i>0) {
      tmp.copy(*y++);
      tmp*=interpolationKernel.at(i);
      acc.add(tmp);
      i-=intervalSize;
    }

    return acc.getClippedRGBPixel();
  }

  template<class T>
  inline T genericInterpolator<T>::compute(const float fy,
                                           const float fx,
                                           const T** data) const {
    int i;
    for (i=0;i<numberOfIntervals;++i) {
      firstElemColumn[i]=compute(fx,data[i]);
    }
    return compute(fy,firstElemColumn);
  }


  template<class T>
  inline T genericInterpolator<T>::interpolateUnchk(const matrix<T>& src,
                                                    const float row,
                                                    const float col) {
    // precomputations for x
    int truncX = static_cast<int>(col);
    float fractX = col - static_cast<float>(truncX);
    truncX-=firstSampleOffset;

    // precomputation for y
    int truncY = static_cast<int>(row);
    float fractY = row - static_cast<float>(truncY);
    truncY-=firstSampleOffset;
    int i;

    for (i=0;i<numberOfIntervals;++i) {
      rows[i]=&src.at(truncY+i,truncX);
    }
    return compute(fractY,fractX,rows);
  }

  template<class T>
  inline T genericInterpolator<T>::interpolateUnchk(const float row,
                                                    const float col) {
    return interpolateUnchk(*this->theMatrix,row,col);
  }

  template<class T>
  inline const vector<float>& genericInterpolator<T>::getLUT() const {
    return interpolationKernel;
  }
}

