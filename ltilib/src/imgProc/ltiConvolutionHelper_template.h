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
 * file .......: ltiConvolutionHelper_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 17.11.99
 * revisions ..: $Id: ltiConvolutionHelper_template.h,v 1.32 2005/11/07 12:13:48 libuda Exp $
 */

namespace lti {

  // ---------------------------------------------
  //   CONVOLUTION HELPER CLASSES
  // ---------------------------------------------


  // ---------------------------------------------
  // 1D Convolution Helper
  // ---------------------------------------------

  template<class T,class A>
    convHelper1D<T,A>::convHelper1D()
    : kernel(0),tmpKernel(0) {
  }

  template<class T,class A>
    convHelper1D<T,A>::~convHelper1D() {
    delete tmpKernel;
  }

  template<class T,class A>
    bool convHelper1D<T,A>::setKernel(const mathObject* kern) {

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
    
    // check for KernelType
    // check for symmetric or asymmetric
    bool isSymmetric = true;
    bool isAsymmetric = true;
    
    if (kernel->firstIdx() != -kernel->lastIdx()) {
      isSymmetric  = false;
      isAsymmetric = false;
    }
    
    for (int f=kernel->firstIdx(), b=kernel->lastIdx();
         (f < 0) && (isSymmetric || isAsymmetric);
         ++f, --b) {
      isSymmetric  = isSymmetric  && closeTo(kernel->at(f),  kernel->at(b));
      isAsymmetric = isAsymmetric 
        && closeTo(kernel->at(f), static_cast<T>(-kernel->at(b)));
    }
    
    // check middle element
    // set Kernel Type
    if (isAsymmetric && closeToZero(kernel->at(0))) {
      kernelType = Asymmetric;
    } else if (isSymmetric) {
      kernelType = Symmetric;
    } else {
      kernelType = Default;
    }
    
    return (notNull(kernel));
  }
  
  template<class T,class A>
  void convHelper1D<T,A>::apply(const vector<T>& src,
                                 vector<T>& dest,
                                 const modifier::parameters& param) 
  {
    // alias for kernel term
    const kernel1D<T>& kern = *kernel;

    // initialize member "norm"
    lresult.setNorm( kern.getNorm() );

    // need kernel as vector
    const vector<T>& filter = kern;

    // kernel > src?
    if (src.size() < filter.size()) {
      applyLargeFilter(src, dest, param);
      return;
    }

    // initialize size of dest (not need to initialization!)
    if (dest.size() != src.size())
      dest.resize(src.size(),T(0),false,false);

    // shortcuts
    const int filterSize  = filter.size();
    const int sourceSize  = src.size();
    const int middleWidth = sourceSize - filterSize + 1;
    const int kernelLast  = kern.lastIdx();
    const int kernelFirst = kern.firstIdx();
    // where to begin and end in the source
    const int begin = kernelLast < 0 ? -kernelLast : 0;
    const int end   = kernelLast >= filterSize ? 
      sourceSize-kernelLast :
      sourceSize-filterSize+1;


    // used for symmetric and anti-symmetric kernels
    const int filterSizeDec = filterSize-1;
    const int filterSizeHalf = filterSizeDec/2;

    // create iterators
    // source
    //  source general
    typename vector<T>::const_iterator sIt;
    //  source helper
    typename vector<T>::const_iterator sIth;
    //  source helper left
    typename vector<T>::const_iterator sIthl = src.begin();
    //  source helper right
    typename vector<T>::const_iterator sIthr;
    //  source begin
    typename vector<T>::const_iterator sItb = src.begin();
    //  source end
    typename vector<T>::const_iterator sIte = src.end();
    //  source end
    typename vector<T>::const_iterator sItpe = src.end()-1;
    // middle block of source (start and end)
//     typename vector<T>::const_iterator sItmbs = sItb + filterSizeHalf;
//     typename vector<T>::const_iterator sItmben = sIte + kernelFirst;

    // destination
    //  destination general
    typename vector<T>::iterator dIt;
    //  destination begin
    typename vector<T>::iterator dItb = dest.begin();
    //  destination end
    typename vector<T>::iterator dIte = dest.end();
    // middle block of destination (start and end)
//     typename vector<T>::iterator dItmbs = dItb + filterSizeHalf;
//     typename vector<T>::iterator dItmbe = dIte - filterSizeHalf-1;
    
    // filter
    //  filter general
    typename vector<T>::const_iterator fIt;
    //  filter begin
    typename vector<T>::const_iterator fItb = filter.begin();
    //  filter end
    typename vector<T>::const_iterator fItpe = filter.end()-1;
    //  filter middle
//     typename vector<T>::const_iterator fItm = fItb + filterSizeHalf;

    // border helpers
    int elemInsource;
    int elemInBorder;
    int k,b,m,f;

    // check for KernelType and apply optimized method
    switch(kernelType)
    {
      // Asymmetric kernel
      case (Asymmetric):
        // middle block filter (the whole kernel can be applied here!)
        for (dIt = dItb + filterSizeHalf,m = 0; m < middleWidth; ++m){
          // go through the filter forward until middle
          for (sIthl = sItb++,sIthr = sIthl + filterSizeDec,lresult.reset(),
                 fIt=fItb,f=0; f<filterSizeHalf;++f,++fIt,++sIthl,--sIthr) 
            // switch left and right for faster processing
            lresult.accumulateASym(*fIt,*sIthr,*sIthl);
          // process middle of filter
//          lresult.accumulate(*fIt,*sIthl);
          lresult.accumulateZero(*sIthl);
          *dIt++ = lresult.getResult();
        }
        break;

      // Symmetric kernel
      case (Symmetric):
        // middle block filter (the whole kernel can be applied here!)
        for (dIt = dItb + filterSizeHalf,m = 0; m < middleWidth; ++m){
          // go through the filter forward until middle
          for (sIthl=sItb++,sIthr=sIthl+filterSizeDec,lresult.reset(),
               fIt=fItb,f=0;f<filterSizeHalf;++f,++fIt,++sIthl,--sIthr) 
            // process multiply of filter and source
            // switch left and right for faster processing
            lresult.accumulateSym(*fIt,*sIthr,*sIthl);
          // process middle of filter
          lresult.accumulate(*fIt,*sIthl);
          *dIt++ = lresult.getResult();
        }
        break;
  
      // Default kernel
      default:
        // preparations for middle block area
        dIt = dItb + kernelLast + begin;
        sItb += begin;
        // middle block filter (the whole kernel can be applied here!)
        for (m = begin; m < end; ++m, ++sItb) {
          // go through the filter backward until middle
          for (sIt = sItb,lresult.reset(),fIt=fItpe,f=0;
               f<filterSize;++f,--fIt,++sIt) {
            lresult.accumulate(*fIt,*sIt);
          }
          *dIt++ = lresult.getResult();
        }
        break;
    }
    // -----------------------
    // border conditions
    // -----------------------

    // set source and destination iterator to source begin
    sIt = sItb = src.begin();
    dIt = dItb;

    // check boundary type
    switch (param.boundaryType)
    {
      // No boundary: the border is not processed
      case NoBoundary:
        break;

      // Zero boundary: the boundary consists of zeros
      case Zero: {
        // left border
        // go until start of middle block area
        elemInBorder = min(kernelLast, filterSize);
        elemInsource = -kernelFirst + 1;
        for (b=0; b<kernelLast; ++b) {
          // process border area, filtering left to right
          for (lresult.reset(),fIt = fItpe,k=0;k<elemInBorder;++k,--fIt) {
            lresult.accumulate(*fIt,T(0));
          }
          // process source area, filtering left to right
          for (sIth=sItb,k=0;k<elemInsource;++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }
          ++elemInsource;
          if (elemInsource>0) --elemInBorder;
          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
        }

        // right border
        // go until end of source
        elemInBorder = end == src.size() ? filterSize : 1;
        for (dIt=dIte+kernelFirst,b=0; b<-kernelFirst; ++b) {
          // process border area, filtering right to left
          for (lresult.reset(),fIt = fItb,k=0;k<elemInBorder;++k,++fIt) {
            lresult.accumulate(*fIt,T(0));
          }
          // process source area, filtering right to left
          for (sIth=sItpe,k=elemInBorder;k<filterSize;++k,++fIt,--sIth) {
            lresult.accumulate(*fIt,*sIth);
          }
          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
          if (elemInBorder < filterSize) ++elemInBorder;
        }
        break;
      }
      // Mirror boundary
      case Mirror: {

        // left border
        // set helper values to elements in border and in source
        const bool allBorderLeft = kernelFirst>0;
        elemInBorder = allBorderLeft ? filterSize : kernelLast;
        elemInsource = -kernelFirst+1;
        sIt = sItb+kernelLast-1;

        // go until start of middle block area
        for (b=0;b<kernelLast;++b,--sIt) {
          // process border area, filtering left to right
          for (lresult.reset(),sIth=sIt,fIt = fItpe,k=0;
               k<elemInBorder;++k,--fIt,--sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          // helper source iterator is at the right place!!!
          // process source area, filtering right to left
          for (k = 0, sIth=sItb; k < elemInsource; ++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          ++elemInsource;            
          if (elemInsource>0) --elemInBorder;

          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
        }

        // right border
        // set source and destination iterator to element
        //  after of middle block area
        const bool allBorder = end == src.size();
        sIt = sItb + end;
        dIt = dIte + kernelFirst ;

        // set helper values to elements in border and in source
        elemInBorder = allBorder ? filterSize : 1;
        elemInsource = filterSize-elemInBorder;

        // go until end of source
        for (b=0; b < -kernelFirst; ++b){
          // set helper source iterator to filtering start
          sIth = sIt;

          // process source area, filtering left to right
          for (lresult.reset(),fIt=fItpe,k=0;
               k<elemInsource;++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }
          --sIth;

          // helper source iterator is at the right place!!!
          // process border area, filtering left to right
          for (k = 0; k < elemInBorder; ++k,--fIt,--sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
        

          // change border and source area
          if (elemInBorder < filterSize) {
            ++elemInBorder;
            ++sIt;
          } else {
            --sIt;
          }
          --elemInsource;
        }
        break;
      }
      // Periodic boundary
      case Periodic: {
        // left border

        // set helper values to elements in border and in source
        elemInBorder = min(kernelLast, filterSize);
        elemInsource = -kernelFirst + 1;
        sIt = sIte-kernelLast;

        // go until start of middle block area
        for (b=0;b<kernelLast;++b,++sIt) {
          // set helper source iterator to filtering start
          sIth = sIt;
          lresult.reset();
          // process border area, filtering left to right
          for (fIt=fItpe, k=0;k<elemInBorder;++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          // process source area, filtering left to right
          for (k=0, sIth=sItb; k<elemInsource; ++k, --fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          ++elemInsource;
          if (elemInsource>0) --elemInBorder;

          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
        }

        // right border
        // set source and destination iterator
        //  to element after of middle block area
        sIt = sItb + end;
        dIt = dIte + kernelFirst ;

        // set helper values to elements in border and in source
        elemInBorder = end==sourceSize ? filterSize : 1;
        elemInsource = filterSize-elemInBorder;

        // go until end of source
        for (b=0; b < -kernelFirst; ++b) {
          sIth = sIt;
          lresult.reset();

          // process source area, filtering left to right
          for (fIt=fItpe, k=0; k < elemInsource; ++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          // process border area, filtering left to right
          for (sIth=sItb + (elemInsource<0 ? -elemInsource : 0),k=0;
               k<elemInBorder; ++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          --elemInsource;
          if (elemInBorder < filterSize) {
            ++elemInBorder;
            ++sIt;
          }

          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
        }
        break;
      }
      // Constant boundary
      case Constant: {
        // left border
        // go until start of middle block area
        const T borderValueLeft = *sItb;
        elemInBorder = min(kernelLast, filterSize);
        elemInsource = -kernelFirst + 1;
        for (b=0; b<kernelLast; ++b) {
          // process border area, filtering left to right
          for (lresult.reset(),fIt = fItpe,k=0;k<elemInBorder;++k,--fIt) {
            lresult.accumulate(*fIt,borderValueLeft);
          }

          // process source area, filtering left to right
          for (sIth=sItb,k=0;k<elemInsource;++k,--fIt,++sIth) {
            lresult.accumulate(*fIt,*sIth);
          }
          ++elemInsource;
          if (elemInsource>0) --elemInBorder;

          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
        }

        // right border
        // go until end of source
        const T borderValueRight = *sItpe;
        elemInBorder = end == src.size() ? filterSize : 1;
        for (dIt=dIte+kernelFirst,b=0; b<-kernelFirst; ++b) {
          // process border area, filtering right to left
          for (lresult.reset(),fIt = fItb,k=0;k<elemInBorder;++k,++fIt) {
            lresult.accumulate(*fIt,borderValueRight);
          }

          // process source area, filtering right to left
          for (sIth=sItpe,k=elemInBorder;k<filterSize;++k,++fIt,--sIth) {
            lresult.accumulate(*fIt,*sIth);
          }

          // set the result into the destination (middle of filter position)
          *dIt++ = lresult.getResult();
          if (elemInBorder < filterSize) ++elemInBorder;
        }
        break;
      }
      // on default throw exception
      default:
        throw exception("unknown border type!");
        break;
    }

    // switch back filter and source, if filter was bigger than source
  }

  template<class T,class A>
  void convHelper1D<T,A>::applyCol(const matrix<T>& src,
                                        matrix<T>& dest,
                                        const modifier::parameters& param)
  {
    // alias for kernel term
    const kernel1D<T>& kern = *kernel;

    // initialize member "norm"
    lresult.setNorm( kern.getNorm() );

    // need kernel as vector
    const vector<T>& filter = kern;

    dest.resize(src.size(),T(0),false,false);

    // shortcuts
    const int sourceCols = src.columns();
    const int sourceRows = src.rows();
    const int filterSize = filter.size();
    const int filterSizeHalf = (filterSize-1)/2;
    const int kernLastIdx = kern.lastIdx();
    const int stepper = sourceCols*(filterSize-1);
    int x,i,f,xx,filterLast,srcSize2;
    int begin,end,col;

    // check if filter is larger than image
    if (sourceRows < filterSize) {
      applyLargeFilterCol(src,dest,param);
      return;
    }

    // last valid index of the filter vector
    filterLast = filter.lastIdx();
    typename vector<T>::const_iterator fIt;
    typename vector<T>::const_iterator fItEnd;
    // filter begin
    const typename vector<T>::const_iterator fItBegin = filter.begin();
    fItEnd = filter.end();
    --fItEnd;

    // todo
    //  as faster implementation create a window(vertical one columns vector,
    //  size of filter kernel, vector of iterators) and move this horizontal
    //  over the image by increasing the iterators within the vector.
    // source general
    typename matrix<T>::const_iterator sIt;
    // source begin
    typename matrix<T>::const_iterator sItb = src.begin();
    // destination general
    typename matrix<T>::iterator dIt;
    // source helpers
    typename matrix<T>::const_iterator sIthu;
    typename matrix<T>::const_iterator sIthl;

    // Init of accumulator
    lresult.reset();

    begin = (kernLastIdx<0) ? -kernLastIdx : 0;
    end   = (kernLastIdx>=filterSize) ?
      sourceRows-kernLastIdx :
      sourceRows-filterSize+1;

    // set destination to begin of middle block
    dIt = dest.begin();
    dIt+= (kernLastIdx+begin)*sourceCols;

    // check for KernelType and apply optimized method
    switch(kernelType)
    {
      // Asymmetric kernel
      case (Asymmetric):
        // middle block filter (the whole kernel can be applied here!)
        for (x=begin,i=kernLastIdx+x;x<end;++x,++i) {
          // for each column of the matrix do...
          for (col=0; col<sourceCols; ++col) {
            sIthu = sItb;
            sIthl = sIthu + stepper;
            for (lresult.reset(),fIt=fItBegin,f=0;
              f < filterSizeHalf;++f,++fIt){
              lresult.accumulateASym(*fIt,*sIthl,*sIthu);
              sIthl -= sourceCols;
              sIthu += sourceCols;
            }
//            lresult.accumulate(*fIt,*sIthu);
            lresult.accumulateZero(*sIthu);
            *dIt=lresult.getResult();
            ++dIt;
            ++sItb;
          }
        }
        break;

      // Symmetric kernel
      case (Symmetric):
        // middle block filter (the whole kernel can be applied here!)
        for (x=begin,i=kernLastIdx+x;x<end;++x,++i) {
          // for each column of the matrix do...
          for (col=0; col<sourceCols; ++col) {
            sIthu = sItb;
            sIthl = sIthu + stepper;
            for (lresult.reset(),fIt=fItBegin,f=0;
              f < filterSizeHalf;++f,++fIt){
              lresult.accumulateSym(*fIt,*sIthl,*sIthu);
              sIthl -= sourceCols;
              sIthu += sourceCols;
            }
            lresult.accumulate(*fIt,*sIthu);
            *dIt++=lresult.getResult();
            ++sItb;
          }
        }
        break;

      // Default
      default:
        // middle block filter (the whole kernel can be applied here!)
        for (x=begin,i=kernLastIdx+x;x<end;++x,++i) {
          // for each column of the matrix do...
          for (col=0; col<sourceCols; ++col) {
            sIt = sItb++;
            sIt += begin;
            for (lresult.reset(),fIt=fItEnd,f=0;
              f < filterSize;++f,--fIt){
              lresult.accumulate(*fIt,*sIt);
              sIt += sourceCols;
            }
            *dIt++=lresult.getResult();
          }
        }
        break;
    }
     
    // border conditions
    switch (param.boundaryType) {
      case NoBoundary:
        break;
      case Zero:
        // left border!
        for (x=begin-1,i=kernLastIdx+x;x>=-kernLastIdx;--x,--i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x+filterLast,lresult.reset(),f=0;
                  ((xx>=0) && (f<filterSize));++f,--xx) {
              lresult.accumulate(filter.at(f),src.at(xx,col));
            }
            for (; f<filterSize; ++f,--xx) {
              lresult.accumulate(filter.at(f),T(0));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }

        // right border!
        for (x=end,i=kernLastIdx+x;x<sourceRows-kernLastIdx;++x,++i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x,lresult.reset(),f=filterLast;
                  ((xx<sourceRows) && (f>=0));--f,++xx) {
              lresult.accumulate(filter.at(f),src.at(xx,col));
            }
            for (; f>=0; --f,++xx) {
              lresult.accumulate(filter.at(f),T(0));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }
        break;

      case Mirror: {
        // left border! (element 0 twice!)
        int elemInBorder = (kern.firstIdx()>0 ? filterSize : kernLastIdx);
        int elemInsource = -kern.firstIdx() + 1;
        int k;
        for (x=kernLastIdx-1, i=0;
             x>=0;
             --x,++i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x,lresult.reset(),f=filterSize-1, k=0;
                 k<elemInBorder;
                 ++k,  --f,--xx) {
              lresult.accumulate(filter.at(f),src.at(xx,col));
            }

            for (k=0, xx=0; k < elemInsource; ++k, --f, ++xx) {
              lresult.accumulate(filter.at(f),src.at(xx,col));
            }

            ++elemInsource;            
            if (elemInsource>0) --elemInBorder;

            dest.at(i,col)=lresult.getResult();
          }
        }
        // right border!
        srcSize2=2*sourceRows-1;
        for (x=end,i=kernLastIdx+x;
              x<sourceRows-kernLastIdx;
              ++x,++i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x,lresult.reset(),f=filterLast;
                  (f>=0);
                  --f,++xx) {
              if (xx<sourceRows)
                lresult.accumulate(filter.at(f),src.at(xx,col));
              else
                lresult.accumulate(filter.at(f),src.at(srcSize2-xx,col));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }
        break;
      }
      case Periodic:
        // left border!
        for (x=begin-1,i=kernLastIdx+x;
              x>=-kernLastIdx;
              --x,--i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x+filterLast,lresult.reset(),f=0;
                  (f<filterSize);
                  ++f,--xx) {
              if (xx>=0)
                lresult.accumulate(filter.at(f),src.at(xx,col));
              else
                lresult.accumulate(filter.at(f),src.at(sourceRows+xx,col));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }

        // right border!
        for (x=end,i=kernLastIdx+x;
              x<sourceRows-kernLastIdx;
              ++x,++i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x,lresult.reset(),f=filterLast;
                  (f>=0);
                  --f,++xx) {
              if (xx<sourceRows)
                lresult.accumulate(filter.at(f),src.at(xx,col));
              else
                lresult.accumulate(filter.at(f),src.at(xx-sourceRows,col));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }
        break;

      case Constant:
        // left border!
        for (x=begin-1,i=kernLastIdx+x;
              x>=-kernLastIdx;
              --x,--i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x+filterLast,lresult.reset(),f=0;
                  (f<filterSize);
                  ++f,--xx) {
              if (xx>=0)
                lresult.accumulate(filter.at(f),src.at(xx,col));
              else
                lresult.accumulate(filter.at(f),src.at(0,col));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }

        // right border!
        for (x=end,i=kernLastIdx+x;
              x<sourceRows-kernLastIdx;
              ++x,++i) {
          for (col=0;col<sourceCols;++col) {
            for (xx=x,lresult.reset(),f=filterLast;
                  (f>=0);
                  --f,++xx) {
              if (xx<sourceRows)
                lresult.accumulate(filter.at(f),src.at(xx,col));
              else
                lresult.accumulate(filter.at(f),src.at(src.lastRow(),col));
            }
            dest.at(i,col)=lresult.getResult();
          }
        }
        break;
      default:
        throw exception("unknown border type");
    }
  }

  template<class T,class A>
  void convHelper1D<T,A>::applyRow(const matrix<T>& src,
                                         matrix<T>& dest,
                                         const modifier::parameters& param){
    // in my benchmarks the (buggy) explicit implementation didn't
    // show any performance gains (actually this version was a bit
    // faster who knows why).
    // Thus I'm reverting to a loop calling apply() which is
    // definitely more maintainable

    dest.resize(src.rows(),src.columns(),T(0),false,false);

    const int rows = src.rows();
    for (int i=0; i < rows; ++i) {
      apply(src.getRow(i),dest.getRow(i),param);
    }
  }


  template<class T,class A>
  void 
  convHelper1D<T,A>::applyLargeFilter(const vector<T>& src,
                                       vector<T>& dest,
                                       const modifier::parameters& param) {


    int x,f,xx;
    int begin,end;
    lresult.reset();

    const T *filterLastp,*filterFirstp,*endp;

    // alias for kernel term
    const kernel1D<T>& kern = *kernel;

    // initialize member "norm"
    lresult.setNorm( kern.getNorm() );

    // need kernel as vector
    const vector<T>& filter = kern;

    // last valid index of the filter vector
    const int& filterLast = filter.lastIdx();

    // initialize size of dest (not need to initialization!)
    if (dest.size() != src.size()) {
      dest.resize(src.size(),T(0),false,false);
    }

    // middle block filter (the whole kernel can be applied here!)
    begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    end   = (kern.lastIdx()>=filter.size()) ?
      src.size()-kern.lastIdx() :
      src.size()-filter.size()+1;

    endp = &src.at(0);
    endp+=end;

    filterFirstp= &filter.at(0);
    --filterFirstp;
    filterLastp = &filter.at(filterLast);

    
    switch (param.boundaryType) {
      case NoBoundary:
        break;
      case Zero:
        
        for (x=0;x<src.size();++x) {
          for (f = kern.lastIdx(), xx = x+f,lresult.reset();
               f >= kern.firstIdx(); --f,--xx) {
            if ((xx>=0) && (xx<src.size())) {
              lresult.accumulate(kern.at(f),src.at(xx));
            }
          }
          dest.at(x) = lresult.getResult();
        }
        
        break;
      case Mirror:
        
        for (x=0;x<src.size();++x) {
          for (f = kern.lastIdx(), xx = x+f,lresult.reset();
               f >= kern.firstIdx(); --f,--xx) {
            if ((xx>=0) && (xx<src.size())) {
              lresult.accumulate(kern.at(f),src.at(xx));
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
              
              lresult.accumulate(kern.at(f),src.at(xp));
              
            }
          }
          dest.at(x) = lresult.getResult();
        }
        
        break;
      case Periodic:
        for (x=0;x<src.size();++x) {
          for (f = kern.lastIdx(), xx = x+f ,lresult.reset();
               f >= kern.firstIdx(); --f,--xx) {
            int xp;
            
            xp = xx%src.size();
            
            if (xp < 0) {
              xp += src.size();
            }
            
            lresult.accumulate(kern.at(f),src.at(xp));
          }
          dest.at(x) = lresult.getResult();
        }
        
        break;
        
      case Constant:
        for (x=0;x<src.size();++x) {
          for (f = kern.lastIdx(), xx = x+f,lresult.reset();
               f >= kern.firstIdx(); --f,--xx) {
            if (xx < 0) {
              lresult.accumulate(kern.at(f),src.at(0));
            } else if (xx > src.lastIdx()) {
              lresult.accumulate(kern.at(f),src.at(src.lastIdx()));
            } else {
              lresult.accumulate(kern.at(f),src.at(xx));
            }
          }
          dest.at(x) = lresult.getResult();
        }
        
        break;
      default:
        throw exception("unknown border type");
    }
  }


  template<class T,class A>
  void 
  convHelper1D<T,A>::applyLargeFilterCol(const matrix<T>& src,
                                         matrix<T>& dest,
                                         const modifier::parameters& param) {

    int x,f,xx,filterLast;
    int begin,end,col;
    lresult.reset();

    const T *filterLastp,*filterFirstp;

    // alias for kernel term
    const kernel1D<T>& kern = *kernel;

    // initialize member "norm"
    lresult.setNorm( kern.getNorm() );

    // need kernel as vector
    const vector<T>& filter = kern;

    // last valid index of the filter vector
    filterLast = filter.lastIdx();

    // initialize size of dest (not need to initialization!)
    dest.resize(src.rows(),src.columns(),T(0),false,false);

    // middle block filter (the whole kernel can be applied here!)
    begin = (kern.lastIdx()<0) ? -kern.lastIdx() : 0;
    end   = (kern.lastIdx()>=filter.size()) ?
      src.rows()-kern.lastIdx() :
      src.rows()-filter.size()+1;

    filterFirstp= &filter.at(0);
    --filterFirstp;
    filterLastp = &filter.at(filterLast);

    switch (param.boundaryType) {
      case lti::NoBoundary:
        break;
      case lti::Zero:
        for (x=0;x<src.rows();++x) {
          for (col=0;col<src.columns();++col) {
            for (f = kern.lastIdx(), xx = x+f,lresult.reset();
                 f >= kern.firstIdx(); --f,--xx) {
              if ((xx>=0) && (xx<src.rows())) {
                lresult.accumulate(kern.at(f),src.at(xx,col));
              }
            }
            dest.at(x,col) = lresult.getResult();
          }
        }
        break;

      case lti::Mirror:
        for (x=0;x<src.rows();++x) {
          for (col=0;col<src.columns();++col) {
            for (f = kern.lastIdx(), xx = x+f,lresult.reset();
                 f >= kern.firstIdx(); --f,--xx) {
              if ((xx>=0) && (xx<src.rows())) {
                lresult.accumulate(kern.at(f),src.at(xx,col));
              } else {
                int p,xp;
                p = xx/src.rows();
                xp = xx%src.rows();

                if (xx < 0) {
                  p = -p + 1;
                  if (xp < 0) {
                    xp += src.rows();
                  }
                }

                if ((p & 01) != 0) { // even number?
                  xp = src.rows() -1 -xp;
                }

                lresult.accumulate(kern.at(f),src.at(xp,col));

              }
            }
            dest.at(x,col) = lresult.getResult();
          }
        }
        break;

      case lti::Periodic:

        for (x=0;x<src.rows();++x) {
          for (col=0;col<src.columns();++col) {
            for (f = kern.lastIdx(), xx = x+f,lresult.reset();
                 f >= kern.firstIdx(); --f,--xx) {
              int xp;

              xp = xx%src.rows();
              if (xp < 0) {
                xp+=src.rows();
              }
              lresult.accumulate(kern.at(f),src.at(xp,col));
            }
            dest.at(x,col) = lresult.getResult();
          }
        }
        break;

      case lti::Constant:
        for (x=0;x<src.rows();++x) {
          for (col=0;col<src.columns();++col) {
            for (f = kern.lastIdx(), xx = x+f,lresult.reset();
                 f >= kern.firstIdx(); --f,--xx) {
              if (xx < 0) {
                lresult.accumulate(kern.at(f),src.at(0,col));
              } else if (xx >= src.rows()) {
                lresult.accumulate(kern.at(f),src.at(src.lastRow(),col));
              } else {
                lresult.accumulate(kern.at(f),src.at(xx,col));
              }
            }
            dest.at(x,col) = lresult.getResult();
          }
        }

        break;
      default:
        throw exception("unknown border type!");
    }
    

  }

  template<class T,class A>
  A& convHelper1D<T,A>::getAccumulator() {
    return lresult;
  }

  template<class T,class A>
  const A& convHelper1D<T,A>::getAccumulator() const {
    return lresult;
  }

  // ---------------------------------------------
  // 2D FILTERING
  // ---------------------------------------------
  template<class T,class A>
    convHelper2D<T,A>::convHelper2D()
    : kernel(0),sKernel(0),mask(0),tmpKernel(0),tmpSKernel(0) {
  }

  template<class T,class A>
    convHelper2D<T,A>::~convHelper2D() {
    delete tmpKernel;
    tmpKernel=0;
    delete tmpSKernel;
    tmpSKernel=0;
  }

  template<class T,class A>
    bool convHelper2D<T,A>::setKernel(const mathObject* kern) {

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

  template<class T,class A>
  bool convHelper2D<T,A>::isKernelSeparable() const {
    return notNull(sKernel);
  }

  template<class T,class A>
  void convHelper2D<T,A>::apply(const matrix<T>& src,
                                 matrix<T>& dest,
                                 const modifier::parameters& param) {

    int x,y,i,j,f,g,xx,yy,filterLastRow,filterLastCol;
    int beginRow,endRow,beginCol,endCol;
    lresult.reset();

    // alias for kernel term
    const kernel2D<T>& kern = *kernel;

    if ((kern.columns() < 1) ||
        (kern.rows() < 1)) {
      throw exception("kernel is emtpy!");
    }

    // initialize member "norm"
    lresult.setNorm(kern.getNorm());

    // need kernel as matrix
    const matrix<T>& filter = kern;
    filterLastRow = filter.lastRow();
    filterLastCol = filter.lastColumn();

    // initialize size of dest
    dest.resize(src.rows(),src.columns(),T(0),false,false);

    // middle block filter (the whole kernel can be applied here!)
    beginRow = (kern.lastRow()<0) ? -kern.lastRow() : 0;
    beginCol = (kern.lastColumn()<0) ? -kern.lastColumn() : 0;
    endRow   = (kern.lastRow()>=filter.rows()) ?
                 src.rows()-kern.lastRow() :
                 src.rows()-filter.rows()+1;
    endCol   = (kern.lastColumn()>=filter.columns()) ?
                 src.columns()-kern.lastColumn() :
                 src.columns()-filter.columns()+1;

    if ((src.rows() >= filter.rows()) &&
        (src.columns() >= filter.columns())) {
      // ----------------------------------------
      // if the kernel is smaller than the matrix


      for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
        for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
          // x,y: begin in src vector
          // i,j: place to write local result
          // lresult : local result
          // xx,yy: sample on image to be checked
          for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
            for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
              lresult.accumulate(filter.at(g,f),src.at(yy,xx));
            }
          }
          dest.at(j,i)=lresult.getResult();
        }
      }

      // -----------------------
      //    border conditions
      // -----------------------

      switch (param.boundaryType) {
        case NoBoundary:
          break;
        case Zero: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
                for (; f<filter.columns(); ++f,--xx) {
                  lresult.accumulate(filter.at(g,f), T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
                for (; f>=0; --f,++xx) {
                  lresult.accumulate(filter.at(g,f),T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
              for (; g<filter.rows(); ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  lresult.accumulate(filter.at(g,f),T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
              }
              for (; g>=0; --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  lresult.accumulate(filter.at(g,f),T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
                for (; f<filter.columns(); ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),T(0));
                }
              }
              for (; g<filter.rows(); ++g,--yy) {
                for (xx=x+filterLastCol,f=0; 
                     f<filter.columns();
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
                for (; f>=0; --f,++xx) {
                  lresult.accumulate(filter.at(g,f), T(0));
                }
              }
              for (; g<filter.rows(); ++g,--yy) {
                for (xx=x,f=filterLastCol; f>=0; --f,++xx) {
                  lresult.accumulate(filter.at(g,f),T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
                for (; f<filter.columns(); ++f,--xx) {
                  lresult.accumulate(filter.at(g,f), T(0));
                }
              }
              for (; g>=0; --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     f<filter.columns();
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f), T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                }
                for (; f>=0; --f,++xx) {
                  lresult.accumulate(filter.at(g,f), T(0));
                }
              }
              for (; g>=0; --g,++yy) {
                for (xx=x,f=filterLastCol; f>=0; --f,++xx) {
                  lresult.accumulate(filter.at(g,f), T(0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        case Mirror: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (xx>=0)
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy,-xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          int srcCol2=2*src.lastColumn();
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (xx<src.columns())
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy,srcCol2-xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (yy>=0)
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(-yy,xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          int srcRow2=2*src.lastRow();
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (yy<src.rows())
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(srcRow2-yy,xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),
                                     src.at((yy>=0)?yy:-yy,
                                            (xx>=0)?xx:-xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),
                                     src.at((yy>=0) ? yy : -yy,
                                        (xx<src.columns()) ? xx : srcCol2-xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),
                                     src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                            (xx>=0) ? xx : -xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),
                                 src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                        (xx<src.columns()) ? xx : srcCol2-xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        case Periodic: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (xx>=0)
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy,src.columns()+xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (xx<src.columns())
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx-src.columns()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (yy>=0)
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy+src.rows(),xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (yy<src.rows())
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy-src.rows(),xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),
                                     src.at((yy>=0) ? yy : yy+src.rows(),
                                            (xx>=0) ? xx : xx+src.columns()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),
                               src.at((yy>=0)?yy:yy+src.rows(),
                                      (xx<src.columns())?xx:xx-src.columns()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),
                             src.at((yy<src.rows())?yy:yy-src.rows(),
                                    (xx>=0)?xx:xx+src.columns()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),
                             src.at((yy<src.rows())?yy:yy-src.rows(),
                                    (xx<src.columns())?xx:xx-src.columns()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        case Constant: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (xx>=0)
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy,0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (xx<src.columns())
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(yy,src.lastColumn()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (yy>=0)
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(0,xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (yy<src.rows())
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  else
                    lresult.accumulate(filter.at(g,f),src.at(src.lastRow(),xx));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),
                             src.at((yy>=0) ? yy : 0,
                                    (xx>=0) ? xx : 0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),
                             src.at((yy>=0)?yy:0,
                                    (xx<src.columns())?xx:src.lastColumn()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  lresult.accumulate(filter.at(g,f),
                             src.at((yy<src.rows()) ? yy : src.lastRow(),
                                    (xx>=0) ? xx : 0));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  lresult.accumulate(filter.at(g,f),
                            src.at((yy<src.rows()) ? yy : src.lastRow(),
                                   (xx<src.columns()) ? xx : src.lastColumn()));
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        default:
         throw exception("unknown border type!");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the vector

      switch (param.boundaryType) {
        case NoBoundary:
          break;
        case Zero:
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {
              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {
                  if ((xx>=0) && (xx<src.columns()) &&
                      (yy>=0) && (yy<src.rows())) {
                    lresult.accumulate(kern.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }
          break;

        case Mirror:
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {

              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {

                  if ((xx>=0) && (xx<src.columns()) &&
                      (yy>=0) && (yy<src.rows())) {
                    lresult.accumulate(kern.at(g,f),src.at(yy,xx));
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
                    lresult.accumulate(kern.at(g,f),src.at(yp,xp));
                  }
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }
          break;

        case Periodic:

          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {
              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {
                  lresult.accumulate(kern.at(g,f),
                             src.at(yy%src.rows(),xx%src.columns()));
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }

          break;

        case Constant:
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {

              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {

                  int xp,yp;
                  yp = (yy<0) ? 0 : (yy>=src.rows()) ? src.lastRow() : yy;
                  xp = (xx<0) ? 0 : (xx>=src.rows()) ? src.lastRow() : xx;

                  lresult.accumulate(kern.at(g,f),src.at(yp,xp));
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }

          break;
        default:
         throw exception("unknown border type!");
      }
    }
  }

  template<class T,class A>
  void convHelper2D<T,A>::applySep(const matrix<T>& src,
                                               matrix<T>& dest,
                                         const modifier::parameters& param)
  {
    // alias for kernel term
    const sepKernel<T>& kern = *sKernel;

    // initialize member "norm"
    convHelper1D<T,A> conv1D;

    // create main buffer
    matrix<T> buffer;

    // check sizes
    if (src.size() != dest.size())
    {
      dest.resize(src.size(),T(0),false,false);
    }

    const int NoP = kern.getNumberOfPairs();
    if (NoP == 1)
    {
      // rows filter
      if ((kern.getRowFilter(0).size() == 1) &&
          (kern.getRowFilter(0).getOffset() == 0)) 
      {
        buffer.multiply(src,kern.getRowFilter(0).at(0));
      } 
      else 
      {
        conv1D.setKernel(&kern.getRowFilter(0));
        conv1D.applyRow(src,buffer,param);
      }

      // columns filter
      if ((kern.getColFilter(0).size() == 1) &&
          (kern.getColFilter(0).getOffset() == 0)) 
      {
        dest.multiply(buffer,kern.getColFilter(0).at(0));
      } 
      else 
      {
        conv1D.setKernel(&kern.getColFilter(0));
        conv1D.applyCol(buffer,dest,param);
      }
    }
    else
    {
      int i;
      matrix<T> tmpB;

      for (i=0;i<NoP;++i) 
      {
        // rows filter
        if ((kern.getRowFilter(i).size() == 1) &&
            (kern.getRowFilter(i).getOffset() == 0)) 
        {
          buffer.multiply(src,kern.getRowFilter(i).at(0));
        } 
        else 
        {
          conv1D.setKernel(&kern.getRowFilter(i));
          conv1D.applyRow(src,buffer,param);
        }

        // columns filter
        if ((kern.getColFilter(i).size() == 1) &&
            (kern.getColFilter(i).getOffset() == 0)) 
        {
          tmpB.multiply(buffer,kern.getColFilter(i).at(0));
        } 
        else 
        {
          conv1D.setKernel(&kern.getColFilter(i));
          conv1D.applyCol(buffer,tmpB,param);
        }

        if (i==0) 
        {
          // the first time just transfer the result to dest
          tmpB.detach(dest);
        }
        else
        {
          // the rest separable parts should be accumulated in the result!
          dest.add(tmpB);
        }
      }
    }
  }

  template<class T,class A>
  A& convHelper2D<T,A>::getAccumulator() {
    return lresult;
  }

  template<class T,class A>
  const A& convHelper2D<T,A>::getAccumulator() const {
    return lresult;
  }

  template<class T,class A>
  void convHelper2D<T,A>::applyMask(const matrix<T>& src,
                                           matrix<T>& dest,
                                     const modifier::parameters& param) {
    // no correlation possible without a matching mask
    if (isNull(mask) || (mask->size() != kernel->size())) {
      throw functor::invalidParametersException();
    }

    int x,y,i,j,xx,yy,f,g,filterLastRow,filterLastCol;
    int beginRow,endRow,beginCol,endCol;
    lresult.reset();

    // alias for kernel term
    const kernel2D<T>& kern = *kernel;

    if ((kern.columns() < 1) ||
        (kern.rows() < 1)) {
     throw exception("unknown border type!");
    }

    // initialize member "norm"
    lresult.setNorm(kern.getNorm());

    // need kernel as matrix
    const matrix<T>& filter = kern;
    filterLastRow = filter.lastRow();
    filterLastCol = filter.lastColumn();

    // initialize size of dest
    dest.resize(src.rows(),src.columns(),T(0),false,true/*false*/);

    // middle block filter (the whole kernel can be applied here!)
    beginRow = (kern.lastRow()<0) ? -kern.lastRow() : 0;
    beginCol = (kern.lastColumn()<0) ? -kern.lastColumn() : 0;
    endRow   = (kern.lastRow()>=filter.rows()) ?
                 src.rows()-kern.lastRow() :
                 src.rows()-filter.rows()+1;
    endCol   = (kern.lastColumn()>=filter.columns()) ?
                 src.columns()-kern.lastColumn() :
                 src.columns()-filter.columns()+1;

    pointList::const_iterator p;

    if ((src.rows() >= filter.rows()) &&
        (src.columns() >= filter.columns())) {
      // ----------------------------------------
      // if the kernel is smaller than the matrix

      areaPoints shape;
      shape.getFromMask(*mask);

      for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
        for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
          // x,y: begin in src vector
          // i,j: place to write local result
          // lresult : local result
          // xx,yy: sample on image to be checked

          lresult.reset();
          for (p = shape.begin(); p != shape.end(); p++) {
            lresult.accumulate(filter.at(*p),
              src.at(y+filterLastRow - p->y,x+filterLastCol - p->x));
          }
          dest.at(j,i)=lresult.getResult();
        }
      }

      // -----------------------
      //    border conditions
      // -----------------------

      switch (param.boundaryType) {
        case NoBoundary:
          break;
        case Zero: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   ((yy>=0) && (g<filter.rows()));
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     ((xx>=0) && (f<filter.columns()));
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   ((yy<src.rows()) && (g>=0));
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     ((xx<src.columns()) && (f>=0));
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        case Mirror: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    if (xx>=0)
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy,-xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          int srcCol2=2*src.lastColumn();
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    if (xx<src.columns())
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy,srcCol2-xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    if (yy>=0)
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(-yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          int srcRow2=2*src.lastRow();
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    if (yy<src.rows())
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(srcRow2-yy,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                                       src.at((yy>=0)?yy:-yy,
                                              (xx>=0)?xx:-xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                                       src.at((yy>=0) ? yy : -yy,
                                          (xx<src.columns()) ? xx : srcCol2-xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                                       src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                              (xx>=0) ? xx : -xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                                   src.at((yy<src.rows()) ? yy : srcRow2-yy,
                                          (xx<src.columns()) ? xx : srcCol2-xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        case Periodic: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    if (xx>=0)
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy,src.columns()+xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    if (xx<src.columns())
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx-src.columns()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    if (yy>=0)
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy+src.rows(),xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    if (yy<src.rows())
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy-src.rows(),xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                                       src.at((yy>=0) ? yy : yy+src.rows(),
                                              (xx>=0) ? xx : xx+src.columns()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                                 src.at((yy>=0)?yy:yy+src.rows(),
                                        (xx<src.columns())?xx:xx-src.columns()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                               src.at((yy<src.rows())?yy:yy-src.rows(),
                                      (xx>=0)?xx:xx+src.columns()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                               src.at((yy<src.rows())?yy:yy-src.rows(),
                                      (xx<src.columns())?xx:xx-src.columns()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        case Constant: {
          // left border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    if (xx>=0)
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy,0));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // right border!
          for (y=beginRow,j=kern.lastRow()+y;y<endRow;++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;g>=0;--g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    if (xx<src.columns())
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(yy,src.lastColumn()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper border!
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    if (yy>=0)
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(0,xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom border!
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol,i=kern.lastColumn()+x;x<endCol;++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;f>=0;--f,++xx ) {
                  if (mask->at(g,f) != 0) {
                    if (yy<src.rows())
                      lresult.accumulate(filter.at(g,f),src.at(yy,xx));
                    else
                      lresult.accumulate(filter.at(g,f),src.at(src.lastRow(),xx));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper left
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                               src.at((yy>=0) ? yy : 0,
                                      (xx>=0) ? xx : 0));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // upper right
          for (y=beginRow-1,j=kern.lastRow()+y;
               y>=-kern.lastRow();
               --y,--j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {

              for (yy=y+filterLastRow,lresult.reset(),g=0;
                   (g<filter.rows());
                   ++g,--yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                               src.at((yy>=0)?yy:0,
                                      (xx<src.columns())?xx:src.lastColumn()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom left
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=beginCol-1,i=kern.lastColumn()+x;
                 x>=-kern.lastColumn();
                 --x,--i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x+filterLastCol,f=0;
                     (f<filter.columns());
                     ++f,--xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                               src.at((yy<src.rows()) ? yy : src.lastRow(),
                                      (xx>=0) ? xx : 0));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }

          // bottom right
          for (y=endRow,j=kern.lastRow()+y;
               y<src.rows()-kern.lastRow();
               ++y,++j) {
            for (x=endCol,i=kern.lastColumn()+x;
                 x<src.columns()-kern.lastColumn();
                 ++x,++i) {
              for (yy=y,lresult.reset(),g=filterLastRow;
                   (g>=0);
                   --g,++yy) {
                for (xx=x,f=filterLastCol;
                     (f>=0);
                     --f,++xx) {
                  if (mask->at(g,f) != 0) {
                    lresult.accumulate(filter.at(g,f),
                              src.at((yy<src.rows()) ? yy : src.lastRow(),
                                     (xx<src.columns()) ? xx : src.lastColumn()));
                  }
                }
              }
              dest.at(j,i)=lresult.getResult();
            }
          }
        }
        break;

        default:
         throw exception("unknown border type!");
      }
    } else {
      // ---------------------------------------
      // if the kernel is bigger than the matrix

      int kernFirstRow = kern.firstRow();
      int kernFirstCol = kern.firstColumn();

      switch (param.boundaryType) {
        case NoBoundary:
          break;
        case Zero:
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {
              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {
                  if ((xx>=0) && (xx<src.columns()) &&
                      (yy>=0) && (yy<src.rows()) &&
                      (mask->at(g - kernFirstRow, f - kernFirstCol) != 0)) {
                    lresult.accumulate(kern.at(g,f),src.at(yy,xx));
                  }
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }
          break;

        case Mirror:
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {

              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {
                  if (mask->at(g - kernFirstRow, f - kernFirstCol) != 0) {
                    continue;
                  }
                  if ((xx>=0) && (xx<src.columns()) &&
                      (yy>=0) && (yy<src.rows())) {
                    lresult.accumulate(kern.at(g,f),src.at(yy,xx));
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
                    lresult.accumulate(kern.at(g,f),src.at(yp,xp));
                  }
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }
          break;

        case Periodic:

          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {
              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {
                  if (mask->at(g - kernFirstRow, f - kernFirstCol) != 0) {
                    lresult.accumulate(kern.at(g,f),
                               src.at(yy%src.rows(),xx%src.columns()));
                  }
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }

          break;

        case Constant:
          for (y=0;y<src.rows();++y) {
            for (x=0;x<src.columns();++x) {

              for (g = kern.lastRow(), yy = y+g, lresult.reset();
                   g >=kern.firstRow();--g,--yy) {
                for (f = kern.lastColumn(), xx = x+f;
                     f >= kern.firstColumn(); --f,--xx) {
                  if (mask->at(g - kernFirstRow, f - kernFirstCol) != 0) {
                    int xp,yp;
                    yp = (yy<0) ? 0 : (yy>=src.rows()) ? src.lastRow() : yy;
                    xp = (xx<0) ? 0 : (xx>=src.rows()) ? src.lastRow() : xx;

                    lresult.accumulate(kern.at(g,f),src.at(yp,xp));
                  }
                }
              }
              dest.at(y,x) = lresult.getResult();
            }
          }

          break;
        default:
         throw exception("unknown border type!");
      }
    }
  }

  template<class T,class A>
  void convHelper2D<T,A>::setMask(const channel8& mask) {
    this->mask = &mask;
  }

  template<class T,class A>
  const channel8* convHelper2D<T,A>::getMask() const {
    return mask;
  }
}
