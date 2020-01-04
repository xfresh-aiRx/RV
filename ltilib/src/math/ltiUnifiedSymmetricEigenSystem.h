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
 * file .......: ltiUnifiedSymmetricEigenSystem.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 18.11.2003
 * revisions ..: $Id: ltiUnifiedSymmetricEigenSystem.h,v 1.8 2005/11/04 15:34:55 doerfler Exp $
 */

#ifndef _LTI_UNIFIED_SYMMETRIC_EIGEN_SYSTEM_H_
#define _LTI_UNIFIED_SYMMETRIC_EIGEN_SYSTEM_H_

#include "ltiEigenSystem.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   * This functor is meant to be used for solving any symmetric
   * Eigenvalue problem without need to think about the most efficient
   * usage of the available methods. If LAPACK is available it will
   * automatically be used since it is much faster than other
   * implementations in the LTI-Lib.
   *
   * This functor only handles symmetric Eigenvalue problems. For
   * non-symmetric or general EV problems there is only a LAPACK
   * functor: genEigenSystem. For the generalized symmetric eigenvalue
   * problem use unifiedGeneralizedEigenSystem.
   *
   * It is preferred to use this class instead of the symmetric
   * eigenSystem functors available (fastEigenSystem, jacobi).
   */
  template<class T>
  class unifiedSymmetricEigenSystem : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class unifiedSymmetricEigenSystem
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * The Eigensystem solution method to use. Note that functors
       * starting with Fast___ are only available if LAPACK is
       * used. The default is set according to your installation.
       */
      enum eEigenSystemType {
        Jacobi,               /*!< use jacobi */
        FastEigenSystem = 10  /*!< use fastEigenSystem */
      };

      /**
       * default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        
        sort = true;
        dimensions = 0;
        useRRR = false;
#ifdef HAVE_LAPACK
        esType = FastEigenSystem;
#else
        esType = Jacobi;
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
        return "unifiedSymmetricEigenSystem::parameters";
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

        
        sort       = other.sort;
        dimensions = other.dimensions;
        useRRR     = other.useRRR;
        esType     = other.esType;
        
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
          lti::write(handler,"useRRR",useRRR);
          
          switch (esType) {
            case Jacobi:
              lti::write(handler,"esType","Jacobi");
              break;
            case FastEigenSystem:
              lti::write(handler,"esType","FastEigenSystem");
              break;
            default:
#ifdef HAVE_LAPACK
              lti::write(handler,"esType","FastEigenSystem");
#else              
              lti::write(handler,"esType","Jacobi");
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
          lti::read(handler,"useRRR",useRRR);

          std::string tmp;
          lti::read(handler,"esType",tmp);
          if (tmp=="Jacobi") {
            esType = Jacobi;
          } else if (tmp=="FastEigenSystem") {
            esType = FastEigenSystem;
          } else {
#ifdef HAVE_LAPACK
            esType = FastEigenSystem;
#else
            esType = Jacobi;
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
       * - jacobi<T>::parameters::dimensions
       * - fastEigenSystem<T>::parameters::dimensions
       */
      int dimensions;

      /**
       * This parameter is used only if esType=FastEigenSystem. It
       * determines whether the relatively robust representation is
       * used. This can be much faster than the default but might also
       * hang. For this reason:
       *
       * Default: false
       *
       * See alse:
       * - fastEigenSystem<T>::parameters::useRRR;
       */
      bool useRRR;

      /**
       * The eigenSystem given is used. Note that you cannot use
       * fastEigenSystem if LAPACK is not installed. The default is
       * dependent on the the system configuration: If LAPACK is
       * available, fastEigenSystem is used, otherwise jacobi.
       */
      eEigenSystemType esType;


    };

    /**
     * default constructor
     */
    unifiedSymmetricEigenSystem();

    /**
     * Construct a functor using the given parameters
     */
    unifiedSymmetricEigenSystem(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    unifiedSymmetricEigenSystem(const unifiedSymmetricEigenSystem& other);

    /**
     * destructor
     */
    virtual ~unifiedSymmetricEigenSystem();

    /**
     * returns the name of this type ("unifiedSymmetricEigenSystem")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unifiedSymmetricEigenSystem& copy(const unifiedSymmetricEigenSystem& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unifiedSymmetricEigenSystem& 
    operator=(const unifiedSymmetricEigenSystem& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * set functor's parameters.
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
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
     * The apply method calls the eigenSystem method chosen in the
     * parameters or automatically derived from the system
     * configuration.
     * 
     * @param theMatrix matrix to be transformed
     * @param eigenvalues elements will contain the eigenvalues
     * @param eigenvectors columns will contain the eigenvectors
     *        corresponding to the eigenvalues
     * @return bool true if successful, false otherwise.
     */
    inline bool apply(const matrix<T>& theMatrix,
                      vector<T>& eigenvalues,
                      matrix<T>& eigenvectors) const {
      return eigen->apply(theMatrix, eigenvalues, eigenvectors);
    }
    
  protected:
    
    /**
     * The actual eigenSystem.
     */
    eigenSystem<T>* eigen;
    
  };
}

#endif

