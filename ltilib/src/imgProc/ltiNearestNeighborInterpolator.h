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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiNearestNeighborInterpolator.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiNearestNeighborInterpolator.h,v 1.3 2004/05/03 12:18:38 alvarado Exp $
 */

#ifndef _LTI_NEAREST_NEIGHBOR_INTERPOLATOR_H_
#define _LTI_NEAREST_NEIGHBOR_INTERPOLATOR_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiScalarValuedInterpolation.h"

namespace lti {
  /**
   * This functor use nearestNeighbor interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class nearestNeighborInterpolator : public scalarValuedInterpolation<T> {
  public:
    typedef typename scalarValuedInterpolation<T>::parameters parameters;

    /**
     * default constructor
     */
    nearestNeighborInterpolator();

    /**
     * Construct an interpolator with the given boundary type.
     */
    nearestNeighborInterpolator(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    nearestNeighborInterpolator(const nearestNeighborInterpolator<T>& other);

    /**
     * destructor
     */
    virtual ~nearestNeighborInterpolator();

    /**
     * returns the name of this type ("nearestNeighborInterpolator")
     */
    virtual const char* getTypeName() const;

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    T apply(const vector<T>& src,const float& x) const;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    T apply(const float& x) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    T apply(const matrix<T>& src,
            const float& row,
            const float& col) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position p.
     *
     * @param src matrix<T> with the source data.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    T apply(const matrix<T>& src,const tpoint<float>& p) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    T apply(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     *
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    T apply(const tpoint<float>& p) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).  This method is not virtual and can be used
     * if this interpolation type is used as template parameter in time
     * critical situations
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const matrix<T>& src,
                         const float row,
                         const float col) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nearestNeighborInterpolator& copy(const nearestNeighborInterpolator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };

  // --------------------------------------
  // implementation of public inline method
  // --------------------------------------

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
  inline T nearestNeighborInterpolator<T>::interpolate(const matrix<T>& src,
                                                       const float row,
                                                       const float col) const {


    const int truncY = iround(row);
    const int truncX = iround(col);

    // the interpolation:
    if ((static_cast<unsigned int>(truncX)<
         static_cast<unsigned int>(src.columns())) &&
        (static_cast<unsigned int>(truncY)<
         static_cast<unsigned int>(src.rows()))) {
      // normal interpolation within matrix range
      return src.at(truncY,truncX);
    } else {
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: 
        case lti::Zero:{
          return static_cast<T>(0);
        } break;
        case lti::Constant: {
          int x=truncX;
          int y=truncY;
          if (x<0) {
            x=0;
          } else if (x>=src.columns()) {
            x=src.lastColumn();
          }
          if (y<0) {
            y=0;
          } else if (y>=src.rows()) {
            y=src.lastRow();
          }
          return src.at(y,x);
        } break;
        case lti::Mirror: {
          int p,xp,yp;

          // fix x
          p = truncX/src.columns();
          xp = truncY%src.columns();

          if (truncX < 0) {
            p = 1-p;
            if (xp < 0) {
              xp += src.columns();
            }
          }

          if ((p & 01) != 0) { // odd number
            xp = src.lastColumn()-xp;
          }

          // fix y
          p = truncY/src.rows();
          yp = truncY%src.rows();

          if (truncY < 0) {
            p = 1-p;
            if (yp < 0) {
              yp += src.rows();
            }
          }

          if ((p & 01) != 0) { // odd number
            yp = src.lastRow()-yp;
          }

          return src.at(yp,xp);
        } break;
        case lti::Periodic: {
          int x = truncX%src.columns();

          if (x < 0) {
            x += src.columns();
          }

          int y = truncY%src.rows();
          
          if (y < 0) {
            y += src.rows();
          }

          return src.at(y,x);
        } break;
        default:
          return T();
      }
    }
    return T();
  }

}

#endif

