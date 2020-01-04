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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiGraphicsPattern.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 05.02.03
 * revisions ..: $Id: ltiGraphicsPattern.h,v 1.9 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_GRAPHICSPATTERN_H_
#define _LTI_GRAPHICSPATTERN_H_

#include "ltiIoObject.h"
#include "ltiRectangle.h"
#include "ltiVector.h"
#include "ltiPolygonPoints.h"
#include "ltiPointList.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * graphicsPattern template class. It is the base class for all
   * drawing patterns. The actual content of such a pattern, and if
   * it is designed for lines or filled shapes, is defined in the
   * subclass.
   * Common property of all patterns include its bounding box,
   * which is a rectangle that encloses all objects.
   * Patterns may be binary, monochromatic, or colored.
   * These classes are used by the drawer classes.
   */
  class graphicsPattern : public ioObject {
  public:

    /**
     * A comparator used for sorting patterns
     */
    struct less {
      bool operator()(const graphicsPattern& a, const graphicsPattern& b) const {
        return a.compareTo(b) < 0;
      }
    };


    struct pless {
      bool operator()(const graphicsPattern* a, const graphicsPattern* b) const {
        return a->compareTo(*b) < 0;
      }
    };

    /**
     * default constructor creates an empty pointList;
     */
    graphicsPattern();

    /**
     * destructor
     */
    virtual ~graphicsPattern();

    /**
     * returns the name of this class: graphicsPattern
     */
    const char* getTypeName() const {return "graphicsPattern";};

    /**
     * Resets this pattern to its default state.
     */
    virtual void clear()=0;


    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source pointList to be copied.
     */
    graphicsPattern& copy(const graphicsPattern& other);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    graphicsPattern& operator=(const graphicsPattern& other);

    /**
     * comparison operator (alias for compareTo(other) == 0).
     * @param other the pattern to be compared
     * @return true if both are equal
     */
    bool operator==(const graphicsPattern& other) const {
      return compareTo(other) == 0;
    }

    /**
     * comparison operator (alias for compareTo(other) != 0).
     * @param other the pattern to be compared
     * @return true if both are unequal
     */
    bool operator!=(const graphicsPattern& other) const {
      return compareTo(other) != 0;
    }

    /**
     * create a clone of this pointList
     * @return a pointer to a copy of this pointList
     */
    virtual object* clone() const=0;

    virtual int compareTo(const graphicsPattern& other) const=0;

    /**
     * Returns the bounding box of this pattern.
     */
    virtual rectangle getBoundingBox() const=0;

    /**
     * write the pattern to the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the pattern from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

  };

  /**
   * Line pattern class. A line pattern is simply an array
   * of T, encoding "on" and "off", and an offset, which determines
   * the start.
   * Example:
   * \code
   * int p1[4]={2,3,1,2};
   * ivector p(4,p1);
   * int offset=0;
   * linePattern<int> pat(p,offset);
   * \endcode
   * Then applying this pattern to a line drawing will result in
   * the following line:
   * \code
   * **   *  **   *  **   *  ** ...
   * \endcode
   */
  class linePattern: public graphicsPattern {

  public:

    /**
     * Default constructor. Creates a pattern for a continous line.
     */
    linePattern();

    /**
     * Constructor. Constructs a line pattern with the given
     * dash intervals and the given offset.
     * @param d vector containing the dash definition
     * @param o the dash offset
     */
    linePattern(const ivector& d, int o);

    /**
     * Copy constructor.
     */
    linePattern(const linePattern& o);

    /**
     * Destructor.
     */
    virtual ~linePattern();

    /**
     * Returns the bounding box of this pattern. This is not really
     * useful except that the returned rectangle's width gives the
     * length of one pattern.
     */
    virtual rectangle getBoundingBox() const;

    /**
     * This is the main application function for pixel-base draw functors.
     * The method should be
     * called with an argument denoting the number of pixel that
     * is supposed to be drawn. The method then returns true
     * if the pixel is permitted by the pattern, or not.
     * @param i the index of the pixel to be drawn.
     */
    inline bool isSet(int i) const {
      return mask.at(i%length);
    }

    /**
     * Vector-oriented draw functors can use this method to obtain the
     * dash definition.
     * @param d here the dash definition is  returned
     * @param o will receive the offset of the pattern
     */
    void getDash(ivector& d, int& o) const {
      d=dash;
      o=offset;
    }

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source pointList to be copied.
     */
    linePattern& copy(const linePattern& other);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the pointList to be copied
     * @return a reference to the actual pointList
     */
    inline linePattern& operator=(const linePattern& other) {
      return copy(other);
    }

    virtual object* clone() const;

    virtual int compareTo(const graphicsPattern& other) const;

    virtual void clear();

    /**
     * write the pattern to the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the pattern from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    /**
     * This method returns a line pattern suitable for dashed lines.
     * @param d the width of the intervals.
     */
    static linePattern* createDashed(int d);

  protected:
    ivector dash;
    genericVector<bool> mask;
    int length;
    int offset;
  };

  /**
   * Fill pattern class. A fill pattern is defined by a set of
   * graphical primitives. At present, the only supported
   * primitives are borderPoints, areaPoints, and polygonPoints.
   *
   */
  class fillPattern: public graphicsPattern {

  public:
    /**
     * The type of mask objects that should be used. At present,
     * only polygon is supported.
     */
    typedef enum {
      border,
      polygon
    } contourType;

    typedef tpointList<float> basicObject;

    /**
     * The representation of a mask in vector form. The list
     * contains pointers to point list objects (maybe different
     * subtypes of pointlist). Once added to the mask, the
     * objects must not be modified.
     */
    typedef std::list<const basicObject*> objectList;


    /**
     * Default constructor. This will create a homogeneous area.
     */
    fillPattern();

    /**
     * Copy constructor.
     */
    fillPattern(const fillPattern& other);

    /**
     * Constructor. The given pointList is cast to the type given
     * as argument.
     *
     * @param base the base object of the pattern.
     * @param type the type of created contour. If you choose, e.g.
     *        Polygon, then all objects that are added get converted to
     *        polygons.
     * @param scale
     */
    fillPattern(const basicObject& base, contourType type, float scale=1.0);

    /**
     * Destructor.
     */
    virtual ~fillPattern();

    /**
     * Adds a contour to the pattern. The given contour gets converted
     * to the type defined in the constructor.
     */
    void add(const basicObject& o);

    /**
     * Sets the pattern mask explicitly. The mask is only used
     * for pixel-based drawing where the vector format usually
     * yields crap.
     */
    template <class T>
    void setMask(const matrix<T>& m) {
      mask.castFrom(m);
      pixeldim =mask.size();
    }

    // void intersect(const fillPattern& fp);

    // void unite(const fillPattern& fp);

    /**
     * Returns true if the given point is allowed by this pattern
     * mask.
     */
    bool isSet(int x,int y) {
      return (mask.at(y%pixeldim.y,x%pixeldim.x) != 0);
    }

    /**
     * Returns true if the given point is allowed by this pattern
     * mask.
     */
    bool isSet(const point& p) {
      return isSet(p.x,p.y);
    }

    /**
     * Returns the list of objects that constitute the pattern.
     */
    const objectList& getObjects() const {
      return objects;
    }

    /**
     * Returns the bounding box of this pattern.
     */
    virtual rectangle getBoundingBox() const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source basicObject to be copied.
     */
    fillPattern& copy(const fillPattern& other);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the basicObject to be copied
     * @return a reference to the actual basicObject
     */
    inline fillPattern& operator=(const fillPattern& other) {
      return copy(other);
    }

    /**
     * create a clone of this basicObject
     * @return a pointer to a copy of this basicObject
     */
    virtual object* clone() const;

    virtual int compareTo(const graphicsPattern& other) const;

    virtual void clear();

    /**
     * write the pattern to the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete = true) const;

    /**
     * read the pattern from the given ioHandler
     */
    virtual bool read(ioHandler& handler,const bool complete = true);

    /**
     * Static method to create a hatch pattern. dx and dy
     * determine two things: The width and height of the returned pattern,
     * and also the slope of the lines. There is only one line per
     * pattern.
     * Examples:
     * \code
     * createHatching(2,2,1);
     * \endcode
     * will create a hatching that looks about:
     * \code
     *  *
     * *
     * \endcode
     * whereas
     * \code
     * createHatching(2,-2,1);
     * \endcode
     * will create a hatching that looks like:
     * \code
     * *
     *  *
     * \endcode
     * A more complex example:
     * \code
     * createHatching(16,4,2);
     *         ********
     *     ********
     * ********
     * ****        ****
     * \endcode
     */
    static fillPattern* createHatching(int dx, int dy, int w);

    static fillPattern* createCrossHatching(int dx, int dy, int w);

    typedef tpoint<float> pointType;
    typedef tpolygonPoints<float> internalObject;


  protected:
    /**
     * Updates the pixel mask from the list of mask objects.
     * Also recomputes the bounding box.
     */
    void updateMask();

    /**
     * Draws a filled polygon into the given mask
     */
    template <class T>
    void drawObject(matrix<T>& pane, const internalObject& p);

    rectangle bb;
    point dim;
    point pixeldim;
    objectList objects;
    // matrix for pixel-based drawing
    matrix<ubyte> mask;
  };



  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,graphicsPattern& plst,
            const bool complete=true);

  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler, const graphicsPattern& plst,
             const bool complete=true);


} // namespace lti

namespace std {

  std::ostream& operator<<(std::ostream& s,const lti::graphicsPattern& pts);
}

#endif

