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
 * file .......: ltiManualCrispDecisionTree.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 25.2.2002
 * revisions ..: $Id: ltiManualCrispDecisionTree.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_MANUAL_CRISP_DECISION_TREE_H_
#define _LTI_MANUAL_CRISP_DECISION_TREE_H_


#include "ltiMatrix.h"
#include "ltiVector.h"

#include "ltiCrispDecisionTree.h"

namespace lti {
  /**
   * This class serves the manual construction of a crisp decision
   * tree with crispNodes that have univariate decision
   * functions. Instead of training data, the train method is given
   * the nodes' conditions and ids in pre-order. Only one element of
   * each row vector can be unequal to zero. This dimension and the
   * value are taken for the dimension and threshold of the
   * condition. Thus, only univariate decisions can be used. The
   * following example shows the data and id values of the train
   * method and the corresponding tree:
   *
   * \verbatim
   *        +-     -+         +-  -+
   *        | 0 0 2 |         | -1 |
   *        | 1 0 0 |         | -1 |
   *        | 0 2 0 |         | -1 |
   *        | 0 1 0 |         | -1 |
   *        | 0 0 0 |         |  2 |
   * data = | 0 0 1 |   ids = | -1 |
   *        | 0 0 0 |         |  5 |
   *        | 0 0 0 |         |  0 |
   *        | 0 0 0 |         |  1 |
   *        | 0 0 0 |         |  3 |
   *        | 0 0 0 |         |  4 |
   *        +-     -+         +-  -+
   * \endverbatim
   *
   * \image html manualCrispDecisionTree.png
   *
   * The outputTemplate %object of the tree will be initialized with the
   * ids given in the ids vector. If other or multiple ids are desired, the
   * outputTemplate %object must be set manually.
   */
  class manualCrispDecisionTree : public crispDecisionTree {
  public:
    /**
     * the parameters for the class manualCrispDecisionTree
     */
    class parameters : public crispDecisionTree::parameters {
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
      virtual ~parameters();

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

      //TODO: comment the parameters of your classifier
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


    };

    /**
     * default constructor
     */
    manualCrispDecisionTree();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    manualCrispDecisionTree(const manualCrispDecisionTree& other);

    /**
     * destructor
     */
    virtual ~manualCrispDecisionTree();

    /**
     * returns the name of this type ("manualCrispDecisionTree")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    manualCrispDecisionTree& copy(const manualCrispDecisionTree& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    manualCrispDecisionTree& operator=(const manualCrispDecisionTree& other);

    /**
     * returns a pointer to a clone of this classifier.
     */
    virtual classifier* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * See description of this class
     * @param input the conditions of the nodes
     * @param ids the ids of the nodes.
     * @return true if successful, false otherwise.  (if false you can check
     *              the error message with getStatusString())
     */
    virtual bool train(const dmatrix& input,
                       const ivector& ids);


    //TODO: comment the attributes of your classifier
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  };
}

#endif

