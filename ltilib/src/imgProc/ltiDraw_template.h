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


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiDraw.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.5.1998
 * revisions ..: $Id: ltiDraw_template.h,v 1.85 2005/10/31 08:26:54 libuda Exp $
 */

#include <cstdio>
#include <cfloat>
#include <limits>
#include "ltiFilledUpsampling.h" //used by draw.text(..)
#include "ltiMath.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

template<class T>
  draw<T>::draw():drawBase<T>() {
    setColor(T());
    this->actX = this->actY = 0;
    img = 0;
    this->actSymbol=drawBase<T>::Pixel;
    this->actSize=5;
    this->filled=false;
  }

template<class T>
  draw<T>::~draw() {
    img = 0;
  }

  template<class T>
  const char* draw<T>::getTypeName() const {
    return "draw";
  }

  template<class T>
  void draw<T>::use(matrix<T>& theImg) {
    if (&theImg == 0)
      return;

    img = &theImg;
  }

  /*
   * get a reference to the image currently being used.
   * \warning if you haven't set any image yet (see use()), an lti::exception
   *          will be thrown
   */
  template<class T>
  matrix<T>& draw<T>::getCanvas() {
    if (img == 0) {
      throw lti::exception("draw<T>: Image not set yet.");
    }
    return *img;
  }

  /*
   * get a read-only reference to the image currently being used.
   * \warning if you haven't set any image yet (see use()), an lti::exception
   *          will be thrown
   */
  template<class T>
  const matrix<T>& draw<T>::getCanvas() const {
    if (img == 0) {
      throw lti::exception("draw<T>: Image not set yet.");
    }
    return *img;
  }

  template<class T>
  void draw<T>::setColor(const T& px) {
    actColor = px;
  }

  template <class T>
  void draw<T>::setGray(float k) {
    setColor(T(k));
  }

  //template<class T>
  //void draw<T>::setGray(const float k) {
  //    setColor(k);
  //}

  template<>
  void draw<rgbPixel>::setGray(float k) {
      setColor(rgbPixel(ubyte(k*255),ubyte(k*255),ubyte(k*255)));
  }

  template<>
  void draw<ubyte>::setGray(float k) {
      setColor(ubyte(k*255));
  }

  template<>
  void draw<int>::setGray(float k) {
      setColor(int(k*255));
  }


template<class T>
  point draw<T>::getCanvasSize() {
    return img->size();
  }


  template<class T>
  void draw<T>::setColor(const char* color) {
    int len=int(strlen(color));

    if (len==0 || len>2) {
      //error
      return;
    }

    switch(color[0]) {

    case 'y':
    case 'm':
    case 'c':
      setColor(0.66f);
      break;
    case 'r':
    case 'g':
    case 'b':
      setColor(0.33f);
      break;
    case 'w':
      setColor(1.0f);
      break;
    case 'k':
      setColor(0.0f);
      break;
    default:
      //error
      return;
    }

    if (len==1) {
      //done, no number following
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    double f=atof(&color[1]);
    f=1.-f/10.;

    T c=getColor();
    setColor(T(f*c));
  }

  template<>
  void draw<ubyte>::setColor(const char* color) {

    int len=int(strlen(color));

    if (len==0 || len>2) {
      //error
      return;
    }

    switch(color[0]) {

    case 'y':
    case 'm':
    case 'c':
      setColor(170);
      break;
    case 'r':
    case 'g':
    case 'b':
      setColor(85);
      break;
    case 'w':
      setColor(255);
      break;
    case 'k':
      setColor(ubyte(0));
      break;
    default:
      //error
      return;
    }

    if (len==1) {
      //done, no number following
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    double f=atof(&color[1]);
    f=1.-f/10.;

    ubyte c=getColor();
    setColor(ubyte(f*c));
  }

  template<>
  void draw<int>::setColor(const char* color) {

    int len=int(strlen(color));

    if (len==0 || len>2) {
      //error
      return;
    }

    switch(color[0]) {

    case 'y':
    case 'm':
    case 'c':
      setColor(170);
      break;
    case 'r':
    case 'g':
    case 'b':
      setColor(85);
      break;
    case 'w':
      setColor(255);
      break;
    case 'k':
      setColor(ubyte(0));
      break;
    default:
      //error
      return;
    }

    if (len==1) {
      //done, no number following
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    double f=atof(&color[1]);
    f=1.-f/10.;

    ubyte c=getColor();
    setColor(ubyte(f*c));
  }

  template<>
  void draw<rgbPixel>::setColor(const char* color) {

    int len=int(strlen(color));

    if (len==0 || len>2) {
      //error
      return;
    }

    rgbPixel rgb;

    switch(color[0]) {

    case 'y':
      rgb.set(255,255,0);
      setColor(rgb);
      break;
    case 'm':
      rgb.set(255,0,255);
      setColor(rgb);
      break;
    case 'c':
      rgb.set(0,255,255);
      setColor(rgb);
      break;
    case 'r':
      rgb.set(255,0,0);
      setColor(rgb);
      break;
    case 'g':
      rgb.set(0,255,0);
      setColor(rgb);
      break;
    case 'b':
      rgb.set(0,0,255);
      setColor(rgb);
      break;
    case 'w':
      rgb.set(255,255,255);
      setColor(rgb);
      break;
    case 'k':
      rgb.set(0,0,0);
      setColor(rgb);
      break;
    default:
      //error
      return;
    }

    if (len==1) {
      //done, no number following
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    int t=atoi(&color[1]);
    double f=1.-double(t)/10.;

    rgbPixel c=getColor();
    setColor(c.multiply(f));
  }

  /*
   * Get color being used
   */
  template<class T>
  const T& draw<T>::getColor() const {
    return actColor;
  }

  template<class T>
  void draw<T>::set(const int x,const int y) {
    if ( !( (((uint32)x) >= (uint32)img->columns()) ||
            (((uint32)y) >= (uint32)img->rows()) )) {
      img->at(y,x) = actColor;
      this->actX = x;
      this->actY = y;
    }
  }


  template<class T>
  void draw<T>::drawSymbol(const int x,const int y,const int w,
                           const typename drawBase<T>::eMarkerType t) {
    if ( !( (((uint32)x) >= (uint32)img->columns()) ||
            (((uint32)y) >= (uint32)img->rows()) )) {
        // save actual symbol
      typename drawBase<T>::eMarkerType tmp=this->actSymbol;
      int tmpSize=this->actSize;

      this->actSymbol=t;
      this->actSize=w;

      if (this->actSymbol==drawBase<T>::Pixel) {
        img->at(y,x) = actColor;
      } else {
        drawSymbol(x,y);
      }

      // restore symbol
      this->actSymbol=tmp;
      this->actSize=tmpSize;

      this->actX = x;
      this->actY = y;
    }
  }

  template<class T>
  void draw<T>::drawSymbol(const int x,
                           const int y,
                           const int w,
                           const char* style) {

    if ( !( (((uint32)x) >= (uint32)img->columns()) ||
            (((uint32)y) >= (uint32)img->rows()) )) {
      T c(0);
      typename drawBase<T>::eMarkerType s(drawBase<T>::Pixel);
      bool f(true);
      int size(0);

      if (style!=0) {
        c=actColor;
        s=this->actSymbol;
        f=this->filled;
        size=this->actSize;
        this->actSize=w;
        this->setStyle(style);
      }

      if (this->actSymbol==drawBase<T>::Pixel) {
        img->at(y,x) = actColor;
      } else {
        drawSymbol(x,y);
      }

      if (style!=0) {
        actColor=c;
        this->actSymbol=s;
        this->filled=f;
        this->actSize=size;
      }

      this->actX = x;
      this->actY = y;
    }
  }


  template<class T>
  void draw<T>::set(const dvector& vct,
                    const T& axisColor,
                    const bool& forceAxis0) {

    if (vct.size()<1) {
      return;
    }

    T actualColor = actColor;

    double theMin,theMax,m,b,mx;
    int i;

    vct.getExtremes(theMin,theMax);

    m = double(img->lastRow())/(theMin-theMax);
    b = -m*theMax;

    if (forceAxis0) {
      if (theMin > 0.0) {
        theMin = 0.0;
      }

      if (theMax < 0.0) {
        theMax = 0.0;
      }
    }

    setColor(axisColor);

    // y-Axis
    line(0,0,0,img->lastRow());

    // x-Axis
    line(0,static_cast<int>(b),img->columns(),static_cast<int>(b));

    // show the vector
    setColor(actualColor);

    set(0,static_cast<int>(m*vct.at(0)+b));

    mx = double(vct.size()-1)/double(img->lastColumn());

    for (i=1;i<img->columns();++i) {
      lineTo(i,static_cast<int>(b+m*vct.at(static_cast<int>(mx*i))));
    }
  }

  template<class T>
  void draw<T>::set(const fvector& vct,
                    const T& axisColor,
                    const bool& forceAxis0) {
    dvector dvct;
    dvct.castFrom(vct);
    set(dvct,axisColor,forceAxis0);
  }

  template<class T>
  void draw<T>::set(const ivector& vct,
                    const T& axisColor,
                    const bool& forceAxis0) {
    dvector dvct;
    dvct.castFrom(vct);
    set(dvct,axisColor,forceAxis0);
  }

  template<class T>
  void draw<T>::line(const int fx, const int fy,
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
    if (!correctLine(x1,y1,x2,y2)) return;
    if (!correctLine(x2,y2,x1,y1)) return;

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
        set(x,y);
        if (d <= 0) {
          d += incrE;
          x++;
        }
        else {
          d += incrNE;
          x++;  y++;
        }
      }
      set(x,y);
    }
    // 4 and 5
    else if ((dy<0)&&(dx>=-dy)) {
      incrE = 2 * -dy;
      incrNE = 2 * (-dy-dx);
      d = 2 * -dy - dx;
      while (x < x2) {
        set(x,y);
        if (d <= 0) {
          d += incrE;
          x++;
        }
        else {
          d += incrNE;
          x++; y--;
        }
      }
      set(x,y);
    }
    // 2 and 8
    else if ((dy>=0)&&(dy>dx)) {
      incrE = 2 * dx;
      incrNE = 2 * (dx-dy);
      d = 2 * dx -dy;
      while (y < y2) {
        set(x,y);
        if (d <= 0) {
          d += incrE;
          y++;
        }
        else {
          d += incrNE;
          x++;y++;
        }
      }
      set(x,y);
    }
    // 3 and 6
    else if ((dy<0)&&(-dy>dx)) {
      incrE = 2 * dx;
      incrNE = 2 * (dx+dy);
      d = 2 * dx + dy;
      while (y >= y2) {
        set(x,y);
        if (d <= 0) {
          d += incrE;
          y--;
        }
        else {
          d += incrNE;
          x++;y--;
        }
      }
    }

    this->actX = tx;
    this->actY = ty;
  }


  template<class T>
  void draw<T>::lineTo(const int x,const int y) {
    line(this->actX,this->actY,x,y);
    this->actX = x;
    this->actY = y;
  }



  template<class T>
  void draw<T>::verticalLine(const point& p1, const point& p2) {
    verticalLine(p1.x,p1.y,p2.y);
    this->actX=p2.x;
  }


  template<class T>
  void draw<T>::verticalLine(const int x, const int y1, const int y2) {
    if (y2 < y1) {
      for (int y=y1; y>=y2; --y) {
        set(x,y);
      }
    } else {
      for (int y=y1; y<=y2; ++y) {
        set(x,y);
      }
    }
  }

  template<class T>
  void draw<T>::horizontalLine(const point& p1, const point& p2) {
    horizontalLine(p1.x,p2.x,p1.y);
  }


  template<class T>
  void draw<T>::horizontalLine(const int x1, const int x2, const int y) {
    int tx1;
    int tx2;
    if (x2 < x1) {
      tx2=x1;
      tx1=x2;
    } else {
      tx1=x1;
      tx2=x2;
    }
    _lti_debug("Drawing line from " << tx1 << " to " << tx2 << " in row " << y << "\nn");
    tx1=lti::max(tx1,0);
    tx2=lti::min(tx2,img->lastColumn());
    if (y >= 0 && y < img->rows()) {
      vector<T>& row=img->getRow(y);
      for (int x=tx1; x<=tx2; ++x) {
        row.at(x)=actColor;
      }
    }
    this->actX=x2;
    this->actY=y;
  }

  template<class T>
  void draw<T>::grid(const point& delta, const bool interval) {
    int tx=this->actX;
    int ty=this->actY;
    if (interval) {
      for (int x=delta.x; x<img->columns(); x+=delta.x) {
        verticalLine(x,0,img->lastRow());
      }
      for (int y=delta.y; y<img->rows(); y+=delta.y) {
        horizontalLine(0,img->lastColumn(),y);
      }
    } else {
      double dx=double(img->columns())/double(delta.x);
      double dy=double(img->rows())/double(delta.y);
      for (double x=dx; x<img->columns(); x+=dx) {
        verticalLine(int(x),0,img->lastRow());
      }
      for (double y=dy; y<img->rows(); y+=dy) {
        horizontalLine(0,img->lastColumn(),int(y));
      }
    }
    this->actX=tx;
    this->actY=ty;
  }


  template<class T>
  void draw<T>::rectangle(const int x1, const int y1, 
                          const int x2, const int y2, const bool& filled) {

    if (filled) {
      int t;

      int tx1,tx2,ty1,ty2;

      if (x2 < x1) {
        t=x2;
        tx2=x1;
        tx1=x2;
      } else {
        tx1=x1;
        tx2=x2;
      }
      if (y2 < y1) {
        t=y2;
        ty2=y1;
        ty1=y2;
      } else {
        ty1=y1;
        ty2=y2;
      }
      tx1=lti::max(tx1,0);
      tx2=lti::min(tx2,img->lastColumn());
      ty1=lti::max(ty1,0);
      ty2=lti::min(ty2,img->lastRow());

      for (int y=ty1; y<=ty2; ++y) {
        vector<T>& row=img->getRow(y);
        for (int x=tx1; x<=tx2; ++x) {
          row.at(x)=actColor;
        }
      }
    } else {
      horizontalLine(x1,x2,y1);
      horizontalLine(x1,x2,y2);
      verticalLine(x1,y1,y2);
      verticalLine(x2,y1,y2);
    }
  }

  template<class T>
  void draw<T>::rectangle(const int x1,const int y1,
                          const int x2,const int y2, 
                          const float angle,const bool& filled) {
    polygonPoints ppts;
    ppts.push_back(point(x1,y1));
    ppts.push_back(point(x2,y1));
    ppts.push_back(point(x2,y2));
    ppts.push_back(point(x1,y2));

    polygon(ppts,filled,angle);
  }

  template<class T>
  void draw<T>::circle(const point& p1,const int r, const bool& filled) {

    if (r>2)  {
      assert(r>=0);
      ellipse(p1,r,r,filled);
    }
    else {
      switch (r) {
        case 1:
          set(p1.x-1,p1.y);
          set(p1.x+1,p1.y);
          set(p1.x,p1.y+1);
          set(p1.x,p1.y-1);
          if (filled)
            set(p1.x,p1.y);
          break;
        case 2:
          if (!filled) {
            line(p1.x-1,p1.y-2,p1.x+1,p1.y-2);
            line(p1.x-2,p1.y-1,p1.x-2,p1.y+1);
            line(p1.x-1,p1.y+2,p1.x+1,p1.y+2);
            line(p1.x+2,p1.y-1,p1.x+2,p1.y+1);
          }
          else {
            line(p1.x-1,p1.y-2,p1.x+1,p1.y-2);
            line(p1.x-2,p1.y-1,p1.x+2,p1.y-1);
            line(p1.x-2,p1.y,p1.x+2,p1.y);
            line(p1.x-2,p1.y+1,p1.x+2,p1.y+1);
            line(p1.x-1,p1.y+2,p1.x+1,p1.y+2);
          }
          break;
        default:
          break;
      }
    }


  }

  template<class T>
  void draw<T>::ellipse(const point& p1,const int aX, const int aY,
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
    } else {
      do {
        set(xM+x-K,yM+y);
        set(xM+x-K,yM-y);
        set(xM-x+K,yM+y);
        set(xM-x+K,yM-y);

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

  // draw a turned ellipsis
  template<class T>
  void draw<T>::ellipse(const point& center,const int aX, const int aY,
                        const float& angle, const bool& filled) {
    const float dr = atan2(0.5f,float(lti::max(aX,aY)));

    const float fax=float(aX);
    const float fay=float(aY);

    int* px1 = new int[img->rows()];
    int* px2 = new int[img->rows()];
    int miny(img->rows()), maxy(0), y;

    if (filled) {
      for (int i=0; i<img->rows(); i++) {
        px2[i]=std::numeric_limits<int>::min();
        px1[i]=std::numeric_limits<int>::max();
      }
    }

    float sphi, cphi;

    for (float phi=0; phi<Pi/2.0; phi+=dr) {
      sincos(phi, sphi, cphi);
      const float x1=fax*cphi;
      const float x2=-x1; //fax*cos(phi+Pi/2.0);
      const float x3=x2; //fax*cos(phi+Pi);
      const float x4=x1; //fax*cos(phi+1.5*Pi);
      const float y1=fay*sphi;
      const float y2=y1; //fay*sin(phi+Pi/2.0);
      const float y3=-y2; //fay*sin(phi+Pi);
      const float y4=y3; //fay*sin(phi+1.5*Pi);
      // now we have the four coordinates, rotate them
      point p1(turnPoint(x1,y1,angle));
      point p2(turnPoint(x2,y2,angle));
      point p3(turnPoint(x3,y3,angle));
      point p4(turnPoint(x4,y4,angle));
      p1.add(center);
      p2.add(center);
      p3.add(center);
      p4.add(center);
      // we get exactly two points for each y ordinate,
      if (filled) {
        enterPX(p1,px1,px2,miny,maxy);
        enterPX(p2,px1,px2,miny,maxy);
        enterPX(p3,px1,px2,miny,maxy);
        enterPX(p4,px1,px2,miny,maxy);
      } else {
        set(p1.x,p1.y);
        set(p2.x,p2.y);
        set(p3.x,p3.y);
        set(p4.x,p4.y);
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
            const float dx2=static_cast<float>(px2[yi]-px2[y-1]);
            const float dy(static_cast<float>(yi-y+1));
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
        if (px1[y] > img->columns()) {
          if (y > miny) {
            // this point is unset, search for next valid point
            int yi=y;
            while (yi < maxy && px1[yi] > img->columns()) {
              yi++;
            }
            // now yi is either maxy or a valid point
            // y-1 is the last valid point
            _lti_debug("Detected undefined range from row " << y << " to " << yi-1 << "\n");
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
            while (yi < maxy && px1[yi] > img->columns()) {
              px1[yi++]=img->columns();
            }
          }
        }
      }
      // now all points are defined
      for (y=miny; y<=maxy; y++) {
        horizontalLine(px1[y],px2[y],y);
      }
    }

    delete[] px1;
    delete[] px2;
  }

  template<class T>
  void draw<T>::polygon(const polygonPoints& poly,
                        const bool filled,
                        const float angle,
                        const point& shift) {

    if (poly.empty()) {
      return;
    }

    polygonPoints::const_iterator it,eit;

    if (angle == 0.0f) {
      if (filled) {
        areaPoints ap;
        ap.castFrom(poly);
        set(ap);
      } else {
        it=poly.begin();
        eit=poly.end();
        set(*it);
        //connect the polygonPoints with lines
        for (++it;it!=eit;++it) {
          lineTo(*it);
        } //close the polygon
        lineTo(*poly.begin());
      }
    } else {
      // create a new polygon rotated and shifted
      polygonPoints pp;
      // first compute the mean value
      point mean,offset;
      int i(0);
      for (it=poly.begin(),eit=poly.end();it!=eit;++it,++i) {
        mean+=(*it);
      }
      mean.divide(i);
      offset = mean+shift;
      // now turn the point around the mean value
      for (it=poly.begin(),eit=poly.end();it!=eit;++it,++i) {
        pp.push_back(offset+turnPoint(static_cast<float>((*it).x-mean.x),
                                static_cast<float>((*it).y-mean.y),
                    angle));
      }
      polygon(pp,filled);
    }
  }

  // overlay general implementations
  template<class T>
  void draw<T>::overlay(const channel& chnl,
                        const int firstX,
                        const int firstY) {
    if (img == 0)
      return;

    const int cols = min(chnl.columns()+firstX,img->columns());
    const int rows = min(chnl.rows()+firstY,img->rows());
    int x,y;

    for (y=max(0,firstY);y<rows;++y) {
      for (x=max(0,firstX);x<cols;++x) {
        float c = chnl.at(y-firstY,x-firstX);
        img->at(y,x) = static_cast<T>((img->at(y,x)*(1.0f-c)) +
                                      (actColor * c));
      }
    }
  }

  // overlay specialization for rgbPixel
  template<>
  void draw<rgbPixel>::overlay(const channel& chnl,
                               const int firstX,
                               const int firstY) {
    if (img == 0)
      return;

    const int cols = min(chnl.columns()+firstX,img->columns());
    const int rows = min(chnl.rows()+firstY,img->rows());
    int x,y;

    for (y=firstY;y<rows;++y) {
      for (x=firstX;x<cols;++x) {
        rgbPixel& p = img->at(y,x);
        float c = chnl.at(y-firstY,x-firstX);

        p.setRed(ubyte((p.getRed()*(1.0f-c)) +
                       (actColor.getRed() * c)));
        p.setGreen(ubyte((p.getGreen()*(1.0f-c)) +
                         (actColor.getGreen() * c)));
        p.setBlue(ubyte((p.getBlue()*(1.0f-c)) +
                        (actColor.getBlue() * c)));
      }
    }
  }


  // overlay general implementations
  template<class T>
  void draw<T>::overlay(const channel8& chnl,
                        const int firstX,
                        const int firstY) {
    if (img == 0)
      return;

    const int cols = min(chnl.columns()+firstX,img->columns());
    const int rows = min(chnl.rows()+firstY,img->rows());
    int x,y;

    for (y=firstY;y<rows;++y) {
      for (x=firstX;x<cols;++x) {
        ubyte c = chnl.at(y-firstY,x-firstX);
        img->at(y,x) = static_cast<T>(((img->at(y,x)*(255.0-c)) +
                                       (actColor * c))/255.0);
      }
    }
  }

  // overlay specialization for rgbPixel
  template<>
  void draw<rgbPixel>::overlay(const channel8& chnl,
                               const int firstX,
                               const int firstY) {
    if (img == 0)
      return;

    const int cols = min(chnl.columns()+firstX,img->columns());
    const int rows = min(chnl.rows()+firstY,img->rows());
    int x,y;

    for (y=firstY;y<rows;++y) {
      for (x=firstX;x<cols;++x) {
        rgbPixel& p = img->at(y-firstY,x-firstX);
        float c = chnl.at(y,x);

        p.setRed(ubyte(((p.getRed()*(255.0f-c)) +
                        (actColor.getRed() * c))/255.0f));
        p.setGreen(ubyte(((p.getGreen()*(255.0f-c)) +
                          (actColor.getGreen() * c))/255.0f));
        p.setBlue(ubyte(((p.getBlue()*(255.0f-c)) +
                         (actColor.getBlue() * c))/255.0f));
      }
    }
  }

  template<class T>
  void draw<T>::overlay(const channel& chnl,
                        const point& p) {
    overlay(chnl,p.x,p.y);
  }

  template<class T>
  void draw<T>::overlay(const channel8& chnl,
                        const point& p) {
    overlay(chnl,p.x,p.y);
  }


  // scale general implementation
  template<class T>
  void draw<T>::scale(const double factor) {
    const int cols = img->columns();
    const int rows = img->rows();
    int x,y;

    for (y=0;y<rows;++y) {
      for (x=0;x<cols;++x) {
        img->at(y,x) = static_cast<T>(factor*img->at(y,x));
      }
    }
  }

  // specialization for rgbPixels
  template<>
  void draw<rgbPixel>::scale(const double factor) {
    const int cols = img->columns();
    const int rows = img->rows();
    int x,y;

    for (y=0;y<rows;++y) {
      for (x=0;x<cols;++x) {
        rgbPixel& p = img->at(y,x);
        p.setRed(static_cast<ubyte>(factor*p.getRed()));
        p.setGreen(static_cast<ubyte>(factor*p.getGreen()));
        p.setBlue(static_cast<ubyte>(factor*p.getBlue()));
      }
    }
  }

  template<class T>
  void draw<T>::arc(const point& p1, const point& pA, const point& pB) {

    int xM=p1.x;
    int yM=p1.y;
    int x1=pA.x;
    int y1=pA.y;
    int x2=pB.x;
    int y2=pB.y;

    float r1 = static_cast<float>(sqrt((x1-xM)*(x1-xM)+(y1-yM)*(y1-yM))); // Radius1
    float w1 = static_cast<float>(atan2(float(y1-yM),float(x1-xM))+Pi); // Winkel1
    float r2 = static_cast<float>(sqrt((x2-xM)*(x2-xM)+(y2-yM)*(y2-yM))); // Radius2
    float w2 = static_cast<float>(atan2(float(y2-yM),float(x2-xM))+Pi); // Winkel2

    // curve equations
    if (w1>=w2) w1 -= static_cast<float>(2*Pi);
    //float m = divide( (r2-r1), (w2-w1) );
    float m;
    if (w2==w1){
      m=0.f;
    }
    else {
      m = (r2-r1)/(w2-w1);
    }
    float b = r1 - m*w1;

    // initialize curve variables
    float sinw, cosw;
    int x, y;
    float r = r1;
    float w = w1;

    this->actX = x1;
    this->actY = y1;

    // evaluate curve eqn.
    do {
      sincos(w, sinw, cosw);
      // calculate next point
      x = (int) (xM - r*cosw);
      y = (int) (yM - r*sinw);
      w += atan2(1.f,r);
      r = m*(w) + b;

      // draw line between last and current coordinate
      lineTo(x, y);

    } while ( w < w2 ); // connect pB -> pB

    // close curve (line between last coordinate and endpoint pB)
    lineTo(x2, y2);
  }

  template<class T>
  void draw<T>::number(const int num,const point& p1, const float& s) {

    assert((s>=0.0)&&(s<=1.0));

    int size = (int) (s/4*img->rows());
    int x = p1.x;
    int y = p1.y;

    char text[10];
    sprintf(text,"%d",num);
    int a=0;
    while (text[a] != 0) {
      switch (text[a]) {
      case '-':
        line(x+size,y-(2*size),x+(2*size),y-(2*size));
        x+=4*size;
        break;
      case '1':
        line(x,y-(3*size),x+(1*size),y-(4*size));
        line(x+(1*size),y-(4*size),x+(1*size),y);
        x+=3*size;
        break;
      case '2':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x+(2*size),y-(4*size),x+(2*size),y-(2*size));
        line(x+(2*size),y-(2*size),x,y-(2*size));
        line(x,y-(2*size),x,y);
        line(x,y,x+(2*size),y);
        x+=4*size;
        break;
      case '3':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x,y-(2*size),x+(2*size),y-(2*size));
        line(x,y,x+(2*size),y);
        line(x+(2*size),y,x+(2*size),y-(4*size));
        x+=4*size;
        break;
      case '4':
        line(x,y-(4*size),x,y-(2*size));
        line(x,y-(2*size),x+(2*size),y-(2*size));
        line(x+(2*size),y,x+(2*size),y-(4*size));
        x+=4*size;
        break;
      case '5':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x,y-(4*size),x,y-(2*size));
        line(x,y-(2*size),x+(2*size),y-(2*size));
        line(x+(2*size),y-(2*size),x+(2*size),y);
        line(x,y,x+(2*size),y);
        x+=4*size;
        break;
      case '6':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x,y-(4*size),x,y);
        line(x,y-(2*size),x+(2*size),y-(2*size));
        line(x+(2*size),y-(2*size),x+(2*size),y);
        line(x,y,x+(2*size),y);
        x+=4*size;
        break;
      case '7':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x+(2*size),y,x+(2*size),y-(4*size));
        x+=4*size;
        break;
      case '8':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x,y-(2*size),x+(2*size),y-(2*size));
        line(x,y,x+(2*size),y);
        line(x+(2*size),y,x+(2*size),y-(4*size));
        line(x,y-(4*size),x,y);
        x+=4*size;
        break;
      case '9':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x,y-(4*size),x,y-(2*size));
        line(x,y-(2*size),x+(2*size),y-(2*size));
        line(x+(2*size),y,x+(2*size),y-(4*size));
        x+=4*size;
        break;
      case '0':
        line(x,y-(4*size),x+(2*size),y-(4*size));
        line(x,y,x+(2*size),y);
        line(x+(2*size),y,x+(2*size),y-(4*size));
        line(x,y-(4*size),x,y);
        x+=4*size;
        break;
      }
      a++;
    }
  }

  template<class T>
  void draw<T>::number(const int num,
                       const int x,
                       const int y,
                       const float& s) {
    number(num,point(x,y),s);
  }

  template<class T>
  void draw<T>::text(const std::string txt,
                     const point& upperleft,
                     const point& scale,
                     const float opaqueness,
                     const bool horizontal) {

    //the size of the characters
    const int charXs=8;
    const int charYs=8;
    //store the exponetial sizes

    //extract the size out of the point only positiv values are allowed
    int scaleX=abs(scale.x);
    int scaleY=abs(scale.y);

    // the size must be at least 1
    if (scaleX < 1) scaleX = 1;
    if (scaleY < 1) scaleY = 1;

    //check if the upperleft point ist negativ and set to 0 if so
    int ulX = upperleft.x;
    int ulY = upperleft.y;


    //check if the opaqueness is between 0 and 1 and correct it otherwise
    float opaq = fabs(opaqueness);
    if(opaq > 1) {
      opaq = 1;
    }

    //saves the matrix for each character
    //the 8 different values are the 8 rows
    //the 8 bits in each value are the columns
    static const unsigned char font[256][8]= {
      {  0,  0,  0,  0,  0,  0,  0,  0},  //   0
      {126,129,165,129,189,153,129,126},  //   1
      {126,255,219,255,195,231,255,126},  //   2
      {108,254,254,254,124, 56, 16,  0},  //   3
      {0x10,0x38,0x7c,0x0fe,0x7c,0x38,0x10,0 }, //   4
      { 56,124, 56,254,254,124, 56,124},  //   5
      { 16, 16, 56,124,254,124, 56,124},  //   6
      {  0,  0, 24, 60, 60, 24,  0,  0},  //   7
      {255,255,231,195,195,231,255,255},  //   8
      {  0, 56,102, 66, 66,102, 60,  0},  //   9
      {255,195,153,189,189,153,195,255},  //  10
      { 15,  7, 15,125,204,204,204,120},  //  11
      { 60,102,102,102, 60, 24,126, 24},  //  12
      { 63, 51, 63, 48, 48,112,240,224},  //  13
      {127, 99,127, 99, 99,103,230,192},  //  14
      {153, 90, 60,231,231, 60, 90,153},  //  15
      {128,224,248,254,248,224,128,  0},  //  16
      {  2, 14, 62,254, 62, 14,  2,  0},  //  17
      { 24, 60,126, 24, 24,126, 60, 24},  //  18
      {102,102,102,102,102,  0,102,  0},  //  19
      {123,219,219,123, 27, 27, 27,  0},  //  20
      { 62, 99, 56,108,108, 56,204,120},  //  21
      {  0,  0,  0,  0,126,126,126,  0},  //  22
      { 24, 60,126, 24,126, 60, 24,255},  //  23
      { 24, 60,126, 24, 24, 24, 24,  0},  //  24
      { 24, 24, 24, 24,126, 60, 24,  0},  //  25
      {  0, 24, 12,254, 12, 24,  0,  0},  //  26
      {  0, 48, 96,254, 96, 48,  0,  0},  //  27
      {  0,  0,192,192,192,254,  0,  0},  //  28
      {  0, 36,102,255,102, 36,  0,  0},  //  29
      {  0, 24, 60,126,255,255,  0,  0},  //  30
      {  0,255,255,126, 60, 24,  0,  0},  //  31
      {  0,  0,  0,  0,  0,  0,  0,  0},  //  32
      { 48,120,120, 48, 48,  0, 48,  0},  //  33
      {108,108,108,  0,  0,  0,  0,  0},  //  34
      {108,108,254,108,254,108,108,  0},  //  35
      { 48,124,192,120, 12,248, 48,  0},  //  36
      {  0,198,204, 24, 48,102,198,  0},  //  37
      { 56,108, 56,118,220,204,118,  0},  //  38
      { 96, 96,192,  0,  0,  0,  0,  0},  //  39
      { 24, 48, 96, 96, 96, 48, 24,  0},  //  40
      { 96, 48, 24, 24, 24, 48, 96,  0},  //  41
      {  0,102, 60,255, 60,102,  0,  0},  //  42
      {  0, 48, 48,252, 48, 48,  0,  0},  //  43
      {  0,  0,  0,  0,  0, 48, 48, 96},  //  44
      {  0,  0,  0,252,  0,  0,  0,  0},  //  45
      {  0,  0,  0,  0,  0, 48, 48,  0},  //  46
      {  6, 12, 24, 48, 96,192,128,  0},  //  47
      {124,198,206,222,246,230,124,  0},  //  48
      { 48,112, 48, 48, 48, 48,252,  0},  //  49
      {120,204, 12, 56, 96,204,252,  0},  //  50
      {120,204, 12, 56, 12,204,120,  0},  //  51
      { 28, 60,108,204,254, 12, 30,  0},  //  52
      {252,192,248, 12, 12,204,120,  0},  //  53
      { 56, 96,192,248,204,204,120,  0},  //  54
      {252,204, 12, 24, 48, 48, 48,  0},  //  55
      {120,204,204,120,204,204,120,  0},  //  56
      {120,204,204,124, 12, 24,112,  0},  //  57
      {  0, 48, 48,  0,  0, 48, 48,  0},  //  58
      {  0, 48, 48,  0,  0, 48, 48, 96},  //  59
      { 24, 48, 96,192, 96, 48, 24,  0},  //  60
      {  0,  0,252,  0,  0,252,  0,  0},  //  61
      { 96, 48, 24, 12, 24, 48, 96,  0},  //  62
      {120,204, 12, 24, 48,  0, 48,  0},  //  63
      {120,198,222,222,222,192,120,  0},  //  64
      { 48,120,204,204,252,204,204,  0},  //  65
      {252,102,102,124,102,102,252,  0},  //  66
      { 60,102,192,192,192,102, 60,  0},  //  67
      {248,108,102,102,102,108,248,  0},  //  68
      {254, 98,104,120,104, 98,254,  0},  //  69
      {254, 98,104,120,104, 96,240,  0},  //  70
      { 60,102,192,192,206,102, 62,  0},  //  71
      {204,204,204,252,204,204,204,  0},  //  72
      {120, 48, 48, 48, 48, 48,120,  0},  //  73
      { 30, 12, 12, 12,204,204,120,  0},  //  74
      {230,102,108,120,108,102,230,  0},  //  75
      {240, 96, 96, 96, 98,102,254,  0},  //  76
      {198,238,254,254,214,198,198,  0},  //  77
      { 98,230,246,222,206,198,198,  0},  //  78
      { 56,108,198,198,198,108, 56,  0},  //  79
      {252,102,102,124, 96, 96,240,  0},  //  80
      {120,204,204,204,220,120, 28,  0},  //  81
      {252,102,102,124,108,102,230,  0},  //  82
      {120,204,224,112, 28,204,120,  0},  //  83
      {252,180, 48, 48, 48, 48,120,  0},  //  84
      {204,204,204,204,204,204,252,  0},  //  85
      {204,204,204,204,204,120, 48,  0},  //  86
      {198,198,198,214,254,238,198,  0},  //  87
      {198,198,108, 56, 56,108,198,  0},  //  88
      {204,204,204,120, 48, 48,120,  0},  //  89
      {254,198,140, 24, 50,102,254,  0},  //  90
      {120, 96, 96, 96, 96, 96,120,  0},  //  91
      {192, 96, 48, 24, 12,  6,  2,  0},  //  92
      {120, 24, 24, 24, 24, 24,120,  0},  //  93
      { 16, 56,108,198,  0,  0,  0,  0},  //  94
      {  0,  0,  0,  0,  0,  0,  0,255},  //  95
      { 48, 48, 24,  0,  0,  0,  0,  0},  //  96
      {  0,  0,120, 12,124,204,118,  0},  //  97
      {224, 96, 96,124,102,102,220,  0},  //  98
      {  0,  0,120,204,192,204,120,  0},  //  99
      { 28, 12, 12,124,204,204,118,  0},  // 100
      {  0,  0,120,204,252,192,120,  0},  // 101
      { 28, 54, 96,240, 96, 96,240,  0},  // 102
      {  0,  0,118,204,204,124, 12,248},  // 103
      {224, 96,108,118,102,102,230,  0},  // 104
      { 48,  0,112, 48, 48, 48,120,  0},  // 105
      { 12,  0, 12, 12, 12,204,204,120},  // 106
      {224, 96,102,108,120,108,230,  0},  // 107
      {112, 48, 48, 48, 48, 48,120,  0},  // 108
      {  0,  0,204,254,254,214,198,  0},  // 109
      {  0,  0,248,204,204,204,204,  0},  // 110
      {  0,  0,120,204,204,204,120,  0},  // 111
      {  0,  0,220,102,102,124, 96,240},  // 112
      {  0,  0,118,204,204,124, 12, 30},  // 113
      {  0,  0,220,118,102, 96,240,  0},  // 114
      {  0,  0,124,192,120, 12,248,  0},  // 115
      { 16, 48,124, 48, 48, 52, 24,  0},  // 116
      {  0,  0,204,204,204,204,118,  0},  // 117
      {  0,  0,204,204,204,120, 48,  0},  // 118
      {  0,  0,198,214,254,254,108,  0},  // 119
      {  0,  0,198,108, 56,108,198,  0},  // 120
      {  0,  0,204,204,204,124, 12,248},  // 121
      {  0,  0,252,152, 48,100,252,  0},  // 122
      { 28, 48, 48,224, 48, 48, 28,  0},  // 123
      { 24, 24, 24,  0, 24, 24, 24,  0},  // 124
      {224, 48, 48, 28, 48, 48,224,  0},  // 125
      {118,220,  0,  0,  0,  0,  0,  0},  // 126
      {  0, 16, 56,108,198,198,254,  0},  // 127
      {120,204,192,204,120, 24, 12,120},  // 128
      {  0,204,  0,204,204,204,126,  0},  // 129
      { 28,  0,120,204,252,192,120,  0},  // 130
      {126,195, 60,  6, 62,102, 63,  0},  // 131
      {204,  0,120, 12,124,204,126,  0},  // 132
      {224,  0,120, 12,124,204,126,  0},  // 133
      { 48, 48,120, 12,124,204,126,  0},  // 134
      {  0,  0,120,192,192,120, 12, 56},  // 135
      {126,195, 60,102,126, 96, 60,  0},  // 136
      {204,  0,120,204,252,192,120,  0},  // 137
      {224,  0,120,204,252,192,120,  0},  // 138
      {204,  0,112, 48, 48, 48,120,  0},  // 139
      {124,198, 56, 24, 24, 24, 60,  0},  // 140
      {224,  0,112, 48, 48, 48,120,  0},  // 141
      {198, 56,108,198,254,198,198,  0},  // 142
      { 48, 48,  0,120,204,252,204,  0},  // 143
      { 28,  0,252, 96,120, 96,252,  0},  // 144
      {  0,  0,127, 12,127,204,127,  0},  // 145
      { 62,108,204,254,204,204,206,  0},  // 146
      {120,204,  0,120,204,204,120,  0},  // 147
      {  0,204,  0,120,204,204,120,  0},  // 148
      {  0,224,  0,120,204,204,120,  0},  // 149
      {120,204,  0,204,204,204,126,  0},  // 150
      {  0,224,  0,204,204,204,126,  0},  // 151
      {  0,204,  0,204,204,124, 12,248},  // 152
      {195, 24, 60,102,102, 60, 24,  0},  // 153
      {204,  0,204,204,204,204,120,  0},  // 154
      { 24, 24,126,192,192,126, 24, 24},  // 155
      { 56,108,100,240, 96,230,252,  0},  // 156
      {204,204,120,252, 48,252, 48, 48},  // 157
      {248,204,204,250,198,207,198,199},  // 158
      { 14, 27, 24, 60, 24, 24,216,112},  // 159
      { 28,  0,120, 12,124,204,126,  0},  // 160
      { 56,  0,112, 48, 48, 48,120,  0},  // 161
      {  0, 28,  0,120,204,204,120,  0},  // 162
      {  0, 28,  0,204,204,204,126,  0},  // 163
      {  0,248,  0,248,204,204,204,  0},  // 164
      {252,  0,204,236,252,220,204,  0},  // 165
      { 60,108,108, 62,  0,126,  0,  0},  // 166
      { 56,108,108, 56,  0,124,  0,  0},  // 167
      { 48,  0, 48, 96,192,204,120,  0},  // 168
      {  0,  0,  0,252,192,192,  0,  0},  // 169
      {  0,  0,  0,252, 12, 12,  0,  0},  // 170
      {195,198,204,222, 51,102,204, 15},  // 171
      {195,198,204,219, 55,111,207,  3},  // 172
      { 24, 24,  0, 24, 24, 24, 24,  0},  // 173
      {  0, 51,102,204,102, 51,  0,  0},  // 174
      {  0,204,102, 51,102,204,  0,  0},  // 175
      { 34,136, 34,136, 34,136, 34,136},  // 176
      { 85,170, 85,170, 85,170, 85,170},  // 177
      {219,119,219,238,219,119,219,238},  // 178
      { 24, 24, 24, 24, 24, 24, 24, 24},  // 179
      { 24, 24, 24, 24,248, 24, 24, 24},  // 180
      { 24, 24,248, 24,248, 24, 24, 24},  // 181
      { 54, 54, 54, 54,246, 54, 54, 54},  // 182
      {  0,  0,  0,  0,254, 54, 54, 54},  // 183
      {  0,  0,248, 24,248, 24, 24, 24},  // 184
      { 54, 54,246,  6,246, 54, 54, 54},  // 185
      { 54, 54, 54, 54, 54, 54, 54, 54},  // 186
      {  0,  0,254,  6,246, 54, 54, 54},  // 187
      { 54, 54,246,  6,254,  0,  0,  0},  // 188
      { 54, 54, 54, 54,254,  0,  0,  0},  // 189
      { 24, 24,248, 24,248,  0,  0,  0},  // 190
      {  0,  0,  0,  0,248, 24, 24, 24},  // 191
      { 24, 24, 24, 24, 31,  0,  0,  0},  // 192
      { 24, 24, 24, 24,255,  0,  0,  0},  // 193
      {  0,  0,  0,  0,255, 24, 24, 24},  // 194
      { 24, 24, 24, 24, 31, 24, 24, 24},  // 195
      {  0,  0,  0,  0,255,  0,  0,  0},  // 196
      { 24, 24, 24, 24,255, 24, 24, 24},  // 197
      { 24, 24, 31, 24, 31, 24, 24, 24},  // 198
      { 54, 54, 54, 54, 55, 54, 54, 54},  // 199
      { 54, 54, 55, 48, 63,  0,  0,  0},  // 200
      {  0,  0, 63, 48, 55, 54, 54, 54},  // 201
      { 54, 54,247,  0,255,  0,  0,  0},  // 202
      {  0,  0,255,  0,247, 54, 54, 54},  // 203
      { 54, 54, 55, 48, 55, 54, 54, 54},  // 204
      {  0,  0,255,  0,255,  0,  0,  0},  // 205
      { 54, 54,247,  0,247, 54, 54, 54},  // 206
      { 24, 24,255,  0,255,  0,  0,  0},  // 207
      { 54, 54, 54, 54,255,  0,  0,  0},  // 208
      {  0,  0,255,  0,255, 24, 24, 24},  // 209
      {  0,  0,  0,  0,255, 54, 54, 54},  // 210
      { 54, 54, 54, 54, 63,  0,  0,  0},  // 211
      { 24, 24, 31, 24, 31,  0,  0,  0},  // 212
      {  0,  0, 31, 24, 31, 24, 24, 24},  // 213
      {  0,  0,  0,  0, 63, 54, 54, 54},  // 214
      { 54, 54, 54, 54,255, 54, 54, 54},  // 215
      { 24, 24,255, 24,255, 24, 24, 24},  // 216
      { 24, 24, 24, 24,248,  0,  0,  0},  // 217
      {  0,  0,  0,  0, 31, 24, 24, 24},  // 218
      {255,255,255,255,255,255,255,255},  // 219
      {  0,  0,  0,  0,255,255,255,255},  // 220
      {240,240,240,240,240,240,240,240},  // 221
      { 15, 15, 15, 15, 15, 15, 15, 15},  // 222
      {255,255,255,255,  0,  0,  0,  0},  // 223
      {  0,  0,118,220,200,220,118,  0},  // 224
      {  0,120,204,248,204,248,192,192},  // 225
      {  0,252,204,192,192,192,192,  0},  // 226
      {  0,254,108,108,108,108,108,  0},  // 227
      {252,204, 96, 48, 96,204,252,  0},  // 228
      {  0,  0,126,216,216,216,112,  0},  // 229
      {  0,102,102,102,102,124, 96,192},  // 230
      {  0,118,220, 24, 24, 24, 24,  0},  // 231
      {252, 48,120,204,204,120, 48,252},  // 232
      { 56,108,198,254,198,108, 56,  0},  // 233
      { 56,108,198,198,108,108,238,  0},  // 234
      { 28, 48, 24,124,204,204,120,  0},  // 235
      {  0,  0,126,219,219,126,  0,  0},  // 236
      {  6, 12,126,219,219,126, 96,192},  // 237
      { 56, 96,192,248,192, 96, 56,  0},  // 238
      {120,204,204,204,204,204,204,  0},  // 239
      {  0,252,  0,252,  0,252,  0,  0},  // 240
      { 48, 48,252, 48, 48,  0,252,  0},  // 241
      { 96, 48, 24, 48, 96,  0,252,  0},  // 242
      { 24, 48, 96, 48, 24,  0,252,  0},  // 243
      { 14, 27, 27, 24, 24, 24, 24, 24},  // 244
      { 24, 24, 24, 24, 24,216,216,112},  // 245
      { 48, 48,  0,252,  0, 48, 48,  0},  // 246
      {  0,118,220,  0,118,220,  0,  0},  // 247
      { 56,108,108, 56,  0,  0,  0,  0},  // 248
      {  0,  0,  0, 24, 24,  0,  0,  0},  // 249
      {  0,  0,  0,  0, 24,  0,  0,  0},  // 250
      { 15, 12, 12, 12,236,108, 60, 28},  // 251
      {120,108,108,108,108,  0,  0,  0},  // 252
      {112, 24, 48, 96,120,  0,  0,  0},  // 253
      {  0,  0, 60, 60, 60, 60,  0,  0},  // 254
      {  0,  0,  0,  0,  0,  0,  0,  0},  // 255
    };

    //channel for the Text with the value 0
    channel tmpChannel(0,0,(channel::value_type)0);
    if (horizontal == true) {
      tmpChannel.resize(charYs,charXs*txt.size(),
                        (channel::value_type)0,false,true);
    } else {
      tmpChannel.resize(charYs*txt.size(),charXs,
                        (channel::value_type)0,false,true);
    }
    //a draw class with the color from the opaqueness
    draw<channel::value_type> myDrawer;
    myDrawer.use(tmpChannel);
    myDrawer.setColor(opaq);
    //an upsampling functor which takes the scale factor
    filledUpsampling fups;
    filledUpsampling::parameters fupsParam;
    fupsParam.factor = point(scaleX,scaleY);
    fups.setParameters(fupsParam);

    //the current x and y coordinate
    int curX = 0;
    int curY = 0;
    int x,y;
    unsigned int n;
    unsigned char maske = 128; //10000000

    if(txt.size() > 0) {
      //if the textsize is bigger than 0
      //loop over each character of the given text
      for(n=0; n < txt.size(); n++){
        //draw the current character in the  channel tmpChannel
        for(y=0; y<charYs; y++) {
          //run thru the rows (Y)
          maske = 128; //10000000;
          for(x=0; x<charXs; x++){
            //run thru the columns (X)
            if(((font[static_cast<unsigned int>(txt[n])][y]) & maske)!=0){
              //set one pixel in the tmpChannel
              myDrawer.set(curX+x,curY+y);
            }
            maske = maske>>1;
          }
        }
        //moves the cursor to the next position
        if(horizontal == true) {
          curX += charXs;
        }
        else {
          curY += charYs;
        }
      }
      //enlarge the text to the desired size
      fups.apply(tmpChannel);
      //overlais the text,in tmpChannel, with the image
      overlay(tmpChannel,ulX,ulY);
    }
  }

  template<class T>
  void draw<T>::text(const std::string txt,
                     const int x,
                     const int y,
                     const point& scale,
                     const float opaqueness,
                     const bool horizontal) {
    text(txt,point(x,y),scale,opaqueness,horizontal);
  }

  template<class T>
  void draw<T>::arrow(const int fx, const int fy,
                      const int tx, const int ty,
                      const float& size){

    line(fx,fy,tx,ty);
    const float angle=atan2(float(ty-fy),float(tx-fx));
    float sizeTemp = fabs(size);
    const float pi43 = 0.75f*static_cast<float>(Pi);
    int xTemp, yTemp;
    float sina, cosa;

    if (sizeTemp<=1.0) {
      sizeTemp = sizeTemp*sqrt((ty-fy)*(ty-fy)+(tx-fx)*(tx-fx));
    }

    sincos(angle+pi43, sina, cosa);
    xTemp = static_cast<int>(tx+sizeTemp*cosa);
    yTemp = static_cast<int>(ty+sizeTemp*sina);
    line(xTemp,yTemp,tx,ty);

    sincos(angle-pi43, sina, cosa);
    xTemp = static_cast<int>(tx+sizeTemp*cosa);
    yTemp = static_cast<int>(ty+sizeTemp*sina);
    line(xTemp,yTemp,tx,ty);

  }

  template<class T>
  void draw<T>::clear() {
    img->fill(actColor);
  }

  template<class T>
  void draw<T>::drawSymbol(const int x, const int y) {
      int i,j;
      int d=int(this->actSize/2.0);
      int ds=int(this->actSize/3.46);
      int dt=int(this->actSize*0.433);
      if (this->actSymbol==drawBase<T>::Pixel) {
        rectangle(x-d,y-d,x+d,y+d,true);
        return;
      }
      switch (this->actSymbol) {

        case drawBase<T>::Xmark:
          line(x-d,y-d,x+d,y+d);
          line(x+d,y-d,x-d,y+d);
          break;
        case drawBase<T>::Star:
          line(x,y+d,x,y-d);
          line(x-d,y-ds,x+d,y+ds);
          line(x+d,y-ds,x-d,y+ds);
          break;
        case drawBase<T>::Plus:
          horizontalLine(x-d,x+d,y);
          verticalLine(x,y-d,y+d);
          break;
        case drawBase<T>::Dot:
        case drawBase<T>::Circle:
          circle(point(x,y),d,this->filled);
          break;
        case drawBase<T>::Square:
          rectangle(x-d,y-d,x+d,y+d,this->filled);
          break;
        case drawBase<T>::Diamond:
          line(x,y-d,x-d,y);
          lineTo(x,y+d);
          lineTo(x+d,y);
          lineTo(x,y-d);
          if (this->filled) {
              j=0;
            for (i=this->actSize/2; i>0; i--) {
              line(x-i,y-j,x+i,y-j);
              line(x-i,y+j,x+i,y+j);
              j++;
            }
          }
          break;
        case drawBase<T>::TriangleUp:
          if (this->filled) {
            line(x,y-d,x-d,y+d);
            line(x,y-d,x+d,y+d);
            line(x-d,y+d,x+d,y+d);
            double tmp;
            j=1;
            for (tmp=d; tmp>=1.; tmp=tmp-0.5) {
              line(x-int(tmp),y+d-j,x+int(tmp),y+d-j);
              j++;
            }
          }
          else {
            line(x-d,y+dt,x,y-dt);
            lineTo(x+d,y+dt);
            lineTo(x-d,y+dt);
          }
          break;
        case drawBase<T>::TriangleDown:
          if (this->filled) {
            line(x,y+d,x-d,y-d);
            line(x,y+d,x+d,y-d);
            line(x-d,y-d,x+d,y-d);
            double tmp;
            j=1;
            for (tmp=d; tmp>=1.; tmp=tmp-0.5) {
              line(x-int(tmp),y-d+j,x+int(tmp),y-d+j);
              j++;
            }
          }
          else {
            line(x-d,y-dt,x+d,y-dt);
            lineTo(x,y+dt);
            lineTo(x-d,y-dt);
          }
          break;
        case drawBase<T>::TriangleRight:
          if (this->filled) {
            line(x+d,y,x-d,y+d);
            line(x+d,y,x-d,y-d);
            line(x-d,y+d,x-d,y-d);
            double tmp;
            j=1;
            for (tmp=d; tmp>=1.; tmp=tmp-0.5) {
              line(x-d+j,y-int(tmp),x-d+j,y+int(tmp));
              j++;
            }
          }
          else {
            line(x+dt,y,x-dt,y-d);
            lineTo(x-dt,y+d);
            lineTo(x+dt,y);
          }
          break;
        case drawBase<T>::TriangleLeft:
          if (this->filled) {
            line(x-d,y,x+d,y+d);
            line(x-d,y,x+d,y-d);
            line(x+d,y+d,x+d,y-d);
            double tmp;
            j=1;
            for (tmp=d; tmp>=1.; tmp=tmp-0.5) {
              line(x+d-j,y-int(tmp),x+d-j,y+int(tmp));
              j++;
            }
          }
          else {
            line(x-dt,y,x+dt,y-d);
            lineTo(x+dt,y+d);
            lineTo(x-dt,y);
          }
          break;
        case drawBase<T>::LtiLogo: {
          int d1=int(this->actSize/9.);
          int d2=int(2.*this->actSize/9.);
          int d4=int(4.*this->actSize/9.);
          rectangle(x-d4,y-d1,x-d2,y+d4,true);
          rectangle(x-d4,y+d2,x+d1,y+d4,true);
          rectangle(x-d4,y-d4,x+d4,y-d2,true);
          rectangle(x-d1,y-d2,x+d1,y+d1,true);
          rectangle(x+d2,y-d1,x+d4,y+d4,true);
          break;
        }
        default:
          return;
      }
  }

  // true if the line or a part of it is within the image
  template<class T>
  bool draw<T>::correctLine(int &x,int &y,const int x2,const int y2) {
    if (inCanvas(x,y))
      return true;

    double m;
    int xn,yn;

    // 0. Case:  line out of the images!
    if (((y >= img->rows()) && (y2 >= img->rows())) ||
        ((y <  0    ) && (y2 <  0    )) ||
        ((x >= img->columns()) && (x2 >= img->columns())) ||
        ((x <  0)     && (x2 <  0    )))
      return false;

    // 1. Case:  vertical line!
    if (x == x2) {
      y = (y > y2) ? img->lastRow() : 0;
      return (true);
    }

    // 2. Case:  horizontal line!
    if (y == y2) {
      x = (x > x2) ? img->lastColumn() : 0;
      return (true);
    }

    // 3. Case:  Another slope

    m = (double)(x-x2)/(double)(y-y2);
    yn = (y > y2) ? img->lastRow() : 0;
    xn = (int)(m*(double)(yn-y2)+x2);

    if (xn < 0) {
      xn = 0;
      yn = (int)(y2 - ((double)x2/m));
    }
    else if (xn >= img->columns()) {
      xn = img->lastColumn();
      yn = (int)(((double)(xn-x2)/m) + y2);
    }

    x = xn;
    y = yn;
    return (inCanvas(x,y));
  }



  /*
   * fill the canvas with the current color, starting at the given seed
   * until a color different to the original one at the seed is 
   * found.  This is similar to the fill tool of usual paint programs
   */
  template<class T> 
  void draw<T>::fill(const point& seed) {
    assert(notNull(img));

    // if the seed is outside the image, we don't need to do anything
    if (!inCanvas(seed)) {
      return;
    }

    const T scolor = img->at(seed);

    // if the seed color is the same color at the image, do nothing
    if (scolor == actColor) {
      return;
    }

    // the neighbors
    static const int deltax[4] = {+1, 0,-1, 0};
    static const int deltay[4] = { 0,+1, 0,-1};

    // region growing
    std::list<point> stack;
    stack.push_back(seed);
    set(seed);
    point p,q;

    while (!stack.empty()) {
      q=stack.front();
      stack.pop_front();
      for (int i=0;i<4;++i) {
        p=q+point(deltax[i],deltay[i]);
        if (inCanvas(p) && (img->at(p) == scolor)) {
          stack.push_back(p);
          img->at(p)=actColor;
        }
      }
    }
  }
  
  /*
   * fillUntil fill the canvas with the current color, 
   * starting at the given seed, until the stop color is reached.
   */
  template<class T> 
  void draw<T>::fillUntil(const point& seed,const T& stopColor) {
    assert(notNull(img));

    // if the seed is outside the image, we don't need to do anything
    if (!inCanvas(seed)) {
      return;
    }
    const T scolor = img->at(seed);
    // if the seed color is equal to the stop color do nothing
    if (scolor == stopColor) {
      return;
    }

    channel8 visited(img->size(),ubyte(0));

    // the neighbors
    static const int deltax[4] = {+1, 0,-1, 0};
    static const int deltay[4] = { 0,+1, 0,-1};

    // region growing
    std::list<point> stack;
    stack.push_back(seed);
    set(seed);
    visited.at(seed)=1;
    point p,q;

    while (!stack.empty()) {
      q=stack.front();
      stack.pop_front();
      for (int i=0;i<4;++i) {
        p=q+point(deltax[i],deltay[i]);
        if (inCanvas(p) && 
            (img->at(p) != stopColor) &&
            (visited.at(p) == 0)) {
          stack.push_back(p);
          img->at(p)=actColor;
          visited.at(p)=1;
        }
      }
    }
  }

} // namespace lti

#include "ltiUndebug.h"
