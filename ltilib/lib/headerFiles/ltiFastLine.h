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
 * file .......: ltiFastLine.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 07.02.2003
 * revisions ..: $Id: ltiFastLine.h,v 1.5 2004/05/03 12:20:01 alvarado Exp $
 */

#ifndef LTI_FAST_LINE_H
#define LTI_FAST_LINE_H

#include "ltiConfig.h"

#include <iostream>
#include <limits>

#include "ltiIoHandler.h"
#include "ltiMath.h"
#include "ltiPoint.h"
#include "ltiLine.h"
#include "ltiRectangle.h"

namespace lti {


  /**
   * Type for computations with lines.
   *
   * A line (or more generally a tfastLine<T>) is represented by a start point
   * and an end point.
   *
   * The type T correspond to the coordinate type used in both points.
   *
   * This class stores besides the two delimiting points other derived
   * information that can save some time in some intensive computations 
   * involving distances and intersections of lines.
   *
   * Of course, the class becomes faster, if the operations are done with
   * lines that do not change much in time.  If you have thousends of lines
   * and want to compute the intersection among all of them, this class 
   * provides a good solution.  If you have one line that changes continously 
   * and want to compute the intersection with another changing line, the 
   * parent class can be even faster, due to the fact that the internal stage
   * do not need to be updated.
   *
   * @see tline<T>
   *
   * @ingroup gGeomData
   */
  template <class T>
  class tfastLine : public tline<T> {
  protected:
    /**
     * @name dependent attributes
     *
     * These attributes depend on the start and end points and cannot be
     * considered as a normal state component of a line instance.  They can be
     * changed by "const" methods, which do not change the start and end
     * points.  Therefore, this attributes are all mutable.
     */
    //@{
    /**
     * flag indicating state of the slope
     */
    mutable bool uptodate;

    /**
     * delta
     */
    mutable tpoint<T> delta;

    /**
     * flag indicating if the stored value is slope (false)  or 1/slope (true)
     */
    mutable bool invSlope;

    /**
     * value of the slope: always between -1 and 1.
     */
    mutable float normSlope;

    //@}

    /**
     * Method called if a change in the points have been done.
     *
     * This policy does nothing here.
     */
    inline void updateRequest();
    
    /**
     * const method called when slope and other attributes need to be
     * updated.
     */
    inline void ensureCorrectSlope() const;

  public:
    /**
     * default constructor.
     *
     * Both points are left uninitialized (this can save some time)
     */
    explicit tfastLine();
    
    /**
     * constructor with both points
     */
    tfastLine(const tpoint<T>& theStart, const tpoint<T>& theEnd);
    
    /**
     * copy constructor
     */
    template <class U> 
    tfastLine(const tfastLine<U>& other) 
      : tline<T>(other),uptodate(other.uptodate),delta(other.delta),
        invSlope(other.invSlope),normSlope(other.normSlope) {
    };

    /**
     * copy constructor
     */
    template <class U> 
    tfastLine(const tline<U>& other) 
      : tline<T>(other),uptodate(false) {
    };

    /**
     * cast operator
     */
    template <class U>
    tfastLine<T>& castFrom(const tfastLine<U>& other) {
      tline<T>::castFrom(other);
      updateRequest();
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
     * @name Computation of line attributes
     */
    //@{
    /**
     * Update slope can be called to force update of the internal variables
     * containing the line attributes (for example, at a not critical
     * computation time you will want to call this.)
     */
    inline void updateSlope();

    /**
     * returns a point containing getEnd() - getStart().
     */
    inline const tpoint<T>& getDelta() const;

    /**
     * Get slope of the line ensuring that its absolute value is
     * between -1 and 1.  
     *
     * Let delta=getDelta():
     *
     * - If the returned boolean is \c true, the \a slope variable will
     *   contain usual slope (delta.y/delta.x).
     * - If the returned boolean is false, it means that the inverse of the
     *   slope has being set in \a slope (delta.x/delta.y).
     *
     * This way, there is no risk to become an infinite slope.
     */
    inline bool getNormalizedSlope(float& slope) const;

    /**
     * To avoid extra computations, this method allows you to get
     * three line attributes at once: 
     * @param deltaLine corresponds to end-start.
     * @param nslope normalized slope, always in interval -1 to 1.  
     * @param normalSlope if true, \a nslope contains the usual definition
     *                     of slope (deltaLine.y/deltaLine.x), if false,
     *                     \a nslope contains (deltaLine.x/deltaLine.y)
     */
    inline void getLineAttributes(tpoint<T>& deltaLine,
                                  float& nslope,
                                  bool&  normalSlope) const;



    //@}

    /**
     * @name Distance computation
     */
    //@{

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
     * If not update of state (slope, delta and so on) is required, this
     * method is about 12\% faster than the normal one (in tline<T>).
     * If an update is required, the opposite happens, and this method is
     * about 10\% slower than the tline<T> one.
     *
     * @see sqrDistanceTo()
     */
    T distanceSqrXPol(const tpoint<T>& c,tpoint<T>& p) const;

    /**
     * Calculate distance to the point c to the infinite line (eXtraPolated)
     * containing this line segment.
     *
     * @return the minimal distance to c
     */
    inline T distanceToXPol(const tpoint<T>& c,tpoint<T>& p) const;

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
    T distanceSqrXPol(const tpoint<T>& c) const;

    /**
     * Calculate distance to the point c to the infinite line (eXtraPolated)
     * containing this line segment.
     *
     * @return the minimal distance to c
     */
    inline T distanceToXPol(const tpoint<T>& c) const;

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
    bool doIntersect(const tfastLine<T>& other) const;

    /**
     * Check if this line segment intersects the \a other given one.
     *
     * @param other the other line segment to which an intersection is
     *              going to be checked.
     * @param p     if there is an intersection between both line segments
     *              or between their respective infinite line extrapolations,
     *              the intersection point will be written here.
     * @param onThisLine if the intersection occurs on a point on the line
     *              segment, this parameter will be set to true.  Otherwise
     *              false.
     * @param onOtherLine if the intersection occurs on a point on the other
     *              line segment, this parameter will be set to true.
     * @param colinear this parameter is set to true in case both line segments
     *              are parallel and co-linear.
     *
     * @return true if both line segments intersect, i.e. this method
     *              returns \c true if the intersection of both infinite
     *              extrapolations lay within both line segments (\a onThisLine
     *              and \a onOtherLine are both set to \c true).  It returns 
     *              \c false otherwise.  Note that even if the return value is
     *              \c false, the value of the point \a p is updated to the
     *              proper intersection of the two infinite lines containing
     *              the segments.  If both line segments are parallel and
     *              colinear, this method returns \c true only if both segments
     *              overlap.
     *            
     *
     * For parallel line segments following values can be therefore expected:
     * - Parallel but not colinear: return false and \a colinear is false.
     * - Parallel and colinear: return \c true if both segments overlap, 
     *            \c false otherwise.  \a colinear is set to \c true.
     *
     * In case the lines are parallel (colinear or not) the point \a p
     * is leaved unchaged, because the intersection occurs at none or
     * more than one points.
     *
     * This method is for updated slope values about 25% faster than the
     * normal tline method.  When one of the lines needs update
     * both methods need almost the same time.
     */
    bool getIntersectionPointXPol(const tfastLine<T>& other, 
				  //bool intersect(const tfastLine<T>& other, 
                   tpoint<T>& p, 
                   bool& onThisLine, 
                   bool& onOtherLine,
                   bool& colinear) const;

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
    inline bool intersect(const tfastLine<T>& other,
                          const trectangle<T>& rect);


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
    inline bool intersectXPol(const tfastLine<T>& other,
                              const trectangle<T>& rect);

    /**
     * @name Scaling and Translation operations
     */
    //@{
    
    /**
     * scale this line by the given \a c factor.
     */
    template<class U>
    inline tfastLine<T>& scale(const U c) {
      this->start.multiply(c);
      this->end.multiply(c);
      updateRequest();
      return *this;
    };

    /**
     * create a new line equal this one scaled by the given \a c factor.
     */
    template<class U>
    inline tfastLine<T> operator*(const U c) const {
      return tfastLine<T>(this->start*c,this->end*c);
    };

    /**
     * scale this line by the given \a c factor.
     */
    template<class U>
    inline tfastLine<T>& operator*=(const U c) {
      return multiply(c);
    };

    /**
     * divide both points by the given \a c factor
     */
    template<class U>
    inline tfastLine<T>& divide(const U c) {
      this->start.divide(c);
      this->end.divide(c);
      updateRequest();
      return *this;      
    };

    /**
     * divide both points by the given \a c factor
     */
    template <class U>
    inline tfastLine<T> operator/(const U c) const {
      return tfastLine<T>(this->start/c,this->end/c);
    };

    /**
     * divide both points of tfastLine<T> by a given factor
     */
    template <class U>
    inline tfastLine<T>& operator/=(const U c) {
      return divide(c);  
    };

    /**
     * add given point to both ends of this line and leave the result here.
     * @param p the other line to be added to this one
     * @return a reference to this line
     */
    inline tfastLine<T>& translate(const tpoint<T>& p);

    /**
     * add given point to both ends of the \a other line and leave the
     * result here.  
     * @param other the other line to be tranlated
     * @param p the translation factor
     * @return a reference to this line
     */
    inline tfastLine<T>& translate(const tline<T>& other,
                                   const tpoint<T>& p);

    //@}

    /**
     * copy operator
     */
    inline tfastLine<T>& copy(const tline<T>& other);

    /**
     * copy operator
     */
    inline tfastLine<T>& copy(const tfastLine<T>& other);

    /**
     * operator =
     */
    inline tfastLine<T>& operator=(const tfastLine<T>& other);

    /**
     * operator =
     */
    inline tfastLine<T>& operator=(const tline<T>& other);
  };


  // ----------------------------------------------------------------------
  // Type definitions
  // ----------------------------------------------------------------------

  /**
   * A line with integer coordinates
   */
  typedef tfastLine<int> fastLine;

  /**
   * A line with double coordinates
   */
  typedef tfastLine<double> dfastLine;

  /**
   * A line with float coordinates
   */
  typedef tfastLine<float> ffastLine;

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
  bool read(ioHandler& handler,tfastLine<T>& l, const bool complete=true);
  

} // namespace lti

namespace std {
  
  template <class T>
  istream& operator>>(istream& s,lti::tfastLine<T>& l);

} // namespace std

// implementation of inline methods
#include "ltiFastLine_inline.h"

#endif
