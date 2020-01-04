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
 * file .......: ltiColors.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 12.11.01
 * revisions ..: $Id: ltiColors.h,v 1.16 2003/11/05 07:15:05 alvarado Exp $
 */

#ifndef LTI_COLORS_H
#define LTI_COLORS_H

#include "ltiRGBPixel.h"
// we need this for min and max
#include "ltiMath.h"

/**
 * @file ltiColors.h
 * This file contains some more symbolic color definitions, like orange,
 * Pink, etc.
 */

namespace lti {

  /**
   * An alias for the rgbPixel type. It has several useful methods
   * though that can be used for reading color definitions
   * from external sources.
   */
  class rgbColor: public rgbPixel {

  public:
    /**
     * default constructor
     */
    rgbColor() : rgbPixel() {};

    /**
     * constructor with member initialization
     *
     * Per default a new rgbColor will be initialized with the given value.
     * @param val a 4 byte value to be
     *            assign to the three channels + dummy.
     *             Note that the order depends on the system endianness:
     *             - If you use little endian (for example: Intel Processor)
     *               a value of 0x00010203 means red=01,green=02 and blue=03
     *             - If you use big endian (for example: PowerPC Processor)
     *               a value of 0x00010203 means red=02,green=01 and blue=00
     *             Avoid the use of this constructor if you want to maintain
     *             platform compatibility.
     */
    rgbColor(const uint32 val)
    : rgbPixel(val) {
      // nothing else
    };

    /**
     * Upgrade constructor.
     */
    rgbColor(const rgbPixel& other)
    : rgbPixel(other.getValue()) {
      // nothing else
    };

    /**
     * rgb constructor
     * @param r 8 bit value for the red component
     * @param g 8 bit value for the green component
     * @param b 8 bit value for the blue component
     * @param d 8 bit value for the dummy byte (default value 0)
     */
    rgbColor(const ubyte r,const ubyte g,const ubyte b,const ubyte d=0)
    : rgbPixel(r,g,b,d) {
      // nothing else
    };


    /**
     * Parses a HTML color definition string and sets this color
     * to the given one. In HTML, RGB colors
     * are defined by the following syntax:
     * <pre>
     * Color = #RRGGBB
     * RR = ( 0 | 1 | 2 | ... | 9 | a | A | b | B | ... | f | F ){2}
     * GG = ( 0 | 1 | 2 | ... | 9 | a | A | b | B | ... | f | F ){2}
     * BB = ( 0 | 1 | 2 | ... | 9 | a | A | b | B | ... | f | F ){2}
     * </pre>
     * For instance, "#ff0000" means red (r=255,g=0,b=0).
     * This method also can accept an extension which also includes
     * the alpha value as a fourth component (Color = #RRGGBBAA).
     * If the component is not present, it will be assumed zero.
     * If a string cannot be parsed, the value of this color
     * object will be undefined.
     *
     * @param s the HTML color definition.
     * @param acceptAlpha if this is true, the method accepts
     * a fourth component which is interpreted as alpha value.
     * If it is false, such a string will lead to an error.
     * @return true if the string could be parsed, false
     *         otherwise.
     */
    bool parseHTML(const std::string& s, bool acceptAlpha=false);

    /**
     * Returns a HTML color definition of this color.
     * @param getAlpha if this is true, the returned definition
     *        will also contain the alpha channel.
     */
    std::string makeHTML(bool getAlpha=false);

    /**
     * Parses a color name (which colors are defined, is
     * system dependent) and sets this color to the giveon one.
     * If the color name is undefined, the method will return false;
     * @param s the color name
     * @return true if the name is known and defined, false otherwise.
     */
    bool parseName(const std::string& s);

  };

  /**
   * @name Color Constants.
   *
   * These color constants are defined in the file ltiColors.h.
   * The six primary and secondary colors are defined in ltiTypes.h
   * (which is also included in ltiColors.h!)
   * @see lti::Red,lti::Green,lti::Blue,lti::Cyan,lti::Magenta,lti::Yellow
   *
   * Note that the color names are all own
   * creations, they are not correlated to any official naming
   * standards at all.
   */
  //@{

  // darker primary colors

  /**
   * Constant for dark red
   */
  static const rgbColor DarkRed(127,0,0);

  /**
   * Constant for dark green
   */
  static const rgbColor DarkGreen(0,127,0);

  /**
   * Constant for dark blue
   */
  static const rgbColor DarkBlue(0,0,127);

  // darker secondary colors

  /**
   * constant for the color dark yellow
   */
  static const rgbColor DarkYellow(127,127,0);

  /**
   * constant for some kind of dark cyan.
   */
  static const rgbColor DarkCyan(0,127,127);

  /**
   * constant for some kind of dark magenta.
   */
  static const rgbColor DarkMagenta(127,0,127);

  // brighter primary colors

  /**
   * constant for a kind of bright green.
   */
  static const rgbColor BrightGreen(0,255,127);

  /**
   * constant for a kind of bright green.
   */
  static const rgbColor BrightBlue(0,127,255);

  /**
   * constant for a kind of bright red.
   */
  static const rgbColor BrightRed(255,127,64);

  // brighter secondary colors

  /**
   * constant for some kind of bright magenta.
   */
  static const rgbColor BrightMagenta(255,127,255);

  /**
   * constant for some kind of bright yellow.
   */
  static const rgbColor BrightYellow(255,255,127);

  /**
   * constant for some kind of bright cyan.
   */
  static const rgbColor BrightCyan(127,255,255);


  // other colors

  /**
   * constant for the color orange.
   */
  static const rgbColor DarkOrange(192,64,0);

  /**
   * constant for the color fusia.
   */
  static const rgbColor Fusia(255,0,127);

  /**
   * constant for the color pink.
   */
  static const rgbColor Pink(255,127,127);

  /**
   * constant for another kind of bright green.
   */
  static const rgbColor LawnGreen(127,255,0);

  /**
   * constant for some kind of lemon color
   */
  static const rgbColor Lemon(127,255,127);

  /**
   * constant for a light blue.
   */
  static const rgbColor LightBlue(0,127,255);

  /**
   * constant for a light blue.
   */
  static const rgbColor DarkViolet(127,0,255);

  /**
   * constant for the color violet.
   */
  static const rgbColor Violet(127,127,255);


  /**
   * Function for scaling a color's intensity.
   * @param src rgbColor of the original color
   * @param f factor by which to multiply the R, G and B values. Warning:
   * Saturation effects might lead to strange results.
   * @return rgbColor where R, G and B have the given value <code>t</code>.
   */
  inline rgbColor scale(const rgbColor& src, double f) {
    double r=max(0.0,min(double(src.getRed())*f,255.0));
    double g=max(0.0,min(double(src.getGreen())*f,255.0));
    double b=max(0.0,min(double(src.getBlue())*f,255.0));
    return rgbColor(int(r),int(g),int(b));
  }

  /**
   * Function for darkening a color.
   * @param src rgbColor of the original color
   * @param f factor which tells how much darker the new color should be.
   * Example: 0.2 means: Make the color 20% darker. 0 means:
   * leave the original, 1.0 means: make it black
   * @return rgbColor where R, G and B have the given value <code>t</code>.
   */
  inline rgbColor darker(const rgbColor& src, double f=0.5) {
    return scale(src,(1.0-f));
  }

  /**
   * Function for brightening a color.
   * @param src rgbColor of the original color
   * @param f factor which tells how much darker the new color should be.
   * Example: 0.2 means: Make the color 20% brighter. 0 means:
   * leave the original, 1.0 means: make it twice as bright. Warning:
   * Saturation effects might lead to strange results.
   * @return rgbColor where R, G and B have the given value <code>t</code>.
   */
  inline rgbColor brighter(const rgbColor& src, double f=0.5) {
    return scale(src,(1.0+f));
  }


  /**
   * function for returning a grey value.
   * @param t grey value must be between 0 and 255
   * @return rgbColor where R, G and B have the given value <code>t</code>.
   */
  inline rgbColor grey(int t) {
    return rgbColor(t,t,t);
  }

  /**
   * function for returning a gray value.
   * @param t grey value must be between 0 and 255
   * @return rgbColor where R, G and B have the given value <code>t</code>.
   */
  inline rgbColor gray(int t) {
    return rgbColor(t,t,t);
  }

  /**
   * function for returning a grey value.
   * @param t grey value must be between 0 and 1
   * @return rgbColor where R, G and B have the given value <code>t*255</code>.
   */
  inline rgbColor grey(double t) {
    return rgbColor(int(t*255),int(t*255),int(t*255.0));
  }

  /**
   * function for returning a gray value.
   * @param t grey value must be between 0 and 1
   * @return rgbColor where R, G and B have the given value <code>t*255</code>.
   */
  inline rgbColor gray(double t) {
    return rgbColor(int(t*255),int(t*255),int(t*255.0));
  }

  /**
   * Definition for a 100% bright grey, i.e. White
   */
  static const rgbColor Grey100(255,255,255);

  /**
   * Definition for a 75% bright grey, i.e. bright grey
   */
  static const rgbColor Grey75(191,191,191);

  /**
   * Definition for a 50% bright grey, i.e. medium grey
   */
  static const rgbColor Grey50(127,127,127);

  /**
   * Definition for a 25% bright grey, i.e. dark grey
   */
  static const rgbColor Grey25(63,63,63);

  /**
   * Definition for a 0% bright grey, i.e. Black
   */
  static const rgbColor Grey0(0,0,0);



}

#endif

