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
 * file .......: ltiColorContrastGradient.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 1.6.2003
 * revisions ..: $Id: ltiColorContrastGradient.h,v 1.16 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_CONTRAST_GRADIENT_H_
#define _LTI_COLOR_CONTRAST_GRADIENT_H_

#include "ltiGradientFunctor.h"

namespace lti {
  /**
   * The contrast gradient is used in color or multi-spectral image as 
   * a replacement of the gray-value gradient in gray images.
   *
   * Here, the approach introduced in A. Cumani,
   * "Edge Detection in Multispectral Images", Technical Report,
   * Istituto Elettrotecnico Nazionale "Galileo Ferraris", 1989 is
   * followed.
   *
   * A more detail theoretical analysis can be found in
   * I.R. Greenshields
   * "Coherent computation of the multispectral maximal directional derivative"
   * Image and Vision Computing,
   * Vol 18. 1999, pp. 1-7
   *
   * With this approach, instead of a gradient, the maximum of the contrast
   * function for each pixel is searched.  The contrast function defines the
   * direction at the (x,y) plane at which the contrast change is maximal.  It
   * corresponds to the greater eigenvalue and corresponding eigenvector of
   * the square of the Jacobian matrix:
   *
   * \f[
   * \begin{pmatrix}
   *   \frac{\partial \mathbf{f}}{\partial x}\cdot
   *   \frac{\partial \mathbf{f}}{\partial x} &
   *   \frac{\partial \mathbf{f}}{\partial x}\cdot
   *   \frac{\partial \mathbf{f}}{\partial y} \\
   *   \frac{\partial \mathbf{f}}{\partial x}\cdot
   *   \frac{\partial \mathbf{f}}{\partial y} &
   *   \frac{\partial \mathbf{f}}{\partial y}\cdot
   *   \frac{\partial \mathbf{f}}{\partial y} \\
   * \end{pmatrix}
   * \f]
   *
   * As suggested in the literature, the difference between both eigenvalues 
   * should be used as contrast, and the direction of the main eigenvector
   * as gradient direction.
   *
   * with \f$\mathbf{f}(x,y)\f$ being any multi-spectral image.  In
   * this case this vector function is defined in a three dimensional color
   * space.
   *
   * Since this method uses the lti::arctanLUT class to accelerate the
   * computation (sacrifying a little bit in angle precision, which anyway
   * cannot be accurately  determined due to the spatial quantization effects)
   * the angles returned in the parameters::Polar mode are always possitive 
   * between 0 and 2*Pi.
   */
  class colorContrastGradient : public gradientFunctor {
  public:
    /**
     * the parameters for the class gradientFunctor
     */
    class parameters : public gradientFunctor::parameters {
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
      enum eContrastFormat {
        MDD,      /**< Maximum Directional Derivative */
        Contrast, /**< Difference between maximum and minimum eigenvalues */
        Maximum   /**< Maximum among all color channels */
      };

      /**
       * The color gradient can be computed in several forms.
       *
       * The maximum directional derivative (MDD) is not exactly contrast, but
       * can provide a good edgeness measure.  Its magnitude is the largest
       * eigenvalue of the Jacobian matrix.  The direction of the maximum
       * directional derivative is used.
       *
       * The Contrast activates the real color contrast form, equal to the
       * difference of the eigenvalues of the square of the Jacobian matrix
       * for each pixel.
       *
       * The Maximum mode is not contrast any more.  It just takes the
       * largest scalar gradient of each color channel.  The angle of the
       * winner channel is used.
       *
       * Default value: Contrast
       */
      eContrastFormat contrastFormat;
    };


    /**
     * default constructor
     */
    colorContrastGradient();

    /**
     * Construct a functor using gradient kernels with the specified values.
     *
     * @param format specify the format to be use: Cartesic or Polar
     * @param gradKernelSize size for the gradient kernel to be used.
     */
    colorContrastGradient(const parameters::eOutputFormat& format,
                          const int gradKernelSize=0);

    /**
     * Construct a functor using the given parameters
     */
    colorContrastGradient(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    colorContrastGradient(const colorContrastGradient& other);

    /**
     * destructor
     */
    virtual ~colorContrastGradient();

    /**
     * returns the name of this type ("colorContrastGradient")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes the color contrast gradient of the given RGB image.
     * @param src image with the source data.  The image will be splitted in
     *            its R,G and B components for the analysis.
     *
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     channel& xOrMag,
                     channel& yOrArg) const;

    /**
     * Computes the (monochromatic) gradient of the given channel.
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
     * Computes the (monochromatic) gradient of the given channel.
     *
     * @param src channel with the source data.
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
     * Computes the color contrast gradient of a tri-spectra image defined
     * by the three given components.
     * @param c1 first channel of the input image.
     * @param c2 second channel of the input image.
     * @param c3 third channel of the input image.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,
               const channel& c2,
               const channel& c3,
               channel& xOrMag,
               channel& yOrArg) const;

    /**
     * Computes the color contrast gradient of a tri-spectral image defined
     * by the three given components.
     * @param c1 first channel of the input image.
     * @param c2 second channel of the input image.
     * @param c3 third channel of the input image.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @param maxMag if polar and cartesic modes (see parameters), the maximum
     *               magnitude of the gradient found.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,
               const channel& c2,
               const channel& c3,
               channel& xOrMag,
               channel& yOrArg,
               float& maxMag) const;

    /**
     * Computes the color contrast gradient of a bi-spectral image defined
     * by the two given components.
     * @param c1 first channel of the input image.
     * @param c2 second channel of the input image.
     * @param xOrMag the resulting x component or magnitude of the gradient
     *               will be left here.
     * @param yOrArg the resulting y component or argument (orientation) of
     *               the gradient will be left here.
     * @param maxMag if polar and cartesic modes (see parameters), the maximum
     *               magnitude of the gradient found.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,
               const channel& c2,
               channel& xOrMag,
               channel& yOrArg,
               float& maxMag) const;

    /**
     * Computes the eigenvalues of the square of the Jacobian matrix of a
     * tri-spectral image.
     *
     * This method ignores the parameter \a format, as it makes only sense for
     * a polar system, and also the \a contrastFormat parameter as it
     * explicitely requests the computation of all eigenvalues.
     *
     * @param c1 first channel of the input image.
     * @param c2 second channel of the input image.
     * @param c3 third channel of the input image.
     * @param lambdaMin the resulting minimal eigenvalues
     * @param lambdaMax the resulting maximal eigenvalues
     * @param orientation the angle of the main axis (diretion of the 
     *                        eigenvector with the greater eigenvalue)
     * @param maxMag maximum contrast value found (lambdaMin-lambdaMax)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& c1,
               const channel& c2,
               const channel& c3,
               channel& lambdaMin,
               channel& lambdaMax,
               channel& orientation,
               float& maxMag) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorContrastGradient& copy(const colorContrastGradient& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorContrastGradient& operator=(const colorContrastGradient& other);

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
     * differentiate the image in X and Y directions
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     * @param gradAbs the absolute values of the gradient
     * @param orientation the orientation map of the gradient image
     * @param maxVal maximum value found in gradAbs
     * @return true if successful, false otherwise
     */
    bool computeGradientPolar(const channel& c1,
                              const channel& c2,
                              const channel& c3,
                              channel& gradAbs,
                              channel& orientation,
                              float& maxVal) const;

    /**
     * differentiate the image in X and Y directions
     * @param c1 first channel
     * @param c2 second channel
     * @param gradAbs the absolute values of the gradient
     * @param orientation the orientation map of the gradient image
     * @param maxVal maximum value found in gradAbs
     * @return true if successful, false otherwise
     */
    bool computeGradientPolar(const channel& c1,
                              const channel& c2,
                              channel& gradAbs,
                              channel& orientation,
                              float& maxVal) const;

    /**
     * differentiate the image in X and Y directions
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     * @param lambdaMin minimum eigenvalues
     * @param lambdaMax maximum eigenvalues
     * @param orientation the orientation map of the gradient image
     * @param maxVal maximum contrast value found (lambdaMin-lambdaMax)
     * @return true if successful, false otherwise
     */
    bool computeGradientPolar(const channel& c1,
                              const channel& c2,
                              const channel& c3,
                              channel& lambdaMin,
                              channel& lambdaMax,
                              channel& orientation,
                              float& maxVal) const;


    /**
     * differentiate the image in X and Y directions
     * @param c1 first channel
     * @param c2 second channel
     * @param xchnl x component
     * @param ychnl y component
     * @param maxVal maximum magnitude (max(sqrt(xchnl*xchnl+ychnl*ychnl))
     * @return true if successful, false otherwise
     */
    bool computeColorGradientCart(const channel& c1,
                                  const channel& c2,
                                  channel& xchnl,
                                  channel& ychnl,
                                  float& maxVal) const;

    /**
     * differentiate the image in X and Y directions
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     * @param xchnl x component
     * @param ychnl y component
     * @param maxVal maximum magnitude (max(sqrt(xchnl*xchnl+ychnl*ychnl))
     * @return true if successful, false otherwise
     */
    bool computeColorGradientCart(const channel& c1,
                                  const channel& c2,
                                  const channel& c3,
                                  channel& xchnl,
                                  channel& ychnl,
                                  float& maxVal) const;

    /**
     * Compute the square of the magnitude: a*a + b*b
     */
    inline float magSqr(const float a,const float b) const;
  };
}

#endif

