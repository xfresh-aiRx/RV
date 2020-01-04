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
 * file .......: ltiRegionGraphFunctor.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.10.2003
 * revisions ..: $Id: ltiRegionGraphFunctor.h,v 1.3 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_REGION_GRAPH_FUNCTOR_H_
#define _LTI_REGION_GRAPH_FUNCTOR_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifdef _LTI_MSC_6

#pragma message("Insufficient C++ Template Support for lti::regionGraphMeans.")
#pragma message("You need a newer compiler")

#else

#include "ltiImage.h"
#include "ltiAdjacencyGraph.h"
#include "ltiFunctor.h"
#include <string>

namespace lti {
  /**
   * Functor to manipulate graphs of adjacent image regions.
   *
   * This class provides some functionality to create and manipulate
   * lti::adjacencyGraph data structures in the context of image region 
   * analysis.
   *
   * It is a template class with template parameter \c G, which is expected
   * to be a valid lti::adjacencyGraph type.
   *
   * The class is "almost" abstract, since the real useful instances belong to
   * derived classes that implement specific graph generation and merging
   * strategies.  However, it can be used as a standalone class for simple
   * graph operations where only the topology of the graph is of interest.
   *
   * Derived classes should reimplement following protected methods:
   * - bool checkInternalData(const point&,const int) which should check
   *   the sizes of internal data structures necessary to compute the graph.
   *   This could be for example the size of color channels or gradient
   *   channels, or the size of vectors containing statistics for each
   *   region.
   * - considerForEdgeData(const point&,const point&,edge_data_type&)
   *   called while computing the graph topology each time two neighbor pixels
   *   belonging to two different regions are found.  You can use it to 
   *   accumulate in the third argument some values that can be later used
   *   to compute further statistics, like border length, mean color or
   *   gradient value at the border, etc.
   * - considerForNodeData(const point&,const int,node_type&) 
   *   called for each pixel of each region when generating the graph topology.
   *   It can serve to precompute some statistics while visiting the labels
   *   mask.  This way you can save traversing the image several times to
   *   compute different statistics, if you accumulate everything that is
   *   required for that task here.
   * - collectEdgeAndNodeData(graph_type&) allows to compute some additional
   *   information from the data accumulated in the graph as its topology
   *   was determined and before the weights for the edges are computed
   *   (see generate() method for more information).
   *
   * An important constraint to the graph is that the data (not the weights)
   * associated to an edge (a,b) is always the same data associated to edge
   * (b,a).
   * 
   */
  template<class G>
  class regionGraphFunctor : public functor {
  public:

    /**
     * Graph type
     */
    typedef G graph_type;

    /**
     * Edge traits
     */
    typedef typename graph_type::edge_traits edge_traits;

    /**
     * Type used for the weights.  They should usually be float or double, but
     * any type supported by the lti::vector can be used.
     */
    typedef typename graph_type::weight_type weight_type;
    
    /**
     * Edge denotation with the pair of node ids
     */
    typedef typename graph_type::node_pair node_pair;

    /**
     * Edge denotation with the pair of node ids
     */
    typedef typename graph_type::node_type node_type;

    /**
     * Edge data type
     */
    typedef typename graph_type::edge_data_type edge_data_type;

    /**
     * the parameters for the class regionGraphFunctor
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Type used to specify how to merge regions
       */
      enum eMergeMode {
        Fast,    /**< Fast method, which does not recompute distances.  Only
                    the initial values are used.*/
        Optimal  /**< Slower method, which recomputes distances when two
                    regions are merged.*/
      };


      /**
       * default constructor
       */
      parameters(const eMergeMode& mode = Fast,
                 const weight_type& threshold = weight_type() ) 
        : functor::parameters() {
        mergeMode = mode;
        mergeThreshold = threshold;
        minRegionNumber = 1;
      };

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
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "regionGraphFunctor::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        functor::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        functor::parameters& (functor::parameters::* p_copy)
                            (const functor::parameters&) =
                            functor::parameters::copy;
        (this->*p_copy)(other);
#     endif
        
        mergeMode = other.mergeMode;
        mergeThreshold = other.mergeThreshold;
        minRegionNumber = other.minRegionNumber;

        return *this;
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other) {
        return copy(other);
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

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
          switch(mergeMode) {
            case Optimal:
              lti::write(handler,"mergeMode","Optimal");
              break;
            case Fast:
              lti::write(handler,"mergeMode","Fast");
              break;
            default:
              lti::write(handler,"mergeMode","Optimal");
          }

          lti::write(handler,"mergeThreshold",mergeThreshold);
          lti::write(handler,"minRegionNumber",minRegionNumber);
        }

#     ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not able to
        // compile...
        b = b && functor::parameters::write(handler,false);
#     else
        bool (functor::parameters::* p_writeMS)(ioHandler&,
                                                         const bool) const =
          functor::parameters::writeMS;
        b = b && (this->*p_writeMS)(handler,false);
#     endif

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
          std::string str;
          lti::read(handler,"mergeMode",str);
          if (str == "Optimal") {
            mergeMode = Optimal;
          } else {
            mergeMode = Fast;
          }

          lti::read(handler,"mergeThreshold",mergeThreshold);
          lti::read(handler,"minRegionNumber",minRegionNumber);
        }

#       ifndef _LTI_MSC_6
        // This is the standard C++ code, which MS Visual C++ 6 is not
        // able to compile...
        b = b && functor::parameters::read(handler,false);
#       else
        bool (functor::parameters::* p_readMS)(ioHandler&,
                                                        const bool) =
          functor::parameters::readMS;
        b = b && (this->*p_readMS)(handler,false);
#       endif

        if (complete) {
          b = b && handler.readEnd();
        }
        
        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Merge strategy to be used:
       * - Fast: freezes the graph weights, recomputing them only if
       *         necessary (when you merge A and B, and both had edges to
       *         an edge C, the new edge will be computed automatically using
       *         the method += of the G::edge_data_type::operator+=).
       *         No distances between nodes will be recomputed, saving lots of
       *         time.  Additionally, the merge order of nodes is 
       * - Optimal: When two nodes are merged, their data and the weights of
       *            the edges to all neighbors will be recomputed.  This will
       *            change the merge order, which always chooses to merge
       *            the nodes with the smallest distance.
       */
      eMergeMode mergeMode;

      /**
       * For the merge methods, this is the maximal weight between two
       * nodes allowed.  This means, edges with weights larger than this
       * value won't be merged.
       *
       * Very low values will produce an oversegmentation, larger values
       * produce undersegmentations.
       *
       *
       * Default value: weight_type()
       */
      weight_type mergeThreshold;

      /**
       * Minimum number of regions.
       *
       * The merge operation can be controled by the merge-threshold, but
       * if it is set too large, the final result would contain just one
       * node.  This parameter allows you to specify the minimal number of
       * nodes the graph has to retain after merging.  Of course, if the
       * input graph has already less nodes than the number specified here,
       * then nothing will be done.
       *
       * Default value: 1
       */
      int minRegionNumber;

    };

    /**
     * default constructor
     */
    regionGraphFunctor(const bool initParams=true);

    /**
     * default constructor
     */
    regionGraphFunctor(const typename parameters::eMergeMode& mode,
                       const weight_type& threshold = weight_type());
    

    /**
     * Construct a functor using the given parameters
     */
    regionGraphFunctor(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regionGraphFunctor(const regionGraphFunctor& other);

    /**
     * destructor
     */
    virtual ~regionGraphFunctor();

    /**
     * returns the name of this type ("regionGraphFunctor")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionGraphFunctor& copy(const regionGraphFunctor& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionGraphFunctor& operator=(const regionGraphFunctor& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Alias for generate()
     */
    bool apply(const matrix<int>& regions,
               const int minLabel,
               graph_type& graph);

    /**
     * Alias for merge()
     */
    bool apply(const weight_type& threshold,
               graph_type& graph,
               ivector& equivalences);

    /**
     * Alias for merge() taking the threshold from the parameters
     */
    bool apply(graph_type& graph,
               ivector& equivalences);

    /**
     * Alias for merge()
     */
    bool apply(const weight_type& threshold,
               const int minLabel,
               graph_type& graph,
               ivector& equivalences);

    /**
     * Alias for merge() taking the threshold from the parameters
     */
    bool apply(const int minLabel,
               graph_type& graph,               
               ivector& equivalences);

    /**
     * Generate the adjacency graph for the given labeled mask \a regions.
     *
     * The generation of a graph consists of three phases:
     * -# Topology creation.  Here the graph topology is determined traversing
     *    the given regions mask and detecting adjacent pixels.  While 
     *    traversing, edge and node data can be accumulated or computed using
     *    the virtual methods considerForEdgeData() and considerForNodeData().
     * -# Data preparation. Once some data in the nodes and edges has been 
     *    collected, you can prepare it for the computation of weights.  This
     *    means, you can compute histograms, maxima and minima, etc. which
     *    can be used to control the range where the weights will be.
     * -# Edge weight computation.  The method in the edge_type of the
     *    graph "weight()" will be used to compute the weights of all edges
     *    at once.
     *
     * This method ensures that the node ids corresponds to the region labels.
     *
     * @param regions the labeled mask to be analyzed
     * @param minLabel only the adjacency between region-labels greater or
     *                 equal this value will be fully analyzed.  No graph edges
     *                 between regions with labels lower than \a minLabel will
     *                 be detected.
     * @param graph the adjacency graph to be generated
     * @return true if successful, false otherwise.
     */
    bool generate(const matrix<int>& regions,
                  const int minLabel,
                        graph_type& graph);
    
    /**
     * Generate the adjacency graph for the given labeled mask \a regions,
     * using for each region-node the data contained in the given \a data
     * vector.  However, the method \a considerForNodeData() has priority,
     * i.e. if this method can change the node's data.
     *
     * This will ensure that the node id corresponds to the region label.
     *
     * @param regions the labeled mask to be analyzed
     * @param minLabel only the adjacency between region-labels greater or
     *                 equal this value will be fully analyzed.  No graph edges
     *                 between regions with labels lower than \a minLabel will
     *                 be detected.
     * @param data vector containing at the element index \a i data for the
     *             node representing region \a i.
     * @param graph the adjacency graph to be generated
     * @return true if successful, false otherwise.
     */
    bool generate(const matrix<int>& regions,
                  const int minLabel,
                  const std::vector<node_type>& data,
                        graph_type& graph);


    /**
     * Compute the affinity matrix and degree vector for the given graph.
     *
     * The affinity matrix containts the weights of the edges.  The degree
     * vector the sum of the outgoing edges for each node.
     *
     * Several functors can require this sort of data structures.  One
     * prominent example is the functor lti::multiclassNormalizedCuts.
     */
    virtual bool affinityMatrix(const graph_type& graph,
                                matrix<weight_type>& affinity,
                                const weight_type noEdgeValue=weight_type()
                                ) const;
    
    /**
     * Merge all nodes whose edges have a weight smaller or equal than the
     * given threshold.
     *
     * The output will be not only the new graph with the merged data, but
     * an equivalences vector, where for each node id in the original graph
     * a new id is determined.
     */
    virtual bool merge(const weight_type& threshold,
                       graph_type& graph,
                       ivector& equivalences) const;

    /**
     * Merge all nodes whose edges have a weight smaller or equal than the
     * given threshold and one or both nodes have a label above or equal
     * the given minLabel.
     *
     * The output will be not only the new graph with the merged data, but
     * an equivalences vector, where for each node id in the original graph
     * a new id is determined.
     */
    virtual bool merge(const weight_type& threshold,
                       const int minLabel,
                       graph_type& graph,
                       ivector& equivalences) const;

    /**
     * Simple helper method to reassign the labels used in the given labeled
     * mask using an equivalences vector, as computed by merge.
     *
     * You must ensure that the equivalences vector has enough elements to
     * avoid an improper memory access, i.e. equivalences.size() >
     * regions.maximum().
     *
     * @param equivalences vector containing for each label \a i the new
     *                     label.
     * @param regions matrix with the source input labels.  The result will
     *                be left here too.
     * @param compact flag to indicate if the equivalences should be use
     *                as they are (false), or if a new labels set should
     *                be computed which lies between 0 and n-1, with n
     *                the total number of different labels used in the
     *                equivalences vector.
     * @return true if successful, false otherwise.
     */
    bool reassignLabels(const ivector& equivalences,
                              imatrix& regions,
                        const bool compact = false) const;
                        

    /**
     * Simple helper method to reassign the labels used in the given labeled
     * mask using an equivalences vector, as computed by merge.
     *
     * You must ensure that the equivalences vector has enough elements to
     * avoid an improper memory access, i.e. equivalences.size() >
     * regions.maximum().
     *
     * @param equivalences vector containing for each label \a i the new
     *                     label.
     * @param regions matrix with the source input labels. 
     * @param newRegions the new labels mask will be left here.
     * @param compact flag to indicate if the equivalences should be use
     *                as they are (false), or if a new labels set should
     *                be computed which lies between 0 and n-1, with n
     *                the total number of different labels used in the
     *                equivalences vector.
     * @return true if successful, false otherwise.
     */
    bool reassignLabels(const ivector& equivalences,
                        const imatrix& regions,
                              imatrix& newRegions,
                        const bool compact = false) const;


    /**
     * Simple helper method to reassign the labels used in the given labeled
     * mask using an equivalences vector, as computed by merge.
     *
     * You must ensure that the equivalences vector has enough elements to
     * avoid an improper memory access, i.e. equivalences.size() >
     * regions.maximum().
     *
     * @param equivalences vector containing for each label \a i the new
     *                     label.
     * @param regions matrix with the source input labels. 
     * @param newRegions the new labels mask will be left here.
     * @param regionSizes recompute the number of pixels per eventually
     *        new region label.
     * @param compact flag to indicate if the equivalences should be use
     *                as they are (false), or if a new labels set should
     *                be computed which lies between 0 and n-1, with n
     *                the total number of different labels used in the
     *                equivalences vector.
     * @return true if successful, false otherwise.
     */
    bool reassignLabels(const ivector& equivalences,
                        const imatrix& regions,
                              imatrix& newRegions,
                              ivector& regionSizes,
                        const bool compact = false) const;
    
    /**
     * Simple helper method to reassign the labels used in the given labeled
     * mask using an equivalences vector, as computed by merge.
     *
     * You must ensure that the equivalences vector has enough elements to
     * avoid an improper memory access, i.e. equivalences.size() >
     * regions.maximum().
     *
     * @param equivalences vector containing for each label \a i the new
     *                     label.
     * @param regions matrix with the source input labels, the resulting
     *                labels will be left here too. 
     * @param regionSizes recompute the number of pixels per eventually
     *        new region label.
     * @param compact flag to indicate if the equivalences should be use
     *                as they are (false), or if a new labels set should
     *                be computed which lies between 0 and n-1, with n
     *                the total number of different labels used in the
     *                equivalences vector.
     * @return true if successful, false otherwise.
     */
    bool reassignLabels(const ivector& equivalences,
                              imatrix& regions,
                              ivector& regionSizes,
                        const bool compact = false) const;

    /**
     * Compact labels
     *
     * Recompute the equivalences vector to use a compact set of labels.
     * @param equivalences equivalences vector as returned by merge()
     * @param newEquivalences new compact equivalences vector, where only
     *        a compact set of labels is used.
     * @return the total number of labels now being used.
     */
    int compactLabels(const ivector& equivalences,
                            ivector& newEquivalences) const;
    
  protected:
    /**
     * @name Methods to be reimplemented
     */
    //@{

    /**
     * Check if the internal data is compatible with the region mask.
     *
     * The weights between region nodes can be computed from very different
     * information sources.  Usually, this information is coded in images and
     * channels also extracted from the same image from which the regions map
     * was computed.  Therefore, it is important to provide a way to check if
     * the internal data is compatible with the regions map, since both must
     * be provided by the user at different times.
     *
     * @param regionsSize size of the original image and region mask.
     *                    This allows to check for channels and other data 
     *                    in the original image size format.
     * @param maxRegionIndex maximum region index employed.  This is used
     *                       to check if the vectors with information per
     *                       region have the appropriate size.
     * @return true if the internal data is compatible, false if there 
     *         are problems.
     */
    virtual bool checkInternalData(const point& regionsSize,
                                   const int maxRegionIndex) const;

    /**
     * For each two neighbor pixels that belong to different regions, this
     * method is called to compute the edge data value in a sequential manner.
     * It will be assumed, that the default constructor of the
     * graph_type::edge_type::data_type "resets" correctly the data of the
     * edge.
     *
     * @param p1 coordinates of pixel belonging to one region
     * @param p2 coordinates of pixel belonging to another region
     * @param edgeData reference to data structure of the edge, where
     *        some values can be accumulated or set.
     * @return true if successful, false otherwise.
     */
    virtual bool considerForEdgeData(const point& p1,
                                     const point& p2,
                                     edge_data_type& edgeData);

    /**
     * For each pixel in each region, this method is called once to 
     * compute statistics or other data for each node.
     *
     * @param p1 coordinates of the current pixel
     * @param label identification label for the region to which the pixel
     *              belongs.
     * @param nodeData reference to the data structure of the node, where
     *                 some values can be accumulated or set.
     * @return true if successful, false otherwise.
     */
    virtual bool considerForNodeData(const point& p1,
                                     const int label,
                                     node_type& nodeData);

    /**
     * This method is called just after the topological graph has been
     * build and all data of edges and nodes has been considered, and
     * before the weights are computed.
     *
     * You can overload this method to, for example, compute mean values
     * per region, compute the min an max values of the means, to adaptivelly
     * assign the weights to predefined ranges, etc.
     *
     * \warning: only the data for the edges(a,b) with a<b contain valid data.
     */
    virtual bool prepareEdgeAndNodeData(graph_type& graph);
    
    //@}

    // Protected stuff
    
    /**
     * Copy the bottom left diagonal matrix into the upper left.
     * This is used if a symmetric graph has been used, for which at first
     * only the data in the bottom matrix is computed.
     */
    bool mirrorEdgeData(graph_type& graph) const;

    /**
     * Generate the adjacency graph for the given labeled mask \a regions.
     *
     * It is assumed that the nodes of the graph have been already initialized.
     *
     * @param regions the labeled mask to be analyzed
     * @param minLabel only the adjacency between region-labels greater or
     *                 equal this value will be fully analyzed.  No graph edges
     *                 between regions with labels lower than \a minLabel will
     *                 be detected.
     * @param graph the adjacency graph to be generated
     * @return true if successful, false otherwise.
     */
    bool generateWorker(const matrix<int>& regions,
                        const int minLabel,
                              graph_type& graph);


  };
}

#include "ltiRegionGraphFunctor_template.h"
#endif
#endif
