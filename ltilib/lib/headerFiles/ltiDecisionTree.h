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
 * file .......: ltiDecisionTree.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 19.2.2002
 * revisions ..: $Id: ltiDecisionTree.h,v 1.9 2005/06/16 09:32:40 doerfler Exp $
 */

#ifndef _LTI_DECISION_TREE_H_
#define _LTI_DECISION_TREE_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiObjectFactory.h"

#include "ltiSupervisedInstanceClassifier.h"

namespace lti {
  /**
   * This is the base class for all decision trees in the LTI-Lib
   */
  class decisionTree : public supervisedInstanceClassifier {
  public:


    /**
     * the parameters for the class decisionTree
     */
    class parameters : public supervisedInstanceClassifier::parameters {
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
     * a simple Node for decision trees
     */
    class dtNode : public object {
    public:

      /**
       * default constructor
       */
      dtNode();

      /**
       * Constructor. Sets the id of the node.
       * @param theId the nodes id.
       */
      dtNode(const int& theId);

      /**
       * copy constructor
       * @param other the dtNode object to be copied
       */
      dtNode(const dtNode& other);

      /**
       * destructor
       */
      virtual ~dtNode();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a dtNode object
       * @param other the dtNode object to be copied
       * @return a reference to this dtNode object
       */
      dtNode& copy(const dtNode& other);

      /**
       * copy the contents of a dtNode object
       * @param other the dtNode object to be copied
       * @return a reference to this dtNode object
       */
      dtNode& operator=(const dtNode& other);


      /**
       * returns a pointer to a clone of the dtNode
       */
      virtual dtNode* clone() const;

      /**
       * Sets the left child node and returns the previous
       * left child node.
       * <p><b>NOTE</b>: No copy or clone is performed, ie left is
       * the same pointer as newLeft. This is necessary for building trees.
       * @param newLeft new left child node
       * @returns the previous left child node
       */
      dtNode* setLeftChild(dtNode* newLeft);

      /**
       * Sets the right child node and returns the previous
       * right child node.
       * <p><b>NOTE</b>: No copy or clone is performed, ie left is
       * the same pointer as newLeft. This is necessary for building trees.
       * @param newRight new right child node
       * @returns the previous right child node
       */
      dtNode* setRightChild(dtNode* newRight);

      /**
       * Gets the left child node.
       */
      const dtNode* getLeftChild() const;

      /**
       * Gets the right child node.
       */
      const dtNode* getRightChild() const;

      /**
       * Returns true if this node is a leaf, ie it has no children.
       */
      bool isLeaf() const;

      /**
       * Set the nodes id
       */
      void setId(const int& theId);

      /**
       * Get the nodes id
       */
      int getId() const;

      /**
       * write the dtNode in the given ioHandler<p>
       * <b>NOTE:</b> Writing a node results in writing of the complete tree
       * following this node.
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the dtNode from the given ioHandler
       * <b>NOTE:</b> Reading a node results in reading of the complete tree
       * following this node.
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
      // the dtNode
      // ------------------------------------------------

    protected:

      /**
       * Assigns ids to the leaf node from left to right starting with
       * 0.  The ids are equivalent to the positions in the
       * decisionTrees outTemplate. The starting point is this
       * node. Usually, this method will be called for the root node
       * of some decisionTree. The method returns the number of leaf nodes.
       */
      int assignIds();

      /**
       * Pointer to the left child node
       */
      dtNode *left;

      /**
       * Pointer to the right child node
       */
      dtNode *right;

      /**
       * The nodes id. It is used as a reference to the nodes position in
       * the decisionTrees outTemplate. So the ids should always be
       * between 0 and n-1 with n being the number of leaf nodes of the
       * decision tree.
       */
      int id;

    };


    /**
     * default constructor
     */
    decisionTree();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    decisionTree(const decisionTree& other);

    /**
     * destructor
     */
    virtual ~decisionTree();

    /**
     * returns the name of this type ("decisionTree")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" classifier.
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    decisionTree& copy(const decisionTree& other);

    /**
     * alias for copy member
     * @param other the classifier to be copied
     * @return a reference to this classifier object
     */
    decisionTree& operator=(const decisionTree& other);

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

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


    //TODO: comment the attributes of your classifier
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

  protected:

    /**
     * The array of all dtNodes.
     */
    static const dtNode* nodeArray[];

    /**
     * The objectFactory for all types of dtNodes.
     */
    static const objectFactory<dtNode> nodeFactory;

    int numberOfLeafs;

  };
}

#endif

