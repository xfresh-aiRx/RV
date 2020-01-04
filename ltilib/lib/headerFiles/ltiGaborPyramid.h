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
 * file .......: ltiGaborPyramid.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 30.10.00
 * revisions ..: $Id: ltiGaborPyramid.h,v 1.5 2003/02/17 07:15:26 alvarado Exp $
 */

#ifndef _LTI_GABOR_PYRAMID_H_
#define _LTI_GABOR_PYRAMID_H_

#include "ltiGaussianPyramid.h"
#include "ltiTypes.h"

namespace lti {
  /**
   * GaborPyramid class.
   *
   * This class implements a dyadic gabor pyramid, which is obtained from the
   * original channel, and its gaussian pyramid, where each scale is
   * also filtered with a gabor kernel.
   *
   * Each element of the pyramid will be obtained with the same
   * filter-orientation.
   *
   * See method generate() to an detailed explanation
   * */
  template <class T>
  class gaborPyramid : public gaussianPyramid<T> {
  public:

    /**
     * create a gabor pyramid with the given number of resolutions
     * @param resolutions the number of resolutions that the pyramid can hold
     *                    (default 0: an empty pyramid will be created)
     * @param orientation the angle (in radians) of the gabor kernel
     *                    (default 0)
     * @param gaborSize the size of the gabor kernel (default 5)
     *
     * @param bandwidth the angular bandwidth in radians.  The default
     *                  value is 0.69813170 (40°), which maximizes coverage
     *                  of the frequency domain while minimizing overlap
     *                  between filters.
     * @param upsampleWithGaussian if false a rectangular kernel will be used
     *                  to reconstruct the scales, otherwise a gaussian kernel
     *                  will be used (default true)
     */
    gaborPyramid(const int& resolutions = 0,
                 const double& orientation = 0,
                 const int& gaborSize = 5,
                 const double& bandwidth = 0.69813170,
                 const bool& upsampleWithGaussian = true);

    /**
     * create this gaborPyramid as a copy of another gaborPyramid
     * @param other the gaborPyramid to be copied.
     */
    gaborPyramid(const gaborPyramid& other);

    /**
     * destructor
     */
    virtual ~gaborPyramid();

    /**
     * return the size, variance and orientation of the used gabor kernel
     */
    void getKernelParameters(double& orientation,
                             int& size,
                             double& bandwidth,
                             bool& upsampleWithGaussian) const;

    /**
     * set the kernel parameters
     *
     * @param orientation the angle (in radians) of the gabor kernel.
     * @param gaborSize the size of the gabor kernel (default 5)
     * @param bandwidth the angular bandwidth in radians.  The default
     *                  value is 0.69813170 (40°), which maximizes coverage of
     *                  the frequency domain while minimizing overlap between
     *                  filters.
     * @param upsampleWithGaussian if false a rectangular kernel will
     *                  be used to reconstruct the scales, otherwise a gaussian
     *                  kernel will be used (default true)
     */
    void setKernelParameters(const double& orientation,
                             const int& gaborSize=5,
                             const double& bandwidth=0.69813170,
                             const bool& upsampleWithGaussian=true);

    /**
     * returns the name of this class: "gaborPyramid"
     */
    const char* getTypeName() const {return "gaborPyramid";};

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source gaborPyramid to be copied.
     * @return a reference to this object
     */
    gaborPyramid<T>& copy(const gaborPyramid<T>& other);

    /**
     * create a clone of this gaborPyramid
     * @return a pointer to a copy of this gaborPyramid
     */
    virtual mathObject* clone() const;

    /**
     * generate the gabor pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified in
     * the construction or in the resize() method.  The resolution "0"
     * will correspond the the original channel, and the resolution
     * i+1 is always a factor 2 smaller than the resolution i.
     */
    void generate(const T& src);

    /**
     * generate the gabor pyramid of the given object.
     *
     * The pyramid will contain the number of resolutions specified
     * by theResolutions.
     * The resolution "0" will correspond the the original channel,
     * and the resolution i+1 is always a factor 2 smaller than
     * the resolution i.
     */
    void generate(const T& src,const int& theResolutions);

    /**
     * generate the gabor pyramid of the given object. Proceed, until
     * given limit.x or limit.y is reached. smallest resolution will
     * be > limit.
     *
     * The resolution "0" corresponds to the original channel, and the
     * resolution i+1 is always a factor 2 smaller than the resolution i.
     */
    void generate(const T& src, const lti::point& limit);

  protected:

    /**
     * kernel orientation in radians
     */
    double kernelOrientation;

    /**
     * kernel variance
     */
    double kernelBandwidth;
  };

} // namespace lti

#include "ltiGaborPyramid_template.h"

#endif

