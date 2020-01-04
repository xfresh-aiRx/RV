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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiPolygonPoints_template.h
 * classes ....: lti::PolygonPoints
 * description.: class for description of an objects contour
 *               as a linear polygon
 * authors ....: Ruediger Weiler, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 15.11.2002
 * revisions ..: $Id: ltiPolygonPoints_template.h,v 1.12 2005/11/09 17:02:11 doerfler Exp $
 */

#include "ltiObject.h"
#include "ltiContour.h"
#include "ltiObjectsFromMask.h"
#include "ltiDraw.h"
#include "ltiImage.h"
#include "ltiSort.h"
#include "ltiGeometry.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

namespace lti {

  // -----------------------------------------------------------------
  // Polygon Points
  // -----------------------------------------------------------------
  template<class T>
  tpolygonPoints<T>::tpolygonPoints() : tpointList<T>() {
  }

  template<class T>
  tpolygonPoints<T>::tpolygonPoints(const tpolygonPoints<T>& other)
    : tpointList<T>() {
    copy(other);
  }

  template<class T>
  tpolygonPoints<T>::~tpolygonPoints() {
  }

  template<class T>
  tpolygonPoints<T>&
  tpolygonPoints<T>::castFrom(const borderPoints& theBorderPoints,
                              const int minLength,
                              const double maxDistance,
                              const bool closed,
                              const bool searchMaxDist) {

    return approximate(theBorderPoints,minLength,
                       maxDistance,closed,searchMaxDist);
  }

  template<class T>
  tpolygonPoints<T>& tpolygonPoints<T>::castFrom(const ioPoints& thePointList) {
    // filter the io points that represent single pixels
    tpointList<T> pts;
    ioPoints::const_iterator it1,it2;

    _lti_debug("Called castFrom(ioPoints)\n");

    it1 = it2 = thePointList.begin();

    if (it1 != thePointList.end()) {
      it2++;
      while (it2 != thePointList.end()) {
        if (*it1 != *it2) {
          // this does not make too much sense to me...
          //pts.push_back(tpoint<T>((*it1).x,(*it2).y));
          pts.push_back(tpoint<T>(static_cast<T>((*it1).x),
                                  static_cast<T>((*it1).y)));
        }
        ++it1;
        ++it2;
      }
      // at this point is it2==thePointList.end(), so this does not make
      // sense
      //pts.push_back(tpoint<T>((*it1).x,(*it2).y));
      // I guess it is supposed to be:
      pts.push_back(tpoint<T>(static_cast<T>((*it1).x),
                              static_cast<T>((*it1).y)));
    } else {
      return *this;
    }

    return convexHullFrom(pts);
  }

  struct pointWithAngle {
    double angle;
    point vector;
  };

  template<class T>
  tpolygonPoints<T>&
  tpolygonPoints<T>::castFrom(const tpointList<T>& thePointList) {
    return convexHullFrom(thePointList);
  }

  template<class T>
  tpolygonPoints<T>&
  tpolygonPoints<T>::convexHullFrom(const tpointList<T>& thePointList) {

    // the algorithm used is similar to the one explained in
    // M. de Berg, et. al. Computational Geometry. Algorithms and Applications,
    // 2nd. edition. Springer, 2000

    _lti_debug("Called castFrom(pointList)\n");
    this->clear();

    if (thePointList.empty()) {
      return *this;  // thePointList is empty!  We are ready!
    }

    vector< tpoint<T> > vPts;
    thePointList.castTo(vPts);
    sort< tpoint<T> > sorter(false); // sort in ascending order
    sorter.apply(vPts);              // sort the points

    if (vPts.size() < 3) {
      tpointList<T>::castFrom(vPts);
      return *this;
    }

    // now use the following ivector to store the indices of the points
    // in the vector
    ivector lidx;
    lidx.resize(vPts.size(),int(),false,false);
    lidx.at(0)=0;
    lidx.at(1)=1;
    int sptr = 2;
    int i;

    // first try the right side
    for (i=2;i<vPts.size();++i) {
      lidx.at(sptr)=i;
      sptr++;
      while ((sptr>2) && (clockwiseTurn(vPts.at(lidx.at(sptr-3)),
                                        vPts.at(lidx.at(sptr-2)),
                                        vPts.at(lidx.at(sptr-1))) <= 0))  {
        // on not a right turn, delete the middle point
        lidx.at(sptr-2) = lidx.at(sptr-1);
        sptr--;
      }
    }

    for (i=0;i<sptr;++i) {
      push_back(vPts.at(lidx.at(i)));
    }

    // and now the left side
    lidx.at(0)=vPts.lastIdx();
    lidx.at(1)=vPts.lastIdx()-1;
    sptr = 2;

    for (i=vPts.lastIdx()-2;i>=0;--i) {
      lidx.at(sptr)=i;
      sptr++;
      while ((sptr>2) && (clockwiseTurn(vPts.at(lidx.at(sptr-3)),
                                        vPts.at(lidx.at(sptr-2)),
                                        vPts.at(lidx.at(sptr-1))) <= 0))  {
        // on not a right turn, delete the middle point
        lidx.at(sptr-2) = lidx.at(sptr-1);
        sptr--;
      }
    }

    sptr--;
    for (i=1;i<sptr;++i) {
      push_back(vPts.at(lidx.at(i)));
    }

    return *this;
  }


  template<class T>
  void tpolygonPoints<T>::invert(){
    tpolygonPoints<T> tmp(*this);
    typename tpolygonPoints<T>::const_iterator it;
    this->clear();
    for (it=tmp.begin();it!=tmp.end();++it){
      push_front(*it);
    }
  }

  template<class T>
  tpolygonPoints<T>&
  tpolygonPoints<T>::clipPoly(const tpolygonPoints<T> &polygon1,
                              const tpolygonPoints<T> &polygon2,
                              const int& minLength,
                              const double& maxDistance){

    //this function draws both polygons in an channel8
    //and extracts the new polygon consisting of pP1 and pP2
    //if the poly1 and poly2 are not overlapping return poly1

    pointList pPs1;
    pPs1.castFrom(polygon1); //non const polygons are need
    pointList pPs2;
    pPs2.castFrom(polygon2);

    typename pointList::iterator itpp;
    typename pointList::const_iterator it_pp;

    rectangle rec1 = pPs1.computeBoundary();
    rectangle rec2 = pPs2.computeBoundary();
    //calculate the ofset of the two polygons
    int xMin = min(rec1.ul.x,rec2.ul.x);
    int xMax = max(rec1.br.x,rec2.br.x);
    int yMin = min(rec1.ul.y,rec2.ul.y);
    int yMax = max(rec1.br.y,rec2.br.y);
    //calculates the real size of the polygons without ofset
    int xSize = (xMax-xMin);
    int ySize = (yMax-yMin);
    //remove the ofset
    for (itpp=pPs1.begin();itpp!=pPs1.end();itpp++){
      (*itpp).x = (*itpp).x - xMin;
      (*itpp).y = (*itpp).y - yMin;
    }
    //remove the ofset
    for (itpp=pPs2.begin();itpp!=pPs2.end();itpp++){
      (*itpp).x = (*itpp).x - xMin;
      (*itpp).y = (*itpp).y - yMin;
    }

    std::list<borderPoints> objectList;
    std::list<borderPoints>::iterator it;

    objectsFromMask extractor;
    objectsFromMask::parameters extractorParams;
    extractorParams.level = -1;
    extractor.setParameters(extractorParams);

    draw <lti::channel8::value_type> drawer;
    channel8 channel;
    channel.resize(ySize+1,xSize+1,0,false,true);

    drawer.use(channel);
    drawer.setColor(255);
    //draw both polygons in one image
    drawer.set(*pPs1.begin());
    for (it_pp=pPs1.begin();it_pp!=pPs1.end();it_pp++){
      drawer.lineTo((*it_pp));
    }//close the polygon
    drawer.lineTo(*pPs1.begin());
    //draw the 2end polygon
    drawer.set(*pPs2.begin());
    for (it_pp=pPs2.begin();it_pp!=pPs2.end();it_pp++){
      drawer.lineTo((*it_pp));
    }//close the polygon
    drawer.lineTo(*pPs2.begin());

    extractor.apply(channel,objectList);
    if(objectList.size() == 1){
      it = objectList.begin();
      castFrom(*it,minLength,maxDistance);
      //add the offset to the borderPoints
      typename tpolygonPoints<T>::iterator it;
      for (it=this->begin();it!=this->end();it++) {
        (*it).x += xMin;
        (*it).y += yMin;
      }
    }
    else{
     copy(polygon1);
    }
    return *this;
  }


  // Segmentation of Arcs into Simple Segments
  // Ramer or Duda and Hart method
  template<class T>
  tpolygonPoints<T>&
  tpolygonPoints<T>::approximate(const borderPoints& theBorderPoints,
                                 const int minLength,
                                 const double maxDistance,
                                 const bool closed,
                                 const bool searchMaxDist) {

    this->clear();

    if (theBorderPoints.empty()) {
      return *this;
    }

    vector< point > pts;
    theBorderPoints.castTo(pts);

    int i,j;
    if (pts.size() < 4) {
      // three or less points are already the searched polygon
      for (i=0;i<pts.size();++i) {
        this->push_back(pts.at(i));
      }
      return *this;
    }

    if (maxDistance>=0.0) {

      // 1. find the two points with the maximal distance to each other.
      //    They belong to the polygon representation
      const int last = pts.lastIdx();
      const int maxDistLast = searchMaxDist ? last : 1;
      int maxDist2 = -1;
      int tmp;

      int maxIdx1 = 0;
      int maxIdx2 = 0;
      for (i=0;i<maxDistLast;++i) {
        const point& p = pts.at(i);
        for (j=i+1;j<=last;++j) {
          tmp = p.distanceSqr(pts.at(j));
          if ( tmp > maxDist2) {
            maxDist2 = tmp;
            maxIdx1 = i;
            maxIdx2 = j;
          }
        }
      }

      _lti_debug("Maximum length found between " << pts.at(maxIdx1) <<
                 " and " << pts.at(maxIdx2) << endl);
      
      // 2. reorder the list making a cyclic shift the original
      //    list in order to get two "adjacent" lists of points.
      vector < point > twolst;
      twolst.resize(pts.size()+1,point(),false,false);

      twolst.fill(pts,0,last,maxIdx1);
      twolst.fill(pts,pts.size()-maxIdx1);
      twolst.at(pts.size()) = twolst.at(0);

      // adapt the two indices to the new list
      maxIdx2 -= maxIdx1;
      maxIdx1 =  0;

      // 3. Begin with the "recursions"
      vector<ubyte> flags(twolst.size(),ubyte(0));
      // the first and second points of the maximal axis belong to the polygon
      flags.at(maxIdx1)=1;
      flags.at(maxIdx2)=1;
      if (closed) {
        // closed means, we want the last point of the polygon too
        flags.at(flags.size()-2)=1;
      }

      fitAndSplit(twolst,maxIdx1,maxIdx2,max(0.0,maxDistance),flags);
      fitAndSplit(twolst,maxIdx2,twolst.lastIdx(),max(0.0,maxDistance),flags);

      // now create the actual point list respecting the "original" sequence
      const int theMinLength = (minLength > 0) ? minLength : flags.size();
      i=0;
      j=0;
      while(i<flags.lastIdx()) { // the last elements was a dummy
        if ((flags.at(i) != 0) || (j>theMinLength)) {
          j=0;
          this->push_back(twolst.at(i));
        } else {
          ++j;
        }
        ++i;
      }
    } else {
      const int theMinLength = (minLength > 0) ? minLength : 1;
      i=0;
      while(i<pts.size()) {
        this->push_back(pts.at(i));
        i+=theMinLength;
      }
    }
    return *this;
  }

  // Segmentation of Arcs into Simple Segments
  // modified Ramer or Duda and Hart method
  template<class T>
  tpolygonPoints<T>&
  tpolygonPoints<T>::approximate(const borderPoints& theBorderPoints,
                                 const pointList& forcedVertices,
                                 const int minLength,
                                 const double maxDistance,
                                 const bool closed,
                                 const bool searchMaxDist) {

    this->clear();

    if (theBorderPoints.empty()) {
      return *this;
    }

    // are there any forced vertices?
    if (forcedVertices.empty()) {
      // no forced vertices -> normal approximate:
      return approximate(theBorderPoints,
                         minLength,maxDistance,closed,searchMaxDist);
    }

    // border points as vector to accelerate access.
    vector< point > pts;
    theBorderPoints.castTo(pts);

    int i,j;
    if (pts.size() < 4) {
      // three or less points are already the searched polygon,
      // and if there are forced vertices, they MUST be in the three point
      // set.
      for (i=0;i<pts.size();++i) {
        this->push_back(pts.at(i));
      }
      return *this;
    }

    // 1. find the two points with the maximal distance to each other.
    //    They belong to the polygon representation
    const int last = pts.lastIdx();
    vector< ubyte > tmpflags(pts.size()+1,ubyte(0));
    pointList::const_iterator fvit,efvit=forcedVertices.end();
    
    // localize which points must be vertices.
    for (i=0,fvit=forcedVertices.begin();i<=last;++i) {
      const point& p = pts.at(i);
      if ((fvit != efvit) && (p == (*fvit))) {
        tmpflags.at(i) = 1;
        ++fvit;
      }
    }
    
    while (fvit != efvit) {        
      // hmm, the user ignored the warning, and the points in forcedVertices
      // seem to be in a different order than in theBorderPoints. 
      // We need to search for the points in another sorted order and the
      // user has to wait!  (but he was warned!)
      bool notBorderPointFlag = true;
      for (i=0;i<last;++i) {
        const point& p = pts.at(i);
        if ((fvit != efvit) && (p == (*fvit))) {
          tmpflags.at(i) = 1;
          ++fvit;
          notBorderPointFlag = false;
        }
      }
      if (notBorderPointFlag) {
        // hmm, even worst!  One of the forced vertices is not even a point
        // in the boundary, so, let's just ignore this point.
        ++fvit;
      }
    }
    
    // try to approximate the polygon or should only the minStep be used?
    if (maxDistance>=0.0) {

      // search only if necessary
      const int maxDistLast = searchMaxDist ? last : 1;

      int maxDist2 = -1;
      int tmp;

      int maxIdx1 = 0;
      int maxIdx2 = 0;

      
      for (i=0;i<maxDistLast;++i) {
        const point& p = pts.at(i);

        for (j=i+1;j<=last;++j) {
          tmp = p.distanceSqr(pts.at(j));
          if ( tmp > maxDist2 ) {
            maxDist2 = tmp;
            maxIdx1 = i;
            maxIdx2 = j;
          }
        }
      }

      _lti_debug("Maximum length found between " << pts.at(maxIdx1) <<
                 " and " << pts.at(maxIdx2) << endl);
        
      // 2. reorder the list making a cyclic shift the original
      //    list in order to get two "adjacent" lists of points.
      vector< ubyte > flags(pts.size()+1,ubyte(0));
      vector< point > twolst;
      twolst.resize(pts.size()+1,point(),false,false);

      twolst.fill(pts,0,last,maxIdx1);
      twolst.fill(pts,pts.size()-maxIdx1);
      twolst.at(pts.size()) = twolst.at(0);

      flags.fill(tmpflags,0,last,maxIdx1);
      flags.fill(tmpflags,pts.size()-maxIdx1);
      flags.at(pts.size()) = flags.at(0);

      // adapt the two indices to the new list
      maxIdx2 -= maxIdx1;
      maxIdx1 =  0;

      // 3. Begin with the "recursions"
      // the first and second points of the maximal axis belong to the polygon
      flags.at(maxIdx1)=1;
      flags.at(maxIdx2)=1;
      if (closed) {
        // closed means, we want the last point of the polygon too
        flags.at(flags.size()-2)=1;
      }

      // the first half 
      int k1,k2;
      k1=maxIdx1;
      k2=k1+1;
      // check if any of the forced vertices need to be split
      while(k2<=maxIdx2) {
        while((k2<maxIdx2) && (flags.at(k2) == 0)) {
          ++k2;
        };
        fitAndSplit(twolst,k1,k2,max(0.0,maxDistance),flags);
        k1=k2;
        k2++;
      }
      
      // the second half
      k1=maxIdx2;
      k2=k1+1;
      while(k2<=twolst.lastIdx()) {
        while((k2<twolst.lastIdx()) && (flags.at(k2) == 0)) {
          ++k2;
        };
        fitAndSplit(twolst,k1,k2,max(0.0,maxDistance),flags);
        k1=k2;
        k2++;
      }

      // now create the actual point list respecting the "original" sequence
      const int theMinLength = (minLength > 0) ? minLength : flags.size();
      i=0;
      j=0;
      while(i<flags.lastIdx()) { // the last elements was a dummy
        if ((flags.at(i) != 0) || (j>theMinLength)) {
          j=0;
          this->push_back(twolst.at(i));
        } else {
          ++j;
        }
        ++i;
      }
    } else {
      // minimum step and forced vertices:

      const int theMinLength = (minLength > 0) ? minLength : tmpflags.size();
      i=0;
      j=0;
      while(i<tmpflags.lastIdx()) { // the last element was a dummy
        if ((tmpflags.at(i) != 0) || (j>theMinLength)) {
          j=0;
          this->push_back(pts.at(i));
        } else {
          ++j;
        }
        ++i;
      }

    }
    return *this;
  }

  template<class T>
  void tpolygonPoints<T>::fitAndSplit(const vector< point >& vct,
                                      const int index1,
                                      const int index2,
                                      const double maxDistance,
                                      vector<ubyte>& flags) {

    // stack to avoid real recursion.  The x coordinate of each
    // point means "from" and the y coordinate "to"
    pointList stack;
    point interv(index1,index2);
    const int& idx1 = interv.x;  // reference to the x component of interv
    const int& idx2 = interv.y;  // reference to the y component of interv
    int i;
    int maxIdx;
    double maxDist;
    double dist;

    stack.push_back(interv);

    do {
      interv=stack.back();
      stack.pop_back();       // indirectly get the idx1 and idx2
                              // values from the stack

      _lti_debug("from " << idx1 << " to " << idx2 << endl)

      // precompute some values to accelerate calculations of distance
      // between the line segment defined by idx1 and idx2 and another point
      const tpoint<double> a(vct.at(idx1));
      const tpoint<double> b(vct.at(idx2));
      const tpoint<double> d = b-a;
      const double norm = sqrt(d.absSqr());
      const double w0 = (b.x*a.y-b.y*a.x)/norm;
      const tpoint<double> w(d.y/norm,-d.x/norm);

      // search for maximal distance to the line segment
      maxIdx=-1;
      maxDist=-1;
      for (i=idx1+1;i<idx2;++i) {
        dist = abs(w0 + w.x*vct.at(i).x + w.y*vct.at(i).y);
        if (dist>maxDist) {
          maxDist = dist;
          maxIdx = i;
        }
      }

      _lti_debug(" max distance " << maxDist << " at " << vct.at(maxIdx))

      if (maxDist > maxDistance) {
        _lti_debug(" -> recurse!")

        // two much distortion... recurse
        flags.at(maxIdx)++;
        if (idx2 != maxIdx) {
          stack.push_back(point(maxIdx,idx2));
        }
        if (idx1 != maxIdx) {
          stack.push_back(point(idx1,maxIdx));
        }
      }

      _lti_debug(endl)

    } while (!stack.empty());
  };

  template<class T>
  T tpolygonPoints<T>::areaX2() const {
    T acc = 0;
    if (this->size()<3) {
      return acc;
    }
    
    typename tpolygonPoints<T>::const_iterator it1,it2;
    for (it1=it2=this->begin(),++it2;it2!=this->end();++it2,++it1) {
      acc+=(((*it2).x-(*it1).x)*((*it2).y+(*it1).y));
    }
    it2=this->begin();
    acc+=(((*it2).x-(*it1).x)*((*it2).y+(*it1).y));

    return acc;
  }
  
  template<class T>
  bool tpolygonPoints<T>::clockwise() const {
    return(areaX2()>0);
  }

} // end of namespace lti::

#include "ltiUndebug.h"
