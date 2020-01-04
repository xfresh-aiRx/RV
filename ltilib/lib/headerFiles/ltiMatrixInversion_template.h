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


#include <limits>
#include "ltiEquationSystem.h"
#include "ltiSVD.h"
#include "ltiMath.h"

#undef min

#undef _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti {
  //--- matrixInversion ---
  //-----------------------


  // use std::numeric_limits<double>::epsilon() as epsilon value
  template <class T>
  const T matrixInversion<T>::my_epsilon = std::numeric_limits<T>::epsilon();


  template<class T>
  matrixInversion<T>::matrixInversion() {
    parameters tmp;
    setParameters(tmp);
  }


  // onCopy version of apply.
  template<class T>
  bool matrixInversion<T>::apply(const matrix<T>& theMatrix,
                                 matrix<T>& theInverse) const {
    int i;
    const int &n=theMatrix.rows();
    if(n!=theMatrix.columns()) throw incompatibleDimensionsException();

    const parameters& p=getParameters();

    if (p.method == parameters::LUD) {
      vector<T> unitVector(n,0.0);
      luSolution<T> eqs;
      typename luSolution<T>::parameters luSolPar;
      // do not copy (theMatrix could be huge!)
      // const_cast valid since luDecomp and luSol don't change
      // the SystemMatrix
      luSolPar.systemMatrix.useExternData(theMatrix.rows(),
                                          theMatrix.columns(),
                                          const_cast<T*>(&theMatrix.at(0,0)));

      eqs.setParameters(luSolPar);

      bool rc = true;

      theInverse.resize(n,n,false,false);
      for(i=0;i<n-1;i++) {
        unitVector[i]=1;
        rc = rc && eqs.apply(unitVector,theInverse[i]);
        unitVector[i]=0;
      }
      unitVector[i]=1;
      rc = rc && eqs.apply(unitVector,theInverse[i]);

      theInverse.transpose();
      
      if (!rc) {
        setStatusString("Error in luDecomposition used by matrixInversion:\n");
        appendStatusString(eqs);
      }

      return rc;
    } else if (p.method == parameters::SVD) {
      singularValueDecomp<T> svd;

      matrix<T> u;
      matrix<T> v;
      vector<T> w;

      if (svd.apply(theMatrix, u,w, theInverse)) {

        //inverse = v* 1./w * u' in Matlab/Octave parlance

        _lti_debug2("u:\n" << u << "\n");
        _lti_debug2("w:\n" << w << "\n");
        _lti_debug2("v:\n" << theInverse << "\n");
        
        _lti_debug3("epsilon: " << my_epsilon << "\n");

        u.transpose();
        bool isSingular = false;
        for (int i=0; i<u.rows(); i++) {
          T wi=0;
          if (i < w.size()) {
            wi=w.at(i);
          }
          _lti_debug3("wi: " << wi << "\n");
          if (closeToZero(wi, my_epsilon)) {
            wi= T(1)/my_epsilon;
            isSingular = true;
            setStatusString("matrix is singular in matrixInversion\n");
          } else {
            wi=T(1)/wi;
          }
          u.getRow(i).multiply(wi);
        }
        theInverse.multiply(u);
        return !isSingular;
      } else {
        setStatusString(svd.getStatusString());
        return false;
      }
    } else {
      setStatusString("Unknown inversion method!");
      return false;
    }

  }

  // onPlace version of apply.
  template<class T>
  bool matrixInversion<T>::apply(matrix<T>& theMatrix) const
  {
    matrix<T> tmp;
    bool r=apply(theMatrix,tmp);
    tmp.detach(theMatrix);
    return r;
  }

  template<class T>
  void matrixInversion<T>::useLUD() {
    parameters t;
    t.method=parameters::LUD;
    setParameters(t);
  }

  template<class T>
  void matrixInversion<T>::useSVD() {
    parameters t;
    t.method=parameters::SVD;
    setParameters(t);
  }

  template <class T>
  const typename matrixInversion<T>::parameters&
  matrixInversion<T>::getParameters() const {
    return
      dynamic_cast<const parameters&>(linearAlgebraFunctor::getParameters());
  }
}

#include "ltiUndebug.h"
