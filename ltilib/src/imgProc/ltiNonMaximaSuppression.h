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
 * file .......: ltiNonMaximaSuppression.h
 * authors ....: Pablo Alvarado, Christian Harte
 * organization: LTI, RWTH Aachen
 * creation ...: 23.5.2003
 * revisions ..: $Id: ltiNonMaximaSuppression.h,v 1.10 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_NON_MAXIMA_SUPPRESSION_H_
#define _LTI_NON_MAXIMA_SUPPRESSION_H_

#include "ltiTransform.h"
#include "ltiPointList.h"

namespace lti {
  /**
   * The non-maxima suppression is usually the last stage of edge
   * detectors (the most prominent example is the Canny Edge Detector,
   * see lti::cannyEdges), but it can also be employed in other
   * similar tasks, like detection of saliency edges from "structural"
   * saliency maps (see for example lti::guyMedioniSaliency).
   *
   * From a sort of "diffuse edge" channel, this functor tries first to find
   * all local maxima, and suppresses the rest.  In an optional
   * hysteresis stage, neighbor pixels of detected maxima can also
   * "survive" the suppression if their value exceeds a lower
   * threshold value.
   *
   * A similar class used in detection of local maxima is the lti::localMaxima.
   *
   * There are two way to given the thresholds.  The classical way is to give
   * them as a percentage of the maximal pixel value found in the image.  This
   * is the way usually described in the text-books when describing the last
   * stages of the Canny Edge Detection.
   *
   * This functor provides an additional (sometimes more robust way)
   * to select the thresholds.  It is an indirect way that computes
   * the threshold such that a given percentage of the total number of
   * pixels have a value above it.  It is a way to tell the functor
   * your expectation how many pixels in an image will be considered
   * edges, instead of telling which value the edges will have. 
   *
   * Some parameters will allow you to select which way of threshold
   * computation you prefer for the low- and high-thresholds.
   * 
   * @see parameters
   * @see localMaxima
   */
  class nonMaximaSuppression : public transform {
  public:
    /**
     * the parameters for the class nonMaximaSuppression
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
       * If a pixel is detected as part of an edge (a response higher
       * than thresholdMax), its neighbors are consider also edges if their
       * values are higher than the given percentage of thresholdMax
       * (i.e. higher than thresholdMax*thresholdMin).
       *
       * If the parameter indirectThresholdMin is \c true, then this value
       * means the percentage of pixels with the highest gradient values that
       * could be possible edges if they are neighbors of an edge pixel.  Note
       * that in this mode, and if indirectThresholdMin is also true, this
       * value should be greater than thresholdMax, since there should me
       * more possible edges than definitively edges.
       *
       * This value must be between 0.0f and 1.0f.
       *
       * Default Value: 0.5f
       */
      float thresholdMin;

      /**
       * If \c true then the real minimum threshold will be internally
       * computed considering that possibly \c thresholdMin
       * percent of the total number of pixels with the highest gradient
       * values are edges.
       *
       * If \c false, then the given \c thresholdMin will be used as is.
       *
       * Default value: false
       */
      bool indirectThresholdMin;

      /**
       * The real maximal threshold is computed from the maximum found in
       * the input image and this thresholdMax parameter, which is the
       * fraction of the maximal value to be considered.
       *
       * If an edge response is higher than the threshold, those
       * pixels will be definitively considered as edge.  This value
       * must be between 0.0f and 1.0f.
       *
       * If the parameter indirectThresholdMax is \c true, then this value
       * means the percentage of pixels with the highest gradient values that
       * are definitively edges.
       *
       * Default Value: 0.04f
       */
      float thresholdMax;
      
      /**
       * If \c true then the real maximum threshold will be internally
       * computed considering that \c thresholdMax
       * percent of the total number of pixels with the highest gradient
       * values are edges.
       *
       * If \c false, then the given \c thresholdMax will be used as is.
       *
       * Default value: false
       */
      bool indirectThresholdMax;

      /**
       * Value used for the background of the response
       *
       * Default value: 0
       */
      ubyte background;

      /**
       * Value used for the detected edges
       *
       * Default value: 255
       */
      ubyte edgeValue;

      /**
       * The expected angles in the orientation map must be between
       * 0.0f and 2*Pi.  If you can ensure this condition previous to
       * call the apply method, you can deactivate the check of this
       * condition setting this parameter to false.  Of course, checking
       * this condition takes its time, therefore it is better if you
       * ensure it in the algorithm that generates the orientation channel.
       *
       * The correction of angle is done in an arithmetical way, so, if the
       * magnitude of the values you provide can be much bigger than 2*Pi, you
       * need to take care of the conversion, or the check will
       * take too long or, in case 2*Pi is negligible compared with your 
       * values, it will remain in an infinity loop.
       *
       * The reason for an arithmetical correction is that in most cases the
       * angles will be between -2*Pi and 4*Pi, and an arithmetical check
       * is the fastest one.
       *
       * Default value: true
       */
      bool checkAngles;

      /**
       * Size of the histogram used to compute automatically the thresholds.
       * This determines the possible threshold values.  The histogram will
       * not be computed, if no threshold is to be computed automatically.
       *
       * Default value: 256
       */
      int gradientHistogramSize;
      
      /**
       * @name extensions to the classical algorithm
       */
      //@{
      /**
       * If false, the traditional algorithm will be applied, which only
       * detects the maxima along the gradient direction followed by 
       * hysteresis thresholding.  
       * If true, additionally a gap filling algorithm will be applied.
       *
       * Default value = false;
       */
      bool fillGaps;

      /**
       * value used to mark end points
       *
       * Default value: 255
       */
      ubyte endPointValue;

      /**
       * value used to mark gap completion
       *
       * Default value: 255
       */
      ubyte gapValue;

      /**
       * Number of pixels used together with an end point to estimate
       * the interpolation of a gap.
       * 
       * Default value: 5
       */
      int numGapHints;

      /**
       * Maximal allowed gap length
       * Default value: 10
       */
      int maxGapLength;
      
      //@}
    };

    /**
     * default constructor
     */
    nonMaximaSuppression();

    /**
     * Construct a functor using the given parameters
     */
    nonMaximaSuppression(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    nonMaximaSuppression(const nonMaximaSuppression& other);

    /**
     * destructor
     */
    virtual ~nonMaximaSuppression();

    /**
     * returns the name of this type ("nonMaximaSuppression")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param preedges channel containing pre-edges, i.e. only some
     *                 confidence values are expected, where the 
     *                 maxima represent edges.  Only those values
     *                 greater than thresholdMax times the maximum
     *                 value here will be considered.
     * @param orientation channel with the angles.  It is an angle in the
     *                 image coordinate system, i.e. a positive angle direction
     *                 is clockwise.  The units must be radians and the values
     *                 MUST be zero or * positive between 0.0f and 2*Pi.  See
     *                 documentation of * parameters::checkAngle for more
     *                 information.
     * @param edges    After the suppression, only the edges will
     *                 be left.
     * @param maxPreedge Optional argument.
     *                 The algorithm uses as maximum threshold the given
     *                 parameters::maxThreshold multiplied by this argument.
     *                 It is usally employed to give here the maximum of the
     *                 preedges, which may be computed previously.
     *                   
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& preedges,
               const channel& orientation,
                     channel8& edges,
               const float maxPreedge=1.0f) const;


    /**
     * operates on a copy of the given %parameters.
     * @param preedges channel containing pre-edges, i.e. only some
     *                 confidence values are expected, where the 
     *                 maxima represent edges.  Only those values
     *                 greater than thresholdMax times the maximum
     *                 value here will be considered.
     * @param orientation channel with the angles.  It is an angle in the
     *                 image coordinate system, i.e. a positive angle direction
     *                 is clockwise.  The units must be radians and the values
     *                 MUST be zero or * positive between 0.0f and 2*Pi.  See
     *                 documentation of * parameters::checkAngle for more
     *                 information.
     * @param relevance the thresholds will be computed "dividing" the ones
     *                 given in the parameters by the corresponding element
     *                 in this channel.
     * @param edges    After the suppression, only the edges will
     *                 be left.
     * @param maxPreedge Optional argument.
     *                 The algorithm uses as maximum threshold the given
     *                 parameters::maxThreshold multiplied by this argument.
     *                 It is usally employed to give here the maximum of the
     *                 preedges, which may be computed previously.
     *                   
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& preedges,
               const channel& orientation,
               const channel& relevance,
                     channel8& edges,
               const float maxPreedge=1.0f) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nonMaximaSuppression& copy(const nonMaximaSuppression& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    nonMaximaSuppression& operator=(const nonMaximaSuppression& other);

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
     * interpolate between two values (bilinear interpolation)
     * @param y1 value at x==0
     * @param y2 value at x==1
     * @param fOffset value in range of 0..1 to beinterpolated at
     * @return returns the interpolated value
     */
    inline float interpolate(const float y1,
                             const float y2,
                             const float fOffset) const;

    /**
     * suppress the maxima.
     * This method assumes the edges channel has been adecuatelly resized.
     *
     * The preedges and orientation channels MUST be connected.
     */
    void nonMaxSuppression(const channel& preedges,
                           const channel& orientation,
                           channel8& edges,
                           const float thresholdMin) const;

    /**
     * hysteresis 
     */
    void hysteresis(const channel& preedge,
                    const channel8& maxima,
                    const float thresholdMax,
                    channel8& dest) const;

    /**
     * compute the real thresholMin and threshodMax values to be used.
     *
     * @param grad magnitude of the gradient
     * @param maxGrad max value in grad
     * @param thresholdMin minimum threshold for a pixel to be possible an edge
     * @param thresholdMax maximum threshold to be reached by a pixel to be
     *                     definitively an edge.
     */
    void thresholdValues(const channel& grad,
                         const float maxGrad,
                         float& thresholdMin,
                         float& thresholdMax) const;



    /**
     * ensure that the angles are between 0.0f and 2*Pi.
     */
    void checkOrientation(const channel& src,channel& dest) const;

    /**
     * Find the edge points in the given src and create a new endPts channel
     * with only those points.
     *
     * At the same time a kd-Tree will be build, containing all end-points.
     * This allows an efficient way to search for other end points
     * in the neighborhood.
     *
     * @param src edge mask
     * @param endPts resulting end-point mask
     * @param endPtsList list of end points
     *
     * @return the number of end-points found
     */
    int findEndPoints(const channel8& src,
                      channel8& endPts,
                      pointList& endPtsList) const;

    
    /**
     * Track points back the given number of pixels
     *
     * This method is used to track back the end points to get enough
     * information about the possible extrapolations.
     *
     * The result is a list of corresponding delta values to extrapolate
     * the line beginning with an end point.
     */
    bool trackPoints(const channel8& edges,
                     const pointList& endPtsList,
                     const channel& orientation,
                     tpointList<float>& deltas) const;

    /**
     * fill the gaps
     */
    bool fillGaps(const channel8& edges,
                  const channel& gradMag,
                  const pointList& endPtsList,
                  const tpointList<float>& deltas,
                  channel8& dest) const;

  private:
    /**
     * add the points a and b ensuring that they remain in a valid range 
     * defined by the size and leave the result in q.  A reference to q
     * is returned
     */
    inline const point& add(const point& a,
                            const point& b,
                            const point& size,
                                  point& q) const;

  };
}

#endif

