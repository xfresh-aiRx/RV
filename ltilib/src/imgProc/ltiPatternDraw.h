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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiPatternDraw.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 05.2.2003
 * revisions ..: $Id: ltiPatternDraw.h,v 1.3 2003/05/16 13:21:38 alvarado Exp $
 */
#ifndef _LTI_PATTERNDRAW_H
#define _LTI_PATTERNDRAW_H

#include "ltiDraw.h"

namespace lti {

  /**
   * Object for drawing lines and points in a lti::matrix with support
   * for line and fill patterns. This means that drawing lines and
   * filled areas is a little slower than the methods of the
   * draw class.
   */
  template<class T>
  class patternDraw : public draw<T> {
  public:

    /**
     * default constructor
     */
    patternDraw();
    /**
     * destructor
     */
    ~patternDraw();

    /**
     * get name of this type
     */
    const char* getTypeName() const;


    /**
     * Draw a line from the point (fx,fy) to point (tx,ty).
     * The "last point" will be defined with the last "set", "line" or
     * "lineTo" method.
     */
    void line(const int fx, const int fy,
              const int tx, const int ty);

    /**
     * Draws a vertical line from (x1,y) to (x2,y).
     */
    void verticalLine(const int x, const int y1, const int y2);

    /**
     * Draws a horizontal line from (x1,y) to (x2,y).
     */
    void horizontalLine(const int x1, const int x2, const int y);

    /**
     * draw a circle with circle center 'p1' and radius 'r'
     */
    void circle(const point& p1,const int r, const bool& filled=false);

    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     */
    void ellipse(const point& p1,const int aX, const int aY,
                 const bool& filled=false);

    /**
     * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
     */
    void ellipse(const point& p1,const int aX, const int aY,
                 const float& angle, const bool& filled=false);

    /**
     * Draws a box.
     */
    void box(const int x1, const int y1, const int x2, const int y2,
             const bool& filled=false);


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

    void resetPixelCount() {
      pixcount=0;
    }

  protected:
    /**
     * The pixel count for drawing lines.
     */
    int pixcount;

    /**
     * Flag which tells us if we are currently filling an area
     * using 2D drawing primitives.
     */
    bool filling;

    /**
     * Set pixel at x,y in the color set by setColor; also
     * considers the current line pattern.
     * @param x x-coordinate of the pixel to be set.
     * @param y y-coordinate of the pixel to be set.
     */
    inline void setLineElement(const int x, const int y);

    /**
     * Set pixel at x,y in the color set by setColor; also
     * considers the current fill pattern.
     * @param x x-coordinate of the pixel to be set.
     * @param y y-coordinate of the pixel to be set.
     */
    inline void setFillElement(const int x, const int y);

    /**
     * Sets a pixel accordings to the current fill or line pattern,
     * depending on the value of filling
     */
    inline void setElement(const int x, const int y);

  };


}

#endif

