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
 * file .......: ltiEigenSystem_template.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 16.06.99
 * revisions ..: $Id: ltiEigenSystem_template.h,v 1.20 2004/05/03 12:20:01 alvarado Exp $
 */

#include <cstdio>
#include "ltiSort.h"

namespace lti
{
  //-----------------------------------
  //--- eigenSystem ---
  //-----------------------------------

    // construction
  template<class T>
  eigenSystem<T>::eigenSystem(const parameters& theParams) {
    setParameters(theParams);
  }

  template<class T>
  eigenSystem<T>::eigenSystem(const bool theSort) {
    parameters tmpParam;
    tmpParam.sort = theSort;
    setParameters(tmpParam);
  }

  // returns the current parameters
  template<class T>
  const typename eigenSystem<T>::parameters&
  eigenSystem<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());

    if(params==0)
      throw invalidParametersException(getTypeName());

    return *params;
  }


  //--------------
  //--- jacobi ---
  //--------------

  // constructor
  template <class T>
  jacobi<T>::jacobi() : eigenSystem<T>() {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  template <class T>
  jacobi<T>::jacobi(const parameters& par) : eigenSystem<T>() {
    setParameters(par);
  }

  // returns the current parameters
  template<class T>
  const typename jacobi<T>::parameters& jacobi<T>::getParameters() const {
    const parameters* params =
      dynamic_cast<const parameters*>(&functor::getParameters());
    
    if(params==0)
      throw functor::invalidParametersException(this->getTypeName());

    return *params;
  }


  template<class T>
  inline void jacobi<T>::rotate(double& g,double& h,matrix<double>& a,
                               const int i,const int j,const int k,const int l,
                               const double s,const double tau) const {
    g=a[i][j];
    h=a[k][l];
    a[i][j]=g-s*(h+g*tau);
    a[k][l]=h+s*(g-h*tau);
  }

  template<class T>
  inline void jacobi<T>::rotateT(double& g,double& h,matrix<T>& a,
                               const int i,const int j,const int k,const int l,
                               const double s,const double tau) const {
    g=a[i][j];
    h=a[k][l];
    a[i][j]=static_cast<T>(g-s*(h+g*tau));
    a[k][l]=static_cast<T>(h+s*(g-h*tau));
  }



  // onCopy version of apply
  // see for example: Press, Vetterling, Teukolsky, Flannery
  //                  Numerical Recipes in C, 2nd edition
  //                  Cambridge University Press, 1992
  //                  p. 467
  template<class T>
  bool jacobi<T>::apply(const matrix<T>& theMatrix,
                        vector<T>& eigenvalues,
                        matrix<T>& eigenvectors) const {
    static const int maxIter=100;

    const int& matrixDim = theMatrix.rows();

    if (matrixDim!=theMatrix.columns()) {
      this->setStatusString("incompatible dimensions");
      eigenvalues.clear();
      eigenvectors.clear();
      return false;
    }

    const parameters& tmpParam = getParameters();

    const bool sort = tmpParam.sort || (tmpParam.dimensions>0);

    int j,iq,ip,i;
    double tresh, theta, tau, sm, g, h;
    T c,s,t;

    matrix<double> cloneMatrix; cloneMatrix.castFrom(theMatrix);
    vector<double> b(matrixDim);
    vector<double> z(matrixDim, 0.0);

    eigenvalues.resize(matrixDim);
    eigenvectors.resize(matrixDim,matrixDim,T(0),false,true);

    for (ip=0;ip<matrixDim;ip++) {
      eigenvectors[ip][ip]=static_cast<T>(1);
      eigenvalues[ip]=static_cast<T>(b[ip]=cloneMatrix[ip][ip]);
    }

    for (i=0;i<maxIter;i++) {
      sm=0.0;
      for (ip=0;ip<matrixDim;ip++) {
        for(iq=ip+1;iq<matrixDim;iq++) {
          sm += fabs(cloneMatrix[ip][iq]);
        }
      }

      if (sm == 0.0) {   // normal return!
        if(sort) {
          sort2<T,T> sorter(true,false); // descending order and columns sorter
          sorter.apply(eigenvalues,eigenvectors);
        }
        
        if ((tmpParam.dimensions>0) && 
            (tmpParam.dimensions<eigenvalues.size())) {
          // reduce the dimensionality of the vector and matrices as requested
          eigenvalues.resize(tmpParam.dimensions,T(),true,false);
          eigenvectors.resize(eigenvectors.rows(),tmpParam.dimensions,T(),
                              true,false);
        }

        return true;
      }

      if (i < 3) {
        tresh= 0.2*sm/(matrixDim*matrixDim);
      } else {
        tresh=0.0;
      }

      for (ip=0;ip<matrixDim-1;ip++) {
        for (iq=ip+1;iq<matrixDim;iq++) {
          g = 100.0*fabs(cloneMatrix[ip][iq]);
          if ((i > 3) && ((fabs(eigenvalues[ip])+g) == fabs(eigenvalues[ip]))
              && ((fabs(eigenvalues[iq])+g) == fabs(eigenvalues[iq]))) {
            cloneMatrix[ip][iq]=0.0;
          }
          else if (fabs(cloneMatrix[ip][iq]) > tresh) {
            h=eigenvalues[iq]-eigenvalues[ip];
            if ((fabs(h)+g) == fabs(h)) {
              t=static_cast<T>((cloneMatrix[ip][iq])/h);
            } else {
              theta = 0.5*h/cloneMatrix[ip][iq];
              t = static_cast<T>(1.0/(fabs(theta)+sqrt(1.0+theta*theta)));
              if (theta < 0.0) {
                t = -t;
              }
            }
            c = static_cast<T>(1)/sqrt(1+t*t);
            s = t*c;
            tau = s/(c+1);
            h=t*cloneMatrix[ip][iq];
            z[ip] -= h;
            z[iq] += h;
            eigenvalues[ip] -= static_cast<T>(h);
            eigenvalues[iq] += static_cast<T>(h);
            cloneMatrix[ip][iq]=0.0;
            for (j=0;j<ip;j++)  {
              rotate(g,h,cloneMatrix,j,ip,j,iq,s,tau);
            }
            for (j=ip+1;j<iq;j++) {
              rotate(g,h,cloneMatrix,ip,j,j,iq,s,tau);
            }
            for (j=iq+1;j<matrixDim;j++) {
              rotate(g,h,cloneMatrix,ip,j,iq,j,s,tau);
            }
            for (j=0;j<matrixDim;j++) {
              rotateT(g,h,eigenvectors,j,ip,j,iq,s,tau);
            }
          }
        }
      }
      for (ip=0;ip<matrixDim;ip++) {
        b[ip] += z[ip];
        eigenvalues[ip] = static_cast<T>(b[ip]);
        z[ip] = 0.0;
      }
    }

    char x[80];
    sprintf(x,"Jacobi Method did not converge after %d iterations!",maxIter);
    this->setStatusString(x);

    return false;
  }

}

