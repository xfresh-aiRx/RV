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
 * file .......: ltiSusanCorners.cpp
 * authors ....: Frederik Lange
 * organization: LTI, RWTH Aachen
 * creation ...: 27.3.2001
 * revisions ..: $Id: ltiSusanCorners.cpp,v 1.19 2005/06/16 12:35:44 doerfler Exp $
 */

#include "ltiSusanCorners.h"

namespace lti {
  // --------------------------------------------------
  // susanCorners::parameters
  // --------------------------------------------------

  // default constructor
  susanCorners::parameters::parameters()
    : cornerDetector::parameters() {

    kernelSize = int(5);
    threshold = int(20);
    geometricThreshold = int(290);
  }

  // copy constructor
  susanCorners::parameters::parameters(const parameters& other)
    : cornerDetector::parameters()  {
    copy(other);
  }

  // destructor
  susanCorners::parameters::~parameters() {
  }

  // get type name
  const char* susanCorners::parameters::getTypeName() const {
    return "susanCorners::parameters";
  }

  // copy member

  susanCorners::parameters&
    susanCorners::parameters::copy(const parameters& other) {
# ifndef _LTI_MSC_6
    // MS Visual C++ 6 is not able to compile this...
    cornerDetector::parameters::copy(other);
# else
    // ...so we have to use this workaround.
    // Conditional on that, copy may not be virtual.
    cornerDetector::parameters& (cornerDetector::parameters::* p_copy)
      (const cornerDetector::parameters&) =
      cornerDetector::parameters::copy;
    (this->*p_copy)(other);
# endif

    kernelSize = other.kernelSize;
    threshold = other.threshold;
    geometricThreshold = other.geometricThreshold;
    return *this;
  }

  // alias for copy member
  susanCorners::parameters&
    susanCorners::parameters::operator=(const parameters& other) {
    return copy(other);
  }

  // clone member
  functor::parameters* susanCorners::parameters::clone() const {
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
  bool susanCorners::parameters::write(ioHandler& handler,
                                         const bool complete) const
# else
  bool susanCorners::parameters::writeMS(ioHandler& handler,
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
      lti::write(handler,"geometricThreshold",geometricThreshold);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && cornerDetector::parameters::write(handler,false);
# else
    bool (cornerDetector::parameters::* p_writeMS)(ioHandler&,
                                                   const bool) const =
      cornerDetector::parameters::writeMS;
    b = b && (this->*p_writeMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool susanCorners::parameters::write(ioHandler& handler,
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
  bool susanCorners::parameters::read(ioHandler& handler,
                                        const bool complete)
# else
  bool susanCorners::parameters::readMS(ioHandler& handler,
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
      lti::read(handler,"geometricThreshold",geometricThreshold);
    }

# ifndef _LTI_MSC_6
    // This is the standard C++ code, which MS Visual C++ 6 is not able to
    // compile...
    b = b && cornerDetector::parameters::read(handler,false);
# else
    bool (cornerDetector::parameters::* p_readMS)(ioHandler&,const bool) =
      cornerDetector::parameters::readMS;
    b = b && (this->*p_readMS)(handler,false);
# endif

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

# ifdef _LTI_MSC_6
  bool susanCorners::parameters::read(ioHandler& handler,
                                        const bool complete) {
    // ...we need this workaround to cope with another really awful MSVC
    // bug.
    return readMS(handler,complete);
  }
# endif

  // --------------------------------------------------
  // susanCorners
  // --------------------------------------------------

  inline int susanCorners::round(const float x) const {
    if (x>=0) {
      return static_cast<int>(x+0.5f);
    } else {
      return static_cast<int>(x-0.5f);
    }
  }

  // default constructor
  susanCorners::susanCorners()
    : cornerDetector(),brightnessLUT(0) {
    parameters defaultParam;
    setParameters(defaultParam);
  }

  // default constructor
  susanCorners::susanCorners(const parameters& par)
    : cornerDetector(),brightnessLUT(0) {
    setParameters(par);
  }

  // copy constructor
  susanCorners::susanCorners(const susanCorners& other)
    : cornerDetector(),brightnessLUT(0)  {
    copy(other);
  }

  // destructor
  susanCorners::~susanCorners() {
    deleteBrightnessLUT(brightnessLUT);
  }

  // returns the name of this type
  const char* susanCorners::getTypeName() const {
    return "susanCorners";
  }

  // copy member
  susanCorners&
  susanCorners::copy(const susanCorners& other) {
    cornerDetector::copy(other);
    return (*this);
  }

  // copy member
  susanCorners&
  susanCorners::operator=(const susanCorners& other) {
    return copy(*this);
  }

  // clone member
  functor* susanCorners::clone() const {
    return new susanCorners(*this);
  }

  // return parameters
  const susanCorners::parameters&
    susanCorners::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  bool susanCorners::updateParameters() {
    const parameters& par = getParameters();
    setupBrightnessLUT(brightnessLUT,par.threshold,6);
    return true;
  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type channel8!
  bool susanCorners::apply(channel8& srcdest) const {
    return cornerDetector::apply(srcdest);
  };

  // On place apply for type channel!
  bool susanCorners::apply(channel& srcdest) const {
    return cornerDetector::apply(srcdest);
  };

  // On copy apply for type channel8!
  bool susanCorners::apply(const channel8& src,channel8& dest) const {
    return cornerDetector::apply(src,dest);
  };

  // On copy apply for type channel!
  bool susanCorners::apply(const channel& src,channel& dest) const {
    return cornerDetector::apply(src,dest);
  };

  bool susanCorners::apply(const channel& src, pointList& dest) const {

    channel8 tmpx;
    tmpx.castFrom(src);

    apply(tmpx,dest);

    return true;
  }

  bool susanCorners::apply(const channel8& src, pointList& dest) const {
    const parameters& par = getParameters();

    if (isNull(brightnessLUT)) {
      setStatusString("No parameters set yet");
      return false;
    }

    const ubyte* bp = brightnessLUT;
    dest.clear();

    // detect the corners in the image
    detect(&src[0][0], // input picture
           bp,
           1850,        // max_no,1850 //??????????
           par.geometricThreshold,
           src.columns(),
           src.rows(),
           dest);

    return true;
  }

  void susanCorners::setupBrightnessLUT(ubyte*& bp,
                                        const int& thresh,
                                        const int& form) {
    int   k;
    float temp;

    if (notNull(bp)) {
      deleteBrightnessLUT(bp);
    }

    bp=new ubyte[516];
    bp+=258;

    for(k=-256;k<257;k++) {
      temp=((float)k)/((float)thresh);
      temp=temp*temp;
      if (form==6)
      	temp=temp*temp*temp;
      temp=100.0f*exp(-temp);
      *(bp+k)= (ubyte)temp;
    }
  }

  void susanCorners::deleteBrightnessLUT(ubyte*& bp) {

    bp-=258;
    delete[] bp;
    bp = 0;
  }

  void susanCorners::detect(const ubyte *in,
                            const ubyte *bp,  // LUT
                            const int& max_no,
                            const int& geometricThreshold,
                            const int& x_size,
                            const int& y_size,
                            pointList& corner_list) const {

    int n,x,y,sq,xx,yy,i,j,*cgx,*cgy;
    float divide;
    ubyte c;
    const ubyte* cp;
    const ubyte* p;
    int *r;

    // allocate and initialize with 0
    r = new int[x_size * y_size];
    memset(r,0,x_size*y_size*sizeof(int));

    cgx=new int[x_size*y_size];
    cgy=new int[x_size*y_size];

    for (i=5;i<y_size-5;i++)
      for (j=5;j<x_size-5;j++) {
        n=100;
        p=in + (i-3)*x_size + j - 1;
        cp=bp + in[i*x_size+j];

        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=x_size-3;

        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=x_size-5;

        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
        p+=x_size-6;

        n+=*(cp-*p++);
        n+=*(cp-*p++);
        n+=*(cp-*p);
      	if (n<max_no){
          /* do this test early and often ONLY to save wasted computation */
          p+=2;
          n+=*(cp-*p++);
          if (n<max_no){
            n+=*(cp-*p++);
            if (n<max_no){
              n+=*(cp-*p);
              if (n<max_no){
                p+=x_size-6;

                n+=*(cp-*p++);
                if (n<max_no){
                  n+=*(cp-*p++);
                  if (n<max_no){
                    n+=*(cp-*p++);
                    if (n<max_no){
                      n+=*(cp-*p++);
                      if (n<max_no){
                        n+=*(cp-*p++);
                        if (n<max_no){
                          n+=*(cp-*p++);
                          if (n<max_no){
                            n+=*(cp-*p);
                            if (n<max_no){
                              p+=x_size-5;

                              n+=*(cp-*p++);
                              if (n<max_no){
                                n+=*(cp-*p++);
                                if (n<max_no){
                                  n+=*(cp-*p++);
                                  if (n<max_no){
                                    n+=*(cp-*p++);
                                    if (n<max_no){
                                      n+=*(cp-*p);
                                      if (n<max_no){
                                        p+=x_size-3;

                                        n+=*(cp-*p++);
                                        if (n<max_no){
                                          n+=*(cp-*p++);
                                          if (n<max_no){
                                            n+=*(cp-*p);

                                            if (n<max_no) {
                                              x=0;y=0;
                                              p=in + (i-3)*x_size + j - 1;

                                              c=*(cp-*p++);x-=c;y-=3*c;
                                              c=*(cp-*p++);y-=3*c;
                                              c=*(cp-*p);x+=c;y-=3*c;
                                              p+=x_size-3;

                                              c=*(cp-*p++);x-=2*c;y-=2*c;
                                              c=*(cp-*p++);x-=c;y-=2*c;
                                              c=*(cp-*p++);y-=2*c;
                                              c=*(cp-*p++);x+=c;y-=2*c;
                                              c=*(cp-*p);x+=2*c;y-=2*c;
                                              p+=x_size-5;

                                              c=*(cp-*p++);x-=3*c;y-=c;
                                              c=*(cp-*p++);x-=2*c;y-=c;
                                              c=*(cp-*p++);x-=c;y-=c;
                                              c=*(cp-*p++);y-=c;
                                              c=*(cp-*p++);x+=c;y-=c;
                                              c=*(cp-*p++);x+=2*c;y-=c;
                                              c=*(cp-*p);x+=3*c;y-=c;
                                              p+=x_size-6;

                                              c=*(cp-*p++);x-=3*c;
                                              c=*(cp-*p++);x-=2*c;
                                              c=*(cp-*p);x-=c;
                                              p+=2;
                                              c=*(cp-*p++);x+=c;
                                              c=*(cp-*p++);x+=2*c;
                                              c=*(cp-*p);x+=3*c;
                                              p+=x_size-6;

                                              c=*(cp-*p++);x-=3*c;y+=c;
                                              c=*(cp-*p++);x-=2*c;y+=c;
                                              c=*(cp-*p++);x-=c;y+=c;
                                              c=*(cp-*p++);y+=c;
                                              c=*(cp-*p++);x+=c;y+=c;
                                              c=*(cp-*p++);x+=2*c;y+=c;
                                              c=*(cp-*p);x+=3*c;y+=c;
                                              p+=x_size-5;

                                              c=*(cp-*p++);x-=2*c;y+=2*c;
                                              c=*(cp-*p++);x-=c;y+=2*c;
                                              c=*(cp-*p++);y+=2*c;
                                              c=*(cp-*p++);x+=c;y+=2*c;
                                              c=*(cp-*p);x+=2*c;y+=2*c;
                                              p+=x_size-3;

                                              c=*(cp-*p++);x-=c;y+=3*c;
                                              c=*(cp-*p++);y+=3*c;
                                              c=*(cp-*p);x+=c;y+=3*c;

                                              xx=x*x;
                                              yy=y*y;
                                              sq=xx+yy;
                                              if ( sq > ((n*n)/2) ) {
                                                if(yy<xx) {
                                                  divide=
                                                    (float)y/(float)abs(x);
                                                  sq=abs(x)/x;
                                                  sq=*(cp-
                                                       in[(i+round(divide))*
                                                         x_size+j+sq]) +
                                                    *(cp-
                                                      in[(i+round(2*divide))*
                                                        x_size+j+2*sq]) +
                                                    *(cp-
                                                      in[(i+round(3*divide))*
                                                        x_size+j+3*sq]);
                                                }
                                                else {
                                                  divide=(float)x/
                                                    (float)abs(y);
                                                  sq=abs(y)/y;
                                                  sq=*(cp-
                                                       in[(i+sq)*x_size+
                                                         j+round(divide)]) +
                                                    *(cp-
                                                      in[(i+2*sq)*x_size+
                                                        j+round(2*divide)]) +
                                                    *(cp-
                                                      in[(i+3*sq)*x_size+
                                                        j+round(3*divide)]);
                                                }

                                                if(sq>geometricThreshold){//290
                                                  r[i*x_size+j] = max_no-n;
                                                  cgx[i*x_size+j] = (51*x)/n;
                                                  cgy[i*x_size+j] = (51*y)/n;}
                                              }
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

    /* to locate the local maxima */
    n=0;
    for (i=5;i<y_size-5;i++)
      for (j=5;j<x_size-5;j++) {
        x = r[i*x_size+j];
        if (x>0)  {
          /* 5x5 mask */
#undef FIVE_SUPP
#ifdef FIVE_SUPP
          if (
              (x>r[(i-1)*x_size+j+2]) &&
              (x>r[(i  )*x_size+j+1]) &&
              (x>r[(i  )*x_size+j+2]) &&
              (x>r[(i+1)*x_size+j-1]) &&
              (x>r[(i+1)*x_size+j  ]) &&
              (x>r[(i+1)*x_size+j+1]) &&
              (x>r[(i+1)*x_size+j+2]) &&
              (x>r[(i+2)*x_size+j-2]) &&
              (x>r[(i+2)*x_size+j-1]) &&
              (x>r[(i+2)*x_size+j  ]) &&
              (x>r[(i+2)*x_size+j+1]) &&
              (x>r[(i+2)*x_size+j+2]) &&
              (x>=r[(i-2)*x_size+j-2]) &&
              (x>=r[(i-2)*x_size+j-1]) &&
              (x>=r[(i-2)*x_size+j  ]) &&
              (x>=r[(i-2)*x_size+j+1]) &&
              (x>=r[(i-2)*x_size+j+2]) &&
              (x>=r[(i-1)*x_size+j-2]) &&
              (x>=r[(i-1)*x_size+j-1]) &&
              (x>=r[(i-1)*x_size+j  ]) &&
              (x>=r[(i-1)*x_size+j+1]) &&
              (x>=r[(i  )*x_size+j-2]) &&
              (x>=r[(i  )*x_size+j-1]) &&
              (x>=r[(i+1)*x_size+j-2]) )
#endif
#define SEVEN_SUPP
#ifdef SEVEN_SUPP
            if (
                (x>r[(i-3)*x_size+j-3]) &&
                (x>r[(i-3)*x_size+j-2]) &&
                (x>r[(i-3)*x_size+j-1]) &&
                (x>r[(i-3)*x_size+j  ]) &&
                (x>r[(i-3)*x_size+j+1]) &&
                (x>r[(i-3)*x_size+j+2]) &&
                (x>r[(i-3)*x_size+j+3]) &&

                (x>r[(i-2)*x_size+j-3]) &&
                (x>r[(i-2)*x_size+j-2]) &&
                (x>r[(i-2)*x_size+j-1]) &&
                (x>r[(i-2)*x_size+j  ]) &&
                (x>r[(i-2)*x_size+j+1]) &&
                (x>r[(i-2)*x_size+j+2]) &&
                (x>r[(i-2)*x_size+j+3]) &&

                (x>r[(i-1)*x_size+j-3]) &&
                (x>r[(i-1)*x_size+j-2]) &&
                (x>r[(i-1)*x_size+j-1]) &&
                (x>r[(i-1)*x_size+j  ]) &&
                (x>r[(i-1)*x_size+j+1]) &&
                (x>r[(i-1)*x_size+j+2]) &&
                (x>r[(i-1)*x_size+j+3]) &&

                (x>r[(i)*x_size+j-3]) &&
                (x>r[(i)*x_size+j-2]) &&
                (x>r[(i)*x_size+j-1]) &&
                (x>=r[(i)*x_size+j+1]) &&
                (x>=r[(i)*x_size+j+2]) &&
                (x>=r[(i)*x_size+j+3]) &&

                (x>=r[(i+1)*x_size+j-3]) &&
                (x>=r[(i+1)*x_size+j-2]) &&
                (x>=r[(i+1)*x_size+j-1]) &&
                (x>=r[(i+1)*x_size+j  ]) &&
                (x>=r[(i+1)*x_size+j+1]) &&
                (x>=r[(i+1)*x_size+j+2]) &&
                (x>=r[(i+1)*x_size+j+3]) &&

                (x>=r[(i+2)*x_size+j-3]) &&
                (x>=r[(i+2)*x_size+j-2]) &&
                (x>=r[(i+2)*x_size+j-1]) &&
                (x>=r[(i+2)*x_size+j  ]) &&
                (x>=r[(i+2)*x_size+j+1]) &&
                (x>=r[(i+2)*x_size+j+2]) &&
                (x>=r[(i+2)*x_size+j+3]) &&

                (x>=r[(i+3)*x_size+j-3]) &&
                (x>=r[(i+3)*x_size+j-2]) &&
                (x>=r[(i+3)*x_size+j-1]) &&
                (x>=r[(i+3)*x_size+j  ]) &&
                (x>=r[(i+3)*x_size+j+1]) &&
                (x>=r[(i+3)*x_size+j+2]) &&
                (x>=r[(i+3)*x_size+j+3]) )
#endif
              {
                corner_list.push_back(point(j,i));
              }
        }
      }

    delete[] cgx;
    delete[] cgy;

    delete[] r;
  }
} // end namespace




