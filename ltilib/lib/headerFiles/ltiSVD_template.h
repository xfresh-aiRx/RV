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


/* ---------------------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiSVD_template.h
 * authors ....: Xin Gu
 * organization: LTI, RWTH Aachen
 * creation ...: 11.01.01
 * version ....: $Id: ltiSVD_template.h,v 1.23 2004/05/03 12:20:02 alvarado Exp $
 */

#include <cstdio>
#include <limits>

#include "ltiMath.h"
#include "ltiSort.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include <iostream>
using std::cout;
#endif

namespace lti {

#ifdef _LTI_DEBUG
  // nur tmp:
  template<class T>
  void printMatX(matrix<T> &mat) {
    int rows = mat.rows();
    int cols = mat.columns();

    int row,col;

    for (row = 0; row < rows; row++) {
      for (col = 0; col < cols; col++) {
//      cout << mat[row][0] << "\t" << mat[row][1] << "\t" << mat[row][2] << endl;
        cout << mat[row][col] << "\t";
      }
      cout << endl;
    }
  }

  template<class T>
  void printVecX(vector<T> &v) {
    int i;
    for (i = 0; i < v.size(); i++) {
      cout << v[i] << "\t";
    }
    cout << endl;
  }
#endif

  // --------------------------------------------------
  // singularValueDecomp
  // --------------------------------------------------
  template<class T>
  singularValueDecomp<T>::singularValueDecomp() {
    parameters defaultParam;
    setParameters(defaultParam);
  }

  template<class T>
  singularValueDecomp<T>::singularValueDecomp(const parameters& par) {
    setParameters(par);
  }

  template<class T>
  singularValueDecomp<T>::singularValueDecomp(bool sort) {
    parameters defaultParam;
    defaultParam.sort=sort;
    setParameters(defaultParam);
  }

  /*
   * copy constructor
   */
  template<class T>
  singularValueDecomp<T>::singularValueDecomp(const singularValueDecomp<T>&
                                              other) : linearAlgebraFunctor() {
    copy(other);
  }

  /*
   * destructor
   */
  template<class T>
  singularValueDecomp<T>::~singularValueDecomp() {
  }


  // returns the name of this type
  template <class T>
  const char* singularValueDecomp<T>::getTypeName() const {
    return "singularValueDecomp";
  }

  // copy member
  template <class T>
  singularValueDecomp<T>&
    singularValueDecomp<T>::copy(const singularValueDecomp<T>& other) {
      linearAlgebraFunctor::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* singularValueDecomp<T>::clone() const {
    return new singularValueDecomp<T>(*this);
  }

  template<class T>
  inline bool singularValueDecomp<T>::isZero(const T x) const {
    return (x == T(0));
  }

  template<class T>
  inline bool singularValueDecomp<T>::notZero(const T x) const {
    return (abs(x) >= std::numeric_limits<T>::epsilon());
    //This is numerically unstable:
    //return (x != T(0));
  }


  /*
   * Function for the Implementation of the SVD-algorithm
   */
  template<class T>
  bool singularValueDecomp<T>::decomposition(matrix<T>& src,
                                             vector<T>& w,
                                             matrix<T>& v) const {

    //get the rows of the matrix
    const int rows=src.rows();
    //get the columns of the matrix
    const int cols=src.columns();

    //int max=rows > cols ? rows : cols;

    static const int maxIter=150;

    //?
    //src.resize(max,max,T(0),true,true);
    w.resize(cols,T(0),false,true);
    v.resize(cols,cols,T(0),false,true);
    //cerr<<"src = "<<src<<endl<<endl;

    int i(-1),its(-1),j(-1),jj(-1),k(-1),nm(0);
    int ppi(0);
    bool flag;
    T anorm(0),c(0),f(0),g(0),h(0),s(0),scale(0),x(0),y(0),z(0);

    // Householder reduction to bidiagonal form
    vector<T> rv1(cols,T(0));

    g=scale=anorm=T(0);
    for (i=0; i < cols; i++){
      ppi=i+1;
      rv1.at(i)=scale*g;
      g=s=scale=T(0);
      if (i < rows ) {
        // compute the sum of the i-th column, starting from the i-th row
        scale=sumOfAbsColumnPart(src,i,i);
        //cerr << "scale = " << scale << endl;
        if (notZero(scale)) {
          // multiply the i-th column by 1.0/scale, start from the i-th element
          multiplyColumn(src,i,static_cast<T>(1)/scale,i);
          // sum of squares of column i, start from the i-th element
          s=dotOfColumns(src,i,i,i);
          f=src.at(i,i);  // f is the diag elem
          g=-sign(sqrt(s),f);
          h=f*g-s;
          src.at(i,i)=f-g;
          for (j=ppi; j < cols; j++) {
            // dot product of columns i and j, starting from the i-th row
            s=dotOfColumns(src,i,j,i);
            f=s/h;
            // copy the scaled i-th column into the j-th column
            for (k=i;k<rows;k++) {
              src.at(k,j)+=f*src.at(k,i);
            }
          }
          multiplyColumn(src,i,scale,i);
        }
      }
      // save singular value
      w.at(i)=scale*g;
      g=s=scale=T(0);
      if ((i < rows) && (i != cols-1)) {
        // sum of row i, start from columns i+1
        scale=sumOfAbsRowPart(src,i,ppi);
        //cerr << "scale2 = " << scale << endl;
        if (notZero(scale)) {
          multiplyRow(src,i,static_cast<T>(1)/scale,ppi);
          s=dotOfRows(src,i,i,ppi);
          f=src.at(i,ppi);
          g=-sign(sqrt(s),f); //<---- do something
          h=f*g-s;
          src.at(i,ppi)=f-g;
          for ( k=ppi; k < cols; k++) {
            rv1.at(k)=src.at(i,k)/h;
          }
          for ( j=ppi; j < rows; j++) {
            s=dotOfRows(src,i,j,ppi);
            for ( k=ppi; k < cols; k++) {
              src.at(j,k)+=s*rv1.at(k);
            }
          }
          multiplyRow(src,i,scale,ppi);
        }
      }
      anorm=max(anorm,abs(w.at(i))+abs(rv1.at(i)));
    }

//    cout << " src1 " << endl;
//    printMatX(src);

    // Accumulation of right-hand transformation
    for (i=cols-1; i>=0; i--) {
      //cerr << "\n";
      if ( i < cols-1 ) {
        //cerr << "g = " << g; // << endl;
        if (notZero(g)) {
          //cerr << "A";
          for ( j=ppi; j < cols; j++) {
            v.at(j,i)=(src.at(i,j)/src.at(i,ppi))/g;
          }
          //cerr << "B";
          for ( j=ppi; j < cols; j++) {
            for (s=T(0),k=ppi;k < cols;k++) {
              s+=src.at(i,k)*v.at(k,j);
            }
            for ( k=ppi;k < cols;k++) {
              v.at(k,j)+=s*v.at(k,i);
            }
          }
        }
        //cerr << "C";
        fillRow(v,i,0,ppi);
        fillColumn(v,i,0,ppi);
      }
      //cerr << "D";
      v.at(i,i)=T(1);
      g=rv1.at(i);
      ppi--;
    }
    //cerr << "E\n";

    // Accumulation of left-hand transformation
    for ( i=min(rows,cols)-1; i>=0; i--) {      //<---- do something
      ppi=i+1;
      g=w.at(i);
      //cerr << "F";
      fillRow(src,i,0,ppi);
      //cerr << "G";
      if (notZero(g)) {
        g=T(1)/g;
        //cerr << "H";
        for ( j=ppi; j < cols; j++) {
          s=dotOfColumns(src,i,j,ppi);
          f=(s/src.at(i,i))*g;
          for ( k=i;k < rows;k++) {
            src.at(k,j)+=f*src.at(k,i);
          }
        }
        //cerr << "I";
        multiplyColumn(src,i,g,i);
      } else {
        //cerr << "J";
        fillColumn(src,i,T(0),i);
      }
      ++src.at(i,i);
    }
    //cerr << "A\n";


    // Diagonalization of the bidiagonal form:
    // Loop over singular values,and over allowed iterations
    for ( k=cols-1; k >= 0; k--) {
      //cerr << "B";
      for (its=1; its <= maxIter; its++) {
        flag=true;
        //Test for splitting. Note that rv1[i] is always 0
        for (ppi=k; ppi >= 0; ppi--) {
          nm=ppi-1;
          if ((abs(rv1.at(ppi))+anorm) == anorm) {
            flag=false;
            break;
          }
          if ((abs(w.at(nm)+anorm) == anorm)) {
            break;
          }
        }
        //cerr << "C";

        //Cancellation of rv1[ppi],if ppi>1.
        if (flag) {
          c=T(0);
          s=T(1);
          for ( i=ppi; i <= k ;i++) {
            f=s*rv1.at(i);
            rv1.at(i)=c*rv1.at(i);
            if ((abs(f)+anorm) == anorm) {
              break;
            }
            g=w.at(i);
            h=pythag(f,g);


            w.at(i)=h;

//cout << "H = " << h << endl;
            h=T(1)/h;
            c=g*h;
            s=-f*h;
            for ( j=0;j < rows; j++) {
              y=src.at(j,nm);
              z=src.at(j,i);
              src.at(j,nm)=y*c+z*s;
              src.at(j,i)=z*c-y*s;
            }
          }
        }
        z=w.at(k);
        //cerr << "D";

        // Convergence. Singular value is made nonnegative.
        if (ppi == k) {
          if (z < T(0)) {
            w.at(k)=-z;
            for (j=0; j < cols; j++) {
              v.at(j,k)=-v.at(j,k);
            }
          }
          break;
        }

        if (its == maxIter) {
          char x[80];
          sprintf(x,"SVD did not converge after %d iterations!",maxIter);
          setStatusString(x);
          // make all singular values zero -> rank 0
          w.fill(T(0));
          return false;
        }
        //cerr << "E";
        // shift from bottom 2-by-2 minor
        x=w.at(ppi);
        nm=k-1;
        y=w.at(nm);
        g=rv1.at(nm);
        h=rv1.at(k);


        f=((y-z)*(y+z)+(g-h)*(g+h))/(T(2)*h*y);

//        cout << f << endl;


        if (abs(f) >= std::numeric_limits<T>::max())
      {
         //cout << "overflowAAAAAAAAAAAXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx0"  << endl;
         f = std::numeric_limits<T>::max();
       }

       g = pythag(f,T(1));
       f = ((x-z)*(x+z)+h*((y/(f+sign(g,f)))-h))/x;

       // deviation from numerical receipes
       if (abs(f) >= std::numeric_limits<T>::max())
       {
         //cout << "overflow1DDDDXXXXXXXXXXXXXXXXXXX, w(ppi):" << w[ppi]  << endl;
         f = std::numeric_limits<T>::epsilon();
       }

        //Next QR transformation
        c=s=T(1);
        for ( j=ppi; j<=nm ;j++){
          //cerr << "F";

          i=j+1;
          g=rv1.at(i);
          y=w.at(i);
          h=s*g;
          g=c*g;
          z=pythag(f,h);
          rv1.at(j)=z;


/*
          // deviation from numerical receipes, maybe not necessary
          if (isZero(z)){
            cout << "underflowXXXXXXXXXXXXXXXXXXXXXXXXXx2"  << endl;
            c = T(0);
            s = T(0);
          }
          else if (tabs(z) >= std::numeric_limits<T>::max()){
            cout << "underflowXXXXXXXXXXXXXXXXXXXXXXXXXX21"  << endl;
            z =  std::numeric_limits<T>::max();
            T sgn = f > 0 ? 1 : -1;
            f =  sgn * std::numeric_limits<T>::max();           // check sign
            c=f/z;
            s=h/z;
          }
          else */
          {
            c=f/z;
            s=h/z;
          }

          f=x*c+g*s;
          g=g*c-x*s;
          h=y*s;
          y*=c;
          for (jj=0; jj<cols ;jj++){
            x=v.at(jj,j);
            z=v.at(jj,i);
            v.at(jj,j)=x*c+z*s;
            v.at(jj,i)=z*c-x*s;
          }
          z=pythag(f,h);
          // Rotation can be arbitrary if z=0
          //cerr << "G";

          w.at(j)=z;

          if (notZero(z)) {
            z=T(1)/z;
            c=f*z;
            s=h*z;
          }
          f=c*g+s*y;
          x=c*y-s*g;
          for (jj=0; jj<rows; jj++){
            y=src.at(jj,j);
            z=src.at(jj,i);
            src.at(jj,j)=y*c+z*s;
            src.at(jj,i)=z*c-y*s;
          }
        }
        rv1.at(ppi)=T(0);
        rv1.at(k)=f;
        w.at(k)=x;

      }
    }

    if (getParameters().sort) {
      int sz=w.size();
      ivector idx(false,sz);
      for (i=0;i<sz;++i) {
        idx.at(i)=i;
      }

      sort2<T,int> sorter(true);
      if (!sorter.apply(w,idx)) {
        setStatusString(sorter.getStatusString());
        return false;
      }

      int r=src.rows();
      matrix<T> tsrc(false,r,sz);
      matrix<T> tv(false,sz,sz);

      for (i=0;i<sz;++i) {
        const int& ix=idx.at(i);
        for (j=0; j<r; ++j) {
          tsrc.at(j,i)=src.at(j,ix);
        }
        for (j=0; j<sz; ++j) {
          tv.at(j,i)=v.at(j,ix);
        }
      }

      tsrc.detach(src);
      tv.detach(v);

    }

    if (getParameters().transposeU) {
      src.transpose();
    }

    if (getParameters().transposeV) {
      v.transpose();
    }

    return true;
  }

  template <class T>
  bool singularValueDecomp<T>::apply(matrix<T>& src, vector<T>& w,
                                     matrix<T>& v) const {
    return decomposition(src,w,v);
  }

  template <class T>
  bool singularValueDecomp<T>::apply(const matrix<T>& src, matrix<T>& u,
                                     vector<T>& w, matrix<T>& v) const {
    u=src;
    return decomposition(u,w,v);
  }


  // help method: computes (a^2+b^2)^0.5 without destructive underflow
  // or overflow
  template<class T>
  T singularValueDecomp<T>::pythag(const T a,const T b) const {

    T tmp1,tmp2;
    tmp1=abs(a);
    tmp2=abs(b);
    if (tmp1 > tmp2)
      return static_cast<T>(tmp1*sqrt(1.0+sqr(tmp2/tmp1)));
    else
      return ((tmp1==T(0)) ? T(0) :
              static_cast<T>(tmp2*sqrt(1.0+sqr(tmp1/tmp2))));
  }

  //layout the matrix
//   template<class T>
//   void singularValueDecomp<T>::sort(matrix<T>& u,
//                                     vector<T>& w,
//                                     matrix<T>& v) const{

//     const int m=w.size();
//     int tmp;
//     double max;
//     //sortieren
//     for (int i=0; i<m-1; i++){
//       max=w.at(i);
//       tmp=i;
//       for(int j=i+1; j<m; j++){
//         if ( max<w.at(j) ){
//           tmp=j;
//           max=w.at(j);
//         }
//       }
//       if ( tmp != i ){
//         w.at(tmp)=w.at(i);
//         w.at(i)=max;
//         swapColumns(u,v,i,tmp);
//       }
//     }
//   }

  template<class T>
  const typename singularValueDecomp<T>::parameters&
  singularValueDecomp<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // swap columns(i,j)
//   template<class T>
//   void singularValueDecomp<T>::swapColumns(matrix<T>& u,
//                                            matrix<T>& v,
//                                            int& i,
//                                            int& j) const {
//     vector<T> h;
//     u.getColumnCopy(i,h);

//     u.setColumn(i,v.getColumnCopy(j));
//     v.setColumn(j,h);
//   }

}

#include "ltiUndebug.h"
