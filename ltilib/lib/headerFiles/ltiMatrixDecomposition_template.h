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
 * file .......: ltiMatrixDecomposition_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 02.06.99
 * revisions ..: $Id: ltiMatrixDecomposition_template.h,v 1.17 2005/11/04 09:40:24 doerfler Exp $
 */

#include "ltiMath.h"
#include <limits>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {
  
//--- LU decomposition functor ---
  //--------------------------------

  // use numeric_limits<double>::epsilon() as epsilon value
  template <class T>
  const T luDecomposition<T>::my_epsilon=std::numeric_limits<T>::epsilon();

  template<class T>
  luDecomposition<T>::luDecomposition() {
//     luRef=0;
//     permRef=0;
    pivsign=1;
  }

  template<class T>
  luDecomposition<T>::luDecomposition(const matrix<T>& theMatrix) {
//     luRef=0;
//     permRef=0;
//     destroy=false;
    pivsign=1;
    luRef=theMatrix;
    apply(luRef,permRef,false);
  }

  /*
   * destructor, delete own matrix copies, if present
   */
  template<class T>
  luDecomposition<T>::~luDecomposition() {
//     if (destroy) {
//       delete luRef;
//       delete permRef;
//     }
  }


  /*
   * get the "L" part of the LU decomposition
   */
  template<class T>
  matrix<T> luDecomposition<T>::getL() const {
    matrix<T> res;
    getL(res);
    return res;
  }

  /*
   * get the "L" part of the LU decomposition
   */
  template<class T>
  void luDecomposition<T>::getL(matrix<T>& l) const {
    const int rows = luRef.rows();
    const int cols = luRef.columns();
    l.resize(rows,cols,T(),false,false);
    
    int i, j;
    for (i = 0; i < rows; i++) {
      for (j = 0; j < i; j++) {
        l.at(i,j) = luRef.at(i,j);
      }
      l.at(i,i) = T(1);
      for (j = i + 1; j < cols; j++) {
        l.at(i,j) = T(0);
      }
    }
  }


  /*
   * get the "U" part of the LU decomposition
   */
  template<class T>
  void luDecomposition<T>::getU(matrix<T>& u) const {
    const int cols = luRef.columns();
    u.resize(cols,cols,T(),false,false);
    
    int i, j;
    for (i = 0; i < cols; i++) {
      for (j = 0; j < i; j++) {
        u.at(i,j) = T(0);
      }
      for (j = i; j < cols; j++) {
        u.at(i,j) = luRef.at(i,j);
      }
    }
  }
    

  /*
   * get the "U" part of the LU decomposition
   */
  template<class T>
  matrix<T> luDecomposition<T>::getU() const {
    matrix<T> res;
    getU(res);
    return res;
  }


  /*
   * compute the determinant of the last LU decomposition
   */
  template<class T>
  T luDecomposition<T>::det() const {
    if (luRef.rows() != luRef.columns()) {
      throw incompatibleDimensionsException();
    }
    T d=T(pivsign);
    for (int j=0; j<luRef.rows(); j++) {
      d*=luRef.at(j,j);
    }
    return d;
  }

  /*
   * compute the determinant of the given matrix
   */
  template<class T>
  T luDecomposition<T>::det(const matrix<T> &theMatrix) {
    luRef=theMatrix;
    if (apply(luRef,permRef,false)) {
      return det();
    } else {
      return 0.0;
    }
  }

  /*
   * determine if the given matrix is regular
   */
  template<class T>
  bool luDecomposition<T>::isRegular(const matrix<T> &theMatrix) {
    return (fabs(det(theMatrix)) > my_epsilon);
  }


  /*
   * determine if the last given matrix was regular
   */
  template<class T>
  bool luDecomposition<T>::isRegular() const {
    return (fabs(det()) > my_epsilon);
  }


  



  // onPlace version of apply
  // see for example: Press, Vetterling, Teukolsky, Flannery
  //                  Numerical Recipes in C, 2nd edition
  //                  Cambridge University Press, 1992
  //                  p. 46
  template<class T>
  bool luDecomposition<T>::apply(matrix<T>& theMatrix,
                                vector<int>& permutation, bool store)
  {
    const int& n=theMatrix.rows();
    if(n!=theMatrix.columns()) throw incompatibleDimensionsException();
    int i,imax,j,k,d;
    T big,dum,temp,sum;
    vector<T> vv(n);
    permutation.resize(n);

    bool isSingular = false;

    d=1;
    big=static_cast<T>(1);
    // loop over rows to get implicit scaling information
    for (i=0;i<n && big >= my_epsilon;i++) {
      big=0;
      for (j=0;j<n;j++) {
        if ((temp=abs(theMatrix[i][j])) > big) big=temp;
      }
      // if a row is all zero, there is nothing we can do...
      if (big >= my_epsilon) {
        // save scaling
        vv[i]=static_cast<T>(1)/big;
      }
    }
    _lti_debug2("LU: big=" << big << "\n");
    if (big < my_epsilon) {
      isSingular = true;
      big = my_epsilon;
    }
    // loop over columns (Crout's method)
    for (j=0;j<n;j++) {
      for (i=0;i<j;i++) {
        sum=theMatrix[i][j];
        for (k=0;k<i;k++) {
          sum -= theMatrix[i][k]*theMatrix[k][j];
        }
        theMatrix[i][j]=sum;
      }
      // initialize for the search for largest pivot element
      big=0.0;
      imax=j;
      for (i=j;i<n;i++) {
        sum=theMatrix[i][j];
        for (k=0;k<j;k++) {
          sum -= theMatrix[i][k]*theMatrix[k][j];
        }
        theMatrix[i][j]=sum;
        // is the figure of merit for the pivot better than the best so far?
        if ( (dum=vv[i]*abs(sum)) >= big) {
          big=dum;
          imax=i;
        }
      }
      if (j != imax) {
        // interchange rows, if necessary
        for (k=0;k<n;k++) {
          dum=theMatrix[imax][k];
          theMatrix[imax][k]=theMatrix[j][k];
          theMatrix[j][k]=dum;
        }
        // change parity of d
        d = -d;
        // interchange the scaling factor
        vv[imax]=vv[j];
      }
      permutation[j]=imax;

      // if the pivot element is zero, the matrix is singular
      // However, we just set it to epsilon, so that we don't get
      // illegal results later
      _lti_debug("LU: mat(j,j)=" << theMatrix[j][j] << "\n");
      if (closeToZero(theMatrix[j][j], my_epsilon)) {
        theMatrix[j][j]=(theMatrix[j][j] >= 0) ? my_epsilon : -my_epsilon;
        isSingular = true;
      }
      if (j != n) {
        // divide by the pivot element
        dum=static_cast<T>(1)/(theMatrix[j][j]);
        for (i=j+1;i<n;i++) {
          theMatrix[i][j] *= dum;
        }
      }
    }
    // algorithm is done now, do some housekeeping
//     if (destroy) {
//       delete luRef;
//       delete permRef;
//     }
    pivsign=d;
    if (store) {
      luRef=theMatrix;
      permRef=permutation;
    }
    //destroy=false;

    if (isSingular) {
      setStatusString("Singular matrix, cannot decompose");
      return false;
    } else {
      return true;
    }
  }

  // onCopy version of apply
  template<class T>
  bool luDecomposition<T>::apply(const matrix<T>& theMatrix,
                                matrix<T>& decomposition,
                                vector<int>& permutation) {
    decomposition.copy(theMatrix);
    return apply(decomposition,permutation);
  }

  // copy data of "other" functor.
  template<class T>
  luDecomposition<T>& luDecomposition<T>::copy(const luDecomposition<T>& other) {
    linearAlgebraFunctor::copy(other);
//     if (destroy) {
//       delete luRef;
//       delete permRef;
//     }
    luRef=other.luRef;
    //destroy=false;
    permRef=other.permRef;

    return (*this);
  }
}

#include "ltiUndebug.h"


