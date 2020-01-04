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
 * revisions ..: $Id: ltiTridiagonalEquationSystem_template.h,v 1.2 2004/05/03 12:20:02 alvarado Exp $
 */

namespace lti {

  /**
   * constructor, sets the parameters
   */
  template <class T>
  tridiagonalEquationSystem<T>::tridiagonalEquationSystem(const parameters& theParams) : linearEquationSystemSolutionMethod<T>() {
    setParameters(theParams);
  }
  
  /**
   * constructor, sets the diagonals
   */
  template <class T>
  tridiagonalEquationSystem<T>::tridiagonalEquationSystem(const vector<T> a,
                                                          const vector<T> b, 
                                                          const vector<T> c)
    : linearEquationSystemSolutionMethod<T>() {
    
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
  const typename tridiagonalEquationSystem<T>::parameters& 
  tridiagonalEquationSystem<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *par;
  }

  /**
   * copy data of "other" functor.
   */
  template <class T>
  tridiagonalEquationSystem<T>& 
  tridiagonalEquationSystem<T>::copy(const tridiagonalEquationSystem<T>& other) {
    linearEquationSystemSolutionMethod<T>::copy(other);
    return (*this);
  }

  /**
   * onPlace version of apply.
   *
   * Uses the diagonals from the parameters to calculate a
   * solution. \a r also contains the result.
   */
  template <class T>
  bool tridiagonalEquationSystem<T>::apply(vector<T>& r) {
    vector<T> tmp(r);
    return apply(tmp,r);
  }
  
  /** onCopy version of apply.
   *
   * Uses the diagonals from the parameters to calculate a
   * solution.
   */
  template <class T>
  bool tridiagonalEquationSystem<T>::apply(const vector<T>& r,vector<T>& x) {
    const parameters& param=getParameters();
    return apply (param.a, param.b, param.c, r, x);
  }
  
  /**
   * Uses the given diagonals instead of those in the parameters to
   * calculate the solution.
   */
  template <class T>
  bool tridiagonalEquationSystem<T>::apply(const vector<T>& a, 
                                           const vector<T>& b, 
                                           const vector<T>& c,
                                           const vector<T>& r, 
                                           vector<T>& x) {
    
    assert(a.size()==b.size());
    assert(a.size()==c.size());
    assert(a.size()==r.size());
    
    if (b.at(0) == T(0)) {
      this->setStatusString("first element of b must be non-zero");
      x.clear();
      return false;
    }

    int sz=a.size();;
    vector<T> tVec(false,sz);
    T sTmp;

    x.resize(sz,T(0),false,false);

    typename vector<T>::const_iterator ait=a.begin(); ++ait;
    typename vector<T>::const_iterator bit=b.begin();
    typename vector<T>::const_iterator cit=c.begin();
    typename vector<T>::const_iterator rit=r.begin();
    typename vector<T>::iterator xit=x.begin();
    typename vector<T>::iterator endit=x.end();
    typename vector<T>::iterator tit=tVec.begin(); ++tit;
    
    sTmp=*(bit);
    *(xit)=*(rit)/sTmp;
    typename vector<T>::iterator xit2=xit;
    
    ++xit; ++bit; ++rit;

    for (; ait!=endit; ++ait, ++bit, ++cit, ++rit, ++xit, ++tit) {
      *tit=*cit/sTmp;
      sTmp=*bit-(*ait)*(*tit);
      if (sTmp==T(0)) {
        this->setStatusString("numeric instbility");
        return false;
      }
      *xit=(*rit-*ait*(*xit2))/sTmp;
      xit2=xit;
    }
    xit=--xit2;
    tit=tVec.begin();
    tit+=(sz-1);
    endit=x.begin();
    *xit-= *tit*(*xit2);
    do {
      xit2=xit;
      --xit; --tit;
      *xit-= *tit*(*xit2);
    } while(xit!=endit);
      
    return true;
 
  }
}
