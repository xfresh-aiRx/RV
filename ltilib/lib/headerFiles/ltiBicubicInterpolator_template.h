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
 * file .......: ltiBicubicInterpolator_template.h
 * authors ....: Jens Rietzschel
 * organization: LTI, RWTH Aachen
 * creation ...: 4.10.2001
 * revisions ..: $Id: ltiBicubicInterpolator_template.h,v 1.8 2004/05/03 12:17:46 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // bicubicInterpolator::parameters
  // --------------------------------------------------

  // implementation in header file due to MS VC++ bug

  // --------------------------------------------------
  // bicubicInterpolator
  // --------------------------------------------------

  // default constructor
  template<class T>
  bicubicInterpolator<T>::bicubicInterpolator()
    : scalarValuedInterpolation<T>() {

    // create an instance of the parameters with the default values
    // parameters defaultParameters;
    // set the default parameters
    // setParameters(defaultParameters);
    typename bicubicInterpolator<T>::parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // copy constructor
  template<class T>
  bicubicInterpolator<T>::bicubicInterpolator(const bicubicInterpolator& other) {
    copy(other);
  }

  // destructor
  template<class T>
  bicubicInterpolator<T>::~bicubicInterpolator() {
  }

  // returns the name of this type
  template<class T>
  const char* bicubicInterpolator<T>::getTypeName() const {
    return "bicubicInterpolator";
  }

  // copy member
  template<class T>
  bicubicInterpolator<T>&
  bicubicInterpolator<T>::copy(const bicubicInterpolator<T>& other) {
    scalarValuedInterpolation<T>::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member

  template<class T>  bicubicInterpolator<T>&
  bicubicInterpolator<T>::operator=(const bicubicInterpolator<T>& other) {
    return (copy(other));
  }

  // clone member
  template<class T>
  functor* bicubicInterpolator<T>::clone() const {
    return new bicubicInterpolator(*this);
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------
  template<class T> 
  T bicubicInterpolator<T>::apply(const vector<T>& src,const float& x) const {

    float position=x;
    int xx[3];//containing the x-values
    T y[3]; //containing the function values
    const typename bicubicInterpolator<T>::parameters& ParaObject=
      this->getParameters();
    int maxposition=src.size()-1;


    if(position>=src.size()){//beyond right border
      switch(ParaObject.boundaryType){
        case lti::Periodic:
          while(position>=src.size()) position-=src.size();break;

        case lti::Mirror:
          int a;
          a=int((position)/src.size());
          a%=2;
          while(position>=src.size()) position-=src.size();
          if(a)position=(maxposition-position);//odd successor
          break;

        case lti::NoBoundary:
        case lti::Zero:
          return static_cast<T>(0);break;

        case lti::Constant:
          return static_cast<T>(src.at(maxposition));break;
      }
    }

    else if(position<=-1){//beyond left border
      switch(ParaObject.boundaryType){
        case lti::Periodic:
          while(position<=-1) position+=src.size();break;

        case lti::Mirror:
          int a;
          a=int((-(position+1))/src.size());
          a%=2;
          while(position<=-1) position+=src.size();
          if(!a)position=(maxposition-position);//odd successor
          break;

        case lti::NoBoundary:
        case lti::Zero:
          return static_cast<T>(0);
          break;

        case lti::Constant:
          return static_cast<T>(src.at(0));break;
      }
    }


    //used within vector or beyond if case Mirror/Periodic

    if((position<1)&&(position>=0)){    //inner left border
      xx[0]=0;xx[1]=1;xx[2]=2;
    }
    else if((position>(maxposition-1))&&(position<=maxposition)){
      //inner right border
      xx[0]=maxposition-2;xx[1]=maxposition-1;xx[2]=maxposition;
    }

    else if((position<0)&&(position>-1)){    //outer left border
      xx[0]=0;xx[1]=1;xx[2]=2;

      switch(ParaObject.boundaryType){
        case lti::Periodic:
          y[0]=src.at(maxposition);y[1]=src.at(0);y[2]=src.at(1);
          position=(1+position);break;
        case lti::Mirror:
          y[0]=src.at(0);y[1]=src.at(0);y[2]=src.at(1);
          position=(1+position);break;
        case lti::NoBoundary:
        case lti::Zero:
          y[0]=0;y[1]=src.at(0);y[2]=src.at(1);
          position=(1+position);break;
        case lti::Constant:
          y[0]=src.at(0);y[1]=src.at(0);y[2]=src.at(1);
          position=(1+position);break;
      }
      return static_cast<T>(newtonInterpolation(xx,y,position));
    }

    else if((position>maxposition)&&(position<maxposition+1)){//outer right border
      xx[0]=0;xx[1]=1;xx[2]=2;

      switch(ParaObject.boundaryType){
        case lti::Periodic:
          y[0]=src.at(maxposition-1);y[1]=src.at(maxposition);y[2]=src.at(0);
          break;
        case lti::Mirror:
          y[0]=src.at(maxposition-1);y[1]=src.at(maxposition);y[2]=src.at(maxposition);
          break;
        case lti::NoBoundary:
        case lti::Zero:
          y[0]=src.at(maxposition-1);y[1]=src.at(maxposition);y[2]=0;
          break;
        case lti::Constant:
          y[0]=src.at(maxposition-1);y[1]=src.at(maxposition);y[2]=src.at(maxposition);
          break;
      }
      position=((position-maxposition)+1);
      return static_cast<T>(newtonInterpolation(xx,y,position));
    }

    else{         //normal case  (left points chosen)
      xx[0]=int(position)-1;xx[1]=int(position);xx[2]=int(position)+1;
    }

    y[0]=src.at(xx[0]);y[1]=src.at(xx[1]);y[2]=src.at(xx[2]);
    return static_cast<T>(newtonInterpolation(xx,y,position));

  }


  template<class T>
  T bicubicInterpolator<T>::apply(const float& x) const{
    return apply(*this->theVector,x);
  }



  template<class T>
  T bicubicInterpolator<T>::apply(const matrix<T>& srcdest,
                                  const float& row, const float& col) const{

    float t,u;//difference to lowleft corner t-column-  u-row-direction
    float y[4],y1[4],y2[4],y12[4];
    T result=0;
    float gradient1=0,gradient2=0;
    int maxcol=(srcdest.lastColumn());
    int maxrow=(srcdest.lastRow());

    //Beyond Borders
    if(((row>=(maxrow+1))||(row<=(-1)))||((col<=-1)||(col>=(maxcol+1)))){
      extraPolation(row,col,srcdest,result,gradient1,gradient2);
      return result;
    }

    //within border columns/rows
    else if((((row<maxrow+1)&&(row>maxrow-1))||((col<maxcol+1)&&(col>maxcol-1)))
            ||(((col<1)&&(col>-1))||((row<1)&&(row>-1)))){

      centeredDifferencingBorder(row,col,t,u,y,y1,y2,y12,srcdest);
      bicubicInterpolation(y,y1,y2,y12,row,col,t,u,result,gradient1,
                           gradient2);
      return static_cast<T>(result);
    }

    //within/on inner matrix: bicubicInterpolation(normal case)
    else {
      centeredDifferencing(row,col,t,u,y,y1,y2,y12,srcdest);
      bicubicInterpolation(y,y1,y2,y12,row,col,t,u,result,gradient1,
                           gradient2);
      return static_cast<T>(result);
    }

  }

  template<class T>
  T bicubicInterpolator<T>::apply(const float& row, const float& col) const{
    return apply(*this->theMatrix,row,col);
  }




  template<class T>
  void bicubicInterpolator<T>::centeredDifferencing(float row,
                                                    float col,
                                                    float& t,
                                                    float& u,
                                                    float y[4],
                                                    float y1[4],
                                                    float y2[4],
                                                    float y12[4],
                                                    const matrix<T>& src)const{

    int lowleftj;     //j=x1 k=x2 direction
    int lowleftk;
    int maxrow=(src.lastRow());
    int maxcol=(src.lastColumn());


    if(col==maxcol-1){//right inner border to LEFT grid
      lowleftj=(maxcol-2);
      t=1;
    }
    else {lowleftj=int(col);//vertical border belongs to right grid square
    t=(col-int(col)); //normal case
    }
    if(row==(maxrow-1)){//low inner border to UPPER grid
      lowleftk=maxrow-1;
      u=0;
    }
    else {lowleftk=(int(row)+1);//horiz. border belongs to lower grid square
    u=(int(row)+1-row);  // normal case
    }

    int j(0),k(0);
    float d=1; //length of grid cell

    for(int i=0;i<=3;i++){
      switch(i){
        case 0:j=lowleftj;k=lowleftk;break;
        case 1:j=lowleftj+1;k=lowleftk;break;
        case 2:j=lowleftj+1;k=lowleftk-1;break;
        case 3:j=lowleftj;k=lowleftk-1;break;
      };
      y[i]=static_cast<float>(src.at(k,j));
      y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2*d);
      y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2*d);
      y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
               -src.at(k-1,j-1)+src.at(k+1,j-1))/(2*d*2*d));
    }
  }







  template<class T>
  void bicubicInterpolator<T>::bicubicInterpolation(float y[4],float y1[4],
                                                    float y2[4],float y12[4],
                                                    float row,float col,
                                                    float t,float u,
                                                    T& result,
                                                    float &gradient1,
                                                    float &gradient2) const {

    float d1,d2;
    float c[4][4];

    d1=1;       //lenght of grid cell
    d2=1;


    //bcucof
    /* bcucof computes some coefficients by using a linear Transformation.
       These are stored in the table c
       d1,d2 length of grid cell (=1)*/

    //complicated linear transfomation with constant coefficients
    //tabulated below
    static const int wt[16][16]=
      {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
       {-3,0,0,3,0,0,0,0,-2,0,0,-1,0,0,0,0},
       {2,0,0,-2,0,0,0,0,1,0,0,1,0,0,0,0},
       {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
       {0,0,0,0,-3,0,0,3,0,0,0,0,-2,0,0,-1},
       {0,0,0,0,2,0,0,-2,0,0,0,0,1,0,0,1},
       {-3,3,0,0,-2,-1,0,0,0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0,-3,3,0,0,-2,-1,0,0},
       {9,-9,9,-9,6,3,-3,-6,6,-6,-3,3,4,2,1,2},
       {-6,6,-6,6,-4,-2,2,4,-3,3,3,-3,-2,-1,-1,-2},
       {2,-2,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
       {0,0,0,0,0,0,0,0,2,-2,0,0,1,1,0,0},
       {-6,6,-6,6,-3,-3,3,3,-4,4,2,-2,-2,-2,-1,-1},
       {4,-4,4,-4,2,2,-2,-2,2,-2,-2,2,1,1,1,1}
      };

    int l,k,j,i;
    float xx,d1d2,cl[16],x[16];
    d1d2=(d1*d2);

    for(i=0;i<4;i++){ //  Pack a temporary vector x
      x[i]=y[i];
      x[i+4]=y1[i]*d1;
      x[i+8]=y2[i]*d2;
      x[i+12]=y12[i]*d1d2;
    }



    for(i=0;i<=15;i++){  // Matrix multiply by the stored table
      xx=0.0;
      for(k=0;k<=15;k++) xx+=wt[i][k]*x[k];
      cl[i]=xx;
    }

    l=0;

    for(i=0;i<4;i++){              //Unpack the result into the output table
      for(j=0;j<4;j++)
        c[i][j]=cl[l++];
    }
    //end bcucof


    result=static_cast<T>(0);
    gradient1=0;
    gradient2=0;


    for(i=3;i>=0;i--){
      result=static_cast<T>(t*result+
                            ((c[i][3]*u+c[i][2])*u+c[i][1])*u+c[i][0]);
      gradient2=static_cast<float>((t*gradient2+(3.0*c[i][3]*u+2.0*c[i][2])*u+c[i][1]));
      gradient1=static_cast<float>((u*gradient1+(3.0*c[3][i]*t+2.0*c[2][i])*t+c[1][i]));
    }
    gradient1/=d1;
    gradient2/=d2;
  }



  template<class T>
  T bicubicInterpolator<T>::newtonInterpolation(const int x[3],
                                                const T y[3],
                                                const float & toinpol) const {

    T a0,a1,a2;    //coefficients
    T result;
    float z=toinpol;

    a0=y[0];
    a1=((y[1]-a0)/(x[1]-x[0]));
    a2=((y[2]-a0-a1*(x[2]-x[0]))/((x[2]-x[0])*(x[2]-x[1])));

    condRoundCastTo(a0+a1*(z-x[0])+a2*(z-x[0])*(z-x[1]),result);
    return result;
  }


  template<class T>
  void bicubicInterpolator<T>::bilinearInterpolation(const matrix<T>& src,
                                                     float row,float col,
                                                     T& result)const{
    int lowleftj,lowleftk;
    float t;  //column-direction
    float u;  //row-direction
    int maxrow=(src.lastRow());
    int maxcol=(src.lastColumn());

    if(col==maxcol){//right border to LEFT grid
      lowleftj=maxcol-1;
      t=1;
    } else {
      lowleftj=int(col);//vertical border belongs to right grid square
      t=(col-int(col));//normal case
    }
    if(row==maxrow){ //low border to UPPER grid
      lowleftk=maxrow;
      u=0;
    }
    else {
      lowleftk=(int(row)+1);//horiz. border belongs to lower grid square
      u=(int(row)+1-row);//normal case
    }

    int j(0),k(0);
    T y[4];

    for(int i=0;i<=3;i++){
      switch(i){
        case 0:j=lowleftj;k=lowleftk;break;
        case 1:j=lowleftj+1;k=lowleftk;break;
        case 2:j=lowleftj+1;k=lowleftk-1;break;
        case 3:j=lowleftj;k=lowleftk-1;break;
      };
      y[i]=(src.at(k,j));
    }
    result=(y[0]*(1-t)*(1-u)+t*(1-u)*y[1]+t*u*y[2]+(1-t)*u*y[3]);

  }


  template<class T>
  bool bicubicInterpolator<T>::getGradient(const matrix<T>& src,const float& row,
                                           const float& col,tpoint<T>& dest)const{


    float t,u;//difference to lowleft corner t-column-  u-row-direction
    float y[4],y1[4],y2[4],y12[4];
    T result=static_cast<T>(0);
    float gradient1=1,gradient2=0;
    int maxcol=(src.lastColumn());
    int maxrow=(src.lastRow());
    const typename bicubicInterpolator<T>::parameters& param =
      this->getParameters();

    //Beyond Borders
    if(((row>=(maxrow+1))||(row<=(-1)))||((col<=-1)||(col>=(maxcol+1)))){
      extraPolation(row,col,src,result,gradient1,gradient2);

      dest.x=static_cast<T>(gradient1);
      dest.y=static_cast<T>(gradient2);
      if(param.boundaryType==lti::Constant){
        if((col>=(maxcol+1))||(col<=-1))dest.x=static_cast<T>(0);
        if((row>=(maxrow+1))||(row<=-1))dest.y=static_cast<T>(0);

      }
      if((param.boundaryType==lti::Zero)||
         (param.boundaryType==lti::NoBoundary)){
        dest.x=static_cast<T>(0);
        dest.y=static_cast<T>(0);
      }
    }

    //within border columns/rows
    else if((((row<maxrow+1)&&(row>maxrow-1))||((col<maxcol+1)&&(col>maxcol-1)))
            ||(((col<1)&&(col>-1))||((row<1)&&(row>-1)))){

      centeredDifferencingBorder(row,col,t,u,y,y1,y2,y12,src);
      bicubicInterpolation(y,y1,y2,y12,row,col,t,u,result,gradient1,
                           gradient2);
      dest.x=static_cast<T>(gradient1);
      dest.y=static_cast<T>(gradient2);
    }

    //within/on inner matrix: bicubicInterpolation(normal case)
    else {
      centeredDifferencing(row,col,t,u,y,y1,y2,y12,src);
      bicubicInterpolation(y,y1,y2,y12,row,col,t,u,result,gradient1,
                           gradient2);
      dest.x=static_cast<T>(gradient1);
      dest.y=static_cast<T>(gradient2);

    }
    return true;
  }


  template<class T>
  void bicubicInterpolator<T>::centeredDifferencingBorder(float row,float col,
                                                          float& t,float& u, float y[4],float y1[4], float y2[4],float y12[4],
                                                          const matrix<T>& src) const{



    int maxcol=src.lastColumn();
    int maxrow=src.lastRow();


    //inner upper border
    if(((row<1)&&(row>=0))&&((col<maxcol-1)&&(col>=1))){

      t=(col-int(col));
      u=(1-row);
      int lowleftj=int(col);
      int lowleftk=1;
      int j(0),k(0);
      const typename  bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        y[i]=static_cast<float>(src.at(k,j));
        y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2);
        if(i>1){ //upper points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y2[i]=static_cast<float>((0-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((0-src.at(k+1,j+1)+src.at(k+1,j-1))/4);
              break;
            case lti::Constant:
              y2[i]=static_cast<float>((src.at(k,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(k,j+1)-src.at(k+1,j+1)
                      -src.at(k,j-1)+src.at(k+1,j-1))/4);
              break;
            case lti::Mirror:
              y2[i]=static_cast<float>((src.at(k,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(k,j+1)-src.at(k+1,j+1)
                      -src.at(k,j-1)+src.at(k+1,j-1))/4);
              break;
            case lti::Periodic:
              y2[i]=static_cast<float>((src.at(maxrow,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(maxrow,j+1)-src.at(k+1,j+1)
                      -src.at(maxrow,j-1)+src.at(k+1,j-1))/4);
              break;
          }
        }

        else{
          y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);
          y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
		  -src.at(k-1,j-1)+src.at(k+1,j-1))/4);
        }
      }
    }

    //inner lower border
    else if(((row>maxrow-1)&&(row<=maxrow))&&((col<maxcol-1)&&(col>=1))){

      t=(col-int(col));
      u=maxrow-row;
      int lowleftj=int(col);
      int lowleftk=maxrow;
      int j(0),k(0);
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        y[i]=static_cast<float>(src.at(k,j));
        y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2);
        if(i<2){ //lower points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y2[i]=static_cast<float>((src.at(k-1,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k-1,j-1))/4);
              break;
            case lti::Constant:
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(maxrow,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(maxrow,j+1)
                      -src.at(k-1,j-1)+src.at(maxrow,j-1))/4);
              break;
            case lti::Mirror:
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(maxrow,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(maxrow,j+1)
                      -src.at(k-1,j-1)+src.at(maxrow,j-1))/4);
              break;

            case lti::Periodic:
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(0,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(0,j+1)
                      -src.at(k-1,j-1)+src.at(0,j-1))/4);
              break;
          }
        }

        else{
          y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);
          y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
		             -src.at(k-1,j-1)+src.at(k+1,j-1))/4);
        }
      }
    }

    //inner right border
    else if(((row>=1)&&(row<maxrow-1))&&((col>maxcol-1)&&(col<=maxcol))){

      if(col<maxcol){t=(col-int(col));}
      else{t=1;}
      u=((int(row)+1)-row);
      int lowleftj;
      if(col<maxcol){lowleftj=int(col);}
      else{lowleftj=maxcol-1;}
      int lowleftk=int(row)+1;
      int j(0),k(0);
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();


      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        y[i]=static_cast<float>(src.at(k,j));
        y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);

        if((i==1)||(i==2)){ //right points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y1[i]=static_cast<float>((0-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((0-src.at(k-1,j-1)+src.at(k+1,j-1))/4);
              break;
            case lti::Constant:
              y1[i]=static_cast<float>((src.at(k,j)-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);
              break;
            case lti::Mirror:
              y1[i]=static_cast<float>((src.at(k,j)-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j)-src.at(k,j)
                      -src.at(k-1,j-1)+src.at(k,j-1))/4);
              break;
            case lti::Periodic:
              y1[i]=static_cast<float>((src.at(k,0)-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,0)-src.at(k+1,0)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);
              break;
          }
        }

        else{
          y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2);
          y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
		             -src.at(k-1,j-1)+src.at(k+1,j-1))/4);
        }
      }
    }


    //inner left border
    else if(((row>=1)&&(row<maxrow-1))&&((col>=0)&&(col<1))){

      int lowleftj=0;
      int lowleftk=int(row)+1;
      u=lowleftk-row;
      t=col;
      int j(0),k(0);
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();


      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        y[i]=static_cast<float>(src.at(k,j));
        y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);

        if((i==0)||(i==3)){ //left points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y1[i]=static_cast<float>((src.at(k,j+1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1))/4);
              break;
            case lti::Constant:
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,j)+src.at(k+1,j))/4);
              break;
            case lti::Mirror:
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,j)+src.at(k+1,j))/4);
              break;
            case lti::Periodic:
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,maxcol))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,maxcol)+src.at(k+1,maxcol))/4);
              break;
          }
        }

        else{
          y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2);
          y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
		                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);
        }
      }
    }


    //outer upper border
    else if(((row<0)&&(row>-1))&&((col<maxcol-1)&&(col>=1))){

      t=(col-int(col));
      u=0-row;
      int lowleftj=int(col);
      int lowleftk=0;
      int j(0),k(0);
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();


      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        if(i<=1){ //lower points
          y[i]=static_cast<float>(src.at(k,j));
          y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2);
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y2[i]=static_cast<float>((0-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((0-src.at(k+1,j+1)+src.at(k+1,j-1))/4);
              break;
            case lti::Constant:
              y2[i]=static_cast<float>((src.at(k,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(k,j+1)-src.at(k+1,j+1)
                      -src.at(k,j-1)+src.at(k+1,j-1))/4);break;
            case lti::Mirror:
              y2[i]=static_cast<float>((src.at(k,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(k,j+1)-src.at(k+1,j+1)
                      -src.at(k,j-1)+src.at(k+1,j-1))/4);
              break;

            case lti::Periodic:
              y2[i]=static_cast<float>((src.at(maxrow,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(maxrow,j+1)-src.at(k+1,j+1)
                      -src.at(maxrow,j-1)+src.at(k+1,j-1))/4);
              break;
          }
        }

        if(i>1){ //upper points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y[i]=0.0f;
              y1[i]=0.0f;
              y2[i]=static_cast<float>((0-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((0-src.at(k+1,j+1)+src.at(k+1,j-1))/4);
              break;
            case lti::Constant:
              y[i]=static_cast<float>(src.at(k+1,j));
              y1[i]=static_cast<float>((src.at(k+1,j+1)-src.at(k+1,j-1))/2);
              y2[i]=static_cast<float>((src.at(k+1,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(k+1,j+1)-src.at(k+1,j+1)
                      -src.at(k+1,j-1)+src.at(k+1,j-1))/4);
              break;
            case lti::Mirror:
              y[i]=static_cast<float>(src.at(k+1,j));
              y1[i]=static_cast<float>((src.at(k+1,j+1)-src.at(k+1,j-1))/2);
              y2[i]=static_cast<float>((src.at(k+2,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(k+2,j+1)-src.at(k+1,j+1)
                      -src.at(k+2,j-1)+src.at(k+1,j-1))/4);
              break;

            case lti::Periodic:
              y[i]=static_cast<float>(src.at(maxrow,j));
              y1[i]=static_cast<float>((src.at(maxrow,j+1)-src.at(maxrow,j-1))/2);
              y2[i]=static_cast<float>((src.at(maxrow-1,j)-src.at(k+1,j))/2);
              y12[i]=static_cast<float>((src.at(maxrow-1,j+1)-src.at(k+1,j+1)
                      -src.at(maxrow-1,j-1)+src.at(k+1,j-1))/4);
              break;
          }
        }
      }
    }

    //outer lower border
    else if(((row>maxrow)&&(row<maxrow+1))&&((col<maxcol-1)&&(col>=1))){

      int lowleftj=int(col);
      int lowleftk=maxrow+1;
      t=(col-int(col));
      u=lowleftk-row;
      int j(0),k(0);
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        if(i>1){ //upper points

          y[i]=static_cast<float>(src.at(k,j));
          y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j-1))/2);
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y2[i]=static_cast<float>((src.at(k-1,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k-1,j-1))/4);
              break;
            case lti::Constant:
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(maxrow,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(maxrow,j+1)
                      -src.at(k-1,j-1)+src.at(maxrow,j-1))/4);
              break;
            case lti::Mirror:
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(maxrow,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(maxrow,j+1)
                      -src.at(k-1,j-1)+src.at(maxrow,j-1))/4);
              break;

            case lti::Periodic:
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(0,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(0,j+1)
                      -src.at(k-1,j-1)+src.at(0,j-1))/4);
              break;
          }
        }

        else{ //lower points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y[i]=static_cast<float>(0);
              y1[i]=static_cast<float>(0);
              y2[i]=static_cast<float>((src.at(maxrow,j))/2);
              y12[i]=static_cast<float>((src.at(maxrow,j+1)-src.at(maxrow,j-1))/4);
              break;
            case lti::Constant:
              y[i]=static_cast<float>(src.at(maxrow,j));
              y1[i]=static_cast<float>((src.at(maxrow,j+1)-src.at(maxrow,j-1))/2);
              y2[i]=static_cast<float>(0);
              y12[i]=static_cast<float>(0);
              break;
            case lti::Mirror:
              y[i]=static_cast<float>(src.at(k-1,j));
              y1[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k-1,j-1))/2);
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k-2,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k-2,j+1)
                      -src.at(k-1,j-1)+src.at(k-2,j-1))/4);
              break;

            case lti::Periodic:
              y[i]=static_cast<float>(src.at(0,j));
              y1[i]=static_cast<float>((src.at(0,j+1)-src.at(0,j-1))/2);
              y2[i]=static_cast<float>((src.at(k-1,j)-src.at(1,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(1,j+1)
                      -src.at(k-1,j-1)+src.at(1,j-1))/4);
              break;
          }
        }
      }
    }


    //outer right border
    else if(((row>=1)&&(row<maxrow-1))&&((col<maxcol+1)&&(col>maxcol))){


      int lowleftj=maxcol;
      int lowleftk=int(row)+1;
      u=lowleftk-row;
      t=col-lowleftj;
      int j(0),k(0);
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();


      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };


        if((i==0)||(i==3)){ //left points
          y[i]=static_cast<float>(src.at(k,j));
          y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y1[i]=static_cast<float>((0-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((0-src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
            case lti::Constant:
              y1[i]=static_cast<float>((src.at(k,j)-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
            case lti::Mirror:
              y1[i]=static_cast<float>((src.at(k,j)-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
            case lti::Periodic:
              y1[i]=static_cast<float>((src.at(k,0)-src.at(k,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,0)-src.at(k+1,0)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
          }
        }

        else{//right points
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y[i]=static_cast<float>(0);
              y1[i]=static_cast<float>((-src.at(k,j-1)/2));
              y2[i]=static_cast<float>(0);
              y12[i]=static_cast<float>((-src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
            case lti::Constant:
              y[i]=static_cast<float>(src.at(k,maxcol));
              y1[i]=static_cast<float>(0);
              y2[i]=static_cast<float>((src.at(k-1,j-1)-src.at(k+1,j-1))/2);
              y12[i]=static_cast<float>(0);break;
            case lti::Mirror:
              y[i]=static_cast<float>(src.at(k,j-1));
              y1[i]=static_cast<float>((src.at(k,j-2)-src.at(k,j-1))/2);
              y2[i]=static_cast<float>((src.at(k-1,j-1)-src.at(k+1,j-1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j-2)-src.at(k+1,j-2)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
            case lti::Periodic:
              y[i]=static_cast<float>(src.at(k,0));
              y1[i]=static_cast<float>((src.at(k,1)-src.at(k,j-1))/2);
              y2[i]=static_cast<float>((src.at(k-1,0)-src.at(k+1,0))/2);
              y12[i]=static_cast<float>((src.at(k-1,1)-src.at(k+1,1)
                      -src.at(k-1,j-1)+src.at(k+1,j-1))/4);break;
          }
        }
      }
    }


    //outer left border
    else if(((row>=1)&&(row<maxrow-1))&&((col>-1)&&(col<0))){

      int lowleftj=-1;
      int lowleftk=int(row)+1;
      u=lowleftk-row;
      t=(1+col);
      int j(0),k(0);

      const typename scalarValuedInterpolation<T>::parameters& ParaObject =
        this->getParameters();

      for(int i=0;i<=3;i++){
        switch(i){
          case 0:j=lowleftj;k=lowleftk;break;
          case 1:j=lowleftj+1;k=lowleftk;break;
          case 2:j=lowleftj+1;k=lowleftk-1;break;
          case 3:j=lowleftj;k=lowleftk-1;break;
        };

        y[i]=static_cast<float>(src.at(k,j));
        y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);

        if((i==1)||(i==2)){ //right points
          y[i]=static_cast<float>(src.at(k,j));
          y2[i]=static_cast<float>((src.at(k-1,j)-src.at(k+1,j))/2);
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y1[i]=static_cast<float>((src.at(k,j+1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1))/4);break;
            case lti::Constant:
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,j)+src.at(k+1,j))/4);
              break;
            case lti::Mirror:
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,j)+src.at(k+1,j))/4);
              break;
            case lti::Periodic:
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,maxcol))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,maxcol)+src.at(k+1,maxcol))/4);break;
          }
        }
        else{
          switch(ParaObject.boundaryType){
            case lti::NoBoundary:
            case lti::Zero:
              y[i]=0.0f;
              y1[i]=static_cast<float>((src.at(k,j+1)/2));
              y2[i]=0.0f;
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1))/4);break;
            case lti::Constant:
              y[i]=static_cast<float>(src.at(k,0));
              y1[i]=0.0f;
              y2[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1))/2);
              y12[i]=0.0f;
              break;
            case lti::Mirror:
              y[i]=static_cast<float>(src.at(k,j+1));
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,j+2))/2);
              y2[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,j+2)+src.at(k+1,j+2))/4);break;
            case lti::Periodic:
              y[i]=static_cast<float>(src.at(k,maxcol));
              y1[i]=static_cast<float>((src.at(k,j+1)-src.at(k,maxcol-1))/2);
              y2[i]=static_cast<float>((src.at(k-1,maxcol)-src.at(k+1,maxcol))/2);
              y12[i]=static_cast<float>((src.at(k-1,j+1)-src.at(k+1,j+1)
                      -src.at(k-1,maxcol-1)+src.at(k+1,maxcol-1))/4);
              break;
          }
        }
      }
    }

    else if((row<1)&&(col>=maxcol-1)){//upper right corner

      float roww,coll;
      matrix<T> mat(5,5,static_cast<T>(0));
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      switch(ParaObject.boundaryType){
        case lti::Zero: break;
        case lti::NoBoundary:break;
        case lti::Constant:
          mat.at(0,0)=src.at(0,maxcol-2);
          mat.at(1,0)=src.at(0,maxcol-2);
          mat.at(0,1)=src.at(0,maxcol-1);
          mat.at(1,1)=src.at(0,maxcol-1);
          mat.at(0,2)=src.at(0,maxcol);
          mat.at(1,2)=src.at(0,maxcol);
          mat.at(0,3)=src.at(0,maxcol);
          mat.at(1,3)=src.at(0,maxcol);
          mat.at(2,3)=src.at(0,maxcol);
          mat.at(0,4)=src.at(0,maxcol);
          mat.at(1,4)=src.at(0,maxcol);
          mat.at(2,4)=src.at(0,maxcol);
          mat.at(3,3)=src.at(1,maxcol);
          mat.at(3,4)=src.at(1,maxcol);
          mat.at(4,4)=src.at(2,maxcol);
          mat.at(4,3)=src.at(2,maxcol);break;
        case lti::Mirror:
          mat.at(0,0)=src.at(1,maxcol-2);
          mat.at(1,0)=src.at(0,maxcol-2);
          mat.at(0,1)=src.at(1,maxcol-1);
          mat.at(1,1)=src.at(0,maxcol-1);
          mat.at(0,2)=src.at(1,maxcol);
          mat.at(1,2)=src.at(0,maxcol);
          mat.at(0,3)=src.at(1,maxcol);
          mat.at(1,3)=src.at(0,maxcol);
          mat.at(2,3)=src.at(0,maxcol);
          mat.at(0,4)=src.at(1,maxcol-1);
          mat.at(1,4)=src.at(0,maxcol-1);
          mat.at(2,4)=src.at(0,maxcol-1);
          mat.at(3,3)=src.at(1,maxcol);
          mat.at(3,4)=src.at(1,maxcol-1);
          mat.at(4,4)=src.at(2,maxcol-1);
          mat.at(4,3)=src.at(2,maxcol);break;
        case lti::Periodic:
          mat.at(0,0)=src.at(maxrow-1,maxcol-2);
          mat.at(1,0)=src.at(maxrow,maxcol-2);
          mat.at(0,1)=src.at(maxrow-1,maxcol-1);
          mat.at(1,1)=src.at(maxrow,maxcol-1);
          mat.at(0,2)=src.at(maxrow-1,maxcol);
          mat.at(1,2)=src.at(maxrow,maxcol);
          mat.at(0,3)=src.at(maxrow-1,0);
          mat.at(1,3)=src.at(maxrow,0);
          mat.at(2,3)=src.at(0,0);
          mat.at(0,4)=src.at(maxrow-1,1);
          mat.at(1,4)=src.at(maxrow,1);
          mat.at(2,4)=src.at(0,1);
          mat.at(3,3)=src.at(1,0);
          mat.at(3,4)=src.at(1,1);
          mat.at(4,4)=src.at(2,1);
          mat.at(4,3)=src.at(2,0);break;
      }

      mat.at(2,0)=src.at(0,maxcol-2);
      mat.at(2,1)=src.at(0,maxcol-1);
      mat.at(2,2)=src.at(0,maxcol);
      mat.at(3,0)=src.at(1,maxcol-2);
      mat.at(3,1)=src.at(1,maxcol-1);
      mat.at(3,2)=src.at(1,maxcol);
      mat.at(4,0)=src.at(2,maxcol-2);
      mat.at(4,1)=src.at(2,maxcol-1);
      mat.at(4,2)=src.at(2,maxcol);

      roww=row+2;
      coll=col-maxcol+2;
      centeredDifferencing(roww,coll,t,u,y,y1,y2,y12,mat);
    }

    else if((row>=maxrow-1)&&(col>=maxcol-1)){//lower right corner

      float roww,coll;
      matrix<T> mat(5,5,static_cast<T>(0));
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      switch(ParaObject.boundaryType){
        case lti::Zero: break;
        case lti::NoBoundary:break;
        case lti::Constant:
          mat.at(0,3)=src.at(maxrow-2,maxcol);
          mat.at(0,4)=src.at(maxrow-2,maxcol);
          mat.at(1,3)=src.at(maxrow-1,maxcol);
          mat.at(1,4)=src.at(maxrow-1,maxcol);
          mat.at(2,3)=src.at(maxrow,maxcol);
          mat.at(2,4)=src.at(maxrow,maxcol);
          mat.at(3,0)=src.at(maxrow,maxcol-2);
          mat.at(3,1)=src.at(maxrow,maxcol-1);
          mat.at(3,2)=src.at(maxrow,maxcol);
          mat.at(3,3)=src.at(maxrow,maxcol);
          mat.at(3,4)=src.at(maxrow,maxcol);
          mat.at(4,0)=src.at(maxrow,maxcol-2);
          mat.at(4,1)=src.at(maxrow,maxcol-1);
          mat.at(4,2)=src.at(maxrow,maxcol);
          mat.at(4,3)=src.at(maxrow,maxcol);
          mat.at(4,4)=src.at(maxrow,maxcol);break;
        case lti::Mirror:
          mat.at(0,3)=src.at(maxrow-2,maxcol);
          mat.at(0,4)=src.at(maxrow-2,maxcol-1);
          mat.at(1,3)=src.at(maxrow-1,maxcol);
          mat.at(1,4)=src.at(maxrow-1,maxcol-1);
          mat.at(2,3)=src.at(maxrow,maxcol);
          mat.at(2,4)=src.at(maxrow,maxcol-1);
          mat.at(3,0)=src.at(maxrow,maxcol-2);
          mat.at(3,1)=src.at(maxrow,maxcol-1);
          mat.at(3,2)=src.at(maxrow,maxcol);
          mat.at(3,3)=src.at(maxrow,maxcol);
          mat.at(3,4)=src.at(maxrow,maxcol-1);
          mat.at(4,0)=src.at(maxrow-1,maxcol-2);
          mat.at(4,1)=src.at(maxrow-1,maxcol-1);
          mat.at(4,2)=src.at(maxrow-1,maxcol);
          mat.at(4,3)=src.at(maxrow-1,maxcol);
          mat.at(4,4)=src.at(maxrow-1,maxcol-1);break;
        case lti::Periodic:
          mat.at(0,3)=src.at(maxrow-2,0);
          mat.at(0,4)=src.at(maxrow-2,1);
          mat.at(1,3)=src.at(maxrow-1,0);
          mat.at(1,4)=src.at(maxrow-1,1);
          mat.at(2,3)=src.at(maxrow,0);
          mat.at(2,4)=src.at(maxrow,1);
          mat.at(3,0)=src.at(0,maxcol-2);
          mat.at(3,1)=src.at(0,maxcol-1);
          mat.at(3,2)=src.at(0,maxcol);
          mat.at(3,3)=src.at(0,0);
          mat.at(3,4)=src.at(0,1);
          mat.at(4,0)=src.at(1,maxcol-2);
          mat.at(4,1)=src.at(1,maxcol-1);
          mat.at(4,2)=src.at(1,maxcol);
          mat.at(4,3)=src.at(1,0);
          mat.at(4,4)=src.at(1,1);break;

      }
      mat.at(0,0)=src.at(maxrow-2,maxcol-2);
      mat.at(0,1)=src.at(maxrow-2,maxcol-1);
      mat.at(0,2)=src.at(maxrow-2,maxcol);
      mat.at(1,0)=src.at(maxrow-1,maxcol-2);
      mat.at(1,1)=src.at(maxrow-1,maxcol-1);
      mat.at(1,2)=src.at(maxrow-1,maxcol);
      mat.at(2,0)=src.at(maxrow,maxcol-2);
      mat.at(2,1)=src.at(maxrow,maxcol-1);
      mat.at(2,2)=src.at(maxrow,maxcol);

      roww=row-maxrow+2;
      coll=col-maxcol+2;
      centeredDifferencing(roww,coll,t,u,y,y1,y2,y12,mat);
    }


    else if((row>=maxrow-1)&&(col<1)){//lower left corner

      float roww,coll;
      matrix<T> mat(5,5,static_cast<T>(0));
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      switch(ParaObject.boundaryType){
        case lti::Zero: break;
        case lti::NoBoundary:break;
        case lti::Constant:
          mat.at(0,0)=src.at(maxrow-2,0);
          mat.at(0,1)=src.at(maxrow-2,0);
          mat.at(1,0)=src.at(maxrow-1,0);
          mat.at(1,1)=src.at(maxrow-1,0);
          mat.at(2,0)=src.at(maxrow,0);
          mat.at(2,1)=src.at(maxrow,0);
          mat.at(3,0)=src.at(maxrow,0);
          mat.at(3,1)=src.at(maxrow,0);
          mat.at(3,2)=src.at(maxrow,0);
          mat.at(3,3)=src.at(maxrow,1);
          mat.at(3,4)=src.at(maxrow,2);
          mat.at(4,0)=src.at(maxrow,0);
          mat.at(4,1)=src.at(maxrow,0);
          mat.at(4,2)=src.at(maxrow,0);
          mat.at(4,3)=src.at(maxrow,1);
          mat.at(4,4)=src.at(maxrow,2);break;
        case lti::Mirror:
          mat.at(0,0)=src.at(maxrow-2,1);
          mat.at(0,1)=src.at(maxrow-2,0);
          mat.at(1,0)=src.at(maxrow-1,1);
          mat.at(1,1)=src.at(maxrow-1,0);
          mat.at(2,0)=src.at(maxrow,1);
          mat.at(2,1)=src.at(maxrow,0);
          mat.at(3,0)=src.at(maxrow,1);
          mat.at(3,1)=src.at(maxrow,0);
          mat.at(3,2)=src.at(maxrow,0);
          mat.at(3,3)=src.at(maxrow,1);
          mat.at(3,4)=src.at(maxrow,2);
          mat.at(4,0)=src.at(maxrow-1,1);
          mat.at(4,1)=src.at(maxrow-1,0);
          mat.at(4,2)=src.at(maxrow-1,0);
          mat.at(4,3)=src.at(maxrow-1,1);
          mat.at(4,4)=src.at(maxrow-1,2);break;
        case lti::Periodic:
          mat.at(0,0)=src.at(maxrow-2,maxcol-1);
          mat.at(0,1)=src.at(maxrow-2,maxcol);
          mat.at(1,0)=src.at(maxrow-1,maxcol-1);
          mat.at(1,1)=src.at(maxrow-1,maxcol);
          mat.at(2,0)=src.at(maxrow,maxcol-1);
          mat.at(2,1)=src.at(maxrow,maxcol);
          mat.at(3,0)=src.at(0,maxcol-1);
          mat.at(3,1)=src.at(0,maxcol);
          mat.at(3,2)=src.at(0,0);
          mat.at(3,3)=src.at(0,1);
          mat.at(3,4)=src.at(0,2);
          mat.at(4,0)=src.at(1,maxcol-1);
          mat.at(4,1)=src.at(1,maxcol);
          mat.at(4,2)=src.at(1,0);
          mat.at(4,3)=src.at(1,1);
          mat.at(4,4)=src.at(1,2);break;

      }
      mat.at(0,2)=src.at(maxrow-2,0);
      mat.at(0,3)=src.at(maxrow-2,1);
      mat.at(0,4)=src.at(maxrow-2,2);
      mat.at(1,2)=src.at(maxrow-1,0);
      mat.at(1,3)=src.at(maxrow-1,1);
      mat.at(1,4)=src.at(maxrow-1,2);
      mat.at(2,2)=src.at(maxrow,0);
      mat.at(2,3)=src.at(maxrow,1);
      mat.at(2,4)=src.at(maxrow,2);

      roww=row-maxrow+2;
      coll=col+2;
      centeredDifferencing(roww,coll,t,u,y,y1,y2,y12,mat);
    }


    else if((row<1)&&(col<1)){//upper left corner

      float roww,coll;
      matrix<T> mat(5,5,static_cast<T>(0));
      const typename bicubicInterpolator<T>::parameters& ParaObject=
        this->getParameters();

      switch(ParaObject.boundaryType){
        case lti::Zero: break;
        case lti::NoBoundary:break;
        case lti::Constant:
          mat.at(0,0)=src.at(0,0);
          mat.at(0,1)=src.at(0,0);
          mat.at(0,2)=src.at(0,0);
          mat.at(0,3)=src.at(0,1);
          mat.at(0,4)=src.at(0,2);
          mat.at(1,0)=src.at(0,0);
          mat.at(1,1)=src.at(0,0);
          mat.at(1,2)=src.at(0,0);
          mat.at(1,3)=src.at(0,1);
          mat.at(1,4)=src.at(0,2);
          mat.at(2,0)=src.at(0,0);
          mat.at(2,1)=src.at(0,0);
          mat.at(3,0)=src.at(1,0);
          mat.at(3,1)=src.at(1,0);
          mat.at(4,0)=src.at(2,0);
          mat.at(4,1)=src.at(2,0);break;
        case lti::Mirror:
          mat.at(0,0)=src.at(1,1);
          mat.at(0,1)=src.at(1,0);
          mat.at(0,2)=src.at(1,0);
          mat.at(0,3)=src.at(1,1);
          mat.at(0,4)=src.at(1,2);
          mat.at(1,0)=src.at(0,1);
          mat.at(1,1)=src.at(0,0);
          mat.at(1,2)=src.at(0,0);
          mat.at(1,3)=src.at(0,1);
          mat.at(1,4)=src.at(0,2);
          mat.at(2,0)=src.at(0,1);
          mat.at(2,1)=src.at(0,0);
          mat.at(3,0)=src.at(1,1);
          mat.at(3,1)=src.at(1,0);
          mat.at(4,0)=src.at(2,1);
          mat.at(4,1)=src.at(2,0);break;
        case lti::Periodic:
          mat.at(0,0)=src.at(maxrow-1,maxcol-1);
          mat.at(0,1)=src.at(maxrow-1,maxcol);
          mat.at(0,2)=src.at(maxrow-1,0);
          mat.at(0,3)=src.at(maxrow-1,1);
          mat.at(0,4)=src.at(maxrow-1,2);
          mat.at(1,0)=src.at(maxrow,maxcol-1);
          mat.at(1,1)=src.at(maxrow,maxcol);
          mat.at(1,2)=src.at(maxrow,0);
          mat.at(1,3)=src.at(maxrow,1);
          mat.at(1,4)=src.at(maxrow,2);
          mat.at(2,0)=src.at(0,maxcol-1);
          mat.at(2,1)=src.at(0,maxcol);
          mat.at(3,0)=src.at(1,maxcol-1);
          mat.at(3,1)=src.at(1,maxcol);
          mat.at(4,0)=src.at(2,maxcol-1);
          mat.at(4,1)=src.at(2,maxcol);break;
      }

      mat.at(2,2)=src.at(0,0);
      mat.at(2,3)=src.at(0,1);
      mat.at(2,4)=src.at(0,2);
      mat.at(3,2)=src.at(1,0);
      mat.at(3,3)=src.at(1,1);
      mat.at(3,4)=src.at(1,2);
      mat.at(4,2)=src.at(2,0);
      mat.at(4,3)=src.at(2,1);
      mat.at(4,4)=src.at(2,2);

      roww=row+2;
      coll=col+2;
      centeredDifferencing(roww,coll,t,u,y,y1,y2,y12,mat);

    }

  }



  template<class T>
  void bicubicInterpolator<T>::extraPolation(float row,float col,
                                             const matrix<T>& src,
                                             T& result,
                                             float &gradient1,
                                             float &gradient2) const {


    float t,u;
    float y[4],y1[4],y2[4],y12[4];
    int maxcol=(src.lastColumn());
    int maxrow=(src.lastRow());
    float roww(0.0f),coll(0.0f);
    const typename bicubicInterpolator<T>::parameters& ParaObject=
      this->getParameters();

    if((col>=(maxcol+1))&&((row<(maxrow+1))&&(row>(-1)))){//beyond right border
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;
        case lti::Constant:
          roww=row;
          coll=static_cast<float>(maxcol);
          break;

        case lti::Mirror:

          int a;
          a=int(col/src.columns());
          a%=2;
          coll=col;
          while(coll>=src.columns()) coll-=(src.columns());
          if(a) coll=(maxcol-coll);//odd successor

          roww=row;break;

        case lti::Periodic:   coll=col;
          while(coll>=src.columns()) coll-=src.columns();
          roww=row;break;

      }
    }

    else if((col<=(-1))&&((row<maxrow+1)&&(row>(-1)))){      //beyond left border
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;

        case lti::Constant:
          roww=row;
          coll=0;
          break;

        case lti::Mirror:

          int a;
          a=int((-(col+1))/src.columns());
          a%=2;
          coll=col;
          while(coll<=-1)coll+=src.columns();
          if(!a) coll=(maxcol-coll);//odd successor

          roww=row;break;

        case lti::Periodic:
          coll=col;
          while(coll<=-1) coll+=src.columns();
          roww=row;break;
      }
    }

    else if((row>=(maxrow+1))&&((col<(maxcol+1))&&(col>(-1)))){ //beyond low border
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;break;

        case lti::Constant:
          roww=static_cast<float>(maxrow);
          coll=col;

          break;

        case lti::Mirror:

          roww=row;
          int a;
          a=int((roww)/src.rows());
          a%=2;
          while(roww>=src.rows()) roww-=src.rows();
          if(a)roww=(maxrow-roww);//odd successor

          coll=col;break;

        case lti::Periodic:
          roww=row;
          while(roww>=src.rows()) roww-=src.rows();
          coll=col;break;
      }
    }

    else if((row<=(-1))&&((col<maxcol+1)&&(col>(-1)))){      //beyond upperborder
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;

        case lti::Constant:
          roww=0;
          coll=col;

          break;

        case lti::Mirror:

          roww=row;
          int a;
          a=int((-(roww+1))/src.rows());
          a%=2;
          while(roww<=-1)roww+=src.rows();
          if(!a) roww=(maxrow-roww);//odd successor

          coll=col;break;

        case lti::Periodic:
          roww=row;
          while(roww<=-1) roww+=src.rows();
          coll=col;break;
      }
    }


    else if((row>=(maxrow+1))&&(col>=(maxcol+1))){ //beyond lower right corner
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;

        case lti::Constant:
          result=src.at(maxrow,maxcol);
          if(gradient1){gradient1=0;gradient2=0;return;}
          return;

        case lti::Mirror:

          coll=col;
          int a;
          a=int(col/src.columns());
          a%=2;
          while(coll>=src.columns()) coll-=(src.columns());
          if(a) coll=(maxcol-coll);//odd successor

          roww=row;
          int b;
          b=int(roww/src.rows());
          b%=2;
          while(roww>=src.rows()) roww-=src.rows();
          if(b)roww=(maxrow-roww);//odd successor

          break;

        case lti::Periodic:
          coll=col;
          while(coll>=src.columns()) coll-=src.columns();
          roww=row;
          while(roww>=src.rows()) roww-=src.rows();
      }
    }



    else if((row<=(-1))&&(col<=(-1))){ //beyond upper left corner
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;

        case lti::Constant:
          result=src.at(0,0);
          if(gradient1){gradient1=0;gradient2=0;return;}
          return;

        case lti::Mirror:

          int a;
          a=int((-(col+1))/src.columns());
          a%=2;
          coll=col;
          while(coll<=-1)coll+=src.columns();
          if(!a) coll=(maxcol-coll);//odd successor

          roww=row;
          int b;
          b=int((-(roww+1))/src.rows());
          b%=2;
          while(roww<=-1)roww+=src.rows();
          if(!b) roww=(maxrow-roww);//odd successor

          break;

        case lti::Periodic:
          roww=row;
          while(roww<=-1) roww+=src.rows();
          coll=col;
          while(coll<=-1) coll+=src.columns();
      }
    }


    else if((row<=-1)&&(col>=(maxcol+1))){ //beyond upper right corner
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;

        case lti::Constant:
          result=src.at(0,maxcol);
          if(gradient1){gradient1=0;gradient2=0;return;}
          return;

        case lti::Mirror:

          coll=col;
          int a;
          a=int(col/src.columns());
          a%=2;
          while(coll>=src.columns()) coll-=(src.columns());
          if(a) coll=(maxcol-coll);//odd successor


          roww=row;
          int b;
          b=int((-(roww+1))/src.rows());
          b%=2;
          while(roww<=-1)roww+=src.rows();
          if(!b) roww=(maxrow-roww);//odd successor


          break;

        case lti::Periodic:
          coll=col;
          while(coll>=src.columns()) coll-=src.columns();
          roww=row;
          while(roww<=-1) roww+=src.rows();
      }
    }

    else if((row>=(maxrow+1))&&(col<=(-1))){ //beyond lower left corner
      switch(ParaObject.boundaryType){
        case lti::NoBoundary:
        case lti::Zero:
          result=static_cast<T>(0);
          return;

        case lti::Constant:
          result=src.at(maxrow,0);
          if(gradient1){gradient1=0;gradient2=0;return;}
          return;

        case lti::Mirror:

          int a;
          a=int((-(col+1))/src.columns());
          a%=2;
          coll=col;
          while(coll<=-1)coll+=src.columns();
          if(!a) coll=(maxcol-coll);//odd successor

          roww=row;
          int b;
          b=int(roww/src.rows());
          b%=2;
          while(roww>=src.rows()) roww-=src.rows();
          if(b)roww=(maxrow-roww);//odd successor


          break;

        case lti::Periodic:
          roww=row;
          while(roww>=src.rows()) roww-=src.rows();
          coll=col;
          while(coll<=-1) coll+=src.columns();
          break;

      }
    }
#if 0
    else {
      cout<<"\n\nerror extrapol col: "<<col<<"     row: "<<row<<"\n\n";
    }
#endif

    //in case Mirror,Periodic,Constant roww and coll are set into
    //the matrix and then bicubicInterpolation is used

    //within border columns/rows
    if((((roww<maxrow+1)&&(roww>maxrow-1))||((coll<maxcol+1)&&(coll>maxcol-1)))
       ||(((coll<1)&&(coll>-1))||((roww<1)&&(roww>-1)))){

      centeredDifferencingBorder(roww,coll,t,u,y,y1,y2,y12,src);
      bicubicInterpolation(y,y1,y2,y12,roww,coll,t,u,result,gradient1,
                           gradient2);
    }

    //within/on inner matrix: bicubicInterpolation(normal case)
    else{
      centeredDifferencing(roww,coll,t,u,y,y1,y2,y12,src);
      bicubicInterpolation(y,y1,y2,y12,roww,coll,t,u,result,gradient1,
                           gradient2);
    }
  }
}

