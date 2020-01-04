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
 * file .......: ltiLaplacianPyramid_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.03.01
 * revisions ..: $Id: ltiLaplacianPyramid_template.h,v 1.7 2003/05/05 06:15:21 alvarado Exp $
 */

#include "ltiLaplacianPyramid.h"
#include "ltiDownsampling.h"
#include "ltiGaussKernels.h"
#include "ltiUpsampling.h"
#include "ltiFilledUpsampling.h"
#include "ltiMath.h"

namespace lti {

  /*
   * default constructor creates an empty laplacianPyramid;
   */
  template <class T>
  laplacianPyramid<T>::laplacianPyramid(const int& resolutions,
                                      const int& gaussianSize,
                                      const double& variance)
    : pyramid<T>(resolutions),kernelSize(gaussianSize),
      kernelVariance(variance) {
  }

  /*
   * create this laplacianPyramid as a copy of another laplacianPyramid
   * @param other the laplacianPyramid to be copied.
   */
  template <class T>
  laplacianPyramid<T>::laplacianPyramid(const laplacianPyramid<T>& other) {
    copy(other);
  }

  /*
   * destructor
   */
  template <class T>
  laplacianPyramid<T>::~laplacianPyramid() {
  }

  /**
   * return the size and variance of the used gaussian kernel
   */
  template <class T>
  void laplacianPyramid<T>::getKernelParameters(int& size,
                                               double& variance) const {
    size = kernelSize;
    variance = kernelVariance;
  }

  /**
   * set the kernel parameters
   *
   * @param size the size of the kernel
   * @param variance the variance for the gaussian kernel.  A negative
   *                 value will force the default variance of a gaussian
   *                 kernel with size <code>size</code>.
   * @param gaussian specify if for the channel upsampling a gaussian or
   *                 a rectangular kernel should be used.
   */
  template <class T>
  void laplacianPyramid<T>::setKernelParameters(const int& size,
                                               const double& variance) {
    kernelSize = size;
    kernelVariance = variance;
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source laplacianPyramid to be copied.
   * @return a reference to this object
   */
  template <class T>
  laplacianPyramid<T>&
    laplacianPyramid<T>::copy(const laplacianPyramid<T>& other) {

    pyramid<T>::copy(other);

    kernelVariance = other.kernelVariance;
    kernelSize = other.kernelSize;

    return *this;
  }

  /*
   * create a clone of this laplacianPyramid
   * @return a pointer to a copy of this laplacianPyramid
   */
  template <class T>
  mathObject* laplacianPyramid<T>::clone() const {
    return new laplacianPyramid(*this);
  }

  /**
   * generate pyramid
   */
  template <class T>
    void laplacianPyramid<T>::generate(const T& src) {

    if (size()<=0) {
      return;
    }

    gaussianPyramid<T> tmpPyram(size(),kernelSize,kernelVariance,true);
    tmpPyram.generate(src);

    generate(tmpPyram);
  }

  template<class T>
    void laplacianPyramid<T>::generate(const gaussianPyramid<T>& src) {

    if (src.size()<=1) {
      return;
    }

    resize(src.size());

    upsampling upsampler;
    upsampling::parameters parUp;

    parUp.boundaryType =  upsampling::parameters::Constant;
    upsampler.setParameters(parUp);

    T tmpChnl;
    int i;
    const int sizem1 = size()-1;
    for (i=0;i<sizem1;++i) {
      upsampler.apply(src.at(i+1),tmpChnl);

      subtract(src.at(i),tmpChnl,at(i));
    }
    at(i).copy(src.at(i));
  }

  /**
   * generate pyramid
   */
  template <class T>
    void laplacianPyramid<T>::generate(const T& src,
                                       const int& theResolutions) {

    resize(theResolutions,false);
    generate(src);
  }

  /**
   * generate pyramid
   */
  template <class T>
    void laplacianPyramid<T>::generate(const T& src,
                                       const lti::point& limit) {

    if ( (limit.x<1)||(limit.y<1) ) {
      return;
    }

    int steps = 1;
    lti::point thePoint = src.size();

    while (    (thePoint.x > limit.x)
            && (thePoint.y > limit.y) ) {
      thePoint.x /= 2;
      thePoint.y /= 2;
      steps++;
    }
    generate(src,steps);
  }

  /**
   * reconstruct an object
   */
  template <class T>
    T& laplacianPyramid<T>::reconstruct(const int& i, T& result) const {

    assert(i<size());

    typedef typename T::value_type value_type;

    if (i == size()-1) {
      result.copy(at(i));
      return result;
    }

    upsampling upsampler;
    upsampling::parameters uParam;

    kernel1D<float> kern1(-1,1,0.5f);
    kern1.at(0)=1.0f;

    sepKernel<float> kern(kern1);

    uParam.setKernel(kern);
    uParam.boundaryType = lti::downsampling::parameters::Constant;
    uParam.factor = point(2,2);
    upsampler.setParameters(uParam);

    int x=size()-1;

    T tmpChnl,partRec;

    upsampler.apply(at(x),tmpChnl);

    while (x>i) {
      add(at(x-1),tmpChnl,result);
      --x;
      if (x>i) {
        upsampler.apply(result,tmpChnl);
      }
    }

    return result;
  }

  /**
   * reconstruct an object
   */
  template <class T>
    pyramid<T>& laplacianPyramid<T>::reconstruct(const int& i,
                                                 pyramid<T>& result) const {

    assert(i<size());

    result.resize(size(),false); // discard old data!
    result.at(size()-1).copy(at(size()-1)); //low freq. information

    if (i == size()-1) {
      return result;
    }

    upsampling upsampler;
    upsampling::parameters uParam;

    kernel1D<float> kern1(-1,1,0.5f);
    kern1.at(0)=1.0f;

    sepKernel<float> kern(kern1);

    uParam.setKernel(kern);
    uParam.boundaryType = lti::downsampling::parameters::Constant;
    uParam.factor = point(2,2);
    upsampler.setParameters(uParam);

    int x=size()-1;

    T tmpChnl,partRec;

    upsampler.apply(at(x),tmpChnl);

    while (x>0) {
      add(at(x-1),tmpChnl,result.at(x-1));
      --x;
      if (x>0) {
        upsampler(result.at(x),tmpChnl);
      }
    }

    return result;
  }

  /*
   * subtract b from a, and leave the result in c, which will have the size
   * of a, even if the sizes of a and b differ.
   */
  template<class T>
  void laplacianPyramid<T>::subtract(const T& a,const T& b, T& c) const {
    if (a.size() == b.size()) {
      c.subtract(a,b);
    } else {
      typedef typename T::value_type value_type;
      c.resize(a.size(),value_type(),false,false);

      const int maxy=min(a.rows(),b.rows());
      const int maxx=min(a.columns(),b.columns());

      const value_type* ptra;
      const value_type* ptrb;
      const value_type* ptre;
      value_type* ptrc;

      int y;

      for (y=0;y<maxy;++y) {
        for (ptra = &a.at(y,0),ptre = ptra+maxx,
               ptrb = &b.at(y,0),ptrc = &c.at(y,0);
             ptra != ptre;
             ++ptra,++ptrb,++ptrc) {
          (*ptrc)=(*ptra)-(*ptrb);
        }
      }

      if (maxx < c.columns()) {
        c.fill(value_type(0),0,maxy-1,maxx,c.lastColumn());
      }

      if (maxy < c.rows()) {
        c.fill(value_type(0),maxy,c.lastRow(),0,c.lastColumn());
      }

    }
  }

  /*
   * add a and b, and leave the result in c, which will have the size
   * of a, even if the sizes of a and b differ.
   */
  template<class T>
  void laplacianPyramid<T>::add(const T& a,const T& b, T& c) const {
    if (a.size() == b.size()) {
      c.add(a,b);
    } else {
      typedef typename T::value_type value_type;

      c.resize(a.size(),value_type(),false,false);

      const int maxy=min(a.rows(),b.rows());
      const int maxx=min(a.columns(),b.columns());


      const value_type* ptra;
      const value_type* ptrb;
      const value_type* ptre;
      value_type* ptrc;

      int y;

      for (y=0;y<maxy;++y) {
        for (ptra = &a.at(y,0),ptre = ptra+maxx,
               ptrb = &b.at(y,0),ptrc = &c.at(y,0);
             ptra != ptre;
             ++ptra,++ptrb,++ptrc) {
          (*ptrc)=(*ptra)+(*ptrb);
        }
      }

      if (maxx < c.columns()) {
        c.fill(value_type(0),0,maxy-1,maxx,c.lastColumn());
      }

      if (maxy < c.rows()) {
        c.fill(value_type(0),maxy,c.lastRow(),0,c.lastColumn());
      }

    }
  }

}

