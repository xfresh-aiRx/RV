/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiGradientFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 1.6.2003
 * revisions ..: $Id: ltiGradientFunctor.h,v 1.8 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GRADIENT_FUNCTOR_H_
#define _LTI_GRADIENT_FUNCTOR_H_

#include "ltiTransform.h"
#include "ltiArctanLUT.h"

namespace lti {
  /**
   * The gradient is a simple wrapper for the convolution functor with
   * some convenience parameterization to choose between different common
   * gradient kernels.
   *
   * Not only the classical simple difference computation (right minus
   * left for the x direction or bottom minus top for the y direction)
   * and the classical Sobel, Prewitt, Robinson, Roberts and Kirsch
   * kernels can be used, but the more sophisticated Ando optimal
   * kernels (see lti::gradientKernelX) and the approximation using
   * oriented gaussian derivatives can be used.
   *
   * The kernel computes the gradient for lti::channel %objects only.  For
   * color images you can use one of the derived classes.
   *
   * @see lti::gradientFunctor,lti::gradientKernelX,lti::colorContrastGradient
   *
   * @ingroup gLinearFilters
   */
  class gradientFunctor : public transform {
  public:
    /**
     * the parameters for the class gradientFunctor
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
       * Format types
       */
      enum eOutputFormat {
        Polar,    /**< Polar coordinates first magnitud, then angle */
        Cartesic, /**< Cartesian coordinates first x, then y        */
      };

      /**
       * The output of the apply methods can be in polar or cartesic
       * coordinates.  You can specify here which format you prefer.
       *
       * Default value: Polar
       */
      eOutputFormat format;


      /**
       * Kernel Type
       */
      enum eKernelType {
        Optimal, /**< Use the optimal kernel gradients of Ando
                  *   (see lti::gradientKernelX).
                  */
        OGD,     /**< Use oriented Gaussian derivatives 
                  *
                  */
        Difference, /**<
                     * Simplest (and fastest) kernel just right-left,
                     * top-bottom
                     */
        Roberts,    /**<
                     * Also very simple, but has a location-bias of half pixel.
                     * These are 2x2 kernels, 
                     */ 
        Sobel,      /**<
                     * 3x3 separable kernel with a 1 2 1, -1 0 1 sub-kernels
                     * (see lti::sobelKernelX)
                     */
        Prewitt,    /**<
                     * 3x3 separable kernel with a 1 1 1, -1 0 1 sub-kernels
                     * (see lti::prewittKernelX)
                     */
        Robinson,   /**
                     * 3x3 non-separable kernel (see lti::robinsonKernelX)
                     */ 
        Kirsch,      /**
                     * 3x3 non-separable kernel (see lti::kirschKernelX)
                     */  
        Harris      /**<
                     * 1x5 separable kernel -2 -1 0 1 2 
                     * (see lti::harrisKernelX)
                     */
      };

      /**
       * Type of gradient kernel to be used.
       *
       * Default value: Optimal
       */
      eKernelType kernelType;

      /**
       * Size of the kernel used.
       *
       * This attribute is used only for the Optimal and OGD Kernel types.
       * 
       * For the Optimal kernel type, the value \b must be 3, 4, or 5.
       * For the OGD odd values greater or equal 3 should be used.
       *
       * Default value: 3 
       */
      int gradientKernelSize;

      /**
       * Variance used for the Gaussian.
       *
       * This argument has effect only if kernelType is OGD. 
       *
       * It is the variance use for the Gaussian to be derived as approximation
       * for a gradient kernel.
       *
       * Default value: -1 (i.e. the variance will be computed from the size
       *                    of the kernel (\a gradientKernelSize) )
       */
      float ogdVariance;
    };

    /**
     * Default constructor
     */
    gradientFunctor(const bool initializeParameters = true);

    /**
     * Construct a functor using gradient kernels with the specified values.
     *
     * @param format specify the format to be use: Cartesic or Polar
     * @param gradKernelSize size for the gradient kernel to be used.
     */
    gradientFunctor(const parameters::eOutputFormat& format,
                    const int gradKernelSize=0);

    /**
     * Construct a functor using the given parameters
     */
    gradientFunctor(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    gradientFunctor(const gradientFunctor& other);

    /**
     * destructor
     */
    virtual ~gradientFunctor();

    /**
     * returns the name of this type ("gradientFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes the gradient of the given channel.
     *
     * @param src channel with the source data.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
                     channel& xOrMag,
                     channel& yOrArg) const;

    /**
     * Computes the gradient magnitude of the given channel.
     *
     * @param src channel with the source data.
     * @param mag the resulting magnitude of the gradient will be left here.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
                     channel& mag) const;

    /**
     * Computes the gradient magnitude of the given channel.
     *
     * @param srcdest channel with source data, the resulting magnitude 
     *                will be left here too.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;


    /**
     * Computes the gradient of the given channel.
     *
     * @param src channel8 with the source data.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
                     channel& xOrMag,
                     channel& yOrArg) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gradientFunctor& copy(const gradientFunctor& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    gradientFunctor& operator=(const gradientFunctor& other);

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
     * @name One channel gradient computation
     */
    //@{
    /**
     * compute the gradient of the src channel8 using the adecuate
     * method xyDifferentiateXYZ
     */
    bool computeGradientCart(const channel& src,
                                   channel& dx,
                                   channel& dy) const;

    /**
     * differentiate the image in X and Y directions using operator (-1 0 1)
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCart(const channel& src,
                                  channel& dx,
                                  channel& dy) const;

    /**
     * differentiate the image in X and Y directions using optimal operators.
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCartOpt(const channel& src,
                                     channel& dx,
                                     channel& dy) const;

    /**
     * differentiate the image in X and Y directions using optimal operators.
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCartClassicSep(const channel& src,
                                            channel& dx,
                                            channel& dy) const;

    /**
     * differentiate the image in X and Y directions using classic gradients.
     * @param src source channel
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImageCartClassic2D(const channel& src,
                                           channel& dx,
                                           channel& dy) const;

    /**
     * Roberts operator
     * 
     * calculates the gradient at the center of a 2x2 mask. The position marked
     * with "S" is the position where the gradient is stored at in the output
     * channels:
     *
     * \code
     * ( S  +) 
     * ( +  +)
     * \endcode
     *
     * (The origin (0,0) is at the bottom left of the image)
     * @param src the already smoothed image
     * @param dx x component of the gradient
     * @param dy y component of the gradient
     * @return returns NULL if successful, a textual error message otherwise
     */
    bool xyDifferentiateImageCartInterPixel(const channel& src,
                                            channel& dx,
                                            channel& dy) const;

    /**
     * Differentiate the image in X and Y directions using operator (-1 0 1)
     * The result will be directly in polar coordinates
     * @param src source channel
     * @param gradAbs magnitude of the gradient.
     * @param gradPhi argument of the gradient (angle).
     * @return true if successful, false otherwise
     */
    bool xyDifferentiateImage(const channel8& src, 
                              channel& gradAbs,
                              channel& gradPhi) const;

    //@}
    
    /**
     * Look-Up Table for computation of arc tangent
     */
    arctanLUT atan2;

    /**
     * convert a pair of values in cartesic system into a pair in
     * polar system
     *
     * The LUT takes care that the returned value lies between 0 and 2*Pi
     */
    inline void cartToPolar(const int dx,const int dy,
                            float& mag,float& angle) const;

  };
}

#endif

