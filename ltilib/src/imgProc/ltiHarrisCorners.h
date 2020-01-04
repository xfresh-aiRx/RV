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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiHarrisCorners.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 31.10.2002
 * revisions ..: $Id: ltiHarrisCorners.h,v 1.13 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HARRIS_CORNERS_H_
#define _LTI_HARRIS_CORNERS_H_

#include "ltiCornerDetector.h"
#include "ltiPointList.h"
#include "ltiLocalMaxima.h"
#include "ltiGradientFunctor.h"

namespace lti {
  /**
   * Harris Corner Detector.
   *
   * The Harris corner detector is based on
   *
   * C. Harris and M. Stephens, "A Combined Corner and Edge Detector",
   * in Proc. 4th Alvey Vision Conference, pp. 147-151, 1988
   *
   * It uses a matrix \e A that averages element-wise products of the
   * gradients \e Ix and \e Iy over a Gaussian window \e W:
   *
   * \f[ A(x,y) = \begin{bmatrix} \sum_W I_x^2 & \sum_W I_xI_y \\
   *                \sum_W I_xI_y & \sum_W I_y^2 \end{bmatrix} \f]
   *
   * The corners are defined as the local maxima of a cornerness
   * channel.  The cornerness \a c(x,y) of a channel \a f(x,y) is
   * defined as:
   *
   * \f[ c(x,y)= \det(A(x,y)) - s \cdot\mathrm{tr}(A(x,y))^2 \f]
   *   
   * Remember that the product of the eigenvalues equals the
   * determinant of the matrix and the sum of eigenvalues equals its
   * trace.
   *
   * Slight modifications are used in order to increase the quality of
   * the detection.
   *
   * The default paramters use the Harris Kernel (see harrisKernelX)
   * as gradient. However, you can choose freely from all available
   * gradient kernels. Most notably, by using
   * gradientFunctor::parameters::OGD, the harrisCorners %functor
   * should behave similarly to the improved Harris introduced by
   * Schmid et al.
   */
  class harrisCorners : public cornerDetector {
  public:
    /**
     * the parameters for the class harrisCorners
     */
    class parameters : public cornerDetector::parameters {
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
       * Variance used for the Gaussian kernel used as window for the
       * autocorrelation.
       *
       * Default value : -1 (see lti::gaussKernel1D<float>::generate() for
       *                 the meaning of this)
       */
      float variance;

      /**
       * size of the Gaussian kernel used as window for the autocorrelation
       *
       * Default value: 7
       */
      int kernelSize;

      /**
       * Maximum number of corners to be detected.
       *
       * Default value: 300
       */
      int maximumCorners;

      /**
       * Scale factor in Harris auto-correlation
       *
       * Default value: 0.04f
       */
      float scale;

      /**
       * A localMaxima<float> %functor is used to detect the corners.
       * The parameters for it can be given here.
       *
       * Default value: localMaxima<float>::parameters()
       */
      localMaxima<float>::parameters localMaximaParameters;

      /**
       * In the original work by Harris and Stephens the kernel [-2 -1
       * 0 1 2] was used to calculate the gradients in x and y
       * directions. However, recently Schmid et al. have found that
       * other kernels e.g. derivatives of gaussians can give superior
       * results (use OGD for that). To provide maximum flexibility
       * the general gradientFunctor is used here.
       *
       * The gradientFunctor::parameters::format \b must always be set
       * to \a Cartesic. This is also default here.
       *
       * Default gradient type is gradientFunctor::parameters::Harris.
       *
       * Other values are the default values specified in
       * gradientFunctor::parameters.
       *
       * See also: harrisKernelX
       */
      gradientFunctor::parameters gradientFunctorParameters;
    };

    /**
     * default constructor
     */
    harrisCorners();

    /**
     * Construct a functor using the given parameters
     */
    harrisCorners(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    harrisCorners(const harrisCorners& other);

    /**
     * destructor
     */
    virtual ~harrisCorners();

    /**
     * returns the name of this type ("harrisCorners")
     */
    virtual const char* getTypeName() const;

    /**
     * set functor's parameters.
     *
     * The gradientFunctorParameters are checked for compliancy with
     * the definitions above. See
     * parameters::gradientFunctorParameters.
     *
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * Read the functor from the given ioHandler. Also sets the
     * gradientFunctor parameters.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * Extract the corners in srcdest and leave the result there too.
     *
     * The result is a mask containing only the corners.  The background
     * will be marked using parameters::noCornerValue and the corners will
     * have the value parameters::cornerValue (these are inherited from
     * the parent class).
     *
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * Extract the corners in srcdest and leave the result there too.
     *
     * The result is a mask containing only the corners.  The background
     * will be marked using parameters::noCornerValue/255 and the corners will
     * have the value parameters::cornerValue/255 (these are inherited from
     * the parent class).
     *
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * Extract the corners in src and leave the result in dest.
     *
     * The result is a mask containing only the corners.  The background
     * will be marked using parameters::noCornerValue and the corners will
     * have the value parameters::cornerValue (these are inherited from
     * the parent class).
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * Extract the corners in src and leave the result in dest.
     *
     * The result is a mask containing only the corners.  The background
     * will be marked using parameters::noCornerValue/255 and the corners will
     * have the value parameters::cornerValue/255 (these are inherited from
     * the parent class).
     *
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;

    /**
     * Extract the corners in src and leave them in dest.
     *
     * @param src channel8 with the source data.
     * @param dest list of corners
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,pointList& dest) const;

    /**
     * Extract the corners in src and leave them in dest.
     *
     * @param src channel with the source data.
     * @param dest list of corners
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,pointList& dest) const;

    /**
     * Extract the cornerness channel belonging to src.
     *
     * Note that the cornerness is in principle a partial result.
     *
     * @param src channel with the source data.
     * @param cornerness channel containing the cornerness of the source data.
     * @param maxCornerness maximum value found in cornerness.
     * @param dest list containing the position of all detected corners.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                       channel& cornerness,
                       float& maxCornerness,
                       pointList& dest) const;

    /**
     * Extract the cornerness channel belonging to src.
     *
     * Note that this is in principle a partial result.  It would require
     * further processing to get the corners, but some other functors could
     * make use of this.
     *
     * @param src channel with the source data.
     * @param cornerness channel containing the cornerness of the source data.
     * @param maxCornerness maximum value found in cornerness.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                       channel& cornerness,
                       float& maxCornerness) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    harrisCorners& copy(const harrisCorners& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    harrisCorners& operator=(const harrisCorners& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * compute the second order.
     *
     * @param gx contains the gradient in x direction as input and the
     *        second derivative fxx as output.
     * @param gy contains the gradient in y direction as input and the
     *        second derivative fyy as output.
     * @param fxy contains the mixed second derivative as output.
     */
    bool getSecondOrder(channel& gx,
                        channel& gy,
                        channel& fxy) const;

    /**
     * compute cornerness
     */
    bool getCornerness(const channel& fxx,
                       const channel& fxy,
                       const channel& fyy,
                       const float scale,
                       channel& cornerness,
                       float& maxCornerness) const;

    /**
     * find corners with maximal cornerness
     */
    bool findCornerMaxima(const channel& cornerness,
                          channel& cornersOnly,
                          pointList& cornerMax) const;


    /**
     * gradientFunctor used to calculate x and y gradients.
     */
    gradientFunctor gradient;
  };
}

#endif

