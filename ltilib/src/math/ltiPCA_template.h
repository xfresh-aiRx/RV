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
 * authors ....: Jochen Wickel, Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.11.2000
 * revisions ..: $Id: ltiPCA_template.h,v 1.53 2005/09/23 15:03:58 haehnel Exp $
 */

#include "ltiEigenSystem.h"
#include "ltiMatrixInversion.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // principalComponents::parameters
  // --------------------------------------------------

  // is in header file due to this ,%&,$& MSVC bug


  // --------------------------------------------------
  // principalComponents
  // --------------------------------------------------

  // default constructor
  template <class T>
  principalComponents<T>::principalComponents(const bool createDefParam)
    : linearAlgebraFunctor() {
    if (createDefParam) {
      parameters tmp;
      setParameters(tmp);
    }
    usedDimensionality=0;
  }

  template <class T>
  principalComponents<T>::principalComponents(const parameters& par)
    : linearAlgebraFunctor() {
    setParameters(par);    

    usedDimensionality=0;
  }

  // copy constructor
  template <class T>
  principalComponents<T>::principalComponents(const principalComponents<T>& oth)
    : linearAlgebraFunctor() {
    copy(oth);
  }

  // destructor
  template <class T>
  principalComponents<T>::~principalComponents() {
  }

  // returns the name of this type
  template <class T>
  const char* principalComponents<T>::getTypeName() const {
    return "principalComponents";
  }

  // copy member
  template <class T>
  principalComponents<T>&
  principalComponents<T>::copy(const principalComponents<T>& other) {
    linearAlgebraFunctor::copy(other);

    orderedEigVec.copy(other.orderedEigVec);
    transformMatrix.copy(other.transformMatrix);
    eigValues.copy(other.eigValues);

    offset.copy(other.offset);
    scale.copy(other.scale);
    whiteScale.copy(other.whiteScale);

    return (*this);
  }

  template <class T>
  principalComponents<T>&
  principalComponents<T>::operator=(const principalComponents<T>& other) {
    return copy(other);
  }

  // clone member
  template <class T>
  functor* principalComponents<T>::clone() const {
    return new principalComponents<T>(*this);
  }

  // return parameters
  template <class T>
  const typename principalComponents<T>::parameters&
  principalComponents<T>::getParameters() const {
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
  bool principalComponents<T>::apply(const matrix<T>& src,
                                     matrix<T>& dest) {
    return computeTransformMatrix(src) && transform(src,dest);
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool principalComponents<T>::computeTransformMatrix(const matrix<T>& src) {

    if (src.empty()) {
      setStatusString("data matrix is empty");
      reset();
      return false;
    }


    const parameters& param = getParameters();
    int i;

    varianceFunctor<T> var;
    meansFunctor<T> mean;

		// determine the center of the data point distribution
    mean.meanOfRows(src,offset);

    if (!param.useSVD) {
      if (param.eigen==0) {
        setStatusString("pointer to eigensystem in parameters is null");
        reset();
        return false;
      }

      eigenSystem<T> &eig=*param.eigen;

      _lti_debug("EIG is " << eig.getTypeName() << "\n");

      _lti_debug("pA");
      int cols=src.columns();

      // correlation coefficient matrix
      matrix<T> cc(cols,cols);
      _lti_debug("pB");

      // compute correlation coefficients of data matrix
      typename varianceFunctor<T>::parameters vp;
      vp.correlation=param.useCorrelation;
      _lti_debug("cvA");
      var.setParameters(vp);
      _lti_debug("cvB");
      var.covarianceMatrixOfRows(src,cc);

      _lti_debug("pC");

      // set the sort-flag of the eigenvector functor
      if (!eig.getParameters().sort) {
        // need clone because we do not know the actual class of the
        // eigenvector instance
        typename eigenSystem<T>::parameters* ep=
          dynamic_cast<typename eigenSystem<T>::parameters*>(eig.getParameters().clone());
        ep->sort=true;
        eig.setParameters(*ep);
        delete ep;
      }

      _lti_debug("pD");

      // now compute eigenvectors of correlation coefficient matrix
      if (!eig.apply(cc,eigValues,orderedEigVec)) {
        _lti_debug("pX");
        setStatusString(eig.getStatusString());
        reset();
        return false;
      }

    } else {   //useSVD

      if (param.svd==0) {
        setStatusString("pointer to SVD in parameters is null");
        reset();
        return false;
      }

      // set the sort-flag of the svd functor
      if (!param.svd->getParameters().sort) {
        // need clone because we do not know the actual class of the
        // eigenvector instance
        typename singularValueDecomp<T>::parameters* sp=
          dynamic_cast<typename singularValueDecomp<T>::parameters*>(param.svd->getParameters().clone());
        sp->sort=true;
        param.svd->setParameters(*sp);
        delete sp;
      }

      _lti_debug("sort after cast is:"<<param.svd->getParameters().sort<<std::endl);


      int m=src.rows();
//       singularValueDecomp<T> &svd=*param.svd;

#ifdef HAVE_LAPACK
      int n=src.columns();

      // if fastSVD is used configure so that only those values that are
      // needed are calculated
      functor::parameters* sp=param.svd->getParameters().clone();
      typename fastSVD<T>::parameters* psvd=
        dynamic_cast<typename fastSVD<T>::parameters*>(sp);
      if (psvd!=0) {
        if (param.autoDim) {
          psvd->useMinDimensions=true;
        } else if (param.resultDim<=m && m<=n) {
          psvd->useMinDimensions=true;
        } else if (n<m) {
          psvd->useMinDimensions=true;
        } else {
          psvd->useMinDimensions=false;
        }
        param.svd->setParameters(*psvd);
      }
      delete sp;
#endif

      matrix<T> tmp(src);
      for (i=0; i<m; i++) {
        tmp.getRow(i).subtract(offset);
      }
      param.svd->apply(tmp,eigValues,orderedEigVec);

      eigValues.apply(lti::sqr);
      eigValues.divide(static_cast<T>(m-1));

      _lti_debug(eigValues << std::endl);

      tmp.clear();
    }

    if (param.useCorrelation) {
      // now normalize the data matrix to zero mean and unit variance
      var.varianceOfRows(src,scale);
      scale.apply(sqrt);
      // mask columns with zero variance
      for (typename vector<T>::iterator si=scale.begin();
           si != scale.end(); si++) {
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
    _lti_debug("transform: " << transformMatrix.size() << std::endl);
    _lti_debug("pF");

    if (param.whitening) {
      whiteScale=eigValues;
      whiteScale.resize(dim,T(0),true,false);
      whiteScale.apply(sqrt);
    }

    if (param.centerData) {
      transformedOffset.resize(dim);
      transformedOffset.fill(T(0));
    } else {
      transformMatrix.leftMultiply(offset,transformedOffset);
    }
    return true;
  }

  template <class T>
  bool principalComponents<T>::train(const matrix<T>& src) {
    return computeTransformMatrix(src);
  }


  //=================================================================
  // Set the covariance matrix and the means directly
  template <class T>
  bool principalComponents<T>::setCovarianceAndMean(const matrix<T>& coVar,
                                                    const vector<T>& meanVec) {
    const parameters& param = getParameters();

    eigenSystem<T> &eig=*param.eigen;

    _lti_debug("EIG is " << eig.getTypeName() << "\n");
    _lti_debug("pA");

    const int cols=coVar.columns(); // src.columns();

    if (meanVec.size() != cols) {
      setStatusString("Size mismatch of input in setCovariance and Mean");
      return false;
    }
    // set the sort-flag of the eigenvector functor
    if (!eig.getParameters().sort) {
      // need clone because we do not know the actual class of the
      // eigenvector instance
      typename eigenSystem<T>::parameters* ep =
        dynamic_cast<typename eigenSystem<T>::parameters*>(eig.getParameters().clone());
      ep->sort=true;
      eig.setParameters(*ep);
      delete ep;
    }

    _lti_debug("pD");

    // now compute eigenvectors of correlation coefficient matrix
    if (!eig.apply(coVar,eigValues,orderedEigVec)) {
      _lti_debug("pX");
      setStatusString(eig.getStatusString());
      usedDimensionality=0;
      eigValues.resize(0);
      orderedEigVec.resize(0,0);
      return false;
    }

    offset = meanVec;

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
    _lti_debug("transform: " << transformMatrix.size() << std::endl);
    _lti_debug("pF");

    if (param.whitening) {
      whiteScale=eigValues;
      whiteScale.resize(dim,T(0),true,false);
      whiteScale.apply(sqrt);
    }

    return true;
  }

  /*
   * Transforms a single vector according to a previously computed
   * transformation matrix.
   */
  template <class T>
  bool principalComponents<T>::transform(const vector<T>& src,
                                               vector<T>& result) const {

    const parameters& p = getParameters();

    vector<T> tmp;

    if (offset.size() != src.size()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    tmp.subtract(src,offset);
    if (p.useCorrelation) {
      tmp.edivide(scale);
    }
    _lti_debug("e");

    transformMatrix.leftMultiply(tmp,result);
    if (p.whitening) {
      result.edivide(whiteScale);
    }
    if (!p.centerData) {
      result.add(transformedOffset);
    }
    _lti_debug("f\n");

    return true;
  }

  /*
   * Transform an entire matrix according to a previously computed
   * transformation matrix. Unfortunately, we must choose a name
   * different from apply.
   * @param src the data matrix
   * @param result the matrix which will receive the transformed data
   * @return a reference to <code>result</code>
   */
  template <class T>
  bool principalComponents<T>::transform(const matrix<T> &src,
                                               matrix<T>& result) const {
    const parameters& p = getParameters();
    if (offset.size() != src.columns()) {
      setStatusString("Transformation matrix and input data incompatible.");
      return false;
    }

    int i;
    result.resize(src.size(),T(),false,false);
    _lti_debug("Transform Matrix: " << transformMatrix << std::endl);
    _lti_debug("PCA: correlation " << scale <<
               (p.useCorrelation ? " (on)" : " (off)") << std::endl);
    for (i=0; i<result.rows(); i++) {
      result.getRow(i).subtract(src.getRow(i),offset);
      if (p.useCorrelation) {
        result.getRow(i).edivide(scale);
      }
    }

    result.multiply(transformMatrix);
    if (p.whitening) {
      _lti_debug("PCA: whiteScale " << whiteScale << std::endl);
      for (i=0; i<result.rows(); i++) {
        result.getRow(i).edivide(whiteScale);
        _lti_if_debug4(if (i%2312==0) {std::cerr <<src.getRow(i) << " -> " << result.getRow(i) << std::endl;});
      }
    }
    if (!p.centerData) {
      for (i=0; i<result.rows(); i++) {
        result.getRow(i).add(transformedOffset);
      }
    }

    return true;
  }

  template <class T>
  bool principalComponents<T>::transform(matrix<T>& srcdest) const {
    matrix<T> tmp;
    if (transform(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }

  template <class T>
  bool principalComponents<T>::apply(matrix<T>& srcdest) {
    matrix<T> tmp;
    if (apply(srcdest,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    return false;
  }


  template <class T>
  bool principalComponents<T>::reconstruct(const vector<T>& coeff, 
                                                 vector<T>& dest) const {
    assert(coeff.size()==transformMatrix.columns());

    transformMatrix.multiply(coeff,dest);
    dest.add(offset);
    return true;
  }

  template <class T>
  bool principalComponents<T>::reconstruct(const matrix<T>& coeff, 
                                                 matrix<T>& dest) const {
   
    bool rc=true;
    dest.resize(coeff.rows(), transformMatrix.rows(), T(0), false, false);
    for (int i=0; i<coeff.rows(); i++) {
      rc = rc && reconstruct(coeff.getRow(i), dest.getRow(i));
    }
    return rc;
  }

  template <class T>
  int principalComponents<T>::checkDim() {
    const parameters& p=getParameters();
    if (p.autoDim) {
      _lti_debug("cdA");
      int n=0;
      if (eigValues.size() > 0) {
        typename vector<T>::const_iterator i=eigValues.begin();
        // set the lowest accepted eigenvalue to the largest eigenvalue
        // divided by 1e6. This works only, if eigValues.at(0) is really
        // the maximum, i.e. if the eigenvalues are sorted descendingly
        // maybe 1e6 could be enlarged to something around 1e10
        T evLimit=abs(*i)/p.relevance;
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
      _lti_debug("cdP");
      usedDimensionality = p.resultDim;
    }
    return usedDimensionality;
  }

  template <class T>
  void principalComponents<T>::reset() {
    usedDimensionality=0;
    eigValues.clear();
    orderedEigVec.clear();
    transformMatrix.clear();
    offset.clear();
    transformedOffset.clear();
    scale.clear();
    whiteScale.clear();
  }

  template <class T>
  void principalComponents<T>::setDimension(int n) {
    parameters p(getParameters());
    p.resultDim=n;
    setParameters(p);
  }

  template <class T>
  bool principalComponents<T>::updateParameters() {

    bool boo=true;
    const parameters& p=getParameters();
    int dim = (p.autoDim) ? usedDimensionality : p.resultDim;

    if (dim <= 0) {
      dim=orderedEigVec.columns();
    }

    if (dim != transformMatrix.columns()) {
      transformMatrix.copy(orderedEigVec,0,MaxInt32,0,dim-1);

      if (p.whitening) {
        whiteScale.copy(eigValues);
        whiteScale.resize(dim,T(0),true,true); // avoid applying sqrt to
                                               // uninitializied data.
                                               // Valgrind detect this as
                                               // error.
        whiteScale.apply(sqrt);
      }
    }

#ifdef HAVE_LAPACK
    if (p.eigen!=0) {
      functor::parameters* ep=p.eigen->getParameters().clone();
      typename fastEigenSystem<T>::parameters* fep=
        dynamic_cast<typename fastEigenSystem<T>::parameters*>(ep);
      if (fep!=0) {
        if (p.autoDim) {
          fep->dimensions=0;
          p.eigen->setParameters(*fep);
        } else {
          fep->dimensions=p.resultDim;
          p.eigen->setParameters(*fep);
        }
      }
      delete ep;
    }


#endif


    return boo;
  }

  template <class T>
  bool principalComponents<T>::getTransformMatrix(matrix<T>& result) const {
    result.copy(transformMatrix);
    return true;
  }

  template <class T>
  const matrix<T>&
  principalComponents<T>::getTransformMatrix() const {
    return transformMatrix;
  }

  template <class T>
  bool principalComponents<T>::getOffsetVector(vector<T>& result) const {
    result.copy(offset);
    return true;
  }

  template <class T>
  const vector<T>&
  principalComponents<T>::getOffsetVector() const {
    return offset;
  }

  template <class T>
  bool principalComponents<T>::getEigenVectors(matrix<T>& result) const {
    result.copy(orderedEigVec);
    return true;
  }

  template <class T>
  bool principalComponents<T>::getEigenVectorsInRows(matrix<T>& result) const {
    result.transpose(orderedEigVec);
    return true;
  }

  template <class T>
  bool principalComponents<T>::getEigenValues(vector<T>& result) const {
    result.copy(eigValues);
    return true;
  }

  template <class T>
  const matrix<T>&
  principalComponents<T>::getEigenVectors() const {
    return orderedEigVec;
  }

  template <class T>
  const vector<T>&
  principalComponents<T>::getEigenValues() const {
    return eigValues;
  }

  template <class T>
  bool principalComponents<T>::read(ioHandler& handler,
                                    const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;
      //cerr << "A";
      lti::read(handler,"parameters",param);
      setParameters(param);
      //cerr << "B";
      lti::read(handler,"eigenVectors",orderedEigVec);
      //cerr << "C";

      transformMatrix.clear();

      lti::read(handler,"eigenValues",eigValues);
      //cerr << "D";

			lti::read(handler,"transformedOffset",transformedOffset);

      lti::read(handler,"offset",offset);
      //cerr << "E";
      lti::read(handler,"scale",scale);
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
  bool principalComponents<T>::write(ioHandler& handler,
                                     const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"parameters",getParameters());
      lti::write(handler,"eigenVectors",orderedEigVec);
      lti::write(handler,"eigenValues",eigValues);
			lti::write(handler,"transformedOffset",transformedOffset);

      lti::write(handler,"offset",offset);
      lti::write(handler,"scale",scale);

      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }
}

#include "ltiUndebug.h"



