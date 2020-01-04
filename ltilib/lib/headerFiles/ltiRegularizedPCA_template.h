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
 * revisions ..: $Id: ltiRegularizedPCA_template.h,v 1.6 2003/02/26 08:35:59 alvarado Exp $
 */

#include <fstream>
#include "ltiEigenSystem.h"
#include "ltiMatrixInversion.h"
#include "ltiMeansFunctor.h"
#include "ltiVarianceFunctor.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // regularizedPCA::parameters
  // --------------------------------------------------

  // is in header file due to this ,%&,$& MSVC bug


  // --------------------------------------------------
  // regularizedPCA
  // --------------------------------------------------

  // default constructor
  template <class T>
  regularizedPCA<T>::regularizedPCA()
    : principalComponents<T>(){
    parameters tmp;
    setParameters(tmp);
  }

  // copy constructor
  template <class T>
  regularizedPCA<T>::regularizedPCA(const regularizedPCA& oth) {
    copy(oth);
  }

  // destructor
  template <class T>
  regularizedPCA<T>::~regularizedPCA() {
  }


  template <class T>
  int regularizedPCA<T>::checkDim() {
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
      _lti_debug("Resultdim: " << n << endl);
    } else {
      _lti_debug("cdP");
      usedDimensionality = p.resultDim;
    }
    if (p.kappa > 0) {
      T v=p.kappa*eigValues[usedDimensionality];
      for (int i=usedDimensionality; i<eigValues.size(); i++) {
        eigValues[i]=v;
      }
      usedDimensionality=eigValues.size();
    }
    return usedDimensionality;
  }

  // clone member
  template <class T>
  functor* regularizedPCA<T>::clone() const {
    return new regularizedPCA<T>(*this);
  }

  // return parameters
  template <class T>
  const typename regularizedPCA<T>::parameters&
    regularizedPCA<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }



}

#include "ltiUndebug.h"


