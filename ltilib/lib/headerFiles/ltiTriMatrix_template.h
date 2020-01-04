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
 * file .......: ltiTriMatrix_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 28.04.99
 * revisions ..: $Id: ltiTriMatrix_template.h,v 1.7 2005/06/16 12:37:35 doerfler Exp $
 */

#include "ltiAssert.h"
#include <cstring>

namespace lti {

  // TRI-MATRIX
  template <class T>
  triMatrix<T>::triMatrix() : mathObject(),mem(0), rows(0), dimension(0) {
  }

  template <class T>
  triMatrix<T>::triMatrix(const int y) : mathObject() {
    int m,n,i;
    n = (y*(y + 1)) / 2;
    mem = new T[n];
    rows = new T*[y];
    dimension = y;

    n=0; m=dimension;

    for (i=0;i<dimension;i++) {
      rows[i]=&mem[n];
      n += m;
      m--;
    }
  }

  template <class T>
  triMatrix<T>::triMatrix(const triMatrix<T>& x)
    : mathObject(),mem(0), rows(0), dimension(0) {
    copy(x);
  }

  template <class T>
  triMatrix<T>::~triMatrix() {
    delete [] rows;
    delete [] mem;
    dimension = 0;
  }

  template <class T>
  T& triMatrix<T>::at(const int y,const int x) {
    assert (y < dimension);
    assert (x < dimension-y);
    return rows[y][x];
  }

  template <class T>
    const T& triMatrix<T>::at(const int y,const int x) const {
    assert (y < dimension);
    assert (x < dimension-y);

    return rows[y][x];
  }

  template <class T>
  T* triMatrix<T>::operator[] (const int y) {
    assert ( y < dimension );
    return (rows[y]);
  }

  template <class T>
  const T* triMatrix<T>::operator[] (const int y) const {
    assert ( y < dimension );
    return (rows[y]);
  }

  template <class T>
  triMatrix<T>& triMatrix<T>::copy(const triMatrix<T>& x) {
    if (&x == this)
      return (*this);

    int m,n,l,i;

    delete [] rows;
    delete [] mem;

    dimension = x.size();
    l = (size()*(size()+1))/2;
    mem = new T[l];
    rows = new T*[size()];
    n=0; m=dimension;

    for (i=0;i<size();i++) {
      rows[i]=&mem[n];
      n += m;
      m--;
    }

    memcpy(mem,&x.at(0,0),l);
    return (*this);
  }

  template <class T>
  triMatrix<T>& triMatrix<T>::operator=(const triMatrix<T>& x) {
    return copy(x);
  }

  /**
   * returns a copy of this object
   */
  template <class T>
  mathObject* triMatrix<T>::clone() const {
    return new triMatrix<T>(*this);
  }

  /**
   * write the object in the given ioHandler
   */
  template <class T>
  bool triMatrix<T>::write(ioHandler& handler,
                           const bool complete) const {

    int i;
    bool b = true;

    if (complete) {
      b = handler.writeBegin();
    }

    b = b && lti::write(handler,"size",size());

    b = b && handler.writeBegin();

    b = b && handler.writeSymbol(std::string("data"));
    b = b && handler.writeKeyValueSeparator();

    // data block
    b = b && handler.writeBegin();

    const int l = ((size()*(size()+1))/2)-1;

    if (b) {
      for (i=0;i<l;++i) {
        b = b && lti::write(handler,mem[i]);
        b = b && handler.writeDataSeparator();
      }
      b = b && lti::write(handler,mem[i]);
    }

    b = b && handler.writeEnd();
    // end data block

    b = b && handler.writeEnd();

    if (complete) {
      b = b && handler.writeEnd();
    }

    return b;
  }

  /**
   * read the object from the given ioHandler
   */
  template <class T>
  bool triMatrix<T>::read(ioHandler& handler,const bool complete) {
    int m,n,l,i;
    bool b = true;
    std::string str;
    int level;

    delete [] rows;
    delete [] mem;

    rows = 0;
    mem = 0;
    dimension = 0;

    if (complete) {
      b = handler.readBegin();
    }

    b = b && lti::read(handler,"size",dimension);

    l = (size()*(size()+1))/2;
    mem = new T[l];
    rows = new T*[size()];
    n=0; m=dimension;

    for (i=0;i<size();i++) {
      rows[i]=&mem[n];
      n += m;
      m--;
    }

    level = handler.getLevel();

    b = b && handler.readBegin();
    b = b && handler.readSymbol(str);

    if (str == "data") {
      b = b && handler.readKeyValueSeparator();

      b = b && handler.readBegin();

      l--; // read all but the last element!
      if (b) {
        for (i=0;i<l;++i) {
          b = b && lti::read(handler,mem[i]);
          b = b && handler.readKeyValueSeparator();
        }
        b = b && lti::read(handler,mem[i]); // read the last element
      }

      b = b && handler.readEnd();
    }

    while (handler.readEnd() && (handler.getLevel() > level));

    if (complete) {
      b = b && handler.readEnd();
    }

    return b;
  }

}

