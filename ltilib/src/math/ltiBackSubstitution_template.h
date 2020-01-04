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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiBackSubstitution.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 20.5.2003
 * revisions ..: $Id: ltiBackSubstitution_template.h,v 1.3 2004/05/03 12:20:00 alvarado Exp $
 */

//TODO: include files
#include "ltiBackSubstitution.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 3
#include "ltiDebug.h"

namespace lti {
  // --------------------------------------------------
  // backSubstitution::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // backSubstitution
  // --------------------------------------------------

  // default constructor
  template<class T>
  backSubstitution<T>::backSubstitution()
    : linearAlgebraFunctor(){

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);
  }

  template<class T>
  backSubstitution<T>::backSubstitution(const matrix<T>& theMatrix, 
                                        const typename parameters::eTriangularType& tType)
    : linearAlgebraFunctor() {

    // create an instance of the parameters and set given values
    parameters pa;
    pa.theMatrix.copy(theMatrix);
    pa.triangularType=tType;
    // set the parameters
    setParameters(pa);
  }

  // default constructor
  template<class T>
  backSubstitution<T>::backSubstitution(const parameters& par)
    : linearAlgebraFunctor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class T>
  backSubstitution<T>::backSubstitution(const backSubstitution<T>& other)
    : linearAlgebraFunctor() {
    copy(other);
  }

  // destructor
  template<class T>
  backSubstitution<T>::~backSubstitution() {
  }

  // returns the name of this type
  template<class T>
  const char* backSubstitution<T>::getTypeName() const {
    return "backSubstitution<T>";
  }

  // copy member
  template<class T>
  backSubstitution<T>&
  backSubstitution<T>::copy(const backSubstitution<T>& other) {
    linearAlgebraFunctor::copy(other);
    return (*this);
  }

  // alias for copy member
  template<class T>
  backSubstitution<T>&
  backSubstitution<T>::operator=(const backSubstitution<T>& other) {
    return (copy(other));
  }


  // clone member
  template<class T>
  functor* backSubstitution<T>::clone() const {
    return new backSubstitution<T>(*this);
  }

  // return parameters
  template<class T>
  const typename backSubstitution<T>::parameters&
  backSubstitution<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------


  /**
   * Performs back substitution with unknown \a x.  U*x=b if \a
   * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
   * triagonal. The result \a x is left in \a b. The matrix in the
   * parameters is used.
   *
   * @param b the right side of the back substitution. The result
   *          will be left here too.
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(vector<T>& b) const {

    const parameters& p = getParameters();
    return apply(p.theMatrix, b, p.triangularType);

  };

  /**
   * Performs back substitution with unknown \a x.  U*x=b if \a
   * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
   * triagonal. The matrix in the parameters is used.
   *
   * @param b the right side of the back substitution. 
   * @param x the result.
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(const vector<T>& b, vector<T>& x) const {

    const parameters& p = getParameters();
    x.copy(b);
    return apply(p.theMatrix, x, p.triangularType);

  };

  /**
   * Performs back substitution with unknown \a x.  U*x=b if \a
   * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
   * triagonal. The result \a x is left in \a b. 
   *
   * @param theMatrix data matrix (L or U)
   * @param b the right side of the back substitution. The result
   *          will be left here too.
   * @param tType defines whether theMatrix is lower or upper triangular
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(const matrix<T>& theMatrix, vector<T>& b,
             const typename parameters::eTriangularType& tType) const {

    //actually we need reverse iterators here, but we use the ones available
    //which has the _nice_ effect that operator -- still means going backward.
    
    const int r=theMatrix.rows();
    const int c=theMatrix.columns();

    typename vector<T>::iterator yit=b.inverseBegin();
    typename vector<T>::iterator bit=yit;
    typename vector<T>::iterator bend=b.inverseEnd();
    T sum=T(0);
    
    if (tType==parameters::Upper) {

      _lti_debug("tType is Upper\n");

      typename vector<T>::const_iterator ait;
      int i;

      *yit=(*bit)/theMatrix.at(r-1,c-1);
      --yit;

      for (i=r-2; yit!=bend; i--, --yit) {
        sum=T(0);
        bit=b.inverseBegin();
        ait=theMatrix.getRow(i).inverseBegin();
        while (bit!=yit) {
          sum+=(*ait)*(*bit);
          _lti_debug3("ait=" << (*ait) << "  bit=" << (*bit) 
                      << "  sum=" << sum << "\n");
          --ait;
          --bit;
        }
        _lti_debug3("y["<<i<<"]= "<<(*yit)<<"\n");
        *(yit)-=sum;
        _lti_debug3("y["<<i<<"]= "<<(*yit)<<"\n");
        *(yit)/=(*ait);
        _lti_debug3("y["<<i<<"]= "<<(*yit)<<"\n");
      }
    } else if (tType==parameters::Lower) {
      
      _lti_debug("tType is Lower\n");

      if (theMatrix.getMode()==matrix<T>::Connected) {
        //we can work with iterators to get column values from theMatrix
      
        _lti_debug("theMatrix is Connected\n");
      
        typename matrix<T>::const_iterator aStart=theMatrix.inverseBegin();
        typename matrix<T>::const_iterator ait;

        *yit=(*bit)/(*aStart);
        --yit;
        --aStart;

        for (;yit!=bend; --yit, --aStart) {
          sum=T(0);
          bit=b.inverseBegin();
          ait=aStart;
          while (bit!=yit) {
            sum+=(*ait)*(*bit);
            ait-=c;
            --bit;
          }
          *(yit)-=sum;
          *(yit)/=(*ait);
        }
  
      } else {
        //theMatrix must be accessed with at() - grmpf!

        _lti_debug("theMatrix is NOT Connected\n");

        int i,j;

        *yit=(*bit)/theMatrix.at(r-1,c-1);
        ++yit;

        for (i=r-2; yit!=bend; i--, --yit) {
          sum=T(0);
          bit=b.inverseBegin();
          for (j=c-1; bit!=yit; j--, --bit) {
            sum+=theMatrix.at(j,i)*(*bit);
          }
          *(yit)-=sum;
          *(yit)/=theMatrix.at(i,i);
        }

      }
      
    } else {
      setStatusString("Unknown type of eTriangularType");
      return false;
    }

    return true;

  };

  /**
   * Performs back substitution with unknown \a x.  U*x=b if \a
   * theMatrix is upper triagonal and x*L=b if \a theMatrix is lower
   * triagonal. 
   *
   * @param theMatrix data matrix (L or U)
   * @param b the right side of the back substitution. The result
   *          will be left here too.
   * @param x the result.
   * @param tType defines whether theMatrix is lower or upper triangular
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(const matrix<T>& theMatrix, 
                                     const vector<T>& b,
                                     vector<T>& x, 
                                     const typename parameters::eTriangularType& tType) const {

    x.copy(b);
    return apply(theMatrix,x,tType);
  };


  /**
   * Performs back substitution with unknowns \a X.  U*X=B if \a
   * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
   * triagonal. In the first case \a X and \a B contain the vectors
   * in colums, in the latter case in rows. The result \a x is left
   * in \a b. The matrix in the parameters is used.
   *
   * @param b the right side of the back substitution. The result
   *          will be left here too.
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(matrix<T>& b) const {
    
    const parameters& pa=getParameters();
    return apply(pa.theMatrix,b,pa.triangularType);
  };

  /**
   * Performs back substitution with unknowns \a X.  U*X=B if \a
   * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
   * triagonal. In the first case \a X and \a B contain the vectors
   * in colums, in the latter case in rows. The data matrix in the
   * parameters is used.
   *
   * @param b the right side of the back substitution. 
   * @param x the result
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(const matrix<T>& b, matrix<T>& x) const {

    const parameters& pa=getParameters();
    return apply(pa.theMatrix,b,x,pa.triangularType);

  };

  /**
   * Performs back substitution with unknowns \a X.  U*X=B if \a
   * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
   * triagonal. In the first case \a X and \a B contain the vectors
   * in colums, in the latter case in rows. The result \a x is left
   * in \a b.
   *
   * @param theMatrix data matrix (L or U)
   * @param b the right side of the back substitution. The result
   *          will be left here too.
   * @param tType defines whether theMatrix is lower or upper triangular
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(const matrix<T>& theMatrix,
                                  matrix<T>& b, 
                                  const typename parameters::eTriangularType& tType) const {
    
    if (tType==parameters::Upper) {
      // solving U*X=B, thus vectors are in columns => transpose
      b.transpose();
    }
    int rows=b.rows();
    int i;
    bool ok=true;
    for (i=0; i<rows; i++) {
      ok = ok && apply(theMatrix,b.getRow(i),tType);
    }
    if (tType==parameters::Upper) {
      b.transpose();
    }

    return ok;
  };

  /**
   * Performs back substitution with unknowns \a X.  U*X=B if \a
   * theMatrix is upper triagonal and X*L=B if \a theMatrix is lower
   * triagonal. In the first case \a X and \a B contain the vectors
   * in colums, in the latter case in rows.
   *
   * @param theMatrix data matrix (L or U)
   * @param b the right side of the back substitution. The result
   *          will be left here too.
   * @param x the result
   * @param tType defines whether theMatrix is lower or upper triangular
   * @return true if apply successful or false otherwise.
   */
  template<class T>
  bool backSubstitution<T>::apply(const matrix<T>& theMatrix,
                                  const matrix<T>& b, 
                                  matrix<T>& x, 
                                  const typename parameters::eTriangularType& tType) const {

    if (tType==parameters::Upper) {
      // solving U*X=B, thus vectors are in columns => transpose
      x.transpose(b);
    } else {
      x.copy(b);
    }
    int rows=x.rows();
    int i;
    bool ok=true;
    for (i=0; i<rows; i++) {
      ok = ok && apply(theMatrix,x.getRow(i),tType);
    }
    if (tType==parameters::Upper) {
      x.transpose();
    }

    return ok;
  };



}

#include "ltiUndebug.h"
