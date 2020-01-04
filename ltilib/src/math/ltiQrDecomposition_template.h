/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiQrDecomposition.cpp
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 26.1.2004
 * revisions ..: $Id: ltiQrDecomposition_template.h,v 1.7 2004/06/07 12:43:48 doerfler Exp $
 */

#include "ltiQrDecomposition.h"
#ifdef HAVE_LAPACK
#include "clapack.h"
#endif

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"


namespace lti {
  // --------------------------------------------------
  // qrDecomposition::parameters
  // --------------------------------------------------

  // implemented in the header due to MSC bug.


  // --------------------------------------------------
  // qrDecomposition<T>
  // --------------------------------------------------

  // default constructor
  template <class T>
  qrDecomposition<T>::qrDecomposition()
#ifdef HAVE_LAPACK
    : linearAlgebraFunctor(), lapackInterface() {
#else
    : linearAlgebraFunctor(){
#endif

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template <class T>
  qrDecomposition<T>::qrDecomposition(const parameters& par)
#ifdef HAVE_LAPACK
    : linearAlgebraFunctor(), lapackInterface() {
#else
    : linearAlgebraFunctor() {
#endif

    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <class T>
  qrDecomposition<T>::qrDecomposition(const qrDecomposition<T>& other)
#ifdef HAVE_LAPACK
    : linearAlgebraFunctor(), lapackInterface() {
#else
    : linearAlgebraFunctor() {
#endif
    copy(other);
  }

  // destructor
  template <class T>
  qrDecomposition<T>::~qrDecomposition() {
  }

  // returns the name of this type
  template <class T>
  const char* qrDecomposition<T>::getTypeName() const {
    return "qrDecomposition<T>";
  }

  // copy member
  template <class T>
  qrDecomposition<T>&
  qrDecomposition<T>::copy(const qrDecomposition<T>& other) {
#ifdef HAVE_LAPACK 
    linearAlgebraFunctor::copy(other);
#else
    linearAlgebraFunctor::copy(other);
#endif

    return (*this);
  }

  // alias for copy member
  template <class T>
  qrDecomposition<T>&
    qrDecomposition<T>::operator=(const qrDecomposition<T>& other) {
    return (copy(other));
  }


  // clone member
  template <class T>
  functor* qrDecomposition<T>::clone() const {
    return new qrDecomposition<T>(*this);
  }

  // return parameters
  template <class T>
  const typename qrDecomposition<T>::parameters&
  qrDecomposition<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // transpose matrix and do qr decomposition in row mode to accelerate memory
  // access after that the result q ist transposed again
  // in the process of transpose a new matrix is created which will be connected
  template <class T>
  bool qrDecomposition<T>::transApply(matrix<T>& srcdest, matrix<T>& r) const {
    int m = srcdest.columns();
    int n = srcdest.rows(); 
    srcdest.transpose();
    int x,z;
    T sum;
    
    typename matrix<T>::iterator itx;
    typename matrix<T>::iterator ity;
    typename matrix<T>::iterator itz;
    typename matrix<T>::iterator itb;
    typename matrix<T>::iterator itrx = r.begin();
    typename matrix<T>::iterator itye;
    
    // for all columns
    //    for (x = 0;x<m;x++) {
    for (itx=srcdest.begin(),x=0;
         x<m;
         itx+=n,itrx+=m,x++) {
      itye=itx+n;
      
      //         cout << x;      
      // sum of squares of the y column of src
      /*
        for (y = 0,sum=T(0);y<n;y++) {
        const T a = srcdest.at(x,y);
        sum += a*a;
       } 
      */
      for (ity = itx,sum=T(0);ity!=itye;ity++) {
        sum += (*ity)*(*ity);
      }
      sum = sqrt(sum);
    
      *(itrx+x) = sum;
      /*
        for (y=0;y<n;y++) {
        srcdest.at(x,y) /= sum;
        }^
      */
      for (ity = itx;ity!=itye;ity++) {
        (*ity) /= sum;
      }
      
      /*
        for (z = x+1;z<m;z++) {
      */
      for (itz = itx+n,z=x+1;z<m; itz +=n,z++) {
        /*
        for (y=0,sum=T(0);y<n;y++) {
        sum += srcdest.at(x,y)*srcdest.at(z,y);
                }
        */
        for (ity=itx,itb=itz,sum=T(0);ity!=itye;ity++,itb++) {
          sum += (*ity)*(*itb);
        }
        /*
          r.at(x,z) = sum;
        */
        *(itrx+z) = sum;
      
        /*
          for(y=0;y<n;y++) {
          srcdest.at(z,y) -= srcdest.at(x,y)*sum;
          }
        */
        for (ity=itx,itb=itz;ity!=(itye);ity++,itb++) {
          (*itb) -= (*ity)*sum;
        }
      }
    }
    srcdest.transpose();
    return true;    
  }

  

  // On place apply for type matrix<T>!
  template <class T>
  bool qrDecomposition<T>::apply(matrix<T>& srcdest, matrix<T>& r) const {
    int m = srcdest.columns();
    int n = srcdest.rows();

    const parameters& par = getParameters();
    int pttft = par.performanceTweakThresholdForTranspose;

    bool success = false;

    T sum;
    // resize m
    r.resize(min(m,n),m);

    // if matrix is not connected switch to failsafe slower method
    if (srcdest.getMode()!=lti::matrix<T>::Connected) {

      // due to memory access reasons for bigger matrices 
      // the matrix is transposed to acelerate qr decomposition
      if ((m>pttft) && (n>pttft)) {
        _lti_debug("transpose first...\n");
        success = transApply(srcdest,r);
      } 
      else {

        int x,y,z;

        // for all columns
        for (x = 0;x<m;x++) {
      
          // sum of squares of the y column of src
          for (y = 0,sum=T(0);y<n;y++) {
            const T a = srcdest.at(y,x);
            sum += a*a;
          }
          sum = sqrt(sum);
          _lti_debug("Sum of squares of column "<<x<<" is "<<sum<<"\n");

          r.at(x,x) = sum;
          for (y=0;y<n;y++) {
            srcdest.at(y,x) /= sum;
          }

          for (z = x+1;z<m;z++) {
            for (y=0,sum=T(0);y<n;y++) {
              sum += srcdest.at(y,x)*srcdest.at(y,z);
            }
            r.at(x,z) = sum;
            for(y=0;y<n;y++) {
              srcdest.at(y,z) -= srcdest.at(y,x)*sum;
            }
          }
        } // for
        success = true;
      } // else
    } 
    // faster method
    else {
#ifdef HAVE_LAPACK
      parameters par = getParameters();
      if (par.useLapack) {

        // transpose matrix for lapack use
        srcdest.transpose();

        /*  M       (input) INTEGER
         *          The number of rows of the matrix A.  M >= 0. */
        long rows = srcdest.columns();


        /*  N       (input) INTEGER
         *          The number of columns of the matrix A.  N >= 0. */
        long cols = srcdest.rows();

        /*  LDA     (input) INTEGER
         *          The leading dimension of the array A.  LDA >= max(1,M). */
        long lda = rows;


        /*  TAU     (output) REAL array, dimension (min(M,N))
         *          The scalar factors of the elementary reflectors (see Further
         *          Details). */
        vector<T> tau(n);

        long info=0;

        // workspace variables
        T twork[1];

        /*  WORK    (workspace/output) REAL array, dimension (LWORK)
         *          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */
        T* work;        

        /*  LWORK   (input) INTEGER
         *          The dimension of the array WORK.  LWORK >= max(1,N).
         *          For optimum performance LWORK >= N*NB, where NB is 
         *          the optimal blocksize.
         *
         *          If LWORK = -1, then a workspace query is assumed; the routine
         *          only calculates the optimal size of the WORK array, returns
         *          this value as the first entry of the WORK array, and no error
         *          message related to LWORK is issued by XERBLA. */
        long lwork = -1;

       
        // ask for optimal workspace size
        geqrf(&rows,&cols,&srcdest.at(0,0), &lda, &tau.at(0),
               &twork[0], &lwork, &info);

        lwork=static_cast<long>(twork[0]);
        work = new T[lwork];
        // now compute qr decomposition
        geqrf(&rows,&cols,&srcdest.at(0,0), &lda, &tau.at(0),
               &work[0], &lwork, &info);
        delete[] work;
        success=(info==0);

        if (success) {

          _lti_debug2("srcdest:\n"<<srcdest<<"\n");
          _lti_debug2("tau:\n"<<tau<<"\n");
         
          // construct r matrix 
//           int i,j;
//           for (i=0;i<r.rows();i++) {
//             for (j=i;j<r.columns();j++) {
//               r.at(i,j) = srcdest.at(j,i);
//             }
//           } 

          _lti_debug2("m="<<m<<" n="<<n<<"\n");
          
          // construct r matrix
          typename matrix<T>::iterator iti;
          typename matrix<T>::iterator itj;
          typename matrix<T>::iterator ita;
          typename matrix<T>::iterator itb;
          typename matrix<T>::iterator itre;
          typename matrix<T>::iterator itend;
          int i;
          for (iti=r.begin(), itre=iti+m, ita=srcdest.begin(), i=1;
               iti!=itre;
               ++iti,ita+=n, i++) {
            itend=ita+min(i,n);
            for (itj=iti, itb=ita; itb!=itend; itj+=m, ++itb) {
              (*itj)=(*itb);
            }
          }


          // construct q matrix
          lwork = -1;
          // aks for optimal workspace size
          long longN = static_cast<long>(cols);

          _lti_debug2("rows="<<rows<<" cols="<<cols
                      <<"\nlongN="<<longN<<" lda="<<lda<<"\n");

          cols=min(rows,cols);longN=cols;
          srcdest.resize(cols,rows,0.,true,true);

          orgqr(&rows,&cols,&longN,&srcdest.at(0,0), &lda, &tau.at(0),
                 &twork[0], &lwork, &info);
          lwork=static_cast<long>(twork[0]);
          work = new T[lwork];

          // now do the work          
          orgqr(&rows,&cols,&longN,&srcdest.at(0,0), &lda, &tau.at(0),
                 &work[0], &lwork, &info);
          delete[] work;
          success=(info==0);
          srcdest.transpose();
        }
      }
      else {
#endif
        // transpose matrix ?
        if ((m>pttft) && (n>pttft)) {
          _lti_debug("transpose first...\n");

          success=transApply(srcdest,r);
        }
        else {

          typename matrix<T>::iterator isx;
          typename matrix<T>::iterator isy, isyEnd;
          typename matrix<T>::iterator isz;
          typename matrix<T>::iterator isp;
          typename matrix<T>::iterator irx;
          typename matrix<T>::iterator irz;
          typename matrix<T>::iterator sb = srcdest.begin();
          // end of first row
          typename matrix<T>::iterator se = sb+m;
          typename matrix<T>::iterator rb = r.begin();
          int size = m*n;
          int xm=0;
          int x=0;
          
          // for all columns of src
          for (isx=sb,irx=rb;isx != (se);
               isx++,
               irx++,
               xm=++x*m) {

            // sum of squares of the x column of src
            isyEnd=isx+size;
    
            for (isy=isx,sum=T(0);isy != isyEnd;isy+=m) {
              sum += (*isy)*(*isy);
            } 
            sum = sqrt(sum);

            /*      r.at(x,x) = sum; */
            *(irx+(xm)) = sum;
            for (isy = isx;isy!=(isx+size);isy+=m) {
              (*isy) /= sum;
            }
            /*    
                  for (z = x+1;z<m;z++) {*/
            for (isz = isx+1, irz = irx+1;
                 isz!=(se);
                 isz++, irz++) {

              /*
                for (y=0,sum=0.0f;y<n;y++) {
                sum += src.at(y,x)*src.at(y,z);
                } */
              for (isy = isx, sum=0.0f, isp = isz;
                   isy!=(isx+size);
                   isy+=m,isp+=m) {
                sum += (*isy)*(*isp);
              } 
              /*
                r.at(x,z) = sum; */
              *(irz+(xm)) = sum;
              /*
                for(y=0;y<n;y++) {
                src.at(y,z) -= src.at(y,x)*r.at(x,z);
                }*/
              for(isy=isz,isp=isx; 
                  isy !=(isz+size);          
                  isy += m,
                  isp += m) {
                (*isy) -= (*isp)*sum;
              }
            } // for (isz = isx+1 ...
          } // for (isx = ...
          success = true;
        } // else transpose
#ifdef HAVE_LAPACK
      }
#endif

    } // else
   
    return success;
  };

  // On copy apply for type matrix<T>!
  template <class T>
  bool qrDecomposition<T>::apply(const matrix<T>& src,matrix<T>& q,matrix<T>& r) const {
    q = src;
    return apply(q,r);
  };

 

}

#include "ltiUndebug.h"
