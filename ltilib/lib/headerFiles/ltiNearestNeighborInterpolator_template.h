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
 * file .......: ltiNearestNeighborInterpolator_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiNearestNeighborInterpolator_template.h,v 1.3 2004/05/03 12:18:38 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // nearestNeighborInterpolator
  // --------------------------------------------------

  // default constructor
  template<class T>
  nearestNeighborInterpolator<T>::nearestNeighborInterpolator()
    : scalarValuedInterpolation<T>() {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  nearestNeighborInterpolator<T>::nearestNeighborInterpolator(
                                            const eBoundaryType boundaryType)
    : scalarValuedInterpolation<T>() {
    parameters defaultParameters(boundaryType);
    setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  nearestNeighborInterpolator<T>::nearestNeighborInterpolator(
                                       const nearestNeighborInterpolator& other
                                       )
    : scalarValuedInterpolation<T>() {
    copy(other);
  }

  // destructor
  template<class T>
  nearestNeighborInterpolator<T>::~nearestNeighborInterpolator() {
  }

  // returns the name of this type
  template<class T>
  const char* nearestNeighborInterpolator<T>::getTypeName() const {
    return "nearestNeighborInterpolator";
  }

  // copy member
  template<class T>
  nearestNeighborInterpolator<T>&
  nearestNeighborInterpolator<T>::copy(const nearestNeighborInterpolator<T>& 
                                       other) {
    scalarValuedInterpolation<T>::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* nearestNeighborInterpolator<T>::clone() const {
    return new nearestNeighborInterpolator(*this);
  }

  // return parameters
  template<class T>
  const typename nearestNeighborInterpolator<T>::parameters&
  nearestNeighborInterpolator<T>::getParameters() const {
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
  T nearestNeighborInterpolator<T>::apply(const vector<T>& src,
                                          const float& x) const {
    const int truncX = iround(x);

    // the interpolation:
    if (static_cast<unsigned int>(truncX) <
        static_cast<unsigned int>(src.lastIdx())) {
      return src.at(truncX);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary:
        case lti::Zero: {
          return static_cast<T>(0);
        } break;
        case lti::Constant: {
          if (truncX<0) {
            return src.at(0);
          } else {
            return src.at(src.lastIdx());
          }
        } break;
        case lti::Mirror: {
          // fix x
          int p = truncX/src.size();
          int xp = truncX%src.size();
          
          if (truncX < 0) {
            p = -p + 1;
            if (xp < 0) {
              xp += src.size();
            }
          }
          
          if ((p & 01) != 0) { // odd number
            xp = src.size()-1-xp;
          }

          return src.at(xp);

          } break;
          case lti::Periodic: {
            int x = truncX%src.size();

            if (x < 0) {
              x += src.size();
            }
            
            return src.at(x);
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
  T nearestNeighborInterpolator<T>::apply(const float& x) const {
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
  T nearestNeighborInterpolator<T>::apply(const matrix<T>& src,
                                          const float& row,
                                          const float& col) const {
    
    return interpolate(src,row,col);
  }

  template<class T>
  T nearestNeighborInterpolator<T>::apply(const matrix<T>& src,
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
  T nearestNeighborInterpolator<T>::apply(const float& row,
                                   const float& col) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,row,col);
  }

  template<class T>
  T nearestNeighborInterpolator<T>::apply(const tpoint<float>& p) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,p.y,p.x);
  }


}

