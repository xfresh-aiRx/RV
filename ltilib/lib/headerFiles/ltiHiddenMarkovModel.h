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
 * file .......: ltiHiddenMarkovModel.h
 * authors ....: Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 14.12.01
 * revisions ..: $Id: ltiHiddenMarkovModel.h,v 1.18 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_HIDDEN_MARKOV_MODEL_H_
#define _LTI_HIDDEN_MARKOV_MODEL_H_

#include <string>
#include <vector>
#include "ltiIoObject.h"
#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiSequence.h"
#include "ltiSTLIoInterface.h"

namespace lti {

  /**
   * Hidden Markov Model (HMM) class. A HMM can be created using the class
   * hmmTrainer, which estimates the HMM %parameters <code>A, B, Pi</code>.
   * These stand for the transitions the emissions in the model states, and
   * the initial probabilities.
   *
   * For an introduction see: L. Rabiner and B.-H. Juang, "An introduction
   * to hidden markov models", IEEE ASSP Magazine no.1, vol.3, pp4-16, 1986.
   *
   * For computational efficiency probabilities p are not explicitly computed
   * but represented by scores (~ -ln(p)). Note that a high score corresponds
   * to a low probability, i.e. it rather resembles a cost.
   */
  class hiddenMarkovModel : public ioObject {
  public:

    /**
     * This class represents a single multivariate (ie multidimensional ) score
     * density function within a state of a hidden markov model. The dimensions
     * of the density function are assumed to be independent of each other.
     */
    class singleDensity : public ioObject {
    public:

      /**
       * default constructor
       */
      singleDensity();

      /**
       * copy constructor
       * @param other the object to be copied
       */
      singleDensity(const singleDensity& other);

      /**
       * destructor
       */
      virtual ~singleDensity();

      /**
       * copy data of "other" singleDensity.
       * Please note that the status string will _NOT_ be copied!
       */
      singleDensity& copy(const singleDensity& other);

      /**
       * assigment operator (alias for copy(other)).
       * @param other the singleDensity to be copied
       * @return a reference to the actual ioObject
       */
      singleDensity& operator=(const singleDensity& other);

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler, const bool complete=true) const;

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
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif


      /**
       * The densities weight expressed as score. The weight score is -ln(w),
       * where w would be the corresponding probability associated with this
       * single density.
       *
       * The weights are required in mixture densities, which consist of
       * several single densities, to rate the contribution of each single
       * density. The sum over all weights w in a mixture density must be one.
       *
       */
      double weightScore;

      /**
       * The mean vector. In other words the center of this density function.
       */
      dvector mean;

      /**
       * Scaling factor vector. The scaling factor is a generic term for the
       * spread of a density function. For example in gaussian functions it
       * is known as the standard deviation.
       */
      dvector scalingFactor;

    };

    /**
     * A mixture density. Composed of single densities to represent the
     * distribution within one state
     */
    typedef std::vector<singleDensity> mixtureDensity;

    /**
     * States of the model, where each state contains a mixture density
     */
    typedef std::vector<mixtureDensity> stateList;


    /**
     * default constructor
     */
    hiddenMarkovModel();

    /**
     * copy constructor
     * @param other the hiddenMarkovModel to be copied
     */
    hiddenMarkovModel(const hiddenMarkovModel& other);

    /**
     * destructor
     */
    virtual ~hiddenMarkovModel();

    /**
     * copy data of "other" hiddenMarkovModel.
     * Please note that the status string will _NOT_ be copied!
     */
    hiddenMarkovModel& copy(const hiddenMarkovModel& other);

    /**
     * assigment operator (alias for copy(other)).
     * @param other the hiddenMarkovModel to be copied
     * @return a reference to the actual hiddenMarkovModel
     */
    hiddenMarkovModel& operator=(const hiddenMarkovModel& other);

    /**
     * returns name of this type
     */
    const char* getTypeName() const;

    /** 
     * Returns the number of states of this model.
     */
    int getNumberOfStates() const;

    /**
     * write the parameters in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * read the parameters from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);

#   ifdef _LTI_MSC_6
    /**
     * this function is required by MSVC only, as a workaround for a
     * very awful bug, which exists since MSVC V.4.0, and still by
     * V.6.0 with all bugfixes (so called "service packs") remains
     * there...  This method is public due to another bug, so please
     * NEVER EVER call this method directly: use read() instead!
     */
    bool readMS(ioHandler& handler,const bool complete=true);

    /**
     * this function is required by MSVC only, as a workaround for a
     * very awful bug, which exists since MSVC V.4.0, and still by
     * V.6.0 with all bugfixes (so called "service packs") remains
     * there...  This method is public due to another bug, so please
     * NEVER EVER call this method directly: use write() instead!
     */
    bool writeMS(ioHandler& handler,const bool complete=true) const;
#   endif



    /**
     * Density type used. Currently gauss or laplace are available.
     */
    enum scoreType {
      gaussScore,
      laplaceScore
    };


    /**
     * This number is considered to be the "impossible" score
     * (score equivalent to a probability of 0.0)
     */
    static const double INFINITE_NUMBER;


    // ------------------------------------------------
    // the data
    // ------------------------------------------------

    /** @name User defined
     *  These members may (but don't have to) be specified by the user
     */
    //@{

      /**
       * Name of the model. Can be used to identify a model by a string.
       */
      std::string name;

      /**
       * Absolute weight of each feature (dimension).
       *
       * Default is a %vector of 1.0 for all features to indicate
       * equal importance (weights are ignored if number of dimensions
       * differs from featureDimension).
       */
      dvector featureWeights;

      /**
       * Relative weight of the emission scores compared to the
       * initial and transition score.  Can be used to compensate the
       * magnitude difference between those.
       *
       * Use a value >1.0, to emphasize the emission or a value <1.0
       * to put more weight into the transitions. Default value is 1.0
       *
       */
      double emissionScoreWeight;

      //@}

    /** @name Generated by hmmTrainer
     *  These members are usually estimated over a given data set
     *  by an automatic training algorithm (see hmmTrainer)
     */
      //@{

      /**
       * Specifies the score function to be used.
       * default: laplaceScore
       */
      scoreType scoreFunction;

      /**
       * List of states of the model <code>s_i</code>.
       * Generally the number of states is denoted <code>N</code>.
       * The states contain the emission functions <code>b_i</code>,
       * known as the HMM %parameter called <code>B</code>
       */
      stateList states;

      /**
       * Number of dimensions for an observation
       */
      int featureDimension;

      /**
       * Score for each state being the starting state.
       * This is the HMM %parameter called <code>Pi</code>
       */
      dvector initialScore;

      /**
       * Score for each transition.
       * TransitionScore[a][b] is the score for a transition from state a to b.
       * This is the HMM %parameter called <code>A</code>
       */
      dmatrix transitionScore;

      //@}

  };


  /**
   * write single density
   */
  bool write(ioHandler& handler,
	     const hiddenMarkovModel::singleDensity& p,
	     const bool complete=true);

  /**
   * read single density
   */
  bool read(ioHandler& handler, hiddenMarkovModel::singleDensity& p,
	    const bool complete=true);


  /**
   * write hidden markov model
   */
  bool write(ioHandler& handler, const hiddenMarkovModel& p,
	     const bool complete=true);

  /**
   * read hidden markov model
   */
  bool read(ioHandler& handler, hiddenMarkovModel& p,
	    const bool complete=true);

}

#endif

