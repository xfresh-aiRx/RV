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
 * file .......: ltiEquationSystem_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 02.06.99
 * revisions ..: $Id: ltiEquationSystem_template.h,v 1.17 2005/11/04 09:37:38 doerfler Exp $
 */

#include "ltiMath.h"
#include "ltiMatrixDecomposition.h"
#include <limits>

namespace lti {
  //--- decompositionSolution ---
  //-----------------------------

  // construction
  template<class T>
  decompositionSolution<T>::decompositionSolution() {
    parameters param;
    setParameters(param);
  }

  template<class T>
  decompositionSolution<T>::decompositionSolution(const parameters& theParams){
    setParameters(theParams);
  }

  template<class T>
  decompositionSolution<T>::decompositionSolution(const matrix<T>& theMatrix) {
    parameters tmpParam;
    tmpParam.systemMatrix = theMatrix;
    setParameters(tmpParam);
  }

  // sets the functor's parameters.
  template<class T>
  bool decompositionSolution<T>::updateParameters() {
    decomposed=false;
    return true;
  }

  // returns the current parameters
  template<class T>
  const typename decompositionSolution<T>::parameters&
  decompositionSolution<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(params)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *params;
  }

  // copy data of "other" functor.
  template<class T>
  decompositionSolution<T>&
  decompositionSolution<T>::copy(const decompositionSolution<T>& other) {
    linearEquationSystemSolutionMethod<T>::copy(other);
    decomposed=other.decomposed;
    dcmpMat=other.dcmpMat;

    return (*this);
  }

  //--- QR solution functor ---
  //---------------------------

  // returns the current parameters
  template<class T>
  const typename qrSolution<T>::parameters&
  qrSolution<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if (isNull(params)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *params;
  }

  // onPlace version of apply
  template<class T>
  double qrSolution<T>::apply(vector<T>& b) {
    vector<T> tmpVec;
    double tmpRes;
    tmpRes=apply(b,tmpVec);
    b=tmpVec;
    return tmpRes;
  }

  // onCopy version of apply
  // from: Engeln-Müllges, Reutter
  //       Formelsammlung zur Numerischen Mathematik mit C-Programmen
  //       BI-Wissenschaftsverlag, 1990
  //       p. 492
  template<class T>
  double qrSolution<T>::apply(const vector<T>& b,vector<T>& x) {
    const parameters& tmpParam = getParameters();
    const matrix<T>& A = tmpParam.systemMatrix;
    const int& n = A.columns();
    const int& m = A.rows();
    vector<T> c(b);
    double radiant,alpha,factor,sum;
    int i,k,j;

    //        int si=b.size();
    if ((m < n)||(m != b.size())) {
      linearAlgebraFunctor::incompatibleDimensionsException tmpExc;
      throw tmpExc;
    }

    if(!this->decomposed) {
      // decompose A, compose R=HA, compose c=Hb (H: transposed Q-matrix)
      this->dcmpMat.castFrom(A);
      dcmpVec.resize(n);
      helpVec.resize(n);
      for(i = 0; i < n; i++) {
        for(radiant = sqr(this->dcmpMat[i][i]),
              k = i + 1; k < m; k++) {
          radiant += sqr(this->dcmpMat[k][i]);
        }
        if(radiant < (4*std::numeric_limits<double>::epsilon())) {
          linearAlgebraFunctor::singularMatrixException tmpExc;
          throw tmpExc;
        }
        if(this->dcmpMat[i][i] > 0) {
          alpha = sqrt(radiant);
        }
        else {
          alpha =- sqrt(radiant);
        }
        helpVec[i] = 1.0 / (radiant + alpha * this->dcmpMat[i][i]);
        this->dcmpMat[i][i] += static_cast<T>(alpha);

        dcmpVec[i] =- alpha;
        for(k = i + 1; k < n; k++) {
          for(factor = .0 ,j = i;j < m;j++) {
            factor += this->dcmpMat[j][k] * this->dcmpMat[j][i];
          }
          factor *= helpVec[i];
          for(j = i; j < m; j++) {
            this->dcmpMat[j][k] -= static_cast<T>(factor * 
                                                  this->dcmpMat[j][i]);
          }
        }
        for(factor = .0 ,j = i;j < m;j++) {
          factor += c[j] * this->dcmpMat[j][i];
        }
        factor *= helpVec[i];
        for(j = i; j < m; j++) {
          c[j] -= static_cast<T>(factor * this->dcmpMat[j][i]);
        }
      }
      this->decomposed = true;
    }
    else { // compose c=Hb
      for(i = 0; i < n; i++) {
        for(factor = .0, j = i; j < m; j++) {
          factor += c[j] * this->dcmpMat[j][i];
        }
        factor *= helpVec[i];
        for(j = i; j < m; j++) {
          c[j] -= static_cast<T>(factor * this->dcmpMat[j][i]);
        }
      }
    }

    // solve Rx=c
    x.resize(n);
    for(i = n-1; i >= 0; i--) {
      for(sum = .0, k = i + 1; k < n; k++) {
        sum += this->dcmpMat[i][k] * x[k];
      }
      x[i] = static_cast<T>((c[i] - sum) / dcmpVec[i]);
    }

    if(tmpParam.computeResiduum) { // calculate residuum
      for(sum = .0, i = n; i < m; i++) {
        sum += c[i]*c[i];
      }
      return sqrt(sum);
    }

    return -1;
  }

  // copy data of "other" functor.
  template<class T>
  qrSolution<T>& qrSolution<T>::copy(const qrSolution<T>& other) {
    decompositionSolution<T>::copy(other);
    helpVec=other.helpVec;
    dcmpVec=other.dcmpVec;

    return (*this);
  }

  //--- LU solution functor ---
  //---------------------------

  // onPlace version of apply
  // see for example: Press, Vetterling, Teukolsky, Flannery
  //                  Numerical Recipes in C, 2nd edition
  //                  Cambridge University Press, 1992
  //                  p. 47
  template<class T>
  bool luSolution<T>::apply(vector<T>& b) {
    const parameters& tmpParam = this->getParameters();
    const int& n=tmpParam.systemMatrix.rows();
    if((n != tmpParam.systemMatrix.columns()) || (n!=b.size())) {
      this->setStatusString("Incompatible dimensions");
      return false;
    }
    int i,ii=-1,ip,j;
    T sum;

    // lu decomposition
    if(!this->decomposed) {
      luDecomposition< T > decmp;
      if (decmp.apply(tmpParam.systemMatrix,this->dcmpMat,dcmpVec)) {
        this->decomposed = true;
      } else {
        this->setStatusString("System matrix could not be decomposed - singular?\n");
        return false;
      }
    }

    // forward substitution and backsubstitution
    for (i=0;i<n;i++) {
      ip=dcmpVec[i];
      sum=b[ip];
      b[ip]=b[i];
      if (ii+1) {
        for (j=ii;j<=i-1;j++) {
          sum -= this->dcmpMat[i][j]*b[j];
        }
      }
      else if (sum) {
        ii=i;
      }
      b[i]=sum;
    }
    for (i=n-1;i>=0;i--) {
      sum=b[i];
      for (j=i+1;j<n;j++) {
        sum -= this->dcmpMat[i][j]*b[j];
      }
      b[i]=sum/this->dcmpMat[i][i];
    }

    return true;
  }

  // onCopy version of apply
  template<class T>
  bool luSolution<T>::apply(const vector<T>& b,vector<T>& x) {
    x.copy(b);
    return apply(x);
  }

  // solve many equations at the same time!
  template<class T>
  bool luSolution<T>::apply(matrix<T>& X) {
    bool result = true;
    X.transpose();
    for (int i=0;i<X.rows();i++) {
      result = result && apply(X.getRow(i));
    }
    X.transpose();
    return result;
  }

  // solve many equations at the same time!
  template<class T>
  bool luSolution<T>::apply(const matrix<T>& B,matrix<T>& X) {
    X.copy(B);
    return apply(X);
  }

  // copy data of "other" functor.
  template<class T>
  luSolution<T>& luSolution<T>::copy(const luSolution<T>& other) {
    decompositionSolution<T>::copy(other);
    dcmpVec=other.dcmpVec;

    return (*this);
  }
}

