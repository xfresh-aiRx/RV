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
 * file .......: ltiQmf_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.99
 * revisions ..: $Id: ltiQmf_template.h,v 1.16 2004/05/03 12:18:39 alvarado Exp $
 */

#include <list>
#include "ltiUpsampling.h"

namespace lti {

  // ---------------------------------------------
  //   CONVOLVE CLASSES
  // ---------------------------------------------

  /*
   * set the norm
   */
  template<class T,class U>
    void lti::qmf::convolve<T,U>::setNorm(const T& theNorm) {
    norm = theNorm;
  }

  /*
   * accumulator (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  void lti::qmf::convolve<T,U>::accumulate(const T& filter,
                                           const T& src,
                                                 U& result) {
    result += static_cast<U>(filter*src);
  }

  /*
   * accumulator (specialization for ubyte elements)
   */
  template<>
  void lti::qmf::convolve<ubyte,int>::accumulate(const ubyte& filter,
                                                 const ubyte& src,
                                                 int& result);

  /*
   * get result (general implementation)
   * this should work for all floating point formats
   */
  template<class T,class U>
  T lti::qmf::convolve<T,U>::getResult(const U& result) {
    return (static_cast<T>(result));
  }

  /*
   * get result (specialization for integers)
   */
  template<>
  int lti::qmf::convolve<int,int>::getResult(const int& result);

  /*
   * get result (specialization for ubyte)
   */
  template<>
  ubyte lti::qmf::convolve<ubyte,int>::getResult(const int& result);

  /*
   * is T a floating point type?
   */
  template<class T,class U>
    lti::qmf::convolve<T,U>::convolve() {
    norm = T(1);
  }

  /*
   * is T a floating point type?
   */
  template<class T,class U>
    lti::qmf::convolve<T,U>::convolve(const kernel1D<T>& kern) {
    lpKernel.copy(kern);
    generateHighPassKernel(kern,hpKernel);
    norm = kern.getNorm();
  }

  /*
   * generate the appropriate low and high-pass kernel pair which
   * correponds to the low-pass kernel given in the parameters object.
   * @param lpk the kernel where the resulting low-pass kernel will be left.
   * @param hpk the kernel where the resulting high-pass kernel will be left.
   * The difference between the lpk the parameters.kernel is just a
   * normalization factor.
   */
  template<class T,class U> void
    lti::qmf::convolve<T,U>::generateHighPassKernel(const kernel1D<T>& src,
                                                    kernel1D<T>& hpk) const {
    int i,s;

    // the wavelet coefficients for the low-pass (h(n)) and high-pass (g(n))
    // are related with g(n) = (-1)^n h(1-n).  And the kernels are related
    // with the coefficients with hh(n) = h(-n) and gg(n) = g(-n).
    // => gg(n) = (-1)^n hh(-n-1)
    //

    hpk.resize(-1-src.lastIdx(),-1-src.firstIdx(),false,false);

    if (hpk.firstIdx() % 2 != 0) {
      s = -1;
    } else {
      s = +1;
    }

    for (i=hpk.firstIdx();i<=hpk.lastIdx();++i) {
      hpk.at(i) = s*src.at(-1-i);
      s *= -1;
    }
  }

  // ---------------------------------------------
  // 1D FILTERING
  // ---------------------------------------------

  template<class T,class U>
    lti::qmf::convolve1D<T,U>::convolve1D(const kernel1D<T>& kern)
    : lti::qmf::convolve<T,U>(kern) {
    initialize = true;
  }

  template<class T,class U>
    lti::qmf::convolve1D<T,U>::~convolve1D() {
  }

  template<class T,class U>
    void lti::qmf::convolve1D<T,U>::reinitialize() {
    initialize = true;
  }

  template<class T,class U>
  void lti::qmf::convolve1D<T,U>::apply(const vector<T>& src,
                                        const int& from,
                                        const int& to,
                                        vector<T>& dest,
                                        const qmf::parameters& param) {

    // check boundaries
    assert((from<=to) && (from>=0) && (to<src.size()));

    int x,i,f,xx,xend,srcSize2;
    static const int step = 2;

    const T *lfilterLastp,*lfilterFirstp,*lfp,*xxp;
    const T *hfilterLastp,*hfilterFirstp,*hfp;
    T *ilp,*ihp;

    U lresult,hresult;

    // need kernel as vector
    const vector<T>& lfilter = this->lpKernel;
    const vector<T>& hfilter = this->hpKernel;

    // last valid index of the filter vector
    const int lfilterLast = lfilter.lastIdx();
    const int hfilterLast = hfilter.lastIdx();

    if (initialize) {
      // boundary between the low-pass and the high-pass bands
      blockSize = to-from+1;

      // boundary is the index of the first element of the hp-band
      boundary = (blockSize/2)+(blockSize%2)+from;

      // initialize size of dest (no need for initialization!)
      if (dest.size() != src.size()) {
        dest.resize(src.size(),T(0),false,false);
      }

      lpfixed = hpfixed = false;

      // middle block filter (the whole kernel can be applied here!)
      // the low-pass kernel
      lbegin = from + ((this->lpKernel.lastIdx()<0) ? 
                       -this->lpKernel.lastIdx() : 0);
      lend   = from + ((this->lpKernel.lastIdx()>=lfilter.size()) ?
                       to+1-this->lpKernel.lastIdx() :
                       to+1-lfilter.size()+1);

      // the high-pass kernel
      hbegin = from + ((this->hpKernel.lastIdx()<0) ? 
                       -this->hpKernel.lastIdx() : 0);
      hend   = from + ((this->hpKernel.lastIdx()>=hfilter.size()) ?
                       to+1-this->hpKernel.lastIdx() :
                       to+1-hfilter.size()+1);

      // calculate if the first calculated positions are valid, or if its
      // required to fit them to the next valid position
      if (((lbegin+this->lpKernel.lastIdx()) % step)!=0) {
        lbegin += step - ((lbegin+this->lpKernel.lastIdx()) % step);
        lpfixed = true;
      }

      if (((lend+this->lpKernel.lastIdx()) % step)!=0) {
        lend -= ((lend+this->lpKernel.lastIdx()) % step);
      }

      if (((hbegin+this->hpKernel.lastIdx()) % step)!=0) {
        hbegin += step - ((hbegin+this->hpKernel.lastIdx()) % step);
        hpfixed = true;
      }

      if (((hend+this->hpKernel.lastIdx()) % step)!=0) {
        hend -= ((hend+this->hpKernel.lastIdx()) % step);
      }

      // the final middle block
      begin = max(lbegin,hbegin);
      end = min(lend,hend);

      // turn off initialization flag
      initialize = false;
    }

    // calculate indices and pointers for the filter region

    lfilterFirstp = &lfilter.at(0);
    lfilterFirstp--;  // due to the backwards counting on the filter, this
                      // is the end position!
    lfilterLastp = &lfilter.at(lfilterLast);

    hfilterFirstp = &hfilter.at(0);
    hfilterFirstp--;  // due to the backwards counting on the filter, this
                      // is the end position!
    hfilterLastp = &hfilter.at(hfilterLast);

    if (blockSize >= lfilter.size()) {
      // ----------------------------------------
      // if the kernel is smaller than the vector

      if ((lbegin%2) == (hbegin%2)) {
        // low- and high-pass bands can be calculated simultaneously!

        for (x=begin,ilp=&dest.at((this->lpKernel.lastIdx()+x)/step),
               ihp=&dest.at(boundary+(this->hpKernel.lastIdx()+x)/step);
             x<end;
             x+=step,++ilp,++ihp) {
          // x: begin in src vector
          // il: place to write local low-pass value
          // ih: place to write local high-pass value
          // lresult : local low-pass result
          // hresult : local high-pass result
          // xx: sample on image to be checked
          for (xxp=&src.at(x),lresult=U(0),hresult=U(0),
                 lfp=lfilterLastp,hfp=hfilterLastp;
               lfp!=lfilterFirstp;--lfp,--hfp,++xxp) {
            accumulate(*lfp,*xxp,lresult);
            accumulate(*hfp,*xxp,hresult);
          }
          (*ilp)=getResult(lresult);
          (*ihp)=getResult(hresult);
        }
      } else {
        // low- and high-pass bands cannot be calculated simultaneously
        for (x=lbegin,ilp=&dest.at((this->lpKernel.lastIdx()+x)/step);
             x<lend;
             x+=step,++ilp) {
          // x: begin in src vector
          // il: place to write local low-pass value
          // lresult : local low-pass result
          // xx: sample on image to be checked
          for (xxp=&src.at(x),lresult=U(0),lfp=lfilterLastp;
               lfp!=lfilterFirstp;--lfp,++xxp) {
            accumulate(*lfp,*xxp,lresult);
          }
          (*ilp)=getResult(lresult);
        }

        for (x=hbegin,ihp=&dest.at(boundary+(this->hpKernel.lastIdx()+x)/step);
             x<hend;
             x+=step,++ihp) {
          // x: begin in src vector
          // ih: place to write local high-pass value
          // lresult : local low-pass result
          // hresult : local high-pass result
          // xx: sample on image to be checked
          for (xxp=&src.at(x),hresult=U(0),hfp=hfilterLastp;
               hfp!=hfilterFirstp;--hfp,++xxp) {
            accumulate(*hfp,*xxp,hresult);
          }
          (*ihp)=getResult(hresult);
        }
      }

      // border conditions
      switch (param.boundaryType) {
        case lti::Zero:

          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,--i) {
            for (xx=x+lfilterLast,lresult=U(0),f=0;
                 ((xx>=from) && (f<lfilter.size()));
                 ++f,--xx) {
              accumulate(lfilter.at(f),src.at(xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,--i) {
            for (xx=x+hfilterLast,hresult=U(0),f=0;
                 ((xx>=from) && (f<hfilter.size()));
                 ++f,--xx) {
              accumulate(hfilter.at(f),src.at(xx),hresult);
            }
            dest.at(i)=getResult(hresult);
          }

          // right border!
          xend = to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,++i) {
            for (xx=x,lresult=U(0),f=lfilterLast;
                 ((xx<=to) && (f>=0));
                 --f,++xx) {
              accumulate(lfilter.at(f),src.at(xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          xend = to+1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,++i) {
            for (xx=x,hresult=U(0),f=hfilterLast;
                 ((xx<=to) && (f>=0));
                 --f,++xx) {
              accumulate(hfilter.at(f),src.at(xx),hresult);
            }
            dest.at(i)=getResult(hresult);
          }
          break;

        case lti::Mirror:
          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (xx=x+lfilterLast,lresult=U(0),f=0;
                 (f<lfilter.size());
                 ++f,--xx) {
              if (xx>=from)
                accumulate(lfilter.at(f),src.at(xx),lresult);
              else
                accumulate(lfilter.at(f),src.at(-xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (xx=x+hfilterLast,hresult=U(0),f=0;
                 (f<hfilter.size());
                 ++f,--xx) {
              if (xx>=from)
                accumulate(hfilter.at(f),src.at(xx),hresult);
              else
                accumulate(hfilter.at(f),src.at(-xx),hresult);
            }
            dest.at(i)=getResult(hresult);
          }

          // right border!
          srcSize2=2*to+1;
          xend = to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (xx=x,lresult=U(0),f=lfilterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<=to)
                accumulate(lfilter.at(f),src.at(xx),lresult);
              else
                accumulate(lfilter.at(f),src.at(srcSize2-xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          xend = to+1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (xx=x,hresult=U(0),f=hfilterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<=to)
                accumulate(hfilter.at(f),src.at(xx),hresult);
              else
                accumulate(hfilter.at(f),src.at(srcSize2-xx),hresult);
            }
            dest.at(i)=getResult(hresult);
          }

          break;
        case lti::Periodic:

          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (xx=x+lfilterLast,lresult=U(0),f=0;
                 (f<lfilter.size());
                 f++,xx--) {
              if (xx>=from)
                accumulate(lfilter.at(f),src.at(xx),lresult);
              else
                accumulate(lfilter.at(f),src.at(blockSize+xx),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (xx=x+hfilterLast,hresult=U(0),f=0;
                 (f<hfilter.size());
                 f++,xx--) {
              if (xx>=from)
                accumulate(hfilter.at(f),src.at(xx),hresult);
              else
                accumulate(hfilter.at(f),src.at(blockSize+xx),hresult);
            }
            dest.at(i)=getResult(hresult);
          }

          // right border!
          xend = to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (xx=x,lresult=U(0),f=lfilterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<=to)
                accumulate(lfilter.at(f),src.at(xx),lresult);
              else
                // thanks to José González Mora for this fix:
                accumulate(lfilter.at(f),src.at(xx-to-1),lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          xend = to+1-this->hpKernel.lastIdx()  - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (xx=x,hresult=U(0),f=hfilterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<=to)
                accumulate(hfilter.at(f),src.at(xx),hresult);
              else
                accumulate(hfilter.at(f),src.at(xx-to-1),hresult);
            }
            dest.at(i)=getResult(hresult);
          }
          break;

        case lti::Constant:
          T y;

          y = src.at(from);
          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (xx=x+lfilterLast,lresult=U(0),f=0;
                 (f<lfilter.size());
                 f++,xx--) {
              if (xx>=from)
                accumulate(lfilter.at(f),src.at(xx),lresult);
              else
                accumulate(lfilter.at(f),y,lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (xx=x+hfilterLast,hresult=U(0),f=0;
                 (f<hfilter.size());
                 f++,xx--) {
              if (xx>=from)
                accumulate(hfilter.at(f),src.at(xx),hresult);
              else
                accumulate(hfilter.at(f),y,hresult);
            }
            dest.at(i)=getResult(hresult);
          }

          y = src.at(to);
          // right border!
          xend = to + 1 - this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (xx=x,lresult=U(0),f=lfilterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<=to)
                accumulate(lfilter.at(f),src.at(xx),lresult);
              else
                accumulate(lfilter.at(f),y,lresult);
            }
            dest.at(i)=getResult(lresult);
          }

          xend = to + 1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (xx=x,hresult=U(0),f=hfilterLast;
                 (f>=0);
                 f--,xx++) {
              if (xx<=to)
                accumulate(hfilter.at(f),src.at(xx),hresult);
              else
                accumulate(hfilter.at(f),y,hresult);
            }
            dest.at(i)=getResult(hresult);
          }

          break;
        case lti::NoBoundary:
          break;
        default:
          throw invalidParametersException("qmf");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector

      switch (param.boundaryType) {
        case lti::Zero:

          for (x=from,i=from;x<=to;x+=step,i++) {
            for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                 f >= this->lpKernel.firstIdx(); f--,xx--) {
              if ((xx>=from) && (xx<=to)) {
                accumulate(this->lpKernel.at(f),src.at(xx),lresult);
              }
            }
            dest.at(i) = getResult(lresult);
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                 f >= this->hpKernel.firstIdx(); f--,xx--) {
              if ((xx>=from) && (xx<=to)) {
                accumulate(this->hpKernel.at(f),src.at(xx),hresult);
              }
            }
            dest.at(i) = getResult(hresult);
          }
          break;

        case lti::Mirror:

          for (x=from,i=from;x<=to;x+=step,i++) {
            for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                 f >= this->lpKernel.firstIdx(); f--,xx--) {
              if ((xx>=from) && (xx<=to)) {
                accumulate(this->lpKernel.at(f),src.at(xx),lresult);
              } else {
                int p,xp;
                p = (xx-from)/blockSize;
                xp = (xx-from)%blockSize;

                if (xx < from) {
                  p = -p + 1;
                  if (xp < 0) {
                    xp += blockSize;
                  }
                }

                if ((p & 01) != 0) {
                  xp = to-xp;
                } else {
                  xp+=from;
                }

                accumulate(this->lpKernel.at(f),src.at(xp),lresult);
              }
            }
            dest.at(i) = getResult(lresult);
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                 f >= this->hpKernel.firstIdx(); f--,xx--) {
              if ((xx>=from) && (xx<=to)) {
                accumulate(this->hpKernel.at(f),src.at(xx),hresult);
              } else {
                int p,xp;
                p = (xx-from)/blockSize;
                xp = (xx-from)%blockSize;

                if (xx < from) {
                  p = -p + 1;
                  if (xp < 0) {
                    xp += blockSize;
                  }
                }

                if ((p & 01) != 0) {
                  xp = to-xp;
                } else {
                  xp+=from;
                }

                accumulate(this->hpKernel.at(f),src.at(xp),hresult);
              }
            }
            dest.at(i) = getResult(hresult);
          }
          break;

        case lti::Periodic:
          for (x=from,i=from;x<=to;x+=step,i++) {
            for (f = this->lpKernel.lastIdx(), xx = x+f ,lresult=U(0);
                 f >= this->lpKernel.firstIdx(); f--,xx--) {
              int xp;

              xp = (xx-from)%blockSize;

              if (xp < 0) {
                xp += (blockSize+from);
              } else {
                xp += from;
              }

              accumulate(this->lpKernel.at(f),src.at(xp),lresult);
            }
            dest.at(i) = getResult(lresult);
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (f = this->hpKernel.lastIdx(), xx = x+f ,hresult=U(0);
                 f >= this->hpKernel.firstIdx(); f--,xx--) {
              int xp;

              xp = (xx-from)%blockSize;;

              if (xp < 0) {
                xp += (blockSize+from);
              } else {
                xp += from;
              }

              accumulate(this->hpKernel.at(f),src.at(xp),hresult);
            }
            dest.at(i) = getResult(hresult);
          }
          break;

        case lti::Constant:
          for (x=from,i=from;x<=to;x+=step,i++) {
            for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                 f >= this->lpKernel.firstIdx(); f--,xx--) {
              if (xx < from) {
                accumulate(this->lpKernel.at(f),src.at(from),lresult);
              } else if (xx > to) {
                accumulate(this->lpKernel.at(f),src.at(to),lresult);
              } else {
                accumulate(this->lpKernel.at(f),src.at(xx),lresult);
              }
            }
            dest.at(i) = getResult(lresult);
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                 f >= this->hpKernel.firstIdx(); f--,xx--) {
              if (xx < from) {
                accumulate(this->hpKernel.at(f),src.at(from),hresult);
              } else if (xx > to) {
                accumulate(this->hpKernel.at(f),src.at(to),hresult);
              } else {
                accumulate(this->hpKernel.at(f),src.at(xx),hresult);
              }
            }
            dest.at(i) = getResult(hresult);
          }

          break;
        case lti::NoBoundary:
          break;
        default:
          throw invalidParametersException("qmf");
      }
    }
  }

  template<class T,class U>
  void lti::qmf::convolve1D<T,U>::applyCol(const matrix<T>& src,
                                           const rectangle& area,
                                                 matrix<T>& dest,
                                           const qmf::parameters& param) {

    // check boundaries
    assert(area.isConsistent());

    int x,i,il,ih,f,xx,xend,srcSize2,col;
    static const int step = 2;

    const T *lfilterLastp,*lfilterFirstp,*lfp;
    const T *hfilterLastp,*hfilterFirstp,*hfp;

    const int from = area.ul.y;
    const int to = area.br.y;

    U lresult,hresult;

    // need kernel as vector
    const vector<T>& lfilter = this->lpKernel;
    const vector<T>& hfilter = this->hpKernel;

    // last valid index of the filter vector
    const int lfilterLast = lfilter.lastIdx();
    const int hfilterLast = hfilter.lastIdx();

    if (initialize) {
      // boundary between the low-pass and the high-pass bands
      blockSize = area.br.y-area.ul.y+1;

      // boundary is the index of the first element of the hp-band
      boundary = (blockSize/2)+(blockSize%2)+area.ul.y;

      // initialize size of dest (no need for initialization!)
      if (dest.size() != src.size()) {
        dest.resize(src.size(),T(0),false,false);
      }

      lpfixed = hpfixed = false;

      // middle block filter (the whole kernel can be applied here!)
      // the low-pass kernel
      lbegin = area.ul.y + ((this->lpKernel.lastIdx()<0) ? 
                            -this->lpKernel.lastIdx() : 0);
      lend   = area.ul.y + ((this->lpKernel.lastIdx()>=lfilter.size()) ?
                            area.br.y+1-this->lpKernel.lastIdx() :
                            area.br.y+1-lfilter.size()+1);

      // the high-pass kernel
      hbegin = area.ul.y + ((this->hpKernel.lastIdx()<0) ? 
                            -this->hpKernel.lastIdx() : 0);
      hend   = area.ul.y + ((this->hpKernel.lastIdx()>=hfilter.size()) ?
                            area.br.y+1-this->hpKernel.lastIdx() :
                            area.br.y+1-hfilter.size()+1);

      // calculate if the first calculated positions are valid, or if its
      // required to fit them to the next valid position
      if (((lbegin+this->lpKernel.lastIdx()) % step)!=0) {
        lbegin += step - ((lbegin+this->lpKernel.lastIdx()) % step);
        lpfixed = true;
      }

      if (((lend+this->lpKernel.lastIdx()) % step)!=0) {
        lend -= ((lend+this->lpKernel.lastIdx()) % step);
      }

      if (((hbegin+this->hpKernel.lastIdx()) % step)!=0) {
        hbegin += step - ((hbegin+this->hpKernel.lastIdx()) % step);
        hpfixed = true;
      }

      if (((hend+this->hpKernel.lastIdx()) % step)!=0) {
        hend -= ((hend+this->hpKernel.lastIdx()) % step);
      }

      // the final middle block
      begin = max(lbegin,hbegin);
      end = min(lend,hend);

      // turn off initialization flag
      initialize = false;
    }

    // calculate indices and pointers for the filter region

    lfilterFirstp = &lfilter.at(0);
    lfilterFirstp--;  // due to the backwards counting on the filter, this
                      // is the end position!
    lfilterLastp = &lfilter.at(lfilterLast);

    hfilterFirstp = &hfilter.at(0);
    hfilterFirstp--;  // due to the backwards counting on the filter, this
                      // is the end position!
    hfilterLastp = &hfilter.at(hfilterLast);

    if (blockSize >= lfilter.size()) {
      // ----------------------------------------
      // if the kernel is smaller than the column-vector

      if ((lbegin%2 == hbegin%2)) {
        // low- and high-pass bands can be calculated simultaneously!

        // for each column of the matrix do...
        for (x=begin,il=(this->lpKernel.lastIdx()+x)/step,
               ih=boundary+(this->hpKernel.lastIdx()+x)/step;
             x<end;
             x+=step,++il,++ih) {
          for (col=area.ul.x;col<=area.br.x;++col) {

            // x: begin in src vector
            // il: place to write local low-pass value
            // ih: place to write local high-pass value
            // lresult : local low-pass result
            // hresult : local high-pass result
            // xx: sample on image to be checked
            for (xx=x,lresult=U(0),hresult=U(0),
                   lfp=lfilterLastp,hfp=hfilterLastp;
                 lfp!=lfilterFirstp;
                 --lfp,--hfp,++xx ) {
              accumulate(*lfp,src.at(xx,col),lresult);
              accumulate(*hfp,src.at(xx,col),hresult);
            }
            dest.at(il,col)=getResult(lresult);
            dest.at(ih,col)=getResult(hresult);
          }
        }
      } else {
        // low- and high-pass bands cannot be calculated simultaneously!

        // for each column of the matrix do...
        // low-pass band
        for (x=lbegin,il=(this->lpKernel.lastIdx()+x)/step;
             x<lend;
             x+=step,++il) {
          for (col=area.ul.x;col<=area.br.x;++col) {

            // x: begin in src vector
            // il: place to write local low-pass value
            // lresult : local low-pass result
            // xx: sample on image to be checked
            for (xx=x,lresult=U(0),lfp=lfilterLastp;
                 lfp!=lfilterFirstp;
                 --lfp,++xx ) {
              accumulate(*lfp,src.at(xx,col),lresult);
            }
            dest.at(il,col)=getResult(lresult);
          }
        }

        // high-pass band
        for (x=hbegin,ih=boundary+(this->hpKernel.lastIdx()+x)/step;
             x<hend;
             x+=step,++ih) {
          for (col=area.ul.x;col<=area.br.x;++col) {

            // x: begin in src vector
            // ih: place to write local high-pass value
            // hresult : local high-pass result
            // xx: sample on image to be checked
            for (xx=x,hresult=U(0),hfp=hfilterLastp;
                 hfp!=hfilterFirstp;
                 --hfp,++xx) {
              accumulate(*hfp,src.at(xx,col),hresult);
            }
            dest.at(ih,col)=getResult(hresult);
          }
        }
      }

      // border conditions
      switch (param.boundaryType) {
        case lti::Zero:
          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+lfilterLast,lresult=U(0),f=0;
                   ((xx>=from) && (f<lfilter.size()));
                   f++,xx--) {
                accumulate(lfilter.at(f),src.at(xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+hfilterLast,hresult=U(0),f=0;
                   ((xx>=from) && (f<hfilter.size()));
                   f++,xx--) {
                accumulate(hfilter.at(f),src.at(xx,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          // right border!
          xend =to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,lresult=U(0),f=lfilterLast;
                   ((xx<=to) && (f>=0));
                   f--,xx++) {
                accumulate(lfilter.at(f),src.at(xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          xend =to+1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,hresult=U(0),f=hfilterLast;
                   ((xx<=to) && (f>=0));
                   f--,xx++) {
                accumulate(hfilter.at(f),src.at(xx,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          break;
        case lti::Mirror:
          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+lfilterLast,lresult=U(0),f=0;
                   (f<lfilter.size());
                   f++,xx--) {
                if (xx>=from)
                  accumulate(lfilter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(lfilter.at(f),src.at(-xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+hfilterLast,hresult=U(0),f=0;
                   (f<hfilter.size());
                   f++,xx--) {
                if (xx>=from)
                  accumulate(hfilter.at(f),src.at(xx,col),hresult);
                else
                  accumulate(hfilter.at(f),src.at(-xx,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          // right border!
          srcSize2=2*to+1;
          xend = to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (xx=x,lresult=U(0),f=lfilterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<=to)
                  accumulate(lfilter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(lfilter.at(f),src.at(srcSize2-xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          xend = to+1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,hresult=U(0),f=hfilterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<=to)
                  accumulate(hfilter.at(f),src.at(xx,col),hresult);
                else
                  accumulate(hfilter.at(f),src.at(srcSize2-xx,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          break;

        case lti::Periodic:
          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+lfilterLast,lresult=U(0),f=0;
                   (f<lfilter.size());
                   f++,xx--) {
                if (xx>=from)
                  accumulate(lfilter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(lfilter.at(f),src.at(blockSize+xx,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+hfilterLast,hresult=U(0),f=0;
                   (f<hfilter.size());
                   f++,xx--) {
                if (xx>=from)
                  accumulate(hfilter.at(f),src.at(xx,col),hresult);
                else
                  accumulate(hfilter.at(f),src.at(blockSize+xx,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }


          // right border!
          xend = to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,lresult=U(0),f=lfilterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<=to)
                  accumulate(lfilter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(lfilter.at(f),src.at(xx-blockSize,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          xend = to+1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,hresult=U(0),f=hfilterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<=to)
                  accumulate(hfilter.at(f),src.at(xx,col),hresult);
                else
                  accumulate(hfilter.at(f),src.at(xx-blockSize,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          break;
        case lti::Constant:
          // left border!
          for (x=lbegin-step,i=(this->lpKernel.lastIdx()+x)/step;
               x>=-this->lpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+lfilterLast,lresult=U(0),f=0;
                   (f<lfilter.size());
                   f++,xx--) {
                if (xx>=from)
                  accumulate(lfilter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(lfilter.at(f),src.at(from,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          for (x=hbegin-step,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x>=-this->hpKernel.lastIdx();
               x-=step,i--) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x+hfilterLast,hresult=U(0),f=0;
                   (f<hfilter.size());
                   f++,xx--) {
                if (xx>=from)
                  accumulate(hfilter.at(f),src.at(xx,col),hresult);
                else
                  accumulate(hfilter.at(f),src.at(from,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          // right border!
          xend = to+1-this->lpKernel.lastIdx();
          for (x=lend,i=(this->lpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,lresult=U(0),f=lfilterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<=to)
                  accumulate(lfilter.at(f),src.at(xx,col),lresult);
                else
                  accumulate(lfilter.at(f),src.at(to,col),lresult);
              }
              dest.at(i,col)=getResult(lresult);
            }
          }

          xend = to+1-this->hpKernel.lastIdx() - (boundary%2);
          for (x=hend,i=boundary+(this->hpKernel.lastIdx()+x)/step;
               x<xend;
               x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {

              for (xx=x,hresult=U(0),f=hfilterLast;
                   (f>=0);
                   f--,xx++) {
                if (xx<=to)
                  accumulate(hfilter.at(f),src.at(xx,col),hresult);
                else
                  accumulate(hfilter.at(f),src.at(to,col),hresult);
              }
              dest.at(i,col)=getResult(hresult);
            }
          }

          break;
        case lti::NoBoundary:
          break;

        default:
          throw invalidParametersException("qmf");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector

      switch (param.boundaryType) {

        case lti::Zero:
          for (x=from,i=from;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                   f >= this->lpKernel.firstIdx(); f--,xx--) {
                if ((xx>=from) && (xx<=to)) {
                  accumulate(this->lpKernel.at(f),src.at(xx,col),lresult);
                }
              }
              dest.at(i,col) = getResult(lresult);
            }
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                   f >= this->hpKernel.firstIdx(); f--,xx--) {
                if ((xx>=from) && (xx<=to)) {
                  accumulate(this->hpKernel.at(f),src.at(xx,col),hresult);
                }
              }
              dest.at(i,col) = getResult(hresult);
            }
          }

          break;

        case lti::Mirror:
          for (x=from,i=from;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                   f >= this->lpKernel.firstIdx(); f--,xx--) {
                if ((xx>=from) && (xx<=to)) {
                  accumulate(this->lpKernel.at(f),src.at(xx,col),lresult);
                } else {
                  int p,xp;
                  p = (xx-from)/blockSize;
                  xp = (xx-from)%blockSize;

                  if (xx < from) {
                    p = -p + 1;
                    if (xp < 0) {
                      xp += blockSize;
                    }
                  }

                  if ((p & 01) != 0) { // even number?
                    xp = to-xp;
                  } else {
                    xp+=from;
                  }

                  accumulate(this->lpKernel.at(f),src.at(xp,col),lresult);

                }
              }
              dest.at(i,col) = getResult(lresult);
            }
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                   f >= this->hpKernel.firstIdx(); f--,xx--) {
                if ((xx>=from) && (xx<=to)) {
                  accumulate(this->hpKernel.at(f),src.at(xx,col),hresult);
                } else {
                  int p,xp;
                  p = (xx-from)/blockSize;
                  xp = (xx-from)%blockSize;

                  if (xx < from) {
                    p = -p + 1;
                    if (xp < 0) {
                      xp += blockSize;
                    }
                  }

                  if ((p & 01) != 0) { // even number?
                    xp = to-xp;
                  } else {
                    xp+=from;
                  }

                  accumulate(this->hpKernel.at(f),src.at(xp,col),hresult);

                }
              }
              dest.at(i,col) = getResult(hresult);
            }
          }
          break;

        case lti::Periodic:

          for (x=from,i=from;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                   f >= this->lpKernel.firstIdx(); f--,xx--) {
                int xp;

                xp = (xx-from)%blockSize;
                if (xp < 0) {
                  xp+=(blockSize+from);
                } else {
                  xp += from;
                }
                accumulate(this->lpKernel.at(f),src.at(xp,col),lresult);
              }
              dest.at(i,col) = getResult(lresult);
            }
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                   f >= this->hpKernel.firstIdx(); f--,xx--) {
                int xp;

                xp = (xx-from)%blockSize;
                if (xp < 0) {
                  xp+=(blockSize+from);
                } else {
                  xp += from;
                }
                accumulate(this->hpKernel.at(f),src.at(xp,col),hresult);
              }
              dest.at(i,col) = getResult(hresult);
            }
          }

          break;

        case lti::Constant:
          for (x=from,i=from;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->lpKernel.lastIdx(), xx = x+f,lresult=U(0);
                   f >= this->lpKernel.firstIdx(); f--,xx--) {
                if (xx < from) {
                  accumulate(this->lpKernel.at(f),src.at(from,col),lresult);
                } else if (xx > to) {
                  accumulate(this->lpKernel.at(f),src.at(to,col),lresult);
                } else {
                  accumulate(this->lpKernel.at(f),src.at(xx,col),lresult);
                }
              }
              dest.at(i,col) = getResult(lresult);
            }
          }

          for (x=from,i=boundary;x<=to;x+=step,i++) {
            for (col=area.ul.x;col<=area.br.x;col++) {
              for (f = this->hpKernel.lastIdx(), xx = x+f,hresult=U(0);
                   f >= this->hpKernel.firstIdx(); f--,xx--) {
                if (xx < from) {
                  accumulate(this->hpKernel.at(f),src.at(from,col),hresult);
                } else if (xx > to) {
                  accumulate(this->hpKernel.at(f),src.at(to,col),hresult);
                } else {
                  accumulate(this->hpKernel.at(f),src.at(xx,col),hresult);
                }
              }
              dest.at(i,col) = getResult(hresult);
            }
          }
          break;
        case lti::NoBoundary:
          break;
        default:
          throw invalidParametersException("qmf");
      }
    }
  }

  template<class T,class U>
  void lti::qmf::convolve1D<T,U>::applyRow(const matrix<T>& src,
                                           const rectangle& area,
                                                 matrix<T>& dest,
                                           const qmf::parameters& param) {
    int i;

    dest.resize(src.size(),T(0),false,false);

    for (i=area.ul.y;i<=area.br.y;++i) {
      apply(src.getRow(i),area.ul.x,area.br.x,dest.getRow(i),param);
    }
  }
}

