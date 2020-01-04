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
 * file .......: ltiOpticalFlowLK.h
 * authors ....: Bernd Mussmann, SUat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 5.9.2000
 * revisions ..: $Id: ltiOpticalFlowLK.h,v 1.8 2004/05/03 12:18:38 alvarado Exp $
 */

#ifndef _LTI_OPTICAL_FLOW_LK_H_
#define _LTI_OPTICAL_FLOW_LK_H_

// include files which are needed in this header!!

#include "ltiGradientKernels.h"
#include "ltiGaussKernels.h"
#include "ltiConvolution.h"
#include "ltiImage.h"
#include "ltiMatrix.h"
#include "ltiEigenSystem.h"

// include parent class
#include "ltiTransform.h"

namespace lti {
  /**
   * This class computes the optical flow between two consecutive images
   * according to the gradient based method of Lucas+Kanade. Optical flow
   * is determined by the velocity components u, v, which are searched to
   * complete the optical flow equation:
   *
   * I(x+u*delta_t, y+v*delta_t, t+delta_t) = I(x,y,t) + e
   *
   * I=intensity function, t=timeindex, x/y=coordinates, e=error
   *
   * Theory and Algorithm:
   * "Performance of Optical Flow Techniques", Barron+Fleet+Beauchemin,
   * IEEE CVPR, 1992.
   * "An iterative image registration technique with ...", Lucas+Kanade,
   * DARPA IU Workshop, 1981.
   */
  class opticalFlowLK : public transform {
  public:
    /**
     * the parameters for the class opticalFlowLK
     */
    class parameters : public transform::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * This is the size of the kernel for computing spatial derivatives.
       * kernelSize = 3,4,5 (default = 3)
       */
      int kernelSize;

      /**
       * This is the width and height of the square correlation window.
       * windowEdgeSize = 2,3,... (default = 7)
       */
      int windowEdgeSize;
    };

    /**
     * default constructor
     */
    opticalFlowLK();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    opticalFlowLK(const opticalFlowLK& other);

    /**
     * destructor
     */
    virtual ~opticalFlowLK();

    /**
     * returns the name of this type ("opticalFlowLK")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * @param at_t channel with the source data at time index t.
     * The x-component of the result (u) will be left here too.
     * @param at_delta_t channel with the source data at time index t+delta_t.
     * The y-component of the result (v) will be left here too.
     */
    void apply(channel& at_t,channel& at_delta_t) const;

    /**
     * operates on a copy of the given parameters.
     * @param at_t channel with the source data at time index t.
     * @param at_delta_t channel with the source data at time index t+delta_t.
     * @param u is the x-component of the resulting optical flow.
     * @param v is the y-component of the resulting optical flow.
     */
    void apply(const channel& at_t,
               const channel& at_delta_t,
               channel& u,
               channel& v) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowLK& copy(const opticalFlowLK& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

