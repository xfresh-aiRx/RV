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
 * file .......: ltiCombination.h
 * authors ....: Pablo Alvarado, Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 5.9.2001
 * revisions ..: $Id: ltiCombination.h,v 1.10 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_COMBINATION_H_
#define _LTI_COMBINATION_H_

#include "ltiClassifier.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This class is used to combine several results of different
   * classifier::output objects.
   *
   * There are many possibilities to do this.  All of them will use
   * internaly the classifier::sortedOutput type to compute the
   * probability for each class.  You can just combine linearly the
   * results or use the probabilistic Borda count to consider also how
   * sure is a classifier of its result.
   *
   * Probabilistic Borda Count is documented in S. Syberichs.
   * "Entscheidungs-Fusion zur 3D-Objekterkennung in einem
   * hierarchischen Klassifikationssystem", Diplomarbeit, Lehrstuhl
   * für Technische Informatik, RWTH-Aachen, Juni 2001
   */
  class combination : public functor {
  public:


    /**
     * the parameters for the class combination
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
       * Possible types of combination strategies.
       * The ProbabilisticBordaCount is described in the diploma thesis of
       * Stefan Syberichs at the LTI, RWTH Aachen: "Decision Fusion for 3D
       * Object Recognition in a Hierarchical Classification System".
       */
      enum eCombinationStrategy {
        Linear,
        ProbabilisticBordaCount
      };

      /**
       * Strategy used for combination. Default Linear.
       * @see eCombinationStrategy
       */
      eCombinationStrategy combinationStrategy;

      /**
       * Normalize the result of the combination
       * Default: true
       */
      bool normalize;

      /**
       * Normalize the outputVectors to be combined before combination.
       * Default false. Usually, the inputs are normalized anyway.
       */
      bool normalizeInputs;

      /**
       * The inputs can be weighted. If this value is true, the corresponding
       * %vector weights must be set as well. Default false.
       */
      bool useWeights;

      /**
       * The weights in case of weighted combination. Default empty.
       */
      dvector weights;
    };

    /**
     * default constructor
     */
    combination();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    combination(const combination& other);

    /**
     * destructor
     */
    virtual ~combination();

    /**
     * returns the name of this type ("combination")
     */
    virtual const char* getTypeName() const;

    /**
     * Combine the given list of classifier::outputVector objects into one
     * outputVector object according to the parameter settings.
     *
     * @param src list of classifier::outputvector objects with the partial
     *            classification results.
     *
     * @param dest classifier::outputvector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const std::list<classifier::outputVector>& src,
               classifier::outputVector& dest) const;

    /**
     * Combine the given list of classifier::outputVector objects into one
     * outputVector object according to the parameter settings. No matter
     * what the parameters are, the given weights will be used for
     * combination.
     *
     * @param src     list of classifier::outputvector objects with the partial
     *                classification results.
     * @param weights used for weighting the inputs in same order.
     * @param dest    classifier::outputvector where the result will be left.
     * @return        true if apply successful or false otherwise.
     */
    bool apply(const std::list<classifier::outputVector>& src,
               const dvector& weights,
               classifier::outputVector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    combination& copy(const combination& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    combination& operator=(const combination& other);

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
     * Calculates the sum of the outputVectors contained in src and
     * leaves the result in dest. Returns false if the %vectors in src
     * are not compatible, ie contain the same labels.
     * @param src list of outputVectors to be summed up
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool sumOfVectors(const std::list<classifier::outputVector> src,
                      classifier::outputVector& dest) const;

    /**
     * Calculates the weighted sum of the outputVectors contained in src and
     * leaves the result in dest. Returns false if the %vectors in src
     * are not compatible, ie contain the same labels.
     * @param src list of outputVectors to be summed up
     * @param weights used for weighting the outputVectors in src
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool sumOfVectors(const std::list<classifier::outputVector> src,
                      const dvector& weights,
                      classifier::outputVector& dest) const;

    /**
     * First calculates the ProbablilisticBordaCount for each input
     * %vector then sums these measures and returns the sum in
     * dest. Returns false if the %vectors in src are not compatible,
     * ie contain the same labels.
     * @param src list of outputVectors to be summed up
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool probBordaCount(const std::list<classifier::outputVector> src,
                        classifier::outputVector& dest) const;

    /**
     * First calculates the ProbablilisticBordaCount for each input
     * %vector then sums these measures weighted by weights and returns the
     * sum in dest. Returns false if the %vectors in src are not compatible,
     * ie contain the same labels.
     * @param src list of outputVectors to be summed up
     * @param weights used for weighting the outputVectors in src
     * @param dest result of the sum
     * @return false if values in list are incompatible
     */
    bool probBordaCount(const std::list<classifier::outputVector> src,
                        const dvector& weights,
                        classifier::outputVector& dest) const;


    /**
     * Checks whether all outputVectors contained in src are
     * compatible to each other and that none of them contains the
     * same label more than once. Returns true if everything is ok.
     */
    bool
    noMultAndCompatible(const std::list<classifier::outputVector> src) const;

  };

}

#endif

