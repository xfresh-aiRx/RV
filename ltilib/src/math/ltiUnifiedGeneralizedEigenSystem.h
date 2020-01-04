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
 * file .......: ltiUnifiedGeneralizedEigenSystem.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 21.11.2003
 * revisions ..: $Id: ltiUnifiedGeneralizedEigenSystem.h,v 1.6 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_UNIFIED_GENERALIZED_EIGEN_SYSTEM_H_
#define _LTI_UNIFIED_GENERALIZED_EIGEN_SYSTEM_H_

#include "ltiGeneralizedEigenSystem.h"
#include "ltiLinearAlgebraFunctor.h"


namespace lti {
  /**
   * This functor is meant to be used for solving any generalized
   * Eigenvalue problem without need to think about the most efficient
   * usage of the available methods. If the LAPACK is available it
   * will automatically be used since it is much faster. 
   *
   * This functor only handles generalized Eigenvalue problems. For
   * non-generalized EV problems use there is only a LAPACK functor:
   * genEigenSystem. For the generalized eigenvalue problem either
   * generalizedEigenSystem or fastGeneralizedEigenSystem can be used.
   *
   * It is preferred to use this class instead of the generalized
   * eigenSystem functors available.
   */
  template<class T>
  class unifiedGeneralizedEigenSystem : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class unifiedGeneralizedEigenSystem.
     *
     * Note that the parameter problemType of
     * fastGeneralizedEigenSystem is statically set to Type1 in this
     * functor, which results in the same functionality as that of
     * generalizedEigenSystem. If you need other types use the LAPACK
     * method directly.
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * The Eigensystem solution method to use. Note that functors
       * starting with Fast___ are only available if LAPACK is
       * used. The default is set according to your installation.
       */
      enum eEigenSystemType {
        GeneralizedEigenSystem,         /*!< use generalizedEigenSystem */
        FastGeneralizedEigenSystem = 10 /*!< use fastGeneralizedEigenSystem */
      };

      /**
       * default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        
        sort = true;
        dimensions = 0;
        useDC = true;
#ifdef HAVE_LAPACK
        esType = FastGeneralizedEigenSystem;
#else
        esType = GeneralizedEigenSystem;
#endif
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : linearAlgebraFunctor::parameters() {
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
        return "unifiedGeneralizedEigenSystem::parameters";
      };

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
        linearAlgebraFunctor::parameters& (linearAlgebraFunctor::parameters::* p_copy)
                            (const linearAlgebraFunctor::parameters&) =
                            linearAlgebraFunctor::parameters::copy;
        (this->*p_copy)(other);
#     endif

        
        sort = other.sort;
        dimensions = other.dimensions;
        esType = other.esType;
        
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
          
          lti::write(handler,"sort",sort);
          lti::write(handler,"dimensions",dimensions);
          
          switch (esType) {
            case GeneralizedEigenSystem:
              lti::write(handler,"esType","GeneralizedEigenSystem");
              break;
            case FastGeneralizedEigenSystem:
              lti::write(handler,"esType","FastGeneralizedEigenSystem");
              break;
            default:
#ifdef HAVE_LAPACK
              lti::write(handler,"esType","FastGeneralizedEigenSystem");
#else              
              lti::write(handler,"esType","GeneralizedEigenSystem");
#endif
              break;
          }

        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && linearAlgebraFunctor::parameters::write(handler,false);
#     else
        bool (linearAlgebraFunctor::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          linearAlgebraFunctor::parameters::writeMS;
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
          
          lti::read(handler,"sort",sort);
          lti::read(handler,"dimensions",dimensions);

          std::string tmp;
          lti::read(handler,"esType",tmp);
          if (tmp=="GeneralizedEigenSystem") {
            esType = GeneralizedEigenSystem;
          } else if (tmp=="FastGeneralizedEigenSystem") {
            esType = FastGeneralizedEigenSystem;
          } else {
#ifdef HAVE_LAPACK
            esType = FastGeneralizedEigenSystem;
#else
            esType = GeneralizedEigenSystem;
#endif
          }

        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && linearAlgebraFunctor::parameters::read(handler,false);
#       else
        bool (linearAlgebraFunctor::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          linearAlgebraFunctor::parameters::readMS;
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


      /**
       * If true the eigenvalues and eigenvectors are sorted in
       * descending order. Default true.
       *
       * See eigenSystem<T>::parameters::sort.
       */
      bool sort;

      /**
       * The number of eigenvalues/-vectors to be calculated. If zero
       * all EV are calculated. Default 0.
       *
       * See also:
       * - generalizedEigenSystem<T>::parameters::dimensions
       * - fastGeneralizedEigenSystem<T>::parameters::dimensions
       */
      int dimensions;

      /**
       * The eigenSystem given is used. Note that you cannot use
       * fastGeneralizedEigenSystem if LAPACK is not installed. The
       * default is dependent on the the system configuration: If
       * LAPACK is available, fastGeneralizedEigenSystem is used,
       * otherwise generalizedEigenSystem.
       */
      eEigenSystemType esType;

      /**
       * This parameters is used only if
       * esType==FastGeneralizedEigenSystem. It determines whether the
       * divide and conquer strategy is used or not. Default true.
       *
       * See fastGeneralizedEigenSystem<T>::parameters::useDC
       */
      bool useDC;

    };

    /**
     * default constructor
     */
    unifiedGeneralizedEigenSystem();

    /**
     * Construct a functor using the given parameters
     */
    unifiedGeneralizedEigenSystem(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    unifiedGeneralizedEigenSystem(const unifiedGeneralizedEigenSystem& other);

    /**
     * destructor
     */
    virtual ~unifiedGeneralizedEigenSystem();

    /**
     * returns the name of this type ("unifiedGeneralizedEigenSystem")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unifiedGeneralizedEigenSystem& 
    copy(const unifiedGeneralizedEigenSystem& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unifiedGeneralizedEigenSystem& 
    operator=(const unifiedGeneralizedEigenSystem& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Update functor's parameters.
     *
     * Update some internal data according to the parameters.
     *
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Read the functor from the given ioHandler. 
     *
     * This method automatically sets the correct eigensystem solver
     * that is given in the parameters and transfers the relevant
     * parameters to that functor.
     *
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

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
    inline bool apply(const matrix<T>& a,
                      const matrix<T>& b,
                      vector<T>& eigenvalues,
                      matrix<T>& eigenvectors) const {
      return eigen->apply(a,b,eigenvalues,eigenvectors);
    }

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
    inline bool apply(const matrix<T>& a,
                      const matrix<T>& b,
                      vector<T>& eigenvalues) const {
      return eigen->apply(a,b,eigenvalues);
    }
    
  protected:
    
    /**
     * The actual eigenSystem.
     */
    generalizedEigenSystem<T>* eigen;
    
  };
}

#endif

