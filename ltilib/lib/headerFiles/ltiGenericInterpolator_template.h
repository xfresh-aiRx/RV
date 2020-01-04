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
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiGenericInterpolator_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 12.6.2001
 * revisions ..: $Id: ltiGenericInterpolator_template.h,v 1.3 2004/05/06 10:19:58 alvarado Exp $
 */

namespace lti {
  // --------------------------------------------------
  // genericInterpolator
  // --------------------------------------------------

  // default constructor
  template<class T>
  genericInterpolator<T>::genericInterpolator()
    : scalarValuedInterpolation<T>(),rows(0),bufferRows(0)  {
    parameters defaultParameters;
    setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  genericInterpolator<T>::genericInterpolator(const eBoundaryType btyp)
    : scalarValuedInterpolation<T>(),rows(0),bufferRows(0) {
    parameters defaultParameters(btyp);
    setParameters(defaultParameters);
  }

  // default constructor
  template<class T>
  genericInterpolator<T>::genericInterpolator(const parameters& params)
    : scalarValuedInterpolation<T>(),rows(0),bufferRows(0) {
    setParameters(params);
  }

  // copy constructor
  template<class T>
  genericInterpolator<T>::genericInterpolator(const
                                              genericInterpolator& other)
    : scalarValuedInterpolation<T>(),rows(0),bufferRows(0) {
    copy(other);
  }

  // destructor
  template<class T>
  genericInterpolator<T>::~genericInterpolator() {
    delete[] rows;
    rows=0;
    delete[] bufferRows;
    bufferRows=0;
  }

  // returns the name of this type
  template<class T>
  const char* genericInterpolator<T>::getTypeName() const {
    return "genericInterpolator";
  }

  // copy member
  template<class T>
  genericInterpolator<T>&
  genericInterpolator<T>::copy(const genericInterpolator<T>& other) {
    scalarValuedInterpolation<T>::copy(other);
    return (*this);
  }

  // clone member
  template<class T>
  functor* genericInterpolator<T>::clone() const {
    return new genericInterpolator(*this);
  }

  // return parameters
  template<class T>
  const typename genericInterpolator<T>::parameters&
  genericInterpolator<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw functor::invalidParametersException(getTypeName());
    }
    return *par;
  }

  template<class T>
  bool 
  genericInterpolator<T>::updateParameters() {
    return initLUT();
  }

  template<class T>
  bool genericInterpolator<T>::initLUT() {

    const parameters& param = getParameters();
    intervalSize = param.numSamplesPerInterval;
    bool result = false;
    // initialize the LUT
    // each method must set the "numberOfIntervals" attribute..
    switch (param.kernelType) {
      case parameters::BilinearKernel:
        result = initBilinearLUT();
        break;
      case parameters::BicubicKernel:
        result = initBicubicLUT();
        break;
      case parameters::GenericKernel:
        result = initGenericLUT();
      default:
        initBilinearLUT();
    }

    // initialize the buffers for 2D interpolation
    column.resize(numberOfIntervals,T(),false,true);
    buffer.resize(numberOfIntervals,numberOfIntervals,T(),false,true);
    firstElemColumn = &column.at(0);
       
    delete[] rows;
    rows=0;
    delete[] bufferRows;
    bufferRows=0;

    rows = new const T*[numberOfIntervals];
    bufferRows = new const T*[numberOfIntervals];
    int i;
    for (i=0;i<numberOfIntervals;++i) {
      rows[i]=0;
      bufferRows[i]=&buffer.at(i,0);
    }

    // initialize some constants
    firstSampleOffset = numberOfIntervals/2 - 1;

    return result;
  }


  template<class T>
  bool genericInterpolator<T>::initBilinearLUT() {
    numberOfIntervals = 2;
    lastIntervalFirstSample = intervalSize;
    interpolationKernel.resize(intervalSize*2);

    const float m = -1.0f/interpolationKernel.size();
    const int center = intervalSize;
    int i;
    interpolationKernel.at(0)=0.0f;
    interpolationKernel.at(center)=1.0f;
    for (i=1;i<intervalSize;++i) {
      interpolationKernel.at(center-i) =
      interpolationKernel.at(center+i) = 1.0f-m*static_cast<float>(i);
    }

    return true;
  }

  template<class T>
  bool genericInterpolator<T>::initBicubicLUT() {
    numberOfIntervals = 4;
    interpolationKernel.resize(intervalSize*4);
    lastIntervalFirstSample = 3*intervalSize;

    const int center = 2*intervalSize;

    int i;
    float x;
    interpolationKernel.at(0)=0.0f;
    interpolationKernel.at(center)=1.0f;

    for (i=1;i<intervalSize;++i) {
      x = static_cast<float>(i)/static_cast<float>(intervalSize);
      interpolationKernel.at(center-i) = 
      interpolationKernel.at(center+i) = 1.0f+x*x*(x-2.0f);
    }
    
    for (;i<2*intervalSize;++i) {
      x = static_cast<float>(i)/static_cast<float>(intervalSize);
      interpolationKernel.at(center-i) = 
      interpolationKernel.at(center+i)=4.0f+(x*(x*(5-x)-8.0f));
    }

    return true;
  }

  template<class T>
  bool genericInterpolator<T>::initGenericLUT() {
    const parameters& par = getParameters();
    numberOfIntervals = iround(par.kernelLUT.size()/intervalSize);
    if ((numberOfIntervals*intervalSize != par.kernelLUT.size()) ||
        odd(numberOfIntervals)) {
      // this is an invalid kernel
      initBilinearLUT();
      return false;
    } else {
      interpolationKernel.copy(par.kernelLUT);
      lastIntervalFirstSample = (numberOfIntervals-1)*intervalSize;
    }
    return true;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  /*
   * Returns the interpolated value of the vector at the real valued
   * position x.
   * @param src vector<T> with the source data.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.
   */
  template<class T>
  T genericInterpolator<T>::apply(const vector<T>& src,
                                  const float& x) const {

    int truncX = static_cast<int>(x);
    float fractX = x - static_cast<float>(truncX);
    truncX-=firstSampleOffset;

    // the interpolation:    
    if ((truncX >= 0) && (truncX <= (src.size()-numberOfIntervals))) {
      return compute(fractX,&src[truncX]);
    } else {
      int i;
      switch (getParameters().boundaryType) {
        case lti::NoBoundary: {
          return T();
        } break;
        
        case lti::Zero: {
          for (i=0;i<numberOfIntervals;++i) {
            firstElemColumn[i] = zeroAt(src,truncX+i);
          }
        } break;
        case lti::Constant: {
          for (i=0;i<numberOfIntervals;++i) {
            firstElemColumn[i] = cstAt(src,truncX+i);
          }
        } break;
        case lti::Mirror: {
          for (i=0;i<numberOfIntervals;++i) {
            firstElemColumn[i] = mirrAt(src,truncX+i);
          }
        } break;
        case lti::Periodic: {
          for (i=0;i<numberOfIntervals;++i) {
            firstElemColumn[i] = periAt(src,truncX+i);
          }
        } break;
        default:
          return T();
      }
      return compute(fractX,firstElemColumn);
    }

    return T();
  }

  /*
   * Returns the interpolated value of the vector specified with
   * use() at the real valued position x.
   * @param x the real valued position to be interpolated.
   * @return the interpolated value of the vector.  */
  template<class T>
  T genericInterpolator<T>::apply(const float& x) const {
    assert(notNull(this->theVector));
    return apply(*this->theVector,x);
  }

  /*
   * Returns the interpolated value of the matrix at the real valued
   * position (row,col).
   *
   * @param src matrix<T> with the source data.
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<class T>
  T genericInterpolator<T>::apply(const matrix<T>& src,
                                   const float& row,
                                   const float& col) const {

    return interpolate(src,row,col);
  }

  template<class T>
  T genericInterpolator<T>::apply(const matrix<T>& src,
                                   const tpoint<float>& p) const {
    return interpolate(src,p.y,p.x);
  }

  /*
   * Returns the interpolated value of the matrix specified with
   * use() at the real valued position (row,col).
   *
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<class T>
  T genericInterpolator<T>::apply(const float& row,
                                   const float& col) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,row,col);
  }

  template<class T>
  T genericInterpolator<T>::apply(const tpoint<float>& p) const {
    assert(notNull(this->theMatrix));
    return interpolate(*this->theMatrix,p.y,p.x);
  }

  /*
   * Returns the interpolated value of the matrix at the real valued
   * position (row,col).
   *
   * @param src matrix<T> with the source data.
   * @param row which row
   * @param col which column
   * @return the interpolated value of the matrix.
   */
  template<class T>
  T genericInterpolator<T>::interpolate(const matrix<T>& src,
                                        const float row,
                                        const float col) const {
    // precomputations for x
    int truncX = static_cast<int>(col);
    float fractX = col - static_cast<float>(truncX);
    truncX-=firstSampleOffset;

    // precomputation for y
    int truncY = static_cast<int>(row);
    float fractY = row - static_cast<float>(truncY);
    truncY-=firstSampleOffset;
    int i,j;

    if ((truncX >= 0) && (truncX <= (src.columns()-numberOfIntervals)) &&
        (truncY >= 0) && (truncY <= (src.rows()-numberOfIntervals))) {
      for (i=0;i<numberOfIntervals;++i) {
        rows[i]=&src.at(truncY+i,truncX);
      }
      return compute(fractY,fractX,rows);
    } else {
      switch(getParameters().boundaryType) {
        case lti::NoBoundary: {
          return T();
        } break;
        case lti::Zero: {         
          for (j=0;j<numberOfIntervals;++j) {
            for (i=0;i<numberOfIntervals;++i) {
              buffer.at(j,i)=zeroAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        case lti::Constant: {
          for (j=0;j<numberOfIntervals;++j) {
            for (i=0;i<numberOfIntervals;++i) {
              buffer.at(j,i)=cstAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        case lti::Mirror: {
          for (j=0;j<numberOfIntervals;++j) {
            for (i=0;i<numberOfIntervals;++i) {
              buffer.at(j,i)=mirrAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        case lti::Periodic: {
          for (j=0;j<numberOfIntervals;++j) {
            for (i=0;i<numberOfIntervals;++i) {
              buffer.at(j,i)=periAt(src,truncY+j,truncX+i);
            }
          }
        } break;
        default:
          return T();
      }
      return compute(fractY,fractX,bufferRows);

    }

    return T();
  }

}

