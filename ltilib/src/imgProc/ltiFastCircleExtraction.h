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
 * file .......: ltiFastCircleExtraction.h
 * authors ....: Ingo Grothues (ingo@isdevelop.de)
 * organization: LTI, RWTH Aachen
 * creation ...: 22.4.2004
 * revisions ..: $Id: ltiFastCircleExtraction.h,v 1.12 2005/08/01 11:23:32 libuda Exp $
 */

#ifndef _LTI_FAST_CIRCLE_EXTRACTION_H_
#define _LTI_FAST_CIRCLE_EXTRACTION_H_

#include "ltiFeatureExtractor.h"
#include "ltiPoint.h"
#include <vector>

namespace lti {
    
  /**
   * This functor implements a fast circle extraction
   * using the line segments found in digital images.
   * 
   * To achieve this, the lines of an image are extracted
   * by a fast line extraction. The edge lines of circles are
   * filtered by several checks. In further passes these lines 
   * are combined to arcs, adjacent arcs are combined to extended 
   * arcs which at last are used to extract circles.
   *
   * The fastCircleExtraction::parameters specify several
   * ranges and conditions that the lines and arcs must fulfill.
   * The result strongly depends on these parameters,
   * especially the number and the precision of the 
   * extracted circles.
   *
   * The extracted arcs and circles are stored in STL vector
   * containers of the types \<arcEntry\>, \<extArcEntry\> and 
   * \<circleEntry\>. They can be accessed by the methods 
   * getArcList(), getExtArcList and getCircleList().
   *
   * \c arcEntry is the data struct for arcs:
   * <pre> struct arcEntry
   * {
   *   ipoint  start;    // start point
   *   ipoint  end;      // end point
   *   ipoint  mid;      // midpoint 
   *   int     r2;       // length or squared radius
   *   ipoint  firstVec; // first line vector                 
   *   ipoint  lastVec;  // last line vector                  
   *   int     used;     // counts arc<->extarc assignments
   *   int     group;    // base line group (1,2,3 or 4)
   *   std::vector<int> *lineIdxList;  // base line index list
   * };</pre>
   *
   * \c extArcEntry is the data struct for extended arcs:
   * <pre> struct extArcEntry
   * {
   *   ipoint  start;     // start point
   *   ipoint  end;       // end point
   *   ipoint  mid;       // circle midpoint 
   *   int     r2;        // squared radius
   *   ipoint  firstMid;  // midpoint of the first segment
   *   double  firstTang; // tangent of the first segment
   *   ipoint  lastMid;   // midpoint of the last segment
   *   double  lastTang;  // tangent of the last segment
   *   int     used;      // counts extarc<->circle assignments
   *   int     group;     // ext arc group (1,2,3 or 4)
   *   int     arcIdxA;   // 1st base arc index (groups I:1, II:3, III:2, IV:4)
   *   int     arcIdxB;   // 2nd base arc index (groups I:3, II:2, III:4, IV:1)
   * };</pre>
   *
   * \c circleEntry is the data struct for circles:
   * <pre>struct circleEntry
   * {
   *   ipoint center;     // circle center
   *   int    radius;     // circle radius
   *   double coverage;   // circle coverage (accuracy strongly depends on radius and other effects)
   * };</pre>
   *
   * The fast circle extraction does not perform any changes
   * to the analyzed image. To visualize the extracted circles
   * they have to be drawn by other code using the
   * circle lists and a drawing functor.
   *
   *
   * Example:
   * \code
   *
   * #include "ltiViewer.h"               // visualization tool
   * #include "ltiDraw.h"                 // drawing tool
   * #include "ltiALLfunctor.h"           // image loader
   * #include "ltiCannyEdges.h"           // edge detector
   * #include "ltiFastCircleExtraction.h" // circle detector
   * 
   * // ...
   * 
   * lti::image inputImg;
   * lti::channel8 edges;
   * lti::channel8 result;
   * 
   * // load image
   * lti::loadImage loader;
   * loader.load("myImage.bmp", inputImg);
   * 
   * // we need the edges
   * lti::cannyEdges canny;
   * canny.apply(inputImg, edges);
   * 
   * // create FCE functor (using default parameters)
   * lti::fastCircleExtraction fce;
   * 
   * // extract some circles
   * fce.apply(edges);
   * 
   * // get the circle list
   * std::vector<lti::fastCircleExtraction::circleEntry> &circles = fce.getCircleList();
   * 
   * // create a drawing functor
   * lti::draw<lti::channel8::value_type> drawer;
   * drawer.use(result);
   * drawer.setColor(255);
   * 
   * // create background with grey edges
   * result = edges;
   * result /= 2;
   * 
   * for(int i=0; i<circles.size(); i++) {
   *   // draw all circles
   *    drawer.circle(circles[i].center.x, circles[i].center.y, circles[i].radius, false);
   *   // print circle data
   *   printf("circle[%i]: center=(%i,%i) radius=%i coverage=%.1f%% \n",
   *          i, circles[i].center.x, circles[i].center.y, circles[i].radius, circles[i].coverage*100);
   * }
   * 
   * // show the extracted circles
   * lti::viewer view("Detected circles");
   * view.show(result);
   *
   * // wait for a mouse click in the viewer window
   * view.waitButtonPressed();
   *
   * \endcode
   */

  class fastCircleExtraction : public featureExtractor {
  public:
  
    struct segmEntry
    {
      ipoint  start;    // start point
      ipoint  end;      // end point
      int     len;      // length
      int     used;     // counts segment<->line assignments
    };

    struct lineEntry
    {
      ipoint  start;    // start point
      ipoint  end;      // end point
      ipoint  mid;      // midpoint 
      double  tangent;  // line tangent
      int     len;      // length
      int     used;     // counts line<->arc assignments
      int     group;    // base segment group (1,2,3 or 4)
      std::vector<int> *segmIdxList;  // base segment index list
    };

    struct arcEntry
    {
      ipoint  start;    // start point
      ipoint  end;      // end point
      ipoint  mid;      // estimated circle midpoint 
      int     r2;       // estimated squared radius
      ipoint  firstVec; // first line vector
      ipoint  lastVec;  // last line vector
      int     used;     // counts arc<->extarc assignments
      int     group;    // base line group (1,2,3 or 4)
      std::vector<int> *lineIdxList;  // base line index list
    };

    struct extArcEntry
    {
      ipoint  start;    // start point
      ipoint  end;      // end point
      ipoint  mid;      // estimated circle midpoint 
      int     r2;       // estimated squared radius
      ipoint  firstMid; // midpoint of the first segment
      double  firstTang;// tangent of the first segment
      ipoint  lastMid;  // midpoint of the last segment
      double  lastTang; // tangent of the last segment
      int     used;     // counts extarc<->circle assignments
      int     group;    // ext arc group (1,2,3 or 4)
      int     arcIdx[2];// base arc indices (groups I:3, II:2, III:4, IV:1)
    };

    struct circleEntry
    {
      ipoint  center;   // circle center
      int     radius;   // circle radius
      double  coverage; // circle coverage (accuracy strongly depends on radius and other effects)
      std::vector<int> *mergedArcs;  // extarc index list
    };

  
    /**
     * the parameters for the class fastCircleExtraction
     */
    class parameters : public featureExtractor::parameters {
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

      /** This threshold constant defines the maximum mismatch
       *  between the found line segments and the estimated analog line.
       *  The segment is discarded if its starting/finishing pixel 
       *  differs more than <code>maxQuantizationError</code>
       *  from the estimated analog line.
       *  If this value is greater than 1.0 the line detection will
       *  also extract lines that are not completely straight, which
       *  may be useful for estimation of lines.
       *  possible values are
       *  <pre>
       *  0.5 < maxQuantizationError
       *  </pre>
       *  A high maxQuantizationError value will extract longer lines
       *  at the cost of precision. Low values result in better pixel
       *  matching but also in shorter lines.
       *  Best compromise between pixel matching and good detection
       *  is a value between 0.6 and 0.7 (the default is 0.79)
       */
      float maxQuantizationError;


      /**
       *  This constant defines how much the value of two adjacent pixels  
       *  may differ and still be considered pixels of the same segment.
       *  A <code>segmentTolerance</code> of 0 will only group pixels
       *  of the same value to one segment. When analyzing binary maps
       *  such as edge-images a value of 0 should be used.
       *  possible values are
       *  <pre>
       *  0 <= segmentTolerance <= 254
       *  </pre>
       *  Higher segmentTolerance values allow non-binary
       *  images to be analysed but may also result in wrong 
       *  segment start/end detection. 
       *  The default is 0
       */
      int segmentTolerance;


      /**
       *  The maximum gap two line segments may have and
       *  still be considered segments of the same line.
       *  This parameter changes the size of the searching window
       *  for line segments. Small sizes allow fast processing but may
       *  also cause detection failures (as well as too high values)
       *  A <code>maxSegmentGap</code> of 1 will only group
       *  contiguous segments to a line.
       *  possible values are
       *  <pre>
       *  maxSegmentGap >= 1 (pixel)
       *  </pre>
       *  Higher maxSegmentGap values also allow extraction of
       *  non-contiguous lines such as found in corrupted images.
       *  The default is 1
       */
      int maxSegmentGap;


      /**
       *  The minimum length a line must have to get extracted.
       *  The parameter <code>minLineLen</code> is used as 
       *  threshold, smaller lines will be ignored.
       *  possible values are
       *  <pre>
       *  minLineLen >= 3 (pixel)
       *  </pre>
       *  Values greater than 3 will strongly decrease
       *  the number of extracted lines and therefore speed up
       *  the following extraction stages. For most circles
       *  the optimal value lies between 5 and 7.
       *  The default is 6
       */
      int minLineLen;


      /**
       *  The minimum length a line segment must have to be detected.
       *  The parameter <code>minSegmLen</code> is used as 
       *  threshold, smaller segments will be ignored.
       *  Values greater than 2 strongly decrease the number
       *  of detected segments and the size of the segment lists.
       *  possible values are
       *  <pre>
       *  minSegmLen >= 2 (pixel)
       *  </pre>
       *  Values greater than 2 result in smaller segment lists
       *  and therefore allow slightly faster line detection.
       *  But mostly good line detection is more important than
       *  a small speedup, so don't change this parameter unless
       *  there is a good reason to do so.
       *  The default is 2
       */
      int minSegmLen;


      /**
       *  The maximum gap two lines may have and
       *  still be considered lines of the same arc.
       *  This parameter changes the size of the searching window
       *  for lines. Best results were obtained with small values,
       *  because large window sizes tend to combine adjacent parallel
       *  lines to one arc which causes detection failures.
       *  A <code>maxLineGap</code> of 1 will only group
       *  contiguous lines to an arc.
       *  possible values are
       *  <pre>
       *  maxLineGap >= 1 (pixel)
       *  </pre>
       *  Higher maxLineGap values also allow extraction of
       *  non-contiguous arcs such as found in corrupted images.
       *  The default is 2
       */
      int maxLineGap;


      /**
       *  The maximum gap two arcs may have and
       *  still be considered arcs of the same extended arc.
       *  This parameter changes the size of the searching window
       *  for arcs. Best results were obtained with large values,
       *  because every arc has to find at least one other to
       *  remain in the extraction process. (the gap between
       *  these arcs mostly lies between 10 and 40 pixels)
       *  A <code>maxArcGap</code> of 1 will only group
       *  contiguous arcs to an extended arc.
       *  possible values are
       *  <pre>
       *  maxArcGap >= 1 (pixel)
       *  </pre>
       *  Higher maxArcGap values also allow extraction of
       *  non-contiguous arcs such as found in corrupted images.
       *  The default is 30
       */
      int maxArcGap;


      /** This parameter defines the maximum difference that
       *  the tangents of two lines may have in relation to the
       *  tangents of the arc that these lines represent.
       *  In other words, this parameter affects the tolerance
       *  in terms of roundness of the countour.
       *  Line pairs which exceed this threshold won't be 
       *  combined to an arc. Too large values result in 
       *  incorrect arcs and detection failures.
       *  reasonable values are
       *  <pre>
       *  8.5 <= maxLineTangentError <= 20
       *  </pre>
       *  Small values cause stricter circle checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 10 and 15 (the default is 15)
       */
      float maxLineTangentError;


      /** This parameter defines the maximum difference that
       *  the tangents of two arcs may have in relation to the
       *  tangents of the circle that these arcs represent.
       *  In other words, this parameter affects the tolerance
       *  in terms of circle matching.
       *  Arc pairs which exceed this threshold won't be 
       *  combined to an extended arc. Too large values result 
       *  incorrect extended arcs and detection failures.
       *  reasonable values are
       *  <pre>
       *  8.5 <= maxArcTangentError <= 20
       *  </pre>
       *  Small values cause stricter circle checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 10 and 15 (the default is 12)
       */
      float maxArcTangentError;


      /** This parameter defines the maximum distance
       *  between the start and end points of one arc and
       *  the estimated circle contour of the other arc.
       *  Arc pairs which exceed this threshold won't be 
       *  combined to an extended arc. Too large values 
       *  cause detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxExtArcMismatch <= 10
       *  </pre>
       *  Small values cause stricter arc matching checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 1.5 and 4 (the default is 2.75)
       */
      float maxExtArcMismatch;


      /** This parameter defines the minimal ratio
       *  of the estimated circle radii of two arcs. 
       *  Arc pairs which under-run this threshold won't be 
       *  combined to an extended arc. Too small values 
       *  cause detection failures.
       *  reasonable values are
       *  <pre>
       *  0.5 <= minExtArcMatchRatio <= 0.95
       *  </pre>
       *  Large values cause stricter arc matching checks
       *  whereas small values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 0.7 and 0.9 (the default is 0.8)
       */
      float minExtArcMatchRatio;


      /** This parameter is a toggle to switch between
       *  the old and the new extended arc checks. 
       *  In general the new checks should be better,
       *  but in some situations the old check obtain
       *  better results. 
       *  The old check required only one threshold 
       *  parameter: <code>oldExtArcThreshold</code>
       *  possible values are
       *  <pre>
       *  true or false
       *  </pre>
       *  (the default is false)
       */
      bool oldExtArcCheck;


      /** This parameter defines an admeasurement
       *  in terms of circle arc matching. 
       *  Arc pairs which exceed this threshold won't be 
       *  combined to an extended arc. Too large values 
       *  cause detection failures. The threshold check
       *  gets stricter with larger radii because of 
       *  comparing squared distances. This is the main
       *  difference between the old and new ExtArc check.
       *  reasonable values are
       *  <pre>
       *  50 <= oldExtArcThreshold <= 250
       *  </pre>
       *  Small values cause stricter arc matching checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 80 and 120 (the default is 100)
       */
      int oldExtArcThreshold;


      /** This parameter defines the maximum gap between
       *  the estimated circle centers of two extended arcs. 
       *  ExtArc pairs which exceed this threshold won't be 
       *  combined to a circle. Too large values cause 
       *  detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxCenterMismatch <= 10
       *  </pre>
       *  Small values cause stricter extArc matching checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 1.5 and 4 (the default is 2.5)
       */
      float maxCenterMismatch;


      /** This parameter defines the minimal ratio
       *  of the estimated circle radii of two extended arcs. 
       *  ExtArc pairs which underrun this threshold won't be 
       *  combined to a circle. Too small values cause 
       *  detection failures.
       *  reasonable values are
       *  <pre>
       *  0.5 <= minRadiusMatchRatio <= 0.95
       *  </pre>
       *  Large values cause stricter extArc matching checks
       *  whereas small values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 0.7 and 0.9 (the default is 0.75)
       */
      float minRadiusMatchRatio;


      /** This parameter defines the minimal coverage for estimated
       *  circles to get extracted. That is the ratio of the estimated
       *  circumference and the number of extracted arc pixels.
       *  Circles which underrun this threshold won't get extracted.
       *  Too small values cause extraction of circles with randomly
       *  matching arcs. Also this threshold shouldn't be set to values
       *  greater than 0.7, since only well fitting parts of a circle 
       *  are extracted. Therefore even a completely present circle  
       *  may get only 70%% coverage.
       *  reasonable values are
       *  <pre>
       *  0.2 <= minCoverage <= 0.7
       *  </pre>
       *  Best compromise between accuracy and tolerance
       *  is a value between 0.3 and 0.5 (the default is 0.3)
       *  Note: Very close and similar circles are sometimes
       *  merged to one circle which shows more than 100%% coverage.
       */
      float minCoverage;
    };

////////////////////////////////////////////////////////////////////////////

    /**
     * default constructor
     */
    fastCircleExtraction();

    /**
     * Construct a functor using the given parameters
     */
    fastCircleExtraction(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fastCircleExtraction(const fastCircleExtraction& other);

    /**
     * destructor
     */
    virtual ~fastCircleExtraction();

    /**
     * returns the name of this type ("fastCircleExtraction")
     */
    virtual const char* getTypeName() const;

    /**
     * Applies fast circle detection on-place.
     * @param srcdest channel8 with the source data.
     * @return true if apply was successful or false otherwise.
     */
    bool apply(const channel8& srcdest);

    /**
     * Applies fast circle detection on-place.
     * @param srcdest channel with the source data.
     * @return true if apply was successful or false otherwise.
     */
    bool apply(const channel& srcdest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastCircleExtraction& copy(const fastCircleExtraction& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastCircleExtraction& operator=(const fastCircleExtraction& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


///////////////////////////////////////////////////////////

    /**
     * returns a vector with line segments of the specified group
     * @param group_number number of the line segment group (1,2,3,4)
     * @return vector with detected line segments
     */
    virtual std::vector<segmEntry>& getSegmentList(const int group_number);

    /**
     * returns a vector with lines of the specified group
     * @param group_number number of the line group (1,2,3,4, or 0 for all lines)
     * @return vector with extracted lines
     */
    virtual std::vector<lineEntry>& getLineList(const int group_number);

    /**
     * returns a vector with arcs of the specified group
     * @param group_number number of the arc group (1,2,3,4, or 0 for all arcs)
     * @return vector with extracted arcs
     */
    virtual std::vector<arcEntry>& getArcList(const int group_number);

    /**
     * returns a vector with extended arcs of the specified group
     * @param group_number number of the arc group (1,2,3,4, or 0 for all arcs)
     * @return vector with extracted extended arcs
     */
    virtual std::vector<extArcEntry>& getExtArcList(const int group_number);

    /**
     * returns a vector with extracted circles.
     * @return vector with extracted circles
     */
    virtual std::vector<circleEntry>& getCircleList();


private:

    /**
     * clears a line list and its allocated objects
     * @param group_number number of the line group (1,2,3,4)
     */
    virtual void clearLineList(const int group_number);

    /**
     * clears an arc list and its allocated objects
     * @param group_number number of the arc group (1,2,3,4)
     */
    virtual void clearArcList(const int group_number);

    /**
     * clears an extended arc list and its allocated objects
     * @param group_number number of the arc group (1,2,3,4)
     */
    virtual void clearExtArcList(const int group_number);

    /**
     * clears the circle list and its allocated objects
     */
    virtual void clearCircleList();

    /**
     * The segment detection methods. They detect segments by scanning pixel rows.
     * For performance and review reasons there are 4 methods (one per segment group).
     * Results are stored in STL vectors of type <segmEntry>. These vectors can
     * be accessed by the method getSegmentList().
     * @param src channel with source data
     */
    virtual void detectGroup1Segments(const channel8& src);
    virtual void detectGroup2Segments(const channel8& src);
    virtual void detectGroup3Segments(const channel8& src);
    virtual void detectGroup4Segments(const channel8& src);

    /**
     * The line extraction methods. They extract lines by tracking line segments.
     * For performance and review reasons there are 4 methods (one per segment group).
     * Results are stored in STL vectors of type <lineEntry>. These vectors can
     * be accessed by the method getLineList().
     */
    virtual void extractGroup1Lines();
    virtual void extractGroup2Lines();
    virtual void extractGroup3Lines();
    virtual void extractGroup4Lines();

    /**
     * The arc extraction methods. They extract arcs by tracking lines.
     * For performance and review reasons there are 4 methods (one per segment group).
     * Results are stored in STL vectors of type <arcEntry>. These vectors can
     * be accessed by the method getArcList().
     */
    virtual void extractGroup1Arcs();
    virtual void extractGroup2Arcs();
    virtual void extractGroup3Arcs();
    virtual void extractGroup4Arcs();

    /**
     * The extended arc extraction methods. They extract extended arcs by tracking arcs.
     * For performance and review reasons there are 4 methods (one per segment group).
     * Results are stored in STL vectors of type <extArcEntry>. These vectors can
     * be accessed by the method getExtArcList().
     */
    virtual void extractExtGroup1Arcs();
    virtual void extractExtGroup2Arcs();
    virtual void extractExtGroup3Arcs();
    virtual void extractExtGroup4Arcs();

    /**
     * Calculates and checks the tangent matching of two extended arcs.
     * @param extarc1 first extarc to be checked
     * @param extarc2 second extarc to be checked
     * @param Xest x coordinate of estimated circle center
     * @param Yest y coordinate of estimated circle center
     * @return the number of tangent mismatches
     */
    virtual int checkExtArcTangents(const extArcEntry &extarc1,
                                    const extArcEntry &extarc2,
                                    const double Xest,
                                    const double Yest);

    /**
     * The circle extraction method.
     * Circles are extracted by merging extended arcs. Each extArc will be checked
     * against each other. This algorithm has a squared complexity but as long as 
     * the number of extArcs doesn't exceed a few hundred, it will be fast enough.
     */
    virtual void extractCircles();

    /**
     * estimates circle parameters by using a least square fitting algorithm.
     * @param idxlist vector with arc line index numbers
     * @param group group number of the arc lines
     * @param Xest the estimated x-coordinate
     * @param Yest the estimated y-coordinate
     * @param R2est the estimated squared radius
     * @return true if calculation was successful otherwise false
     */
    virtual bool estimateCircle(std::vector<int> &idxlist, const int group,
                                double &Xest, double &Yest, double &R2est);

    /**
     * estimates circle parameters by using a least square fitting algorithm.
     * @param idxlist1 first vector with arc line index numbers
     * @param group1 group number of the first arc lines
     * @param idxlist2 second vector with arc line index numbers
     * @param group2 group number of the second arc lines
     * @param Xest the estimated x-coordinate
     * @param Yest the estimated y-coordinate
     * @param R2est the estimated squared radius
     * @return true if calculation was successful otherwise false
     */
    virtual bool estimateCircle(std::vector<int> &idxlist1, const int group1,
                                std::vector<int> &idxlist2, const int group2,
                                double &Xest, double &Yest, double &R2est);

    /**
     * estimates circle parameters by using a least square fitting algorithm.
     * @param arc1 first extended arc
     * @param arc2 second extended arc
     * @param Xest the estimated x-coordinate
     * @param Yest the estimated y-coordinate
     * @param R2est the estimated squared radius
     * @return true if calculation was successful otherwise false
     */
    virtual bool estimateCircle(const extArcEntry &arc1, const extArcEntry &arc2, 
                                double &Xest, double &Yest, double &R2est);


    /**
     * estimates circle parameters by using a least square fitting algorithm.
     * @param start1 startpoint of 1st extended arc
     * @param end1 endpoint of 1st extended arc
     * @param start2 startpoint of 2nd extended arc
     * @param end2 endpoint of 2nd extended arc
     * @param Xest the estimated x-coordinate
     * @param Yest the estimated y-coordinate
     * @param R2est the estimated squared radius
     * @return true if calculation was successful otherwise false
     */
    virtual bool estimateCircle(const ipoint &start1, const ipoint &end1, 
                                const ipoint &start2, const ipoint &end2,
                                double &Xest, double &Yest, double &R2est);

    /**
     * estimates circle parameters by using a least square fitting algorithm.
     * @param lines vector with arc lines
     * @param Xest the estimated x-coordinate
     * @param Yest the estimated y-coordinate
     * @param R2est the estimated squared radius
     * @return true if calculation was successful otherwise false
     */
    virtual bool estimateCircle(std::vector<lineEntry> &lines, 
                                double &Xest, double &Yest, double &R2est);
                                
    /**
     * The segment containers. (to allow easier implementation
     * the arraysize is 5 although only 4 conatainers are used)
     * m_vecSegments[1..4] contain segment groups I..IV,
     * m_vecSegments[0] is not used. 
     */
    std::vector<segmEntry> m_vecSegments[5];

    /**
     * The line containers. 
     * m_vecLines[1..4] contain line groups I..IV,
     * m_vecLines[0] contains all lines.
     */
    std::vector<lineEntry> m_vecLines[5];

    /**
     * The arc containers.
     * m_vecArcs[1..4] contain arc groups I..IV,
     * m_vecArcs[0] contains all arcs. 
     */
    std::vector<arcEntry> m_vecArcs[5];

    /**
     * The extended arc containers.
     * m_vecExtArcs[1..4] contain extArc groups I..IV,
     * m_vecExtArcs[0] contains all extended arcs. 
     */
    std::vector<extArcEntry> m_vecExtArcs[5];

    /**
     * The circle container.
     */
    std::vector<circleEntry> m_vecCircles;

    /**
     * The input image width. Needed for coordinate transformation 
     * when extracting diagonal lines.
     */
    int m_iWidth;

    /**
     * The number of pre-allocated vector entries.
     */
    static const int SegmPreallocation;
    static const int LinePreallocation;
    static const int ArcPreallocation;
    static const int ExtArcPreallocation;
    static const int CirclePreallocation;
    
    // Circle constant
    static const double PI;

  };
}

#endif

