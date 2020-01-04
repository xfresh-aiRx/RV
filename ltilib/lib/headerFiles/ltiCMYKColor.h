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
 * file .......: ltiCMYKColor.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 12.11.01
 * revisions ..: $Id: ltiCMYKColor.h,v 1.4 2003/03/31 06:36:31 alvarado Exp $
 */

#ifndef LTI_CMYK_COLOR_H
#define LTI_CMYK_COLOR_H

namespace lti {

  /**
   * A little class for defining points in the CMYK color space.
   * At present, CMYK colors can only be used for EPS drawings.
   */
  class cmykColor {
  public:

    /**
     * Default constructor. Sets this color to (0,0,0,0) which
     * is the CMYK representation of white.
     */
    cmykColor() { c=m=y=k=0; };

    /**
     * Constructor. Sets the C, M, Y, and K components to the
     * given values.
     */
    cmykColor(float nc, float nm, float ny, float nk) {
      c=nc; m=nm; y=ny; k=nk;
    }

    /**
     * Returns the value of the C (=Cyan) component of this color.
     */
    inline float getCyan() const {
      return c;
    }

    /**
     * Returns the value of the M (=Magenta) component of this color.
     */
    inline float getMagenta() const {
      return m;
    }

    /**
     * Returns the value of the Y (=Yellow) component of this color.
     */
    inline float getYellow() const {
      return y;
    }

    /**
     * Returns the value of the K (=blacK) component of this color.
     */
    inline float getBlack() const {
      return k;
    }

    /**
     * sets the value of the Cyan component of this color
     */
    void setCyan(const float& cyan) {
      c=cyan;
    }

    /**
     * sets the value of the Magenta component of this color
     */
    void setMagenta(const float& magenta) {
      m=magenta;
    }

    /**
     * sets the value of the Yellow component of this color
     */
    void setYellow(const float& yellow) {
      y=yellow;
    }

    /**
     * sets the value of the Black component of this color
     */
    void setBlack(const float& black) {
      k=black;
    }

    /**
     * Copies the "other" color to this color.
     */
    inline cmykColor& copy(const cmykColor& other) {
      c=other.c;
      m=other.m;
      y=other.y;
      k=other.k;
      return (*this);
    };

    /**
     * Alias for copy.
     */
    inline cmykColor& operator=(const cmykColor& other) {
      return(copy(other));
    };

    /**
     * Compare two colors (true if equal!)
     */
    inline bool isEqual(const cmykColor& other) const {
      return (c == other.getCyan() && m == other.getMagenta()
              && y == other.getYellow() && k == other.getBlack());
    }

    /**
     * Alias for compare()
     */
    inline bool operator==(const cmykColor& other) const {
      return (isEqual(other));
    };

    /**
     * Alias for !compare()
     */
    inline bool operator!=(const cmykColor& other) const {
      return (!isEqual(other));
    };

    /**
     * An cmykColor is said to be "smaller" than another one, if
     * any of its component values is smaller than the other
     * component.
     */
    inline bool operator<(const cmykColor& other) const {
      return (k < other.getBlack() || c < other.getCyan()
              || m < other.getMagenta() || y < other.getYellow());
    }

    /**
     * An cmykColor is said to be "bigger" than another one, if
     * any of its component values is smaller than the other
     * component.
     */
    inline bool operator>(const cmykColor& other) const {
      return (k > other.getBlack() || c > other.getCyan()
              || m > other.getMagenta() || y > other.getYellow());
    }


  private:
    float c,m,y,k;

  };


  /**
   * read the vector from the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be readed
   *
   * @ingroup gColor
   */
  inline bool read(ioHandler& handler,cmykColor& p,const bool complete=true) {
    float tmp;
    bool b = true;

    // the begin and end tokens are mandatory here! ignore the complete flag...
    handler.readBegin();

    b = b && handler.read(tmp);
    p.setCyan(tmp);

    b = b && handler.readDataSeparator();

    b = b && handler.read(tmp);
    p.setMagenta(tmp);

    b = b && handler.readDataSeparator();

    b = b && handler.read(tmp);
    p.setYellow(tmp);

    b = b && handler.readDataSeparator();

    b = b && handler.read(tmp);
    p.setBlack(tmp);

    b = b && handler.readEnd();

    return b;
  };


  /**
   * write the vector in the given ioHandler.  The complete flag indicates
   * if the enclosing begin and end should be also be written or not
   *
   * @ingroup gStorable
   */
  inline bool write(ioHandler& handler,const cmykColor& p,
                    const bool complete=true) {
    bool b = true;

    // the begin token is mandatory here, so ignore the complete flag
    b = b && handler.writeBegin();

    b = b && handler.write(p.getCyan());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.getMagenta());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.getYellow());
    b = b && handler.writeDataSeparator();
    b = b && handler.write(p.getBlack());
    b = b && handler.writeEnd();

    return b;
  };

}

#endif

