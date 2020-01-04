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
 * file .......: ltiCrispDecisionTree.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 20.2.2002
 * revisions ..: $Id: ltiCrispDecisionTree.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_CRISP_DECISION_TREE_H_
#define _LTI_CRISP_DECISION_TREE_H_


#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiObjectFactory.h"

#include "ltiDecisionTree.h"

namespace lti {
  /**
   * Base class for all crisp decision trees. A crisp decision tree
   * has the following properties:
   * <ul>
   * <li> Each node has zero, one or two children.</li>
   * <li> Propagation of a %vector results in one (or zero) leaf nodes
   *      being activated</li>
   * <li> The exact type of the node as well as the condition in the
   *      decision function can be chosen for each node individually.</li>
   * </ul>
   */
  class crispDecisionTree : public decisionTree {
  public:
    /**
     * the parameters for the class crispDecisionTree
     */
    class parameters : public decisionTree::parameters {
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
     * Base class for decision functions of crispNode. The decision
     * function implements a condition which can either be true or
     * false. Usually in the tree, the left node will represent true
     * and the right node false.
     */
    class crispDecisionFunction : public ioObject {
    public:
      /**
       * default constructor
       */
      crispDecisionFunction();

      /**
       * copy constructor
       * @param other the crispDecisionFunction object to be copied
       */
      crispDecisionFunction(const crispDecisionFunction& other);

      /**
       * destructor
       */
      ~crispDecisionFunction();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a crispDecisionFunction object
       * @param other the crispDecisionFunction object to be copied
       * @return a reference to this crispDecisionFunction object
       */
      crispDecisionFunction& copy(const crispDecisionFunction& other);

      /**
       * copy the contents of a crispDecisionFunction object
       * @param other the crispDecisionFunction object to be copied
       * @return a reference to this crispDecisionFunction object
       */
      crispDecisionFunction& operator=(const crispDecisionFunction& other);

      /**
       * returns a pointer to a clone of the crispDecisionFunction
       */
      virtual crispDecisionFunction* clone() const=0;

      /**
       * Evaluate the condition implemented in the cdf.
       * @param data value to be evaluated
       * @return true if it meets the condition (left), false if not
       */
      virtual bool apply(const dvector& data) const=0;

      /**
       * write the crispDecisionFunction in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the crispDecisionFunction from the given ioHandler
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
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug!, so please
       * NEVER EVER call this method directly
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the crispDecisionFunction
      // ------------------------------------------------

      //TODO: comment the crispDecisionFunction of your DT
      // If you add more crispDecisionFunction manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new crispDecisionFunction
      // 3. make sure that the read and write members also read and
      //    write your crispDecisionFunction

    };

    /**
     * This class implements the most common decision function: Each
     * node has a real-valued threshold in one dimension only.
     */
    class univariateCrispDecisionFunction : public crispDecisionFunction {
    public:
      /**
       * default constructor
       */
      univariateCrispDecisionFunction();

      /**
       * Sets the condition of the decision function.
       * @param dim dimension for the decision
       * @param thresh the threshold
       */
      univariateCrispDecisionFunction(const int& dim,
                                      const double& thresh);

      /**
       * copy constructor
       * @param other the univariateCrispDecisionFunction object to be copied
       */
      univariateCrispDecisionFunction
        (const univariateCrispDecisionFunction& other);

      /**
       * destructor
       */
      ~univariateCrispDecisionFunction();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a univariateCrispDecisionFunction object
       * @param other the univariateCrispDecisionFunction object to be copied
       * @return a reference to this univariateCrispDecisionFunction object
       */
      univariateCrispDecisionFunction& copy
        (const univariateCrispDecisionFunction& other);

      /**
       * copy the contents of a univariateCrispDecisionFunction object
       * @param other the univariateCrispDecisionFunction object to be copied
       * @return a reference to this univariateCrispDecisionFunction object
       */
      univariateCrispDecisionFunction& operator=
        (const univariateCrispDecisionFunction& other);

      /**
       * returns a pointer to a clone of the univariateCrispDecisionFunction
       */
      virtual crispDecisionFunction* clone() const;

      /**
       * Evaluate the condition implemented in the cdf.
       * @param data value to be evaluated
       * @return true if it meets the condition (left), false if not
       */
      virtual bool apply(const dvector& data) const;

      /**
       * Sets the condition to be tested.
       * @param dim the dimension to be tested.
       * @param thresh the threshold in that dimension
       */
      void setCondition(const int& dim, const double& thresh);

      /**
       * write the univariateCrispDecisionFunction in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the univariateCrispDecisionFunction from the given ioHandler
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
      // the univariateCrispDecisionFunction
      // ------------------------------------------------

      //TODO: comment the univariateCrispDecisionFunction of your DT
      // If you add more univariateCrispDecisionFunction manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new univariateCrispDecisionFunction
      // 3. make sure that the read and write members also read and
      //    write your univariateCrispDecisionFunction

    protected:

      /**
       * The dimension to be tested
       */
      int dimension;

      /**
       * Threshold against which the data is tested in the given dimension.
       */
      double threshold;

    };

    /**
     * This class implements a multivariate decision. The apply method is
     * true, if the following condition is met.<p>
     * \f$\mathbf{x}\cdot\mathbf{a} < c \f$ with \f$\mathbf{x}\f$ the data,
     * \f$\mathbf{a}\f$ a weighting vector and \f$c\f$ the threshold.<p>
     * Thus, each node performs an arbitrary linear seperation of the
     * remaining feature space.
     */
    class multivariateCrispDecisionFunction : public crispDecisionFunction {
    public:
      /**
       * default constructor
       */
      multivariateCrispDecisionFunction();

      /**
       * New decision function with the given parameters.
       * @param w the weighting vector for the data.
       * @param thresh the threshold for the weighted sum
       */
      multivariateCrispDecisionFunction(const dvector& w,
                                        const double& thresh);

      /**
       * copy constructor
       * @param other the multivariateCrispDecisionFunction object to be copied
       */
      multivariateCrispDecisionFunction
        (const multivariateCrispDecisionFunction& other);

      /**
       * destructor
       */
      ~multivariateCrispDecisionFunction();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a multivariateCrispDecisionFunction object
       * @param other the multivariateCrispDecisionFunction object to be copied
       * @return a reference to this multivariateCrispDecisionFunction object
       */
      multivariateCrispDecisionFunction& copy
        (const multivariateCrispDecisionFunction& other);

      /**
       * copy the contents of a multivariateCrispDecisionFunction object
       * @param other the multivariateCrispDecisionFunction object to be copied
       * @return a reference to this multivariateCrispDecisionFunction object
       */
      multivariateCrispDecisionFunction& operator=
        (const multivariateCrispDecisionFunction& other);

      /**
       * returns a pointer to a clone of the multivariateCrispDecisionFunction
       */
      virtual crispDecisionFunction* clone() const;

      /**
       * Evaluate the condition implemented in the cdf.
       * @param data value to be evaluated
       * @return true if it meets the condition (left), false if not
       */
      virtual bool apply(const dvector& data) const;

      /**
       * Sets the condition to be tested.
       * @param w the weighting vector for the data.
       * @param thresh the threshold for the weightes sum
       */
      void setCondition(const dvector& w, const double& thresh);

      /**
       * write the multivariateCrispDecisionFunction in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the multivariateCrispDecisionFunction from the given ioHandler
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
      // the multivariateCrispDecisionFunction
      // ------------------------------------------------

      //TODO: comment the multivariateCrispDecisionFunction of your DT
      // If you add more multivariateCrispDecisionFunction manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new multivariateCrispDecisionFunction
      // 3. make sure that the read and write members also read and
      //    write your multivariateCrispDecisionFunction

    protected:

      /**
       * the weighting vector
       */
      dvector weights;

      /**
       * Threshold against which the data is tested in the given dimension.
       */
      double threshold;

    };


    /**
     * A crisp decision tree node. Its main difference to the dtNode is
     * that it contains a crispDecisionFunction. The propagate method uses
     * this function to determine which child node to return: left node if
     * the condition is true, right node if it is false.
     */
    class crispNode : public decisionTree::dtNode {
    public:
      /**
       * default constructor
       */
      crispNode();

      /**
       * Constructor. Sets the nodes id.
       * @param theId the nodes id
       */
      crispNode(const int& theId);

      /**
       * Conveniance Constructor. New node with
       * univariateCrispDecisionFunction with the given parameters.
       * @param dim the dimension to be tested.
       * @param thresh the threshold in that dimension
       * @param theId the nodes id
       */
      crispNode(const int& dim, const double& thresh, const int& theId=-1);

      /**
       * Conveniance Constructor. New node with
       * multivariateCrispDecisionFunction with the given parameters.
       * @param w the weighting vector for the data.
       * @param thresh the threshold.
       * @param theId the nodes id
       */
      crispNode(const dvector& w, const double& thresh, const int& theId=-1);


      /**
       * copy constructor
       * @param other the crispNode object to be copied
       */
      crispNode(const crispNode& other);

      /**
       * destructor
       */
      ~crispNode();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a crispNode object
       * @param other the crispNode object to be copied
       * @return a reference to this crispNode object
       */
      crispNode& copy(const crispNode& other);

      /**
       * copy the contents of a crispNode object
       * @param other the crispNode object to be copied
       * @return a reference to this crispNode object
       */
      crispNode& operator=(const crispNode& other);


      /**
       * returns a pointer to a clone of the crispNode
       */
      virtual decisionTree::dtNode* clone() const;

      /**
       * Sets the crispDecisionFunction for this node
       * @param cdf the new %crispDecisionFunction
       */
      void setDecisionFunction(const crispDecisionFunction* cdf);

      /**
       * Gets the crispDecisionFunction this node uses.
       * @returns const pointer to the %crispDecisionFunction
       */
      const crispDecisionFunction*
      getDecisionFunction(const crispDecisionFunction* cdf);

      /**
       * Propagates the given value through the node. If the
       * crispDecisionFunction is true for this value, the left
       * child node is returned, else the right.
       * @param data value to be propagated
       * @returns left or right child node
       */
      const crispNode* propagate(const dvector& data) const;

      /**
       * write the crispNode in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the crispNode from the given ioHandler
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
      // the crispNode
      // ------------------------------------------------

    protected:

      /**
       * The objectFactory for all crispDecisionFunctions.
       */
      static const objectFactory<crispDecisionFunction>
      crispDecisionFunctionFactory;

      /**
       * The array of all crispDecisionFunctions.
       */
      static const crispDecisionFunction* cdfArray[];

      /**
       * The crispDecisionFunction used for this node
       */
      crispDecisionFunction* decisionF;

    };


    /**
     * default constructor
     */
    crispDecisionTree();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    crispDecisionTree(const crispDecisionTree& other);

    /**
     * destructor
     */
    virtual ~crispDecisionTree();

    /**
     * returns the name of this type ("crispDecisionTree")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this functor object
     */
    crispDecisionTree& copy(const crispDecisionTree& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    crispDecisionTree& operator=(const crispDecisionTree& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Classification.
     * Classifies the feature and returns the outputVector with
     * the classification result.
     * @param feature the %vector to be classified
     * @param result the result of the classification
     * @return false if an error occurred during classification else true
     */
    virtual bool
      classify(const dvector& feature, outputVector& result) const;

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

  protected:

    crispNode* root;

  };
}

#endif

