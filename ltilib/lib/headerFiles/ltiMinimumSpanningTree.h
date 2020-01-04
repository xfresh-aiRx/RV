/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
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
 * file .......: ltiMinimumSpanningTree.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.4.2003
 * revisions ..: $Id: ltiMinimumSpanningTree.h,v 1.8 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_MINIMUM_SPANNING_TREE_H_
#define _LTI_MINIMUM_SPANNING_TREE_H_

#include "ltiFunctor.h"
#include "ltiL2Distance.h"
#include "ltiWeightedGraph.h"

namespace lti {

  /**
   * This class computes minimum spanning trees.  There are two ways
   * two compute a minimum spanning trees.  Either reduce a given
   * weighted graph to its minimum spanning tree or compute the
   * minimum spanning tree direct from a given data set.  If you only
   * want to reduce a weighted graph, just use the apply methods.  If
   * you want to compute the tree from a given data set (keys and
   * values), there a three possibilities.
   * -# The keys (data points) are elements of a std::vector
   *    Then use the apply method of this class.
   * -# A matrix where each row represents one key (data point). Use the 
   *    class minimumSpanningTreeOfKeyValuetype
   * -# A matrix where each element represents one key (data point).
   *    In this case use the class minimumSpanningTreeOfKeytype
   *
   * A minimum spanning tree has the following template types:
   * - \a K: the key used for building the MST
   * - \a V: the value or data contained in each node of the tree
   *   default is int for an id.
   * - \a Distantor: policy class for the calculation of the distance
   *   between two keys in the tree (see below). Default is the square
   *   of the L2 distance lti::l2SquareDistantor.
   *
   * The Distantor must implement the operator() which takes two
   * arguments of type K and calculates their distance. It must define
   * a type Distantor::distance_type which is also the return type of
   * the operator. See lti::l2SquareDistantor for an example.
   */
  template <class K, class V=int, class Distantor=l2SquareDistantor<K> >
  class minimumSpanningTree : public functor {
  public:

    /**
     * return type of a call to the distantor, Distantor::distance_type
     */
    typedef typename Distantor::distance_type distance_type;

    /**
     * the parameters for the class minimumSpanningTree
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : functor::parameters() {
        
        nbNeighbors = 7;
        useCompleteGraph = false;
      }

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : functor::parameters() {
        copy(other);
      }

      /**
       * destructor
       */
      ~parameters() {
      }

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "minimumSpanningTree<K,V,Distantor>::parameters";
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
# ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
# else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
          (const functor::parameters&) =
          functor::parameters::copy;
        (this->*p_copy)(other);
# endif

        nbNeighbors = other.nbNeighbors;
        useCompleteGraph = other.useCompleteGraph;

        return *this;
      }

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      }

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
        /**
         * this function is required by MSVC only, as a workaround for a
         * very awful bug, which exists since MSVC V.4.0, and still by
         * V.6.0 with all bugfixes (so called "service packs") remains
         * there...  This method is also public due to another bug, so please
         * NEVER EVER call this method directly: use write() instead
         */
        bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.writeBegin();
        }

        if (b) {
      
          lti::write(handler,"nbNeighbors",nbNeighbors);
          lti::write(handler,"useCompleteGraph",useCompleteGraph);
        }

# ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && functor::parameters::write(handler,false);
# else
        bool (functor::parameters::* p_writeMS)(ioHandler&,const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
# endif

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool write(ioHandler& handler,
                 const bool complete=true) const {
        // ...we need this workaround to cope with another really
        // awful MSVC bug.
        return writeMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true)
#     else
        /**
         * this function is required by MSVC only, as a workaround for a
         * very awful bug, which exists since MSVC V.4.0, and still by
         * V.6.0 with all bugfixes (so called "service packs") remains
         * there...  This method is also public due to another bug, so please
         * NEVER EVER call this method directly: use read() instead
         */
        bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
      
          lti::read(handler,"nbNeighbors",nbNeighbors);
          lti::read(handler,"useCompleteGraph",useCompleteGraph);
        }

# ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && functor::parameters::read(handler,false);
# else
        bool (functor::parameters::* p_readMS)(ioHandler&,const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
# endif

        if (complete) {
          b = b && handler.readEnd();
        }

        return b;
      }

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * The number of neighboring points each point is connected with,
       * before minimization
       */
      int nbNeighbors;

      /** 
       * If true, first a complete graph of the input data is computed, 
       * for searching the minimum spanning tree. If false an approximation 
       * is used of the complete graph is used. In most cases this approxiation
       * leads to the correct minimum spanning tree.
       * default is false.
       */
      bool useCompleteGraph;

    };

    /**
     * default constructor
     */
    minimumSpanningTree();

    /**
     * Construct a functor using the given parameters
     */
    minimumSpanningTree(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    minimumSpanningTree(const minimumSpanningTree& other);

    /**
     * destructor
     */
    virtual ~minimumSpanningTree();

    /**
     * returns the name of this type ("minimumSpanningTree")
     */
    virtual const char* getTypeName() const;

    /**
     * Computes the minimum spanning tree from the given data. The resulting
     * tree is returned in this graph.
     */
    virtual bool apply(const std::vector<K>& src,
                       const std::vector<V>& datas,
                       weightedGraph<K,V,distance_type>& graph) const;

    /**
     * Computes the minimum spanning tree from the given Graph. The resulting
     * tree is returned in graph dest.
     */
    virtual bool apply(
                       const weightedGraph<K,V,distance_type>& src,
                       weightedGraph<K,V,distance_type>& dest) const;

    /**
     * Computes the minimum spanning tree from the given Graph. The resulting
     * tree is returned in this graph.
     */
    virtual bool apply(weightedGraph<K,V,distance_type>& srcDest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimumSpanningTree& copy(const minimumSpanningTree& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimumSpanningTree& operator=(const minimumSpanningTree& other);

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
     * The value type of the key type.
     */
    typedef typename K::value_type key_value_type;

    /**
     * the type of a node of the weightedGraph used in the MST
     */
    typedef typename weightedGraph<K,V,distance_type>::node node_type;

    /**
     * Distantor
     */
    Distantor distantor;

    /**
     * Builds up a graph that can be used for computing the minimum spanning
     * tree. This graph is not complete. But in most cases the edges that 
     * this graph contains, are all edges that are needed for finding the 
     * correct minimum spanning tree.
     * The advantage of this version is that searching for the minimum 
     * spanning tree will be much faster.
     * For this tree all points are connected with their neighbors. The 
     * number of neighbors it is connected to can be set by the parameter
     * nbNeighbors. After that it is guaranteed that the graph is a 
     * connected graph.
     */
    bool buildGraph(const std::vector<K>& src,
                    const std::vector<V>& datas,
                    weightedGraph<K,V,distance_type>& graph) const;

    /**
     * Builds a complete graph of the input data.
     */
    bool buildCompleteGraph(const std::vector<K>& src,
                            const std::vector<V>& datas,
                            weightedGraph<K,V,distance_type>& graph) const;


  };
}

#include "ltiMinimumSpanningTree_template.h"

#endif

