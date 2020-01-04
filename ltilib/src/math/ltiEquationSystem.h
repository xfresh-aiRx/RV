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
 * file .......: ltiEquationSystem.h
 * authors ....: Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 02.06.99
 * revisions ..: $Id: ltiEquationSystem.h,v 1.17 2005/06/05 16:09:45 doerfler Exp $
 */

#ifndef _LTI_EQUATION_SYSTEM_H_
#define _LTI_EQUATION_SYSTEM_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"

namespace lti {
  /** Abstract base class for all lti linear equation system solution functors
      to solve Ax=b. */
  template<class T>
  class linearEquationSystemSolutionMethod : public linearAlgebraFunctor {
  public:
    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const
    {return "linearEquationSystemSolutionMethod";};
  };

  /**
   * Abstract base class for all lti linear equation system solution functors
   * using matrix decomposition to solve Ax=b.
   */
  template<class T>
  class decompositionSolution : public linearEquationSystemSolutionMethod<T> {
  public:
    /**
     * decompositionSolution parameter class
     */
    class parameters
      : public linearEquationSystemSolutionMethod<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : linearEquationSystemSolutionMethod<T>::parameters() {};
      /**
       * copy constructor
       */
      parameters(const parameters& other)
        : linearEquationSystemSolutionMethod<T>::parameters() {
        copy(other);
      };

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const
      {return "decompositionSolution::parameters";};

      /**
       * copy member.
       *
       * if the system-matrix of the other object is a normal matrix,
       * its contents will be copied to the system-matrix of this
       * parameters-object.  If the other system-matrix contains just
       * a reference to external data, then the system-matrix of this
       * object will contain the same reference to the data!  The
       * reason for this is to allow the use of this functor with huge
       * matrices without the need of duplication.  Use this data
       * referencing option carefully!
       */
      parameters& copy(const parameters& other) {
#ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        linearEquationSystemSolutionMethod<T>::parameters::copy(other);
#else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters&
          (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
#endif

        // if other has its own data, make a copy of it
        if (other.systemMatrix.ownsData()) {
          systemMatrix.copy(other.systemMatrix);
        } else {
          // if other does not own its data, continue referencing the original
          // data (this is used in case of huge matrices (> 1GB!)
          systemMatrix.useExternData(other.systemMatrix.rows(),
                                     other.systemMatrix.columns(),
                                     const_cast<T*>(&other.systemMatrix.at(0,0)));
        }

        return (*this);
      };
      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };

      // -------------------------------------------------------
      // the parameters
      // -------------------------------------------------------

      /**
       * matrix A
       */
      matrix<T> systemMatrix;

    };

    /**
     * default constructor
     */
    decompositionSolution();

    /**
     * constructor, sets the parameters
     */
    decompositionSolution(const parameters& theParams);

    /**
     * constructor, sets the matrix A
     */
    decompositionSolution(const matrix<T>& theMatrix);

    /**
     * destructor
     */
    virtual ~decompositionSolution() {};

    /**
     * sets the functor's parameters.
     * @return true if successful, false otherwise.
     */
    virtual bool updateParameters();

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /**
     * copy data of "other" functor.
     */
    decompositionSolution<T>& copy(const decompositionSolution<T>& other);
    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {return "decompositionSolution";};

  protected:
    //decompositionSolution<T>& copy(
    //  const decompositionSolution<T>& other);

    bool decomposed;
    matrix<T> dcmpMat;
  };

  /**
   * QR solution functor.
   * Solves the linear equation Ax=b as a least-squares problem using QR
   * decomposition A=QR  (Householder transformation) of the given
   * (m,n)-matrix A.
   * @see decompositionSolution::parameters
   */
  template<class T>
  class qrSolution : public decompositionSolution<T> {
  public:
    /**
     * qrSolution parameter class
     */
    class parameters : public decompositionSolution<T>::parameters {
    public:
      /**
       * default constructor
       */
      parameters()
        : decompositionSolution<T>::parameters(),computeResiduum(false) {};

     /**
       * copy constructor
       */
      parameters(const parameters& other)
        : decompositionSolution<T>::parameters() {
        copy(other);
      };

      /**
       * compute residuum?
       */
      bool computeResiduum;

      /**
       * returns the name of this type
       */
      virtual const char* getTypeName() const {
        return "qrSolution::parameters";
      };

      /**
       * copy member.
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        decompositionSolution<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        decompositionSolution<T>::parameters&
          (decompositionSolution<T>::parameters::* p_copy)
          (const decompositionSolution<T>::parameters&) =
          decompositionSolution<T>::parameters::copy;
        (this->*p_copy)(other);
#     endif

        computeResiduum = other.computeResiduum;

        return (*this);
      };
      /**
       * returns a pointer to a clone of the parameters.
       */
      virtual functor::parameters* clone() const {
        return (new parameters(*this));
      };
    };

    /**
     * default constructor
     */
    qrSolution() : decompositionSolution<T>() {
      parameters params;
      setParameters(params);
    };

    /**
     * constructor, sets the parameters
     */
    qrSolution(const parameters& theParams) :
      decompositionSolution<T>(theParams) {};

    /**
     * constructor, sets the matrix A
     */
    qrSolution(const matrix<T>& theMatrix) :
      decompositionSolution<T>(theMatrix) {
      parameters tmpParam;
      tmpParam.systemMatrix = theMatrix;
      setParameters(tmpParam);
    };

    /**
     * returns the current parameters.
     */
    const parameters& getParameters() const;

    /** onPlace version of apply.
        Solves the least-squares problem Ax=b and returns the residuum if
        computeResiduum==true.

        WARNING: For use with multiple right sides b of a set of
        equation systems Ax=b, the matrix decomposition is computed
        only on calling <em>apply()</em> the first time. After that
        the existing decomposition will be used until calling
        <em>setParameters()</em>.
    */
    double apply(vector<T>& b);

    /** onCopy version of apply.
        Solves the least-squares problem Ax=b and returns the residuum if
        computeResiduum==true.

        WARNING: For use with multiple right sides b of a set of
        equation systems Ax=b, the matrix decomposition is computed
        only on calling <em>apply()</em> the first time. After that
        the existing decomposition will be used until calling
        <em>setParameters()</em>.
    */
    double apply(const vector<T>& b,vector<T>& x);

    /**
     * copy data of "other" functor.
     */
    qrSolution& copy(const qrSolution& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const {
      return (new qrSolution<T>(*this));
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "qrSolution";
    };

  protected:
    //qrSolution<T>& copy(const qrSolution<T>& other);

    vector<double> dcmpVec;
    vector<double> helpVec;
  };

  /**
   * LU solution functor.
   * Solves the linear equation Ax=b using LU decomposition.
   */
  template<class T>
  class luSolution : public decompositionSolution<T> {
  public:
    typedef typename decompositionSolution<T>::parameters parameters;

    /**
     * default constructor
     */
    luSolution() : decompositionSolution<T>() {};

    /**
     * constructor, sets the parameters
     * @see decompositionSolution::parameters
     */
    luSolution(const parameters& theParams) :
      decompositionSolution<T>(theParams) {};

    /**
     * constructor, sets the matrix A
     */
    luSolution(const matrix<T>& theMatrix) :
      decompositionSolution<T>(theMatrix) {};

    /**
     * onPlace version of apply.
     * Solves the set of n linear equations Ax=b. For use with multiple right
     * sides b of a set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.
     * After that the existing decomposition will be used until calling
     * setParameters.*/
    bool apply(vector<T>& b);

    /**
     * onCopy version of apply.
     * Solves the set of n linear equations Ax=b. For use with multiple right
     * sides b of a set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.
     * After that the existing decomposition will be used until calling
     * setParameters.
     */
    bool apply(const vector<T>& b,vector<T>& x);

    /**
     * onPlace version of apply.  Solves the set of n linear equations
     * A x=b where x is the i-th _column_ vector of X and b the i-th
     * _column_ vector of B. For use with multiple right sides b of a
     * set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.  After that the
     * existing decomposition will be used until calling
     * setParameters.
     */
    bool apply(matrix<T>& B);

    /**
     * onCopy version of apply.  Solves the set of n linear equations
     * A x=b where x is the i-th _column_ vector of X and b the i-th
     * _column_ vector of B. For use with multiple right sides b of a
     * set of equation systems Ax=b, the matrix decomposition is
     * computed only on calling apply the first time.  After that the
     * existing decomposition will be used until calling
     * setParameters.
     */
    bool apply(const matrix<T>& B,matrix<T>& X);

    /**
     * copy data of "other" functor.
     */
    luSolution& copy(const luSolution& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const { return (new luSolution<T>(*this));};

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {return "luSolution";};

  protected:
    //luSolution<T>& copy(const luSolution<T>& other);

    vector<int> dcmpVec;
  };
}

#endif

