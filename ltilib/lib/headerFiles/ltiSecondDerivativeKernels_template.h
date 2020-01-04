/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiSecondDerivativeKernels_template.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 20.03.2004
 * revisions ..: $Id: ltiSecondDerivativeKernels_template.h,v 1.2 2004/05/03 12:18:39 alvarado Exp $
 */

#include "ltiMath.h"

namespace lti {
  // ANDO

  // constructor
  template<class T>
    andoKernelXX<T>::andoKernelXX(const int& size)
    : sepKernel<T>() {
    generate(size);
  }

  // generate filter
  template<class T>
    void  andoKernelXX<T>::generate(const int& size) {

    this->setNumberOfPairs(1);

    if ((size==5) || (size==7) || (size==9)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).resize(l,h,T(),false,false);
      this->getColFilter(0).resize(l,h,T(),false,false);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k5row[5]
        = {1.0, 0.0,-2.0, 0.0, 1.0};
      static const double k5col[5]
        = {0.012709631169, 0.061898475324, 0.100783787014,
           0.061898475324, 0.012709631169};
 
      static const double k7row[7]
        = {0.757841632681, 1.741082, -0.741082,
           -3.515683265362,
           -0.741082, 1.741082, 0.757841632681};
      static const double k7col[7]
        = {0.00064891448644, 0.0057568750096, 0.0185249710256, 
           0.02683402100488, 
           0.0185249710256, 0.0057568750096, 0.00064891448644};

      static const double k9row[9]
        = {0.143657676484, 0.758044, 1.0,
           -0.758044, -2.287315352968, -0.758044,
           1.0, 0.758044, 0.143657676484};
      static const double k9col[9]
        = {0.00010341466249, 0.00144042643078, 0.00750935121449,
           0.01880633767998, 0.02526967608356, 0.01880633767998,
           0.00750935121449, 0.00144042643078, 0.00010341466249};


      switch(size) {
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5col[i])*colNorm;
          }
          break;
        case 7:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-3) = static_cast<T>(k7row[i])*rowNorm;
            this->getColFilter(0).at(i-3) = static_cast<T>(k7col[i])*colNorm;
          }
          break;
        case 9:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-4) = static_cast<T>(k9row[i])*rowNorm;
            this->getColFilter(0).at(i-4) = static_cast<T>(k9col[i])*colNorm;
          }
          break;
      }
    }
  }

  // constructor
  template<class T>
    andoKernelXY<T>::andoKernelXY(const int& size)
    : sepKernel<T>() {
    generate(size);
  }

  // generate filter
  template<class T>
    void  andoKernelXY<T>::generate(const int& size) {

    this->setNumberOfPairs(1);

    if ((size==5) || (size==7) || (size==9)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).resize(l,h,T(),false,false);
      this->getColFilter(0).resize(l,h,T(),false,false);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k5XY[5]
        = {0.112737, 0.274526, 0.0, -0.274526, -0.112737};
 
      static const double k7XY[7]
        = {0.0221759873258, 0.123841450836, 0.185889850836,
           0.0,
           -0.185889850836, -0.123841450836, -0.0221759873258};

      static const double k9XY[9]
        = {0.0038543884246, 0.0370125097906, 0.117291155244,
           0.1392759097906, 0.0, -0.1392759097906,
           -0.117291155244, -0.0370125097906, -0.0038543884246};


      switch(size) {
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5XY[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5XY[i])*colNorm;
          }
          break;
        case 7:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-3) = static_cast<T>(k7XY[i])*rowNorm;
            this->getColFilter(0).at(i-3) = static_cast<T>(k7XY[i])*colNorm;
          }
          break;
        case 9:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-4) = static_cast<T>(k9XY[i])*rowNorm;
            this->getColFilter(0).at(i-4) = static_cast<T>(k9XY[i])*colNorm;
          }
          break;
      }
    }
  }

  // constructor
  template<class T>
    andoKernelYY<T>::andoKernelYY(const int& size)
    : sepKernel<T>() {
    generate(size);
  }

  // generate filter
  template<class T>
  void  andoKernelYY<T>::generate(const int& size) {

    this->setNumberOfPairs(1);

    if ((size==5) || (size==7) || (size==9)) {
      int i,l,h;
      l = -size/2;
      h = l+size-1;

      this->getRowFilter(0).resize(l,h,T(),false,false);
      this->getColFilter(0).resize(l,h,T(),false,false);

      const T rowNorm = this->getRowFilter(0).getNorm();
      const T colNorm = this->getColFilter(0).getNorm();

      static const double k5col[5]
        = {1.0, 0.0,-2.0, 0.0, 1.0};
      static const double k5row[5]
        = {0.012709631169, 0.061898475324, 0.100783787014,
           0.061898475324, 0.012709631169};
 
      static const double k7col[7]
        = {0.757841632681, 1.741082, -0.741082,
           -3.515683265362,
           -0.741082, 1.741082, 0.757841632681};
      static const double k7row[7]
        = {0.00064891448644, 0.0057568750096, 0.0185249710256, 
           0.02683402100488, 
           0.0185249710256, 0.0057568750096, 0.00064891448644};

      static const double k9col[9]
        = {0.143657676484, 0.758044, 1.0,
           -0.758044, -2.287315352968, -0.758044,
           1.0, 0.758044, 0.143657676484};
      static const double k9row[9]
        = {0.00010341466249, 0.00144042643078, 0.00750935121449,
           0.01880633767998, 0.02526967608356, 0.01880633767998,
           0.00750935121449, 0.00144042643078, 0.00010341466249};


      switch(size) {
        case 5:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-2) = static_cast<T>(k5row[i])*rowNorm;
            this->getColFilter(0).at(i-2) = static_cast<T>(k5col[i])*colNorm;
          }
          break;
        case 7:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-3) = static_cast<T>(k7row[i])*rowNorm;
            this->getColFilter(0).at(i-3) = static_cast<T>(k7col[i])*colNorm;
          }
          break;
        case 9:
          for (i=0;i<size;i++) {
            this->getRowFilter(0).at(i-4) = static_cast<T>(k9row[i])*rowNorm;
            this->getColFilter(0).at(i-4) = static_cast<T>(k9col[i])*colNorm;
          }
          break;
      }
    }
  }

  // -----------------------------
  // SOBEL
  
  template<class T>
  sobelKernelXX<T>::sobelKernelXX(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter 1 0 -2 0 1
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-2) =  static_cast<T>( 1);
    this->getRowFilter(0).at(-1) =  static_cast<T>( 0);
    this->getRowFilter(0).at( 0) =  static_cast<T>(-2);
    this->getRowFilter(0).at( 1) =  static_cast<T>( 0);
    this->getRowFilter(0).at( 2) =  static_cast<T>( 1);

    // col filter 1 4 6 4 1
    this->getColFilter(0).resize(-2,2,static_cast<T>(1),false,false);
    this->getColFilter(0).at(-2) = static_cast<T>( 1);
    this->getColFilter(0).at(-1) = static_cast<T>( 4);
    this->getColFilter(0).at( 0) = static_cast<T>( 6);
    this->getColFilter(0).at( 1) = static_cast<T>( 4);
    this->getColFilter(0).at(-2) = static_cast<T>( 1);

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getColFilter(0).setNorm(16);
      } else {
        this->getColFilter(0).divide(static_cast<T>(16));
      }
    }

  }

  template<class T>
  sobelKernelXY<T>::sobelKernelXY() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter 1 2 0 -2 -1
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-2) =  static_cast<T>( 1);
    this->getRowFilter(0).at(-1) =  static_cast<T>( 2);
    this->getRowFilter(0).at( 0) =  static_cast<T>( 0);
    this->getRowFilter(0).at( 1) =  static_cast<T>(-2);
    this->getRowFilter(0).at( 2) =  static_cast<T>(-1);

    // col filter 1 2 0 -2 -1
    this->getColFilter(0).copy(this->getRowFilter(0));

  }
  
  template<class T>
  sobelKernelYY<T>::sobelKernelYY(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter 1 0 -2 0 1
    this->getColFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getColFilter(0).at(-2) =  static_cast<T>( 1);
    this->getColFilter(0).at(-1) =  static_cast<T>( 0);
    this->getColFilter(0).at( 0) =  static_cast<T>(-2);
    this->getColFilter(0).at( 1) =  static_cast<T>( 0);
    this->getColFilter(0).at( 2) =  static_cast<T>( 1);

    // row filter 1 4 6 4 1
    this->getRowFilter(0).resize(-2,2,static_cast<T>(1),false,false);
    this->getRowFilter(0).at(-2) = static_cast<T>( 1);
    this->getRowFilter(0).at(-1) = static_cast<T>( 4);
    this->getRowFilter(0).at( 0) = static_cast<T>( 6);
    this->getRowFilter(0).at( 1) = static_cast<T>( 4);
    this->getRowFilter(0).at(-2) = static_cast<T>( 1);

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(16);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(16));
      }
    }

  }

  // -----------------------------
  // PREWITT
  
  template<class T>
  prewittKernelXX<T>::prewittKernelXX(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter 1 0 -2 0 1
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-2) =  static_cast<T>( 1);
    this->getRowFilter(0).at(-1) =  static_cast<T>( 0);
    this->getRowFilter(0).at( 0) =  static_cast<T>(-2);
    this->getRowFilter(0).at( 1) =  static_cast<T>( 0);
    this->getRowFilter(0).at( 2) =  static_cast<T>( 1);

    // col filter 1 2 3 2 1
    this->getColFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getColFilter(0).at(-2) =  static_cast<T>( 1);
    this->getColFilter(0).at(-1) =  static_cast<T>( 2);
    this->getColFilter(0).at( 0) =  static_cast<T>( 3);
    this->getColFilter(0).at( 1) =  static_cast<T>( 2);
    this->getColFilter(0).at( 2) =  static_cast<T>( 1);

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getColFilter(0).setNorm(9);
      } else {
        this->getColFilter(0).divide(static_cast<T>(9));
      }
    }

  }

  template<class T>
  prewittKernelXY<T>::prewittKernelXY() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter 1 1 0 -1 1
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-2) =  static_cast<T>( 1);
    this->getRowFilter(0).at(-1) =  static_cast<T>( 1);
    this->getRowFilter(0).at( 0) =  static_cast<T>( 0);
    this->getRowFilter(0).at( 1) =  static_cast<T>(-1);
    this->getRowFilter(0).at( 2) =  static_cast<T>(-1);

    // col filter 1 1 0 -1 -1
    this->getColFilter(0).copy(this->getRowFilter(0));

  }
  
  template<class T>
  prewittKernelYY<T>::prewittKernelYY(const bool normalized) : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter 1 0 -2 0 1
    this->getColFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getColFilter(0).at(-2) =  static_cast<T>( 1);
    this->getColFilter(0).at(-1) =  static_cast<T>( 0);
    this->getColFilter(0).at( 0) =  static_cast<T>(-2);
    this->getColFilter(0).at( 1) =  static_cast<T>( 0);
    this->getColFilter(0).at( 2) =  static_cast<T>( 1);

    // row filter 1 2 3 2 1
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-2) =  static_cast<T>( 1);
    this->getRowFilter(0).at(-1) =  static_cast<T>( 2);
    this->getRowFilter(0).at( 0) =  static_cast<T>( 3);
    this->getRowFilter(0).at( 1) =  static_cast<T>( 2);
    this->getRowFilter(0).at( 2) =  static_cast<T>( 1);

    if (normalized) {
      if (std::numeric_limits<T>::is_integer) {
        this->getRowFilter(0).setNorm(9);
      } else {
        this->getRowFilter(0).divide(static_cast<T>(9));
      }
    }

  }

  // -----------------------------
  // HARRIS
  
  template<class T>
  harrisKernelXX<T>::harrisKernelXX() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter 4 4 1 -4 -10 -4 1 4 4
    this->getRowFilter(0).resize(-4,4,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-4) = static_cast<T>(  4);
    this->getRowFilter(0).at(-3) = static_cast<T>(  4);
    this->getRowFilter(0).at(-2) = static_cast<T>(  1);
    this->getRowFilter(0).at(-1) = static_cast<T>( -4);
    this->getRowFilter(0).at( 0) = static_cast<T>(-10);
    this->getRowFilter(0).at( 1) = static_cast<T>( -4);
    this->getRowFilter(0).at( 2) = static_cast<T>(  1);
    this->getRowFilter(0).at( 3) = static_cast<T>(  4);
    this->getRowFilter(0).at( 4) = static_cast<T>(  4);

    // col filter empty
    this->getColFilter(0).resize(0,0,static_cast<T>(1));

  }

  template<class T>
  harrisKernelXY<T>::harrisKernelXY() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // row filter -2 -1 0 1 2
    this->getRowFilter(0).resize(-2,2,static_cast<T>(0),false,false);
    this->getRowFilter(0).at(-2) = static_cast<T>(-2);
    this->getRowFilter(0).at(-1) = static_cast<T>(-1);
    this->getRowFilter(0).at( 0) = static_cast<T>( 0);
    this->getRowFilter(0).at( 1) = static_cast<T>( 1);
    this->getRowFilter(0).at( 2) = static_cast<T>( 2);

    // col filter empty
    this->getColFilter(0).copy(this->getRowFilter(0));

  }

  template<class T>
  harrisKernelYY<T>::harrisKernelYY() : sepKernel<T>() {
    this->setNumberOfPairs(1);

    // col filter 4 4 1 -4 -10 -4 1 4 4
    this->getColFilter(0).resize(-4,4,static_cast<T>(0),false,false);
    this->getColFilter(0).at(-4) = static_cast<T>(  4);
    this->getColFilter(0).at(-3) = static_cast<T>(  4);
    this->getColFilter(0).at(-2) = static_cast<T>(  1);
    this->getColFilter(0).at(-1) = static_cast<T>( -4);
    this->getColFilter(0).at( 0) = static_cast<T>(-10);
    this->getColFilter(0).at( 1) = static_cast<T>( -4);
    this->getColFilter(0).at( 2) = static_cast<T>(  1);
    this->getColFilter(0).at( 3) = static_cast<T>(  4);
    this->getColFilter(0).at( 4) = static_cast<T>(  4);

    // col filter empty
    this->getRowFilter(0).resize(0,0,static_cast<T>(1));

  }
  

  // ---------------------------------------
  // ROBINSON
  
  template<class T>
  robinsonKernelXX<T>::robinsonKernelXX() : kernel2D<T>(-2,-2,2,2) {
    this->at(-2,-2) =  1;
    this->at(-2,-1) =  2;
    this->at(-2, 0) = -1;
    this->at(-2, 1) = -2;
    this->at(-2, 2) =  1;
    this->at(-1,-2) =  2;
    this->at(-1,-1) = -2;
    this->at(-1, 0) = -8;
    this->at(-1, 1) =  2;
    this->at(-1, 2) =  2;
    this->at( 0,-2) =  3;
    this->at( 0,-1) =  0;
    this->at( 0, 0) =  0;
    this->at( 0, 1) =  0;
    this->at( 0, 2) =  3;
    this->at( 1,-2) =  2;
    this->at( 1,-1) = -2;
    this->at( 1, 0) = -8;
    this->at( 1, 1) =  2;
    this->at( 1, 2) =  2;
    this->at( 2,-2) =  1;
    this->at( 2,-1) =  2;
    this->at( 2, 0) = -1;
    this->at( 2, 1) = -2;
    this->at( 2, 2) =  1;
  }

  template<class T>
  robinsonKernelXY<T>::robinsonKernelXY() : kernel2D<T>(-2,-2,2,2) {
    this->at(-2,-2) =  1;
    this->at(-2,-1) =  2;
    this->at(-2, 0) =  1;
    this->at(-2, 1) =  0;
    this->at(-2, 2) = -1;
    this->at(-1,-2) =  2;
    this->at(-1,-1) = -2;
    this->at(-1, 0) = -4;
    this->at(-1, 1) =  0;
    this->at(-1, 2) = -2;
    this->at( 0,-2) =  1;
    this->at( 0,-1) = -4;
    this->at( 0, 0) =  4;
    this->at( 0, 1) =  0;
    this->at( 0, 2) = -1;
    this->at( 1,-2) =  0;
    this->at( 1,-1) =  0;
    this->at( 1, 0) =  0;
    this->at( 1, 1) =  6;
    this->at( 1, 2) =  0;
    this->at( 2,-2) = -1;
    this->at( 2,-1) = -2;
    this->at( 2, 0) = -1;
    this->at( 2, 1) =  0;
    this->at( 2, 2) =  1;
  }

  template<class T>
  robinsonKernelYY<T>::robinsonKernelYY() : kernel2D<T>(-2,-2,2,2) {
    this->at(-2,-2) =  1;
    this->at(-2,-1) =  2;
    this->at(-2, 0) =  3;
    this->at(-2, 1) =  2;
    this->at(-2, 2) =  1;
    this->at(-1,-2) =  2;
    this->at(-1,-1) = -2;
    this->at(-1, 0) =  0;
    this->at(-1, 1) = -2;
    this->at(-1, 2) =  2;
    this->at( 0,-2) = -1;
    this->at( 0,-1) = -8;
    this->at( 0, 0) =  0;
    this->at( 0, 1) = -8;
    this->at( 0, 2) = -1;
    this->at( 1,-2) =  2;
    this->at( 1,-1) = -2;
    this->at( 1, 0) =  0;
    this->at( 1, 1) = -2;
    this->at( 1, 2) =  2;
    this->at( 2,-2) =  1;
    this->at( 2,-1) =  2;
    this->at( 2, 0) =  3;
    this->at( 2, 1) =  2;
    this->at( 2, 2) =  1;
  }

  // ---------------------------------------
  // KIRSCH
  
  template<class T>
  kirschKernelXX<T>::kirschKernelXX() : kernel2D<T>(-2,-2,2,2) {
    this->at(-2,-2) =   9;
    this->at(-2,-1) =  18;
    this->at(-2, 0) = -21;
    this->at(-2, 1) = -30;
    this->at(-2, 2) =  25;
    this->at(-1,-2) =  18;
    this->at(-1,-1) =  18;
    this->at(-1, 0) = -60;
    this->at(-1, 1) = -30;
    this->at(-1, 2) =  50;
    this->at( 0,-2) =  27;
    this->at( 0,-1) =  36;
    this->at( 0, 0) = -72;
    this->at( 0, 1) = -60;
    this->at( 0, 2) =  75;
    this->at( 1,-2) =  18;
    this->at( 1,-1) =  18;
    this->at( 1, 0) = -60;
    this->at( 1, 1) = -30;
    this->at( 1, 2) =  50;
    this->at( 2,-2) =   9;
    this->at( 2,-1) =  18;
    this->at( 2, 0) = -21;
    this->at( 2, 1) = -30;
    this->at( 2, 2) =  25;
  }

  template<class T>
  kirschKernelXY<T>::kirschKernelXY() : kernel2D<T>(-2,-2,2,2) {
    this->at(-2,-2) =   9;
    this->at(-2,-1) =  18;
    this->at(-2, 0) =   3;
    this->at(-2, 1) =  -6;
    this->at(-2, 2) = -15;
    this->at(-1,-2) =  18;
    this->at(-1,-1) =  18;
    this->at(-1, 0) = -12;
    this->at(-1, 1) =  -6;
    this->at(-1, 2) = -30;
    this->at( 0,-2) =   3;
    this->at( 0,-1) = -12;
    this->at( 0, 0) =  -8;
    this->at( 0, 1) =   4;
    this->at( 0, 2) =  -5;
    this->at( 1,-2) =  -6;
    this->at( 1,-1) =  -6;
    this->at( 1, 0) =   4;
    this->at( 1, 1) =  34;
    this->at( 1, 2) =  10;
    this->at( 2,-2) = -15;
    this->at( 2,-1) = -30;
    this->at( 2, 0) =  -5;
    this->at( 2, 1) =  10;
    this->at( 2, 2) =  25;
  }

  template<class T>
  kirschKernelYY<T>::kirschKernelYY() : kernel2D<T>(-2,-2,2,2) {
    this->at(-2,-2) =   9;
    this->at(-2,-1) =  18;
    this->at(-2, 0) =  27;
    this->at(-2, 1) =  18;
    this->at(-2, 2) =   9;
    this->at(-1,-2) =  18;
    this->at(-1,-1) =  18;
    this->at(-1, 0) =  36;
    this->at(-1, 1) =  18;
    this->at(-1, 2) =  18;
    this->at( 0,-2) = -21;
    this->at( 0,-1) = -60;
    this->at( 0, 0) = -72;
    this->at( 0, 1) = -60;
    this->at( 0, 2) = -21;
    this->at( 1,-2) = -30;
    this->at( 1,-1) = -30;
    this->at( 1, 0) = -60;
    this->at( 1, 1) = -30;
    this->at( 1, 2) = -30;
    this->at( 2,-2) =  25;
    this->at( 2,-1) =  50;
    this->at( 2, 0) =  75;
    this->at( 2, 1) =  50;
    this->at( 2, 2) =  25;
  }
  

}

