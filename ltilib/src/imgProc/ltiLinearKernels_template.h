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
 * file .......: ltiLinearKernels_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.10.00
 * revisions ..: $Id: ltiLinearKernels_template.h,v 1.26 2005/06/16 12:35:11 doerfler Exp $
 */

#include "ltiMinimizeBasis.h"
#include "ltiSerialStatsFunctor.h"
#include <list>

namespace lti {
  // --------------------------------------------------
  // One Dimensional Kernel
  // --------------------------------------------------

  // constructor
  template<class T>
  kernel1D<T>::kernel1D() : array<T>() {
    norm = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<class T>
  kernel1D<T>::kernel1D(const int& from,
                        const int& to,
                        const T& init)
    : array<T>(from,to,init) {
    norm = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<class T>
  kernel1D<T>::kernel1D(const vector<T>& other,
                        const int& theOffset)
    : array<T>(other,theOffset) {
    norm = typeInfo<T>::suggestedNorm();
  }

  // copy constructor
  template<class T>
  kernel1D<T>::kernel1D(const kernel1D<T>& other) : array<T>() {
    copy(other);
  }

  template<class T>
  kernel1D<T>::kernel1D(const bool& init, const int& from, const int& to)
    : array<T>(init,from,to) {
    norm = typeInfo<T>::suggestedNorm();
  }

  // destructor
  template<class T>
  kernel1D<T>::~kernel1D() {
  }

  // copy
  template<class T>
  kernel1D<T>& kernel1D<T>::copy(const kernel1D<T>& other) {
    array<T>::copy(other);
    norm=other.getNorm();
    return (*this);
  }

  // get Offset
  template<class T>
  int kernel1D<T>::getOffset() const {
    return (-array<T>::firstIdx());
  }

  template<class T>
  void kernel1D<T>::setOffset(const int& theOffset) {
    this->setFirstIdx(-theOffset);
  }

  // denormalize
  template<class T>
  void kernel1D<T>::denormalize() {
    if (norm == T(1)) {
      return; // nothing to be done (already normalized!)
    }

    this->divide(norm);
    norm = T(1);
  }

  /*
   * copy the content of the other vector in this kernel and assign
   * the index (firstElement) to the first element of the vector.
   * For example if <code>other</code> is a 3D vector, then
   * <code>castFrom(other,-1)</code> is a 3-elements-kernel which indices
   * lay inside [-1,1].
   * @param other the vector with the data to be copied
   * @param firstElement index for the first element of the vector
   * @return a reference to this instance
   */
  template<class T>
  kernel1D<T>& kernel1D<T>::castFrom(const vector<T>& other,
                                     const int& firstElement) {
    vector<T>::copy(other);
    setOffset(-firstElement);
    return (*this);
  }

  // mirror other kernel
  template <class T>
  kernel1D<T>& kernel1D<T>::mirror(const kernel1D<T>& other) {

    vector<T>& vct = (*this);
    const vector<T>& ovct = other;

    vct.resize(ovct.size(),T(),false,false);

    const int last = vct.size()-1;
    typename vector<T>::const_iterator cit,eit;
    T* ptr;

    for (cit=ovct.begin(),eit=ovct.end(),ptr=&vct.at(last);
         cit!=eit;
         ++cit,--ptr) {
      (*ptr)=(*cit);
    }

    setOffset(last-other.getOffset());

    norm = other.getNorm();

    return (*this);
  }

  // mirror
  template <class T>
  kernel1D<T>& kernel1D<T>::mirror() {
    kernel1D<T> tmp;
    tmp.mirror(*this);
    setOffset(tmp.getOffset());
    tmp.detach(*this);
    return (*this);
  }

  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<class T>
  bool kernel1D<T>::write(ioHandler& handler,const bool complete) const {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      handler.write("norm",getNorm());
      array<T>::write(handler,false);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the object from the given ioHandler
   */
  template<class T>
  bool kernel1D<T>::read(ioHandler& handler,const bool complete) {
    T norm;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }
    
    if (b) {
      handler.read("norm",norm);
      array<T>::read(handler,false);
      setNorm(norm);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }



  // --------------------------------------------------
  // Two Dimensional Kernel
  // --------------------------------------------------

  // outer product of two 1D kernels
  template <class T>
  kernel2D<T>& kernel2D<T>::outerProduct(const kernel1D<T>& k1,
					 const kernel1D<T>& k2) {
    matrix<T>::outerProduct(static_cast<const vector<T> >(k1),
			    static_cast<const vector<T> >(k2));


    offset.y = k1.getOffset();
    offset.x = k2.getOffset();

    matrix<T>::divide(k2.getNorm());

    norm = k1.getNorm();
    return (*this);
  }

  template<>
  kernel2D<ubyte>& kernel2D<ubyte>::outerProduct(const kernel1D<ubyte>& k1,
                                                 const kernel1D<ubyte>& k2);


  // constructor
  template<class T>
  kernel2D<T>::kernel2D() : matrix<T>() {
    norm = typeInfo<T>::suggestedNorm();
  }

  // copy constructor
  template<class T>
  kernel2D<T>::kernel2D(const kernel2D<T>& other) : matrix<T>() {
    copy(other);
  }

  // constructor
  template<class T>
  kernel2D<T>::kernel2D(const int& rows, const int& columns,
                        const T& iniValue)
    : matrix<T>(rows,columns,iniValue) {
    norm = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<class T>
  kernel2D<T>::kernel2D(const int& fromRow,const int& fromCol,
                        const int& toRow,const int& toCol,
                        const T& iniValue)
    : matrix<T>(toRow-fromRow+1,toCol-fromCol+1,iniValue) {
    offset = point(-fromCol,-fromRow);
    norm = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<class T>
  kernel2D<T>::kernel2D(const bool& init, const int& rows, const int& columns)
    : matrix<T>(init,rows,columns) {
    norm = typeInfo<T>::suggestedNorm();
  }

  // constructor
  template<class T>
  kernel2D<T>::kernel2D(const bool& init,
                        const int& fromRow,const int& fromCol,
                        const int& toRow,const int& toCol)
    : matrix<T>(init,toRow-fromRow+1,toCol-fromCol+1) {
    offset = point(-fromCol,-fromRow);
    norm = typeInfo<T>::suggestedNorm();
  }

  // destructor
  template<class T>
  kernel2D<T>::~kernel2D() {
  }

  // copy
  template<class T>
  kernel2D<T>& kernel2D<T>::copy(const kernel2D<T>& other) {
    matrix<T>::copy(other);
    offset = other.getOffset();
    norm = other.getNorm();
    return *this;
  }

  // get Offset
  template<class T>
  const point& kernel2D<T>::getOffset() const {
    return offset;
  }

  // set Offset
  template<class T>
  void kernel2D<T>::setOffset(const point& p) {
    offset = p;
  }

  template<class T>
  void kernel2D<T>::resize(const int& fromRow,const int& fromCol,
                           const int& toRow,const int& toCol,
                           const T& iniValue,
                           const bool& copyData,
                           const bool& initNew) {
    matrix<T>::resize(toRow-fromRow+1,toCol-fromCol+1,
                      iniValue,copyData,initNew);
    offset = point(-fromCol,-fromRow);
  }

  // denormalize
  template<class T>
  void kernel2D<T>::denormalize() {
    if (norm == T(1)) {
      return; // nothing to be done (already normalized!)
    }

    this->divide(norm);
    norm = T(1);
  }

  /*
   * copy the content of the other matrix in this kernel and assign
   * the index (firstRow,firstColumn) to the first element of the matrix.
   * For example if <code>other</code> is a 3x3 matrix, then
   * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
   * lay inside [-1,1].
   * @param other the matrix with the data to be copied
   * @param firstRow index for the first row of the matrix
   * @param firstColumn index for the first column of the matrix
   * @return a reference to this instance
   */
  template<class T>
  kernel2D<T>& kernel2D<T>::castFrom(const matrix<T>& other,
                                     const int& firstRow,
                                     const int& firstColumn) {
    matrix<T>::copy(other);
    setOffset(point(-firstColumn,-firstRow));
    return (*this);
  }

  /*
   * copy the content of the other matrix in this kernel and assign
   * the index (firstRow,firstColumn) to the first element of the matrix.
   * For example if <code>other</code> is a 3x3 matrix, then
   * <code>castFrom(other,-1,-1)</code> is a 3x3 kernel which indices
   * lay inside [-1,1].
   * @param other the matrix with the data to be copied
   * @param firstRow index for the first row of the matrix
   * @param firstColumn index for the first column of the matrix
   * @return a reference to this instance
   */
  template<class T>
  kernel2D<T>& kernel2D<T>::castFrom(const matrix<T>& other,
                                     const point& firstElement) {
    return castFrom(other,firstElement.y,firstElement.x);
  }

  template <class T>
  void kernel2D<T>::fill(const matrix<T>& mat,
                         const int& fromRow, const int& fromCol,
                         const int& toRow,   const int& toCol,
                         const int& startAtRow,
                         const int& startAtCol) {
    point from(fromRow,fromCol);
    point to(toRow,toCol);
    from.add(offset);
    to.add(offset);

    matrix<T>::fill(mat,from.y,from.x,to.y,to.x,startAtRow,startAtCol);
  }

  /*
   * fills this kernel between the "from's" and "to's" with the
   * contents of the matrix <code>mat</code> starting at
   * <code>startAtRow</code> and <code>startAtCol</code>
   *
   * @param data     the data that should fill this kernel
   * @param fromRow  first row of the submatrix to be filled
   * @param fromCol  first column of the submatrix to be filled
   * @param toRow    last row of the submatrix to be filled
   * @param toCol    last column of the submatrix to be filled
   */
  template <class T>
  void kernel2D<T>::fill(const T* data,
                         const int& fromRow,
                         const int& fromCol,
                         const int& toRow,
                         const int& toCol) {

    point from(fromRow,fromCol);
    point to(toRow,toCol);
    from.add(offset);
    to.add(offset);

    matrix<T>::fill(data,from,to);
  }

  template<class T>
  kernel2D<T>& kernel2D<T>::castFrom(const sepKernel<T>& other) {

    resize(0,0,0,0,0,false,false);

    if (other.getNumberOfPairs()<1) {
      return *this;
    }

    int i,j;
    // find out filter dimensions
    rectangle tmpSize;
    tmpSize.ul.y = other.getColFilter(0).firstIdx();
    tmpSize.br.y = other.getColFilter(0).lastIdx();

    tmpSize.ul.x = other.getRowFilter(0).firstIdx();
    tmpSize.br.x = other.getRowFilter(0).lastIdx();

    for (i=1;i<other.getNumberOfPairs();i++) {
      tmpSize.ul.y = min(tmpSize.ul.x,other.getColFilter(i).firstIdx());
      tmpSize.br.y = max(tmpSize.br.x,other.getColFilter(i).lastIdx());

      tmpSize.ul.x = min(tmpSize.ul.y,other.getRowFilter(i).firstIdx());
      tmpSize.br.x = max(tmpSize.br.y,other.getRowFilter(i).lastIdx());
    }

    // resize the kernel
    resize(tmpSize.ul.y,tmpSize.ul.x,tmpSize.br.y,tmpSize.br.x,
           T(0),false,true);

    // add all kernels
    vector<T> tmpRow(tmpSize.br.x-tmpSize.ul.x+1,T(0));

    for (i=0;i<other.getNumberOfPairs();i++) {
      tmpRow.fill(static_cast<vector<T> >(other.getRowFilter(i)),
                  other.getRowFilter(i).firstIdx()-tmpSize.ul.x);
      for (j=other.getColFilter(i).firstIdx();
           j<=other.getColFilter(i).lastIdx();
           j++) {
        this->getRow(j+offset.y).addScaled(other.getColFilter(i).at(j),tmpRow);
      }
      tmpRow.fill(T(0));
    }

    return (*this);
  }


  // mirror other kernel
  template <class T>
  kernel2D<T>& kernel2D<T>::mirror(const kernel2D<T>& other) {
    matrix<T>& mat = (*this);
    const matrix<T>& omat = other;

    mat.resize(omat.size(),T(),false,false);

    int y;
    const int lastRow = mat.lastRow();
    typename vector<T>::const_iterator cit,eit;
    T* ptr;
    for (y=0;y<omat.rows();++y) {
      const vector<T>& vct = omat.getRow(y);
      for (cit=vct.begin(),eit=vct.end(),ptr=&mat.at(lastRow-y,vct.lastIdx());
           cit!=eit;
           ++cit,--ptr) {
        (*ptr)=(*cit);
      }
    }

    const point& oos = other.getOffset();

    setOffset(point(omat.lastColumn()-oos.x,
                    omat.lastRow()-oos.y));

    norm = other.getNorm();

    return (*this);
  }

  // mirror
  template <class T>
  kernel2D<T>& kernel2D<T>::mirror() {
    kernel2D<T> tmp;
    tmp.mirror(*this);
    setOffset(tmp.getOffset());
    tmp.detach(*this);
    return (*this);
  }


  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<class T>
  bool kernel2D<T>::write(ioHandler& handler,
                          const bool complete) const {

    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"norm",getNorm());
      lti::write(handler,"offset",getOffset());
      matrix<T>::write(handler,false);
//       const matrix<T>& mat = *this; // array as vector
//       lti::write(handler,mat,false);
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }

  /*
   * read the object from the given ioHandler
   */
  template<class T>
  bool kernel2D<T>::read(ioHandler& handler,const bool complete) {
    T norm;
    point offset;

    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      lti::read(handler,"norm",norm);
      lti::read(handler,"offset",offset);
//       matrix<T>& mat = *this; // array as vector
//       lti::read(handler,mat,false);
      matrix<T>::read(handler,false);
      setNorm(norm);
      setOffset(offset);
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


  // --------------------------------------------------
  // Separable Kernel
  // --------------------------------------------------

  // constructor
  template<class T>
  sepKernel<T>::sepKernel() : mathObject() {
    rowKernels.clear();
    colKernels.clear();
  }

  // copy constructor
  template<class T>
  sepKernel<T>::sepKernel(const sepKernel<T>& other) : mathObject() {
    copy(other);
  }

  // constructor
  template<class T>
  sepKernel<T>::sepKernel(const int& from,const int& to,const T& iniValue)
    : mathObject() {

    rowKernels.clear();
    rowKernels.resize(1);
    colKernels.clear();
    colKernels.resize(1);

    kernel1D<T> tmp(from,to,iniValue);
    rowKernels[0] = tmp;
    colKernels[0] = tmp;
  }

  // constructor
  template<class T>
  sepKernel<T>::sepKernel(const kernel1D<T>& subkernel)
    : mathObject() {

    rowKernels.clear();
    rowKernels.resize(1);
    colKernels.clear();
    colKernels.resize(1);

    rowKernels[0] = subkernel;
    colKernels[0] = subkernel;
  }

  // destructor
  template<class T>
  sepKernel<T>::~sepKernel() {
    rowKernels.clear();
    colKernels.clear();
  }

  // copy
  template<class T>
  sepKernel<T>& sepKernel<T>::copy(const sepKernel<T>& other) {
    rowKernels = other.rowKernels;
    colKernels = other.colKernels;
    return *this;
  }

  // separate two dimensional kernel
  template<class T>
  bool sepKernel<T>::separate(const kernel2D<T>& k,
			      const double& maxDev) {

    const matrix<T>& km=static_cast<const matrix<T> >(k);
    point offset = k.getOffset();

    // operator to separate a matrix into basis vectors
    minimizeBasis<double> separator;
    // stateFunctor to calculate basic statistics of the matrix
    serialStatsFunctor<T> stat;

    // get the statistics of the matrix
    stat.reset();          // clear statistics
    stat.considerElements(km);  // iterate to get data

    // set parameters for separation
    minimizeBasis<double>::parameters param;
    param.deviationType = minimizeBasis<double>::parameters::Matrix;
    param.maxDeviation  = sqrt(double(stat.getSumOfSquares()))*maxDev;
    param.method        = minimizeBasis<double>::parameters::Exact;

    separator.setParameters(param);

    // --- Zero analysis ---

    std::list<int> zeroRows;
    std::list<int> zeroCols;
    std::list<int>::iterator itCols,itRows;
    int x,y,i,j;

    // look for 0-cols
    for (x=0;x<km.columns();++x) {
      y=0;
      while ((y<km.rows()) && (km.at(y,x) == static_cast<T>(0))) {
        ++y;
      }

      if (y==km.rows()) {
        zeroCols.push_back(x);
      }
    }

    // look for 0-rows
    for (y=0;y<km.rows();++y) {
      x=0;
      while ((x<km.columns()) && (km.at(y,x) == 0)) {
        ++x;
      }

      if (x==km.columns()) {
        zeroRows.push_back(y);
      }
    }

    // create a matrix without the 0-rows or 0-columns

    matrix<double> mtxNot0(km.rows()-zeroRows.size(),
                           km.columns()-zeroCols.size());

    for (y=0,j=0,itRows=zeroRows.begin();y<km.rows();++y) {
      if ((itRows==zeroRows.end()) || ((*itRows) != y)) {
        for (x=0,i=0,itCols=zeroCols.begin();x<km.columns();++x) {
          if ((itCols==zeroCols.end()) || ((*itCols) != x)) {
            mtxNot0.at(j,i) = km.at(y,x);
            ++i;
          } else {
            ++itCols;
          }
        }
        ++j;
      } else {
        ++itRows;
      }
    }

    // separate!
    matrix<double> basis,factors;
    matrix<T> rows,cols;
    separator.apply(mtxNot0,basis,factors);

    // fill with 0-cols and 0-rows (transpose of basis + 0-cols)
    rows.resize(basis.columns(),basis.rows()+zeroCols.size(),
		T(),false,false);
    cols.resize(factors.columns(),factors.rows()+zeroRows.size(),
		T(),false,false);

    // fill the rows
    for (y=0;y<rows.rows();++y) {
      for (x=0,i=0,itCols=zeroCols.begin();x<rows.columns();++x) {
	if ((itCols==zeroCols.end()) || ((*itCols) != x)) {
	  rows.at(y,x) = static_cast<T>(basis.at(i,y));
	  ++i;
	} else {
	  rows.at(y,x) = T(0);
	  ++itCols;
	}
      }
    }

    // fill the cols
    for (y=0;y<cols.rows();++y) {
      for (x=0,i=0,itRows=zeroRows.begin();x<cols.columns();++x) {
	if ((itRows==zeroRows.end()) || ((*itRows) != x)) {
	  cols.at(y,x) = static_cast<T>(factors.at(i,y));
	  ++i;
	} else {
	  cols.at(y,x) = T(0);
	  ++itCols;
	}
      }
    }

    // fill the filter list with the kernels

    // this must always be OK!
    assert(rows.rows() == cols.rows());


    rowKernels.clear();
    rowKernels.resize(rows.rows());
    colKernels.clear();
    colKernels.resize(cols.rows());

    for (y=0;y<rows.rows();++y) {
      rowKernels[y] = kernel1D<T>(rows.getRow(y),offset.x);
      colKernels[y] = kernel1D<T>(cols.getRow(y),offset.y);
    }

    return true;
  }

  template<class T>
  int sepKernel<T>::getNumberOfPairs() const {
    return rowKernels.size();
  }

  template<class T>
  void sepKernel<T>::setNumberOfPairs(const int& numPairs) {
    rowKernels.resize(numPairs);
    colKernels.resize(numPairs);
  }

  template<class T>
  void sepKernel<T>::denormalize() {
    typename std::vector< kernel1D<T> >::iterator it;

    for (it = rowKernels.begin();it != rowKernels.end(); ++it) {
      (*it).denormalize();
    }

    for (it = colKernels.begin();it != colKernels.end(); ++it) {
      (*it).denormalize();
    }
  }

  template<class T>
  sepKernel<T>& sepKernel<T>::multiply(const T& value) {
    typename std::vector< kernel1D<T> >::iterator it;

    for (it = rowKernels.begin();it != rowKernels.end(); ++it) {
      (*it).multiply(value);
    }

    for (it = colKernels.begin();it != colKernels.end(); ++it) {
      (*it).multiply(value);
    }

    return (*this);
  }

  template<class T>
  T sepKernel<T>::sumOfElements() const {

    T result = T(0);
    int i;
    for (i=0;i<getNumberOfPairs();++i) {
      result+=getRowFilter(i).sumOfElements()*
        getColFilter(i).sumOfElements();
    }

    return result;
  }


  template<class T>
  void sepKernel<T>::setNorm(const T& newNorm) {

    typename std::vector< kernel1D<T> >::iterator it;
    for (it = rowKernels.begin();it != rowKernels.end(); ++it) {
      (*it).setNorm(newNorm);
    }

    for (it = colKernels.begin();it != colKernels.end(); ++it) {
      (*it).setNorm(newNorm);
    }
  }

  // mirror other kernel
  template <class T>
  sepKernel<T>& sepKernel<T>::mirror(const sepKernel<T>& other) {
    int i;

    setNumberOfPairs(other.getNumberOfPairs());

    for (i=0;i<getNumberOfPairs();++i) {
      getRowFilter(i).mirror(other.getRowFilter(i));
      getColFilter(i).mirror(other.getColFilter(i));
    }

    return (*this);
  }

  // mirror
  template <class T>
  sepKernel<T>& sepKernel<T>::mirror() {

    int i;
    for (i=0;i<getNumberOfPairs();++i) {
      getRowFilter(i).mirror();
      getColFilter(i).mirror();
    }

    return (*this);
  }

  // applies a C-Function to each element
  template<class T>
  sepKernel<T>& sepKernel<T>::apply(T (*function)(T)) {

    typename std::vector< kernel1D<T> >::iterator rit,cit;

    for (rit=rowKernels.begin(),cit=colKernels.begin();
         rit!=rowKernels.end();
         ++rit,++cit) {
      (*rit).apply(*function);
      (*cit).apply(*function);
    }

    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  sepKernel<T>& sepKernel<T>::apply(T (*function)(const T&)) {

    typename std::vector< kernel1D<T> >::iterator rit,cit;

    for (rit=rowKernels.begin(),cit=colKernels.begin();
         rit!=rowKernels.end();
         ++rit,++cit) {
      (*rit).apply(*function);
      (*cit).apply(*function);
    }

    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  sepKernel<T>& sepKernel<T>::apply(const sepKernel<T>& other,
                                    T (*function)(T)) {

    copy(other);
    return apply(*function);
  };

  // applies a C-Function to each element
  template<class T>
  sepKernel<T>& sepKernel<T>::apply(const sepKernel<T>& other,
                                    T (*function)(const T&)) {

    copy(other);
    return apply(*function);
  };


  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /*
   * write the object in the given ioHandler
   */
  template<class T>
  bool sepKernel<T>::write(ioHandler& handler,
                           const bool complete) const {
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"pairs",getNumberOfPairs());
      handler.writeBegin();
      handler.writeSymbol("data");
      handler.writeKeyValueSeparator();
      handler.writeBegin();
      int i;
      for (i=0;i<getNumberOfPairs();++i) {
        lti::write(handler,getRowFilter(i));
        handler.writeEOL();
        lti::write(handler,getColFilter(i));
        handler.writeEOL();
      }
      handler.writeEnd();
      handler.writeEnd();
    }

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;

  }

  /*
   * read the object from the given ioHandler
   */
  template<class T>
  bool sepKernel<T>::read(ioHandler& handler,const bool complete) {
    int pairs;

    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      std::string str;
      int i;

      lti::read(handler,"pairs",pairs);
      setNumberOfPairs(pairs);
      handler.readBegin();
      handler.readSymbol(str);
      if (str == "data") {
        handler.readBegin();
        for (i=0;i<pairs;++i) {
          lti::read(handler,getRowFilter(i));
          lti::read(handler,getColFilter(i));
        }
        handler.readEnd();
      }
      handler.readEnd();
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }
}

