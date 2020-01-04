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
 * file .......: ltiPCA_template.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiSerialPCA_template.h,v 1.8 2004/05/06 10:31:26 alvarado Exp $
 */

#include "ltiEigenSystem.h"
#include "ltiMatrixInversion.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // serialPCA::parameters
  // --------------------------------------------------

  // is in header file due to this ,%&,$& MSVC bug


  // --------------------------------------------------
  // serialPCA
  // --------------------------------------------------

  // default constructor
  template <class T>
  serialPCA<T>::serialPCA()
    : linearAlgebraFunctor(){
    parameters tmp;
    usedDimensionality=0;
    setParameters(tmp);
  }

  // copy constructor
  template <class T>
  serialPCA<T>::serialPCA(const serialPCA& oth)
    : linearAlgebraFunctor() {
    copy(oth);
  }

  // destructor
  template <class T>
  serialPCA<T>::~serialPCA() {
  }

  // returns the name of this type
  template <class T>
  const char* serialPCA<T>::getTypeName() const {
    return "serialPCA";
  }

  // copy member
  template <class T>
  serialPCA<T>&
    serialPCA<T>::copy(const serialPCA<T>& other) {
    linearAlgebraFunctor::copy(other);

    orderedEigVec.copy(other.orderedEigVec);
    transformMatrix.copy(other.transformMatrix);
    eigValues.copy(other.eigValues);

    offset.copy(other.offset);
    scale.copy(other.scale);
    whiteScale.copy(other.whiteScale);

    return (*this);
  }

  // clone member
  template <class T>
  functor* serialPCA<T>::clone() const {
    return new serialPCA<T>(*this);
  }

  // return parameters
  template <class T>
  const typename serialPCA<T>::parameters&
    serialPCA<T>::getParameters() const {
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

  // On copy apply for type matrix<T>!
  template <class T>
  bool serialPCA<T>::consider(const matrix<T>& src) {
    //const parameters& param = getParameters();

    //int cols=src.columns();

    vstat.considerRows(src);
    isValid=false;
    return true;
  }


  // On copy apply for type matrix<T>!
  template <class T>
  bool serialPCA<T>::transform(const matrix<T>& src, matrix<T>& dest) {
    const parameters& param = getParameters();

    varianceFunctor<T> var;
    meansFunctor<T> mean;
    _lti_debug("pA");
    //int cols=src.columns();

    //std::ofstream out("ltipca.txt");

    dest.copy(src);
    int i;

    if (!isValid) {
      buildTransform();
    }

    for (i=0; i<dest.rows(); i++) {
      vector<T>& vct = dest.getRow(i);
      vct.subtract(offset);
      if (param.useCorrelation) {
        vct.edivide(scale);
      }
    }

    dest.multiply(transformMatrix);

    if (param.whitening) {
      whiteScale=eigValues;
      whiteScale.resize(usedDimensionality,T(0),true,false);
      whiteScale.apply(sqrt);

      for (int j=0; j<dest.rows(); j++) {
        dest.getRow(j).edivide(whiteScale);
      }
    }

    //out.close();
    return true;
  }

  template <class T>
   bool serialPCA<T>::buildTransform() {
    const parameters& param=getParameters();

    eigenSystem<T> &eig=*param.eigen;

    _lti_debug("EIG is " << eig.getTypeName() << "\n");

    // correlation coefficient matrix
    matrix<T> cc;
    _lti_debug("pB");

    // compute correlation coefficients of data matrix
    typename serialVectorStats<T>::parameters vp;
    vp.correlation=param.useCorrelation;
    _lti_debug("cvA");
    vstat.setParameters(vp);
    _lti_debug("cvB");
    vstat.getCovariance(cc);

    _lti_debug("pC");

    // set the sort-flag of the eigenvector functor
    if (!eig.getParameters().sort) {
      // need clone because we do not know the actual class of the
      // eigenvector instance
      typename eigenSystem<T>::parameters* ep=dynamic_cast<typename eigenSystem<T>::parameters*>(eig.getParameters().clone());
      ep->sort=true;
      eig.setParameters(*ep);
      delete ep;
    }

    _lti_debug("pD");

    // now compute eigenvectors of correlation coefficient matrix
    if (!eig.apply(cc,eigValues,orderedEigVec)) {
      _lti_debug("pX");
      setStatusString(eig.getStatusString());
      usedDimensionality=0;
      eigValues.resize(0);
      orderedEigVec.resize(0,0);
      return false;
    }

    vstat.getMean(offset);

    if (param.useCorrelation) {
      // now normalize the data matrix to zero mean and unit variance
      vstat.getVariance(scale);
      scale.apply(sqrt);
      // mask columns with zero variance
      for (typename vector<T>::iterator si=scale.begin(); si != scale.end(); si++) {
        if (abs(*si) < std::numeric_limits<T>::epsilon()) {
          *si=T(1);
        }
      }
    }
    int dim = min(checkDim(),orderedEigVec.columns());

    if (dim <= 0) {
      transformMatrix.clear();
      setStatusString("Covariance matrix has rank 0");
      return false;
    }

    _lti_debug("pE");

    // the transformation matrix is simply the first columns of
    // the ordered eigenvectors
    transformMatrix.copy(orderedEigVec,0,MaxInt32, 0,dim-1);
    _lti_debug("peig: " << orderedEigVec.size() << std::endl);
    _lti_debug("dest: " << dest.size() << std::endl);
    _lti_debug("transform: " << transformMatrix.size() << std::endl);
    _lti_debug("pF");
    isValid=true;

    return true;
  }


  /*
   * Transforms a single vector according to a previously computed
   * transformation matrix.
   */
  template <class T>
  bool serialPCA<T>::transform(const vector<T>& src,
                               vector<T>& result) {

    if (!isValid) {
      buildTransform();
    }

    const parameters& p = getParameters();

    vector<T> tmp=src;

    if (offset.size() != tmp.size()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    tmp.subtract(offset);
    if (p.useCorrelation) {
      tmp.edivide(scale);
    }
    _lti_debug("e");
    //std::cerr << "eig: " << orderedEigVec.size() << std::endl;
    //std::cerr << "src: " << src.size() << std::endl;
    //std::cerr << "transform: " << transformMatrix.size() << std::endl;
    transformMatrix.leftMultiply(tmp,result);
    if (p.whitening) {
      result.edivide(whiteScale);
    }
    _lti_debug("f\n");

    return true;
  }


  template <class T>
  bool serialPCA<T>::apply(const matrix<T>& src, matrix<T>& dest) {
    return transform(src,dest);
  }


  template <class T>
  bool serialPCA<T>::apply(matrix<T>& srcdest) {
    matrix<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }


  template <class T>
  int serialPCA<T>::checkDim() {
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
  void serialPCA<T>::setDimension(int n) {
    parameters p(getParameters());
    p.resultDim=n;
    setParameters(p);
  }

  template <class T>
  bool serialPCA<T>::updateParameters() {
    bool boo=true;
    const parameters& p=getParameters();
    int dim = (p.autoDim) ? usedDimensionality : p.resultDim;

    if (dim <= 0) {
      dim=orderedEigVec.columns();
    }

    if (dim != transformMatrix.columns()) {
      transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);

      if (p.whitening) {
        whiteScale=eigValues;
        whiteScale.resize(dim,T(0),true,false);
        whiteScale.apply(sqrt);
      }
    }
    return boo;
  }

  template <class T>
  bool serialPCA<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix);
    return true;
  }

  template <class T>
  bool serialPCA<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec);
    return true;
  }

  template <class T>
  bool serialPCA<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec);
    return true;
  }

  template <class T>
  bool serialPCA<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues);
    return true;
  }

  template <class T>
  const matrix<T>&
  serialPCA<T>::getTransformMatrix() const {
    return transformMatrix;
  }

  template <class T>
  const matrix<T>&
  serialPCA<T>::getEigenVectors() const {
    return orderedEigVec;
  }

  template <class T>
  const vector<T>&
  serialPCA<T>::getEigenValues() const {
    return eigValues;
  }

  template <class T>
  bool serialPCA<T>::read(ioHandler& handler,
                                    const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      //cerr << "A";
      b=lti::read(handler,"parameters",param);
      setParameters(param);

      b=b && lti::read(handler, "stats", vstat);
      b=b && lti::read(handler, "isValid", isValid);

      //cerr << "B";
      b=b && lti::read(handler,"eigenVectors",orderedEigVec);
      //cerr << "C";

      transformMatrix.clear();

      b=b && lti::read(handler,"eigenValues",eigValues);
      //cerr << "D";

      b=b && lti::read(handler,"offset",offset);
      //cerr << "E";
      b=b && lti::read(handler,"scale",scale);
      //cerr << "F";

      int dim=min(checkDim(),orderedEigVec.columns());

      transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);

      if (getParameters().whitening) {
        whiteScale=eigValues;
        whiteScale.resize(dim,T(0),true,false);
        whiteScale.apply(sqrt);
      }

      if (complete) {
        b=b && handler.readEnd();
      }
      //cerr << "G";
    }
    return b;
  }

  template <class T>
  bool serialPCA<T>::write(ioHandler& handler,
                                     const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      b=lti::write(handler,"parameters",getParameters());
      b=b && lti::write(handler, "stats", vstat);
      b=b && lti::write(handler, "isValid", isValid);

      b=b && lti::write(handler,"eigenVectors",orderedEigVec);
      b=b && lti::write(handler,"eigenValues",eigValues);

      b=b && lti::write(handler,"offset",offset);
      b=b && lti::write(handler,"scale",scale);

      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }
}

#include "ltiUndebug.h"




