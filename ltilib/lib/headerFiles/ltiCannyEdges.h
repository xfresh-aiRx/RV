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
 * file .......: ltiCannyEdges.h
 * authors ....: Christian Harte
 * organization: LTI, RWTH Aachen
 * creation ...: 19.7.2002
 * revisions ..: $Id: ltiCannyEdges.h,v 1.21 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_CANNY_EDGES_H_
#define _LTI_CANNY_EDGES_H_

#include "ltiEdgeDetector.h"
#include "ltiNonMaximaSuppression.h"
#include "ltiColorContrastGradient.h"
#include "ltiArctanLUT.h"

namespace lti {
  /**
   * The Canny Edge Detector is a standard algorithm, designed to detect
   * "optimal" edges.  Almost every image processing book (e.g. Sonka et.al
   * Image Processing, Analysis and Machine Vision) explains the details for
   * the algorithm.
   *
   * As all other edge detectors in this Library, the colors for the edges and
   * background are specified in the parameters (see
   * lti::edgeDetector::parameters)
   *
   * The construction of the first cannyEdges functor in your
   * application will take some time to initialize a Look-Up-Table
   * that allows a much faster edge detection later  (about 0.13 seconds
   * on a Pentium III, 450MHz).  The LUT will require about 1MB.
   * Please see lti::arctanLUT for more information.
   *
   * You can also choose between the "classic" simple gradient operator,
   * which considers only two neighbors at each axis \c x and \c y or the more
   * accurate gradient kernels (see lti::gradientKernelX).  In the latter
   * mode, the edge detection is about a factor 2 slower.
   *
   * For color images the color contrast gradient is used.
   *
   * @see lti::nonMaximumSuppression
   * @see lti::colorContrastGradient
   * @see lti::edgeDetector
   */
  class cannyEdges : public edgeDetector {
  public:
    /**
     * the parameters for the class cannyEdges
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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Variance for the Gaussian to smooth the image.
       * (see also gaussKernel1D<ubyte>::gaussKernel1D())
       *
       * Default value: 1.0
       */
      float variance;

      /**
       * Size of the Gaussian kernel used to smooth the image
       *(see also variance).
       *
       * Set to zero if you don't want any smoothing.
       *
       * Default value: 7
       */
      int kernelSize;

      /**
       * If a pixel is detected as part of an edge (a response higher
       * than thresholdMax), its neighbors are consider also edges if their
       * values are higher than the given percentage of thresholdMax
       * (i.e. higher than thresholdMax*thresholdMin)
       *
       * This value must be between 0.0 and 1.0
       *
       * Default Value: 0.5
       */
      float thresholdMin;

      /**
       * If an edge response is higher than this value, those pixels will be
       * definite an edge.  This value MUST be between 0 and 1.0.
       *
       * Please note that the older value range between 0 and 255 is 
       * deprecated.
       *
       * Default Value: 0.04f;
       */
      float thresholdMax;

      /**
       * Parameters for gradient computation. 
       *
       * To provide more alternatives to the gradient computation, the
       * lti::gradientFunctor is used.  Here you can select all options
       * available for that functor.
       *
       * The default behaviour is to use the
       * lti::gradientFunctor::parameters::Difference kernel, which is the
       * fastest but also the less precise.  You can use the optimal
       * kernels if you require a more stable orientation detection.
       *
       * Default value: Difference mode (fast gradient computation)
       *
       * @see lti::gradientFunctor
       */
      colorContrastGradient::parameters gradientParameters;
    };

    /**
     * default constructor
     */
    cannyEdges();

    /**
     * constructor with parameters
     */
    cannyEdges(const parameters& thePars);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    cannyEdges(const cannyEdges& other);

    /**
     * destructor
     */
    virtual ~cannyEdges();

    /**
     * returns the name of this type ("cannyEdges")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * operates on a copy of the given %parameters.  This one is the fastest,
     * because it won't require casting the channel to any other type.
     *
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * compute the canny edges for the given source channel.
     * The found edges will be left on the edges channel, and the
     * orientation angle of the edges (that can be used in other functors, like
     * the hough transform) is left on the orientation channel.
     *
     * @param src channel8 with the source data.
     * @param edges channel8 where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel8& src,
                             channel8& edges,
                             channel& orientation) const;


    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;


    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel8& dest) const;

    /**
     * compute the canny edges for the given source channel.
     * The found edges will be left on the edges channel, and the
     * orientation angle of the edges (that can be used in other functors, like
     * the hough transform) is left on the orientation channel.
     *
     * @param src channel with the source data.
     * @param edges channel8 where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,
                             channel8& edges,
                             channel& orientation) const;

    /**
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * The problem lies in the definition of the "gradient" for a
     * color channel.  Here, the approach introduced in A. Cumani,
     * "Edge Detection in Multispectral Images", Technical Report,
     * Istituto Elettrotecnico Nazionale "Galileo Ferraris", 1989 is
     * followed.
     *
     * With this approach, instead of the usual gradient, the maxima of the
     * contrast function are searched.  The contrast function defines
     * the direction in the (x,y) plane at which the contrast change is
     * maximal.  See lti::colorContrastGradient for more information.
     *
     * The Canny-typical non-maxima suppression procedure is applied on the
     * contrast result.
     *
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,channel8& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest channel where the result will be left.
     * @param orientation channel where the orientation for the channel will
     *        be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                             channel8& dest,
                             channel& orientation) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the result in each channel of image.
     * @param srcdest image with the source data.  The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the result in each channel of the destination image.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;

    /**
     * Strictly speaking, this method does not do a Canny Edge
     * Detection any more, but, because of the strong similarity with
     * the Canny later stages, it is provided here.
     *
     * The problem lies in the definition of the "gradient" for a
     * color channel.  Here, the approach introduced in A. Cumani,
     * "Edge Detection in Multispectral Images", Technical Report,
     * Istituto Elettrotecnico Nazionale "Galileo Ferraris", 1989 is
     * followed.
     *
     * With this approach, instead of the usual gradient, the maxima of the
     * contrast function are searched.  The contrast function defines
     * the direction in the (x,y) plane at which the contrast change is
     * maximal.  See lti::colorContrastGradient for more information.
     *
     * The Canny-typical non-maxima suppression procedure is applied on the
     * contrast result.
     *
     * @param c1 first channel
     * @param c2 second channel
     * @param c3 third channel
     * @param edges the found edges
     * @param orientation orientation of the found angles.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& c1,
                       const channel& c2,
                       const channel& c3,
                       channel8& edges,
                       channel& orientation) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cannyEdges& copy(const cannyEdges& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cannyEdges& operator=(const cannyEdges& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:
    
    /**
     * non maxima suppression functor
     */
    nonMaximaSuppression suppressor;

    /**
     * Look-Up Table for the computations of atan2
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

    /**
     * The gradient functor used
     */
    colorContrastGradient grad;

  };
}

#endif

