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
 * file .......: ltiMaskFunctors_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 08.10.99
 * revisions ..: $Id: ltiMaskFunctors_template.h,v 1.9 2003/05/05 06:42:08 alvarado Exp $
 */

namespace lti  {

  template<class T>
  maskFunctor<T>::maskFunctor() : functor() {};

  template<class T>
  maskFunctor<T>::~maskFunctor() {};

  template<class T>
  void maskFunctor<T>::copy(const maskFunctor<T>& other) {};

  /*
   * apply the given function at each pixel of m1 and leave the
   * result in the <code>result</code> matrix.
   * @param m1 the first matrix
   * @param result the resulting matrix.  The size of this matrix will be
   *        the size of m1
   * @param the function to be applied.  The first parameter will be the
   *        element value of the first matrix, the second parameter will
   *        be usually a normalization factor.
   * @return a reference to the result matrix.
   */
  template<class T>
  bool maskFunctor<T>::apply(const matrix<T>& m1,
                                   matrix<T>& result,
                                   T (*function)(const T&,
                                                 const T&)) const {

    T norm = typeInfo<T>::suggestedNorm();

    int y;
    result.resize(m1.size(),T(),false,false);

    typename vector<T>::const_iterator sit,eit;
    typename vector<T>::iterator dit;

    for (y=0;y<m1.rows();++y) {
      const vector<T>& src = m1.getRow(y);
      vector<T>& dest = result.getRow(y);
      for (sit=src.begin(),eit=src.end(),dit=dest.begin();
           sit!=eit;
           ++sit,++dit) {
        (*dit) = (*function)(*sit,norm);
      }
    }

    return true;
  }

  /*
   * apply the given function at each pixel of m1 and leave the
   * result in the <code>result</code> matrix.
   * @param m1 the first matrix
   * @param result the resulting matrix.  The size of this matrix will be
   *        the size of m1
   * @param the function to be applied.  The first parameter will be the
   *        element value of the first matrix, the second parameter will
   *        be usually a normalization factor.
   * @return a reference to the result matrix.
   */
  template<class T>
  bool maskFunctor<T>::apply(matrix<T>& srcdest,
                             T (*function)(const T&,
                                           const T&)) const {

    T norm;
    norm = typeInfo<T>::suggestedNorm();

    int y;

    typename vector<T>::iterator dit,eit;

    for (y=0;y<srcdest.rows();++y) {
      vector<T>& dest = srcdest.getRow(y);
      for (dit=dest.begin(),eit=dest.end();
           dit!=eit;
           ++dit) {
        (*dit) = (*function)(*dit,norm);
      }
    }

    return true;
  }

  /*
   * apply the given function at each element of m1 and m2 and leave the
   * result in the <code>result</code> matrix.
   * @param m1 the first matrix
   * @param m2 the second matrix
   * @param result the resulting matrix.  The size of this matrix will be
   *        the smallest matrix which can contain both matrices m1 and m2.
   *        For those parts covered only by one of the matrices, the assumed
   *        value for the other one will be zero.
   * @param the function to be applied.  The first parameter will be the
   *        element value of the first matrix, the second parameter the
   *        element value of the second matrix and the third parameter will
   *        be usually a normalization factor.
   * @return a reference to the result matrix.
   */
  template<class T>
  bool maskFunctor<T>::apply(const matrix<T>& m1,
                             const matrix<T>& m2,
                                   matrix<T>& result,
                                   T (*function)(const T&,
                                                 const T&,
                                                 const T&)) const {

    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (m1.size() == m2.size()) {
      int y;
      result.resize(m1.size(),T(),false,false);

      typename vector<T>::const_iterator s1it,eit,s2it;
      typename vector<T>::iterator dit;

      for (y=0;y<m1.rows();++y) {
        const vector<T>& src1 = m1.getRow(y);
        const vector<T>& src2 = m2.getRow(y);
        vector<T>& dest = result.getRow(y);
        for (s1it=src1.begin(),eit=src1.end(),s2it=src2.begin(),
               dit=dest.begin();
             s1it!=eit;
             ++s1it,++s2it,++dit) {
          (*dit) = (*function)(*s1it,*s2it,norm);
        }
      }
    } else {
      // the size can not be greater than the smallest matrix
      point size;
      int y;

      result.resize(max(m1.rows(),m2.rows()),
                    max(m1.columns(),m2.columns()),
                    T(),false,false);

      size.x = min(m1.columns(),m2.columns());
      size.y = min(m1.rows(),m2.rows());

      typename vector<T>::const_iterator s1it,eit,s2it;
      typename vector<T>::iterator dit;

      if (m1.columns()<m2.columns()) {
        for (y=0;y<size.y;y++) {
          const vector<T>& src1 = m1.getRow(y);
          const vector<T>& src2 = m2.getRow(y);
          vector<T>& dest = result.getRow(y);
          for (s1it=src1.begin(),eit=src1.end(),s2it=src2.begin(),
                 dit=dest.begin();
               s1it!=eit;
               ++s1it,++s2it,++dit) {
            (*dit) = (*function)(*s1it,*s2it,norm);
          }
        }

        // the border must be initialized
        switch (check(*function,0)) {
        case Black:
          result.fill(T(0),rectangle(m1.columns(),0,result.lastColumn(),y-1));
          break;
        case White:
          result.fill(norm,rectangle(m1.columns(),0,result.lastColumn(),y-1));
          break;
        case NoEffect:
          result.fill(m2,rectangle(m1.columns(),0,result.lastColumn(),y-1),
                         point(m1.columns(),0));
          break;
        default:
          result.fill(T(0),rectangle(m1.columns(),0,result.lastColumn(),y-1));
        }
      } else {
        for (y=0;y<size.y;y++) {
          const vector<T>& src1 = m1.getRow(y);
          const vector<T>& src2 = m2.getRow(y);
          vector<T>& dest = result.getRow(y);
          for (s1it=src1.begin(),eit=src2.end(),s2it=src2.begin(),
                 dit=dest.begin();
               s2it!=eit;
               ++s1it,++s2it,++dit) {
            (*dit) = (*function)(*s1it,*s2it,norm);
          }
        }
        // the border must be initialized
        switch (check(*function,1)) {
        case Black:
          result.fill(T(0),rectangle(m2.columns(),0,result.lastColumn(),y-1));
          break;
        case White:
          result.fill(norm,rectangle(m2.columns(),0,result.lastColumn(),y-1));
          break;
        case NoEffect:
          result.fill(m1,rectangle(m2.columns(),0,result.lastColumn(),y-1),
                         point(m2.columns(),0));
          break;
        default:
          result.fill(T(0),rectangle(m2.columns(),0,result.lastColumn(),y-1));
        }
      }

      // the "unused" borders must be set to zero!
      // the bottom-right corner
      if (size.y<result.rows()) {
        result.fill((*function)(0,0,norm),
                    rectangle(size,result.size()));
      }

      // the botton-left corner
      if (m1.rows()<m2.rows()) {
        // the border must be initialized
        switch (check(*function,0)) {
        case Black:
          result.fill(T(0),rectangle(0,m1.rows(),size.x-1,result.lastRow()));
          break;
        case White:
          result.fill(norm,rectangle(0,m1.rows(),size.x-1,result.lastRow()));
          break;
        case NoEffect:
          result.fill(m2,rectangle(0,m1.rows(),size.x-1,result.lastRow()),
                         point(0,m1.rows()));
          break;
        default:
          result.fill(T(0),rectangle(0,m1.rows(),size.x-1,result.lastRow()));
        }
      } else {
        // the border must be initialized
        switch (check(*function,1)) {
        case Black:
          result.fill(T(0),rectangle(0,m2.rows(),size.x-1,result.lastRow()));
          break;
        case White:
          result.fill(norm,rectangle(0,m2.rows(),size.x-1,result.lastRow()));
          break;
        case NoEffect:
          result.fill(m1,rectangle(0,m2.rows(),size.x-1,result.lastRow()),
                         point(0,m2.rows()));
          break;
        default:
          result.fill(T(0),rectangle(0,m2.rows(),size.x-1,result.lastRow()));
        }
      }
    }

    return true;
  };


  /*
   * apply the given function at each element of m1 and m2 and leave the
   * result in the <code>result</code> matrix.
   * @param m1 the first matrix
   * @param m2 the second matrix
   * @param result the resulting matrix.  The size of this matrix will be
   *        the smallest matrix which can contain both matrices m1 and m2.
   *        For those parts covered only by one of the matrices, the assumed
   *        value for the other one will be zero.
   * @param the function to be applied.  The first parameter will be the
   *        element value of the first matrix, the second parameter the
   *        element value of the second matrix and the third parameter will
   *        be usually a normalization factor.
   * @return a reference to the result matrix.
   */
  template<class T>
  bool maskFunctor<T>::apply(const matrix<T>& m1,
                                   matrix<T>& srcdest,
                                   T (*function)(const T&,
                                                 const T&,
                                                 const T&)) const {

    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (m1.size() == srcdest.size()) {
      int y;

      typename vector<T>::const_iterator sit,eit;
      typename vector<T>::iterator dit;

      for (y=0;y<m1.rows();++y) {
        const vector<T>& src = m1.getRow(y);
        vector<T>& dest = srcdest.getRow(y);
        for (sit=src.begin(),eit=src.end(),dit=dest.begin();
             sit!=eit;
             ++sit,++dit) {
          (*dit) = (*function)(*sit,*dit,norm);
        }
      }
    } else {
      // the size can not be greater than the smallest matrix
      point size;
      int y;

      size.x = min(m1.columns(),srcdest.columns());
      size.y = min(m1.rows(),srcdest.rows());

      typename vector<T>::const_iterator sit;
      typename vector<T>::iterator dit;

      if (m1.columns()<srcdest.columns()) {
        typename vector<T>::const_iterator eit;
        for (y=0;y<size.y;y++) {
          const vector<T>& src = m1.getRow(y);
          vector<T>& dest = srcdest.getRow(y);
          for (sit=src.begin(),eit=src.end(),dit=dest.begin();
               sit!=eit;
               ++sit,++dit) {
            (*dit) = (*function)(*sit,*dit,norm);
          }
        }

        // the border must be initialized
        switch (check(*function,0)) {
        case Black:
          srcdest.fill(T(0),rectangle(m1.columns(),0,srcdest.lastColumn(),y-1));
          break;
        case White:
          srcdest.fill(norm,rectangle(m1.columns(),0,srcdest.lastColumn(),y-1));
          break;
        case NoEffect:
          break;
        default:
          // TODO: operate on the region with the function...
          srcdest.fill(T(0),rectangle(m1.columns(),0,srcdest.lastColumn(),y-1));
        }
      } else {
        typename vector<T>::iterator eit;
        for (y=0;y<size.y;y++) {
          const vector<T>& src = m1.getRow(y);
          vector<T>& dest = srcdest.getRow(y);
          for (sit=src.begin(),eit=dest.end(),dit=dest.begin();
               dit!=eit;
               ++sit,++dit) {
            (*dit) = (*function)(*sit,*dit,norm);
          }
        }
      }

      // the "unused" borders must be set to zero!
      // the bottom-right corner
      if (size.y<srcdest.rows()) {
        srcdest.fill((*function)(0,0,norm),
                     rectangle(size,srcdest.size()));
      }

      // the botton-left corner
      if (m1.rows()<srcdest.rows()) {
        // the border must be initialized
        switch (check(*function,0)) {
        case Black:
          srcdest.fill(T(0),rectangle(0,m1.rows(),size.x-1,srcdest.lastRow()));
          break;
        case White:
          srcdest.fill(norm,rectangle(0,m1.rows(),size.x-1,srcdest.lastRow()));
          break;
        case NoEffect:
          break;
        default:
          //TODO: operate function on the region with 0
          srcdest.fill(T(0),rectangle(0,m1.rows(),size.x-1,srcdest.lastRow()));
        }
      }
    }

    return true;
  };


  template<class T>
  typename maskFunctor<T>::eFunctionType
  maskFunctor<T>::check(T (*function)(const T&,
                                      const T&,
                                      const T&),
                        const int& par) const {
    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (par == 0) {
      if ((*function)(0,0,norm) == T(0)) {
        if ((*function)(0,norm,norm) == T(0)) {
          if ((*function)(0,T(norm/T(2)),norm) == T(0)) {
            return Black;
          }
        } else if ((*function)(0,norm,norm) == norm) {
          if ((*function)(0,T(norm/T(2)),norm) == T(norm/T(2))) {
            return NoEffect;
          }
        }
      } else if ((*function)(0,0,norm) == norm) {
        if ((*function)(0,norm,norm) == norm) {
          if ((*function)(0,T(norm/T(2)),norm) == norm) {
            return White;
          }
        }
      }
    } else {
      if ((*function)(0,0,norm) == T(0)) {
        if ((*function)(norm,0,norm) == T(0)) {
          if ((*function)(T(norm/T(2)),0,norm) == T(0)) {
            return Black;
          }
        } else if ((*function)(norm,0,norm) == norm) {
          if ((*function)(T(norm/T(2)),0,norm) == T(norm/T(2))) {
            return NoEffect;
          }
        }
      } else if ((*function)(0,0,norm) == norm) {
        if ((*function)(norm,0,norm) == norm) {
          if ((*function)(T(norm/T(2)),0,norm) == norm) {
            return White;
          }
        }
      }
    }

    return Unknown;
  }


  template<class T>
    matrix<T>& maskFunctor<T>::multiply(const matrix<T>& m1,
                                              matrix<T>& m2) {
    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (norm == T(1)) {
      apply(m1,m2,multiplyNoNorm);
    } else {
      apply(m1,m2,multiplyNorm);
    }

    return m2;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::multiply(const matrix<T>& m1,
                                      const matrix<T>& m2,
                                            matrix<T>& result) {
    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (norm == T(1)) {
      apply(m1,m2,result,multiplyNoNorm);
    } else {
      apply(m1,m2,result,multiplyNorm);
    }

    return result;
  }


  template<class T>
    matrix<T>& maskFunctor<T>::algebraicSum(const matrix<T>& m1,
                                                  matrix<T>& m2) {

    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (norm == T(1)) {
      apply(m1,m2,algebraicSumNoNorm);
    } else {
      apply(m1,m2,algebraicSumNorm);
    }

    return m2;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::algebraicSum(const matrix<T>& m1,
                                          const matrix<T>& m2,
                                          matrix<T>& result) {

    T norm;
    norm = typeInfo<T>::suggestedNorm();

    if (norm == T(1)) {
      apply(m1,m2,result,algebraicSumNoNorm);
    } else {
      apply(m1,m2,result,algebraicSumNorm);
    }

    return result;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::maskAnd(const matrix<T>& m1,
                                           matrix<T>& m2) {

    apply(m1,m2,andNorm);
    return m2;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::maskAnd(const matrix<T>& m1,
                                     const matrix<T>& m2,
                                           matrix<T>& result) {

    apply(m1,m2,result,andNorm);
    return result;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::maskOr(const matrix<T>& m1,
                                          matrix<T>& m2) {

    apply(m1,m2,orNorm);
    return m2;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::maskOr(const matrix<T>& m1,
                                    const matrix<T>& m2,
                                          matrix<T>& result) {

    apply(m1,m2,result,orNorm);
    return result;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::invert(matrix<T>& result) {
    apply(result,invertNorm);
    return result;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::invert(const matrix<T>& m1,
                                          matrix<T>& result) {
    apply(m1,result,invertNorm);
    return result;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::maskNot(matrix<T>& result) {

    apply(result,notMaskNorm);
    return result;
  }

  template<class T>
  matrix<T>& maskFunctor<T>::maskNot(const matrix<T>& m1,
                                           matrix<T>& result) {
    apply(m1,result,notMaskNorm);
    return result;

  }


}

