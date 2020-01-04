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
 * file .......: ltiOrientedHLTransform.h
 * authors ....: Henning Luepschen
 * organization: LTI, RWTH Aachen
 * creation ...: 02.10.2002
 * revisions ..: $Id: ltiOrientedHLTransform.h,v 1.12 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_ORIENTED_HL_TRANSFORM_H_
#define _LTI_ORIENTED_HL_TRANSFORM_H_

#include "ltiTransform.h"

namespace lti {

  /**
   * This functor is used to make a fast Hough Line Transform
   * and creates a channel32 transformation accumulator.
   *
   * The houghLineTransform::parameters specify what part of
   * a given image/channel is transformed and which point values
   * are considered.
   *
   * To accelerate the computations, an orientation map containing the angles
   * perpendicular to the edges is used.  This can be generated using
   * for example lti::orientationMap.
   *
   * Two additional methods help in the use of the hough space.  The first
   * one detects the local maxima ( getHoughSpacePoints() ), and the second
   * one finds two points that define the line segment found in the image
   * ( getSegmentPoints() ).
   *
   * The hough space contains as expected two parameters: radius and angle.
   * They are measured from the middle point of the analysed region.  The angle
   * is always between 0 and 180 degrees.  The radius can be negative and its
   * absolute value will be between 0 and the image diagonal divided by two.
   *
   * Example:
   * \code
   *
   * #include "ltiOrientedHLTransform.h" // the hough transform
   * #include "ltiCannyEdges.h"          // the edge detector
   * #include "ltiALLFunctor.h"          // to read png, jpg and bmp images
   * #include "ltiDraw.h"                // drawing tool
   * #include "ltiViewer.h"              // visualization tool
   *
   * // ...
   *
   * lti::image inputImg;
   * lti::channel8 edges;
   * lti::channel angles; // orientation map
   * lti::channel32 transformAccu;
   *  
   * // load an image and extract the intensity channel from it
   * lti::loadImage imgLoader;
   * imgLoader.load("../img/hough1.png",inputImg); // load an image
   *
   * // we need the edges
   * cannyEdges canny;
   * canny.apply(inputImg,edges,angles);
   *
   * // create default HLT (Hough Line Transform) using default parameters
   * lti::orientedHLTransform HLT;
   * 
   * // The Hough Transform
   * HLT.apply(edges,angles,transformAccu);
   *
   * // Display the Hough space
   * viewer viewHS("Hough Space");
   * viewHS.show(transformAccu);
   *  
   * // Now get the local maxima of the transformAccu
   * lti::vector< tpoint<float> > maxpos;
   * HLT.getHoughSpacePoints(transformAccu,maxpos);
   * 
   * // And draw the found lines in the original image
   * lti::draw<rgbPixel> drawer;
   * drawer.use(inputImg);
   * drawer.setColor(lti::White);
   * point a,b;
   * for (int k=0;k<maxpos.size();++k) {
   *   HLT.getSegmentPoints(inputImg.size(),maxpos.at(k).x,maxpos.at(k).y,a,b);
   *   drawer.line(a,b);
   * }
   * 
   * // at last, show the resulting lines
   * viewer view("Detected lines");
   * view.show(inputImg);
   *
   * view.waitButtonPressed(); // wait until the user presses the mouse button
   *                           // on the viewer window.
   * \endcode
   */
  class orientedHLTransform : public transform {
  public:
    /**
     * the parameters for the class orientedHLTransform
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
       * Part of the input image/channel that will be considered for
       * transformation.
       *
       * If this area is bigger than the image/channel size given in the
       * apply() method, the size of the channel will be used.
       *
       * The default values are (0,0,
       *                         std::numeric_limits<int>::max(),
       *                         std::numeric_limits<int>::max())
       * (i.e. use the whole image)
       */
      trectangle<int> transformationArea;

      /**
       * All points of the input image/channel whose value is greater
       * than the base value are used for transformation.
       * (usually the base value will be 0 (or a little higher) which
       * results in all coloured lines being considered on a black background)
       *
       * For channels (with float values), the comparison value will be
       * baseValue/255.0.
       *
       * Default Value: 0
       */
      int baseValue;

      /**
       * Accuracy of Hough Line Transformation. Determines what kind of
       * line angles can be detected. The following values are
       * allowed:
       * - 45  (4 degrees steps)  :   very inaccurate, but very fast
       * - 90  (2 degrees steps)  :   inaccurate, but fast
       * - 180 (1 degree steps)   :   quite accurate
       * - 360 (1/2 degree steps) :   accurate, but slower
       * - 540 (1/3 degree steps) :   very accurate, but very slow
       * - 720 (1/4 degree steps) :   even more accurate, but even slower...
       * - ...
       *
       * Higher values that are divisible by 180 are also allowed, but
       * do make the transformation _really_ slow.
       *
       * Default value: 180
       */
      int accuracy;

      /**
       * Angle range. The sinusoids are only be drawn from
       * (gradientAngle-range) to (gradientAngle+range). Values lower
       * than 5 are not recommended.
       *
       * Default value is 10.
       */
      int range;

      /**
       * Accumulation modes
       */
      enum eAccumulationMode {
        Classic,  /**< A value of 1 will be accumulated in the hough space
                   *   each time an edge is found with a value greater than
                   *   baseValue
                   */
        Gradient  /**< The value in the given edges image will be accumulated
                   *   allowing to weight the strongness of an edge.  If the
                   *   edges image is a channel, their values will be
                   *   multiplied by 255 before accumulating.
                   */
      };

      /**
       * mode used for the accumulation
       *
       * Default: Classic
       */
      eAccumulationMode accumulationMode;
    };

    /**
     * default constructor, member variable m_iAccuracy is set to 180
     * @see m_iAccuracy()
     * @see setAccuracy()
     * @see getAccuracy()
     */
    orientedHLTransform();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    orientedHLTransform(const orientedHLTransform& other);

    /**
     * destructor
     */
    virtual ~orientedHLTransform();

    /**
     * returns the name of this type ("orientedHLTransform")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data (gradient).
     * @param angle_src channel with gradient angles.
     * @param dest channel32 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,
               const channel& angle_src,
               channel32& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param angle_src channel with gradient angles.
     * @param dest channel32 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,
               const channel& angle_src,
               channel32& dest) const;

    /**
     * Make Hough Transform of the given image and
     * write the transformation accumulator into dest.
     *
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param angle_src channel with gradient angles.
     * @param dest channel32 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel& angle_src,
               channel32& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel32 with the source data.
     * @param angle_src channel with gradient angles.
     * @param dest channel32 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel32& src,
               const channel& angle_src,
               channel32& dest) const;


    /**
     * @name Help methods
     */
    //@{

    /**
     * This method is used to get the list of points in the parameter
     * space, sorted by their hough transformation value, with the
     * maxima at the beginning.
     *
     * @param hough a hough transformation image, as produced by the apply
     *              methods
     * @param positions the positions of the points found in the hough image,
     *                  sorted by the hough value.  The x value will be the
     *                  radius (in pixels) and the y value the angle
     *                  (in radians).
     * @param stdDevFactor the average and std. deviation values
     *                     between all detected maxima will be computed.
     *                     Only those values greater than the average plus this
     *                     parameter multiplied by the std. deviation will be
     *                     considered.  Default value 1.0 should be ok.
     * @param hystheresis  to detect the local maxima a region search starting
     *                     from preliminary maxima will be done.  This value
     *                     multiplied by the preliminary maxima determine the
     *                     threshold that must be reached in order to continue
     *                     the search.  The value must be between 0 and 1.  If
     *                     it is choose too low, just the biggest maxima will
     *                     be found.  If too high, too many maxima will be
     *                     found.  Default value 0.5 work well for the most
     *                     cases.
     */
    bool getHoughSpacePoints(const channel32& hough,
                             vector< tpoint<float> >& positions,
                             const float stdDevFactor = 1.0f,
                             const float hystheresis = 0.5f) const;


    /**
     * Given a radius and an angle, compute the two points at the border of
     * the image.
     * @param imageSize size of the image for which the points a and b
     *                  need to be computed
     * @param radius radius parameter of the hough space
     * @param angle angle parameter of the hough space
     * @param a begin point
     * @param b end point
     * @param origin optional parameter that specifies the origin of
     *               the coordinates system in the image.  If not given
     *               or if it contains negative values, the middle of the
     *               image will be used as origin.
     */
    bool getSegmentPoints(const point& imageSize,
                          const float radius,
                          const float angle,
                          point& a,
                          point& b,
                          const point& origin = point(-1,-1)) const;

    //@}


    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    orientedHLTransform& copy(const orientedHLTransform& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    orientedHLTransform& operator=(const orientedHLTransform& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * returns accuracy of Hough Line Transform
     * @see m_iAccuracy()
     * @return value of member variable m_iAccuracy
     */
    int getAccuracy() const;

    /**
     * sets accuracy of Hough Line Transform and creates the
     * appropriate sinus and cosinus integer tables
     * @see m_iAccuracy()
     * @return true, if successful
     */
    bool setAccuracy( int iAccuracy );

  private:
    /**
     * Create sinus/cosinus integer tables of given size.
     */
    void createSinCosTable(const int iSize);

    /**
     * Pointer to cosinus integer table. We want to speed
     * the transformation up.
     */
    int* m_pCosinus;

    /**
     * Pointer to sinus integer table. We want to speed
     * the transformation up.
     */
    int* m_pSinus;

    /**
     * Accuracy of Hough Line Transform. Determines what kind of
     * line angles can be detected. The following values are
     * allowed:
     *
     * 45  (4 degrees steps)  :   very inaccurate, but very fast
     * 90  (2 degrees steps)  :   inaccurate, but fast
     * 180 (1 degree steps)   :   quite accurate
     * 360 (1/2 degree steps) :   accurate, but slower
     * 540 (1/3 degree steps) :   very accurate, but very slow
     * 720 (1/4 degree steps) :   even more accurate, but even slower...
     * ...
     *
     * Higher values that are divisible by 180 are also allowed, but
     * do make the transformation _really_ slow. 180 is the DEFAULT value.
     *
     * -------------------------------------------------------------------
     *  The private copy of the parameters.accuracy is only kept in order
     *  to check if it is necessary to create _new_ sinus/cosinus tables
     * -------------------------------------------------------------------
     */
    int m_iAccuracy;
  };

}

#endif

