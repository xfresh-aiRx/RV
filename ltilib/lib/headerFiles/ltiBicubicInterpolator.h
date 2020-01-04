
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
 * file .......: ltiBicubicInterpolator.h
 * authors ....: Jens Rietzschel
 * organization: LTI, RWTH Aachen
 * creation ...: 4.10.2001
 * revisions ..: $Id: ltiBicubicInterpolator.h,v 1.11 2004/05/03 12:17:46 alvarado Exp $
 */

#ifndef _LTI_BICUBIC_INTERPOLATOR_H_
#define _LTI_BICUBIC_INTERPOLATOR_H_

#include "ltiScalarValuedInterpolation.h"


namespace lti {
  /**
   * This functor use bicubic interpolation to approximate values
   * between the pixels or elements of vectors and matrices.
   *
   * The type T of the template is the type of the elements of the vector
   * or matrix used.
   */
  template<class T>
  class bicubicInterpolator : public scalarValuedInterpolation<T> {
  public:

    /**
     * default constructor
     */
    bicubicInterpolator();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    bicubicInterpolator(const bicubicInterpolator& other);

    /**
     * destructor
     */
    virtual ~bicubicInterpolator();

    /**
     * returns the name of this type ("bicubicInterpolator")
     */
    virtual const char* getTypeName() const;

    /**
     * Returns the interpolated value of the vector at the real valued
     * position x.
     * @param src vector<T> with the source data.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.
     */
    virtual T apply(const vector<T>& src,const float& x) const;

    /**
     * Returns the interpolated value of the vector specified with
     * use() at the real valued position x.
     * @param x the real valued position to be interpolated.
     * @return the interpolated value of the vector.  */
    virtual T apply(const float& x) const;

    /**
     * Returns the interpolated value of the matrix at the real valued
     * position (row,col).
     *
     * @param srcdest matrix<T> with the source and destination data.
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.
     */
    virtual T apply(const matrix<T>& srcdest,
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
    inline T apply(const matrix<T>& src,const tpoint<float>& p) const {
      return apply(src,p.y,p.x);
    };


    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position p.
     * @param p the real valued position to be interpolated.
     * @return the interpolated value of the matrix.
     */
    inline T apply(const tpoint<float>& p) const {
      return apply(p.y,p.x);
    };

    /**
     * Returns the interpolated value of the matrix specified with
     * use() at the real valued position (row,col).
     *
     * @param row which row
     * @param col which column
     * @return the interpolated value of the matrix.  */
    virtual T apply(const float& row, const float& col) const;

    /**
     * Returns the interpolated value of the gradient of the matrix src
     * at the real valued position (row,col).
     * dest.x row-direction  dest.y column-direction
     * @param src matrix with the source data
     * @param row which row
     * @param col which column
     * @param dest gradient
     */
    bool getGradient(const matrix<T>& src,const float& row, const float& col,
                      tpoint<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bicubicInterpolator& copy(const bicubicInterpolator& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bicubicInterpolator<T>& operator=(const bicubicInterpolator<T>& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

 private:

    /**
     * The grid square containing the desired point for interpolation
     * is considered here. The four points are numbered
     * counterclockwise starting at the low left corner.Within the
     * loop the function value, the gradients, and the cross
     * derivative are determined and stored in
     * y[4],y1[4],y2[4],y12[4].
     *
     * This is all the information needed from the given matrix to
     * compute the interpolation value(done in bicubicInterpolation ).
     */
    void centeredDifferencing(float row,float col,float& t,float& u,
                              float y[4],float y1[4], float y2[4],
                              float y12[4],const matrix<T>& src) const;


    /**
     * like centeredDifferencing but used at the border of the matrix
     */
    void centeredDifferencingBorder(float row,float col,float& t,float& u,
                                    float y[4],float y1[4], float y2[4],
                                    float y12[4],const matrix<T>& src) const;


    /**
     * This method computes the interpolation value(only for
     * matrixes).  The value is returned in result, whereas gradient1 and
     * gradient2 give the interpolated gradients
     */
    void bicubicInterpolation(float y[4],float y1[4],float y2[4],float y12[4],
                              float row,float col,float t,float u,
                              T& result,
                              float &gradient1,
                              float &gradient2) const;


    /**
     * This method computes the interpolation value for vectors using
     * the newton polynom of second degree.The values next to the
     * desired point and one to the left are used for the polynom
     */
    T newtonInterpolation(const int x[3],const T y[3],
                          const float & toinpol) const;

    /**
     * get bilinear interpolated value
     */
    void bilinearInterpolation(const matrix<T>& src,
                               float row,float col,T& result)const;

    /**
     * extraPolation is used for interpolation beyond the borders. The
     * boundaryType is decisive for the result
     */
    void extraPolation(float row,float col,const matrix<T>& src,T& result,
                       float &gradient1,float &gradient2) const;
  };

}

#endif

