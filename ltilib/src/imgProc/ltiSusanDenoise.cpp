/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 * This code is based on the original C code by S. Smith, which contains
 * following header:
 *
 **********************************************************************
 *
 * SUSAN Version 2l by Stephen Smith
 *
 * Oxford Centre for Functional Magnetic Resonance Imaging of the
 * Brain, Department of Clinical Neurology, Oxford University, Oxford,
 * UK (Previously in Computer Vision and Image Processing Group - now
 * Computer Vision and Electro Optics Group - DERA Chertsey, UK)
 *
 * Email:    steve@fmrib.ox.ac.uk
 * WWW:      http://www.fmrib.ox.ac.uk/~steve
 *
 * (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
 * Farnborough, Hampshire, GU14 6TD, UK
 * DERA WWW site:
 * http://www.dera.gov.uk/
 * DERA Computer Vision and Electro Optics Group WWW site:
 * http://www.dera.gov.uk/imageprocessing/dera/group_home.html
 * DERA Computer Vision and Electro Optics Group point of contact:
 * Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203
 *
 * A UK patent has been granted: "Method for digitally processing
 * images to determine the position of edges and/or corners therein for
 * guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
 * Secretary of State for Defence, UK. 15 January 1997
 *
 * This code is issued for research purposes only and remains the
 * property of the UK Secretary of State for Defence. This code must
 * not be passed on without this header information being kept
 * intact. This code must not be sold.
 **********************************************************************
 *
 *
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
 *
 */


/* -----------------------------------------------------------------------
 * project ....: LTI Digitale Bild/Signal Verarbeitungsbibliothek
 * file .......: ltiSusanDenoise.cpp
 * authors ....: Christian Harte
 * organization: LTI, RWTH Aachen
 * creation ...: 18.4.2000
 * revisions ..: ltiSusanDenoise.cpp,v $
 */

#include "ltiSusanDenoise.h"
#include "ltiTypes.h"
#include "ltiMath.h"
#include <cstdio> // for sprintf
#include <string>

namespace lti {
  // --------------------------------------------------
  // susanDenoise::parameters
  // --------------------------------------------------

  // default constructor
  susanDenoise::parameters::parameters()
    : modifier::parameters() {

    /*
     * Defines the size of the used area. Take 37 for high quality
     * pictures and a value of 9 for fast processing
     */
    kernelSize = int(37);  //two values allowed: 9 / 37

    /*
     * a value of 9.0 seems to be a good value not to smooth the
     * picture to much, but to eliminate all the noise of the
     * picture. This value has to be changed if a new image source is
     * used.  The noisier a picture is the larger this value must be.
     */
    threshold = double(9);//brightnes threshold
  }

  // copy constructor
  susanDenoise::parameters::parameters(const parameters& other)
    : modifier::parameters()  {
    copy(other);
  }

  // destructor
  susanDenoise::parameters::~parameters() {
  }

  // get type name
  const char* susanDenoise::parameters::getTypeName() const {
    return "susanDenoise::parameters";
  }

  // copy member

  susanDenoise::parameters&
    susanDenoise::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    modifier::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    modifier::parameters& (modifier::parameters::* p_copy)
      (const modifier::parameters&) =
      modifier::parameters::copy;
    (this->*p_copy)(other);
# endif

    kernelSize = other.kernelSize;
    threshold = other.threshold;

    return *this;
  }

  // clone member
  functor::parameters* susanDenoise::parameters::clone() const {
    return new parameters(*this);
  }

  /*
   * write the parameters in the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also written, otherwise only the data block will be written.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool susanDenoise::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool susanDenoise::parameters::writeMS(ioHandler& handler,
                                           const bool complete) const
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"kernelSize",kernelSize);
      lti::write(handler,"threshold",threshold);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && modifier::parameters::write(handler,false);
# else
    bool (modifier::parameters::* p_writeMS)(ioHandler&,const bool) const =
      modifier::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool susanDenoise::parameters::write(ioHandler& handler,
                                             const bool complete) const {
    // ...we need this workaround to cope with another really awful MSVC bug.
    return writeMS(handler,complete);
  }
# endif

  /*
   * read the parameters from the given ioHandler
   * @param handler the ioHandler to be used
   * @param complete if true (the default) the enclosing begin/end will
   *        be also read, otherwise only the data block will be read.
   * @return true if write was succeful
   */
# ifndef _LTI_MSC_6
  bool susanDenoise::parameters::read(ioHandler& handler,
                                        const bool complete)
# else
  bool susanDenoise::parameters::readMS(ioHandler& handler,
                                          const bool complete)
# endif
  {
    bool b = true;
    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"kernelSize",kernelSize);
      lti::read(handler,"threshold",threshold);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && modifier::parameters::read(handler,false);
# else
    bool (modifier::parameters::* p_readMS)(ioHandler&,const bool) =
      modifier::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool susanDenoise::parameters::read(ioHandler& handler,
                                    const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // --------------------------------------------------
  // susanDenoise
  // --------------------------------------------------

  // default constructor
  susanDenoise::susanDenoise()
    : modifier() {
    parameters tmp;
    setParameters(tmp);
  }

  // copy constructor
  susanDenoise::susanDenoise(const susanDenoise& other)
    : modifier()  {
    copy(other);
  }

  // destructor
  susanDenoise::~susanDenoise() {
  }

  // returns the name of this type
  const char* susanDenoise::getTypeName() const {
    return "susanDenoise";
  }

  // copy member
  susanDenoise&
    susanDenoise::copy(const susanDenoise& other) {
      modifier::copy(other);
    return (*this);
  }

  // clone member
  functor* susanDenoise::clone() const {
    return new susanDenoise(*this);
  }

  // return parameters
  const susanDenoise::parameters&
    susanDenoise::getParameters() const {
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
  bool susanDenoise::apply(channel8& srcdest) const {

    channel8 dest;
    if (apply(srcdest,dest)) {
      dest.detach(srcdest);
      return true;
    }

    //maybe apply(srcdest,srcdest); works...... (to be tested)

    return false;
  };

  // On place apply for type channel!
  bool susanDenoise::apply(channel& srcdest) const {

    //for maximum speed call apply(channel8,channel8)
    channel8 tempImage;

    tempImage.castFrom(srcdest);

    if (apply(tempImage)) {

      srcdest.castFrom(tempImage);
      return true;
    }
    return false;
  };

  // On copy apply for type channel8!
  bool susanDenoise::apply(const channel8& src,channel8& dest) const {

    int xSize=src.columns();
    int ySize=src.rows();

    dest.resize(ySize,xSize,0,false,false);

    const lti::ubyte* origImgArray=&src.at(0,0);
    lti::ubyte* destImgArray=&dest.at(0,0);

    //Susan-Denoise:

    int threeByThree=0;
    float distThreshold=0.0;//distance threshold

    /*
     * it does not take any advantage to support more than those two
     * kernels. The kernel with size of 37 pixels (dist=4.0) produces
     * nearly the optimum at quality. A larger mask may (not will)
     * produce a picture with better quality (but you won't see any
     * difference).  The larger mask will also take much more time to
     * calculate that it isn't worth it.
     *
     * With the smaller kernel (kernelSize==9) the susan algorithm is
     * very fast.
     *
     * So you can decide between speed and quality (as usual).
     */

    if (getParameters().kernelSize==37) {
      threeByThree=0;
      distThreshold=4.0;
    } else if (getParameters().kernelSize==9) {
      threeByThree=1;
      distThreshold=0.0;
    } else {
      // kernel size not supported, will assume kernelSize 37...
      threeByThree=0;
      distThreshold=4.0;
    }

    lti::ubyte* bp=0;
    lti::ubyte* memBaseBp=0;//base memory pointer of brightness table

    memBaseBp=new lti::ubyte[516];

    bp=memBaseBp;
    bp=bp+258;
    setupBrightnessLUT(bp,int(getParameters().threshold+0.5),2);

    susanSmoothing(threeByThree,(lti::ubyte*)origImgArray,
                    destImgArray,distThreshold,xSize,ySize,bp);

    //free memory
    delete [] memBaseBp; // brightness look_up_table

    return true;
  };

  // On copy apply for type channel!
  bool susanDenoise::apply(const channel& src,channel& dest) const {
    //for maximum speed call apply(channel8,channel8)

    lti::channel8 tempSrc;
    lti::channel8 tempDest;

    tempSrc.castFrom(src);

    if (apply(tempSrc,tempDest)) {

      dest.castFrom(tempDest);

      return true;
    }

    return false;
  };

  // the protected static members (from the original susan denoiser adapted
  // for the LTI-Lib

  void susanDenoise::setupBrightnessLUT(lti::ubyte* bp,int thresh,int form) {
    int   k;
    float temp;

    for(k=-256;k<257;k++) {
      temp=((float)k)/((float)thresh);
      temp=temp*temp;
      if (form==6)
        temp=temp*temp*temp;
      temp=100.0f*exp(-temp);
      *(bp+k)= (lti::ubyte)temp;
    }
  }

  int susanDenoise::enlarge(lti::ubyte*& in,
                            lti::ubyte* tmp_image,
                            int& xSize,int& ySize,
                            const int& border) {
    int i,j;

    for(i=0; i<ySize; i++)   // copy *in into tmp_image
      memcpy(tmp_image+(i+border)*(xSize+2*border)+border,
             in + i*xSize, xSize);

    for(i=0; i<border; i++) { // copy top and bottom rows;
                              // invert as many as necessary

      memcpy(tmp_image+(border-1-i)*(xSize+2*border)+border,
             in + i*xSize,xSize);
      memcpy(tmp_image+(ySize+border+i)*(xSize+2*border)+border,
             in+(ySize-i-1)*xSize,xSize);
    }

    for(i=0; i<border; i++) { // copy left and right columns
      for(j=0; j<ySize+2*border; j++) {
        tmp_image[j*(xSize+2*border)+border-1-i]=
          tmp_image[j*(xSize+2*border)+border+i];

        tmp_image[j*(xSize+2*border)+ xSize+border+i]=
          tmp_image[j*(xSize+2*border)+ xSize+border-1-i];
      }
    }

    xSize+=2*border;  // alter image size
    ySize+=2*border;
    in=tmp_image;      // repoint in

    return(0);
  }

  lti::ubyte susanDenoise::median(lti::ubyte* in,int i,int j,int xSize) {
    int p[8],k,l,tmp;

    p[0]=in[(i-1)*xSize+j-1];
    p[1]=in[(i-1)*xSize+j  ];
    p[2]=in[(i-1)*xSize+j+1];
    p[3]=in[(i  )*xSize+j-1];
    p[4]=in[(i  )*xSize+j+1];
    p[5]=in[(i+1)*xSize+j-1];
    p[6]=in[(i+1)*xSize+j  ];
    p[7]=in[(i+1)*xSize+j+1];
    for(k=0; k<7; k++) {
      for(l=0; l<(7-k); l++) {
        if (p[l]>p[l+1]) {
          tmp=p[l]; p[l]=p[l+1]; p[l+1]=tmp;
        }
      }
    }
    return( (p[3]+p[4]) / 2 );
  }

  void susanDenoise::susanSmoothing(int threeByThree,
                                    lti::ubyte* in,
                                    lti::ubyte* &out,
                                    float dt,
                                    int xSize,
                                    int ySize,
                                    lti::ubyte* bp) {

    float temp;
    int n_max, increment, maskSize,
      i,j,x,y,area,brightness,tmp,centre;
    lti::ubyte *ip, *dp, *dpt, *cp, *tmp_image;
    TOTAL_TYPE total;

    dp=0;

    if (threeByThree==0)
      maskSize = ((int)(1.5 * dt)) + 1;
    else
      maskSize = 1;

    total=(TOTAL_TYPE)0.1; /* test for total's type */
    if ( (dt>15) && (total==0) )  {
      char buffer[1024];
      std::string txt("susanDenoise::susanSmoothing: ");

      sprintf(buffer,"Distance_thresh (%f) too big for integer arithmetic.\n",
              dt);

      txt+= buffer;
      txt+= "Either reduce it to <=15 or recompile with ";
      txt+= "variable \"total\" as a float.";
      throw exception(txt.c_str());
    }

    if ( (2*maskSize+1>xSize) || (2*maskSize+1>ySize) ) {
      char buffer[1024];
      std::string txt("susanDenoise::susanSmoothing: ");

      sprintf(buffer,"Mask size (1.5*distance_thresh+1= %i)",maskSize);
      txt+=buffer;
      sprintf(buffer," too big for image (%i x %i).",xSize,ySize);
      txt+=buffer;
      throw exception(txt.c_str());
    }

    tmp_image = new lti::ubyte[ (xSize+maskSize*2) * (ySize+maskSize*2) ];

    enlarge(in,tmp_image,xSize,ySize,maskSize);


    if (threeByThree==0) { //large gaussian mask

      n_max = (maskSize*2) + 1;

      increment = xSize - n_max;

      dp = new lti::ubyte[ n_max*n_max ];

      dpt    = dp;
      temp   = -(dt*dt);

      for(i=-maskSize; i<=maskSize; i++) {
        for(j=-maskSize; j<=maskSize; j++) {
          x = (int) (100.0 * exp( ((float)((i*i)+(j*j))) / temp ));
          *dpt++ = (unsigned char)x;
        }
      }

      for (i=maskSize;i<ySize-maskSize;i++) {
        for (j=maskSize;j<xSize-maskSize;j++) {
          area = 0;
          total = 0;
          dpt = dp;
          ip = in + ((i-maskSize)*xSize) + j - maskSize;
          centre = in[i*xSize+j];
          cp = bp + centre;
          for(y=-maskSize; y<=maskSize; y++) {
            for(x=-maskSize; x<=maskSize; x++) {
              brightness = *ip++;
              tmp = *dpt++ * *(cp-brightness);
              area += tmp;
              total += tmp * brightness;
            }
            ip += increment;
          }
          tmp = area-10000;
          if (tmp==0)
            *out++=median(in,i,j,xSize);
          else
            *out++=(lti::ubyte)((total-(centre*10000))/tmp);
        }
      }
    } else {     /* 3x3 constant mask */
      /*  main section */

      for (i=1;i<ySize-1;i++) {
        for (j=1;j<xSize-1;j++) {
          area = 0;
          total = 0;
          ip = in + ((i-1)*xSize) + j - 1;
          centre = in[i*xSize+j];
          cp = bp + centre;

          brightness=*ip++;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          brightness=*ip++;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          brightness=*ip;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          ip += xSize-2;

          brightness=*ip++;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          brightness=*ip++;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          brightness=*ip;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          ip += xSize-2;

          brightness=*ip++;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          brightness=*ip++;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          brightness=*ip;
          tmp=*(cp-brightness);
          area += tmp;
          total += tmp * brightness;

          tmp = area-100;
          if (tmp==0) {
            *out++=median(in,i,j,xSize);
          } else {
            *out++=(lti::ubyte)((total-(centre*100))/tmp);
          }
        }
      }
    }

    //now free allocated memory
    delete [] tmp_image; // brightness look_up_table
    delete [] dp; // brightness look_up_table
  }

}




