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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiGeneralizedEigenSystem.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 10.7.2003
 * revisions ..: $Id: ltiGeneralizedEigenSystem.h,v 1.8 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_GENERALIZED_EIGEN_SYSTEM_H_
#define _LTI_GENERALIZED_EIGEN_SYSTEM_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiCholeskyDecomposition.h"
#include "ltiForwardSubstitution.h"
#include "ltiBackSubstitution.h"
#include "ltiEigenSystem.h"

namespace lti {
  /**
   *  Computes all the eigenvalues, and optionally, the eigenvectors
   *  of a real generalized symmetric-definite eigenproblem, of the
   *  form A*x=(lambda)*B*x.  Here A and B are assumed to be symmetric
   *  and B is also positive definite.
   *
   *  If parameters::sort is true the eigenvalues and corresponding
   *  eigenvectors are sorted in descending order. Default is true.
   */
  template<class T>
  class generalizedEigenSystem : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class generalizedEigenSystem
     */
    class parameters : public eigenSystem<T>::parameters {
    public:
      /**
       * default constructor. Set sort to true.
       */
      parameters() : eigenSystem<T>::parameters(), dimensions(0) {
        
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : eigenSystem<T>::parameters() {
        copy(other);
      }
      
      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "generalizedEigenSystem::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        eigenSystem<T>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
                            (const functor::parameters&) =
                            functor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        
        dimensions = other.dimensions;

        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }
        
        if (b) {
          
          lti::write(handler,"dimensions",dimensions);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && eigenSystem<T>::parameters::write(handler,false);
#     else
        bool (eigenSystem<T>::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          eigenSystem<T>::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif


#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }
        
        if (b) {
          
          lti::read(handler,"dimensions",dimensions);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && eigenSystem<T>::parameters::read(handler,false);
#       else
        bool (eigenSystem<T>::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          eigenSystem<T>::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      

      /**
       * number of dimensions calculated. The default is zero. In this
       * case, all eigenvectors and eigenvalues are calculated.
       *
       * This option is only provided for compatibility with the
       * fastGeneralizedEigenSystem functor based on LAPACK, but it
       * does not make (yet) the computation of the eigensolution any
       * faster.  It just will cut the already computed complete
       * solution to the desired size!
       *
       * Default value: 0 (implying all eigenvalues will be computed)
       */
      int dimensions;

    };

    /**
     * default constructor
     */
    generalizedEigenSystem();

    /**
     * Construct a functor using the given parameters
     */
    generalizedEigenSystem(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    generalizedEigenSystem(const generalizedEigenSystem& other);

    /**
     * destructor
     */
    virtual ~generalizedEigenSystem();

    /**
     * returns the name of this type ("generalizedEigenSystem")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes eigenvalues and eigenvectors of the given matrix. The
     * functor can efficiently calculate only a few dimensions of the
     * eigenspace, taking those with the largest eigenvalues. The
     * number of dimensions is set with parameters::dimensions.
     *
     * @param a the symmetric matrix A
     * @param b the symmetric, positive definite matrix B
     * @param eigenvalues  elements will contain the eigenvalues
     * @param eigenvectors columns will contain the  eigenvectors
     *                     corresponding to the eigenvalues
     */
    virtual bool apply(const matrix<T>& a,
                       const matrix<T>& b,
                       vector<T>& eigenvalues,
                       matrix<T>& eigenvectors) const;

    /**
     * Computes eigenvalues and eigenvectors of the given matrix. The
     * functor can efficiently calculate only a few dimensions of the
     * eigenspace, taking those with the largest eigenvalues. The
     * number of dimensions is set with parameters::dimensions.
     *
     * @param a the symmetric matrix A
     * @param b the symmetric, positive definite matrix B
     * @param eigenvalues  elements will contain the eigenvalues
     */
    virtual bool apply(const matrix<T>& a,
                       const matrix<T>& b,
                       vector<T>& eigenvalues) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    generalizedEigenSystem& copy(const generalizedEigenSystem& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    generalizedEigenSystem& operator=(const generalizedEigenSystem& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  private:

    choleskyDecomposition<T> cholesky;
    
    forwardSubstitution<T> fSubst;
    backSubstitution<T> bSubst;

    jacobi<T> simpleEigen;
  };
}

#endif

