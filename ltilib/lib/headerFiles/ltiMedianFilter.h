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

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiMedianFilter.h
 * authors ....: Guy Wafo Moudhe
 * organization: LTI, RWTH Aachen
 * creation ...: 16.3.2001
 * revisions ..: $Id: ltiMedianFilter.h,v 1.18 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_MEDIAN_FILTER_H_
#define _LTI_MEDIAN_FILTER_H_

#include "ltiFilter.h"
#include "ltiImage.h"
#include "ltiVector.h"
#include "ltiQuickMedian.h"

namespace lti {

  /**
   * This class implements the median filter.
   * The median filter is a non-linear filter.
   * It takes the median value (see lti::quickMedian) of a n*n kernel.
   *
   * The algorithm used for lti::channel uses the straightforward method, and
   * is not as fast as the histogram-based method used for lti::channel8.
   */
  class medianFilter : public filter {
  public:
    /**
     * the parameters for the class medianFilter
     */
    class parameters : public filter::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * size of the kernel to be used (default 3, which means a 3x3 kernel);
       */
      int kernelSize;

    };

    /**
     * default constructor
     */
    medianFilter();

    /**
     * default constructor
     */
    medianFilter(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    medianFilter(const medianFilter& other);

    /**
     * destructor
     */
    virtual ~medianFilter();

    /**
     * returns the name of this type ("medianFilter")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    medianFilter& copy(const medianFilter& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * shortcut for resizing the kernel.  Following code using the shortcut
     *
     * \code
     * medianFilter theMedFilter;
     * theMedFilter.setKernelSize(5);
     * \endcode
     *
     * is equivalent to
     *
     * \code
     * medianFilter theMedFilter;
     *
     * medianFilter::parameters param(theMedFilter.getParameters);
     * param.kernelSize = 5;
     * theMedFilter.setParameters(param);
     * \endcode
     */
    void setKernelSize(const int& newKernelSize);
  protected:

    /**
     * runs inside the image src
     */
    void histogramMethodMiddle(const channel8& src,
                               channel8& dest,
                               ivector& histogram,
                               int& lessThanMedian,
                               const int& row,
                               int& col,
                               ubyte& median,
                               const int& sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Zero
     */
    bool histogramMethodZero(const channel8& src,
                              channel8& dest,
                              const int& sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Constant
     */
    bool histogramMethodConstant(const channel8& src,
                                 channel8& dest,
                                 const int& sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Periodic
     */
    bool histogramMethodPeriodic(const channel8& src,
                                 channel8& dest,
                                 const int& sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary Mirror
     */
    bool histogramMethodMirror(const channel8& src,
                               channel8& dest,
                               const int& sizeOfKernel) const;

    /**
     * applies the histogramMethod for the type boundary NoBoundary
     */
    bool histogramMethodNoBoundary(const channel8& src,
                                   channel8& dest,
                                   const int& sizeOfKernel) const;

    /**
     * the histogramMethod apply the median filter to a matrix
     * of type channel8.In difference with the function realMedian,
     * histogramMethod use a histogram,which avoid to sort each time
     * the kernel
     */
    bool histogramMethod(const channel8& src, channel8& dest) const;

    /**
     * realMedian is a template function which apply the median filter
     * to a matrix of type T.
     * The median filter does not depend on
     * the type of the elements of a matrix.  The implementation must
     * be here due to a bug of the MSVC++
     */
    template <class T>
    bool realMedian(const matrix<T>& src, matrix<T>& dest) const {

      if (src.empty()) {
        dest.clear();
        return true;
      }

      const parameters& param = getParameters();

      if (param.kernelSize <= 1) {
        dest.copy(src);
        return true;
      }

      // Median Filter
      int i,j,row,col,z;
      int k=0;

      // the method can only be applied with a even value of kernelSize
      const int sizeOfKernel = param.kernelSize+((param.kernelSize%2 == 0) ? 1:0);
      const int limit = sizeOfKernel/2;

      lti::quickMedian<T> quickmedia;
      vector<T> vect(sizeOfKernel*sizeOfKernel,T(0));

      //resizes the destination matrix
      dest.resize(src.rows(),src.columns(),T(0),false,false);

      const int lastCol = src.lastColumn()-limit;
      const int lastRow = src.lastRow()-limit;

      //runs through the src's columns
      for(row=limit;row<=lastRow;++row) {
        const int r=row+limit;
        for(col=limit;col<=lastCol;++col) {  //runs through the src's rows
          const int c=col+limit;
          for(i=row-limit;i<=r;++i) { //runs through the rows within the kernel
            //runs through the columns within the kernel
            for(j=col-limit;j<=c;++j) {
              //gives the value from the kernel to the vector
              vect.at(k)=src[i][j];
              k++;
            }
          }
          //fills the destination matrix
          dest.at(row,col)=quickmedia.apply(vect);
          k=0;
        }
      }

      // boundary conditions

      switch (param.boundaryType) {
      case lti::Zero:
        for(row=limit;row<=lastRow;++row) {  //runs the left columns
          z = sizeOfKernel*(sizeOfKernel-limit-1);
          vect.fill(T(0),0,z-1);
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=sizeOfKernel*(sizeOfKernel-limit-1-col);
            for(i=row-limit;i<=r;++i) {
              for(j=0;j<=c;++j){
                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs the right columns
          for(col=lastCol+1;col<=src.lastColumn();++col) {
            z=sizeOfKernel*(sizeOfKernel-(limit+1)-(src.lastColumn()-col));
            vect.fill(T(0),0,z-1);
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=src.lastColumn();++j) {
                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        for(row=0;row<limit;++row) { //runs the rows at the top
          z=sizeOfKernel*(sizeOfKernel-limit-1);
          vect.fill(T(0),0,z-1);
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=sizeOfKernel*(sizeOfKernel-(limit+1)-row);

            for(i=0;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

          // runs at the top,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=sizeOfKernel*sizeOfKernel-(limit+1)*(limit+1+row+col)-col*row;
            vect.fill(T(0),0,z-1);
            for(i=0;i<=r;++i) {

              for(j=0;j<=c;++j) {

                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<=src.lastColumn();++col) {

            z=sizeOfKernel*sizeOfKernel-(limit+1)*
              (limit+1+row+(src.lastColumn()-col))-row*(src.lastColumn()-col);
            vect.fill(T(0),0,z-1);
            for(i=0;i<=r;++i) {
              for(j=col-limit;j<=src.lastColumn();++j){
                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          z=sizeOfKernel*(sizeOfKernel-limit-1);
          vect.fill(T(0),0,z-1);
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=sizeOfKernel*(sizeOfKernel-(limit+1)-(src.lastRow()-row));
            vect.fill(T(0),0,z-1);
            for(i=row-limit;i<=src.lastRow();++i) {
              for(j=col-limit;j<=c;++j) {
                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the bottom,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=sizeOfKernel*sizeOfKernel-(limit+1)*
              (limit+1+(src.lastRow()-row)+col)-col*(src.lastRow()-row);
            vect.fill(T(0),0,z-1);
            for(i=row-limit;i<=src.lastRow();++i) {

              for(j=0;j<=c;++j) {

                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<=src.lastColumn();++col) {

            z=sizeOfKernel*sizeOfKernel-(limit+1)*
              (limit+1+(src.lastRow()-row)+(src.lastColumn()-col))-
              (src.lastRow()-row)*(src.lastColumn()-col);
            vect.fill(T(0),0,z-1);
            for (i=row-limit;i<=src.lastRow();++i) {
              for (j=col-limit;j<=src.lastColumn();++j) {
                vect.at(z)=src[i][j];
                z++;
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        break;
      case lti::Mirror:
        for(row=limit;row<=lastRow;++row) {   //runs the columns left
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(j<0) {
                  vect.at(z)=src[i][-j-1];
                  z++;
                }
                else{// if(j>=0){
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

          for(col=lastCol+1;col<=src.lastColumn();++col) {
            //runs the columns right
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        for(row=0;row<limit;++row) {   //runs the rows at the top
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(i<0) {
                  vect.at(z)=src[-i-1][j];
                  z++;
                }
                else { //if(i>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

          for(col=0;col<limit;++col) {   // runs at the top,left in the corner
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<0){
                  vect.at(z)=src[-i-1][-j-1];
                  z++;
                }
                else if(i>=0 && j<0) {
                  vect.at(z)=src[i][-j-1];
                  z++;
                }
                else if(i<0 && j>=0) {
                  vect.at(z)=src[-i-1][j];
                  z++;
                }
                else {   //if(i>=0 && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }

            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<=src.lastColumn()) {
                  vect.at(z)=src[-i-1][j];
                  z++;
                }
                else if(i<0 && j>src.lastColumn()) {
                  vect.at(z)=src[-i-1][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
                else if(i>=0 && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
                else { //if(i>=0 && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          const int r=row+limit;
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else {// if(i>src.lastRow()) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          for(col=0;col<limit;++col) { //runs at the bottom,left in the corner
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<0) {
                  vect.at(z)=src[i][-j-1];
                  z++;
                }
                else if(i<=src.lastRow() && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i>src.lastRow() && j<0) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][-j-1];
                  z++;
                }
                else if(i>src.lastRow() && j>=0) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i<=src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()+(src.columns()-j)];
                  z++;
                }
                else if(i>src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)][j];
                  z++;
                }
                else if(i>src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()+(src.rows()-i)]
                    [src.lastColumn()+(src.columns()-j)];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        break;
      case lti::Periodic:
        for(row=limit;row<=lastRow;++row) {          //runs the left columns
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(j<0) {
                  vect.at(z)=src[i][src.columns()+j];
                  z++;
                }
                else{// if(j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

          //runs right the columns
          for(col=lastCol+1;col<=src.lastColumn();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(j>src.lastColumn()) {
                  vect.at(z)=src[i][j-src.columns()];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        for(row=0;row<limit;++row) {                //runs the rows at the top
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(i<0) {
                  vect.at(z)=src[i+src.rows()][j];
                  z++;
                }
                else{//if(i>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
           // runs at the top,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<0) {
                  vect.at(z)=src[src.rows()+i][src.columns()+j];
                  z++;
                }
                else if(i>=0 && j<0) {
                  vect.at(z)=src[i][src.columns()+j];
                  z++;
                }
                else if(i<0 && j>=0) {
                  vect.at(z)=src[src.rows()+i][j];
                  z++;
                }
                else if(i>=0 && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }

            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<=src.lastColumn()) {
                  vect.at(z)=src[src.rows()+i][j];
                  z++;
                }
                else if(i<0 && j>src.lastColumn()) {
                  vect.at(z)=src[src.rows()+i][j-src.columns()];
                  z++;
                }
                else if(i>=0 && j>src.lastColumn()) {
                  vect.at(z)=src[i][j-src.columns()];
                  z++;
                }
                else {//if(i>=0 && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          const int r=row+limit;
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(i>src.lastRow()) {
                  vect.at(z)=src[i-src.rows()][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          for(col=0;col<limit;++col) {  //runs at the bottom,left in the corner
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<0) {
                  vect.at(z)=src[i][src.columns()+j];
                  z++;
                }
                else if(i<=src.lastRow() && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i>src.lastRow() && j<0) {
                  vect.at(z)=src[i-src.rows()][src.columns()+j];
                  z++;
                }
                else if(i>src.lastRow() && j>=0) {
                  vect.at(z)=src[i-src.rows()][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i<=src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i][j-src.columns()];
                  z++;
                }
                else if(i>src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i-src.rows()][j];
                  z++;
                }
                else if(i>src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i-src.rows()][j-src.columns()];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

        }
        break;

      case lti::Constant:
        for(row=limit;row<=lastRow;++row) {          //runs the left columns
          const int r=row+limit;
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(j<0) {
                  vect.at(z)=src[i][0];
                  z++;
                }
                else{// if(j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }

          //runs the right columns
          for(col=lastCol+1;col<=src.lastColumn();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        for(row=0;row<limit;++row) {                //runs the rows at the top
          const int r=row+limit;
          // runs the rows at the top in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {

                if(i<0) {
                  vect.at(z)=src[0][j];
                  z++;
                }
                else{//if(i>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          // runs at the top,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {

              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<0){
                  vect.at(z)=src[0][0];
                  z++;
                }
                else if(i>=0 && j<0) {
                  vect.at(z)=src[i][0];
                  z++;
                }
                else if(i<0 && j>=0) {
                  vect.at(z)=src[0][j];
                  z++;
                }
                else if(i>=0 && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }

            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the top,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<0 && j<=src.lastColumn()) {
                  vect.at(z)=src[0][j];
                  z++;
                }
                else if(i<0 && j>src.lastColumn()) {
                  vect.at(z)=src[0][src.lastColumn()];
                  z++;
                }
                else if(i>=0 && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()];
                  z++;
                }
                else {//if(i>=0 && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }
        //runs the rows at the bottom
        for(row=lastRow+1;row<=src.lastRow();++row) {
          const int r=row+limit;
          //runs the rows at the bottom in the middle
          for(col=limit;col<=lastCol;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else{// if(i>src.lastRow()) {
                  vect.at(z)=src[src.lastRow()][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the bottom,left in the corner
          for(col=0;col<limit;++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<0) {
                  vect.at(z)=src[i][0];
                  z++;
                }
                else if(i<=src.lastRow() && j>=0) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i>src.lastRow() && j<0) {
                  vect.at(z)=src[src.lastRow()][0];
                  z++;
                }
                else if(i>src.lastRow() && j>=0) {
                  vect.at(z)=src[src.lastRow()][j];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
          //runs at the bottom,right in the corner
          for(col=lastCol+1;col<src.columns();++col) {
            const int c=col+limit;
            z=0;
            for(i=row-limit;i<=r;++i) {
              for(j=col-limit;j<=c;++j) {
                if(i<=src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[i][j];
                  z++;
                }
                else if(i<=src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[i][src.lastColumn()];
                  z++;
                }
                else if(i>src.lastRow() && j<=src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()][j];
                  z++;
                }
                else if(i>src.lastRow() && j>src.lastColumn()) {
                  vect.at(z)=src[src.lastRow()][src.lastColumn()];
                  z++;
                }
              }
            }
            dest.at(row,col)=quickmedia.apply(vect);
          }
        }

        break;

      default:
        break;

      }

      return true;
    };

  };
}

#endif

