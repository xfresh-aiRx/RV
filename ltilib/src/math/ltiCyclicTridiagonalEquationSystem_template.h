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
 * file .......: ltiTridiagonalEquationSystem_template.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 15.04.2003
 * revisions ..: $Id: ltiCyclicTridiagonalEquationSystem_template.h,v 1.2 2004/05/03 12:20:01 alvarado Exp $
 */

namespace lti {

  /**
   * constructor, sets the parameters
   */
  template <class T>
  cyclicTridiagonalEquationSystem<T>::cyclicTridiagonalEquationSystem(const parameters& theParams) : tridiagonalEquationSystem<T>() {
    setParameters(theParams);
  }
  
  /**
   * constructor, sets the diagonals
   */
  template <class T>
  cyclicTridiagonalEquationSystem<T>::cyclicTridiagonalEquationSystem(const vector<T> a, const vector<T> b, const vector<T> c, const T& tr, const T& bl)
    : tridiagonalEquationSystem<T>() {
    
    parameters p=getParameters();
    p.a.copy(a);
    p.b.copy(b);
    p.c.copy(c);
    setParameters(p);
  }

  /**
   * returns the current parameters.
   */
  template <class T>
  const typename cyclicTridiagonalEquationSystem<T>::parameters& 
  cyclicTridiagonalEquationSystem<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw functor::invalidParametersException(this->getTypeName());
    }
    return *par;
  }

  /**
   * copy data of "other" functor.
   */
  template <class T>
  cyclicTridiagonalEquationSystem<T>& 
  cyclicTridiagonalEquationSystem<T>::copy(const cyclicTridiagonalEquationSystem<T>& other) {
    tridiagonalEquationSystem<T>::copy(other);
    return (*this);
  }

  /**
   * onPlace version of apply.
   *
   * Uses the diagonals from the parameters to calculate a
   * solution. \a r also contains the result.
   */
  template <class T>
  bool cyclicTridiagonalEquationSystem<T>::apply(vector<T>& r) {
    vector<T> tmp(r);
    return apply(tmp,r);
  }
  
  /** onCopy version of apply.
   *
   * Uses the diagonals from the parameters to calculate a
   * solution.
   */
  template <class T>
  bool cyclicTridiagonalEquationSystem<T>::apply(const vector<T>& r,vector<T>& x) {
    const parameters& param=getParameters();
    return apply (param.a, param.b, param.c, param.tr, param.bl, r, x);
  }
  
  /**
   * Uses the given diagonals instead of those in the parameters to
   * calculate the solution.
   */
  template <class T>
  bool cyclicTridiagonalEquationSystem<T>::apply(const vector<T>& a, 
                                                 const vector<T>& b, 
                                                 const vector<T>& c,
                                                 const T& tr, const T& bl,
                                                 const vector<T>& r, 
                                                 vector<T>& x) {
    
    assert(a.size()==b.size());
    assert(a.size()==c.size());
    assert(a.size()==r.size());

    bool ok=true;

    int sz=a.size();

    if (sz<3) {
      this->setStatusString("at least two dimensions necessary");
      x.clear();
      return false;
    }
    
    T s= -b.at(0);

    vector<T> btmp(b);
    btmp.at(0)-=s;
    btmp.at(sz-1)-=tr*bl/s;
    
    ok = tridiagonalEquationSystem<T>::apply(a,btmp,c,r,x);

    vector<T> z;
    vector<T> rtmp(sz,T(0));
    rtmp.at(0)=s;
    rtmp.at(sz-1)=bl;
    ok = ok && tridiagonalEquationSystem<T>::apply(a,btmp,c,rtmp,z);

    T fac=(x.at(0)+tr*x.at(sz-1)/s)/(T(1)+x.at(0)+tr*z.at(sz-1)/s);

    typename vector<T>::iterator xit=x.begin();
    typename vector<T>::iterator endit=x.end();
    typename vector<T>::iterator zit=z.begin();

    for (; xit!=endit; ++xit, ++zit) {
      *xit-=fac*(*zit);
    }

    return ok;
 
  }
}
