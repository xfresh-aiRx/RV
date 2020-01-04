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
 * file .......: ltiUnifiedSVD.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 18.11.2003
 * revisions ..: $Id: ltiUnifiedSVD_template.h,v 1.5 2005/06/16 12:37:35 doerfler Exp $
 */

//TODO: include files
#include "ltiUnifiedSVD.h"

#ifdef HAVE_LAPACK
#include "ltiFastSVD.h"
#endif

namespace lti {
  // --------------------------------------------------
  // unifiedSVD::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // unifiedSVD
  // --------------------------------------------------

  // default constructor
  template<class T>
  unifiedSVD<T>::unifiedSVD()
    : linearAlgebraFunctor(), svd(0) {

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template<class T>
  unifiedSVD<T>::unifiedSVD(const parameters& par)
    : linearAlgebraFunctor(), svd(0) {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  unifiedSVD<T>::unifiedSVD(const unifiedSVD<T>& other)
    : linearAlgebraFunctor(), svd(0) {
    copy(other);
  }

  // destructor
  template<class T>
  unifiedSVD<T>::~unifiedSVD() {
    delete svd;
  }

  // returns the name of this type
  template<class T>
  const char* unifiedSVD<T>::getTypeName() const {
    return "unifiedSVD<T>";
  }

  // copy member
  template<class T>
  unifiedSVD<T>& unifiedSVD<T>::copy(const unifiedSVD<T>& other) {
    linearAlgebraFunctor::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class T>
  unifiedSVD<T>&
  unifiedSVD<T>::operator=(const unifiedSVD<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* unifiedSVD<T>::clone() const {
    return new unifiedSVD<T>(*this);
  }

  // return parameters
  template<class T>
  const typename unifiedSVD<T>::parameters&
  unifiedSVD<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool unifiedSVD<T>::updateParameters() {

    bool b=true;
    const parameters& theParam = getParameters();

#ifdef HAVE_LAPACK
    switch (theParam.svdType) {
      case parameters::SVD: {
        svd=new singularValueDecomp<T>();
        typename singularValueDecomp<T>::parameters p;
        p.sort = theParam.sort;
        p.transposeU = theParam.transposeU;
        p.transposeV = theParam.transposeV;
        b = b && svd->setParameters(p);
        break;
      }
      case parameters::FastSVD: {
        svd=new fastSVD<T>();
        typename fastSVD<T>::parameters p;
        p.sort = theParam.sort;
        p.transposeU = theParam.transposeU;
        p.transposeV = theParam.transposeV;
        p.useDC = theParam.useDC;
        p.useMinDimensions = theParam.useMinDimensions;
        b = b && svd->setParameters(p);
        break;
      }
    }
#else
    switch (theParam.svdType) {
      case parameters::FastSVD:
        setStatusString("Illegal value of parameter svdType of unifiedSVD. fastSVD can only be used if LAPACK is available. Using singularValueDecomp.\n");
        b=false;
      case parameters::SVD: {
        svd=new singularValueDecomp<T>();
        typename singularValueDecomp<T>::parameters p;
        p.sort = theParam.sort;
        p.transposeU = theParam.transposeU;
        p.transposeV = theParam.transposeV;
        b = b && svd->setParameters(p);
        break;
      }
    }
#endif

    return b;
  }

  template<class T>
  bool unifiedSVD<T>::read(ioHandler& handler,
                           const bool complete) {

    // have to call setParameters with the just loaded parameters to
    // ensure correct allocation of svd
    bool b=linearAlgebraFunctor::read(handler,complete);
    b = b && setParameters(getParameters());
    return b;

  }


}

