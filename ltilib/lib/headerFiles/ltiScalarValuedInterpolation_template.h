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
 * file .......: ltiScalarValuedInterpolation_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiScalarValuedInterpolation_template.h,v 1.5 2004/05/03 12:20:02 alvarado Exp $
 */

namespace lti {

  // -------------------------------------------------------------------
  // The inline boundary access methods
  // -------------------------------------------------------------------

  /*
   * Access with zero boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::zeroAt(const vector<T>& vct,
                                                const int x) const {
    if (static_cast<uint32>(x)<=static_cast<uint32>(vct.size())) {
      return vct.at(x);
    } 
    return T(0);
  }

  /*
   * Access with constant boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::cstAt(const vector<T>& vct,
                                               const int x) const {
    if (x<0) {
      return vct.at(0);
    } else if (x>vct.lastIdx()) {
      return vct.at(vct.lastIdx());
    }
    return vct.at(x);
  }

  /*
   * Access with mirrored boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::mirrAt(const vector<T>& vct,
                                                const int x) const {

    int p,xp;
    
    // fix x
    p = x/vct.size();
    xp = x%vct.size();

    if (x < 0) {
      p = 1-p;
      if (xp < 0) {
        xp += vct.size();
      }
    }
            
    if ((p & 01) != 0) { // odd number
      xp = vct.lastIdx()-xp;
    }

    return vct.at(xp);
  }

  /*
   * Access with periodic boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::periAt(const vector<T>& vct,
                                                const int x) const {
    int xp;
    xp = x % vct.size();
    if (xp < 0) {
      xp += vct.size();
    }
    return vct.at(xp);
  }

  /*
   * Access with zero boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::zeroAt(const matrix<T>& img,
                                                const int y,
                                                const int x) const {
    if (static_cast<uint32>(y)<=static_cast<uint32>(img.lastRow()) && 
        static_cast<uint32>(x)<=static_cast<uint32>(img.lastColumn())) {
      return img.at(y,x);
    } 
    return T(0);
  }

  /*
   * Access with constant boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::cstAt(const matrix<T>& img,
                                               const int y,
                                               const int x) const {

    if (x<0) {
      if (y<0) {
        return img.at(0,0);
      } else if (y>img.lastRow()) {
        return img.at(img.lastRow(),0);
      } 
      return img.at(y,0);
    } else if (x>img.lastColumn()) {
      if (y<0) {
        return img.at(0,img.lastColumn());
      } else if (y>img.lastRow()) {
        return img.at(img.lastRow(),img.lastColumn());
      } 
      return img.at(y,img.lastColumn());
    }

    if (y<0) {
      return img.at(0,x);
    } else if (y>img.lastRow()) {
      return img.at(img.lastRow(),x);
    } 
    return img.at(y,x);
  }

  /*
   * Access with mirrored boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::mirrAt(const matrix<T>& img,
                                                const int y,
                                                const int x) const {


    int p,xp,yp;
    
    // fix x
    p = x/img.columns();
    xp = x%img.columns();

    if (x < 0) {
      p = 1-p;
      if (xp < 0) {
        xp += img.columns();
      }
    }
            
    if ((p & 01) != 0) { // odd number
      xp = img.lastColumn()-xp;
    }

    // fix y
    p  = y/img.rows();
    yp = y%img.rows();

    if (y < 0) {
      p = 1-p;
      if (yp < 0) {
        yp += img.rows();
      }
    }
            
    if ((p & 01) != 0) { // odd number
      yp = img.lastRow()-yp;
    }

    return img.at(yp,xp);
  }

  /*
   * Access with period boundary
   */
  template<class T>
  inline T scalarValuedInterpolation<T>::periAt(const matrix<T>& img,
                                              const int y,
                                              const int x) const {
    int xp,yp;
    xp = x % img.columns();
    if (xp < 0) {
      xp += img.columns();
    }

    yp = y % img.rows();
    if (yp < 0) {
      yp += img.rows();
    }

    return img.at(yp,xp);
  }


  // --------------------------------------------------
  // scalarValuedInterpolation<T>::parameters
  // --------------------------------------------------

  // implementation of parameters in header file due to MSVC++ bugs

  // --------------------------------------------------
  // scalarValuedInterpolation
  // --------------------------------------------------

  // default constructor
  template<class T>
  scalarValuedInterpolation<T>::scalarValuedInterpolation()
    : equallySpacedSamplesInterpolator(), theVector(0),theMatrix(0) {
  }

  // copy constructor
  template<class T>
  scalarValuedInterpolation<T>::scalarValuedInterpolation(const scalarValuedInterpolation& other)
    : equallySpacedSamplesInterpolator(), theVector(0),theMatrix(0) {
    copy(other);
  }

  // destructor
  template<class T>
  scalarValuedInterpolation<T>::~scalarValuedInterpolation() {
  }

  // returns the name of this type
  template<class T>
  const char* scalarValuedInterpolation<T>::getTypeName() const {
    return "scalarValuedInterpolation";
  }

  // copy member
  template<class T>
  scalarValuedInterpolation<T>& scalarValuedInterpolation<T>::
  copy(const scalarValuedInterpolation<T>& other) {
    equallySpacedSamplesInterpolator::copy(other);

    theVector = other.theVector;
    theMatrix = other.theMatrix;

    return (*this);
  }

  // return parameters
  template<class T>
  const typename scalarValuedInterpolation<T>::parameters&
  scalarValuedInterpolation<T>::getParameters() const {
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

  template<class T>
  bool scalarValuedInterpolation<T>::use(const vector<T>& vct) {
    theVector = &vct;
    return true;
  }

  template<class T>
  bool scalarValuedInterpolation<T>::use(const matrix<T>& mat) {
    theMatrix = &mat;
    return true;
  }

}

