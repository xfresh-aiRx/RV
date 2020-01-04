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
 * file .......: ltiDownsampling_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.99
 * revisions ..: $Id: ltiDownsampling_template.h,v 1.18 2004/05/03 12:18:12 alvarado Exp $
 */

#include <list>

namespace lti {

  // ---------------------------------------------
  //   CONVOLVE CLASSES
  // ---------------------------------------------

  /* set the norm */
  template<class T,class U>
  void lti::downsampling::convolve<T,U>::setNorm(const T& theNorm) {
    norm = theNorm;
  }

  /* accumulator (general implementation)
     this should work for all floating point formats
  */
  template<class T,class U>
  void lti::downsampling::convolve<T,U>::accumulate(const T& filter,
                                             const T& src,
                                                   U& result) {
    result += static_cast<U>(filter*src);
  }

  /* accumulator (specialization for ubyte elements)
   */
  template<>
  void lti::downsampling::convolve<ubyte,int>::accumulate(const ubyte& filter,
                                                          const ubyte& src,
                                                           int& result);

  /* get result (general implementation)
     this should work for all floating point formats
  */
  template<class T,class U>
  T lti::downsampling::convolve<T,U>::getResult(const U& result) {
    return (static_cast<T>(result));
  }

  /* get result (specialization for integers)
  */
  template<>
  int lti::downsampling::convolve<int,int>::getResult(const int& result);

  /* get result (specialization for ubyte)
  */
  template<>
  ubyte lti::downsampling::convolve<ubyte,int>::getResult(const int& result);

  // ---------------------------------------------
  // 1D FILTERING
  // ---------------------------------------------

  template<class T,class U>
  lti::downsampling::convolve1D<T,U>::convolve1D()
    : kernel(0),tmpKernel(0) {
  }

  template<class T,class U>
  lti::downsampling::convolve1D<T,U>::~convolve1D() {
    delete tmpKernel;
  }

  template<class T,class U>
  bool lti::downsampling::convolve1D<T,U>::setKernel(const mathObject* kern) {

    kernel = dynamic_cast<const kernel1D<T>*>(kern);

    // it is not the expected type, try to cast from know possibilities
    if (isNull(kernel)) {

      delete tmpKernel;
      tmpKernel = 0;

      // may be a kernel of integers?
      if (notNull(dynamic_cast<const ikernel1D* >(kern))) {
        tmpKernel = new kernel1D<T>;
        tmpKernel->castFrom(*dynamic_cast<const ikernel1D*>(kern));
        if (typeInfo<T>::isFloatingPointType()) {
          tmpKernel->denormalize();
        }
        kernel = tmpKernel;
      } else if (notNull(dynamic_cast<const bkernel1D*>(kern))) {
        tmpKernel = new kernel1D<T>;
        tmpKernel->castFrom(*dynamic_cast<const bkernel1D*>(kern));
        if (typeInfo<T>::isFloatingPointType()) {
          tmpKernel->denormalize();
        }
        kernel = tmpKernel;
      } else if (notNull(dynamic_cast<const fkernel1D*>(kern))) {
        tmpKernel = new kernel1D<T>;

        if (typeInfo<T>::isFloatingPointType()) {
          tmpKernel->castFrom(*dynamic_cast<const fkernel1D*>(kern));
        } else {
          fkernel1D* tmpFloat =
            new fkernel1D(*dynamic_cast<const fkernel1D*>(kern));
          tmpFloat->multiply(static_cast<float>(typeInfo<T>::suggestedNorm()));
          tmpKernel->castFrom(*tmpFloat);
          delete tmpFloat;
        }

        kernel = tmpKernel;
      } else if (notNull(dynamic_cast<const dkernel1D*>(kern))) {
        tmpKernel = new kernel1D<T>;

        if (typeInfo<T>::isFloatingPointType()) {
          tmpKernel->castFrom(*dynamic_cast<const dkernel1D*>(kern));
        } else {
          dkernel1D* tmpFloat =
            new dkernel1D(*dynamic_cast<const dkernel1D*>(kern));
          tmpFloat->multiply(static_cast<double>(typeInfo<T>::suggestedNorm()));
          tmpKernel->castFrom(*tmpFloat);
          delete tmpFloat;
        }

        kernel = tmpKernel;
      }

    }

    return (notNull(kernel));
  }

  template<class T,class U>
  void lti::downsampling::convolve1D<T,U>::apply(const vector<T>& src,
                                                       vector<T>& dest,
                                  const downsampling::parameters& param) {

    int x,i,f,xx,srcSize2;
    int begin,end;
    int newSize;
    const point& step = param.factor;

    const T *filterLastp,*filterFirstp,*fp,*xxp;
    T *ip;

    U lresult;

    // alias for kernel term
    const kernel1D<T>& kern = *kernel;

    // initialize member "norm"
    setNorm( kern.getNorm() );

    // need kernel as vector
    const vector<T>& filter = kern;

    // last valid index of the filter vector
    const int& filterLast = filter.lastIdx();

    // size of the downsampled image
    newSize = (src.size()+(step.x-1))/step.x;

    // initialize size of dest (no need for initialization!)
    if (dest.size() != newSize) {
      dest.resize(newSize,T(0),false,false);
    }

    // middle block filter (the whole kernel can be applied here!)
    begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    end   = (kern.lastIdx()>=filter.size()) ?
      src.size()-kern.lastIdx() :
      src.size()-filter.size()+1;

    if (((begin+kern.lastIdx()) % step.x)!=0) {
      begin += step.x - ((begin+kern.lastIdx()) % step.x);
    }

    if (((end+kern.lastIdx()) % step.x)!=0) {
      end -= ((end+kern.lastIdx()) % step.x);
    }

    filterFirstp = &filter.at(0);
    filterFirstp--;
    filterLastp = &filter.at(filterLast);

    if (src.size() >= filter.size()) {
      // ----------------------------------------
      // if the kernel is smaller than the vector

      for (x=begin,ip=&dest.at((kern.lastIdx()+x)/step.x);
           x<end;
           x+=step.x,ip++) {
        // x: begin in src vector
        // i: place to write local downsampling
        // lresult : local result
        // xx: sample on image to be checked
        for (xxp=&src.at(x),lresult=U(0),fp=filterLastp;
             fp!=filterFirstp;fp--,xxp++ ) {
          accumulate(*fp,*xxp,lresult);
        }
        (*ip)=getResult(lresult);
      }

      // border conditions
      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero:
          // left border!
          for (x=begin-step.x,i=(kern.lastIdx()+x)/step.x;
               x>=-kern.lastIdx();
               x-=step.x,i--) {
            for (xx=x+filterLast,lresult=U(0),f=0;
                 ((xx>=0) && (f<filter.size()));
                 f++,xx--) {
              accumulate(filter.at(f),src.at(xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          // right border!
          for (x=end,i=(kern.lastIdx()+x)/step.x;
               x<src.size()-kern.lastIdx();
               x+=step.x,i++) {
            for (xx=x,lresult=U(0),f=filterLast;
                 ((xx<src.size()) && (f>=0));
                 f--,xx++) {
              accumulate(filter.at(f),src.at(xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          break;
        case lti::Mirror:
          // left border!
          for (x=begin-step.x,i=(kern.lastIdx()+x)/step.x;
               x>=-kern.lastIdx();
               x-=step.x,i--) {
            for (xx=x+filterLast,lresult=U(0),f=0;
                 (f<filter.size());
                 f++,xx--) {
              if (xx>=0)
                accumulate(filter.at(f),src.at(xx),lresult);
              else
                accumulate(filter.at(f),src.at(-xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          // right border!
          srcSize2=2*src.size()-1;
          for (x=end,i=(kern.lastIdx()+x)/step.x;
               x<src.size()-kern.lastIdx();
               x+=step.x,i++) {
            for (xx=x,lresult=U(0),f=filterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<src.size())
                accumulate(filter.at(f),src.at(xx),lresult);
              else
                accumulate(filter.at(f),src.at(srcSize2-xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          break;
        case lti::Periodic:
          // left border!
          for (x=begin-step.x,i=(kern.lastIdx()+x)/step.x;
               x>=-kern.lastIdx();
               x-=step.x,i--) {
            for (xx=x+filterLast,lresult=U(0),f=0;
                 (f<filter.size());
                 f++,xx--) {
              if (xx>=0)
                accumulate(filter.at(f),src.at(xx),lresult);
              else
                accumulate(filter.at(f),src.at(src.size()+xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          // right border!
          for (x=end,i=(kern.lastIdx()+x)/step.x;
               x<src.size()-kern.lastIdx();
               x+=step.x,i++) {
            for (xx=x,lresult=U(0),f=filterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<src.size())
                accumulate(filter.at(f),src.at(xx),lresult);
              else
                accumulate(filter.at(f),src.at(xx-src.size()),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          break;
        case lti::Constant:
          T y;

          y = src.at(0);
          // left border!
          for (x=begin-step.x,i=(kern.lastIdx()+x)/step.x;
               x>=-kern.lastIdx();
               x-=step.x,i--) {
            for (xx=x+filterLast,lresult=U(0),f=0;
                 (f<filter.size());
                 f++,xx--) {
              if (xx>=0)
                accumulate(filter.at(f),src.at(xx),lresult);
              else
                accumulate(filter.at(f),y,lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          y = src.at(src.lastIdx());
          // right border!
          for (x=end,i=(kern.lastIdx()+x)/step.x;
               x<src.size()-kern.lastIdx();
               x+=step.x,i++) {
            for (xx=x,lresult=U(0),f=filterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<src.size())
                accumulate(filter.at(f),src.at(xx),lresult);
              else
                accumulate(filter.at(f),y,lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          break;
        default:
          throw invalidParametersException("downsampling");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector

      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero:

          for (x=0,i=0;x<src.size();x+=step.x,i++) {
            for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                 f >= kern.firstIdx(); f--,xx--) {
              if ((xx>=0) && (xx<src.size())) {
                accumulate(kern.at(f),src.at(xx),lresult);
              }
            }
            dest.at(i) = getResult(lresult);
          }

          break;
        case lti::Mirror:

          for (x=0,i=0;x<src.size();x+=step.x,i++) {
            for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                 f >= kern.firstIdx(); f--,xx--) {
              if ((xx>=0) && (xx<src.size())) {
                accumulate(kern.at(f),src.at(xx),lresult);
              } else {
                int p,xp;
                p = xx/src.size();
                xp = xx%src.size();

                if (xx < 0) {
                  p = -p + 1;
                  if (xp < 0) {
                      xp += src.size();
                  }
                }

                if ((p & 01) != 0) {
                  xp = src.size()-1-xp;
                }

                accumulate(kern.at(f),src.at(xp),lresult);

              }
            }
            dest.at(i) = getResult(lresult);
          }

          break;
        case lti::Periodic:
          for (x=0,i=0;x<src.size();x+=step.x,i++) {
            for (f = kern.lastIdx(), xx = x+f ,lresult=U(0);
                 f >= kern.firstIdx(); f--,xx--) {
              int xp;

              xp = xx%src.size();

              if (xp < 0) {
                xp += src.size();
              }

              accumulate(kern.at(f),src.at(xp),lresult);
            }
            dest.at(i) = getResult(lresult);
          }

          break;

        case lti::Constant:
          for (x=0,i=0;x<src.size();x+=step.x,i++) {
            for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                 f >= kern.firstIdx(); f--,xx--) {
              if (xx < 0) {
                accumulate(kern.at(f),src.at(0),lresult);
              } else if (xx > src.lastIdx()) {
                accumulate(kern.at(f),src.at(src.lastIdx()),lresult);
              } else {
                accumulate(kern.at(f),src.at(xx),lresult);
              }
            }
            dest.at(i) = getResult(lresult);
          }

          break;
        default:
          throw invalidParametersException("downsampling");
      }
    }
  }

  template<class T,class U>
  void lti::downsampling::convolve1D<T,U>::applyCol(const matrix<T>& src,
                                              matrix<T>& dest,
                                        const downsampling::parameters& param) {

    int x,i,f,xx,filterLast,srcSize2;
    int begin,end,col;

    point newSize;
    const point& step = param.factor;

    U lresult;

    const T *fp,*filterLastp,*filterFirstp;

    // alias for kernel term
    const kernel1D<T>& kern = *kernel;

    // initialize member "norm"
    setNorm( kern.getNorm() );

    // need kernel as vector
    const vector<T>& filter = kern;

    // last valid index of the filter vector
    filterLast = filter.lastIdx();

    // size of the downsampled image
    newSize.x = src.columns();
    newSize.y = (src.rows()+(step.y-1))/step.y;

    // initialize size of dest (not need to initialization!)
    dest.resize(newSize.y,newSize.x,T(0),false,false);

    // middle block filter (the whole kernel can be applied here!)
    begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    end   = (kern.lastIdx()>=filter.size()) ?
      src.rows()-kern.lastIdx() :
      src.rows()-filter.size()+1;

    if (((begin+kern.lastIdx()) % step.y)!=0) {
      begin += step.y - ((begin+kern.lastIdx()) % step.y);
    }

    if (((end+kern.lastIdx()) % step.y)!=0) {
      end -= ((end+kern.lastIdx()) % step.y);
    }

    filterFirstp = &filter.at(0);
    filterFirstp--;
    filterLastp = &filter.at(filterLast);

    if (src.rows() >= filter.size()) {
      // ----------------------------------------
      // if the kernel is smaller than the vector


      // for each column of the matrix do...
      for (x=begin,i=(kern.lastIdx()+x)/step.y;x<end;x+=step.y,i++) {
        for (col=0;col<src.columns();col++) {

          // x: begin in src vector
          // i: place to write local downsampling
          // lresult : local result
          // xx: sample on image to be checked
          for (xx=x,lresult=U(0),fp=filterLastp;
               fp!=filterFirstp;
               fp--,xx++ ) {
            accumulate(*fp,src.at(xx,col),lresult);
          }
          dest.at(i,col)=getResult(lresult);
        }
      }

      // border conditions
      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero:
          // left border!
          for (x=begin-step.y,i=(kern.lastIdx()+x)/step.y;
               x>=-kern.lastIdx();
               x-=step.y,i--) {
            for (col=0;col<src.columns();col++) {

              for (xx=x+filterLast,lresult=U(0),f=0;
                   ((xx>=0) && (f<filter.size()));
                   f++,xx--) {
                accumulate(filter.at(f),src.at(xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          // right border!
          for (x=end,i=(kern.lastIdx()+x)/step.y;
               x<src.rows()-kern.lastIdx();
               x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {

              for (xx=x,lresult=U(0),f=filterLast;
                   ((xx<src.rows()) && (f>=0));
                   f--,xx++) {
                accumulate(filter.at(f),src.at(xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }
          break;
        case lti::Mirror:
          // left border!
          for (x=begin-step.y,i=(kern.lastIdx()+x)/step.y;
               x>=-kern.lastIdx();
               x-=step.y,i--) {
            for (col=0;col<src.columns();col++) {

              for (xx=x+filterLast,lresult=U(0),f=0;
                   (f<filter.size());
                   f++,xx--) {
                if (xx>=0)
                  accumulate(filter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(filter.at(f),src.at(-xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          // right border!
          srcSize2=2*src.lastRow();
          for (x=end,i=(kern.lastIdx()+x)/step.y;
               x<src.rows()-kern.lastIdx();
               x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {

              for (xx=x,lresult=U(0),f=filterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<src.rows())
                  accumulate(filter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(filter.at(f),src.at(srcSize2-xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }
          break;
        case lti::Periodic:
          // left border!
          for (x=begin-step.y,i=(kern.lastIdx()+x)/step.y;
               x>=-kern.lastIdx();
               x-=step.y,i--) {
            for (col=0;col<src.columns();col++) {

              for (xx=x+filterLast,lresult=U(0),f=0;
                   (f<filter.size());
                   f++,xx--) {
                if (xx>=0)
                  accumulate(filter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(filter.at(f),src.at(src.rows()+xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }
          // right border!
          for (x=end,i=(kern.lastIdx()+x)/step.y;
               x<src.rows()-kern.lastIdx();
               x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {

              for (xx=x,lresult=U(0),f=filterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<src.rows())
                  accumulate(filter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(filter.at(f),src.at(xx-src.rows(),col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }
          break;
        case lti::Constant:
          // left border!
          for (x=begin-step.y,i=(kern.lastIdx()+x)/step.y;
               x>=-kern.lastIdx();
               x-=step.y,i--) {
            for (col=0;col<src.columns();col++) {

              for (xx=x+filterLast,lresult=U(0),f=0;
                   (f<filter.size());
                   f++,xx--) {
                if (xx>=0)
                  accumulate(filter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(filter.at(f),src.at(0,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          // right border!
          for (x=end,i=(kern.lastIdx()+x)/step.y;
               x<src.rows()-kern.lastIdx();
               x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {

              for (xx=x,lresult=U(0),f=filterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<src.rows())
                  accumulate(filter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(filter.at(f),src.at(src.lastRow(),col),
                             lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }
          break;
        default:
          throw invalidParametersException("downsampling");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector

      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero:
          for (x=0,i=0;x<src.rows();x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {
              for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                   f >= kern.firstIdx(); f--,xx--) {
                if ((xx>=0) && (xx<src.rows())) {
                  accumulate(kern.at(f),src.at(xx,col),lresult);
                }
              }
              dest.at(i,col) = getResult(lresult);
            }
          }
          break;

        case lti::Mirror:
          for (x=0,i=0;x<src.rows();x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {
              for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                   f >= kern.firstIdx(); f--,xx--) {
                if ((xx>=0) && (xx<src.rows())) {
                  accumulate(kern.at(f),src.at(xx,col),lresult);
                } else {
                  int p,xp;
                  p = xx/src.rows();
                  xp = xx&src.rows();

                  if (xx < 0) {
                    p = -p + 1;
                    if (xp < 0) {
                      xp += src.rows();
                    }
                  }

                  if ((p & 01) != 0) { // even number?
                    xp = src.rows() -1 -xp;
                  }

                  accumulate(kern.at(f),src.at(xp,col),lresult);

                }
              }
              dest.at(i,col) = getResult(lresult);
            }
          }
          break;

        case lti::Periodic:

          for (x=0,i=0;x<src.rows();x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {
              for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                   f >= kern.firstIdx(); f--,xx--) {
                int xp;

                xp = xx%src.rows();
                if (xp < 0) {
                  xp+=src.rows();
                }
                accumulate(kern.at(f),src.at(xp,col),lresult);
              }
              dest.at(i,col) = getResult(lresult);
            }
          }
          break;

        case lti::Constant:
          for (x=0,i=0;x<src.rows();x+=step.y,i++) {
            for (col=0;col<src.columns();col++) {
              for (f = kern.lastIdx(), xx = x+f,lresult=U(0);
                   f >= kern.firstIdx(); f--,xx--) {
                if (xx < 0) {
                  accumulate(kern.at(f),src.at(0,col),lresult);
                } else if (xx >= src.rows()) {
                  accumulate(kern.at(f),src.at(src.lastRow(),col),lresult);
                } else {
                  accumulate(kern.at(f),src.at(xx,col),lresult);
                }
              }
              dest.at(i,col) = getResult(lresult);
            }
          }

          break;
        default:
          throw invalidParametersException("downsampling");
      }

    }
  }

  template<class T,class U>
  void lti::downsampling::convolve1D<T,U>::applyRow(const matrix<T>& src,
                                                          matrix<T>& dest,
                                     const downsampling::parameters& param){

    int i;
    const point& step = param.factor;
    point newSize;

    newSize.x = (src.columns()+(step.x-1))/step.x;
    newSize.y = (src.rows());

    dest.resize(newSize.y,newSize.x,T(0),false,false);

    for (i=0;i<src.rows();i++) {
      apply(src.getRow(i),dest.getRow(i),param);
    }
  }



  // ---------------------------------------------
  // 2D FILTERING
  // ---------------------------------------------
  template<class T,class U>
  lti::downsampling::convolve2D<T,U>::convolve2D()
    : kernel(0),sKernel(0),tmpKernel(0),tmpSKernel(0) {
  }

  template<class T,class U>
  lti::downsampling::convolve2D<T,U>::~convolve2D() {
    delete tmpKernel;
    tmpKernel=0;
    delete tmpSKernel;
    tmpSKernel=0;
  }

  template<class T,class U>
  bool lti::downsampling::convolve2D<T,U>::setKernel(const mathObject* kern) {

    kernel = dynamic_cast<const kernel2D<T>*>(kern);
    sKernel = 0;

    // it is not the expected type, try to cast from know possibilities
    if (isNull(kernel)) {

      sKernel = dynamic_cast<const sepKernel<T>*>(kern);

      if (isNull(sKernel)) {
        // try to get what is it through the name...
        if (*(kern->getTypeName())=='k') {
          // it may be a kernel2D... try it!

          delete tmpKernel;
          tmpKernel = 0;

          // may be a kernel of integers?
          if (notNull(dynamic_cast<const ikernel2D* >(kern))) {
            tmpKernel = new kernel2D<T>;
            tmpKernel->castFrom(*dynamic_cast<const ikernel2D*>(kern));
            if (typeInfo<T>::isFloatingPointType()) {
              tmpKernel->denormalize();
            }
            kernel = tmpKernel;
          } else if (notNull(dynamic_cast<const bkernel2D*>(kern))) {
            tmpKernel = new kernel2D<T>;
            tmpKernel->castFrom(*dynamic_cast<const bkernel2D*>(kern));
            if (typeInfo<T>::isFloatingPointType()) {
              tmpKernel->denormalize();
            }
            kernel = tmpKernel;
          } else if (notNull(dynamic_cast<const fkernel2D*>(kern))) {
            tmpKernel = new kernel2D<T>;

            if (typeInfo<T>::isFloatingPointType()) {
              tmpKernel->castFrom(*dynamic_cast<const fkernel2D*>(kern));
            } else {
              fkernel2D* tmpFloat =
                new fkernel2D(*dynamic_cast<const fkernel2D*>(kern));
              tmpFloat->multiply(static_cast<float>(typeInfo<T>::suggestedNorm()));
              tmpKernel->castFrom(*tmpFloat);
              delete tmpFloat;
            }

            kernel = tmpKernel;
          } else if (notNull(dynamic_cast<const dkernel2D*>(kern))) {
            tmpKernel = new kernel2D<T>;

            if (typeInfo<T>::isFloatingPointType()) {
              tmpKernel->castFrom(*dynamic_cast<const dkernel2D*>(kern));
            } else {
              dkernel2D* tmpFloat =
                new dkernel2D(*dynamic_cast<const dkernel2D*>(kern));
              tmpFloat->multiply(static_cast<double>(typeInfo<T>::suggestedNorm()));
              tmpKernel->castFrom(*tmpFloat);
              delete tmpFloat;
            }

            kernel = tmpKernel;
          }

        } else if (*(kern->getTypeName())=='s') {
          // may be a separable kernel

          delete tmpSKernel;
          tmpSKernel = 0;

          // may be a kernel of integers?
          if (notNull(dynamic_cast<const isepKernel* >(kern))) {
            tmpSKernel = new sepKernel<T>;
            tmpSKernel->castFrom(*dynamic_cast<const isepKernel*>(kern));
            if (typeInfo<T>::isFloatingPointType()) {
              tmpSKernel->denormalize();
            }
            sKernel = tmpSKernel;
          } else if (notNull(dynamic_cast<const bsepKernel*>(kern))) {
            tmpSKernel = new sepKernel<T>;
            tmpSKernel->castFrom(*dynamic_cast<const bsepKernel*>(kern));
            if (typeInfo<T>::isFloatingPointType()) {
              tmpSKernel->denormalize();
            }
            sKernel = tmpSKernel;
          } else if (notNull(dynamic_cast<const fsepKernel*>(kern))) {
            tmpSKernel = new sepKernel<T>;

            if (typeInfo<T>::isFloatingPointType()) {
              tmpSKernel->castFrom(*dynamic_cast<const fsepKernel*>(kern));
            } else {
              fsepKernel* tmpFloat =
                new fsepKernel(*dynamic_cast<const fsepKernel*>(kern));
              tmpFloat->multiply(static_cast<float>(typeInfo<T>::suggestedNorm()));
              tmpSKernel->castFrom(*tmpFloat);
              tmpSKernel->setNorm(typeInfo<T>::suggestedNorm());
              delete tmpFloat;
            }

            sKernel = tmpSKernel;
          } else if (notNull(dynamic_cast<const dsepKernel*>(kern))) {
            tmpSKernel = new sepKernel<T>;

            if (typeInfo<T>::isFloatingPointType()) {
              tmpSKernel->castFrom(*dynamic_cast<const dsepKernel*>(kern));
            } else {
              dsepKernel* tmpFloat =
                new dsepKernel(*dynamic_cast<const dsepKernel*>(kern));
              tmpFloat->multiply(static_cast<double>(typeInfo<T>::suggestedNorm()));
              tmpSKernel->castFrom(*tmpFloat);
              delete tmpFloat;
            }

            sKernel = tmpSKernel;
          }


        }
      }
    }
    return (notNull(kernel) || notNull(sKernel));
  }

  template<class T,class U>
  bool lti::downsampling::convolve2D<T,U>::isKernelSeparable() const {
    return notNull(sKernel);
  }

  template<class T,class U>
  void lti::downsampling::convolve2D<T,U>::apply(const matrix<T>& src,
                                                       matrix<T>& dest,
                                  const downsampling::parameters& param) {

    int x,y,i,j,f,g,xx,yy,filterLastRow,filterLastCol;
    int beginRow,endRow,beginCol,endCol;
    point newSize;
    const point& step = param.factor;

    U lresult;

    // alias for kernel term
    const kernel2D<T>& kern = *kernel;

    if ((kern.columns() < 1) ||
        (kern.rows() < 1)) {
      throw functor::invalidParametersException("downsampling");
    }

    // initialize member "norm"
    this->norm = kern.getNorm();

    // need kernel as matrix
    const matrix<T>& filter = kern;
    filterLastRow = filter.lastRow();
    filterLastCol = filter.lastColumn();

    // initialize size of dest

    newSize.x = (src.columns()+(step.x-1))/step.x;
    newSize.y = (src.rows()+(step.y-1))/step.y;

    dest.resize(newSize.y,newSize.x,T(0),false,false);

    // middle block filter (the whole kernel can be applied here!)
    beginRow = (kern.lastRow()<0) ? -kern.lastRow() : 0;
    beginCol = (kern.lastColumn()<0) ? -kern.lastColumn() : 0;
    endRow   = (kern.lastRow()>=filter.rows()) ?
                 src.rows()-kern.lastRow() :
                 src.rows()-filter.rows()+1;
    endCol   = (kern.lastColumn()>=filter.columns()) ?
                 src.columns()-kern.lastColumn() :
                 src.columns()-filter.columns()+1;

    if (((beginCol+kern.lastColumn()) % step.x)!=0) {
      beginCol += step.x - ((beginCol+kern.lastColumn()) % step.x);
    }

    if (((beginRow+kern.lastRow()) % step.y)!=0) {
      beginRow += step.y - ((beginRow+kern.lastRow()) % step.y);
    }

    if (((endCol+kern.lastColumn()) % step.x)!=0) {
      endCol -= ((endCol+kern.lastColumn()) % step.x);
    }

    if (((endRow+kern.lastRow()) % step.y)!=0) {
      endRow -= ((endRow+kern.lastRow()) % step.y);
    }

    for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
      for (x=beginCol,i=kern.lastColumn()+x;x<endCol;x+=step.x,i++) {
        // x,y: begin in src vector
        // i,j: place to write local downsampling
        // lresult : local result
        // xx,yy: sample on image to be checked
        for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
          for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
            accumulate(filter.at(g,f),src.at(yy,xx),lresult);
          }
        }
        dest.at(j,i)=getResult(lresult);
      }
    }

    if ((src.rows() >= filter.rows()) &&
        (src.columns() >= filter.columns())) {
      // ----------------------------------------
      // if the kernel is smaller than the matrix

        // border conditions
      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero: {
          // left border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     f++,xx--) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // right border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     f--,xx++) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper border!
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   g++,yy--) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom border!
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;x<endCol;x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   g--,yy++) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper left
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   g++,yy--) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     f++,xx--) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper right
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {

              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   g++,yy--) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     f--,xx++) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom left
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     f++,xx--) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom right
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   g--,yy++) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     f--,xx++) {
                  accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }
        }
        break;

        case lti::Mirror: {
          // left border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  if (xx>=0)
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(yy,-xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // right border!
          int srcCol2=2*src.lastColumn();
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  if (xx<src.columns())
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(yy,srcCol2-xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper border!
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  if (yy>=0)
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(-yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom border!
          int srcRow2=2*src.lastRow();
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  if (yy<src.rows())
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(srcRow2-yy,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper left
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  accumulate(filter.at(g,f),
                             src.at((yy>=0)?yy:-yy,
                                    (xx>=0)?xx:-xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper right
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {

              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  accumulate(filter.at(g,f) ,
                             src.at((yy>=0) ? yy : -yy,
                                    (xx<src.columns()) ? xx : srcCol2-xx),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom left
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  accumulate(filter.at(g,f),
                             src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                    (xx>=0) ? xx : -xx),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom right
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  accumulate(filter.at(g,f),
                             src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                    (xx<src.columns()) ? xx : srcCol2-xx),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }
        }
        break;

        case lti::Periodic: {
          // left border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  if (xx>=0)
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),
                               src.at(yy,src.columns()+xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // right border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  if (xx<src.columns())
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),
                               src.at(yy,xx-src.columns()),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper border!
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  if (yy>=0)
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),
                               src.at(yy+src.rows(),xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom border!
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  if (yy<src.rows())
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),
                               src.at(yy-src.rows(),xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper left
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  accumulate(filter.at(g,f),
                             src.at((yy>=0) ? yy : yy+src.rows(),
                                    (xx>=0) ? xx : xx+src.columns()),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper right
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {

              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  accumulate(filter.at(g,f),
                             src.at((yy>=0)?yy:yy+src.rows(),
                                    (xx<src.columns())?xx:xx-src.columns()),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom left
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  accumulate(filter.at(g,f),
                             src.at((yy<src.rows())?yy:yy-src.rows(),
                                    (xx>=0)?xx:xx+src.columns()),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom right
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  accumulate(filter.at(g,f),
                             src.at((yy<src.rows())?yy:yy-src.rows(),
                                    (xx<src.columns())?xx:xx-src.columns()),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }
        }
        break;

        case lti::Constant: {
          // left border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  if (xx>=0)
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(yy,0),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // right border!
          for (y=beginRow,j=(kern.lastRow()+y)/step.y;y<endRow;y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;g>=0;g--,yy++) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  if (xx<src.columns())
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),
                               src.at(yy,src.lastColumn()),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper border!
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  if (yy>=0)
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(0,xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom border!
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol,i=(kern.lastColumn()+x)/step.x;
                 x<endCol;x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x,f=filterLastCol;f>=0;f--,xx++ ) {
                  if (yy<src.rows())
                    accumulate(filter.at(g,f),src.at(yy,xx),lresult);
                  else
                    accumulate(filter.at(g,f),src.at(src.lastRow(),xx),lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper left
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  accumulate(filter.at(g,f),
#if defined(_LTI_MSC_DOT_NET)
                             // MS VC++ .NET 2003 cannot compile the
                             // code below in "else"
                             src.at(max(yy,0),max(xx,0)),
#else
                             src.at( ( (yy>=0) ? yy : 0 ) ,
                                     ( (xx>=0) ? xx : 0 ) ),
#endif
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // upper right
          for (y=beginRow-step.y,j=(kern.lastRow()+y)/step.y;
               y>=-kern.lastRow();
               y-=step.y,j--) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {

              for (yy=y+filterLastRow,lresult=U(0),g=0;
                   (g<filter.rows());
                   g++,yy--) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  accumulate(filter.at(g,f),
                             src.at((yy>=0)?yy:0,
                                    (xx<src.columns())?xx:src.lastColumn()),
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom left
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=beginCol-step.x,i=(kern.lastColumn()+x)/step.x;
                 x>=-kern.lastColumn();
                 x-=step.x,i--) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     f++,xx--) {
                  accumulate(filter.at(g,f),
#if defined(_LTI_MSC_DOT_NET)
                             // MS VC++ .NET 2003 cannot compile the
                             // code below in "else"
                             src.at(min(yy,src.lastRow()),max(xx,0)),
#else
                             src.at((yy<src.rows()) ? yy : src.lastRow(),
                                    (xx>=0) ? xx : 0),
#endif
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }

          // bottom right
          for (y=endRow,j=(kern.lastRow()+y)/step.y;
               y<src.rows()-kern.lastRow();
               y+=step.y,j++) {
            for (x=endCol,i=(kern.lastColumn()+x)/step.x;
                 x<src.columns()-kern.lastColumn();
                 x+=step.x,i++) {
              for (yy=y,lresult=U(0),g=filterLastRow;
                   (g>=0);
                   g--,yy++) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     f--,xx++) {
                  accumulate(filter.at(g,f),
#if defined(_LTI_MSC_DOT_NET)
                             src.at(min(yy,src.lastRow()),min(xx,src.lastColumn())),
#else
                             src.at((yy<src.rows()) ? yy : src.lastRow(),
                                    (xx<src.columns()) ? xx : src.lastColumn()),
#endif
                             lresult);
                }
              }
              dest.at(j,i)=getResult(lresult);
            }
          }
        }
        break;

        default:
          throw functor::invalidParametersException("downsampling");
      }
    } else {

      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero:
          for (y=0,j=0;y<src.rows();y+=step.y,j++) {
            for (x=0,i=0;x<src.columns();x+=step.x,i++) {
              for (g = kern.lastRow(), yy = y+g, lresult=U(0);
                   g >=kern.firstRow();g--,yy--) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); f--,xx--) {
                  if ((xx>=0) && (xx<src.columns()) &&
                      (yy>=0) && (yy<src.rows())) {
                    accumulate(kern.at(g,f),src.at(yy,xx),lresult);
                  }
                }
              }
              dest.at(j,i) = getResult(lresult);
            }
          }
          break;

        case lti::Mirror:
          for (y=0,j=0;y<src.rows();y+=step.y,j++) {
            for (x=0,i=0;x<src.columns();x+=step.x,i++) {

              for (g = kern.lastRow(), yy = y+g, lresult=U(0);
                   g >=kern.firstRow();g--,yy--) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); f--,xx--) {

                  if ((xx>=0) && (xx<src.columns()) &&
                      (yy>=0) && (yy<src.rows())) {
                    accumulate(kern.at(g,f),src.at(yy,xx),lresult);
                  } else {
                    int px,py,xp,yp;

                    px = xx/src.columns();
                    py = yy/src.rows();

                    xp = xx % src.columns();
                    yp = yy % src.rows();

                    if (xx < 0) {
                      px = -px + 1;
                      if (xp < 0) {
                        xp += src.columns();
                      }
                    }

                    if (yy < 0) {
                      py = -py + 1;
                      if (yp < 0) {
                        yp += src.rows();
                      }
                    }

                    xp = ((px & 01) == 0) ? xp % src.columns() :
                                            src.lastColumn()-(xp%src.columns());
                    yp = ((py & 01) == 0) ? yp % src.rows() :
                                            src.lastRow()-(yp%src.rows());
                    accumulate(kern.at(g,f),src.at(yp,xp),lresult);
                  }
                }
              }
              dest.at(j,i) = getResult(lresult);
            }
          }
          break;

        case lti::Periodic:

          for (y=0,j=0;y<src.rows();y+=step.y,j++) {
            for (x=0,i=0;x<src.columns();x+=step.x,i++) {
              for (g = kern.lastRow(), yy = y+g, lresult=U(0);
                   g >=kern.firstRow();g--,yy--) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); f--,xx--) {
                  accumulate(kern.at(g,f),
                             src.at(yy%src.rows(),xx%src.columns()),lresult);
                }
              }
              dest.at(y,x) = getResult(lresult);
            }
          }

          break;

        case lti::Constant:
          for (y=0,j=0;y<src.rows();y+=step.y,j++) {
            for (x=0,i=0;x<src.columns();x+=step.x,i++) {

              for (g = kern.lastRow(), yy = y+g, lresult=U(0);
                   g >=kern.firstRow();g--,yy--) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); f--,xx--) {

                  int xp,yp;
                  yp = (yy<0) ? 0 : (yy>=src.rows()) ? src.lastRow() : yy;
                  xp = (xx<0) ? 0 : (xx>=src.rows()) ? src.lastRow() : xx;

                  accumulate(kern.at(g,f),src.at(yp,xp),lresult);
                }
              }
              dest.at(y,x) = getResult(lresult);
            }
          }

          break;
        default:
          throw functor::invalidParametersException("downsampling");
      }

    }

  }

  template<class T,class U>
  void lti::downsampling::convolve2D<T,U>::applySep(const matrix<T>& src,
                                                          matrix<T>& dest,
                                      const downsampling::parameters& param){


    // alias for kernel term
    const sepKernel<T>& kern = *sKernel;

    int i;
    matrix<T> tmpA,tmpB;

    // initialize member "norm"
    lti::downsampling::convolve1D<T,U> conv1D;

    for (i=0;i<kern.getNumberOfPairs();i++) {
      // rows filter
      conv1D.setKernel(&kern.getRowFilter(i));
      conv1D.applyRow(src,tmpA,param);

      // TODO: sometimes is better to transpose the matrix and then
      //       use applyRow again!  This should be tested!

      // columns filter
      conv1D.setKernel(&kern.getColFilter(i));
      conv1D.applyCol(tmpA,tmpB,param);

      if (i==0) {
        // the first time just transfer the result to dest
        tmpB.detach(dest);
      } else {
        // the rest separable parts should be accumulated in the result!
        dest.add(tmpB);
      }
    }
  }

}

