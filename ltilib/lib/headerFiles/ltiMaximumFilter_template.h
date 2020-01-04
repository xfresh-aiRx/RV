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
 * file .......: ltiMaximumFilter_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 08.01.2002
 * revisions ..: $Id: ltiMaximumFilter_template.h,v 1.9 2004/05/06 10:19:58 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // maximumFilter<T>::parameters
  // --------------------------------------------------

  // see header file (code there due to MSVC bug!)

  // --------------------------------------------------
  // maximumFilter<T>
  // --------------------------------------------------

  // default constructor
  template<class T>
  maximumFilter<T>::maximumFilter()
    : filter(){
    rawBuffer = buffer = 0;

    parameters tmp;
    setParameters(tmp); // default parameters

  }

  // default constructor
  template<class T>
  maximumFilter<T>::maximumFilter(const parameters& par)
    : filter() {
    rawBuffer = buffer = 0;

    setParameters(par); // default parameters
  }

  // default constructor
  template<class T>
  maximumFilter<T>::maximumFilter(const int k)
    : filter() {
    rawBuffer = buffer = 0;

    parameters tmp(k);
    setParameters(tmp); // default parameters
  }

  // copy constructor
  template<class T>
  maximumFilter<T>::maximumFilter(const maximumFilter<T>& o)
    : filter() {
    rawBuffer = buffer = 0;
    copy(o);
  }

  // destructor
  template<class T>
  maximumFilter<T>::~maximumFilter() {
    delete[] rawBuffer;
    buffer = rawBuffer = 0;
  }

  // returns the name of this type
  template<class T>
  const char* maximumFilter<T>::getTypeName() const {
    return "maximumFilter<T>";
  }

  // copy member
  template<class T>
  maximumFilter<T>&
  maximumFilter<T>::copy(const maximumFilter<T>& other) {
    filter::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* maximumFilter<T>::clone() const {
    return new maximumFilter<T>(*this);
  }

  // return parameters
  template<class T>
  const typename maximumFilter<T>::parameters&
  maximumFilter<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool maximumFilter<T>::updateParameters() {
    delete[] rawBuffer;
    const parameters& mfpar = getParameters();

    // two extra elements added: one at the beginning of buffer and one
    // at the end...
    rawBuffer = new T[max(mfpar.kernelSize.getDimensions().y,
                          mfpar.kernelSize.getDimensions().x)+2];
    
    buffer = &rawBuffer[1];
    return true;
  }

  // set new parameters with the given kernel size
  template<class T>
  bool maximumFilter<T>::setKernelSize(const rectangle& ksize) {
    // we need to use clone here, because this method can be called
    // from an inherited class
    parameters* par = // get a copy of whatever these parameters are
      dynamic_cast<typename maximumFilter<T>::parameters*>(
                                                      getParameters().clone());
    if (isNull(par)) {
      return false;
    }
    par->kernelSize = ksize;                    // change just the size
    setParameters(*par);
    delete par;                                // delete the temp. object
    return true;
  }

  // set new parameters with the given kernel size
  template<class T>
  bool maximumFilter<T>::setSquareKernel(const int& ksize) {
    // we need to use clone here, because this method can be called
    // from an inherited class
    parameters* par = // get a copy of whatever these parameters are
      dynamic_cast<typename maximumFilter<T>::parameters*>(
                                                      getParameters().clone());
    if (isNull(par)) {
      return false;
    }
    par->initSquare(ksize);
    setParameters(*par);
    delete par;
    return true;
  }

  template<class T>
  void maximumFilter<T>::insert(const int size,const T elem) {
    // assume the buffer is already sorted, so we need just to insert
    // the element

    // small trick to avoid compairing the index too...
    // it assumes that rawBuffer is 1 element before buffer
    rawBuffer[0] = elem;

    int i=size-2;

    while (elem>buffer[i]) {
      buffer[i+1] = buffer[i];
      i--;
    }
    buffer[i+1] = elem;

  }

  template<class T>
  void maximumFilter<T>::insdel(const int size,
                                const T insElem,
                                const T delElem) {
    int i;

    if (insElem == delElem) {
      return;
    } else if (insElem > delElem) {
      rawBuffer[0] = insElem;

      // find the position of the delElem
      i=size-1;
      while (delElem >= buffer[i]) {
        --i;
      }
      // now i points to one element before the element to be deleted

      // shift the elements until there is place for the new insElem
      while (insElem > buffer[i]) {
        buffer[i+1] = buffer[i];
        --i;
      }
      buffer[i+1] = insElem;
    } else {
      // flag to ensure end of following loop (memory already reserved)
      buffer[size] = insElem;

      // find the position of the delElem
      i=0;
      while (delElem <= buffer[i]) {
        i++;
      }
      // now i points to one element after the element to be deleted


      // shift the elements until there is place for the new insElem
      while (insElem < buffer[i]) {
        buffer[i-1] = buffer[i];
        i++;
      }
      buffer[i-1] = insElem;
    }


  }


  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<T>!
  template<class T>
  bool maximumFilter<T>::apply(matrix<T>& srcdest) {

    matrix<T> tmp;
    tmp.resize(srcdest.size(),T(),false,false);

    int y;
    for (y=0;y<srcdest.rows();y++) {
      apply(srcdest.getRow(y),tmp.getRow(y));
    }

    return applyCol(tmp,srcdest);
  };

  // On place apply for type vector<T>!
  template<class T>
  bool maximumFilter<T>::apply(vector<T>& srcdest) {

    vector<T> tmp;

    apply(srcdest,tmp);
    tmp.detach(srcdest);

    return true;
  };

  // On copy apply for type matrix<T>!
  template<class T>
  bool maximumFilter<T>::apply(const matrix<T>& src,
                                     matrix<T>& dest) {
    matrix<T> tmp;
    tmp.resize(src.size(),T(),false,false);

    int y;
    for (y=0;y<src.rows();y++) {
      apply(src.getRow(y),tmp.getRow(y));
    }

    dest.resize(src.size(),T(),false,false);
    return applyCol(tmp,dest);
  };

  // On copy apply for type vector<T>!
  template<class T>
  bool maximumFilter<T>::apply(const vector<T>& src,
                                     vector<T>& dest) {
    const parameters& param = getParameters();

    // last valid index of the filter vector
    const int filterSize = param.kernelSize.getDimensions().x;
    const int kernLastIdx = param.kernelSize.br.x;

    // if filter-kernel has dimension 1: just normalize
    if ((filterSize == 1) && (kernLastIdx == 0)) {
      dest.copy(src);
      return true;
    }

    int i;
    int begin,end;
    vector<T> firstResult;
    firstResult.resize(filterSize,T(),false,false);

    // initialize size of dest (if needed)
    if (dest.size() != src.size()) {
      dest.resize(src.size(),T(0),false,false);
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

      // first result at buffer
      xp=&src.at(begin);              // position at source
      ip=&dest.at(kernLastIdx+begin); // position at destination

      buffer[0]=*xp;
      for (xxp=xp+1,endxxp=xp+filterSize,i=2;
           xxp!=endxxp;
           ++xxp,++i) {
        insert(i,*xxp);
      }

      // store result for first pixel
      firstResult.fill(buffer);

      (*ip)=buffer[0];

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
        insdel(filterSize,*xxp,*fp);
        (*ip)=buffer[0];
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
                insdel(filterSize,T(0),*fp);
                (*ip)=buffer[0];
              }
            } else {
              endfp = &src.at(0);
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++) {
                insdel(filterSize,T(0),*fp);
                (*ip)=buffer[0];
              }
              for (;ip<endip;ip++) {
                (*ip)=buffer[0];
              }
            }

            // left border!
            xp=&src.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1); // position at destination

            memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));

            endip = &dest.at(0);
            endip--;

            if (filterSize>=kernLastIdx) {
              for (;ip>endip;ip--,xp--) {
                insdel(filterSize,0,*xp);
                (*ip)=buffer[0];
              }
            } else {
              endxxp = &src.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,xp--) {
                insdel(filterSize,0,*xp);
                (*ip)=buffer[0];
              }
              for (;ip>endip;ip--) {
                (*ip)=buffer[0];
              }

            }
            break;

          case lti::Mirror:

            // right border!

            tp = &(src.at(src.lastIdx())); // pointer to the mirrored data

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer[0];
              }
            } else {
              endfp = &src.at(0);
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer[0];
              }
              fp--;
              for (;ip<endip;fp--,ip++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer[0];
              }
            }

            // left border!
            xp=&src.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1);  // position at destination
            memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));

            endip = &dest.at(0);
            endip--;

            tp = &(src.at(0));

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,xp--,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer[0];
              }
            } else {
              endxxp = &src.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,xp--,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer[0];
              }
              xp++;
              for (;ip>endip;ip--,xp++,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer[0];
              }
            }

            break;

          case lti::Periodic:
            // right border!

            tp = &(src.at(0)); // pointer to the periodic data

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer[0];
              }
            } else {
              endfp = &src.at(0);
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer[0];
              }
              fp = &src.at(0);
              for (;ip<endip;fp++,ip++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=buffer[0];
              }
            }

            // left border!
            xp=&src.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1); // position at destination
            memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));
            endip = &dest.at(0);
            endip--;

            tp = &(src.at(src.lastIdx()));

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer[0];
              }
            } else {
              endxxp = &src.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer[0];
              }
              xp = &src.at(src.lastIdx());
              for (;ip>endip;ip--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=buffer[0];
              }
            }

            break;
          case lti::Constant:
            T y;

            // right border!
            y = src.at(src.lastIdx());

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++) {
                insdel(filterSize,y,*fp);
                (*ip)=buffer[0];
              }
            } else {
              endfp = &src.at(0);
              endfp+=src.size();
              for (;fp<endfp;fp++,ip++) {
                insdel(filterSize,y,*fp);
                (*ip)=buffer[0];
              }
              for (;ip<endip;ip++) {
                (*ip)=buffer[0];
              }
            }

            // left border!
            y = src.at(0);

            xp=&src.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1); // position at destination
            memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));
            endip = &dest.at(0);
            endip--;

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,xp--) {
                insdel(filterSize,y,*xp);
                (*ip)=buffer[0];
              }
            } else {
              endxxp = &src.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,xp--) {
                insdel(filterSize,y,*xp);
                (*ip)=buffer[0];
              }
              for (;ip>endip;ip--,xp--) {
                (*ip)=buffer[0];
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
      int f,x;

      if ((param.boundaryType == lti::Mirror) ||
          (param.boundaryType == lti::Periodic)) {
        // all elements are equal the maximum!
        dest.fill(src.maximum());

      } else if ((param.boundaryType == lti::Zero) ||
                 (param.boundaryType == lti::Constant)) {

        // initialize lresult

        T right = src.at(src.lastIdx());
        T left =  src.at(0);

        if (param.boundaryType == lti::Zero) {
          right = T(0);
          left =  T(0);
        } else {
          right = src.at(src.lastIdx());
          left =  src.at(0);
        }

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        const int lastInit2 = min(lastInit,src.size());
        const int lastInit0 = min(0,-kernFirstIdx);

        i=1;

        for (f=-kernLastIdx-1;f<lastInit0;++f) {
          insert(i,left);
          ++i;
        }

        for (;f<lastInit2;++f) {
          insert(i,src.at(f));
          ++i;
        }

        for (;f<lastInit;++f) {
          insert(i,right);
          ++i;
        }

        // the last kernel element
        int l;
        l = lastInit;

        // the first kernel element
        f = (-kernLastIdx-1);

        T ins,del;

        // apply kernel
        for (x=0;x<src.size();++x,++f,++l) {

          if (f<0) {
            del=left;
          } else if (f>=src.size()) {
            del=right;
          } else {
            del=src.at(f);
          }

          if (l<0) {
            ins=left;
          } else if (l>=src.size()) {
            ins=right;
          } else {
            ins=src.at(l);
          }

          insdel(filterSize,ins,del);

          dest.at(x) = buffer[0];
        }
      }
    }

    return true;
  };

  // On copy apply for type matrix<T>!
  template<class T>
  bool maximumFilter<T>::applyCol(const matrix<T>& src,
                                        matrix<T>& dest) {
    const parameters& param = getParameters();

    // last valid index of the filter vector
    const int& filterSize = param.kernelSize.getDimensions().y;
    const int& kernLastIdx = param.kernelSize.br.y;

    // if filter-kernel has dimension 1: just normalize
    if ((filterSize == 1) && (kernLastIdx == 0)) {
      dest.copy(src);
      return true;
    }

    int x,i,f,xx,endxx,endi,col,t,s;
    int begin,end;

    vector<T> firstResult;
    firstResult.resize(filterSize,T(),false,false);

    // initialize size of dest (if needed)
    if (dest.size() != src.size()) {
      dest.resize(src.size(),T(0),false,false);
    }

    // This is the standard way to implement the convolution
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
      // the best way to fix this is creating a maximumFilterHelper,
      // which assumes a normal kernel, and an extra function which
      // converts the "weird" kernel into an equivalent normal one!!!!

      if ((begin>=src.rows()) || (end<begin)) {
        switch (param.boundaryType) {
            case lti::Zero: {
              dest.fill(T(0));
              return true;
            }
            break;
            case lti::Mirror: {
              //TODO: redefine begin and end
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
              return true;
            }
            break;
            case lti::NoBoundary: {
            }
            break;
          default: {
            setStatusString("unknown border type!");
            return false;
          }
        }
      }

      // first lresult value
      for (col=0;col<src.columns();col++) {

        x=begin;             // position at source
        i=kernLastIdx+begin; // position at destination

        buffer[0]=src.at(x,col);
        for (xx=x+1,endxx=x+filterSize,s=2;
             xx!=endxx;
             ++xx,++s) {
          insert(s,src.at(xx,col));   // accumulate
        }

        // store result for first pixel
        firstResult.fill(buffer);

        // store result for first pixel
        dest.at(i,col)=buffer[0];

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
          insdel(filterSize,src.at(xx,col),src.at(f,col));
          dest.at(i,col)=buffer[0];
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
                  insdel(filterSize,0,src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;f<endi;f++,i++) {
                  insdel(filterSize,0,src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
                for (;i<src.rows();i++) {
                  dest.at(i,col)=buffer[0];
                }
              }
              // left border!

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1; // position at destination
              memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));

              endi = -1;

              if (x>=i) {
                for (;i>=0;i--,x--) {
                  insdel(filterSize,0,src.at(x,col));
                  dest.at(i,col) = buffer[0];
                }
              } else {
                for (;x>=0;i--,x--) {
                  insdel(filterSize,0,src.at(x,col));
                  dest.at(i,col) = buffer[0];
                }
                for (;i>=0;i--) {
                  dest.at(i,col)=buffer[0];
                }
              }

              break;

            case lti::Mirror:

              // right border!

              t = src.rows()-1; // pointer to the mirrored data

              if (i>=f) {
                for (;i<endi;f++,i++,t--) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;f<endi;f++,i++,t--) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
                f--;
                for (;i<endi;f--,i++,t--) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
              }

              // left border!

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1;  // position at destination
              memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));

              endi = -1;

              t = 0;

              if (x>=i) {
                for (;i>=0;i--,x--,t++) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;x>=0;i--,x--,t++) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
                x++;
                for (;i>=0;i--,x++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
              }

              break;

            case lti::Periodic:
              // right border!

              t = 0; // pointer to the periodic data

              if (i>=f) {
                for (;i<endi;f++,i++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;f<endi;f++,i++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
                f=0;
                for (;i<endi;f++,i++,t++) {
                  insdel(filterSize,src.at(t,col),src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
              }

              // left border!

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1; // position at destination
              memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));
              endi = -1;

              t = src.rows()-1;

              if (x>=i) {
                for (;i>=0;i--,x--,t--) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;x>=0;i--,x--,t--) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
                x=src.rows()-1;
                for (;i>=0;i--,x--,t--) {
                  insdel(filterSize,src.at(t,col),src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
              }

              break;
            case lti::Constant:
              T y;

              // right border!
              y = src.at(src.rows()-1,col);

              if (i>=f) {
                for (;i<endi;f++,i++) {
                  insdel(filterSize,y,src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;f<endi;f++,i++) {
                  insdel(filterSize,y,src.at(f,col));
                  dest.at(i,col)=buffer[0];
                }
                for (;i<dest.rows();i++) {
                  dest.at(i,col)=buffer[0];
                }
              }

              // left border!
              y = src.at(0,col);

              x=begin+filterSize-1; // position at source to the last
              // element of the kernel region

              i=kernLastIdx+begin-1; // position at destination
              memcpy(buffer,&firstResult.at(0),filterSize*sizeof(T));
              endi = -1;

              if (x>=i) {
                for (;i>=0;i--,x--) {
                  insdel(filterSize,y,src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
              } else {
                for (;x>=0;i--,x--) {
                  insdel(filterSize,y,src.at(x,col));
                  dest.at(i,col)=buffer[0];
                }
                for (;i>=0;i--) {
                  dest.at(i,col)=buffer[0];
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

      // compute the maximum of each column

      const int& kernFirstIdx = param.kernelSize.ul.x;

      if ((param.boundaryType == lti::Mirror) ||
          (param.boundaryType == lti::Periodic)) {

        // all elements are equal the maximum!
        for (col=0;col<src.columns();++col) {
          T tmp = src.at(0,col);
          for (f=1;f<src.rows();++f) {
            tmp = max(tmp,src.at(f,col));
          }

          for (f=0;f<dest.rows();++f) {
            dest.at(f,col) = tmp;
          }
        }

      } else if ((param.boundaryType == lti::Zero) ||
                 (param.boundaryType == lti::Constant)) {

        // initialize lresult
        for (col=0;col<src.columns();++col) {

          T right = src.at(src.rows()-1,col);
          T left =  src.at(0,col);

          if (param.boundaryType == lti::Zero) {
            right = T(0);
            left =  T(0);
          } else {
            right = src.at(src.rows()-1,col);
            left =  src.at(0,col);
          }

          // initialize the lresult with the kernel at(-1)
          const int lastInit = -kernFirstIdx;
          const int lastInit2 = min(lastInit,src.rows());
          const int lastInit0 = min(0,-kernFirstIdx);

          i=1;

          for (f=-kernLastIdx-1;f<lastInit0;++f) {
            insert(i,left);
            ++i;
          }

          for (;f<lastInit2;++f) {
            insert(i,src.at(f,col));
            ++i;
          }

          for (;f<lastInit;++f) {
            insert(i,right);
            ++i;
          }

          // the last kernel element
          int l;
          l = lastInit;

          // the first kernel element
          f = (-kernLastIdx-1);

          T ins,del;

          // apply kernel
          for (x=0;x<src.rows();++x,++f,++l) {

            if (f<0) {
              del=left;
            } else if (f>=src.rows()) {
              del=right;
            } else {
              del=src.at(f,col);
            }

            if (l<0) {
              ins=left;
            } else if (l>=src.rows()) {
              ins=right;
            } else {
              ins=src.at(l,col);
            }

            insdel(filterSize,ins,del);

            dest.at(x,col) = buffer[0];
          }
        }
      }
    }

    return true;
  };

  template<class T>
  inline int maximumFilter<T>::getMirrorIndex(const int& x,
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


