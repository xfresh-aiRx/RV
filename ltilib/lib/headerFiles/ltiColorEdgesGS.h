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
 * file .......: ltiColorEdgesGS.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.10.2003
 * revisions ..: $Id: ltiColorEdgesGS.h,v 1.7 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_EDGES_G_S_H_
#define _LTI_COLOR_EDGES_G_S_H_

#include "ltiImage.h"
#include "ltiEdgeDetector.h"
#include "ltiGradientFunctor.h"
#include "ltiLinearKernels.h"
#include <limits>

namespace lti {
  /**
   * Gevers and Stokman color edges.
   *
   * This functor implements the color edges detector described in:
   *
   * Theo Gevers and Harro Stokman, "Classifying Color Edges in Video into
   *   Shadow-Geometry, Highlight or Material Transitions", IEEE Trans. on
   *   Multimedia, Vol. 5 No. 2, June 2003
   *
   * Note that it is intended for color images only.  If you use in on
   * gray-valued image the results won't be good.
   *
   * The edgeDetector::parameters::edgeValue attribute will be used to mark
   * the material edges, which are usually the ones you really want to have.
   *
   * This detector is in general slow, since it has to compute many color
   * channels and gradients to classify the edges into material, highlight
   * or shadow edges.  If you are looking for a faster detector use the
   * lti::cannyEdges functor, which uses the lti::colorContrastGradient to
   * compute also edges of color images.
   */
  class colorEdgesGS : public edgeDetector {
  public:
    /**
     * the parameters for the class colorEdgesGS
     */
    class parameters : public edgeDetector::parameters {
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

      /**
       * Assuming the given image corresponds to a homogeneous color patch,
       * the noise standard deviation will be computed as the standard 
       * deviation for each channel.
       *
       * The result will be left in the attribute noiseStdDeviation.
       * 
       * @param img image containing a homogeneous color patch.
       * @param window region of the image to be considered.
       * @return true if successful, false otherwise.
       */
      bool measureNoise(const image& img,
                        const rectangle& window =
                        rectangle(0,0,
                                  std::numeric_limits<int>::max(),
                                  std::numeric_limits<int>::max()));


      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Value used to mark the shadow edges. 
       *
       * Default value 128
       */
      ubyte shadowEdgeValue;

      /**
       * Value used to mark the highlight edges.
       * Default value 192
       */
      ubyte highlightEdgeValue;

      /**
       * The algorithm of Gevers and Stokman requires the standard deviation
       * for the noise at each channel.
       *
       * You can compute this standard deviation with the methods
       * of this parameter class measureNoise().
       *
       * The noise standard deviation is computed for the range [0,255].
       * 
       * Default value: trgbPixel<float>(1.5f,1.5f,1.5f)
       */
      trgbPixel<float> noiseStdDeviation;

      /**
       * Parameters for the gradient functor used to compute single channel
       * gradients.
       *
       * The parameters used are mostly the default ones, but the
       * mode is always set to cartesian, since that is the mode
       * required by this functor, and the default kernel is the Sobel.
       */
      gradientFunctor::parameters gradient;

      /**
       * Multiplicative factor for the uncertainty.
       *
       * The gradient value must surpass this factor times the uncertainty in
       * order to be considered as edge.
       *
       * Default value: 3.0
       */
      float uncertaintyFactor;

    };

    /**
     * default constructor
     */
    colorEdgesGS();

    /**
     * Construct a functor using the given parameters
     */
    colorEdgesGS(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    colorEdgesGS(const colorEdgesGS& other);

    /**
     * destructor
     */
    virtual ~colorEdgesGS();

    /**
     * returns the name of this type ("colorEdgesGS")
     */
    virtual const char* getTypeName() const;


    /**
     * @name Gray valued images.
     *
     * This methods are supplied for completeness, but since the algorithms
     * are designed to work with color images, the results won't be good.
     * Use the lti::cannyEdges functor instead.
     */
    //@{
    virtual bool apply(channel8 &srcdest) const;
    virtual bool apply(channel &srcdest) const;
    virtual bool apply(const channel8 &src, channel8 &dest) const;
    virtual bool apply(const channel &src, channel &dest) const; 
    virtual bool apply(const channel &src, channel8 &dest) const; 
    //@}


    /**
     * @name Color Images
     */
    //@{
    /**
     * Compute the edges for the given image and leave the result there too.
     * See apply(const image&, image&) for more details.
     * @param srcdest input and output image.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image &srcdest) const;

    /**
     * Compute the edges for the given image.
     * - The material edges will be marked with a gray tone with the value
     *   edgeValue given in the parameters object.
     * - The shadow edges will be red (with the shadow value given in the 
     *   parameters)
     * - The highlight edges will be green (with the highlight value given in
     *   the parameters.
     * The background will be gray, with the value given as noEdgeValue in
     * the parameters.
     *
     * @param src input image
     * @param dest colored edges
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image &src, image &dest) const;


    /**
     * Detect the edges in the given color image.
     * The edges will belong to one of three classes: material edge, highlight
     * edge or shadow edge.  Each one of them will be mark with the 
     * corresponding parameter value.
     * @see edgeDetector::parameters , parameters
     *
     * @param src input color image
     * @param dest detected edges
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply (const image &src, channel8 &dest) const;
    //@}
  
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorEdgesGS& copy(const colorEdgesGS& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorEdgesGS& operator=(const colorEdgesGS& other);

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
     * Get gradient modula and uncertainty values for each one of them
     */
    bool getGradients(const channel& red,
                      const channel& green,
                      const channel& blue,
                      const channel& o1,
                      const channel& o2,
                      const channel& c1,
                      const channel& c2,
                      const float& stdDevR,
                      const float& stdDevG,
                      const float& stdDevB,
                      const float& stdDevO1,
                      const float& stdDevO2,
                      const channel& stdDevC1,
                      const channel& stdDevC2,
                            channel& gradRGB,
                            channel& stdDevRGB,
                            channel& gradO1O2,
                            channel& stdDevO1O2,
                            channel& gradC1C2,
                            channel& stdDevC1C2) const;

    /**
     * Compute the kernels necessary to compute the uncertainty of the
     * gradient.
     */
    bool constructUncertaintyKernels(sepKernel<float>& filterX,
                                    sepKernel<float>& filterY) const;
    
  };
}

#endif

