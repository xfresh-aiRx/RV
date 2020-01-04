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
 * file .......: ltiHistogram_inline.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 29.06.00
 * revisions ..: $Id: ltiHistogram_inline.h,v 1.16 2005/06/16 12:37:34 doerfler Exp $
 */

namespace lti {
  template<class T>
  inline const T& thistogram<T>::outerBoundsCell() {
    static const T val(0);
    return val;
  }

  /*
   * returns the number of dimensions of this histogram
   */
  template<class T>
  inline int thistogram<T>::dimensions() const {
    return dimensionality;
  };

  /*
   * get the number of cells of the dimension <code>dim</code>
   * @param dimension the index of the dimension to be checked
   * @return the number of cells of the dimension specified by
   *         <code>dim</code>
   */
  template<class T>
  inline int thistogram<T>::cellsInDimension(const int& dimension) const {
    return theCellsPerDimension.at(dimension);
  }

  template<class T>
  inline const ivector& thistogram<T>::cellsPerDimension() const {
    return theCellsPerDimension;
  }


  /*
   * returns a vector to the first element of the histogram
   * (usually every element of the vector is 0;
   */
  template<class T>
  inline const ivector& thistogram<T>::getFirstCell() const {
    return firstCell;
  }

  /*
   * returns a vector to the last element of the histogram
   */
  template<class T>
  inline const ivector& thistogram<T>::getLastCell() const {
    return lastCell;
  }

  /*
   * get the index of the biggest element in the histogram
   */
  template<class T>
  inline ivector thistogram<T>::getIndexOfMaximum() const {
    return indexToVector(theHistogram.getIndexOfMaximum());
  }

  /**
   * get the index of the smallest element in the histogram
   */
  template<class T>
  inline ivector thistogram<T>::getIndexOfMinimum() const {
    return indexToVector(theHistogram.getIndexOfMinimum());
  }

  /*
   * returns the number of entries registered by now
   * (i.e. how many put() 'till now?).
   */
  template<class T>
  inline const T& thistogram<T>::getNumberOfEntries() const {
    return numberOfEntries;
  }

  /*
   * compute the integer index for the data vector using the
   * given index vector.
   */
  template<class T>
  inline int thistogram<T>::vectorToIndex(const ivector& x) const {
    int i=dimensionality-1;
    assert ((x.at(i)>=0) && (x.at(i)<theCellsPerDimension.at(i)));
    int idx = x.at(i);

    for (--i;i>=0;--i) {
      assert ((x.at(i)>=0) && (x.at(i)<theCellsPerDimension.at(i)));
      idx*=theCellsPerDimension.at(i);
      idx+=x.at(i);
    }

    return idx;
  }

  /*
   * compute the index vector for the data vector using the
   * given integer index.
   */
  template<class T>
  inline ivector thistogram<T>::indexToVector(const int x) const {
    assert (x>=0 && x<theHistogram.size());
    int i=0;
    int tmp=x;
    ivector idx(dimensionality);

    for (; i<dimensionality-1; ++i) {
      const int &sz=theCellsPerDimension.at(i);
      idx.at(i)=tmp%sz;
      tmp/=sz;
    }
    idx.at(dimensionality-1)=tmp;

    return idx;
  }


  // -----------------------------------------------------------------
  //             HISTOGRAM 1D
  // -----------------------------------------------------------------

  /*
   * returns a vector to the first element of the histogram1D
   * (usually every element of the vector is 0;
   */
  inline const int& histogram1D::getFirstCell() const {
    return firstCell;
  }

  /*
   * returns a vector to the last element of the histogram1D
   */
  inline const int& histogram1D::getLastCell() const {
    return lastCell;
  }

  /*
   * number of cells in the vector
   */
  inline int histogram1D::size() const {
    return lastCell+1;
  }

  /*
   * read-only access to the element x of the histogram1D
   * @param x index of the histogram1D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline const histogram::value_type& histogram1D::at(const int& x) const {
    return theHistogram.at(x);
  }

  /*
   * access element x of the histogram1D
   * @param x index of the histogram1D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline histogram::value_type& histogram1D::at(const int& x) {
    return theHistogram.at(x);
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram1D.
   * @param x         index of the histogram1D element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  inline const histogram::value_type&
  histogram1D::put(const int& x,
                   const histogram::value_type& increment) {

    if (uint32(x)>uint32(theCellsPerDimension.at(0))) {
      return outerBoundsCell();
    }

    theHistogram.at(x)+=increment;
    numberOfEntries += increment;

    return theHistogram.at(x);
  }

  /*
   * read-only access to the element x of the histogram1D as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram1D (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram1D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram1D
   *         data.
   */
  inline histogram::value_type histogram1D::getProbability(const int& x) const{
    if (uint32(x)>uint32(theCellsPerDimension.at(0))) {
      return outerBoundsCell();
    }
    return theHistogram.at(x)/numberOfEntries;
  }

  /**
   * get the index of the biggest element in the histogram
   */
  inline int histogram1D::getIndexOfMaximum() const {
    return theHistogram.getIndexOfMaximum();
  };
  
  /**
   * get the index of the smallest element in the histogram
   */
  inline int histogram1D::getIndexOfMinimum() const {
    return theHistogram.getIndexOfMinimum();
  };
  
  // -----------------------------------------------------------------
  //             HISTOGRAM 2D
  // -----------------------------------------------------------------


  /*
   * returns a vector to the first element of the histogram2D
   * (usually every element of the vector is 0;
   */
  inline const point& histogram2D::getFirstCell() const {
    return firstCell;
  }

  /*
   * returns a vector to the last element of the histogram2D
   */
  inline const point& histogram2D::getLastCell() const {
    return lastCell;
  }


  /*
   * read-only access to the element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline const histogram::value_type& histogram2D::at(const point& p) const {
    return theHistoMatrix.at(p);
  }

  /*
   * access element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline histogram::value_type& histogram2D::at(const point& p) {
    return theHistoMatrix.at(p);
  }

  /*
   * read-only access to the element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline const histogram::value_type& histogram2D::at(const int& y,
                                               const int& x) const {
    return theHistoMatrix.at(y,x);
  }

  /*
   * access element p of the histogram2D
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the number of entries in the given cell
   */
  inline histogram::value_type& histogram2D::at(const int& y,const int& x) {
    return theHistoMatrix.at(y,x);
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram2D.
   * @param x         index of the histogram2D element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  inline const histogram::value_type& histogram2D::put(const int& y,
                                                       const int& x,
                                     const histogram::value_type& increment) {
    if ((uint32)y >= uint32(theCellsPerDimension.at(1)) ||
        ((uint32)x >= uint32(theCellsPerDimension.at(0)))) {
      return outerBoundsCell();
    }
    theHistoMatrix.at(y,x)+=increment;
    numberOfEntries += increment;

    return theHistoMatrix.at(y,x);
  }

  /*
   * increment the cell at \e x by the given number of elements (or 1.0 if
   * nothing is explicitly indicated!) and update the number of entries in
   * the histogram2D.
   * @param x         index of the histogram2D element to be incremented
   * @param increment amount of the incrementation (default: 1)
   * @return the new number of entries of the incremented cell.
   */
  inline const histogram::value_type&
  histogram2D::put(const point& p,
                   const histogram::value_type& increment) {
    if ((uint32)p.y >= uint32(theCellsPerDimension.at(1)) ||
        ((uint32)p.x >= uint32(theCellsPerDimension.at(0)))) {
      return outerBoundsCell();
    }

    theHistoMatrix.at(p)+=increment;
    numberOfEntries += increment;

    return theHistoMatrix.at(p);
  }

  /*
   * read-only access to the element x of the histogram2D as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram2D (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram2D
   *         data.
   */
  inline histogram::value_type
  histogram2D::getProbability(const int& y,const int& x) const {

    if ((uint32)y >= uint32(theCellsPerDimension.at(1)) ||
        ((uint32)x >= uint32(theCellsPerDimension.at(0)))) {
      return outerBoundsCell();
    }
    return theHistoMatrix.at(y,x)/numberOfEntries;
  }

  /*
   * read-only access to the element x of the histogram2D as a discrete
   * probability distribution term.  This is equivalent to \f$at(x)/n\f$,
   * where \e n is the number of entries in the histogram2D (see
   * getNumberOfEntries()).
   *
   * @param x index of the histogram2D element to be accessed.  It should
   *          be between getFirstCell() and getLastCell()
   * @return the probabilty of the element x, respect to the histogram2D
   *         data.
   */
  inline histogram::value_type
  histogram2D::getProbability(const point& p) const {
    if ((uint32)p.y >= uint32(theCellsPerDimension.at(1)) ||
        ((uint32)p.x >= uint32(theCellsPerDimension.at(0)))) {
      return outerBoundsCell();
    }

    return theHistoMatrix.at(p)/numberOfEntries;
  }

  /**
   * get the index of the biggest element in the histogram
   */
  inline point histogram2D::getIndexOfMaximum() const {
    return theHistoMatrix.getIndexOfMaximum();
  }

  /**
   * get the index of the smallest element in the histogram
   */
  inline point histogram2D::getIndexOfMinimum() const {
    return theHistoMatrix.getIndexOfMinimum();
  }
  

} // namespace lti

