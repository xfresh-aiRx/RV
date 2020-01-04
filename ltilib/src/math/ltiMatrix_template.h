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
 * file .......: ltiMatrix.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 09.04.99
 * revisions ..: $Id: ltiMatrix_template.h,v 1.78 2005/06/03 11:46:45 doerfler Exp $
 */

#include "ltiPerformanceConfig.h"


namespace lti {

  // default constructor
  template <class T>
  matrix<T>::matrix() : genericMatrix<T>() {}

  // constructor: rows X cols matrix
  template <class T>
  matrix<T>::matrix(const int rows, const int cols, const T& iniValue)
    : genericMatrix<T>(rows,cols,iniValue) {}

  // constructor: rows X cols matrix
  template <class T>
  matrix<T>::matrix(const int rows, const int cols, const T data[])
    : genericMatrix<T>(rows,cols,data) {}

  // constructor: rows X cols matrix
  template <class T>
  matrix<T>::matrix(const ipoint& dim, const T& iniValue)
    : genericMatrix<T>(dim,iniValue) {}

  // copy constructor
  template <class T>
  matrix<T>::matrix(const bool copyData, matrix<T>& other,
                    const int fromRow,const int toRow,
                    const int fromCol,const int toCol)
    : genericMatrix<T>(copyData,other,fromRow,toRow,fromCol,toCol) {}

  // copy constructor
  template <class T>
  matrix<T>::matrix(const matrix<T>& other,
                    const int fromRow,const int toRow,
                    const int fromCol,const int toCol)
    : genericMatrix<T>(other,fromRow,toRow,fromCol,toCol) {}

  template <class T>
  matrix<T>::matrix(const bool init, const int rows, const int cols)
    : genericMatrix<T>(init,rows,cols) {}

  template <class T>
  matrix<T>::matrix(const bool init, const ipoint& size)
    : genericMatrix<T>(init,size) {}

  // copy constructor
  template <class T>
  matrix<T>::matrix(const matrix<T>& other,
                    const genericVector<int>& idx)
    : genericMatrix<T>(other,idx) {}

  template <class T>
  matrix<T>::~matrix() {}


  // return the name of this type
  template <class T>
  const char* matrix<T>::getTypeName() const {
    return "matrix";
  }


  /* compare this matrix with other, and use the given tolerance to
     determine if the value of each element of the other matrix
     approximately equals the values of the actual matrix elements.

     An element <i>x</i> is approximately equal to another element <i>y</i>
     with a tolerance <i>t</i>, if following equation holds:
     <i>x</i>-t < <i>y</i> < <i>x</i>+t

     @param other the other matrix to be compared with
     @param tolerance the tolerance to be used

     @return true if both matrices are approximatly equal up to the given
     tolerance
  */
  template<class T>
  bool matrix<T>::prettyCloseTo(const matrix<T>& other,
                                const T& tolerance) const {
    bool result;

    result = (this->size()==other.size());

    if (result) {
      for (int y=0; result && (y<this->rows()); ++y) {
        result = result && getRow(y).prettyCloseTo(other.getRow(y),tolerance);
      }
    }

    return result;
  }

  // creates a clone of this matrix
  template<class T>
  mathObject* matrix<T>::clone() const {
    return new matrix<T>(*this);
  }

  // applies a C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(T (*function)(T)) {
    genericMatrix<T>::apply(function);
    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(const matrix<T>& other,T (*function)(T)) {
    genericMatrix<T>::apply(other,function);
    return (*this);
  };


  // applies a C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(T (*function)(const T&)) {
    genericMatrix<T>::apply(function);
    return (*this);
  };

  // applies a C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(const matrix<T>& other,T (*function)(const T&)) {
    genericMatrix<T>::apply(other,function);
    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(const matrix<T>& other,
                              T (*function)(const T&,const T&)) {
    genericMatrix<T>::apply(other,function);
    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(const matrix<T>& other,
                              T (*function)(T,T)) {
    genericMatrix<T>::apply(other,function);
    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(const matrix<T>& a,
                              const matrix<T>& b,
                              T (*function)(T,T)) {
    genericMatrix<T>::apply(a,b,function);
    return (*this);
  };

  // applies a two-parameter C-Function to each element
  template<class T>
  matrix<T>& matrix<T>::apply(const matrix<T>& a,
                              const matrix<T>& b,
                              T (*function)(const T&,const T&)) {
    genericMatrix<T>::apply(a,b,function);
    return (*this);
  };

  // add this matrix with 'other'
  template <class T>
  matrix<T>& matrix<T>::add(const matrix<T>& other) {
    assert(this->size() == other.size());

    if ((this->getMode()==genericMatrix<T>::Connected) && 
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ito,itoe;
      typename matrix<T>::iterator it;
      for(it=this->begin(),ito=other.begin(),itoe=other.end();
          ito!=itoe;
          ++it,ito++) {
        (*it) += (*ito);
      }
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).add(other.getRow(y));
      };
    }

    return (*this);
  }

  // add matrixes a and b
  template <class T>
  matrix<T>& matrix<T>::add(const matrix<T>& a, const matrix<T>& b) {
    assert(a.size() == b.size());

    // although there is a bounding check in resize() itself
    // checking is done here to avoid copying if the matrix is not connected
    if (this->size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    if (((this->getMode()==genericMatrix<T>::Connected) && 
         (a.getMode()==genericMatrix<T>::Connected)) && 
        (b.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ita,itb,ite;
      typename matrix<T>::iterator it;
      ite = a.end();
      for (it=this->begin(),ita=a.begin(),itb=b.begin();
           ita!=ite;
           ++it,++ita,++itb) {
        (*it) = (*ita) + (*itb);
      }          
    } else {

      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).add(a.getRow(y),b.getRow(y));
      }
    }

    return (*this);
  }

  // add a scalar value to all elements
  template <class T>
  matrix<T>& matrix<T>::add(const T value) {
    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix<T>::iterator it;
      typename matrix<T>::iterator ite=this->end();
      for(it=this->begin();it!=ite;++it) {
        (*it) += value;
      }
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).add(value);
      }
    }

    return (*this);
  }

  // add a scalar value to all elements
  template <class T>
  matrix<T>& matrix<T>::add(const matrix<T>& other,const T value) {

    // although there is a bounding check in resize() itself
    // checking is done here to avoid copying if the matrix is not connected
    if (this->size()!=other.size()) {
      resize(other.size(),T(),false,false);
    }
    if ((this->getMode()==genericMatrix<T>::Connected) &&
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator it;
      typename matrix<T>::const_iterator ite = other.end();
      typename matrix<T>::iterator itt;
      for (itt=this->begin(),it=other.begin();it!=ite;++it,itt++) {
        (*itt) = (*it) + value;
      }  
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).add(other.getRow(y),value);
      }
    }

    return (*this);
  }

  template<class T>
  matrix<T> matrix<T>::operator+(const matrix<T>& other) const {
    matrix<T> tmp;
    tmp.add(*this,other);
    return tmp;
  }

  /*
   * add constant value to this matrix, and leave result in a new
   * matrix. This object is not changed.
   * @param value a constant value to be added to all matrix elements
   * @return a new matrix with the result
   */
  template<class T>
    matrix<T> matrix<T>::operator+(const T value) const {
    matrix<T> tmp;
    tmp.add(*this,value);
    return tmp;
  }

  // add other matrix scaled to this matrix
  template <class T>
  matrix<T>& matrix<T>::addScaled(const T a, const matrix<T>& other) {

    assert(this->size() == other.size());

    if ((this->getMode()==genericMatrix<T>::Connected) && 
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ito,itoe;
      typename matrix<T>::iterator it;
      for(it=this->begin(),ito=other.begin(),itoe=other.end();
          ito!=itoe;
          ++it,ito++) {
        (*it) += a*(*ito);
      }
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).addScaled(a, other.getRow(y));
      };
    }

    return (*this);
  }

  // add scaled matrixes a and b
  template <class T>
  matrix<T>& matrix<T>::addScaled(const T sa, const matrix<T>& a,
                                  const T sb, const matrix<T>& b) {


    assert(a.size() == b.size());

    // although there is a bounding check in resize() itself
    // checking is done here to avoid copying if the matrix is not connected
    if (this->size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    if (((this->getMode()==genericMatrix<T>::Connected) &&
         (a.getMode()==genericMatrix<T>::Connected)) && 
        (b.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ita,itb,ite;
      typename matrix<T>::iterator it;
      ite = a.end();
      for (it=this->begin(),ita=a.begin(),itb=b.begin();
           ita!=ite;
           ++it,++ita,++itb) {
        (*it) = sa*(*ita) + sb*(*itb);
      }          
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).addScaled(sa, a.getRow(y), sb, b.getRow(y));
      }
    }

    return (*this);
  }

  // add scaled matrixes a and b
  template <class T>
  matrix<T>& matrix<T>::addScaled(const matrix<T>& a,
                                  const T sb,
                                  const matrix<T>& b) {

    assert(a.size() == b.size());

    // although there is a bounding check in resize() itself
    // checking is done here to avoid copying if the matrix is not connected
    if (this->size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    if (((this->getMode()==genericMatrix<T>::Connected) && 
         (a.getMode()==genericMatrix<T>::Connected)) 
        && (b.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ita,itb,ite;
      typename matrix<T>::iterator it;
      ite = a.end();
      for (it=this->begin(),ita=a.begin(),itb=b.begin();
           ita!=ite;
           ++it,++ita,++itb) {
        (*it) = (*ita) + sb*(*itb);
      }          
    } else {
  
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).addScaled(a.getRow(y), sb, b.getRow(y));
      }
    }

    return (*this);
  }

  // sum of elements
  template<class T>
  T matrix<T>::sumOfElements() const {
    T sum;
    if (this->getMode()==genericMatrix<T>::Connected) {

      typename matrix<T>::const_iterator it;
      typename matrix<T>::const_iterator itb = this->begin();
      typename matrix<T>::const_iterator ite = this->end();
      // if matrix is empty return something unknown
      if (!((this->rows()>0) && (this->columns()>0))) {
        sum = T();
      } else {
        sum = (*itb);
      }
      for (it=itb+1;it!=ite;++it) {
        sum += (*it);
      }
    }
    else {
      sum = T(); // assume default constructor for numeric types is T()!

      for (int y=0; y<this->rows(); ++y) {
        sum+=getRow(y).sumOfElements();
      }
    }
    return sum;
  }

  // multiply elementwise this matrix with 'other'
  template <class T>
  matrix<T>& matrix<T>::emultiply(const matrix<T>& other) {
    assert(this->size() == other.size());

    if ((this->getMode()==genericMatrix<T>::Connected) && 
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ito,itoe;
      typename matrix<T>::iterator it;
      for(it=this->begin(),ito=other.begin(),itoe=other.end();
          ito!=itoe;
          ++it,ito++) {
        (*it) *= (*ito);
      }
    } else {

      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).emultiply(other.getRow(y));
      };
    }

    return (*this);
  }

  // multiply elementwise the matrixes a and b
  template <class T>
  matrix<T>& matrix<T>::emultiply(const matrix<T>& a, const matrix<T> &b) {

    assert(a.size() == b.size());

    if (this->size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    if (((this->getMode()==genericMatrix<T>::Connected) && 
         (a.getMode()==genericMatrix<T>::Connected)) 
        && (b.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ita,itb,ite;
      typename matrix<T>::iterator it;
      ite = a.end();
      for (it=this->begin(),ita=a.begin(),itb=b.begin();
           ita!=ite;
           ++it,++ita,++itb) {
        (*it) = (*ita) * (*itb);
      }          
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).emultiply(a.getRow(y),b.getRow(y));
      };
    }

    return (*this);
  }

  // divide elementwise this matrix with 'other'
  template <class T>
  matrix<T>& matrix<T>::edivide(const matrix<T>& other) {
    assert(this->size() == other.size());
    if ((this->getMode()==genericMatrix<T>::Connected) &&
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ito,itoe;
      typename matrix<T>::iterator it;
      for(it=this->begin(),ito=other.begin(),itoe=other.end();
          ito!=itoe;
          ++it,ito++) {
        (*it) /= (*ito);
      }
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).edivide(other.getRow(y));
      };
    }

    return (*this);
  }

  // divide elementwise the matrixes a and b
  template <class T>
  matrix<T>& matrix<T>::edivide(const matrix<T>& a, const matrix<T> &b) {
    assert(a.size() == b.size());

    if (this->size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    if (((this->getMode()==genericMatrix<T>::Connected) &&
         (a.getMode()==genericMatrix<T>::Connected)) 
        && (b.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ita,itb,ite;
      typename matrix<T>::iterator it;
      ite = a.end();
      for (it=this->begin(),ita=a.begin(),itb=b.begin();
           ita!=ite;
           ++it,++ita,++itb) {
        (*it) = (*ita) / (*itb);
      }          
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).edivide(a.getRow(y),b.getRow(y));
      };
    }

    return (*this);
  }

  // subtract 'other' from this matrix
  template <class T>
  matrix<T>& matrix<T>::subtract(const matrix<T>& other) {
    assert(this->size() == other.size());

    if ((this->getMode()==genericMatrix<T>::Connected) &&
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ito,itoe;
      typename matrix<T>::iterator it;
      for(it=this->begin(),ito=other.begin(),itoe=other.end();
          ito!=itoe;
          ++it,ito++) {
        (*it) -= (*ito);
      }
    } else {

      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).subtract(other.getRow(y));
      };
    }

    return (*this);
  }

  // subtract matrix b from a
  template <class T>
  matrix<T>& matrix<T>::subtract(const matrix<T>& a, const matrix<T>& b) {
    assert(a.size() == b.size());

    if (this->size() != a.size()) {
      resize(a.size(),T(),false,false);
    }

    if (((this->getMode()==genericMatrix<T>::Connected) &&
         (a.getMode()==genericMatrix<T>::Connected)) 
        && (b.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator ita,itb,ite;
      typename matrix<T>::iterator it;
      ite = a.end();
      for (it=this->begin(),ita=a.begin(),itb=b.begin();
           ita!=ite;
           ++it,++ita,++itb) {
        (*it) = (*ita) - (*itb);
      }          
    } else {

      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).subtract(a.getRow(y),b.getRow(y));
      };
    }

    return (*this);
  }

  // subtract a scalar value from all elements
  template <class T>
  matrix<T>& matrix<T>::subtract(const T value) {

    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix<T>::iterator it;
      typename matrix<T>::iterator ite=this->end();
      for(it=this->begin();it!=ite;++it) {
        (*it) -= value;
      }
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).subtract(value);
      }
    }

    return (*this);
  }

  // subtract a scalar value from all elements
  template <class T>
  matrix<T>& matrix<T>::subtract(const matrix<T>& other,const T value) {
    if (this->size()!=other.size()) {
      resize(other.size(),T(),false,false);
    }
    if ((this->getMode()==genericMatrix<T>::Connected) &&
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator it;
      typename matrix<T>::const_iterator ite = other.end();
      typename matrix<T>::iterator itt;
      for (itt=this->begin(),it=other.begin();it!=ite;++it,itt++) {
        (*itt) = (*it) - value;
      }  
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).subtract(other.getRow(y),value);
      }
    }
    return (*this);
  }


  // multiply this matrix with other (TODO: optimize!)
  template <class T>
  matrix<T>& matrix<T>::multiply(const matrix<T>& other) {
    assert(this->columns() == other.rows());

    int tmpRows,tmpCols,i,j,k;
    tmpRows = this->rows();
    tmpCols = this->columns();

    matrix<T> a;

    if (this->mode == genericMatrix<T>::Connected) {
      detach(a);
    } else {
      a.copy(*this);
    }

    // no copy but initialize with 0
    resize(tmpRows,other.columns(),T(0),false,true);

    /* if the other matrix is big enough, it is faster to transpose it
       first and then use the usual vector inner product (dot).
       At which value a matrix is considered as big enough is controlled via
       basics/ltiPerformanceConfig.h.
    */

    if ((other.rows()*other.columns())
          <_LTI_PERFORMANCE_MATRIX_MATRIX_MULTIPLY) {

      iterator itthis;
      typename vector<T>::const_iterator itf;
      vector<T> ftemp;

      // traditional implementation
      for (itthis=this->begin(), i=0; i<this->rows(); ++i) {
        ftemp=a.getRow(i);
        for (j=0;j<this->columns();++j) {
          itf=ftemp.begin();
          for (k=0;k<tmpCols;++k) {
            *itthis+=(*itf)*other.at(k,j);
            ++itf;
          }
          ++itthis;
        }
      }

    } else {
      // transpose the other matrix first

      matrix<T> b;
      b.transpose(other);

      for (i=0;i<this->rows();++i) {
        for (j=0;j<this->columns();++j) {
          this->at(i,j)=a.getRow(i).dot(b.getRow(j));
        }
      }
    }

    return (*this);
  }

  // multiply matrix first with second
  template <class T>
  matrix<T>& matrix<T>::multiply(const matrix<T>& first,
                                 const matrix<T>& second) {
    assert(first.columns() == second.rows());

    int tmpRows,tmpCols,i,j,k;
    tmpRows = first.rows();
    tmpCols = first.columns();

    // no copy but initialize with 0
    resize(tmpRows,second.columns(),T(0),false,true);

    /* if the other matrix is big enough, it is faster to transpose it
       first and then use the usual vector inner product (dot).
       At which value a matrix is considered as big enough is controlled via
       basics/ltiPerformanceConfig.h.
    */

    if ((second.rows()*second.columns())
          < _LTI_PERFORMANCE_MATRIX_MATRIX_MULTIPLY) {

      iterator itthis;
      typename vector<T>::const_iterator itf;
      vector<T> ftemp;

      // traditional implementation
      for (i=0, itthis=this->begin(); i<this->rows(); ++i) {
        ftemp=first.getRow(i);
        for (j=0;j<this->columns();++j) {
          itf=ftemp.begin();
          for (k=0;k<tmpCols;++k) {
            *itthis+=(*itf)*second.at(k,j);
            ++itf;
          }
          ++itthis;
        }
      }
    } else {
      // transpose the second matrix first

      matrix<T> b(second);
      b.transpose();

      for (i=0;i<this->rows();++i) {
        for (j=0;j<this->columns();++j) {
          this->at(i,j)=first.getRow(i).dot(b.getRow(j));
        }
      };
    }

    return (*this);
  }

  // multiply this matrix with a vector, and leave result in 'result'.
  template <class T>
  vector<T>&
  matrix<T>::multiply(const vector<T>& other,vector<T>& result) const {
    assert(this->columns() == other.size());

    int i;
    result.resize(this->rows(),T(),false,false);

    for (i=0; i<this->rows(); ++i) {
      result.at(i)=getRow(i).dot(other);
    }

    return (result);
  }

  // multiply this matrix with a vector, and leave result in the same
  // vector (On-Place)
  template <class T>
  vector<T>&
  matrix<T>::multiply(vector<T>& srcdest) const {
    vector<T> tmp;
    multiply(srcdest,tmp);
    tmp.detach(srcdest);
    return srcdest;
  }

  // multiply each element with the value 'value'
  template <class T>
  matrix<T>& matrix<T>::multiply(const T value) {

    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix<T>::iterator it;
      typename matrix<T>::iterator ite=this->end();
      for(it=this->begin();it!=ite;++it) {
        (*it) *= value;
      }
    } else {
      int i;
      for (i=0;i<this->numRows;++i) {
        getRow(i).multiply(value);
      };
    }

    return (*this);
  }

  // multiply a scalar value to all elements
  template <class T>
  matrix<T>& matrix<T>::multiply(const matrix<T>& other,const T value) {

    if (this->size()!=other.size()) {
      resize(other.size(),T(),false,false);
    }
    if ((this->getMode()==genericMatrix<T>::Connected) &&
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator it;
      typename matrix<T>::const_iterator ite = other.end();
      typename matrix<T>::iterator itt;
      for (itt=this->begin(),it=other.begin();it!=ite;++it,itt++) {
        (*itt) = (*it) * value;
      }  
    } else {
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).multiply(other.getRow(y),value);
      }
    }
    
    return (*this);
  }


  // left multiply vct with this matrix
  template <class T>
  vector<T>& matrix<T>::leftMultiply(const vector<T>& vct,
                                     vector<T>& result) const {
    assert(vct.size() == this->rows());

    int i;  
    result.multiply(this->getRow(0),vct[0]);
    for (i=1;i<this->numRows;++i) {
      result.addScaled(vct[i],this->getRow(i));
    }
    return (result);
  }

  // left multiply this matrix with a vector
  template <class T>
  vector<T>& matrix<T>::leftMultiply(vector<T>& srcdest) const {

    assert(srcdest.size() == this->rows());

    vector<T> tmp;
    leftMultiply(srcdest,tmp);
    tmp.detach(srcdest);

    return (srcdest);
  }

  // left multiply this matrix with other (TODO: optimize!)
  template <class T>
  matrix<T>& matrix<T>::leftMultiply(const matrix<T>& other) {
    assert(other.columns() == this->rows());

    matrix<T> a;

    a.multiply(other,*this);
    a.detach(*this);

    return (*this);
  }

  // divide each element by the value 'value'
  template <class T>
  matrix<T>& matrix<T>::divide(const T value) {
    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix<T>::iterator it;
      typename matrix<T>::iterator ite=this->end();
      for(it=this->begin();it!=ite;++it) {
        (*it) /= value;
      }
    } else {
      int i;
      for (i=0;i<this->numRows;++i) {
        getRow(i).divide(value);
      }
    }
    return (*this);
  }

  // divide by a scalar value
  template <class T>
  matrix<T>& matrix<T>::divide(const matrix<T>& other,const T value) {
    if (this->size()!=other.size()) {
      resize(other.size(),T(),false,false);
    }
    if ((this->getMode()==genericMatrix<T>::Connected) &&
        (other.getMode()==genericMatrix<T>::Connected)) {
      typename matrix<T>::const_iterator it;
      typename matrix<T>::const_iterator ite = other.end();
      typename matrix<T>::iterator itt;
      for (itt=this->begin(),it=other.begin();it!=ite;++it,itt++) {
        (*itt) = (*it) / value;
      }  
    } else {      
      int y;
      for (y=0;y<this->rows();++y) {
        getRow(y).divide(other.getRow(y),value);
      }
    }
    return (*this);
  }

  // outer product of two vectors
  template <class T>
  matrix<T>& matrix<T>::outerProduct(const vector<T>& a,
                                     const vector<T>& b) {
    // resize the matrix
    // it should have a.size() rows and b.size() columns
    if (this->rows() != a.size() || this->columns() != b.size()) {
      resize(a.size(),b.size(),T(),false,false);
    }

    // the iterators for the input vectors
    typename vector<T>::const_iterator i, ie=a.end();
    typename vector<T>::const_iterator j, je=b.end();

    if (this->getMode() == genericMatrix<T>::Connected) {

      typename matrix<T>::iterator d=this->begin();

      for (i=a.begin(); i != ie; ++i) {
        for (j=b.begin(); j != je; ++j,++d) {
          (*d)=(*i)*(*j);
        }
      }

    } else {
      // for lined matrices...

      typename vector<T>::iterator d;
      int r;

      for (r=0,i=a.begin(); i != ie; ++i,++r) {
        for (j=b.begin(),d=getRow(r).begin(); j != je; ++j,++d) {
          *d=(*i)*(*j);
        }
      }

    }

    return (*this);
  }

  // transpose matrix
  template <class T>
  matrix<T>& matrix<T>::transpose() {
    matrix<T> tmp;
    tmp.transpose(*this);
    tmp.detach(*this);
    return (*this);
  }

  // transpose matrix
  template <class T>
  matrix<T>& matrix<T>::transpose(const matrix<T>& other) {
    int m = other.rows();
    int n = other.columns();
    // resizes this matrix
    // it should have m columns and n rows
    if ((this->rows() != n) || (this->columns() != m)) {
      resize(n,m,T(),false,false);
    }

    if ((other.getMode()==genericMatrix<T>::Connected) &&
        (this->getMode()==genericMatrix<T>::Connected)) {

      typename matrix<T>::iterator itr = this->begin();
      typename matrix<T>::iterator itc;
      typename matrix<T>::const_iterator itoc,itor,itore;
      // end of first row
      typename matrix<T>::iterator itre = itr+m;
      for (itoc=other.begin();
           itr!=itre; //i<m
           ++itr,itoc+=n) {
        // end of row of other 
        itore = itoc+n;
        for (itc=itr,itor=itoc;
             itor!=itore; //j<n
             itc+=m,++itor) {
          (*itc) = (*itor);
        }
      }
    } else {

      int i,j;
      for(i=0;i<this->columns();++i) {
        for(j=0;j<this->rows();++j) {
          this->at(j,i)=other.at(i,j);
        }
      }
    }

    return (*this);
  }


  // minimum
  template<class T>
  T matrix<T>::minimum() const {
    // if matrix is empty return something unknown
    if (!((this->rows()>0) && (this->columns()>0))) {
      return T();
    }

    T theMinimum;
    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix::const_iterator it;
      typename matrix::const_iterator itb=this->begin();
      typename matrix::const_iterator ite=this->end();
      theMinimum = (*itb);
      for (it = itb+1;it != ite; ++it) {
        theMinimum = min(theMinimum,(*it));
      }
    }
    else {
   
      theMinimum = getRow(0).minimum();
      for (int y=1; y<this->rows(); ++y) {
        theMinimum = min(theMinimum,getRow(y).minimum());
      }
    }

    return theMinimum;
  }

  // minimum
  template<class T>
  ipoint matrix<T>::getIndexOfMinimum() const {
    ipoint p;
    ipoint idxOfMin;
    T theMinimum;

    if ((this->rows()>0) && (this->columns()>0)) {
      theMinimum = this->at(0,0);
      idxOfMin = ipoint(0,0);
    } else {
      return ipoint(0,0); // return something unknown
    }

    for (p.y=0;p.y<this->rows();++p.y) {
      for (p.x=0;p.x<this->columns();++p.x) {
        if (this->at(p) < theMinimum) {
          idxOfMin = p;
          theMinimum = this->at(p);
        }
      }
    }

    return idxOfMin;
  }


  // maximum
  template<class T>
  T matrix<T>::maximum() const {
    // if matrix is empty return something unknown
    if (!((this->rows()>0) && (this->columns()>0))) {
      return T();
    }

    T theMaximum;
    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix::const_iterator it;
      typename matrix::const_iterator itb=this->begin();
      typename matrix::const_iterator ite=this->end();
      theMaximum = (*itb);
      for (it = itb+1;it != ite; ++it) {
        theMaximum = max(theMaximum,(*it));
      }
    }
    else {
      theMaximum = getRow(0).maximum();
      for (int y=1; y<this->rows(); ++y) {
        theMaximum =  max(theMaximum,getRow(y).maximum());
      }
    }

    return theMaximum;
  }

  // maximum
  template<class T>
  ipoint matrix<T>::getIndexOfMaximum() const {

    ipoint p;
    ipoint idxOfMax;
    T theMaximum;

    if ((this->rows()>0) && (this->columns()>0)) {
      theMaximum = this->at(0,0);
      idxOfMax = ipoint(0,0);
    } else {
      return ipoint(0,0); // return something unknown
    }

    for (p.y=0;p.y<this->rows();++p.y) {
      for (p.x=0;p.x<this->columns();++p.x) {
        if (theMaximum < this->at(p)) {
          idxOfMax = p;
          theMaximum = this->at(p);
        }
      }
    }

    return idxOfMax;
  }

  // minimum and maximum
  template<class T>
  void matrix<T>::getExtremes(T& theMinimum,T& theMaximum) const {
    // if matrix is empty return something unknown
    if (!((this->rows()>0) && (this->columns()>0))) {
      return;
    }

    if (this->getMode()==genericMatrix<T>::Connected) {
      typename matrix::const_iterator it;
      typename matrix::const_iterator itb=this->begin();
      typename matrix::const_iterator ite=this->end();
      theMaximum = (*itb);
      theMinimum = (*itb);
      for (it = itb+1;it != ite; ++it) {
        theMaximum = max(theMaximum,(*it));
        theMinimum = min(theMinimum,(*it));
      }
    }
    else {    
      T theMinTmp,theMaxTmp;
      getRow(0).getExtremes(theMinimum,theMaximum);

      for (int y=1; y<this->rows(); ++y) {
        getRow(y).getExtremes(theMinTmp,theMaxTmp);
        theMaximum = max(theMaximum,theMaxTmp);
        theMinimum = min(theMinimum,theMinTmp);
      }
    }
  }

  // indices of minimum and maximum
  template<class T>
  void matrix<T>::getIndexOfExtremes(point& idxOfMin,point& idxOfMax) const {

    ipoint p;
    T theMaximum;
    T theMinimum;

    if ((this->rows()>0) && (this->columns()>0)) {
      theMinimum = theMaximum = this->at(0,0);
      idxOfMin = idxOfMax = ipoint(0,0);
    } else {
      return;
    }

    for (p.y=0;p.y<this->rows();++p.y) {
      for (p.x=0;p.x<this->columns();++p.x) {
        if (theMaximum < this->at(p)) {
          idxOfMax = p;
          theMaximum = this->at(p);
        }

        if (this->at(p) < theMinimum) {
          idxOfMin = p;
          theMinimum = this->at(p);
        }

      }
    }
  }

  // set equal to identity
  template<class T>
  void matrix<T>::setIdentity(const T scale) {
    fill(T(0));
    const int n = min(this->rows(),this->columns());
    int i;
    for (i = 0;i<n;++i) {
      this->at(i,i) = scale;
    }
  }

  // return trace of matrix
  template<class T>
  T matrix<T>::trace() const {
    const int n = min(this->rows(),this->columns());
    int i;
    T sum(T(0));
    for (i = 0;i<n;++i) {
      sum+=this->at(i,i);
    }
    return sum;
  }

} // namespace lti
