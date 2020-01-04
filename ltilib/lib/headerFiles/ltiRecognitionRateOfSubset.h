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

 
/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiRecognitionRateOfSubset.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 22.10.2002
 * revisions ..: $Id: ltiRecognitionRateOfSubset.h,v 1.7 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_RECOGNITION_RATE_OF_SUBSET_H_
#define _LTI_RECOGNITION_RATE_OF_SUBSET_H_

#include "ltiCostFunction.h"
#include "ltiCrossValidator.h"

namespace lti {
  /**
   *  Computes the average recognition rate of a given subset of features
   *  from a data set. A cross validation is performed on the given subset
   *  of features. The average recognition rate of the used classifier, 
   *  is returned as the value of this cost function.
   */
  class recognitionRateOfSubset : public costFunction {
  public:
    /**
     * the parameters for the class recognitionRateOfSubset
     */
    class parameters : public costFunction::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);
   
      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6     
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
      
      /**
       * parameters class used for the internal cross validator
       */
      crossValidator::parameters cvParameter;

    };

    /**
     * default constructor
     */
    recognitionRateOfSubset();

//     /**
//      * 
//      */
//     recognitionRateOfSubset(const dmatrix& src,const ivector& srcIds);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    recognitionRateOfSubset(const recognitionRateOfSubset& other);

    /**
     * destructor
     */
    virtual ~recognitionRateOfSubset();

    /**
     * returns the name of this type ("recognitionRateOfSubset")
     */
    virtual const char* getTypeName() const;
  
    /**
     * Set the src data, that is used in applies. This must be done before 
     * you call one of the apply methods. Later each feature can be adressed 
     * by its column number.
     */
    virtual void setSrc(const dmatrix& src, const ivector& srcIds);

    /**
     * computes the average recognition rate of the given feature subset
     * using cross validation
     * @param in std::list with the features that are included in this test.
     * @param value the resulting average recognition rate
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::list<int>& in,double& value);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    recognitionRateOfSubset& copy(const recognitionRateOfSubset& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    recognitionRateOfSubset& operator=(const recognitionRateOfSubset& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif



