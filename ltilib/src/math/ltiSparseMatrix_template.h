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
 * file .......: ltiSparseMatrix.cpp
 * authors ....: Bastian Ibach
 * organization: LTI, RWTH Aachen
 * creation ...: 26.04.02
 * revisions ..: $Id: ltiSparseMatrix_template.h,v 1.15 2005/08/14 17:02:09 sltilib Exp $
 */

#include "ltiSTLIoInterface.h"

namespace lti {

  // default constructor
  template <class T>
  sparseMatrix<T>::sparseMatrix()
    : theSize(0,0),numRows(theSize.y),numCols(theSize.x),
      numNonSparseValues(0),sparseValue(T()){

    _lti_debug(*this);

    // reserve memory for values and colIndex vector (5%)
    const int siz = min(100,static_cast<int>(0.05*numRows*numCols));
    values.reserve(siz);
    colIndex.reserve(siz);
  }

  //
  template<class T>
  sparseMatrix<T>::sparseMatrix(const int theRows,
                                const int theCols,
                                const T& sparseVal)
    : theSize(theCols,theRows),numRows(theSize.y),numCols(theSize.x),
      numNonSparseValues(0),sparseValue(sparseVal) {

    rowPtr.resize(numRows+1,0,false,true);

    // reserve memory for values and colIndex vector
    const int siz = min(100,static_cast<int>(0.05*numRows*numCols));
    values.reserve(siz);
    colIndex.reserve(siz);
  }

  //
  template<class T>
  sparseMatrix<T>::sparseMatrix(const point& size,
                                const T& sparseVal)
    : theSize(size),numRows(theSize.y),numCols(theSize.x),
      numNonSparseValues(0),sparseValue(sparseVal) {

    rowPtr.resize(numRows+1,0,false,true);

    // reserve memory for values and colIndex vector
    const int siz = min(100,static_cast<int>(0.05*numRows*numCols));
    values.reserve(siz);
    colIndex.reserve(siz);
  }

  // copy constructor
  template <class T>
  sparseMatrix<T>::sparseMatrix(const sparseMatrix<T>& other)
    : mathObject(),theSize(other.theSize),numRows(theSize.y),numCols(theSize.x),
      numNonSparseValues(other.numNonSparseValues),
      sparseValue(other.sparseValue) {

    values = other.getValues();
    colIndex = other.getColIndex();
    rowPtr = other.getRowPtr();
  }


  /*
   * convert Matrix to SparseMatrix
   */
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::castFrom(const matrix<T>& srcMatrix,
                                             const T& sValue){
    numRows = srcMatrix.rows();
    numCols = srcMatrix.columns();
    sparseValue = sValue;
    rowPtr.resize(numRows+1,0,false,false);

    //convert srcMatrix to sparseMatrix
    //iterate over all elements
    int size=0;         // save size of values-vector
                        // iterate over matrix
    for (int y=0; y < numRows; y++) {
      for (int x=0; x < numCols; x++) {

        // save value if it is not the sparseValue and
        // increment the size of the values-vector
        if (srcMatrix.at(y,x) != sparseValue){
          values.push_back(srcMatrix.at(y,x));
          colIndex.push_back(x);
          size++;
        }
      }
      //update values in rowPtr
      rowPtr[y+1]=size;
    }
    numNonSparseValues = values.size();

    _lti_debug("Values of the sparseMatrix converted from srcMatrix\n" <<
               "number of rows " << numRows << std::endl <<
               "number of cols " << numCols << std::endl <<
               "size of rowPtr " << rowPtr.size() << std::endl <<
               "sparse Value " << sparseValue << std::endl <<
               "numNonSparseValue " << numNonSparseValues << std::endl <<
               *this);

    return *this;
  }

  // convert Matrix to sparseMatrix
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::castFrom(const matrix<T>& srcMatrix){

    // map used for statistical search of the value used
    // as sparseValue
    std::map<T,int> sparseCount;

    // iterate through srcMatrix an count how frequent
    // each value occurs, this is done by the map sparseCount
    typename vector<T>::const_iterator it,eit;
    for(int r=0; r < srcMatrix.rows(); ++r) {
      const vector<T>& vct = srcMatrix.getRow(r);
      for(it=vct.begin(),eit=vct.end(); it != eit; ++it) {
        sparseCount[*it]++;
      }
    }

    // find value which occurs most frequent
    typename std::map<T,int>::const_iterator iter;
    int temp=0;
    for (iter = sparseCount.begin(); iter != sparseCount.end(); ++iter) {
      if (iter->second > temp) {
        temp = iter->second;
        sparseValue = iter->first;
      }
    }

    return castFrom(srcMatrix, sparseValue);
  }


  /*
   * convert sparseMatrix to Matrix
   */
  template <class T>
  bool sparseMatrix<T>::castTo( matrix<T>& destMatrix ){
    //resize Matrix and fill with SparseValue
    destMatrix.resize(numRows, numCols, sparseValue, false, true);
    for(int r=0; r < numRows; ++r){
      for(int c = rowPtr[r]; c < rowPtr[r+1]; ++c){
        destMatrix.at(r,colIndex[c])=values[c];
      }
    }
    return true;
  }

  template <class T>
  bool sparseMatrix<T>::resize(const point& size,const T& sparseVal) {
    return resize(size.y,size.x,sparseVal);
  }

  template <class T>
  bool sparseMatrix<T>::resize(const int rows,
                               const int cols,
                               const T& sparseVal) {

    values.resize(0);
    colIndex.resize(0);
    rowPtr.resize(rows+1,0,false,true);
    numRows = rows;
    numCols = cols;
    numNonSparseValues = 0;
    sparseValue = sparseVal;

    return true;
  }

  /*
   * copy the contents of other to this object
   * @other the Matrix to be copied
   */
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::copy(const sparseMatrix<T>& other){

    if(&other == this) { // Caution could be the same Matrix
      return(*this);
    }
    theSize = other.size();
    values = other.getValues();
    colIndex = other.getColIndex();
    rowPtr = other.getRowPtr();
    numNonSparseValues = other.getNumNonSparseValues();
    sparseValue = other.sparseValue;
    return (*this);
  };


  /*
   * iterator begin
   */
  template <class T>
  typename sparseMatrix<T>::iterator sparseMatrix<T>::begin(){
    return values.begin();
  }

  /*
   * iterator end
   */
  template <class T>
  typename sparseMatrix<T>::iterator sparseMatrix<T>::end(){
    return values.end();
  }


  /*
   * const_iterator begin
   */
  template <class T>
  typename sparseMatrix<T>::const_iterator sparseMatrix<T>::begin() const {
    return values.begin();
  }

  /*
   * const_iterator end
   */
  template <class T>
  typename sparseMatrix<T>::const_iterator sparseMatrix<T>::end() const {
    return values.end();
  }

  /*
   * get value of the element at the given row and column
   */
  template <class T>
  const T& sparseMatrix<T>::getAt(const int row, const int col) const {

    assert( static_cast<uint32>(row) < static_cast<uint32>(numRows) );
    assert( static_cast<uint32>(col) < static_cast<uint32>(numCols) );

    _lti_debug("start getAt function, " << "getAt("<<row<<","<<col<<")\n");

    int middle; //used for binary search
    int leftBorder = rowPtr[row];
    int rightBorder = rowPtr[row+1]-1;

    if (rightBorder < leftBorder) {
      // the row is empty.  Return sparse value!
      return sparseValue;
    }
    else {
      // binary search on colIndex-vector
      while (rightBorder-leftBorder > 4) { // magic number 4 means,
                                           // that only for intervals
                                           // bigger than 5 we should
                                           // try the binary search.
                                           // Otherwise do a linear
                                           // search.
        middle = (leftBorder+rightBorder)/2;
        if (col == colIndex[middle]){
          return values[middle];
        } else if (col < colIndex[middle]) {
          rightBorder=middle;
        } else {
          leftBorder=middle;
        }
      }

      // linear search on colIndex-vector
      while (leftBorder<=rightBorder) {
        if (colIndex[leftBorder]==col) {
          return values[leftBorder];
        }
        leftBorder++;
      }
    }

    return sparseValue; // never reached
  }

  template <class T>
  T& sparseMatrix<T>::forceAt(const int row, const int col) {

    assert( static_cast<uint32>(row) < static_cast<uint32>(numRows) );
    assert( static_cast<uint32>(col) < static_cast<uint32>(numCols) );

    int middle; //used for binary search
    int leftBorder = rowPtr[row];
    int rightBorder = rowPtr[row+1]-1;
    int c;

    if (rightBorder >= leftBorder) {
      // binary search on colIndex-vector
      while (rightBorder-leftBorder > 4) { // magic number 4 means,
                                           // that only for intervals
                                           // bigger than 5 we should
                                           // try the binary search.
                                           // Otherwise do a linear
                                           // search.
        middle = (leftBorder+rightBorder)/2;
        if (col == colIndex[middle]){
          return values[middle];
        } else if (col < colIndex[middle]) {
          rightBorder=middle;
        } else {
          leftBorder=middle;
        }
      }

      // linear search on colIndex-vector
      while (leftBorder<=rightBorder) {
        c = colIndex[leftBorder];
        if (c==col) {
          return values[leftBorder];
        } else if (c>col) {
          break; //
        }
        leftBorder++;
      }
    }

    // if we're still here, we need to insert the (not yet) existing
    // element into the values/colIndex lists.
    values.insert(values.begin()+leftBorder,sparseValue);
    colIndex.insert(colIndex.begin()+leftBorder,col);
    numNonSparseValues++;
    vector<int>::iterator it,eit;
    it = rowPtr.begin()+(row+1);
    eit = rowPtr.end();
    for (;it != eit;++it) {
      (*it)++;
    }

    return values[leftBorder];
  }

  // set element at the given row and column
  template <class T>
  bool sparseMatrix<T>::setAt(const int row, const int col,
                              const T newValue) {

    assert( static_cast<uint32>(row) < static_cast<uint32>(numRows) );
    assert( static_cast<uint32>(col) < static_cast<uint32>(numCols) );

    // check if value and sparseValue are equal
    if( newValue == sparseValue){
      setSparseValue(row,col);
      return true;
    } else {
      forceAt(row,col)=newValue;
      return true;
    }

    return true; // never reached
  }

  // insert element at the given row and colum
  template <class T>
  void sparseMatrix<T>::fill(const T& newValue,
                             const int& fromRow,
                             const int& fromCol,
                             const int& toRow,
                             const int& toCol) {
    // check and correct boundaries
    int fc, tc,fr,tr;
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
    if (tc >= numCols) {
      tc = numCols-1;
    }
    if (tr >= numRows) {
      tr = numRows-1;
    }

    if ((fc > tc) || (fr > tr)) // negative areas?
      return;                   // yes -> do nothing!

    // whole Matrix to fill, set new sparseValue
    // and clear vector values, colIndex, rowPtr
    if( (fr == 0) && (fc == 0) &&
        (tr == numRows-1) && (tc == numCols-1) ){
      sparseValue = newValue;
      values.clear();
      colIndex.clear();
      rowPtr.fill(0);
      return;
    }

    // check if newValue is equal to sparseValue
    if (newValue == sparseValue) {
      fillSparseValue(fromRow,fromCol,toRow,toCol);
      return;
    }

    // TODO: this can be optimized for "fill"
    for(int r=tr; r>=fr; r--){
      for(int c=tc; c>=fc; c--){
        forceAt(r,c)=newValue;
      }
    }
  }


  // clear sparseMatrix, you get an emtpy sparseMatrix
  template <class T>
  void sparseMatrix<T>::clear(){
    values.clear();
    colIndex.clear();
    rowPtr.clear();
    numRows=0;
    numCols=0;
    numNonSparseValues=0;
    sparseValue = T();
  }


  // returns true if the sparseMatrix ist empty
  // (there are no nonSparseValues)
  template <class T>
  bool sparseMatrix<T>::empty() const{
    return (numNonSparseValues == 0);
  }

  /*
   * get the position in the Matrix of the iterator
   * @param iterator
   * @return point, the coordinates
   */
  template <class T>
  point sparseMatrix<T>::getPosition(const iterator& iter){
    int posOfIterator;
    point p;
    //get position in values vector
    posOfIterator = (iter-values.begin());

    //get column
    p.x = colIndex[posOfIterator];

//     //get row, iterate over rowPtr vector not very fast
//     p.y=0;
//     while (posOfIterator >= rowPtr[p.y]) {
//       p.y++;
//     }

//     --p.y;
//     return p;

    // logarithmic search
    int bottom(0),top(rowPtr.size()-2);
    do {
      p.y=(top+bottom)/2;
      if (rowPtr[p.y] > posOfIterator) {
        if (rowPtr[p.y-1] <= posOfIterator) {
          // found!
          --p.y;
          return p;
        }
        top=p.y;
      } else {
        if (rowPtr[p.y+1] > posOfIterator) {
          // found!
          return p;
        }
        bottom=p.y;
      }
    } while (top>bottom);

    // we should not come here, but in case...
    return p;
  }

  /*
   * get the position in the Matrix of the const_iterator
   * @param const_iterator
   * @return point, the coordinates
   */
  template <class T>
  point sparseMatrix<T>::getPosition(const const_iterator& iter) {
    int posOfIterator;
    point p;
    //get position in values vector
    posOfIterator = (iter-values.begin());

    //get column
    p.x = colIndex[posOfIterator];

//     //get row, iterate over rowPtr vector not very fast
//     p.y=0;
//     while (posOfIterator >= rowPtr[p.y]) {
//       p.y++;
//     }

//     --p.y;
//     return p;

    // logarithmic search
    int bottom(0),top(rowPtr.size()-2);
    do {
      p.y=(top+bottom)/2;
      if (rowPtr[p.y] > posOfIterator) {
        if (rowPtr[p.y-1] <= posOfIterator) {
          // found!
          --p.y;
          return p;
        }
        top=p.y;
      } else {
        if (rowPtr[p.y+1] > posOfIterator) {
          // found!
          return p;
        }
        bottom=p.y;
      }
    } while (top>bottom);

    // we should not come here, but in case...
    return p;

  }

  // get a copy of the row of the matrix
  template <class T>
  lti::vector<T> sparseMatrix<T>::getRowCopy(const int& row) const {
    // output vector initialized with number of cols
    // and thesparseValue
    lti::vector<T> outPut(numCols, sparseValue);

    // iterate over the the part of the values vector
    // belonging to the wanted row
    // get col indices and values and write them to
    // the coresponding positions in the outputVector
    const int last = rowPtr[row+1];
    for(int i = rowPtr[row]; i < last; ++i) {
      outPut[colIndex[i]]= values[i];
    }
    return outPut;
  }

  // get a copy of the column of the matrix
  template <class T>
  lti::vector<T> sparseMatrix<T>::getColumnCopy(const int& col) const {
    // output vector initialized with number of cols
    // and thesparseValue
    lti::vector<T> outPut(numRows, sparseValue);

    // variables for binary search
    int leftBorder, rightBorder; //used for binary search

    // go from row to row and use binary search to
    // get value of the column you are looking for
    leftBorder = rowPtr[0];
    for (int r=0; r<numRows; r++) {
      rightBorder = rowPtr[r+1];
      // check for empty row
      if (leftBorder != rightBorder) {
        outPut.at(r)=getAt(r,col);
      }
      leftBorder = rightBorder;
    }
    return outPut;
  }

  // return the name of this type
  template <class T>
  const char* sparseMatrix<T>::getTypeName() const {
    return "sparseMatrix";
  }

  template <class T>
  sparseMatrix<T>::~sparseMatrix(){
  }

  // creates a clone of this matrix
  template<class T>
  mathObject* sparseMatrix<T>::clone() const {
    return new sparseMatrix<T>(*this);
  }

  // assignment operator (alias for copy(other))
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::operator=(const sparseMatrix<T>& other){
    return copy(other);
  }

  // alias for add(const T value)
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::operator+=(const T value){
    return add(value);
  }

  // add const value to this matrix
  template <class T>
  sparseMatrix<T> sparseMatrix<T>::operator+(const T value) const{
    sparseMatrix<T> tmp(*this);
    tmp.add(value);
    return tmp;
  }

  // compare two matrices
  template <class T>
  bool sparseMatrix<T>::operator==(const sparseMatrix<T> other) const{
    if((sparseValue == other.sparseValue) &&
       (values == other.values) &&
       (colIndex == other.colIndex) &&
       (rowPtr == other.rowPtr)){
      return true;
    }
    return false;
  }

  // alias for multiply(const T value)
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::operator*=(const T value){
    return multiply(value);
  }

  // returns values vector
  template <class T>
  const std::vector<T>& sparseMatrix<T>::getValues() const{
    return values;
  }


  // returns colIndex  vector
  template <class T>
  const std::vector<int>& sparseMatrix<T>::getColIndex() const{
    return colIndex;
  }


  // returns rowPtr vector
  template <class T>
  const lti::vector<int>& sparseMatrix<T>::getRowPtr() const{
    return rowPtr;
  }


  // returns size of the sparseMatrix
  template <class T>
  const point& sparseMatrix<T>::size() const{
    return theSize;
  }


  // return numNonSparseValues of the sparseMatrix
  template <class T>
  int sparseMatrix<T>::getNumNonSparseValues() const {
    return numNonSparseValues;
  }

  template <class T>
  T sparseMatrix<T>::getSparseValue() const {
    return sparseValue;
  }

  // *******************************************************//
  //   aritmetical operations
  // *******************************************************//

  /*
   * add a constant value to all elements of the matrix
   * and leave the result here
   * @param value the constant value
   * @return return a referenz to the actual matrix
   */
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::add(const T value){
    typename std::vector<T>::iterator i,e;
    e = values.end();

    //add sparseValue
    sparseValue += value;
    for(i=values.begin(); i!=e; ++i){
      (*i) += value;
    }
    return(*this);
  }


  /*
   * divide the elements of the matrix by a constant value
   * and leave the result here
   * @param value the constant value (divisor)
   * @return return a referenz to the actual matrix
   */
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::divide(const T value){
    typename std::vector<T>::iterator i,e;
    e = values.end();

    //devide sparseValue
    sparseValue /= value;
    for(i=values.begin(); i!=e; ++i){
      (*i) /= value;
    }
    return(*this);
  };


  /*
   * multiply the elements of the matrix by a constant value
   * and leave the result there
   * @param value the constant value
   * @return a referenz to the actual matrix
   */
  template <class T>
  sparseMatrix<T>& sparseMatrix<T>::multiply(const T value){
    typename std::vector<T>::iterator i,e;
    e = values.end();

    //multiply sparseValue
    sparseValue *= value;
    for(i=values.begin(); i!=e; ++i){
      (*i) *= value;
    }
    return(*this);
  };

  /*
   * multiply sparseMatrix with vector, write result
   * to a new vector
   *
   * @param srcVec, vector to be multiplied with,
   * Its dimension must be equal to the number of columns of the matrix.
   * @param destVec, vector to write result to
   * Its dimension will be equal to the number of rows of the matrix.
   */
  template <class T>
  void sparseMatrix<T>::multiply(const vector<T>& srcVec,
                                       vector<T>& destVec) const{
    // check dimension of srcVector with number of columns of the
    // sparseMatrix
    if (srcVec.size() != numCols) {
      return;
    }

    T sValMulVec;
    // multiply sparseValue with srcVector
    sValMulVec  = srcVec.sumOfElements() * sparseValue;

    // resize destVec to number of rows of the sparseMatrix
    // initialize with sValMulVec
    destVec.resize(numRows, sValMulVec, false, true);

    int numValInRow, posInColIndex;
    posInColIndex = 0;
    for (int r=0; r < numRows; ++r) {
      // calculate number of nonSparseValues in current row
      numValInRow = rowPtr[r+1]-rowPtr[r];
      T nonSValMulVecVal = T(0);
      for(int cp = posInColIndex; cp < posInColIndex+numValInRow; ++cp){
	// subtract sparseValue multiplied with corresponding
	// value in srcVector from value in row of destVec
        destVec[r] -= (sparseValue * srcVec[colIndex[cp]]);
	// acumulate nonSparseValue multiplied with corresponding
	// value in srcVector
	// colIndex[cp] returns column in sparseMatrix, which is
	// equal to row in srcVec
        nonSValMulVecVal += (values[cp]*srcVec[colIndex[cp]]);
      }
      // save position in colIndex vector
      posInColIndex += numValInRow;
      // add sum of nonSparseValue * srcVecValue to row in destVec
      destVec[r] += nonSValMulVecVal;
    }
  }


  /*
   * transpose sparseMatrix
   */
  template <class T>
  sparseMatrix<T> sparseMatrix<T>::transpose() {

    int currentRow, colCount;
    int* expandRow = new int[numNonSparseValues];

    std::vector<int>* valInCol = new std::vector<int>[numCols];
    currentRow = 0;
    colCount = 0;

    // expand rowPtr vector to size of colIndex vector
    // for each value in values Vector save at same position
    // in expanded rowPtr vector, number of row
    //
    // save position of values,
    // in the vectors of the vector array,
    // the index of the array is the column of the values
    while(currentRow < numRows){
      // cout << "Current Row: " << currentRow << endl;
      while(colCount < rowPtr[currentRow+1]){
	      // cout << "Position in colIndex: " << colCount << endl;
        expandRow[colCount]=currentRow;
        valInCol[colIndex[colCount]].push_back(colCount);
        colCount++;
      }
      currentRow++;
    }


    // create vectors of the transposed sparseMatrix
    // cout << "start filling new vectors ...\n";
    lti::vector<int> newRowPtr(numCols+1,0);
    std::vector<int> newColIndex(numNonSparseValues);
    std::vector<T> newValues(numNonSparseValues);
    int countNSV, numInRow;
    countNSV=0;
    numInRow=0;
    colCount=0;
    // iterate
    while (countNSV < numNonSparseValues) {
      //cout << "position in colindex: " << countNSV << endl;

      // while the there are elemnts in the vectors of the array
      // iterate over them to get position in the old sparseMatrix
      // vector (values, colIndex, expandRow)
      //
      // save values, colIndices in the new vectors
      // (newValues, newColIndex), the new cols are the old rows
      //
      // example:
      // values[valInCol[countCol][numInRow]]
      // values is the old values vector
      // valInCol[countCol] the array to use, to look up positions of values
      // [numInRow] index in the vector given by the array position

      while((unsigned)numInRow != valInCol[colCount].size()){
	//cout << "valInCol[colCount][numInRow] "
	//     << valInCol[colCount][numInRow] << endl;
	newValues[countNSV] = values[valInCol[colCount][numInRow]];
	newColIndex[countNSV] = expandRow[valInCol[colCount][numInRow]];
	countNSV++;
	numInRow++;
      }
      // get start position of a new row
      newRowPtr[colCount+1] = countNSV;
      colCount++;
      numInRow=0;
    }

    int newRows, newCols;
    newCols = numRows;
    newRows = numCols;
    rowPtr = newRowPtr;
    colIndex = newColIndex;
    values = newValues;
    theSize.y = newRows;
    theSize.x = newCols;

    delete[] expandRow;
    delete[] valInCol;
    return (*this);
  }


  /*
   * get maximum value of the Matrix
   * @return return a copy of this value
   */
  template <class T>
  T sparseMatrix<T>::maximum() const{
    typename std::vector<T>::const_iterator i,e;
    e = values.end();
    i = values.begin();
    T maximum = (*i);

    //iterate over values vector to find maximum
    for(; i!=e; ++i){
      if((*i) > maximum){
        maximum = (*i);
      }
    }
    if(sparseValue > maximum){
      maximum = sparseValue;
    }
    return maximum;
  }

  /*
   * get index of maximum value of the Matrix
   * @return return a copy of the index as a point
   */
  template <class T>
  point sparseMatrix<T>::getIndexOfMaximum() const{
    point idxOfMax;
    int positionOfMax, colIndOfMax, rowIndOfMax;
    T maximum;

    maximum = values[0];
    positionOfMax = 0;
    rowIndOfMax = 0;
    for(unsigned int v=0; v < values.size(); ++v){
      if(values[v] > maximum){
        maximum = values[v];
        positionOfMax = v;
      }
      while(positionOfMax >= rowPtr[rowIndOfMax+1]){
        ++rowIndOfMax;
      }
    }
    colIndOfMax = colIndex[positionOfMax];
    idxOfMax.x = colIndOfMax;
    idxOfMax.y = rowIndOfMax;
    return idxOfMax;
  };


  /*
   * get minimum value of the Matrix
   * @return return a copy of this value
   */
  template <class T>
  T sparseMatrix<T>::minimum() const{
    typename std::vector<T>::const_iterator i,e;
    e = values.end();
    i = values.begin();
    T minimum = (*i);

    //iterate over values vector to find maximum
    for(; i!=e; ++i){
      if((*i) < minimum){
        minimum = (*i);
      }
    }
    if(sparseValue < minimum){
      minimum = sparseValue;
    }
    return minimum;
  };



  /*
   * get index of minimum value of the Matrix
   * @return return a copy of the index as a point
   */
  template <class T>
  point sparseMatrix<T>::getIndexOfMinimum() const{
    point idxOfMin;
    int positionOfMin, colIndOfMin, rowIndOfMin;
    T minimum;

    minimum = values[0];
    positionOfMin = 0;
    rowIndOfMin = 0;
    for(unsigned int v=0; v < values.size(); ++v){
      if(values[v] < minimum){
        minimum = values[v];
        positionOfMin = v;
      }
      while(positionOfMin >= rowPtr[rowIndOfMin+1]){
        ++rowIndOfMin;
      }
    }
    colIndOfMin = colIndex[positionOfMin];
    idxOfMin.x = colIndOfMin;
    idxOfMin.y = rowIndOfMin;
    return idxOfMin;
  };


  //**************** private function for internal use ********//

  /*
   * !!! for internal use only !!
   *
   * setSparseValue, insert sparseValue at given row and column
   * return false if already sparse value or true, if the found element
   * was deleted.
   *
   */
  template <class T>
  bool sparseMatrix<T>::setSparseValue(const int& row,
                                       const int& col) {

    assert( static_cast<uint32>(row) < static_cast<uint32>(numRows) );
    assert( static_cast<uint32>(col) < static_cast<uint32>(numCols) );

    _lti_debug("start setSparseValue(" << row << "," << col <<")\n");

    int middle; // used for binary search
    int leftBorder = rowPtr[row];
    int rightBorder = rowPtr[row+1]-1;

    if (rightBorder < leftBorder) {
      // the row is empty.  Return sparse value!
      return false;
    }
    else {
      // binary search on colIndex-vector
      while (rightBorder-leftBorder > 4) { // magic number 4 means,
                                           // that only for intervals
                                           // bigger than 5 we should
                                           // try the binary search.
                                           // Otherwise do a linear
                                           // search.
        middle = (leftBorder+rightBorder)/2;
        if (col == colIndex[middle]) {
          rightBorder=leftBorder=middle;
        } else if (col < colIndex[middle]) {
          rightBorder=middle;
        } else {
          leftBorder=middle;
        }
      }

      // linear search on colIndex-vector
      while (leftBorder<=rightBorder) {
        if (colIndex[leftBorder]==col) {
          typename std::vector<T>::iterator vit=values.begin()+leftBorder;
          typename std::vector<int>::iterator cit=colIndex.begin()+leftBorder;
          values.erase(vit);
          colIndex.erase(cit);
          typename lti::vector<int>::iterator it = rowPtr.begin()+(row+1);
          typename lti::vector<int>::iterator eit = rowPtr.end();
          for (;it!=eit;++it) {
            (*it)--;
          }
          numNonSparseValues--;
          return true;
        }
        leftBorder++;
      }
    }

    return false; // never reached
  }

  /*
   * !!! for internal use only !!!
   *
   * fillSparseValue, fill region of sparseMatrix with sparseValue
   *
   */
  template <class T>
  bool sparseMatrix<T>::fillSparseValue(const int& fromRow, const int& fromCol,
                                        const int& toRow, const int& toCol) {

    int c,r;
    for (r=fromRow;r<=toRow;++r) {
      for (c=fromCol;c<=toCol;++c) {
        setSparseValue(r,c);
      }
    }

    return true;
  }

  /*
   * write the object in the given ioHandler
   */
  template <class T>
  bool sparseMatrix<T>::write(ioHandler& handler,
                              const bool complete) const {

    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b & lti::write(handler,"size",theSize);
    b = b & lti::write(handler,"numNonSparseValues",numNonSparseValues);
    b = b & lti::write(handler,"sparseValue",sparseValue);
    b = b & lti::write(handler,"values",values);
    b = b & lti::write(handler,"colIndex",colIndex);
    b = b & lti::write(handler,"rowPtr",rowPtr);

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /*
   * read the object from the given ioHandler
   */
  template <class T>
  bool sparseMatrix<T>::read(ioHandler& handler,const bool complete) {
    bool b = true;

    if (complete) {
      b = handler.readBegin();
    }

    b = b & lti::read(handler,"size",theSize);
    b = b & lti::read(handler,"numNonSparseValues",numNonSparseValues);
    b = b & lti::read(handler,"sparseValue",sparseValue);
    b = b & lti::read(handler,"values",values);
    b = b & lti::read(handler,"colIndex",colIndex);
    b = b & lti::read(handler,"rowPtr",rowPtr);

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;

  }

}; // end of namespace LTI

/// ASCII output of a matrix to a std stream.
namespace std {

  template <class T>
  ostream& operator<<(ostream& outStr, const lti::sparseMatrix<T>& sMatrix) {

    /* This is correct, but does not compile with MS VC:
    const std::vector<T>&   outValues(sMatrix.getValues());
    const std::vector<int>& outColInd(sMatrix.getColIndex());
    const lti::vector<int>& outRowPtr(sMatrix.getRowPtr());
    */
	//so we have to use an ugly workaround:
	const std::vector<T>* outValues=new std::vector<T>(sMatrix.getValues());
    const std::vector<int>* outColInd=new std::vector<int>(sMatrix.getColIndex());
    const lti::vector<int>* outRowPtr=new lti::vector<int>(sMatrix.getRowPtr());


    if (outValues->size() > 0){
      outStr << "Values    :";
      for(unsigned int i=0; i < outValues->size(); i++){
        outStr << "\t" << (*outValues)[i];
      }
      outStr << std::endl <<"colIndex  :";
      for(unsigned int k=0; k < outColInd->size(); k++){
        outStr << "\t" << (*outColInd)[k];
      }
      outStr << "\n\nrowPtr    :";
      for(int j=0; j < outRowPtr->size(); j++){
        outStr << "\t" << (*outRowPtr)[j];
      }
      outStr << std::endl << "numNonSparseValues :";
      outStr << sMatrix.getNumNonSparseValues()
             << std::endl << std::endl;
      outStr << "\n\nSparse Value:" << sMatrix.getSparseValue() << endl;
    }
    else {
      outStr << "Sparse Matrix " << sMatrix.size()
             << " filled with " << sMatrix.getSparseValue() << std::endl;
    }
	// ugly workaround, part 2:
	delete outValues;
	delete outColInd;
	delete outRowPtr;
    return outStr;
  }
}


