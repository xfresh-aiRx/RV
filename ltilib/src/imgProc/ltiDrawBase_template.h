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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiDrawBase.cpp
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 19.11.2002
 * revisions ..: $Id: ltiDrawBase_template.h,v 1.6 2005/10/28 15:56:41 doerfler Exp $
 */


namespace lti {

  // --------------------------------------------------
  // drawBase
  // --------------------------------------------------

  // default constructor
template<class T>
  drawBase<T>::drawBase()
    : object(){
  }

 // copy member
template<class T>
  drawBase<T>& drawBase<T>::copy(const drawBase& other) {
    return (*this);
  }

  // copy constructor
template<class T>
  drawBase<T>::drawBase(const drawBase& other)
    : object() {
    copy(other);
  }

  // destructor
template<class T>
  drawBase<T>::~drawBase() {
  }

  // returns the name of this type
template<class T>
  const char* drawBase<T>::getTypeName() const {
    return "drawBase";
  }

  template <class T>
  void drawBase<T>::setLinePattern(const linePattern& pat) {
    linepat=pat;
    useLinePattern=true;
  }

  template <class T>
  void drawBase<T>::setFillPattern(const fillPattern& pat) {
    fillpat=pat;
    useFillPattern=true;
  }


  template<class T>
  void drawBase<T>::setMarkerType(const eMarkerType style) {
    actSymbol=style;
  }

  template<class T>
  void drawBase<T>::setMarkerSize(const int s) {
    actSize=s;
  }

  template<class T>
  void drawBase<T>::setStyle(const char* style) {

    int len=int(strlen(style));

    filled=false;

    if (len==0 || len>4) {
      //error
      return;
    }

    switch(style[0]) {

    case '.':
      setMarkerType(Pixel);
      break;
    case 'o':
      setMarkerType(Circle);
      break;
    case 'x':
      setMarkerType(Xmark);
      break;
    case '+':
      setMarkerType(Plus);
      break;
    case '*':
      setMarkerType(Star);
      break;
    case 's':
      setMarkerType(Square);
      break;
    case 'd':
      setMarkerType(Diamond);
      break;
    case '^':
      setMarkerType(TriangleUp);
      break;
    case 'v':
      setMarkerType(TriangleDown);
      break;
    case '<':
      setMarkerType(TriangleLeft);
      break;
    case '>':
      setMarkerType(TriangleRight);
      break;
    case '#':
      setMarkerType(Dot);
      break;
    case 'l':
      setMarkerType(LtiLogo);
      break;
    default:
      if (len==1) {
        setColor(style);
      } else if (len==2) {
        if (isdigit(style[1])) {
          setColor(style);
        } else {
          char color[2]="";
          strncpy(color,style,1);
          setColor(color);
          setStyle(&style[1]);
        }
      } else if (len==3) {
        if (isdigit(style[1])) {
          char color[3]="";
          strncpy(color,style,2);
          setColor(color);
          setStyle(&style[2]);
        } else {
          char color[2]="";
          strncpy(color,style,1);
          setColor(color);
          setStyle(&style[1]);
        }
      } else {
        char color[3]="";
        strncpy(color,style,2);
        setColor(color);
        setStyle(&style[2]);
      }
      return;
    }

    if (len==1) return; //done

    if (style[1]=='f') {
      filled=true;
      if (len>2) {
        setColor(&style[2]);
      }
    } else {
      filled=false;
      setColor(&style[1]);
    }

  }


template<class T>
  void drawBase<T>::marker(const pointList& c, int width, eMarkerType t,
                        const point& offset) {

    pointList::const_iterator it;
    pointList::const_iterator itEnd;

    it = c.begin();
    itEnd = c.end();

    if (offset==lti::point(0,0)) {
      while (it!=itEnd) {
        marker((*it),width,t);
        it++;
      }
    } else {
      while (it!=itEnd) {
        marker((*it).x+offset.x,(*it).y+offset.y,width,t);
        it++;
      }
    }
  }

template<class T>
  void drawBase<T>::marker(const pointList& c, int width, char* style,
                        const point& offset) {

    pointList::const_iterator it;
    pointList::const_iterator itEnd;

    it = c.begin();
    itEnd = c.end();

    if (offset==lti::point(0,0)) {
      while (it!=itEnd) {
        marker((*it),width,style);
        it++;
      }
    } else {
      while (it!=itEnd) {
        marker((*it).x+offset.x,(*it).y+offset.y,width,style);
        it++;
      }
    }
  }


template<class T>
  void drawBase<T>::set(const pointList& c, const point& offset) {

    pointList::const_iterator it;
    pointList::const_iterator itEnd;

    it = c.begin();
    itEnd = c.end();

    if (offset==lti::point(0,0)) {
      while (it!=itEnd) {
        set((*it));
        it++;
      }
    }
    else {
      while (it!=itEnd) {
        set((*it).x+offset.x,(*it).y+offset.y);
        it++;
      }
    }

  }


  template<class T>
  void drawBase<T>::set(const location& loc,const bool showAngleLine) {
    // assuming image coordinate system (positive angle clock-wise)
    
    float ctmp, stmp;
    sincos(loc.angle, stmp, ctmp);

    const float cosa = ctmp;
    const float sina = -stmp;

    // compute the four points
    point corn[4];
    const float r = loc.radius;

    corn[0].x  = static_cast<int>(0.5f + r*(cosa + sina) + loc.position.x);
    corn[0].y  = static_cast<int>(0.5f + r*(-sina + cosa) + loc.position.y);

    corn[1].x  = static_cast<int>(0.5f + r*(-cosa + sina) + loc.position.x);
    corn[1].y  = static_cast<int>(0.5f + r*(sina + cosa) + loc.position.y);

    corn[2].x  = static_cast<int>(0.5f + r*(-cosa - sina) + loc.position.x);
    corn[2].y  = static_cast<int>(0.5f + r*(sina - cosa) + loc.position.y);

    corn[3].x  = static_cast<int>(0.5f + r*(cosa - sina) + loc.position.x);
    corn[3].y  = static_cast<int>(0.5f + r*(-sina - cosa) + loc.position.y);


    // draw the box
    line(corn[0],corn[1]);
    lineTo(corn[2]);
    lineTo(corn[3]);
    lineTo(corn[0]);

    // draw the radius
    if (showAngleLine) {
      line(static_cast<int>(0.5f + loc.position.x),
           static_cast<int>(0.5f + loc.position.y),
           static_cast<int>(0.5f + r*cosa+loc.position.x),
           static_cast<int>(0.5f - r*sina+loc.position.y));
    }
  }


template<class T>
  void drawBase<T>::set(const rectLocation& loc,const bool showAngleLine) {
    // assuming image coordinate system (positive angle clock-wise)
    
    float ctmp, stmp;
    sincos(loc.angle, stmp, ctmp);

    const float cosa = ctmp;
    const float sina = -stmp;

    // compute the four points
    point corn[4];
    const float l = loc.minLength/2;
    const float m = loc.maxLength/2;

    corn[0].x  = static_cast<int>(0.5f + m*cosa + l*sina + loc.position.x);
    corn[0].y  = static_cast<int>(0.5f - m*sina + l*cosa + loc.position.y);

    corn[1].x  = static_cast<int>(0.5f - m*cosa + l*sina + loc.position.x);
    corn[1].y  = static_cast<int>(0.5f + m*sina + l*cosa + loc.position.y);

    corn[2].x  = static_cast<int>(0.5f - m*cosa - l*sina + loc.position.x);
    corn[2].y  = static_cast<int>(0.5f + m*sina - l*cosa + loc.position.y);

    corn[3].x  = static_cast<int>(0.5f + m*cosa - l*sina + loc.position.x);
    corn[3].y  = static_cast<int>(0.5f - m*sina - l*cosa + loc.position.y);

    // draw the box
    line(corn[0],corn[1]);
    lineTo(corn[2]);
    lineTo(corn[3]);
    lineTo(corn[0]);

    // draw the radius
    if (showAngleLine) {
      line(static_cast<int>(0.5f + loc.position.x),
           static_cast<int>(0.5f + loc.position.y),
           static_cast<int>(0.5f + m*cosa + loc.position.x),
           static_cast<int>(0.5f - m*sina + loc.position.y));
    }
  }



template<class T>
  void drawBase<T>::set(const std::list<location>& locs,
                    const bool showAngleLine) {
    std::list<location>::const_iterator it;
    for (it=locs.begin();it!=locs.end();++it) {
      set(*it,showAngleLine);
    }
  }

  template<class T>
  void drawBase<T>::polarLine(const point& p,
                              const float& length,
                              const float& angle) {

    float stmp, ctmp;
    sincos(angle, stmp, ctmp);
    int xdiff = iround( length * ctmp );
    int ydiff = iround( length * stmp );

    line(p.x,p.y,p.x+xdiff,p.y+ydiff);

  }



}  // namespace lti


