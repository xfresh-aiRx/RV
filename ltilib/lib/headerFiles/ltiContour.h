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
 * project ....: LTI Digital Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiContour.h
 * authors ....: Axon Development Team
 * organization: LTI, RWTH Aachen
 * creation ...: 19.08.98
 * revisions ..: $Id: ltiContour.h,v 1.33 2005/11/09 17:02:11 doerfler Exp $
 */

#ifndef _LTICONTOUR_H
#define _LTICONTOUR_H

#include <list>
#include "ltiTypes.h"
#include "ltiPointList.h"
#include "ltiImage.h"

namespace lti {

  /**
   * Elements of a Chaincode.
   * The used directions follows the next diagram:
   *
   * \code
   * NW   N   NE
   *    \ | /
   * W -  x - E
   *    / | \
   * SW   S   SE
   * \endcode
   *
   * The "int" value resulting from casting these chain codes is
   * proportional to the angle with a proportionality constant of 45°,
   * i.e. if 'c' is a chainCode object, then the equivalent direction
   * in degrees is 45*int(c.getDirection()).
   *
   * @ingroup gShape
   */
  class chainCode {
  public:
    /**
     * Directions used in chain codes
     * The used directions follows the next diagram:
     *
     * \code
     * NW   N   NE
     *    \ | /
     * W -  x - E
     *    / | \
     * SW   S   SE
     * \endcode
     *
     * The "int" value resulting from casting these chain codes is
     * proportional to the angle with a proportionality constant of 45°,
     * i.e. if 'c' is a chainCode object, then the equivalent direction
     * in degrees is 45*int(c.getDirection()).
     */
    enum direction {UNKNOWN=-2, /**< Unknown */
                    NOMOVE=-1,  /**< No Move */
                    E=0,        /**< East (or Right) */
                    NE,         /**< North-East (or Right-Up) */
                    N,          /**< North (or Up) */
                    NW,         /**< North-West (or North-West) */
                    W,          /**< West (or West) */
                    SW,         /**< South-West (or Left-Down) */
                    S,          /**< South (or Down) */
                    SE          /**< South-East (or Right-Down) */
    };

    /**
     * @name Canzler Codes
     *
     * Given two chain codes for a sequence of three adjacent pixels,
     * the Canzler-Codes give you information which borders of the middle
     * pixel belong to the boundary.  With the getCanzlerCode you can obtain
     * the corresponding coding for the pixel pointed by this chain code, if
     * the next chain code is the one given.  They are somehow similar to the
     * so called "chain crack codes", but instead of specifying a direction
     * of the boundary, they code which "crack codes" belong to the middle
     * pixel between two chain codes.
     *
     * There are 16 Canzler-Codes, that can be obtained by the combination
     * of the four values Top, Right, Bottom and Left, which specify that
     * the border runs on the top, right, bottom and/or left edge of the
     * pixel respectively.
     */
    //@{
    static const ubyte Nothing; /// Nothing has the value 0
    static const ubyte Top;     /// Top has value 1 (the first bit)
    static const ubyte Right;   /// Right has value 2 (the second bit)
    static const ubyte Bottom;  /// Bottom has value 4 (the third bit)
    static const ubyte Left;    /// Left has value 8 (the fourth bit)
    //@}

    /**
     * default constructor
     */
    chainCode(const direction& v = NOMOVE)
      : value(v) {
    };

    /**
     * constructor to cast an integer
     */
    chainCode(const int& v) : value(direction(v%8)) {};

    /**
     * copy constructor
     */
    chainCode(const chainCode& other)
      : value(other.value) {
    };

    /** Constrcutor. This constructor creates a chainCode object using the
	sign of the parameters to calculate the direction.
	Image-coordinates are used, i.e. a positive "y" implies a
	change towards "south", and a positive x implies a
	change towards "east". */
    chainCode(const int& x,const int& y) : value(NOMOVE) {
      fromDeltas(x,y);
    };

    /**
     * Constructor.This constructor creates a chainCode object using
     * two points
     */
    chainCode(const point& here,const point& next) : value(NOMOVE) {
      point tmp(next);
      tmp.subtract(here);
      fromDeltas(tmp.x,tmp.y);
    };

    /**
     * returns direction of this chain element
     */
    inline direction getDirection() const;

    /**
     * Possible Direction.returns true if this object contains one of
     * the eight posible directions, and false otherwise
     */
    inline bool isDirection() const;

    /**
     * returns next point with start point "here" and using this chainCode.
     */
    inline point getNext(const point& here) const;

    /**
     * returns previous point with start point "here" and using this chainCode.
     */
    inline point getPrevious(const ipoint& here) const;

    /**
     * returns change in x.
     * For example, if the value is NW, deltaX() returns -1.
     */
    inline int deltaX() const;

    /**
     * returns change in y.
     * For example, if the value is NW, deltaY() returns -1.
     */
    inline int deltaY() const;

    /**
     * returns point with changes in both direction, x and y.
     */
    inline point delta() const;

    /**
     * become other object
     */
    inline chainCode& operator=(const direction& other) {
      return copy(other);
    };

    /**
     * become other object
     */
    inline chainCode& copy(const direction& other);

    /**
     * become other object
     */
    inline chainCode& copy(const chainCode& other);

    /**
     * become other object
     */
    inline chainCode& operator=(const chainCode& other) {
      return(copy(other));
    };

    /**
     * compare with another object
     */
    inline bool compare(const chainCode& other);

    /**
     * compare with another object
     */
    inline bool operator==(const chainCode& other) {
      return compare(other);
    };

    /**
     * compare with another object
     */
    inline bool compare(const direction& other);

    /**
     * compare with another object
     */
    inline bool operator==(const direction& other) {
      return compare(other);
    };

    /**
     * subtract the angles of the first and second chain code elements and
     * leave the result here!
     */
    inline chainCode& subtract(const chainCode& first,
                               const chainCode& second);
    /**
     * subtract the angle equivalent of the <em>other</em> chainCode
     * from *this
     */
    inline chainCode operator-(const chainCode& other) const {
      chainCode tmp;
      tmp.subtract(*this,other);
      return tmp;
    };

    /**
     * subtract the angle equivalent of the other chain code from this one.
     * The result will be left in this instance, and a reference to it will
     * be returned.
     */
    inline chainCode& subtract(const chainCode& other);

    /**
     * subtract another chain code
     */
    inline chainCode& operator-=(const chainCode& other) {
      return subtract(other);
    };

    /**
     * add the angles of the first and second chain codes
     */
    inline chainCode& add(const chainCode& first,
                          const chainCode& second);

    /**
     * add the angles of this and the other chain code
     */
    inline chainCode operator+(const chainCode& other) const {
      chainCode tmp;
      tmp.add(*this,other);
      return tmp;
    };

    /**
     * add the angles of the other chain code to this chainCode and
     * leave the result in this instance.  Returns a reference to the instance
     */
    inline chainCode& add(const chainCode& other);

    /**
     * add another chain code
     */
    inline chainCode& operator+=(const chainCode& other) {
      return add(other);
    };

    /**
     * divide chain code equivalent angle with an integer.  The result
     * will be left in this instance and a reference to it will be returned
     */
    inline chainCode& divide(const int& other) {
      value=(direction)((int)value/other);
      return (*this);
    };

    /**
     * divide chain code angle with integer.
     */
    inline chainCode operator/(const int& other) {
      chainCode tmp(*this);
      tmp.divide(other);
      return (tmp);
    }

    /**
     * return the Canzler Code for the pixel pointed by this chainCode
     * if the second chainCode is the one given as parameter.
     *
     * The result a the bitwise OR of some of the codes Top, Bottom, Left and
     * Right.
     *
     * @param nextChainCode the next chain code of this one in the chain
     *                      code sequence.
     * @return the canzler bit-coding for the edges of the pixel pointed
     *         by this chainCode which are part of the boundary.  The values
     *         returned are bit or of the constants Left, Top, Right and
     *         Bottom.
     */
    inline ubyte getCanzlerCode(const chainCode& nextChainCode) const;

  protected:
    /**
     * Attribute containing the chain code
     */
    direction value;

    /**
     * compute chain code from x and y deltas
     */
    inline void fromDeltas(const int& x,const int& y);
  };

  /**
   * read the matrix from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,chainCode& cc,const bool complete=true);

  /**
   * write the matrix in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const chainCode& cc,const bool complete=true);

  //declared in ltiPolygonPoints.h
  class polygonPoints;
  class borderPoints;
  class areaPoints;

  /**
   * Contour classes: IO-Points.
   *
   * For the explanation of the contour description in this class, see
   * following image:
   *
   * \code
   *   -- 00000000001111111111222222222233
   *   -- 01234567890123456789012345678901
   *   00 --------------------------------
   *   01 --------------------------------
   *   02 --------------------------------
   *   03 --------BBBB------BBBB----------
   *   04 -------B****BBB----B**B---------
   *   05 -------B*******B---B***B--------
   *   06 ------B*******B-----B*B---------
   *   07 -------B*******BBBBB**B---------
   *   08 ---------B*************B--------
   *   09 --------B**----*********B-------
   *   10 --------B**-----********B-------
   *   11 -------B**-----*******BB--------
   *   12 ------B**-----*******B----------
   *   13 ------B**-------******BB--------
   *   14 -----B**---------*******B-------
   *   15 -----B**--------*********B------
   *   16 ----B**-------**********BB------
   *   17 ---B***----*******----BB--------
   *   18 ----BBBBBBBBB***  --*B----------
   *   19 -------------BBBBBBBB-----------
   *   20 --------------------------------
   *   21 --------------------------------
   *   22 --------------------------------
   *   23 --------------------------------
   *
   *  "-" means background and the rest is part of the object.
   *  "B" indicates a borderpoint.
   *  \endcode
   *
   *  There are mainly three representations of a contour:
   *  - IO-Points. It contains all input and output points for
   *    each line.  For example, for the previous image:
   *    (8,3)(11,3)(18,3)(21,3)(7,4)(14,4)(19,4)(22,4)...
   *    Note that every IO-point is a border point, but not all border
   *    points are IO-points.
   *  - Border Boints.  It contains a list of the points at the border.
   *    Beginning with the point at (8,3) the chain code for our example
   *    image is:
   *    (8,3)(9,3)(10,3)(11,3)(12,4)(13,4)(14,4)(15,5)(14,6)...
   *  - Area-Points.  It contains all points in the object.
   *
   *  @see lti::borderPoints, lti::areaPoints
   *
   *  @ingroup gAggregate
   *  @ingroup gShape
   */
  class ioPoints : public pointList {
  public:
    /**
     * default constructor creates an empty io-points list
     */
    ioPoints();

    /**
     * create this pointList as a copy of another pointList
     * @param other the pointList to be copied.
     */
    ioPoints(const ioPoints& other);

    /**
     * destructor
     */
    virtual ~ioPoints();

    /**
     * returns the name of this class: "ioPoints"
     */
    const char* getTypeName() const {return "ioPoints";};

    /**
     * creates this io-list from the given area-points-list
     *
     */
    ioPoints& castFrom(const areaPoints& theAreaPoints);

    /**
     * creates this io-list from the given border-points-list
     */
    ioPoints& castFrom(const borderPoints& theBorderPoints);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    ioPoints& operator=(const ioPoints& other) {
      copy(other);
      return *this;
    };

    /**
     * generate ioPoint-list from the given mask image.
     *
     * This function assumes that the mask contains JUST ONE connected
     * object.  To get the biggest object on the mask see the
     * lti::objectsFromMask functor.
     */
    bool getFromMask(const channel8& mask);

    /**
     * generate mask from io-points.
     *
     * The dimensions of the resulting mask are the smallest rectangle to
     * contain the point (0,0) (optional) and all points in this list, plus
     * 1 pixel in both width and heigth.
     * If the boundary of the point list is not up to date, it can
     * be calculated if specified in the %parameters:
     * @param mask The calculated mask will be stored here
     * @param computeBoundary If false, the internal boundary of the
     *    point list will be used, otherwise the boundary
     *    will be calculated (but NOT updated!!!).
     * @param exactBoundaryDimensions If true, the dimensions of the resulting
     *    mask will be the smallest rectangle to contain only the points in
     *    this list. The origin (0,0) may not be included, therefore a 1:1
     *    correspondence of coordinates will generally not be given.
     * @param keepMaskData if false, the mask will be initialized with 
     *                     zero before getting the area mask.  If true,
     *                     the previous mask data is not deleted, but
     *                     the mask will be resized if necessary.
     * @return true if successful, false otherwise.
     */
    bool generateMask(channel8& mask,
                      const bool computeBoundary=true,
                      const bool exactBoundaryDimensions=false,
                      const bool keepMaskData=false) const;

    /**
     * sort the points of this in ascending order first of y, and than of
     * x.
     */
    void sort();
  };

  /**
   * Contour classes: Border-Points.
   *
   * For the explanation of the contour description in this class, see
   * following image:
   *
   * \code
   *   -- 00000000001111111111222222222233
   *   -- 01234567890123456789012345678901
   *   00 --------------------------------
   *   01 --------------------------------
   *   02 --------------------------------
   *   03 --------BBBB------BBBB----------
   *   04 -------B****BBB----B**B---------
   *   05 -------B*******B---B***B--------
   *   06 ------B*******B-----B*B---------
   *   07 -------B*******BBBBB**B---------
   *   08 ---------B*************B--------
   *   09 --------B**----*********B-------
   *   10 --------B**-----********B-------
   *   11 -------B**-----*******BB--------
   *   12 ------B**-----*******B----------
   *   13 ------B**-------******BB--------
   *   14 -----B**---------*******B-------
   *   15 -----B**--------*********B------
   *   16 ----B**-------**********BB------
   *   17 ---B***----*******----BB--------
   *   18 ----BBBBBBBBB***  --*B----------
   *   19 -------------BBBBBBBB-----------
   *   20 --------------------------------
   *   21 --------------------------------
   *   22 --------------------------------
   *   23 --------------------------------
   *
   *  "-" means background and the rest is part of the object.
   *  "B" indicates a borderpoint.
   *  \endcode
   *
   *  This contour class allows three representations of a contour:
   *  - IO-Points. It contains all input and output points for
   *    each line.  For example, for the previous image:
   *    (8,3)(11,3)(18,3)(21,3)(7,4)(14,4)(19,4)(22,4)...
   *    Note that every IO-point is a border point, but not all border
   *    points are IO-points.
   *  - Border Boints.  It contains a list of the points at the border.
   *    Beginning with the point at (8,3) the chain code for our example
   *    image is:
   *    (8,3)(9,3)(10,3)(11,3)(12,4)(13,4)(14,4)(15,5)(14,6)...
   *  - Area-Points.  It contains all points in the object.
   *
   *  @see lti::ioPoints, lti::areaPoints
   *
   *  @ingroup gAggregate
   *  @ingroup gShape
   */
  class borderPoints : public pointList {
  public:
    /**
     * default constructor creates an empty border-point-list
     */
    borderPoints();

    /**
     * create this pointList as a copy of another pointList
     * @param other the pointList to be copied.
     */
    borderPoints(const borderPoints& other);

    /**
     * destructor
     */
    virtual ~borderPoints();

    /**
     * returns the name of this class: "borderPoints"
     */
    const char* getTypeName() const {return "borderPoints";};

    /**
     * creates this border-point-list from the given area-point-list
     */
    borderPoints& castFrom(const areaPoints& theAreaPoints);

    /**
     * creates this io-list from the given io-point-list
     */
    borderPoints& castFrom(const ioPoints& theIOPoints);

   /**
     * creates this borderPoint-List from the given PolygonPoint-List
     * conecting the points with lines
     * @param thePolygonPoints polygon points with source data
     * @return a reference to this object
     */
    borderPoints& castFrom(const polygonPoints& thePolygonPoints);

    /**
     * creates this borderPoint-List from the given PolygonPoint-List
     * interpolating the curve with a cubicspline.
     * @param thePolygonPoints polygon points with source data
     * @param segments each space between 2 polygonPoints
     *        is parted in segments lines.
     * @return a reference to this object
     */
    borderPoints& castFrom(const polygonPoints& thePolygonPoints,
                           const int& segments);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    borderPoints& operator=(const borderPoints& other) {
      copy(other);
      return *this;
    };

    /**
     * generate a border-point-list from the given mask image.
     *
     * This function assumes that the mask contains JUST ONE connected
     * object.  To get the biggest object on the mask see the
     * lti::objectsFromMask functor.
     */
    bool getFromMask(const channel8& mask);

    /**
     * generate mask from border-points.
     *
     * The dimensions of the resulting mask are the smallest rectangle to
     * contain the point (0,0) (optional) and all points in this list, plus
     * 1 pixel in both width and heigth.
     * If the boundary of the point list is not up to date, it can
     * be calculated if specified in the %parameters:
     * @param mask The calculated mask will be stored here
     * @param computeBoundary If false, the internal boundary of the
     *    point list will be used, otherwise the boundary
     *    will be calculated (but NOT updated!!!).
     * @param exactBoundaryDimensions If true, the dimensions of the resulting
     *    mask will be the smallest rectangle to contain only the points in
     *    this list. The origin (0,0) may not be included, therefore a 1:1
     *    correspondence of coordinates will generally not be given.
     * @param keepMaskData if false, the mask will be initialized with 
     *                     zero before getting the area mask.  If true,
     *                     the previous mask data is not deleted, but
     *                     the mask will be resized if necessary.
     * @return true if successful, false otherwise.
     */
    bool generateMask(channel8& mask,
                      const bool computeBoundary=true,
                      const bool exactBoundaryDimensions=false,
                      const bool keepMaskData=false) const;

    /**
     * invert the direction of the border points
     */
    void invert();

    /**
     * Check consistency of border points. Border points are
     * consistent if the D8 distance between any two subsequent points
     * (with the first point being subsequent to the last) is exactly
     * 1, i.e. if every border point is "next to" its predecessor,
     * with "next to" meaning "one of the 8 neighbouring pixels in the
     * grid".
     */
    bool isConsistent() const;

  protected:
    inline bool inside(const point& p,const channel8& mask) {
      return ((p.x>=0) && (p.x<mask.columns()) &&
              (p.y>=0) && (p.y<mask.rows()));
    };

  };

  /**
   * Contour classes: Area-Points.
   *
   * For the explanation of the contour description in this class, see
   * following image:
   *
   * \code
   *   -- 00000000001111111111222222222233
   *   -- 01234567890123456789012345678901
   *   00 --------------------------------
   *   01 --------------------------------
   *   02 --------------------------------
   *   03 --------BBBB------BBBB----------
   *   04 -------B****BBB----B**B---------
   *   05 -------B*******B---B***B--------
   *   06 ------B*******B-----B*B---------
   *   07 -------B*******BBBBB**B---------
   *   08 ---------B*************B--------
   *   09 --------B**----*********B-------
   *   10 --------B**-----********B-------
   *   11 -------B**-----*******BB--------
   *   12 ------B**-----*******B----------
   *   13 ------B**-------******BB--------
   *   14 -----B**---------*******B-------
   *   15 -----B**--------*********B------
   *   16 ----B**-------**********BB------
   *   17 ---B***----*******----BB--------
   *   18 ----BBBBBBBBB***  --*B----------
   *   19 -------------BBBBBBBB-----------
   *   20 --------------------------------
   *   21 --------------------------------
   *   22 --------------------------------
   *   23 --------------------------------
   *
   *  "-" means background and the rest is part of the object.
   *  "B" indicates a borderpoint.
   *  \endcode
   *
   *  This contour class allows three representations of a contour:
   *  - IO-Points. It contains all input and output points for
   *    each line.  For example, for the previous image:
   *    (8,3)(11,3)(18,3)(21,3)(7,4)(14,4)(19,4)(22,4)...
   *    Note that every IO-point is a border point, but not all border
   *    points are IO-points.
   *  - Border Boints.  It contains a list of the points at the border.
   *    Beginning with the point at (8,3) the chain code for our example
   *    image is:
   *    (8,3)(9,3)(10,3)(11,3)(12,4)(13,4)(14,4)(15,5)(14,6)...
   *  - Area-Points.  It contains all points in the object.
   *
   *  @see lti::ioPoints, lti::borderPoints
   *
   *  @ingroup gAggregate
   *  @ingroup gShape
   */
  class areaPoints : public pointList {
  public:
    /**
     * default constructor creates an empty area-point-list
     */
    areaPoints();

    /**
     * create this pointList as a copy of another pointList
     * @param other the pointList to be copied.
     */
    areaPoints(const areaPoints& other);

    /**
     * destructor
     */
    virtual ~areaPoints();

    /**
     * returns the name of this class: "areaPoints"
     */
    const char* getTypeName() const {return "areaPoints";};

    /**
     * creates this area-point-list from the given border-point-list
     */
    areaPoints& castFrom(const borderPoints& theBorderPoints);

    /**
     * creates this io-list from the given io-point-list
     */
    areaPoints& castFrom(const ioPoints& theIOPoints);

    /**
     * creates this io-list from the given io-point-list
     */
    areaPoints& castFrom(const polygonPoints& theIOPoints);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    areaPoints& operator=(const areaPoints& other) {
      copy(other);
      return *this;
    };

    /**
     * generate a area-point-list from the given mask image.
     *
     * This function assumes that the mask contains JUST ONE connected
     * object.  To get the biggest object on the mask see the
     * lti::objectsFromMask functor.
     */
    bool getFromMask(const channel8& mask);

    /**
     * generate mask from area-points.
     *
     * The dimensions of the resulting mask are the smallest rectangle to
     * contain the point (0,0) (optional) and all points in this list, plus
     * 1 pixel in both width and heigth.  If the given mask is bigger than
     * this size, its dimensions will be kept.
     *
     * If the boundary of the point list is not up to date, it can
     * be calculated if specified in the %parameters:
     * @param mask The calculated mask will be stored here
     * @param computeBoundary If false, the internal boundary of the
     *    point list will be used, otherwise the boundary
     *    will be calculated (but NOT updated!!!).
     * @param exactBoundaryDimensions If true, the dimensions of the resulting
     *    mask will be the smallest rectangle to contain only the points in
     *    this list. The origin (0,0) may not be included, therefore a 1:1
     *    correspondence of coordinates will generally not be given.
     * @param keepMaskData if false, the mask will be initialized with 
     *                     zero before getting the area mask.  If true,
     *                     the previous mask data is not deleted, but
     *                     the mask will be resized if necessary.
     * @return true if successful, false otherwise.
     */
    bool generateMask(channel8& mask,
                      const bool computeBoundary=true,
                      const bool exactBoundaryDimensions=false,
                      const bool keepMaskData=false) const;

    /**
     * sort the points of this in ascending order first of y, and than of
     * x.
     */
    void sort();

    /**
     * Compute intersection area with a rectangle
     */
    int intersectionArea(const rectangle& rect) const;
  };

}

#include "ltiContour_inline.h"

#endif

