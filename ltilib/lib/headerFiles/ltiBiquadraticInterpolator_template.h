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
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiBiquadraticInterpolator_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiBiquadraticInterpolator_template.h,v 1.2 2004/05/03 12:17:46 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // biquadraticInterpolator
  // --------------------------------------------------

  // default constructor
  template<class T>
  biquadraticInterpolator<T>::biquadraticInterpolator()
    : scalarValuedInterpolation<T>() {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  biquadraticInterpolator<T>::biquadraticInterpolator(const eBoundaryType btyp)
    : scalarValuedInterpolation<T>() {
    parameters defaultParameters(btyp);
    setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  biquadraticInterpolator<T>::biquadraticInterpolator(const
                                                biquadraticInterpolator& other)
    : scalarValuedInterpolation<T>() {
    copy(other);
  }

  // destructor
  template<class T>
  biquadraticInterpolator<T>::~biquadraticInterpolator() {
  }

  // returns the name of this type
  template<class T>
  const char* biquadraticInterpolator<T>::getTypeName() const {
    return "biquadraticInterpolator";
  }

  // copy member
  template<class T>
  biquadraticInterpolator<T>&
  biquadraticInterpolator<T>::copy(const biquadraticInterpolator<T>& other) {
    scalarValuedInterpolation<T>::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* biquadraticInterpolator<T>::clone() const {
    return new biquadraticInterpolator(*this);
  }

  // return parameters
  template<class T>
  const typename biquadraticInterpolator<T>::parameters&
  biquadraticInterpolator<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *par;
  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  /*
   * Returns the interpolated value of the vector at the real valued
   * position x.
   * @param src vector<T> with the source data.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.
   */
  template<class T>
  T biquadraticInterpolator<T>::apply(const vector<T>& src,
                                      const float& x) const {

    int truncX = static_cast<int>(x);
    if (x<0) {
      --truncX;
    }
    float fractX = x - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // the interpolation:    
    if ((truncX >= 0) && (truncX < (src.size()-2))) {
      return compute(fractX,src[truncX],src[truncX+1],src[truncX+2]);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: {
        } break;
        
        case lti::Zero: {
          return compute(fractX,
                         zeroAt(src,truncX),
                         zeroAt(src,truncX+1),
                         zeroAt(src,truncX+2));
        } break;
        case lti::Constant: {
          return compute(fractX,
                         cstAt(src,truncX),
                         cstAt(src,truncX+1),
                         cstAt(src,truncX+2));
          
        } break;
        case lti::Mirror: {
          return compute(fractX,
                         mirrAt(src,truncX),
                         mirrAt(src,truncX+1),
                         mirrAt(src,truncX+2));
        } break;
        case lti::Periodic: {
          return compute(fractX,
                         periAt(src,truncX),
                         periAt(src,truncX+1),
                         periAt(src,truncX+2));
        } break;
        default:
          return T();
      }
    }
    return T();
  }

  /*
   * Returns the interpolated value of the vector specified with
   * use() at the real valued position x.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.  */
  template<class T>
  T biquadraticInterpolator<T>::apply(const float& x) const {
    assert(notNull(this->theVector));
    return apply(*this->theVector,x);
  }

  /*
   * Returns the interpolated value of the matrix at the real valued
   * position (row,col).
   *
   * @param src matrix<T> with the source data.
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<class T>
  T biquadraticInterpolator<T>::apply(const matrix<T>& src,
                                   const float& row,
                                   const float& col) const {

    return interpolate(src,row,col);
  }

  template<class T>
  T biquadraticInterpolator<T>::apply(const matrix<T>& src,
                                   const tpoint<float>& p) const {
    return interpolate(src,p.y,p.x);
  }

  /*
   * Returns the interpolated value of the matrix specified with
   * use() at the real valued position (row,col).
   *
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<class T>
  T biquadraticInterpolator<T>::apply(const float& row,
                                   const float& col) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,row,col);
  }

  template<class T>
  T biquadraticInterpolator<T>::apply(const tpoint<float>& p) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,p.y,p.x);
  }



  /*
   * Returns the interpolated value of the matrix at the real valued
   * position (row,col).
   *
   * @param src matrix<T> with the source data.
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<class T>
  T biquadraticInterpolator<T>::interpolate(const matrix<T>& src,
                                            const float row,
                                            const float col) const {
    // precomputations for x
    int truncX = static_cast<int>(col);
    if (col<0) {
      --truncX;
    }
    float fractX = col - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // precomputation for y
    int truncY = static_cast<int>(row);
    if (row<0) {
      --truncY;
    }
    float fractY = row - static_cast<float>(truncY);
    if (fractY <= 0.5f) {
      --truncY;
    } else {
      fractY-=1.0f;
    }

    const int x1=truncX+1;
    const int x2=truncX+2;
    const int y1=truncY+1;
    const int y2=truncY+2;

    if ((truncX >= 0) && (x2 < (src.columns())) &&
        (truncY >= 0) && (y2 < (src.rows()))) {
      return compute(fractY,fractX,
                     src.at(truncY,truncX),src.at(truncY,x1),src.at(truncY,x2),
                     src.at(y1,truncX),    src.at(y1,x1),    src.at(y1,x2),
                     src.at(y2,truncX),    src.at(y2,x1),    src.at(y2,x2));
    } else {     
      switch(getParameters().boundaryType) {
        case lti::NoBoundary: {
        } break;
        case lti::Zero: {         
          return compute(fractY,fractX,
                         zeroAt(src,truncY,truncX),
                         zeroAt(src,truncY,x1),
                         zeroAt(src,truncY,x2),
                         zeroAt(src,y1,truncX),
                         zeroAt(src,y1,x1),
                         zeroAt(src,y1,x2),
                         zeroAt(src,y2,truncX),
                         zeroAt(src,y2,x1),
                         zeroAt(src,y2,x2));
        } break;
        case lti::Constant: {
          return compute(fractY,fractX,
                         cstAt(src,truncY,truncX),
                         cstAt(src,truncY,x1),
                         cstAt(src,truncY,x2),
                         cstAt(src,y1,truncX),
                         cstAt(src,y1,x1),
                         cstAt(src,y1,x2),
                         cstAt(src,y2,truncX),
                         cstAt(src,y2,x1),
                         cstAt(src,y2,x2));
        } break;
        case lti::Mirror: {
          return compute(fractY,fractX,
                         mirrAt(src,truncY,truncX),
                         mirrAt(src,truncY,x1),
                         mirrAt(src,truncY,x2),
                         mirrAt(src,y1,truncX),
                         mirrAt(src,y1,x1),
                         mirrAt(src,y1,x2),
                         mirrAt(src,y2,truncX),
                         mirrAt(src,y2,x1),
                         mirrAt(src,y2,x2));
        } break;
        case lti::Periodic: {
          return compute(fractY,fractX,
                         periAt(src,truncY,truncX),
                         periAt(src,truncY,x1),
                         periAt(src,truncY,x2),
                         periAt(src,y1,truncX),
                         periAt(src,y1,x1),
                         periAt(src,y1,x2),
                         periAt(src,y2,truncX),
                         periAt(src,y2,x1),
                         periAt(src,y2,x2));
          
        } break;
        default:
          return T();
      }
    }

    return T();
  }

}

