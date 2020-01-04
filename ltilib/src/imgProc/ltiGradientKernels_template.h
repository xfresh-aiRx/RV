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
 * file .......: ltiGradientKernels_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 11.07.00
 * revisions ..: $Id: ltiGradientKernels_template.h,v 1.12 2004/05/03 12:18:13 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {
  // ANDO

  // constructor
  template<class T>
    gradientKernelX<T>::gradientKernelX(const int& size)
    : sepKernel<T>() {
    generate(size);
  }

  // generate filter
  template<class T>
    void  gradientKernelX<T>::generate(const int& size) {

    this->setNumberOfPairs(1);

    if ((size>=3) && (size<=5)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).resize(l,h,T(),false,false);
      this->getColFilter(0).resize(l,h,T(),false,false);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k3row[3] = {1.000000,0.000000,-1.000000};
      static const double k3col[3] = { 0.112737,0.274526,0.112737};

      static const double k4row[4] =
        {+0.870541 , +1.000000,  -1.000000, -0.870541};
      static const double k4col[4] =
        { 0.0254738,  0.112996,  0.112996, 0.0254738};

      static const double k5row[5] =
        {+0.379022,+1.000000, 0.000000, -1.000000, -0.379022};
      static const double k5col[5] =
        { 0.0101693, 0.0708223, 0.122602, 0.0708223, 0.0101693};

      switch(size) {
        case 3:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-1) = static_cast<T>(k3row[i])*rowNorm;
            this->getColFilter(0).at(i-1) = static_cast<T>(k3col[i])*colNorm;
          }
          break;
        case 4:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k4row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k4col[i])*colNorm;
          }
          break;
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5col[i])*colNorm;
          }
          break;
      }
    }
  }

  // constructor
  template<class T>
    gradientKernelY<T>::gradientKernelY(const int& size)
    : sepKernel<T>() {
    generate(size);
  }

  // generate filter
  template<class T>
    void  gradientKernelY<T>::generate(const int& size) {

    this->setNumberOfPairs(1);

    if ((size>=3) && (size<=5)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).resize(l,h,T(),false,false);
      this->getColFilter(0).resize(l,h,T(),false,false);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k3col[3] = {+1.000000,0.000000,-1.000000};
      static const double k3row[3] = { 0.112737,0.274526,0.112737};

      static const double k4col[4] =
        {+0.870541 , +1.000000,  -1.000000, -0.870541};
      static const double k4row[4] =
        { 0.0254738,  0.112996,  0.112996, 0.0254738};

      static const double k5col[5] =
        {+0.379022,+1.000000, 0.000000, -1.000000, -0.379022};
      static const double k5row[5] =
        { 0.0101693, 0.0708223, 0.122602, 0.0708223, 0.0101693};

      switch(size) {
        case 3:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-1) = static_cast<T>(k3row[i])*rowNorm;
            this->getColFilter(0).at(i-1) = static_cast<T>(k3col[i])*colNorm;
          }
          break;
        case 4:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k4row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k4col[i])*colNorm;
          }
          break;
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5col[i])*colNorm;
          }
          break;
      }
    }
  }

  // -----------------------------
  // SOBEL
  
  template<class T>
  sobelKernelX<T>::sobelKernelX(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -1 0 1
    this->getRowFilter(0).resize(-1,1,static_cast<T>(0),false,true);
    this->getRowFilter(0).at(-1) = -1;
    this->getRowFilter(0).at( 1) =  1;

    // col filter 1 2 1
    this->getColFilter(0).resize(-1,1,static_cast<T>(1),false,true);
    this->getColFilter(0).at(0)=2;

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(2);
        this->getColFilter(0).setNorm(2);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(2));
        this->getColFilter(0).divide(static_cast<T>(2));
      }
    }

  }
  
  template<class T>
  sobelKernelY<T>::sobelKernelY(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter -1 0 1
    this->getColFilter(0).resize(-1,1,static_cast<T>(0),false,true);
    this->getColFilter(0).at(-1) = -1;
    this->getColFilter(0).at( 1) =  1;

    // row filter 1 2 1
    this->getRowFilter(0).resize(-1,1,static_cast<T>(1),false,true);
    this->getRowFilter(0).at(0)=2;

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(2);
        this->getColFilter(0).setNorm(2);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(2));
        this->getColFilter(0).divide(static_cast<T>(2));
      }
    }

  }

  // -----------------------------
  // PREWITT
  
  template<class T>
  prewittKernelX<T>::prewittKernelX(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -1 0 1
    this->getRowFilter(0).resize(-1,1,static_cast<T>(0),false,true);
    this->getRowFilter(0).at(-1) = -1;
    this->getRowFilter(0).at( 1) =  1;

    // col filter 1 1 1
    this->getColFilter(0).resize(-1,1,static_cast<T>(1),false,true);

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getColFilter(0).setNorm(3);
      } else {
        this->getColFilter(0).divide(static_cast<T>(3));
      }
    }

  }
  
  template<class T>
  prewittKernelY<T>::prewittKernelY(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter -1 0 1
    this->getColFilter(0).resize(-1,1,static_cast<T>(0),false,true);
    this->getColFilter(0).at(-1) = -1;
    this->getColFilter(0).at( 1) =  1;

    // row filter 1 1 1
    this->getRowFilter(0).resize(-1,1,static_cast<T>(1),false,true);

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(3);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(3));
      }
    }

  }

  // -----------------------------
  // HARRIS
  
  template<class T>
  harrisKernelX<T>::harrisKernelX() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -2 -1 0 1 2
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,true);
    this->getRowFilter(0).at(-2) = static_cast<T>(-2);
    this->getRowFilter(0).at(-1) = static_cast<T>(-1);
    this->getRowFilter(0).at( 1) = static_cast<T>(1);
    this->getRowFilter(0).at( 2) = static_cast<T>(2);

    // col filter empty
    this->getColFilter(0).resize(0,0,static_cast<T>(1));

  }
  
  template<class T>
  harrisKernelY<T>::harrisKernelY() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter -2 -1 0 1 2
    this->getColFilter(0).resize(-2,2,static_cast<T>(0),false,true);
    this->getColFilter(0).at(-2) = static_cast<T>(-2);
    this->getColFilter(0).at(-1) = static_cast<T>(-1);
    this->getColFilter(0).at( 1) = static_cast<T>(1);
    this->getColFilter(0).at( 2) = static_cast<T>(2);

    // row filter empty
    this->getRowFilter(0).resize(0,0,static_cast<T>(1));

  }

  // ---------------------------------------
  // ROBINSON
  
  template<class T>
  robinsonKernelX<T>::robinsonKernelX() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -1;
    this->at(-1, 0) =  1;
    this->at(-1, 1) =  1;
    this->at( 0,-1) = -1;
    this->at( 0, 0) = -2;
    this->at( 0, 1) =  1;
    this->at( 1,-1) = -1;
    this->at( 1, 0) =  1;
    this->at( 1, 1) =  1;
  }
  
  template<class T>
  robinsonKernelY<T>::robinsonKernelY() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -1;
    this->at(-1, 0) = -1;
    this->at(-1, 1) = -1;
    this->at( 0,-1) =  1;
    this->at( 0, 0) = -2;
    this->at( 0, 1) =  1;
    this->at( 1,-1) =  1;
    this->at( 1, 0) =  1;
    this->at( 1, 1) =  1;
  }

  // ---------------------------------------
  // KIRSCH
  
  template<class T>
  kirschKernelX<T>::kirschKernelX() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -5;
    this->at(-1, 0) =  3;
    this->at(-1, 1) =  3;
    this->at( 0,-1) = -5;
    this->at( 0, 0) =  0;
    this->at( 0, 1) =  3;
    this->at( 1,-1) = -5;
    this->at( 1, 0) =  3;
    this->at( 1, 1) =  3;
  }
  
  template<class T>
  kirschKernelY<T>::kirschKernelY() : kernel2D<T>(-1,-1,1,1) {
    this->at(-1,-1) = -5;
    this->at(-1, 0) = -5;
    this->at(-1, 1) = -5;
    this->at( 0,-1) =  3;
    this->at( 0, 0) =  0;
    this->at( 0, 1) =  3;
    this->at( 1,-1) =  3;
    this->at( 1, 0) =  3;
    this->at( 1, 1) =  3;
  }

}

