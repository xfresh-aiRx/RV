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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiCholeskyDecomposition.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 7.7.2003
 * revisions ..: $Id: ltiCholeskyDecomposition_template.h,v 1.5 2004/08/20 12:47:18 doerfler Exp $
 */


#include "ltiCholeskyDecomposition.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 4
#include "ltiDebug.h"
#include <limits>


namespace lti {
  // --------------------------------------------------
  // choleskyDecomposition::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // choleskyDecomposition
  // --------------------------------------------------

  // default constructor
  template<class T>
  choleskyDecomposition<T>::choleskyDecomposition()
    : linearAlgebraFunctor(){

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template<class T>
  choleskyDecomposition<T>::choleskyDecomposition(const parameters& par)
    : linearAlgebraFunctor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  choleskyDecomposition<T>::choleskyDecomposition(const choleskyDecomposition<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template<class T>
  choleskyDecomposition<T>::~choleskyDecomposition() {
  }

  // returns the name of this type
  template<class T>
  const char* choleskyDecomposition<T>::getTypeName() const {
    return "choleskyDecomposition<T>";
  }

  // copy member
  template<class T>
  choleskyDecomposition<T>&
  choleskyDecomposition<T>::copy(const choleskyDecomposition<T>& other) {
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
  choleskyDecomposition<T>&
  choleskyDecomposition<T>::operator=(const choleskyDecomposition<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* choleskyDecomposition<T>::clone() const {
    return new choleskyDecomposition<T>(*this);
  }

  // return parameters
  template<class T>
  const typename choleskyDecomposition<T>::parameters&
  choleskyDecomposition<T>::getParameters() const {
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

  
  // On place apply for type matrix<T>!
  template<class T>
  bool choleskyDecomposition<T>::apply(matrix<T>& a) const {
    return apply(a, getParameters().triangularType);
  }

  template<class T>
  bool choleskyDecomposition<T>::apply(matrix<T>& a,
                                       const typename parameters::eTriangularType& tType) const {

    
    
    const int sz=a.rows();
    int i,j,k;

    _lti_debug2("input matrix\n" << a << "\n");
    
    if (tType==parameters::Upper) {
      
      _lti_debug2("Upper");

      for (k=0; k<sz; ++k) {
        if (a.at(k,k)<std::numeric_limits<T>::epsilon()) {
          setStatusString("Matrix is not positiv definite\n");
          a.clear();
          return false;
        }
        a.at(k,k)=sqrt(a.at(k,k));
        for (i=k+1; i<sz; ++i) {
          a.at(k,i)/=a.at(k,k);
        }
        for (j=k+1; j<sz; ++j) {
          for (i=j; i<sz; ++i) {
            a.at(j,i)-=a.at(k,i)*a.at(k,j);
          }
        }
        _lti_debug4("matrix at k=" << k << "\n" << a << "\n");
      }
      
      //delete lower triangle
      for (i=0; i<sz; i++) {
        for (j=i+1; j<sz; j++) {
          a.at(j,i)=T(0);
        }
      }
      
      _lti_debug2("result matrix\n" << a << "\n");

      return true;
      
    } else if (tType==parameters::Lower) {
      
      _lti_debug2("Lower");

      for (k=0; k<sz; ++k) {
        if (a.at(k,k)<std::numeric_limits<T>::epsilon()) {
          setStatusString("Matrix is not positiv definite\n");
          a.clear();
          return false;
        }
        a.at(k,k)=sqrt(a.at(k,k));
        for (i=k+1; i<sz; ++i) {
          a.at(i,k)/=a.at(k,k);
        }
        for (j=k+1; j<sz; ++j) {
          for (i=j; i<sz; ++i) {
            a.at(i,j)-=a.at(i,k)*a.at(j,k);
          }
        }
      }
      
      //delete upper  triangle
      for (i=0; i<sz; i++) {
        for (j=i+1; j<sz; j++) {
          a.at(i,j)=T(0);
        }
      }
      
      _lti_debug2("result matrix\n" << a << "\n");

      return true;

    } else {

      setStatusString("Illegal value of triangularType");
      a.clear();
      return false;
    }

  };

  // On copy apply for type matrix<T>!
  template<class T>
  bool choleskyDecomposition<T>::apply(const matrix<T>& src,matrix<T>& dest,
                                       const typename parameters::eTriangularType& tType) const {
    dest.copy(src);
    return apply(dest,tType);

  };

  template<class T>
  bool choleskyDecomposition<T>::apply(const matrix<T>& src,matrix<T>& dest) const {
    dest.copy(src);
    return apply(dest,getParameters().triangularType);

  };



}

#include "ltiUndebug.h"
