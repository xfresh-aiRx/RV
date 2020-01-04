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
 * file .......: ltiPatternDraw.cpp
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 25.5.1998
 * revisions ..: $Id: ltiPatternDraw_template.h,v 1.7 2005/11/09 17:02:11 doerfler Exp $
 */

#include <limits>

#undef _LTI_DEBUG 
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  template<class T>
  patternDraw<T>::patternDraw()
    : draw<T>() {
    this->useLinePattern=false;
    this->useFillPattern=false;
    filling=false;
  }

  template<class T>
  patternDraw<T>::~patternDraw() {
    this->img = 0;
  }

  template<class T>
  const char* patternDraw<T>::getTypeName() const {
    return "patternDraw";
  }


  template<class T>
  void patternDraw<T>::line(const int fx, const int fy,
                            const int tx, const int ty) {
    // use the Bresenham method for the generation of the optimal line

    this->actX = tx;
    this->actY = ty;

    int b, dx, dy, d, incrE, incrNE, x, y;
    int x1=fx,x2=tx,y1=fy,y2=ty; //i need non const objects
    //    \2|3/
    //    1\|/4
    //   -------   the direction for the algorithm
    //    5/|\7
    //    /6|8\    --
    if (!this->correctLine(x1,y1,x2,y2)) return;
    if (!this->correctLine(x2,y2,x1,y1)) return;

    // always start left go rigth
    if (x2 < x1) {
      //switch points
      b = x1; x1 = x2; x2 = b;
      b = y1; y1 = y2; y2 = b;
    }
    // origin
    x = x1;
    y = y1;
    // the derivative of the line
    dx = x2 - x1;
    dy = y2 - y1;
    // 1 and 7
    if ((dy >= 0)&&(dx>=dy)) {
      incrE = 2 * dy;
      incrNE = 2 * (dy-dx);
      d = 2 * dy - dx;
      while (x < x2) {
        setElement(x,y);
        if (d <= 0) {
          d += incrE;
          x++;
        }
        else {
          d += incrNE;
          x++;  y++;
        }
      }
      setElement(x,y);
    }
    // 4 and 5
    else if ((dy<0)&&(dx>=-dy)) {
      incrE = 2 * -dy;
      incrNE = 2 * (-dy-dx);
      d = 2 * -dy - dx;
      while (x < x2) {
        setElement(x,y);
        if (d <= 0) {
          d += incrE;
          x++;
        }
        else {
          d += incrNE;
          x++; y--;
        }
      }
      setElement(x,y);
    }
    // 2 and 8
    else if ((dy>=0)&&(dy>dx)) {
      incrE = 2 * dx;
      incrNE = 2 * (dx-dy);
      d = 2 * dx -dy;
      while (y < y2) {
        setElement(x,y);
        if (d <= 0) {
          d += incrE;
          y++;
        }
        else {
          d += incrNE;
          x++;y++;
        }
      }
      setElement(x,y);
    }
    // 3 and 6
    else if ((dy<0)&&(-dy>dx)) {
      incrE = 2 * dx;
      incrNE = 2 * (dx+dy);
      d = 2 * dx + dy;
      while (y >= y2) {
        setElement(x,y);
        if (d <= 0) {
          d += incrE;
          y--;
        } else {
          d += incrNE;
          x++;y--;
        }
      }
    }

    this->actX = tx;
    this->actY = ty;
  }


  template<class T>
  void patternDraw<T>::verticalLine(const int x, const int y1, const int y2) {
    if (y2 < y1) {
      for (int y=y1; y>=y2; --y) {
        setElement(x,y);
      }
    } else {
      for (int y=y1; y<=y2; ++y) {
        setElement(x,y);
      }
    }
  }

  template<class T>
  void patternDraw<T>::horizontalLine(const int x1, const int x2, const int y) {
    int tx1;
    int tx2;
    if (x2 < x1) {
      tx2=x1;
      tx1=x2;
    } else {
      tx1=x1;
      tx2=x2;
    }
    _lti_debug("Drawing line from " << tx1 << " to " << tx2 <<
               " in row " << y << "\nn");

    tx1=lti::max(tx1,0);
    tx2=lti::min(tx2,this->img->lastColumn());
    if (y >= 0 && y < this->img->rows()) {
      for (int x=tx1; x<=tx2; ++x) {
        setElement(x,y);
      }
    }
    this->actX=x2;
    this->actY=y;
  }


  template<class T>
  void patternDraw<T>::circle(const point& p1,const int r, const bool& filled) {

    if (r>2)  {
      //assert(r>=0);
      ellipse(p1,r,r,filled);
    } else {
      switch (r) {
        case 1:
          this->set(p1.x-1,p1.y);
          this->set(p1.x+1,p1.y);
          this->set(p1.x,p1.y+1);
          this->set(p1.x,p1.y-1);
          if (filled)
            this->set(p1.x,p1.y);
          break;
        case 2:
          if (!filled) {
            bool tmp=this->useLinePattern;
            this->useLinePattern=false;
            line(p1.x-1,p1.y-2,p1.x+1,p1.y-2);
            line(p1.x-2,p1.y-1,p1.x-2,p1.y+1);
            line(p1.x-1,p1.y+2,p1.x+1,p1.y+2);
            line(p1.x+2,p1.y-1,p1.x+2,p1.y+1);
            this->useLinePattern=tmp;
          } else {
            bool tmp=filling;
            filling=true;
            bool tmp2=this->useFillPattern;
            this->useFillPattern=false;
            line(p1.x-1,p1.y-2,p1.x+1,p1.y-2);
            line(p1.x-2,p1.y-1,p1.x+2,p1.y-1);
            line(p1.x-2,p1.y,p1.x+2,p1.y);
            line(p1.x-2,p1.y+1,p1.x+2,p1.y+1);
            line(p1.x-1,p1.y+2,p1.x+1,p1.y+2);
            filling=tmp;
            this->useFillPattern=tmp2;
          }
          break;
        default:
          break;
      }
    }


  }

  template<class T>
  void patternDraw<T>::ellipse(const point& p1,const int aX, const int aY,
                               const bool& filled) {

    assert(aX>=0);
    assert(aY>=0);

    int x,y,K=1;
    int xM = p1.x;
    int yM = p1.y;
    double qr1,qr2,dx,dy,da;

    if (aX <= 0 || aY <= 0) {
      return;
    }

    if (aX>=aY){
      K=2;
    }

    x=0;
    y=aY;
    qr1=2*aX*aX; qr2=2*aY*aY;
    dx=1;dy=(qr1*aY)-1;
    da=(int)dy/2;


    if (filled) {
      bool tmp=filling;
      filling=true;
      _lti_debug("Drawing filled ellipse\n");
      do {
        horizontalLine(xM+x-K,xM-x+K,yM+y);
        horizontalLine(xM+x-K,xM-x+K,yM-y);

        if (da >= 0){
          da-=dx-1;
          dx+=qr2;
          x++;
        } else {
          da=da+dy-1;
          dy=dy-qr1;
          y--;
        }
      } while (y>=0);
      filling=tmp;
    } else { // not filled ellipse
      if (this->useLinePattern) {
        const int n=2*(2*aX+2*aY);
        int k=0;
        vector<point> p1(n),p2(n),p3(n),p4(n);
        do {
          p1.at(k).x=xM+x-K;
          p1.at(k).y=yM+y;
          p2.at(k).x=xM+x-K;
          p2.at(k).y=yM-y;
          p3.at(k).x=xM-x+K;
          p3.at(k).y=yM+y;
          p4.at(k).x=xM-x+K;
          p4.at(k).y=yM-y;
          k++;
          if (da >= 0){
            da-=dx-1;
            dx+=qr2;
            x++;
          } else {
            da=da+dy-1;
            dy=dy-qr1;
            y--;
          }
        } while (y>=0);
        int pc1=0,pc2=k,pc3=2*k,pc4=3*k;
        for (int i=0; i<k; i++) {
          if (this->linepat.isSet(pc1++)) {
            this->set(p1.at(i));
          }
          if (this->linepat.isSet(pc2++)) {
            this->set(p2.at(i));
          }
          if (this->linepat.isSet(pc3++)) {
            this->set(p3.at(i));
          }
          if (this->linepat.isSet(pc4++)) {
            this->set(p4.at(i));
          }
        }
      } else {
        // no pattern, so use fast method
        do {
          this->set(xM+x-K,yM+y);
          this->set(xM+x-K,yM-y);
          this->set(xM-x+K,yM+y);
          this->set(xM-x+K,yM-y);
          if (da >= 0){
            da-=dx-1;
            dx+=qr2;
            x++;
          } else {
            da=da+dy-1;
            dy=dy-qr1;
            y--;
          }
        } while (y>=0);
      }
    }

  }

  // draw a turned ellipsis
  template<class T>
  void patternDraw<T>::ellipse(const point& center,const int aX, const int aY,
                        const float& angle, const bool& filled) {
    float dr;
    dr=atan2(0.5f,float(lti::max(aX,aY)));

    float fax=float(aX);
    float fay=float(aY);

    int* px1 = new int[this->img->rows()];
    int* px2 = new int[this->img->rows()];
    int miny(this->img->rows()),maxy(0),y;

    if (filled) {
      for (int i=0; i<this->img->rows(); i++) {
        px2[i]=std::numeric_limits<int>::min();
        px1[i]=std::numeric_limits<int>::max();
      }
    }
    for (float phi=0; phi<Pi/2.0; phi+=dr) {
      float x1=fax*cos(phi);
      float x2=-x1; //fax*cos(phi+Pi/2.0);
      float x3=x2; //fax*cos(phi+Pi);
      float x4=x1; //fax*cos(phi+1.5*Pi);
      float y1=fay*sin(phi);
      float y2=y1; //fay*sin(phi+Pi/2.0);
      float y3=-y2; //fay*sin(phi+Pi);
      float y4=y3; //fay*sin(phi+1.5*Pi);
      // now we have the four coordinates, rotate them
      point p1(this->turnPoint(static_cast<float>(x1),
                               static_cast<float>(y1),angle));
      point p2(this->turnPoint(static_cast<float>(x2),
                               static_cast<float>(y2),angle));
      point p3(this->turnPoint(static_cast<float>(x3),
                               static_cast<float>(y3),angle));
      point p4(this->turnPoint(static_cast<float>(x4),
                               static_cast<float>(y4),angle));
      p1.add(center);
      p2.add(center);
      p3.add(center);
      p4.add(center);
      // we get exactly two points for each y ordinate,
      if (filled) {
        this->enterPX(p1,px1,px2,miny,maxy);
        this->enterPX(p2,px1,px2,miny,maxy);
        this->enterPX(p3,px1,px2,miny,maxy);
        this->enterPX(p4,px1,px2,miny,maxy);
      } else {
        // there is a  bug here: we do not use the line pattern
        this->set(p1.x,p1.y);
        this->set(p2.x,p2.y);
        this->set(p3.x,p3.y);
        this->set(p4.x,p4.y);
      }
    }

    if (filled) {
      for (y=miny; y<=maxy; y++) {
        if (px2[y] < -1) {
          // right point is undefined
          if (y > miny) {
            // this point is unset, search for next valid point
            int yi=y;
            while (yi < maxy && px2[yi] < -1) {
              yi++;
            }
            // now yi is either maxy or a valid point
            // y-1 is the last valid point
            _lti_debug("Detected undefined range from row " << y << " to " << yi-1 << "\n");
            float dx2=static_cast<float>(px2[yi]-px2[y-1]);
            float dy=static_cast<float>(yi-y+1);
            //int tmp=yi;
            yi--;
            while (yi >= y) {
              px2[yi]=px2[y-1]+iround(dx2*float(yi-y+1)/dy);
              yi--;
            }
          } else {
            int yi=y;
            while (yi < maxy && px2[yi] < -1) {
              px2[yi++]=-1;
            }
          }
        }
        if (px1[y] > this->img->columns()) {
          if (y > miny) {
            // this point is unset, search for next valid point
            int yi=y;
            while (yi < maxy && px1[yi] > this->img->columns()) {
              yi++;
            }
            // now yi is either maxy or a valid point
            // y-1 is the last valid point
            _lti_debug("Detected undefined range from row " << y << 
                       " to " << yi-1 << "\n");
            float dx1=static_cast<float>(px1[yi]-px1[y-1]);
            float dy=static_cast<float>(yi-y+1);
            int tmp=yi;
            yi--;
            while (yi >= y) {
              px1[yi]=px1[y-1]+iround(dx1*float(yi-y+1)/dy);
              yi--;
            }
            y=tmp;
          } else {
            int yi=y;
            while (yi < maxy && px1[yi] > this->img->columns()) {
              px1[yi++]=this->img->columns();
            }
          }
        }
      }
      // now all points are defined
      bool tmp=filling;
      filling=true;
      for (y=miny; y<=maxy; y++) {
        horizontalLine(px1[y],px2[y],y);
      }
      filling=tmp;
    }

    delete[] px1;
    delete[] px2;
  }

  template<class T>
  void patternDraw<T>::box(const int x1, const int y1,
                           const int x2, const int y2,
                           const bool& filled) {

    if (!filled) {
      draw<T>::box(x1,y1,x2,y2,false);
    } else {
      int tx1,tx2,ty1,ty2;

      if (x2 < x1) {
        tx2=x1;
        tx1=x2;
      } else {
        tx1=x1;
        tx2=x2;
      }
      if (y2 < y1) {
        ty2=y1;
        ty1=y2;
      } else {
        ty1=y1;
        ty2=y2;
      }
        
      tx1=lti::max(tx1,0);
      tx2=lti::min(tx2,this->img->lastColumn());
      ty1=lti::max(ty1,0);
      ty2=lti::min(ty2,this->img->lastRow());

      for (int y=ty1; y<=ty2; ++y) {
        for (int x=tx1; x<=tx2; ++x) {
          setFillElement(x,y);
        }
      }
    }
  }


  /*
   * Sets the pattern for drawing lines. All subsequent lines
   * will be drawn in the given pattern. The default is the
   * continuous line.
   */
  template <class T>
  void patternDraw<T>::setLinePattern(const linePattern& pat) {
    drawBase<T>::setLinePattern(pat);
    pixcount=0;
  }

  /*
   * Sets the pattern for filling areas. All subsequenly filled
   * areas will be filled with the given pattern. The default
   * pattern is a homogenouos fill.
   * @param pat pattern for filling areas.
   */
  template <class T>
  void patternDraw<T>::setFillPattern(const fillPattern& pat) {
    drawBase<T>::setFillPattern(pat);
  }

  template<class T>
  void patternDraw<T>::setLineElement(const int x,const int y) {
    if (!this->useLinePattern || this->linepat.isSet(pixcount++)) {
      this->set(x,y);
    }
  }

  template<class T>
  void patternDraw<T>::setFillElement(const int x,const int y) {
    if (!this->useFillPattern || this->fillpat.isSet(x,y)) {
      //debug("Setting " << x << "," << y << "\n");
      this->set(x,y);
    } //else {
      //debug("Not setting " << x << "," << y << "\n");
    //}
  }

  /*
   * Sets a pixel accordings to the current fill or line pattern
   * depending on the value of filling
   */
  template <class T>
  void patternDraw<T>::setElement(const int x, const int y) {
    if (filling) {
      setFillElement(x,y);
    } else {
      setLineElement(x,y);
    }
  }


} // namespace lti

#include "ltiUndebug.h"
