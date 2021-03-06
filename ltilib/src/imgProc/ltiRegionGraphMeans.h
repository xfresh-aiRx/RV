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
 * revisions ..: $Id: ltiRegionGraphMeans.h,v 1.2 2004/05/10 09:17:57 alvarado Exp $
 */

#ifndef _LTI_REGION_GRAPH_MEANS_H_
#define _LTI_REGION_GRAPH_MEANS_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifdef _LTI_MSC_6

#pragma message("Insufficient C++ Template Support for lti::regionGraphMeans.")
#pragma message("You need a newer compiler")

#else

#include "ltiImage.h"
#include "ltiRegionGraphFunctor.h"
#include "ltiL2Distance.h"
#include <string>

namespace lti {

  /**
   * Type for adjacency graph nodes containing the mean values of each
   * region.
   *
   * The template value specifies the data type representing a "mean", or 
   * better said, \a n times the mean, with \a n the number of pixels in the
   * region.
   *
   * The type \c T can be for instance trgbPixel<float> or just a
   * scalar \c double or \c float, but in general it is a type with a
   * default constructor that initialize to zero or a neutral element,
   * and also accepts the operator+=, and operator/(int).
   */
  template <class T>
  class regionGraphMeansNode : public ioObject {
  public:
    /**
     * Type used to represent the mean value
     */
    typedef T value_type;

    /**
     * Default constructor initialize class with "zero" state
     */
    regionGraphMeansNode();

    /**
     * Default constructor initialize class with "zero" state
     */
    ~regionGraphMeansNode();

    /**
     * Copy constructor
     */
    regionGraphMeansNode(const regionGraphMeansNode<T>& other);

    /**
     * Operator+= required by the interface to combine two nodes
     */
    regionGraphMeansNode<T>& 
    operator+=(const regionGraphMeansNode<T>& other);

    /**
     * Assignment operator
     */
    regionGraphMeansNode<T>&
    operator=(const regionGraphMeansNode<T>& other);

    /**
     * Consider one element to the means computation.
     */
    regionGraphMeansNode<T>& consider(const T& elem);

    /**
     * Compute the mean value
     */
    T computeMean() const;

    /**
     * Return the number of points considered
     */
    int size() const;

    /**
     * Read node from ioHandler
     */
    bool read(ioHandler& handler,const bool complete=true);

    /**
     * Write node to ioHandler
     */
    bool write(ioHandler& handler,const bool complete=true) const;

  protected:

    /**
     * Contains the sum of the elements considered until now.
     */
    T sumOfElements;

    /**
     * Number of elements considered until now.
     */
    int n;    
  };
  
  /**
   * Compute the weight between two scalar nodes as the absolute value
   * of the difference
   */
  class regionGraphScalarMeanDistance {
  public:    
    typedef regionGraphMeansNode<float> node_type;
    float operator()(const node_type& a,
                     const node_type& b,
                     const int& data) const;
  };
    
  /**
   * Compute the weight between two scalar nodes as the absolute value
   * of the difference
   */
  class regionGraphScalarHarisDistance {
  public:    
    typedef regionGraphMeansNode<float> node_type;
    float operator()(const node_type& a,
                     const node_type& b,
                     const int& data) const;
  };

  /**
   * Compute the weight between two color point nodes as the euclidean
   * distance between both points.
   */
  class regionGraphColorMeanDistance {
  public:    
    typedef regionGraphMeansNode< trgbPixel<float> > node_type;
    float operator()(const node_type& a,
                     const node_type& b,
                     const int& data) const;
  };

  /**
   * Compute the weight between two color point nodes as the euclidean
   * distance between both points.
   */
  class regionGraphColorHarisDistance {
  public:    
    typedef regionGraphMeansNode< trgbPixel<float> > node_type;
    float operator()(const node_type& a,
                     const node_type& b,
                     const int& data) const;
  };


  /**
   * Functor to manipulate graphs of adjacent image regions, where the
   * nodes of the graphs contain the mean value of the region represented by
   * the node.
   *
   * The template F is the distance computation functor required by the graph.
   */
  template<class F>
  class regionGraphColor
    : public regionGraphFunctor< 
                      adjacencyGraph< regionGraphMeansNode< trgbPixel<float> >,
                                      float,
                                      int,
                                      F,
                                      symmetricEdgeTraits<float> > > {
  public:
    /**
     * Parent class shortcut
     */
    typedef regionGraphFunctor< 
                      adjacencyGraph< regionGraphMeansNode< trgbPixel<float> >,
                                      float,
                                      int,
                                      F,
                                      symmetricEdgeTraits<float> > > parent;

    typedef typename parent::parameters parameters;
    typedef typename parent::graph_type graph_type;
    typedef typename parent::weight_type weight_type;
    typedef typename parent::edge_data_type edge_data_type;
    typedef typename parent::node_type node_type;

    /**
     * default constructor
     */
    regionGraphColor();

    /**
     * Construct a functor using the given parameters
     */
    regionGraphColor(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regionGraphColor(const regionGraphColor& other);

    /**
     * destructor
     */
    virtual ~regionGraphColor();

    /**
     * returns the name of this type ("regionGraphColorMean")
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;
    
    /**
     * Generate a graph representation for the given \a image using the
     * partition in \a regions and considering only the labels above or
     * equal \a minLabel.  The nodes of the graph contain at the end enough
     * information to compute the color mean values of each region.
     */
    bool apply(const matrix<int>& regions,
               const image& image,
               const int minLabel,
               graph_type& graph);
 
    /**
     * Generate a graph representation for the given \a image using the
     * partition in \a regions and considering only the labels above or
     * equal \a minLabel. The nodes of the graph contain at the end enough
     * information to compute the color mean values of each region.
     */
    bool apply(const matrix<int>& regions,
               const channel& c1,
               const channel& c2,
               const channel& c3,
               const int minLabel,
               graph_type& graph);
  
    /**
     * Generate a graph representation for the given \a image using the
     * partition in \a regions and considering only the labels above or
     * equal \a minLabel.  The nodes of the graph contain at the end enough
     * information to compute the color mean values of each region.
     *
     * This is an alias.  You can also directly call the corresponding
     * apply method.
     */
    bool generate(const matrix<int>& regions,
                  const image& image,
                  const int minLabel,
                  graph_type& graph);
 
    /**
     * Generate a graph representation for the given \a image using the
     * partition in \a regions and considering only the labels above or
     * equal \a minLabel. The nodes of the graph contain at the end enough
     * information to compute the color mean values of each region.
     *
     * This is an alias.  You can also directly call the corresponding
     * apply method.
     */
    bool generate(const matrix<int>& regions,
                  const channel& c1,
                  const channel& c2,
                  const channel& c3,
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

  protected:
    /**
     * Alias for generate()
     */
    bool apply(const matrix<int>& regions,
               const int minLabel,
               graph_type& graph);

    /**
     * @name Reimplemented virtual methods
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
    
    //@}

    /**
     * @name Internal data
     */
    //@{
    channel c1;
    channel c2;
    channel c3;
    //@}
    

  };

  /**
   * Functor to manipulate graphs of adjacent image regions, where the
   * nodes of the graphs contain the mean value of the region represented by
   * the node.
   *
   * The template F is the distance computation functor required by the graph.
   */
  template<class F = regionGraphScalarMeanDistance>
  class regionGraphGray
    : public regionGraphFunctor< 
                      adjacencyGraph< regionGraphMeansNode< float >,
                                      float,
                                      int,
                                      F,
                                      symmetricEdgeTraits<float> > > {
  public:
 
    /**
     * Type definitions of the parent class
     */
    //@{
    typedef regionGraphFunctor< 
                      adjacencyGraph< regionGraphMeansNode< float >,
                                      float,
                                      int,
                                      F,
                                      symmetricEdgeTraits<float> > > parent;

    typedef typename parent::parameters parameters;
    typedef typename parent::graph_type graph_type;
    typedef typename parent::weight_type weight_type;
    typedef typename parent::edge_data_type edge_data_type;
    typedef typename parent::node_type node_type;
    //@}
    
    /**
     * default constructor
     */
    regionGraphGray();

    /**
     * Construct a functor using the given parameters
     */
    regionGraphGray(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regionGraphGray(const regionGraphGray& other);

    /**
     * destructor
     */
    virtual ~regionGraphGray();

    /**
     * returns the name of this type ("regionGraphGrayMean")
     */
    virtual const char* getTypeName() const;

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;
    
    /**
     * Alias for generate()
     */
    bool apply(const matrix<int>& regions,
               const channel& chnl,
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

  protected:
    /**
     * Alias for generate()
     */
    bool apply(const matrix<int>& regions,
               const int minLabel,
               graph_type& graph);

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
    
    //@}

    /**
     * @name Internal data
     */
    //@{
    channel c1;
    //@}
    

  };


  /**
   * Alias for graph representation with plain mean value distances.
   */
  typedef regionGraphColor<regionGraphColorMeanDistance> regionGraphColorMeans;

  /**
   * Alias for graph representation with weighted mean value distances.
   */
  typedef regionGraphColor<regionGraphColorHarisDistance> 
          regionGraphColorHaris;

  /**
   * Alias for graph representation with plain mean value distances.
   */
  typedef regionGraphGray<regionGraphScalarMeanDistance> regionGraphGrayMeans;

  /**
   * Alias for graph representation with weighted mean value distances.
   */
  typedef regionGraphGray<regionGraphScalarHarisDistance> 
          regionGraphGrayHaris;
}

#endif
#endif
