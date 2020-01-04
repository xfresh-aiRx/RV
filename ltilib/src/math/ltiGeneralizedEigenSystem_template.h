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
 * file .......: ltiGeneralizedEigenSystem.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 10.7.2003
 * revisions ..: $Id: ltiGeneralizedEigenSystem_template.h,v 1.4 2004/05/06 10:31:26 alvarado Exp $
 */

#include "ltiGeneralizedEigenSystem.h"

namespace lti {
  // --------------------------------------------------
  // generalizedEigenSystem::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // generalizedEigenSystem
  // --------------------------------------------------

  // default constructor
  template<class T>
  generalizedEigenSystem<T>::generalizedEigenSystem()
    : linearAlgebraFunctor(), cholesky(), fSubst(), bSubst(), simpleEigen() {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template<class T>
  generalizedEigenSystem<T>::generalizedEigenSystem(const parameters& par)
    : linearAlgebraFunctor(), cholesky(), fSubst(), bSubst(), simpleEigen() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  generalizedEigenSystem<T>::generalizedEigenSystem(const generalizedEigenSystem<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template<class T>
  generalizedEigenSystem<T>::~generalizedEigenSystem() {
  }

  // returns the name of this type
  template<class T>
  const char* generalizedEigenSystem<T>::getTypeName() const {
    return "generalizedEigenSystem<T>";
  }

  // copy member
  template<class T>
  generalizedEigenSystem<T>&
  generalizedEigenSystem<T>::copy(const generalizedEigenSystem<T>& other) {
      linearAlgebraFunctor::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.
      cholesky.copy(other.cholesky);
      fSubst.copy(other.fSubst);

    return (*this);
  }

  // alias for copy member
  template<class T>
  generalizedEigenSystem<T>&
  generalizedEigenSystem<T>::operator=(const generalizedEigenSystem<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* generalizedEigenSystem<T>::clone() const {
    return new generalizedEigenSystem<T>(*this);
  }

  // return parameters
  template<class T>
  const typename generalizedEigenSystem<T>::parameters&
  generalizedEigenSystem<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  /* sets the functor's parameters.
     The functor keeps its own copy of the given parameters.*/
  template<class T>
  bool generalizedEigenSystem<T>::updateParameters() {
    bool b=true;
    typename jacobi<T>::parameters eP;
    const parameters& param=getParameters();
    eP.sort=param.sort;
    eP.dimensions=param.dimensions;
    b = b && simpleEigen.setParameters(eP);
    return b;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  template <class T>
  bool generalizedEigenSystem<T>::apply(const matrix<T>& a,
                                        const matrix<T>& b,
                                        vector<T>& eigenvalues) const
  {

    //calculate the cholesky decomposition u of b
    matrix<T> u;
    if (!cholesky.apply(b,u,triangularMatrixType::Upper)) {
      eigenvalues.clear();
      setStatusString(cholesky.getStatusString());
      return false;
    }

    //calculate c=u^-1'*a*u^-1
    //Solve first part of linear problem
    matrix<T> y;
    if (!fSubst.apply(u,a,y,triangularMatrixType::Upper)) {
      eigenvalues.clear();
      setStatusString(fSubst.getStatusString());
      return false;
    }
    //Solve second part of the linear problem
    matrix<T> c;
    u.transpose();
    if (!fSubst.apply(u,y,c,triangularMatrixType::Lower)) {
      eigenvalues.clear();
      setStatusString(fSubst.getStatusString());
      return false;
    }
    //now calculate eigenvalues of c which are the result
    //y is just a dummy here
    if (!simpleEigen.apply(c,eigenvalues,y)) {
      eigenvalues.clear();
      setStatusString(simpleEigen.getStatusString());
      return false;
    }

    return true;
  }

  template <class T>
  bool generalizedEigenSystem<T>::apply(const matrix<T>& a,
                                        const matrix<T>& b,
                                        vector<T>& eigenvalues,
                                        matrix<T>& eigenvectors) const {
    //calculate the cholesky decomposition u of b
    matrix<T> u;
    if (!cholesky.apply(b,u,triangularMatrixType::Upper)) {
      eigenvalues.clear();
      eigenvectors.clear();
      setStatusString(cholesky.getStatusString());
      return false;
    }

    //calculate c=u^-1'*a*u^-1
    //Solve first part of linear problem
    matrix<T> y;
    if (!fSubst.apply(u,a,y,triangularMatrixType::Upper)) {
      eigenvalues.clear();
      eigenvectors.clear();
      setStatusString(fSubst.getStatusString());
      return false;
    }
    //Solve second part of the linear problem
    matrix<T> c;
    matrix<T> l; //the transpose of u
    l.transpose(u);
    if (!fSubst.apply(l,y,c,triangularMatrixType::Lower)) {
      eigenvalues.clear();
      eigenvectors.clear();
      setStatusString(fSubst.getStatusString());
      return false;
    }
    // l is no longer needed
    l.clear();

    //now calculate eigenvalues of c which are final
    //and the eigenvalues y which are temp
    if (!simpleEigen.apply(c,eigenvalues,y)) {
      eigenvalues.clear();
      eigenvectors.clear();
      setStatusString(simpleEigen.getStatusString());
      return false;
    }

    //now get the eigenvectors v from: u*x=y
    if (!bSubst.apply(u,y,eigenvectors,triangularMatrixType::Upper)) {
      eigenvalues.clear();
      eigenvectors.clear();
      setStatusString(bSubst.getStatusString());
      return false;
    }
    return true;
  }

}

