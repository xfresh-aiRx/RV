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
 * file .......: ltiFastEllipseExtraction.h
 * authors ....: Ingo Grothues (ingo@isdevelop.de)
 * organization: LTI, RWTH Aachen
 * creation ...: 23.4.2004
 * revisions ..: $Id: ltiFastEllipseExtraction.h,v 1.9 2005/08/01 11:23:32 libuda Exp $
 */

#ifndef _LTI_FAST_ELLIPSE_EXTRACTION_H_
#define _LTI_FAST_ELLIPSE_EXTRACTION_H_

#include "ltiFeatureExtractor.h"
#include "ltiPoint.h"
#include <vector>

namespace lti {
  
  /**
   * This functor implements a fast ellipse extraction
   * using the line segments found in digital images.
   * 
   * To achieve this, the lines of an image are extracted
   * by a fast line extraction. The edge lines of ellipses are
   * filtered by several checks. In further passes these lines 
   * are combined to arcs, adjacent arcs are combined to extended 
   * arcs which at last are used to extract ellipses.
   *
   * The fastEllipseExtraction::parameters specify several
   * ranges and conditions that the lines and arcs must fulfill.
   * The result strongly depends on these parameters,
   * especially the number and the precision of the 
   * extracted ellipses.
   *
   * The extracted arcs and ellipses are stored in STL vector
   * containers of the types \<ellArcEntry\>, \<ellExtArcEntry\> and 
   * \<ellipseEntry\>. They can be accessed by the methods 
   * getEllArcList(), getEllExtArcList and getEllipseList().
   *
   * \c ellArcEntry is the data struct for elliptic arcs:
   * <pre> struct ellArcEntry
   * {
   *   ipoint  start;    // start point                       
   *   ipoint  end;      // end point                         
   *   ipoint  mid;      // estimated circle midpoint         
   *   int     r2;       // estimated squared radius          
   *   ipoint  firstVec; // first line vector                 
   *   ipoint  lastVec;  // last line vector                  
   *   int     used;     // counts arc<->extarc assignments   
   *   int     group;    // base line group (1,2,3 or 4)      
   *   std::vector<int> *lineIdxList;  // base line index list
   * };</pre>                                                 
   *
   * \c ellExtArcEntry is the data struct for elliptic extended arcs:
   * <pre> struct ellExtArcEntry
   * {
   *   double  x,y;      // estimated ellipse center           
   *   double  a,b;      // estimated ellipse semi-axis        
   *   double  t;        // estimated ellipse orientation angle                  
   *   double  ix, iy;   // intersection point of end tangents 
   *   double  slopeLB;  // slope of line beam                 
   *   int     used;     // counts extarc<->ellipse assignments
   *   int     group;    // group of 2nd base arc                   
   *   int     arcIdx[3];// base arc indices                    
   * };</pre>
   *
   * \c ellipseEntry is the data struct for ellipses:
   * <pre>struct ellipseEntry
   * {
   *   double  x,y;      // estimated ellipse center   
   *   double  a,b;      // estimated ellipse semi-axis
   *   double  t;        // estimated ellipse orientation angle
   *   double  coverage; // ellipse coverage (accuracy 
   *   std::vector<int> *mergedArcs;  // arc index list
   * };</pre>
   *
   * The fast ellipse extraction does not perform any changes
   * to the analyzed image. To visualize the extracted ellipses
   * they have to be drawn by other code using the
   * ellipse lists and a drawing functor.
   *
   *
   * Example:
   * \code
   *
   * #include "ltiViewer.h"                // visualization tool
   * #include "ltiDraw.h"                  // drawing tool
   * #include "ltiALLfunctor.h"            // image loader
   * #include "ltiCannyEdges.h"            // edge detector
   * #include "ltiFastEllipseExtraction.h" // ellipse detector
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
   * // create FEE functor (using default parameters)
   * lti::fastEllipseExtraction fee;
   * 
   * // extract some ellipses
   * fee.apply(edges);
   * 
   * // get the ellipse list
   * std::vector<lti::fastEllipseExtraction::ellipseEntry> &ellipses = fee.getEllipseList();
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
   * for(int i=0; i<ellipses.size(); i++) {
   *   // draw ellipse
   *   lti::point center((int)ellipses[i].x, (int)ellipses[i].y);
   *   drawer.ellipse(center, (int)ellipses[i].b, (int)ellipses[i].a, ellipses[i].t, false);
   *   // print ellipse data
   *   printf("ellipse[%i]: center=(%.0f,%.0f) semiaxis=(%.0f,%.0f) angle=%.1f coverage=%.1f%% \n",
   *          i, ellipses[i].x, ellipses[i].y, ellipses[i].a, ellipses[i].b, ellipses[i].t*180/3.14, ellipses[i].coverage*100);
   * }
   * 
   * // show the extracted ellipses
   * lti::viewer view("Detected ellipses");
   * view.show(result);
   * 
   * // wait for a mouse click in the viewer window
   * view.waitButtonPressed();
   *
   * \endcode
   */
   
  class fastEllipseExtraction : public featureExtractor {
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

    struct ellArcEntry
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

    struct ellExtArcEntry
    {
      double  x,y;      // estimated ellipse center
      double  a,b;      // estimated ellipse semi-axis
      double  t;        // orientation angle
      double  ix, iy;   // intersection point of end tangents
      double  slopeLB;  // slope of line beam
      int     used;     // counts extarc<->ellipse assignments
      int     group;    // group of 2nd arc
      int     arcIdx[3];// ell arc indices
    };

    struct ellipseEntry
    {
      double  x,y;      // estimated ellipse center
      double  a,b;      // estimated ellipse semi-axis
      double  t;        // orientation angle
      double  coverage; // ellipse coverage (accuracy strongly depends on radius and other effects)
      std::vector<int> *mergedArcs;  // extarc index list
    };
  
    /**
     * the parameters for the class fastEllipseExtraction
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
       *  The default is 3
       */
      int maxLineGap;


      /**
       *  The maximum gap two arcs may have and
       *  still be considered arcs of the same extended arc.
       *  This parameter changes the size of the searching window
       *  for arcs. Best results were obtained with large values,
       *  because every arc has to find at least two neighbours
       *  to remain in the extraction process. (the gap between
       *  these arcs mostly lies between 10 and 40 pixels)
       *  A <code>maxArcGap</code> of 1 will only group
       *  contiguous arcs to an extended arc.
       *  possible values are
       *  <pre>
       *  maxArcGap >= 1 (pixel)
       *  </pre>
       *  Higher maxArcGap values also allow extraction of
       *  non-contiguous arcs such as found in corrupted images.
       *  The default is 25
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
       *  is a value between 10 and 15 (the default is 14)
       */
      float maxLineTangentError;


      /** This parameter defines the maximum difference that
       *  the tangents of two arcs may have in relation to the
       *  tangents of the ellipse that these arcs represent.
       *  In other words, this parameter affects the tolerance
       *  in terms of ellipse matching.
       *  Arcs exceeding this threshold won't be combined
       *  to an extended arc. Too large values result incorrect
       *  extended arcs and detection failures.
       *  reasonable values are
       *  <pre>
       *  10 <= maxArcTangentError <= 20
       *  </pre>
       *  Small values cause stricter ellipse checks
       *  whereas large values allow more tolerance.
       *  Best results were obtained with more tolerant
       *  values between 15 and 20 (the default is 18)
       */
      float maxArcTangentError;


      /** This parameter defines the maximum value that the
       *  start and end points of one extArc inserted into the
       *  normal ellipse equation of the other extArc may give.
       *  ExtArcs which exceed this threshold won't
       *  be merged into an ellipse. Too large values 
       *  cause detection failures.
       *  reasonable values are
       *  <pre>
       *  0 < maxExtArcMismatch < 1
       *  </pre>
       *  Small values cause stricter ellipse matching checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 0.2 and 0.4 (the default is 0.3)
       */
      float maxExtArcMismatch;


      /** This parameter defines the maximum gap between
       *  the estimated ellipse centers of two extended arcs. 
       *  ExtArc pairs which exceed this threshold won't be 
       *  combined to an ellipse. Too large values cause 
       *  detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxCenterMismatch <= 10
       *  </pre>
       *  Small values cause stricter extArc matching checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 3 and 7 (the default is 5)
       */
      float maxCenterMismatch;


      /** This parameter defines the minimal matching ratio
       *  of the estimated ellipse radii of two extended arcs. 
       *  ExtArc pairs which underrun this threshold won't be 
       *  combined to an ellipse. Too small values cause 
       *  detection failures.
       *  reasonable values are
       *  <pre>
       *  0.5 <= minRadiusMatchRatio <= 0.95
       *  </pre>
       *  Large values cause stricter extArc matching checks
       *  whereas small values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 0.7 and 0.9 (the default is 0.8)
       */
      float minRadiusMatchRatio;


      /** This parameter defines the minimal coverage for estimated
       *  ellipses to get extracted. That is the ratio of the estimated
       *  circumference and the number of extracted arc pixels.
       *  Ellipses which underrun this threshold won't get extracted.
       *  Too small values cause extraction of ellipses with randomly
       *  matching arcs. Also this threshold shouldn't be set to values
       *  greater than 0.7, since only well fitting parts of a ellipses 
       *  are extracted. Therefore even a completely present ellipse  
       *  may get only 70%% coverage.
       *  reasonable values are
       *  <pre>
       *  0.2 <= minCoverage <= 0.7
       *  </pre>
       *  Best compromise between accuracy and tolerance
       *  is a value between 0.2 and 0.4 (the default is 0.25)
       *  Note: Very close and similar ellipses are sometimes
       *  merged to one ellipse which shows more than 100%% coverage.
       */
      float minCoverage;
      

      /** This parameter defines the minimum relative 
       *  distance of two arcs. That is the distance of
       *  the start points of both arcs compared to the 
       *  start-end point distance of the first arc.
       *  Arcs which underrun this threshold won't be 
       *  combined to extArcs. 
       *  reasonable values are
       *  <pre>
       *  0 <= minArcDistanceRatio <= 1
       *  </pre>
       *  Values \<1 allow extraction of overlapping
       *  arcs. Best results were obtained with values
       *  between 0.5 and 0.7 (the default is 0.56)
       */
      float minArcDistanceRatio;


      /** This parameter defines the maximum angle
       *  between the connection line of the start points
       *  of two arcs and the line from start point to end 
       *  point of the first arc.
       *  reasonable values are
       *  <pre>
       *  10 <= maxArcDistanceAngle <= 20
       *  </pre>
       *  Small values increase the separation accuracy of
       *  adjacent ellipse arcs but also lowers tolerance 
       *  against corrupted arcs. Best results were obtained 
       *  with a value of 16 (the default).
       */
      float maxArcDistanceAngle;


      /** This parameter is a threshold for the gap angle
       *  check. Arcs with a gap smaller than this value are
       *  considered close enough to have a continous transition.
       *  Arcs with greater gaps must pass the additional gap
       *  angle check. Too large values cause detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= minGAPangleDistance <= 3
       *  </pre>
       *  Best results were obtained with a value of 2 (the default).
       */
      int   minGAPangleDistance;


      /** This parameter defines the maximum angle between
       *  the end tangent of the first arc and the start tangent
       *  of the second arc and the connection tangent.
       *  Arc pairs which exceed this threshold won't be 
       *  combined to an extArc. Too large values cause 
       *  detection failures whereas too small values reject
       *  correct arcs.
       *  reasonable values are
       *  <pre>
       *  25 <= maxGAPangle <= 35
       *  </pre>
       *  Best results were obtained with a value of 30 (the default).
       */
      float maxGAPangle;


      /** This parameter defines the maximum radial gap two
       *  overlapping arcs. ExtArc pairs which exceed this 
       *  threshold won't be combined to an extArc. 
       *  Too large values cause detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxArcOverlapGap <= 3
       *  </pre>
       *  Best results were obtained with a value of 1 (the default).
       */
      float maxArcOverlapGap;


      /** This parameter defines the maximum number of errors
       *  in the interior angle check. The interior angles
       *  of all lines of the three arcs must be smaller than 90
       *  degrees to remain in the extArc extraction process. 
       *  Because of image corruptions one or more errors should
       *  be allowed. Too large values cause detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxIntAngleMismatches <= 3
       *  </pre>
       *  Best results were obtained with a value of 1 (the default).
       */
      int   maxIntAngleMismatches;


      /** This parameter defines the maximum number of errors
       *  in the arc tangent check. The tangents of all lines of 
       *  the three base arcs of one extArc must match the
       *  corresponding tangents of the estimated ellipse.
       *  The maximum difference between ideal and real tangent
       *  is defined by the parameter maxArcTangentError.
       *  Because of image corruptions one or more errors should
       *  be allowed. Too large values cause detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxTangentErrors <= 3
       *  </pre>
       *  Best results were obtained with a value of 1 (the default).
       */
      int   maxTangentErrors;


      /** This parameter defines the maximum distance between
       *  the line beam of the extArc and the center point of 
       *  the estimated ellipse.
       *  ExtArcs which exceed this threshold won't be 
       *  combined to an ellipse. Too large values cause 
       *  detection failures.
       *  reasonable values are
       *  <pre>
       *  1 <= maxLBcenterMismatch <= 10
       *  </pre>
       *  Small values cause stricter extArc matching checks
       *  whereas large values allow more tolerance.
       *  Best compromise between accuracy and tolerance
       *  is a value between 3 and 6 (the default is 4)
       */
      float maxLBcenterMismatch;
      

      /** The number of checks that an extArc must pass 
       *  before it gets extracted. This parameter was mainly
       *  used for debugging purposes but might further on be
       *  useful for the finding of optimal extraction parameters.
       *  <pre>
       *  1 = interior angle check
       *  2 = tangent check
       *  3 = line beam check
       *  </pre>
       *  With a value of 1 or 2 you can examine the behaviour 
       *  of the extArc checks. Unless there is a good reason 
       *  for less extArc checks this this value should be 3.
       */
      int   ellExtArcExtractionStage;
    };

    /**
     * default constructor
     */
    fastEllipseExtraction();

    /**
     * Construct a functor using the given parameters
     */
    fastEllipseExtraction(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fastEllipseExtraction(const fastEllipseExtraction& other);

    /**
     * destructor
     */
    virtual ~fastEllipseExtraction();

    /**
     * returns the name of this type ("fastEllipseExtraction")
     */
    virtual const char* getTypeName() const;


    /**
     * Applies fast ellipse detection on-place.
     * @param srcdest channel8 with the source data.
     * @return true if apply was successful or false otherwise.
     */
    bool apply(const channel8& srcdest);

    /**
     * Applies fast ellipse detection on-place.
     * @param srcdest channel with the source data.
     * @return true if apply was successful or false otherwise.
     */
    bool apply(const channel& srcdest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastEllipseExtraction& copy(const fastEllipseExtraction& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastEllipseExtraction& operator=(const fastEllipseExtraction& other);

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
     * @param group_number number of the arc group (1,2,3,4,5,6,7,8 or 0 for all arcs)
     * @return vector with extracted arcs
     */
    virtual std::vector<ellArcEntry>& getEllArcList(const int group_number);

    /**
     * returns a vector with extended arcs of the specified group
     * @param group_number number of the arc group (1,2,3,4,5,6,7,8 or 0 for all arcs)
     * @return vector with extracted extended arcs
     */
    virtual std::vector<ellExtArcEntry>& getEllExtArcList(const int group_number);

    /**
     * returns a vector with extracted ellipses.
     * @return vector with extracted ellipses
     */
    virtual std::vector<ellipseEntry>& getEllipseList();


private:

    /**
     * clears a line list and its allocated objects
     * @param group_number number of the line group (0,1,2,3,4)
     */
    virtual void clearLineList(const int group_number);

    /**
     * clears an arc list and its allocated objects
     * @param group_number number of the arc group (0,1,2,3,4,5,6,7,8)
     */
    virtual void clearEllArcList(const int group_number);

    /**
     * clears an extended arc list and its allocated objects
     * @param group_number number of the arc group (0,1,2,3,4,5,6,7,8)
     */
    virtual void clearEllExtArcList(const int group_number);

    /**
     * clears the ellipse list and its allocated objects
     */
    virtual void clearEllipseList();

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
     * Results are stored in STL vectors of type <ellArcEntry>. These vectors can
     * be accessed by the method getEllArcList().
     */
    virtual void extractGroup1and5Arcs();
    virtual void extractGroup3and7Arcs();
    virtual void extractGroup2and6Arcs();
    virtual void extractGroup4and8Arcs();

    /**
     * The extended arc extraction method. It extracts extended arcs by tracking arcs.
     * Results are stored in STL vectors of type <ellExtArcEntry>. These vectors can
     * be accessed by the method getEllExtArcList().
     * @param grp the extended arc group to extract
     */
    void extractEllExtArcs(const int grp);


    /**
     * The ellipse extraction method.
     * Ellipses are extracted by merging extended arcs.
     */
    void extractEllipses();


    /**
     * Calculates and checks the interior angles of two arcs.
     * @param a index of the first arc
     * @param grpA group of the first arc
     * @param b index of the second arc
     * @param grpB group of the second arc
     * @return the number of angle mismatches
     */
    int checkInteriorAngles(int a, int grpA, int b, int grpB);


    /**
     * Calculates and checks the tangent matching of an extended arc and an ellipse.
     * @param extarc the arc to be checked
     * @param Xest x coordinate of the ellipse
     * @param Yest y coordinate of the ellipse
     * @param Aest large semi-axis of the ellipse
     * @param Best small semi-axis of the ellipse
     * @param Test orientation angle of the ellipse
     * @return the number of tangent mismatches
     */
    int checkEllExtArcTangents(const ellExtArcEntry &extarc,
                               double Xest, double Yest, double Aest, double Best, double Test);


    /**
     * Calculates the distance from the linebeam of an extended arc to an ellipse center point.
     * The intersection point of the start and end tangents and the line beam slope are stored
     * in the given extArc entry.
     * @param firstLine first line of the arc to be checked
     * @param lastLine last line of the arc to be checked
     * @param extarc the arc to be checked
     * @param Xest x coordinate of the ellipse
     * @param Yest y coordinate of the ellipse
     * @return true if successful
     */
    bool lineBeamCenterDistance(const lineEntry &firstLine,
                                const lineEntry &lastLine,
                                ellExtArcEntry &extarc,
                                double &distance, const double Xest, const double Yest);


    /**
     * Calculates the start and end point distance of an extended arc from an estimated ellipse.
     * @param extarc the arc to be checked
     * @param Xest x coordinate of the ellipse
     * @param Yest y coordinate of the ellipse
     * @param Aest large semi-axis of the ellipse
     * @param Best small semi-axis of the ellipse
     * @param Test orientation angle of the ellipse
     * @return the maximum distance
     */
    double ellipsePointMismatch(ellExtArcEntry &extarc,
                              double Xest, double Yest, double Aest, double Best, double Test);


    /**
     * Calculates the intersection point of the line beams of two extended arcs and its
     * distance from an ellipse center point.
     * @param extarcA first arc
     * @param extarcB second arc
     * @param Xest x coordinate of the ellipse
     * @param Yest y coordinate of the ellipse
     * @return the distance of intersection and center point
     */
    double lineBeamIsecCenterDistance(const ellExtArcEntry &extarcA,
                                      const ellExtArcEntry &extarcB,
                                      double Xest, double Yest);
  

    /**
     * Calculates the distance between a point and a line.
     * @param testPtX x coordinate of the point
     * @param testPtY y coordinate of the point
     * @param linePtY x coordinate of a line point
     * @param testPtY y coordinate of a line point
     * @param lineSlope slope of the line
     * @return the distance between the point and the line.
     */
    double linePointDistance(double testPtX, double testPtY, 
                             double linePtX, double linePtY,
                             double lineSlope);
  


    /**
     * Calculates the coverage of an estimated ellipse by approximating the ideal  
     * circumference and counting the edge pixels of the extracted extended arcs.
     * The coverage value is stored in the given ellipse entry.
     * @param ellipse the ellipse
     */
    void calculateEllipseCoverage(ellipseEntry &ellipse);
  
  
  
    /**
     * Estimates ellipse parameters by using a least square fitting algorithm.
     * @param arcA first arc of the ellipse
     * @param arcB second arc of the ellipse
     * @param arcC third arc of the ellipse
     * @param Xest estimated x coordinate of the ellipse
     * @param Yest estimated y coordinate of the ellipse
     * @param Aest estimated large semi-axis of the ellipse
     * @param Best estimated small semi-axis of the ellipse
     * @param Test estimated orientation angle of the ellipse
     * @return true if estimation was successful
     */
    bool estimateEllipse(const ellArcEntry &arcA, 
                         const ellArcEntry &arcB, 
                         const ellArcEntry &arcC, 
                         double &Xest, double &Yest, double &Aest, double &Best, double &Test);
                         

    /**
     * Estimates ellipse parameters by using a least square fitting algorithm.
     * @param mergedArcs pointer to vector with extArc index numbers
     * @param Xest estimated x coordinate of the ellipse
     * @param Yest estimated y coordinate of the ellipse
     * @param Aest estimated large semi-axis of the ellipse
     * @param Best estimated small semi-axis of the ellipse
     * @param Test estimated orientation angle of the ellipse
     * @return true if estimation was successful
     */
    bool estimateEllipse(const std::vector<int> *mergedArcs, 
                         double &Xest, double &Yest, double &Aest, double &Best, double &Test);
  

    /**
     * A least square fitting algorithm for ellipse estimation.
     * @param Design matrix with ellipse edge pixel data
     * @param Xest estimated x coordinate of the ellipse
     * @param Yest estimated y coordinate of the ellipse
     * @param Aest estimated large semi-axis of the ellipse
     * @param Best estimated small semi-axis of the ellipse
     * @param Test estimated orientation angle of the ellipse
     * @return true if estimation was successful
     */
    bool directEllipseFitting(const lti::matrix<double> &Design, 
                              double &Xest, double &Yest, double &Aest, double &Best, double &Test);
  

    /**
     * A least square fitting algorithm for ellipse estimation. 
     * Results are coefficients of the equation for conic sections.
     * @param Design matrix with ellipse edge pixel data
     * @param A first conic section coefficient
     * @param B second conic section coefficient
     * @param C third conic section coefficient
     * @param D fourth conic section coefficient
     * @param E fifth conic section coefficient
     * @param F sixth conic section coefficient
     * @return true if estimation was successful
     */
    bool directEllipseCoefficientFitting(const lti::matrix<double> &Design, 
                                         double &A, double &B, double &C, double &D, double &E, double &F);
  

    /**
     * Estimates circle parameters by using a least square fitting algorithm.
     * @param idxlist vector with arc line index numbers
     * @param group group number of the arc lines
     * @param Xest the estimated x-coordinate
     * @param Yest the estimated y-coordinate
     * @param R2est the estimated squared radius
     * @return true if calculation was successful
     */
    virtual bool estimateCircle(std::vector<int> &idxlist, const int group,
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
     * The elliptic arc containers (8-goup classification).
     * m_vecExtArcs[1..9] contain arc groups 1..8,
     * m_vecExtArcs[0] contains all arcs. 
     */
    std::vector<ellArcEntry> m_vecEllArcs[9];

    /**
     * The elliptic extended arc containers (8-goup classification).
     * m_vecExtArcs[1..9] contain extArc groups 1..8,
     * m_vecExtArcs[0] contains all extended arcs. 
     */
    std::vector<ellExtArcEntry> m_vecEllExtArcs[9];

    /**
     * The ellipse container.
     */
    std::vector<ellipseEntry> m_vecEllipses;

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
    static const int EllArcPreallocation;
    static const int EllExtArcPreallocation;
    static const int EllipsePreallocation;
    
    /**
     * Numeric constants.
     */
    static const double PI;
    static const double INFINITE_SLOPE;

  };
}

#endif

