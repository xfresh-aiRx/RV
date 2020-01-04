/*
 * Copyright (C) 1998 - 2005
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

 
/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiScaleSpacePyramid.cpp
 * authors ....: Frederik Lange
 * organization: LTI, RWTH Aachen
 * creation ...: 17.03.2003
 * revisions ..: $Id: ltiScaleSpacePyramid_template.h,v 1.7 2005/05/27 10:02:50 sltilib Exp $
 */

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

#include "ltiConvolution.h"
#include "ltiGaussKernels.h"
#include "ltiScaling.h"

namespace lti {

  
  // --------------------------------------------------
  // scaleSpacePyramid<T>
  // --------------------------------------------------

  // default constructor
  template<class T>
  scaleSpacePyramid<T>::scaleSpacePyramid()
    : pyramid<T>() {
    bilinIpl.setBoundaryType(Constant);
    biquadIpl.setBoundaryType(Constant);
  }

  template<class T>
  scaleSpacePyramid<T>::scaleSpacePyramid(const int levels,
                                          const parameters& par)
    : pyramid<T>() {
    bilinIpl.setBoundaryType(Constant);
    biquadIpl.setBoundaryType(Constant);
    resize(levels);
    setParameters(par);
  }


  // copy constructor
  template<class T>
  scaleSpacePyramid<T>::scaleSpacePyramid(
                                           const scaleSpacePyramid<T>& other) {
    bilinIpl.setBoundaryType(Constant);
    biquadIpl.setBoundaryType(Constant);

    copy(other);
  }

  // destructor
  template<class T>
  scaleSpacePyramid<T>::~scaleSpacePyramid() {
  }

  // returns the name of this type
  template<class T>
  const char* scaleSpacePyramid<T>::getTypeName() const {
    return "scaleSpacePyramid<T>";
  }

  // copy member
  template<class T>
  scaleSpacePyramid<T>&
  scaleSpacePyramid<T>::copy(const scaleSpacePyramid<T>& other) {
    pyramid<T>::copy(other);
    param.copy(other.param);
    levelFactor.resize(this->size(),0.0,false,false);
    initLevelFactor();
    return (*this);
  }

  // alias for copy member
  template<class T>
  scaleSpacePyramid<T>&
  scaleSpacePyramid<T>::operator=(const scaleSpacePyramid<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  mathObject* scaleSpacePyramid<T>::clone() const {
    return new scaleSpacePyramid<T>(*this);
  }

  // get parameters
  template<class T>
  const typename scaleSpacePyramid<T>::parameters& 
  scaleSpacePyramid<T>::getParameters() const {
    return param;
  }

  // set parameters
  template<class T>
  bool scaleSpacePyramid<T>::setParameters(const parameters& par) {
    if ((par.factor > 0.0) && (par.factor < 1.0)) {
      param.copy(par);
      initLevelFactor();
      return true;
    }
    return false;
  }
  
  /*
   * Access with constant boundary
   */
  template<class T>
  inline typename scaleSpacePyramid<T>::value_type
  scaleSpacePyramid<T>::cstAt(const T& img,
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
   * Map the coordinates of the given level into coordinates of level 0
   */
  template<class T>
  inline void scaleSpacePyramid<T>::mapToLevel0(const int level,
                                                const float row,
                                                const float col,
                                                float& row0,
                                                float& col0) const {
    row0=static_cast<float>(row/levelFactor.at(level));
    col0=static_cast<float>(col/levelFactor.at(level));
  }

  /*
   * Map the coordinates of level 0 into the coordinates of the given level
   */
  template<class T>
  inline void scaleSpacePyramid<T>::mapToLevel(const int level,
                                               const float& row0,
                                               const float& col0,
                                               float& row,
                                               float& col) const {
    row=static_cast<float>(row0*levelFactor.at(level));
    col=static_cast<float>(col0*levelFactor.at(level));
  }

  /*
   * Map the coordinates from level "from" to level "to"
   */
  template<class T>
  inline void scaleSpacePyramid<T>::mapToLevel(const int fromLevel,
                                               const int toLevel,
                                               const float& rowFrom,
                                               const float& colFrom,
                                               float& rowTo,
                                               float& colTo) const {
    rowTo=rowFrom*levelFactor.at(toLevel)/levelFactor.at(fromLevel);
    colTo=colFrom*levelFactor.at(toLevel)/levelFactor.at(fromLevel);
  }

  // -------------------------------------------------------------------
  // Generate methods
  // -------------------------------------------------------------------
  template<class T>
  void scaleSpacePyramid<T>::generate(const T& src) {
    if (this->size()<=0) {
      // nothing to be done, since the pyramid hasn't been resized.
      return;
    }

    // determine variance and size of the kernel
    int kernelS;
    double kernelVar;
    if (param.automaticKernel) {
      // variance = stdDev^2
      kernelVar = 0.5*(1.0/(param.factor*param.factor)-1.0);
      // kernel rule: 5% of the Gaussian should be reached with the given
      // size.
      kernelS = max(3,1+2*iround(sqrt(-kernelVar*2.0*log(0.05))));
    } else {
      kernelS = param.kernelSize;
      kernelVar = param.kernelVariance;
    }
    
    convolution::parameters convPar;
    convPar.setKernel(gaussKernel2D<float>(kernelS,kernelVar));
    convPar.boundaryType = Constant;
    convolution fil(convPar);

    // get the source at level 0
    this->at(0).copy(src);

    T tmp;
    int i;
    
    // generate each level of the pyramid  
    levelFactor.at(0)=1.0;

    scaling::parameters scalingPar;
    scalingPar.interpolatorType = param.interpolatorType;
    scaling scaler(scalingPar);
    tpoint<float> scalingFactor;

    for (i = 1; i < this->size(); i++) {
      if (param.gaussian) {
        fil.apply(this->at(i-1)); // smooth the current channel with a gaussian
      }

      scalingFactor.x = static_cast<float>(this->at(0).size().x*
                                           levelFactor.at(i)/
                                           this->at(i-1).size().x);
      scalingFactor.y = static_cast<float>(this->at(0).size().y*
                                           levelFactor.at(i)/
                                           this->at(i-1).size().y);

      scaler.apply(scalingFactor,this->at(i-1),this->at(i));
    }
  }
  
  /*
   * generate the pyramid of the given object.
   *
   * The pyramid will contain the number of resolutions specified 
   * by theResolutions.
   *
   * The resolution "0" will correspond the the original channel, and
   * the resolution i+1 is always by "factor" specified in the
   * parameters smaller than the resolution i.
   */
  template<class T>
  void scaleSpacePyramid<T>::generate(const T& src,const int& theResolutions) {
    resize(theResolutions, false);
    generate(src);
  }

  /*
   * Initialize values of levelFactor vector
   */
  template<class T>
  void scaleSpacePyramid<T>::initLevelFactor() {
    int i;
    if (levelFactor.empty()) {
      return;
    }

    levelFactor.at(0)=1.0;
    for (i=1;i<levelFactor.size();++i) {
      levelFactor.at(i)=pow(param.factor, double(i)); 
    }
  }
  
  /*
   * Change the number of resolutions of the pyramdi
   */
  template<class T>
  void scaleSpacePyramid<T>::resize(const int& resolutions,
				    const bool& copyData) {

    pyramid<T>::resize(resolutions,copyData);
    levelFactor.resize(resolutions,0.0,false,false);
    initLevelFactor();
  }

  /*
   * Get an "in level" bilinear interpolation.
   *
   * The given scale \a s must be in the pyramid.
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type
  scaleSpacePyramid<T>::nearestAt(const float y,
                                  const float x,
                                  const int s) const {
    assert((static_cast<uint32>(y) <=
            static_cast<uint32>(this->at(0).lastRow())) &&
           (static_cast<uint32>(x) <=
            static_cast<uint32>(this->at(0).lastColumn())));

    return nnIpl.interpolate(this->at(s),
                             static_cast<float>(y*levelFactor.at(s)),
                             static_cast<float>(x*levelFactor.at(s)));
  }

  /*
   * Get an "in level" bilinear interpolation.
   *
   * The given scale \a s must be in the pyramid.
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type
  scaleSpacePyramid<T>::bilinearAt(const float y,
                                   const float x,
                                   const int s) const {
    assert((static_cast<uint32>(y) <=
            static_cast<uint32>(this->at(0).lastRow())) &&
           (static_cast<uint32>(x) <=
            static_cast<uint32>(this->at(0).lastColumn())));

    return bilinIpl.interpolate(this->at(s),
                                static_cast<float>(y*levelFactor.at(s)),
                                static_cast<float>(x*levelFactor.at(s)));
  }
 
  /*
   * Get an "in level" biquadratic interpolation.
   *
   * The given scale \a s must be in the pyramid.
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type 
  scaleSpacePyramid<T>::biquadraticAt(const float y,
                                      const float x,
                                      const int s) const {
    assert((static_cast<uint32>(y) <=
            static_cast<uint32>(this->at(0).lastRow())) &&
           (static_cast<uint32>(x) <=
            static_cast<uint32>(this->at(0).lastColumn())));

    return biquadIpl.interpolate(this->at(s),
                                 static_cast<float>(y*levelFactor.at(s)),
                                 static_cast<float>(x*levelFactor.at(s)));
  }
  
  /*
   * Get an "in level" quadratic interpolation.
   *
   * The given scale \a s must be in the pyramid.  
   *
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type 
  scaleSpacePyramid<T>::quadraticAt(const float y,
                                    const float x,
                                    const int s) const {

    const float xp = x*levelFactor.at(s);
    const float yp = y*levelFactor.at(s);

    // precomputations for x
    int truncX = static_cast<int>(xp);
    if (xp<0) {
      --truncX;
    }
    float fractX = xp - static_cast<float>(truncX);
    if (fractX <= 0.5f) {
      --truncX;
    } else {
      fractX-=1.0f;
    }

    // precomputation for y
    int truncY = static_cast<int>(yp);
    if (yp<0) {
      --truncY;
    }
    float fractY = yp - static_cast<float>(truncY);
    if (fractY <= 0.5f) {
      --truncY;
    } else {
      fractY-=1.0f;
    }
    
    const T& img = this->at(s);
    const int y1 = truncY+1;
    const int y2 = truncY+2;
    const int x1 = truncX+1;
    const int x2 = truncX+2;

    const value_type v4 = cstAt(img,y1,x1);

    const value_type v0 = cstAt(img,truncY,truncX)-v4;
    const value_type v1 = cstAt(img,truncY,x1)-v4;
    const value_type v2 = cstAt(img,truncY,x2)-v4;
    const value_type v3 = cstAt(img,y1,truncX)-v4;
    const value_type v5 = cstAt(img,y1,x2)-v4;
    const value_type v6 = cstAt(img,y2,truncX)-v4;
    const value_type v7 = cstAt(img,y2,x1)-v4;
    const value_type v8 = cstAt(img,y2,x2)-v4;

    // This is the SVD solution for the quadratic approximation
    const value_type t1 = v1+v7;
    const value_type t2 = v3+v5;
    const value_type t3 = v0+v8;
    const value_type t4 = v2+v6;
    const value_type t5 = t3+t4;
    const value_type t6 = v8-v0;
    const value_type t7 = v6-v2;
    
    const value_type a=static_cast<value_type>(t5*0.1 - t1*0.2 + t2*0.3);
    const value_type b=static_cast<value_type>((t3-t4)*0.25);
    const value_type c=static_cast<value_type>(t5*0.1 - t2*0.2 + t1*0.3);
    const value_type d=static_cast<value_type>((t6+v5-v3-t7)/6.0);
    const value_type e=static_cast<value_type>((t7+v7+t6-v1)/6.0);

    return static_cast<value_type>((a*fractX + b*fractY + d)*fractX +
                                   (c*fractY + e)*fractY + v4);
  }

  /*
   * Get a "between-scales" trilinear interpolation.
   *
   * The given scale \a s must be in the pyramid.
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type 
  scaleSpacePyramid<T>::trilinearAt(const float y,
                                    const float x,
                                    const float s) const {

    const int last = this->size()-1;
    assert((s>=0.0f) && (s<=last));

    const int is = static_cast<int>(s);
    const float fs = s - static_cast<float>(is);

    const value_type l0 = bilinearAt(y,x,is);
    if (is < last) { 
      const value_type l1 = bilinearAt(y,x,is+1);    
      return bilinIpl.compute(fs,l0,l1);
    } else {
      return l0;
    }
  }
 
  /*
   * Get a "between-scales" triquadratic interpolation.
   *
   * The given scale \a s must be in the pyramid.
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type 
  scaleSpacePyramid<T>::triquadraticAt(const float y,
                                       const float x,
                                       const float s) const {

    int truncS = static_cast<int>(s);
    if (s<0) {
      --truncS;
    }
    float fractS = s - static_cast<float>(truncS);
    if (fractS <= 0.5f) {
      --truncS;
    } else {
      fractS-=1.0f;
    }

    const int ts0 = (truncS >= 0) ? truncS : 0;
    const int ts1 = (truncS<=this->size()-2) ? truncS+1 : this->size()-1;
    const int ts2 = (truncS<=this->size()-3) ? truncS+2 : this->size()-1;
    
    const value_type l0 = biquadraticAt(y,x,ts0);    
    const value_type l1 = biquadraticAt(y,x,ts1);
    const value_type l2 = biquadraticAt(y,x,ts2);

    return biquadIpl.compute(fractS,l0,l1,l2);
  }

  /*
   * Get an "between-scales" quadratic interpolation.
   *
   * The given scale \a s must be in the pyramid.  
   *
   * Since the pixel (x,y) at level \a s has 26 neighbors and the
   * quadratic function only ten coefficients, an SVD solution with the
   * least square error will be used.
   *
   */
  template<class T>
  typename scaleSpacePyramid<T>::value_type
  scaleSpacePyramid<T>::quadraticAt(const float y,
                                    const float x,
                                    const float s) const {

    int truncS = static_cast<int>(s);
    if (s<0) {
      --truncS;
    }
    float fractS = s - static_cast<float>(truncS);
    if (fractS <= 0.5f) {
      --truncS;
    } else {
      fractS-=1.0f;
    }

    const int ts0 = (truncS >= 0) ? truncS : 0;
    const int ts1 = (truncS<=this->size()-2) ? truncS+1 : this->size()-1;
    const int ts2 = (truncS<=this->size()-3) ? truncS+2 : this->size()-1;

    const value_type v0 = quadraticAt(y,x,ts0);
    const value_type v1 = quadraticAt(y,x,ts1);
    const value_type v2 = quadraticAt(y,x,ts2);

    return biquadIpl.compute(fractS,v0,v1,v2);
  }

  template<class T>
  float scaleSpacePyramid<T>::getScaleForRadius(const float radius) const {
    return static_cast<float>(-log(2.0*radius)/log(param.factor));
  }

  template<class T>
  float scaleSpacePyramid<T>::getRadiusForScale(const float scale) const {
    return static_cast<float>(0.5/pow(param.factor,double(scale)));
  }

  template<class T>
  inline const double& 
  scaleSpacePyramid<T>::getLevelScale(const int level) const {
    return levelFactor.at(level);
  }

  template<class T>
  bool scaleSpacePyramid<T>::interpolateMaximum(const int row, 
                                                const int col,
                                                const int level,
                                                float& spRow,
                                                float& spCol) const {
    return (interpolateExtremum(row,col,level,spRow,spCol) == Maximum);
  }

  template<class T>
  typename scaleSpacePyramid<T>::eExtremaType 
  scaleSpacePyramid<T>::interpolateExtremum(const int row, 
                                            const int col,
                                            const int level,
                                            float& spRow,
                                            float& spCol) const {

    eExtremaType ret = NoExtremum;

    const int x0 = col-1;
    const int x1 = col;
    const int x2 = col+1;
    
    const int y0 = row-1;
    const int y1 = row;
    const int y2 = row+1;       

    const T& img = this->at(level);

    const value_type v4 = cstAt(img,y1,x1);

    const value_type v0 = cstAt(img,y0,x0)-v4;
    const value_type v1 = cstAt(img,y0,x1)-v4;
    const value_type v2 = cstAt(img,y0,x2)-v4;
    const value_type v3 = cstAt(img,y1,x0)-v4;
    //               v4
    const value_type v5 = cstAt(img,y1,x2)-v4;
    const value_type v6 = cstAt(img,y2,x0)-v4;
    const value_type v7 = cstAt(img,y2,x1)-v4;
    const value_type v8 = cstAt(img,y2,x2)-v4;

    // This is the SVD solution for the quadratic approximation
    const value_type t1 = v1+v7;
    const value_type t2 = v3+v5;
    const value_type t3 = v0+v8;
    const value_type t4 = v2+v6;
    const value_type t5 = t3+t4;

    const value_type a=static_cast<value_type>(t5*0.1 - t1*0.2 + t2*0.3);
    const value_type c=static_cast<value_type>(t5*0.1 - t2*0.2 + t1*0.3);

    if ((a<value_type(0)) && (c<value_type(0))) {
      ret = Maximum;
    } else if ((a>value_type(0)) && (c>value_type(0))) {
      ret = Minimum;
    } else {
      return ret;
    }

    const value_type t6 = v8-v0;
    const value_type t7 = v6-v2;

    const value_type b=static_cast<value_type>((t3-t4)*0.25);   
    const value_type d=static_cast<value_type>((t6+v5-v3-t7)/6.0);
    const value_type e=static_cast<value_type>((t7+v7+t6-v1)/6.0);
    
    const value_type det = b*b-a*c*4;
    if (det != static_cast<value_type>(0)) {      
      spCol = (c*d*2 - b*e)/det;
      spRow = (a*e*2 - b*d)/det;
    } else {
      // possibly many maxima?
      spCol = col;
      spRow = row;
      return NoExtremum; // too unstable
    }

    if ((abs(spCol)<static_cast<value_type>(1)) && 
        (abs(spRow)<static_cast<value_type>(1))) {
      spRow+=row;
      spCol+=col;
      return ret;
    }
    
    return NoExtremum;
  }



  template<class T>
  bool scaleSpacePyramid<T>::interpolateMaximum(const int row, 
                                                const int col,
                                                const int level,
                                                float& spRow,
                                                float& spCol,
                                                float& spLevel) const {
    return (interpolateExtremum(row,col,level,spRow,spCol,spLevel) == Maximum);
  }

  template<class T>
  typename scaleSpacePyramid<T>::eExtremaType
  scaleSpacePyramid<T>::interpolateExtremum(const int row, 
                                            const int col,
                                            const int level,
                                            float& spRow,
                                            float& spCol,
                                            float& spLevel) const {
    eExtremaType ret = NoExtremum;

    // coordinates at the middle level
    const int x10 = col-1;
    const int x11 = col;
    const int x12 = col+1;
    
    const int y10 = row-1;
    const int y11 = row;
    const int y12 = row+1;

    // the three real levels, i.e. casted to real ones
    const int lev0 = (level > 0) ? level-1 : 0;
    const int lev1 = level;
    const int lev2 = (level < this->size()-1) ? level+1 : this->size()-1;

    // the level channels
    const T& sc0 = this->at(lev0);
    const T& sc1 = this->at(lev1);
    const T& sc2 = this->at(lev2);

    // the coordinates at a higher resolution
    const int x01 = iround(x11/param.factor);
    const int x00 = x01-1;
    const int x02 = x01+1;

    const int y01 = iround(y11/param.factor);
    const int y00 = y01-1;
    const int y02 = y01+1;
    
    // the coordinates at a lower resolution
    const int x21 = iround(x11*param.factor);
    const int x20 = x21-1;
    const int x22 = x21+1;

    const int y21 = iround(y11*param.factor);
    const int y20 = y21-1;
    const int y22 = y21+1;

    // the 26 values used as base to find the maximum
    const value_type v13= cstAt(sc1,y11,x11);

    const value_type v0 = cstAt(sc0,y00,x00)-v13;
    const value_type v1 = cstAt(sc0,y00,x01)-v13;
    const value_type v2 = cstAt(sc0,y00,x02)-v13;
    const value_type v3 = cstAt(sc0,y01,x00)-v13;
    const value_type v4 = cstAt(sc0,y01,x01)-v13;
    const value_type v5 = cstAt(sc0,y01,x02)-v13;
    const value_type v6 = cstAt(sc0,y02,x00)-v13;
    const value_type v7 = cstAt(sc0,y02,x01)-v13;
    const value_type v8 = cstAt(sc0,y02,x02)-v13;

    const value_type v9 = cstAt(sc1,y10,x10)-v13;
    const value_type v10= cstAt(sc1,y10,x11)-v13;
    const value_type v11= cstAt(sc1,y10,x12)-v13;
    const value_type v12= cstAt(sc1,y11,x10)-v13;
    //               v13= cstAt(sc1,y11,x11)-v13;
    const value_type v14= cstAt(sc1,y11,x12)-v13;
    const value_type v15= cstAt(sc1,y12,x10)-v13;
    const value_type v16= cstAt(sc1,y12,x11)-v13;
    const value_type v17= cstAt(sc1,y12,x12)-v13;

    const value_type v18= cstAt(sc2,y20,x20)-v13;
    const value_type v19= cstAt(sc2,y20,x21)-v13;
    const value_type v20= cstAt(sc2,y20,x22)-v13;
    const value_type v21= cstAt(sc2,y21,x20)-v13;
    const value_type v22= cstAt(sc2,y21,x21)-v13;
    const value_type v23= cstAt(sc2,y21,x22)-v13;
    const value_type v24= cstAt(sc2,y22,x20)-v13;
    const value_type v25= cstAt(sc2,y22,x21)-v13;
    const value_type v26= cstAt(sc2,y22,x22)-v13;

    // This is the SVD solution for the quadratic approximation
    const value_type t1 = (v3+v5+v21+v23);
    const value_type t2 = (v9+v11+v15+v17);
    const value_type t3 = (v1+v7+v19+v25);
    const value_type t4 = v12+v14;
    const value_type t5 = v10+v16;
    const value_type t6 = v4+v22;
    const value_type t42 = 
      static_cast<value_type>((v0+v2+v6+v8+v18+v20+v24+v26)/42.0);

    // The coefficients of the approximation
    // f(x,y,s) = a*x^2 + b*x*y + c*x*s + d*y^2 + e*y*s + f*s^2 +
    //            g*x + h*y + i*s + k
    const value_type a = 
      static_cast<value_type>(t42 -
                              t3/10.5 + 
                              (t1+t2)/14.0 -
                              (t5+t6)/21.0 +
                              t4/8.4);
    const value_type d=
      static_cast<value_type>(t42 -
                              t1/10.5 +
                              (t2+t3)/14.0 -
                              (t4+t6)/21.0 +
                              t5/8.4);
    const value_type f=
      static_cast<value_type>(t42 -
                              t2/10.5 +
                              (t3+t1)/14.0 -
                              (t5+t4)/21.0 +
                              t6/8.4);

    // Check if there is a maximum
    if ((a < value_type(0)) &&
        (d < value_type(0)) &&
        (f < value_type(0))) {
      ret = Maximum;
    } else if ((a > value_type(0)) &&
               (d > value_type(0)) &&
               (f > value_type(0))) {
      ret = Minimum;
    } else {
      return ret;
    }

    const value_type t7 = v0+v26;
    const value_type t8 = v6+v20;
    const value_type t9 = v2+v24;
    const value_type t10 = v8+v18;
    const value_type t11 = v8+v17+v26;
    const value_type t12 = v0+v9+v18;

    const value_type b =
      static_cast<value_type>((t7+v8+v9+v17+v18 - t9-t8-v11-v15)/12.0);
    const value_type c =
      static_cast<value_type>((t7+v3+v6+v20+v23 - t9-v5-t10-v21)/12.0);

    const value_type e=
      static_cast<value_type>((t7+v1+v2+v24+v25 - t8-v7-t10-v19)/12.0);

    const value_type g=
      static_cast<value_type>((t11 + v2+v5+v11+v14+v20+v23 -
                               t12 - v3-v6-v12-v15-v21-v24)/18.0);

    const value_type h=
      static_cast<value_type>((t11 + v6+v7+v15+v16+v24+v25 -
                               t12 - v1-v2-v10-v11-v19-v20)/18.0);

    const value_type i=
      static_cast<value_type>((v18+v19+v20+v21+v22+v23+v24+v25+v26 -
                               v0-v1-v2-v3-v4-v5-v6-v7-v8)/18.0);

    // now compute the maximum
    const value_type det = (a*e*e + b*b*f + c*c*d - b*c*e - a*d*f*4)*2;
    if (det != static_cast<value_type>(0)) {
      spCol = (g*(d*f*4-e*e) + h*(c*e - b*f*2) + i*(b*e - c*d*2))/det;
      spRow = (g*(c*e-b*f*2) + h*(a*f*4-c*c)   - i*(a*e*2-b*c))/det;
      spLevel=(g*(b*e-c*d*2) - h*(a*e*2-b*c)   + i*(a*d*4-b*b))/det; 
    } else {
      spCol = static_cast<float>(col);
      spRow = static_cast<float>(row);
      spLevel = static_cast<float>(level);
      return NoExtremum; // too unstable
    }

    if ((abs(spCol)<=static_cast<value_type>(0.51)) && 
        (abs(spRow)<=static_cast<value_type>(0.51)) &&
        (abs(spLevel)<=static_cast<value_type>(0.51))) {

      spRow+=row;
      spCol+=col;
      spLevel+=level;

      return ret;
    }
    
    return NoExtremum;
  }

  /*
   * Accumulate
   */
  template<class T>
  inline const typename scaleSpacePyramid<T>::value_type& 
  scaleSpacePyramid<T>::tacc(const value_type& in,
                             value_type& out) const {
    out+=in;
    return in;
  }

  /*
   * Check if the given pixel at the given level (in the respective
   * coordinate system) is a maximum in the 3x3 neighborhood.
   *
   * @param row row of the point to be checked.  It must be between 0 and
   *            at(level).lastRow(), but for the borders \a false will be
   *            always returned, as a "constant boundary" will be always
   *            assumed.
   * @param col column of the point to be checked.  It must be between 0 and
   *            at(level).lastColumn(), but for the borders false will be
   *            always returned, as a "constant boundary" will be always
   *            assumed.
   * @param level the level where the maximum has to be checked.
   */ 
  template<class T>
  bool scaleSpacePyramid<T>::checkMaximum(const int row,
                                          const int col,
                                          const int level) const {
    const T& lev = this->at(level);
    if ((row>0) && (row < lev.lastRow()) &&
        (col>0) && (col < lev.lastColumn())) {
      const value_type val = lev.at(row,col);
#if 1
      value_type acc(0);
      // check that all neighbors are less or equal the center point and
      // at least one of them is smaller 
      return ((val >= tacc(lev.at(row-1,col),acc)) &&
              (val >= tacc(lev.at(row+1,col),acc)) &&
              (val >= tacc(lev.at(row,col-1),acc)) &&
              (val >= tacc(lev.at(row,col+1),acc)) &&
              (val >= tacc(lev.at(row-1,col-1),acc)) &&
              (val >= tacc(lev.at(row+1,col+1),acc)) &&
              (val >= tacc(lev.at(row-1,col+1),acc)) &&
              (val >= tacc(lev.at(row+1,col-1),acc)) &&
              (acc < val*8));
#else
      // check that all neighbors are less or equal the center point and
      // at least one of them is smaller 
      return ((val > lev.at(row-1,col)) &&
              (val > lev.at(row+1,col)) &&
              (val > lev.at(row,col-1)) &&
              (val > lev.at(row,col+1)) &&
              (val > lev.at(row-1,col-1)) &&
              (val > lev.at(row+1,col+1)) &&
              (val > lev.at(row-1,col+1)) &&
              (val > lev.at(row+1,col-1)) );
#endif
    }

    return false;
  }
    
  /*
   * Check if the given pixel at the given level (in the respective
   * coordinates) is a maximum in its 3x3x3 neighborhood
   *
   * @param row row of the point to be checked.  It must be between 0 and
   *            at(level).lastRow(), but for the borders false will be
   *            always returned, as a "constant boundary" will be always
   *            assumed.
   * @param col column of the point to be checked.  It must be between 0 and
   *            at(level).lastColumn(), but for the borders false will be
   *            always returned, as a "constant boundary" will be always
   *            assumed.
   * @param level the level where the maximum has to be checked.
   */ 
  template<class T>
  bool scaleSpacePyramid<T>::checkInterlevelMaximum(const int row,
                                                    const int col,
                                                    const int level) const {
    const T& lev = this->at(level);
    if ((level>0) && (level < this->size()-1)) {
      if (checkMaximum(row,col,level)) {
        // the level channels
        const T& sc0 = this->at(level-1);
        const T& sc2 = this->at(level+1);

        // the coordinates at a higher resolution
        const int x01 = iround(col/param.factor);
        const int x00 = max(0,x01-1);
        const int x02 = min(sc0.lastColumn(),x01+1);

        const int y01 = iround(row/param.factor);
        const int y00 = max(0,y01-1);
        const int y02 = min(sc0.lastRow(),y01+1);
    
        // the coordinates at a lower resolution
        const int x21 = iround(col*param.factor);
        const int x20 = max(0,x21-1);
        const int x22 = min(sc2.lastColumn(),x21+1);
        
        const int y21 = iround(row*param.factor);
        const int y20 = max(0,y21-1);
        const int y22 = min(sc2.lastRow(),y21+1);

        const value_type val = lev.at(row,col);
        return ((val > sc0.at(y01,x01)) &&
                (val > sc2.at(y21,x21)) &&
                (val > sc0.at(y00,x01)) &&
                (val > sc2.at(y22,x21)) &&
                (val > sc0.at(y02,x01)) &&
                (val > sc2.at(y20,x21)) &&
                (val > sc0.at(y01,x00)) &&
                (val > sc2.at(y21,x22)) &&
                (val > sc0.at(y01,x02)) &&
                (val > sc2.at(y21,x20)) &&
                (val > sc0.at(y00,x00)) &&
                (val > sc2.at(y22,x22)) &&
                (val > sc0.at(y02,x02)) &&
                (val > sc2.at(y20,x20)) &&
                (val > sc0.at(y00,x02)) &&
                (val > sc2.at(y22,x20)) &&
                (val > sc0.at(y02,x00)) &&
                (val > sc2.at(y20,x22)));
      }
    }
    return false;
  }


}                                   

#include "ltiUndebug.h"
