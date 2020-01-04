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
 * file .......: ltiBilinearInterpolator.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiBilinearInterpolator.h,v 1.13 2004/05/03 12:17:46 alvarado Exp $
 */

#ifndef _LTI_BILINEAR_INTERPOLATOR_H_
#define _LTI_BILINEAR_INTERPOLATOR_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiScalarValuedInterpolation.h"

namespace lti {
  /**
   * This functor use bilinear interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class bilinearInterpolator : public scalarValuedInterpolation<T> {
  public:
    typedef typename scalarValuedInterpolation<T>::parameters parameters;

    /**
     * default constructor
     */
    bilinearInterpolator();

    /**
     * Construct an interpolator with the given boundary type
     */
    bilinearInterpolator(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    bilinearInterpolator(const bilinearInterpolator<T>& other);

    /**
     * destructor
     */
    virtual ~bilinearInterpolator();

    /**
     * returns the name of this type ("bilinearInterpolator")
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
    T interpolate(const matrix<T>& src,
                  const float row,
                  const float col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolate(const float row,
                         const float col) const {
      return interpolate(*this->theMatrix,row,col);
    };

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * This method does not check if the given coordinates and the rest of
     * used points in the src matrix lie within the valid range.  This is
     * left to you.  Please consider that for the bilinear interpolation
     * not only the point(trunc(col),trunc(row)) is used, but also its three
     * "next" neighbors ((col,row+1),(col+1,row),(col+1,row+1)).
     *
     * This method is not virtual and can be used in time critical situations,
     * where you prefer to take the boundary control by yourself.
     *
     * @param src matrix<T> with the source data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const matrix<T>& src,
                              const float row,
                              const float col) const;

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).  This method is
     * not virtual and can be used if this interpolation type is used
     * as template parameter in time critical situations
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    inline T interpolateUnchk(const float row,
                                     const float col) const;
    
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bilinearInterpolator& copy(const bilinearInterpolator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Compute the bilinear interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between 0 and 1
     * @param fx fractional term between 0 and 1
     * @param syx value for fx==0 fy==0
     * @param syx1 value for fx==1 fy==0
     * @param sy1x value for fx==0 fy==1
     * @param sy1x1 value for fx==1 fy==1
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float fy, 
                     const float fx, 
                     const T syx, const T syx1,
                     const T sy1x, const T sy1x1) const;

    /**
     * Compute the linear interpolated value for the given coefficients
     * and values
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fx fractional term between 0 and 1
     * @param sx value for sx==0
     * @param sx1 value for sx==1
     *
     * @return interpolated value between the two extremes.
     */
    inline T compute(const float fx, 
                     const T sx, const T sx1) const;
  };


  // implementation of compute() inline methods
  
  template<class T>
  inline T bilinearInterpolator<T>::compute(const float fx,
                                            const T sx, const T sx1) const {
    return static_cast<T>(sx + fx*(sx1-sx));
  }

  // specialization for pixels
  template<>
  inline rgbPixel bilinearInterpolator<rgbPixel>::compute(const float fx,
                                                          const rgbPixel sx,
                                                          const rgbPixel sx1
                                                          ) const {
    const trgbPixel<float> sxf(sx);
    const trgbPixel<float> sx1f(sx1);
    return ((sxf + (sx1f-sxf)*fx).getRGBPixel());
  }

  template<class T>
  inline T bilinearInterpolator<T>::compute(const float fy,
                                            const float fx,
                                            const T syx, const T syx1,
                                            const T sy1x, const T sy1x1
                                            ) const {
    const float tmp1 = static_cast<float>(syx  + (syx1-syx)*fx);
    const float tmp2 = static_cast<float>(sy1x + (sy1x1-sy1x)*fx);
    return static_cast<T>(tmp1 + (tmp2-tmp1)*fy);
  }

  template<>
  inline rgbPixel
  bilinearInterpolator<rgbPixel>::compute(const float fy,
                                          const float fx,
                                          const rgbPixel syx,
                                          const rgbPixel syx1,
                                          const rgbPixel sy1x,
                                          const rgbPixel sy1x1) const {
    
    const trgbPixel<float> syxf(syx);
    const trgbPixel<float> syx1f(syx1);
    const trgbPixel<float> sy1xf(sy1x);
    const trgbPixel<float> sy1x1f(sy1x1);

    const trgbPixel<float> tmp1 = syxf  + (syx1f-syxf)*fx;
    const trgbPixel<float> tmp2 = sy1xf + (sy1x1f-sy1xf)*fx;
    
    return (tmp1 + (tmp2-tmp1)*fy).getRGBPixel();
  }


  template<class T>
  inline T bilinearInterpolator<T>::interpolateUnchk(const matrix<T>& src,
                                                     const float row,
                                                     const float col) const {
    const int truncY = static_cast<int>(row);
    const float fractY = row - static_cast<float>(truncY);

    const int truncX = static_cast<int>(col);
    const float fractX = col - static_cast<float>(truncX);

    const T *const row1 = &src.at(truncY,truncX);
    const T *const row2 = &src.at(truncY+1,truncX);

    // the interpolation:
    return compute(fractY,fractX,row1[0],row1[1],row2[0],row2[1]);
  }
  
  template<class T>
  inline T bilinearInterpolator<T>::interpolateUnchk(const float row,
                                                     const float col) const {
    return interpolateUnchk(*this->theMatrix,row,col);
  };


}

#endif

