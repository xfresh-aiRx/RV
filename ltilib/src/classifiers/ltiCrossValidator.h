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
 * file .......: ltiCrossValidator.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 25.6.2002
 * revisions ..: $Id: ltiCrossValidator.h,v 1.7 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_CROSS_VALIDATOR_H_
#define _LTI_CROSS_VALIDATOR_H_

//TODO: include only those files which are needed in this header!! 

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiSupervisedInstanceClassifier.h"
#include "ltiUniformDist.h"

#include "ltiFunctor.h"

namespace lti {
  /**
   *  This class does a cross validation on the given dataset and returns
   *  the average recognition rates.
   *  It divides the data at random in nbOfSplits distinct segments. Then the 
   *  network is trained with nbOfSplits-1 of the segments. Then each data 
   *  point of the remaining segment is classified. 
   *  This process is repeated for every possible choice of the 
   *  the segment which is omited from the classification process.
   *  The classifier that is used for validation is expected to delete old
   *  results from previous trainings, because the classifier is trained with
   *  different training data set during validation. 
   */
  class crossValidator : public functor {
  public:
    /**
     * the parameters for the class crossValidator
     */
    class parameters : public functor::parameters {
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
       * the data set is divided in this number of splits
       */
      int nbOfSplits;

      /**
       * the classifier that is used for training and classifing
       */
      supervisedInstanceClassifier* classify;

    };

    /**
     * default constructor
     */
    crossValidator();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    crossValidator(const crossValidator& other);

    /**
     * destructor
     */
    virtual ~crossValidator();

    /**
     * returns the name of this type ("crossValidator")
     */
    virtual const char* getTypeName() const;
  
    /**
     * Computes the average the average recognition rate of the given data
     * using the specified classifier.
     * @param data dmatrix with the source data.
     * @param ids ivector with the ids of each data point in data.
     * @return the average recogntion rate
     */
    double apply(const dmatrix& data, const ivector& ids) const;

    /**
     * Computes the average the average recognition rate of the given data
     * using the specified classifier.
     * @param data dmatrix with the source data.
     * @param ids ivector with the ids of each data point in data.
     * @param avRecogRate the average recognition rate
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& data, const ivector& ids,
               double& avRecogRate) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    crossValidator& copy(const crossValidator& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    crossValidator& operator=(const crossValidator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected: 

    /**
     * Splits the given data into parts. The points are randomly drawn from 
     * the the src data.
     */
    bool splitData(const dmatrix& data,std::list<ivector>& splittedData) const;

    static uniformDistribution randomGenerator;

  };
}

#endif



