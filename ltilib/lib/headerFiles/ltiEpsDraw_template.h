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
 * file .......: ltiEpsDraw_template.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 20.11.2002
 * revisions ..: $Id: ltiEpsDraw_template.h,v 1.23 2005/10/31 08:26:54 libuda Exp $
 */

#include <limits>
#include "ltiProcessInfo.h"
#include "ltiTimer.h"
#include "ltiAscii85Codec.h"
#include "ltiAsciiHexCodec.h"
#include "ltiFlateCodec.h"
#include "ltiCombinedCodec.h"
#include "ltiRunLengthCodec.h"
#include "ltiBinarizationCodec.h"
#include "ltiProcessInfo.h"
#include "ltiJPEGFunctor.h"
#include "ltiConstants.h"
#include <cstdio>

namespace lti {

  template<class T>
  const int epsDraw<T>::a4Width=595;
  template<class T>
  const int epsDraw<T>::a4Height=842;

  template<class T>
  const float epsDraw<T>::defaultResolution=72;

  template<class T>
  const char* epsDraw<T>::alignCmd[3][3] = {
    { "lbs", "lms", "lts" },
    { "cbs", "cms", "cts" },
    { "rbs", "rms", "rts" }
  };

  /*
   * Constructor. It gets a drawing area, the bounding box.
   * The stuff that is drawn is scaled to that area if
   * scale is true. Otherwise, it is simply clipped.
   */
  template<class T>
  epsDraw<T>::epsDraw(std::ostream &os, const point& drawingArea,
                      bool scale, bool eps)
    : out(os) {
    dimension=drawingArea;
    bbox.ul.x=std::numeric_limits<int>::max();
    bbox.ul.y=std::numeric_limits<int>::max();
    bbox.br.x=std::numeric_limits<int>::min();
    bbox.br.y=std::numeric_limits<int>::min();
    doClip=false;
    colorCount=0;
    fillpatCount=0;
    linepatCount=0;
    rgbColorNames.clear();
    cmykColorNames.clear();
    linePatternNames.clear();
    fillPatternNames.clear();

    objects.clear();
    objCount=0;
    isEps=eps;
    currentColor="";
    title="LTIlib drawing";
    fontSize=10;
    
    this->actSize=2;
    this->actSymbol=drawBase<T>::Pixel;
    this->filled=false;

    autoScale=true;
    resolution=1.0;
  }

  /*
   * Constructor. It determines the size of the bounding box
   * from the stuff that is being drawn.
   */
  template<class T>
  epsDraw<T>::epsDraw(std::ostream &os, bool scale, bool eps)
    : out(os) {
    dimension=point(500,-1);
    bbox.ul.x=std::numeric_limits<int>::max();
    bbox.ul.y=std::numeric_limits<int>::max();
    bbox.br.x=std::numeric_limits<int>::min();
    bbox.br.y=std::numeric_limits<int>::min();
    doClip=false;
    colorCount=0;
    fillpatCount=0;
    linepatCount=0;
    rgbColorNames.clear();
    cmykColorNames.clear();
    linePatternNames.clear();
    fillPatternNames.clear();
    objects.clear();
    objCount=0;
    isEps=eps;
    currentColor="";
    title="LTIlib drawing";
    fontSize=10;

    this->actSize=2;
    this->actSymbol=drawBase<T>::Pixel;
    this->filled=false;

    autoScale=true;
  }

  /*
   * destructor
   */
  template<class T>
  epsDraw<T>::~epsDraw() {
    // do nothing
    close();
  }

  /*
   * get name of this type
   */
  template<class T>
  const char* epsDraw<T>::getTypeName() const {
    return "epsDraw";
  }

  /*
   * Specifies the clipping rectangle. Default is the drawing area.
   */
  template<class T>
  void epsDraw<T>::setClip(const trectangle<int>& rc) {
    clipRect=rc;
    doClip=true;
  }

  /*
   * Specifies the drawing area
   */
  template<class T>
  void epsDraw<T>::setSize(const point& rc) {
    dimension=rc;
  }

  template<class T>
  void epsDraw<T>::setResolution(float r) {
    resolution=r/defaultResolution;
  }

  /*
   * Specifies the clipping rectangle. Default is the drawing area.
   */
  template<class T>
  void epsDraw<T>::setTransform(const tpoint<float>& offset,
                                const tpoint<float>& scale) {
    autoScale=false;
    xScale=scale.x;
    yScale=scale.y;
    xOffset=offset.x;
    yOffset=offset.y;
  }

  template<class T>
  point epsDraw<T>::getCanvasSize() {


    return dimension;
  }

  /*
   * Sets the pattern for drawing lines. All subsequent lines
   * will be drawn in the given pattern. The default is the
   * continuous line.
   */
  template <class T>
  void epsDraw<T>::setLinePattern(const linePattern& pat) {
    std::string cn=getLinePatName(pat);
    if (currentlpat == "" || cn != currentlpat) {
      objects.push_back(cn);
      currentlpat=cn;
    }
  }

  /*
   * Sets the pattern for filling areas. All subsequenly filled
   * areas will be filled with the given pattern. The default
   * pattern is a homogenouos fill.
   */
  template <class T>
  void epsDraw<T>::setFillPattern(const fillPattern& pat) {
    std::string cn=getFillPatName(pat);
    if (currentfpat == "" || cn != currentfpat) {
      objects.push_back(cn);
      currentfpat=cn;
    }
  }


  template<class T>
  void epsDraw<T>::setColor(const T& c) {
    std::string cn=getColorName(c);
    if (currentColor == "" || cn != currentColor) {
      objects.push_back(cn);
      currentColor=cn;
    }
  }

  template<>
  void epsDraw<float>::setColor(const float& c) {
    setGray(c);
  }


  /*
   * Specifies color to be used (in CMYK, ranges from 0 to 1)
   */
  template<class T>
  void epsDraw<T>::setLineWidth(float w) {
    sprintf(numBuffer, "%4.2f lw", w);
    objects.push_back(numBuffer);
  }

  /*
   * Specifies grayscale level to be used (range from 0 to 1)
   */
  template<class T>
  void epsDraw<T>::setGray(float k) {
    sprintf(numBuffer, "%6.4f g", k);
    currentColor=numBuffer;
    objects.push_back(numBuffer);
  }


  /*
   * Set pixel at x,y with the color in use
   */
  template<class T>
  void epsDraw<T>::drawSymbol(const int x, const int y,const int w,
                              const typename drawBase<T>::eMarkerType t) {
    adjustBoundingBox(x-w/2,y-w/2,x+w/2,y+w/2);
    this->actX=x;
    this->actY=y;

    typename drawBase<T>::eMarkerType tmp=this->actSymbol;
    std::string c=currentColor;
    int tmpSize=this->actSize;

    this->actSize=w;
    this->actSymbol=t;

    drawSymbol(x,y);

    this->actSymbol=tmp;
    this->actSize=tmpSize;
  }

  template<class T>
  void epsDraw<T>::drawSymbol(const int x,const int y,const int w,
                              const char* style) {

    // save actual style settings
    typename drawBase<T>::eMarkerType t=this->actSymbol;
    std::string c=currentColor;
    bool f=this->filled;
    this->setStyle(style);
    marker(x,y,w,this->actSymbol);

    // restore style settings
    this->actSymbol=t;
    currentColor=c;
    objects.push_back(c);
    this->filled=f;
  }


  template<>
  void epsDraw<cmykColor>::setColor(const char* color) {
    int len=int(strlen(color));
    if (len==0 || len>2) {
      //error
      return;
    }

    cmykColor c;
    switch(color[0]) {

      case 'y':
        c=cmykColor(0,0,1,0);
        break;
      case 'm':
        c=cmykColor(0,1,0,0);
        break;
      case 'c':
        c=cmykColor(1,0,0,0);
        break;
      case 'r':
        c=cmykColor(0,1,1,0);
        break;
      case 'g':
        c=cmykColor(1,0,1,0);
        break;
      case 'b':
        c=cmykColor(1,1,0,0);
        break;
      case 'k':
        c=cmykColor(0,0,0,1);
        break;
    }
    if (len==1) {
      setColor(c);
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    int t=atoi(&color[1]);
    float f=1.0f-static_cast<float>(t)/10.0f;
    c=cmykColor(c.getCyan(),c.getMagenta(),c.getYellow(),f);
    setColor(c);

  }

  template<>
  void epsDraw<rgbPixel>::setColor(const char* color) {
    int len=int(strlen(color));
    if (len==0 || len>2) {
      //error
      return;
    }
    rgbPixel c;

    switch(color[0]) {

      case 'y':
        c= rgbColor(255, 255, 0);
        break;
      case 'm':
        c= rgbColor(255 ,0 ,255);
        break;
      case 'c':
        c= rgbColor(0, 255, 255);
        break;
      case 'r':
        c= rgbColor(255,0,0);
        break;
      case 'g':
        c= rgbColor(0,255,0);
        break;
      case 'b':
        c= rgbColor(0,0,255);
        break;
      case 'k':
        c= rgbColor(0,0,0);
        break;
      case 'w':
        c= rgbColor(255,255,255);
        break;
    }

    if (len==1) {
      setColor(c);//done, no number following
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    int t=atoi(&color[1]);
    double f=1.-double(t)/10.;

    setColor(c.multiply(f));
  }


  template<>
  void epsDraw<float>::setColor(const char* color) {
    int len=int(strlen(color));
    if (len==0 || len>2) {
      //error
      return;
    }
    float c;
    switch(color[0]) {

      case 'y':
      case 'm':
      case 'c':
        c=0.66f;
        break;
      case 'r':
      case 'g':
      case 'b':
        c=0.33f;
        break;
      case 'w':
        c=1.0f;
        break;
      case 'k':
        c=0.0f;
        break;
      default:
        //error
        return;
    }

    if (len==1) {
      //done, no number following
      setGray(c);
      return;
    }

    if (!isdigit(color[1])) {
      //error
      return;
    }

    double f=atof(&color[1]);
    f=1.-f/10.;

    setGray(float(f*c));
  }


  /*
   * draws a single,small point
   */
  template<class T>
  void epsDraw<T>::set(const int x,const int y) {
    marker(x,y,1,drawBase<T>::Pixel);
  }


  /*
   * draw the contents of the vector using the whole image
   * @param vct the vector to be shown
   * @param axisColor color for the axis.  The vector will be drawn in the
   *                  default color (see setColor())
   * @param forceAxis0 if true, both axis will be shown, i.e. the value 0 for
   *                  the vector will be always shown.  If false, only the
   *                  required value-range will be shown
   */
  template<class T>
  void epsDraw<T>::set(const ivector& vct,
                       const bool& forceAxis0) {
  }


  template<class T>
  void epsDraw<T>::polygon(const point plist[], int n, bool clp, bool fil) {
    adjustBoundingBox(plist[0].x,plist[0].y);
    moveTo(tmp,plist[0].x,plist[0].y);
    for (int i=0; i<n; i++) {
      adjustBoundingBox(plist[i].x,plist[i].y);
      lineTo(tmp,plist[i].x,plist[i].y);
    }
    if (clp) {
      closePath(tmp);
    }
    if (fil) {
      fill(tmp);
    } else {
      stroke(tmp);
    }
  }

  /*
   * Adds a comment to the (E)PS file.
   */
  template <class T>
  void epsDraw<T>::addComment(const std::string& msg) {
    std::string tmp="\n% ";
    for (unsigned int i=0; i<msg.length(); i++) {
      if (msg[i] == '\n') {
        tmp+="\n% ";
      } else {
        tmp+=msg[i];
      }
    }
    objects.push_back(tmp);
  }



  /*
   * Draw a line from the point (fx,fy) to point (tx,ty).
   * The "last point" will be defined with the last "set", "line" or
   * "lineTo" method.
   */
  template<class T>
  void epsDraw<T>::line(const int fx, const int fy,
                        const int tx, const int ty) {
    adjustBoundingBox(fx,fy,tx,ty);
    moveTo(tmp,fx,fy);
    lineTo(tmp,tx,ty);
    stroke(tmp);
    this->actX=tx;
    this->actY=ty;
  }

  // TODO: Reimplement to use the proper PS statements
  template<class T>
  void epsDraw<T>::lineTo(const int x, const int y) {
    line(this->actX,this->actY,x,y);
    this->actX=x;
    this->actY=y;
  }


  /*
   * Draws a grid in the image.
   * The interpretation of delta depends on the value of interval.
   * if interval is true, the values are taken as number of pixels
   * between two grid lines in x and y direction. Otherwise, it
   * is used as number of grid lines.
   */
  template<class T>
  void epsDraw<T>::grid(const point& delta, const bool interval) {
  }

  /*
   * Draw a line from the point p with the length "length" and the
   * angle "angle".  The angles should be a value between -2Pi and
   * 2Pi in radian.  */
  //    void epsDraw::polarLine(const point& p,const float& length, const float& angle) {
  //      float tx=length*cos(angle);
  //      float ty=length*sin(angle);

  //      line(p.x,p.y,int(tx+p.x),int(ty+p.y));
  //    }

  /*
   * Draws a vertical line from (x,y1) to (x,y2).
   */
  template<class T>
  void epsDraw<T>::verticalLine(const int x, const int y1, const int y2) {
    adjustBoundingBox(x,y1,x,y2);
    moveTo(tmp,x,y1);
    lineTo(tmp,x,y2);
    stroke(tmp);
  }

  /*
   * Draws a horizontal line from (x1,y) to (x2,y).
   */
  template<class T>
  void epsDraw<T>::horizontalLine(const int x1, const int x2, const int y) {
    adjustBoundingBox(x1,y,x2,y);
    moveTo(tmp,x1,y);
    lineTo(tmp,x2,y);
    stroke(tmp);
  }

  /*
   * draw a box
   */
  template<class T>
  void epsDraw<T>::rectangle(const int x1, const int y1,
                       const int x2, const int y2, const bool& filled) {
    adjustBoundingBox(x1,y1,x2,y2);
    moveTo(tmp,x1,y1);
    lineTo(tmp,x2,y1);
    lineTo(tmp,x2,y2);
    lineTo(tmp,x1,y2);
    closePath(tmp);
    if (filled) {
      fill(tmp);
    } else {
      stroke(tmp);
    }
  }

  /*
   * draw a circle with circle center 'p1' and radius 'r'
   */
  template<class T>
  void epsDraw<T>::circle(const point& p1,const int r, const bool& filled) {
    adjustBoundingBox(p1.x-r,p1.y-r,p1.x+r,p1.x+r);
    sprintf(numBuffer, "%d %d %d 0 360 arc", p1.x,p1.y,r);
    tmp=numBuffer;
    if (filled) {
      fill(tmp);
    } else {
      stroke(tmp);
    }
  }

  /*
   * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
   */
  template<class T>
  void epsDraw<T>::ellipse(const point& p1,const int aX, const int aY,
                           const bool& filled) {
    adjustBoundingBox(p1.x-aX,p1.y-aY,p1.x+aX,p1.x+aY);
    tmp="m cm ";
    sprintf(numBuffer,"%d %d t %d %d sc", p1.x,p1.y, aX, aY);
    tmp+=numBuffer;
    tmp+=" 0 0 1 0 360 arc sm ";
    if (filled) {
      fill(tmp);
    } else {
      stroke(tmp);
    }
  }

  /*
   * draw an ellipse with center 'p1' and main axes 'aX' and 'aY'
   */
  template<class T>
  void epsDraw<T>::ellipse(const point& p1,const int aX, const int aY,
                           const float& angle, const bool& filled) {
    tmp="gs ";
    float adeg=radToDeg(angle);
    sprintf(numBuffer,"%d %d t m cm %f r %d %d sc" , p1.x, p1.y, adeg, aX, aY);
    tmp+=numBuffer;
    tmp+=" 0 0 1 0 360 arc sm ";
    if (filled) {
      tmp+="f gr";
    } else {
      tmp+="s gr";
    }
    addCmd(tmp);
  }

  template<class T>
  void epsDraw<T>::arc(const point& p1, const point& pA, const point& pB) {

    float angle1=atan2(float(pA.x-p1.x),float(pA.y-p1.y));
    float angle2=atan2(float(pB.x-p1.x),float(pB.y-p1.y));

    float r1=sqrt(sqr(float(pA.x-p1.x))+sqr(float(pA.y-p1.y)));
    float r2=sqrt(sqr(float(pB.x-p1.x))+sqr(float(pB.y-p1.y)));

    tmp="m cm ";
    sprintf(numBuffer,"%d %d %f %f %f arc sm ", p1.x,p1.y, (r1+r2)/2.0, angle1, angle2);
    tmp+=numBuffer;
    stroke(tmp);
  }

  template<class T>
  void epsDraw<T>::number(const int num,
                          const point& p1) {
    sprintf(numBuffer, "%d", num);
    text(numBuffer, p1);
  }

  template<class T>
  void epsDraw<T>::number(const int num,
                          const int x,
                          const int y) {
    sprintf(numBuffer, "%d", num);
    text(numBuffer, x,y);
  }


  template<class T>
  void epsDraw<T>::text(const std::string txt,
                        const int x,
                        const int y,
                        const alignType h,
                        const vAlignType v) {
    adjustBoundingBox(x,y);
    moveTo(tmp,x,y);
    tmp+="(";
    tmp+=txt;
    tmp+=") ";
    if (h >= alignLeft && h <= alignRight &&
        v >= valignBottom && v <= valignTop) {
      tmp+=alignCmd[h][v];
    } else {
      tmp+=alignCmd[alignLeft][valignBottom];
    }
    addCmd(tmp);
    stroke(tmp);
  }


  template<class T>
  void epsDraw<T>::arrow(const int fx, const int fy,
                         const int tx, const int ty,
                         const float& size) {
    adjustBoundingBox(fx,fy,tx,ty);
    moveTo(tmp,fx,fy);
    lineTo(tmp,tx,ty);
    const float angle=atan2(float(ty-fy),float(tx-fx));
    float sizeTemp = fabs(size);
    int xTemp, yTemp;
    float sina, cosa;
    const float pi43 = 0.75f*static_cast<float>(Pi);

    if (sizeTemp<=1.0) {
      sizeTemp = sizeTemp*sqrt((ty-fy)*(ty-fy)+(tx-fx)*(tx-fx));
    }

    sincos(angle+pi43, sina, cosa);
    xTemp = static_cast<int>(tx+sizeTemp*cosa);
    yTemp = static_cast<int>(ty+sizeTemp*sina);
    lineTo(tmp,xTemp,yTemp);

    sincos(angle-pi43, sina, cosa);
    xTemp = static_cast<int>(tx+sizeTemp*cosa);
    yTemp = static_cast<int>(ty+sizeTemp*sina);
    lineTo(tmp,xTemp,yTemp);
    lineTo(tmp,tx,ty);
    closePath(tmp);
    stroke(tmp);
  }


  template<class T>
  const std::string& epsDraw<T>::getLinePatName(const linePattern& x) {
    std::string name=linePatternNames[x];
    if (name == "") {
      linepatCount++;
      sprintf(numBuffer, "lp%d",linepatCount);
      linePatternNames[x]=numBuffer;
    }
    return linePatternNames[x];
  }

  template<class T>
  const std::string& epsDraw<T>::getFillPatName(const fillPattern& x) {
    std::string name=fillPatternNames[x];
    if (name == "") {
      fillpatCount++;
      sprintf(numBuffer, "fp%d",fillpatCount);
      fillPatternNames[x]=numBuffer;
    }
    return fillPatternNames[x];
  }

  template<class T>
  const std::string& epsDraw<T>::getColorName(const rgbColor& x) {
    std::string name=rgbColorNames[x];
    if (name == "") {
      colorCount++;
      sprintf(numBuffer, "rgb%d",colorCount);
      rgbColorNames[x]=numBuffer;
    }
    return rgbColorNames[x];
  }

  template<class T>
  const std::string& epsDraw<T>::getColorName(const cmykColor& x) {
    std::string name=cmykColorNames[x];
    if (name == "") {
      colorCount++;
      sprintf(numBuffer, "cmyk%d",colorCount);
      cmykColorNames[x]=numBuffer;
      //        cout << "cmykColorNames[cmykColor(1,1,0,0)]"
      //             << cmykColorNames[cmykColor(1,1,0,0)] << endl;
    }
    return cmykColorNames[x];
  }

  // internal stuff

  template<class T>
  void epsDraw<T>::adjustBoundingBox(int x, int y) {
    if (x < bbox.ul.x) {
      bbox.ul.x=x;
    }
    if (x > bbox.br.x) {
      bbox.br.x=x;
    }
    if (y < bbox.ul.y) {
      bbox.ul.y=y;
    }
    if (y > bbox.br.y) {
      bbox.br.y=y;
    }
  }

  template<class T>
  void epsDraw<T>::moveTo(std::string& tmp, int x, int y, bool init) {
    if (init) {
      tmp="";
    }
    sprintf(numBuffer,"%d %d mt ",x,y);
    tmp+=numBuffer;
  }

  template<class T>
  void epsDraw<T>::lineTo(std::string& tmp, int x, int y) {
    sprintf(numBuffer,"%d %d lt ",x,y);
    tmp+=numBuffer;
  }

  template<class T>
  void epsDraw<T>::rlineTo(std::string& tmp, int x, int y) {
    sprintf(numBuffer,"%d %d rl ",x,y);
    tmp+=numBuffer;
  }

  template<class T>
  void epsDraw<T>::moveTo(std::string& tmp, float x, float y, bool init) {
    if (init) {
      tmp="";
    }
    sprintf(numBuffer,"%4.2f %4.2f mt ",x,y);
    tmp+=numBuffer;
  }

  template<class T>
  void epsDraw<T>::lineTo(std::string& tmp, float x, float y) {
    sprintf(numBuffer,"%4.2f %4.2f lt ",x,y);
    tmp+=numBuffer;
  }

  template<class T>
  void epsDraw<T>::rlineTo(std::string& tmp, float x, float y) {
    sprintf(numBuffer,"%4.2f %4.2f rl ",x,y);
    tmp+=numBuffer;
  }

  template<class T>
  void epsDraw<T>::closePath(std::string& tmp) {
    tmp+="cp";
  }

  template<class T>
  bool epsDraw<T>::addImageCommand(bool color, int width, int height,
                                   imageCoding comp, const point& offset,
                                   const dataCodec::buffer& src,
                                   const bool blackWhite) {
    static const int linelen=80;
    char buf[linelen+2];

    sprintf(buf,"%d",height);
    std::string rows=buf;
    sprintf(buf,"%d",width);
    std::string cols=buf;

    if (blackWhite && color) {
      this->setStatusString("Cannot encode color images in black and white");
      return false;
    }

    tmp+=color ? "/DeviceRGB" : "/DeviceGray";
    tmp+=" setcolorspace\n";
    tmp+="<<\n/ImageType 1\n";
    tmp+="/Width ";
    tmp+=cols+"\n";
    tmp+="/Height ";
    tmp+=rows+"\n";
    // the next lines are correct if the transform given by
    // setTransform is an identity; however, we cannot determine that
    // at this point
    //     tmp+="/ImageMatrix [ 1 0 0 -1 ";
    //     //tmp+="1+" 0 0 ";
    //     //tmp+=rows+" 0 ";
    //     sprintf(buf,"%d ",offset.x);
    //     tmp+=buf;
    //     sprintf(buf,"%d ",offset.y+height);
    //     tmp+=buf;
    //     tmp+="]\n";
    // this is an identity transform, but moved by the offset
    tmp+="/ImageMatrix [ 1 0 0 1 ";
    sprintf(buf,"%d %d ]\n",offset.x,offset.y);
    tmp+=buf;
    // old version: ignored the offset
    //tmp+="/ImageMatrix [ 1 0 0 1 0 0 ]\n";
    if (blackWhite) {
      tmp+="/BitsPerComponent 1\n";
    } else {
      tmp+="/BitsPerComponent 8\n";
    }
    tmp+="/Decode ";
    tmp+=color ? "[ 0 1 0 1 0 1 ]\n" : "[ 0 1 ]\n";

    dataCodec* codec;
    
    std::string decoderName;
    std::string eod;
    switch (comp) {
      case asciiHex:
        codec=new asciiHexCodec();
        decoderName="/ASCIIHexDecode";
        eod=">";
        break;
      case ascii85:
        codec=new ascii85Codec();
        decoderName="/ASCII85Decode";
        eod="~>";
        break;
#if HAVE_LIBZ
      case flate:
        codec=new combinedCodec(flateCodec(),ascii85Codec());
        decoderName="/ASCII85Decode filter /FlateDecode";
        eod="~>";
        break;
#endif
      case jpeg:
        // JPEG compression has been performed by the caller
        codec=new ascii85Codec();
        decoderName="/ASCII85Decode filter /DCTDecode";
        eod="~>";
        break;
      case runLength:
        codec=new combinedCodec(runLengthCodec(),ascii85Codec());
        decoderName="/ASCII85Decode filter /RunLengthDecode";
        eod="~>";
        break;
      default:
        codec=new ascii85Codec();
        decoderName="/ASCII85Decode";
        eod="~>";
        break;
    }

    tmp+="/DataSource currentfile ";
    tmp+=decoderName;
    tmp+=" filter\n";
    tmp+=">> image\n";

    int size=src.size();

    dataCodec::buffer dest(codec->estimateEncodedSize(size));

    if (codec->encode(src,dest)) {

      // now print coding buffer
      buf[linelen]='\n';
      buf[linelen+1]='\0';

      int r=dest.size()%linelen;
      int n=dest.size()-r;
      for (int i=0; i < n; i+=linelen) {
        memcpy(buf,&dest.at(i),linelen);
        tmp+=buf;
      }
      if (r != 0) {
        memcpy(buf,&dest.at(n),r);
        buf[r]='\n';
        buf[r+1]='\0';
        tmp+=buf;
      }
      tmp+=eod;
      tmp+="\n";
      return true;
    } else {
      return false;
    }
  }

  //  template<class T>
  //    bool epsDraw<T>::addImageCommand(bool color, int width, int height,
  //                                  imageCoding comp, const point& offset,
  //                                  const dataCodec::buffer& src) {
  //      static const int linelen=80;
  //      char buf[linelen+2];

  //      sprintf(buf,"%d",height);
  //      std::string rows=buf;
  //      sprintf(buf,"%d",width);
  //      std::string cols=buf;

  //      tmp+=color ? "/DeviceRGB" : "/DeviceGray";
  //      tmp+=" setcolorspace\n";
  //      tmp+="<<\n/ImageType 1\n";
  //      tmp+="/Width ";
  //      tmp+=cols+"\n";
  //      tmp+="/Height ";
  //      tmp+=rows+"\n";
  //      // the next lines are correct if the transform given by
  //      // setTransform is an identity; however, we cannot determine that
  //      // at this point
  //  //     tmp+="/ImageMatrix [ 1 0 0 -1 ";
  //  //     //tmp+="1+" 0 0 ";
  //  //     //tmp+=rows+" 0 ";
  //  //     sprintf(buf,"%d ",offset.x);
  //  //     tmp+=buf;
  //  //     sprintf(buf,"%d ",offset.y+height);
  //  //     tmp+=buf;
  //  //     tmp+="]\n";
  //      // this is an identity transform
  //      tmp+="/ImageMatrix [ 1 0 0 1 0 0 ]\n";
  //      tmp+="/BitsPerComponent 8\n";
  //      tmp+="/Decode ";
  //      tmp+=color ? "[ 0 1 0 1 0 1 ]\n" : "[ 0 1 ]\n";

  //      dataCodec* codec;
  //      std::string decoderName;
  //      std::string eod;
  //      switch (comp) {
  //        case asciiHex:
  //          codec=new asciiHexCodec();
  //          decoderName="/ASCIIHexDecode";
  //          eod=">";
  //          break;
  //        case ascii85:
  //          codec=new ascii85Codec();
  //          decoderName="/ASCII85Decode";
  //          eod="~>";
  //          break;
  //  #if __linux__
  //        case flate:
  //          codec=new combinedCodec(flateCodec(),ascii85Codec());
  //          decoderName="/ASCII85Decode filter /FlateDecode";
  //          eod="~>";
  //          break;
  //  #endif
  //        case runLength:
  //          codec=new combinedCodec(runLengthCodec(),ascii85Codec());
  //          decoderName="/ASCII85Decode filter /RunLengthDecode";
  //          eod="~>";
  //          break;
  //        default:
  //          codec=new ascii85Codec();
  //          decoderName="/ASCII85Decode";
  //          eod="~>";
  //          break;
  //      }

  //      tmp+="/DataSource currentfile ";
  //      tmp+=decoderName;
  //      tmp+=" filter\n";
  //      tmp+=">> image\n";

  //      int size=width*height*(color ? 3 : 1);

  //      dataCodec::buffer dest(codec->estimateEncodedSize(size));

  //      if (codec->encode(src,dest)) {

  //        // now print coding buffer
  //        buf[linelen]='\n';
  //        buf[linelen+1]='\0';

  //        int r=dest.size()%linelen;
  //        int n=dest.size()-r;
  //        for (int i=0; i < n; i+=linelen) {
  //          memcpy(buf,&dest.at(i),linelen);
  //          tmp+=buf;
  //        }
  //        if (r != 0) {
  //          memcpy(buf,&dest.at(n),r);
  //          buf[r]='\n';
  //          buf[r+1]='\0';
  //          tmp+=buf;
  //        }
  //        tmp+=eod;
  //        tmp+="\n";
  //        return true;
  //      } else {
  //        return false;
  //      }
  //    }


  /*
   * Draws an image, using grayscale pixels. The image is drawn
   * in the coordinate space from (offset.x,offset.y) to
   * (offset.x+img.columns,offset.y+img.rows)
   */
  //  template<class T>
  //    void epsDraw<T>::drawImage(channel8& img, imageCoding comp, const point& offset) {
  //      int width=img.columns();
  //      int height=img.rows();

  //      adjustBoundingBox(offset.x,offset.y, offset.x+width,offset.y+height);

  //      tmp="currentcolorspace\n";

  //      dataCodec::buffer src(width*height,&img.at(0,0),true);

  //      if (addImageCommand(false, width,height,comp,offset,src)) {
  //        tmp+="setcolorspace\n";
  //        addCmd(tmp);
  //      } else {
  //        sprintf(numBuffer," %d %d %d %d rectfill ",offset.x,offset.y,
  //                width,height);
  //        tmp="gsave ";
  //        std::string cn=getColorName(rgbColor(197,218,249));
  //        tmp+=cn;
  //        tmp+=numBuffer;
  //        moveTo(tmp,offset.x+width/2,offset.y+height/2,false);
  //        cn=getColorName(rgbColor(168,0,0));
  //        tmp+=cn;
  //        tmp+=" (Could not encode image) ";
  //        tmp+=alignCmd[alignCenter][valignMiddle];
  //        tmp+=" grestore";
  //        addCmd(tmp);
  //      }
  //    }

  template<class T>
  void epsDraw<T>::drawImage(const channel8& img,
                             const imageCoding comp,
                             const point& offset,
                             bool blackWhite,
                             const int t) {
    int width=img.columns();
    int height=img.rows();

    adjustBoundingBox(offset.x,offset.y, offset.x+width,offset.y+height);

    tmp="currentcolorspace\n";

    dataCodec::buffer *src;
    channel8 tmpch;
    
    if (comp == jpeg) {
#if defined(HAVE_LOCALJPEG) || defined(HAVE_LIBJPEG)
      processInfo host;
      saveJPEG saver;
      // we need a temporary JPEG file
      char buf[80];
#ifdef _LTI_LINUX
      sprintf(buf,"/tmp/ltiEpsDraw.%ld", host.getPid());
#else
#ifdef _LTI_WIN32
      sprintf(buf,"C:/TEMP/lti%ld", host.getPid());
#else
      sprintf(buf,"lti%ld", host.getPid());
#endif
#endif
      saver.save(buf,img);
      src=new dataCodec::buffer(0);
      src->loadRaw(buf);
      remove(buf);
#endif
    } else {
      ubyte* rawbuf=const_cast<ubyte*>(&img.at(0,0));
      if (blackWhite && width%8 != 0) {
        width+=(8-width%8);
        tmpch.copy(img);
        tmpch.resize(height,width);
        rawbuf=&tmpch.at(0,0);
      }
      src=new dataCodec::buffer(width*height,rawbuf,true);
      if (blackWhite) {
        binarizationCodec binCodec; 
        binarizationCodec::parameters binPar=binCodec.getParameters(); 
        binPar.threshold=t;
        binCodec.setParameters(binPar);
        dataCodec::buffer *src2;
        src2=new dataCodec::buffer(binCodec.estimateEncodedSize(width*height));
        if (binCodec.encode(*src,*src2)) {
          delete src;
          src=src2;
        } else {
          delete src2;
          this->setStatusString("Error in binarization, using raw data");
          blackWhite=false;
        }
      }
    }

    if (addImageCommand(false, width,height,comp,offset,*src,blackWhite)) {
      tmp+="setcolorspace\n";
      addCmd(tmp);
    } else {
      sprintf(numBuffer," %d %d %d %d rectfill ",offset.x,offset.y,
              width,height);
      tmp="gsave ";
      std::string cn=getColorName(rgbColor(197,218,249));
      tmp+=cn;
      tmp+=numBuffer;
      moveTo(tmp,offset.x+width/2,offset.y+height/2,false);
      cn=getColorName(rgbColor(168,0,0));
      tmp+=cn;
      tmp+=" (Could not encode image) ";
      tmp+=alignCmd[alignCenter][valignMiddle];
      tmp+=" grestore";
      addCmd(tmp);
    }
    delete src;
  }

  /*
   * Draws an image, using RGB color pixels. The image is drawn
   * in the coordinate space from (offset.x,offset.y) to
   * (offset.x+img.columns,offset.y+img.rows)
   */
  template<class T>
  void epsDraw<T>::drawImage(const image& img,
                             const imageCoding comp,
                             const point& offset) {
    int width=img.columns();
    int height=img.rows();

    adjustBoundingBox(offset.x,offset.y, offset.x+width,offset.y+height);

    tmp="currentcolorspace\n";

    dataCodec::buffer* src;

    if (comp == jpeg) {
#if defined(HAVE_LOCALJPEG) || defined(HAVE_LIBJPEG)
      processInfo host;
      saveJPEG saver;
      // we need a temporary JPEG file
      char buf[80];
#ifdef __linux__
      sprintf(buf,"/tmp/ltiEpsDraw.%ld", host.getPid());
#else
#ifdef _LTI_WIN32
      sprintf(buf,"C:/TEMP/lti%ld", host.getPid());
#else
      sprintf(buf,"lti%ld", host.getPid());
#endif
#endif
      saver.save(buf,img);
      src=new dataCodec::buffer(0);
      src->loadRaw(buf);
      remove(buf);
#endif
    } else {
      src=new dataCodec::buffer(width*height*3);
      dataCodec::buffer::iterator di=src->begin();
      for (image::const_iterator si=img.begin(); si != img.end(); si++) {
        *di++=(*si).getRed();
        *di++=(*si).getGreen();
        *di++=(*si).getBlue();
      }
    }


    if (addImageCommand(true, width,height,comp,offset,*src)) {
      tmp+="setcolorspace\n";
      addCmd(tmp);
    } else {
      sprintf(numBuffer,"%d %d %d %d rectfill ",offset.x,offset.y,
              width,height);
      tmp="gsave ";
      tmp+="0.9 g ";
      tmp+=numBuffer;
      moveTo(tmp,offset.x+width/2,offset.y+height/2,false);
      tmp+="0 g (Could not encode image) ";
      tmp+=alignCmd[alignCenter][valignMiddle];
      tmp+=" grestore";
      addCmd(tmp);
    }
    delete src;
  }


  template<class T>
  void epsDraw<T>::drawSymbol(const int x,const int y) {
    float w=static_cast<float>(this->actSize);
    switch (this->actSymbol) {
      case drawBase<T>::Dot:
      case drawBase<T>::Circle:
        sprintf(numBuffer, "%d %d %f 0 360 arc", x,y,w/2.0f);
        if (this->filled) {
          fill(numBuffer);
        } else {
          stroke(numBuffer);
        }
        break;
      case drawBase<T>::Xmark:
        moveTo(tmp,x-w/2.0f,y-w/2.0f);
        rlineTo(tmp,w,w);
        moveTo(tmp,x+w/2.0f,y-w/2.0f,false);
        rlineTo(tmp,-w,w);
        stroke(tmp);
        break;
      case drawBase<T>::Plus:
        moveTo(tmp,static_cast<float>(x),y-w/2.0f);
        rlineTo(tmp,0.0f,w);
        moveTo(tmp,x-w/2.0f,static_cast<float>(y),false);
        rlineTo(tmp,w,0.0f);
        stroke(tmp);
        break;
      case drawBase<T>::Star:
        moveTo(tmp,x-w/2.0f,y-(w/2.0f)/1.73f);
        rlineTo(tmp,w,w/1.73f);
        moveTo(tmp,x+w/2.0f,y-w/2.0f/1.73f,false);
        rlineTo(tmp,-w,w/1.73f);
        moveTo(tmp,static_cast<float>(x),y-w/2.0f,false);
        rlineTo(tmp,0.0f,w);
        stroke(tmp);
        break;
      case drawBase<T>::Pixel:
        w=1;
      case drawBase<T>::Square:
        moveTo(tmp,x-w/2.0f,y-w/2.0f);
        rlineTo(tmp,w,0.0f);
        rlineTo(tmp,0.0f,w);
        rlineTo(tmp,-w,0.0f);
        closePath(tmp);
        if (this->filled) {
          fill (tmp);
        } else {
          stroke(tmp);
        }
        break;
      case drawBase<T>::Diamond:
        {
          moveTo(tmp,static_cast<float>(x),y-w/2.0f);
          rlineTo(tmp,-w/2.0f,w/2.0f);
          rlineTo(tmp,w/2.0f,w/2.0f);
          rlineTo(tmp,w/2.0f,-w/2.0f);
          closePath(tmp);
          if (this->filled) {
            fill(tmp);
          } else {
            stroke(tmp);
          }
          break;
        }
      case drawBase<T>::TriangleUp:
        {
          float dx=w/2.0f*0.866f;   // sin(30)
          float dy=w/2.0f*0.5f; // cos(30)
          moveTo(tmp,x-dx,y+dy);
          rlineTo(tmp,2*dx,0.0f);
          rlineTo(tmp,-dx,-w/2.0f-dy);
          closePath(tmp);
          if (this->filled) {
            fill(tmp);
          } else {
            stroke(tmp);
          }
          break;
        }
      case drawBase<T>::TriangleDown:
        {
          float dx=w/2.0f*0.866f;   // sin(30)
          float dy=w/2.0f*0.5f;     // cos(30)
          moveTo(tmp,x-dx,y-dy);
          rlineTo(tmp,2*dx,0.0f);
          rlineTo(tmp,-dx,w/2.0f+dy);
          closePath(tmp);
          if (this->filled) {
            fill(tmp);
          } else {
            stroke(tmp);
          }
          break;
        }
      case drawBase<T>::TriangleLeft:
        {
          float dy=w/2.0f*0.866f;   // sin(30)
          float dx=w/2.0f*0.5f; // cos(30)
          moveTo(tmp,x+dx,y-dy);
          rlineTo(tmp,0.0f,2.0f*dy);
          rlineTo(tmp,-w/2.0f-dx,-dy);
          closePath(tmp);
          if (this->filled) {
            fill(tmp);
          } else {
            stroke(tmp);
          }

          break;
        }
      case drawBase<T>::TriangleRight:
        {
          float dy=w/2.0f*0.866f;   // sin(30)
          float dx=w/2.0f*0.5f; // cos(30)
          moveTo(tmp,static_cast<float>(x-dx),static_cast<float>(y-dy));
          rlineTo(tmp,0.0f,2.0f*dy);
          rlineTo(tmp,w/2.0f+dx,-dy);
          closePath(tmp);
          if (this->filled) {
            fill(tmp);
          } else {
            stroke(tmp);
          }

          break;
        }
      case drawBase<T>::LtiLogo:
        {
          float dx=w/9.0f;
          float dy=w/9.0f;
          moveTo(tmp,x-4*dx,y-4*dy);
          rlineTo(tmp,8*dx,0.0f);
          rlineTo(tmp,0.0f,2*dy);
          rlineTo(tmp,-3*dx,0.0f);
          rlineTo(tmp,0.0f,3*dy);
          rlineTo(tmp,-2*dx,0.0f);
          rlineTo(tmp,0.0f,-3*dy);
          rlineTo(tmp,-3*dx,0.0f);
          closePath(tmp);
          fill(tmp);
          moveTo(tmp,x-4*dy,y+4*dy);
          rlineTo(tmp,5*dx,0.0f);
          rlineTo(tmp,0.0f,-2*dy);
          rlineTo(tmp,-3*dx,0.0f);
          rlineTo(tmp,0.0,-3*dy);
          rlineTo(tmp,-2*dx,0.0f);
          closePath(tmp);
          fill(tmp);
          moveTo(tmp,x+4*dy,y+4*dy);
          rlineTo(tmp,0.0f,-5*dy);
          rlineTo(tmp,-2*dx,0.0f);
          rlineTo(tmp,0.0f,5*dy);
          closePath(tmp);
          fill(tmp);
          break;
        }
      default :
        break;
    }
  }

  /*
   * Draws an image, using RGB color pixels. The image is drawn
   * in the coordinate space from (offset.x,offset.y) to
   * (offset.x+img.columns,offset.y+img.rows)
   */
  //  template<class T>
  //    void epsDraw<T>::drawImage(image& img, imageCoding comp, const point& offset) {
  //      int width=img.columns();
  //      int height=img.rows();

  //      adjustBoundingBox(offset.x,offset.y, offset.x+width,offset.y+height);

  //      tmp="currentcolorspace\n";

  //      dataCodec::buffer src(width*height*3);

  //      dataCodec::buffer::iterator di=src.begin();
  //      for (image::const_iterator si=img.begin(); si != img.end(); si++) {
  //        *di++=(*si).getRed();
  //        *di++=(*si).getGreen();
  //        *di++=(*si).getBlue();
  //      }

  //      if (addImageCommand(true, width,height,comp,offset,src)) {
  //        tmp+="setcolorspace\n";
  //        addCmd(tmp);
  //      } else {
  //        sprintf(numBuffer,"%d %d %d %d rectfill ",offset.x,offset.y,
  //                width,height);
  //        tmp="gsave ";
  //        tmp+="0.9 g ";
  //        tmp+=numBuffer;
  //        moveTo(tmp,offset.x+width/2,offset.y+height/2,false);
  //        tmp+="0 g (Could not encode image) ";
  //        tmp+=alignCmd[alignCenter][valignMiddle];
  //        tmp+=" grestore";
  //        addCmd(tmp);
  //      }

  //    }



  template<class T>
  void epsDraw<T>::close() {
    if (objects.size() > 0) {
      writeHeader();
      writeProlog();
      writeSetup();
      writeObjects();
      writeTrailer();
      out.flush();
      rgbColorNames.clear();
      cmykColorNames.clear();
      objects.clear();
    }
  }


  template<class T>
  std::string epsDraw<T>::getBoundingBox() {
    makeTransform();
    int width=dimension.x;
    int height=dimension.y;
    // there should be a nicer way to include the resolution factor
    if (isEps) {
      sprintf(numBuffer,"0 0 %d %d", 
              int(width/resolution+0.5), 
              int(height/resolution+0.5));
    } else {
      int lx=(a4Width-width)/2;
      int ly=(a4Height-height)/2;
      sprintf(numBuffer,"%d %d %d %d",
              int(lx/resolution+0.5),
              int(ly/resolution+0.5),
              int((lx+width)/resolution+0.5),
              int((ly+height)/resolution+0.5));
    }
    return numBuffer;
  }

  template<class T>
  void epsDraw<T>::writeHeader() {
    if (isEps) {
      out << "%!PS-Adobe-3.0 EPSF 2.0\n";
    } else {
      out << "%!PS-Adobe-3.0\n";
    }
    dsc("Creator", "LTIlib");
    dsc("Title", title);
    processInfo pinf;
    dsc("For", pinf.getUsername()+"@"+pinf.getHostname());
    dsc("CreationDate", timer::getDateAndTime());
    dsc("LanguageLevel", "2");
    dsc("BoundingBox", getBoundingBox());
    dsc("DocumentNeededFonts","Helvetica");
  }

  template<class T>
  void epsDraw<T>::writeProlog() {

    dsc("BeginProlog");
    // use own dictionary
    int size=50;
    size+=colorCount;
    sprintf(numBuffer, "/LTIlib %d dict begin",size);
    cmd(numBuffer);
    cmd("/bd {bind def} bind def");
    cmd("/ld {load def} bind def");
    bdef("xd", "exch def");
    // operator abbreviations
    ldef("cp", "closepath");
    ldef("s", "stroke");
    ldef("f", "fill");
    ldef("mt", "moveto");
    ldef("lt", "lineto");
    ldef("rt", "rmoveto");
    ldef("rl", "rlineto");
    ldef("m", "matrix");
    ldef("sm", "setmatrix");
    ldef("cm", "currentmatrix");
    ldef("t", "translate");
    ldef("r", "rotate");
    ldef("sc", "scale");
    ldef("g", "setgray");
    ldef("lw", "setlinewidth");
    ldef("gs", "gsave");
    ldef("gr", "grestore");
    writeColorDefinitions();
    writePatternDefinitions();

    sprintf(numBuffer, "%d",fontSize);
    def("fsize",numBuffer);
    sprintf(numBuffer, "%f",xScale/resolution);
    def("xscale", numBuffer);
    sprintf(numBuffer, "%f",yScale/resolution);
    def("yscale", numBuffer);
    bdef("sw", "dup stringwidth pop");
    bdef("sh", "fsize yscale neg div");

    //bdef("oshow","dup gsave false charpath 0 g 1 setlinewidth stroke grestore show");
    ldef("oshow","show");

    // text, alignment LEFT BOTTOM
    bdef(alignCmd[alignLeft][valignBottom],
         "oshow");
    // text, alignment CENTER BOTTOM
    bdef(alignCmd[alignCenter][valignBottom],
         "sw neg 2 div 0 rt oshow");
    // text, alignment RIGHT BOTTOM
    bdef(alignCmd[alignRight][valignBottom],
         "sw neg 0 rt oshow");
    // text, alignment LEFT MIDDLE
    bdef(alignCmd[alignLeft][valignMiddle],
         "sh 2 div 0 exch rt oshow");
    // text, alignment CENTER MIDDLE
    bdef(alignCmd[alignCenter][valignMiddle],
         "sw neg 2 div sh 2 div rt oshow");
    // text, alignment RIGHT MIDDLE
    bdef(alignCmd[alignRight][valignMiddle],
         "sw neg sh 2 div rt oshow");
    // text, alignment LEFT TOP
    bdef(alignCmd[alignLeft][valignTop],
         "0 sh rt oshow");
    // text, alignment CENTER TOP
    bdef(alignCmd[alignCenter][valignTop],
         "sw neg 2 div sh rt oshow");
    // text, alignment RIGHT TOP
    bdef(alignCmd[alignRight][valignTop],
         "sw neg sh rt oshow");

    cmd("currentdict end def");
    dsc("EndProlog");
  }

  template<class T>
  void epsDraw<T>::writePatternDefinitions() {
    std::map<linePattern,std::string,graphicsPattern::less>::const_iterator i=linePatternNames.begin();
    while (i != linePatternNames.end()) {
      const linePattern& c=i->first;
      const std::string& name=i->second;

      ivector dash;
      int offset;
      c.getDash(dash,offset);
      char tmp[256];
      sprintf(tmp,"[");

      for (int k=0; k<dash.size(); k++) {
        sprintf(numBuffer," %d",dash.at(k));
        strcat(tmp,numBuffer);
      }
      sprintf(numBuffer," ] %d setdash",offset);
      strcat(tmp,numBuffer);

      bdef(name,tmp);
      i++;
    }

    std::map<fillPattern,std::string,graphicsPattern::less>::const_iterator j=fillPatternNames.begin();
    while (j != fillPatternNames.end()) {
      const fillPattern& c=j->first;
      const std::string& name=j->second;

      // use "<< /PaintType 2\n" for uncolored patterns
      point bb=c.getBoundingBox().getDimensions();
      sprintf(numBuffer,"[ %d %d %d %d ]\n",0,0,bb.x-1,bb.y-1);
      std::string pdef=
        "<<\n  /PaintType 2\n" \
        "  /PatternType 1 /TilingType 3\n" \
        "  /BBox ";
      pdef+=numBuffer;
      sprintf(numBuffer, "  /XStep %d /YStep %d\n", bb.x-1,bb.y-1);
      pdef+=numBuffer;
      pdef+="  /PaintProc { begin \n    ";
      const fillPattern::objectList& objects=c.getObjects();
      fillPattern::objectList::const_iterator i=objects.begin();
      while (i != objects.end()) {
        // we now have a polygon to draw
        fillPattern::internalObject::const_iterator j=(*i)->begin();
        moveTo(pdef,j->x,j->y,false);
        pdef+="\n    ";
        j++;
        while (j != (*i)->end()) {
          lineTo(pdef,j->x,j->y);
          pdef+="\n    ";
          j++;
        }
        i++;
      }
      pdef+="closepath fill end\n";
      // insert here the definition from the pattern objects
      pdef+="  } >> matrix makepattern";
      // store the pattern definition
      def(name+"def",pdef);

      pdef=" currentcolor [/Pattern currentcolorspace] setcolorspace ";
      pdef+=name+"def setpattern";
      bdef(name,pdef);
      j++;
    }


  }


  template<class T>
  void epsDraw<T>::writeColorDefinitions() {
    std::map<rgbColor,std::string>::const_iterator i=rgbColorNames.begin();
    while (i != rgbColorNames.end()) {
      const rgbColor& c=i->first;
      const std::string& name=i->second;
      sprintf(numBuffer,"%6.4f %6.4f %6.4f setrgbcolor",
              float(c.getRed())/255.0,
              float(c.getGreen())/255.0,
              float(c.getBlue())/255.0);
      bdef(name,numBuffer);
      i++;
    }
    std::map<cmykColor,std::string>::const_iterator j=cmykColorNames.begin();
    while (j != cmykColorNames.end()) {
      const cmykColor& c=j->first;
      const std::string& name=j->second;
      sprintf(numBuffer,"%f %f %f %f setcmykcolor",c.getCyan(),
              c.getMagenta(),c.getYellow(), c.getBlack());
      bdef(name,numBuffer);
      j++;
    }
  }

  template<class T>
  void epsDraw<T>::makeTransform() {
    if (doClip) {
      if (bbox.ul.x < clipRect.ul.x) {
        bbox.ul.x=clipRect.ul.x;
      }
      if (bbox.br.x > clipRect.br.x) {
        bbox.br.x=clipRect.br.x;
      }
      if (bbox.ul.y < clipRect.ul.y) {
        bbox.ul.y=clipRect.ul.y;
      }
      if (bbox.br.y > clipRect.br.y) {
        bbox.br.y=clipRect.br.y;
      }
    }

    float dx=static_cast<float>(bbox.br.x-bbox.ul.x);
    float dy=static_cast<float>(bbox.br.y-bbox.ul.y);
    int width=dimension.x;
    int height=dimension.y;
    if (width < 0) {
      width=(int)((float)height*dx/dy);
    } else if (height < 0) {
      height=(int)((float)width*dy/dx);
    }
    if (!isEps) {
      if (width > a4Width) {
        float scale=(float)a4Width/(float)width;
        width=a4Width;
        height=(int)((float)height*scale);
      }
      if (height > a4Height) {
        float scale=(float)a4Height/(float)height;
        height=a4Height;
        width=(int)((float)width*scale);
      }
    }

    dimension.x=width;
    dimension.y=height;
    if (autoScale) {
      xScale=static_cast<float>(width/dx);
      yScale=-static_cast<float>(height/dy);
      xOffset=static_cast<float>(-bbox.ul.x);
      yOffset=static_cast<float>(-bbox.br.y);
    }

  }

  template<class T>
  void epsDraw<T>::writeSetup() {
    dsc("BeginSetup");
    // now, write the transform from user space to PS space
    // we have the user bounding box in the limits arrays

    cmd("LTIlib begin");
    cmd("/fmatrix fsize xscale div fsize yscale div matrix scale def");
    cmd("end");
    dsc("EndSetup");
  }

  template<class T>
  void epsDraw<T>::writeTrailer() {
    dsc("Trailer");
    dsc("EOF");
  }

  template<class T>
  void epsDraw<T>::writeObjects() {
    if (!isEps) {
      dsc("Page: 1 1");
    }
    cmd("save");
    cmd("LTIlib begin");
    int width=dimension.x;
    int height=dimension.y;  
    if (!isEps) {
      sprintf(numBuffer, "%d %d t", (a4Width-width)/2,
              (a4Height-height)/2);
      cmd(numBuffer);
    }
    if (doClip) {
      sprintf(numBuffer, "0 0 %d %d rectclip",width,height);
      cmd(numBuffer);
    }
    cmd("xscale yscale sc");
    sprintf(numBuffer, "%f %f t", xOffset, yOffset);
    cmd(numBuffer);
    cmd("/Helvetica findfont fmatrix makefont setfont");
    cmd("0 setlinewidth");
    std::list<std::string>::const_iterator i=objects.begin();
    while (i != objects.end()) {
      cmd(*i++);
    }
    cmd("end");
    if (!isEps) {
      cmd("showpage");
    }
    cmd("restore");
  }





}

