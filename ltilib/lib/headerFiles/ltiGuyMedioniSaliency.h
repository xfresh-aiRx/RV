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
 * file .......: ltiGuyMedioniSaliency.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 19.5.2003
 * revisions ..: $Id: ltiGuyMedioniSaliency.h,v 1.7 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GUY_MEDIONI_SALIENCY_H_
#define _LTI_GUY_MEDIONI_SALIENCY_H_


#include "ltiSaliency.h"
#include "ltiBilinearInterpolator.h"
#include "ltiArctanLUT.h"

namespace lti {
  /**
   * The Guy-Medioni saliency is a procedure for perceptual grouping of
   * low level features like points and edges.
   *
   * For this task it computes first a extension field, an based on it
   * a saliency map, where only perceptual relevant features survive, or
   * illusory ones appear.
   *
   * As input it expects the output of an orientation map, with a
   * relevance channel and an orientation channel.  The relevance can
   * be replaced by an edge map.  With a threshold-parameter you can
   * decide how much relevant a pixel must be in order to be considered in
   * the saliency map.
   *
   * The original theory can be found in:
   *
   * G. Guy and G. Medioni. 
   * "Inferring global perceptual contours from local features"
   * International Journal of Computer Vision, Vol.20, No.1/2, 1996, 
   * pp. 113-133
   *
   * http://citeseer.nj.nec.com/guy96inferring.html
   *
   * Example:
   * \code
   * lti::image img;
   * lti::channel chnl,rel,ori;
   * lti::channel sal,sala;
   * lti::channel8 rel8;
   * float maxSal;
   *
   * // load an image (suppose you have one in the directory ../img/ called
   * //                image1.bmp)
   * lti::loadImage loader;
   * loader.load("../img/image1.bmp",img);
   * chnl.castFrom(img); // convert the color image into a gray valued one
   *
   * // get the edges and orientation channels
   * lti::cannyEdges canny;
   * canny.apply(chnl,rel8,ori);
   * rel.castFrom(rel8);
   *
   * // get the Guy-Medioni saliency (sal) and the orientation (sala)
   * // and the maximal value found in the sal channel (maxSal).
   * lti::guyMedioniSaliency gmSaliency;
   * gmSaliency.apply(rel,ori,sal,sala,maxSal);
   * \endcode
   *
   * The saliency channel is somehow blurred.  You can use the
   * lti::nonMaximaSuppression to get again an edge channel from the saliency:
   *
   * \code
   * lti::channel8 newEdges;
   * lti::nonMaximaSuppression suppressor;
   * suppressor.apply(sal,sala,newEdges,maxSal);
   * \endcode
   */
  class guyMedioniSaliency : public saliency {
  public:
    /**
     * the parameters for the class guyMedioniSaliency
     */
    class parameters : public saliency::parameters {
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
       * Parameter used in the creation of the Extension Field.
       * It controls the curvature decay, and was named \c B in the original
       * paper.  This value must be greater than zero.
       *
       * Default value: 2.85f
       */
      float highCurvatureDecay;

      /**
       * Parameter used in the creation of the Extension Field.
       * It controls the proximity decay, and was named \c A in the original
       * paper.  This number must be greater than zero.
       *
       * Default value: 0.003f
       */
      float proximityDecay;

      /**
       * Percentage of the maximum value in the relevance channel that
       * a pixel must reach in order to be considered for the saliency 
       * computation.  Values must be between 0.0f and 1.0f.
       *
       * Default value: 0.1f (i.e. 10% of maximum necessary)
       */
      float threshold;

      /**
       * In case threshold is too low, do not allow the field radius
       * become bigger than the given value.
       *
       * Default value: 128
       */
      int maxFieldRadius;

      /**
       * Field Threshold
       *
       * The extension field will be computed in a discrete grid.  Depending
       * on the highCurvatureDecay and proximityDecay values, the size of
       * the grid needs to be adapted.  This threshold determines which values
       * cannot be neglected and therefore controls the size (and indirectly 
       * speed) of the algorithm.  More precisely, this value determinies the
       * the minimum valid value of the x-coordinate.
       *
       * This value must be greater than 0 and less than 1
       *
       * Default value: 0.1f; (With default values, about 57x57 kernel)
       */
      float fieldThreshold;
    };

    /**
     * default constructor
     */
    guyMedioniSaliency();

    /**
     * Construct a functor using the given parameters
     */
    guyMedioniSaliency(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    guyMedioniSaliency(const guyMedioniSaliency& other);

    /**
     * destructor
     */
    virtual ~guyMedioniSaliency();

    /**
     * returns the name of this type ("guyMedioniSaliency")
     */
    virtual const char* getTypeName() const;

    /**
     * Generates the raw saliency and junction saliency maps using
     * Guy-Medioni's approach.
     *
     * To obtain the final saliency it is more efficient to use the alternative
     * apply() method than to subtract both lambda channels.
     * 
     * @param edgeRelevance Relevance for each pixel.  It can be an edge image.
     * @param orientation Orientation of each pixel.  Please note that
     *                    the orientation channel expected by this functor 
     *                    contains the angle perpendicular to the edgels 
     *                    as returned by the gradient or orientationMap 
     *                    functors.
     * @param saliencyLambdaX resulting major eigenvalue: raw saliency.
     * @param saliencyLambdaY resulting minor eigenvalues: junction saliency.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& edgeRelevance,
               const channel& orientation,
               channel& saliencyLambdaX,
               channel& saliencyLambdaY);

    /**
     * Generates the enhanced saliency measure.
     *
     * This method is more efficient than to compute both lambda channels with
     * the alternative apply() method and to subtract them.
     * 
     * @param edgeRelevance Relevance for each pixel.  It can be an edge image.
     * @param orientation Orientation of each pixel.  Please note that
     *                    the orientation channel expected by this functor 
     *                    contains the angle perpendicular to the edgels 
     *                    as returned by the gradient or orientationMap 
     *                    functors.
     * @param saliency resulting saliency channel.
     * @return true if apply successful or false otherwise.
     *
     * \warning The orientation channel expected differs from the one returned 
     *          by orientation map functors.  It must represent the real 
     *          direction of the edgels instead of the direction perpendicular
     *          to them.
     */
    bool apply(const channel& edgeRelevance,
               const channel& orientation,
               channel& saliency);


    /**
     * Generates the enhanced saliency measure.
     *
     * This method is more efficient than to compute both lambda channels with
     * the alternative apply() method and to subtract them.
     * 
     * @param edgeRelevance Relevance for each pixel.  It can be an edge image.
     * @param orientation Orientation of each pixel.  Please note that
     *                    the orientation channel expected by this functor 
     *                    contains the angle perpendicular to the edgels 
     *                    as returned by the gradient or orientationMap 
     *                    functors.
     * @param saliency resulting saliency channel.
     * @param angle resulting direction for each pixel
     * @param maxSaliency maximum saliency value.
     * @return true if apply successful or false otherwise.
     *
     * \warning The orientation channel expected differs from the one returned 
     *          by orientation map functors.  It must represent the real 
     *          direction of the edgels instead of the direction perpendicular
     *          to them.
     */
    bool apply(const channel& edgeRelevance,
               const channel& orientation,
               channel& saliency,
               channel& angle,
               float& maxSaliency);

    /**
     * Return a read only copy of the x component of the extension field.
     * The returned field component will be updated after setParameters().
     */
    const matrix<float>& getExtensionFieldX() const;
    
    /**
     * Return a read only copy of the y component of the extension field.
     * The returned field component will be updated after setParameters().
     */
    const matrix<float>& getExtensionFieldY() const;

    /**
     * Return a read only copy of the magnitude of the extension field.
     * The returned field component will be updated after setParameters().
     */
    const matrix<float>& getExtensionFieldM() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    guyMedioniSaliency& copy(const guyMedioniSaliency& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    guyMedioniSaliency& operator=(const guyMedioniSaliency& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor's parameters.
     *
     * This member updates the internal fields according to the values 
     * set in the parameters.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  protected:
    /**
     * @name Extension Field
     */
    //@{

    /**
     * generate extension field
     *
     * Based on the parameters object, a "kernel" oriented with 0 degrees
     * will be constructed. 
     *
     */
    bool generateExtensionField();

    /**
     * Accumulate the extension field components on the saliency map
     * considering the given position, angle and weight.
     *
     * The saliency fields must be previously initialized
     */
    void vote(const point p,const float angle,const float weight);

    /**
     * x component of the extension field currently used
     */
    matrix<float> extensionFieldX;

    /**
     * y component of the extension field currently used
     */
    matrix<float> extensionFieldY;

    /**
     * Magnitude of the extension field currently used
     */
    matrix<float> extensionFieldM;


    /**
     * interpolator for extensionFieldX
     */
    bilinearInterpolator<float> ifieldX;

    /**
     * interpolator for extensionFieldY
     */
    bilinearInterpolator<float> ifieldY;

    /**
     * Center of the extension field
     */
    point extFieldCenter;

    /**
     * Extension field radius
     */ 
    int extFieldRadius;

    /**
     * Matrix containing the ioPoints of a circle covering the relevant
     * extension field
     */
    imatrix extFieldCircle;

    /**
     * Threshold for the extension field magnitude
     */
    float fieldThreshold;
    
    //@}

    /**
     * @name Saliency Accumulators
     *
     * Six accumulators are necessary to compute the saliency
     */
    //@{

    /**
     * sum of the x^2 components
     */
    matrix<float> sumx2;

    /**
     * sum of the y^2 components
     */
    matrix<float> sumy2;

    /**
     * sum of the xy components
     */
    matrix<float> sumxy;
    //@}

    /**
     * compute the first step of the saliency, accumulating the fields
     * of each edge-element.
     */
    void accumulate(const channel& edgeRelevance,
                    const channel& orientation,
                    const float threshold);
                    
    /**
     * From the accumulated members compute the saliency components.
     */
    void getSaliency(channel& salLambdaMax,
                  channel& salLambdaMin);

    /**
     * From the accumulated members compute the saliency components.
     */
     void getSaliency(channel& sal);

    /**
     * From the accumulated members compute the saliency components.
     */
     void getSaliency(channel& sal,channel& angle,float& maxSaliency);

    /**
     * Look up table for sin values
     */
    //static const float* sinLUT;
    
    /**
     * Look up table for cos values
     */
    //static const float* cosLUT;

    /**
     * LUT to compute the arc tangent
     */
    arctanLUT atan2;
    
  };
}

#endif

