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
 * file .......: ltiLkTracker.h
 * authors ....: Frederik Lange
 * organization: LTI, RWTH Aachen
 * creation ...: 26.4.2001
 * revisions ..: $Id: ltiLkTracker.h,v 1.18 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LK_TRACKER_H_
#define _LTI_LK_TRACKER_H_

#include "ltiImage.h"
#include "ltiModifier.h"
#include "ltiPointList.h"
#include "ltiGaussianPyramid.h"
#include "ltiCornerDetector.h"

namespace lti {

  /**
   * This functor implements a hierachical Lucas Kanade optical flow.
   * It is used for feature tracking through a image sequence.
   * Therefore it uses a corner detector to find the initial features
   *
   * For more details please look at Jean-Yves Bouguet's paper "Pyramidal
   * Implementation of the Lucas Kanade Feature Tracker.  Description of the
   * algorithm"
   *
   */
  class lkTracker : public modifier {
  public:

    /**
     * constant which describes a point outOfRange.  In general, a point
     * with negative coordinates can be considered out of range
     */
    static const tpoint<float> outOfRange;

    /**
     * the parameters for the class lkTracker
     */
    class parameters : public modifier::parameters {
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
       * write the parameters in the given ioHandler
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
       * WindowSize gives the size of the window used for comparsion
       * of the features.
       *
       * Default value is 4
       */
      int windowSize;

      /**
       * NumLevels gives the number of levels in the Pyramid.
       *
       * Default value is 4
       */
      int numLevels;

      /**
       * maximal number of iteration steps per pyramid level.
       * Default value: 10
       */
      int maxStepsPerLevel;

      /**
       * This is the square of the computed pixel residual which must be
       * reached before the algorithm stops.
       * Default value: 0.0009f (0.03*0.03);
       */
      float eta2Threshold;

      /**
       * if the difference between both matched windows is greater
       * than this threshold, the point will be marked as invalid, and it
       * will not be tracked any more.
       * Default value: 0.99f
       */
      float errorThreshold;

      /**
       * @name Corner Extraction
       * The following parameters controls the internal corner detection
       * algorithms.
       */
      //@{
      /**
       * if true, a corner detector is used.
       *
       * Default is true
       */
      bool autoCorners;

      /**
       * set the corner detector to be used.  A copy of the given
       * functor will be made.
       */
      bool setCornerDetector(const cornerDetector& cdet);

      /**
       * get a reference to the corner detector functor being used
       */
      cornerDetector& getCornerDetector();

      /**
       * get a read only reference to the corner detector object being used
       */
      const cornerDetector& getCornerDetector() const;
      //@}

    protected:
      /**
       * cornerDetector used to generate the corners.
       *
       * Default value:
       */
      cornerDetector* cornerFinder;
    };

    /**
     * default constructor
     */
    lkTracker();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    lkTracker(const lkTracker& other);

    /**
     * destructor
     */
    virtual ~lkTracker();

    /**
     * returns the name of this type ("lkTracker")
     */
    virtual const char* getTypeName() const;

    /**
     * Track the points in the src channel and return the new positions in
     * the given point list.
     * Please note that internally the pointList contains points with
     * <code>float</code> precision, and to return an <code>pointList</code>
     * they need to be casted.  Is faster if you try to use directly the
     * other apply member, which works directly with the float-points.
     *
     * @param src the channel with the points being tracked.
     *            The first presentation (or the next presentation after a
     *            reset() ) is used for initialization only.  The
     *            next ones are used to track the first given point set, and
     *            the results will be left in <code>featurePoints</code>.
     *
     * @param featurePoints the first time the apply is called (or after a
     *            reset() ), this list contain the positions of the points to
     *            track.  If the attribute <code>autoCorners</code> of the
     *            parameters is true, the content of this list will be
     *            discarded and the positions  of the automatic detected
     *            corners will be left here.  Points getting
     *            invalid during tracking are marked by negative values in the
     *            point list.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, pointList& featurePoints);

    /**
     * Track the points in the src channel and return the new positions in
     * the given point list.
     * Please note that internally the pointList contains points with
     * <code>float</code> precision, and to return an <code>pointList</code>
     * they need to be casted.  Is faster if you try to use directly the
     * other apply member, which works directly with the float-points.
     *
     * @param src the channel with the points being tracked.
     *            The first presentation (or the next presentation after a
     *            reset() ) is used for initialization only.  The
     *            next ones are used to track the first given point set, and
     *            the results will be left in <code>featurePoints</code>.
     *
     * @param featurePoints the first time the apply is called (or after a
     *            reset() ), this list contain the positions of the points to
     *            track.  If the attribute <code>autoCorners</code> of the
     *            parameters is true, the content of this list will be
     *            discarded and the positions  of the automatic detected
     *            corners will be left here.  Points getting
     *            invalid during tracking are marked by negative values in the
     *            point list.
     * @param validPoints returns the number of valid points in the list
     *            feature points.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, pointList& featurePoints,int& validPoints);

    /**
     * Track the points in the src channel and return the new positions in
     * the given point list.
     *
     * @param src the channel with the points being tracked.
     *            The first presentation (or the next presentation after a
     *            reset() ) is used for initialization only.  The
     *            next ones are used to track the first given point set, and
     *            the results will be left in <code>featurePoints</code>.
     *
     * @param featurePoints the first time the apply is called (or after a
     *            reset() ), this list contain the positions of the points to
     *            track.  If the attribute <code>autoCorners</code> of the
     *            parameters is true, the content of this list will be
     *            discarded and the positions  of the automatic detected
     *            corners will be left here.  Points getting
     *            invalid during tracking are marked by negative values in the
     *            point list.
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, tpointList<float>& featurePoints);

    /**
     * Track the points in the src channel and return the new positions in
     * the given point list.
     *
     * @param src the channel with the points being tracked.
     *            The first presentation (or the next presentation after a
     *            reset() ) is used for initialization only.  The
     *            next ones are used to track the first given point set, and
     *            the results will be left in <code>featurePoints</code>.
     *
     * @param featurePoints the first time the apply is called (or after a
     *            reset() ), this list contain the positions of the points to
     *            track.  If the attribute <code>autoCorners</code> of the
     *            parameters is true, the content of this list will be
     *            discarded and the positions  of the automatic detected
     *            corners will be left here.  Points getting
     *            invalid during tracking are marked by negative values in the
     *            point list.
     * @param validPoints returns the number of valid points in the list
     *            featurePoints
     *
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, tpointList<float>& featurePoints,
               int& validPoints);

    /**
     * Resets the internal state but not the parameters
     */
    bool reset();

    /**
     * Reset some points in the tracking list.
     *
     * Please note that this reset takes effect only if there is already
     * a tracking point list in the functor, i.e. if a "total" reset was
     * already done followed by an apply.
     *
     * @param whichPoints Only those points in the list with valid
     * values (coordinates > 0) will be reset: they will get this
     * valid value.  All invalid points in the list, i.e. with
     * coordinates lower than zero will be keped as they are.
     * Please note that point sequence in this list must correspond to
     * the sequence of the tracking list!
     */
    bool reset(const tpointList<float>& whichPoints);

    /**
     * Reset some points in the tracking list.
     *
     * Please note that this reset takes effect only if there is already
     * a tracking point list in the functor, i.e. if a "total" reset was
     * already done followed by an apply.
     *
     * @param whichPoints Only those points in the list with valid
     * values (coordinates > 0) will be reset: they will get this
     * valid value.  All invalid points in the list, i.e. with
     * coordinates lower than zero will be keped as they are.
     * Please note that point sequence in this list must correspond to
     * the sequence of the tracking list!
     */
    bool reset(const pointList& whichPoints);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lkTracker& copy(const lkTracker& other);

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
     * a position/direction vector type
     */
    class vec2d {
    public:
      /**
       * Default constructor.  Intialize all members with 0.0f
       */
      vec2d() : error(0.0f) {};

      /**
       * default destructor
       */
      ~vec2d(){};

      /**
       * position of the origin of the vector
       */
      tpoint<float> p;

      /**
       * direction of the vector
       */
      tpoint<float> d;

      /**
       * error term
       */
      float error;
    };

  private:
    /**
     * automatic detection of corners
     */
    void findInitialPoints(const channel& img,pointList& initialPoints);

    /**
     * exchange both pyramids and generate the new gassian pyramid for
     * the given channel in the pyramid B.
     */
    void buildPyramid(const channel& img);

    /**
     * generate the gradient of the given channel
     */
    void buildGradientMaps(const gaussianPyramid<channel>& src,
                           channel*& gradX,
                           channel*& gradY);

    /**
     * return the result of a bilinear interpolation for the value at(y,x)
     */
    inline float getBilinear(const float x, const float y, const channel& map);

    /**
     * indicates if the tracker is initialised
     */
    bool initialised;

    /**
     * the two pyramids for the downsampled images
     * These are pointers to allow efficient exchange from B to A without
     * copying anything.
     */
    gaussianPyramid<channel>
      *pyramidA,
      *pyramidB;

    /**
     * the flow vectors between the images
     */
    std::list<vec2d> flowVectors;

  };
}

#endif

