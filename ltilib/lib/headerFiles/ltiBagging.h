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
 * file .......: ltiBagging.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 24.2.2003
 * revisions ..: $Id: ltiBagging.h,v 1.6 2005/06/16 09:32:39 doerfler Exp $
 */

#ifndef _LTI_BAGGING_H_
#define _LTI_BAGGING_H_

#include "ltiClassifier.h"
#include "ltiMatrix.h"
#include "ltiCombination.h"
#include "ltiSupervisedInstanceClassifier.h"

#include "ltiSupervisedInstanceClassifierFactory.h"

namespace lti {
  /**
   * Implementation of bagging classification.
   * For the bagging a set of classifiers, the ensemble, is trained with
   * randomly drawn subsets of the training data. For classication 
   * each classifier in the ensemble is classifies the data and the 
   * different results of each classifier are combined to a single result.
   */
  class bagging : public classifier {
  public:
    /**
     * the parameters for the class bagging
     */
    class parameters : public classifier::parameters {
    public:

//         enum eClassifiers {
//             ManualCrispDecisionTree=0,
//             Lvq,
//             MLP,
//             Rbf,
//             ShClassifier,
//             Svm
//         };

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
      virtual classifier::parameters* clone() const;

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
       * number of classifiers in the ensemble
       */
      int nbClassifiers;

      /**
       * number of elements in each ensemble
       */
      int nbTrainingVectors;

      /**
       * The classifier that is used for building the ensemble
       */
      std::string useClassifier;

      /**
       * The classifier that is used for building the ensemble
       */
      supervisedInstanceClassifier::parameters *classifierParam;

      /**
       * The parameter object for the combine object that is used for
       * aggregating.
       */
      combination::parameters combineParam;


    };

    /**
     * default constructor
     */
    bagging();

    /**
     * Construct a classifier using the given parameters
     */
    bagging(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    bagging(const bagging& other);

    /**
     * destructor
     */
    virtual ~bagging();

    /**
     * returns the name of this type ("bagging")
     */
    virtual const char* getTypeName() const;

    /**
     * training of the ensembles
     */ 
    bool train(const dmatrix& input, const ivector& ids);

    /**
     * classification and aggregating
     */ 
    bool classify(const dvector& feature,
                  classifier::outputVector& result) const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    bagging& copy(const bagging& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    bagging& operator=(const bagging& other);

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * read the bagging classifier from the given ioHandler
     */
    bool read(ioHandler& handler,const bool complete=true);

    /**
     * write to bagging classifier object to the given ioHandler
     */
    bool write(ioHandler& handler,const bool complete=true) const;

  protected: 

    /** 
     * list that collects all classifiers that are trained with different
     * training data sets.
     */
   std::list<supervisedInstanceClassifier*> *ensemble;

    /**
     * object factory for constructing the classifiers in ensemble
     */
   supervisedInstanceClassifierFactory factory;

  };
}

#endif

