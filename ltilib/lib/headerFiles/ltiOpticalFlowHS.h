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
 * file .......: ltiOpticalFlowHS.h
 * authors ....: Bernd Mussmann, Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 15.8.2000
 * revisions ..: $Id: ltiOpticalFlowHS.h,v 1.11 2004/05/03 12:18:38 alvarado Exp $
 */

#ifndef _LTI_OPTICAL_FLOW_HS_H_
#define _LTI_OPTICAL_FLOW_HS_H_

// include files which are needed in this header!!

#include "ltiGradientKernels.h"
#include "ltiConvolution.h"
#include "ltiSequence.h"
#include "ltiGaussKernels.h"
#include "ltiDownsampling.h"
#include "ltiFilledUpsampling.h"

// include parent class:
#include "ltiTransform.h"

namespace lti {
  /**
   * Computes the optical flow between two consecutive images
   * according to Horn-Schunks gradient based method. Optical
   * flow is determined by velocity components u, v, which are
   * searched to complete the optical flow equation:
   *
   * I(x+u*delta_t, y+v*delta_t, t+delta_t) = I(x,y,t) + e
   *
   * I=intensity function, t=timeindex, x/y=coordinates, e=error
   *
   *
   * Theory: "The Computation of Optical Flow", Beauchemin & Barron,
   * ACM Computing Surveys, Vol.27 No. 3
   *
   * Algorithm: "Computer Vision", Klette, Schluens & koschan, Springer, pp.190
   */
  class opticalFlowHS : public transform {
  public:
    /**
     * the parameters for the class opticalFlowHS
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

      //  parameters of functor

      /**
       * Size of kernel for spatial gradient computation.
       * kernelSize = 3,4,5 (default=3)
       */
      int kernelSize;

      /**
       * Number of iterations.
       * iterations = 1,2,... (default=100)
       */
      int iterations;

      /**
       * Smoothness constraint factor.
       * lambda > 0.0 (default = 10.0)
       *
       * The solutions (u,v) of the optical flow equation are ambiguous.
       * The additional "smoothness-constraint" allows to select a solution
       * conditional on the desired smoothness of the flowfield. Smoothness
       * is controlled by parameter 'lambda'. Higher values produce
       * smoother flowfields but also higher error e.
      */
      float lambda;

      /**
       * Toggles multi scale approach for computation of optical flow.
       * (default = true)
       *
       * If multiScale==true, then the two input images are sampled down to
       * several scale levels, before the optical flow is computed iteratively
       * from coarse to fine resolution. This approach allows faster computation
       * and can cope with large motion.
       * Remark: The number of scale levels can vary, since it depends on the
       * number of iterations and the original image size.
      */
      bool multiScale;

      /**
       * If you want to hand over previously calculated flow results (u,v) as initial values
       * of iterative flow computation, then this must be set to 'true'. Otherwise initial
       * values will always be 0.
       * (default = false)
       * Remark: previous results must be passed as function parameters with the 'on copy apply'.
       * (u,v) must be equal sized to input channels ch_t0, ch_t1, or else they will be ignored.
      */
      bool initialize;
    };

    /**
     * default constructor
     */
    opticalFlowHS();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    opticalFlowHS(const opticalFlowHS& other);

    /**
     * destructor
     */
    virtual ~opticalFlowHS();

    /**
     * returns the name of this type ("opticalFlowHS")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * @param ch_t0_u channel at timeindex 0.
     * @param ch_t1_v channel at timeindex 1. Note that ch_t0 is the predecessor of ch_t1.
     * u,v (x,y components of flowfield) are returned in ch_t0_u, ch_t1_v
     */
    void apply(channel& ch_t0_u,channel& ch_t1_v) const;

    /**
     * operates on a copy of the given parameters.
     * @param ch_t0 channel at timeindex 0.
     * @param ch_t1 channel at timeindex 1. Note that ch_t0 is the predecessor of ch_t1.
     * @param u x component of flowfield
     * @param v y component of flowfield
     */
    void apply(const channel& ch_t0,
               const channel& ch_t1,
               channel& u,
               channel& v) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    opticalFlowHS& copy(const opticalFlowHS& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    private:

    /**
     * Implements functionality of optical flow computation.
     * Uses Horn-Schunck Method with given %parameters, except
     * for '%parameter.iterations', which is replaced by the
     * function call %parameter. Iteration is always initialized
     * with (u,v), which MUST have same size as ch_t0 and ch_t1.
     */
    void computeOpticalFlow(const channel& ch_t0,
                            const channel& ch_t1,
                            channel& u,
                            channel& v,
                            int iterations) const;

    /**
     * Will be used, when multi scale approach is desired.
     * Calls the function 'computeOpticalFlow' for each sampling
     * level.
     */
    void computeMultiScaled(const channel& ch_t0,
                            const channel& ch_t1,
                            channel& u,
                            channel& v) const;

    /**
     * Will be used, when single scale approach is desired.
     * Calls the function 'computeOpticalFlow' once.
     */
    void computeSingleScaled(const channel& ch_t0,
                             const channel& ch_t1,
                             channel& u,
                             channel& v) const;

  };
}

#endif

