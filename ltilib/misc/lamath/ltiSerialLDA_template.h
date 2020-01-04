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
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiLDA_template.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiSerialLDA_template.h,v 1.8 2005/02/02 14:17:33 doerfler Exp $
 */

#include "ltiMatrixInversion.h"
#include "ltiGenEigVectors.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"
#include "ltiSTLIoInterface.h"
#include <fstream>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // serialLDA::parameters
  // --------------------------------------------------

  // default constructor
  template <class T>
  serialLDA<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters() {
    //TODO: Initialize your parameter values!
    resultDim=3;
    autoDim=false;
  }

  // copy constructor
  template <class T>
  serialLDA<T>::parameters::parameters(const parameters& other) {
    copy(other);
  }

  // destructor
  template <class T>
  serialLDA<T>::parameters::~parameters() {
  }

  // get type name
  template <class T>
  const char* serialLDA<T>::parameters::getTypeName() const {
    return "serialLDA::parameters";
  }

  // copy member

  template <class T>
  typename serialLDA<T>::parameters&
    serialLDA<T>::parameters::copy(const parameters& other) {
#ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    linearAlgebraFunctor::parameters::copy(other);
#else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    linearAlgebraFunctor::parameters& (linearAlgebraFunctor::parameters::* p_copy)
      (const linearAlgebraFunctor::parameters&) =
      linearAlgebraFunctor::parameters::copy;
    (this->*p_copy)(other);
# endif

    resultDim=other.resultDim;
    autoDim=other.autoDim;
    return *this;
  }

  // clone member
  template <class T>
  functor::parameters* serialLDA<T>::parameters::clone() const {
    return new parameters(*this);
  }



  template <class T>
  bool serialLDA<T>::parameters::read(ioHandler& handler,
                                                const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      lti::read(handler,"dim",resultDim);
      lti::read(handler,"auto",autoDim);
      if (complete) {
        b=b && handler.readEnd();
      }
    }
    return b;
  }


  template <class T>
  bool serialLDA<T>::parameters::write(ioHandler& handler,
                                                 const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"dim",resultDim);
      lti::write(handler,"auto",autoDim);
      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }



  // --------------------------------------------------
  // serialLDA
  // --------------------------------------------------

  // default constructor
  template <class T>
  serialLDA<T>::serialLDA()
    : linearAlgebraFunctor(){
    parameters tmp;
    setParameters(tmp);
    // we only need maximum likelihood estimation of covariances
    typename varianceFunctor<T>::parameters vp(mlvar.getParameters());
    vp.type=varianceFunctor<T>::parameters::Maxlikely;

    mlvar.setParameters(vp);
    inv.useLUD();
  }

  // copy constructor
  template <class T>
  serialLDA<T>::serialLDA(const serialLDA& other) {
    copy(other);
  }

  // destructor
  template <class T>
  serialLDA<T>::~serialLDA() {
  }

  // returns the name of this type
  template <class T>
  const char* serialLDA<T>::getTypeName() const {
    return "serialLDA";
  }

  // copy member
  template <class T>
  serialLDA<T>&
    serialLDA<T>::copy(const serialLDA& other) {
    linearAlgebraFunctor::copy(other);

    orderedEigVec=other.orderedEigVec;
    transformMatrix=other.transformMatrix;
    eigValues=other.eigValues;
    classes=other.classes;
    elements=other.elements;
    Sw=other.Sw;
    Sb=other.Sb;
    sumOfElements=other.sumOfElements;
    classMeans=other.classMeans;
    classCounts=other.classCounts;
    isValid=other.isValid;

    return (*this);
  }

  // clone member
  template <class T>
  functor* serialLDA<T>::clone() const {
    return new serialLDA<T>(*this);
  }

  // return parameters
  template <class T>
  const typename serialLDA<T>::parameters&
  serialLDA<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException();
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type matrix<T>!
  /*
  template <class T>
  bool serialLDA<T>::apply(matrix<T>& srcdest,
                                            const ivector &labels) const {
    matrix<T> dest;
    bool flag=apply(srcdest,dest,labels);

    srcdest=dest;
    return flag;
  }

  template <class T>
  void serialLDA<T>::find(const ivector &data,
                                           int value, ivector &result) const {
    int n=data.size();
    std::vector<int> index;
    result.resize(n);
    for (int i=0; i < data.size(); i++) {
      if (data.at(i) == value) {
        index.push_back(i);
      }
    }
    result.castFrom(index);
  }

  template <class T>
  void serialLDA<T>::submatrix(const matrix<T>& data,
                                                const ivector &index,
                                                matrix<T>& result) const {

    int j=0;
    result.resize(index.size(),data.columns());
    for (ivector::const_iterator i=index.begin(); i != index.end(); i++) {
      result.setRow(j++,data.getRow(*i));
    }
  }

  template <class T>
  bool serialLDA<T>::computeSb(const matrix<T>& src,
                               matrix<T>& Sb,
                               const ivector &labels) const {
    int cols=src.columns();
    int nclasses=labels.maximum()+1;
    // class mean
    matrix<T> cmean(nclasses,cols);
    matrix<T> tmp;
    vector<T> tmp2;
    vector<T> mc(nclasses);
    vector<T> tmean(cols,0.0);
    int i;

    // first, compute the class-mean of each class
    for (i=0; i<nclasses; i++) {
      ivector index;
      find(labels,i,index);
      submatrix(src,index,tmp);
      // mean of class i
      mean.meanOfRows(tmp,cmean.getRow(i));
      tmp2=cmean.getRow(i);
      // number of elements in class i
      mc.at(i)=tmp.rows();
      tmp2.multiply(mc.at(i));
      // compute sum of all vectors so far
      tmean.add(tmp2);
    }
    // now tmean becomes the total mean vector
    tmean.divide(T(src.rows()));

    _lti_debug("Tmean = " << tmean << endl);

    // normalize class-means with respect to total mean
    for (i=0; i<nclasses; i++) {
      cmean.getRow(i).subtract(tmean);
      cmean.getRow(i).multiply(sqrt(mc.at(i)));
    }
    Sb.resize(cols,cols);
    // compute scatter matrix
    for (i=0; i<nclasses; i++) {
      const vector<T>& r=cmean.getRow(i);
      tmp.outerProduct(r,r);
      Sb.add(tmp);
    }
    return true;
  }
  */

  // On copy apply for type matrix<T>!
  template <class T>
  bool serialLDA<T>::consider(const matrix<T>& src) {
    int cols=src.columns();
    int nk=src.rows();

    if (classes > 0 && src.columns() != Sb.columns()) {
      setStatusString("Trying to consider data with different dimension!");
      return false;
    }

    // class covariance matrix
    matrix<T> ccov;

    // update Sw
    mlvar.covarianceMatrixOfRows(src,ccov);
    ccov.multiply(T(src.rows()));
    if (classes == 0) {
      Sw=ccov;
    } else {
      Sw.add(ccov);
    }

    // update Sb in three steps:
    // first, update the global mean
    // second, recompute Sb with the new mean

    if (classes = 0) {
      sumOfElements.resize(cols);
      sumOfElements.fill(T(0));
      Sb.resize(cols,cols);
      Sb.fill(T(0));
    }
    classes++;
    vector<T> tmpMean(cols);
    for (int i=0; i<nk; i++) {
      tmpMean.add(src.getRow(i));
    }
    sumOfElements.add(tmpMean);
    tmpMean.divide(T(elements));
    classCounts.push_back(nk);
    classMeans.push_back(tmpMean);
    elements+=nk;

    vector<T> mean(sumOfElements);
    mean.divide(T(elements));
    for (int j=0; j<classes; j++) {
      vector<T> tmp(classMeans[j]);
      tmp.subtract(mean);
      matrix<T> tmp2;
      tmp2.outerProduct(tmp,tmp);
      Sb.addScaled(T(nk),tmp2);
    }
    isValid=false;
    return true;
  }

  template <class T>
  bool serialLDA<T>::apply(const matrix<T>& src, matrix<T>& dest) {

    if (!isValid) {
      if (!buildTransform()) {
        return false;
      }
    }
    const parameters& p = getParameters();

    int dim = (p.autoDim) ? usedDimensionality : p.resultDim;
    if (dim <= 0) {
      dim=src.columns();
    }

    if (dim != transformMatrix.columns()) {
      transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);
    }

    dest=src;
    dest.multiply(transformMatrix);
    return true;
  }

  template <class T>
  bool serialLDA<T>::apply(const vector<T>& src, vector<T>& dest) {
    const parameters& p = getParameters();

    int dim = (p.autoDim) ? usedDimensionality : p.resultDim;
    if (dim <= 0) {
      dim=src.size();
    }

    if (dim != transformMatrix.columns()) {
      transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);
    }

    dest.resize(getParameters().resultDim);
    //dest.fill(T(0));
    transformMatrix.leftMultiply(src,dest);
    /*
    matrix<T>::const_iterator mi=transformMatrix.begin();
    // do it by hand, otherwise it would be too slow
    for (vector<T>::const_iterator i=src.begin(); i != src.end(); i++) {
      for (vector<T>::iterator di=dest.begin(); di != dest.end(); di++) {
        *di+=*i**mi++;
      }
    }
    */
    return true;
  }

  template <class T>
  bool serialLDA<T>::apply(matrix<T>& srcdest) {
    matrix<T> src=srcdest;
    return apply(src,srcdest);
  }

  template <class T>
  bool serialLDA<T>::buildTransform() {

    matrix<T> tmp;

    _lti_debug("Sw = " << Sw << endl);
    _lti_debug("Sb = " << Sb << endl);

    if (inv.apply(Sw,tmp)) {
      _lti_debug("inv(Sw) = " << tmp << std::endl);

      tmp.multiply(Sb);
      //tmp.multiply(Sb);

      //*out << "\nintermediate matrix = \n" << tmp << std::endl;

      if (!eig.apply(tmp,eigValues,orderedEigVec)) {
        setStatusString(eig.getStatusString());
        usedDimensionality=0;
        eigValues.resize(0);
        orderedEigVec.resize(0,0);
        return false;
      }
    } else {
      setStatusString("Matrix inversion failed!");
      return false;
    }

    _lti_debug( eigValues << endl);
    _lti_debug(orderedEigVec << endl);

    int dim=min(checkDim(),orderedEigVec.columns());

    if (dim <= 0) {
      transformMatrix.clear();
      setStatusString("Scatter matrix has rank 0");
      return false;
    }

    transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);
    isValid=true;
    return true;
  }


/*
  // On copy apply for type matrix<T>!
  template <class T>
  bool serialLDA<T>::apply(const matrix<T>& src,
                                            matrix<T>& dest,
                                            const matrix<T>& mySb) {
    varianceFunctor<T> var;
    generalEigenVectors<T> eig;
    matrixInversion<T> inv;
    int cols=src.columns();

    // class covariance matrix
    matrix<T> ccov(cols,cols);
    matrix<T> tmp;

    // intra-class covariance matrix
    var.covarianceMatrixOfRows(src,ccov);

    buildTransform(ccov,mySb);

    return apply(src,dest);
  }
       */

  template <class T>
  int serialLDA<T>::checkDim() {
    const parameters& p=getParameters();
    if (p.autoDim) {
      _lti_debug("cdA");
      int n=0;
      if (eigValues.size() > 0) {
        typename lti::vector<T>::const_iterator i=eigValues.begin();
        // set the lowest accepted eigenvalue to the largest eigenvalue
        // divided by 1e6. This works only, if eigValues.at(0) is really
        // the maximum, i.e. if the eigenvalues are sorted descendingly
        // maybe 1e6 could be enlarged to something around 1e10
        T evLimit=abs(*i)/T(100000.0);
        _lti_debug("cdB");
        while (i != eigValues.end()) {
          if (abs(*i) > evLimit) {
            n++;
          }
          i++;
        }
        _lti_debug("cdC");
        usedDimensionality=n;
      } else {
        usedDimensionality=0;
      }
      _lti_debug("Resultdim: " << n << endl);
    } else {
      _lti_debug("cdP");
      usedDimensionality = p.resultDim;
    }
    return usedDimensionality;
  }


  template <class T>
  void serialLDA<T>::setDimension(int n) {
    try {
      parameters p=getParameters();
      p.resultDim=n;
      setParameters(p);
    } catch (std::exception x) {
      std::cerr << x.what() << std::endl;
    }
  }

  template <class T>
  bool serialLDA<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix);
    return true;
  }

  template <class T>
  bool serialLDA<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec);
    return true;
  }

  template <class T>
  bool serialLDA<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec);
    return true;
  }

  template <class T>
  bool serialLDA<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues);
    return true;
  }

  template <class T>
  const matrix<T>&
  serialLDA<T>::getTransformMatrix() const {
    return transformMatrix;
  }

  template <class T>
  const matrix<T>&
  serialLDA<T>::getEigenVectors() const {
    return orderedEigVec;
  }

  template <class T>
  const vector<T>&
  serialLDA<T>::getEigenValues() const {
    return eigValues;
  }

  template <class T>
  bool serialLDA<T>::read(ioHandler& handler, const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      b=b && lti::read(handler, "parameters", param);
      setParameters(param);
      b=b && lti::read(handler, "classes", classes);
      b=b && lti::read(handler, "elements", elements);
      b=b && lti::read(handler, "Sw", Sw);
      b=b && lti::read(handler, "Sb", Sb);
      b=b && lti::read(handler, "sumOfElements", sumOfElements);
      b=b && lti::read(handler, "classMeans", classMeans);
      b=b && lti::read(handler, "classCounts", classCounts);
      b=b && lti::read(handler, "eigenVectors", orderedEigVec);
      b=b && lti::read(handler, "eigenValues", eigValues);

      buildTransform();
      checkDim();

      if (complete) {
        b=b && handler.readEnd();
      }
    }
    return b;
  }

  template <class T>
  bool serialLDA<T>::write(ioHandler& handler, const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      b=b && lti::write(handler, "parameters", getParameters());
      b=b && lti::write(handler, "classes", classes);
      b=b && lti::write(handler, "elements", elements);
      b=b && lti::write(handler, "Sw", Sw);
      b=b && lti::write(handler, "Sb", Sb);
      b=b && lti::write(handler, "sumOfElements", sumOfElements);
      b=b && lti::write(handler, "classMeans", classMeans);
      b=b && lti::write(handler, "classCounts", classCounts);
      b=b && lti::write(handler, "eigenVectors", orderedEigVec);
      b=b && lti::write(handler, "eigenValues", eigValues);

      b=b && lti::write(handler, "eigenVectors", orderedEigVec);
      b=b && lti::write(handler, "eigenValues", eigValues);

      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }



}

#include "ltiUndebug.h"
