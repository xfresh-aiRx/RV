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
 * file .......: ltiSquareConvolution_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 13.10.2000
 * revisions ..: $Id: ltiSquareConvolution_template.h,v 1.15 2004/05/03 12:18:39 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // squareConvolution<T,A>::parameters
  // --------------------------------------------------

  // see header file (code there due to MSVC bug!)

  // --------------------------------------------------
  // squareConvolution<T,A>
  // --------------------------------------------------

  // default constructor
  template<class T,class A>
  squareConvolution<T,A>::squareConvolution()
    : filter(){
    parameters tmp;
    setParameters(tmp); // default parameters
  }

  // default constructor
  template<class T,class A>
  squareConvolution<T,A>::squareConvolution(const int k)
    : filter() {
    parameters tmp(k);
    setParameters(tmp); // default parameters
  }

  // copy constructor
  template<class T,class A>
  squareConvolution<T,A>::squareConvolution(const squareConvolution<T,A>& o)
    : filter() {
    copy(o);
  }

  // destructor
  template<class T,class A>
  squareConvolution<T,A>::~squareConvolution() {
  }

  // returns the name of this type
  template<class T,class A>
  const char* squareConvolution<T,A>::getTypeName() const {
    return "squareConvolution<T,A>";
  }

  // copy member
  template<class T,class A>
  squareConvolution<T,A>&
    squareConvolution<T,A>::copy(const squareConvolution<T,A>& other) {
      filter::copy(other);
    return (*this);
  }

  // clone member
  template<class T,class A>
  functor* squareConvolution<T,A>::clone() const {
    return new squareConvolution<T,A>(*this);
  }

  // return parameters
  template<class T,class A>
  const typename squareConvolution<T,A>::parameters&
  squareConvolution<T,A>::getParameters() const {
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

  // On place apply for type matrix<T>!
  template<class T,class A>
  bool squareConvolution<T,A>::apply(matrix<T>& srcdest) const {

    if (srcdest.empty()) {
      return true;
    }

    matrix<T> tmp;
    tmp.resize(srcdest.size(),T(),false,false);

    int y;
    for (y=0;y<srcdest.rows();y++) {
      apply(srcdest.getRow(y),tmp.getRow(y));
    }

    applyCol(tmp,srcdest);


    return true;
  };

  // On place apply for type vector<T>!
  template<class T,class A>
  bool squareConvolution<T,A>::apply(vector<T>& srcdest) const {

    if (srcdest.empty()) {
      return true;
    }

    vector<T> tmp;

    apply(srcdest,tmp);
    tmp.detach(srcdest);

    return true;
  };

  // On copy apply for type matrix<T>!
  template<class T,class A>
  bool squareConvolution<T,A>::apply(const matrix<T>& src,
                                           matrix<T>& dest) const {
    if (src.empty()) {
      return true;
    }

    matrix<T> tmp;
    tmp.resize(src.size(),T(),false,false);

    int y;
    for (y=0;y<src.rows();y++) {
      apply(src.getRow(y),tmp.getRow(y));
    }

    dest.copy(tmp);
    applyCol(tmp,dest);

    return true;
  };

  // On copy apply for type vector<T>!
  template<class T,class A>
  bool squareConvolution<T,A>::apply(const vector<T>& src,
                                           vector<T>& dest) const {

    if (src.empty()) {
      return true;
    }

    const parameters& param = getParameters();

    int x,f,xx;
    int begin,end;
    A lresult;
    A firstResult;

    // last valid index of the filter vector
    const int& filterSize = param.kernelSize.getDimensions().x;
    const int& kernLastIdx = param.kernelSize.br.x;

    // initialize size of dest (if needed)
    if (dest.size() != src.size()) {
      dest.resize(src.size(),T(0),false,false);
    }

    // if filter-kernel has dimension 1: just normalize
    if ((filterSize == 1) && (kernLastIdx == 0)) {
      if (param.kernelNorm != static_cast<T>(1)) {
        dest.divide(src,param.kernelNorm);
      } else {
        dest.copy(src);
      }

      return true;
    }

    // ----------------------------------------
    // if the kernel is smaller than the vector
    // ----------------------------------------
    if (src.size() >= filterSize) {

      // middle block filter (the whole kernel can be applied here!)
      begin = (kernLastIdx<0) ? -kernLastIdx : 0;
      end   = (kernLastIdx >= filterSize) ?
        src.size()-kernLastIdx-1 :
        src.size()-filterSize;

      // the used pointers
      const T *xp,*fp,*tp,*xxp,*endxxp,*endp,*endfp;
      T *ip,*endip;

      // This is the standard way to implement the convolution:
      //   for (x=begin,i=kern.lastIdx()+x;x<end;x++,i++) {
      //     // x: begin in src vector
      //     // i: place to write local dilation
      //     // lresult : local result
      //     // xx: sample on image to be checked
      //     for (xx=x,lresult.reset(),f=filterLast;f>=0;f--,xx++ ) {
      //       lresult.accumulate(filter.at(f),src.at(xx));
      //     }
      //     dest.at(i)=lresult.getResult();
      //   }

      endp = &src.at(0);
      endp+=end;
      endip = &dest.at(dest.lastIdx());
      endip++;

      // first lresult value
      xp=&src.at(begin);              // position at source
      ip=&dest.at(kernLastIdx+begin); // position at destination
      lresult = 0;                    // initialize accumulator

      for (xxp=xp,endxxp=xxp+filterSize;
           xxp!=endxxp;
           xxp++) {
        lresult+=(*xxp); // accumulate
      }

      firstResult = lresult;

      // store result for first pixel
      (*ip)=lresult/param.kernelNorm;

      // remember beginning!
      fp = xp;

      // next elements
      ip++;

      // the rest pixels can be calculated using only one new element!

      // The loop with pointer arithmetic (much faster this way!)
      for (;fp<endp;fp++,ip++,xxp++) {
        // x: begin in src vector
        // i: place to write local dilation
        // lresult : local result
        // xx: sample on image to be checked
        lresult-=(*fp);
        lresult+=(*xxp);
        (*ip)=lresult/param.kernelNorm;
      }

      // -----------------------
      //    border conditions
      // -----------------------

      switch (param.boundaryType) {
        case lti::NoBoundary:
          break;
        case lti::Zero:

          // right border!
          if (kernLastIdx>=-1) {
            for (;ip<endip;fp++,ip++) {
              lresult-=(*fp);
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endfp = &src.at(0);
            endfp+=src.size();
            for (;fp<endfp;fp++,ip++) {
              lresult-=(*fp);
              (*ip)=lresult/param.kernelNorm;
            }
            for (;ip<endip;ip++) {
              (*ip)=lresult/param.kernelNorm;
            }
          }

          // left border!
          xp=&src.at(begin+filterSize-1); // position at source to the last
                                          // element of the kernel region

          ip=&dest.at(kernLastIdx+begin-1); // position at destination
          lresult=firstResult;              // initialize accumulator
          endip = &dest.at(0);
          endip--;

          if (filterSize>=kernLastIdx) {
            for (;ip>endip;ip--,xp--) {
              lresult-=(*xp);
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endxxp = &src.at(0);
            endxxp--;
            for (;xp>endxxp;ip--,xp--) {
              lresult-=(*xp);
              (*ip)=lresult/param.kernelNorm;
            }
            for (;ip>endip;ip--) {
              (*ip)=lresult/param.kernelNorm;
            }

          }
          break;

        case lti::Mirror:

          // right border!

          tp = &(src.at(src.lastIdx())); // pointer to the mirrored data

          if (kernLastIdx>=-1) {
            for (;ip<endip;fp++,ip++,tp--) {
              lresult-=(*fp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endfp = &src.at(0);
            endfp+=src.size();
            for (;fp<endfp;fp++,ip++,tp--) {
              lresult-=(*fp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
            fp--;
            for (;ip<endip;fp--,ip++,tp--) {
              lresult-=(*fp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          }

          // left border!
          xp=&src.at(begin+filterSize-1); // position at source to the last
                                          // element of the kernel region

          ip=&dest.at(kernLastIdx+begin-1);  // position at destination
          lresult = firstResult;             // initialize accumulator
          endip = &dest.at(0);
          endip--;

          tp = &(src.at(0));

          if (filterSize >= kernLastIdx) {
            for (;ip>endip;ip--,xp--,tp++) {
              lresult-=(*xp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endxxp = &src.at(0);
            endxxp--;
            for (;xp>endxxp;ip--,xp--,tp++) {
              lresult-=(*xp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
            xp++;
            for (;ip>endip;ip--,xp++,tp++) {
              lresult-=(*xp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          }

          break;

        case lti::Periodic:
          // right border!

          tp = &(src.at(0)); // pointer to the periodic data

          if (kernLastIdx>=-1) {
            for (;ip<endip;fp++,ip++,tp++) {
              lresult-=(*fp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endfp = &src.at(0);
            endfp+=src.size();
            for (;fp<endfp;fp++,ip++,tp++) {
              lresult-=(*fp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
            fp = &src.at(0);
            for (;ip<endip;fp++,ip++,tp++) {
              lresult-=(*fp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          }

          // left border!
          xp=&src.at(begin+filterSize-1); // position at source to the last
                                          // element of the kernel region

          ip=&dest.at(kernLastIdx+begin-1); // position at destination
          lresult = firstResult;            // initialize accumulator
          endip = &dest.at(0);
          endip--;

          tp = &(src.at(src.lastIdx()));

          if (filterSize >= kernLastIdx) {
            for (;ip>endip;ip--,xp--,tp--) {
              lresult-=(*xp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endxxp = &src.at(0);
            endxxp--;
            for (;xp>endxxp;ip--,xp--,tp--) {
              lresult-=(*xp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
            xp = &src.at(src.lastIdx());
            for (;ip>endip;ip--,xp--,tp--) {
              lresult-=(*xp);
              lresult+=(*tp);
              (*ip)=lresult/param.kernelNorm;
            }
          }

          break;
        case lti::Constant:
          T y;

          // right border!
          y = src.at(src.lastIdx());

          if (kernLastIdx>=-1) {
            for (;ip<endip;fp++,ip++) {
              lresult-=(*fp);
              lresult+=y;
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endfp = &src.at(0);
            endfp+=src.size();
            for (;fp<endfp;fp++,ip++) {
              lresult-=(*fp);
              lresult+=y;
              (*ip)=lresult/param.kernelNorm;
            }
            for (;ip<endip;ip++) {
              (*ip)=lresult/param.kernelNorm;
            }
          }

          // left border!
          y = src.at(0);

          xp=&src.at(begin+filterSize-1); // position at source to the last
                                          // element of the kernel region

          ip=&dest.at(kernLastIdx+begin-1); // position at destination
          lresult = firstResult;            // initialize accumulator
          endip = &dest.at(0);
          endip--;

          if (filterSize >= kernLastIdx) {
            for (;ip>endip;ip--,xp--) {
              lresult-=(*xp);
              lresult+=y;
              (*ip)=lresult/param.kernelNorm;
            }
          } else {
            endxxp = &src.at(0);
            endxxp--;
            for (;xp>endxxp;ip--,xp--) {
              lresult-=(*xp);
              lresult+=y;
              (*ip)=lresult/param.kernelNorm;
            }
            for (;ip>endip;ip--,xp--) {
              (*ip)=lresult/param.kernelNorm;
            }
          }

          break;
        default:
          throw exception("unknown border type!");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector
      // ---------------------------------------

      const int& kernFirstIdx = param.kernelSize.ul.x;

      switch (param.boundaryType) {
      case lti::NoBoundary:
        break;

      case lti::Zero: {

        // initialize lresult
        const int initial = min(src.size(),-kernFirstIdx);
        lresult=0;

        for (f=max(0,-kernLastIdx-1);f<initial;++f) {
          lresult += src.at(f);
        }

        x = 0;

        // outside the vector on the left
        if (initial<0) {
          const int mi = min(-initial,dest.size());;
          for (;x<mi;++x) {
            dest.at(x) = lresult;
          }
        }

        // intervall where kernel lies on the left part of the vector
        const int lastLeft = src.lastIdx()-x;
        for (;f<=lastLeft;++x,++f) {
          lresult += src.at(f);
          dest.at(x) = lresult/param.kernelNorm;
        }

        // intervall where the kernel covers the whole vector
        const int lastX = min(src.lastIdx(),kernLastIdx);
        for (;x<=lastX;++x) {
          dest.at(x) = lresult/param.kernelNorm;
        }

        // intervall where the kernel lies on the right part of the vector
        f=max(0,-kernLastIdx-1);
        const int lastRight = src.size()-f;
        for (;x<lastRight;++x,++f) {
          lresult-=src.at(f);
          dest.at(x) = lresult/param.kernelNorm;
        }

        // the rest...
        for (;x<src.size();++x) {
          dest.at(x) = 0;
        }

      }
      break;

      case lti::Mirror: {

        // initialize lresult
        lresult=0;
        bool mirror;

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        for (f=-kernLastIdx-1;f<lastInit;++f) {
          lresult += src.at(getMirrorIndex(f,src.size(),mirror));
        }

        // the last kernel element
        int l,stepl(1),stepf(1);
        l = getMirrorIndex(lastInit,src.size(),mirror);
        if (mirror) {
          stepl=-1;
        }

        // the first kernel element
        f = getMirrorIndex((-kernLastIdx-1),src.size(),mirror);
        if (mirror) {
          stepf=-1;
        }

        // apply kernel
        for (x=0;x<src.size();++x,f+=stepf,l+=stepl) {
          if (f>=src.size()) {
            f=src.lastIdx();
            stepf = -1;
          } else if (f<0) {
            f = 0;
            stepf = +1;
          }

          if (l>=src.size()) {
            l=src.lastIdx();
            stepl = -1;
          } else if (l<0) {
            l = 0;
            stepl = +1;
          }

          lresult+=(src.at(l)-src.at(f));
          dest.at(x) = lresult/param.kernelNorm;
        }
      }


      break;
      case lti::Periodic: {
        // initialize lresult
        lresult=0;

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        for (f=-kernLastIdx-1;f<lastInit;++f) {
          xx = f%src.size();
          if (xx < 0) {
            xx += src.size();
          }
          lresult += src.at(xx);
        }

        // the last kernel element
        int l;
        l = lastInit % src.size();
        if (l<0) l+=src.size();

        // the first kernel element
        f = (-kernLastIdx-1) % src.size();
        if (f<0) f+=src.size();

        // .apply kernel
        for (x=0;x<src.size();++x,++f,++l) {
          if (f>=src.size()) f=0;
          if (l>=src.size()) l=0;
          lresult+=(src.at(l)-src.at(f));
          dest.at(x) = lresult/param.kernelNorm;
        }
      }
      break;

      case lti::Constant: {

        // initialize lresult

        const T right = src.at(src.lastIdx());
        const T left =  src.at(0);

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        const int lastInit2 = min(lastInit,src.size());
        const int lastInit0 = min(0,-kernFirstIdx);

        lresult = 0;
        for (f=-kernLastIdx-1;f<lastInit0;++f) {
          lresult += left;
        }

        for (;f<lastInit2;++f) {
          lresult += src.at(f);
        }

        for (;f<lastInit;++f) {
          lresult += right;
        }

        // the last kernel element
        int l;
        l = lastInit;

        // the first kernel element
        f = (-kernLastIdx-1);

        // apply kernel
        for (x=0;x<src.size();++x,++f,++l) {
          if (f<0) {
            lresult-=left;
          } else if (f>=src.size()) {
            lresult-=right;
          } else {
            lresult-=src.at(f);
          }

          if (l<0) {
            lresult+=left;
          } else if (l>=src.size()) {
            lresult+=right;
          } else {
            lresult+=src.at(l);
          }

          dest.at(x) = lresult/param.kernelNorm;
        }
      }


      break;

      default:
        throw exception("unknown border type");
      }
    }


    return true;
  };

  // On copy apply for type matrix<T>!
  template<class T,class A>
  matrix<T>& squareConvolution<T,A>::applyCol(const matrix<T>& src,
                                              matrix<T>& dest) const {
    const parameters& param = getParameters();

    int x,i,f,xx,endxx,endi,col,t;
    int begin,end;
    A lresult;
    A firstResult;

    // last valid index of the filter vector
    const int& filterSize = param.kernelSize.getDimensions().y;
    const int& kernLastIdx = param.kernelSize.br.y;

    // initialize size of dest (if needed)
    if (dest.size() != src.size()) {
      dest.resize(src.size(),T(0),false,false);
    }

    // This is the standard way to implement the dilation:
    //   for (x=begin,i=kern.lastIdx()+x;x<end;x++,i++) {
    //     // x: begin in src vector
    //     // i: place to write local dilation
    //     // lresult : local result
    //     // xx: sample on image to be checked
    //     for (xx=x,lresult.reset(),f=filterLast;f>=0;f--,xx++ ) {
    //       lresult.accumulate(filter.at(f),src.at(xx));
    //     }
    //     dest.at(i)=lresult.getResult();
    //   }

    if (src.rows() > filterSize) {
      // ----------------------------------------
      // if the kernel is smaller than the vector
      // ----------------------------------------

      // middle block filter (the whole kernel can be applied here!)
      begin = (kernLastIdx<0) ? -kernLastIdx : 0;
      end   = (kernLastIdx >= filterSize) ?
        src.rows()-kernLastIdx-1 :
        src.rows()-filterSize;

      // some weird kernels...
      // TODO: BUGGY this must still be fixed!
      // the best way to fix this is creating a squareConvolutionHelper,
      // which assumes a normal kernel, and an extra function which
      // converts the "weird" kernel into an equivalent normal one!!!!

      if ((begin>=src.rows()) || (end<begin)) {
        switch (param.boundaryType) {
        case lti::Zero: {
          dest.fill(T(0));
          return dest;
        }
        break;
        case lti::Mirror: {
          //TODO: redifine begin and end
        }
        break;
        case lti::Periodic: {
          begin = (begin % src.rows());
          // TODO: check end
        }
        break;
        case lti::Constant: {
          if (end>begin) {
            int idx = src.rows()-1;
            for (i=0;i<dest.rows();i++) {
              dest.getRow(i).copy(src.getRow(idx));
            }
          } else {
            for (i=0;i<dest.rows();i++) {
              dest.getRow(i).copy(src.getRow(0));
            }
          }
          return (dest);
        }
        break;
        case lti::NoBoundary: {
        }
        break;
        default:
          throw exception("unknown border type!");
        }
      }

      // first lresult value
      for (col=0;col<src.columns();col++) {

        x=begin;             // position at source
        i=kernLastIdx+begin; // position at destination
        lresult = 0;         // initialize accumulator



        for (xx=x,endxx=xx+filterSize;
             xx!=endxx;
             xx++) {
          lresult+=src.at(xx,col);   // accumulate
        }

        firstResult = lresult;

        // store result for first pixel
        dest.at(i,col)=lresult;

        // remember beginning!
        f = x;

        // next elements
        i++;

        // the rest pixels can be calculated using only one new element!

        // The loop
        for (;f<end;f++,i++) {
          // x: begin in src vector
          // i: place to write local dilation
          // lresult : local result
          // xx: sample on image to be checked
          lresult-=src.at(f,col);
          lresult+=src.at(xx,col);
          dest.at(i,col)=lresult;
          xx++;
        }

        // -----------------------
        //    border conditions
        // -----------------------
        endi = dest.rows();

        switch (param.boundaryType) {
          case lti::Zero:
            // right border!
            if (i>=f) {
              for (;i<endi;f++,i++) {
                lresult-=src.at(f,col);
                dest.at(i,col)=lresult;
              }
            } else {
              for (;f<endi;f++,i++) {
                lresult-=src.at(f,col);
                dest.at(i,col)=lresult;
              }
              for (;i<src.rows();i++) {
                dest.at(i,col)=lresult;
              }
            }
            // left border!

            x=begin+filterSize-1; // position at source to the last
                                  // element of the kernel region

            i=kernLastIdx+begin-1; // position at destination
            lresult=firstResult;   // initialize accumulator
            endi = -1;

            if (x>=i) {
              for (;i>=0;i--,x--) {
                lresult-=src.at(x,col);
                dest.at(i,col) = lresult;
              }
            } else {
              for (;x>=0;i--,x--) {
                lresult-=src.at(x,col);
                dest.at(i,col) = lresult;
              }
              for (;i>=0;i--) {
                dest.at(i,col)=lresult;
              }
            }

            break;

        case lti::Mirror:

            // right border!

            t = src.rows()-1; // pointer to the mirrored data

            if (i>=f) {
              for (;i<endi;f++,i++,t--) {
                lresult-=src.at(f,col);
                lresult+=src.at(t,col);
                dest.at(i,col)=lresult;
              }
            } else {
              for (;f<endi;f++,i++,t--) {
                lresult-=src.at(f,col);
                lresult+=src.at(t,col);
                dest.at(i,col)=lresult;
              }
              f--;
              for (;i<endi;f--,i++,t--) {
                lresult-=src.at(f,col);
                lresult+=src.at(t,col);
                dest.at(i,col)=lresult;
              }
            }

            // left border!

            x=begin+filterSize-1; // position at source to the last
                                  // element of the kernel region

            i=kernLastIdx+begin-1;  // position at destination
            lresult = firstResult;             // initialize accumulator
            endi = -1;

            t = 0;

            if (x>=i) {
              for (;i>=0;i--,x--,t++) {
                lresult-=src.at(x,col);
                lresult+=src.at(t,col);
                dest.at(i,col)=lresult;
              }
            } else {
              for (;x>=0;i--,x--,t++) {
                lresult-=src.at(x,col);
                lresult+=src.at(t,col);
                dest.at(i,col)=lresult;
              }
              x++;
              for (;i>=0;i--,x++,t++) {
                lresult-=src.at(x,col);
                lresult+=src.at(t,col);
                dest.at(i,col)=lresult;
              }
            }

          break;

        case lti::Periodic:
          // right border!

          t = 0; // pointer to the periodic data

          if (i>=f) {
            for (;i<endi;f++,i++,t++) {
              lresult-=src.at(f,col);
              lresult+=src.at(t,col);
              dest.at(i,col)=lresult;
            }
          } else {
            for (;f<endi;f++,i++,t++) {
              lresult-=src.at(f,col);
              lresult+=src.at(t,col);
              dest.at(i,col)=lresult;
            }
            f=0;
            for (;i<endi;f++,i++,t++) {
              lresult-=src.at(f,col);
              lresult+=src.at(t,col);
              dest.at(i,col)=lresult;
            }
          }

          // left border!

          x=begin+filterSize-1; // position at source to the last
                                          // element of the kernel region

          i=kernLastIdx+begin-1; // position at destination
          lresult = firstResult;          // initialize accumulator
          endi = -1;

          t = src.rows()-1;

          if (x>=i) {
            for (;i>=0;i--,x--,t--) {
              lresult-=src.at(x,col);
              lresult+=src.at(t,col);
              dest.at(i,col)=lresult;
            }
          } else {
            for (;x>=0;i--,x--,t--) {
              lresult-=src.at(x,col);
              lresult+=src.at(t,col);
              dest.at(i,col)=lresult;
            }
            x=src.rows()-1;
            for (;i>=0;i--,x--,t--) {
              lresult-=src.at(x,col);
              lresult+=src.at(t,col);
              dest.at(i,col)=lresult;
            }
          }

          break;
        case lti::Constant:
          T y;

          // right border!
          y = src.at(src.rows()-1,col);

          if (i>=f) {
            for (;i<endi;f++,i++) {
              lresult-=src.at(f,col);
              lresult+=y;
              dest.at(i,col)=lresult;
            }
          } else {
            for (;f<endi;f++,i++) {
              lresult-=src.at(f,col);
              lresult+=y;
              dest.at(i,col)=lresult;
            }
            for (;i<dest.rows();i++) {
              dest.at(i,col)=lresult;
            }
          }

          // left border!
          y = src.at(0,col);

          x=begin+filterSize-1; // position at source to the last
                                 // element of the kernel region

          i=kernLastIdx+begin-1; // position at destination
          lresult = firstResult; // initialize accumulator
          endi = -1;

          if (x>=i) {
            for (;i>=0;i--,x--) {
              lresult-=src.at(x,col);
              lresult+=y;
              dest.at(i,col)=lresult;
            }
          } else {
            for (;x>=0;i--,x--) {
              lresult-=src.at(x,col);
              lresult+=y;
              dest.at(i,col)=lresult;
            }
            for (;i>=0;i--) {
              dest.at(i,col)=lresult;
            }
          }
          break;

        case lti::NoBoundary:
          break;
        default:
          throw exception("unknown border type!");
        }
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector
      // ----------------------------------------

      const int& kernFirstIdx = param.kernelSize.ul.y;

      vector<T> lresultVct(src.columns(),T(0));

      switch (param.boundaryType) {

      case lti::Zero: {
        // initialize lresult

        const int initial = min(src.rows(),-kernFirstIdx);

        for (f=max(0,-kernLastIdx-1);f<initial;++f) {
          lresultVct.add(src.getRow(f));
        }

        x = 0;

        // outside the vector on the left
        if (initial<0) {
          const int mi = min(-initial,dest.rows());;
          for (;x<mi;++x) {
            dest.getRow(x).copy(lresultVct);
          }
        }

        // intervall where kernel lies on the left part of the vector
        const int lastLeft = src.rows()-1-x;
        for (;f<=lastLeft;++x,++f) {
          lresultVct.add(src.getRow(f));
          dest.getRow(x).copy(lresultVct);
        }

        // intervall where the kernel covers the whole vector
        const int lastX = min(src.rows()-1,kernLastIdx);
        for (;x<=lastX;++x) {
          dest.getRow(x).copy(lresultVct);
        }

        // intervall where the kernel lies on the right part of the vector
        f=max(0,-kernLastIdx-1);
        const int lastRight = src.rows()-f;
        for (;x<lastRight;++x,++f) {
          lresultVct.subtract(src.getRow(f));
          dest.getRow(x).copy(lresultVct);
        }

        // the rest...
        for (;x<src.rows();++x) {
          dest.getRow(x).fill(T(0));
        }

      }
      break;

      case lti::Mirror: {
        // initialize lresult
        bool mirror;

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        for (f=-kernLastIdx-1;f<lastInit;++f) {
          lresultVct.add(src.getRow(getMirrorIndex(f,src.rows(),mirror)));
        }

        // the last kernel element
        int l,stepl(1),stepf(1);
        l = getMirrorIndex(lastInit,src.rows(),mirror);
        if (mirror) {
          stepl=-1;
        }

        // the first kernel element
        f = getMirrorIndex((-kernLastIdx-1),src.rows(),mirror);
        if (mirror) {
          stepf=-1;
        }

        // apply kernel
        for (x=0;x<src.rows();++x,f+=stepf,l+=stepl) {
          if (f>=src.rows()) {
            f=src.rows()-1;
            stepf = -1;
          } else if (f<0) {
            f = 0;
            stepf = +1;
          }

          if (l>=src.rows()) {
            l=src.rows()-1;
            stepl = -1;
          } else if (l<0) {
            l = 0;
            stepl = +1;
          }

          lresultVct.add(src.getRow(l));
          lresultVct.subtract(src.getRow(f));
          dest.getRow(x).copy(lresultVct);
        }
      }

      break;

      case lti::Periodic: {
        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        for (f=-kernLastIdx-1;f<lastInit;++f) {
          xx = f%src.rows();
          if (xx < 0) {
            xx += src.rows();
          }
          lresultVct.add(src.getRow(xx));
        }

        // the last kernel element
        int l;
        l = lastInit % src.rows();
        if (l<0) l+=src.rows();

        // the first kernel element
        f = (-kernLastIdx-1) % src.rows();
        if (f<0) f+=src.rows();

        // .apply kernel
        for (x=0;x<src.rows();++x,++f,++l) {
          if (f>=src.rows()) f=0;
          if (l>=src.rows()) l=0;
          lresultVct.add(src.getRow(l));
          lresultVct.subtract(src.getRow(f));
          dest.getRow(x).copy(lresultVct);
        }

      }
      break;

      case lti::Constant: {

        // initialize lresult
        const vector<T>& right = src.getRow(src.rows()-1);
        const vector<T>& left =  src.getRow(0);

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        const int lastInit2 = min(lastInit,src.rows());
        const int lastInit0 = min(0,-kernFirstIdx);

        for (f=-kernLastIdx-1;f<lastInit0;++f) {
          lresultVct.add(left);
        }

        for (;f<lastInit2;++f) {
          lresultVct.add(src.getRow(f));
        }

        for (;f<lastInit;++f) {
          lresultVct.add(right);
        }

        // the last kernel element
        int l;
        l = lastInit;

        // the first kernel element
        f = (-kernLastIdx-1);

        // apply kernel
        for (x=0;x<src.rows();++x,++f,++l) {
          if (f<0) {
            lresultVct.subtract(left);
          } else if (f>=src.rows()) {
            lresultVct.subtract(right);
          } else {
            lresultVct.subtract(src.getRow(f));
          }

          if (l<0) {
            lresultVct.add(left);
          } else if (l>=src.rows()) {
            lresultVct.add(right);
          } else {
            lresultVct.add(src.getRow(l));
          }

          dest.getRow(x).copy(lresultVct);
        }

      }
      break;

      case lti::NoBoundary:
        break;

      default:
        throw exception("unknown border type");
      }

    }

    return dest;
  };

  template<class T,class A>
    inline int squareConvolution<T,A>::getMirrorIndex(const int& x,
                                                 const int& srcSize,
                                                 bool& mirror) const {
    int tmp;
    mirror = (((x/srcSize)%2) != 0);

    tmp = x % srcSize;
    if (tmp<0) {
      tmp+=srcSize;
      mirror = !mirror;
    }

    if (mirror) {
      tmp = srcSize-1-tmp;
    }

    return tmp;
  }


}

