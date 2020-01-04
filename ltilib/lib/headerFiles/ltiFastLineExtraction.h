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
 * file .......: ltiFastLineExtraction.h
 * authors ....: Ingo Grothues (ingo@isdevelop.de)
 * organization: LTI, RWTH Aachen
 * creation ...: 10.3.2004
 * revisions ..: $Id: ltiFastLineExtraction.h,v 1.13 2005/08/01 11:23:32 libuda Exp $
 */

#ifndef _LTI_FAST_LINE_EXTRACTION_H_
#define _LTI_FAST_LINE_EXTRACTION_H_

#include "ltiFeatureExtractor.h"
#include "ltiPoint.h"
#include <vector>

namespace lti {
  
  /**
   * This functor implements a fast line extraction
   * using the line segments found in digital images.
   * 
   * To achieve this, the (edge-)image is decomposed into
   * line segments of four types (I=horizontal, II=vertical, 
   * III=diagonal horizontal, IV=diagonal vertical)
   * which are recomposed to lines in a second pass.
   * 
   * The fastLineExtraction::parameters specify ranges
   * for segments/lines and conditions that a set of 
   * segments must fulfill to be considered as a line.
   * The result strongly depends on these parameters,
   * especially the number and the precision of the 
   * extracted lines.
   *
   * The extracted segments and lines are stored in STL vector
   * containers of the type \<segmEntry\> and can be accessed by
   * the methods getSegmentList() and getLineList().
   *
   * \c ipoint is the data type for coordinates:
   * <pre> typedef tpoint<int> ipoint; </pre>
   *   
   * \c segmEntry is the data struct for segments/lines:
   * <pre> struct segmEntry
   *  {
   *    ipoint  start;    // start point of segment or line
   *    ipoint  end;      // end point of segment or line
   *    int     len;      // length of segment or line
   *    int     used;     // counts segment<->line assignments
   *  };</pre>
   * 
   * Additionally there are two methods which return some
   * statistical information about the line extraction process:
   * getNonGroupedSegmentCount() and getMultiGroupedSegmentCount()
   *
   * The fast line extraction does not perform any changes
   * to the analyzed image. To visualize the extracted lines
   * they have to be drawn by other code using the
   * segment lists and a drawing functor.
   *
   *
   * Example:
   * \code
   *    
   * #include "ltiViewer.h"               // visualization tool
   * #include "ltiDraw.h"                 // drawing tool
   * #include "ltiALLfunctor.h"           // image loader   
   * #include "ltiCannyEdges.h"           // edge detector
   * #include "ltiFastLineExtraction.h"   // line detector
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
   * // create FLE functor (using default parameters)
   * lti::fastLineExtraction fle;
   * 
   * // extract the lines
   * fle.apply(m_edges);
   * 
   * // get the complete line list
   * std::vector<lti::fastLineExtraction::segmEntry> &lines = fle.getLineList(0);
   * 
   * // create a drawing functor
   * lti::draw<lti::channel8::value_type> drawer;
   * drawer.use(result);
   * drawer.setColor(255);
   * 
   * for(int i=0; i<lines.size(); i++) {
   *   // draw all lines
   *   drawer.line(lines[i].start, lines[i].end);
   *   // print line data
	 *   printf("line[%i]: start=(%i,%i) end=(%i,%i) length=%i \n", i, 
	 *           lines[i].start.x, lines[i].start.y, lines[i].end.x, lines[i].end.y, lines[i].len);
   * }
   * 
   * // show the extracted lines
   * lti::viewer view("Detected lines");
   * view.show(result);
   * 
   * // wait for a mouse click in the viewer window
   * view.waitButtonPressed();
   * 
   * \endcode
   */

  class fastLineExtraction : public featureExtractor {
  public:

    struct segmEntry
    {
      ipoint  start;    // start point of segment or line
      ipoint  end;      // end point of segment or line
      int     len;      // length of segment or line
      int     used;     // counts segment<->line assignments
    };
  

    /**
     * the parameters for the class fastLineExtraction
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

#ifdef _LTI_MSC_6
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
#endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------



      /** This threshold constant defines the maximum mismatch
       *  between the found line segments and the estimated analog line.
       *  The segment is discarded if its starting/finishing pixel 
       *  differs more than \c maxQuantizationError
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
       *  is a value between 0.6 and 0.7 (the default is 1.0)
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
       *	This parameter changes the size of the searching window
       *	for line segments. Small sizes allow fast processing but may
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
       *  The parameter \c minLineLen is used as 
       *  threshold, smaller lines will be ignored.
       *  possible values are
       *  <pre>
       *  minLineLen >= 3 (pixel)
       *  </pre>
       *  Values greater than 3 will strongly decrease
       *  the number of extracted lines.
       *  The default is 3
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
    };

////////////////////////////////////////////////////////////////////////////

    /**
     * default constructor
     */
    fastLineExtraction();

    /**
     * Construct a functor using the given parameters
     */
    fastLineExtraction(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    fastLineExtraction(const fastLineExtraction& other);

    /**
     * destructor
     */
    virtual ~fastLineExtraction();

    /**
     * returns the name of this type ("fastLineExtraction")
     */
    virtual const char* getTypeName() const;

    
    /**
     * Applies fast line detection on-place.
     * @param srcdest channel8 with the source data.
     * @return true if apply was successful or false otherwise.
     */
    bool apply(const channel8& srcdest);

    /**
     * Applies fast line detection on-place.
     * @param srcdest channel with the source data.
     * @return true if apply was successful or false otherwise.
     */
    bool apply(const channel& srcdest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastLineExtraction& copy(const fastLineExtraction& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastLineExtraction& operator=(const fastLineExtraction& other);

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
    virtual std::vector<segmEntry>& getLineList(const int group_number);

    /**
     * returns number of segments that could not be grouped to a line
     * @param group_number number of the line group (1,2,3,4)
     * @return number of non-grouped line segments
     */
    virtual const int getNonGroupedSegmentCount(const int group_number) const;

    /**
     * returns number of segments that are used in several lines
     * @param group_number number of the line group (1,2,3,4)
     * @return number of multi-grouped line segments
     */
    virtual const int getMultiGroupedSegmentCount(const int group_number) const;


private:

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
     * Results are stored in STL vectors of type <segmEntry>. These vectors can
     * be accessed by the method getLineList().
     */
    virtual void extractGroup1Lines();
    virtual void extractGroup2Lines();
    virtual void extractGroup3Lines();
    virtual void extractGroup4Lines();
    
    /**
     * The line containers. 
     * m_vecLines[1..4] contain line groups I..IV,
     * m_vecLines[0] contains all lines.
     */
    std::vector<segmEntry> m_vecLines[5];

    /**
     * The segment containers. (to allow easier implementation
     * the arraysize is 5 although only 4 conatainers are used)
     * m_vecSegments[1..4] contain segment groups I..IV,
     * m_vecSegments[0] is not used. 
     */
    std::vector<segmEntry> m_vecSegments[5];
    
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
  };
}

#endif

