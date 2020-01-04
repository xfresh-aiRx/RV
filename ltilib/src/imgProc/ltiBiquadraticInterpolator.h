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
 * file .......: ltiBiquadraticInterpolator.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiBiquadraticInterpolator.h,v 1.2 2004/05/03 12:17:46 alvarado Exp $
 */

#ifndef _LTI_BIQUADRATIC_INTERPOLATOR_H_
#define _LTI_BIQUADRATIC_INTERPOLATOR_H_

#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiScalarValuedInterpolation.h"

namespace lti {
  /**
   * This functor use biquadratic interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template <class T>
  class biquadraticInterpolator : public scalarValuedInterpolation<T> {
  public:
    typedef typename scalarValuedInterpolation<T>::parameters parameters;

    /**
     * default constructor
     */
    biquadraticInterpolator();

    /**
     * Construct an interpolator with the given boundary type
     */
    biquadraticInterpolator(const eBoundaryType boundaryType);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    biquadraticInterpolator(const biquadraticInterpolator<T>& other);

    /**
     * destructor
     */
    virtual ~biquadraticInterpolator();

    /**
     * returns the name of this type ("biquadraticInterpolator")
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
     * left to you.  Please consider that for the biquadratic interpolation
     * not only the point(trunc(col),trunc(row)) is used, but a 3x3 
     * neighborhood centered such that (col,row) is nearest to the center.
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
    biquadraticInterpolator& copy(const biquadraticInterpolator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Compute the biquadratic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fx fractional term between -0.5 and 0.5
     * @param y0 value for fx==-1.0 (note that is a value beyond the valid 
     *                               range of fx)
     * @param y1 value for fx==0.0
     * @param y2 value for fx==1.0  (note that is a value beyond the valid 
     *                               range of fx)
     *
     * @return interpolated value between the three values
     */
    inline T compute(const float fx,const T y0,const T y1,const T y2) const;
    
    /**
     * Compute the biquadratic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between -0.5 and 0.5
     * @param fx fractional term between -0.5 and 0.5
     * @param data array of 9 elements
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float fy,
                     const float fx,
                     const T *const  data) const;

    /**
     * Compute the biquadratic interpolated value for the given coefficients
     * and values.
     *
     * This method is provided for convenience only.  Use at your own
     * risk.
     *
     * @param fy fractional term between -0.5 and 0.5
     * @param fx fractional term between -0.5 and 0.5
     * @param y00 value for fx==-1 fy==-1
     * @param y01 value for fx==-1 fy== 0
     * @param y02 value for fx==-1 fy==+1
     * @param y10 value for fx== 0 fy==-1
     * @param y11 value for fx== 0 fy== 0
     * @param y12 value for fx== 0 fy==+1
     * @param y20 value for fx==+1 fy==-1
     * @param y21 value for fx==+1 fy== 0
     * @param y22 value for fx==+1 fy==+1
     *
     * @return interpolated value between the four corners
     */
    inline T compute(const float fy,
                     const float fx,
                     const T y00,const T y01,const T y02,
                     const T y10,const T y11,const T y12,
                     const T y20,const T y21,const T y22) const;

  };

  // --------------------------------------------
  // implementation of some inline helper methods
  // --------------------------------------------
  
  template<class T>
  inline T biquadraticInterpolator<T>::compute(const float fx,
                                               const T y0,
                                               const T y1,
                                               const T y2) const {

    const T a = (y0+y2)/2 - y1;
    const T b = (y2-y0)/2;
    const T c = y1;

    return (static_cast<T>((a*fx + b)*fx + c));
  }

  template<>
  inline rgbPixel 
  biquadraticInterpolator<rgbPixel>::compute(const float fx,
                                             const rgbPixel y0,
                                             const rgbPixel y1,
                                             const rgbPixel y2) const {
    const trgbPixel<float> y0f(y0);
    const trgbPixel<float> y1f(y1);
    const trgbPixel<float> y2f(y2);

    const trgbPixel<float> a = (y0f+y2f)/2.0f - y1f;
    const trgbPixel<float> b = (y2f-y0f)/2.0f;
    const trgbPixel<float> c = y1f;

    return ((a*fx + b)*fx + c).getClippedRGBPixel();
  }

  template<class T>
  inline T biquadraticInterpolator<T>::compute(const float fy,
                                               const float fx,
                                               const T *const y) const {
    
    const T v1 = compute(fx,y[0],y[1],y[2]);
    const T v2 = compute(fx,y[3],y[4],y[5]);
    const T v3 = compute(fx,y[6],y[7],y[8]);

    return compute(fy,v1,v2,v3);
  }

  template<class T>
  inline T biquadraticInterpolator<T>::compute(const float fy,
                                               const float fx,
                                               const T y00,
                                               const T y01,
                                               const T y02,
                                               const T y10,
                                               const T y11,
                                               const T y12,
                                               const T y20,
                                               const T y21,
                                               const T y22) const {
    
    const T v1 = compute(fx,y00,y01,y02);
    const T v2 = compute(fx,y10,y11,y12);
    const T v3 = compute(fx,y20,y21,y22);

    return compute(fy,v1,v2,v3);
  }


  template<class T>
  inline T biquadraticInterpolator<T>::interpolateUnchk(const matrix<T>& src,
                                                        const float row,
                                                        const float col) const{
    // precomputations for x
    int truncX = static_cast<int>(col);
    float fractX = col - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // precomputation for y
    int truncY = static_cast<int>(row);
    float fractY = row - static_cast<float>(truncY);
    if (fractY <= 0.5f) {
      --truncY;
    } else {
      fractY-=1.0f;
    }

    const T *const row1 = &src.at(truncY,truncX);
    const T *const row2 = &src.at(truncY+1,truncX);
    const T *const row3 = &src.at(truncY+2,truncX);

    return compute(fractY,fractX,
                   row1[0],row1[1],row1[2],
                   row2[0],row2[1],row2[2],
                   row3[0],row3[1],row3[2]);
    
  }

  template<class T>
  inline T biquadraticInterpolator<T>::interpolateUnchk(const float row,
                                                        const float col) const{
    return interpolateUnchk(*this->theMatrix,row,col);
  }


}

#endif

