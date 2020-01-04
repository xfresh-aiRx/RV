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
 * revisions ..: $Id: ltiLDA_template.h,v 1.23 2005/11/14 15:53:03 doerfler Exp $
 */

#include "ltiMatrixInversion.h"
#include "ltiGenEigVectors.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"
#include <fstream>

#undef _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // linearDiscriminantAnalysis::parameters
  // --------------------------------------------------

  // default constructor
  template <class T>
  linearDiscriminantAnalysis<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters() {
    //TODO: Initialize your parameter values!
    resultDim=3;
    autoDim=false;
    // this is only for the restriction to retain the old behavior
    // when introducing new parameters.
    useSVD=false;
  }

  // copy constructor
  template <class T>
  linearDiscriminantAnalysis<T>::parameters::parameters(const parameters& other) {
    copy(other);
  }

  // destructor
  template <class T>
  linearDiscriminantAnalysis<T>::parameters::~parameters() {
  }

  // get type name
  template <class T>
  const char* linearDiscriminantAnalysis<T>::parameters::getTypeName() const {
    return "linearDiscriminantAnalysis::parameters";
  }

  // copy member

  template <class T>
  typename linearDiscriminantAnalysis<T>::parameters&
    linearDiscriminantAnalysis<T>::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    linearAlgebraFunctor::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    linearAlgebraFunctor::parameters& (linearAlgebraFunctor::parameters::* p_copy)
      (const linearAlgebraFunctor::parameters&) =
      linearAlgebraFunctor::parameters::copy;
    (this->*p_copy)(other);
# endif

    resultDim=other.resultDim;
    autoDim=other.autoDim;
    useSVD=other.useSVD;
    return *this;
  }

  // clone member
  template <class T>
  functor::parameters* linearDiscriminantAnalysis<T>::parameters::clone() const {
    return new parameters(*this);
  }



  template <class T>
  bool linearDiscriminantAnalysis<T>::parameters::read(ioHandler& handler,
                                                const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    _lti_debug("reading params\n");

    if (b) {
      lti::read(handler,"dim",resultDim);
      lti::read(handler,"auto",autoDim);
      lti::read(handler,"useSVD",useSVD);
      if (complete) {
        b=b && handler.readEnd();
      }
    }
    return b;
  }


  template <class T>
  bool linearDiscriminantAnalysis<T>::parameters::write(ioHandler& handler,
                                                 const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    _lti_debug("writing params\n");

    if (b) {
      lti::write(handler,"dim",resultDim);
      lti::write(handler,"auto",autoDim);
      lti::write(handler,"useSVD",useSVD);
      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }



  // --------------------------------------------------
  // linearDiscriminantAnalysis
  // --------------------------------------------------

  // default constructor
  template <class T>
  linearDiscriminantAnalysis<T>::linearDiscriminantAnalysis()
    : linearAlgebraFunctor(){
    parameters tmp;
    setParameters(tmp);
    // we only need maximum likelihood estimation of covariances
    typename varianceFunctor<T>::parameters vp(mlvar.getParameters());
    vp.type=varianceFunctor<T>::parameters::Maxlikely;

    mlvar.setParameters(vp);
    if (tmp.useSVD) {
      inv.useSVD();
    } else {
      inv.useLUD();
    }
  }

  // copy constructor
  template <class T>
  linearDiscriminantAnalysis<T>::linearDiscriminantAnalysis(const linearDiscriminantAnalysis& other) {
    copy(other);
  }

  // destructor
  template <class T>
  linearDiscriminantAnalysis<T>::~linearDiscriminantAnalysis() {
  }

  // returns the name of this type
  template <class T>
  const char* linearDiscriminantAnalysis<T>::getTypeName() const {
    return "linearDiscriminantAnalysis";
  }

  // copy member
  template <class T>
  linearDiscriminantAnalysis<T>&
    linearDiscriminantAnalysis<T>::copy(const linearDiscriminantAnalysis& other) {
    linearAlgebraFunctor::copy(other);

    orderedEigVec=other.orderedEigVec;
    transformMatrix=other.transformMatrix;
    eigValues=other.eigValues;
    return (*this);
  }

  // clone member
  template <class T>
  functor* linearDiscriminantAnalysis<T>::clone() const {
    return new linearDiscriminantAnalysis<T>(*this);
  }

  // return parameters
  template <class T>
  const typename linearDiscriminantAnalysis<T>::parameters&
    linearDiscriminantAnalysis<T>::getParameters() const {
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
  template <class T>
  bool linearDiscriminantAnalysis<T>::apply(matrix<T>& srcdest,
                                            const ivector &labels) {
    matrix<T> dest;
    bool flag=apply(srcdest,dest,labels);

    srcdest=dest;
    return flag;
  }


  template <class T>
  void linearDiscriminantAnalysis<T>::find(const ivector &data,
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

    // This one is for debugging:
//     int k=0;
//     _lti_debug2("Before resize: result.size is " << result.size() << "\n");
//     result.resize(n);
//     ivector::iterator j=result.begin();
//     _lti_debug2("After resize: data.size is " << n << "\n");
//     _lti_debug2("After resize: result size is " << result.size() << "\n");
//     for (int i=0; i < data.size(); i++) {
//       _lti_debug3("Data " << i << " is " << data[i] << ": ");
//       if (data.at(i) == value) {
//         _lti_debug3("pushed");
//         *j++=i;
//         k++;
//       }
//       _lti_debug3("\n");
//     }
//     _lti_debug2("Before 2nd resize: target is " << k << "\n");
//    result.resize(k);
  }

  template <class T>
  void linearDiscriminantAnalysis<T>::submatrix(const matrix<T>& data,
                                                const ivector &index,
                                                matrix<T>& result) const {

    int j=0;
    result.resize(index.size(),data.columns());
    for (ivector::const_iterator i=index.begin(); i != index.end(); i++) {
      result.setRow(j++,data.getRow(*i));
    }
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::computeSb(const matrix<T>& src,
                                                matrix<T>& Sb,
                                                const ivector &labels) const {
    _lti_debug("begin computeSb");
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
      mc.at(i)=static_cast<T>(tmp.rows());
      tmp2.multiply(mc.at(i));
      // compute sum of all vectors so far
      tmean.add(tmp2);
    }
    // now tmean becomes the total mean vector
    tmean.divide(T(src.rows()));

    _lti_debug("Tmean = " << tmean << std::endl);

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
    _lti_debug("end computeSb");
    return true;
  }


  // On copy apply for type matrix<T>!
  template <class T>
  bool linearDiscriminantAnalysis<T>::apply(const matrix<T>& src,
                                            matrix<T>& dest,
                                            const ivector &labels) {
    int cols=src.columns();
    int nclasses=labels.maximum()+1;

    _lti_debug("Training LDA with " << nclasses << " classes\n");
    _lti_debug("Training LDA for " << cols << " columns\n");
    

    // class mean
    vector<T> cmean(cols);
    // class covariance matrix
    matrix<T> ccov(cols,cols);
    // sum of class covariance matrices
    matrix<T> Sw(cols,cols);
    // between-class scatter matrix
    matrix<T> Sb(cols,cols);

    matrix<T> tmp;

    ivector index;
    for (int i=0; i<nclasses; i++) {
      _lti_debug("Before find\n");
      find(labels,i,index);
      //*out << "Index class " << i << ":\n" << index << std::endl;
      _lti_debug("Before submatrix\n");
      submatrix(src,index,tmp);
      // mean of class
      //mean.meanOfRows(tmp,cmean);
      // intra-class covariance matrix

      _lti_debug2("TMP = " << tmp << std::endl);
      mlvar.covarianceMatrixOfRows(tmp,ccov);
      _lti_debug2("CC = " << ccov << std::endl);
      ccov.multiply(T(tmp.rows()));
      Sw.add(ccov);
    }

    //Sw.divide(nclasses);

    computeSb(src,Sb,labels);
    //mlvar.covarianceMatrixOfRows(cmean,Sb);

    if (buildTransform(Sw,Sb)) {
      return apply(src,dest);
    } else {
      return false;
    }
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::apply(const matrix<T>& src,
                                                  matrix<T>& dest) {
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
  bool linearDiscriminantAnalysis<T>::apply(const vector<T>& src,
                                                  vector<T>& dest) {
    const parameters& p = getParameters();

    _lti_debug("clf apply on " << src << "\n");

    int dim = (p.autoDim) ? usedDimensionality : p.resultDim;
    if (dim <= 0) {
      dim=src.size();
    }
    _lti_debug("dim = " << dim << "\n");

    if (dim != transformMatrix.columns()) {
      transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);
    }
    _lti_debug("transform size = " << transformMatrix.size() << "\n");

    dest.resize(getParameters().resultDim);
    dest.fill(T(0));
    _lti_debug("target size = " << dest.size() << "\n");
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
  bool linearDiscriminantAnalysis<T>::apply(matrix<T>& srcdest) {
    matrix<T> src=srcdest;
    return apply(src,srcdest);
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::buildTransform(const matrix<T>& Sw,
                                                     const matrix<T>& Sb) {

    matrix<T> tmp;

    _lti_debug("Sw = " << Sw << std::endl);
    _lti_debug("Sb = " << Sb << std::endl);

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

    _lti_debug("Eigenvalues = " << eigValues << std::endl);
    _lti_debug("Eigenvectors = " << orderedEigVec << std::endl);

    int dim=min(checkDim(),orderedEigVec.columns());

    if (dim <= 0) {
      transformMatrix.clear();
      setStatusString("Scatter matrix has rank 0");
      return false;
    }

    transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);
    return true;
  }


  // On copy apply for type matrix<T>!
  template <class T>
  bool linearDiscriminantAnalysis<T>::apply(const matrix<T>& src,
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


  template <class T>
  int linearDiscriminantAnalysis<T>::checkDim() {
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
      _lti_debug("Resultdim: " << n << std::endl);
    } else {
      _lti_debug("cdP\n");
      usedDimensionality = p.resultDim;
    }
    return usedDimensionality;
  }


  template <class T>
  bool linearDiscriminantAnalysis<T>::setParameters(const parameters& theParam) {
    bool tmp=linearAlgebraFunctor::setParameters(theParam);
    if (getParameters().useSVD) {
      inv.useSVD();
    } else {
      inv.useLUD();
    }
    return tmp;
  }


  template <class T>
  void linearDiscriminantAnalysis<T>::setDimension(int n) {
    try {
      parameters p=getParameters();
      p.resultDim=n;
      setParameters(p);
    } catch (std::exception x) {
      std::cerr << x.what() << std::endl;
    }
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix);
    return true;
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec);
    return true;
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec);
    return true;
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues);
    return true;
  }

  template <class T>
  const matrix<T>&
  linearDiscriminantAnalysis<T>::getTransformMatrix() const {
    return transformMatrix;
  }

  template <class T>
  const matrix<T>&
  linearDiscriminantAnalysis<T>::getEigenVectors() const {
    return orderedEigVec;
  }

  template <class T>
  const vector<T>&
  linearDiscriminantAnalysis<T>::getEigenValues() const {
    return eigValues;
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::read(ioHandler& handler, const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      b=b && lti::read(handler, "parameters", param);
      setParameters(param);
      b=b && lti::read(handler, "eigenVectors", orderedEigVec);
      b=b && lti::read(handler, "eigenValues", eigValues);

      checkDim();

      if (complete) {
        b=b && handler.readEnd();
      }
    }
    return b;
  }

  template <class T>
  bool linearDiscriminantAnalysis<T>::write(ioHandler& handler, const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      b=b && lti::write(handler, "parameters", getParameters());
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
