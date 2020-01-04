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
 * file .......: ltiLocalMaxima_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 1.11.2002
 * revisions ..: $Id: ltiLocalMaxima_template.h,v 1.13 2004/11/12 13:19:48 hannemann Exp $
 */

// #define _LTI_DEBUG 2

#include <limits>
#include <list>

#include "ltiSort.h"

#ifdef _LTI_DEBUG
#include "ltiViewer.h"
#include <iostream>
#endif

namespace lti {
  // --------------------------------------------------
  // localMaxima<T>::parameters
  // --------------------------------------------------

  // implementation of parameters in header file due to MS-VC++ bug

  // static members
  template<class T>
  const T localMaxima<T>::minimumT = std::numeric_limits<T>::min();

  // --------------------------------------------------
  // localMaxima
  // --------------------------------------------------



  // default constructor
  template <class T>
  localMaxima<T>::localMaxima() : maximumFilter<T>() {

    parameters param;
    setParameters(param);
  }

  // default constructor
  template <class T>
  localMaxima<T>::localMaxima(const parameters& par)
    : maximumFilter<T>() {
    setParameters(par);
  }


  // copy constructor
  template <class T>
  localMaxima<T>::localMaxima(const localMaxima& other)  : maximumFilter<T>() {
    copy(other);
  }

  // destructor
  template <class T>
  localMaxima<T>::~localMaxima() {
  }

  // returns the name of this type
  template <class T>
  const char* localMaxima<T>::getTypeName() const {
    return "localMaxima";
  }

  // copy member
  template <class T>
  localMaxima<T>&
    localMaxima<T>::copy(const localMaxima<T>& other) {
      maximumFilter<T>::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  template <class T>
  localMaxima<T>&
  localMaxima<T>::operator=(const localMaxima<T>& other) {
    return (copy(other));
  }


  // clone member
  template <class T>
  functor* localMaxima<T>::clone() const {
    return new localMaxima(*this);
  }

  // return parameters
  template <class T>
  const typename localMaxima<T>::parameters&
    localMaxima<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *par;
  }

  // set parameters
  template<class T>
  bool localMaxima<T>::updateParameters() {
    suppressValue = getParameters().noMaxValue;
    maximumFilter<T>::updateParameters();
    return true;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  // On place apply for type channel8!
  template <class T>
  bool localMaxima<T>::apply(matrix<T>& srcdest) {
    matrix<T> tmp;
    tmp.resize(srcdest.size(),T(),false,false);

    if (applyCol(srcdest,tmp)) {
      int y;
      for (y=0;y<srcdest.rows();y++) {
        apply(tmp.getRow(y),srcdest.getRow(y));
      }
      return true;
    }
    return false;
  };

  // On place apply for type vector<ubyte>!
  template <class T>
  bool localMaxima<T>::apply(vector<T>& srcdest) {

    vector<T> tmp;

    apply(srcdest,tmp);
    tmp.detach(srcdest);

    return true;
  };

  // On copy apply for type matrix<T>!
  template <class T>
  bool localMaxima<T>::apply(const matrix<T>& src,
                                   pointList& theMaxs) {
    matrix<T> dest;
    return apply(src,dest,theMaxs);
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool localMaxima<T>::apply(const matrix<T>& src,
                                   matrix<T>& dest) {
    pointList theMaxs;
    return apply(src,dest,theMaxs);
  }

  // On copy apply for type matrix<T>!
  template <class T>
  bool localMaxima<T>::apply(const matrix<T>& src,
                                   matrix<T>& dest,
                                   pointList& theMaxs) {


    matrix<T> tmp;
    tmp.resize(src.size(),T(),false,false);
    dest.resize(src.size(),T(),false,false);

    if (applyCol(src,tmp)) {

      int y;
      for (y=0;y<src.rows();y++) {
        apply(src.getRow(y),tmp.getRow(y),dest.getRow(y));
      }

      return hystheresisSuppression(src,dest,theMaxs);
    }

    return false;
  };

  template <class T>
  inline T localMaxima<T>::getValue(const T srcVal) const {
    // the given value is a local maximum if and only if it is also
    // greater than the second biggest value.  Otherwise is just another
    // non-maximum and must be suppressed.
    if ((srcVal >= this->buffer[0]) &&
        (srcVal >  this->buffer[1])) {
      return (srcVal);
    }
    return minimumT;
  }

  // On copy apply for type vector<ubyte>!
  template <class T>
  bool localMaxima<T>::apply(const vector<T>& src,
                                   vector<T>& dest) {
    const parameters& param = getParameters();
    if (apply(src,src,dest)) {
      // search for global maxima
      typename vector<T>::iterator it,eit;
      eit=dest.end();
      T theMax = minimumT;
      if ((param.relativeMinimum > T(0)) &&
          (param.relativeMinimum <= 1.0)) {
        for (it=dest.begin();it!=eit;++it) {
          theMax = max(theMax,*it);
        }
      } else {
        theMax = T(0);
      }

      const T thresh = static_cast<T>(theMax*param.relativeMinimum);

      for (it=dest.begin();it!=eit;++it) {
        if (*it < thresh) {
          *it = suppressValue;
        }
      }

      return true;
    }

    return false;
  }


  // On copy apply for type vector<ubyte>!
  template <class T>
  bool localMaxima<T>::apply(const vector<T>& src,
                             const vector<T>& maxRes,
                                   vector<T>& dest) {
    const parameters& param = getParameters();

    // last valid index of the filter vector
    const int filterSize = param.kernelSize.getDimensions().x;
    const int kernLastIdx = param.kernelSize.br.x;

    // if filter-kernel has dimension 1: just normalize
    if ((filterSize == 1) && (kernLastIdx == 0)) {
      dest.copy(maxRes);
      return true;
    }

    int i;
    int begin,end;
    vector<T> firstResult;
    firstResult.resize(filterSize,T(),false,false);

    // initialize size of dest (if needed)
    if (dest.size() != maxRes.size()) {
      dest.resize(maxRes.size(),T(0),false,false);
    }

    // ----------------------------------------
    // if the kernel is smaller than the vector
    // ----------------------------------------
    if (maxRes.size() >= filterSize) {

      // middle block filter (the whole kernel can be applied here!)
      begin = (kernLastIdx<0) ? -kernLastIdx : 0;
      end   = (kernLastIdx >= filterSize) ?
        maxRes.size()-kernLastIdx-1 :
        maxRes.size()-filterSize;

      // the used pointers
      const T *xp,*fp,*tp,*xxp,*endxxp,*endp,*endfp;
      T *ip,*endip;
      const T* op;

      // This is the standard way to implement the convolution:
      //   for (x=begin,i=kern.lastIdx()+x;x<end;x++,i++) {
      //     // x: begin in maxRes vector
      //     // i: place to write local dilation
      //     // lresult : local result
      //     // xx: sample on image to be checked
      //     for (xx=x,lresult.reset(),f=filterLast;f>=0;f--,xx++ ) {
      //       lresult.accumulate(filter.at(f),maxRes.at(xx));
      //     }
      //     dest.at(i)=lresult.getResult();
      //   }

      endp = &maxRes.at(0);
      endp+=end;
      endip = &dest.at(dest.lastIdx());
      endip++;

      // first result at buffer
      xp=&maxRes.at(begin);              // position at source
      ip=&dest.at(kernLastIdx+begin); // position at destination
      op=&src.at(kernLastIdx+begin); // position at destination

      this->buffer[0]=*xp;
      for (xxp=xp+1,endxxp=xp+filterSize,i=2;
           xxp!=endxxp;
           ++xxp,++i) {
        insert(i,*xxp);
      }

      // store result for first pixel
      firstResult.fill(this->buffer);

      (*ip)=getValue(*op);

      // remember beginning!
      fp = xp;

      // next elements
      ip++;
      op++;

      // the rest pixels can be calculated using only one new element!

      // The loop with pointer arithmetic (much faster this way!)
      for (;fp<endp;fp++,ip++,op++,xxp++) {
        // x: begin in maxRes vector
        // i: place to write local dilation
        // lresult : local result
        // xx: sample on image to be checked
        insdel(filterSize,*xxp,*fp);
        (*ip)=getValue(*op);
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
              for (;ip<endip;fp++,ip++,op++) {
                insdel(filterSize,T(0),*fp);
                (*ip)=getValue(*op);
              }
            } else {
              endfp = &maxRes.at(0);
              endfp+=maxRes.size();
              for (;fp<endfp;fp++,ip++,op++) {
                insdel(filterSize,T(0),*fp);
                (*ip)=getValue(*op);
              }
              for (;ip<endip;ip++,op++) {
                (*ip)=getValue(*op);
              }
            }

            // left border!
            xp=&maxRes.at(begin+filterSize-1); // position at source to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1); // position at destination
            op=&src.at(kernLastIdx+begin-1); // position at destination

            memcpy(this->buffer,&firstResult.at(0),filterSize*sizeof(T));

            endip = &dest.at(0);
            endip--;

            if (filterSize>=kernLastIdx) {
              for (;ip>endip;ip--,op--,xp--) {
                insdel(filterSize,0,*xp);
                (*ip)=getValue(*op);
              }
            } else {
              endxxp = &maxRes.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,op--,xp--) {
                insdel(filterSize,0,*xp);
                (*ip)=getValue(*op);
              }
              for (;ip>endip;ip--,op--) {
                (*ip)=getValue(*op);
              }

            }
            break;

          case lti::Mirror:

            // right border!

            tp = &(maxRes.at(maxRes.lastIdx())); // pointer to the mirrored data

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,op++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=getValue(*op);
              }
            } else {
              endfp = &maxRes.at(0);
              endfp+=maxRes.size();
              for (;fp<endfp;fp++,ip++,op++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=getValue(*op);
              }
              fp--;
              for (;ip<endip;fp--,ip++,op++,tp--) {
                insdel(filterSize,*tp,*fp);
                (*ip)=getValue(*op);
              }
            }

            // left border!
            xp=&maxRes.at(begin+filterSize-1); // position at source
                                               // to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1);  // position at destination
            op=&src.at(kernLastIdx+begin-1);  // position at destination
            memcpy(this->buffer,&firstResult.at(0),filterSize*sizeof(T));

            endip = &dest.at(0);
            endip--;

            tp = &(maxRes.at(0));

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,op--,xp--,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=getValue(*op);
              }
            } else {
              endxxp = &maxRes.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,op--,xp--,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=getValue(*op);
              }
              xp++;
              for (;ip>endip;ip--,op--,xp++,tp++) {
                insdel(filterSize,*tp,*xp);
                (*ip)=getValue(*op);
              }
            }

            break;

          case lti::Periodic:
            // right border!

            tp = &(maxRes.at(0)); // pointer to the periodic data

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,op++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=getValue(*op);
              }
            } else {
              endfp = &maxRes.at(0);
              endfp+=maxRes.size();
              for (;fp<endfp;fp++,ip++,op++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=getValue(*op);
              }
              fp = &maxRes.at(0);
              for (;ip<endip;fp++,ip++,op++,tp++) {
                insdel(filterSize,*tp,*fp);
                (*ip)=getValue(*op);
              }
            }

            // left border!
            xp=&maxRes.at(begin+filterSize-1); // position at source
                                               // to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1); // position at destination
            op=&src.at(kernLastIdx+begin-1);  // position at destination
            memcpy(this->buffer,&firstResult.at(0),filterSize*sizeof(T));
            endip = &dest.at(0);
            endip--;

            tp = &(maxRes.at(maxRes.lastIdx()));

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,op--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=getValue(*op);
              }
            } else {
              endxxp = &maxRes.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,op--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=getValue(*op);
              }
              xp = &maxRes.at(maxRes.lastIdx());
              for (;ip>endip;ip--,op--,xp--,tp--) {
                insdel(filterSize,*tp,*xp);
                (*ip)=getValue(*op);
              }
            }

            break;
          case lti::Constant:
            T y;

            // right border!
            y = maxRes.at(maxRes.lastIdx());

            if (kernLastIdx>=-1) {
              for (;ip<endip;fp++,ip++,op++) {
                insdel(filterSize,y,*fp);
                (*ip)=getValue(*op);
              }
            } else {
              endfp = &maxRes.at(0);
              endfp+=maxRes.size();
              for (;fp<endfp;fp++,ip++,op++) {
                insdel(filterSize,y,*fp);
                (*ip)=getValue(*op);
              }
              for (;ip<endip;ip++,op++) {
                (*ip)=getValue(*op);
              }
            }

            // left border!
            y = maxRes.at(0);

            xp=&maxRes.at(begin+filterSize-1); // position at source
                                               // to the last
            // element of the kernel region

            ip=&dest.at(kernLastIdx+begin-1); // position at destination
            op=&src.at(kernLastIdx+begin-1); // position at destination
            memcpy(this->buffer,&firstResult.at(0),filterSize*sizeof(T));
            endip = &dest.at(0);
            endip--;

            if (filterSize >= kernLastIdx) {
              for (;ip>endip;ip--,op--,xp--) {
                insdel(filterSize,y,*xp);
                (*ip)=getValue(*op);
              }
            } else {
              endxxp = &maxRes.at(0);
              endxxp--;
              for (;xp>endxxp;ip--,op--,xp--) {
                insdel(filterSize,y,*xp);
                (*ip)=getValue(*op);
              }
              for (;ip>endip;ip--,op--,xp--) {
                (*ip)=getValue(*op);
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
        int pmin,pmax;
        T vmin,vmax;
        maxRes.getIndexOfExtremes(pmin,pmax);
        vmin = maxRes.at(pmin);
        vmax = maxRes.at(pmax);
        dest.fill(minimumT);
        if (pmin < pmax) {
          dest.at(pmax)=vmax;
        }
      } else if ((param.boundaryType == lti::Zero) ||
                 (param.boundaryType == lti::Constant)) {

        // initialize lresult

        T right = maxRes.at(maxRes.lastIdx());
        T left =  maxRes.at(0);

        if (param.boundaryType == lti::Zero) {
          right = T(0);
          left =  T(0);
        } else {
          right = maxRes.at(maxRes.lastIdx());
          left =  maxRes.at(0);
        }

        // initialize the lresult with the kernel at(-1)
        const int lastInit = -kernFirstIdx;
        const int lastInit2 = min(lastInit,maxRes.size());
        const int lastInit0 = min(0,-kernFirstIdx);

        i=1;

        for (f=-kernLastIdx-1;f<lastInit0;++f) {
          insert(i,left);
          ++i;
        }

        for (;f<lastInit2;++f) {
          insert(i,maxRes.at(f));
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
        for (x=0;x<maxRes.size();++x,++f,++l) {

          if (f<0) {
            del=left;
          } else if (f>=maxRes.size()) {
            del=right;
          } else {
            del=maxRes.at(f);
          }

          if (l<0) {
            ins=left;
          } else if (l>=maxRes.size()) {
            ins=right;
          } else {
            ins=maxRes.at(l);
          }

          insdel(filterSize,ins,del);

          dest.at(x) = getValue(maxRes.at(x));
        }
      }
    }

    return true;
  };

  /*
   * Suppression with hystheresis.
   *
   * Given all local maxima in localMax, this method sorts them
   * and beginning with the smallest ones, it starts a region
   * search through all pixels with a value greater than the local
   * maximum being evaluated multiplied with the parameter
   * hystheresisThreshold.  All other local maxima found in the region
   * will be absorved if they are smaller, or they eliminate the actual
   * local maximum if they are greater.
   *
   * @param src the source image
   * @param localMax local maxima found.  It may contain at the end
   *                 of the algorithm less maxima as in the beginning.
   *                 Also the minimumT value will be replaced by
   *                 suppressValue.
   * @param maxList list of local maxima sorted in descending order.
   */
  template<class T>
  bool localMaxima<T>::hystheresisSuppression(const matrix<T>& src,
                                                    matrix<T>& localMax,
                                                    pointList& maxList) const {
    typename matrix<T>::iterator it,eit;
    typename pointList::iterator pit;
    std::list<T> maxVals;
    typename std::list<T>::iterator vit;
    maxList.clear();

    int numMax=0;
    int i;
    T maxVal = minimumT;
    const parameters& par = getParameters();

    // first suppress all non-maxima values and find the global
    // maximum.  Create the list of local maxima to speed up the later
    // stages
    for (it=localMax.begin(),eit=localMax.end(),i=0;
         it != eit;
         ++it,++i) {
      if (*it > minimumT) {
        maxVal = max(maxVal,*it);
        ++numMax;
        maxVals.push_back(*it);
        maxList.push_back(point(i%localMax.columns(),i/localMax.columns()));
      } else {
        *it = suppressValue;
      }
    }


    // suppress all local maxima that are not big enough, and prepare an
    // array to sort them later

    // reserve some memory for the maxima. They will be attached later
    // to some lti::vector object, which will take charge of the reserved
    // memory
    point* thePoints = new point[numMax];
    T* theVals = new T[numMax];

    if ((par.relativeMinimum > 0.0f) &&
        (par.relativeMinimum <= 1.0f)) {
      T thresh = static_cast<T>(par.relativeMinimum*maxVal);
      i=0;
      pit=maxList.begin();
      vit=maxVals.begin();
      while (pit!=maxList.end()) {
        if (*vit < thresh) {
          localMax.at(*pit) = suppressValue;
          pit = maxList.erase(pit);
          vit = maxVals.erase(vit);
        } else {
          thePoints[i].copy(*pit);
          theVals[i]=*vit;
          ++pit;
          ++vit;
          ++i;
        }
      }
      numMax=i;
    } else {
      i=0;
      pit=maxList.begin();
      vit=maxVals.begin();
      while (pit!=maxList.end()) {
        thePoints[i].copy(*pit);
        theVals[i]=*vit;
        ++pit;
        ++vit;
        ++i;
      }
    }

    // sort the local maxima
    vector<point> vPt;
    vector<T> vVal;

    // these vectors will take charge of the memory reserved before
    vPt.useExternData(numMax,thePoints);
    vVal.useExternData(numMax,theVals);

    sort2<T,point> sorter(true); // descending order
    sorter.apply(vVal,vPt);

    const float hystheresis = par.hystheresisThreshold;

    // now check if the region search is necessary
    if ((hystheresis >= 0.0f) &&
        (hystheresis <= 1.0f)) {

      // do region growing from the smallest maxima to the biggest ones
      T actualMax,actualThresh,v;
      int j;
      matrix<T> mask(src);
      static const int deltax[] = {1,1,0,-1,-1,-1,0,1};
      static const int deltay[] = {0,-1,-1,-1,0,1,1,1};
      point p,q,actualMaxPos;
      maxList.clear();
      pointList stack;

      i=vPt.lastIdx();
      while (i>=0) {

        actualMaxPos = p = vPt.at(i);
        actualMax = mask.at(p);

        if (actualMax > minimumT) { // if local maximum not absorved yet:
          actualThresh = static_cast<T>(actualMax*hystheresis);
          stack.push_back(p);

          while (!stack.empty()) { // do region search
            // get next point to be evaluated
            p = stack.back();
            stack.pop_back();
            v = mask.at(p);
            mask.at(p) = minimumT; // flag: already evaluated

            if (v > actualMax) {
              // absorve previous local maxima
              actualMax = v;
              actualMaxPos = p;
            } else if (v > actualThresh) {
              for (j=0;j<8;j++) {
                q.x = p.x+deltax[j];
                q.y = p.y+deltay[j];

                if ((q.x < 0) ||
                    (q.x >= mask.columns()) ||
                    (q.y < 0) ||
                    (q.y >= mask.rows())) continue;

                if (mask.at(q) > actualThresh) {
                  stack.push_back(q);
                } else {
                  mask.at(q) = minimumT;
                }
              }
            }
          }

          // the surviving maxima is a local maxima!
          if (localMax.at(actualMaxPos) != suppressValue) {
            maxList.push_back(actualMaxPos);
          }
        }

        i--;
      }

      numMax = maxList.size();

      vPt.attach(numMax,thePoints);
      vVal.attach(numMax,theVals);

      localMax.fill(suppressValue);
      for (pit=maxList.begin(),i=0;pit!=maxList.end();++pit,++i) {
        vVal.at(i)=localMax.at(*pit)=src.at(*pit);
        vPt.at(i)=*pit;
      }
      
      // sort again
      sorter.apply(vVal,vPt);
      maxList.clear();
      
      // Takes at most par.maxNumber 
      if ((par.maxNumber>0) && (par.maxNumber<vPt.size())) {
        numMax = par.maxNumber;
      }
      else {
        numMax = vPt.size();
      }
      
      for (i=0;i<numMax;++i) {
        maxList.push_back(vPt.at(i));
      }
      
    } else {
      // hystheresis threshold not necessary, just update the pointList
      maxList.clear();
      // Takes at most par.maxNumber 
      if ((par.maxNumber>0) && (par.maxNumber<vPt.size())) {
        numMax = par.maxNumber;
      }
      else { 
        numMax = vPt.size();
      }

      for (i=0;i<numMax;++i) {
        maxList.push_back(vPt.at(i));
      }
    }

    return true;
  }
}
