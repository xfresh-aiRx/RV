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

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiLinearRegression_template.h
 * authors ....: Holger Fillbrandt
 * organization: LTI, RWTH Aachen
 * creation ...: 4.7.2003
 * revisions ..: $Id: ltiLinearRegression_template.h,v 1.4 2004/05/03 12:20:01 alvarado Exp $
 */


#include "ltiEigenSystem.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // linearRegression::parameters
  // --------------------------------------------------

  // is in header file due to this ,%&,$& MSVC bug


  // --------------------------------------------------
  // linearRegression
  // --------------------------------------------------

  // default constructor
  template <class T>
  linearRegression<T>::linearRegression()
    : linearAlgebraFunctor() {

    parameters tmp;
    setParameters(tmp);
  }

  template <class T>
  linearRegression<T>::linearRegression(const parameters& par)
    : linearAlgebraFunctor() {
    setParameters(par);    

  }

  // copy constructor
  template <class T>
  linearRegression<T>::linearRegression(const linearRegression<T>& oth)
    : linearAlgebraFunctor() {
    copy(oth);
  }

  // destructor
  template <class T>
  linearRegression<T>::~linearRegression() {
  }

  // returns the name of this type
  template <class T>
  const char* linearRegression<T>::getTypeName() const {
    return "linearRegression";
  }

  // copy member
  template <class T>
  linearRegression<T>&
  linearRegression<T>::copy(const linearRegression<T>& other) {
    linearAlgebraFunctor::copy(other);

    linRegMatrix.copy(other.linRegMatrix);

    return (*this);
  }

  template <class T>
  linearRegression<T>&
  linearRegression<T>::operator=(const linearRegression<T>& other) {
    return copy(other);
  }

  // clone member
  template <class T>
  functor* linearRegression<T>::clone() const {
    return new linearRegression<T>(*this);
  }

  // return parameters
  template <class T>
  const typename linearRegression<T>::parameters&
  linearRegression<T>::getParameters() const {
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

  /*
	 * The linear regression algorithm. It calculates a linear transformation
	 * matrix linRegMatrix, that approximately fulfills
   * destMatrix = linRegMatrix * srcMatrix.
	 * @param srcMatrix the columns of this matrix are the source training vectors of dimension srcDim
	 * @param destMatrix the columns of this matrix are the destination training vectors of dimension destDim
	 * @return resulting (destDim x srcDim) transformation matrix. This is also kept in the parameter linRegMatrix and can be used directly with "transform"
   */
  template <class T>
  matrix<T> linearRegression<T>::apply(matrix<T>& srcMatrix, matrix<T>& destMatrix) {

    const parameters pars = getParameters();
 
	  matrix <T> eigenVectorMatrix;
	  vector<T>  eigenValues;
	  matrix <T> covar;

	  point srcSize  = srcMatrix.size();
	  point destSize = destMatrix.size();

	  if (srcSize.x != destSize.x) return matrix<T>();

    _lti_debug("calculate matrix...");
	  covar = srcMatrix;
	  covar.transpose();
	  covar.multiply(srcMatrix); // "covariance" matrix srcMatrix^T * srcMatrix

	  // calculate the eigenvectors of the covariance matrix, sorted by the
	  // magnitude of the corresponding eigenvalues
    // if the LAPACK library is used apply fastEigenSystem, otherwise
    // apply the slower ltilib jacobi functor 
    _lti_debug("calculate eigensystem...");
#ifdef HAVE_LAPACK
    _lti_debug("use LAPACK: fastEigenSystem for calculating the eigensystem");
    fastEigenSystem<T> theEigenSystem;
    typename fastEigenSystem<T>::parameters theEigenSystemPar;
    theEigenSystemPar.dimensions = pars.eigenSystemDim;
    theEigenSystem.setParameters(theEigenSystemPar);
    theEigenSystem.apply(covar, eigenValues, eigenVectorMatrix);
#else
    _lti_debug("use ltilib jacobi functor for calculating the eigensystem");
    jacobi<T> theEigenSystem;
    typename jacobi<T>::parameters theEigenSystemPar;
    theEigenSystemPar.sort = true;
    theEigenSystem.setParameters(theEigenSystemPar);
    theEigenSystem.apply(covar, eigenValues, eigenVectorMatrix);
	  // keep only the (eigenSystemDim) eigenvectors with the largest eigenvalues
	  eigenValues.resize(pars.eigenSystemDim, 0.0f,true, false);
	  eigenVectorMatrix.resize(srcSize.x, pars.eigenSystemDim,0.0f,true,false);
#endif
 
    _lti_debug("calculate linear regression matrix...");

	  // (diag): diagonal matrix of the inverted eigenvalues
    matrix<T> diag(pars.eigenSystemDim, pars.eigenSystemDim, 0.0f);
    for (int i=0; i < pars.eigenSystemDim; i++) {
		if (eigenValues.at(i) == 0.0f) {
		    diag.at(i,i) = 0.0f;
		  } else {
	      diag.at(i,i) = 1.0f / eigenValues.at(i);
		  }
	  }

	  // calculate linear regression matrix
	  matrix<T> resultMatrix;
    srcMatrix.transpose();
    eigenVectorMatrix.transpose();
    resultMatrix.multiply(eigenVectorMatrix, srcMatrix);
    diag.multiply(resultMatrix);
    eigenVectorMatrix.transpose();
    eigenVectorMatrix.multiply(diag);
    resultMatrix.multiply(destMatrix, eigenVectorMatrix);
	  linRegMatrix.copy(resultMatrix);

    srcMatrix.transpose();
	  return resultMatrix;
 
  }


  /*
	 * multiplication of srcVector with the linear regression matrix
   * of dimension (destDim x srcDim)
   * @param srcVector source vector of dimension srcDim
   * @param destVector destination vector of dimension destDim
   */
  template <class T>
  void linearRegression<T>::transform(vector<T>& srcVector, vector<T>& destVector) const {
	  linRegMatrix.multiply(srcVector, destVector);
  }


  /* 
	 * multiplication of srcVector with the linear regression matrix
   * of dimension (destDim x srcDim)  
   * @param srcVector source vector of dimension srcDim
   * @return destination vector of dimension destDim
   */
  template <class T>
  vector<T> linearRegression<T>::transform(vector<T>& srcVector) const {
	  vector<T> resVector;
	  linRegMatrix.multiply(srcVector, resVector);
	  return resVector;
  }


  /*
   * get linear regression matrix
   */
  template <class T>
  matrix<T> linearRegression<T>::getLinRegMatrix() const {

	  return linRegMatrix;
  }

  /*
   * set linear regression matrix
   */
  template <class T>
  void linearRegression<T>::setLinRegMatrix(matrix<T> &linRegMatrixSet) {

    linRegMatrix = linRegMatrixSet;
  }



  template <class T>
  bool linearRegression<T>::read(ioHandler& handler,
                                    const bool complete) {
    bool b=true;

    if (complete) {
      b=handler.readBegin();
    }

    if (b) {
      parameters param;

      lti::read(handler,"parameters",param);
      setParameters(param);

      lti::read(handler,"linRegMatrix",linRegMatrix);

    }
    return b;
  }

  template <class T>
  bool linearRegression<T>::write(ioHandler& handler,
                                     const bool complete) const {
    bool b=true;

    if (complete) {
      b=handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"parameters",getParameters());
      lti::write(handler,"linRegMatrix", linRegMatrix);
 
      if (complete) {
        b=b && handler.writeEnd();
      }
    }
    return b;
  }
}

#include "ltiUndebug.h"



