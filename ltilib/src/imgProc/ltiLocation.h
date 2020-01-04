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
 * file .......: ltiLocation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 7.1.2002
 * revisions ..: $Id: ltiLocation.h,v 1.13 2003/02/18 17:39:25 alvarado Exp $
 */

#ifndef _LTI_LOCATION_H_
#define _LTI_LOCATION_H_

#include "ltiPoint.h"

namespace lti {
  /**
   * The location class specifies a small region in an image or channel.
   * It is mainly used in the extraction of local features.   There
   * are some functors to extract the important or salient locations in an
   * image, for example lti::axLocalRegions.
   *
   * The locations are represented by its position in an image or channel,
   * an orientation and a radius.
   *
   */
  class location {
  public:
    /**
     * default constructor
     */
    location();

    /**
     * Constructor
     * @param pos position of the middle point of the location
     * @param ang angle of the location (in radians)
     * @param rad radius or size of the location
     */
    location(const point& pos,const float& ang,const float& rad);

    /**
     * Constructor
     * @param pos position of the middle point of the location
     * @param ang angle of the location (in radians)
     * @param rad radius or size of the location
     */
    location(const tpoint<float>& pos,const float& ang,const float& rad);

    /**
     * copy constructor
     */
    location(const location& other);

    /**
     * copy operator
     */
    location& copy(const location& other);

    /**
     * alias for copy operator
     */
    location& operator=(const location& other);

    /**
     * operator ==
     */
    bool operator==(const location& p) const;

    /**
     * operator !=
     */
    bool operator!=(const location& p) const;

    /**
     * Comparition operator.
     *
     * A location is smaller than another one if its radius is smaller, or
     * in case of equal radii, if the position is smaller, i.e. if it has
     * smaller y-coordinate, or in case of equal y-position, if it has
     * smaller x-coordinate.
     */
    bool operator<(const location& other);

    /**
     * Comparition operator.
     *
     * A location is greater than another one if its radius is greater, or
     * in case of equal radii, if the position is greater, i.e. if it has
     * greater y-coordinate, or in case of equal y-position, if it has
     * greater x-coordinate.
     */
    bool operator>(const location& other);

    /**
     * Multiply the radius and position with the given factor
     */
    location& scale(const float& factor);

    /**
     * Multiply the other location's radius and position with the
     * given factor and leave the result here.
     */
    location& scale(const location& other,const float& factor);

    /**
     * Shift the location by the given point
     */
    location& shift(const point& shft);

    /**
     * Shift the location by the given point
     */
    location& shift(const tpoint<float>& shft);

    /**
     * Shift the other location by the given point and leave the result here
     */
    location& shift(const location& other,const point& shft);

    /**
     * Shift the other location by the given point and leave the result here
     */
    location& shift(const location& other,const tpoint<float>& shft);

    /**
     * Add the given angle in radius to the actual angle
     */
    location& rotate(const float& factor);

    /**
     * Add the given angle in radius to the angle of the other location and
     * leave the result here.
     */
    location& rotate(const location& other,const float& factor);

    /**
     * Check if the given point can be considered within the location, i.e.
     * if ||p-position|| < radius
     */
    bool contains(const point& p) const;

    /**
     * Check if the given point can be considered within the location, i.e.
     * if ||p-position|| < radius
     */
    bool contains(const tpoint<float>& p) const;

    /**
     * returns the distance between the borders of two
     * locations or zero if they overlap or if one of the locations
     * lies inside the other one.
     */
    float distance(const location& other);

    /**
     * Position of the location
     */
    tpoint<float> position;

    /**
     * Angle in radius of the location.  It is usually given for
     * the image coordinates, i.e. for a left coordinate system, in which
     * positive angles are given clock-wise.
     */
    float angle;

    /**
     * Radius or size of the location
     */
    float radius;
  };

  /**
   * The rectLocation class specifies a small region in an image or channel.
   * The rectangular locations are represented by its position in an
   * image or channel, an orientation, the length at the orientation
   * direction (maxLenght), and the length at the perpendicular
   * direction (minLength).
   *
   * The difference to lti::location is that the regions are considered
   * rectangular.  Two lengths are needed, where the orientation angle is
   * always given for the maxLength.
   *
   * You can get rectangular %locations with the lti::boundingBox %functor, and
   * used them to check if objects overlap or not.
   */
  class rectLocation {
  public:
    /**
     * default constructor
     */
    rectLocation();

    /**
     * Constructor
     * @param pos position of the middle point of the rectLocation
     * @param ang angle of the rectLocation (in radians)
     * @param maxLength length in pixels of the principal axis
     * @param minLength length in pixels of the second axis
     */
    rectLocation(const point& pos,
                 const float& ang,
                 const float& maxLength,
                 const float& minLength);

    /**
     * Constructor
     * @param pos position of the middle point of the rectLocation
     * @param ang angle of the rectLocation (in radians)
     * @param maxLength length in pixels of the principal axis
     * @param minLength length in pixels of the second axis
     */
    rectLocation(const tpoint<float>& pos,
                 const float& ang,
                 const float& maxLength,
                 const float& minLength);

    /**
     * copy constructor from a normal location
     */
    rectLocation(const location& loc);

    /**
     * copy constructor
     */
    rectLocation(const rectLocation& other);

    /**
     * copy operator
     */
    rectLocation& copy(const rectLocation& other);

    /**
     * copy the other location into this rectLocation.  The radius of
     * the location will be assumed as the maxLength and minLength.
     */
    rectLocation& castFrom(const location& other);


    /**
     * alias for copy operator
     */
    rectLocation& operator=(const rectLocation& other);

    /**
     * operator ==
     */
    bool operator==(const rectLocation& p) const;

    /**
     * operator !=
     */
    bool operator!=(const rectLocation& p) const;

    /**
     * Comparition operator.
     *
     * A rectLocation is smaller than another one if its area is smaller, or
     * in case of equal areas, if the position is smaller, i.e. if it has
     * smaller y-coordinate, or in case of equal y-position, if it has
     * smaller x-coordinate.
     */
    bool operator<(const rectLocation& other);

    /**
     * Comparition operator.
     *
     * A rectLocation is greater than another one if its area is greater, or
     * in case of equal radii, if the position is greater, i.e. if it has
     * greater y-coordinate, or in case of equal y-position, if it has
     * greater x-coordinate.
     */
    bool operator>(const rectLocation& other);

    /**
     * Multiply the lengths and position with the given factor
     */
    rectLocation& scale(const float& factor);

    /**
     * Multiply the other rectLocation's lengths and position with the
     * given factor and leave the result here.
     */
    rectLocation& scale(const rectLocation& other,
                        const float& factor);

    /**
     * Shift the rectLocation by the given point
     */
    rectLocation& shift(const point& shft);

    /**
     * Shift the rectLocation by the given point
     */
    rectLocation& shift(const tpoint<float>& shft);

    /**
     * Shift the other rectLocation by the given point and leave the
     * result here
     */
    rectLocation& shift(const rectLocation& other,const point& shft);

    /**
     * Shift the other rectLocation by the given point and leave the
     * result here
     */
    rectLocation& shift(const rectLocation& other,const tpoint<float>& shft);

    /**
     * Add the given angle in radius to the actual angle
     */
    rectLocation& rotate(const float& factor);

    /**
     * Add the given angle in radius to the angle of the other rectLocation and
     * leave the result here.
     */
    rectLocation& rotate(const rectLocation& other,const float& factor);

    /**
     * Check if the given point can be considered within the rectLocation.
     */
    bool contains(const point& p) const;

    /**
     * Check if the given point can be considered within the rectLocation.
     */
    bool contains(const tpoint<float>& p) const;

    /**
     * returns the square of the distance between the borders of two
     * locations or zero if they overlap or if one of the locations
     * lies inside the other one.
     *
     * @param other the other rectLocation to be compared with
     * @return minimal distance to other location
     */
    float distanceSqr(const rectLocation& other);

    /**
     * returns the square of the distance between the borders of two
     * locations or zero if they overlap or if one of the locations
     * lies inside the other one.
     *
     * @param other the other rectLocation to be compared with
     * @param pt point in the border of this location with the
     *           smallest distance.
     * @param po point in the border of the other location with the
     *           smallest distance.
     * @return minimal distance to other location
     */
    float distanceSqr(const rectLocation& other,
                    tpoint<float>& pt,
                    tpoint<float>& po);

    /**
     * Get the area of this location (maxLength*minLength)
     */
    float getArea() const;

    /**
     * Position of the rectLocation
     */
    tpoint<float> position;

    /**
     * Angle in radius of the rectLocation.  It is usually given for
     * the image coordinates, i.e. for a left coordinate system, in which
     * positive angles are given clock-wise.
     */
    float angle;

    /**
     * maximum length.  The angle corresponds to the axis with the maxLength
     */
    float maxLength;

    /**
     * minimum length.  Should be smaller than maximum length
     */
    float minLength;
  };


  bool read(ioHandler& handler,
            location& mat,
            const bool complete=true);

  bool write(ioHandler& handler,
             const location& mat,
             const bool complete=true);

  bool read(ioHandler& handler,
            rectLocation& mat,
            const bool complete=true);

  bool write(ioHandler& handler,
             const rectLocation& mat,
             const bool complete=true);
}

namespace std {
  ostream& operator<<(ostream& s,
                      const lti::location& loc);

  ostream& operator<<(ostream& s,
                      const lti::rectLocation& loc);

}

#endif


