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
 * file .......: ltiLaplacianPyramid.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.03.01
 * revisions ..: $Id: ltiLaplacianPyramid.h,v 1.6 2003/11/18 08:19:11 alvarado Exp $
 */

#ifndef _LTI_LAPLACIAN_PYRAMID_H_
#define _LTI_LAPLACIAN_PYRAMID_H_

#include "ltiPyramid.h"
#include "ltiGaussianPyramid.h"
#include "ltiTypes.h"

namespace lti {
  /**
   * LaplacianPyramid class.
   *
   * This class implements the Laplacian pyramids as described in
   * Greenspan et.at. "Overcomplete Steerable Pyramid Filters and
   * Rotation Invariance", Proc. of the IEEE Conf. on Computer Vision
   * and Pattern Recognition, Seattle, 1994
   *
   * Each level corresponds to a difference of Gaussians, where the
   * level distance is always one octave.
   *
   * It allows an easy manipulation of the different resolutions.
   *
   * See method generate() to an detailed explanation
   *
   */
  template <class T>
  class laplacianPyramid : public pyramid<T> {
  public:

    /**
     * create a laplacian pyramid with the given number of resolutions
     * @param resolutions the number of resolutions that the pyramid can hold
     *                    (default 0: an empty pyramid will be created)
     * @param gaussianSize the size of the laplacian kernel (default 3)
     * @param variance the variance of the laplacian kernel (default -1,
     *                 meaning that the variance should be calculated as
     *                 described in the lti::gaussKernel1D<T>)
     */
    laplacianPyramid(const int& resolutions = 0,
                    const int& gaussianSize = 3,
                    const double& variance = -1);

    /**
     * create this laplacianPyramid as a copy of another laplacianPyramid
     * @param other the laplacianPyramid to be copied.
     */
    laplacianPyramid(const laplacianPyramid& other);

    /**
     * destructor
     */
    virtual ~laplacianPyramid();

    /**
     * return the size and variance of the used gaussian kernel
     */
    void getKernelParameters(int& size,
                             double& variance) const;

    /**
     * set the kernel parameters
     *
     * @param size the size of the kernel
     * @param variance the variance for the laplacian kernel.  A negative
     *                 value will force the default variance of a laplacian
     *                 kernel with size <code>size</code>.
     */
    void setKernelParameters(const int& size,
                             const double& variance=-1);

    /**
     * returns the name of this class: "laplacianPyramid"
     */
    const char* getTypeName() const {return "laplacianPyramid";};

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source laplacianPyramid to be copied.
     * @return a reference to this object
     */
    laplacianPyramid<T>& copy(const laplacianPyramid<T>& other);

    /**
     * create a clone of this laplacianPyramid
     * @return a pointer to a copy of this laplacianPyramid
     */
    virtual mathObject* clone() const;

    /**
     * generate the laplacian pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified in
     * the construction or in the resize() method.  The resolution "0"
     * will correspond the the original channel, and the resolution
     * i+1 is always a factor 2 smaller than the resolution i.
     */
    void generate(const T& src);

    /**
     * generate the laplacian pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified
     * by theResolutions.
     * The resolution "0" will correspond the the original channel,
     * and the resolution i+1 is always a factor 2 smaller than
     * the resolution i.
     */
    void generate(const T& src,const int& theResolutions);

    /**
     * generate the laplacian pyramid of the given object. Proceed,
     * until given limit.x or limit.y is reached. smallest resolution
     * will be > limit.
     *
     * The resolution "0" corresponds to the original channel, and the
     * resolution i+1 is always a factor 2 smaller than the resolution
     * i.
     */
    void generate(const T& src, const lti::point& limit);


    /**
     * generate the laplacian pyramid using the given gaussian pyramid
     */
    void generate(const gaussianPyramid<T>& src);


    /**
     * reconstructs the resolution with index i.
     *
     * @param i the resolution to be reconstructed
     * @param result the resulting object
     * @return a reference to the result object.
     */
    T& reconstruct(const int& i, T& result) const;

    /**
     * reconstructs the resolution with index i.
     *
     * All partial reconstructions will be stored at the given
     * gaussian pyramid.
     *
     * @param i the resolution to be reconstructed
     * @param result the resulting pyramid.
     * @return a reference to the result pyramid
     */
    pyramid<T>& reconstruct(const int& i, pyramid<T>& result) const;

  protected:

    /**
     * kernel size
     */
    int kernelSize;

    /**
     * kernel variance
     */
    double kernelVariance;

    /**
     * subtract b from a, and leave the result in c, which will have the size
     * of a, even if the sizes of a and b differ.
     */
    void subtract(const T& a,const T& b, T& c) const;

    /**
     * add a and b, and leave the result in c, which will have the size
     * of a, even if the sizes of a and b differ.
     */
    void add(const T& a,const T& b, T& c) const;

  };

} // namespace lti

#include "ltiLaplacianPyramid_template.h"

#endif

