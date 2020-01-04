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
 * file .......: ltiGradientASM.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 1.3.2002
 * revisions ..: $Id: ltiGradientASM.h,v 1.10 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GRADIENT_A_S_M_H_
#define _LTI_GRADIENT_A_S_M_H_

#include "ltiActiveShapeModel.h"
#include "ltiImage.h"
#include "ltiConvolution.h"
#include "ltiTriangularKernels.h"
#include "ltiGaussKernels.h"

namespace lti {
  /**
   * This class implements an alignment strategy for activeShapeModel (ASM).
   * See: Sonka's "Image Processing, Analysis, and Machine Vision", p380ff.
   *
   * For this purpose, strong gradients perpendicular to the border of a given shape are searched,
   * starting from each of the shapes's landmark points. The best candidate is selected by gradient
   * strength AND distance to the border (see parameters::weightingKernel).
   * The resulting shape's parameters are then adjusted to fit the underlying pointDistributionModel
   * (PDM) in two steps:
   * <ol>
   * <li> geometric tranformation (coarse fit)
   * <li> deformation (fine fit)
   * </ol>
   * The procedure is repeated, until a given number of iterations is reached.
   */
  class gradientASM : public activeShapeModel {
  public:
    /**
     * the parameters for the class gradientASM
     */
    class parameters : public activeShapeModel::parameters {
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
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Kernel types for additional weigthing of feature points by distance from border:
       * <ul>
       * <li> <b>useNoKernel</b>: points are not weighted
       * <li> <b>useGaussKernel</b>: gradient values of searched points are weighted with a gaussian kernel, so closer points are preferred
       * <li> <b>useTriangularKernel</b>: gradient values of searched points are weighted with a triangular kernel
       * </ul>
       */
      enum eWeightingKernel {
        useNoKernel,
        useGaussKernel,
        useTriangularKernel
      };


      /**
       * defines the number of iterations for alignment.
       * default is 3.
       */
      int iterations;

      /**
       * defines the radius in which the next border is searched (along the normal of a point).
       * default is 5.
       */
      int searchExtent;

      /**
       * use value in the channel as reliability of a point in the shape
       * default value: false
       */
      bool gradientValueAsReliability;

      /**
       * If multiple feature points along the normal of the border are found, generally the the
       * Found feature points are weighted for weighting kernel for gradient values
       * default is a triangular kernel
       */
      eWeightingKernel weightingKernel;
    };

    /**
     * default constructor
     */
    gradientASM();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    gradientASM(const gradientASM& other);

    /**
     * destructor
     */
    virtual ~gradientASM();

    /**
     * returns the name of this type ("gradientASM")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gradientASM& copy(const gradientASM& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gradientASM& operator=(const gradientASM& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;



    /**
     * align shape on the given gradient channel.
     * iteratively, scan for closest border:
     * iteration process:
     *   the channel's values along a control point's normal are saved and afterwards weighted with the
     *   weighting function specified in the parameters class.
     *   the control point is then moved to the point with the highest value.
     *   the shape is then trimmed to a valid shape.
     * @param srcdest shape with the source data.  The result will be left here too.
     * @param gradientChannel gradient channel. the pdmShape is aligned along higher values in this channel.
     * @result a reference to the <code>srcdest</code>.
     */
    pointDistributionModel::shape& apply(pointDistributionModel::shape& srcdest, const channel &gradientChannel) const;


  protected:

    /**
     * align shape on the given gradient channel (no iterations!)
     * @return a reference to the <code>srcdest</code>.
     */
    pointDistributionModel::shape& adjustShape(pointDistributionModel::shape &srcdest,
                                               const channel &gradientChannel,
                                               fvector &pointReliability) const;

    /**
     * create weighting kernel according to parameters
     * return false, if no kernel was built
     */
    bool createWeightingKernel(kernel1D<float> &theKernel) const;

    /**
     * get intensity array from a channel for a specific starting point, normal and given borders
     */
    void getIntensityArray(const tpoint<float> &start, const tpoint<float> &normal, const channel &chnl, array<float> &values) const;

    /**
     * round tpoint<float> and convert to tpoint<int>
     */
    tpoint<int> roundPoint(const tpoint<float> &floatPoint) const;

  };
}

#endif

