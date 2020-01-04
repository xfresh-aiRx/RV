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
 * file .......: ltiDrawBase.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 19.11.2002
 * revisions ..: $Id: ltiDrawBase.h,v 1.13 2005/06/16 12:40:23 doerfler Exp $
 */

#ifndef _LTI_DRAW_BASE_H_
#define _LTI_DRAW_BASE_H_

#include "ltiObject.h"
#include "ltiStatus.h"
#include "ltiTypes.h"
#include "ltiHTypes.h"
#include "ltiImage.h"
#include "ltiContour.h"
#include "ltiIoHandler.h"
#include "ltiLocation.h"
#include "ltiLinearKernels.h"
#include "ltiGraphicsPattern.h"


namespace lti {

  /**
   *  Abstract parent class for draw and epsDraw.
   *  This class provides all methods that are implemented in ltiDraw and
   *  ltiEpsDraw. It makes a base class availabel with all generall drawing
   *  methods. Some special methods, are not implemented in this base class.
   *  The class is aiming at providing a base class, so that, according to
   *  the given parameter, a picture is drawn or an eps file written. <br>
   */
template<class T>
  class drawBase : public object, public status {
  public:

    /**
     * Specifies different types of marker which can be defined for
     * the marker-method.
     *
     * @see setMarkerType, setStyle
     */
    enum eMarkerType {
      Pixel = 0,     /**< Pixel          ('.') */
      Circle,        /**< Circle         ('o') */
      Xmark,         /**< Xmark          ('x') */
      Plus,          /**< Plus           ('+') */
      Star,          /**< Star           ('*') */
      Square,        /**< Square         ('s') */
      Diamond,       /**< Diamond        ('d') */
      TriangleUp,    /**< Triangle up    ('^') */
      TriangleDown,  /**< Triangle down  ('v') */
      TriangleLeft,  /**< Triangle left  ('<') */
      TriangleRight, /**< Triangle right ('>') */
      Dot,           /**< Dot            ('#') */
      LtiLogo        /**< LTI-Logo*/
    };

    /**
     * default constructor
     */
    drawBase();


    /**
     * copy constructor
     * @param other the object to be copied
     */
    drawBase(const drawBase& other);

    /**
     * destructor
     */
    virtual ~drawBase();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    drawBase& copy(const drawBase& other);


    /**
     * returns the name of this type ("drawBase")
     */
    virtual const char* getTypeName() const;


    /**
     * Specifies grayscale level to be used (range from 0 to 1) for
     * the next graphics objects.
     */
    virtual void setGray(const float k)=0;


    /**
     * Specifies the size for the next markers.
     * Default is 5 in draw and 2 in epsDraw
     */
    virtual void setMarkerSize(const int size);

    /**
     * Specifies color to be used.
     */
    virtual void setColor(const char* color)=0;

    /**
     * Specifies color to be used.
     */
    virtual void setColor(const T& color)=0;

    /**
     * Sets the style used for drawing points. This includes the color and
     * the symbols for the %point
     * @see point(const int, const int, const char*)
     *
     * The string characterizing consists of two parts: the first
     * definining color and the second defining the symbol. These are
     * as close to the MATLAB(TM) attributes as possible. If one of the
     * parts is missing, the corresponding value remains unchanged.
     *
     * <table>
     * <tr>
     * <th>Colors</th> <th width="10%" > <br></th> <th>Symbols</th>
     * </tr>
     * <tr> <td>
     * <table>
     * <tr><td>y</td><td>yellow</td></tr>
     * <tr><td>m</td><td>magenta</td></tr>
     * <tr><td>c</td><td>cyan</td></tr>
     * <tr><td>r</td><td>red</td></tr>
     * <tr><td>g</td><td>green</td></tr>
     * <tr><td>b</td><td>blue</td></tr>
     * <tr><td>w</td><td>white</td></tr>
     * <tr><td>k</td><td>black</td></tr>
     * </table>
     * </td> <td> <br> </td>
     * <td>
     * <table>
     * <tr><td>.</td><td>point/pixel</td></tr>
     * <tr><td>o</td><td>circle</td></tr>
     * <tr><td>x</td><td>x-mark</td></tr>
     * <tr><td>+</td><td>plus</td></tr>
     * <tr><td>*</td><td>star</td></tr>
     * <tr><td>s</td><td>square</td></tr>
     * <tr><td>d</td><td>diamond</td></tr>
     * <tr><td>^</td><td>triangle (up)</td></tr>
     * <tr><td>v</td><td>triangle (down)</td></tr>
     * <tr><td><</td><td>triangle (left)</td></tr>
     * <tr><td>></td><td>triangle (right)</td></tr>
     * <tr><td>#</td><td>dot</td></tr>
     * </table>
     * </td></tr>
     * </table>
     *
     * In addition all colors except for black can be darkened by a
     * number between 0 and 9 following the color letter. The default
     * is 0. The higher the number, the darker the color. Also the
     * closed symbols are available in a filled version. This is
     * activated by an 'f' following the symbol specifier.
     *
     * Colors only work properly if the canvas used is of type image,
     * ie using rgbPixel as template type. Otherwise, gray-levels are
     * used instead of the colors. Here, 'c', 'm', and 'y' result in
     * the same gray-level as do 'r', 'g' and 'b'. For better
     * predictability use white with different factors for getting
     * gray-levels.
     *
     * A change of the color using setColor(const char*) or
     * setColor(const T&) does not affect the type of symbol being
     * used.
     */
    virtual void setStyle(const char* style);

    /**
     * Specifies the marker type to be used for points
     */
    virtual void setMarkerType(const eMarkerType m);
   

    /**
     * Sets the pattern for drawing lines. All subsequent lines
     * will be drawn in the given pattern. The default is the
     * continuous line.
     * @param pat pattern for drawing lines.
     */
    virtual void setLinePattern(const linePattern& pat);

    /**
     * Sets the pattern for filling areas. All subsequenly filled
     * areas will be filled with the given pattern. The default
     * pattern is a homogenouos fill.
     * @param pat pattern for filling areas.
     */
    virtual void setFillPattern(const fillPattern& pat);

    /**
     * returns the size of the drawing area.
     */
    virtual point getCanvasSize()=0;

    /**
     * sets a single pixel at point (x,y)
     * In epsDraw this pixel is a small square.
     */
    virtual void set(const int x,const int y)=0;

    /**
     * sets a single pixel at p
     */
    virtual void set(const point& p) {
        set(p.x,p.y);
    };

    /**
     * sets a single pixel at p
     */
    template<class U>
    void set(const hPoint2D<U>& p)
    {set(p.x/p.h,p.y/p.h);};

    /**
     * sets pixels at all points in c, moved by the given offset
     */
    virtual void set(const pointList& c, const point& offset=point(0,0));

    /**
     * draw a location.
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    virtual void set(const location& loc,const bool showAngleLine = false);

    /**
     * draw a rectLocation
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    virtual void set(const rectLocation& loc,const bool showAngleLine = false);

    /**
     * draws a list of locations
     */
    virtual void set(const std::list<location>& locs,
             const bool showAngleLine = false);


    /**
     * Set marker at point (x,y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     */
   virtual void marker(const int x,const int y) {
        marker(x,y,actSize,actSymbol);    };

    /**
     * Set marker at point (x,y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param width the size of the marker symbol
     */
    virtual void marker(const int x,const int y,const int width) {
        marker(x, y, width, actSymbol);    };

    /**
     * Set marker at point (x,y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param style the style of the marker symbol.
     */
    virtual void marker(const int x,const int y,const char* style) {
        marker(x,y,actSize,style);    };

    /**
     * Set marker at point (x,y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param t the type of the marker
     */
    virtual void marker(const int x,const int y,const eMarkerType t) {
        marker(x,y,actSize,t);    };

    /**
     * Set marker at point (x,y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param w the size of the marker symbol
     * @param style the style of the marker symbol.
     */
    virtual void marker(const int x,const int y,const int w,const char* style){
        drawSymbol(x,y,w,style);  };

    /**
     * Set marker at point (x,y).
     * If there is no style, markerType or width is given, the value
     * set by the setStyle or setWidth is used.
     * @param x the x coordinate
     * @param y the y coordinate
     * @param w the size of the marker symbol
     * @param t the type of the marker
     */
    virtual void marker(const int x,const int y,const int w, eMarkerType t)
       {drawSymbol(x,y,w,t);};

    /**
     * Set marker at p.
     * @see marker(int,int)
     */
    virtual void marker(const point& p) {
        marker(p.x,p.y,actSize,actSymbol);    };

    /**
     * Set marker at p with given width.
     * @see marker(int,int)
     */
    virtual void marker(const point& p,const int width) {
        marker(p.x, p.y, width, actSymbol);    };

    /**
     * Set marker at p with given style
     * @see marker(int,int)
     */
    virtual void marker(const point& p, char* style) {
        marker(p.x,p.y,actSize,style);    };

    /**
     * Set given marker type at p.
     * @see marker(int,int)
     */
    virtual void marker(const point& p, eMarkerType t) {
        marker(p.x,p.y,actSize,t);    };

    /**
     * Set given marker type at p with given width.
     * @see marker(int,int)
     */
    virtual void marker(const point& p, int width, eMarkerType t) {
        marker(p.x,p.y,width,t);    };

    /**
     * Set marker at p with given width and style.
     * @see marker(int,int)
     */
    virtual void marker(const point& p, int width, char* style) {
        marker(p.x,p.y,width,style);    };

    /**
     * Set marker at p.
     * @param p coordinates of the pixel to be set
     * @see marker(int,int)
     */
    template <class U>
    void marker(const hPoint2D<U>& p) {
      marker(p.x/p.h,p.y/p.h,actSize,actSymbol);    };

    /**
     * Set marker at p width given width.
     * @see marker(int,int)
     */
    template <class U>
    void marker(const hPoint2D<U>& p, const int width) {
      marker(p.x/p.h,p.y/p.h,width,actSymbol);    };

    /**
     * Set given marker type at p.
     * @see marker(int,int)
     */
    template <class U>
    void marker(const hPoint2D<U>& p, const eMarkerType t) {
      marker(p.x/p.h,p.y/p.h,actSize,t);    };

    /**
     * Set marker at p width given style.
     * @see marker(int,int,char*)
     */
    template <class U>
    void marker(const hPoint2D<U>& p, const char* style) {
      marker(p.x/p.h,p.y/p.h,actSize,style);    };

    /**
     * Set given marker type at p with given width.
     * @see marker(int,int)
     */
    template <class U>
    void marker(const hPoint2D<U>& p, const int width, const eMarkerType t) {
      marker(p.x/p.h,p.y/p.h,width,t);    };

    /**
     * Set marker at p with given width and style.
     * @see marker(int,int)
     */
    template <class U>
    void marker(const hPoint2D<U>& p, const int width, const char* style) {
      marker(p.x/p.h,p.y/p.h,width,style);    };


    /**
     * Sets markers at all positions in c,moved by offset.
     * @see marker(int,int)
     */

    virtual void marker(const pointList& c,const point& offset=point(0,0)) {
        marker(c,actSize,actSymbol,offset); };

    /**
     * Sets markers at all positions in c,moved by offset, with given width.
     * @see marker(int,int)
     */
    virtual void marker(const pointList& c, int width,
                        const point& offset=point(0,0)) {
        marker(c,width,actSymbol,offset);    };

    /**
     * Sets markers at all positions in c,moved by offset, with given style.
     * @see marker(int,int)
     */
    virtual void marker(const pointList& c, char* style,
                        const point& offset=point(0,0)) {
        marker(c,actSize,style,offset);    };

    /**
     * Sets given marker types at all positions in c,moved by offset.
     * @see marker(int,int)
     */
    virtual void marker(const pointList& c, eMarkerType t,
                        const point& offset=point(0,0)) {
        marker(c,actSize,t,offset);    };

    /**
     * Sets markers at all positions in the c, moved by offset, with given
     * width and style.
     * @see marker(int,int)
     */
    virtual void marker(const pointList& c, int width, char* style,
                        const point& offset=point(0,0));

    /**
     * Sets given marker types at all positions in the c, moved by offset,
     * with given width.
     * @see marker(int,int)
     */
    virtual void marker(const pointList& c, int width, eMarkerType t,
                        const point& offset=point(0,0));

    /**
     * Draws a line from the point (fx,fy) to point (tx,ty).
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     */
    virtual void line(const int fx, const int fy,
                      const int tx, const int ty)=0;


    /**
     * Draws a line from the point p to point p2.
     * @see line(int,int,int,int)
     */
    virtual void line(const point& p1,const point& p2) {
      line(p1.x,p1.y,p2.x,p2.y);
    }

    /**
     * Draws a line from the last point to point (x,y).
     * The "last point" will be defined with the last "set", "point", "line" or
     * "lineTo" method.
     */
    virtual void lineTo(const int x,const int y)=0;

    /**
     * Draw a line from the last point to point p.
     * @see lineTo(int,int)
     */
    virtual void lineTo(const point& p) {
      lineTo(p.x,p.y);
    }

    /**
     * Draws a vertical line from (x,y1) to (x,y2).
     */
    virtual void verticalLine(const int x, const int y1, const int y2)=0;

    /**
     * Draws a horizontal line from (x1,y) to (x2,y).
     */
    virtual void horizontalLine(const int x1, const int x2, const int y)=0;

    /**
     * Draws a vertical line from (p1.x,p1.y) to (p1.x,p2.y).
     */
    virtual void verticalLine(const point& p1, const point& p2) {
        verticalLine(p1.x,p1.y,p2.y);
    }

    /**
     * Draws a horizontal line from (p1.x,p1.y) to (p2.x,p1.y).
     */
    virtual void horizontalLine(const point& p1, const point& p2) {
        horizontalLine(p1.x,p2.x,p1.y);
    }

    /**
     * Draw a line from the point p with the length "length" and the
     * angle "angle".  The angles should be a value between -2Pi and
     * 2Pi in radian.  */
    virtual void polarLine(const point& p,const float& length,
                           const float& angle);

    /**
     * Draws a grid in the image.
     * The interpretation of delta depends on the value of interval.
     * if interval is true, the values are taken as number of pixels
     * between two grid lines in x and y direction. Otherwise, it
     * is used as number of grid lines.
     * This method is actually not implemented in epsDraw !!!
     */
    virtual void grid(const point& delta, const bool interval)=0;

    /**
     * draw a box.
     *
     * \deprecated use rectangle(int, int, int, int, const bool) instead!
     *
     * @param x1 left x-coordinate.
     * @param y1 upper y-coordinate.
     * @param x2 right x-coordinate.
     * @param y2 bottom y-coordinate.
     * @param filled if true box is filled
     */
    virtual void box(const int x1, const int y1, const int x2, const int y2,
                     const bool& filled=false) {
      rectangle(x1,y1,x2,y2,filled);
    }

    /**
     * draws a box.
     *
     * \deprecated use rectangle(point&, point&, const bool) instead!
     * 
     * @see box(int,int,int,int,const bool)
     */
    virtual void box(const point& upperLeft, const point& bottomRight,
                     const bool& filled=false) {
     box(upperLeft.x,upperLeft.y,bottomRight.x,bottomRight.y,filled);
   }

    /**
     * draw a box.
     * the rectangle must contain the upper left and the bottom right point
     *
     * \deprecated use rectangle(rectangle&, const bool) instead!
     * 
     * @see box(int,int,int,int,const bool)
     */
    virtual void box(const trectangle<int>& r, const bool& filled=false) {
      box(r.ul.x,r.ul.y,r.br.x,r.br.y,filled);
    }

    /**
     * draw a rectangle.
     * @param x1 left x-coordinate.
     * @param y1 upper y-coordinate.
     * @param x2 right x-coordinate.
     * @param y2 bottom y-coordinate.
     * @param filled if true rectangle is filled
     */
    virtual void rectangle(const int x1, const int y1, 
                           const int x2, const int y2,
                           const bool& filled=false)=0;

    /**
     * draws a rectangle.
     * @param upperLeft upper left corner of the rectangle
     * @param bottomRight bottom right corner of the rectangle
     * @param filled if true rectangle is filled
     */
    virtual void rectangle(const point& upperLeft, const point& bottomRight,
                     const bool& filled=false) {
      rectangle(upperLeft.x,upperLeft.y,bottomRight.x,bottomRight.y,filled);
   }

    /**
     * draw a rectangle.
     * the rectangle must contain the upper left and the bottom right point
     * @param r rectangle to be drawn
     * @param filled if true rectangle is filled
     */
    virtual void rectangle(const trectangle<int>& r, 
                           const bool& filled=false) {
      rectangle(r.ul.x,r.ul.y,r.br.x,r.br.y,filled);
    }

    /**
     * draw a circle with circle center 'p1' and radius 'r'
     * @param p1 center of the circle
     * @param r radius of the circle
     * @param filled if true circle is filled
     */
    virtual void circle(const point& p1,const int r, 
                        const bool& filled=false)=0;


    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     */
    virtual void ellipse(const point& p1,const int aX, const int aY,
                 const bool& filled=false)=0;

    /**
     * Draw an ellipse with center 'p1' and main axes 'aX' and 'aY'.
     * The ellipse is turned by the given angle.
     * @param p1 center of ellipse
     * @param aX radius of ellipse in the first main direction
     * @param aY radius of ellipse in the second main direction
     * @param angle angle between first main direction and x axis. Must be
     *         given in radians (rad=deg/180*Pi).
     * @param filled if true, the ellipse is filled
     */
    virtual void ellipse(const point& p1,const int aX, const int aY,
                 const float& angle, const bool& filled=false)=0;

    /**
     * draw an arc from 'pA' to 'pB', clockwise around center 'p1'.
     */
    virtual void arc(const point& p1, const point& pA, const point& pB)=0;


    /**
     * draw an arrow. arrow tip will be at (tx,ty).
     * If size<1.0 then tipsize will be the relative portion of arrow length.
     * If size>1.0 then tipsize will be (int)size, independent of arrow length.
     */
    virtual void arrow(const int fx, const int fy,
               const int tx, const int ty,
               const float& size=0.2f)=0;

    /**
     * draw an arrow. arrow tip will be at p2.
     * If size<1.0 then tipsize will be the relative portion of arrow length.
     * If size>1.0 then tipsize will be (int)size, independent of arrow length.
     */
    virtual void arrow(const point& p1,const point& p2,const float& size=0.2f){
      arrow(p1.x,p1.y,p2.x,p2.y,size);
    };

    /**
     * Draw the given text at the position x,y.
     * According to the typ of the subclass the position and the lenght of
     * the drawn text could differ
     * @param txt the output text
     * @param x the x coordinate in the image where the text will be
     *                written
     * @param y the y coordinate in the image where the text will be
     *                written
     */
    virtual void text(const std::string txt,const int x, const int y)=0;

    /**
     * Draw the given text at the position p=(x,y).
     * @see test(std::string,int,int)
     */
    virtual void text(const std::string txt,const point& p) {
        text(txt,p.x,p.y);};

    /**
     * draw an integer <code>num</code> at position 'x'/'y'.
     * According to the typ of the subclass the position and the lenght of
     * the drawn text could differ
     * @param num the number to be written
     * @param x the column where the number will be drawn
     * @param y the row where the number will be drawn
     */
    virtual void number(const int num, const int x, const int y)=0;

    /**
     * draw an integer <code>num</code> at position p='x'/'y'.
     * @see number(int,int,int)
     */
    virtual void number(const int num,const point& p) {
        number(num,p.x,p.y);};


  protected:

    /**
     * actual (last used) position X
     */
    int actX;

    /**
     * actual (last used) position Y
     */
    int actY;

    /**
     * the actual symbol that is used for markers
     */
    eMarkerType actSymbol;

    /**
     * the actual size that is used for markers
     * for epsDraw the default is 1 and for draw the default is 5
     * because the elements are much smaller and with a size smaller than 5
     * they are indistinguishable.
     */
    int actSize;

    /**
     * If true symbols are filled where possible.
     */
    bool filled;

    /**
     * The pattern for drawing lines.
     */
    linePattern linepat;

    /**
     * The pattern for filling areas.
     */
    fillPattern fillpat;

    /**
     * Flag telling the drawing methods to use the pattern.
     */
    bool useLinePattern;

    /**
     * Flag telling the drawing methods to use the pattern.
     */
    bool useFillPattern;

  private:
    /**
     * Abstract drawing methods for markers. This methods are needed by the
     * marker methods
     */
    virtual void drawSymbol(const int x,const int y,const int w,const char* style)=0;
    /**
     * Abstract drawing methods for markers. This methods are needed by the
     * marker methods
     */
    virtual void drawSymbol(const int x,const int y,const int w,const eMarkerType t)=0;


  };

}

#endif

