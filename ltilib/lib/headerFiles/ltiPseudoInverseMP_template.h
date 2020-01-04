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
 * file .......: ltiPseudoInverseMP_template.h
 * authors ....: Peter Dörfler
 * organization: LTI, RWTH Aachen
 * creation ...: 6.10.2000
 * revisions ..: $Id: ltiPseudoInverseMP_template.h,v 1.11 2005/02/02 14:16:23 doerfler Exp $
 */

#include "ltiMatrixInversion.h"
#include "ltiEquationSystem.h"

#include "ltiTimer.h"
#include <iostream>

namespace lti {
  // --------------------------------------------------
  // pseudoInverseMP::parameters
  // --------------------------------------------------

  // Due to a bug in VC++ this code must be written in the
  // declaration
#if 0

  // default constructor
  template <class T>
  pseudoInverseMP<T>::parameters::parameters()
    : linearAlgebraFunctor::parameters() {

    lambda=0.01;
  }

  // copy constructor
  template <class T>
  pseudoInverseMP<T>::parameters::parameters(const parameters& other)
    : linearAlgebraFunctor::parameters() {
    copy(other);
  }

  // destructor
  template <class T>
  pseudoInverseMP<T>::parameters::~parameters() {
  }

  // get type name
  template <class T>
  const char* pseudoInverseMP<T>::parameters::getTypeName() const {
    return "pseudoInverseMP<T>::parameters";
  }

  // copy member

  template <class T>
  pseudoInverseMP<T>::parameters&
    pseudoInverseMP<T>::parameters::copy(const parameters& other) {
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



    return *this;
  }

  // clone member
  template <class T>
  functor::parameters* pseudoInverseMP<T>::parameters::clone() const {
    return new parameters(*this);
  }

#endif // VC++ bug

  // --------------------------------------------------
  // pseudoInverseMP
  // --------------------------------------------------

  // default constructor
  template<class T>
  pseudoInverseMP<T>::pseudoInverseMP()
    : linearAlgebraFunctor(){
  }

  // constructor setting lambda
  template<class T>
  pseudoInverseMP<T>::pseudoInverseMP(const double& lambda) {
    parameters p;
    p.lambda = lambda;
    setParameters(p);
  }

  // copy constructor
  template<class T>
  pseudoInverseMP<T>::pseudoInverseMP(const pseudoInverseMP<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template<class T>
  pseudoInverseMP<T>::~pseudoInverseMP() {
  }

  // returns the name of this type
  template<class T>
  const char* pseudoInverseMP<T>::getTypeName() const {
    return "pseudoInverseMP<T>";
  }

  // copy member
  template<class T>
  pseudoInverseMP<T>&
  pseudoInverseMP<T>::copy(const pseudoInverseMP<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* pseudoInverseMP<T>::clone() const {
    return new pseudoInverseMP<T>(*this);
  }

  // return parameters
  template<class T>
  const typename pseudoInverseMP<T>::parameters&
  pseudoInverseMP<T>::getParameters() const {
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

  //for testing only
  //used to run a bit faster when Matrix multiplication was using
  //transpose. But this used way to much memory. Now, the method
  //below is about 4 times as fast.
/*    template<class T> */
/*    matrix<T>& pseudoInverseMP<T>::applyS(const matrix<T>& src, */
/*                                      matrix<T>& dest) const { */

/*      timer t; */
/*      t.start(); */
/*      matrix<T> trans; */

/*      cout << "transpose\n"; */
/*      trans.transpose(src); */

/*      cout << "mult1\n"; */
/*      dest.multiply(trans, src); */

/*      cout << "inversion\n"; */
/*      matrixInversion<T> inv; */
/*      inv.apply(dest); */

/*      cout << "mult2\n"; */
/*      dest.multiply(trans); */
/*      t.stop(); */
/*      cout << "S " << t.getTime() << "\n"; */
/*      return dest; */
/*    } */

  // calculate MP pseudo inverse NO smoothing
  template<class T>
  matrix<T>& pseudoInverseMP<T>::apply(const matrix<T>& src,
                                    matrix<T>& dest) const {


    matrix<T> dummy(src.columns(),src.columns(),0.);

    typename vector<T>::const_iterator iti ,itj;
    typename vector<T>::const_iterator itie, itje;
    typename matrix<T>::iterator itd;

    int i,j,k;

/*      // src transposed times src */
/*      for (int k=0; k<src.rows(); k++) { */
/*        itd=dummy.begin(); */
/*        for (iti=src.getRow(k).begin(), itie=src.getRow(k).end(); */
/*             iti!=itie; iti++) { */
/*          for (itj=src.getRow(k).begin(), itje=src.getRow(k).end(); */
/*               itj!=itje; itj++) { */
/*            *itd+=(*itj)*(*iti); */
/*            itd++; */
/*          } */
/*        } */
/*      } */

    // src transposed times src
    for (k=0; k<src.rows(); k++) {
      itd=dummy.begin();
      for (i=0, iti=src.getRow(k).begin(), itie=src.getRow(k).end();
           iti!=itie; iti++, i++) {
        itd+=i;
        itj=src.getRow(k).begin();
        for (itj+=i, itje=src.getRow(k).end();
             itj!=itje; itj++) {
          *itd+=(*itj)*(*iti);
          itd++;
        }
      }
    }
    for (i=1; i<dummy.rows(); i++) {
      for (j=0; j<i; j++) {
        dummy.at(i,j)=dummy.at(j,i);
      }
    }

    luSolution<T> luSol;
    typename luSolution<T>::parameters luSolPar;
    // do not copy (dummy could be huge!)
    luSolPar.systemMatrix.useExternData(dummy.rows(),
                                        dummy.columns(),
                                        &dummy.at(0,0));

    luSol.setParameters(luSolPar);

    dest=src;
    for (int i=0; i<src.rows(); i++) {
      luSol.apply(dest.getRow(i));
    }

    dest.transpose();

    return dest;
  };

  template<class T>
  matrix<T>& pseudoInverseMP<T>::apply(const matrix<T>& src,
                                    const matrix<T>& smooth,
                                    matrix<T>& dest) const {


    const parameters& param=getParameters();

    matrix<T> dummy(src.columns(),src.columns(),0.);

    typename vector<T>::const_iterator iti ,itj;
    typename vector<T>::const_iterator itie, itje;
    typename matrix<T>::iterator itd;

    int i,j,k;

    // src transposed times src
    for (k=0; k<src.rows(); k++) {
      itd=dummy.begin();
      for (i=0, iti=src.getRow(k).begin(), itie=src.getRow(k).end();
           iti!=itie; iti++, i++) {
        itd+=i;
        itj=src.getRow(k).begin();
        for (itj+=i, itje=src.getRow(k).end();
             itj!=itje; itj++) {
          *itd+=(*itj)*(*iti);
          itd++;
        }
      }
    }
    for (i=1; i<dummy.rows(); i++) {
      for (j=0; j<i; j++) {
        dummy.at(i,j)=dummy.at(j,i);
      }
    }

    dummy.addScaled(param.lambda, smooth);

    luSolution<T> luSol(dummy);
/*      luSolution<T>::parameters luSolPar; */
/*      // do not copy (dummy could be huge!) */
/*      luSolPar.systemMatrix.useExternData(dummy.rows(), */
/*                                          dummy.columns(), */
/*                                          &dummy.at(0,0)); */

/*      luSol.setParameters(luSolPar); */

    dest=src;
    for (i=0; i<src.rows(); ++i) {
      luSol.apply(dest.getRow(i));
    }

    dest.transpose();

    return dest;
  }

}


