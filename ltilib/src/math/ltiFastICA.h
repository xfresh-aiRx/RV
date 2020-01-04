/*
 * Copyright (C) 1998 - 2005
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
 * file .......: ltiFastICA.h
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 3.3.2004
 * revisions ..: $Id: ltiFastICA.h,v 1.8 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_FAST_I_C_A_H_
#define _LTI_FAST_I_C_A_H_

#include "ltiLinearAlgebraFunctor.h"
#include "ltiUniformDist.h"
#include "ltiPCA.h"

namespace lti {
  /**
   * This class implements a fast fixed point algorithm for the Independent
   * Component Analysis (ICA).
   *
   * It receives a set of input vectors in form of a matrix (each row of
   * the matrix corresponds to an input vector), which will be transformed
   * with ICA. (apply() methods)
   *
   * If you only want to compute the transformation matrix you can use
   * the computeTransformMatrix() methods. 
   *
   * Once the transformationMatrix is computed (by an apply,
   * or computeTransformMatrix method) you can use the transform() methods to
   * %transform other datasets with the same transformation matrix.
   *
   * Note that you have to subtract the offset vector from data you want to 
   * %transform with the transformation matrix if \b not 
   * using the transform() methods.
   */
  template<class T>
  class fastICA : public linearAlgebraFunctor {
  public:
    /**
     * The parameters for the class fastICA
     */
    class parameters : public linearAlgebraFunctor::parameters {
    public:

      /**
       * Enumeration of orthogonalization methods.
       */
      enum eOrthogonalizationMethod {
        Deflationary, /**< The ICs are computed one after another and 
                       *   orthogonalization is done dependent on the
                       *   already computed ICs (first component has
                       *   a free choice of direction)
                       */
        Symmetric     /**< The ICs are computed in parallel and 
                       *   orthogonalization is done after each 
                       *   iteration step considering all ICs.
                       */
      };

      /**
       * Enumeration of negentropyApproximation methods.
       *
       * In fact the first and second derivation of the stated functions are
       * used in the algorithm.
       */
      enum eNegentropyApproximation {
        Exp, /**< using 
              * \f$G(x)=-\frac{1}{\alpha_{exp}}*exp(-\frac{\alpha_{exp}}{2}*u^2)
              * \f$ as negentropy approximation function
              */
        Tanh, /**< using
               * \f$G(u)=\frac{1}{\alpha_{tanh}}*log (cosh(\alpha_{tanh}*u))\f$
               * as negentropy approximation function
               */
        Kurt /**< using
              * \f$G(u)=u^4 \f$ as negentropy Approximation function
              */
      };

      /**
       * Default constructor
       */
      parameters() : linearAlgebraFunctor::parameters() {
        //TODO: Initialize your parameter values!
        // If you add more parameters manually, do not forget to do following:
        // 1. indicate in the default constructor the default values
        // 2. make sure that the copy member also copy your new parameters
        // 3. make sure that the read and write members also read and
        //    write your parameters

        // maxSteps
        // delta
        maxSteps = 1000;
        orthogonalizationMethod = Deflationary;
        negentropyApproximation = Exp;
        epsilon = static_cast<T>(0.0001f);
        expAlpha = 1;
        tanhAlpha = 1;
      };

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : linearAlgebraFunctor::parameters() {
        copy(other);
      }
      
      /**
       * Destructor
       */
      ~parameters() {
      };

      /**
       * Returns name of this type
       */
      const char* getTypeName() const {
        return "fastICA::parameters";
      };

      /**
       * Copy the contents of a parameters object
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

        maxSteps = other.maxSteps;
        orthogonalizationMethod = other.orthogonalizationMethod;
        negentropyApproximation = other.negentropyApproximation;  
        expAlpha = other.expAlpha;
        tanhAlpha = other.tanhAlpha;      
        epsilon = other.epsilon;
        return *this;
      };

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

#     ifndef _LTI_MSC_6
      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      /**
       * This function is required by MSVC only, as a workaround for a
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

          std::string str1,str2;
          switch (orthogonalizationMethod) {
              case Deflationary: {
                str1 = "Deflationary";
                break;
              }
              case Symmetric: {
                str1 = "Symmetric";
                break;
              }
              default: {
                str1 = "Deflationary";
                break;
              }
          }
          switch (negentropyApproximation) {
              case Exp: {
                str2 = "Exp";
                break;
              }
              case Tanh: {
                str2 = "Tanh";
                break;
              }
              case Kurt: {
                str2 = "Kurt";
                break;
              }
          }
          
          lti::write(handler,"maxSteps",maxSteps);
          lti::write(handler,"epsilon",epsilon);
          lti::write(handler,"expAlpha",expAlpha);
          lti::write(handler,"tanhAlpha",tanhAlpha);
          lti::write(handler,"orthogonalizationMethod",str1);
          lti::write(handler,"negentropyApproximation",str2);
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
       * Write the parameters in the given ioHandler
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
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if read was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
      /**
       * This function is required by MSVC only, as a workaround for a
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
          std::string str1,str2;
          lti::write(handler,"maxSteps",maxSteps);
          lti::write(handler,"epsilon",epsilon);
          lti::write(handler,"expAlpha",expAlpha);
          lti::write(handler,"tanhAlpha",tanhAlpha);
          lti::write(handler,"orthogonalizationMethod",str1);
          lti::write(handler,"negentropyApproximation",str2);
          if (str1=="Symmetric") {
            orthogonalizationMethod = Symmetric;
          } // default
          else {
            orthogonalizationMethod = Deflationary;
          }

          if (str2=="Tanh") {
            negentropyApproximation = Tanh;
          } else if (str2=="Kurt") {
            negentropyApproximation = Kurt;
          } //default
          else {
            negentropyApproximation = Exp;
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
       * Read the parameters from the given ioHandler
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
       * maximum number of iterations to spend on one component
       *
       * Default: 1000
       */
      int maxSteps;

      /**
       * at which epsilon the vector is assumed to be converged
       *
       * Default: 0.0001
       */
      T epsilon;

      /**
       * orthogonalization method used
       *
       * Default: Deflationary
       */
      eOrthogonalizationMethod orthogonalizationMethod;

      /**
       * negentropy approximation
       *
       * Default: Exp
       */
      eNegentropyApproximation negentropyApproximation;

      /**
       * parameter for tuning the exponential negentropy approximation
       *
       * Default: 1
       */
      T expAlpha;
 
      /**  
       * parameter for tuning the tanh negentropy approximation
       *
       * Default: 1
       */
      T tanhAlpha;
             
    };

    /**
     * Default constructor
     */
    fastICA();

    /**
     * Construct a functor using the given parameters
     */
    fastICA(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    fastICA(const fastICA& other);

    /**
     * Destructor
     */
    virtual ~fastICA();

    /**
     * Returns the name of this type ("fastICA")
     */
    virtual const char* getTypeName() const;

    
    /**
     * operates on the given %parameter.
     * @param srcdest matrix with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(matrix<T>& srcdest);

    /**
     * This computes a transformation matrix and transforms the given data.
     * @param src matrix with the source data. Each row represents a dataset.
     * @param dest matrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const matrix<T>& src,matrix<T>& dest);


    /** 
     * only computes the transformation matrix.
     * @param src matrix with the sourc data. Each row represents a dataset
     * @return true if apply successful or false otherwise.
     */
    bool computeTransformMatrix(const matrix<T>& src);

    /**
     * Returns the saved rotation matrix, which was computed with ICA.
     * @return a const reference to the last computed or used 
     * rotation matrix.
     */
    const matrix<T>& getRotationMatrix() const;

    /**
     * Returns the saved rotation matrix, which was computed with ICA.
     * @param dest last computed or used  rotation matrix will be stored here.
     */
    bool getRotationMatrix(matrix<T>& dest) const;

    /**
     * Returns the saved whitening matrix, which was computed with PCA
     * @return a const reference to the last computed or used 
     * whitening matrix.
     */
    const matrix<T>& getWhiteningMatrix() const;

    /**
     * Returns the saved whitening matrix, which was computed with ICA.
     * @param dest last computed or used  whitening matrix will be stored here.
     */
    bool getWhiteningMatrix(matrix<T>& dest) const;

    /**
     * Returns the saved transformation matrix.
     * This is only the product of (whitening matrix) * (rotation matrix).
     *
     * \b Note: If you want to use the transformation matrix to transform
     * datasets, you should subtract the offset vector from your data first.
     *
     * @return a const reference to the last computed or used 
     * transform matrix.
     */
    const matrix<T>& getTransformMatrix() const;


    /**
     * Returns the saved transformation matrix.
     * @param dest last computed or used transformation matrix will be stored
     *  here.
     */
    bool getTransformMatrix(matrix<T>& dest) const;

    /**
     * Returns the previously computed offset vector, which corresponds to the
     * mean of the data.
     *
     * @param result the offset vector will be written here.
     * @return true if the matrix could be computed, false otherwise.
     */
    bool getOffsetVector(vector<T>& result) const;

    /**
     * Returns the previously computed offset vector, which corresponds to the
     * mean of the data.
     *
     * @return a const reference to the last computed or used offset vector.
     */
    const vector<T>& getOffsetVector() const;

    /**
     * Transform an entire matrix according to a previosly computed transformation
     * matrix.
     * @param src the input data
     * @param dest here the transformed data will be left
     *
     * @return true if operation was succesfull and false otherwise
     */
    bool transform(const matrix<T>& src, matrix<T>& dest) const;  

    /**
     * Transform an entire matrix according to a previosly computed transformation
     * matrix.
     * @param srcdest the input data, the result will be left here, too
     *
     * @return true if operation was succesfull and false otherwise
     */
    bool transform(matrix<T>& srcdest) const;


    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     * @param src the data vector
     * @param dest the transformed vector will be left here
     */
    bool transform(const vector<T>& src, vector<T>& dest) const;

    /**
     * Transforms a single vector according to a previously computed
     * transformation matrix.
     * @param srcdest the input data, the result will be left here too
     * @return true if operation was succesfull and false otherwise
     */
    bool transform(vector<T>& srcdest) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastICA& copy(const fastICA& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    fastICA& operator=(const fastICA& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     * @return const reference to the parameter object of this functor.
     */
    const parameters& getParameters() const;


    /**
     * Update functor's parameters.
     * This member updates the internal state of the functor according to the parameter set.
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

 
  protected:
    /**
     * PCA functor which is applied before ICA with Whitening
     */
    principalComponents<T> pca;  

    /**
    * method to center the data
    */
    bool centerData(matrix<T>& srcdest) const;

    /**
     * ICA with deflationary approach
     */
    bool icaDeflationary(const matrix<T>& src, matrix<T>& dest, const parameters& par) const;

    /**
     * ICA with symmetric approach
     */
    bool icaSymmetric(const matrix<T>& src, matrix<T>& dest, const parameters& par) const;


    /**
     * method to determine if a vector is close enough to another
     */
    bool closeEnough(const vector<T>& a,
                     const vector<T>& b, 
                     const T epsilon) const;

    /**
     * method to determine if a vector is close enough to another
     */
    bool closeEnough(const matrix<T>& a,
                     const matrix<T>& b, 
                     const T epsilon) const;

    /**
     * Function pointer to the non-linearity function which should be used.
     * This pointer is set in updateParameters dependent on the
     * negentropyAproximation setting.
     *
     * Default: negentropyApproximation
     */
    T (fastICA<T>::*g) (const T x) const;

    /**
     * Function pointer to derivation of above g.
     */
    T (fastICA<T>::*gd) (const T x) const;

    /**
     * Constants for tuning negentropyApproximation functions are copied
     * from parameters in setParameters to this variable.
     */
    T a1;

    /**
     * Used to choose initial random vectors.
     */
    uniformDistribution uniDist;

    /**
     * First derivation of Exp negentropyApproximation.
     */
    T expo1(const T x) const;

    /**
     * Second derivation of Exp negentropyApproximation.
     */
    T expo2(const T x) const;

    /**
     * First derivation of Tanh negentropyApproximation.
     */
    T tanh1(const T x) const;

    /**
     * Second derivation of Tanh negentropyApproximation.
     */
    T tanh2(const T x) const;

    /**
     * First derivation of Pow4 negentropyApproximation (Kurtosis).
     */
    T kurt1(const T x) const;

    /**
     * Second derivation of Pow4 negentropyApproximation (Kurtosis).
     */
    T kurt2(const T x) const;

    /**
     * Matrix which is used to save the transform matrix.
     */
    matrix<T> transformMatrix;

    /**
     * Matrix which is used to save the whitening matrix.
     */
    matrix<T> whiteningMatrix;

    /**
     * Matrix which is used to save the whitening matrix.
     */
    matrix<T> rotationMatrix;

 };
}

#endif

