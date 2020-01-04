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
 * file .......: ltiBoundingBox_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 9.10.2000
 * revisions ..: $Id: ltiBoundingBox_template.h,v 1.18 2005/10/28 15:56:41 doerfler Exp $
 */

#include "ltiContour.h"
#include "ltiPolygonPoints.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

#if defined(_LTI_DEBUG) && (_LTI_DEBUG > 2)
#include "ltiViewer.h"
#include "ltiDraw.h"
#include <cstdio>
#endif

namespace lti {

  // --------------------------------------------------
  // boundingBox
  // --------------------------------------------------

  // default constructor
  template<class T>
  boundingBox<T>::boundingBox()
    : segmentation(){
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  boundingBox<T>::boundingBox(const boundingBox<T>& other)
    : segmentation() {
    copy(other);
  }

  // destructor
  template<class T>
  boundingBox<T>::~boundingBox() {
  }

  // returns the name of this type
  template<class T>
  const char* boundingBox<T>::getTypeName() const {
    return "boundingBox";
  }

  // copy member
  template<class T>
  boundingBox<T>&
    boundingBox<T>::copy(const boundingBox<T>& other) {
    segmentation::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* boundingBox<T>::clone() const {
    return new boundingBox<T>(*this);
  }

  // return parameters
  template<class T>
  const typename boundingBox<T>::parameters&
  boundingBox<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type channel8!
  template<class T>
  bool boundingBox<T>::apply(matrix<T>& srcdest,
                             const areaPoints& obj) const {
    matrix<T> tmp;
    if (apply(srcdest,obj,tmp)) {
      tmp.detach(srcdest);
      return true;
    }

    srcdest.clear();
    return false;
  };

  template<class T>
  bool boundingBox<T>::apply(const matrix<T>& src,
                             const areaPoints& obj,
                             matrix<T>& dest,
                             rectLocation& loc) const {

    if (apply(src,obj,dest)) {
      return apply(obj,loc);
    }

    return false;

  }

  template<class T>
  bool boundingBox<T>::apply(const areaPoints& obj,
                             rectLocation& loc) const {
    // too many area points -> reduce the number of point using an
    // ioPoints object
    ioPoints pts;
    pts.castFrom(obj);

    return apply(pts,loc);
  }

  template<class T>
  bool boundingBox<T>::apply(const ioPoints& obj,
                             rectLocation& loc) const {
    // too many area points -> reduce the number of point using an
    // ioPoints object
    _lti_debug(obj.getTypeName() << " " << obj.size() << " points" << endl);

    polygonPoints convexHull;
    convexHull.castFrom(obj);
    return computeLocationFromCH(convexHull,loc);
  }


  template<class T>
  bool boundingBox<T>::apply(const pointList& obj,
                                   rectLocation& loc) const {

    // first, generate the convex hull of the points
    // in the object
    _lti_debug(obj.getTypeName() << " " << obj.size() << " points" << endl);

    polygonPoints convexHull;
    convexHull.castFrom(obj);
    return computeLocationFromCH(convexHull,loc);
  }

  template<class T>
  bool boundingBox<T>::computeLocationFromCH(const pointList& convexHull,
                                                   rectLocation& loc) const {

#ifdef _LTI_DEBUG 
#if _LTI_DEBUG > 2
    _lti_debug("Convex hull has " << convexHull.size() << " points" << endl);

    image canvas;
    draw<rgbPixel> drawer;
    static lti::viewer view("Debug");

    drawer.use(canvas);

    canvas.resize(convexHull.computeBoundary().br + point(1,1),
                  Black,false,true);

    // Blue: the object
    drawer.setColor(rgbPixel(0,127,255));
    drawer.set(obj);

    // Green: the convex hull
    drawer.setColor(rgbPixel(0,127,0));
    borderPoints bp;

    bp.castFrom(convexHull);
    drawer.set(bp);


    // White the vertices of the convex hull
    drawer.setColor(White);
    drawer.set(static_cast<pointList>(convexHull));

    view.show(canvas);
#endif
#endif

    if (convexHull.size() == 1) {
      loc.angle = 0;
      loc.maxLength = loc.minLength = 1;
      loc.position.castFrom( convexHull.front() );
      return true;
    }

    // now compute the principal components of the convex
    // hull.
    int n(0);
    tpoint<double> sum(0,0);
    double m00(0),m01(0),m11(0); // m10 == m01;
    pointList::const_iterator it,eit;

    for (it=convexHull.begin(),eit=convexHull.end();
         it!=eit;
         ++it) {
      m00 += (*it).x * (*it).x;
      m01 += (*it).x * (*it).y;
      m11 += (*it).y * (*it).y;

      sum.x += (*it).x;
      sum.y += (*it).y;

      _lti_debug("CX = " << (*it).x << ", CY = " << (*it).y << "\n");

      ++n;
    }

    double t1,t2,t3,t4,t5;
    double eval[2];
    tpoint<double> evec[2];

    // estimate the mean value and covariance matrix
    tpoint<double> mean(sum);
    mean.divide(n);

    _lti_debug("Mean point " << mean << endl);

    m00 = (m00 - n*(mean.x*mean.x))/(n-1);
    m01 = (m01 - n*(mean.x*mean.y))/(n-1);
    m11 = (m11 - n*(mean.y*mean.y))/(n-1);

    _lti_debug("Covariance matrix: ((" << m00 << "\t " << m01 << ")"  << endl);
    _lti_debug("                    (" << m01 << "\t " << m11 << "))" << endl);

    // the eigenvalues and eigenvectors of the covariance matrix
    // describe the principal components.
    t3 = (m11+m00);
    t2 = (m11-m00);
    t1 = sqrt(t2*t2 + 4*m01*m01);

    eval[0] = 0.5*static_cast<double>(t3+t1);
    eval[1] = 0.5*static_cast<double>(t3-t1);

    // the main axis is the one with the greatest eigenvalue:
    int mainAxis,sndAxis;

    if (fabs(eval[0]) > fabs(eval[1])) {
      mainAxis = 0;
      sndAxis = 1;
    } else {
      mainAxis = 1;
      sndAxis = 0;
    }

    if (m01 == 0) {
      // Why are these indices not 0 and 1?
      evec[mainAxis].x = 1;
      evec[mainAxis].y = 0;

      evec[sndAxis].x = 0;
      evec[sndAxis].y = 1;
    } else {
      t4 = -static_cast<double>((t2 - t1)/(2*m01));
      t5 = sqrt(1+t4*t4);
      evec[0].x = t4/t5;
      evec[0].y = 1.0f/t5;

      t4 = -static_cast<double>((t2 + t1)/(2*m01));
      t5 = sqrt(1+t4*t4);
      evec[1].x = t4/t5;
      evec[1].y = 1.0f/t5;
    }

    _lti_debug("Main axis: " << evec[mainAxis] <<
          " eigenvalue: " << eval[mainAxis] << endl);
    _lti_debug("2nd  axis: " << evec[sndAxis] <<
          " eigenvalue: " << eval[sndAxis] << endl);


    // the angle can now be computed from the main axis
    loc.angle = static_cast<float>(atan2(evec[mainAxis].y,evec[mainAxis].x));

    // now position and lengths of the location can be computed
    computeLocation(convexHull,loc);

    if (getParameters().optimalBox) {
      rectLocation optLoc(loc);
      const float origAngle = loc.angle;
      const float a45 = static_cast<float>(45.0*Pi/180.0);
      const float step = static_cast<float>(1.0*Pi/180.0);
      float area = static_cast<float>(loc.maxLength*loc.minLength);
      float minArea = area;
      bool pos(true),neg(true);
      for (float angle = 0;angle < a45;angle+=step) {
        if (pos) {
          loc.angle = angle+origAngle;
          computeLocation(convexHull,loc);
          area = static_cast<float>(loc.maxLength*loc.minLength);
          if (area<minArea) {
            minArea = area;
            optLoc.copy(loc);
          }
        }

        if (neg) {
          loc.angle = angle-origAngle;
          computeLocation(convexHull,loc);
          area = loc.maxLength*loc.minLength;
          if (area<minArea) {
            minArea = area;
            optLoc.copy(loc);
          }
        }
      }
      loc.copy(optLoc);
    }

    if (getParameters().useLengths) {
      if (loc.minLength > loc.maxLength) {
        // switch lengths
        float h=loc.minLength;
        loc.minLength=loc.maxLength;
        loc.maxLength=h;
        // now adjust angle
        if (loc.angle > 0) {
          loc.angle-=static_cast<float>(Pi/2.0);
        } else {
          loc.angle+=static_cast<float>(Pi/2.0);
        }
      }
    }

    return true;
  }


  /*
   * compute the location given the angle in the location.
   */
  template<class T>
  void boundingBox<T>::computeLocation(const pointList& pts,
                                       rectLocation& loc) const {


    tpoint<double> evec[2];
    polygonPoints::const_iterator it,eit;

    double sina, cosa;
    sincos(static_cast<double>(loc.angle), sina, cosa);
    evec[0] = tpoint<double>( cosa, sina);
    evec[1] = tpoint<double>(-sina, cosa);

    it=pts.begin();
    eit=pts.end();

    tpoint<double> pmin((*it).x,(*it).y),pmax((*it).x,(*it).y),tp;
    double t1;

    tp.x = (*it).x;
    tp.y = (*it).y;

    pmax.x = pmin.x = evec[0].dot(tp);
    pmax.y = pmin.y = evec[1].dot(tp);

    ++it;
    for (;it!=eit;++it) {
      tp.x = (*it).x;
      tp.y = (*it).y;

      t1 = evec[0].dot(tp);
      pmin.x = min(pmin.x,t1);
      pmax.x = max(pmax.x,t1);

      t1 = evec[1].dot(tp);
      pmin.y = min(pmin.y,t1);
      pmax.y = max(pmax.y,t1);
    }

    loc.maxLength = static_cast<float>(pmax.x - pmin.x);
    loc.minLength = static_cast<float>(pmax.y - pmin.y);

    // tp is the center in the new coordinate system
    tp.x = (pmax.x + pmin.x)/2.0f;
    tp.y = (pmax.y + pmin.y)/2.0f;

    // compute the position with the INVERSE of the matrix
    t1 = evec[0].y*evec[1].x - evec[0].x*evec[1].y;
    loc.position.x = static_cast<float>((tp.y*evec[0].y-
                                         tp.x*evec[1].y)/t1);
    loc.position.y = static_cast<float>((tp.x*evec[1].x-
                                         tp.y*evec[0].x)/t1);


  }


  template<class T>
  bool boundingBox<T>::apply(const matrix<T>& src,
                             const areaPoints& obj,
                             matrix<T>& dest) const {

    const parameters& param = getParameters();

    if (!param.justSuppressBackground) {
      point destSize,deltaTmp;

      calculatePositions(obj,destSize,deltaTmp);
      dest.resize(destSize,param.backgroundColor,false,true);

      const point delta(deltaTmp);

      // copy only the area points
      areaPoints::const_iterator it;
      point tmp;

      for (it=obj.begin();
           it != obj.end();
           it++) {
        tmp.add(delta,*it);
        dest.at(tmp) = src.at(*it);
      }
    } else {

      dest.resize(src.size(),param.backgroundColor,false,true);

      // copy only the area points
      areaPoints::const_iterator it;
      for (it=obj.begin();
           it != obj.end();
           it++) {
        dest.at(*it) = src.at(*it);
      }

    }

    return true;
  };

  template<class T>
    void boundingBox<T>::calculatePositions(const areaPoints& obj,
                                            point& destSize,
                                            point& delta) const {
    const parameters& param = getParameters();
    rectangle origPos(obj.computeBoundary());
    origPos.ensureConsistency();

    //cerr << "OP: " << origPos << endl;

    if (!param.centerOfGravity) {

      destSize = origPos.getDimensions();
      destSize.add(point(param.borderSize*2,param.borderSize*2));

      delta = point(param.borderSize,param.borderSize)-origPos.ul;
    } else {
      // calculate center of gravity
      areaPoints::const_iterator it;
      point cog(0,0);
      int n = 0;

      for (it=obj.begin();
           it != obj.end();
           it++) {
        cog.add(*it);
        n++;
      }

      cog.divide(n);

      // determine the longest side of the object
      point longest;

      longest.x = max(cog.x-origPos.ul.x,origPos.br.x-cog.x);
      longest.y = max(cog.y-origPos.ul.y,origPos.br.y-cog.y);

      longest.add(point(param.borderSize,param.borderSize));

      destSize = point(2*longest.x+1,2*longest.y+1);

      delta = longest - cog;
    }
  }

}

#include "ltiUndebug.h"
