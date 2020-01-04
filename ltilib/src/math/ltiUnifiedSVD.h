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
 * file .......: ltiUnifiedSVD.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 20.11.2003
 * revisions ..: $Id: ltiUnifiedSVD.h,v 1.6 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_UNIFIED_SVD_H_
#define _LTI_UNIFIED_SVD_H_

#include "ltiSVD.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   * This functor is meant to be used for calculating the Singular
   * Vector Decomposition without need to think about the most
   * efficient usage of the available methods. If the LAPACK is
   * available it will automatically be used since it is much faster.
   *
   * It is preferred to use this class instead of the SVD functors
   * available.
   */
  template<class T>
  class unifiedSVD : public linearAlgebraFunctor {
  public:
    /**
     * the parameters for the class unifiedSVD
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * The SVD method to use. Note that functors
       * starting with Fast___ are only available if LAPACK is
       * used. The default is set according to your installation.
       */
      enum eSVDType {
        SVD,            /*!< use singularValueDecomp */
        FastSVD = 10    /*!< use fastSVD */
      };

      /**
       * default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        
        sort = true;
        transposeU = false;
        transposeV = false;
        useDC = true;
        useMinDimensions = true;
#ifdef HAVE_LAPACK
        svdType = FastSVD;
#else
        svdType = SVD;
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
        return "unifiedSVD::parameters";
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
        transposeU = other.transposeU;
        transposeV = other.transposeV;
        useDC = other.useDC;
        useMinDimensions = other.useMinDimensions;
        svdType = other.svdType;
        
        
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
          lti::write(handler,"transposeU",transposeU);
          lti::write(handler,"transposeV",transposeV);
          lti::write(handler,"useDC",useDC);
          lti::write(handler,"useMinDimensions",useMinDimensions);
          
          switch (svdType) {
            case SVD:
              lti::write(handler,"svdType","SVD");
              break;
            case FastSVD:
              lti::write(handler,"svdType","FastSVD");
              break;
            default:
#ifdef HAVE_LAPACK
              lti::write(handler,"svdType","FastSVD");
#else              
              lti::write(handler,"svdType","SVD");
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
       * @return true if read was successful
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
          lti::read(handler,"transposeU",transposeU);
          lti::read(handler,"transposeV",transposeV);
          lti::read(handler,"useDC",useDC);
          lti::read(handler,"useMinDimensions",useMinDimensions);

          std::string tmp;
          lti::read(handler,"svdType",tmp);
          if (tmp=="SVD") {
            svdType = SVD;
          } else if (tmp=="FastSVD") {
            svdType = FastSVD;
          } else {
#ifdef HAVE_LAPACK
            svdType = FastSVD;
#else
            svdType = SVD;
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
       * @return true if read was successful
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
       * See singularValueDecomp<T>::parameters::sort.
       */
      bool sort;

      /**
       * If true the transpose of matrix U is returned instead of
       * U. Default false.
       *
       * See singularValueDecomp<T>::parameters::transposeU
       */
      bool transposeU;

      /**
       * If true the transpose of matrix V is returned instead of
       * V. Default false.
       *
       * See singularValueDecomp<T>::parameters::transposeV
       */
      bool transposeV;

      /**
       * This parameters is used only if svdType=FastSVD. It
       * determines whether the divide and conquer strategy is used or
       * not. Default true.
       *
       * See fastSVD<T>::parameters::useDC
       */
      bool useDC;

      /**
       * This parameters is used only if svdType=FastSVD. If true only
       * min(#rows, #columns) of the data matrix singular vectors are
       * calculated. Default true.
       *
       * See fastSVD<T>::parameters::useMinDimensions
       */
      bool useMinDimensions;

      /**
       * The SVD type given is used. Note that you cannot use
       * fastSVD if LAPACK is not installed. The default is
       * dependent on the the system configuration: If LAPACK is
       * available, fastSVD is used, otherwise singularValueDecomp.
       */
      eSVDType svdType;


    };

    /**
     * default constructor
     */
    unifiedSVD();

    /**
     * Construct a functor using the given parameters
     */
    unifiedSVD(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    unifiedSVD(const unifiedSVD& other);

    /**
     * destructor
     */
    virtual ~unifiedSVD();

    /**
     * returns the name of this type ("unifiedSVD")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unifiedSVD& copy(const unifiedSVD& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unifiedSVD& operator=(const unifiedSVD& other);

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
     * This method automatically sets the correct SVD solver
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
     * OnPlace version of Singular Value Decomposition. Singular Value
     * Decomposition means that a m*n-matrix A is decomposed into three
     * matrices U,W,V, such that  A = U*W*V'. U is m*n, W is a diagonal
     * matrix with n elements (which is implemented as vector), V is a
     * n*n-matrix. Note that the function returns V, not V'.
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned. If
     *            src is a m*n matrix, U will also be of size m*n
     * @param w vector<T> with the singular values, sorted descendingly
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    inline bool decomposition(matrix<T>& src, 
                              vector<T>& w, 
                              matrix<T>& v) const {
      return svd->decomposition(src,w,v);
    }

    /**
     * OnPlace version of Singular Value Decomposition.
     * @param src matrix<T> with the source matrix, will also contain
     *            the U matrix after the function has returned.
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    inline bool apply(matrix<T>& src, vector<T>& w, matrix<T>& v) const {
      return svd->apply(src,w,v);
    }

    /**
     * OnCopy version of Singular Value Decomposition.
     * @param src matrix<T> with the source matrix
     * @param u the U matrix
     * @param w vector<T> with the singular values, sorted descendingly if
     *                    parameters::sort is true.
     *                    The elements of this vector constitute the diagonal
     *                    of the W matrix.
     * @param v the V matrix
     * @return true is the decomposition was successfull, false if an
     *         error occured
     */
    inline bool apply(const matrix<T>& src, matrix<T>& u,
                      vector<T>& w, matrix<T>& v) const {
      return svd->apply(src,u,w,v);
    }

    
  protected:
    
    /**
     * The actual eigenSystem.
     */
    singularValueDecomp<T>* svd;
    
  };
}

#endif

