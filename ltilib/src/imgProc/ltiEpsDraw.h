/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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

/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiEpsDraw.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 20.7.2002
 * revisions ..: $Id: ltiEpsDraw.h,v 1.23 2005/02/02 14:14:17 doerfler Exp $
 */
#ifndef _LTI_EPS_DRAW_H
#define _LTI_EPS_DRAW_H

#ifdef _LTI_MSC_6
#pragma warning ( disable : 4786 )
#endif

#include <vector>
#include <string>
#include <fstream>
#include <list>
#include <map>

#include "ltiDrawBase.h"

#include "ltiTypes.h"
#include "ltiHTypes.h"
#include "ltiCMYKColor.h"
#include "ltiColors.h"
#include "ltiContour.h"
#include "ltiIoHandler.h"
#include "ltiLocation.h"
#include "ltiDataCodec.h"


namespace lti {

  /**
   * Object for drawing lines and points and geometric objects
   * to an (E)PS file.
   *
   * Example:
   *
   * if you want to draw some rectanglees, you can create an instance of
   * epsDraw-object this way:
   *
   * \code
   * lti::epsDraw<cmykColor> drawer(std::cout);      // dump EPS to stdout
   *
   * drawer.rectangle(10,10,200,200); // draw a large rectangle
   * drawer.rectangle(50,50,100,150); // and a smaller one
   * \endcode
   * The natural coordinate system uses points (pt, 1pt=1/72 inch) as basic
   * unit. All values are specified in pt.
   */
template<class T>
  class epsDraw : public drawBase<T> {
  public:


    /**
     * This is used for setting the vertical alignment of text.
     */
    enum vAlignType {
      valignBottom,    ///< reference point is the bottom line of the text
      valignMiddle,    ///< reference point is the middle of the text
      valignTop        ///< reference point is the top of the text
    };

    /**
     * This is used for setting the horizontal alignment of text.
     */
    enum alignType {
      alignLeft,       ///< reference point is at the left
      alignCenter,     ///< reference point is at the left
      alignRight       ///< reference point is at the left
    };


    /**
     * The encoding used for embedded bitmaps.
     */
    enum imageCoding {
      asciiHex,          ///< uses ASCII Hex digits (simple, but huge)
      ascii85,           ///< uses ASCII Hex digits (a little bit more efficient)
      runLength,         ///< uses runlength compression
#if HAVE_LIBZ
      flate,             ///< uses zLib deflate compression (Postscript Level 3 only)
#endif
      jpeg               ///< uses JPEG compression
    };



    /**
     * The width of an ISO A4 sheet in points (1pt = 1/72in)
     */
    static const int a4Width;
    /**
     * The height of an ISO A4 sheet in points (1pt = 1/72in)
     */
    static const int a4Height;

    /**
     * The default resolution of the PostScript interpreter
     */
    static const float defaultResolution;

    /**
     * Constructor. It gets a drawing area, the bounding box.
     * The stuff that is drawn is scaled to that area if
     * scale is true. Otherwise, it is simply clipped.
     */
    epsDraw(std::ostream &os, const point& drawingArea,
            bool scale=true, bool eps=true);

    /**
     * Constructor. It determines the size of the bounding box
     * from the stuff that is being drawn.
     */
    epsDraw(std::ostream &os, bool scale=true, bool eps=true);

    /**
     * destructor
     */
    ~epsDraw();

    /**
     * Set the title of this drawing.
     */
    void setTitle(const std::string &t) {
      title=t;
    }

    /**
     * Set the font size that is used for text. At present, only
     * a single font size is allowed in each EPS document.
     */
    void setFontSize(int t) {
      fontSize=t;
    }

    /**
     * get name of this type.
     */
    const char* getTypeName() const;

    /**
     * Specifies the line width. Default is 0, which is the thinnest
     * line an output device can generate.
     */
    void setLineWidth(float f);

    /**
     * Specifies the clipping rectangle and enables clipping.
     * If not called, there is no clipping.
     */
    void setClip(const trectangle<int>& rc);

    /**
     * Specifies the size of the drawing area in pt. The complete figure
     * is scaled and moved such that it fits tightly into a box of this size.
     */
    void setSize(const point& dim);
    
    /**
     * Specifies the resolution of the figure.  By default, each pixel
     * is considered an PostScript point (1/72 inch). By setting the,
     * resolution, you can enlarge or shrink the entire EPS figure.
     * This value can be set independently from the remaining parameters,
     * most notably the transform, because it only involves the
     * PostScript coordinate to device coordinate conversion, not the
     * image coordinate to PostScript coordinate conversion.
     */
    void setResolution(float r);

    /**
     * Adds a comment to the (E)PS file.
     */
    void addComment(const std::string& msg);

    /**
     * returns the size of the drawing area
     */
    point getCanvasSize();


    /**
     * Sets the transform for the coordinates. With this, you
     * override the default transform.
     */
    void setTransform(const tpoint<float>& offset=tpoint<float>(0.0f,0.0f),
                      const tpoint<float>& scale=tpoint<float>(1.0f,1.0f));

    /**
     * Specifies color to be used.
     */
    void setColor(const T& color);

    void setColor(const char* color);

    /**
     * Specifies grayscale level to be used (range from 0 to 1) for
     * the next graphics objects. If you want to specify a gray
     * value, you should always use this method instead of setColor
     * with grey RGB values. The reason for this is a PostScript
     * convention: Most color printers mix colors by subtractive
     * colors. If you specify RGB, you will get black mixed out of
     * the colors of the printer. If you specify the grey value by
     * setGray, you will get simple black ink. (An alternative
     * is to use setColor with cmyk values, setting cmy to zero and
     * k to the gray level.
     */
    void setGray(const float k);


    /**
     * Set pixel at x,y.
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     */
    void set(const int x, const int y);

    /**
     * Set pixel at p.
     */
    void set(const point& p) {
        set(p.x,p.y);  }

    /**
     * Set pixels at all positions in c
     */
    void set(const pointList& c, const point& offset=point(0,0)) {
      drawBase<T>::set(c,offset);    }

     /**
     * Set pixel at p.  If no style is given, the color and symbol
     * type set by the respective methods are used, else the style
     * given as an argument is used.
     * @param p coordinates of the pixel to be set
     */
    template<class U>
    void set(const hPoint2D<U>& p)
    {set(p.x/p.h,p.y/p.h);};

    /**
     * Draw a location.
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    void set(const location& loc,const bool showAngleLine = false) {
          drawBase<T>::set(loc,showAngleLine);
    }

    /**
     * Draws a rectLocation
     *
     * @param loc location with position, angle and radius
     * @param showAngleLine if true, a line from the middle point of the
     *        location (given by loc.position) with the angle given by
     *        loc.angle will be drawn.
     */
    void set(const rectLocation& loc,const bool showAngleLine = false) {
        drawBase<T>::set(loc,showAngleLine);
    }

    /**
     * Draws a list of locations
     */
    void set(const std::list<location>& locs,
             const bool showAngleLine = false) {
        drawBase<T>::set(locs,showAngleLine);
    }

    /**
     * Draw the contents of the vector using the whole image
     * @param vct the vector to be shown
     * @param forceAxis0 if true, both axis will be shown, i.e. the value 0 for
     *                  the vector will be always shown.  If false, only the
     *                  required value-range will be shown
     */
    void set(const vector<int>& vct,
             const bool& forceAxis0 = true);

    /**
     * Draws an image, using RGB color pixels. The image is drawn
     * in the coordinate space from (offset.x,offset.y) to
     * (offset.x+img.columns,offset.y+img.rows)
     */
    void drawImage(const image& img,
                   const imageCoding code=ascii85,
                   const point& offset=point(0,0));

    /**
     * Draws an image, using grayscale pixels. The image is drawn
     * in the coordinate space from (offset.x,offset.y) to
     * (offset.x+img.columns,offset.y+img.rows)
		 * If blackWhite is set to true, the resulting image will be
		 * pure black-and-white. Which pixel is black is defined by t,
		 * all values < t whill appear white, all values > t will appear black.
     */
    inline void drawImage(const channel& img,
                          const imageCoding code=ascii85,
                          const point& offset=point(0,0),
                          bool blackWhite=false,
                          const float t=0.5) {
      channel8 tmp;
      tmp.castFrom(img);
      drawImage(tmp,code,offset,blackWhite,int(t*256+0.5));
    }

    /**
     * Draws an image, using grayscale pixels. The image is drawn
     * in the coordinate space from (offset.x,offset.y) to
     * (offset.x+img.columns,offset.y+img.rows)
		 * If blackWhite is set to true, the resulting image will be
		 * pure black-and-white. Which pixel is black is defined by t,
		 * all values < t whill appear white, all values > t will appear black.
     */
    void drawImage(const channel8& img,
                   const imageCoding code=ascii85,
                   const point& offset=point(0,0),
                   bool blackWhite=false,
                   const int t=128);

    /**
     * Draws a polygon.
     */
    void polygon(const point plist[], int n, bool closed=true, bool filled=false);

    /**
     * Draw a line from the point (fx,fy) to point (tx,ty).
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     */
    void line(const int fx, const int fy,
              const int tx, const int ty);

    /**
     * Draw a line from the point p to point p2.
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     */
    void line(const point& p1,const point& p2) {
      line(p1.x,p1.y,p2.x,p2.y);
    };

    /**
     * Draw a line from the last point to (x,y).
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     */
    void lineTo(const int x, const int y);

    /**
     * Draw a line from the last point to point p.
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     */
    void lineTo(const point& p) {lineTo(p.x,p.y);};

    /**
     * Draws a grid in the image.
     * The interpretation of delta depends on the value of interval.
     * if interval is true, the values are taken as number of pixels
     * between two grid lines in x and y direction. Otherwise, it
     * is used as number of grid lines. For drawing, the alternate
     * color is used.
     */
    void grid(const point& delta, const bool interval=true);

    /**
     * Draws a vertical line from (x,y1) to (x,y2).
     */
    void verticalLine(const int x, const int y1, const int y2);

    void verticalLine(const point& p1, const point& p2) {
      verticalLine(p1.x,p1.y,p2.y);
    }

    /**
     * Draws a horizontal line from (x1,y) to (x2,y)
     */
    void horizontalLine(const int x1, const int x2, const int y);

    void horizontalLine(const point& p1, const point& p2) {
      horizontalLine(p1.x,p2.x,p1.y);
    }


    /**
     * draw a box
     *
     * \deprecated use rectangle(int, int, int, int, const bool&) instead!
     *
     */
    void box(const int x1, const int y1, const int x2, const int y2,
             const bool& filled=false) {
      rectangle(x1,y1,x2,y2,filled);
    }

    /**
     * draw a box
     *
     * \deprecated use rectangle(rectangle&, const bool&) instead!
     * 
     * @see box(int,int,int,int,const bool&)
     */
    void box(const trectangle<int>& r, const bool& filled=false)
    {box(r.ul.x,r.ul.y,r.br.x,r.br.y,filled);}

    /**
     * draw a box
     *
     * \deprecated use rectangle(point&, point&, const bool&) instead!
     * 
     * @see box(int,int,int,int,const bool&)
     */
    void box(const point& upperLeft, const point& bottomRight,
             const bool& filled=false)
    {box(upperLeft.x,upperLeft.y,bottomRight.x,bottomRight.y,filled);}

    /**
     * draw a rectangle
     *
     * @param x1 left x-coordinate.
     * @param y1 upper y-coordinate.
     * @param x2 right x-coordinate.
     * @param y2 bottom y-coordinate.
     * @param filled if true box is filled
     */
    void rectangle(const int x1, const int y1, const int x2, const int y2,
             const bool& filled=false);

    /**
     * draw a rectangle.
     * the rectangle must contain the upper left and the bottom right point
     * @param r rectangle to be drawn
     * @param filled if true rectangle is filled
     */
    void rectangle(const trectangle<int>& r, const bool& filled=false)
    {rectangle(r.ul.x,r.ul.y,r.br.x,r.br.y,filled);}

    /**
     * draw a rectangle
     * @param upperLeft upper left corner of the rectangle
     * @param bottomRight bottom right corner of the rectangle
     * @param filled if true rectangle is filled
     */
    void rectangle(const point& upperLeft, const point& bottomRight,
             const bool& filled=false)
    {rectangle(upperLeft.x,upperLeft.y,bottomRight.x,bottomRight.y,filled);}

    /**
     * draw a circle with circle center 'p1' and radius 'r'
     */
    void circle(const point& p1,const int r, const bool& filled=false);

    /**
     * draw a circle with circle center '(x,y)' and radius 'r'
     */
    void circle(const int x,const int y,const int r, const bool& filled=false){
        circle(point(x,y),r,filled);    };

    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     */
    void ellipse(const point& p1,const int aX, const int aY,
                 const bool& filled=false);

    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     * @see ellipse()
     * @param p1 center of the ellipse
     * @param aX length of the main axis
     * @param aY length of the secondary axis
     * @param angle between the main axis and the horizontal axis
     * @param filled indicates if the ellipse should be draw filled or not
     */
    void ellipse(const point& p1,const int aX, const int aY,
                 const float& angle, const bool& filled=false);

    /**
     * draw an arc from 'pA' to 'pB', clockwise around center 'p1'.
     */
    void arc(const point& p1, const point& pA, const point& pB);

    /**
     * draw an integer <code>num</code> at position 'p1'. The alignment
     * is left/bottom.
     * @see text()
     * @param num the number to be written
     * @param p1 the position where the number will be drawn
     */
    void number(const int num,
                const point& p1 = point(0,0));

    /**
     * draw an integer <code>num</code> at position 'x'/'y'.
     * @see text()
     */
    void number(const int num,
                const int x,
                const int y);

    /**
     * Draw the given text at the position <code>upperleft</code>.
     * @param txt the output text
     * @param pos the position where the text will be written
     * @param a the alignType of the horizonal alignment.
     * @param b the vAlignType of the vertical alignment.
     */
    void text(const std::string txt,
              const point& pos = point(0,0),
              const alignType a=alignLeft,
              const vAlignType b=valignTop) {
      text(txt,pos.x,pos.y,a,b);
    }

    /**
     * Draw the given text at the position <code>upperleft</code>.
     * @param txt the output text
     * @param x the x coordinate in the image where the text will be
     *                written
     * @param y the y coordinate in the image where the text will be
     *                written
     * @param a the alignType of the horizonal alignment.
     * @param b the vAlignType of the vertical alignment.
     */
    void text(const std::string txt,
              const int x, const int y,
              const alignType a,
              const vAlignType b);


    /**
     * Draw the given text at the position <code>upperleft</code>.
     * @param txt the output text
     * @param x the x coordinate in the image where the text will be
     *                written
     * @param y the y coordinate in the image where the text will be
     *                written
     */
    void text(const std::string txt,
              const int x, const int y) {
        text(txt,x,y,alignLeft,valignTop);
    }

    /**
     * draw an arrow. arrow tip will be at (tx,ty).
     * If size<1.0 then tipsize will be the relative portion of arrow length.
     * If size>1.0 then tipsize will be (int)size, independent of arrow length.
     */
    void arrow(const int fx, const int fy,
               const int tx, const int ty,
               const float& size=0.2f);


    void arrow(const point& p1,const point& p2,const float& size=0.2f){
      arrow(p1.x,p1.y,p2.x,p2.y,size);
    };

    /**
     * Closes this object. At this time, all Postscript objects are
     * written to the output stream.
     */
    void close();

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


  protected:
    /**
     * Returns the name of the given line pattern.
     * This name can be used as
     * PostScript command to switch to the given pattern. The method
     * will ensure that the PS header contains the required command
     * definition.
     */
    const std::string& getLinePatName(const linePattern& c);

    /**
     * Returns the name of the given fill pattern.
     * This name can be used as
     * PostScript command to switch to the given pattern. The method
     * will ensure that the PS header contains the required command
     * definition.
     */
    const std::string& getFillPatName(const fillPattern& c);

    /**
     * Returns the name of the color c. This name can be used as
     * PostScript command to switch to the given color. The method
     * will ensure that the PS header contains the required command
     * definition.
     */
    const std::string& getColorName(const rgbColor& c);


    /**
     * Returns the name of the color c. This name can be used as
     * PostScript command to switch to the given color. The method
     * will ensure that the PS header contains the required command
     * definition.
     */
    const std::string& getColorName(const cmykColor& c);


    /**
     * draws the actual symbol at the actual position actX,actY
     */
    void drawSymbol(const int x,const int y);

    void drawSymbol(const int x, const int y,const int width,
                    const typename drawBase<T>::eMarkerType t);

    void drawSymbol(const int x, const int y,const int width,const char* pT);

  protected:

    /**
     * this is a list of strings that contain the postscript command
     * sequence for a graphical object
     */
    std::list<std::string> objects;

    /**
     * this is a map from colors to the corresponding postscript command.
     */
    std::map<rgbColor,std::string> rgbColorNames;

    /**
     * this is a map from colors to the corresponding postscript command.
     */
    std::map<cmykColor,std::string> cmykColorNames;

    /**
     * A map for storing line pattern definitions.
     */
    std::map<linePattern, std::string, graphicsPattern::less> linePatternNames;

    /**
     * A map for storing fill pattern definitions.
     */
    std::map<fillPattern, std::string, graphicsPattern::less> fillPatternNames;

    // the number of colors
    int colorCount;

    // the number of fill patterns
    int fillpatCount;

    // the number of line patterns
    int linepatCount;

    // two temporary buffers that are used in the methods
    std::string tmp;
    char numBuffer[256];

    // the number of graphical objects
    int objCount;

    // the clipping rectangle, which is used only if doClip==true
    trectangle<int> clipRect;
    bool doClip;

    // this is not the EPS bounding box, but the
    // bounding box of the stuff that has been drawn.
    // the EPS bounding box is always set to that which has been
    // set to the drawing area
    trectangle<int> bbox;

    // the size of the drawing area in pt
    point dimension;

    // the coordinate transform from user coordinates to pt
    float xScale,yScale, xOffset, yOffset;
    bool autoScale;

    // flag denoting if we want an EPS file or a printable page.
    bool isEps;
    // the output stream
    std::ostream& out;
    // the currently active color
    std::string currentColor;

    // the currently active line pattern
    std::string currentlpat;

    // the currently active fill pattern
    std::string currentfpat;

    // the title of the figure
    std::string title;
    // the global font size
    int fontSize;

    // the resolution in dots per point (dots per 1/72 inch)
    // this value is used at two points: 1. the computation of the
    // bounding box and 2. for computing the final scale variables
    float resolution;

    // contains the names of the postscript command
    static const char* alignCmd[3][3];

    // internal stuff

    // expands the bounding box to include the given point, if necessary
    void adjustBoundingBox(int x, int y);

    // expands the bounding box to include the given point, if necessary
    inline void adjustBoundingBox(const point& p) {
      adjustBoundingBox(p.x,p.y);
    }

    // expands the bounding box to include the given points, if necessary
    inline void adjustBoundingBox(int x1, int y1, int x2, int y2) {
      adjustBoundingBox(x1,y1);
      adjustBoundingBox(x2,y2);
    }

    // expands the bounding box to include the given points, if necessary
    inline void adjustBoundingBox(const point& p1, const point& p2) {
      adjustBoundingBox(p1);
      adjustBoundingBox(p2);
    }

    // sets a new active point, initializes the given string
    void moveTo(std::string& tmp, int x, int y, bool init=true);
    // appends a line segment to the path in tmp
    void lineTo(std::string& tmp, int x, int y);
    // appends a line segment to the path in tmp
    void rlineTo(std::string& tmp, int x, int y);
    // sets a new active point, initializes the given string
    void moveTo(std::string& tmp, float x, float y, bool init=true);
    // appends a line segment to the path in tmp
    void lineTo(std::string& tmp, float x, float y);
    // appends a line segment to the path in tmp
    void rlineTo(std::string& tmp, float x, float y);
    // closes the path in tmp
    void closePath(std::string& tmp);

    // adds a command
    inline void addCmd(const std::string& x) {
      objects.push_back(x);
    }

    // strokes the path in x
    inline void stroke(const std::string& x) {
      objects.push_back(x+" s");
    }

    // fills the path in x
    inline void fill(const std::string& x) {
      objects.push_back(x+" f");
    }

    // returns the bounding box in a format as in the PS header
    std::string getBoundingBox();

    // dumps the PS header
    void writeHeader();

    // dumps the prolog
    void writeProlog();

    // dumps the color definitions
    void writeColorDefinitions();

    // dumps the color definitions
    void writePatternDefinitions();

    // computes the transform from user coordinates to pt
    void makeTransform();

    // writes the EPS setup
    void writeSetup();

    // writes the EPS trailer
    void writeTrailer();

    // writes the graphical objects
    void writeObjects();

    // issues a PS comment
    inline void cmt(const std::string& x) {
      out << "% " << x << std::endl;
    }

    // issues a PS command
    inline void cmd(const std::string& x) {
      out << x << std::endl;
    }

    // issues a PS command with a gsave/grestore wrapper
    inline void unsafecmd(const std::string& x) {
      out << "gsave\n" << x << std::endl << "grestore\n";
    }

    // inserts a dsc comment
    inline void dsc(const std::string& x) {
      out << "%%" << x << std::endl;
    }

    // inserts a dsc comment with argument
    inline void dsc(const std::string& x, const std::string& arg) {
      out << "%%" << x << ": " << arg << std::endl;
    }

    // binds and defines a command
    inline void bdef(const std::string& name, const std::string& def) {
      cmd("/"+name+" { "+def+" } bd");
    }

    // loads and defines a command
    inline void ldef(const std::string& name, const std::string& def) {
      cmd("/"+name+" /"+def+" ld");
    }

    // makes a definition
    inline void def(const std::string& name, const std::string& def) {
      cmd("/"+name+" "+def+" def");
    }

    bool addImageCommand(bool color, int width, int height,
                         imageCoding comp, const point& offset,
                         const dataCodec::buffer& src,
                         const bool blackWhite=false);

  };
}

//#include "ltiEpsDraw_template.h"

#endif

