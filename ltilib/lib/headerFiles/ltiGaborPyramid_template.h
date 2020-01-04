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
 * file .......: ltiGaborPyramid_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiGaborPyramid_template.h,v 1.10 2004/05/03 12:18:12 alvarado Exp $
 */

#include "ltiGaborPyramid.h"
#include "ltiDownsampling.h"
#include "ltiConvolution.h"
#include "ltiGaussKernels.h"
#include "ltiGaborKernels.h"
#include "ltiUpsampling.h"
#include "ltiFilledUpsampling.h"
#include "ltiMath.h"

namespace lti {

  /*
   * default constructor creates an empty gaborPyramid;
   */
  template <class T>
  gaborPyramid<T>::gaborPyramid(const int& resolutions,
                                const double& orientation,
                                const int& gaborSize,
                                const double& bandwidth,
                                const bool& upsampleWithGaussian)
    : gaussianPyramid<T>(resolutions,gaborSize,-1,upsampleWithGaussian),
      kernelOrientation(orientation), kernelBandwidth(bandwidth) {
  }

  /*
   * create this gaborPyramid as a copy of another gaborPyramid
   * @param other the gaborPyramid to be copied.
   */
  template <class T>
  gaborPyramid<T>::gaborPyramid(const gaborPyramid<T>& other) : gaussianPyramid<T>() {
    copy(other);
  }

  /*
   * destructor
   */
  template <class T>
  gaborPyramid<T>::~gaborPyramid() {
  }

  /*
   * return the size and variance of the used gabor kernel
   */
  template <class T>
  void gaborPyramid<T>::getKernelParameters(double& orientation,
                                            int& size,
                                            double& bandwidth,
                                            bool& usplGaussian) const {

    double tmp;
    gaussianPyramid<T>::getKernelParameters(size,tmp,usplGaussian);
    orientation = kernelOrientation;
    bandwidth = kernelBandwidth;
  }

  /*
   * set the kernel parameters
   *
   * @param size the size of the kernel
   * @param variance the variance for the gabor kernel.  A negative
   *                 value will force the default variance of a gabor
   *                 kernel with size <code>size</code>.
   * @param gabor specify if for the channel upsampling a gabor or
   *                 a rectangular kernel should be used.
   */
  template <class T>
  void gaborPyramid<T>::setKernelParameters(const double& orientation,
                                            const int& size,
                                            const double& bandwidth,
                                            const bool& usplGaussian) {
    gaussianPyramid<T>::setKernelParameters(size,-1,usplGaussian);
    kernelOrientation = orientation;
    kernelBandwidth = bandwidth;
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source gaborPyramid to be copied.
   * @return a reference to this object
   */
  template <class T>
  gaborPyramid<T>&
    gaborPyramid<T>::copy(const gaborPyramid<T>& other) {

    gaussianPyramid<T>::copy(other);

    kernelOrientation = other.kernelOrientation;
    kernelBandwidth = other.kernelBandwidth;

    return *this;
  }

  /*
   * create a clone of this gaborPyramid
   * @return a pointer to a copy of this gaborPyramid
   */
  template <class T>
  mathObject* gaborPyramid<T>::clone() const {
    return new gaborPyramid(*this);
  }

  /*
   * generate pyramid
   */
  template <class T>
  void gaborPyramid<T>::generate(const T& src) {

    if (this->size()<=0) {
      return;
    }

    T tmp(src);

    // downsampling filter
    downsampling downsampler;
    downsampling::parameters dParam;

    typedef typename T::value_type value_type;
    gaussKernel2D<value_type> kern(this->kernelSize,this->kernelVariance);
    dParam.setKernel(kern);
    dParam.boundaryType = lti::Constant;
    dParam.factor = point(2,2);

    downsampler.setParameters(dParam);

    // gabor filter
    double w1; // the first gabor frequency is a function of the bandwidth
    double sigma1;

    w1 = 2*Pi*0.5/(1+tan(kernelBandwidth/2.0));
    sigma1 = 2.0*log(2.0)/(w1*tan(kernelBandwidth/2.0));

    gaborKernelSep<value_type> gabor(this->kernelSize,
                                     sigma1*sigma1,
                                     w1,
                                     kernelOrientation);

    lti::convolution conv;
    lti::convolution::parameters cparam;

    cparam.setKernel(gabor);
    cparam.boundaryType = lti::Constant;

    conv.setParameters(cparam);

    int i=0;
    conv.apply(tmp,this->at(i));
    for (i=1;i<this->size();++i) {
      downsampler.apply(tmp);
      conv.apply(tmp,this->at(i));
    }
  }

  /*
   * generate pyramid
   */
  template <class T>
  void gaborPyramid<T>::generate(const T& src, const int& theResolutions) {

    this->resize(theResolutions,false);
    generate(src);
  }

  /*
   * generate pyramid
   */
  template <class T>
  void gaborPyramid<T>::generate(const T& src,
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

}

