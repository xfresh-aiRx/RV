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
 * file .......: ltiHistogram_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiHistogram_template.h,v 1.14 2005/06/16 12:37:34 doerfler Exp $
 */

#include "ltiHistogram.h"

namespace lti {


  /*
   * default constructor creates an empty histogram;
   */
  template<class T>
  thistogram<T>::thistogram()
    : mathObject(), totalNumberOfCells(0),numberOfEntries(0),
      dimensionality(0) {
  }

  /*
   * create a histogram of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the histogram
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the histogram.
   * @param cells the number of cells per dimension.
   */
  template<class T>
  thistogram<T>::thistogram(const int& dimensions,const int& cells)
    : mathObject(), totalNumberOfCells(0),numberOfEntries(0),
      dimensionality(0) {
    resize(dimensions,cells);
  }

  /*
   * create a histogram with the given dimensionality.
   *
   * Each dimension \e i will have the number of cells indicated in
   * the \e i-th element of the vector <code>cells</code>.
   *
   * If the dimensionaly differs from the size of the given vector, the
   * number of cells of the dimension \e i will be given by
   * \f$dim_i = cells[i \mod cells.size()].
   *
   * This means, if you want a 6-dimensional histogram, and your cells-vector
   * has only three elements [10,15,5], the number of cells per dimension
   * will be [10,15,5,10,15,5]
   *
   * @param dimensions the dimensionality of the histogram
   * @param cells a vector with the number of cell per dimension
   */
  template<class T>
  thistogram<T>::thistogram(const int& dimensions,const ivector& cells)
    : mathObject(), totalNumberOfCells(0),numberOfEntries(0),
      dimensionality(0) {
    resize(dimensions,cells);
  }

  /*
   * create this histogram as a copy of another histogram
   * @param other the histogram to be copied.
   */
  template<class T>
  thistogram<T>::thistogram(const thistogram<T>& other)
    : mathObject() {
    copy(other);
  }

  /*
   * destructor
   */
  template<class T>
  thistogram<T>::~thistogram() {
  }

  /*
   * change dimensionality and cell number of the histogram.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram
   * @param cells      the number of cells per dimension
   *
   */
  template<class T>
  void thistogram<T>::resize(const int& dimensions,
                         const int& cells) {
    // a temporary dimensionality vector
    ivector tmp(dimensions,cells);
    // resize
    resize(dimensions,tmp);
  }

  /*
   * change dimensionality and cell number of the histogram.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram
   * @param cells      the number of cells per dimension
   *
   */
  template<class T>
  void thistogram<T>::resize(const int& dim,
                         const ivector& cells) {
    // should this histogram be cleaned up?
    if ((dim < 1) || (cells.size() < 1)) {
      totalNumberOfCells = 0;
      numberOfEntries = 0;
      dimensionality = 0;
      theHistogram.clear();
      theCellsPerDimension.clear();
      firstCell.clear();
      lastCell.clear();
      return;
    }

    // determine how many cells the new histogram should have
    int i,totalCells;

    theCellsPerDimension.resize(dim);
    theCellsPerDimension.at(0)=cells.at(0);

    for (totalCells=cells.at(0),i=1;
         i<dim;
         i++) {
      totalCells *= cells.at(i % dim);
      theCellsPerDimension.at(i)=cells.at(i % dim);
    }

    // resize and initialize the vector
    theHistogram.resize(totalCells,0,false,true);
    totalNumberOfCells = totalCells;

    numberOfEntries = 0;
    dimensionality = dim;
    firstCell.resize(dim,0);
    lastCell.copy(theCellsPerDimension);
    lastCell.add(-1); // subtract 1 from all elements
  }

  template<class T>
  void thistogram<T>::clear() {
    resize(0,0);
  }

  /*
   * initialize all cells of the histogram with 0 (or another specified
   * number).
   */
  template<class T>
  void thistogram<T>::initialize(const T& value) {
    fill(value);
  }


  /*
   * Normalize the histogram and then denormalize it with the given number
   * of entries
   */
  template<class T>
  void thistogram<T>::setNumberOfEntries(const T& newNumberOfEntries) {

    T tmp = static_cast<T>(0);
    if (numberOfEntries != 0) {
      tmp = newNumberOfEntries/numberOfEntries;
    }

    theHistogram.multiply(tmp);

//      normalize();
//      theHistogram.multiply(newNumberOfEntries);
    numberOfEntries = newNumberOfEntries;
  }

  /*
   * counts the number of entries in the whole histogram and sets
   * the internal counter for the total number of entries.
   * if some direct access to the cell contents
   * have been done, you should update the number of entries with this
   * function
   */
  template<class T>
  void thistogram<T>::updateNumberOfEntries() {
    const_iterator it,e;
    T entries = 0;

    for (it = begin(),e=end();
         it!=e;
         it++) {
      entries += (*it);
    }

    numberOfEntries = entries;
  };

  /*
   * fills the histogram elements with <code>iniValue</code> between
   * the n-dimensional points <code>from</code> and <code>to</code>.
   * @param iniValue the elements will be initialized with this
   *                 value.
   * @param from     first element index
   * @param to       last element index
   *
   * If <code>from</code> or <code>to</code> are out of bounds,
   * they will be (internaly) adjusted to a correct value.
   *
   * Example:
   * \code
   *   lti::histogram hist(1,10); // 1D-histogram with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  template<class T>
  void thistogram<T>::fill(const T& iniValue,
                           const ivector& from,
                           const ivector& to) {

    ivector first;
    ivector last;

    if (from.size() == 0) {
      first.copy(firstCell);
    } else {
      first.copy(from);
    }

    if (to.size() == 0) {
      last.copy(lastCell);
    } else {
      last.copy(to);
    }

    if ((first.size() != last.size()) || (first.size() != dimensionality)) {
      // invalid dimensionality
      return;
    }

    // can I fill everything?
    if (first.equals(firstCell) && last.equals(lastCell)) {
      theHistogram.fill(iniValue);
      numberOfEntries = iniValue*totalNumberOfCells;
      return;
    }

    int i,mi,ma;
    T tmp;

    // check and correct the boundaries
    for (i=0;i<dimensionality;i++) {
      mi = min(first.at(i),last.at(i));
      ma = max(first.at(i),last.at(i));

      first.at(i)= max( mi, firstCell.at(i) );
      last.at(i) = min( ma, lastCell.at(i)  );
    }

    ivector idx(first); // the index! this will iterate on the histogram
                        // hyperbox

    bool ready = false;

    // to check if index at the end, we just need to check the last
    // dimension of the index
    while ( !ready ) {

      // update of numberOfEntries
      tmp = iniValue - at(idx); // update of entry number due to this cell
      numberOfEntries += tmp;

      // set the cell value
      at(idx) = iniValue;

      // idx update
      i = 0;
      while (i<dimensionality) {
        idx.at(i)++;
        if (idx.at(i)>last.at(i)) {
          idx.at(i)=first.at(i);
          i++;
          if (i==dimensionality) {
            ready = true;
          }
        } else {
          i=dimensionality; // ready flag!
        }
      }
    }
  }

  /*
   * read-only access to the element x of the histogram
   * @param x index of the histogram element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  template<class T>
  const T& thistogram<T>::at(const ivector& x) const {
    return theHistogram.at(vectorToIndex(x));
  }

  /*
   * access element x of the histogram
   * @param x index of the histogram element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  template<class T>
  T& thistogram<T>::at(const ivector& x) {
    return theHistogram.at(vectorToIndex(x));
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram.
   * @param x         index of the histogram element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  template<class T>
  const T& thistogram<T>::put(const ivector& x,const T& increment) {
    int idx(vectorToIndex(x));
    theHistogram.at(idx)+=increment;
    numberOfEntries += increment;
    return theHistogram.at(idx);
  }

  /*
   * read-only access to the element x of the histogram as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram
   *         data.
   */
  template<class T>
  double thistogram<T>::getProbability(const ivector& x) const {
    int idx(vectorToIndex(x));
    return static_cast<double>(theHistogram.at(idx))/numberOfEntries;
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source histogram to be copied.
   * @return a reference to this object
   */
  template<class T>
  thistogram<T>& thistogram<T>::copy(const thistogram<T>& other) {

    totalNumberOfCells = other.totalNumberOfCells;
    numberOfEntries = other.numberOfEntries;
    dimensionality = other.dimensionality;
    theHistogram.copy(other.theHistogram);
    theCellsPerDimension.copy(other.theCellsPerDimension);
    firstCell.copy(other.firstCell);
    lastCell.copy(other.lastCell);

    return *this;
  }

  /*
   * create a clone of this histogram
   * @return a pointer to a copy of this histogram
   */
  template<class T>
  mathObject* thistogram<T>::clone() const {
    return new thistogram<T>(*this);
  }

  /*
   * compare this histogram with another one.
   *
   * @param other the other histogram to be compared with
   * @return true if both histograms have the same elements and same size
   */
  template<class T>
  bool thistogram<T>::equals(const thistogram<T>& other) const {
    return (( totalNumberOfCells == other.totalNumberOfCells) &&
            ( dimensionality == other.dimensionality) &&
            ( theHistogram.equals(other.theHistogram) ) &&
            ( theCellsPerDimension.equals(other.theCellsPerDimension)));
  }

  /*
   * compare this histogram with another one, and use the given tolerance to
   * determine if the value of each element of the other histogram
   * approximately equals the values of the actual histogram elements.
   *
   * An element \e x is approximately equal to another element \e y
   * with a tolerance \e t, if following equation holds:
   * <i>x</i>-t < <i>y</i> < <i>x</i>+t
   *
   * @param other the other histogram to be compared with
   * @param tolerance the tolerance to be used
   *
   * @return true if both histograms are approximatly equal
   */
  template<class T>
  bool thistogram<T>::prettyCloseTo(const thistogram<T>& other,
                                const T& tolerance) const {
    return (( totalNumberOfCells == other.totalNumberOfCells) &&
            ( dimensionality == other.dimensionality) &&
            ( theHistogram.prettyCloseTo(other.theHistogram,tolerance )) &&
            ( theCellsPerDimension.equals(other.theCellsPerDimension)));
  }

  /*
   * applies a C-function to each element of the histogram.
   * @param function a pointer to a C-function
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::apply(T (*function)(T)) {
    theHistogram.apply(function);
    return (*this);
  }

  /*
   * applies a C-function to each element of the histogram.
   * @param function a pointer to a C-function
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::apply(T (*function)(const T&)) {
    theHistogram.apply(function);
    return (*this);
  }

  /*
   * Elementwise multiplication.
   *
   * Each element of this histogram will be multiplied with the elements
   * of the other histogram and the result will be left in this %object!
   *
   * Both histograms are first normalized, then multiplied. After the
   * multiplication, this histogram cannot be interpreted anymore as a
   * histogram, but as a combined probability distribution.  You can
   * use setNumberOfEntries() to change this fact under your own risk
   * (the semathical meaning of that is left to you!).
   *
   * The returned histogram is this %object!
   *
   * @param other the other histogram to be multiplied with
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::emultiply(const thistogram<T>& other) {
    assert ( totalNumberOfCells == other.totalNumberOfCells );
    assert ( dimensionality == other.dimensionality);
    int i;

    for (i=0;i<totalNumberOfCells;i++) {
      theHistogram.at(i)/=numberOfEntries;
      theHistogram.at(i)*=(other.theHistogram.at(i)/other.numberOfEntries);
    }

    numberOfEntries=theHistogram.sumOfElements();

    return *this;
  }

  /*
   * Elementwise multiplication.
   * This histogram will contain the elementwise multiplication of the
   * elements in <code>first</code> and <code>second</code>.
   *
   * Both histograms are first normalized, then multiplied, and the number of
   * entries is after the multiplication 1!  After this multiplication, this
   * histogram cannot be anymore interpreted as a histogram, but as a
   * combined probabilty distribution.  You can use setNumberOfEntries() to
   * change this fact under your own risk (the semathical meaning of that is
   * left to you!).
   *
   * @param first the first histogram
   * @param second the second histogram will be multiplied with the
   *               first histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::emultiply(const thistogram<T>& first,
                                          const thistogram<T>& second) {
    assert ( first.totalNumberOfCells == second.totalNumberOfCells);
    assert ( first.dimensionality     == second.dimensionality);

    int i;
    theHistogram.copy(first.theHistogram);

    for (i=0;i<totalNumberOfCells;i++) {
      theHistogram.at(i)/=numberOfEntries;
      theHistogram.at(i)*=(second.theHistogram.at(i)/second.numberOfEntries);
    }


    totalNumberOfCells = first.totalNumberOfCells;
    numberOfEntries=theHistogram.sumOfElements();
    dimensionality = first.dimensionality;
    theCellsPerDimension.copy(first.theCellsPerDimension);
    firstCell.copy(first.firstCell);
    lastCell.copy(first.lastCell);

    return *this;
  }

  /*
   * Add another histogram of the same type and same dimension and
   * leave the result in this %object.  The number of entries of both
   * histograms are also added.
   *
   * @param other the other histogram to be added with
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::add(const thistogram<T>& other) {
    assert ( totalNumberOfCells == other.totalNumberOfCells );
    assert ( dimensionality     == other.dimensionality);

    theHistogram.add(other.theHistogram);
    numberOfEntries+=other.numberOfEntries;

    return *this;
  }

  /*
   * Add two histogram and leave the result in this %object.
   * @param first the first histogram.  The number of entries of both
   * histograms are also added.
   * @param second the second histogram will be added with the first
   *               histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::add(const thistogram<T>& first,
                                    const thistogram<T>& second) {
    assert ( first.totalNumberOfCells == second.totalNumberOfCells);
    assert ( first.dimensionality     == second.dimensionality);
   
    theHistogram.add(first.theHistogram,second.theHistogram);
    totalNumberOfCells = first.totalNumberOfCells;
    numberOfEntries = first.numberOfEntries+second.numberOfEntries;
    dimensionality = first.dimensionality;
    theCellsPerDimension.copy(first.theCellsPerDimension);
    firstCell.copy(first.firstCell);
    lastCell.copy(first.lastCell);
    
    return *this;
  }

  /*
   * Subtracts another histogram of the same type and same dimension
   * and leaves the result in this %object
   *
   * @param other will be substracted from this histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::subtract(const thistogram<T>& other) {
    assert ( totalNumberOfCells == other.totalNumberOfCells );
    assert ( dimensionality     == other.dimensionality);

    theHistogram.subtract(other.theHistogram);
    numberOfEntries-=other.numberOfEntries;

    return *this;
  }

  /*
   * Subtracts two histograms and leaves the result in this %object.
   * @param first the first histogram
   * @param second the second histogram will be substracted from the
   *                   first histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::subtract(const thistogram<T>& first,
                                         const thistogram<T>& second) {
    assert ( first.totalNumberOfCells == second.totalNumberOfCells );
    assert ( first.dimensionality     == second.dimensionality );
    
    theHistogram.subtract(first.theHistogram,second.theHistogram);

    totalNumberOfCells = first.totalNumberOfCells;
    numberOfEntries = first.numberOfEntries-second.numberOfEntries;
    dimensionality = first.dimensionality;
    theCellsPerDimension.copy(first.theCellsPerDimension);
    firstCell.copy(first.firstCell);
    lastCell.copy(first.lastCell);

    return *this;
  }

  /*
   * Multiply this histogram with a constant.
   * Returns this histogram.  The total number of entries will also be
   * updated. Note that if you use this operation, the number of entries
   * could be false at the end due to numerical instabilities.
   *
   * @see setNumberOfEntries()
   *
   * @param cst constant scalar to be multiplied with
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::multiply(const T& cst) {
    const T cstm1 = cst - static_cast<T>(1);

    for (int i=0;i<totalNumberOfCells;i++) {
      numberOfEntries+=(theHistogram.at(i)*cstm1);
      theHistogram.at(i)*=cst;
    }

    return *this;
  }

  /*
   * Multiply the other %histogram with a constant and leave the result here.
   * Returns a reference to this histogram.
   * Note that if you use this operation, the number of entries
   * could be false at the end due to numerical instabilities.
   * @see setNumberOfEntries()
   *
   * @param other the other histogram to be multiplied with the
   *              constant value
   * @param cst constant scalar to be multiplied with the other histogram.
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::multiply(const thistogram<T>& other,
                                         const T& cst) {
    copy(other);
    return multiply(cst);
  }

  /* free the data of this object and attach it to the "receiver".
   *
   * It is a very efficient way to make a copy of this histogram, if you
   * don't need the source data anymore!
   *
   * At the end of the detachment, this histogram will be empty.
   * @param receiver the histogram which will receive the memory.
   *                 this histogram!
   */
  template<class T>
  void thistogram<T>::detach(thistogram<T>& receiver) {
    receiver.totalNumberOfCells = totalNumberOfCells;
    receiver.numberOfEntries = numberOfEntries;
    receiver.dimensionality = dimensionality;

    totalNumberOfCells = 0;
    numberOfEntries = 0;
    dimensionality = 0;

    theHistogram.detach(receiver.theHistogram);
    theCellsPerDimension.detach(receiver.theCellsPerDimension);
    firstCell.detach(receiver.firstCell);
    lastCell.detach(receiver.lastCell);
  }

  /*
   * Divide this histogram with a constant.
   * Returns this histogram.  @see setNumberOfEntries()
   *
   * @param cst the elements of the histogram will be divided with this
   *            constant
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::divide(const T& cst) {
    const double cstm1 = (1.0/double(cst)) - 1.0;
    double delta = 0;

    for (int i=0;i<totalNumberOfCells;i++) {
      delta+=cstm1*theHistogram.at(i);
      theHistogram.at(i)/=cst;
    }
    numberOfEntries+=static_cast<T>(delta);
    return *this;
  }

  /*
   * Divide the other histogram with a constant and leave the result here.
   * Returns a reference to this histogram.
   * @param other the histogram to be divide by the constant value
   * @param cst the elements of the histogram will be divided with this
   *            constant
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::divide(const thistogram<T>& other,
                                       const T& cst) {
    copy(other);
    return divide(cst);
  }

  /*
   * Add constant to this histogram.  This histogram is changed.
   * Returns this histogram.
   * @param cst constant scala to be added with each element
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::add(const T& cst) {
    theHistogram.add(cst);
    numberOfEntries += (cst*totalNumberOfCells);
    return *this;
  }

  /*
   * Add constant to the other histogram and leave the result here.
   * Returns a reference to this histogram.
   * @param other the oder histogram
   * @param cst constant scala to be added with each element of the other
   *            histogram
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::add(const thistogram<T>& other,const T& cst) {
    copy(other);
    return add(cst);
  }

  /*
   * normalize the histogram
   * the total number of entries will be set to 1.0
   * @return a reference to the actual histogram
   */
  template<class T>
  thistogram<T>& thistogram<T>::normalize(const bool forceUpdateOfNumEntries) {
    
    if (forceUpdateOfNumEntries) {
      updateNumberOfEntries();
    }

    theHistogram.divide(numberOfEntries);
    numberOfEntries = static_cast<T>(1.0);

    return *this;
  }


  /**
   * write the object in the given ioHandler
   */
  template<class T>
  bool thistogram<T>::write(ioHandler& handler,const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
      lti::write(handler,"totalNumberOfCells",totalNumberOfCells);
      lti::write(handler,"numberOfEntries",numberOfEntries);
      lti::write(handler,"dimensionality",dimensionality);
      lti::write(handler,"data",theHistogram);
      lti::write(handler,"cellsPerDimension",theCellsPerDimension);
      lti::write(handler,"firstCell",firstCell);
      lti::write(handler,"lastCell",lastCell);
    }

    if (complete) b = handler.writeEnd();
    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template<class T>
  bool thistogram<T>::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
      lti::read(handler,"totalNumberOfCells",totalNumberOfCells);
      lti::read(handler,"numberOfEntries",numberOfEntries);
      lti::read(handler,"dimensionality",dimensionality);
      lti::read(handler,"data",theHistogram);
      lti::read(handler,"cellsPerDimension",theCellsPerDimension);
      lti::read(handler,"firstCell",firstCell);
      lti::read(handler,"lastCell",lastCell);
    }

    if (complete) b = handler.readEnd();
    return b;
  }


  // -----------------------------------------------------------------
  //             HISTOGRAM 1D
  // -----------------------------------------------------------------

  /*
   * default constructor creates an empty histogram1D;
   */
  histogram1D::histogram1D()
    : histogram(), firstCell(0), lastCell(0) {
  }

  /*
   * create a histogram1D of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the histogram1D
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the histogram1D.
   * @param cells the number of cells per dimension.
   */
  histogram1D::histogram1D(const int& cells)
    : histogram(), firstCell(0), lastCell(0)  {
    resize(cells);
  }

  /*
   * create this histogram1D as a copy of another histogram1D
   * @param other the histogram1D to be copied.
   */
  histogram1D::histogram1D(const histogram1D& other)
    : histogram() {
    copy(other);
  }

  /*
   * destructor
   */
  histogram1D::~histogram1D() {
  }

  /*
   * change dimensionality and cell number of the histogram1D.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram1D
   * @param cells      the number of cells per dimension
   *
   */
  void histogram1D::resize(const int& cells) {
    // a temporary dimensionality vector
    ivector tmp(1,cells);
    firstCell = 0;
    lastCell = cells-1;
    // resize
    histogram::resize(1,tmp);
  }

  /*
   * initialize all cells of the histogram1D with 0 (or another specified
   * number).
   */
  void histogram1D::initialize(const histogram::value_type& value) {
    fill(value);
  }

  /*
   * fills the histogram1D elements with <code>iniValue</code> between
   * the n-dimensional points <code>from</code> and <code>to</code>.
   * @param iniValue the elements will be initialized with this
   *                 value.
   * @param from     first element index
   * @param to       last element index
   *
   * If <code>from</code> or <code>to</code> are out of bounds,
   * they will be (internaly) adjusted to a correct value.
   *
   * Example:
   * \code
   *   lti::histogram1D hist(1,10); // 1D-histogram1D with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  void histogram1D::fill(const histogram::value_type& iniValue,
                         const int& from,
                         const int& to) {


    int f,t;
    int idx;
    double tmp;

    // check and correct the for the boundaries
    f = max( min(from,to) , firstCell );
    t = min( max(from,to) , lastCell  );

    // can I fill everything?
    if ((f==firstCell) && (t==lastCell)) {
      theHistogram.fill(iniValue);
      numberOfEntries = totalNumberOfCells*iniValue;
      return;
    }

    idx = f; // the index! this will iterate on the histogram1D
             // hyperbox

    // to check if index at the end, we just need to check the last
    // dimension of the index
    for (idx=f;idx<=t;idx++) {
      // update of entry number due to this cell
      tmp = iniValue - theHistogram.at(idx);
      numberOfEntries += tmp;

      // set the cell value
      theHistogram.at(idx) = iniValue;
    }
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source histogram1D to be copied.
   * @return a reference to this object
   */
  histogram1D& histogram1D::copy(const histogram1D& other) {

    histogram::copy(other);
    firstCell =  other.firstCell;
    lastCell  =  other.lastCell;

    return *this;
  }

  /*
   * create a clone of this histogram1D
   * @return a pointer to a copy of this histogram1D
   */
  mathObject* histogram1D::clone() const {
    return new histogram1D(*this);
  }

  /**
   * write the object in the given ioHandler
   */
  bool histogram1D::write(ioHandler& handler,
                          const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
//       lti::write(handler,"firstCell",firstCell);
//       lti::write(handler,"lastCell",lastCell);
      histogram::write(handler,false);
    };

    if (complete) b = handler.writeEnd();
    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  bool histogram1D::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
//       lti::read(handler,"firstCell",firstCell);
//       lti::read(handler,"lastCell",lastCell);
      histogram::read(handler,false);
      if (dimensionality==1) {
        firstCell = histogram::firstCell.at(0);
        lastCell  = histogram::lastCell.at(0);
      } else {
        handler.setStatusString("Not a 1D histogram.");
        b=false;
      }
    }

    if (complete) b = handler.readEnd();
    return b;
  }

  // -----------------------------------------------------------------
  //             HISTOGRAM 2D
  // -----------------------------------------------------------------



  /*
   * default constructor creates an empty histogram2D;
   */
  histogram2D::histogram2D()
    : histogram(), firstCell(point(0,0)), lastCell(point(0,0)) {
  }

  /*
   * create a histogram2D of the given dimensionality.
   *
   * Each dimension will have the given number of cells, i.e. the histogram2D
   * will have \f$cells^dimensions\f$ cells.
   *
   * @param dimensions the dimensionality of the histogram2D.
   * @param cells the number of cells per dimension.
   */
  histogram2D::histogram2D(const int& cells)
    : histogram(), firstCell(0), lastCell(0)  {
    resize(cells,cells);
  }

  histogram2D::histogram2D(const int& cellsY,const int& cellsX)
    : histogram(), firstCell(0), lastCell(0)  {
    resize(cellsY,cellsX);
  }

  histogram2D::histogram2D(const point& cells)
    : histogram(), firstCell(0), lastCell(0)  {
    resize(cells);
  }

  /*
   * create this histogram2D as a copy of another histogram2D
   * @param other the histogram2D to be copied.
   */
  histogram2D::histogram2D(const histogram2D& other)
    : histogram() {
    copy(other);
  }

  /*
   * destructor
   */
  histogram2D::~histogram2D() {
    theHistoMatrix.resize(0,0,0.0f,false,false);
  }

  /*
   * change dimensionality and cell number of the histogram2D.  All data will
   * be lost!
   *
   * @param dimensions the new dimensionality of the histogram2D
   * @param cells      the number of cells per dimension
   *
   */
  void histogram2D::resize(const int& cellsY,const int& cellsX) {
    // a temporary dimensionality vector
    ivector tmp(2,cellsY);
    tmp.at(0)=cellsX;
    firstCell = point(0,0);
    lastCell =  point(cellsX-1,cellsY-1);
    // resize
    histogram::resize(2,tmp);
    theHistoMatrix.useExternData(cellsY,cellsX,&theHistogram.at(0));
  }

  void histogram2D::resize(const point& cells) {
    resize(cells.y,cells.x);
  }

  /*
   * initialize all cells of the histogram2D with 0 (or another specified
   * number).
   */
  void histogram2D::initialize(const histogram::value_type& value) {
    fill(value);
  }

  /*
   * fills the histogram2D elements with <code>iniValue</code> between
   * the n-dimensional points <code>from</code> and <code>to</code>.
   * @param iniValue the elements will be initialized with this
   *                 value.
   * @param from     first element index
   * @param to       last element index
   *
   * If <code>from</code> or <code>to</code> are out of bounds,
   * they will be (internaly) adjusted to a correct value.
   *
   * Example:
   * \code
   *   lti::histogram2D hist(1,10); // 1D-histogram2D with 10 elements
   *   hist.clear;
   *   hist.fill(9,ivector(1,1),ivector(1,3)); // hist=[0,9,9,9,0,0,0,0,0,0]
   * \endcode
   */
  void histogram2D::fill(const histogram::value_type& iniValue,
                         const point& from,
                         const point& to) {


    point f,t;
    point idx;
    double tmp;

    // check and correct the for the boundaries
    f.x = max( min(from.x,to.x) , firstCell.x );
    t.x = min( max(from.x,to.x) , lastCell.x  );
    f.y = max( min(from.y,to.y) , firstCell.y );
    t.y = min( max(from.y,to.y) , lastCell.y  );

    // can I fill everything?
    if ((f==firstCell) && (t==lastCell)) {
      theHistogram.fill(iniValue);
      numberOfEntries = totalNumberOfCells*iniValue;
      return;
    }

    idx = f; // the index! this will iterate on the histogram2D
             // hyperbox

    // to check if index at the end, we just need to check the last
    // dimension of the index
    for (idx.y=f.y;idx.y<=t.y;idx.y++) {
      for (idx.x=f.x;idx.x<=t.x;idx.x++) {

        // update of entry number due to this cell
        tmp = iniValue - theHistoMatrix.at(idx);
        numberOfEntries += tmp;

        // set the cell value
        theHistoMatrix.at(idx) = iniValue;
      }
    }
  }

  /*
   * assigment operator.
   * copy the contents of <code>other</code> in this %object.
   * @param other the source histogram2D to be copied.
   * @return a reference to this object
   */
  histogram2D& histogram2D::copy(const histogram2D& other) {

    histogram::copy(other);
    firstCell =  other.firstCell;
    lastCell  =  other.lastCell;
    theHistoMatrix.useExternData(lastCell.y+1,lastCell.x+1,
                                 &theHistogram.at(0));
    return *this;
  }

  /*
   * create a clone of this histogram2D
   * @return a pointer to a copy of this histogram2D
   */
  mathObject* histogram2D::clone() const {
    return new histogram2D(*this);
  }

  /**
   * write the object in the given ioHandler
   */
  bool histogram2D::write(ioHandler& handler,
                          const bool complete) const {
    bool b = true;
    if (complete) b = handler.writeBegin();

    if (b) {
//       lti::write(handler,"firstCell",firstCell);
//       lti::write(handler,"lastCell",lastCell);
      histogram::write(handler,false);
    };

    if (complete) b = handler.writeEnd();

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  bool histogram2D::read(ioHandler& handler,const bool complete) {
    bool b = true;
    if (complete) b = handler.readBegin();

    if (b) {
//       lti::read(handler,"firstCell",firstCell);
//       lti::read(handler,"lastCell",lastCell);
      histogram::read(handler,false);
      if (dimensionality==2) {
        firstCell.x = histogram::firstCell.at(0);
        firstCell.y = histogram::firstCell.at(1);
        lastCell.x  = histogram::lastCell.at(0);
        lastCell.y  = histogram::lastCell.at(1);
      } else {
        handler.setStatusString("Not a 2D histogram");
        b=false;
      }
      theHistoMatrix.useExternData(theCellsPerDimension.at(1),
                                   theCellsPerDimension.at(0),
                                   &theHistogram.at(0));
    }

    if (complete) b = handler.readEnd();
    return b;
  }


} // namespace lti

/// outputs the elements of the histogram on a stream
template <class T>
std::ostream& operator<<(std::ostream& s,const lti::thistogram<T>& v) {

  lti::ivector idx(v.getFirstCell()); // the index! this will iterate on
                                      // the histogram hyperbox

  const lti::ivector& first = v.getFirstCell();
  const lti::ivector& last = v.getLastCell();

  // index of the last dimension
  const int dims = v.dimensions();
  int i,j;
  bool ready = false;

  // string with spaces!
  char* spc = new char[dims+1];
  for (i=0;i<dims;i++) {
    spc[i]=' ';
  }
  spc[dims]=0;

  j = dims;

  // to check if index at the end, we just need to check the last
  // dimension of the index
  while ( !ready ) {

    // j indicates how many "(" must be written

    if (j>0) {
      s << std::endl;

      for (i=dims;i>0;i--) {
        if (i>j)
          s << ' ';
        else
          s << '(';
      }
    }

    s << v.at(idx) << "\t ";

    // idx update
    i = j = 0;

    while (i<dims) {
      idx.at(i)++;
      if (idx.at(i)>last.at(i)) {
        s << ")";
        j++;
        idx.at(i)=first.at(i);
        i++;
        if (i==dims) {
          ready = true;
        }
      } else {
        i=dims; // ready flag!
      }
    }

    // j indicates how many ")" were written.
  }

  return s;
}

