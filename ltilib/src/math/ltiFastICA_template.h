/*
 * Copyright (C) 1998 -  2004
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
 * file .......: ltiFastICA.cpp
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 3.3.2004
 * revisions ..: $Id: ltiFastICA_template.h,v 1.7 2004/05/06 10:31:26 alvarado Exp $
 */

//TODO: include files
#include "ltiFastICA.h"
#include "ltiPCA.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // fastICA::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // fastICA
  // --------------------------------------------------

  // default constructor
  template<class T>
  fastICA<T>::fastICA()
    : linearAlgebraFunctor() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  fastICA<T>::fastICA(const parameters& par)
    : linearAlgebraFunctor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  fastICA<T>::fastICA(const fastICA<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template<class T>
  fastICA<T>::~fastICA() {
  }

  // returns the name of this type
  template<class T>
  const char* fastICA<T>::getTypeName() const {
    return "fastICA<T>";
  }

  // copy member
  template<class T>
  fastICA<T>&
  fastICA<T>::copy(const fastICA<T>& other) {
      linearAlgebraFunctor::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  template<class T>
  fastICA<T>&
  fastICA<T>::operator=(const fastICA<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* fastICA<T>::clone() const {
    return new fastICA<T>(*this);
  }

  // return parameters
  template<class T>
  const typename fastICA<T>::parameters&
  fastICA<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }


  template <class T>
  bool fastICA<T>::updateParameters() {
    bool b = true;

    const parameters& par = getParameters();

    // negentropy approximation method selection
    if (b) {
      switch (par.negentropyApproximation) {
          case parameters::Exp: {  
            g = &fastICA<T>::expo1;       
            gd = &fastICA<T>::expo2;
            // copy tuning constant
            a1 = par.expAlpha;
            break;
          }
          case parameters::Tanh: {
            g = &fastICA<T>::tanh1;
            gd = &fastICA<T>::tanh2;
            a1 = par.tanhAlpha;
            break;
          }
          case parameters::Kurt: {
            g = &fastICA<T>::kurt1;
            gd = &fastICA<T>::kurt2;
            // there is no tuning constant for this function
            break;
          }
      }
    }  
    return b;    
  }

  // -------------------------------------------------------------------
  //  apply-methods!
  // -------------------------------------------------------------------

  /********************************************
   * negentropy approximation functions
   ********************************************/

  template <class T>
  T fastICA<T>::expo1(const T u) const {
    return (u*exp(static_cast<T>(-0.5)*a1*u*u));
  }

  template <class T>
  T fastICA<T>::expo2(const T u) const {
    return ((static_cast<T>(1)-u*u*a1)*exp(static_cast<T>(-0.5)*a1*u*u));
  }

  template <class T>
  T fastICA<T>::tanh1(const T u) const {
    return tanh(a1*u);
  }

  template <class T>
  T fastICA<T>::tanh2(const T u) const {
    const T buf = tanh(a1*u);
    return ((1-buf*buf)*a1);
  }

  template <class T>
  T fastICA<T>::kurt1(const T u) const {
    return (u*u*u);
  }
 
  template <class T>
  T fastICA<T>::kurt2(const T u) const {
    return (3*u*u);
  }


  template <class T>
  bool fastICA<T>::centerData(matrix<T>& srcdest) const {
    // x = x' - E{x'}
    const int cols = srcdest.columns();
    const int rows = srcdest.rows();
    vector<T> e(false,cols);
    e = srcdest.getRow(0);
    int i;
    for (i = 1;i<rows;i++) {
      e.add(srcdest.getRow(i));
    }
    e.divide(static_cast<T>(rows));

    for (i = 0;i<rows;i++) {
      srcdest.getRow(i).subtract(e);
    }
    return true;
  }

  template <class T>
  const matrix<T>& fastICA<T>::getRotationMatrix() const {
    return rotationMatrix;
  }

  template <class T>
  const matrix<T>& fastICA<T>::getWhiteningMatrix() const {
    return whiteningMatrix;
  }

  template <class T>
  const matrix<T>& fastICA<T>::getTransformMatrix() const {
    return transformMatrix;
  }

  template <class T>
  const vector<T>& fastICA<T>::getOffsetVector() const {
    return pca.getOffsetVector();
  }


  template <class T>
  bool fastICA<T>::getTransformMatrix(matrix<T>& dest) const {
    dest.copy(transformMatrix);
    return true;
  }

  template <class T>
  bool fastICA<T>::getWhiteningMatrix(matrix<T>& dest) const {
    dest.copy(whiteningMatrix);
    return true;
  }

  template <class T>
  bool fastICA<T>::getRotationMatrix(matrix<T>& dest) const {
    dest.copy(rotationMatrix);
    return true;
  }

  template <class T>
  bool fastICA<T>::getOffsetVector(vector<T>& dest) const {
    return pca.getOffsetVector(dest);
  }



  template <class T>
  bool fastICA<T>::computeTransformMatrix(const matrix<T>& src) {
    _lti_debug("computing transformation matrix...\n");
    matrix<T> tmp;
    bool rc = true;

    // length of input vectors
    int cols = src.columns();

    /*****************
     * preprocessing *
     *****************/

    // whitening data 
    _lti_debug("Whitening data...\n");
    typename principalComponents<T>::parameters pCPar;
    pCPar.whitening = true;
    pCPar.resultDim = cols;
    pca.setParameters(pCPar);
    rc = pca.apply(src,tmp);
    whiteningMatrix = pca.getTransformMatrix();
    _lti_debug("PCA transformMatrix is:\n"<<whiteningMatrix<<"\n");

    vector<T> w = pca.getEigenValues();
    w.resize(cols,T(0),true,false);
    w.apply(sqrt);
    int i;
    for (i=0;i<whiteningMatrix.rows();i++) {
      whiteningMatrix.getRow(i).edivide(w);
    }
    _lti_debug("whiteningMatrix is:\n"<<whiteningMatrix<<"\n");

    /*****************
     * now do ICA... *
     *****************/

    // number of independent components to estimate
    int m = cols;

    // resize destination matrix to suit output
    rotationMatrix.resize(m,cols,T(),false,false);
  
    const parameters& par = getParameters();

    switch (par.orthogonalizationMethod) {
        case parameters::Deflationary : {
          rc = rc && icaDeflationary(tmp,rotationMatrix,par);
          break;
        }
        case parameters::Symmetric : {
          rc = rc && icaSymmetric(tmp,rotationMatrix,par);
          break;
        }
    }

    transformMatrix.multiply(whiteningMatrix,rotationMatrix);
    _lti_debug("transformMatrix is:\n"<<transformMatrix<<"\n");

    return rc;
  }

  // on copy transform for matrices
  template <class T>
  bool fastICA<T>::transform(const matrix<T>& src, matrix<T>& dest) const {


    _lti_debug("Doing transformation of input data...\n");
    dest.resize(src.size(),T(),false,false);

    // subtract offset of input data
    const vector<T>& offset = pca.getOffsetVector();

    if (offset.size() != src.columns()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    int i;
    for (i=0; i<src.rows(); i++) {
      dest.getRow(i).subtract(src.getRow(i),offset);
    } 

  
    // do transform
    dest.multiply(transformMatrix);

    return true;
  }

  // on copy transform for vectors
  template <class T>
  bool fastICA<T>::transform(const vector<T>& src, vector<T>& dest) const {
    _lti_debug("Doing transformation of input data...\n");
    dest.resize(src.size(),T(),false,false);

    // subtract offset of input data
    const vector<T>& offset = pca.getOffsetVector();

    if (offset.size() != src.size()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    dest.subtract(src,offset);
    
    // doing transformation
    transformMatrix.leftMultiply(dest);
    return true;
  }


  // on place version for vectors
  template <class T>
  bool fastICA<T>::transform(vector<T>& srcdest) const {
    vector<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }  

  // on place version for vectors
  template <class T>
  bool fastICA<T>::transform(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }
    return false;
  }

  template <class T>
  bool fastICA<T>::apply(const matrix<T>& src,matrix<T>& dest) {
    //  returncode
    bool rc = computeTransformMatrix(src);
    rc = rc && transform(src,dest);
    return rc;   
  }

  template <class T>
  bool fastICA<T>::apply(matrix<T>& srcdest) {
    matrix<T> tmp;
    bool rc = apply(srcdest,tmp);
    srcdest.detach(tmp);
    return rc;
  }

  // iteration with deflationary orthogonalization
  template <class T>
  bool fastICA<T>::
    icaDeflationary(const matrix<T>& src, 
                          matrix<T>& dest,const parameters& par) const {

    _lti_debug("starting ICA with deflationary approach...\n");
    // m = number of independent components to estimate
    const int m = dest.rows();
    const int cols = dest.columns();

    // number of samples
    const int rows = src.rows();
    const T trows=static_cast<T>(rows);

    int i,j,p;

    // foreach component
    for (p=0;p<m;p++) {
      // choose initial random value of unit form
      vector<T> w(false,cols);
      T qsum = 0;
      for (i=0;i<cols;i++) {
        T buf = static_cast<T>(uniDist.draw()*10);
        qsum += buf*buf;
        w[i] = buf;
      }
      // normalize
      w.divide(sqrt(qsum));
      

      // number of iteration stepps done per component
      int steps = par.maxSteps;
      
      vector<T> walt(cols,static_cast<T>(0));
      // repeat until converged
      do {
        steps--;
        _lti_debug2("component: "<<p<< 
                    " iteration step: "<<par.maxSteps-steps<<"\n");
        _lti_debug3("w is:\n"<<w<<"\nwalt is:\n"<<walt<<"\n");
        walt = w;

#ifdef LTI_DEBUG 
        if (step==0) {
          _lti_debug("no convergence after "<<maxsteps<<" result may be inaccurate\n");
        }
#endif
        // first component
        // w+ = E{xG'(w^Tx)} - E{G''(w^Tx)}w
        vector<T> e1(cols);
        T e2 = T();
        for (i = 0;i<rows;i++) {
          const vector<T>& x = src.getRow(i);    
          T a = w.dot(x);
          e1.addScaled((*this.*g)(a),x);
          e2 += (*this.*gd)(a);
        }
        e1.divide(trows);
        e2 /= trows;
        w.addScaled(e1,-e2,w);

        // deflationary orthogonalization
        if (p != 0) {
          _lti_debug2("do orthogonalization");
          const vector<T> wp = w;
          for (j=0;j<p;j++) {
            const vector<T>& wj = dest.getRow(j);
            T scale = wp.dot(wj);
            w.addScaled(-scale,wj);
          }
        }

        //normalize w
        T val = T();
        for (i = 0;i<cols;i++) {
          const T b = w[i];
          val += b*b;
        }
        w.divide(sqrt(val));
      } while ((!(closeEnough(w,walt,par.epsilon))) && (steps>0));

      // safe this component
      dest.getRow(p) = w;
    } // for p
    return true;
  }


  // iteration with symmetric orthogonalization
  template <class T>
  bool fastICA<T>::icaSymmetric(const matrix<T>& src, matrix<T>& dest,const parameters& par) const {

    // number of independent components to estimate
    const int m = dest.rows();
    const int cols = dest.columns();

    // number of samples
    const int rows = src.rows();
    const T trows = static_cast<T>(rows);

    int i,p;

    // choose initial randoms value for all independent components
    // save those in dest matrix
    for (p=0;p<m;p++) {
      T qsum = 0;
      vector<T>& w = dest.getRow(p);
      for (i=0;i<cols;i++) {
        T buf = static_cast<T>(uniDist.draw()*10);
        qsum += buf*buf;
        w[i] = buf;
      }
      // normalize
      w.divide(sqrt(qsum));
    }

    // number of iterations done
    int steps = par.maxSteps;

    // matrix to compare iteration steps 
    matrix<T> walt(false,m,cols);
    // do iteration until convergent
    do {
      _lti_debug("w:\n"<<dest<<"\n");
      steps--;

      // foreach component
      for (p=0;p<m;p++) {
        vector<T>& w = dest.getRow(p);
        // w+ = E{xG'(w^Tx)} - E{G''(w^Tx)}w
        vector<T> e1(cols);
        T e2 = T();
        for (i = 0;i<rows;i++) {
          const vector<T>& x = src.getRow(i);    
          T a = w.dot(x);
          e1.addScaled((*this.*g)(a),x);
          e2 += (*this.*gd)(a);
        }
        e1.divide(trows);
        e2 /= trows;
        w.addScaled(e1,-e2,w);

      }

      // symmetric orthogonalization (iterative method)
      // 1. W/ ||W||
      // 2. W = 3/2W - 1/2WW^TW
      // repeat until WW^T is close enough to identity from step 2

      _lti_debug("w before normalization:\n"<<dest<<"\n");
      // norm
      T norm = 0;
      for (p=0;p<m;p++) {
        vector<T>& r = dest.getRow(p);
        T sum = abs(r[0]);
        int j;
        for (j=0;j<cols;j++) {
          sum += abs(r[0]);
        }
        if (norm < sum){
          norm = sum;
        }
      }
      dest.divide(norm);
      _lti_debug("w after normalization:\n"<<dest<<"\n");

      matrix<T> a,e;
      e.resize(dest.size(),T(),false,false);
      e.setIdentity();
      do {
        a.transpose(dest);
        a.leftMultiply(dest);
        a.multiply(dest);
        a.multiply(static_cast<T>(-0.5f));
        a.addScaled(static_cast<T>(1.5f),dest);
        dest.copy(a);
        a.transpose(dest);
        a.leftMultiply(dest);
        _lti_debug("orthogonalization step:\n"<<dest<<"\n");
      } while (!(closeEnough(a,e,par.epsilon)));
 

    } while ((!(closeEnough(dest,walt,par.epsilon))) && (steps>0));
    
    return true;
  }


  // method to determine if a value is closeEnough to another value
  template <class T>
  bool fastICA<T>::closeEnough(const matrix<T>& a,
                                  const matrix<T>& b, 
                                  const T epsilon) const {
    bool rc = true;
    const int rows = a.rows();
    assert(rows==b.rows());
    int i = 0;
    while ((rc) && (i<rows)) {
      rc = closeEnough(a.getRow(i),b.getRow(i),epsilon);
      i++;
    }
    return rc;
  }

  template <class T>
  bool fastICA<T>::closeEnough(const vector<T>& a,
                                  const vector<T>& b, 
                                  const T epsilon) const {
    int i;
    const int size=a.size();
    assert(size==b.size());
    T sum = static_cast<T>(0);
    for (i=0;i<size;i++) {
      const T c = a[i]-b[i];
      sum += c*c;
    } 
    if (sum >= (epsilon*epsilon)) {
      sum = a[0]+b[0];
      for (i=1;i<size;i++) {
        const T c = a[i]+b[i];
        sum += c*c;
      }
    }
    return (sum < (epsilon*epsilon));
  }


}

#include "ltiUndebug.h"

