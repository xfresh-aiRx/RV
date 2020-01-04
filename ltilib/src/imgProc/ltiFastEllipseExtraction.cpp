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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiFastEllipseExtraction.cpp
 * authors ....: Ingo Grothues (ingo@isdevelop.de)
 * organization: LTI, RWTH Aachen
 * creation ...: 23.4.2004
 * revisions ..: $Id: ltiFastEllipseExtraction.cpp,v 1.11 2005/10/28 15:56:41 doerfler Exp $
 */

#include "ltiFastEllipseExtraction.h"

#include "ltiCholeskyDecomposition.h"
#include "ltiMatrixInversion.h"
#include "ltiEigenSystem.h"

// number of pre-allocated vector elements
const int lti::fastEllipseExtraction::SegmPreallocation = 5000;
const int lti::fastEllipseExtraction::LinePreallocation = 1000;
const int lti::fastEllipseExtraction::EllArcPreallocation = 50;
const int lti::fastEllipseExtraction::EllExtArcPreallocation = 20;
const int lti::fastEllipseExtraction::EllipsePreallocation = 10;
const double lti::fastEllipseExtraction::PI = 3.14159265358979323846264338327;
const double lti::fastEllipseExtraction::INFINITE_SLOPE = 999999999.0;

namespace lti {
  // --------------------------------------------------
  // fastEllipseExtraction::parameters
  // --------------------------------------------------

  // default constructor
  fastEllipseExtraction::parameters::parameters()
    : featureExtractor::parameters() {

    minSegmLen = 2;
    minLineLen = 6;
    segmentTolerance = 0;
    maxSegmentGap = 1;
    maxQuantizationError = 0.74f;
    maxLineGap = 3;
    maxArcGap = 25;
    maxLineTangentError = 14.0f;
    maxArcTangentError = 18.0f;
    
    maxExtArcMismatch = 0.3f;
    maxCenterMismatch = 5.0f;
    minRadiusMatchRatio = 0.8f;
    minCoverage = 0.25f;
    
    maxArcDistanceAngle = 16.0f;
    minArcDistanceRatio = 0.56f;
    minGAPangleDistance = 2;
    maxGAPangle = 30.0f;
    maxArcOverlapGap = 1.0f;

    maxIntAngleMismatches = 1;
    maxTangentErrors = 1;
    maxLBcenterMismatch = 4.0f;
    
    ellExtArcExtractionStage = 3;
  }

  // copy constructor
  fastEllipseExtraction::parameters::parameters(const parameters& other)
    : featureExtractor::parameters() {
    copy(other);
  }

  // destructor
  fastEllipseExtraction::parameters::~parameters() {
  }

  // get type name
  const char* fastEllipseExtraction::parameters::getTypeName() const {
    return "fastEllipseExtraction::parameters";
  }

  // copy member

  fastEllipseExtraction::parameters&
    fastEllipseExtraction::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    featureExtractor::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    featureExtractor::parameters& (featureExtractor::parameters::* p_copy)
      (const featureExtractor::parameters&) =
      featureExtractor::parameters::copy;
    (this->*p_copy)(other);
# endif

    minSegmLen = other.minSegmLen;
    minLineLen = other.minLineLen;
    segmentTolerance = other.segmentTolerance;
    maxSegmentGap = other.maxSegmentGap;
    maxQuantizationError = other.maxQuantizationError;

    maxLineGap = other.maxLineGap;
    maxArcGap = other.maxArcGap;
    maxLineTangentError = other.maxLineTangentError;
    maxArcTangentError = other.maxArcTangentError;

    maxExtArcMismatch = other.maxExtArcMismatch;
    maxCenterMismatch = other.maxCenterMismatch;
    minRadiusMatchRatio = other.minRadiusMatchRatio;
    minCoverage = other.minCoverage;

    maxIntAngleMismatches = other.maxIntAngleMismatches;
    minGAPangleDistance = other.minGAPangleDistance;
    maxGAPangle = other.maxGAPangle;
    maxTangentErrors = other.maxTangentErrors;
    maxLBcenterMismatch = other.maxLBcenterMismatch;
    maxArcOverlapGap = other.maxArcOverlapGap;
    
    maxArcDistanceAngle = other.maxArcDistanceAngle;
    minArcDistanceRatio = other.minArcDistanceRatio;
    
    ellExtArcExtractionStage = other.ellExtArcExtractionStage;

    return *this;
  }

  // alias for copy member
  fastEllipseExtraction::parameters&
    fastEllipseExtraction::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  functor::parameters* fastEllipseExtraction::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool fastEllipseExtraction::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool fastEllipseExtraction::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"minSegmLen",minSegmLen);
      lti::write(handler,"minLineLen",minLineLen);
      lti::write(handler,"segmentTolerance",segmentTolerance);
      lti::write(handler,"maxSegmentGap",maxSegmentGap);
      lti::write(handler,"maxQuantizationError",maxQuantizationError);
      
      lti::write(handler,"maxLineGap",maxLineGap);
      lti::write(handler,"maxArcGap",maxArcGap);
      lti::write(handler,"maxLineTangentError",maxLineTangentError);
      lti::write(handler,"maxArcTangentError",maxArcTangentError);

      lti::write(handler,"maxExtArcMismatch",maxExtArcMismatch);
      lti::write(handler,"maxCenterMismatch",maxCenterMismatch);
      lti::write(handler,"minRadiusMatchRatio",minRadiusMatchRatio);
      lti::write(handler,"minCoverage",minCoverage);
            
      lti::write(handler, "maxIntAngleMismatches", maxIntAngleMismatches);
      lti::write(handler, "minGAPangleDistance", minGAPangleDistance);
      lti::write(handler, "maxGAPangle", maxGAPangle);
      lti::write(handler, "maxTangentErrors", maxTangentErrors);
      lti::write(handler, "maxLBcenterMismatch", maxLBcenterMismatch);
      lti::write(handler, "maxArcOverlapGap", maxArcOverlapGap);
      
      lti::write(handler, "maxArcDistanceAngle", maxArcDistanceAngle);
      lti::write(handler, "minArcDistanceRatio", minArcDistanceRatio);
      
      lti::write(handler, "ellExtArcExtractionStage", ellExtArcExtractionStage);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && featureExtractor::parameters::write(handler,false);
# else
    bool (featureExtractor::parameters::* p_writeMS)(ioHandler&,const bool) const =
      featureExtractor::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool fastEllipseExtraction::parameters::write(ioHandler& handler,
                                         const bool complete) const {
    // ...we need this workaround to cope with another really awful MSVC bug.
    return writeMS(handler,complete);
  }
# endif

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool fastEllipseExtraction::parameters::read(ioHandler& handler,
                                        const bool complete)
# else
  bool fastEllipseExtraction::parameters::readMS(ioHandler& handler,
                                          const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"minSegmLen",minSegmLen);
      lti::read(handler,"minLineLen",minLineLen);
      lti::read(handler,"segmentTolerance",segmentTolerance);
      lti::read(handler,"maxSegmentGap",maxSegmentGap);
      lti::read(handler,"maxQuantizationError",maxQuantizationError);
      
      lti::read(handler,"maxLineGap",maxLineGap);
      lti::read(handler,"maxArcGap",maxArcGap);
      lti::read(handler,"maxLineTangentError",maxLineTangentError);
      lti::read(handler,"maxArcTangentError",maxArcTangentError);

      lti::read(handler,"maxExtArcMismatch",maxExtArcMismatch);
      lti::read(handler,"maxCenterMismatch",maxCenterMismatch);
      lti::read(handler,"minRadiusMatchRatio",minRadiusMatchRatio);
      lti::read(handler,"minCoverage",minCoverage);
      
      lti::read(handler, "maxIntAngleMismatches", maxIntAngleMismatches);
      lti::read(handler, "minGAPangleDistance", minGAPangleDistance);
      lti::read(handler, "maxGAPangle", maxGAPangle);
      lti::read(handler, "maxTangentErrors", maxTangentErrors);
      lti::read(handler, "maxLBcenterMismatch", maxLBcenterMismatch);
      lti::read(handler, "maxArcOverlapGap", maxArcOverlapGap);
      
      lti::read(handler, "maxArcDistanceAngle", maxArcDistanceAngle);
      lti::read(handler, "minArcDistanceRatio", minArcDistanceRatio);

      lti::read(handler, "ellExtArcExtractionStage", ellExtArcExtractionStage);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && featureExtractor::parameters::read(handler,false);
# else
    bool (featureExtractor::parameters::* p_readMS)(ioHandler&,const bool) =
      featureExtractor::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool fastEllipseExtraction::parameters::read(ioHandler& handler,
                                        const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // --------------------------------------------------
  // fastEllipseExtraction
  // --------------------------------------------------

  // default constructor
  fastEllipseExtraction::fastEllipseExtraction()
    : featureExtractor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
    // default initialization
    m_iWidth = 0;
  }

  // default constructor
  fastEllipseExtraction::fastEllipseExtraction(const parameters& par)
    : featureExtractor() {

    // set the given parameters
    setParameters(par);
    // default initialization
    m_iWidth = 0;
  }


  // copy constructor
  fastEllipseExtraction::fastEllipseExtraction(const fastEllipseExtraction& other)
    : featureExtractor() {
    copy(other);
  }

  // destructor
  fastEllipseExtraction::~fastEllipseExtraction() {
    for(int i=0; i<5; i++) {
      m_vecSegments[i].clear();
      clearLineList(i);
      clearEllArcList(i);
      clearEllExtArcList(i);
    }
    clearEllipseList();   
  }

  // returns the name of this type
  const char* fastEllipseExtraction::getTypeName() const {
    return "fastEllipseExtraction";
  }

  // copy member
  fastEllipseExtraction&
    fastEllipseExtraction::copy(const fastEllipseExtraction& other) {

    featureExtractor::copy(other);
    int i;
    for(i=0; i<5; i++) {
      m_vecLines[i] = other.m_vecLines[i];
      m_vecSegments[i] = other.m_vecSegments[i];
      // copy segmIdxLists
      const int nLines = static_cast<int>(m_vecLines[i].size());
      int j;
      for(j=0; j<nLines; j++) {
        m_vecLines[i][j].segmIdxList = new std::vector<int>(*other.m_vecLines[i][j].segmIdxList);
      }
    }

    for(i=0; i<9; i++) {
      m_vecEllArcs[i] = other.m_vecEllArcs[i];
      m_vecEllExtArcs[i] = other.m_vecEllExtArcs[i];
      // copy lineIdxLists
      const int nArcs = static_cast<int>(m_vecEllArcs[i].size());
      int j;
      for(j=0; j<nArcs; j++) {
        m_vecEllArcs[i][j].lineIdxList = new std::vector<int>(*other.m_vecEllArcs[i][j].lineIdxList);
      }
    }

    m_vecEllipses = other.m_vecEllipses;
    // copy arcIdxLists
    const int nEllipses = static_cast<int>(m_vecEllipses.size());
    for(i=0; i<nEllipses; i++) {
      m_vecEllipses[i].mergedArcs = new std::vector<int>(*other.m_vecEllipses[i].mergedArcs);
    }

    m_iWidth = other.m_iWidth;
    return (*this);
  }

  // alias for copy member
  fastEllipseExtraction&
    fastEllipseExtraction::operator=(const fastEllipseExtraction& other) {
    return (copy(other));
  }


  // clone member
  functor* fastEllipseExtraction::clone() const {
    return new fastEllipseExtraction(*this);
  }

  // return parameters
  const fastEllipseExtraction::parameters&
    fastEllipseExtraction::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }


////////////////////////////////////////////////////////////


  // returns vector with line segments
  std::vector<fastEllipseExtraction::segmEntry>& fastEllipseExtraction::getSegmentList(const int group_number) {
    if((group_number < 0)||(group_number > 4)) {
      return m_vecSegments[0];
    }
    return m_vecSegments[group_number];
  }

  // returns vector with lines
  std::vector<fastEllipseExtraction::lineEntry>& fastEllipseExtraction::getLineList(const int group_number) {
    if((group_number < 0)||(group_number > 4)) {
      return m_vecLines[0];
    }
    return m_vecLines[group_number];
  }

  // returns vector with arcs
  std::vector<fastEllipseExtraction::ellArcEntry>& fastEllipseExtraction::getEllArcList(const int group_number) {
    if((group_number < 0)||(group_number > 8)) {
      return m_vecEllArcs[0];
    }
    return m_vecEllArcs[group_number];
  }  

  // returns vector with extended arcs
  std::vector<fastEllipseExtraction::ellExtArcEntry>& fastEllipseExtraction::getEllExtArcList(const int group_number) {
    if((group_number < 0)||(group_number > 8)) {
      return m_vecEllExtArcs[0];
    }
    return m_vecEllExtArcs[group_number];
  }  

  // returns vector with ellipses
  std::vector<fastEllipseExtraction::ellipseEntry>& fastEllipseExtraction::getEllipseList() {
    return m_vecEllipses;
  }  

  // clears line lists
  void fastEllipseExtraction::clearLineList(const int group_number) {
      if(group_number > 0) {  // don't delete shared list[0] idx_vectors
        const int n = static_cast<int>(m_vecLines[group_number].size());
        for(int j=0; j<n; j++) {
          (*m_vecLines[group_number][j].segmIdxList).clear();
          delete m_vecLines[group_number][j].segmIdxList;
        }
      }
      m_vecLines[group_number].clear(); 
  }

  // clears arc lists
  void fastEllipseExtraction::clearEllArcList(const int group_number) {
      if(group_number > 0) {  // don't delete shared list[0] idx_vectors
        const int n = static_cast<int>(m_vecEllArcs[group_number].size());
        for(int j=0; j<n; j++) {
          (*m_vecEllArcs[group_number][j].lineIdxList).clear();
          delete m_vecEllArcs[group_number][j].lineIdxList;
        }
      }
      m_vecEllArcs[group_number].clear(); 
  }

  // clears extended arc lists
  void fastEllipseExtraction::clearEllExtArcList(const int group_number) {
      m_vecEllExtArcs[group_number].clear(); 
  }

  // clears the ellipse list
  void fastEllipseExtraction::clearEllipseList() {
      const int n = static_cast<int>(m_vecEllipses.size());
      for(int j=0; j<n; j++) {
        (*m_vecEllipses[j].mergedArcs).clear();
        delete m_vecEllipses[j].mergedArcs;
      }
      m_vecEllipses.clear(); 
  }

///////////////////////////////////////////////////////////////////////////////

  // creates segment lists
  void fastEllipseExtraction::detectGroup1Segments(const channel8& src) {
    
    // get parameters
    const parameters& par = getParameters();

    // get size of the input image
    const int rows = src.rows();
    const int cols = src.columns();
    m_iWidth = cols;
    
    // calculate max ranges (horizontal lines)
    const int lineCount = rows;
    const int lineLength = cols;

    // clear old list and reserve memory
    m_vecSegments[1].clear(); 
    m_vecSegments[1].reserve(SegmPreallocation);

    // create new list  
    for(int a=0; a<lineCount; a++) { // line scan

      const vector<ubyte> line = src.getRow(a);   // copy actual line (horizontal)

      int   prevVal = 0;   // value of the previous pixel
      int   start = 0;     // start of the actual segment
      int   len = 0;       // length of the actual segment
      int   startX = 0;    // starting x-coordinate
      int   startY = 0;    // starting y-coordinate

      for(int b=0; b<lineLength+1; b++) { // pixel scan

        ubyte val; 
        if(b < lineLength) val = line[b];  // actual pixel
        else val = 0;  // extra loop for lines ending at the border

        // calculate coordinates
        const int x = b;
        const int y = a;
        
        if(val > 0) {

          // segment detection
          if(len == 0) {  // start of segment
            start = b;
            startX = x;
            startY = y;
            len++;
          }
          else {          // part or end of segment

            if((val >= (prevVal - par.segmentTolerance))    // check value of this pixel
             &&(val <= (prevVal + par.segmentTolerance))) { // if in tolerance range -> segment part
              len++;
            }
            else {       // otherwise -> segment end

              if(len >= par.minSegmLen) {
                // add segment to group list
                segmEntry newSegment;
                newSegment.len = len;
                newSegment.used = 0;
                newSegment.start.x = start;
                newSegment.start.y = y;
                newSegment.end.x = b-1;
                newSegment.end.y = y;
                m_vecSegments[1].push_back(newSegment);
              }
              len = 0;
            }
          }
        }
        else {  // val == 0 

          if(len >= par.minSegmLen) {
            // add segment to group list
            segmEntry newSegment;
            newSegment.len = len;
            newSegment.used = 0;            
            newSegment.start.x = start;
            newSegment.start.y = y;
            newSegment.end.x = b-1;
            newSegment.end.y = y;
            m_vecSegments[1].push_back(newSegment); 
          }
          len = 0;
        }
        
        prevVal = val;
      } // pixel scan
    } // line scan
  }

///////////////////////////////////////////////////////////////////////////////

  // creates segment lists
  void fastEllipseExtraction::detectGroup2Segments(const channel8& src) {

    // get parameters
    const parameters& par = getParameters();

    // get size of the input image
    const int rows = src.rows();
    const int cols = src.columns();
    m_iWidth = cols;
    
    // calculate max ranges (vertical)
    const int lineCount = cols;
    const int lineLength = rows;

    // clear old list and reserve memory
    m_vecSegments[2].clear(); 
    m_vecSegments[2].reserve(SegmPreallocation);

    // create new list  
    for(int a=0; a<lineCount; a++) { // line scan
    
      const vector<ubyte> line = src.getColumnCopy(a);  // copy actual line (vertical)

      int   prevVal = 0;   // value of the previous pixel
      int   start = 0;     // start of the actual segment
      int   len = 0;       // length of the actual segment
      int   startX = 0;    // starting x-coordinate
      int   startY = 0;    // starting y-coordinate

      for(int b=0; b<lineLength+1; b++) { // pixel scan
      
        ubyte val; 
        if(b < lineLength) val = line[b];  // actual pixel
        else val = 0;  // extra loop for lines ending at the border

        // calculate coordinates
        const int x = a;
        const int y = b;
        
        if(val > 0) {

          // segment detection
          if(len == 0) {  // start of segment
          
            start = b;
            startX = x;
            startY = y;
            len++;
          }
          else {          // part or end of segment
          
            if((val >= (prevVal - par.segmentTolerance))    // check value of this pixel
             &&(val <= (prevVal + par.segmentTolerance))) { // if in tolerance range -> segment part
              len++;
            }
            else        // otherwise -> segment end
            {
              if(len >= par.minSegmLen) {
                // add segment to group list
                segmEntry newSegment;
                newSegment.len = len;
                newSegment.used = 0;
                newSegment.start.x = x;
                newSegment.start.y = start;
                newSegment.end.x = x;
                newSegment.end.y = b-1;
                m_vecSegments[2].push_back(newSegment); 
              }
              len = 0;
            }
          }
        }
        else {  // val == 0 
        
          if(len >= par.minSegmLen) {
            // add segment to group list
            segmEntry newSegment;
            newSegment.len = len;
            newSegment.used = 0;
            newSegment.start.x = x;
            newSegment.start.y = start;
            newSegment.end.x = x;
            newSegment.end.y = b-1;
            m_vecSegments[2].push_back(newSegment); 
          }
          len = 0;
        }
        
        prevVal = val;
      } // pixel scan
    } // line scan
  }

///////////////////////////////////////////////////////////////////////////////

  // creates segment lists
  void fastEllipseExtraction::detectGroup3Segments(const channel8& src) {
  
    // get parameters
    const parameters& par = getParameters();

    // get size of the input image
    const int rows = src.rows();
    const int cols = src.columns();
    m_iWidth = cols;
    
    // maximum values
    const int maxDiag   = (rows < cols) ? rows : cols;
    
    // calculate max ranges (diagonal)
    const int lineCount = rows+cols-1;  // scan from upper right over upper left to lower left corner
    int lineLength = 0;                 // line length depends on actual line scan position

    // clear old list and reserve memory
    m_vecSegments[3].clear(); 
    m_vecSegments[3].reserve(SegmPreallocation);

    // create new list  
    for(int a=0; a<lineCount; a++) { // line scan
    
      // copy actual line (diagonal horizontal)
      vector<ubyte> line;
      
      if(a < cols) { // scan from upper_right to upper_left corner
      
        if(lineLength < maxDiag) lineLength++;     // set line length
        line.resize(lineLength, 0, false, false);  // resize vector

        for(int i=0; i<lineLength; i++) {          // fill vector
          line[i] = src.at(i, cols-1-a+i);         // copy diagonal right down
        }
      }
      else {  // scan from upper_left to lower_left corner
      
        // check if line length starts decreasing
        if((lineCount - a) < maxDiag) lineLength--; // set line length
        line.resize(lineLength, 0, false, false);   // resize vector

        for(int i=0; i<lineLength; i++) {           // fill vector
          line[i] = src.at(a-(cols-1)+i, i);        // copy diagonal right down
        }
      }

      int   prevVal = 0;   // value of the previous pixel
      int   start = 0;     // start of the actual segment
      int   len = 0;       // length of the actual segment
      int   startX = 0;    // starting x-coordinate
      int   startY = 0;    // starting y-coordinate

      for(int b=0; b<lineLength+1; b++) { // pixel scan
      
        ubyte val; 
        if(b < lineLength) val = line[b];  // actual pixel
        else val = 0;  // extra loop for lines ending at the border

        // calculate coordinates
        int x,y;
        if(a < cols) {
          x = cols-1-a+b;         // scan from upper right to upper left corner
          y = b;
        }
        else {                    // scan from upper left to lower left corner
          x = b;
          y = a-(cols-1)+b;
        }
        
        if(val > 0) {

          // segment detection
          if(len == 0) {  // start of segment
            start = b;
            startX = x;
            startY = y;
            len++;
          }
          else {          // part or end of segment

            if((val >= (prevVal - par.segmentTolerance))    // check value of this pixel
             &&(val <= (prevVal + par.segmentTolerance))) { // if in tolerance range -> segment part
              len++;
            }
            else {       // otherwise -> segment end

              if(len >= par.minSegmLen) {
                // add segment to group list
                segmEntry newSegment;
                newSegment.len = len;
                newSegment.used = 0;
                newSegment.start.x = startX;
                newSegment.start.y = startY;
                newSegment.end.x = x-1;
                newSegment.end.y = y-1;
                m_vecSegments[3].push_back(newSegment); 
              }
              len = 0;
            }
          }
        }
        else {  // val == 0 
        
          if(len >= par.minSegmLen) {
            // add segment to group list
            segmEntry newSegment;
            newSegment.len = len;
            newSegment.used = 0;
            newSegment.start.x = startX;
            newSegment.start.y = startY;
            newSegment.end.x = x-1;
            newSegment.end.y = y-1;
            m_vecSegments[3].push_back(newSegment); 
          }
          len = 0;
        }
        
        prevVal = val;
      } // pixel scan
    } // line scan
  }

///////////////////////////////////////////////////////////////////////////////

  // creates segment lists
  void fastEllipseExtraction::detectGroup4Segments(const channel8& src) {
  
    // get parameters
    const parameters& par = getParameters();

    // get size of the input image
    const int rows = src.rows();
    const int cols = src.columns();
    m_iWidth = cols;
    
    // maximum values
    const int maxDiag   = (rows < cols) ? rows : cols;
    
    // calculate max ranges (diagonal)
    const int lineCount = rows+cols-1;  // scan from lower right over lower left to upper left corner
    int lineLength = 0;                 // line length depends on actual line scan position

    // clear old list and reserve memory
    m_vecSegments[4].clear(); 
    m_vecSegments[4].reserve(SegmPreallocation);

    // create new list  
    for(int a=0; a<lineCount; a++) { // line scan

      // copy actual line (diagonal vertical)
      vector<ubyte> line;

      if(a < cols) { // scan from lower_right to lower_left corner

        if(lineLength < maxDiag) lineLength++;     // set line length
        line.resize(lineLength, 0, false, false);  // resize vector

        for(int i=0; i<lineLength; i++) {          // fill vector
          line[i] = src.at(rows-1-i, cols-1-a+i);  // copy diagonal right up
        }
      }
      else {  // scan from lower_left to upper_left corner
      
        // check if line length starts decreasing
        if((lineCount - a) < maxDiag) lineLength--; // set line length
        line.resize(lineLength, 0, false, false);   // resize vector
        
        for(int i=0; i<lineLength; i++) {                // fill vector
          line[i] = src.at((rows-1)-(a-(cols-1))-i, i);  // copy diagonal right up
        }
      }

      int   prevVal = 0;   // value of the previous pixel
      int   start = 0;     // start of the actual segment
      int   len = 0;       // length of the actual segment
      int   startX = 0;    // starting x-coordinate
      int   startY = 0;    // starting y-coordinate

      for(int b=0; b<lineLength+1; b++) { // pixel scan

        ubyte val; 
        if(b < lineLength) val = line[b];  // actual pixel
        else val = 0;  // extra loop for lines ending at the border

        // calculate coordinates (diagonal vertical)
        int x,y;
        if(a < cols) {            // scan from lower right to lower left corner
          x = cols-1-a+b;
          y = rows-1-b;
        }
        else {                    // scan from lower left to upper left corner
          x = b;
          y = (rows-1)-(a-(cols-1))-b;
        }
        
        
        if(val > 0) {

          // segment detection
          if(len == 0) {  // start of segment
            start = b;
            startX = x;
            startY = y;
            len++;
          }
          else {        // part or end of segment
            if((val >= (prevVal - par.segmentTolerance))    // check value of this pixel
             &&(val <= (prevVal + par.segmentTolerance))) { // if in tolerance range -> segment part
              len++;
            }
            else {      // otherwise -> segment end

              if(len >= par.minSegmLen) {
                // add segment to group list
                segmEntry newSegment;
                newSegment.len = len;
                newSegment.used = 0;
                newSegment.start.x = startX;
                newSegment.start.y = startY;
                newSegment.end.x = x-1;
                newSegment.end.y = y+1;
                m_vecSegments[4].push_back(newSegment); 
              }
              len = 0;
            }
          }
        }
        else {  // val == 0 
        
          if(len >= par.minSegmLen) {
            // add segment to group list
            segmEntry newSegment;
            newSegment.len = len;
            newSegment.used = 0;
            newSegment.start.x = startX;
            newSegment.start.y = startY;
            newSegment.end.x = x-1;
            newSegment.end.y = y+1;
            m_vecSegments[4].push_back(newSegment); 
          }
          len = 0;
        }
        
        prevVal = val;
      } // pixel scan
    } // line scan
  }


///////////////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup1Lines() {
  
    // clear old complete line list and reserve memory
    m_vecLines[0].clear(); 
    m_vecLines[0].reserve(4*LinePreallocation);

    // clear old linelist and reserve memory
    clearLineList(1); 
    m_vecLines[1].reserve(LinePreallocation);
    
    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecSegments[1].size());

    if(segmCount > 0) {

      // set first element as starting segment Si
      int Si = 0;
      int Sq, SS, SL;

      do {
        // create new segment grouping vector
        std::vector<int> *vecLineSegments = new std::vector<int>;

        bool positive_slope = true;   // sign of the slope of the first segment pair
        bool first_candidate = true;  // only the first segment pair defines the slope
        
        // STEP1
        vecLineSegments->push_back(Si);  // store Si in linelist (starting segment)
        SS = Si;                        // set Si as new target segment SS for search
        SL = Si;                        // set Si as actual longest linesegment SL
        Sq = Si + 1;                    // first candidate segment
        
        while(Sq < segmCount) {   // track line segments
          // STEP2
          // calculate window boundaries for target segment SS
          const segmEntry segSS = m_vecSegments[1][SS];
          const int GAPx = par.maxSegmentGap;
          const int GAPx_pos = segSS.end.x + GAPx;
          const int GAPx_neg = segSS.start.x - GAPx;
          const float deltaY = -0.5f; //segSS.end.y - segSS.start.y - 0.5;
          const float deltaX = static_cast<float>(segSS.end.x - segSS.start.x);
          const float min_slope = deltaY / deltaX;
          const float GAPy_fract = min_slope * (GAPx_neg - segSS.end.x);
          const float GAPy_round = floor(GAPy_fract + 0.5f);
          const int GAPy = static_cast<int>(segSS.end.y + GAPy_round);
          
          // search new candidate segment Sq (from top to bottom)
          while(Sq < segmCount) {
            
            // no multi-use of segments in circle extraction
            if(m_vecSegments[1][Sq].used > 0) {
              Sq++;
              continue;
            }
            const segmEntry segSq = m_vecSegments[1][Sq];
            
            if(segSq.start.y > GAPy) { // window boundary reached ?
              Sq = segmCount;          // no candidate segment found
              break;
            }

            // positive slope or first candidate
            if(positive_slope || first_candidate) {
              if((segSq.start.x <= GAPx_pos)                         // Sq starts in window 
              && (segSq.start.x >= segSS.end.x - segSS.len/2 + 1)) { // Sq starts right from center of SS
                positive_slope = true;
                break;
              }
            }

            // negative slope or first candidate
            if(!positive_slope || first_candidate) {
              if((segSq.end.x >= GAPx_neg)                           // Sq ends in window 
              && (segSq.end.x <= segSS.start.x + segSS.len/2 - 1)) { // Sq ends left from center of SS
                positive_slope = false;
                break;
              }
            }
            Sq++;
          }

          if(Sq == segmCount) break;    // no candidate segment found

          const segmEntry segSi = m_vecSegments[1][Si];
          const segmEntry segSq = m_vecSegments[1][Sq];

          // calculate longest line segment SL (old SL or new Sq)
          if(m_vecSegments[1][SL].len < segSq.len) {
            SL = Sq;
          }
          const segmEntry segSL = m_vecSegments[1][SL];
          
          // STEP3
          // calculate slope
          float slope;
          if(positive_slope) {
            slope = ((float)(segSq.end.y - segSi.start.y)) / (segSq.end.x - segSi.start.x);
          }
          else {
            slope = ((float)(segSi.end.y - segSq.start.y)) / (segSi.end.x - segSq.start.x);
          }
          
          // STEP4
          // check if SL fits in estimated line
          //const float deltaD = segSL.start.y - (slope * segSL.start.x)
          //                   -(  segSL.end.y - (slope * segSL.end.x) );
          const float deltaD = slope * (segSL.end.x - segSL.start.x);

          // if not satisfied
          if((deltaD > par.maxQuantizationError) || (deltaD < -par.maxQuantizationError)) {
            if(first_candidate) {
              positive_slope = true;    // reset slope flag
            }
            break;   
          }
          else first_candidate = false;

          // store Sq in linelist (actual finishing segment)
          vecLineSegments->push_back(Sq);
          
          // STEP5
          // set Sq as new target segment SS and increment candidate segment
          SS = Sq++;
        }

        // STEP7
        // calculate line length
        const int nSegments = static_cast<int>(vecLineSegments->size());
        int length = 0;

        if(positive_slope) {
          const int x1 = m_vecSegments[1][(*vecLineSegments)[0]].start.x;
          const int x2 = m_vecSegments[1][(*vecLineSegments)[nSegments-1]].end.x;
          length = x2 - x1 + 1;
        }
        else {
          const int x1 = m_vecSegments[1][(*vecLineSegments)[nSegments-1]].start.x;
          const int x2 = m_vecSegments[1][(*vecLineSegments)[0]].end.x;
          length = x2 - x1 + 1;
        }

        // line length must be >= minLineLen for extraction
        if(length >= par.minLineLen) {
          
          // register grouped segments as extracted line
          lineEntry newline;
          if(positive_slope) {
            newline.start.x = m_vecSegments[1][(*vecLineSegments)[0]].start.x;
            newline.start.y = m_vecSegments[1][(*vecLineSegments)[0]].start.y;
            newline.end.x = m_vecSegments[1][(*vecLineSegments)[nSegments-1]].end.x;
            newline.end.y = m_vecSegments[1][(*vecLineSegments)[nSegments-1]].end.y;
          }
          else {
            newline.start.x = m_vecSegments[1][(*vecLineSegments)[nSegments-1]].start.x;
            newline.start.y = m_vecSegments[1][(*vecLineSegments)[nSegments-1]].start.y;
            newline.end.x = m_vecSegments[1][(*vecLineSegments)[0]].end.x;
            newline.end.y = m_vecSegments[1][(*vecLineSegments)[0]].end.y;
          }
          // calculate midpoint
          newline.mid.x = (newline.start.x + newline.end.x)/2;
          newline.mid.y = (newline.start.y + newline.end.y)/2;
          // calculate tangent
          newline.tangent = atan2((double) -(newline.end.y-newline.start.y), (double)(newline.end.x-newline.start.x));
          //newline.tangent = atan((double) -(newline.end.y-newline.start.y) / (newline.end.x-newline.start.x));
          // set other values
          newline.len = length;
          newline.used = 0;
          newline.group = 1;
          // store data
          newline.segmIdxList = vecLineSegments;
          m_vecLines[1].push_back(newline);
          m_vecLines[0].push_back(newline);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecSegments[1][(*vecLineSegments)[i]].used++;
          }
        }
        else delete vecLineSegments;

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecSegments[1][Si].used == 0) break;
        }
        
      }
      while(Si < segmCount);
    }
  }

///////////////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup2Lines() {

    // clear old linelist and reserve memory
    clearLineList(2);
    m_vecLines[2].reserve(LinePreallocation);
  
    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecSegments[2].size());

    if(segmCount > 0) {

      // set first element as starting segment Si
      int Si = 0;
      int Sq, SS, SL;

      do {
        // create new segment grouping vector
        std::vector<int> *vecLineSegments = new std::vector<int>;

        bool positive_slope = true;   // sign of the slope of the first segment pair
        bool first_candidate = true;   // only the first segment pair defines the slope
        
        // STEP1
        vecLineSegments->push_back(Si);  // store Si in linelist (starting segment)
        SS = Si;                        // set Si as new target segment SS for search
        SL = Si;                        // set Si as actual longest linesegment SL
        Sq = Si + 1;                    // first candidate segment


        while(Sq < segmCount) {   // track line segments
          // STEP2
          // calculate window boundaries for target segment SS
          const segmEntry segSS = m_vecSegments[2][SS];
          const int GAPx = par.maxSegmentGap;
          const int GAPx_pos = segSS.end.y + GAPx;
          const int GAPx_neg = segSS.start.y - GAPx;
          const float deltaY = -0.5f; //segSS.end.x - segSS.start.x - 0.5;
          const float deltaX = static_cast<float>(segSS.end.y - segSS.start.y);
          const float min_slope = deltaY / deltaX;
          const float GAPy_fract = min_slope * (GAPx_neg - segSS.end.y);
          const float GAPy_round = floor(GAPy_fract + 0.5f);
          const int GAPy = static_cast<int>(segSS.end.x + GAPy_round);


          // search new candidate segment Sq (from left to right)
          while(Sq < segmCount) {
          
            // no multi-use of segments in circle extraction
            if(m_vecSegments[2][Sq].used > 0) {
              Sq++;
              continue;
            }
            const segmEntry segSq = m_vecSegments[2][Sq];

            if(segSq.start.x > GAPy) {  // window boundary reached ?
              Sq = segmCount;   // no candidate segment found
              break;
            }

            // positive slope or first candidate
            if(positive_slope || first_candidate) {
              if((segSq.start.y <= GAPx_pos)                         // Sq starts in window 
              && (segSq.start.y >= segSS.end.y - segSS.len/2 + 1)) { // Sq starts below from center of SS
                  positive_slope = true;
                  break;
              }
            }

            // negative slope or first candidate
            if(!positive_slope || first_candidate) {
              if((segSq.end.y >= GAPx_neg)                           // Sq ends in window 
              && (segSq.end.y <= segSS.start.y + segSS.len/2 - 1)) { // Sq ends above from center of SS
                  positive_slope = false;
                  break;
              }
            }

            Sq++;
          }

          if(Sq == segmCount) break;    // no candidate segment found

          const segmEntry segSi = m_vecSegments[2][Si];
          const segmEntry segSq = m_vecSegments[2][Sq];

          // calculate longest line segment SL (old SL or new Sq)
          if(m_vecSegments[2][SL].len < segSq.len) {
            SL = Sq;
          }
          const segmEntry segSL = m_vecSegments[2][SL];

          // STEP3
          // calculate slope
          float slope;
          if(positive_slope) {
            slope = ((float)(segSq.end.x - segSi.start.x)) / (segSq.end.y - segSi.start.y);
          }
          else {
            slope = ((float)(segSi.end.x - segSq.start.x)) / (segSi.end.y - segSq.start.y);
          }
          
          // STEP4
          // check if SL fits in estimated line
          //const float deltaD = segSL.start.x - (slope * segSL.start.y)
          //                   -(  segSL.end.x - (slope * segSL.end.y) );
          const float deltaD = slope * (segSL.end.y - segSL.start.y);
          
          // if not satisfied
          if((deltaD > par.maxQuantizationError) || (deltaD < -par.maxQuantizationError)) {
            if(first_candidate) {
              positive_slope = true;    // reset slope flag
            }
            break;   
          }
          else first_candidate = false;

          // store Sq in linelist (actual finishing segment)
          vecLineSegments->push_back(Sq);
          
          // STEP5
          // set Sq as new target segment SS and increment candidate segment
          SS = Sq++;
        }

        // STEP7
        // calculate line length
        const int nSegments = static_cast<int>(vecLineSegments->size());
        int length = 0;

        if(positive_slope) {
          const int y1 = m_vecSegments[2][(*vecLineSegments)[0]].start.y;
          const int y2 = m_vecSegments[2][(*vecLineSegments)[nSegments-1]].end.y;
          length = y2 - y1 + 1;
        }
        else {
          const int y1 = m_vecSegments[2][(*vecLineSegments)[nSegments-1]].start.y;
          const int y2 = m_vecSegments[2][(*vecLineSegments)[0]].end.y;
          length = y2 - y1 + 1;
        }

        // line length must be >= minLineLen for extraction
        if(length >= par.minLineLen) {
        
          // register grouped segments as extracted line
          lineEntry newline;
          if(positive_slope) {
            newline.start.x = m_vecSegments[2][(*vecLineSegments)[0]].start.x;
            newline.start.y = m_vecSegments[2][(*vecLineSegments)[0]].start.y;
            newline.end.x = m_vecSegments[2][(*vecLineSegments)[nSegments-1]].end.x;
            newline.end.y = m_vecSegments[2][(*vecLineSegments)[nSegments-1]].end.y;
          }
          else {
            newline.start.x = m_vecSegments[2][(*vecLineSegments)[nSegments-1]].start.x;
            newline.start.y = m_vecSegments[2][(*vecLineSegments)[nSegments-1]].start.y;
            newline.end.x = m_vecSegments[2][(*vecLineSegments)[0]].end.x;
            newline.end.y = m_vecSegments[2][(*vecLineSegments)[0]].end.y;
          }
          // calculate midpoint
          newline.mid.x = (newline.start.x + newline.end.x)/2;
          newline.mid.y = (newline.start.y + newline.end.y)/2;
          // calculate tangent (wrong direction because of legacy implementation)
          newline.tangent = atan2((double)(newline.end.y-newline.start.y), (double) -(newline.end.x-newline.start.x));
          //newline.tangent = atan((double) -(newline.end.y-newline.start.y) / (newline.end.x-newline.start.x));
          // set other values
          newline.len = length;
          newline.used = 0;
          newline.group = 2;
          // store data
          newline.segmIdxList = vecLineSegments;
          m_vecLines[2].push_back(newline);
          m_vecLines[0].push_back(newline);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecSegments[2][(*vecLineSegments)[i]].used++;
          }
        }
        else delete vecLineSegments;

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecSegments[2][Si].used == 0) break;
        }
        
      }
      while(Si < segmCount);
    }
  }

///////////////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup3Lines() {

    // clear old linelist and reserve memory
    clearLineList(3);
    m_vecLines[3].reserve(LinePreallocation);

    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecSegments[3].size());
    
    if(segmCount > 0) {

      // set first element as starting segment Si
      int Si = 0;
      int Sq, SS, SL;

      do {
        // create new segment grouping vector
        std::vector<int> *vecLineSegments = new std::vector<int>;
        
        bool positive_slope = true;   // sign of the slope of the first segment pair
        bool first_candidate = true;   // only the first segment pair defines the slope
        
        // STEP1
        vecLineSegments->push_back(Si);  // store Si in linelist (starting segment)
        SS = Si;                        // set Si as new target segment SS for search
        SL = Si;                        // set Si as actual longest linesegment SL
        Sq = Si + 1;                    // first candidate segment


        while(Sq < segmCount) {  // track line segments

          const segmEntry segSS = m_vecSegments[3][SS];

          // transform coordinates (diagonal -> horizontal)
          const int SS_start_x = segSS.start.x + segSS.start.y;
          const int SS_end_x   = segSS.end.x + segSS.end.y;
          const int SS_end_y   = m_iWidth - 1 - segSS.end.x + segSS.end.y;
          
          // STEP2
          // calculate window boundaries for target segment SS
          const int GAPx = par.maxSegmentGap;
          const int GAPx_pos = SS_end_x + 1 + GAPx*2;   // double gap + 1 
          const int GAPx_neg = SS_start_x - 1 - GAPx*2; // double gap + 1 
          const float deltaY = -0.5f; //SS_end_y - SS_start_y - 0.5;
          const float deltaX = static_cast<float>(SS_end_x - SS_start_x);
          const float min_slope = deltaY / deltaX;
          const float GAPy_fract = min_slope * (GAPx_neg - SS_end_x);
          const float GAPy_round = floor(GAPy_fract + 0.5f);
          const int GAPy = static_cast<int>(SS_end_y + GAPy_round);


          // search new candidate segment Sq (from top to bottom)
          while(Sq < segmCount) {
          
            // no multi-use of segments in circle extraction
            if(m_vecSegments[3][Sq].used > 0) {
              Sq++;
              continue;
            }
            const segmEntry segSq = m_vecSegments[3][Sq];

            // transform coordinates (diagonal -> horizontal)
            const int Sq_start_x = segSq.start.x + segSq.start.y;
            const int Sq_start_y = m_iWidth - 1 - segSq.start.x + segSq.start.y;
            const int Sq_end_x   = segSq.end.x + segSq.end.y;

            if(Sq_start_y > GAPy) { // window boundary reached ?
              Sq = segmCount;       // no candidate segment found
              break;
            }

            // positive slope or first candidate
            if(positive_slope || first_candidate) {
              if((Sq_start_x <= GAPx_pos)                    // Sq starts in window 
              && (Sq_start_x >= SS_end_x - segSS.len + 1)) { // Sq lies right from center of SS
                positive_slope = true;
                break;
              }
            }

            // negative slope or first candidate
            if(!positive_slope || first_candidate) {
              if((Sq_end_x >= GAPx_neg)                      // Sq ends in window 
              && (Sq_end_x <= SS_start_x + segSS.len - 1)) { // Sq lies left from center of SS
                positive_slope = false;
                break;
              }
            }

            Sq++;
          }

          if(Sq == segmCount) break;    // no candidate segment found

          const segmEntry segSi = m_vecSegments[3][Si];
          const segmEntry segSq = m_vecSegments[3][Sq];

          // calculate longest line segment SL (old SL or new Sq)
          if(m_vecSegments[3][SL].len < segSq.len) {
            SL = Sq;
          }
          const segmEntry segSL = m_vecSegments[3][SL];

          // STEP3
          // calculate slope
          float slope;
          if(positive_slope) {
            slope = ((float)(segSq.end.y - segSi.start.y)) / (segSq.end.x - segSi.start.x);
          }
          else {
            slope = ((float)(segSi.end.y - segSq.start.y)) / (segSi.end.x - segSq.start.x);
          }
          
          // STEP4
          // check if SL fits in estimated line
          float deltaD = segSL.start.y - (slope * segSL.start.x)
                       -(  segSL.end.y - (slope * segSL.end.x) );
          //float deltaD = slope * (segSL.end.x - segSL.start.x);
          
          deltaD *= 0.5f; // check against deltaD/2 because of diagonal double quantization error
          
          // if not satisfied (check against deltaD/2 because of diagonal double quantization error)
          if((deltaD > par.maxQuantizationError) || (deltaD < -par.maxQuantizationError)) {
            if(first_candidate) {
              positive_slope = true;    // reset slope flag
            }
            break;   
          }
          else first_candidate = false;

          // store Sq in linelist (actual finishing segment)
          vecLineSegments->push_back(Sq);
          
          // STEP5
          // set Sq as new target segment SS and increment candidate segment
          SS = Sq++;
        }

        // STEP7
        // calculate line length
        const int nSegments = static_cast<int>(vecLineSegments->size());
        int length = 0;

        if(positive_slope) {
          const int y1 = m_vecSegments[3][(*vecLineSegments)[0]].start.y;
          const int y2 = m_vecSegments[3][(*vecLineSegments)[nSegments-1]].end.y;
          length = y2 - y1 + 1;
        }
        else {
          const int x1 = m_vecSegments[3][(*vecLineSegments)[nSegments-1]].start.x;
          const int x2 = m_vecSegments[3][(*vecLineSegments)[0]].end.x;
          length = x2 - x1 + 1;
        }

        // line length must be >= minLineLen for extraction
        if(length >= par.minLineLen) {
        
          // register grouped segments as extracted line
          lineEntry newline;
          if(positive_slope) {
            newline.start.x = m_vecSegments[3][(*vecLineSegments)[0]].start.x;
            newline.start.y = m_vecSegments[3][(*vecLineSegments)[0]].start.y;
            newline.end.x = m_vecSegments[3][(*vecLineSegments)[nSegments-1]].end.x;
            newline.end.y = m_vecSegments[3][(*vecLineSegments)[nSegments-1]].end.y;
          }
          else {
            newline.end.x = m_vecSegments[3][(*vecLineSegments)[0]].end.x;
            newline.end.y = m_vecSegments[3][(*vecLineSegments)[0]].end.y;
            newline.start.x = m_vecSegments[3][(*vecLineSegments)[nSegments-1]].start.x;
            newline.start.y = m_vecSegments[3][(*vecLineSegments)[nSegments-1]].start.y;
          }
          // calculate midpoint
          newline.mid.x = (newline.start.x + newline.end.x)/2;
          newline.mid.y = (newline.start.y + newline.end.y)/2;
          // calculate tangent
          newline.tangent = atan2((double) -(newline.end.y-newline.start.y), (double)(newline.end.x-newline.start.x));
          //newline.tangent = atan((double) -(newline.end.y-newline.start.y) / (newline.end.x-newline.start.x));
          // set other values
          newline.len = length;
          newline.used = 0;
          newline.group = 3;
          // store data
          newline.segmIdxList = vecLineSegments;
          m_vecLines[3].push_back(newline);
          m_vecLines[0].push_back(newline);


          for(int i=0; i<nSegments; i++) {
            // increase used-value of the grouped segments
            m_vecSegments[3][(*vecLineSegments)[i]].used++;
          }
        }
        else delete vecLineSegments;

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecSegments[3][Si].used == 0) break;
        }
        
      }
      while(Si < segmCount);
    }
  }

///////////////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup4Lines() {

    // clear old linelist and reserve memory
    clearLineList(4);
    m_vecLines[4].reserve(LinePreallocation);
  
    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecSegments[4].size());
    
    if(segmCount > 0) {

      // set first element as starting segment Si
      int Si = 0;
      int Sq, SS, SL;

      do {
        // create new segment grouping vector
        std::vector<int> *vecLineSegments = new std::vector<int>;

        bool positive_slope = true;   // sign of the slope of the first segment pair
        bool first_candidate = true;   // only the first segment pair defines the slope
        
        // STEP1
        vecLineSegments->push_back(Si);  // store Si in linelist (starting segment)
        SS = Si;                        // set Si as new target segment SS for search
        SL = Si;                        // set Si as actual longest linesegment SL
        Sq = Si + 1;                    // first candidate segment


        while(Sq < segmCount) {  // track line segments
        
          const segmEntry segSS = m_vecSegments[4][SS];

          // transform coordinates (diagonal -> horizontal)
          const int SS_start_y = m_iWidth - 1 - segSS.start.x + segSS.start.y;
          const int SS_end_x   = segSS.end.x + segSS.end.y;
          const int SS_end_y   = m_iWidth - 1 - segSS.end.x + segSS.end.y;
          
          // STEP2
          // calculate window boundaries for target segment SS
          const int GAPx = par.maxSegmentGap;
          const int GAPx_pos = SS_start_y + 1 + GAPx*2;   // double gap + 1 
          const int GAPx_neg = SS_end_y - 1 - GAPx*2; // double gap + 1 
          const float deltaY = -0.5f; //SS_start_x - SS_end_x - 0.5;
          const float deltaX = static_cast<float>(SS_start_y - SS_end_y);
          const float min_slope = deltaY / deltaX;
          const float GAPy_fract = min_slope * (GAPx_neg - SS_start_y);
          const float GAPy_round = floor(GAPy_fract + 0.5f);
          const int GAPy = static_cast<int>(SS_end_x - GAPy_round);
          
          // search new candidate segment Sq (from right to left)
          while(Sq < segmCount) {
          
            // no multi-use of segments in circle extraction
            if(m_vecSegments[4][Sq].used > 0) {
              Sq++;
              continue;
            }
            const segmEntry segSq = m_vecSegments[4][Sq];

            // transform coordinates (diagonal -> horizontal)
            const int Sq_start_x = segSq.start.x + segSq.start.y;
            const int Sq_start_y = m_iWidth - 1 - segSq.start.x + segSq.start.y;
            const int Sq_end_y   = m_iWidth - 1 - segSq.end.x + segSq.end.y;

            if(Sq_start_x < GAPy) { // window boundary reached ?
              Sq = segmCount;       // no candidate segment found
              break;
            }

            // positive slope or first candidate
            if(positive_slope || first_candidate) {
              if((Sq_start_y >= GAPx_neg)                     // Sq starts in window 
              && (Sq_start_y <= SS_end_y + segSS.len - 1)) {  // Sq lies above from center of SS
                positive_slope = true;
                break;
              }
            }

            // negative slope or first candidate
            if(!positive_slope || first_candidate) {
              if((Sq_end_y <= GAPx_pos)                      // Sq ends in window 
              && (Sq_end_y >= SS_start_y - segSS.len + 1)) { // Sq lies below from center of SS
                positive_slope = false;
                break;
              }
            }

            Sq++;
          }

          if(Sq == segmCount) break;    // no candidate segment found

          const segmEntry segSi = m_vecSegments[4][Si];
          const segmEntry segSq = m_vecSegments[4][Sq];

          // calculate longest line segment SL (old SL or new Sq)
          if(m_vecSegments[4][SL].len < segSq.len) {
            SL = Sq;
          }
          const segmEntry segSL = m_vecSegments[4][SL];

          // STEP3
          // calculate slope
          float slope;
          if(positive_slope) {
            slope = ((float)(segSq.end.y - segSi.start.y)) / (segSq.end.x - segSi.start.x);
          }
          else {
            slope = ((float)(segSi.end.y - segSq.start.y)) / (segSi.end.x - segSq.start.x);
          }
          
          // STEP4
          // check if SL fits in estimated line
          float deltaD = segSL.start.y - (slope * segSL.start.x)
                       -(  segSL.end.y - (slope * segSL.end.x) );
          //float deltaD = slope * (segSL.end.x - segSL.start.x);
          
          deltaD *= 0.5f; // check against deltaD/2 because of diagonal double quantization error
          
          // if not satisfied (check against deltaD/2 because of diagonal double quantization error)
          if((deltaD > par.maxQuantizationError) || (deltaD < -par.maxQuantizationError)) {
            if(first_candidate) {
              positive_slope = true;    // reset slope flag
            }
            break;   
          }
          else first_candidate = false;

          // store Sq in linelist (actual finishing segment)
          vecLineSegments->push_back(Sq);
          
          // STEP5
          // set Sq as new target segment SS and increment candidate segment
          SS = Sq++;
        }

        // STEP7
        // calculate line length
        const int nSegments = static_cast<int>(vecLineSegments->size());
        int length = 0;

        if(positive_slope) {
          const int y1 = m_vecSegments[4][(*vecLineSegments)[nSegments-1]].end.y;
          const int y2 = m_vecSegments[4][(*vecLineSegments)[0]].start.y;
          length = y2 - y1 + 1;
        }
        else {
          const int x1 = m_vecSegments[4][(*vecLineSegments)[nSegments-1]].start.x;
          const int x2 = m_vecSegments[4][(*vecLineSegments)[0]].end.x;
          length = x2 - x1 + 1;
        }

        // line length must be >= minLineLen for extraction
        if(length >= par.minLineLen) {
        
          // register grouped segments as extracted line
          lineEntry newline;
          if(positive_slope) {
            newline.start.x = m_vecSegments[4][(*vecLineSegments)[nSegments-1]].end.x;
            newline.start.y = m_vecSegments[4][(*vecLineSegments)[nSegments-1]].end.y;
            newline.end.x = m_vecSegments[4][(*vecLineSegments)[0]].start.x;
            newline.end.y = m_vecSegments[4][(*vecLineSegments)[0]].start.y;
          }
          else {
            newline.start.x = m_vecSegments[4][(*vecLineSegments)[0]].end.x;
            newline.start.y = m_vecSegments[4][(*vecLineSegments)[0]].end.y;
            newline.end.x = m_vecSegments[4][(*vecLineSegments)[nSegments-1]].start.x;
            newline.end.y = m_vecSegments[4][(*vecLineSegments)[nSegments-1]].start.y;
          }
          // calculate midpoint
          newline.mid.x = (newline.start.x + newline.end.x)/2;
          newline.mid.y = (newline.start.y + newline.end.y)/2;
          // calculate tangent (wrong direction because of legacy implementation)
          newline.tangent = atan2((double)(newline.end.y-newline.start.y), (double) -(newline.end.x-newline.start.x));
          //newline.tangent = atan((double) -(newline.end.y-newline.start.y) / (newline.end.x-newline.start.x));
          
          // set other values
          newline.len = length;
          newline.used = 0;
          newline.group = 4;
          // store data
          newline.segmIdxList = vecLineSegments;
          m_vecLines[4].push_back(newline);
          m_vecLines[0].push_back(newline);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecSegments[4][(*vecLineSegments)[i]].used++;
          }
        }
        else delete vecLineSegments;

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecSegments[4][Si].used == 0) break;
        }
        
      }
      while(Si < segmCount);
    }
  }

///////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup1and5Arcs() {

    // clear old arclist and reserve memory
    clearEllArcList(0);
    m_vecEllArcs[0].reserve(8*EllArcPreallocation);
    clearEllArcList(1);
    m_vecEllArcs[1].reserve(EllArcPreallocation);
    clearEllArcList(5);
    m_vecEllArcs[5].reserve(EllArcPreallocation);
    
    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecLines[1].size());
    
    if(segmCount > 0) {

      int Si = 0;
      int Sq, SS, Sl;

      do {
        // create new line grouping vector
        std::vector<int> *vecArcSegments = new std::vector<int>;

        bool forward_search = false;  // search direction along the arc (forward = left->right)
        bool upward_search = false;   // search direction in the window (upward = bottom->up)
        bool extra_down = false;      // extra search cycle to continue inverted direction segments
        bool extra_up = false;        // extra search cycle to continue inverted direction segments

        // STEP1
        vecArcSegments->push_back(Si);  // store Si in arc segment list (starting line)
        SS = Si;                        // set Si as new target arc segment SS for search
        Sq = Si + 1;                    // first candidate segment
        Sl = -1;                        // last fitting candidate
        
        while(true) {   // track lines (line order: left->right, top->down)
          // STEP2
          const lineEntry segSS = m_vecLines[1][SS];
          
          int SSy, SSx;
          if(forward_search) {
            SSx = segSS.end.x;
            SSy = segSS.end.y;
          } else {
            SSx = segSS.start.x;
            SSy = segSS.start.y;
          }

          // calculate window
          const int GAPx_pos = par.maxLineGap;
          const int GAPx_neg = segSS.len/2 - 1;
          int GAPy_pos, GAPy_neg;
          if(upward_search) {
            GAPy_pos = par.maxLineGap - 1;
            GAPy_neg = 1;
          } else {
            GAPy_pos = 1;
            GAPy_neg = par.maxLineGap - 1;
          }
          const int minY = SSy - GAPy_pos;
          const int maxY = SSy + GAPy_neg;
          
          // search new candidate segment Sq
          while((Sq >= 0)&&(Sq < segmCount)) {

            const lineEntry segSq = m_vecLines[1][Sq];
            
            // vertical window boundary reached ? (both points outside the window)
            if(((segSq.start.y < minY)||(segSq.start.y > maxY)) 
            &&((segSq.end.y < minY)||(segSq.end.y > maxY))) { 
              Sq = -1;   // no candidate segment found
              break;
            }
            
            // calculate distance of Sq
            int dx,dy;
            if(forward_search) {
              dx = segSq.start.x - SSx;
              dy = segSq.start.y - SSy;
            } else {
              dx = SSx - segSq.end.x;
              dy = SSy - segSq.end.y;
            }             

            // calculate relative candidate angle
            const double angle = segSq.tangent - segSS.tangent;

            // check negative conditions
            if((dx > GAPx_pos)||(dx < -GAPx_neg)  // dx too large
            ||(dy > GAPy_pos)||(dy < -GAPy_neg)   // dy too large
            ||((dx <= 0)&&((dy > 1)||(dy < -1)))  // parallel gap too large
            ||((dx > 0)&&((dy > dx)||(dy < -dx))) // parallel gap too large
            ||((forward_search)&&(upward_search)&&(angle < 0))  // tangent directions
            ||((forward_search)&&(!upward_search)&&(angle > 0))
            ||((!forward_search)&&(upward_search)&&(angle > 0))
            ||((!forward_search)&&(!upward_search)&&(angle < 0))) {

              if(upward_search) {
                Sq--;
              } else {
                Sq++;
              }
              continue;
            }
            
            break;
          }

          if((Sq == segmCount)||(Sq < 0)) {   // no candidate segment found

            // next search cycle (STAGE 1: backward_down/extra_up -> backward_up/extra_down
            //                    STAGE 2: forward_down/extra_up -> forward_up/extra_down 
            //                    STAGE 3: end)
            if(upward_search) {
              if(extra_up) {            // NEXT: backward_up/forward_up
                extra_up = false;
                SS = Si;                // reset target segment
                Sq = Si - 1;            // first upward_search candidate
                Sl = -1;                // reset last fitting candidate
                continue;
              }
              if(!extra_down) {         // NEXT: extra_down
                extra_down = true;
                upward_search = false;  // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl + 1;
                }
                continue;
              }
            
            } else {  // down
              
              if(extra_down) {          // NEXT: forward_down/end
                extra_down = false;
                if(forward_search) {    // set next search stage
                  break;  // END
                } else {
                  forward_search = true;  // START FORWARD_SEARCH
                }
                SS = Si;                // reset target segment
                Sq = Si + 1;            // first forward_search candidate
                Sl = -1;                // reset last fitting candidate
                // resort segment list
                const int n = static_cast<int>(vecArcSegments->size());
                for(int i=0; i<(n/2); i++) {  
                  const int tmp = (*vecArcSegments)[i];
                  (*vecArcSegments)[i] = (*vecArcSegments)[n-1-i];
                  (*vecArcSegments)[n-1-i] = tmp;
                }
                continue;
              }
              
              if(!extra_up) {           // !extra_up
                extra_up = true;        
                upward_search = true;   // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl - 1;
                }
                continue;
              }
            }
          }


        
          // STEP3: check interior angle
          const double tangent1 = m_vecLines[1][SS].tangent;
          const double tangent2 = m_vecLines[1][Sq].tangent;
          double interiorAngle = (tangent2-tangent1)*180/PI;
          if(interiorAngle < 0) interiorAngle = -interiorAngle;
          interiorAngle = 180 - interiorAngle;

          if((interiorAngle <= 180)&&(interiorAngle >= 135)) {
            double Xest, Yest, R2est;
            vecArcSegments->push_back(Sq);  // store Sq in linelist
            // estimate circle parameters
            if(!estimateCircle(*vecArcSegments, 1, Xest, Yest, R2est)) {
              vecArcSegments->pop_back();   // circle estimation failed
            } else {
              // STEP4: check tangents
              const double deltaX1 = Xest - m_vecLines[1][SS].mid.x;
              const double deltaY1 = Yest - m_vecLines[1][SS].mid.y;
              double TetaEst1 = atan2(deltaX1, deltaY1);
              if((deltaY1 < 0)&&(deltaX1 >= 0)) TetaEst1 -= PI;
              if((deltaY1 < 0)&&(deltaX1 < 0)) TetaEst1 += PI;
  
              const double deltaX2 = Xest - m_vecLines[1][Sq].mid.x;
              const double deltaY2 = Yest - m_vecLines[1][Sq].mid.y;
              double TetaEst2 = atan2(deltaX2, deltaY2);
              if((deltaY2 < 0)&&(deltaX2 >= 0)) TetaEst2 -= PI;
              if((deltaY2 < 0)&&(deltaX2 < 0)) TetaEst2 += PI;
  
              if(((TetaEst1-tangent1)*180/PI > par.maxLineTangentError)
              ||((TetaEst1-tangent1)*180/PI < -par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI > par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI < -par.maxLineTangentError)) {
                vecArcSegments->pop_back(); // error exceeds maximum
              } else {
                SS = Sq;    // set Sq as new target segment SS
                Sl = Sq;    // last fitting candidate
              }
            } //circle estimation
          } //interior angle
          
          if(upward_search) {
            Sq--;
          } else {
            Sq++;
          }
        } //line tracking


      // CHECK ARC EXTRACTION CONDITIONS
        const int nSegments = static_cast<int>(vecArcSegments->size());
        
        int x = 0;
        int y = 0;
        int r2 = 0;
        double Xest, Yest, R2est;
        if(estimateCircle(*vecArcSegments, 1, Xest, Yest, R2est)) {
          x = static_cast<int>(floor(Xest + 0.5));
          y = static_cast<int>(floor(Yest + 0.5));
          r2 = static_cast<int>(floor(R2est + 0.5));
        } else {
          r2 = 0;   // estimation failed
        }

        // radius must be greater than 10 pixel for extraction
        // at least 2 segments are neccessary for circle estimation
        if((r2 >= 100)&&(nSegments >= 2)) {

      // EXTRACT ARC
          // register grouped segments as extracted arc
          ellArcEntry newarc;
          // start/end points
          const int x1 = m_vecLines[1][(*vecArcSegments)[0]].start.x;
          const int y1 = m_vecLines[1][(*vecArcSegments)[0]].start.y;
          const int x2 = m_vecLines[1][(*vecArcSegments)[nSegments-1]].end.x;
          const int y2 = m_vecLines[1][(*vecArcSegments)[nSegments-1]].end.y;
          
          bool reverse = false;
          // check direction and group
          if(y >= y1) {
            newarc.group = 1; // top
            if(x1 > x2) {
              reverse = true;
            }
          } else {
            newarc.group = 5; // bottom
            if(x1 < x2) {
              reverse = true;
            }
          }
          
          if(reverse) {     // reverse idxlist
            int i;
            for(i=0; i<nSegments/2; i++) {
              int swap = (*vecArcSegments)[i];
              (*vecArcSegments)[i] = (*vecArcSegments)[nSegments-1-i];
              (*vecArcSegments)[nSegments-1-i] = swap;
            }
            // start/end points
            newarc.start.x = x2;
            newarc.start.y = y2;
            newarc.end.x = x1;
            newarc.end.y = y1;
          } else {
            newarc.start.x = x1;
            newarc.start.y = y1;
            newarc.end.x = x2;
            newarc.end.y = y2;
          }
          
          // first/last line vectors
          if(newarc.group == 1) {
            newarc.firstVec = m_vecLines[1][(*vecArcSegments)[0]].end 
                            - m_vecLines[1][(*vecArcSegments)[0]].start;
            newarc.lastVec = m_vecLines[1][(*vecArcSegments)[nSegments-1]].end 
                           - m_vecLines[1][(*vecArcSegments)[nSegments-1]].start;
          } else {
            newarc.firstVec = m_vecLines[1][(*vecArcSegments)[0]].start
                            - m_vecLines[1][(*vecArcSegments)[0]].end;
            newarc.lastVec = m_vecLines[1][(*vecArcSegments)[nSegments-1]].start
                           - m_vecLines[1][(*vecArcSegments)[nSegments-1]].end;
          }
          
          // midpoint
          newarc.mid.x = x;
          newarc.mid.y = y;
          // other values
          newarc.r2 = r2;
          newarc.used = 0;
          // line index list
          newarc.lineIdxList = vecArcSegments;
          
          // store data
          m_vecEllArcs[newarc.group].push_back(newarc);
          m_vecEllArcs[0].push_back(newarc);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecLines[1][(*vecArcSegments)[i]].used++;
          }
        } else {
          delete vecArcSegments;
        }

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecLines[1][Si].used == 0) break;
        }
      }
      while(Si < segmCount);    // extract next arc if possible
    }
  }


///////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup3and7Arcs() {

    // clear old arclist and reserve memory
    clearEllArcList(3);
    m_vecEllArcs[3].reserve(EllArcPreallocation);
    clearEllArcList(7);
    m_vecEllArcs[7].reserve(EllArcPreallocation);

    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecLines[2].size());
    
    if(segmCount > 0) {

      int Si = 0;
      int Sq, SS, Sl;

      do {
        // create new line grouping vector
        std::vector<int> *vecArcSegments = new std::vector<int>;

        bool forward_search = false;  // search direction along the arc (forward = top->down)
        bool upward_search = false;   // search direction in the window (upward = right->left)
        bool extra_down = false;      // extra search cycle to continue inverted direction segments
        bool extra_up = false;        // extra search cycle to continue inverted direction segments

        
        // STEP1
        vecArcSegments->push_back(Si);  // store Si in arc segment list (starting line)
        SS = Si;                        // set Si as new target arc segment SS for search
        Sq = Si + 1;                    // first candidate segment
        Sl = -1;                        // last fitting candidate
        
        while(true) {   // track lines (line order: top->down, left->right)
          // STEP2
          const lineEntry segSS = m_vecLines[2][SS];
          
          int SSy, SSx;
          if(forward_search) {
            SSx = segSS.end.x;
            SSy = segSS.end.y;
          } else {
            SSx = segSS.start.x;
            SSy = segSS.start.y;
          }

          // calculate window
          const int GAPy_pos = par.maxLineGap;
          const int GAPy_neg = segSS.len/2 - 1;
          int GAPx_pos, GAPx_neg;
          if(upward_search) {
            GAPx_pos = par.maxLineGap - 1;
            GAPx_neg = 1;
          } else {
            GAPx_pos = 1;
            GAPx_neg = par.maxLineGap - 1;
          }
          const int minX = SSx - GAPx_pos;
          const int maxX = SSx + GAPx_neg;

          // search new candidate segment Sq
          while((0 <= Sq)&&(Sq < segmCount)) {

            const lineEntry segSq = m_vecLines[2][Sq];

            // horizontal window boundary reached ? (both points outside the window)
            if(((segSq.end.x < minX)||(segSq.end.x > maxX)) 
            &&((segSq.start.x < minX)||(segSq.start.x > maxX))) { 
              Sq = -1;   // no candidate segment found
              break;
            }

            // calculate of Sq
            int dx,dy;
            if(forward_search) {
              dx = segSq.start.x - SSx;
              dy = segSq.start.y - SSy;
            } else {
              dx = SSx - segSq.end.x;
              dy = SSy - segSq.end.y;
            }             

            // calculate relative candidate angle
            const double angle = segSq.tangent - segSS.tangent;

            // check negative conditions
            if((dy > GAPy_pos)||(dy < -GAPy_neg)  // dy too large
            ||(dx > GAPx_pos)||(dx < -GAPx_neg)   // dx too large
            ||((dy <= 0)&&((dx > 1)||(dx < -1)))  // parallel gap too large
            ||((dy > 0)&&((dx > dy)||(dx < -dy))) // parallel gap too large
            ||((forward_search)&&(upward_search)&&(angle > 0))  // tangent directions (grp2: false tangent sign)
            ||((forward_search)&&(!upward_search)&&(angle < 0))
            ||((!forward_search)&&(upward_search)&&(angle < 0))
            ||((!forward_search)&&(!upward_search)&&(angle > 0))) {

              if(upward_search) {
                Sq--;
              } else {
                Sq++;
              }
              continue;
            }
            
            break;
          }

          if((Sq == segmCount)||(Sq < 0)) {   // no candidate segment found

            // next search cycle (STAGE 1: backward_down/extra_up -> backward_up/extra_down
            //                    STAGE 2: forward_down/extra_up -> forward_up/extra_down 
            //                    STAGE 3: end)
            if(upward_search) {
              if(extra_up) {            // NEXT: backward_up/forward_up
                extra_up = false;
                SS = Si;                // reset target segment
                Sq = Si - 1;            // first upward_search candidate
                Sl = -1;                // reset last fitting candidate
                continue;
              }
              if(!extra_down) {         // NEXT: extra_down
                extra_down = true;
                upward_search = false;  // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl + 1;
                }
                continue;
              }
            } else {  // down
              
              if(extra_down) {          // NEXT: forward_down/end
                extra_down = false;
                if(forward_search) {    // set next search stage
                  break;  // END
                } else {
                  forward_search = true;  // START FORWARD_SEARCH
                }
                SS = Si;                // reset target segment
                Sq = Si + 1;            // first forward_search candidate
                Sl = -1;                // reset last fitting candidate
                // resort segment list
                const int n = static_cast<int>(vecArcSegments->size());
                for(int i=0; i<(n/2); i++) {  
                  const int tmp = (*vecArcSegments)[i];
                  (*vecArcSegments)[i] = (*vecArcSegments)[n-1-i];
                  (*vecArcSegments)[n-1-i] = tmp;
                }
                continue;
              }
              
              if(!extra_up) {           // extra_up
                extra_up = true;        
                upward_search = true;   // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl - 1;
                }
                continue;
              }
            }
          }
        
          // STEP3: check interior angle
          const double tangent1 = m_vecLines[2][SS].tangent;
          const double tangent2 = m_vecLines[2][Sq].tangent;
          double interiorAngle = (tangent2-tangent1)*180/PI;
          if(interiorAngle < 0) interiorAngle = -interiorAngle;
          interiorAngle = 180 - interiorAngle;

          if((interiorAngle <= 180)&&(interiorAngle >= 135)) {
            double Xest, Yest, R2est;
            vecArcSegments->push_back(Sq);  // store Sq in linelist
            // estimate circle parameters
            if(!estimateCircle(*vecArcSegments, 2, Xest, Yest, R2est)) {
              vecArcSegments->pop_back();   // circle estimation failed
            } else {
              // STEP4: check tangents
              const double deltaX1 = Xest - m_vecLines[2][SS].mid.x;
              const double deltaY1 = Yest - m_vecLines[2][SS].mid.y;
              double TetaEst1 = atan2(deltaX1, deltaY1);
              if((deltaX1 < 0)&&(deltaY1 >= 0)) TetaEst1 += PI;
              if((deltaX1 < 0)&&(deltaY1 < 0)) TetaEst1 += PI;
              
              const double deltaX2 = Xest - m_vecLines[2][Sq].mid.x;
              const double deltaY2 = Yest - m_vecLines[2][Sq].mid.y;
              double TetaEst2 = atan2(deltaX2, deltaY2);
              if((deltaX2 < 0)&&(deltaY2 >= 0)) TetaEst2 += PI;
              if((deltaX2 < 0)&&(deltaY2 < 0)) TetaEst2 += PI;
  
              if(((TetaEst1-tangent1)*180/PI > par.maxLineTangentError)
              ||((TetaEst1-tangent1)*180/PI < -par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI > par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI < -par.maxLineTangentError)) {
                // tangent estimation error exceeded maximum
                vecArcSegments->pop_back();
              } else {
                SS = Sq;    // set Sq as new target segment SS
                Sl = Sq;    // last fitting candidate
              }
            }
          }

          if(upward_search) {
            Sq--;
          } else {
            Sq++;
          }
        }

      // CHECK ARC EXTRACTION CONDITIONS
        const int nSegments = static_cast<int>(vecArcSegments->size());
        
        int x = 0;
        int y = 0;
        int r2 = 0;
        double Xest, Yest, R2est;
        if(estimateCircle(*vecArcSegments, 2, Xest, Yest, R2est)) {
          x = static_cast<int>(floor(Xest + 0.5));
          y = static_cast<int>(floor(Yest + 0.5));
          r2 = static_cast<int>(floor(R2est + 0.5));
        } else {
          r2 = 0;   // estimation failed
        }

        // radius must be greater than 10 pixel for extraction
        // at least 2 segments are neccessary for circle estimation
        if((r2 >= 100)&&(nSegments >= 2)) {
          
      // EXTRACT ARC
          // register grouped segments as extracted arc
          ellArcEntry newarc;
          // start/end points
          const int x1 = m_vecLines[2][(*vecArcSegments)[0]].start.x;
          const int y1 = m_vecLines[2][(*vecArcSegments)[0]].start.y;
          const int x2 = m_vecLines[2][(*vecArcSegments)[nSegments-1]].end.x;
          const int y2 = m_vecLines[2][(*vecArcSegments)[nSegments-1]].end.y;

          bool reverse = false;
          // check direction and group
          if(x <= x1) {
            newarc.group = 3; // right
            if(y1 > y2) {
              reverse = true;
            }
          } else {
            newarc.group = 7; // left
            if(y1 < y2) {
              reverse = true;
            }
          }
          
          if(reverse) {     // reverse idxlist
            int i;
            for(i=0; i<nSegments/2; i++) {
              int swap = (*vecArcSegments)[i];
              (*vecArcSegments)[i] = (*vecArcSegments)[nSegments-1-i];
              (*vecArcSegments)[nSegments-1-i] = swap;
            }
            // start/end points
            newarc.start.x = x2;
            newarc.start.y = y2;
            newarc.end.x = x1;
            newarc.end.y = y1;
          } else {
            newarc.start.x = x1;
            newarc.start.y = y1;
            newarc.end.x = x2;
            newarc.end.y = y2;
          }

          // first/last line vectors
          if(newarc.group == 3) {
            newarc.firstVec = m_vecLines[2][(*vecArcSegments)[0]].end 
                            - m_vecLines[2][(*vecArcSegments)[0]].start;
            newarc.lastVec = m_vecLines[2][(*vecArcSegments)[nSegments-1]].end 
                           - m_vecLines[2][(*vecArcSegments)[nSegments-1]].start;
          } else {
            newarc.firstVec = m_vecLines[2][(*vecArcSegments)[0]].start
                            - m_vecLines[2][(*vecArcSegments)[0]].end;
            newarc.lastVec = m_vecLines[2][(*vecArcSegments)[nSegments-1]].start
                           - m_vecLines[2][(*vecArcSegments)[nSegments-1]].end;
          }

          // midpoint
          newarc.mid.x = x;
          newarc.mid.y = y;
          // other values
          newarc.r2 = r2;
          newarc.used = 0;
          // line index list
          newarc.lineIdxList = vecArcSegments;
          
          // store data
          m_vecEllArcs[newarc.group].push_back(newarc);
          m_vecEllArcs[0].push_back(newarc);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecLines[2][(*vecArcSegments)[i]].used++;
          }
        } else {
          delete vecArcSegments;
        }

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecLines[2][Si].used == 0) break;
        }
      }
      while(Si < segmCount);    // extract next arc if possible
    }
  }
  
///////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup2and6Arcs() {
    
    // clear old arclist and reserve memory
    clearEllArcList(2);
    m_vecEllArcs[2].reserve(EllArcPreallocation);
    clearEllArcList(6);
    m_vecEllArcs[6].reserve(EllArcPreallocation);

    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecLines[3].size());
    
    if(segmCount > 0) {

      int Si = 0;
      int Sq, SS, Sl;

      do {
        // create new line grouping vector
        std::vector<int> *vecArcSegments = new std::vector<int>;

        bool forward_search = false;  // search direction along the arc (forward = left->right)
        bool upward_search = false;   // search direction in the window (upward = bottom->up)
        bool extra_down = false;      // extra search cycle to continue inverted direction segments
        bool extra_up = false;        // extra search cycle to continue inverted direction segments

        // STEP1
        vecArcSegments->push_back(Si);  // store Si in arc segment list (starting line)
        SS = Si;                        // set Si as new target arc segment SS for search
        Sq = Si + 1;                    // first candidate segment
        Sl = -1;                        // last fitting candidate
        
        while(true) {   // track lines (line order: left->right, top->down)
          // STEP2
          const lineEntry segSS = m_vecLines[3][SS];

          // transform coordinates (diagonal -> horizontal)
          const int SS_start_x = segSS.start.x + segSS.start.y;
          const int SS_start_y = m_iWidth - 1 - segSS.start.x + segSS.start.y;
          const int SS_end_x   = segSS.end.x + segSS.end.y;
          const int SS_end_y   = m_iWidth - 1 - segSS.end.x + segSS.end.y;
          
          int SSy, SSx;
          if(forward_search) {
            SSx = SS_end_x;
            SSy = SS_end_y;
          } else {
            SSx = SS_start_x;
            SSy = SS_start_y;
          }

          // calculate window
          const int GAPx_pos = par.maxLineGap*2 + 1;
          const int GAPx_neg = segSS.len/2;
          int GAPy_pos, GAPy_neg;
          if(upward_search) {
            GAPy_pos = par.maxLineGap;
            GAPy_neg = 1;
          } else {
            GAPy_pos = 1;
            GAPy_neg = par.maxLineGap;
          }
          const int minY = SSy - GAPy_pos;
          const int maxY = SSy + GAPy_neg;


          // search new candidate segment Sq
          while((0 <= Sq)&&(Sq < segmCount)) {

            const lineEntry segSq = m_vecLines[3][Sq];

            // transform coordinates (diagonal -> horizontal)
            const int Sq_start_x = segSq.start.x + segSq.start.y;
            const int Sq_start_y = m_iWidth - 1 - segSq.start.x + segSq.start.y;
            const int Sq_end_x   = segSq.end.x + segSq.end.y;
            const int Sq_end_y   = m_iWidth - 1 - segSq.end.x + segSq.end.y;

            // vertical window boundary reached ? (both points outside the window)
            if(((Sq_start_y < minY)||(Sq_start_y > maxY)) 
            &&((Sq_end_y < minY)||(Sq_end_y > maxY))) { 
              Sq = -1;   // no candidate segment found
              break;
            }

            // calculate distance of Sq
            int dx,dy;
            if(forward_search) {
              dx = Sq_start_x - SSx;
              dy = Sq_start_y - SSy;
            } else {
              dx = SSx - Sq_end_x;
              dy = SSy - Sq_end_y;
            }             

            // calculate relative candidate angle
            const double angle = segSq.tangent - segSS.tangent;

            // check negative conditions
            if((dx > GAPx_pos)||(dx < -GAPx_neg)      // dx too large
            ||(dy > GAPy_pos)||(dy < -GAPy_neg)       // dy too large
            ||((dx <= 1)&&((dy > 1)||(dy < -1)))      // parallel gap too large
            ||((dx > 1)&&((dy > dx/2)||(dy < -dx/2))) // parallel gap too large
            ||((forward_search)&&(upward_search)&&(angle < 0))  // tangent directions
            ||((forward_search)&&(!upward_search)&&(angle > 0))
            ||((!forward_search)&&(upward_search)&&(angle > 0))
            ||((!forward_search)&&(!upward_search)&&(angle < 0))) {

              if(upward_search) {
                Sq--;
              } else {
                Sq++;
              }
              continue;
            }
            
            break;
          }

          if((Sq == segmCount)||(Sq < 0)) {   // no candidate segment found

            // next search cycle (STAGE 1: backward_down/extra_up -> backward_up/extra_down
            //                    STAGE 2: forward_down/extra_up -> forward_up/extra_down 
            //                    STAGE 3: end)
            if(upward_search) {
              if(extra_up) {            // NEXT: backward_up/forward_up
                extra_up = false;
                SS = Si;                // reset target segment
                Sq = Si - 1;            // first upward_search candidate
                Sl = -1;                // reset last fitting candidate
                continue;
              }
              if(!extra_down) {         // NEXT: extra_down
                extra_down = true;
                upward_search = false;  // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl + 1;
                }
                continue;
              }
            } else {  // down
              
              if(extra_down) {          // NEXT: forward_down/end
                extra_down = false;
                if(forward_search) {    // set next search stage
                  break;  // END
                } else {
                  forward_search = true;  // START FORWARD_SEARCH
                }
                SS = Si;                // reset target segment
                Sq = Si + 1;            // first forward_search candidate
                Sl = -1;                // reset last fitting candidate
                // resort segment list
                const int n = static_cast<int>(vecArcSegments->size());
                for(int i=0; i<(n/2); i++) {  
                  const int tmp = (*vecArcSegments)[i];
                  (*vecArcSegments)[i] = (*vecArcSegments)[n-1-i];
                  (*vecArcSegments)[n-1-i] = tmp;
                }
                continue;
              }
              
              if(!extra_up) {           // extra_up
                extra_up = true;        
                upward_search = true;   // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl - 1;
                }
                continue;
              }
            }
          }

          // STEP3: check interior angle
          const double tangent1 = m_vecLines[3][SS].tangent;
          const double tangent2 = m_vecLines[3][Sq].tangent;
          double interiorAngle = (tangent2-tangent1)*180/PI;
          if(interiorAngle < 0) interiorAngle = -interiorAngle;
          interiorAngle = 180 - interiorAngle;

          if((interiorAngle <= 180)&&(interiorAngle >= 135)) {
            double Xest, Yest, R2est;
            vecArcSegments->push_back(Sq);  // store Sq in linelist
            // estimate circle parameters
            if(!estimateCircle(*vecArcSegments, 3, Xest, Yest, R2est)) {
              vecArcSegments->pop_back();   // circle estimation failed
            } else {
              // STEP4: check tangents
              const double deltaX1 = Xest - m_vecLines[3][SS].mid.x;
              const double deltaY1 = Yest - m_vecLines[3][SS].mid.y;
              double TetaEst1 = atan2(deltaX1, deltaY1);
              if((deltaY1 < 0)&&(deltaX1 >= 0)) TetaEst1 -= PI;
              if((deltaY1 < 0)&&(deltaX1 < 0)) TetaEst1 += PI;
  
              const double deltaX2 = Xest - m_vecLines[3][Sq].mid.x;
              const double deltaY2 = Yest - m_vecLines[3][Sq].mid.y;
              double TetaEst2 = atan2(deltaX2, deltaY2);
              if((deltaY2 < 0)&&(deltaX2 >= 0)) TetaEst2 -= PI;
              if((deltaY2 < 0)&&(deltaX2 < 0)) TetaEst2 += PI;
  
              if(((TetaEst1-tangent1)*180/PI > par.maxLineTangentError)
              ||((TetaEst1-tangent1)*180/PI < -par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI > par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI < -par.maxLineTangentError)) {
                // tangent estimation error exceeded maximum
                vecArcSegments->pop_back();
              } else {
                SS = Sq;    // set Sq as new target segment SS
                Sl = Sq;    // last fitting candidate
              }
            }
          }

          if(upward_search) {
            Sq--;
          } else {
            Sq++;
          }
        }


      // CHECK ARC EXTRACTION CONDITIONS
        const int nSegments = static_cast<int>(vecArcSegments->size());
        
        int x = 0;
        int y = 0;
        int r2 = 0;
        double Xest, Yest, R2est;
        if(estimateCircle(*vecArcSegments, 3, Xest, Yest, R2est)) {
          x = static_cast<int>(floor(Xest + 0.5));
          y = static_cast<int>(floor(Yest + 0.5));
          r2 = static_cast<int>(floor(R2est + 0.5));
        } else {
          r2 = 0;   // estimation failed
        }

        // radius must be greater than 10 pixel for extraction
        // at least 2 segments are neccessary for circle estimation
        if((r2 >= 100)&&(nSegments >= 2)) {

      // EXTRACT ARC
          // register grouped segments as extracted arc
          ellArcEntry newarc;
          // start/end points
          const int x1 = m_vecLines[3][(*vecArcSegments)[0]].start.x;
          const int y1 = m_vecLines[3][(*vecArcSegments)[0]].start.y;
          const int x2 = m_vecLines[3][(*vecArcSegments)[nSegments-1]].end.x;
          const int y2 = m_vecLines[3][(*vecArcSegments)[nSegments-1]].end.y;

          // transform coordinates (diagonal -> horizontal)
          const int X1 = x1 + y1;
          const int X2 = x2 + y2;
          const int Y1 = m_iWidth - 1 - x1 + y1;
          const int Y = m_iWidth - 1 - x + y;

          bool reverse = false;
          // check direction and group
          if(Y >= Y1) {
            newarc.group = 2; // top right
            if(X1 > X2) {
              reverse = true;
            }
          } else {
            newarc.group = 6; // bottom left
            if(X1 < X2) {
              reverse = true;
            }
          }
          
          if(reverse) {     // reverse idxlist
            int i;
            for(i=0; i<nSegments/2; i++) {
              int swap = (*vecArcSegments)[i];
              (*vecArcSegments)[i] = (*vecArcSegments)[nSegments-1-i];
              (*vecArcSegments)[nSegments-1-i] = swap;
            }
            // start/end points
            newarc.start.x = x2;
            newarc.start.y = y2;
            newarc.end.x = x1;
            newarc.end.y = y1;
          } else {
            newarc.start.x = x1;
            newarc.start.y = y1;
            newarc.end.x = x2;
            newarc.end.y = y2;
          }

          // first/last line vectors
          if(newarc.group == 2) {
            newarc.firstVec = m_vecLines[3][(*vecArcSegments)[0]].end 
                            - m_vecLines[3][(*vecArcSegments)[0]].start;
            newarc.lastVec = m_vecLines[3][(*vecArcSegments)[nSegments-1]].end 
                           - m_vecLines[3][(*vecArcSegments)[nSegments-1]].start;
          } else {
            newarc.firstVec = m_vecLines[3][(*vecArcSegments)[0]].start
                            - m_vecLines[3][(*vecArcSegments)[0]].end;
            newarc.lastVec = m_vecLines[3][(*vecArcSegments)[nSegments-1]].start
                           - m_vecLines[3][(*vecArcSegments)[nSegments-1]].end;
          }
          
          // midpoint
          newarc.mid.x = x;
          newarc.mid.y = y;
          // other values
          newarc.r2 = r2;
          newarc.used = 0;
          // line index list
          newarc.lineIdxList = vecArcSegments;
          
          // store data
          m_vecEllArcs[newarc.group].push_back(newarc);
          m_vecEllArcs[0].push_back(newarc);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecLines[3][(*vecArcSegments)[i]].used++;
          }
        } else {
          delete vecArcSegments;
        }

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecLines[3][Si].used == 0) break;
        }
      }
      while(Si < segmCount);    // extract next arc if possible
    }
  }

  
///////////////////////////////////////////////////////////////////////

  void fastEllipseExtraction::extractGroup4and8Arcs() {

    // clear old arclist and reserve memory
    clearEllArcList(4);
    m_vecEllArcs[4].reserve(EllArcPreallocation);
    clearEllArcList(8);
    m_vecEllArcs[8].reserve(EllArcPreallocation);

    // get parameters
    const parameters& par = getParameters();

    const int segmCount = static_cast<int>(m_vecLines[4].size());
    
    if(segmCount > 0) {

      int Si = 0;
      int Sq, SS, Sl;

      do {
        // create new line grouping vector
        std::vector<int> *vecArcSegments = new std::vector<int>;

        bool forward_search = false;  // search direction along the arc (forward = bottom->up)
        bool upward_search = false;   // search direction in the window (upward = left->right)
        bool extra_down = false;      // extra search cycle to continue inverted direction segments
        bool extra_up = false;        // extra search cycle to continue inverted direction segments

        // STEP1
        vecArcSegments->push_back(Si);  // store Si in arc segment list (starting line)
        SS = Si;                        // set Si as new target arc segment SS for search
        Sq = Si + 1;                    // first candidate segment
        Sl = -1;                        // last fitting candidate

        while(true) {   // track lines (line order: bottom->up, right->left)
          // STEP2
          const lineEntry segSS = m_vecLines[4][SS];

          // transform coordinates (diagonal -> horizontal)
          const int SS_start_x = segSS.start.x + segSS.start.y;
          const int SS_start_y = m_iWidth - 1 - segSS.start.x + segSS.start.y;
          const int SS_end_x   = segSS.end.x + segSS.end.y;
          const int SS_end_y   = m_iWidth - 1 - segSS.end.x + segSS.end.y;

          int SSy, SSx;
          if(forward_search) {
            SSx = SS_start_x;
            SSy = SS_start_y;
          } else {
            SSx = SS_end_x;
            SSy = SS_end_y;
          }
          
          // calculate window
          const int GAPy_pos = par.maxLineGap*2 + 1;
          const int GAPy_neg = segSS.len/2;
          int GAPx_pos, GAPx_neg;
          if(upward_search) {
            GAPx_pos = 1;
            GAPx_neg = par.maxLineGap;
          } else {
            GAPx_pos = par.maxLineGap;
            GAPx_neg = 1;
          }
          const int minX = SSx - GAPx_pos;      // grpIV: inverted search direction (compared to grpII)
          const int maxX = SSx + GAPx_neg;


          // search new candidate segment Sq
          while((0 <= Sq)&&(Sq < segmCount)) {

            const lineEntry segSq = m_vecLines[4][Sq];

            // transform coordinates (diagonal -> horizontal)
            const int Sq_start_x = segSq.start.x + segSq.start.y;
            const int Sq_start_y = m_iWidth - 1 - segSq.start.x + segSq.start.y;
            const int Sq_end_x   = segSq.end.x + segSq.end.y;
            const int Sq_end_y   = m_iWidth - 1 - segSq.end.x + segSq.end.y;


            // horizontal window boundary reached ? (both points outside the window)
            if(((Sq_end_x < minX)||(Sq_end_x > maxX)) 
            &&((Sq_start_x < minX)||(Sq_start_x > maxX))) { 
              Sq = -1;   // no candidate segment found
              break;
            }

            // calculate distance of Sq
            int dx,dy;
            if(forward_search) {
              dx = SSx - Sq_end_x;
              dy = SSy - Sq_end_y;
            } else {
              dx = Sq_start_x - SSx;
              dy = Sq_start_y - SSy;
            }

            // calculate relative candidate angle
            const double angle = segSq.tangent - segSS.tangent;

            // check negative conditions
            if((dy > GAPy_pos)||(dy < -GAPy_neg)      // dy too large
            ||(dx > GAPx_pos)||(dx < -GAPx_neg)       // dx too large
            ||((dy <= 1)&&((dx > 1)||(dx < -1)))      // parallel gap too large
            ||((dy > 1)&&((dx > dy/2)||(dx > dy/2)))  // parallel gap too large
            ||((forward_search)&&(upward_search)&&(angle > 0))  // tangent directions
            ||((forward_search)&&(!upward_search)&&(angle < 0))
            ||((!forward_search)&&(upward_search)&&(angle < 0))
            ||((!forward_search)&&(!upward_search)&&(angle > 0))) {

              if(upward_search) {
                Sq--;
              } else {
                Sq++;
              }
              continue;
            }
            
            break;
          }

          if((Sq == segmCount)||(Sq < 0)) {   // no candidate segment found

            // next search cycle (STAGE 1: backward_down/extra_up -> backward_up/extra_down
            //                    STAGE 2: forward_down/extra_up -> forward_up/extra_down 
            //                    STAGE 3: end)
            if(upward_search) {
              if(extra_up) {            // NEXT: backward_up/forward_up
                extra_up = false;
                SS = Si;                // reset target segment
                Sq = Si - 1;            // first upward_search candidate
                Sl = -1;                // reset last fitting candidate
                continue;
              }
              if(!extra_down) {         // NEXT: extra_down
                extra_down = true;
                upward_search = false;  // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl + 1;
                }
                continue;
              }
            } else {  // down
              
              if(extra_down) {          // NEXT: forward_down/end
                extra_down = false;
                if(forward_search) {    // set next search stage
                  break;  // END
                } else {
                  forward_search = true;  // START FORWARD_SEARCH
                }
                SS = Si;                // reset target segment
                Sq = Si + 1;            // first forward_search candidate
                Sl = -1;                // reset last fitting candidate
                // resort segment list
                const int n = static_cast<int>(vecArcSegments->size());
                for(int i=0; i<(n/2); i++) {  
                  const int tmp = (*vecArcSegments)[i];
                  (*vecArcSegments)[i] = (*vecArcSegments)[n-1-i];
                  (*vecArcSegments)[n-1-i] = tmp;
                }
                continue;
              }
              
              if(!extra_up) {           // extra_up
                extra_up = true;        
                upward_search = true;   // invert search direction
                if(Sl != -1) {
                  SS = Sl;              // if possible continue with last fitting candidate Sl
                  Sq = Sl - 1;
                }
                continue;
              }
            }
          }

          // STEP3: check interior angle
          const double tangent1 = m_vecLines[4][SS].tangent;
          const double tangent2 = m_vecLines[4][Sq].tangent;
          double interiorAngle = (tangent2-tangent1)*180/PI;
          if(interiorAngle < 0) interiorAngle = -interiorAngle;
          interiorAngle = 180 - interiorAngle;

          if((interiorAngle <= 180)&&(interiorAngle >= 135)) {
            double Xest, Yest, R2est;
            vecArcSegments->push_back(Sq);  // store Sq in linelist
            // estimate circle parameters
            if(!estimateCircle(*vecArcSegments, 4, Xest, Yest, R2est)) {
              vecArcSegments->pop_back();   // circle estimation failed
            } else {
              // STEP4: check tangents
              const double deltaX1 = Xest - m_vecLines[4][SS].mid.x;
              const double deltaY1 = Yest - m_vecLines[4][SS].mid.y;
              double TetaEst1 = atan2(deltaX1, deltaY1);
              if((deltaX1 < 0)&&(deltaY1 >= 0)) TetaEst1 += PI;
              if((deltaX1 < 0)&&(deltaY1 < 0)) TetaEst1 += PI;
  
              const double deltaX2 = Xest - m_vecLines[4][Sq].mid.x;
              const double deltaY2 = Yest - m_vecLines[4][Sq].mid.y;
              double TetaEst2 = atan2(deltaX2, deltaY2);
              if((deltaX2 < 0)&&(deltaY2 >= 0)) TetaEst2 += PI;
              if((deltaX2 < 0)&&(deltaY2 < 0)) TetaEst2 += PI;
  
              if(((TetaEst1-tangent1)*180/PI > par.maxLineTangentError)
              ||((TetaEst1-tangent1)*180/PI < -par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI > par.maxLineTangentError)
              ||((TetaEst2-tangent2)*180/PI < -par.maxLineTangentError)) {
                // tangent estimation error exceeded maximum
                vecArcSegments->pop_back();
              } else {
                SS = Sq;    // set Sq as new target segment SS
                Sl = Sq;    // last fitting candidate
              }
            }
          }

          if(upward_search) {
            Sq--;
          } else {
            Sq++;
          }
        }


      // CHECK ARC EXTRACTION CONDITIONS
        const int nSegments = static_cast<int>(vecArcSegments->size());
        
        int x = 0;
        int y = 0;
        int r2 = 0;
        double Xest, Yest, R2est;
        if(estimateCircle(*vecArcSegments, 4, Xest, Yest, R2est)) {
          x = static_cast<int>(floor(Xest + 0.5));
          y = static_cast<int>(floor(Yest + 0.5));
          r2 = static_cast<int>(floor(R2est + 0.5));
        } else {
          r2 = 0;   // estimation failed
        }

        // radius must be greater than 10 pixel for extraction
        // at least 2 segments are neccessary for circle estimation
        if((r2 >= 100)&&(nSegments >= 2)) {

      // EXTRACT ARC
          // register grouped segments as extracted arc
          ellArcEntry newarc;
          // start/end points

          // TODO: fix wrong segment order
          for(int j=0; j<nSegments/2; j++) {
            int swap = (*vecArcSegments)[j];
            (*vecArcSegments)[j] = (*vecArcSegments)[nSegments-1-j];
            (*vecArcSegments)[nSegments-1-j] = swap;
          }

          const int x1 = m_vecLines[4][(*vecArcSegments)[0]].start.x;
          const int y1 = m_vecLines[4][(*vecArcSegments)[0]].start.y;
          const int x2 = m_vecLines[4][(*vecArcSegments)[nSegments-1]].end.x;
          const int y2 = m_vecLines[4][(*vecArcSegments)[nSegments-1]].end.y;

          // transform coordinates (diagonal -> horizontal)
          const int X1 = x1 + y1;
          const int Y1 = m_iWidth - 1 - x1 + y1;
          const int Y2 = m_iWidth - 1 - x2 + y2;
          const int X = x + y;

          bool reverse = false;
          // check direction and group
          if(X <= X1) {
            newarc.group = 4; // bottom right
            if(Y1 > Y2) {
              reverse = true;
            }
          } else {
            newarc.group = 8; // top left
            if(Y1 < Y2) {
              reverse = true;
            }
          }
          
          if(reverse) {     // reverse idxlist
            int i;
            for(i=0; i<nSegments/2; i++) {
              int swap = (*vecArcSegments)[i];
              (*vecArcSegments)[i] = (*vecArcSegments)[nSegments-1-i];
              (*vecArcSegments)[nSegments-1-i] = swap;
            }
            // start/end points
            newarc.start.x = x2;
            newarc.start.y = y2;
            newarc.end.x = x1;
            newarc.end.y = y1;
          } else {
            newarc.start.x = x1;
            newarc.start.y = y1;
            newarc.end.x = x2;
            newarc.end.y = y2;
          }

          // first/last line vectors
          if(newarc.group == 4) {
            newarc.firstVec = m_vecLines[4][(*vecArcSegments)[0]].end 
                            - m_vecLines[4][(*vecArcSegments)[0]].start;
            newarc.lastVec = m_vecLines[4][(*vecArcSegments)[nSegments-1]].end 
                           - m_vecLines[4][(*vecArcSegments)[nSegments-1]].start;
          } else {
            newarc.firstVec = m_vecLines[4][(*vecArcSegments)[0]].start
                            - m_vecLines[4][(*vecArcSegments)[0]].end;
            newarc.lastVec = m_vecLines[4][(*vecArcSegments)[nSegments-1]].start
                           - m_vecLines[4][(*vecArcSegments)[nSegments-1]].end;
          }
          
          // midpoint
          newarc.mid.x = x;
          newarc.mid.y = y;
          // other values
          newarc.r2 = r2;
          newarc.used = 0;
          // line index list
          newarc.lineIdxList = vecArcSegments;
          
          // store data
          m_vecEllArcs[newarc.group].push_back(newarc);
          m_vecEllArcs[0].push_back(newarc);

          // increase used-value of the grouped segments
          for(int i=0; i<nSegments; i++) {
            m_vecLines[4][(*vecArcSegments)[i]].used++;
          }
        } else {
          delete vecArcSegments;
        }

        // set next non-used segment as new starting segment Si
        while(++Si < segmCount) {
          if(m_vecLines[4][Si].used == 0) break;
        }
      }
      while(Si < segmCount);    // extract next arc if possible
    }
  }






/////////////////////////////////////////////////////////////////////

void fastEllipseExtraction::extractEllExtArcs(const int grp) {

  clearEllExtArcList(grp);

  // get parameters
  const parameters& par = getParameters();

  // extended group classification for ellipses (XeCG = eXtended elliptic Circle Groups)
  const int XeCG[9] = { 0, 1,3,2,4, 1,3,2,4 };
  const int XXXeEG[9][3] = {{0,0,0}, {8,1,2},{1,2,3},{2,3,4},{3,4,5}, {4,5,6},{5,6,7},{6,7,8},{7,8,1}};

  const int grpA = XXXeEG[grp][0];
  const int grpB = XXXeEG[grp][1];
  const int grpC = XXXeEG[grp][2];
  const std::vector<lineEntry> &linesA = getLineList(XeCG[grpA]);
  const std::vector<lineEntry> &linesC = getLineList(XeCG[grpC]);
  const std::vector<ellArcEntry> &arcsA = m_vecEllArcs[grpA];
  const std::vector<ellArcEntry> &arcsB = m_vecEllArcs[grpB];
  const std::vector<ellArcEntry> &arcsC = m_vecEllArcs[grpC];
  const int nArcsA = arcsA.size();
  const int nArcsB = arcsB.size();
  const int nArcsC = arcsC.size();

  /////////////////////
  /////// ARC B ///////
  /////////////////////
  int b;
  for(b=0; b<nArcsB; b++)
  {
    // new 2nd arc (B)
    std::vector<int> listA;
    std::vector<int> listC;
    
    /////////////////////
    /////// ARC A ///////
    /////////////////////
    int a;
    for(a=0; a<nArcsA; a++)
    {
      //////////////////
      // check window
      const int Xdiff = arcsB[b].start.x - arcsA[a].end.x;
      const int Ydiff = arcsB[b].start.y - arcsA[a].end.y;
      if((Xdiff > par.maxArcGap)||(Xdiff < -par.maxArcGap)
       ||(Ydiff > par.maxArcGap)||(Ydiff < -par.maxArcGap)) {
        continue;
      }

      // calculate startpoint distance ratio
      const int ABx = arcsB[b].start.x - arcsA[a].start.x;
      const int ABy = arcsB[b].start.y - arcsA[a].start.y;
      const double ABlen = sqrt(static_cast<double>(ABx*ABx + ABy*ABy));
      const int Ax = arcsA[a].end.x - arcsA[a].start.x;
      const int Ay = arcsA[a].end.y - arcsA[a].start.y;
      const double Alen = sqrt(static_cast<double>(Ax*Ax + Ay*Ay));
      const double ratio = ABlen / Alen;
      if(ratio < par.minArcDistanceRatio) {
        continue;
      }
      
      const double phi = acos(static_cast<double>(Ax*ABx + Ay*ABy)/sqrt(static_cast<double>((Ax*Ax+Ay*Ay)*(ABx*ABx+ABy*ABy))))*180/PI;
      if(phi > par.maxArcDistanceAngle) {
        continue;
      }

      if((Xdiff > par.minGAPangleDistance)||(Xdiff < -par.minGAPangleDistance)
       ||(Ydiff > par.minGAPangleDistance)||(Ydiff < -par.minGAPangleDistance)) {
        //////////////////////////
        // check GAP angles
        const int LAx = arcsA[a].lastVec.x;
        const int LAy = arcsA[a].lastVec.y;
        const int LBx = arcsB[b].firstVec.x;
        const int LBy = arcsB[b].firstVec.y;
        const double GAPangleA = acos(static_cast<double>(LAx*Xdiff + LAy*Ydiff)/sqrt(static_cast<double>((LAx*LAx+LAy*LAy)*(Xdiff*Xdiff+Ydiff*Ydiff))))*180/PI;
        const double GAPangleB = acos(static_cast<double>(LBx*Xdiff + LBy*Ydiff)/sqrt(static_cast<double>((LBx*LBx+LBy*LBy)*(Xdiff*Xdiff+Ydiff*Ydiff))))*180/PI;

        if((GAPangleA > par.maxGAPangle)||(GAPangleB > par.maxGAPangle)) {
          if((GAPangleA > 180-par.maxGAPangle)||(GAPangleB > 180-par.maxGAPangle)) {
            const int dx = arcsA[a].end.x - arcsB[b].mid.x;
            const int dy = arcsA[a].end.y - arcsB[b].mid.y;
            const double d = sqrt(dx*dx + dy*dy) - sqrt(arcsB[b].r2);
            if((d > par.maxArcOverlapGap)||(d < -par.maxArcOverlapGap)) {
              continue;
            }
          } else {
            continue;
          }
        }
      }
      ///////////////////////////////
      // check interior angles (A,B)
      if(checkInteriorAngles(a, grpA, b, grpB) <= par.maxIntAngleMismatches) {
        listA.push_back(a);
      }
    }//arcA

    const int sizeA = listA.size();
    if(sizeA == 0) continue;  // check next B arc

    /////////////////////
    /////// ARC C ///////
    /////////////////////
    int c;
    for(c=0; c<nArcsC; c++)
    {
      //////////////////
      // check window
      int Xdiff = arcsC[c].start.x - arcsB[b].end.x;
      int Ydiff = arcsC[c].start.y - arcsB[b].end.y;
      if((Xdiff > par.maxArcGap)||(Xdiff < -par.maxArcGap)
       ||(Ydiff > par.maxArcGap)||(Ydiff < -par.maxArcGap)) {
        continue;
      }

      // calculate startpoint distance ratio
      const int BCx = arcsC[c].start.x - arcsB[b].start.x;
      const int BCy = arcsC[c].start.y - arcsB[b].start.y;
      const double BClen = sqrt(static_cast<double>(BCx*BCx + BCy*BCy));
      const int Bx = arcsB[b].end.x - arcsB[b].start.x;
      const int By = arcsB[b].end.y - arcsB[b].start.y;
      const double Blen = sqrt(static_cast<double>(Bx*Bx + By*By));
      const double ratio = BClen / Blen;
      if(ratio < par.minArcDistanceRatio) {
        continue;
      }
      const double phi = acos(static_cast<double>(Bx*BCx + By*BCy)/sqrt(static_cast<double>((Bx*Bx+By*By)*(BCx*BCx+BCy*BCy))))*180/PI;
      if(phi > par.maxArcDistanceAngle) {
        continue;
      }

      if((Xdiff > par.minGAPangleDistance)||(Xdiff < -par.minGAPangleDistance)
       ||(Ydiff > par.minGAPangleDistance)||(Ydiff < -par.minGAPangleDistance)) {
        //////////////////////////
        // check GAP angles
        const int LBx = arcsB[b].lastVec.x;
        const int LBy = arcsB[b].lastVec.y;
        const int LCx = arcsC[c].firstVec.x;
        const int LCy = arcsC[c].firstVec.y;
        const double GAPangleB = acos(static_cast<double>(LBx*Xdiff + LBy*Ydiff)/sqrt(static_cast<double>((LBx*LBx+LBy*LBy)*(Xdiff*Xdiff+Ydiff*Ydiff))))*180/PI;
        const double GAPangleC = acos(static_cast<double>(LCx*Xdiff + LCy*Ydiff)/sqrt(static_cast<double>((LCx*LCx+LCy*LCy)*(Xdiff*Xdiff+Ydiff*Ydiff))))*180/PI;

        if((GAPangleB > par.maxGAPangle)||(GAPangleC > par.maxGAPangle)) {
          if((GAPangleB > 180-par.maxGAPangle)||(GAPangleC > 180-par.maxGAPangle)) {
            const int dx = arcsC[c].start.x - arcsB[b].mid.x;
            const int dy = arcsC[c].start.y - arcsB[b].mid.y;
            const double d = sqrt(dx*dx + dy*dy) - sqrt(arcsB[b].r2);
            if((d > par.maxArcOverlapGap)||(d < -par.maxArcOverlapGap)) {
              continue;
            }
          } else {
            continue;
          }
        }
      }
      ///////////////////////////////
      // check interior angles (B,C)
      if(checkInteriorAngles(b, grpB, c, grpC) <= par.maxIntAngleMismatches) {
        listC.push_back(c);
      }
    }//arcC

    const int sizeC = listC.size();
    if(sizeC == 0) continue;  // check next B arc

    int i;
    for(i=0; i<sizeA; i++) {
      int a = listA[i];

      int j;
      for(j=0; j<sizeC; j++) {
        int c = listC[j];

        ////////////////////////////////
        // check interior angles (A,C)      
        if(checkInteriorAngles(a, grpA, c, grpC) <= par.maxIntAngleMismatches) {

          /////////////////////
          // estimate ellipse
          double Xest, Yest, Aest, Best, Test;
          if(!estimateEllipse(arcsA[a], arcsB[b], arcsC[c], Xest, Yest, Aest, Best, Test)) {
            continue;
          }

          // prepare ellExtArc
          ellExtArcEntry entry;
          entry.used = 0;
          entry.group = grpB;
          entry.arcIdx[0] = a;
          entry.arcIdx[1] = b;
          entry.arcIdx[2] = c;
          entry.x = Xest;
          entry.y = Yest;
          entry.a = Aest;
          entry.b = Best;
          entry.t = Test;

          ///////////////////
          // check tangents
          const int mismatches = checkEllExtArcTangents(entry, Xest, Yest, Aest, Best, Test);
          if(mismatches <= par.maxTangentErrors) {
          
            ////////////////////////////////////////
            // calculate line beam center distance
            double distance;
            const std::vector<int> &LidxA = *arcsA[a].lineIdxList;
            const std::vector<int> &LidxC = *arcsC[c].lineIdxList;
            const int nC = LidxC.size();
            if((lineBeamCenterDistance(linesA[LidxA[0]], linesC[LidxC[nC-1]], entry, distance, Xest, Yest)
            &&(distance < par.maxLBcenterMismatch)
            &&(distance > -par.maxLBcenterMismatch))
            ||(par.ellExtArcExtractionStage < 3)) {
              // store ellExtArc
              m_vecEllExtArcs[grpB].push_back(entry);
              m_vecEllExtArcs[0].push_back(entry);
            }

          }//tangent test successful
        }//angle test successful
      }//arcC* (j)
    }//arcA* (i)
  }//arcB
}


///////////////////////////////////////////////////////////////////////

void fastEllipseExtraction::extractEllipses() {

  clearEllipseList();

  // get parameters
  const parameters& par = getParameters();

  const int XXXeEG[9][3] = {{0,0,0}, {8,1,2},{1,2,3},{2,3,4},{3,4,5}, {4,5,6},{5,6,7},{6,7,8},{7,8,1}};
  std::vector<ellExtArcEntry> &extarcs = m_vecEllExtArcs[0];
  const int nExtArcs = extarcs.size();
  int i, j;

  //////////////////////////////////////////
  // try merging of overlapping extarcs

  for(i=0; i<nExtArcs; i++)
  {
    if(extarcs[i].used > 0) continue;
    
    // new estimations only once for each group
    int grpEstimations[9] = {0};
    
    // prepare ellipse entry
    ellipseEntry entry;
    entry.x = extarcs[i].x;
    entry.y = extarcs[i].y;
    entry.a = extarcs[i].a;
    entry.b = extarcs[i].b;
    entry.t = extarcs[i].t;
    entry.coverage = 0;
    entry.mergedArcs = new std::vector<int>;

    std::vector<int> &mergeList = (*entry.mergedArcs);
    mergeList.push_back(i);
    extarcs[i].used++;

    // step through actual mergeList
    for(j=0; j<static_cast<int>(mergeList.size()); j++) {

      const int targetIdx = mergeList[j];
      const int targetGrp = extarcs[targetIdx].group;
      
      // step through the three base arcs
      int k;
      for(k=0; k<3; k++) {
        const int targetArcIdx = extarcs[targetIdx].arcIdx[k];
        const int targetArcGrp = XXXeEG[targetGrp][k];

        // search other extArcs
        int o;
        for(o=0; o<nExtArcs; o++) {
          if(o == i) continue;
          const int candGrp = extarcs[o].group;

          // compare base arcs
          int l;
          for(l=0; l<3; l++) {
            const int candArcIdx = extarcs[o].arcIdx[l];
            const int candArcGrp = XXXeEG[candGrp][l];
            
            if((candArcGrp == targetArcGrp)&&(candArcIdx == targetArcIdx)) {
              // MATCH
              if(extarcs[o].used == 0) {

                // insert candArc temporarily in mergeList
                mergeList.push_back(o);
                bool keep = false;

                // estimate new ellipse for first new group arcs
                bool estimationOK = true;
                bool newEstimation = false;
                double Xest, Yest, Aest, Best, Test;

                if(grpEstimations[candGrp] < 1) {
                  estimationOK = estimateEllipse(entry.mergedArcs, Xest, Yest, Aest, Best, Test);
                  grpEstimations[candGrp]++;
                  newEstimation = true;
                } else {
                  Xest = entry.x;
                  Yest = entry.y;
                  Aest = entry.a;
                  Best = entry.b;
                  Test = entry.t;
                }

                if(estimationOK) {
                  const int tangentErrors = checkEllExtArcTangents(extarcs[o], Xest, Yest, Aest, Best, Test);
                  const double pointMismatch = ellipsePointMismatch(extarcs[o], Xest, Yest, Aest, Best, Test);
                  const double centerDistance = linePointDistance(Xest, Yest, extarcs[o].ix, extarcs[o].iy, extarcs[o].slopeLB);
                  
                  if((tangentErrors <= par.maxTangentErrors)
                  &&(pointMismatch < par.maxExtArcMismatch)
                  &&(centerDistance < par.maxLBcenterMismatch)
                  &&(centerDistance > -par.maxLBcenterMismatch)) {
                    keep = true;
                    extarcs[o].used++;
                  } else {
                    // something is wrong with this arc - ignore it in next passes
                    extarcs[o].used++;
                  }
                }//estimation
                
                if(keep) {
                  if(newEstimation) {
                    entry.x = Xest;
                    entry.y = Yest;
                    entry.a = Aest;
                    entry.b = Best;
                    entry.t = Test;
                  }
                } else {  // !keep
                  mergeList.pop_back();
                  if(newEstimation) {
                    grpEstimations[candGrp]--;
                  }
                }
                
              }//used==0
              break;
            }// MATCH
          }//l (candBaseArcs)
        }//o (candExtArcs)
      }//k (targetBaseArcs)
    }//j (mergedExtArcs)


    const int nMergedArcs = mergeList.size();
    if(nMergedArcs > 1) {
      // estimate new ellipse
      double Xest, Yest, Aest, Best, Test;
      if(estimateEllipse(entry.mergedArcs, Xest, Yest, Aest, Best, Test)) {
        entry.x = Xest;
        entry.y = Yest;
        entry.a = Aest;
        entry.b = Best;
        entry.t = Test;
        calculateEllipseCoverage(entry);
      }
    }

    if(entry.coverage > par.minCoverage) {
      // store ellipse
      m_vecEllipses.push_back(entry);
    } else {
      // reset used count
      for(int m=0; m<nMergedArcs; m++) {
        extarcs[mergeList[m]].used = 0;
      }
      delete entry.mergedArcs;
    }

  }//i


  ///////////////////////////////////////
  // ellipse search for unused extarc
  
  std::vector<ellipseEntry> &ellipses = m_vecEllipses;
  const int nCurrentEllipses = ellipses.size();
  for(i=0; i<nCurrentEllipses; i++) { // target index

    ellipseEntry &ellipse = m_vecEllipses[i];
    
    for(j=0; j<nExtArcs; j++) {     // candidate index

      ellExtArcEntry &candArc = m_vecEllExtArcs[0][j];
      if(candArc.used > 0) continue;

      // check extarc/ellipse matching
      const int tangentErrors = checkEllExtArcTangents(candArc, ellipse.x, ellipse.y, ellipse.a, ellipse.b, ellipse.t);
      const double pointMismatch = ellipsePointMismatch(candArc, ellipse.x, ellipse.y, ellipse.a, ellipse.b, ellipse.t);
      //const double centerDistance = linePointDistance(ellipse.x, ellipse.y, candArc.ix, candArc.iy, candArc.slopeLB);
      
      if((tangentErrors <= par.maxTangentErrors)
      &&(pointMismatch < par.maxExtArcMismatch)) {
      //&&(centerDistance < par.maxLBcenterMismatch)
      //&&(centerDistance > -par.maxLBcenterMismatch)) {
        // estimate new ellipse
        (*ellipse.mergedArcs).push_back(j);
        double Xest, Yest, Aest, Best, Test;
        if(estimateEllipse(ellipse.mergedArcs, Xest, Yest, Aest, Best, Test)) {
          ellipse.x = Xest;
          ellipse.y = Yest;
          ellipse.a = Aest;
          ellipse.b = Best;
          ellipse.t = Test;
          calculateEllipseCoverage(ellipse);
          candArc.used++;
        }
      }
    
    }//j (candArc)
  }//i (targetEllipse)



  /////////////////////////////////////////
  // try cross merging of unused extarcs

  for(i=0; i<nExtArcs; i++) {     // target index

    const ellExtArcEntry &targetArc = m_vecEllExtArcs[0][i];
    if(targetArc.used > 0) continue;
    
    // prepare ellipse entry
    ellipseEntry entry;
    entry.x = targetArc.x;
    entry.y = targetArc.y;
    entry.a = targetArc.a;
    entry.b = targetArc.b;
    entry.t = targetArc.t;
    entry.coverage = 0;
    entry.mergedArcs = new std::vector<int>;
    std::vector<int> &mergeList = (*entry.mergedArcs);
    mergeList.push_back(i);
    
    for(j=0; j<nExtArcs; j++) {     // candidate index
      if(i==j) continue;
      const ellExtArcEntry &candArc = m_vecEllExtArcs[0][j];
      if(candArc.used > 0) continue;

      // compare parameters
      const double diffX = targetArc.x - candArc.x;
      const double diffY = targetArc.y - candArc.y;
      double ratioA, ratioB;
      if(targetArc.a > candArc.a) {
        ratioA = candArc.a / targetArc.a;
      } else {
        ratioA = targetArc.a / candArc.a;
      }

      if(targetArc.b > candArc.b) {
        ratioB = candArc.b / targetArc.b;
      } else {
        ratioB = targetArc.b / candArc.b;
      }
      
      if(((diffX < par.maxCenterMismatch)&&(diffX > -par.maxCenterMismatch))
      &&((diffY < par.maxCenterMismatch)&&(diffY > -par.maxCenterMismatch))
      &&(ratioA > par.minRadiusMatchRatio)
      &&(ratioB > par.minRadiusMatchRatio)) {
        // MATCH
        mergeList.push_back(j);
      }
    }//j (candArc)

    const int nMergedArcs = mergeList.size();

    if(nMergedArcs > 1) {
      // estimate new ellipse
      double Xest, Yest, Aest, Best, Test;
      if(estimateEllipse(entry.mergedArcs, Xest, Yest, Aest, Best, Test)) {
        entry.x = Xest;
        entry.y = Yest;
        entry.a = Aest;
        entry.b = Best;
        entry.t = Test;
        calculateEllipseCoverage(entry);
      }
    } else {
      // sincle arc coverage
      calculateEllipseCoverage(entry);
    }

    if(entry.coverage > par.minCoverage) {
      // store ellipse
      m_vecEllipses.push_back(entry);
      // mark used extarcs
      for(int m=0; m<nMergedArcs; m++) {
        m_vecEllExtArcs[0][mergeList[m]].used++;
      }
    }
    
  }//i (targetArc)
}


///////////////////////////////////////////////////////////////////////

int fastEllipseExtraction::checkInteriorAngles(int a, int grpA, int b, int grpB) {

  // get parameters
  const parameters& par = getParameters();

  const int XeCG[9] = { 0, 1,3,2,4, 1,3,2,4 };

  const std::vector<lineEntry> &linesA = getLineList(XeCG[grpA]);
  const std::vector<int> &idxA = *m_vecEllArcs[grpA][a].lineIdxList;
  const int nIdxA = idxA.size();

  const std::vector<lineEntry> &linesB = getLineList(XeCG[grpB]);
  const std::vector<int> &idxB = *m_vecEllArcs[grpB][b].lineIdxList;
  const int nIdxB = idxB.size();

  ////////////////////////////////////////////
  // check interior angles between all lines
  int mismatches = 0;
  
  int iB;
  for(iB=0; iB<nIdxB; iB++) 
  {
    int iA;
    for(iA=0; iA<nIdxA; iA++)   // inner loop: first arc
    {
      // calculate line vectors
      int AstartX, AstartY, AendX, AendY;
      int BstartX, BstartY, BendX, BendY;
      if(grpA <= 4) {
        AstartX = linesA[idxA[iA]].start.x;
        AstartY = linesA[idxA[iA]].start.y;
        AendX = linesA[idxA[iA]].end.x;
        AendY = linesA[idxA[iA]].end.y;
      } else {
        AstartX = linesA[idxA[iA]].end.x;
        AstartY = linesA[idxA[iA]].end.y;
        AendX = linesA[idxA[iA]].start.x;
        AendY = linesA[idxA[iA]].start.y;
      }
      if(grpB <= 4) {
        BstartX = linesB[idxB[iB]].start.x;
        BstartY = linesB[idxB[iB]].start.y;
        BendX = linesB[idxB[iB]].end.x;
        BendY = linesB[idxB[iB]].end.y;
      } else {
        BstartX = linesB[idxB[iB]].end.x;
        BstartY = linesB[idxB[iB]].end.y;
        BendX = linesB[idxB[iB]].start.x;
        BendY = linesB[idxB[iB]].start.y;
      }

      // calculate vectors A and B
      const int Ax = AendX - AstartX;
      const int Ay = -(AendY - AstartY);  // invert y-axis (to normal cartesian)
      const int Bx = BendX - BstartX;
      const int By = -(BendY - BstartY);  // invert y-axis (to normal cartesian)

      // calculate normal vectors AN and BN
      const int ANx = Ay;
      const int ANy = -Ax;
      const int BNx = By;
      const int BNy = -Bx;

      // calculate vectors AB and BA
      const int ABx = BstartX - AstartX;
      const int ABy = -(BstartY - AstartY);  // invert y-axis (to normal cartesian)
      const int BAx = -ABx;
      const int BAy = -ABy;

      // calculate interior angles
      const double TH1 = acos((ABx*ANx + ABy*ANy) / sqrt(static_cast<double>((ABx*ABx + ABy*ABy) * (ANx*ANx + ANy*ANy))));
      const double TH2 = acos((BAx*BNx + BAy*BNy) / sqrt(static_cast<double>((BAx*BAx + BAy*BAy) * (BNx*BNx + BNy*BNy))));

      const double Alen = sqrt(static_cast<double>(Ax*Ax + Ay*Ay));
      const double ABlen = sqrt(static_cast<double>(ABx*ABx + ABy*ABy));
      const double ABdivA = ABlen/Alen;
      if(ABdivA < 1.0) {
        continue;
      }

      // check max value
      if((TH1 > PI/2) || (TH2 > PI/2)) {
        mismatches++;
      }

      if(mismatches > par.maxIntAngleMismatches) {
        break;
      }
    }//iA
    if(mismatches > par.maxIntAngleMismatches) {
      break;
    }
  }//iB
  
  return mismatches;
}


///////////////////////////////////////////////////////////////////////

double fastEllipseExtraction::lineBeamIsecCenterDistance(const ellExtArcEntry &extarcA,
                                                         const ellExtArcEntry &extarcB,
                                                         double Xest, double Yest) {
  const double x1 = extarcA.ix;
  const double x2 = extarcB.ix;
  const double y1 = extarcA.iy;
  const double y2 = extarcB.iy;
  const double m1 = extarcA.slopeLB;
  const double m2 = extarcB.slopeLB;
  const double det0 = -m1 + m2;
  const double det1 = -(y2-x2*m2) + y1-x1*m1;
  const double det2 = (y1-x1*m1)*m2 - m1*(y2-x2*m2);
            
  if(det0 == 0) return -1;  // parallel lines
  
  // intersection point of line beams
  const double ix = det1 / det0;
  const double iy = det2 / det0;
  
  // center distance
  return sqrt((ix-Xest)*(ix-Xest) + (iy-Yest)*(iy-Yest));
}


///////////////////////////////////////////////////////////////////////

double fastEllipseExtraction::linePointDistance(double testPtX, double testPtY, 
                                                double linePtX, double linePtY,
                                                double lineSlope) {
  const double C = linePtY - linePtX * lineSlope;
  double u = 1 / sqrt(lineSlope * lineSlope + 1);   // scale factor for HESSEsche normalform
  if(C > 0) u = -u;
  return (lineSlope * testPtX - testPtY + C) * u;
}


///////////////////////////////////////////////////////////////////////

double fastEllipseExtraction::ellipsePointMismatch(ellExtArcEntry &extarc,
                                                   double Xest, 
                                                   double Yest, 
                                                   double Aest, 
                                                   double Best, 
                                                   double Test) {
  
  const int XXXeEG[9][3] = {{0,0,0}, 
                            {8,1,2},
                            {1,2,3},
                            {2,3,4},
                            {3,4,5}, 
                            {4,5,6},
                            {5,6,7},
                            {6,7,8},
                            {7,8,1}};
  double extArcMismatch = 0;

  // correct orientation angle
  if(Test*180/PI < -89.5) {
    Test = -Test;
  }
  const double phi = PI/2 - Test;
  double x, y, X, Y, xterm, yterm;
  double cosa, sina;

  int a;
  for(a=0; a<3; a++) {
    const int idx = extarc.arcIdx[a];
    const int grp = XXXeEG[extarc.group][a];
    
    const ellArcEntry &arc = m_vecEllArcs[grp][idx];

    x = static_cast<double>(arc.start.x) - Xest;
    y = Yest - static_cast<double>(arc.start.y);
    // rotate coordinate system
    sincos(phi, sina, cosa);
    X =  x*cosa + y*sina;
    Y = -x*sina + y*cosa;
    // ellipse equation
    xterm = X / Aest;
    yterm = Y / Best;
    const double startMismatch = fabs(xterm*xterm + yterm*yterm - 1);
    if(startMismatch > extArcMismatch) {
      extArcMismatch = startMismatch;
    }

    x = static_cast<double>(arc.end.x) - Xest;
    y = Yest - static_cast<double>(arc.end.y);
    // rotate coordinate system
    X = x*cos(phi) + y*sin(phi);
    Y = -x*sin(phi) + y*cos(phi);
    // ellipse equation
    xterm = X / Aest;
    yterm = Y / Best;
    const double endMismatch = fabs(xterm*xterm + yterm*yterm - 1);
    if(endMismatch > extArcMismatch) {
      extArcMismatch = endMismatch;
    }
  }
  
  return extArcMismatch;
}


///////////////////////////////////////////////////////////////////////

int fastEllipseExtraction::checkEllExtArcTangents(const ellExtArcEntry &extarc,
                                                  double Xest,
                                                  double Yest,
                                                  double Aest,
                                                  double Best,
                                                  double Test) {
  // get parameters
  const parameters& par = getParameters();

  const int XeCG[9] = { 0, 1,3,2,4, 1,3,2,4 };
  const int XXXeEG[9][3] = {{0,0,0},
                            {8,1,2},
                            {1,2,3},
                            {2,3,4},
                            {3,4,5}, 
                            {4,5,6},
                            {5,6,7},
                            {6,7,8},
                            {7,8,1}};

  // correct orientation angle
  if(Test*180/PI < -89.5) {
    Test = -Test;
  }
  const double phi = PI/2 - Test;
  
  double ellTang;
  double lineTang;
  double tangErr;
  double sinp, cosp;
  int mismatches = 0;
  int k;
  int l;
  for(k=0; k<3; k++)  // check all lines of the three arcs
  {
    const int grp = XXXeEG[extarc.group][k];    // grpA, grpB, grpC
    
    // references to lists
    const std::vector<lineEntry> &lines = getLineList(XeCG[grp]);
    const std::vector<ellArcEntry> &arcs = m_vecEllArcs[grp];
    const std::vector<int> &Lidx = *arcs[extarc.arcIdx[k]].lineIdxList;
    const int nIdx = Lidx.size();

    for(l=0; l<nIdx; l++)
    {
      lineTang = lines[Lidx[l]].tangent;

      // wrong grpII tangent direction 
      //if(((grp == 3)||(grp == 7))&&(lineTang > PI/2)) lineTang -= PI;

      
      // rotate coordinate system
      const double x = static_cast<double>(lines[Lidx[l]].mid.x - Xest);
      const double y = static_cast<double>(Yest - lines[Lidx[l]].mid.y);

      sincos(phi, sinp, cosp);
      const double X =  x*cosp + y*sinp;
      const double Y = -x*sinp + y*cosp;
      
      const double BBX = Best*Best*X;
      const double AAY = Aest*Aest*Y;
      
      //  calculate ellipse tangent and avoid division by zero
      if      ((AAY == 0)&&(BBX >= 0)) {
        ellTang = -PI/2;
      } else if ((AAY == 0)&&(BBX < 0)) {
        ellTang = PI/2;
      } else {
        ellTang = atan(-BBX/AAY) + phi;
      }

      if(ellTang > PI/2) {
        ellTang -= PI;
      }

      // wrong grpII tangent direction 
      if(((grp == 3)||(grp == 7))&&(ellTang < 0)) ellTang += PI;

      tangErr = (ellTang-lineTang)*180/PI;
      
      if     ((tangErr > 178)&&(tangErr < 182)) {
        tangErr -=180;
      } else if((tangErr < -178)&&(tangErr > -182)) {
        tangErr +=180;
      }
      
      if((tangErr > par.maxArcTangentError)
        ||(tangErr < -par.maxArcTangentError)) {
        // tangent estimation error exceeded maximum
        if(par.ellExtArcExtractionStage > 1) {
          mismatches++;
        }

        if(mismatches > par.maxTangentErrors) {
          break;
        }
      }
    }//l
    if(mismatches > par.maxTangentErrors) {
      break;
    }
  }//k (basearcs)
  
  return mismatches;
} 


///////////////////////////////////////////////////////////////////////

bool fastEllipseExtraction::lineBeamCenterDistance(const lineEntry &firstLine,
                                                   const lineEntry &lastLine,
                                                   ellExtArcEntry &extarc,
                                                   double &distance, 
                                                   const double Xest, 
                                                   const double Yest) {
  const double x1 = firstLine.mid.x;
  const double y1 = firstLine.mid.y;
  const double x2 = lastLine.mid.x;
  const double y2 = lastLine.mid.y;
  const double dx1 = firstLine.end.x - firstLine.start.x;
  const double dy1 = firstLine.end.y - firstLine.start.y;
  const double dx2 = lastLine.end.x - lastLine.start.x;
  const double dy2 = lastLine.end.y - lastLine.start.y;

  double m1, m2;
  if((dx1 == 0)&&(dy1 > 0)) m1 = INFINITE_SLOPE;
  else if((dx1 == 0)&&(dy1 < 0)) m1 = -INFINITE_SLOPE;
  else m1 = dy1/dx1;

  if((dx2 == 0)&&(dy2 > 0)) m2 = INFINITE_SLOPE;
  else if((dx2 == 0)&&(dy2 < 0)) m2 = -INFINITE_SLOPE;
  else m2 = dy2/dx2;

  // calculate midpoint of endpoint connection
  const double cx = (x1 + x2) / 2;
  const double cy = (y1 + y2) / 2;
    
  // calculate intersection point
  const double det0 = -m1 + m2;
  const double det1 = -(y2-x2*m2) + y1-x1*m1;
  const double det2 = (y1-x1*m1)*m2 - m1*(y2-x2*m2);

  if(det0 == 0) {   // parallel lines
    return false;
  }
  
  const double ix = det1 / det0;
  const double iy = det2 / det0;

  // calculate line beam
  double m;
  const double dx = cx - ix;
  const double dy = cy - iy;
  if((dx == 0)&&(dy > 0)) m = INFINITE_SLOPE;
  else if((dx == 0)&&(dy < 0)) m = -INFINITE_SLOPE;
  else m = dy/dx;
  
  // calculate center distance
  const double C = iy - ix*m;
  double u = 1 / sqrt(m*m + 1);   // scale factor for HESSEsche normalform
  if(C > 0) u = -u;

  // results  
  extarc.ix = ix;
  extarc.iy = iy;
  extarc.slopeLB = m;
  distance = (m*Xest - Yest + C) * u;
  return true;
}


///////////////////////////////////////////////////////////////////////

void fastEllipseExtraction::calculateEllipseCoverage(ellipseEntry &ellipse)
{
  const int XeCG[9] = { 0, 1,3,2,4, 1,3,2,4 };
  const int XXXeEG[9][3] = {{0,0,0}, {8,1,2},{1,2,3},{2,3,4},{3,4,5}, {4,5,6},{5,6,7},{6,7,8},{7,8,1}};
  const std::vector<int> &mergeList = *ellipse.mergedArcs;
  const int nExtArcs = mergeList.size();
  
  int pixels = 0;

  // RESET OLD USED COUNT
    
  // iterate over all merged extarcs
  int e;
  for(e=0; e<nExtArcs; e++) {
    const ellExtArcEntry &extarc = m_vecEllExtArcs[0][mergeList[e]];
    
    // iterate over all arcs
    for(int a=0; a<3; a++) {
      const int arcGrp = XXXeEG[extarc.group][a];
      const ellArcEntry &arc = m_vecEllArcs[arcGrp][extarc.arcIdx[a]];
      const std::vector<int> &lineList = *arc.lineIdxList;
      const int nLines = lineList.size();
      std::vector<lineEntry> &lines = getLineList(XeCG[arcGrp]);
      
      // iterate over all lines
      for(int l=0; l<nLines; l++) {
        // Reset used-count of extracted lines
        lines[lineList[l]].used = 0;
      }//lines
    }//arcs
  }//extarcs


  // COUNT EXTRACTED EDGE PIXELS      

  // iterate over all merged extarcs
  for(e=0; e<nExtArcs; e++) {
    const ellExtArcEntry &extarc = m_vecEllExtArcs[0][mergeList[e]];
    
    // iterate over all arcs
    for(int a=0; a<3; a++) {
      const int arcGrp = XXXeEG[extarc.group][a];
      const int lineGrp = XeCG[arcGrp];
      const ellArcEntry &arc = m_vecEllArcs[arcGrp][extarc.arcIdx[a]];
      const std::vector<int> &lineList = *arc.lineIdxList;
      const int nLines = lineList.size();
      std::vector<lineEntry> &lines = getLineList(lineGrp);
      
      // iterate over all lines
      for(int l=0; l<nLines; l++) {
        // count edge pixels
        if(lines[lineList[l]].used == 0) {
          pixels += lines[lineList[l]].len;
          lines[lineList[l]].used++;
        }
      }//lines
    }//arcs
  }//extarcs
  
  // approximate ideal ellipse circumference
  const double circumference = PI * ( 1.5 * (ellipse.a + ellipse.b) - sqrt(ellipse.a * ellipse.b));
  
  //const double lambda = (ellipse.a - ellipse.b) / (ellipse.a + ellipse.b);
  //const double temp = (64 - 3*lambda*lambda*lambda*lambda) / (64 - 16*lambda*lambda);
  //const double temp = 1 + (3*lambda*lambda / (10 + sqrt(4- 3*lambda*lambda)));
  //const double circumference = PI * (ellipse.a + ellipse.b) * temp;

  ellipse.coverage = static_cast<double>(pixels) / circumference;
}


/////////////////////////////////////////////////////////////////////

bool fastEllipseExtraction::estimateEllipse(const std::vector<int> *mergedArcs, double &Xest, double &Yest, double &Aest, double &Best, double &Test)
  {
    const int XXXeEG[9][3] = {{0,0,0}, {8,1,2},{1,2,3},{2,3,4},{3,4,5}, {4,5,6},{5,6,7},{6,7,8},{7,8,1}};
    const int XeCG[9] = { 0, 1,3,2,4, 1,3,2,4 };

    std::vector<ellExtArcEntry> &extarcs = m_vecEllExtArcs[0];
    const int nMerged = mergedArcs->size();
    int m;
    
    // COUNT INPUT PIXELS
    int nPixels = 0;
    
    for(m=0; m<nMerged; m++) 
    {
      int k;
      for(k=0; k<3; k++) 
      {
        const int arcGrp = XXXeEG[extarcs[(*mergedArcs)[m]].group][k];
        const int index = extarcs[(*mergedArcs)[m]].arcIdx[k];
        const ellArcEntry &arc = m_vecEllArcs[arcGrp][index];

        const std::vector<lineEntry> &lines = getLineList(XeCG[arc.group]);
        const std::vector<int> &lineidx = *arc.lineIdxList;
        const int nLines = lineidx.size();

        int l;
        for(l=0; l<nLines; l++) 
        {
          // get number of line segments and segment group
          const int nSegments = (*lines[lineidx[l]].segmIdxList).size();
          const int group = lines[lineidx[l]].group;
      
          const std::vector<segmEntry>& segmList = getSegmentList(group);
    
          // accumulate all segments
          for(int i=0; i<nSegments; i++) {
            // segment index
            const int idx = (*lines[lineidx[l]].segmIdxList)[i];
        
            // segments length
            nPixels += segmList[idx].len;
          }//i
        }//l
      }//k
    }//m


  // FILL DESIGN MATRIX
    lti::matrix<double> Design(false, nPixels, 6);
    int row = 0;


    for(m=0; m<nMerged; m++) 
    {
      int k;
      for(k=0; k<3; k++) 
      {
        const int arcGrp = XXXeEG[extarcs[(*mergedArcs)[m]].group][k];
        const int index = extarcs[(*mergedArcs)[m]].arcIdx[k];
        const ellArcEntry &arc = m_vecEllArcs[arcGrp][index];

        const std::vector<lineEntry> &lines = getLineList(XeCG[arc.group]);
        const std::vector<int> &lineidx = *arc.lineIdxList;
        const int nLines = lineidx.size();

        int l;
        for(l=0; l<nLines; l++) 
        {
          // get number of line segments and segment group
          const int nSegments = (*lines[lineidx[l]].segmIdxList).size();
          const int group = lines[lineidx[l]].group;
      
          const std::vector<segmEntry>& segmList = getSegmentList(group);
    
          // accumulate all segments
          for(int i=0; i<nSegments; i++) {
            // segment index
            const int idx = (*lines[lineidx[l]].segmIdxList)[i];
        
            // starting point of segment[i]
            int x = segmList[idx].start.x;
            int y = segmList[idx].start.y;
          
            // segments length
            const int len = segmList[idx].len;
            
            // accumulate all segment pixels
            for(int j=0; j<len; j++) {
    
              // Now fill design matrix
              double data[6] = { x*x, x*y, y*y, x, y, 1.0 };
              lti::vector<double> line(6, data);
              Design.setRow(row, line);
           
              row++;
              
              // set next segment pixel coordinates
              // orientation:  ---x
              //               |
              //               y
              switch(group) {
                case 1: {   // horizontal
                  x++;
                  break;
                }
                case 2: {   // vertical
                  y++;
                  break;
                }
                case 3: {   // diagonal horizontal: \ (starts top left)
                  x++;
                  y++;
                  break;
                }
                case 4: {   // diagonal vertical: / (starts bottom left)
                  x++;
                  y--;
                  break;
                }
              }
            }// next pixel (j)
          }//i
        }//l
      }//k
    }//m

    return directEllipseFitting(Design, Xest, Yest, Aest, Best, Test);
  }


///////////////////////////////////////////////////////////////////////

  bool fastEllipseExtraction::estimateEllipse(const ellArcEntry &arcA, 
                                              const ellArcEntry &arcB, 
                                              const ellArcEntry &arcC, 
                                              double &Xest, double &Yest, double &Aest, double &Best, double &Test)
  {
    const int XeCG[9] = { 0, 1,3,2,4, 1,3,2,4 };
    
    // COUNT INPUT PIXELS
    int nPixels = 0;
    int l;

    // arcA
    const std::vector<lineEntry> &linesA = getLineList(XeCG[arcA.group]);
    const std::vector<int> &lineidxA = *arcA.lineIdxList;
  
    const int nLinesA = lineidxA.size();
    for(l=0; l<nLinesA; l++) 
    {
      // get number of line segments and segment group
      const int nSegments = (*linesA[lineidxA[l]].segmIdxList).size();
      const int group = linesA[lineidxA[l]].group;
  
      const std::vector<segmEntry>& segmList = getSegmentList(group);

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*linesA[lineidxA[l]].segmIdxList)[i];
    
        // segments length
        nPixels += segmList[idx].len;
      }
    }


    // arcB
    const std::vector<lineEntry> &linesB = getLineList(XeCG[arcB.group]);
    const std::vector<int> &lineidxB = *arcB.lineIdxList;
  
    const int nLinesB = lineidxB.size();
    for(l=0; l<nLinesB; l++) 
    {
      // get number of line segments and segment group
      const int nSegments = (*linesB[lineidxB[l]].segmIdxList).size();
      const int group = linesB[lineidxB[l]].group;
  
      const std::vector<segmEntry>& segmList = getSegmentList(group);

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*linesB[lineidxB[l]].segmIdxList)[i];
    
        // segments length
        nPixels += segmList[idx].len;
      }
    }


    // arcC
    const std::vector<lineEntry> &linesC = getLineList(XeCG[arcC.group]);
    const std::vector<int> &lineidxC = *arcC.lineIdxList;
  
    const int nLinesC = lineidxC.size();
    for(l=0; l<nLinesC; l++) 
    {
      // get number of line segments and segment group
      const int nSegments = (*linesC[lineidxC[l]].segmIdxList).size();
      const int group = linesC[lineidxC[l]].group;
  
      const std::vector<segmEntry>& segmList = getSegmentList(group);

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*linesC[lineidxC[l]].segmIdxList)[i];
    
        // segments length
        nPixels += segmList[idx].len;
      }
    }


  // FILL DESIGN MATRIX
    lti::matrix<double> Design(false, nPixels, 6);
    int row = 0;


    // arcA
    for(l=0; l<nLinesA; l++) 
    {
      // get number of line segments and segment group
      const int nSegments = (*linesA[lineidxA[l]].segmIdxList).size();
      const int group = linesA[lineidxA[l]].group;
  
      const std::vector<segmEntry>& segmList = getSegmentList(group);

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*linesA[lineidxA[l]].segmIdxList)[i];
        
        // starting point of segment[i]
        int x = segmList[idx].start.x;
        int y = segmList[idx].start.y;
      
        // segments length
        const int len = segmList[idx].len;
        
        // accumulate all segment pixels
        for(int j=0; j<len; j++) {

          // Now fill design matrix
          double data[6] = { x*x, x*y, y*y, x, y, 1.0 };
          lti::vector<double> line(6, data);
          Design.setRow(row, line);
       
          row++;
          
          // set next segment pixel coordinates
          // orientation:  ---x
          //               |
          //               y
          switch(group) {
            case 1: {   // horizontal
              x++;
              break;
            }
            case 2: {   // vertical
              y++;
              break;
            }
            case 3: {   // diagonal horizontal: \ (starts top left)
              x++;
              y++;
              break;
            }
            case 4: {   // diagonal vertical: / (starts bottom left)
              x++;
              y--;
              break;
            }
          }
        }// next pixel
      }
    }


    // arcB
    for(l=0; l<nLinesB; l++) 
    {
      // get number of line segments and segment group
      const int nSegments = (*linesB[lineidxB[l]].segmIdxList).size();
      const int group = linesB[lineidxB[l]].group;
  
      const std::vector<segmEntry>& segmList = getSegmentList(group);

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*linesB[lineidxB[l]].segmIdxList)[i];
        
        // starting point of segment[i]
        int x = segmList[idx].start.x;
        int y = segmList[idx].start.y;
      
        // segments length
        const int len = segmList[idx].len;
        
        // accumulate all segment pixels
        for(int j=0; j<len; j++) {

          // Now fill design matrix
          double data[6] = { x*x, x*y, y*y, x, y, 1.0 };
          lti::vector<double> line(6, data);
          Design.setRow(row, line);
       
          row++;
          
          // set next segment pixel coordinates
          // orientation:  ---x
          //               |
          //               y
          switch(group) {
            case 1: {   // horizontal
              x++;
              break;
            }
            case 2: {   // vertical
              y++;
              break;
            }
            case 3: {   // diagonal horizontal: \ (starts top left)
              x++;
              y++;
              break;
            }
            case 4: {   // diagonal vertical: / (starts bottom left)
              x++;
              y--;
              break;
            }
          }
        }// next pixel
      }
    }



    // arcC
    for(l=0; l<nLinesC; l++) 
    {
      // get number of line segments and segment group
      const int nSegments = (*linesC[lineidxC[l]].segmIdxList).size();
      const int group = linesC[lineidxC[l]].group;
  
      const std::vector<segmEntry>& segmList = getSegmentList(group);

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*linesC[lineidxC[l]].segmIdxList)[i];
        
        // starting point of segment[i]
        int x = segmList[idx].start.x;
        int y = segmList[idx].start.y;
      
        // segments length
        const int len = segmList[idx].len;
        
        // accumulate all segment pixels
        for(int j=0; j<len; j++) {

          // Now fill design matrix
          double data[6] = { x*x, x*y, y*y, x, y, 1.0 };
          lti::vector<double> line(6, data);
          Design.setRow(row, line);
       
          row++;
          
          // set next segment pixel coordinates
          // orientation:  ---x
          //               |
          //               y
          switch(group) {
            case 1: {   // horizontal
              x++;
              break;
            }
            case 2: {   // vertical
              y++;
              break;
            }
            case 3: {   // diagonal horizontal: \ (starts top left)
              x++;
              y++;
              break;
            }
            case 4: {   // diagonal vertical: / (starts bottom left)
              x++;
              y--;
              break;
            }
          }
        }// next pixel
      }
    }


    return directEllipseFitting(Design, Xest, Yest, Aest, Best, Test);
  }


///////////////////////////////////////////////////////////////////////

  bool fastEllipseExtraction::directEllipseFitting(const lti::matrix<double> &Design, 
                                                   double &Xest, double &Yest, double &Aest, double &Best, double &Test)
  {
    //printLtiMatrix(Design, "Design matrix:");

    // Create 6x6 constraint matrix (applet version is *(-1) ?)
    lti::matrix<double> CO(6, 6, 0.0);
    CO.at(0,2) = -2;
    CO.at(1,1) = 1;
    CO.at(2,0) = -2;
    //printLtiMatrix(CO, "Constraint matrix:");


    // Create transposed design matrix Dt
    lti::matrix<double> Dt(false, Design.columns(), Design.rows());
    Dt.transpose(Design);
    
    // Create scatter matrix  S
    lti::matrix<double> S(Dt);
    S.multiply(Design);
    //printLtiMatrix(S, "Scatter matrix:");

    // Cholesky decomposition (lti version rounds values ?)
    lti::choleskyDecomposition<double> colesDcFunctor;
    if(!colesDcFunctor.apply(S)) {
      return false;
    }
    S.transpose();  // modified dc in java applet ?
    //printLtiMatrix(S, "Cholesky decomposed scatter matrix (transposed):");

    // Create inverted cholesky decomposed scatter matrix Si
    lti::matrix<double> Si(S);
    lti::matrixInversion<double> invFunctor;
    if(!invFunctor.apply(Si)) {
      return false;
    }
    //printLtiMatrix(Si, "Inverted cholesky decomposed scatter matrix:");

    // Create transposed inverted scatter matrix Sit
    lti::matrix<double> Sit(false, Si.columns(), Si.rows());
    Sit.transpose(Si);
    ////printLtiMatrix(Sit, "Transposed cholesky decomposed inverted scatter matrix:");

    // Multiply CO and Sit
    lti::matrix<double> COSit(CO);
    COSit.multiply(Sit);
    //printLtiMatrix(COSit, "Temporary matrix:");

    // Multiply Si and COSit
    lti::matrix<double> SiCOSit(Si);
    SiCOSit.multiply(COSit);
    //printLtiMatrix(SiCOSit, "System matrix:");


    // Solve eigensystem
    lti::jacobi<double> jacobiFunctor;
    lti::vector<double> EVals(6);
    lti::matrix<double> EVecs(false, 6, 6);
    jacobiFunctor.apply(SiCOSit, EVals, EVecs);

    //printLtiVector(EVals, "Eigenvalues");
    //printLtiMatrix(EVecs, "Eigenvectors:");


    // Multiply Sit and EVecs
    lti::matrix<double> Sol(Sit);
    Sol.multiply(EVecs);
    //printLtiMatrix(Sol, "Unnormalized solution:");
    
    // Normalization
    for(int j=0; j<6; j++) {  // scan columns
      int i;
      double mod = 0.0;
      for(i=0; i<6; i++) {
        mod += Sol[i][j] * Sol[i][j];
      }
      for(i=0; i<6; i++) {
        Sol[i][j] /= sqrt(mod); 
      }
    }
    //printLtiMatrix(Sol, "Normalized solution:");


    // Find negative non-zero EVal
    int index = -1;
    const double zero = 10e-20;   // ?
    for(int i=0; i<6; i++) {
      //double x = EVals[i];
      if(EVals[i] < 0) {
        if(-EVals[i] > zero) {
          index = i;
        }
      }
    }
    if(index == -1) {
      return false;
    }

    // Fetch the right solution
    lti::vector<double> solvEVec(6);
    Sol.getColumnCopy(index, solvEVec);

    //printLtiVector(solvEVec, "The solving Eigenvector:");



    // Calculate ellipse parameters
    const double A = solvEVec[0];
    const double B = solvEVec[1]/2;
    const double C = solvEVec[2];
    const double D = solvEVec[3]/2;
    const double E = solvEVec[4]/2;
    const double F = solvEVec[5];

    // Invarianten der Kegelschnittgleichung (Bronstein S.213)
    const double delta = A*C - B*B;
    const double DELTA = A*C*F + B*E*D + D*B*E - A*E*E - B*B*F - D*C*D;

    if(delta == 0) {
      return false;   // division by zero
    }

    // Ellipsen-Mittelpunkt und Orientierung
    Xest = (B*E - C*D) / delta;
    Yest = (B*D - A*E) / delta;
    Test = atan2(2*B, A-C)/2;
    //Test = PI/2 - atan2(2*B, A-C)/2;
    
    // Koeffizienten der Bronstein-Normalform nach Transformation (Bronstein S.212)
    const double sqrtResult = sqrt((A-C)*(A-C) + 4*B*B);
    const double a = (A+C + sqrtResult) / 2;
    const double c = (A+C - sqrtResult) / 2;

    if((a == 0)||(c == 0)) {
      return false;   // division by zero
    }

    // Umrechnung Bronstein-Normalform in Ellipsen-Normalform
    const double Ddivd = DELTA / delta;
    const double a2 = -Ddivd / a;
    const double b2 = -Ddivd / c;

    if((a2 < 0)||(b2 < 0)) {
      return false;   // negative sqrt argument
    }

    //Aest = sqrt(a2);
    //Best = sqrt(b2);
    Aest = sqrt(b2);
    Best = sqrt(a2);
    
    return true;
  }


///////////////////////////////////////////////////////////////////////

  bool fastEllipseExtraction::directEllipseCoefficientFitting(const lti::matrix<double> &Design, 
                                                              double &A, double &B, double &C, double &D, double &E, double &F)
  {
    //printLtiMatrix(Design, "Design matrix:");

    // Create 6x6 constraint matrix (applet version is *(-1) ?)
    lti::matrix<double> CO(6, 6, 0.0);
    CO.at(0,2) = -2;
    CO.at(1,1) = 1;
    CO.at(2,0) = -2;
    //printLtiMatrix(CO, "Constraint matrix:");


    // Create transposed design matrix Dt
    lti::matrix<double> Dt(false, Design.columns(), Design.rows());
    Dt.transpose(Design);
    
    // Create scatter matrix  S
    lti::matrix<double> S(Dt);
    S.multiply(Design);
    //printLtiMatrix(S, "Scatter matrix:");

    // Cholesky decomposition (lti version rounds values ?)
    lti::choleskyDecomposition<double> colesDcFunctor;
    if(!colesDcFunctor.apply(S)) {
      return false;
    }
    S.transpose();  // modified dc in java applet ?
    //printLtiMatrix(S, "Cholesky decomposed scatter matrix (transposed):");

    // Create inverted cholesky decomposed scatter matrix Si
    lti::matrix<double> Si(S);
    lti::matrixInversion<double> invFunctor;
    if(!invFunctor.apply(Si)) {
      return false;
    }
    //printLtiMatrix(Si, "Inverted cholesky decomposed scatter matrix:");

    // Create transposed inverted scatter matrix Sit
    lti::matrix<double> Sit(false, Si.columns(), Si.rows());
    Sit.transpose(Si);
    ////printLtiMatrix(Sit, "Transposed cholesky decomposed inverted scatter matrix:");

    // Multiply CO and Sit
    lti::matrix<double> COSit(CO);
    COSit.multiply(Sit);
    //printLtiMatrix(COSit, "Temporary matrix:");

    // Multiply Si and COSit
    lti::matrix<double> SiCOSit(Si);
    SiCOSit.multiply(COSit);
    //printLtiMatrix(SiCOSit, "System matrix:");


    // Solve eigensystem
    lti::jacobi<double> jacobiFunctor;
    lti::vector<double> EVals(6);
    lti::matrix<double> EVecs(false, 6, 6);
    jacobiFunctor.apply(SiCOSit, EVals, EVecs);

    //printLtiVector(EVals, "Eigenvalues");
    //printLtiMatrix(EVecs, "Eigenvectors:");


    // Multiply Sit and EVecs
    lti::matrix<double> Sol(Sit);
    Sol.multiply(EVecs);
    //printLtiMatrix(Sol, "Unnormalized solution:");
    
    // Normalization
    for(int j=0; j<6; j++) {  // scan columns
      int i;
      double mod = 0.0;
      for(i=0; i<6; i++) {
        mod += Sol[i][j] * Sol[i][j];
      }
      for(i=0; i<6; i++) {
        Sol[i][j] /= sqrt(mod); 
      }
    }
    //printLtiMatrix(Sol, "Normalized solution:");


    // Find negative non-zero EVal
    int index = -1;
    const double zero = 10e-20;   // ?
    for(int i=0; i<6; i++) {
      //double x = EVals[i];
      if(EVals[i] < 0) {
        if(-EVals[i] > zero) {
          index = i;
        }
      }
    }
    if(index == -1) {
      return false;
    }

    // Fetch the right solution
    lti::vector<double> solvEVec(6);
    Sol.getColumnCopy(index, solvEVec);

    //printLtiVector(solvEVec, "The solving Eigenvector:");



    // Return coefficients
    A = solvEVec[0];
    B = solvEVec[1]/2;
    C = solvEVec[2];
    D = solvEVec[3]/2;
    E = solvEVec[4]/2;
    F = solvEVec[5];

    return true;
  }


/////////////////////////////////////////////////////////////////////
  
  bool fastEllipseExtraction::estimateCircle(std::vector<int> &idxlist, const int group,
                                             double &Xest, double &Yest, double &R2est) {
    lti::int64 N = 0;
    lti::int64 x_sum = 0;
    lti::int64 y_sum = 0; 
    lti::int64 x2_sum = 0;
    lti::int64 y2_sum = 0; 
    lti::int64 xy_sum = 0;
    lti::int64 x3_sum = 0;
    lti::int64 y3_sum = 0;
    lti::int64 xy2_sum = 0;
    lti::int64 x2y_sum = 0;
  
    const int nLines = static_cast<int>(idxlist.size());
  
    // accumulate all lines
    for(int l=0; l<nLines; l++) {
      // get number of line segments and segment group
      const int nSegments = static_cast<int>((*m_vecLines[group][idxlist[l]].segmIdxList).size());

      // accumulate all segments
      for(int i=0; i<nSegments; i++) {
        // segment index
        const int idx = (*m_vecLines[group][idxlist[l]].segmIdxList)[i];
        
        // starting point of segment[i]
        int x = m_vecSegments[group][idx].start.x;
        int y = m_vecSegments[group][idx].start.y;
      
        // segments length
        const int len = m_vecSegments[group][idx].len;
        
        // accumulate all segment pixels
        for(int j=0; j<len; j++) {
          // calculate sums
          x_sum += x;
          y_sum += y;
          x2_sum += x*x;
          y2_sum += y*y;
          xy_sum += x*y;
          x3_sum += x*x*x;
          y3_sum += y*y*y;
          xy2_sum += x*y*y;
          x2y_sum += x*x*y;
          
          // set next segment pixel coordinates
          // orientation:  ---x
          //               |
          //               y
          switch(group) {
            case 1: {   // horizontal
              x++;
              break;
            }
            case 2: {   // vertical
              y++;
              break;
            }
            case 3: {   // diagonal horizontal: \ (starts top left)
              x++;
              y++;
              break;
            }
            case 4: {   // diagonal vertical: / (starts bottom left)
              x++;
              y--;
              break;
            }
          }
          N++;  // increase number of all pixels
        
        }// next pixel
      }// next segment
    }// next line
  
    const double  a1 = static_cast<double>(2*(x_sum * x_sum - N * x2_sum));
    const double  b1 = static_cast<double>(2*(x_sum * y_sum - N * xy_sum));
    const double  a2 = b1;
    const double  b2 = static_cast<double>(2*(y_sum * y_sum - N * y2_sum));
    const double  c1 = static_cast<double>(x2_sum * x_sum - N * x3_sum + x_sum * y2_sum - N * xy2_sum);
    const double  c2 = static_cast<double>(x2_sum * y_sum - N * y3_sum + y_sum * y2_sum - N * x2y_sum);
      
    // calculate circle parameters
    if((a1*b2 - a2*b1) != 0) {      // avoid division by zero
      Xest = (c1*b2 - c2*b1)/(a1*b2 - a2*b1);
      Yest = (a1*c2 - a2*c1)/(a1*b2 - a2*b1);
      R2est = (double)(x2_sum - 2*Xest*x_sum + N*Xest*Xest + y2_sum - 2*Yest*y_sum + N*Yest*Yest)/N;
      return true;
    }
    
    return false;   // division by zero
  }



  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On place apply for type channel8!
  bool fastEllipseExtraction::apply(const channel8& srcdest) {

    // extract line segments
    detectGroup1Segments(srcdest);
    detectGroup2Segments(srcdest);
    detectGroup3Segments(srcdest);
    detectGroup4Segments(srcdest);
    
    // extract lines
    extractGroup1Lines();
    extractGroup2Lines();
    extractGroup3Lines();
    extractGroup4Lines();

    // extract elliptic arcs
    extractGroup1and5Arcs();
    extractGroup2and6Arcs();
    extractGroup3and7Arcs();
    extractGroup4and8Arcs();

    // extract elliptic extended arcs
    clearEllExtArcList(0);
    for(int g=1; g<=8; g++) {
      extractEllExtArcs(g);
    }

    // extract ellipses
    extractEllipses();

    return true;
  };


  // On place apply for type channel!
  bool fastEllipseExtraction::apply(const channel& srcdest) {
  
    lti::channel8 m_temp;
    m_temp.castFrom(srcdest, false, false);   // channel -> channel8
    apply(m_temp);
    return true;
  };


}

