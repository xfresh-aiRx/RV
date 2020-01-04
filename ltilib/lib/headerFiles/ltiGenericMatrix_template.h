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
 * file .......: ltiGenericMatrix.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiGenericMatrix_template.h,v 1.13 2005/11/24 14:21:32 doerfler Exp $
 */

// this allows to deactivate the instantiation of the generic matrix
#ifndef _LTI_GENERIC_MATRIX_DONT_INSTANTIATE_REQUEST

#ifndef _LTI_GENERIC_MATRIX_TEMPLATE_H_
#define _LTI_GENERIC_MATRIX_TEMPLATE_H_

namespace lti {


  // default constructor
  template <class T>
  genericMatrix<T>::genericMatrix()
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(0,0),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(0),metaNumColumns(0),totalSize(0),
      ownData(true),mode(Connected),theElements(0),rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

  }

  // constructor: rows X cols genericMatrix
  template <class T>
  genericMatrix<T>::genericMatrix(const int rows, const int cols, 
                                  const T& iniValue)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(cols,rows),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(rows),metaNumColumns(cols),
      totalSize(rows*cols),ownData(true),mode(Connected),theElements(0),
      rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

    if((rows<=0)||(cols<=0)) {
      return;
    }

    int i;

    theElements = new T[metaNumRows*metaNumColumns];
    rowAddressTable = allocRows(metaNumRows);

    if ((theElements == 0) || (rowAddressTable == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements;
    for (i=0; i<metaNumRows; ++i,tptr+=metaNumColumns) {
      rowAddressTable[i].useExternData(metaNumColumns,tptr,true);
    }

    fill(iniValue);
  }

  // constructor: rows X cols genericMatrix
  template <class T>
  genericMatrix<T>::genericMatrix(const int rows, const int cols,
                                  const T data[])
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(cols,rows),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(rows),metaNumColumns(cols),
      totalSize(rows*cols),ownData(true),mode(Connected), theElements(0),
      rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

    if((rows<=0)||(cols<=0)) {
      return;
    }

    int i;

    theElements = new T[metaNumRows*metaNumColumns];
    rowAddressTable = allocRows(metaNumRows);

    if ((theElements == 0) || (rowAddressTable == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements;
    for (i = 0; i<metaNumRows; ++i,tptr+=metaNumColumns) {
      rowAddressTable[i].useExternData(metaNumColumns,tptr,true);
    }

    fill(data);
  }

  // constructor: rows X cols genericMatrix
  template <class T>
  genericMatrix<T>::genericMatrix(const ipoint& dim, const T& iniValue)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(dim),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(dim.y),metaNumColumns(dim.x),totalSize(dim.y*dim.x),
      ownData(true),mode(Connected), theElements(0), rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

    if((dim.y<=0)||(dim.x<=0)) {
      return;
    }

    int i;

    theElements = new T[metaNumRows*metaNumColumns];
    rowAddressTable = allocRows(metaNumRows);

    if ((theElements == 0) || (rowAddressTable == 0)) {
      // memory allocation check
      throw allocException();
    }

    // initialize row address table:
    T* tptr = theElements;
    for (i = 0; i<metaNumRows; ++i,tptr+=metaNumColumns) {
      rowAddressTable[i].useExternData(metaNumColumns,tptr,true);
    }

    fill(iniValue);
  }


  // copy constructor
  template <class T>
  genericMatrix<T>::genericMatrix(const bool copyData, 
                                  genericMatrix<T>& other,
                                  const int fromRow,const int toRow,
                                  const int fromCol,const int toCol)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(0,0),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(0),metaNumColumns(0),totalSize(0),
      ownData(true),mode(Connected),theElements(0),rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    // check bounds
    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= other.columns()) {
      tc = other.lastColumn();
    }
    if (tr >= other.rows()) {
      tr = other.lastRow();
    }

    if ((fc > tc) || (fr > tr)) { // negative areas?
      return;                     // yes -> do nothing!
    }

    numColumns = tc - fc + 1;
    numRows    = tr - fr + 1;

    lastRowIdx=numRows-1;
    lastColIdx=numColumns-1;

    if (copyData) {
      copy(other,fr,tr,fc,tc);
    } else { // don't copy -> reference to 'other'`s data.
      int i,j;
      metaNumColumns = other.metaColumns();
      metaNumRows    = other.metaRows();
      totalSize = metaNumRows*metaNumColumns;

      theElements = other.theElements;

      ownData = false;

      if (metaNumColumns == numColumns) {
        mode = Connected;
      } else {
        mode = Line;
      }

      // initialize row address table
      rowAddressTable = allocRows(numRows);
      if (rowAddressTable == 0) {  // memory allocation check
        throw allocException();
      }

      for (i=0,j=fr;j<=tr;++i,++j) {
        rowAddressTable[i].useExternData(numColumns,&other.at(j,fc),true);
      }
    }
  }

  // copy constructor
  template <class T>
  genericMatrix<T>::genericMatrix(const genericMatrix<T>& other,
                                  const int fromRow,const int toRow,
                                  const int fromCol,const int toCol)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(0,0),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(0),metaNumColumns(0),totalSize(0),
      ownData(true),mode(Connected),theElements(0),rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

    copy(other,fromRow,toRow,fromCol,toCol);
  }

  template <class T>
  genericMatrix<T>::genericMatrix(const bool init,
                                  const int rows, const int cols)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(0,0),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(0),metaNumColumns(0),totalSize(0),
      ownData(true),mode(Connected),theElements(0),rowAddressTable(0) {

    resize(rows,cols,T(),false,init);
  }

  template <class T>
  genericMatrix<T>::genericMatrix(const bool init,
                                  const ipoint& size)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(0,0),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(0),metaNumColumns(0),totalSize(0),
      ownData(true),mode(Connected),theElements(0),rowAddressTable(0) {

    resize(size,T(),false,init);
  }

  // copy constructor
  template <class T>
  genericMatrix<T>::genericMatrix(const genericMatrix<T>& other,
                                  const genericVector<int>& idx)
    : mathObject(),numRows(theSize.y),numColumns(theSize.x),theSize(0,0),
      lastRowIdx(numRows-1),lastColIdx(numColumns-1),
      metaNumRows(0),metaNumColumns(0),totalSize(0),
      ownData(true),mode(Connected),theElements(0),rowAddressTable(0) {
    // numRows and numColumns are REFERENCES to theSize attributes!

    copy(other, idx);
  }

  template <class T>
  genericMatrix<T>::~genericMatrix() {
    if (ownData) {
      delete[] theElements;
    }

    delete[] rowAddressTable;

    theElements = 0;
    rowAddressTable = 0;
    numColumns = metaNumColumns = numRows = metaNumRows = totalSize = 0;
  }

  template <class T>
  void genericMatrix<T>::useExternData(const int rows, const int cols, 
                                       T* data) {
    if (isNull(data) ||
        (rows == 0) ||
        (cols == 0)) {
      clear();
      return;
    }

    if (ownData) {
      delete [] theElements;
    }

    ownData = false;

    delete[] rowAddressTable;

    mode = Connected;
    theElements = data;       // reference to the data
    numRows = metaNumRows = rows;
    numColumns = metaNumColumns = cols;
    lastRowIdx=numRows-1;
    lastColIdx=numColumns-1;

    totalSize = metaNumColumns*metaNumRows;

    rowAddressTable = allocRows(numRows);
    if (rowAddressTable == 0) { // memory allocation check
      throw allocException();
    }

    T* tptr = theElements;
    for (int i=0; i<numRows; ++i,tptr+=numColumns) {
      rowAddressTable[i].useExternData(numColumns,tptr,true);
    }
  }

  // return the name of this type
  template <class T>
  const char* genericMatrix<T>::getTypeName() const {
    return "genericMatrix";
  }

  // attach external memory region to this object
  template <class T>
  void genericMatrix<T>::attach(const int rows, const int cols, T* data) {
    useExternData(rows,cols,data);
    ownData = true;
  }

  // detach internal memory to an external 'receiver' genericMatrix
  template <class T>
  void genericMatrix<T>::detach(genericMatrix<T>& receiver) {
    // cannot detach data that is not connected since the call to
    // useExternData() would fail.
    assert(mode==Connected);
    // receiver should know whether he owns data, so it is not
    // accidentally deleted
    if (ownData) {
      receiver.attach(metaNumRows,metaNumColumns,theElements);
    } else {
      receiver.useExternData(metaNumRows,metaNumColumns,theElements);
    }
    ownData = false;
    resize(0,0,T(),false,false);
  }

  // detach internal memory to an external 'receiver' genericVector
  template <class T>
  void genericMatrix<T>::detach(genericVector<T>& receiver) {
    // cannot detach data that is not connected since the call to
    // useExternData() would fail.
    assert(mode==Connected);
    // receiver should know whether he owns data, so it is not
    // accidentally deleted
    if (ownData) {
      receiver.attach(metaNumRows*metaNumColumns,theElements);
    } else {
      receiver.useExternData(metaNumRows*metaNumColumns,theElements);
    }
    ownData = false;
    resize(0,0,T(),false,false);
  }

  template <class T>
  void genericMatrix<T>::exchange(genericMatrix<T>& other) {
    swap(other);
  }

  // exchange data with another genericMatrix
  template <class T>
  void genericMatrix<T>::swap(genericMatrix<T>& other) {

    bool tmpBool;
    eStoreMode tmpMode;
    T* tmpTPtr;
    genericVector<T>* tmpVctPtr;
    int tmpInt;

    tmpInt = theSize.x;
    theSize.x = other.theSize.x;
    other.theSize.x = tmpInt;

    tmpInt = theSize.y;
    theSize.y = other.theSize.y;
    other.theSize.y = tmpInt;

    tmpInt = metaNumRows;
    metaNumRows = other.metaNumRows;
    other.metaNumRows = tmpInt;

    tmpInt = metaNumColumns;
    metaNumColumns = other.metaNumColumns;
    other.metaNumColumns = tmpInt;

    tmpInt = totalSize;
    totalSize = other.totalSize;
    other.totalSize = tmpInt;

    tmpBool = ownData;
    ownData = other.ownData;
    other.ownData = tmpBool;

    tmpMode = mode;
    mode = other.mode;
    other.mode = tmpMode;

    tmpTPtr = theElements;
    theElements = other.theElements;
    other.theElements = tmpTPtr;

    tmpVctPtr = rowAddressTable;
    rowAddressTable = other.rowAddressTable;
    other.rowAddressTable = tmpVctPtr;
  }

  // resize genericMatrix
  template <class T>
  void genericMatrix<T>::resize(const int newRows,const int newCols,
                                const T& iniValue,
                                const bool copyData,
                                const bool initNew) {

    // if the new size is zero then clear all data
    if ((newRows <= 0) || (newCols <= 0)) {
      if (ownData) {
        delete[] theElements;
      } else {
        ownData = true;
      }

      numRows = metaNumRows = 0;
      numColumns = metaNumColumns = 0;
      lastRowIdx=numRows-1;
      lastColIdx=numColumns-1;
      totalSize = 0;

      theElements = 0;

      delete[] rowAddressTable;
      rowAddressTable = 0;

      return; // ready!
    }

    // is resize needed?
    if ((newRows==rows()) &&
        (newCols==columns()) &&
        (mode==Connected)) {
      // resize not needed, just initialize if asked...
      if (!copyData && initNew) {
        fill(iniValue);
      }

      return;
    }

    int fromCol(0), fromRow(0);

    T* newElements = new T[newRows*newCols];

    if (newElements == 0) { // memory allocation check
      throw allocException();
    }

    if (copyData) { // old data should be copied
      fromRow = min(numRows,newRows);
      fromCol = min(numColumns,newCols);
      // if connected memory then data can be copied at once...
      if ((mode == Connected) && (newCols == metaNumColumns)) {
        const int elem = newCols*fromRow;
        if (elem>0) {
          memcpy(newElements,theElements,elem*sizeof(T));
        }
      } else {
        if (fromCol>0) {
          // copy line by line...
          T* tptr = newElements;
          for (int i=0;i<fromRow;++i,tptr+=newCols) {
            memcpy(tptr,&at(i,0),fromCol*sizeof(T));
          }
        }
      }
    }

    if (ownData) {
      delete[] theElements;
    } else {
      ownData = true;
    }

    mode       = Connected;
    numRows    = metaNumRows    = newRows;
    numColumns = metaNumColumns = newCols;
    lastRowIdx = numRows-1;
    lastColIdx = numColumns-1;
    totalSize  = metaNumRows*metaNumColumns;

    delete[] rowAddressTable;
    rowAddressTable = allocRows(numRows);

    if (rowAddressTable == 0) {
      throw allocException();
    }

    theElements = newElements;

    T* tptr = theElements;
    for (int i=0;i<numRows;++i,tptr+=numColumns) {
      rowAddressTable[i].useExternData(numColumns,tptr,true);
    }

    if (initNew) {
      fill(iniValue,0,fromCol,fromRow-1,lastColIdx);
      fill(iniValue,fromRow,0,numRows-1,lastColIdx);
    }
  }

  //
  // restore ownership
  template <class T>
  void genericMatrix<T>::restoreOwnership() {

    // already my data, go away...
    if (ownData) {
      return;
    }

    const int newRows = rows();
    const int newCols = columns();
    const int elem    = newCols*newRows;

    T* newElements = new T[elem];

    if (newElements == 0) { // memory allocation check
      throw allocException();
    }

    // if connected memory then data can be copied at once...
    if ((mode == Connected) && (newCols == metaNumColumns)) {
      if (elem>0) {
        memcpy(newElements,theElements,elem*sizeof(T));
      }
    } else {
      int i;
      if (newCols>0) {
        // copy line by line...
        T* tptr=newElements;
        for (i=0;i<newRows;++i,tptr+=newCols) {
          memcpy(tptr,&at(i,0),newCols*sizeof(T));
        }
      }
    }

    ownData    = true;
    mode       = Connected;
    numRows    = metaNumRows    = newRows;
    numColumns = metaNumColumns = newCols;
    lastRowIdx = numRows-1;
    lastColIdx = numColumns-1;
    totalSize  = metaNumRows*metaNumColumns;

    delete[] rowAddressTable;
    rowAddressTable = allocRows(numRows);
    if (rowAddressTable == 0) {
      throw allocException();
    }

    theElements = newElements;
    T* tptr = theElements;
    for (int i=0;i<numRows;++i,tptr+=numColumns) {
      rowAddressTable[i].useExternData(numColumns,tptr,true);
    }
  }

  template<class T>
  void genericMatrix<T>::clear() {
    resize(0,0,T(),false,false);
  }

  template<class T>
  bool genericMatrix<T>::empty() const {
    return ((numColumns == 0) || (numRows == 0));
  }

  // fill the genericMatrix with 'iniValue'
  template <class T>
  void genericMatrix<T>::fill(const T& iniValue,
                              const int fromRow,
                              const int fromCol,
                              const int toRow,
                              const int toCol) {
    // check and correct boundaries
    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= columns()) {
      tc = lastColumn();
    }
    if (tr >= rows()) {
      tr = lastRow();
    }

    if ((fc > tc) || (fr > tr)) { // negative areas?
      return;                     // yes -> do nothing!
    }

    if ((fc == 0) && (tc == lastColumn()) && (mode == Connected)) {
      // fill as a genericVector!!
      genericVector<T> v;
      v.useExternData(columns()*(tr-fr+1),&at(fr,0),true);
      v.fill(iniValue);
    } else {
      // each line will be treated as a genericVector.
      int i,size;
      genericVector<T> v;
      size = tc-fc+1;

      for (i=fr;i<=tr;++i) {
        v.useExternData(size,&at(i,fc),false);
        v.fill(iniValue);
      }
    }
  }

  // fill the genericMatrix with 'iniValue'
  template <class T>
  void genericMatrix<T>::fill(const T    data[],
                              const int fromRow,
                              const int fromCol,
                              const int toRow,
                              const int toCol) {
    // check and correct boundaries
    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= columns()) {
      tc = lastColumn();
    }
    if (tr >= rows()) {
      tr = lastRow();
    }

    if ((fc > tc) || (fr > tr)) { // negative areas?
      return;                     // yes -> do nothing!
    }

    if ((fc == 0) && (tc == lastColumn()) && (mode == Connected)) {
      // fill as a genericVector!!
      genericVector<T> v;
      v.useExternData(columns()*(tr-fr+1),&at(fr,0),true);
      v.fill(data);
    } else {
      // each line will be treated as a genericVector.
      int i,size;
      genericVector<T> v;
      size = tc-fc+1;
      const T* ptr = data;

      for (i=fr;i<=tr;++i) {
        v.useExternData(size,&at(i,fc),false);
        v.fill(ptr);
        ptr+=size;
      }

      ptr = 0;
    }
  }

  // fill (partially) this genericMatrix with the contents of another genericMatrix
  template <class T>
  void genericMatrix<T>::fill(const genericMatrix<T>& mat,
                              const int fromRow, const int fromCol,
                              const int toRow, const int toCol,
                              const int startAtRow,
                              const int startAtCol) {
    int fr,tr,fc,tc,nr,i,row,sr;
    genericVector<T> vct,tmp;

    // check bounds

    fr = max(0,fromRow);
    fr = min(fr,lastRow());

    tr = min(lastRow(),toRow);
    tr = max(0,tr);

    fc = max(0,fromCol);
    fc = min(fc,lastColumn());

    tc = min(lastColumn(),toCol);
    tc = max(0,tc);

    // invalid source or target genericMatrix boundaries?
    if ((fc > tc) || (fr > tr) ||
        (startAtRow >= mat.rows()) || (startAtCol >= mat.columns())) {
      return; // yes!
    }

    // number of rows
    nr = tr-fr+1;
    nr = min(nr,mat.rows()-startAtRow);

    // copy data
    for (i=0,row=fr,sr=startAtRow;i<nr;++i,++sr,++row) {
      getRow(row).fill(mat.getRow(sr),fc,tc,startAtCol);
    }
  }

  // return a copy of a columns genericVector
  template <class T>
  void genericMatrix<T>::getColumnCopy(const int col,
                                       genericVector<T>& theCol) const {
    assert(col<numColumns);

    theCol.resize(rows(),T(),false,false);
    for (int i=0;i<rows();++i)
      theCol.at(i) = at(i,col);
  }

  // return a copy of the diagonal of the genericMatrix
  template <class T>
  void genericMatrix<T>::getDiagonal(genericVector<T>& theCol) const {
    int n=min(rows(),columns());
    theCol.resize(n,T(),false,false);
    typename genericVector<T>::iterator j=theCol.begin();
    for (int i=0; i<n; ++i) {
      *j++ = at(i,i);
    }
  }
  
  template <class T>
  void genericMatrix<T>::setDiagonal(const genericVector<T>& diag) {
    
    typename genericVector<T>::const_iterator dit=diag.begin();
    const int stopper=min(diag.size(), min(rows(),columns()));

    if (mode==Connected) {
      iterator it=begin();
      const int incr=columns()+1;
      iterator ite=it+incr*stopper;
      for (;it!=ite;it+=incr,++dit) {
        (*it)=(*dit);
      }
    } else {
      int i=0;
      for (;i<stopper;i++,++dit) {
        at(i,i)=(*dit);
      }
    }
    
  }


  // fills a column with the contents of an external genericVector
  template <class T>
  void genericMatrix<T>::setColumn(const int col,
                                   const genericVector<T>& theCol) {
    assert(col<numColumns);
    int i,minRows;
    minRows=min(rows(),theCol.size());
    for (i=0;i<minRows;++i)
      at(i,col) = theCol.at(i);
  }


  // copy an external genericMatrix
  template <class T>
  genericMatrix<T>& genericMatrix<T>::copy(const genericMatrix<T>& other) {

    if (&other == this) { // Caution!! It could be the same genericMatrix...
      return (*this);
    }

    /*
     * pointer to the elements of the genericMatrix.
     *
     * If set to non-zero means that, for some reason, the data cannot be 
     * deleted yet, and needs to be removed after everything has been
     * copied.
     */
    T* delayedElements = 0;

    // erase old data!
    if (ownData) {
      if ((other.columns() != columns()) || (other.rows() != rows())) {

        if ((other.theElements == theElements) && !other.ownData) {
          // The other genericMatrix seems to be a submatrix of this one
          // Delay the removal of theElements until the data has been copied.
          delayedElements = theElements;
          theElements = 0;
        } else {
          delete[] theElements;
          theElements = 0;
        }

        delete[] rowAddressTable;
        rowAddressTable = 0;        
      }
    } else {
      ownData = true;
      theElements = 0;
      delete[] rowAddressTable;
      rowAddressTable = 0;
    }

    if (other.size() == ipoint(0,0)) { // empty genericMatrix?
      // generate empty genericMatrix
      numColumns = metaNumColumns = 0;
      numRows    = metaNumRows    = 0;
      lastRowIdx=numRows-1;
      lastColIdx=numColumns-1;
      totalSize  = 0;
      ownData = true;
      mode = Connected;

      if (notNull(delayedElements)) {
        delete[] delayedElements;
      }
      return (*this);
    }

    numColumns = other.columns();
    numRows    = other.rows();
    lastRowIdx=numRows-1;
    lastColIdx=numColumns-1;


    // The data must be copied!
    // It will be saved as "connected" memory
    mode = Connected;

    // Physical and access boundaries are the same!
    metaNumColumns = numColumns;
    metaNumRows    = numRows;
    totalSize      = metaNumColumns*metaNumRows;

    // allocate memory
    if (isNull(theElements)) {
      theElements =  new T[totalSize];
      if (theElements == 0) { // memory allocation check
        if (notNull(delayedElements)) {
          delete[] delayedElements;
        }
        throw allocException();
      }
    }

    if (isNull(rowAddressTable)) {
      rowAddressTable = allocRows(numRows);

      T* tptr=theElements;
      for (int i=0;i<numRows;++i,tptr+=numColumns) {
        rowAddressTable[i].useExternData(numColumns,tptr,true);
      }
    }

    // Could the whole data be copied at once?
    if ((other.getMode() == Connected) &&
        (metaNumColumns == other.metaColumns())) {
      int elem;
      elem = numRows*numColumns;
      if (elem>0) {
        // the whole data will be copied at once
        memcpy(theElements,&other.at(0,0),elem*sizeof(T));
      }

    } else {
      // the data must be copied line by line
      int i;
      if (numColumns>0) {
        T* tptr = theElements;
        for (i=0;i<other.rows();++i,tptr+=numColumns) {
          memcpy(tptr,&other.at(i,0),numColumns*sizeof(T));
        }
      }
    }

    // Remove the data of the src genericMatrix, in those cases where that data
    // belonged also to this genericMatrix:
    if (notNull(delayedElements)) {
      delete[] delayedElements;
    }
    
    return (*this);
  }


  // copy an external genericMatrix
  template <class T>
  genericMatrix<T>& genericMatrix<T>::copy(const genericMatrix<T>& other,
                                           const int fromRow,const int toRow,
                                           const int fromCol,const int toCol
                                           ) {
    int fc,tc,fr,tr;
    fc = fromCol;
    tc = toCol;
    fr = fromRow;
    tr = toRow;

    // check bounds
    if (fc < 0) {
      fc = 0;
    }
    if (fr < 0) {
      fr = 0;
    }
    if (tc >= other.columns()) {
      tc = other.lastColumn();
    }
    if (tr >= other.rows()) {
      tr = other.lastRow();
    }

    if ((fc == 0) && (tc == other.lastColumn()) &&
        (fr == 0) && (tr == other.lastRow())) {
      return copy(other);
    }

    if (&other == this) { // Caution!! It could be the same genericMatrix...
      if (((tc - fc + 1) == numColumns) && ((tr - fr + 1) == numRows)) {
        return (*this); // a copy of itself is ready!
      } else {
        // a submatrix should be generated:
        genericMatrix<T> tmpMat(*this);  // temporal copy of this
        copy(tmpMat,fr,tr,fc,tc); // extract submatrix
        return *this;             // return itself!
      }
    }

    // erase old data!
    if (ownData) {
      delete[] theElements;
    } else {
      ownData = true;
    }

    theElements = 0;

    delete[] rowAddressTable;
    rowAddressTable = 0;

    if ((fc > tc) || (fr > tr)) { // negative areas?
      // generate empty genericMatrix
      numColumns = metaNumColumns = 0;
      numRows    = metaNumRows    = 0;
      lastRowIdx=numRows-1;
      lastColIdx=numColumns-1;
      totalSize  = 0;
      ownData = true;
      mode = Connected;
      return (*this);
    }

    numColumns = tc - fc + 1;
    numRows    = tr - fr + 1;
    lastRowIdx=numRows-1;
    lastColIdx=numColumns-1;

    // The data must be copied!
    // It will be saved as "connected" memory
    mode = Connected;

    // Physical and access boundaries are the same!
    metaNumColumns = numColumns;
    metaNumRows    = numRows;
    lastRowIdx=numRows-1;
    lastColIdx=numColumns-1;
    totalSize      = metaNumColumns*metaNumRows;

    // allocate memory
    theElements =  new T[totalSize];

    if (theElements == 0) { // memory allocation check
      throw allocException();
    }

    rowAddressTable = allocRows(numRows);

    // Could the whole data be copied at once?
    if ((other.getMode() == Connected) &&
        (metaNumColumns == other.metaColumns())) {
      int elem;
      elem = numRows*numColumns;
      if (elem>0) {
        // the whole data will be copied at once
        memcpy(theElements,&other.at(fr,0),numRows*numColumns*sizeof(T));
      }

      T* tptr = theElements;
      for (int i=0;i<numRows;++i,tptr+=numColumns) {
        rowAddressTable[i].useExternData(numColumns,tptr,true);
      }
    } else {
      // the data must be copied line by line
      int i,j;
      if (numColumns>0) {
        T* tptr = theElements;
        for (i=0,j=fr;j<=tr;++i,++j,tptr+=numColumns) {
          rowAddressTable[i].useExternData(numColumns,tptr,true);
          memcpy(tptr,&other.at(j,fc),numColumns*sizeof(T));
        }
      }
    }

    return (*this);
  }

  // copy an external genericMatrix
  template <class T>
  genericMatrix<T>& genericMatrix<T>::copy(const genericMatrix<T>& other,
                                           const genericVector<int>& idx,
                                           bool rowWise) {


    if (&other == this) { // Caution!! It could be the same genericMatrix...
                          // a submatrix should be generated:
      genericMatrix<T> tmpMat(*this);  // temporal copy of this
      copy(tmpMat,idx);         // extract submatrix
      return *this;             // return itself!
    }

    // erase old data!
    if (ownData) {
      delete[] theElements;
    } else {
      ownData = true;
    }

    theElements = 0;

    delete[] rowAddressTable;
    rowAddressTable = 0;

    if (idx.empty()) { // copy no rows
      // generate empty genericMatrix
      numColumns = metaNumColumns = 0;
      numRows    = metaNumRows    = 0;
      lastRowIdx=numRows-1;
      lastColIdx=numColumns-1;
      totalSize  = 0;
      ownData = true;
      mode = Connected;
      return (*this);
    }

    if (rowWise) {
      numColumns = other.numColumns;
      numRows    = idx.size();
    } else {
      numColumns = idx.size();
      numRows    = other.numRows;
    }

    // The data must be copied!
    // It will be saved as "connected" memory
    mode = Connected;

    // Physical and access boundaries are the same!
    metaNumColumns = numColumns;
    metaNumRows    = numRows;
    lastRowIdx=numRows-1;
    lastColIdx=numColumns-1;
    totalSize      = metaNumColumns*metaNumRows;

    // allocate memory
    theElements =  new T[totalSize];

    if (theElements == 0) { // memory allocation check
      throw allocException();
    }

    rowAddressTable = allocRows(numRows);

    int i;
    T* tptr = theElements;
    if (rowWise) {
      for (i=0; i<numRows; ++i,tptr+=numColumns) {
        rowAddressTable[i].useExternData(numColumns,tptr,true);
        memcpy(tptr,&other.at(idx.at(i),0),numColumns*sizeof(T));
      }
    } else {
      int j;
      for (i=0; i<numRows; ++i) {
        const genericVector<T>& vct = other.getRow(i);
        rowAddressTable[i].useExternData(numColumns,tptr,true);
        for (j=0; j<numColumns; ++j, ++tptr) {
          (*tptr)=vct.at(idx.at(j));
        }
      }
    }

    return (*this);
  }

  /* compare this genericMatrix with other
     @param other the other genericMatrix to be compared with
     @return true if both matrices have the same elements and same size
  */
  template<class T>
  bool genericMatrix<T>::equals(const genericMatrix<T>& other) const {
    bool result;

    result = (size() == other.size());

    if (mode == Connected) {
      result = result &&
        (0 == memcmp(theElements,other.theElements,totalSize*sizeof(T)));
    } else {
      int y;
      for (y=0;result && (y<rows());++y) {
        result = result && getRow(y).equals(other.getRow(y));
      }
    }

    return result;
  }

  // creates a clone of this genericMatrix
  template<class T>
  mathObject* genericMatrix<T>::clone() const {
    genericMatrix<T>* tmp = new genericMatrix<T>(*this);
    return tmp;
  }

  // applies a C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(T (*function)(T)) {
    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(function);
    };

    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(T)) {
    if (size() != other.size()) {
      resize(other.size(),T(),false,false);
    }

    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    };

    return (*this);
  };


  // applies a C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(T (*function)(const T&)) {
    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(function);
    };

    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(const T&)) {
    if (size() != other.size()) {
      resize(other.size(),T(),false,false);
    }

    int y;
    for (y=0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    };

    return (*this);
  };


  // applies a two-parameter C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(const T&,const T&)) {

    assert (other.size() == size());

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    }

    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& other,
                                            T (*function)(T,T)) {

    assert (other.size() == size());

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(other.getRow(y),function);
    }

    return (*this);
  };


  // applies a two-parameter C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& a,
                                            const genericMatrix<T>& b,
                                            T (*function)(T,T)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(a.getRow(y),b.getRow(y),function);
    }

    return (*this);
  };


  // applies a two-parameter C-Function to each element
  template<class T>
  genericMatrix<T>& genericMatrix<T>::apply(const genericMatrix<T>& a,
                                            const genericMatrix<T>& b,
                                            T (*function)(const T&,const T&)) {

    assert (a.size() == b.size());

    if (size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    for (int y = 0;y<rows();++y) {
      getRow(y).apply(a.getRow(y),b.getRow(y),function);
    }

    return (*this);
  };

  // -----------------------------------------------------------------
  // Storable interface
  // -----------------------------------------------------------------

  /**
   * write the object in the given ioHandler
   */
  template<class T>
  bool genericMatrix<T>::write(ioHandler& handler,const bool complete) const {

    ipoint i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    if (b) {
      lti::write(handler,"size",size());

      handler.writeBegin();
      handler.writeSymbol(std::string("data"));
      handler.writeKeyValueSeparator();
      handler.writeBegin();

      int lastCol = lastColumn();

      for (i.y=0;i.y<rows();++i.y) {
        handler.writeBegin();
        if (columns() > 0) {
          for (i.x=0;i.x<lastCol;++i.x) {
            lti::write(handler,at(i));
            handler.writeDataSeparator();
          }
          lti::write(handler,at(i));
        }
        handler.writeEnd();
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

  /**
   * read the object from the given ioHandler
   */
  template<class T>
  bool genericMatrix<T>::read(ioHandler& handler,const bool complete) {
    ipoint i,size;
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    if (b) {
      b = b && lti::read(handler,"size",size);
      std::string str;

      int level = handler.getLevel();

      b = b && handler.readBegin();
      b = b && handler.readSymbol(str);

      if (str == "data") {
        b = b && handler.readKeyValueSeparator();
        b = b && handler.readBegin();
        resize(size,T(),false,false);
        if (size.y > 0) {
          size.x--;
          i.y=0;
          while (b && (i.y<size.y)) {
            b = handler.readBegin();
            if (b) {
              i.x=0;
              while (b && (i.x<size.x)) {
                b = b && lti::read(handler,at(i));
                b = b && handler.readKeyValueSeparator();
                if (b) {
                  ++i.x;
                }
              }

              b = b && lti::read(handler,at(i));
              b = b && handler.readEnd();
              ++i.y;
            }
          }
        }
        if (!b) {
          handler.setStatusString("Error reading genericMatrix at row ");
          handler.appendStatusString(i.y);
          handler.appendStatusString(", col ");
          handler.appendStatusString(i.x);
          handler.appendStatusString(".");
        }
      }

      while (handler.readEnd() && (handler.getLevel() > level));
    }

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }


} // namespace lti

/// ASCII output of a genericMatrix to a std stream.
namespace std {
  template <class T>
  ostream& operator<<(ostream& s,const lti::genericMatrix<T>& amat) {

    int row;
    s << "(";

    if (amat.rows()>0) {
      s << amat.getRow(0);

      for (row=1;row<amat.rows();++row) {
        s << std::endl << " " << amat.getRow(row);
      }
      s << ")";
    } else {
      s << " )";
    }
    return s;
  }
}

#endif
#endif
