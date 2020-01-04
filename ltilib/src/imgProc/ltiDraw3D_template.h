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
 * file .......: ltiDraw3D_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.4.2002
 * revisions ..: $Id: ltiDraw3D_template.h,v 1.14 2005/10/28 15:56:41 doerfler Exp $
 */

#include <cstdio>
#include <cfloat>
#include <limits>
#include "ltiMath.h"
#include "ltiMatrixInversion.h"

#undef _LTI_DEBUG
// #define _LTI_DEBUG
#include "ltiDebug.h"

namespace lti {

  // ---------------- 3D Graphics --------------------------

  template<class T>
  draw3D<T>::draw3D() : draw<T>() {
    // parameter object with default values!
    parameters defaultValues;
    setParameters(defaultValues);

    deepCtrl = false;
  }

  template<class T>
  draw3D<T>::~draw3D() {
    this->img = 0;
  }

  /*
   * return the name of the this object
   */
  template<class T>
  const char* draw3D<T>::getTypeName() const {
    return "draw3D";
  }

  template<class T>
  void draw3D<T>::use(matrix<T>& img) {
    draw<T>::use(img);
    resetDeepCtrl(deepCtrl);

    center.x = int(precenter.x*img.columns());
    center.y = int(precenter.y*img.rows());
  }

  template<class T>
  bool draw3D<T>::setParameters(const parameters& opt) {
    param = opt;
    setCamera(opt.camPos,opt.elevation,opt.azimuth);
    setCenter(opt.center);
    setPerspective(opt.perspective);
    resetDeepCtrl(deepCtrl);
    return true;
  }

  template<class T>
  void draw3D<T>::setCenter(const double& x,const double& y) {
    precenter.x = x;
    precenter.y = y;

    if (notNull(this->img)) {
      center.x = static_cast<int>(0.5+this->img->columns()*x);
      center.y = static_cast<int>(0.5+this->img->rows()*y);
    }

    param.center.x = x;
    param.center.y = y;
  }

  template<class T>
  void draw3D<T>::setCenter(const dpoint& p) {
    setCenter(p.x,p.y);
  }

  template<class T>
  void draw3D<T>::setCamera(const double& x1,
                            const double& y1,
                            const double& z1,
                            const double& x2,
                            const double& y2,
                            const double& z2) {
    double dx,dy,dz;
    double dx2,dy2,dz2;
    double alpha,beta,cosalpha,cosbeta;

    cam.clear();

    dx = x1-x2;
    dy = y1-y2;
    dz = z1-z2;

    dx2 = dx*dx;
    dy2 = dy*dy;
    dz2 = dz*dz;

    if ((dx2+dy2) != 0) {
      cosalpha = dx/(sqrt(dx2+dy2));
      alpha = copysign(acos(cosalpha),dy);
    }
    else {
      cosalpha = 1.0;
      alpha = 0;
    }

    if ((dx2+dy2+dz2) != 0) {
      cosbeta = sqrt((dx2+dy2)/(dx2+dy2+dz2));
      beta  = copysign(acos(cosbeta),dz);
    }
    else {
      cosbeta = 1.0;
      beta  = 0;
    }


    setCamera(x2,y2,z2,(Pi/2.0) - beta,alpha);
  };

  template<class T>
  void draw3D<T>::setCamera(const double& x1,
                            const double& y1,
                            const double& z1,
                            const double& elevation,
                            const double& azimuth) {

    double alpha,beta,cosalpha,cosbeta,sinalpha,sinbeta;

    cam.clear();

    param.camPos.x = x1;
    param.camPos.y = y1;
    param.camPos.z = z1;

    alpha = param.azimuth = azimuth;
    param.elevation = elevation;
    beta = (Pi/2.0) - elevation;

    sincos(alpha, sinalpha, cosalpha);
    sincos(beta,  sinbeta,  cosbeta);

    cam.at(0,0) = cosalpha*cosbeta;
    cam.at(0,1) = sinalpha*cosbeta;
    cam.at(0,2) = sinbeta;
    cam.at(0,3) = 0; // -param.camPos.x;

    cam.at(1,0) = -sinalpha;
    cam.at(1,1) = cosalpha;
    cam.at(1,2) = 0;
    cam.at(1,3) = 0; // -param.camPos.y;

    cam.at(2,0) = -sinbeta*cosalpha;
    cam.at(2,1) = -sinalpha*sinbeta;
    cam.at(2,2) = cosbeta;
    cam.at(2,3) = 0; // -param.camPos.z;

    dpoint3D tmp(-x1,-y1,-z1),tmp2;
    tmp2 = cam*tmp;

    cam.at(0,3) = tmp2.x; // -param.camPos.x;
    cam.at(1,3) = tmp2.y; // -param.camPos.y;
    cam.at(2,3) = tmp2.z; // -param.camPos.z;

    camTarget = dpoint3D(-cam.at(0,0),
                         -cam.at(0,1),
                         -cam.at(0,2));

    camTarget.add(param.camPos);

    _lti_debug("camPos   : " << param.camPos << endl);
    _lti_debug("camTarget: " << camTarget    << endl);
  }


  template<class T>
  void draw3D<T>::setCamera(const dpoint3D& p1,
                            const dpoint3D& p2) {
    setCamera(p1.x,p1.y,p1.z,p2.x,p2.y,p2.z);
  }

  template<class T>
  void draw3D<T>::setCamera(const dpoint3D& p1,
                            const double& elevation,
                            const double& azimuth) {
    setCamera(p1.x,p1.y,p1.z,elevation,azimuth);
  }

  template<class T>
  dpoint draw3D<T>::getOriginPosition() const {
    return center;
  }

  template<class T>
  const typename draw3D<T>::hmatrix&
  draw3D<T>::getCameraMatrix() const {
    return cam;
  }


  template<class T>
  void draw3D<T>::setPerspective(const double& p) {
    param.perspective = p;
  }

  template<class T>
  void draw3D<T>::setZoom(const double& z) {
    param.zoom = z;
  }

  template<class T>
  void draw3D<T>::p3Dto2D(const double& x,const double& y,const double& z,
                          int &x2,int &y2,double &z2) const {
    dpoint3D p3(x,y,z);
    point p2;

    p3Dto2D(p3,p2,z2);
    x2 = p2.x;
    y2 = p2.y;
  }

  template<class T>
  void draw3D<T>::p3Dto2D(const dpoint3D& p3,point& p2,double &z2) const {
    dpoint3D p;

    p = cam*p3;
    z2   = param.zoom*p.x;
    double f = param.zoom/(1.0 - z2*param.perspective);
    p2.x = iround(center.x + f*p.y);
    p2.y = iround(center.y - f*p.z);
  }


  template<class T>
  void draw3D<T>::ellipsoid(const vector<double> &mean,
                            const matrix<double> &sigma,
                            const double &cst) {
    rayTraceEllipsoid<T> drawer(*this,
                                dpoint3D(mean[0],mean[1],mean[2]),
                                sigma,
                                cst);
    drawer.render();
  }
    
  template<class T> 
  void draw3D<T>::marker3D(const double& x, const double& y, const double& z) {
    drawSymbol(x,y,z);
    act.x = x; act.y = y; act.z = z;
  }

  template<class T> 
  void draw3D<T>::marker3D(const double& x,
                           const double& y,
                           const double& z,
                           const char* style) {
      // save old drawing settings
    typename draw3D<T>::eMarkerType oldSymbol=this->actSymbol;
    T oldColor=this->actColor;
    bool oldFilled=this->filled;
    int oldSize=this->actSize;

    this->setStyle(style);
    drawSymbol(x,y,z);

    // restore old drawing settings
    this->actSymbol=oldSymbol;
    this->actColor=oldColor;
    this->filled=oldFilled;
    this->actSize=oldSize;

    act.x = x; act.y = y; act.z = z;
  }

  template<class T>
  void draw3D<T>::set2D(const int& x,const int& y,const double& z) {
    if (deepCtrl) {
      if (this->inCanvas(x,y) && (deepImg.at(y,x)<=(float)z)) {
        this->set(x,y);
        deepImg.at(y,x)=(float)z;
      }
    }
    else
      this->set(x,y);
  }

  template<class T>
  void draw3D<T>::set3D(const double& x,const double& y,const double& z) {
    int x2,y2;
    double z2;
    p3Dto2D(x,y,z,x2,y2,z2);
    set2D(x2,y2,z2);
    act.x = x; act.y = y; act.z = z;
  }

  template<class T>
  void draw3D<T>::set3D(const dpoint3D& p) {
    set3D(p.x,p.y,p.z);
  }

  template<class T>
  void draw3D<T>::line3DTo(const double& x,const double& y,const double& z) {
    line3D(act.x,act.y,act.z,x,y,z);
    act.x = x; act.y = y; act.z = z;
  }

  template<class T>
  void draw3D<T>::line3DTo(const dpoint3D& p) {
    line3D(act,p);
    act = p;
  }

  template<class T>
  void draw3D<T>::line3D(const double& x, const double& y, const double& z,
                         const double& x2,const double& y2,const double& z2) {
    dpoint3D a(x,y,z);
    dpoint3D b(x2,y2,z2);
    line3D(a,b);
  }

  template<class T>
  void draw3D<T>::line3D(const dpoint3D& a,const dpoint3D& b) {
    double microstep,mx,my,stepz;
    int step;
    int xx,yy;
    double z1,z2,z;
    point p1,p2;

    p3Dto2D(a,p1,z1);
    p3Dto2D(b,p2,z2);

    act = b;

    if (!correctLine(p1.x,p1.y,z1,p2.x,p2.y,z2)) return;
    if (!correctLine(p2.x,p2.y,z2,p1.x,p1.y,z1)) return;

    if (abs(p2.x-p1.x) <= abs(p2.y-p1.y)) {
      if (p2.y==p1.y) {
        set2D(p1.x,p1.y,max(z1,z2));
      }
      else {
        step  = (p2.y-p1.y)/abs(p2.y-p1.y);
        z = z1;
        stepz = step*(z2-z1)/(p2.y-p1.y);
        mx = p1.x;
        microstep = step*((double)(p2.x-p1.x))/(double)(p2.y-p1.y);
        for (yy=p1.y;yy!=p2.y;yy+=step) {
          set2D(static_cast<int>(0.5+mx),yy,z);
          mx += microstep;
          z  += stepz;
        }
        set2D(static_cast<int>(0.5+mx),yy,z);
      }
    }
    else {
      step = (p2.x-p1.x)/abs(p2.x-p1.x);
      z = z1;
      stepz = step*(z2-z1)/(double)(p2.x-p1.x);
      my = p1.y;
      microstep = step*(double)(p2.y-p1.y)/(double)(p2.x-p1.x);
      for (xx=p1.x;xx!=p2.x;xx+=step) {
        set2D(xx,static_cast<int>(my+0.5),z);
        my += microstep;
        z  += stepz;
      }
      set2D(xx,static_cast<int>(my+0.5),z);
    }
  }

  template<class T>
  bool draw3D<T>::correctLine(int &x,int &y,double &z,
                              const int& x2, const int& y2, const double& z2) {
    int xb,yb;
    xb = x; yb = y;
    if (!draw<T>::correctLine(x,y,x2,y2)) return false;
    if ((x == xb) && (y == yb)) return true;
    if ((xb == x2) && (yb == y2)) {
      z = z2;
      return true;
    }
    z = z2 + (z - z2)*sqrt((double)((x2-x)*(x2-x)+(y2-y)*(y2-y))/
         (double)((x2-xb)*(x2-xb)+(y2-yb)*(y2-yb)));
    return true;
  }

  template<class T>
  void draw3D<T>::set3D(const triangle& t,const bool filled) {
    if (filled) {
      point pts[3];
      double zs[3];
      int indices[3];
      int i,j,y;

      // compute the 3D transformations of the triangle
      for (i=0;i<3;++i) {
        indices[i] = i;
        p3Dto2D(t[i],pts[i],zs[i]);
      }

      // sort the y coordinates of the points
      if (pts[1].y < pts[0].y) {
        indices[0] = 1;
        indices[1] = 0;
      }

      for (i=1;i>=0;--i) {
        if (pts[indices[i]].y <= pts[2].y) {
          break;
        }
        indices[i+1]=indices[i];
      }
      indices[i+1]=2;

      // compute the io points

      // how many io points are required?
      int firstY = max(0,pts[indices[0]].y);
      int lastY = min(this->img->rows(),pts[indices[2]].y);

      const int ioEntries = lastY-firstY+1;

      ivector in(ioEntries,this->img->columns());
      ivector out(ioEntries,-1);
      dvector zIn(ioEntries,0.0);
      dvector zOut(ioEntries,0.0);

      ioLine(pts[indices[0]],zs[indices[0]],
             pts[indices[1]],zs[indices[1]],
             in,out,zIn,zOut,firstY);

      ioLine(pts[indices[1]],zs[indices[1]],
             pts[indices[2]],zs[indices[2]],
             in,out,zIn,zOut,firstY);

      ioLine(pts[indices[0]],zs[indices[0]],
             pts[indices[2]],zs[indices[2]],
             in,out,zIn,zOut,firstY);

      // now draw the computed io points
      double z,stepz;
      for (i=0;i<in.size();++i) {
        const int firstx = in.at(i);
        const int lastx = out.at(i);
        z = zIn.at(i);
        stepz = (zOut.at(i)-z)/(lastx-firstx+1);

        y = i+firstY;
        for (j=firstx;j<lastx;++j,z+=stepz) {
          set2D(j,y,z);
        }
      }
    } else {
      line3D(t[0],t[1]);
      line3DTo(t[2]);
      line3DTo(t[0]);
    }
  }

  template<class T>
  void draw3D<T>::ioLine(const point& p1,const double& z1,
                         const point& p2,const double& z2,
                         ivector& in, ivector& out,
                         dvector& zIn, dvector& zOut,
                         const int firstY) const {


    if (p2.y == p1.y) {
      int y;
      y = p1.y - firstY;
      if ((y>=0) && (y<in.size())) {
        if (p1.x < p2.x) {
          if (p1.x < in.at(y)) {
            in.at(y) = p1.x;
            zIn.at(y) = z1;
          }

          if (p2.x > out.at(y)) {
            out.at(y) = p2.x;
            zOut.at(y) = z2;
          }
        } else {
          if (p2.x < in.at(y)) {
            in.at(y) = p2.x;
            zIn.at(y) = z2;
          }

          if (p1.x > out.at(y)) {
            out.at(y) = p1.x;
            zOut.at(y) = z1;
          }
        }
      }
    } else {
      const int steps = max(abs(p2.y-p1.y),abs(p2.x-p1.x))+1;

      dpoint3D step;
      step.x = double(p2.x-p1.x)/steps;
      step.y = double(p2.y-p1.y)/steps;
      step.z = (z2-z1)/steps;

      int x,y,i;
      double xx,yy,zz;

      for (i=0, xx=p1.x, yy=p1.y,zz=z1;
           i<steps;
           ++i,xx+=step.x,yy+=step.y,zz+=step.z) {

        y = static_cast<int>(round(yy))-firstY;
        if ((y>=0) && (y<in.size())) {
          x = static_cast<int>(round(xx));
          if ((x < in.at(y)) && (x>=0)) {
            in.at(y) = x;
            zIn.at(y) = zz;
          }

          if ((x > out.at(y)) && (x<this->img->columns())) {
            out.at(y) = x;
            zOut.at(y) = zz;
          }
        }
      }
    }
  }

  template<class T>
  void draw3D<T>::box(const dpoint3D& a,
                      const dpoint3D& b,
                      const bool filled) {
    dpoint3D mn;
    dpoint3D mx;

    // get the minimum and maximum points for the box
    if (a.x < b.x) {
      mn.x = a.x;
      mx.x = b.x;
    } else {
      mx.x = a.x;
      mn.x = b.x;
    }

    if (a.y < b.y) {
      mn.y = a.y;
      mx.y = b.y;
    } else {
      mx.y = a.y;
      mn.y = b.y;
    }

    if (a.z < b.z) {
      mn.z = a.z;
      mx.z = b.z;
    } else {
      mx.z = a.z;
      mn.z = b.z;
    }

    if (filled) {
      triangle t;

      // front face
      t[0] = mx;
      t[1] = dpoint3D(mx.x,mn.y,mx.z);
      t[2] = dpoint3D(mx.x,mx.y,mn.z);

      if (t.visible(cam)) {
        set3D(t);
        t[0] = t[2];
        t[2] = dpoint3D(mx.x,mn.y,mn.z);
        set3D(t);
      }

      // back face
      t[0] = mn;
      t[1] = dpoint3D(mn.x,mn.y,mx.z);
      t[2] = dpoint3D(mn.x,mx.y,mx.z);

      if (t.visible(cam)) {
        set3D(t);
        t[1] = t[2];
        t[2] = dpoint3D(mn.x,mx.y,mn.z);
        set3D(t);
      }

      // top face
      t[0] = mx;
      t[1] = dpoint3D(mn.x,mn.y,mx.z);
      t[2] = dpoint3D(mx.x,mn.y,mx.z);

      if (t.visible(cam)) {
        set3D(t);
        t[2] = t[1];
        t[1] = dpoint3D(mn.x,mx.y,mx.z);
        set3D(t);
      }

      // bottom face
      t[0] = mn;
      t[1] = dpoint3D(mx.x,mx.y,mn.z);
      t[2] = dpoint3D(mx.x,mn.y,mn.z);

      if (t.visible(cam)) {
        set3D(t);
        t[2] = t[1];
        t[1] = dpoint3D(mn.x,mx.y,mn.z);
        set3D(t);
      }

      // right face
      t[0] = mx;
      t[1] = dpoint3D(mx.x,mx.y,mn.z);
      t[2] = dpoint3D(mn.x,mx.y,mn.z);

      if (t.visible(cam)) {
        set3D(t);
        t[1] = t[2];
        t[2] = dpoint3D(mn.x,mx.y,mx.z);
        set3D(t);
      }

      // left face
      t[0] = mn;
      t[1] = dpoint3D(mx.x,mn.y,mn.z);
      t[2] = dpoint3D(mx.x,mn.y,mx.z);

      if (t.visible(cam)) {
        set3D(t);
        t[1] = t[2];
        t[2] = dpoint3D(mn.x,mn.y,mx.z);
        set3D(t);
      }

    } else {
      // wireframe box
      // front
      line3D(mx.x,mx.y,mx.z,
             mx.x,mx.y,mn.z);
      line3DTo(mx.x,mn.y,mn.z);
      line3DTo(mx.x,mn.y,mx.z);
      line3DTo(mx.x,mx.y,mx.z);

      // top
      line3DTo(mn.x,mx.y,mx.z);
      line3DTo(mn.x,mn.y,mx.z);
      line3DTo(mx.x,mn.y,mx.z);

      // back
      line3D(mn.x,mn.y,mx.z,
             mn.x,mn.y,mn.z);
      line3DTo(mn.x,mx.y,mn.z);
      line3DTo(mn.x,mx.y,mx.z);

      // bottom right
      line3D(mx.x,mx.y,mn.z,
             mn.x,mx.y,mn.z);

      // bottom left
      line3D(mx.x,mn.y,mn.z,
             mn.x,mn.y,mn.z);

    }


  }

  template<class T>
  void draw3D<T>::box(const dpoint3D& a,
                      const dpoint3D& b,
                      const T lineColor) {
    const T ac(this->actColor);

    box(a,b,true);
    setColor(lineColor);
    box(a,b,false);
    this->actColor = ac;
  }

  template<class T>
  void draw3D<T>::drawCell(const int x,const int y) {
    double xx,yy,zz;
    float px;
    triangle t;
    int m;

    yy = (mChannel->rows()-y-1)*mGridSize.y/mSampleRate.y;
    xx = x*mGridSize.x/mSampleRate.x;

    px = mChannel->at(y,x);
    zz = px*mHeight;

    const T hcolor(getGray(px));
    const T origColor(this->actColor);
    const point sbox(0,mGridSize.y);
    const point ebox(mGridSize.x,0);

    if (mDrawLines) {
      setColor(mLineColor);
      line3D(xx,yy,0,xx,yy,zz);
    }

    if (mHeightColor) {
      setColor(hcolor);
    } else {
      setColor(origColor);
    }

    if (mOnlyPoints) {
      // only show points

      set3D(xx,yy,zz);
    } else if (mUseBoxes) {
      // show rectangular boxes
      t[0] = dpoint3D(xx-sbox.x,yy+sbox.y,zz);
      t[1] = dpoint3D(xx-sbox.x,yy-ebox.y,zz);
      t[2] = dpoint3D(xx+ebox.x,yy-ebox.y,zz);
      set3D(t);

      t[1] = t[2];
      t[2] = dpoint3D(xx+ebox.x,yy+sbox.y,zz);
      set3D(t);

      if (mDrawContour) {
        setColor(mContourColor);
        line3D(xx-sbox.x,yy+sbox.y,zz,
               xx-sbox.x,yy-ebox.y,zz);
        line3DTo(xx+ebox.x,yy-ebox.y,zz);
        line3DTo(xx+ebox.x,yy+sbox.y,zz);
        line3DTo(xx-sbox.x,yy+sbox.y,zz);
      }

    } else {
      if (((y+mSampleRate.y) >= mChannel->rows())    ||
          ((x+mSampleRate.x) >= mChannel->columns()) ) {
      return;
    }

      // show a triangular mesh
      m = ((y/mSampleRate.y)+(x/mSampleRate.x)) % 2;
      if (m==0) {

        t[0] = dpoint3D(xx,yy,zz);
        t[1] = dpoint3D(xx,yy-mGridSize.y,
                        mChannel->at(y+mSampleRate.y,x)*mHeight);
        t[2] = dpoint3D(xx+mGridSize.x,yy-mGridSize.y,
                        mChannel->at(y+mSampleRate.y,x+mSampleRate.x)*mHeight);

        if (mHeightColor) {
          setColor(shadingColor(t.getNormal(),hcolor));
        } else {
          setColor(shadingColor(t.getNormal(),origColor));
        }

        set3D(t);

        if (mDrawContour) {
          setColor(mContourColor);
          set3D(t,false);
        }

        t[1] = t[2];
        t[2] = dpoint3D(xx+mGridSize.x,yy,
                        mChannel->at(y,x+mSampleRate.x)*mHeight);

        if (mHeightColor) {
          setColor(shadingColor(t.getNormal(),hcolor));
        } else {
          setColor(shadingColor(t.getNormal(),origColor));
        }

        set3D(t);

        if (mDrawContour) {
          setColor(mContourColor);
          set3D(t,false);
        }

      } else {

        t[0] = dpoint3D(xx,yy,zz);
        t[1] = dpoint3D(xx,yy-mGridSize.y,
                        mChannel->at(y+mSampleRate.y,x)*mHeight);
        t[2] = dpoint3D(xx+mGridSize.x,yy,
                        mChannel->at(y,x+mSampleRate.x)*mHeight);

        setColor(shadingColor(t.getNormal(),origColor));
        set3D(t);

        if (mDrawContour) {
          setColor(mContourColor);
          set3D(t,false);
        }

        t[0] = t[1];
        t[1] = dpoint3D(xx+mGridSize.x,yy-mGridSize.y,
                        mChannel->at(y+mSampleRate.y,x+mSampleRate.x)*mHeight);
        setColor(shadingColor(t.getNormal(),origColor));
        set3D(t);

        if (mDrawContour) {
          setColor(mContourColor);
          set3D(t,false);
        }
      }
    }

    this->actColor = origColor;

  }

  template<class T>
  void draw3D<T>::set3D(const channel& chnl,
                        const float& height,
                        const point& gridSize,
                        const point& sampleRate,
                        const bool onlyPoints,
                        const bool useBoxes,
                        const bool heightColor,
                        const bool drawLines,
                        const T& lineColor,
                        const bool drawContour,
                        const T& contourColor) {

    const T origColor(this->actColor);

    mChannel = &chnl;
    mHeight = height;
    mGridSize = gridSize;
    mSampleRate = sampleRate;
    mOnlyPoints = onlyPoints;
    mUseBoxes = useBoxes;
    mHeightColor = heightColor;
    mDrawLines = drawLines;
    mLineColor = lineColor;
    mDrawContour = drawContour;
    mContourColor = contourColor;

    int x,y;

    for (y=0;y<chnl.rows();y+=sampleRate.y) {
      for (x=0;x<chnl.columns();x+=sampleRate.x) {
        drawCell(x,y);
      }

      // the last x element
      drawCell(chnl.lastColumn(),y);
    }

    y=chnl.lastRow();
    for (x=0;x<chnl.columns();x+=sampleRate.x) {
      drawCell(x,y);
    }
    drawCell(chnl.lastColumn(),y);

    this->actColor = origColor;
  }


  template<class T>
  void draw3D<T>::axis(const double size) {
    line3D(-size,0,0,size,0,0);
    line3D(0,-size,0,0,size,0);
    line3D(0,0,-size,0,0,size);
  }

  template<class T>
  void draw3D<T>::axis(const double size,
                       const T& colorX,
                       const T& colorY,
                       const T& colorZ) {
    const T origColor(this->actColor);

    setColor(colorX);
    line3D(-size,0,0,size,0,0);
    setColor(colorY);
    line3D(0,-size,0,0,size,0);
    setColor(colorZ);
    line3D(0,0,-size,0,0,size);

    this->actColor = origColor;
  }

  template<class T>
  void draw3D<T>::grid(const double size,const int nbSteps) {
      double steps=size/double(nbSteps);
      double mi=-size/2.0;
      double ma=-mi;
      int i=0;
      
      for (i=0; i<=steps; i++) {
        line3D(mi,mi+i*steps,mi,ma,mi+i*steps,mi);
        line3D(mi,mi,mi+i*steps,ma,mi,mi+i*steps);
        line3D(mi+i*steps,mi,mi,mi+i*steps,ma,mi);
        line3D(mi,mi,mi+i*steps,mi,ma,mi+i*steps);
        line3D(mi+i*steps,mi,mi,mi+i*steps,mi,ma);
        line3D(mi,mi+i*steps,mi,mi,mi+i*steps,ma);
      }
  }


  template<class T>
  void draw3D<T>::resetDeepCtrl(const bool b) {
    deepImg.resize(0,0,0,false,false); // delete old z-Buffer
    deepCtrl = b;

    if (notNull(this->img)) {
      if (deepCtrl && (this->img->columns() > 0) && (this->img->rows() > 0)) {
        deepImg.resize(this->img->rows(),this->img->columns(),
                       -std::numeric_limits<float>::max(),false,true);
      }
    }
  }

  // general implementation of getGray
  template<class T>
  T draw3D<T>::getGray(const float& val) {
    return static_cast<T>(val);
  }

  // specialization for channel8
  template<>
  ubyte draw3D<ubyte>::getGray(const float& val) {
    return static_cast<ubyte>(val*255+0.5);
  }

  // specialization for channel
  template<>
  rgbPixel draw3D<rgbPixel>::getGray(const float& val) {
    ubyte grey = static_cast<ubyte>(val*255+0.5);
    return rgbPixel(grey,grey,grey);
  }

  // general implementation for shading
  template<class T>
  T draw3D<T>::shadingColor(const dpoint3D& nrm,
                            const T& color) const {

    // assuming a white light source at z-infinity
    double r = sqrt(nrm.dot(nrm));
    if (r == 0.0) {
      r = 1.0;
    } else {
      r = nrm.z/r;
    }

    return static_cast<T>(color*r);
  }

  // specialization for ubyte
  template<>
  ubyte draw3D<ubyte>::shadingColor(const dpoint3D& nrm,
                                    const ubyte& color) const {
    // assuming light source is at z-infinity
    double r = sqrt(nrm.dot(nrm));
    if (r == 0.0) {
      r = 1.0;
    } else {
      r = nrm.z/r;
    }

    return static_cast<ubyte>(0.5+double(color)*r);
  }

  // specialization for rgbPixel
  template<>
  rgbPixel draw3D<rgbPixel>::shadingColor(const dpoint3D& nrm,
                                          const rgbPixel& color) const {
    // assuming light source is at z-infinity
    double r = sqrt(nrm.dot(nrm));
    if (r == 0.0) {
      r = 1.0;
    } else {
      r = nrm.z/r;
    }

    return rgbPixel(static_cast<ubyte>(0.5+double(color.getRed())*r),
                    static_cast<ubyte>(0.5+double(color.getGreen())*r),
                    static_cast<ubyte>(0.5+double(color.getBlue())*r));
  }

  template<class T>
  void draw3D<T>::drawSymbol(double x, double y,double z) {
    int i,j;
    int d=static_cast<int>(this->actSize/2.0);
    if (this->actSymbol==drawBase<T>::Pixel) {
      set3D(x,y,z);
      return;
    }
    int x2,y2;
    double z2;
    p3Dto2D(x,y,z,x2,y2,z2);
    switch (this->actSymbol) {
      case drawBase<T>::Xmark: {
        for (i=-d; i<=d; i++) {
          set2D(x2+i,y2+i,z2);
          set2D(x2-i,y2+i,z2);
        }
        break;
      }
      case drawBase<T>::Star: {
        for (i=-d; i<=d; i++) {
          set2D(x2+i,y2,z2);
          set2D(x2+i,y2+i,z2);
          set2D(x2+i,y2-i,z2);
        }
        break;
      }
      case drawBase<T>::Plus:
        for (i=-d; i<=d; i++) {
          set2D(x2+i,y2,z2);
          set2D(x2,y2+i,z2);
        }
        break;
      case drawBase<T>::Dot:
      case drawBase<T>::Circle: {
        int tmp=0;
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            tmp=iround(this->actSize/2.*
                       sin(Pi*static_cast<double>((i+d))
                           /static_cast<double>((2*d))));
            set2D(x2-i,y2+tmp,z2);
            set2D(x2-i,y2-tmp,z2);
          }
        }
        else {
          for (i=-d; i<=d; i++) {
            tmp=iround(this->actSize/2.*
                       sin(Pi*static_cast<double>((i+d))
                           /static_cast<double>((2*d))));
            for (j=-tmp; j<=tmp; j++) {
              set2D(x2-i,y2+j,z2);
            }
          }
        }
      }
        break;
      case drawBase<T>::Square:
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            set2D(x2+i,y2+this->actSize,z2);
            set2D(x2+this->actSize,y2+i,z2);
            set2D(x2+i,y2-this->actSize,z2);
            set2D(x2-this->actSize,y2+i,z2);
          }
        }
        else {
          for (i=-d; i<=d; i++) {
            for (j=-d; j<=d; j++) {
              set2D(x2+i,y2+j,z2);
            }
          }
        }
        break;
      case drawBase<T>::Diamond:
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            set2D(x2+i,y2-abs(i)+d,z2);
            set2D(x2+i,y2+abs(i)-d,z2);
          }
        }
        else {
          for (i=-d; i<=d; i++) {
            for (j=-d+abs(i); j<=d-abs(i); j++) {
              set2D(x2+j,y2+i,z2);
            }
          }
        }
        break;
      case drawBase<T>::TriangleUp:
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            set2D(x2+i,y2+abs(i)-d/2,z2);
          }
        }
        else {
          for (i=-d; i<=0; i++) {
            for (j=-d+abs(i); j<=d-abs(i); j++) {
              set2D(x2+j,y2+i+d/2,z2);
            }
          }
        }
        break;
      case drawBase<T>::TriangleDown:
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            set2D(x2+i,y2-abs(i)+d/2,z2);
          }
        }
        else {
          for (i=0; i<=d; i++) {
            for (j=-d+abs(i); j<=d-abs(i); j++) {
              set2D(x2+j,y2+i-d/2,z2);
            }
          }
        }
        break;
      case drawBase<T>::TriangleRight:
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            set2D(x2-abs(i)+d/2,y2+i,z2);
          }
        }
        else {
          for (i=0; i<=d; i++) {
            for (j=-d+abs(i); j<=d-abs(i); j++) {
              set2D(x2+i-d/2,y2+j,z2);
            }
          }
        }
        break;
      case drawBase<T>::TriangleLeft:
        if (!this->filled) {
          for (i=-d; i<=d; i++) {
            set2D(x2+abs(i)-d/2,y2+i,z2);
          }
        }
        else {
          for (i=-d; i<=0; i++) {
            for (j=-d+abs(i); j<=d-abs(i); j++) {
              set2D(x2+i+d/2,y2+j,z2);
            }
          }
        }
        break;
      default:
        set2D(x2,y2,z2);
        break;

    }
  }

  // -----------------------------------------------------------------------
  // -----------------------------------------------------------------------
  // rayTraceObject
  // -----------------------------------------------------------------------
  // -----------------------------------------------------------------------

  /*
   * The constructor must receive the drawer object, where this
   * object will be draw.
   */
  template<class T>
  rayTraceObject<T>::rayTraceObject(draw3D<T>& drawObj)
    : object(),drawer(drawObj),param(drawObj.getParameters()),
      cam(drawer.getCameraMatrix()),center(drawer.getOriginPosition()) {
    buildInverseCamera();
  }

  /*
   * multiply ((a/norm) * (b/norm))*norm
   */
  template<class T>
  T rayTraceObject<T>::multiplyNormed(const T& a, const T& b) const {
    return a*b;
  }

  template<>
  rgbPixel rayTraceObject<rgbPixel>::multiplyNormed(const rgbPixel& a,
                                                    const rgbPixel& b) const {
    return
      rgbPixel(static_cast<ubyte>(float(a.getRed())*float(b.getRed())/255.0),
         static_cast<ubyte>(float(a.getGreen())*float(b.getGreen())/255.0),
         static_cast<ubyte>(float(a.getBlue())*float(b.getBlue())/255.0));
  }

  template<>
  ubyte rayTraceObject<ubyte>::multiplyNormed(const ubyte& a,
                                       const ubyte& b) const {
    return static_cast<ubyte>(int(a)*int(b)/255);
  }

  /*
   * add with clipping
   */
  template<class T>
  T rayTraceObject<T>::addSaturated(const T& a,const T& b) const {
    return a+b;
  }


  template<>
  rgbPixel rayTraceObject<rgbPixel>::addSaturated(const rgbPixel& x,
                                                  const rgbPixel& y) const {

    int r,g,b;

    if ( (r = (int(x.getRed()) + int(y.getRed()))) > 255) {
      r = 255;
    }

    if ( (g = (int(x.getGreen()) + int(y.getGreen()))) > 255) {
      g = 255;
    }

    if ( (b = (int(x.getBlue()) + int(y.getBlue()))) > 255) {
      b = 255;
    }
    return rgbPixel(r,g,b);
  }

  template<>
  ubyte rayTraceObject<ubyte>::addSaturated(const ubyte& x,
                                     const ubyte& y) const {
    int val = int(x) + int(y);
    if (val > 255) {
      val = 255;
    }
    return val;
  }

  /*
   * compute the vector in world coordinates parallel to the ray
   * going through the focal point and the point (x,y) in the image plane.
   *
   * @param x image coordinate x
   * @param y image coordinate y
   * @param imgp position of the image point (x,y) (on the image plane) in
   *             the world coordinate system
   * @param ray  direction of the ray going through the point (x,y) and
   *             the focal point.
   */
  template<class T>
  void rayTraceObject<T>::p2Dto3D(const int& x,
                               const int& y,
                               dpoint3D& imgp,
                               dpoint3D& ray) const {
    // world points
    dpoint3D q,p;

    // compute the coordinates of the given image point in the world
    // coordinate system
    q.x = 0;
    q.y = (x-center.x)/param.zoom;
    q.z = (center.y-y)/param.zoom;

    imgp = camInv*q;

    if (param.perspective > 0) {
      q.x = param.zoom/param.perspective;
      q.y = 0;
      q.z = 0;

      p = camInv*q; // this is the position of the focal point in the
                           // world coordinates
      ray.subtract(imgp,p);  // the ray goes through the image point and
                             // through the focal point.
    } else {
      q.x = -1;
      q.y = 0;
      q.z = 0;

      ray = camInv*q;  // until now ray lies one unit from the
                              // camera position.
      ray.subtract(param.camPos); // to get the right direction we
                                  // need to substract the camera position
    }


    ray.divide(sqrt(ray.absSqr()));
  }



  /*
   * compute the inverse of the camera transformation matrix
   * (only the rotation part is created.  The result is left
   * in the attribute member camInv
   */
  template<class T>
  void rayTraceObject<T>::buildInverseCamera() {
    // the inverse rotation matrix is the transposed rotation matrix

    camInv.at(0,0) = cam.at(0,0);
    camInv.at(1,1) = cam.at(1,1);
    camInv.at(2,2) = cam.at(2,2);

    camInv.at(0,1) = cam.at(1,0);
    camInv.at(1,0) = cam.at(0,1);

    camInv.at(0,2) = cam.at(2,0);
    camInv.at(2,0) = cam.at(0,2);

    camInv.at(1,2) = cam.at(2,1);
    camInv.at(2,1) = cam.at(1,2);

    camInv.at(0,3) = param.camPos.x;
    camInv.at(1,3) = param.camPos.y;
    camInv.at(2,3) = param.camPos.z;
  }

  template<class T>
  const char* rayTraceObject<T>::getTypeName() const {
    return "rayTraceObject";
  }

  // -----------------------------------------------------------------------
  // rayTraceObject
  // -----------------------------------------------------------------------

  /*
   * The constructor must receive the drawer object, where this
   * object will be draw.
   */
  template<class T>
  rayTraceEllipsoid<T>::rayTraceEllipsoid(draw3D<T>& drawObj)
    : rayTraceObject<T>(drawObj) {
    mean = dpoint3D(0,0,0);
    sigma.resize(3,3,0.0,true,false);
    sigma.setIdentity();
    mahDist = 1.0;
  }

  /*
   * The constructor must receive the drawer object, where this
   * object will be draw.
   *
   * @param theMean mean point of the ellipsoid
   * @param covar covariance matrix used in the definition of the
   *              ellipsoid.  This should be a 3x3 symmetric positive
   *              definite matrix.
   * @param theMahDist mahalanobis distance using the given covariance
   *                   matrix between the mean point and the surface of
   *                   the ellipsoid.
   */
  template<class T>
  rayTraceEllipsoid<T>::rayTraceEllipsoid(draw3D<T>& drawObj,
                                          const dpoint3D& theMean,
                                          const dmatrix& covar,
                                          const double& theMahDist)
    : rayTraceObject<T>(drawObj) {
    setEllipsoid(theMean,covar,theMahDist);
  }

  /*
   * set the parameters of the ellipsoid.
   *
   * @param theMean mean point of the ellipsoid
   * @param covar covariance matrix used in the definition of the
   *              ellipsoid. This should be a 3x3 symmetric positive
   *              definite matrix.
   * @param theMahDist mahalanobis distance using the given covariance
   *                   matrix between the mean point and the surface of
   *                   the ellipsoid.
   */
  template<class T>
  void rayTraceEllipsoid<T>::setEllipsoid(const dpoint3D& theMean,
                                          const dmatrix& covar,
                                          const double& theMahDist) {
    mean = theMean;
    sigma.resize(3,3,0.0,false,false);
    sigma.fill(covar);
    mahDist = theMahDist;
  }

  template<class T>
  const char* rayTraceEllipsoid<T>::getTypeName() const {
    return "rayTraceEllipsoid";
  }

  /*
   * Compute the value of the pixel at x,y
   */
  template<class T>
  bool rayTraceEllipsoid<T>::rayTrace(const int x,const int y,
                                      double& z2,T& val) {
    this->p2Dto3D(x, y, ip, ray);
    if (intersectRayEllipsoid(ip,ray,mean,sigma,mahDist,lambda,sp)) {
      calcNormal(sp,mean,invMat,normal);

      lDir.subtract(this->param.lightPosition,sp);
      lDir.divide(sqrt(lDir.absSqr()));
      cosL = lDir.dot(normal);

      if (cosL < 0) {
        // opposite side from illumination... consider ambient light only
        cosL = 0;
      }
      val=addSaturated(multiplyNormed(this->param.ambientColor,storedColor),
                       static_cast<T>(multiplyNormed(this->param.diffuseColor,
                                                     storedColor)*cosL));

      z2 = (this->cam.at(0,0)*sp.x+
            this->cam.at(0,1)*sp.y+
            this->cam.at(0,2)*sp.z+
            this->cam.at(0,3));

      return true;
    }
    return false;
  }

  /*
   * render the whole object
   */
  template<class T>
  bool rayTraceEllipsoid<T>::render() {
    double p,z2;
    const point pmax(this->drawer.getCanvas().size());
    rectangle bbox;
    point pcenter;
    bool doy,dox;
    bool res = false;
    int x,y,i;
    T finalColor;
    storedColor = this->drawer.getColor();

    // invert covariance matrix to compute the normal vector
    matrixInversion<double> matInv;
    invMat.copy(sigma);
    matInv.apply(invMat);

    // compute the boundaries to be scanned...
    this->drawer.p3Dto2D(mean,pcenter,p);
    bbox.ul = pcenter;
    bbox.br = pcenter;

    // which quadrants need to be considered?
    bool q[5]; // (q[0] ignored... q indexed with traditional quadrant numbers
               // from 1 to 4
    for (i=0;i<5;++i) {
      q[i] = true; // default, do all quadrants
    }

    if (pcenter.x < 0) {
      q[2] = q[3] = false;
    } else if (pcenter.x >= pmax.x) {
      q[1] = q[4] = false;
    }

    if (pcenter.y < 0) {
      q[1] = q[2] = false;
    } else if (pcenter.y >= pmax.y) {
      q[3] = q[4] = false;
    }

    // ray tracing

    // first quadrant
    for (y=min(pmax.y-1,pcenter.y),doy=true;doy && (y>=0);--y) {
      res = false;
      for (x=max(0,pcenter.x),dox=true;dox && (x<pmax.x);++x) {
        if (rayTrace(x,y,z2,finalColor)) {
          this->drawer.setColor(finalColor);
          this->drawer.set2D(x,y,z2);
          res = true;
        } else if (res) {
          dox = false;
          bbox.br.x = max(bbox.br.x,x);
        }
      }
      doy = res;
    }
    bbox.ul.y = min(bbox.ul.y,y);

    // second quadrant
    for (y=min(pmax.y-1,pcenter.y),doy=true;doy && (y>=0);--y) {
      res = false;
      for (x=min(pmax.x-1,pcenter.x),dox=true;dox && (x>=0);--x) {
        if (rayTrace(x,y,z2,finalColor)) {
          this->drawer.setColor(finalColor);
          this->drawer.set2D(x,y,z2);
          res = true;
        } else if (res) {
          dox = false;
          bbox.ul.x = min(bbox.ul.x,x);
        }
      }
      doy = res;
    }
    bbox.ul.y = min(bbox.ul.y,y);

    // fourth quadrant
    for (y=max(0,pcenter.y),doy=true;doy && (y<pmax.y);++y) {
      res = false;
      for (x=max(0,pcenter.x),dox=true;dox && (x<pmax.x);++x) {
        if (rayTrace(x,y,z2,finalColor)) {
          this->drawer.setColor(finalColor);
          this->drawer.set2D(x,y,z2);
          res = true;
        } else if (res) {
          dox = false;
          bbox.br.x = max(bbox.br.x,x);
        }
      }
      doy = res;
    }
    bbox.br.y = max(bbox.br.y,y);

    // third quadrant
    for (y=max(0,pcenter.y),doy=true;doy && (y<pmax.y);++y) {
      res = false;
      for (x=min(pmax.x-1,pcenter.x),dox=true;dox && (x>=0);--x) {
        if (rayTrace(x,y,z2,finalColor)) {
          this->drawer.setColor(finalColor);
          this->drawer.set2D(x,y,z2);
          res = true;
        } else if (res) {
          dox = false;
          bbox.ul.x = min(bbox.ul.x,x);
        }
      }
      doy = res;
    }
    bbox.br.y = max(bbox.br.y,y);

    this->drawer.setColor(storedColor);

#ifdef _LTI_DEBUG
    bbox.ensureConsistency();
    draw<T>(this->drawer).box(bbox,false);
#endif

    return true;
  }

  /*
   * Given a point of the image plane in world coordinates and the
   * direction of a ray passing through the focal point and the
   * image point, compute the intersection of the ray and the
   * ellipsoid formed using a constant mahalanobis distance from
   * the mean value to the surface of the ellipsoid under
   * consideration of the given covariance matrix.
   *
   * @param ip point of the image plane in world coordinates
   *               (see p2Dto3D())
   * @param ray point describing the direction of a ray passing through the
   *            image point and the focal point
   * @param mu  mean value of the ellipsoid
   * @param sigma covariance matrix
   * @param mah mahalanobis distance between the ellipsoid surface and
   *            the mean value mu
   * @param lambda imgPnt + lambda*ray is the point on the ellipsoid
   *        surface to be considered
   * @param sp point at the surface of the ellipsoid
   * @return true if point was found, or false if the image point
   * does not belong to the ellipsoid
   */
  template<class T> bool
  rayTraceEllipsoid<T>::intersectRayEllipsoid( const dpoint3D &ip,
                                               const dpoint3D &ray,
                                               const dpoint3D &mu,
                                               const matrix<double> &sigma,
                                               const double &mah,
                                               double &lambda,
                                               dpoint3D &sp) {
    // next code was automatically generated to solve the equation system
    // (ip+lambda*ray - mu)^T sigma^-1 (ip+lambda*ray - mu) - mah*zoom*zoom = 0
    // This is a quadratic equation, and the first step is to find
    // this equation represented as a1*lambda^2 + b1*lambda + c1 = 0

    double a1,b1,c1,lambda1,lambda2;
    const double zoom = this->param.zoom;

    t1 = ray.y;
    t2 = t1*t1;
    t3 = sigma[0][2];
    t4 = t3*t3;
    t6 = ray.z;
    t7 = t6*t6;
    t8 = sigma[0][1];
    t9 = t8*t8;
    t11 = ray.x;
    t12 = t11*t11;
    t13 = sigma[1][2];
    t14 = t13*t13;
    t16 = t11*t3;
    t17 = sigma[1][1];
    t18 = t17*t6;
    t21 = sigma[0][0];
    t22 = t1*t21;
    t23 = t13*t6;
    t26 = t1*t8;
    t27 = t3*t6;
    t30 = t11*t8;
    t31 = sigma[2][2];
    t32 = t31*t1;
    t35 = t13*t1;
    t46 = t2*t4+t7*t9+t12*t14+2.0*t16*t18+2.0*t22*t23-2.0*t26*t27+
      2.0*t30*t32-2.0*t16*t35-2.0*t30*t23-t2*t21*t31-t7*t21*t17-t12*t17*t31;
    t47 = t21*t17;
    t48 = t47*t31;
    t49 = t21*t14;
    t50 = t9*t31;
    t52 = t8*t3*t13;
    t54 = t4*t17;
    t56 = 1/(-t48+t49+t50-2.0*t52+t54);

    // first term
    a1 = t46*t56;

    t57 = ip.x;
    t58 = t57*t14;
    t61 = mu.x;
    t63 = t57*t3;
    t65 = mu.z;
    t66 = t13*t65;
    t69 = mu.y;
    t71 = t17*t65;
    t73 = ip.z;
    t74 = t73*t9;
    t76 = ip.y;
    t77 = t31*t76;
    t81 = t76*t4;
    t83 = t76*t8;
    t85 = t13*t73;
    t88 = t57*t17;
    t91 = t57*t8;
    t93 = t69*t8;
    t96 = t61*t3;
    t98 = -t58*t11+t11*t14*t61+t63*t35-t30*t66+t1*t4*t69+t16*t71-t74*t6-
      t30*t77+t6*t9*t65-t81*t1+t83*t27+t30*t85-t63*t18+t88*t31*t11-t91*t32-
      t93*t27+t91*t23-t96*t35;
    t99 = t61*t8;
    t102 = t3*t65;
    t104 = t31*t69;
    t106 = t3*t73;
    t108 = t76*t21;
    t110 = t69*t21;
    t114 = t73*t21;
    t117 = t13*t76;
    t119 = t13*t69;
    t125 = t31*t61;
    t127 = t17*t73;
    t130 = t99*t32+t96*t18-t26*t102-t22*t104+t26*t106+t108*t32+
      t110*t23-t6*t21*t71+t114*t18-t99*t23+t16*t117-t16*t119+t22*t66-
      t108*t23+t30*t104-t11*t17*t125-t16*t127-t22*t85;

    // second term
    b1 = -2.0*(t98+t130)*t56;

    t138 = t17*t31;
    t139 = t57*t57;
    t141 = t21*t31;
    t142 = t69*t69;
    t144 = t65*t65;
    t146 = t76*t76;
    t158 = -2.0*t81*t69-2.0*t74*t65-t138*t139-t141*t142-t47*t144-t141*t146
      -2.0*t58*t61+2.0*t83*t102-2.0*t108*t66+2.0*t108*t85+2.0*t93*t106;
    t161 = t73*t73;
    t168 = zoom*zoom;
    t169 = mah*t168;
    t179 = t61*t61;
    t183 = -2.0*t110*t85+t9*t161-2.0*t96*t127+2.0*t114*t71+t14*t139+
      t169*t48+2.0*t99*t85+2.0*t96*t117-2.0*t99*t66+2.0*t96*t71-
      t138*t179+2.0*t108*t104;
    t205 = -2.0*t83*t106+t4*t146+2.0*t99*t104-2.0*t96*t119+2.0*t110*t66-
      2.0*t93*t102-t47*t161-t169*t50+2.0*t63*t119+2.0*t169*t52-2.0*t63*t117-
      t169*t49;
    t226 = -2.0*t91*t104+2.0*t88*t125+2.0*t91*t77-t169*t54-2.0*t91*t85+
      2.0*t91*t66+t9*t144+t4*t142+2.0*t63*t127+t14*t179-2.0*t99*t77-
      2.0*t63*t71;

    // third term
    c1 = (t158+t183+t205+t226)*t56;

    // compute the determinant:
    double delta = b1*b1-4*a1*c1;
    if (delta<0)
      return false;

    double sqrtdelta = sqrt(delta);
    lambda1 = (-b1 + sqrtdelta) / (2*a1);
    lambda2 = (-b1 - sqrtdelta) / (2*a1);

    // lambda is usually the nearest point, but we maybe are inside
    // the ellipsoid, or the ellipsoid is behind the camera:
    if (lambda1 < lambda2) {
      lambda = lambda1;
      if (lambda < 0) {
        lambda = lambda2;
      }
    } else {
      lambda = lambda2;
      if (lambda < 0) {
        lambda = lambda1;
      }
    }

    if (lambda < 0) {
      // ellipsoid behind the camera
      return false;
    }

    dpoint3D tmp = ray*lambda;
    sp.add(ip,tmp);

    return true;
  }


  /*
   * compute the normal vector of an ellipsoid surface at a specific point
   */
  template<class T>
  void rayTraceEllipsoid<T>::calcNormal(const dpoint3D &intersectionPoint,
                                        const dpoint3D &mu,
                                        const matrix<double> &sigma,
                                        dpoint3D &normal) const {

    dpoint3D d(intersectionPoint);
    d.subtract(mu);

    normal.x = d.x * sigma.at(0,0) + d.y * sigma.at(1,0) + d.z * sigma.at(2,0);
    normal.y = d.x * sigma.at(0,1) + d.y * sigma.at(1,1) + d.z * sigma.at(2,1);
    normal.z = d.x * sigma.at(0,2) + d.y * sigma.at(1,2) + d.z * sigma.at(2,2);

    double len1 = sqrt(normal.absSqr());
    normal /= len1;
  }

} // namespace lti

#include "ltiUndebug.h"


