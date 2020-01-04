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
 * file .......: ltiSOFM2DVisualizer.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 31.10.2002
 * revisions ..: $Id: ltiSOFM2DVisualizer.h,v 1.11 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_S_O_F_M_2D_VISUALIZER_H_
#define _LTI_S_O_F_M_2D_VISUALIZER_H_

#include "ltiDrawBase.h"
#include "ltiFunctor.h"
#include "ltiSOFM2D.h"
#include "ltiSammonsMapping.h"
#include "ltiL1Distance.h"
#include "ltiL2Distance.h"
#include "ltiBoundsFunctor.h"

namespace lti {
  /**
   *  This class provides some possibilites to visualize the
   *  given SOFM.
   *  If the drawer is an instance of ltiDraw<T>, you have to set
   *  the background color of the image that is used to a value
   *  that is different from the drawing colors in the colormap.
   */
  class SOFM2DVisualizer : public functor {
  public:
    /**
     * the parameters for the class SOFM2DVisualizer
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       *  a list of colors, that is used for visualization
       */
      vector<rgbPixel> colorMap;

      /**
       *  the size of the symbols in the visualization
       */
      int symbolSize;

      /**
       *  the parameters for the Sammons Mapper. You have to set this parameter
       *  only if you use sammonsMapping and want to use other parameters for
       *  the Sammons Mapper than the default.
       */
      sammonsMapping::parameters smP;


    protected:
      static const vector<rgbPixel> defaultColors;


    };

    /**
     * default constructor
     */
    SOFM2DVisualizer();

    /**
     * Construct a functor using the given parameters
     */
    SOFM2DVisualizer(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    SOFM2DVisualizer(const SOFM2DVisualizer& other);

    /**
     * destructor
     */
    virtual ~SOFM2DVisualizer();

    /**
     * returns the name of this type ("SOFM2DVisualizer")
     */
    virtual const char* getTypeName() const;

    /**
     * Draws the unified distane matrix of the given som.
     */
    template<typename T>
    bool uMatrix(drawBase<T>& drawer,const SOFM2D& som) const {
      // collect information from the som that is used for visualisation

      bool ok=true;
      int sizeX=som.xSize();
      int sizeY=som.ySize();
      dmatrix grid(som.getPrototypes());

      dmatrix dist(sizeY,sizeX,0.);
      l2Distance<double> dFunc;

      dmatrix region;
      dvector tmp;
      ivector idx(3);
      int i,j;

      //corners
      idx[0]=1;
      idx[1]=sizeX;
      idx[2]=sizeX+1;
      region.copy(grid,idx);
      dFunc.apply(region,grid.getRow(0),tmp);
      dist.at(0,0)=tmp.sumOfElements()/3.;

      idx[0]=sizeX-2;
      idx[1]=2*sizeX-1;
      idx[2]=2*sizeX-2;
      region.copy(grid,idx);
      dFunc.apply(region,grid.getRow(sizeX-1),tmp);
      dist.at(0,sizeX-1)=tmp.sumOfElements()/3.;

      idx[0]=sizeX*(sizeY-1)+1;
      idx[1]=sizeX*(sizeY-2);
      idx[2]=sizeX*(sizeY-2)+1;
      region.copy(grid,idx);
      dFunc.apply(region,grid.getRow(sizeX*(sizeY-1)),tmp);
      dist.at(sizeY-1,0)=tmp.sumOfElements()/3.;

      idx[0]=sizeX*sizeY-2;
      idx[1]=sizeX*(sizeY-1)-1;
      idx[2]=sizeX*(sizeY-1)-2;
      region.copy(grid,idx);
      dFunc.apply(region,grid.getRow(sizeX*sizeY-1),tmp);
      dist.at(sizeY-1,sizeX-1)=tmp.sumOfElements()/3.;

      //egdes
      idx.resize(5);
      for (i=1; i<sizeX-1; i++) {
        idx[0]=i-1;
        idx[1]=i+1;
        idx[2]=i+sizeX-1;
        idx[3]=i+sizeX;
        idx[4]=i+sizeX+1;
        region.copy(grid,idx);
        dFunc.apply(region,grid.getRow(i),tmp);
        dist.at(0,i)=tmp.sumOfElements()/5.;
      }
      for (i=1; i<sizeX-1; i++) {
        idx[0]=(sizeY-1)*sizeX+i-1;
        idx[1]=(sizeY-1)*sizeX+i+1;
        idx[2]=(sizeY-2)*sizeX+i-1;
        idx[3]=(sizeY-2)*sizeX+i;
        idx[4]=(sizeY-2)*sizeX+i+1;
        region.copy(grid,idx);
        dFunc.apply(region,grid.getRow((sizeY-1)*sizeX+i),tmp);
        dist.at(sizeY-1,i)=tmp.sumOfElements()/5.;
      }
      for (i=1; i<sizeY-1; i++) {
        idx[0]=(i-1)*sizeX;
        idx[1]=(i+1)*sizeX;
        idx[2]=(i-1)*sizeX+1;
        idx[3]=i*sizeX+1;
        idx[4]=(i+1)*sizeX+1;
        region.copy(grid,idx);
        dFunc.apply(region,grid.getRow(i*sizeX),tmp);
        dist.at(i,0)=tmp.sumOfElements()/5.;
      }
      for (i=1; i<sizeY-1; i++) {
        idx[0]=i*sizeX-1;
        idx[1]=(i+2)*sizeX-1;
        idx[2]=i*sizeX-2;
        idx[3]=(i+1)*sizeX-2;
        idx[4]=(i+2)*sizeX-2;
        region.copy(grid,idx);
        dFunc.apply(region,grid.getRow((i+1)*sizeX-1),tmp);
        dist.at(i,sizeX-1)=tmp.sumOfElements()/5.;
      }

      //rest
      idx.resize(8);
      for (i=1; i<sizeY-1; i++) {
        for (j=1; j<sizeX-1; j++) {
          idx[0]=(i-1)*sizeX+j-1;
          idx[1]=(i-1)*sizeX+j;
          idx[2]=(i-1)*sizeX+j+1;
          idx[3]=i*sizeX+j-1;
          idx[4]=i*sizeX+j+1;
          idx[5]=(i+1)*sizeX+j-1;
          idx[6]=(i+1)*sizeX+j;
          idx[7]=(i+1)*sizeX+j+1;
          region.copy(grid,idx);
          dFunc.apply(region,grid.getRow(i*sizeX+j),tmp);
          dist.at(i,j)=tmp.sumOfElements()/8.;
        }
      }

      double maxDist=dist.maximum();
      dist.divide(-1.*maxDist);
      dist.add(1.);

      int f=getParameters().symbolSize;;
      //    const point size=drawer.getCanvasSize();
      //      if (f*sizeX>point.y || f*sizeY>point.X) { // not for epsDraw because
      //                                                // canvas size is mutable
      //          ok=false;
      //          setStatusString("the image is to small");
      //      }
      //      channel img(f*sizeY,f*sizeX);
      //      lti::draw<float> drawer;
      //      drawer.use(img);

      for (i=0; i<sizeY; i++) {
        for (j=0; j<sizeX; j++) {
          drawer.setGray(dist.at(i,j));
          drawer.rectangle(j*f,i*f,(j+1)*f-1,(i+1)*f-1,true);
        }
      }

      return ok;
    };



    /**
     * draws distribution of data colored according to ids.
     * @param drawer the drawing object
     * @param som the som to draw
     * @param data the
     * @param ids the ids according to the data
     * @return true if apply successful or false otherwise.
     */
    template<typename T>
    bool drawClasses(drawBase<T>& drawer,const SOFM2D& som,
                     const dmatrix& data,const ivector& ids) const {
      bool ok=true;
      dmatrix grid(som.getPrototypes());
      int sizeX=som.xSize();
      int sizeY=som.ySize();
      parameters p=getParameters();


      std::map<int,int> realToIntern;
      int rows=grid.rows();
      int nextId=0;

      ivector r(rows,0); //red
      ivector g(rows,0); //green
      ivector b(rows,0); //blue
      ivector n(rows,0); //count hits

      int i,j,w,currId;
      classifier::outputVector iv;
      for (i=0; i<data.rows(); i++) {
        som.classify(data.getRow(i), iv);
        currId=ids.at(i);
        if (realToIntern.find(currId)==realToIntern.end()) {
          realToIntern[currId]=nextId++;
        }
        w=iv.getWinnerUnit();
        n[w]++;
        r[w]+=p.colorMap[realToIntern[currId]].getRed();
        g[w]+=p.colorMap[realToIntern[currId]].getGreen();
        b[w]+=p.colorMap[realToIntern[currId]].getBlue();
      }

      image img(sizeY,sizeX);
      int nMax=n.maximum();
      for (i=0; i<sizeY; i++) {
        for (j=0; j<sizeX; j++) {
          w=i*sizeX+j;
          img.at(i,j).setRed(ubyte(r[w]/nMax));
          img.at(i,j).setGreen(ubyte(g[w]/nMax));
          img.at(i,j).setBlue(ubyte(b[w]/nMax));
        }
      }
      int f=p.symbolSize;
      //      image realImg(f*sizeY,f*sizeX);
      //      lti::draw<rgbPixel> drawer;
      //      drawer.use(realImg);


      for (i=0; i<sizeY; i++) {
        for (j=0; j<sizeX; j++) {
          drawer.setColor(img.at(i,j));
          drawer.rectangle(j*f,i*f,(j+1)*f-1,(i+1)*f-1,true);
        }
      }


      return ok;
    };

    /**
     * Classifies the given data with the som and draws the distribution
     * of the hits where the data points were classified in the som.
     */
    template<typename T>
    bool drawHits(drawBase<T>& drawer,const SOFM2D& som,
                  const dmatrix& data) const {

      bool ok=true;
      int sizeX=som.xSize();
      int sizeY=som.ySize();

      fmatrix count(sizeY,sizeX,0.);

      int i,j,w,x,y;
      classifier::outputVector iv;
      for (i=0; i<data.rows(); i++) {
        som.classify(data.getRow(i), iv);
        w=iv.getWinnerUnit();
        y=w/sizeX;
        x=w%sizeX;
        count.at(y,x)+=1.0f;
      }

      float chMax=count.maximum();
      count.divide(chMax);

      int f=getParameters().symbolSize;

      for (i=0; i<sizeY; i++) {
        for (j=0; j<sizeX; j++) {
          drawer.setGray(count.at(i,j));
          drawer.rectangle(j*f,i*f,(j+1)*f-1,(i+1)*f-1,true);
        }
      }

      return ok;
    }


    /**
     * Draws one component of the given som.
     */
    template<typename T>
    bool componentPlane(drawBase<T>& drawer,const SOFM2D& som,
                        const int& comp) const {
      bool ok=true;
      dvector grid(som.getPrototypes().getColumnCopy(comp));
      int sizeX=som.xSize();
      int sizeY=som.ySize();
      int i,j;

      int f=getParameters().symbolSize;
      double minimum=grid.minimum();
      if (minimum<0)
        grid.add(-1.*minimum);
      grid.divide(grid.maximum());

      for (i=0; i<sizeY; i++) {
        for (j=0; j<sizeX; j++) {
          drawer.setGray(grid.at(i*sizeX+j));
          drawer.rectangle(j*f,i*f,(j+1)*f-1,(i+1)*f-1,true);
        }
      }
      return ok;
    }

    /**
     * Applies a sammons mapping to the given som and draws the
     * resulting grid. Please not that the sammonsMapping does not
     * respect the orientation in the image plane. Thus, the result
     * might be rotated by an arbitrary angle.
     */
    template<typename T>
    bool sammonsMapper(drawBase<T>& drawer,const SOFM2D& som) const {

      bool ok=true;
      int i;

      sammonsMapping mapper;
      mapper.setParameters(getParameters().smP);

      dmatrix res;
      double error;
      ok = ok && mapper.apply(som.getPrototypes(),res,error);

      boundsFunctor<double> bounds;
      dvector mx, mn;
      bounds.boundsOfRows(res,mn,mx);

      const ipoint canvasSize=drawer.getCanvasSize();

      const double facX=(canvasSize.x-10)/(mx[0]-mn[0]);
      const double facY=(canvasSize.y-10)/(mx[1]-mn[1]);
      const double fac=min(facX,facY);

      drawer.setStyle("bo");
      for (i=0; i<res.rows(); i++) {
        drawer.marker(int(fac*(res.at(i,0)-mn[0])+5),
                      int(fac*(res.at(i,1)-mn[1])+5));
      }
      drawer.setStyle("r.");
      int x=som.xSize();
      int y=som.ySize();
      int j;
      for (i=0; i<y; i++) {
        drawer.set(int(fac*(res.at(i*x,0)-mn[0])+5),
                   int(fac*(res.at(i*x,1)-mn[1])+5));
        for(j=1; j<x; j++) {
          drawer.lineTo(int(fac*(res.at(i*x+j,0)-mn[0])+5),
                        int(fac*(res.at(i*x+j,1)-mn[1])+5));
        }
      }
      drawer.setStyle("g.");
      for (i=0; i<x; i++) {
        drawer.set(int(fac*(res.at(i,0)-mn[0])+5),
                   int(fac*(res.at(i,1)-mn[1])+5));
        for(j=1; j<y; j++) {
          drawer.lineTo(int(fac*(res.at(i+j*x,0)-mn[0])+5),
                        int(fac*(res.at(i+j*x,1)-mn[1])+5));
        }
      }

      return ok;
    }

    //  template<typename T>
    //     bool plotIds(drawBase<T>& drawer,const dmatrix& data,
    //                  const ivector& ids) const {

    //     int f=getParameters().symbolSize;

    //     int outsize=sizeX*sizeY;

    //     bool allOk=true;
    //     int i, j, k;
    //     matrix<int> clCount(outSize,outSize,0);
    //     classifier::outputVector outV;

    //     //map the actual ids to numbers between 0 and n-1
    //     std::map<int, int> realToIntern;
    //     ivector internToReal(outSize);
    //     j=0;
    //     for (i=0; i<ids.size(); i++) {
    //       if (realToIntern.find(ids.at(i))==realToIntern.end()) {
    //         realToIntern[ids.at(i)]=j;
    //         internToReal.at(j)=ids.at(i);
    //         j++;
    //       }
    //     }

    //     //initialize outTemplate with values from 0 to n-1 matching the
    //     //positions in the outTemplate
    //     ivector ideez(outSize);
    //     for (i=0; i<outSize; i++) {
    //       ideez.at(i)=i;
    //     }
    //     outTemplate=outputTemplate(ideez);

    //     //classify all data and count 'hits' for each position
    //     for(i=0;i<data.rows();i++) {
    //       allOk = classify(data[i], outV) && allOk;
    //       clCount[outV.getWinnerUnit()][realToIntern[ids[i]]]++;
    //     }

    //     double rowsum;
    //     int rowsize;
    //     outTemplate=outputTemplate(outSize);
    //     for(i=0;i<outSize;i++) {
    //       rowsum=clCount[i].sumOfElements();
    //       classifier::outputVector rowV;
    //       if (rowsum!=0) {
    //         rowsize=0;
    //         for (j=0;j<outSize;j++) {
    //           if (clCount[i][j]!=0) {
    //             rowsize++;
    //           }
    //         }
    //         rowV=outputVector(rowsize);
    //         for(j=0,k=0;j<outSize;j++) {
    //           if (clCount[i][j]!=0) {
    //             rowV.setPair(k++, internToReal[j], clCount[i][j]/rowsum);
    //           }
    //         }
    //       } else {
    //         allOk=false;
    //       }
    //       outTemplate.setProbs(i, rowV);
    //     }
    //     return allOk;
    //   }

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    SOFM2DVisualizer& copy(const SOFM2DVisualizer& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    SOFM2DVisualizer& operator=(const SOFM2DVisualizer& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

