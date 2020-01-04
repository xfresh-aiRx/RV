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
 * file .......: ltiSupervisedSequenceClassifier.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 29.08.2001
 * revisions ..: $Id: ltiSupervisedSequenceClassifier.h,v 1.8 2003/05/16 07:25:24 alvarado Exp $
 */


#ifndef _LTI_SUPERVISEDSEQUENCECLASSIFIER_H_
#define _LTI_SUPERVISEDSEQUENCECLASSIFIER_H_

#include "ltiClassifier.h"
#include "ltiVector.h"
#include "ltiSequence.h"

#include <vector>

namespace lti {

  /**
   * Abstract class, parent of all supervised classifiers for sequences.
   * This class defines the interface for training methods of classifiers
   * that work on time series.
   */
  class supervisedSequenceClassifier : public classifier {

  public:

    /**
     * default constructor
     */
    supervisedSequenceClassifier();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    supervisedSequenceClassifier(const supervisedSequenceClassifier& other);

    /**
     * returns the name of this type ("supervisedSequenceClassifier")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    supervisedSequenceClassifier& copy(const supervisedSequenceClassifier& other);
    /**
     * Alias for "copy".
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    inline supervisedSequenceClassifier& operator=(const supervisedSequenceClassifier& other) {
      return copy(other);
    }

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Supervised sequence training.
     * The sequences in the <code>input</code> %vector are used for training
     * the %sequence %classifier. Each %sequence is associated with an id
     * given in the %vector <code>ids</code>.
     * @param input %vector of training %sequences
     * @param ids the classes of the training sequences
     * @return true if successful, false otherwise.  (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const std::vector< sequence<dvector> > &input,
                       const ivector& ids) = 0;

    /**
     * Classification.
     * Classifies the %sequence of features and returns the outputVector with
     * the classification result. Since %sequence classifiers usually keep
     * an inner state, this method is not const.
     * @param observations the %sequence to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool classify(const sequence<dvector>& observations,
                          outputVector& result) = 0;


  protected:

    /**
     * Sets the outputTemplate probability distributions according to
     * the classification of the given data. The distributions are
     * built by the follwing rule: <p>
     * <ol>
     * <li>Classify next data sequence</li>
     * <li>For the position in the output with the highest probability
     *     increase the count for the actual id by one.</li>
     * <li>While there is more data go back to 1</li>
     * <li>For each position: divide each count by total number of counts</li>
     * </ol>
     * This results in a distribution over the ids that caused highest
     * probability for each position of the output.
     * @param outSize size of the outputTemplate
     * @param data train of validation data %sequences
     * @param ids ids of the data-vectors
     * @returns false upon error
     */
    bool makeOutputTemplate(const int& outSize,
                            const std::vector< sequence<dvector> >& data,
                            const ivector& ids);
  };
}

#endif

