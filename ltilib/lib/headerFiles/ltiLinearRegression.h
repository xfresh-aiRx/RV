/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiLinearRegression.h
 * authors ....: Holger Fillbrandt
 * organization: LTI, RWTH Aachen
 * creation ...: 4.7.2003
 * revisions ..: $Id: ltiLinearRegression.h,v 1.5 2005/06/16 12:37:35 doerfler Exp $
 */


#ifndef _LTI_LINEAR_REGRESSION_H_
#define _LTI_LINEAR_REGRESSION_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiEigenSystem.h"
#include "ltiIoHandler.h"

#ifdef HAVE_LAPACK
#include "ltiFastEigenSystem.h"
#endif

namespace lti {
  /**
   * The linear regression algorithm estimates a linear relation between
   * two vector spaces. To each vector <code>srcVect</code> of the source vector space with
   * dimension <code>srcDim</code>, the corresponding vector <code>destDim</code> of the destination
   * vector space with dimension <code>destDim</code> can be calculated using the resulting
   * regression matrix R:
   *
   *                           destVect = R * srcVect.
   *
   * R is calculated from a training set of corresponding vectors in both spaces.
   * Note that <code>srcVect</code> = 0 can only result in <code>destVect</code> = 0, so it is important
   * to eliminate any offsets from both vector spaces before using this algorithm:
   *
   *              destVect = destVectMean + R * (srcVect - srcVectMean)
   *
   * For example application see ltiLinearRegressionTracking.
   *
   * @ingroup gLinearAlgebra
   */
  template <class T>
  class linearRegression : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class linearRegression
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() 
        : linearAlgebraFunctor::parameters() {

        eigenSystemDim = 10; // dimensionality of eigensystem   

      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) 
        : linearAlgebraFunctor::parameters() {
        copy(other);
      }


      /**
       * destructor
       */
      ~parameters(){
      }

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "linearRegression::parameters";
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearAlgebraFunctor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        eigenSystemDim=other.eigenSystemDim;

        return *this;
      }


      /**
       * Assigns the contents of the other object to this object
       */
      inline parameters& operator=(const parameters& other) {
        copy(other);
        return *this;
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.readBegin();
        }

        if (b) {
          lti::read(handler,"eigenSystemDim",eigenSystemDim);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::read(handler,false);
#     else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#     endif

        if (complete) {
          b=b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b=handler.writeBegin();
        }

        if (b) {
          lti::write(handler,"eigenSystemDim",eigenSystemDim);

        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::write(handler,false);
#       else
        bool
        (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#       endif

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif

	    /**
	     * Dimensionality of the eigensystem which is constructed
	     * during calculation of the linear regression matrix.
	     * It must not be bigger than the number of trainingvectors used.
	     *  The quality of the resulting matrix depends on this value, it should
	     * be neither to small nor to big. There are methods to estimate the
	     * optimal value in the literature, but none has been implemented 
	     * in this functor yet. The default is 10.
	     */
	    int eigenSystemDim;


    };

    /**
     * default constructor
     */
    linearRegression();

    /**
     * default constructor with parameters
     * 
     */
    linearRegression(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    linearRegression(const linearRegression& other);

    /**
     * destructor
     */
    virtual ~linearRegression();

    /**
     * returns the name of this type ("linearRegression")
     */
    virtual const char* getTypeName() const;

	  /**
	   * The linear regression algorithm. It calculates a linear transformation
	   * matrix linRegMatrix, that approximately fulfills
     * destMatrix = linRegMatrix * srcMatrix.
	   * @param srcMatrix the columns of this matrix are the source training vectors of dimension srcDim
	   * @param destMatrix the columns of this matrix are the destination training vectors of dimension destDim
	   * @return resulting (destDim x srcDim) transformation matrix. This is also kept in the parameter linRegMatrix and can be used directly with "transform"
	   */
	  matrix<T> apply(matrix<T>& srcMatrix, matrix<T>& destMatrix);

    /**
	   * multiplication of srcVector with the linear regression matrix
     * of dimension (destDim x srcDim)
	   * @param srcVector source vector of dimension srcDim
	   * @param destVector destination vector of dimension destDim
     */
    void transform(vector<T>& srcVector, vector<T>& destVector) const;

    /**
	   * multiplication of srcVector with the linear regression matrix
     * of dimension (destDim x srcDim)
	   * @param srcVector source vector of dimension srcDim
	   * @return destination vector of dimension destDim
     */
    vector<T> transform(vector<T>& srcVector) const;

    /**
     * gets the linear regression matrix
     */
    matrix<T> getLinRegMatrix() const;

    /**
     * sets the linear regression matrix
     */
    void setLinRegMatrix(matrix<T> &linRegMatrixSet);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearRegression& copy(const linearRegression& other);

    /**
     * Alias for copy.
     */
    linearRegression& operator=(const linearRegression& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


    /**
     * Reads this functor from the given handler.
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * Writes this functor to the given handler.
     */
    virtual bool write(ioHandler &handler, const bool complete=true) const;

    protected:
    matrix<T> linRegMatrix;

  };

  template <class T>
  bool read(ioHandler& handler,
            linearRegression<T>& linReg,
            const bool complete=true) {
    return linReg.read(handler,complete);
  }


  template <class T>
  bool write(ioHandler& handler,
             const linearRegression<T>& linReg,
             const bool complete=true) {
    return linReg.write(handler,complete);
  }

}

#endif


