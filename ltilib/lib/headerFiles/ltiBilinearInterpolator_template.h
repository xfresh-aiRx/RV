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
 * file .......: ltiBilinearInterpolator_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiBilinearInterpolator_template.h,v 1.17 2004/05/03 12:17:46 alvarado Exp $
 */

namespace lti {

  // --------------------------------------------------
  // bilinearInterpolator
  // --------------------------------------------------

  // default constructor
  template<class T>
  bilinearInterpolator<T>::bilinearInterpolator()
    : scalarValuedInterpolation<T>() {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  bilinearInterpolator<T>::bilinearInterpolator(const eBoundaryType btype)
    : scalarValuedInterpolation<T>() {
    parameters defaultParameters(btype);
    setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  bilinearInterpolator<T>::bilinearInterpolator(const
                                                bilinearInterpolator& other)
    : scalarValuedInterpolation<T>() {
    copy(other);
  }

  // destructor
  template<class T>
  bilinearInterpolator<T>::~bilinearInterpolator() {
  }

  // returns the name of this type
  template<class T>
  const char* bilinearInterpolator<T>::getTypeName() const {
    return "bilinearInterpolator";
  }

  // copy member
  template<class T>
  bilinearInterpolator<T>&
  bilinearInterpolator<T>::copy(const bilinearInterpolator<T>& other) {
    scalarValuedInterpolation<T>::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* bilinearInterpolator<T>::clone() const {
    return new bilinearInterpolator(*this);
  }

  // return parameters
  template<class T>
  const typename bilinearInterpolator<T>::parameters&
  bilinearInterpolator<T>::getParameters() const {
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
  T bilinearInterpolator<T>::apply(const vector<T>& src,const float& x) const {
    int truncX = static_cast<int>(x);
    if (x<0) {
      --truncX;
    }
    const float fractX = x - static_cast<float>(truncX);

    // the interpolation:
    if (static_cast<unsigned int>(truncX) <
        static_cast<unsigned int>(src.lastIdx())) {
      return compute(fractX,src.at(truncX),src.at(truncX + 1));
    } else {
      int x,x1;
      switch (getParameters().boundaryType) {
          case lti::NoBoundary: {
          } break;

          case lti::Zero: {
            T a1(0),a2(0);

            if (truncX < 0) { // at the left side
              if (truncX == -1) { // has only one pixel
                a2 = src.at(0);
              } 
            } else {
              // if (truncX >= 0):
              if (truncX >= src.lastIdx()) { // and at the right side
                                             // is outside
                if (truncX < src.size()) {
                  a1 = src.at(truncX);         // or has also one pixel
                }
              }
            }

            return compute(fractX,a1,a2);


          } break;
          case lti::Constant: {
            x = truncX;
            x1 = truncX+1;

            if (x<0) {
              x=0;
            } else if (x>=src.size()) {
              x = src.lastIdx();
            }

            if (x1<0) {
              x1=0;
            } else if (x1>=src.size()) {
              x1 = src.lastIdx();
            }

            return compute(fractX,src.at(x),src.at(x1));

          } break;
          case lti::Mirror: {
            x = truncX;
            x1 = truncX+1;

            int p,xp;

            // fix x
            p = x/src.size();
            xp = x%src.size();

            if (x < 0) {
              p = -p + 1;
              if (xp < 0) {
                xp += src.size();
              }
            }

            if ((p & 01) != 0) { // odd number
              xp = src.size()-1-xp;
            }

            x = xp;

            // fix x1
            p = x1/src.size();
            xp = x1%src.size();

            if (x1 < 0) {
              p = -p + 1;
              if (xp < 0) {
                xp += src.size();
              }
            }

            if ((p & 01) != 0) { // odd number
              xp = src.size()-1-xp;
            }

            x1 = xp;

            return compute(fractX,src.at(x),src.at(x1));

          } break;
          case lti::Periodic: {
            x = truncX;
            x1 = truncX+1;

            x = x%src.size();

            if (x < 0) {
              x += src.size();
            }

            x1 = x1%src.size();

            if (x1 < 0) {
              x1 += src.size();
            }

            return compute(fractX,src.at(x),src.at(x1));
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
  T bilinearInterpolator<T>::apply(const float& x) const {
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
  T bilinearInterpolator<T>::apply(const matrix<T>& src,
                                   const float& row,
                                   const float& col) const {

    return interpolate(src,row,col);
  }

  template<class T>
  T bilinearInterpolator<T>::apply(const matrix<T>& src,
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
  T bilinearInterpolator<T>::apply(const float& row,
                                   const float& col) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,row,col);
  }

  template<class T>
  T bilinearInterpolator<T>::apply(const tpoint<float>& p) const {
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
  T bilinearInterpolator<T>::interpolate(const matrix<T>& src,
                                         const float row,
                                         const float col) const {

    const int truncY = (row < 0.0f) ?
      static_cast<int>(row)-1 : static_cast<int>(row);
    const int truncY1 = truncY+1;
    const float fractY = row - static_cast<float>(truncY);

    const int truncX = (col < 0.0f) ?
      static_cast<int>(col)-1 : static_cast<int>(col);
    const int truncX1 = truncX+1;
    const float fractX = col - static_cast<float>(truncX);

    // the interpolation:
    if ((static_cast<unsigned int>(truncX)<
         static_cast<unsigned int>(src.lastColumn())) &&
        (static_cast<unsigned int>(truncY)<
         static_cast<unsigned int>(src.lastRow()))) {
      // normal interpolation within matrix range
      return compute(fractY,fractX,
                     src.at(truncY ,truncX),src.at(truncY ,truncX1),
                     src.at(truncY1,truncX),src.at(truncY1,truncX1));
    } else {
      int x,y,x1,y1;
      switch (getParameters().boundaryType) {
          case lti::NoBoundary: {
            return T();
          } break;
          case lti::Zero: {
            T a1(0),a2(0),a3(0),a4(0);

            if (truncY < 0) {
              if (truncY == -1) { // the first line
                if (truncX < 0) { // at the left side
                  if (truncX == -1) { // has only one pixel
                    a4 = src.at(0,0);
                  } 
                } else {
                  // if (truncX >= 0):
                  if (truncX1 >= (src.columns())) { // and at the right side
                                                    // is outside
                    if (truncX < src.columns()) {
                      a3 = src.at(0,truncX);         // or has also one pixel
                    }
                  } else {
                    a3 = src.at(0,truncX);           // two pixels are in
                    a4 = src.at(0,truncX1);          // middle of the first
                    // line
                  }
                }
              }
            } else {
              // if (truncY >= 0)
              if (truncY1 >= (src.rows())) {
                if (truncY < src.rows()) {
                  // last line
                  if (truncX < 0) { // at the left side
                    if (truncX == -1) { // has only one pixel
                      a2 = src.at(0,0);
                    }
                  } else {
                    // if (truncX >= 0):
                    if (truncX1 >= (src.columns())) { // at the right side
                                                       // is outside
                      if (truncX < src.columns()) {
                        a1 = src.at(truncY,truncX); // or has also one pixel
                      }
                    } else {
                      a1 = src.at(truncY,truncX);   // two pixel in the mid.
                      a2 = src.at(truncY,truncX1); // of the last line
                    }
                  }
                }
              } else {
                // for the y inbetween...
                if (truncX < 0) {
                  // left border
                  if (truncX == -1) {
                    // still one pixel there
                    a2 = src.at(truncY,0);
                    a4 = src.at(truncY1,0);
                  } 
                } else {
                  // right border
                  if (truncX1 >=(src.columns())) {
                    // outside
                    if (truncX < src.columns()) {
                      // still one pixel there
                      a1 = src.at(truncY,truncX);
                      a3 = src.at(truncY1,truncX);
                    }
                  }
                }
              }
            }
            return compute(fractY,fractX,a1,a2,a3,a4);

          } break;
          case lti::Constant: {
            x = truncX;
            y = truncY;
            x1 = truncX1;
            y1 = truncY1;

            if (x<0) {
              x=0;
            } else if (x>=src.columns()) {
              x = src.lastColumn();
            }

            if (x1<0) {
              x1=0;
            } else if (x1>=src.columns()) {
              x1 = src.lastColumn();
            }

            if (y<0) {
              y=0;
            } else if (y>=src.rows()) {
              y = src.lastRow();
            }

            if (y1<0) {
              y1=0;
            } else if (y1>=src.rows()) {
              y1 = src.lastRow();
            }

            return compute(fractY,fractX,
                           src.at(y,x),src.at(y,x1),
                           src.at(y1,x),src.at(y1,x1));

          } break;
          case lti::Mirror: {
            x = truncX;
            y = truncY;
            x1 = truncX1;
            y1 = truncY1;

            int p,xp;

            // fix x
            p = x/src.columns();
            xp = x%src.columns();

            if (x < 0) {
              p = 1-p;
              if (xp < 0) {
                xp += src.columns();
              }
            }

            if ((p & 01) != 0) { // odd number
              xp = src.lastColumn()-xp;
            }

            x = xp;

            // fix x1
            p = x1/src.columns();
            xp = x1%src.columns();

            if (x1 < 0) {
              p = 1-p;
              if (xp < 0) {
                xp += src.columns();
              }
            }

            if ((p & 01) != 0) { // odd number
              xp = src.lastColumn()-xp;
            }

            x1 = xp;

            // fix y
            p = y/src.rows();
            xp = y%src.rows();

            if (y < 0) {
              p = 1-p;
              if (xp < 0) {
                xp += src.rows();
              }
            }

            if ((p & 01) != 0) { // odd number
              xp = src.lastRow()-xp;
            }

            y = xp;

            // fix y1
            p = y1/src.rows();
            xp = y1%src.rows();

            if (y1 < 0) {
              p = 1-p;
              if (xp < 0) {
                xp += src.rows();
              }
            }

            if ((p & 01) != 0) { // odd number
              xp = src.lastRow()-xp;
            }

            y1 = xp;

            return compute(fractY,fractX,
                           src.at(y,x),src.at(y,x1),
                           src.at(y1,x),src.at(y1,x1));
          } break;
          case lti::Periodic: {
            x = truncX;
            y = truncY;
            x1 = truncX;
            y1 = truncY;

            x = x%src.columns();

            if (x < 0) {
              x += src.columns();
            }

            x1 = x1%src.columns();

            if (x1 < 0) {
              x1 += src.columns();
            }

            y = y%src.rows();

            if (y < 0) {
              y += src.rows();
            }

            y1 = y1%src.rows();

            if (y1 < 0) {
              y1 += src.rows();
            }

            return compute(fractY,fractX,
                           src.at(y,x),src.at(y,x1),
                           src.at(y1,x),src.at(y1,x1));
          } break;
          default:
            return T();
      }
    }
    return T();
  }

}

