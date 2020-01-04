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
 * file .......: ltiDraw2DDistribution.cpp
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 16.9.2002
 * revisions ..: $Id: ltiDraw2DDistribution_template.h,v 1.19 2005/02/02 14:14:17 doerfler Exp $
 */

#include "ltiDraw2DDistribution.h"
#include "ltiCMYKColor.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {

  // --------------------------------------------------
  // draw2DDistribution::parameters
  // --------------------------------------------------

#ifndef _LTI_MSC_6
  template<class T>
  void draw2DDistribution<T>::parameters::initPalette() {
    // insert complemenatry color to background
    palette.push_back(getDefaultAxisColor());
    palette.push_back(static_cast<T>(0.33));
    palette.push_back(static_cast<T>(0.66));
  }
#endif

  template<>
  void draw2DDistribution<rgbPixel>::parameters::initPalette(){
    palette.push_back(rgbPixel(255,255,0));
    palette.push_back(rgbPixel(255,0,255));
    palette.push_back(rgbPixel(0,255,255));
    palette.push_back(rgbPixel(255,0,0));
    palette.push_back(rgbPixel(0,255,0));
    palette.push_back(rgbPixel(0,0,255));
    double luminanz=(background.getRed()+background.getGreen()+
                     background.getBlue())*.33;
    if (luminanz > 127)
      palette.push_back(rgbPixel(0,0,0));
    else
      palette.push_back(rgbPixel(255,255,255));
  }

  template<>
  void draw2DDistribution<cmykColor>::parameters::initPalette() {
    palette.push_back(cmykColor(1,0,0,0));
    palette.push_back(cmykColor(0,1,0,0));
    palette.push_back(cmykColor(0,0,1,0));
    palette.push_back(cmykColor(1,1,0,0));
    palette.push_back(cmykColor(1,0,1,0));
    palette.push_back(cmykColor(1,1,0,0));
    double luminanz=(background.getYellow()+background.getMagenta()+
                     background.getCyan())*.33;
    if (luminanz>0.5) {
      palette.push_back(cmykColor(0,0,0,0));
    } else {
      palette.push_back(cmykColor(0,0,0,1));
    }

  }


  // --------------------------------------------------
  // draw2DDistribution
  // --------------------------------------------------

  // default constructor
  template<class T>
  draw2DDistribution<T>::draw2DDistribution()
    : functor(){

    testDist = new dmatrix;
    testIds =  new ivector;
    trainDist = new dmatrix;
    trainIds = new ivector;
    styles = new std::map<int,char*>;
    colorMap = new std::map<int,T>;

    // create an instance of the parameters with the default values

    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  draw2DDistribution<T>::draw2DDistribution(const draw2DDistribution& other)
    : functor() {
    copy(other);
  }

  // destructor
  template<class T>
  draw2DDistribution<T>::~draw2DDistribution() {
  }

  // returns the name of this type
  template<class T>
  const char* draw2DDistribution<T>::getTypeName() const {
    return "draw2DDistribution";
  }

  // copy member
  template<class T>
  draw2DDistribution<T>&
  draw2DDistribution<T>::copy(const draw2DDistribution& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class T>
  draw2DDistribution<T>&
  draw2DDistribution<T>::operator=(const draw2DDistribution& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* draw2DDistribution<T>::clone() const {
    return new draw2DDistribution(*this);
  }

  // return parameters
  template<class T>
  const typename draw2DDistribution<T>::parameters&
  draw2DDistribution<T>::getParameters() const {
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


  template<class T>
  bool draw2DDistribution<T>::apply(drawBase<T>& drawer,const dmatrix& dist,
                                    const bool& hold,const char* style) const {
    if (dist.columns()!=2) {
      setStatusString("this functor is only able to show 2d distributions");
      return false;
    }
    bool ok=true;
    parameters param=getParameters();

    char* s = new char[5];
    strcpy(s,style);
    int i=-1;
    // find first free negativ id
    while ( (*styles)[i]!=NULL ) i--;
    (*styles)[i]=s;
    (*colorMap)[i]=T();
    // create id vector for distribution.
    ivector ids(dist.rows(),i);
    if (!hold) {
      testDist->clear();
      testIds->clear();
      trainDist->clear();
      trainIds->clear();
    }
    ok=apply(drawer,dist,ids,dmatrix(),ivector(),true);
    return ok;
  };

  template<class T>
  bool draw2DDistribution<T>::apply(drawBase<T>& drawer,const dmatrix& dist,
                                    const int first,const int second,
                                    const bool hold,const char* style) const {
    bool ok;
    dmatrix tmp(dist.rows(),2);
    tmp.setColumn(0,dist.getColumnCopy(first));
    tmp.setColumn(1,dist.getColumnCopy(second));
    ok=apply(drawer,tmp,hold,style);
    return ok;
  }

  template<class T>
  bool draw2DDistribution<T>::apply(drawBase<T>& drawer,const dvector& x,
                                    const dvector& y,const bool hold,
                                    const char* style) const {
    bool ok;
    if ( x.size()!=y.size() ) {
      setStatusString("both vectors must have the same size");
      return false;
    }
    dmatrix tmp(x.size(),2);
    tmp.setColumn(0,x);
    tmp.setColumn(1,y);
    ok=apply(drawer,tmp,hold,style);
    return ok;
  }

  /**
   * Draws the points in dist into an image. For each new ID a new style
   * is used.
   * @param dist dmatrix with the source data.
   * @param ids the ID's accoding to the points in dist
   * @param hold if true the old image is not deleted
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool draw2DDistribution<T>::apply(drawBase<T>& drawer, const dmatrix& dist,
                                    const ivector& ids,const bool& hold) const{
    bool ok=true;
    vector<char> darkness(3);
    darkness.at(0)='0';      darkness.at(1)='3';      darkness.at(2)='6';
    parameters param=getParameters();
    int nbUsedStyles=0;
    if (hold)
      nbUsedStyles=styles->size();
    else {
      testDist->clear();
      testIds->clear();
      trainDist->clear();
      trainIds->clear();
      styles->clear();

      colorMap->clear();
      nbUsedStyles=0;
    }
    T useColor;
    char* style;
    for (int i=0; i<ids.size(); i++) {
      if ( (*styles)[ids.at(i)] == NULL ) { // id not yet used
        style = new char[2];
        style[0]=param.marker[( nbUsedStyles / param.palette.size()
                                %param.marker.size() )];
        style[1]=0;
        useColor=param.palette[( nbUsedStyles%param.palette.size() )];

        (*styles)[ids.at(i)]=style;

        (*colorMap)[ids.at(i)]=useColor;

        nbUsedStyles++;
      }
    }
    // now all styles are assigned
    ok=apply(drawer,dist,ids,dmatrix(),ivector(),hold);

    return ok;
  }

  /**
   * Draws the points in dist1 and dist2 into an image. In ids1 and ids2
   * the ids according to the points in dist1 and dist2 are expected.
   * For each new ID a new style is used. If points in dist1 and dist2
   * have equal ids, the pionts will be drawn with the same symbol and color,
   * but the symbol will be filled.
   * @param dist1 dmatrix with the source data.
   * @param ids1 the ID's accoding to the points in dist
   * @param dist2 dmatrix with the source data.
   * @param ids2 the ID's accoding to the points in dist
   * @param hold if true the old image is not deleted
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool draw2DDistribution<T>::apply(drawBase<T>& drawer,const dmatrix& dist1,
                                    const ivector& ids1,const dmatrix& dist2,
                                    const ivector& ids2,const bool& hold
                                    ) const {
    bool ok=true;

    parameters param=getParameters();

    vector<char> darkness(3);
    darkness.at(0)='0';      darkness.at(1)='3';      darkness.at(2)='6';
    int nbUsedStyles,i;
    // is hold is true count the number of styles that are already used
    // because the calculation of new styles is based an the number of used
    // styles. If hold is false, delete all so far create styles.
    if (hold) {
      nbUsedStyles=styles->size();
    }
    else {
      testDist->clear();
      testIds->clear();
      trainDist->clear();
      trainIds->clear();
      styles->clear();

      colorMap->clear();
      nbUsedStyles=0;
    }
    char* style;
    T useColor;

    // create styles for the ids without style
    for (i=0; i<ids1.size(); i++) {
      if ( (*styles)[ids1.at(i)] == NULL ) { // id not yet used
        style = new char[2];
        style[0]=param.marker[( nbUsedStyles / param.palette.size()
                                %param.marker.size())];
        style[1]=0;
        useColor=param.palette[( nbUsedStyles%param.palette.size())];

        (*styles)[ids1.at(i)]=style;

        (*colorMap)[ids1.at(i)]=useColor;

        nbUsedStyles++;
      }
    }
    // if hold is true, resize the old distributions and add the new points
    // otherwise delete the old points and assign the new ones
    if (hold) {
      int oldRows1=testDist->rows();
      int oldRows2=trainDist->rows();
      testDist->resize(oldRows1+dist1.rows(),2);
      testDist->fill(dist1,oldRows1,0,testDist->rows()-1,1,0,0);
      testIds->resize(oldRows1+ids1.size());
      testIds->fill(ids1,oldRows1,testIds->size(),0);
      trainDist->resize(oldRows2+dist2.rows(),2);
      trainDist->fill(dist2,oldRows2,0,trainDist->rows()-1,1,0,0);
      trainIds->resize(oldRows2+ids2.size());
      trainIds->fill(ids2,oldRows2,trainIds->size(),0);
    } else {
      testDist->resize(dist1.rows(),2);
      *testDist=dist1;
      testIds->resize(ids1.size(),2);
      *testIds=ids1;
      trainDist->resize(dist2.rows(),2);
      *trainDist=dist2;
      trainIds->resize(ids2.size(),2);
      *trainIds=ids2;
    }

    // now set up an image and a drawer for this image
    boundsFunctor<double> bF;
    dvector minimum(2);
    dmatrix extrema(4,2);
    dvector range(2);
    // find the extrems of both distributions and compute the
    // maximum range over both distributions
    if (testDist->rows()>0) {
      if (trainDist->rows()>0) {
        // if both are filled, find the global extrema
        bF.boundsOfRows((*testDist),extrema.getRow(0),extrema.getRow(1));
        bF.boundsOfRows((*trainDist),extrema.getRow(2),extrema.getRow(3));
        minimum.at(0)=extrema.getColumnCopy(0).minimum();
        minimum.at(1)=extrema.getColumnCopy(1).minimum();
        range.at(0)=extrema.getColumnCopy(0).maximum()-minimum.at(0);
        range.at(1)=extrema.getColumnCopy(1).maximum()-minimum.at(1);
      } else {
        // if only the test distribution is filled, find their extrema
        bF.boundsOfRows((*testDist),extrema.getRow(0),extrema.getRow(1));
        minimum=extrema.getRow(0);
        range.at(0)=extrema.getRow(1).at(0)-extrema.getRow(0).at(0);
        range.at(1)=extrema.getRow(1).at(1)-extrema.getRow(0).at(1);
      }
    } else {
      // testDist is empty
      if (trainDist->rows()>0) {
        // if only trainDist is filled, find their extrema
        bF.boundsOfRows((*trainDist),extrema.getRow(0),extrema.getRow(1));
        minimum=extrema.getRow(0);
        range.at(0)=extrema.getRow(1).at(0)-extrema.getRow(0).at(0);
        range.at(1)=extrema.getRow(1).at(1)-extrema.getRow(0).at(1);
      }
    }

    point size=drawer.getCanvasSize();

    //draw background
    drawer.setColor(param.background);
    drawer.rectangle(point(0,0),size,true);

    if (param.autoscale==true) {
      param.scale.x=static_cast<float>(size.x*0.96/range.at(0));

      if (param.scaleEqual==false) {
        param.scale.y=static_cast<float>(size.y*0.96/range.at(1));
      } else {
        param.scale.y=param.scale.x;
      }
      param.offset.x=static_cast<float>(-param.scale.x*minimum.at(0)+
                                        size.x*0.02);
      param.offset.y=static_cast<float>(-param.scale.y*minimum.at(1)+
                                        size.y*0.02);
    }

    // according to the parameter a coordiante system is drawed
    if (param.coSystem) {
      drawer.setColor(param.axisColor);
      //drawer.setColor(White);
      rectangle r(iround(size.x*0.02),
                  iround(size.y*0.02),
                  iround(size.x*0.98),
                  iround(size.y*0.98));
      drawer.rectangle(r);
    }

    for (i=0; i<testDist->rows(); i++) {
      drawer.setColor( (*colorMap)[testIds->at(i)] );
      drawer.setStyle( (*styles)[testIds->at(i)] );

      drawer.marker(iround(testDist->at(i,0)*param.scale.x+param.offset.x),
                    iround(size.y-(testDist->at(i,1)*param.scale.y+param.offset.y)));

    }

    char s[3];
    // if there is a second distribution set up new style and draw these points
    // also
    for (i=0; i<trainDist->rows(); i++) {
      s[0]=(*styles)[trainIds->at(i)][0];
      s[1]='f';
      s[2]=0;
      drawer.setColor( (*colorMap)[trainIds->at(i)] );
      drawer.setStyle(s);
      drawer.marker(iround(trainDist->at(i,0)*param.scale.x+param.offset.x),
                    iround(size.y-(trainDist->at(i,1)*param.scale.x+param.offset.y)));

    }
    return ok;
  }


} // namespace lti

#include "ltiUndebug.h"

