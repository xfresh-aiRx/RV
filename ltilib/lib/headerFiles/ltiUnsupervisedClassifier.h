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
 * file .......: ltiUnsupervisedClassifier.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 29.08.2001
 * revisions ..: $Id: ltiUnsupervisedClassifier.h,v 1.10 2004/06/30 08:44:49 doerfler Exp $
 */


#ifndef _LTI_UNSUPERVISEDCLASSIFIER_H_
#define _LTI_UNSUPERVISEDCLASSIFIER_H_

#include "ltiClassifier.h"
#include "ltiVector.h"
#include "ltiMatrix.h"

namespace lti {

  /**
   * Abstract class, parent of all unsupervised classifiers.
   * This class defines the interface for all unsupervised train methods.
   */
  class unsupervisedClassifier : public classifier {

  public:

    // --------------------------------------------------
    // unsupervisedClassifier::parameters
    // --------------------------------------------------

    /**
     * the parameters for the class unsupervisedClassifier are the
     * same as for classifier, except that the default value of
     * mulipleMode is changed to outputTemplate::Ignore.
     */
    class parameters : public classifier::parameters {
    public:

      /**
       * default constructor.  Changes the defaults value of
       * parameters.multipleMode to outputTemplate::Ignore.
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters %object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters %object
       * @param other the parameters %object to be copied
       * @return a reference to this parameters %object
       */
      parameters& copy(const parameters& other);

      /**
       * Alias for copy
       */
      inline parameters& operator=(const parameters& other) {
        return copy(other);
      }

    };

    /**
     * default constructor
     */
    unsupervisedClassifier();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    unsupervisedClassifier(const unsupervisedClassifier& other);

    /**
     * returns the name of this type ("unsupervisedClassifier")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    unsupervisedClassifier& copy(const unsupervisedClassifier& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Unsupervised training.
     * The vectors in the <code>input</code> matrix
     * will be clustered using each specific method.
     * @param input the matrix with the input vectors (each row is a training
     *              vector)
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input) =0;

    /**
     * Unsupervised training.  The vectors in the <code>input</code>
     * matrix will be put into groups according to the training
     * algorithm.  Additionally, an integer indicating the class each
     * point belongs to is returned.
     *
     * By default this method uses the other train method
     * (see train(const dmatrix&)) and then calls
     * classifier::classify(const dvector&) to get the ids for each
     * train-vector. These ids are then returned.
     *
     * @param input the matrix with the input vectors
     *              (each row is a training vector)
     * @param ids vector of class ids for each input point
     * @return true if successful, false otherwise. (if false you can check
     *              the error message with getStatusString()) */
    virtual bool train(const dmatrix& input, ivector& ids);

    /**
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool
      classify(const dvector& feature, outputVector& result) const =0;

  protected:

    /**
     * Randomly selects numberOfPoints points (rows) from the data matrix.
     * If nubmerOfPoints is greater than the number of points in data,
     * points will be multiply selected.
     * @param data contains points to select from in rows
     * @param numberOfPoints number of points expected as result
     * @param randomPoints the points randomly selected from data
     * @return false if something went wrong, else true
     */
    bool selectRandomPoints(const dmatrix& data,
                            int numberOfPoints,
                            dmatrix& randomPoints);
  };
}

#endif

