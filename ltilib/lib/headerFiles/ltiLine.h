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
 * file .......: ltiLine.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 07.02.2003
 * revisions ..: $Id: ltiLine.h,v 1.6 2003/06/06 09:09:20 goenner Exp $
 */

#ifndef LTI_LINE_H
#define LTI_LINE_H

#include "ltiConfig.h"

#include <iostream>
#include <limits>

#include "ltiIoHandler.h"
#include "ltiMath.h"
#include "ltiPoint.h"
#include "ltiRectangle.h"

namespace lti {


  /**
   * Type for computations with lines.
   *
   * A line (or more generally a tline<T>) is represented by a start point
   * and an end point.
   *
   * The type T correspond to the coordinate type used in both points.
   *
   * This class stores only the two points, and provides some functionality
   * using them.  Other operations can be achieved more efficiently if more
   * information about the line, like its slope, is also stored.  This is 
   * done by some derived classes.
   *
   * @ingroup gGeomData
   */
  template <class T>
  class tline {
  protected:
    /**
     * start point
     */
    tpoint<T> start;
    
    /**
     * end point
     */
    tpoint<T> end;

  public:
    /**
     * default constructor.
     *
     * Both points are left uninitialized (this can save some time)
     */
    explicit tline();
    
    /**
     * constructor with both points
     */
    tline(const tpoint<T>& theStart, const tpoint<T>& theEnd);
    
    /**
     * copy constructor
     */
    template <class U> 
    tline(const tline<U>& other) 
      : start(castFrom(other.start)),end(castFrom(other.end)) {
    };

    /**
     * cast operator
     */
    template <class U>
    tline<T>& castFrom(const tline<U>& other) {
      start.castFrom(other.start);
      end.castFrom(other.end);
      return (*this);
    };

    /**
     * general operator to set both points of the line
     */
    inline void set(const tpoint<T>& theStart,const tpoint<T>& theEnd);

    /**
     * set the start point. 
     */
    inline void setStart(const tpoint<T>& theStart);

    /**
     * set the end point. Does not compute the slope.
     */
    inline void setEnd(const tpoint<T>& theEnd);

    /**
     * exchange the start and end points, making the previous end a
     * start point and the previous start the end point.
     */
    inline void invert();
    
    /**
     * return a read only reference to the start point
     */
    inline const tpoint<T>& getStart() const;

    /**
     * return a read only reference to the end point
     */
    inline const tpoint<T>& getEnd() const;

    /**
     * @name Distance computation
     */
    //@{
    /**
     * calculate minimal euclidian distance of the line segment to the point c.
     *
     * This method is slower than the sqrDistanceTo, which avoids the
     * computation of a (in many cases not required) square root.
     *
     * @see sqrDistanceTo() distanceToXPol()
     */
    inline T distanceTo(const tpoint<T>& c) const;

    /**
     * Calculate minimal %square of euclidian distance to the point c. This
     * method is faster than distanceTo (because it does not calculate
     * the square root).
     *
     * @param c point to which the minimal distance is searched.
     * @return the square of the minimal distance to c
     */
    inline T distanceSqr(const tpoint<T>& c) const;

    /**
     * Calculate minimal %square of euclidian distance to the point c. This
     * method is faster than distanceTo (because it does not calculate
     * the square root).
     *
     * @param c point to which the minimal distance is searched.
     * @param p point in the line segment with the minimal distance to c.
     *
     * @return the square of the minimal distance to c
     */
    T distanceSqr(const tpoint<T>& c,tpoint<T>& p) const;

    /**
     * Calculate distance to the point c to the infinite line (eXtraPolated)
     * containing this line segment.
     *
     * @return the minimal distance to c
     */
    inline T distanceToXPol(const tpoint<T>& c) const;

    /**
     * Calculate %square of distance to the point c to the infinite
     * line (eXtraPolated) containing this line segment.
     *
     * @param c point to which the minimal distance is searched.
     *
     * @return the square of the minimal distance to c
     * @see sqrDistanceTo()
     *
     * This method is faster than distanceToXPol (because it does not
     * calculate the square root).
     */
    inline T distanceSqrXPol(const tpoint<T>& c) const;

    /**
     * Calculate %square of distance to the point c to the infinite
     * line (eXtraPolated) containing this line segment.
     *
     * @param c point to which the minimal distance is searched.
     * @param p point in the extrapolated line segment with the
     *          minimal distance to c.
     *
     * This method is faster than distanceToXPol (because it does not
     * calculate the square root).
     *
     *
     * @see sqrDistanceTo()
     */
    T distanceSqrXPol(const tpoint<T>& c,tpoint<T>& p) const;

    /** 
     * square of the length of this line
     */
    inline T sqrLength() const;
    //@}
  
    /**
     * @name Intersections
     */
    //@{ 

    /**
     * Check if this line segment intersects the \a other given one.
     *
     * @param other the other line segment to which an intersection is
     *              going to be checked.
     * @return true if both line segments intersect.
     */
    bool doesIntersect(const tline<T>& other) const;

    /**
     * Check if this line segment is parallel to the \a other given one.
     *
     * @param other the other line segment to which parallelism is
     *              going to be checked.
     * @return true if both line segments are parallel.
     */
    bool isParallel(const tline<T>& other) const;  

    /**
     * Check if this line segment is parallel and colinear to the
     * \a other given one.
     *
     * @param other the other line segment to which parallelism is
     *              going to be checked.
     * @return true if both line segments are parallel.
     */
    bool isColinear(const tline<T>& other) const;    

    /**
     * Compute the part of this line segment which lies within the
     * given rectangle, and leave the result here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    bool intersect(const trectangle<T>& rect);

    /**
     * Compute the part of the \a other line segment which lies within the
     * given rectangle, and leave the result here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    inline bool intersect(const tline<T>& other,
			  const trectangle<T>& rect);

    /**
     * Compute the intersection point of this line segment with the 
     * \a other given one.
     *
     * @param other the other line segment to which the intersection point
     *              is going to be computed.
     * @param p     if there is an intersection between both line segments
     *              the intersection point will be written here.
     * @param colinear this parameter is set to true in case both line segments
     *              are parallel and co-linear.
     *
     * @return true if an unique intersection point exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c true and determines the
     *              intersection if the intersection is inifinitely small.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    template<class U>
    bool getIntersectionPoint(const tline<T>& other, tpoint<U>& p,
			      bool& colinear ) const
    {
      // due to the template argument tpoint<U>, this method must be here
      // otherwise MS Visual C++ won't compile this file!
      colinear = false;
    
      // NOTE:
      // This method requires following time constants: (out of date -> ToDo)
      // Additions/LogicOp : 13
      // Multiplications   : 8 
      // Divisions         : 2    
      // Comparisons       : 6~   
      
      const tpoint<T> d1(end-start);
      const tpoint<T> d2(other.end-other.start);
      const tpoint<T> d0(other.start-start);
      
      // determinant
      const T det = d1.y*d2.x - d1.x*d2.y;
      
      // auxiliar variables containing information of the intersection point
      const T t1 = d2.x*d0.y - d2.y*d0.x;
      const T t2 = d1.x*d0.y - d1.y*d0.x;
 
      // just one copy for the whole history of this class (faster!)
      static const T epsilon = std::numeric_limits<T>::epsilon();
      
      // the numerical tolerance for parallelism is limited by epsilon.
      if (det > epsilon) {
	//check overlap
	if ( (t1 < 0) || (det < t1) || (t2 < 0) || (det < t2) )
	  return false;
	
	// compute the intersection point
	const double f = static_cast<double>(t1)/static_cast<double>(det);
	condRoundCastTo(start.x + f*d1.x, p.x );
	condRoundCastTo(start.y + f*d1.y, p.y );
	return true;
	
      } else if (det < -epsilon) {
	if ( (t1 > 0) || (det > t1) || (t2 > 0) || (det > t2) )
	  return false;
	
	// compute the intersection point
	const double f = static_cast<double>(t1)/static_cast<double>(det);
	condRoundCastTo(start.x + f*d1.x,p.x );
	condRoundCastTo(start.y + f*d1.y,p.y );
	return true;
	
      } else {
	// lines are parallel, we just need to check, if the distance between
	// them is zero (in which case they may overlap in more than one point)
	colinear   = (abs(t1) <= epsilon) && (abs(t2) <= epsilon);
	
	if (colinear) {
	  // lines are colinear, but do they overlap? in latter case they
	  // intersect! also check if they overlap is at a single point
	  if ( end.x <= other.start.x + epsilon &&
	       end.x >= other.start.x - epsilon ) {
	    p = end;
	    return true;
	  }
	  else if ( end.x <= other.end.x + epsilon &&
		    end.x >= other.end.x - epsilon ) {
	    p = end;
	    return true;
	  }
	  else if ( start.x <= other.start.x + epsilon &&
		    start.x >= other.start.x - epsilon ) {
	    p = start;
	    return true;
	  }
	  else if ( start.x <= other.end.x + epsilon &&
		    start.x >= other.end.x - epsilon ) {
	    p = start;
	    return true;
	  }
	}
      }
      return false;
    };

    /**
     * Compute the common line segment between this line segment and the 
     * \a other given one and leave the result here. This intersection is only
     * going to be computed if both lines are colinear.
     *
     * @param other the other line segment to which the intersection
     *              is going to be computed.
     *
     * @return true if an common line segment exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c true and determines the
     *              line segment even if it is inifinitely small, i.e. a point.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    bool getCommonLine(const tline<T>& other);
    
    /**
     * Compute the common line segment between the given line
     * segments.  This intersection is only going to be computed if
     * both lines are colinear.
     *
     * @param first first line segment.
     * @param second second line segment.
     *
     * @return true if a common line segment exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c true and determines the
     *              line segment even if it is inifinitely small, i.e. a point.
     *
     * The common line segment will be left in this instance.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations. 
     */
    inline bool getCommonLine(const tline<T>& first, const tline<T>& second);

    /**
     * Check if this infinitely extrapolated line intersects the \a other given
     * infinite line at a single finite point.
     *
     * @param other the other line segment to which an intersection is
     *              going to be checked.
     *
     * @return true if both inifinite lines intersect at a single finite point.
     *            
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    bool doesPointIntersectXPol(const tline<T>& other) const;
    //----------------
    /**
     * Compute the intersection point of this infinitely extrapolated line with the 
     * \a other given infinite line.
     *
     * @param other the other line segment to which the intersection point
     *              is going to be computed.
     * @param p     if there is an intersection between both line segments
     *              or between their respective infinite line extrapolations,
     *              the intersection point will be written here.
     * @param onThisLine if the intersection occurs at a point on the line
     *              segment, this parameter will be set to true.  Otherwise
     *              false.
     * @param onOtherLine if the intersection occurs at a point on the other
     *              line segment, this parameter will be set to true.
     * @param colinear this parameter is set to true in case both line segments
     *              are parallel and co-linear.
     *
     * @return true if an unique intersection point exists. It returns 
     *              \c false otherwise. If both line segments are parallel and
     *              colinear, this method returns \c false.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    template<class U>
    bool getIntersectionPointXPol(const tline<T>& other, tpoint<U>& p,
				  bool& onThisLine, bool& onOtherLine,
				  bool& colinear) const {

      // due to the template argument tpoint<U>, this method must be here
      // otherwise MS Visual C++ won't compile this file!
      colinear = false; 
    
      // NOTE:
      // This method requires following time constants:
      // Additions/LogicOp : 13
      // Multiplications   : 8 
      // Divisions         : 2    
      // Comparisons       : 6~   
      
      const tpoint<T> d1(end-start);
      const tpoint<T> d2(other.end-other.start);
      const tpoint<T> d0(other.start-start);
      
      // determinant
      const T det = d1.y*d2.x - d1.x*d2.y;
      
      // auxiliar variables containing information of the intersection point
      const T t1 = d2.x*d0.y - d2.y*d0.x;
      const T t2 = d1.x*d0.y - d1.y*d0.x;
      
      // just one copy for the whole history of this class (faster!)
      static const T epsilon = std::numeric_limits<T>::epsilon();
      
      // the numerical tolerance for parallelism is limited by epsilon.
      if (det > epsilon) {
	// compute the intersection point on the extrapolated lines
	const double f = static_cast<double>(t1)/static_cast<double>(det);
	condRoundCastTo(start.x + f*d1.x,p.x );
	condRoundCastTo(start.y + f*d1.y,p.y );
	
	onThisLine  = ((t1>=0) && (det >= t1));
	onOtherLine = ((t2>=0) && (det >= t2));
	
	return true;
      } else if (det < -epsilon) {
	// compute the intersection point on the extrapolated lines
	const double f = static_cast<double>(t1)/static_cast<double>(det);
	condRoundCastTo(start.x + f*d1.x,p.x );
	condRoundCastTo(start.y + f*d1.y,p.y );
	
	onThisLine  = ((t1<=0) && (det <= t1));
	onOtherLine = ((t2<=0) && (det <= t2));
	
	return true;
      } else {
	// lines are parallel, we just need to check, if the distance between
	// them is zero (in which case they may overlap in more than one point)
	colinear = (abs(t1) <= epsilon) && (abs(t2) <= epsilon);
	
	if (colinear) {
	  // lines are colinear, but do they overlap? in latter case they
	  // intersect! But we only return true, if a single inersection point exists
	  if (d1.x > epsilon) {	  
	    onThisLine = onOtherLine
	      = ((start.x <= other.start.x) && (end.x >= other.start.x)) ||
	      ((start.x <= other.end.x)   && (end.x >= other.end.x));
	  } else if (d1.x < -epsilon) {
	    onThisLine = onOtherLine
	      = ((end.x <= other.start.x) && (start.x >= other.start.x)) ||
	      ((end.x <= other.end.x)   && (start.x >= other.end.x));
	  } else if (d1.y > epsilon) {
	    onThisLine = onOtherLine
	      = ((start.y <= other.start.y) && (end.y >= other.start.y)) ||
	      ((start.y <= other.end.y)   && (end.y >= other.end.y));
	  } else {
	    onThisLine = onOtherLine
	      = ((end.y <= other.start.y) && (start.y >= other.start.y)) ||
	      ((end.y <= other.end.y)   && (start.y >= other.end.y));
	  }
	}
      }
      
      return false;
    };

    /**
     * Compute the intersection point of this infinitely extrapolated line with the 
     * \a other given infinite line.
     *
     * @param other the other line segment to which the intersection point
     *              is going to be computed.
     * @param p     if there is an intersection between both line segments
     *              or between their respective infinite line extrapolations,
     *              the intersection point will be written here.
     *
     * @return true if an unique intersection point exists. It returns 
     *              \c false otherwise.
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    template<class U>
    bool getIntersectionPointXPol(const tline<T>& other, tpoint<U>& p) const {

      // due to the template argument tpoint<U>, this method must be here
      // otherwise MS Visual C++ won't compile this file!
      
      // NOTE:
      // This method requires following time constants:
      // Additions/LogicOp : 7
      // Multiplications   : 6
      // Divisions         : 1
      // Comparisons       : 2~
      
      const tpoint<T> d1(end-start);
      const tpoint<T> d2(other.end-other.start);
     
      // determinant
      const T det = d1.y*d2.x - d1.x*d2.y;
      //cout << " det " << det << " d1 " << d1 << " d2 " << d2;

      // just one copy for the whole history of this class (faster!)
      static const T epsilon = std::numeric_limits<T>::epsilon();
      
      // the numerical tolerance for parallelism is limited by epsilon.
      if (det > epsilon || det < -epsilon) {
	// compute the intersection point on the extrapolated lines
	const tpoint<T> d0(other.start-start);
	const T t1 = d2.x*d0.y - d2.y*d0.x;
	const double f = static_cast<double>(t1)/static_cast<double>(det);
	//cout << " f " << f;
	condRoundCastTo(start.x + f*d1.x,p.x );
	condRoundCastTo(start.y + f*d1.y,p.y );
	
	return true;
      }
    
      return false;      
    };

    /**
     * Compute the part of the infinite extrapolated line containing this line
     * segment which lies within the given rectangle, and leave the result
     * here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    bool intersectXPol(const trectangle<T>& rect);

    /**
     * Compute the part of the infinite extrapolated line containing the \a
     * other line segment which lies within the given rectangle, and leave the
     * result here.
     *
     * This method assumes, the rectangle is already consistent, i.e.
     * the \a rect.ul point is in both coordinates smaller than \a rect.br.
     *
     * @return true if part of this line lies within the rectangle or its
     *              border, false otherwise.
     */
    inline bool intersectXPol(const tline<T>& other,
                              const trectangle<T>& rect);

    /**
     * @name Scaling and Translation operations
     */
    //@{
    
    /**
     * scale this line by the given \a c factor.
     */
    template<class U>
    inline tline<T>& scale(const U c) {
      start.multiply(c);
      end.multiply(c);
      return *this;
    };

    /**
     * create a new line equal this one scaled by the given \a c factor.
     */
    template<class U>
    inline tline<T> operator*(const U c) const {
      return tline<T>(start*c,end*c);
    };

    /**
     * scale this line by the given \a c factor.
     */
    template<class U>
    inline tline<T>& operator*=(const U c) {
      return multiply(c);
    };

    /**
     * divide both points by the given \a c factor
     */
    template<class U>
    inline tline<T>& divide(const U c) {
      start.divide(c);
      end.divide(c);
      return *this;      
    };

    /**
     * divide both points by the given \a c factor
     */
    template <class U>
    inline tline<T> operator/(const U c) const {
      return tline<T>(start/c,end/c);
    };

    /**
     * divide both points of tline<T> by a given factor
     */
    template <class U>
    inline tline<T>& operator/=(const U c) {
      return divide(c);  
    };

    /**
     * add given point to both ends of this line and leave the result here.
     * @param p the other line to be added to this one
     * @return a reference to this line
     */
    inline tline<T>& translate(const tpoint<T>& p);

    /**
     * add given point to both ends of the \a other line and leave the
     * result here.  
     * @param other the other line to be tranlated
     * @param p the translation factor
     * @return a reference to this line
     */
    inline tline<T>& translate(const tline<T>& other,const tpoint<T>& p);

    /**
     * Compute the orthogonal line and leave the result here.
     *
     * @param offset the offset to the point on the line, where the orthogonal
     *               shall start. This parameter is scaled by the length of the line.
     * @return a reference to this line
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    tline<T>& getOrthogonal(double offset);
    
    /**
     * Compute the orthogonal line to the other line and leave the result here.
     *
     * @param other  the line segment of which the orthogonal line
     *               is going to be computed.
     * @param offset the offset to the point on the line, where the orthogonal
     *               shall start. This parameter is scaled by the length of the line.
     * @return a reference to this line
     *
     * This method can be overloaded in derived classes and other information
     * to accellerate the computations.
     */
    inline tline<T>& getOrthogonal(const tline<T>& other, double offset);
  
    //@}

    /**
     * copy operator
     */
    inline tline<T>& copy(const tline<T>& other);

    /**
     * operator =
     */
    inline tline<T>& operator=(const tline<T>& other) {return copy(other);};

    /**
     * operator ==
     */
    inline bool operator==(const tline<T>& other) const;

    /**
     * operator !=
     */
    inline bool operator!=(const tline<T>& other) const;
  };


  // ----------------------------------------------------------------------
  // Type definitions
  // ----------------------------------------------------------------------

  /**
   * A line with integer coordinates
   */
  typedef tline<int> line;

  /**
   * A line with double coordinates
   */
  typedef tline<double> dline;

  /**
   * A line with float coordinates
   */
  typedef tline<float> fline;

  // ---------------------------------------------------
  // Storable interface
  // ---------------------------------------------------

  /**
   * read the vector from the given ioHandler. The complete flag indicates
   * if the enclosing begin and end should be also be read
   *
   * @ingroup gStorable
   */
  template <class T>
  bool read(ioHandler& handler,tline<T>& l, const bool complete=true);
  
  /**
   * write the vector in the given ioHandler. The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  template<class T>
  bool write(ioHandler& handler,const tline<T>& l,const bool complete=true);

} // namespace lti

namespace std {

  template <class T>
  ostream& operator<<(ostream& s,const lti::tline<T>& l);
  
  template <class T>
  istream& operator>>(istream& s,lti::tline<T>& l);

} // namespace std

// implementation of inline methods
#include "ltiLine_inline.h"

#endif
