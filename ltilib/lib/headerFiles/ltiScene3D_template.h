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
 * file .......: ltiScene3D.cpp
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 14.1.2003
 * revisions ..: $Id: ltiScene3D_template.h,v 1.5 2004/05/03 12:18:39 alvarado Exp $
 */

#include "ltiScene3D.h"

namespace lti {

  // --------------------------------------------------
  // scene3D
  // --------------------------------------------------

  // default constructor
  template<class T>
  scene3D<T>::scene3D()
    : draw3D<T>(){
      ranges = new dmatrix(3,2);
  }


  // copy constructor
  template<class T>
  scene3D<T>::scene3D(const scene3D& other) : draw3D<T>() {
    copy(other);
  }

  // destructor
  template<class T>
  scene3D<T>::~scene3D() {
  }

  // returns the name of this type
  template<class T>
  const char* scene3D<T>::getTypeName() const {
    return "scene3D";
  }

  // copy member
  template<class T>
  scene3D<T>& scene3D<T>::copy(const scene3D& other) {
    draw3D<T>::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class T>
  scene3D<T>& scene3D<T>::operator=(const scene3D& other) {
    return (copy(other));
  }


  // clone member
  //  template<class T>
  //    functor* scene3D<T>::clone() const {
  //      return new scene3D(*this);
  //    }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  template<class T>
  bool scene3D<T>::flush() {
    bool ok=true;

    ok=flush((*this->img));
    return ok;
  };

  template<class T>
  bool scene3D<T>::flush(const typename draw3D<T>::parameters& camera) {
    bool ok=true;
    draw3D<T>::setCamera(camera.camPos,camera.elevation,camera.azimuth);
    ok=flush((*this->img));
    return ok;
  }

  template<class T>
  bool scene3D<T>::flush(const typename draw3D<T>::parameters& camera,
                         matrix<T>& image) {
    bool ok=true;
    draw3D<T>::setCamera(camera.camPos,camera.elevation,camera.azimuth);
    ok=flush(image);
    return ok;
  }

  template<class T>
  bool scene3D<T>::flush(matrix<T>& image) {
    bool ok=true;

    draw3D<T>::use(image);
  
    typename std::list<Instructions>::const_iterator instIt;
    typename std::list<double>::const_iterator dIt=doubles.begin();
    typename std::list<int>::const_iterator iIt=ints.begin();
    typename std::list<T>::const_iterator cIt=colors.begin();
    typename std::list<bool>::const_iterator bIt=bools.begin();
    typename std::list<dvector>::const_iterator dvIt=dvectors.begin();
    typename std::list<dmatrix>::const_iterator dmIt=dmatrices.begin();
    typename std::list<channel>::const_iterator chIt=channels.begin();
    typename std::list<std::string>::const_iterator stylesIt=styles.begin();

    for (instIt=instructionList.begin(); instIt!=instructionList.end();
         instIt++) {
      switch ( (*instIt) ) {
        case setCol: {
          T x=(*cIt); cIt++;
          draw3D<T>::setColor(x);
          break;
        }
        case setSty: {         
          draw3D<T>::setStyle(stylesIt->c_str());
          stylesIt++;
          break;
        }
        case text_s_p: {
          double x=(*dIt); dIt++;
          double y=(*dIt); dIt++;
          double z=(*dIt); dIt++;
          int x1,y1;
          this->p3Dto2D(x,y,z,x1,y1,z);
          std::string s=(*stylesIt); stylesIt++;
          draw3D<T>::text(s,x1,y1);
          break;
        }
        case number_i_dp: {
            int n=(*iIt); iIt++;
            double x=(*dIt); dIt++;
            double y=(*dIt); dIt++;
            double z=(*dIt); dIt++;
            int x1,y1;
            this->p3Dto2D(x,y,z,x1,y1,z);
            draw3D<T>::number(n,x1,y1);
            break;
        }
        case set2D_i_i_d: {
          int x=(*iIt); iIt++;
          int y=(*iIt); iIt++;
          double z=(*dIt); dIt++;
          draw3D<T>::set2D(x,y,z);
          break;
        }
        case marker3D_d_d_d_c: {
          double x=(*dIt); dIt++;
          double y=(*dIt); dIt++;
          double z=(*dIt); dIt++; 
          draw3D<T>::setStyle(stylesIt->c_str());
          stylesIt++;
          draw3D<T>::marker3D(x,y,z);
          break;
        }
        case marker3D_d_d_d: {
          double x=(*dIt); dIt++;
          double y=(*dIt); dIt++;
          double z=(*dIt); dIt++;
          draw3D<T>::marker3D(x,y,z);
          break;
        }
        case set3D_d: {
          double x=(*dIt); dIt++;
          double y=(*dIt); dIt++;
          double z=(*dIt); dIt++;
          draw3D<T>::set3D(x,y,z);
          break;
        }
        case line3DTo_d: {
          double x=(*dIt); dIt++;
          double y=(*dIt); dIt++;
          double z=(*dIt); dIt++;
          draw3D<T>::line3DTo(x,y,z);
          break;
        }
        case line3D_d: {
          double x1=(*dIt);  dIt++;
          double y1=(*dIt);  dIt++;
          double z1=(*dIt);  dIt++;
          double x2=(*dIt);  dIt++;
          double y2=(*dIt);  dIt++;
          double z2=(*dIt);  dIt++;
          draw3D<T>::line3D(x1,y1,z1,x2,y2,z2);
          break;
        }
        case box_dp_dp: {
          dpoint3D p1,p2;
          p1.x=(*dIt); dIt++;
          p1.y=(*dIt); dIt++;
          p1.z=(*dIt); dIt++;
          p2.x=(*dIt); dIt++;
          p2.y=(*dIt); dIt++;
          p2.z=(*dIt); dIt++;
          bool f=(*bIt); bIt++;
          draw3D<T>::box(p1,p2,f);
          break;
        }
        case box_dp_dp_T: {
          dpoint3D p1,p2;
          p1.x=(*dIt); dIt++;
          p1.y=(*dIt); dIt++;
          p1.z=(*dIt); dIt++;
          p2.x=(*dIt); dIt++;
          p2.y=(*dIt); dIt++;
          p2.z=(*dIt); dIt++;
          T c=(*cIt); cIt++;
          draw3D<T>::box(p1,p2,c);
          break;
        }
        case ellips: {
          dvector m=(*dvIt); dvIt++;
          dmatrix s=(*dmIt); dmIt++;
          double cst=(*dIt); dIt++;
          draw3D<T>::ellipsoid(m,s,cst);
          break;
        }
        case set3D_ch: {
          dpoint g,s;
          channel ch=(*chIt); chIt++;
          float h=float( (*dIt) ); dIt++;
          g.x=(*dIt); dIt++;
          g.y=(*dIt); dIt++;
          s.x=(*dIt); dIt++;
          s.y=(*dIt); dIt++;
          bool o=(*bIt); bIt++;
          bool u=(*bIt); bIt++;
          bool hC=(*bIt); bIt++;
          bool dL=(*bIt); bIt++;
          T lC(*cIt); cIt++;
          bool dC=(*bIt); bIt++;
          T cC(*cIt); cIt++;
          draw3D<T>::set3D(ch,h,g,s,o,u,hC,dL,lC,dC,cC);
          break;
        }
        case axis_d: {
          double s=(*dIt); dIt++;
          draw3D<T>::axis(s);
          break;
        }
        case axis_d_T_T_T: {
          double s=(*dIt); dIt++;
          T cx=(*cIt); cIt++;
          T cy=(*cIt); cIt++;
          T cz=(*cIt); cIt++;
          draw3D<T>::axis(s,cx,cy,cz);
          break;
        }
        case grid_d: {
            draw3D<T>::grid((*dIt),(*iIt)); dIt++; iIt++;
          break;
        }
        default:
          break;
      }
    }

    return ok;
  };

  template<class T>
  void scene3D<T>::reset() {
    instructionList.clear();
    doubles.clear();
    ints.clear();
    colors.clear();
    bools.clear();
    dvectors.clear();
    dmatrices.clear();
    channels.clear();
    styles.clear();
  }

  template<class T>
  void scene3D<T>::text(const std::string& str,const dpoint3D& p) {
    instructionList.push_back(text_s_p);
    styles.push_back(str);
    doubles.push_back(p.x);
    doubles.push_back(p.y);
    doubles.push_back(p.z);
  }

  template<class T>
  void scene3D<T>::number(const int& n,const dpoint3D& p) {
    instructionList.push_back(number_i_dp);
    ints.push_back(n);
    doubles.push_back(p.x);
    doubles.push_back(p.y);
    doubles.push_back(p.z);
  }

  template<class T>
  void scene3D<T>::setColor(const T& px) {
    instructionList.push_back(setCol);
    colors.push_back(px);
  }

  template<class T> 
  void scene3D<T>::setStyle(const char* style) {
    instructionList.push_back(setSty);
    std::string s(style);
    styles.push_back(s);
  }

  template<class T>
  void scene3D<T>::set2D(const int& x,const int& y,const double& z) {
    instructionList.push_back(set2D_i_i_d);
    ints.push_back(x);
    ints.push_back(y);
    doubles.push_back(z);
  }

  template<class T>
  void scene3D<T>::marker3D(const double& x,const double& y,const double& z,
                            char* marker) {
    instructionList.push_back(marker3D_d_d_d_c);
    doubles.push_back(x);
    doubles.push_back(y);
    doubles.push_back(z);
    styles.push_back(marker);
  }

  template<class T>
  void scene3D<T>::marker3D(const double& x,const double& y,const double& z) {
    instructionList.push_back(marker3D_d_d_d);
    doubles.push_back(x);
    doubles.push_back(y);
    doubles.push_back(z);
  }

  template<class T>
  void scene3D<T>::set3D(const dpoint3D& p) {
      set3D(p.x,p.y,p.z);
  }

  template<class T>
  void scene3D<T>::set3D(const double& x,const double& y,const double& z) {
    instructionList.push_back(set3D_d);
    doubles.push_back(x);
    doubles.push_back(y);
    doubles.push_back(z);
  }

  template<class T>
  void scene3D<T>::line3DTo(const dpoint3D& p) {
      line3DTo(p.x,p.y,p.z);
  }

  template<class T>
  void scene3D<T>::line3DTo(const double& x,const double& y,const double& z){
    instructionList.push_back(line3DTo_d);
    doubles.push_back(x);
    doubles.push_back(y);
    doubles.push_back(z);
  }

  template<class T>
  void scene3D<T>::line3D(const dpoint3D& a,const dpoint3D& b) {
    line3D(a.x,a.y,a.z,b.x,b.y,b.z);
  }

  template<class T>
  void scene3D<T>::line3D(const double& x, const double& y, const double& z,
                          const double& x2,const double& y2,const double& z2) {
    instructionList.push_back(line3D_d);
    doubles.push_back(x);
    doubles.push_back(y);
    doubles.push_back(z);
    doubles.push_back(x2);
    doubles.push_back(y2);
    doubles.push_back(z2);
  }

  template<class T>
  void scene3D<T>::box(const dpoint3D& a,
                       const dpoint3D& b, const bool filled) {
    instructionList.push_back(box_dp_dp);
    doubles.push_back(a.x);
    doubles.push_back(a.y);
    doubles.push_back(a.z);
    doubles.push_back(b.x);
    doubles.push_back(b.y);
    doubles.push_back(b.z);
    bools.push_back(filled);
  }

  template<class T>
  void scene3D<T>::box(const dpoint3D& a,
                       const dpoint3D& b,
                       const T lineColor) {
    instructionList.push_back(box_dp_dp_T);
    doubles.push_back(a.x);
    doubles.push_back(a.y);
    doubles.push_back(a.z);
    doubles.push_back(b.x);
    doubles.push_back(b.y);
    doubles.push_back(b.z);
    colors.push_back(lineColor);
  }

  template<class T>
  void scene3D<T>::ellipsoid(const vector<double> &mean,
                             const matrix<double> &sigma,
                             const double &cst) {
    instructionList.push_back(ellips);
    dvectors.push_back(mean);
    dmatrices.push_back(sigma);
    doubles.push_back(cst);
  }

  template<class T>
  void scene3D<T>::set3D(const channel& chnl,
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
    instructionList.push_back(set3D_ch);
    channels.push_back(chnl);
    doubles.push_back( double(height) );
    doubles.push_back(gridSize.x);
    doubles.push_back(gridSize.y);
    doubles.push_back(sampleRate.x);
    doubles.push_back(sampleRate.y);
    bools.push_back(onlyPoints);
    bools.push_back(useBoxes);
    bools.push_back(heightColor);
    bools.push_back(drawLines);
    colors.push_back(lineColor);
    bools.push_back(drawContour);
    colors.push_back(contourColor);
  }


  /**
   * draw axis with dimension "size"
   */
  template<class T>
  void scene3D<T>::axis(const double size) {
    instructionList.push_back(axis_d);
    doubles.push_back(size);
  }

  /**
   * draw axis with dimension "size" and using the given color for each
   * axis
   */
  template<class T>
  void scene3D<T>::axis(const double size,
                        const T& colorX,
                        const T& colorY,
                        const T& colorZ) {
    instructionList.push_back(axis_d_T_T_T);
    colors.push_back(colorX);
    colors.push_back(colorY);
    colors.push_back(colorZ);
  }

  template<class T>
  void scene3D<T>::grid(const double size,const int nbSteps) {
    instructionList.push_back(grid_d);
    doubles.push_back(size);
    ints.push_back(nbSteps);
  }

  template<class T>
  void scene3D<T>::setRange(const dmatrix& range) {
    (*ranges)=range;
  }

  template<class T> 
  dmatrix scene3D<T>::getRange() const {
    return (*ranges);
  }


}

