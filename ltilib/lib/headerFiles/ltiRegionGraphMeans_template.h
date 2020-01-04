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


/* -------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiRegionGraphMeans_template.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.10.2003
 * revisions ..: $Id: ltiRegionGraphMeans_template.h,v 1.3 2004/05/10 10:46:23 alvarado Exp $
 */

#include "ltiSplitImageToRGB.h"

namespace lti {

  // --------------------------------------------------
  // regionGraphMeansNode
  // --------------------------------------------------
  template<class T> 
  regionGraphMeansNode<T>::regionGraphMeansNode() 
    : sumOfElements(T()),n(0) {    
  }

  template<class T> 
  regionGraphMeansNode<T>::~regionGraphMeansNode() {
  }

  template<class T> 
  regionGraphMeansNode<T>::regionGraphMeansNode(
                                       const regionGraphMeansNode<T>& other) 
    : sumOfElements(other.sumOfElements),n(other.n) {
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::operator+=(const regionGraphMeansNode<T>& other) {
    n+=other.n;
    sumOfElements+=other.sumOfElements;
    return *this;
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::operator=(const regionGraphMeansNode<T>& other) {
    n=other.n;
    sumOfElements=other.sumOfElements;
    return *this;
  }

  template<class T> 
  regionGraphMeansNode<T>&
  regionGraphMeansNode<T>::consider(const T& elem) {
    ++n;
    sumOfElements+=elem;
    return *this;
  }

  template<class T> 
  T regionGraphMeansNode<T>::computeMean() const {
    return sumOfElements/static_cast<float>(n);
  }

  template<class T> 
  int regionGraphMeansNode<T>::size() const {
    return n;
  }

  // IO methods

  template<class T>
  bool regionGraphMeansNode<T>::read(ioHandler& handler,const bool complete) {
    if (complete) {
      handler.readBegin();
    }
    
    lti::read(handler,"sum",sumOfElements);
    lti::read(handler,"n",n);

    if (complete) {
      handler.readEnd();
    }
    return true;
  }

  template<class T> 
  bool regionGraphMeansNode<T>::write(ioHandler& handler,const bool complete) const {
    if (complete) {
      handler.writeBegin();
    }
    
    lti::write(handler,"sum",sumOfElements);
    lti::write(handler,"n",n);

    if (complete) {
      handler.writeEnd();
    }

    return true;
  }


  // --------------------------------------------------
  // regionGraphColor
  // --------------------------------------------------
  template<class F>
  regionGraphColor<F>::regionGraphColor() 
    : parent() {
  }

  template<class F>
  regionGraphColor<F>::regionGraphColor(const parameters& par) 
    : parent(par) {
  }

  template<class F>
  regionGraphColor<F>::regionGraphColor(const regionGraphColor& other)
    : parent() {
    copy(other);
  }

  template<class F>
  regionGraphColor<F>::~regionGraphColor() {
  }

  // returns the name of this type
  template<class F>
  const char* regionGraphColor<F>::getTypeName() const {
    return "regionGraphColor<F>";
  }

  // clone member
  template<class F>
  functor* regionGraphColor<F>::clone() const {
    return new regionGraphColor(*this);
  }

  template<class F>
  bool regionGraphColor<F>::apply(const matrix<int>& regions,
                                  const image& image,
                                  const int minLabel,
                                  graph_type& graph) {
    
    if (regions.size() != image.size()) {
      this->setStatusString("image and labeled mask have different sizes");
      return false;
    }

    splitImageToRGB splitter;
    splitter.apply(image,c1,c2,c3); // c1,c2,c3 are attributes of the class
    
    return apply(regions,minLabel,graph);
  }

  template<class F>
  bool regionGraphColor<F>::generate(const matrix<int>& regions,
                                     const image& image,
                                     const int minLabel,
                                     graph_type& graph) {
    return apply(regions,image,minLabel,graph);
  }
  
  template<class F>
  bool regionGraphColor<F>::apply(const matrix<int>& regions,
                                  const channel& sc1,
                                  const channel& sc2,
                                  const channel& sc3,
                                  const int minLabel,
                                  graph_type& graph) {
    
    if ( (regions.size() != sc1.size()) ||
         (sc1.size() != sc2.size()) ||
         (sc2.size() != sc3.size()) ) {
      this->setStatusString("image and labeled mask have different sizes");
      return false;
    }

    // c1,c2,c3 are attributes of the class
    c1.copy(sc1);
    c2.copy(sc2);
    c3.copy(sc3);
    
    return apply(regions,minLabel,graph);
  }

  template<class F>
  bool regionGraphColor<F>::generate(const matrix<int>& regions,
                                     const channel& sc1,
                                     const channel& sc2,
                                     const channel& sc3,
                                     const int minLabel,
                                     graph_type& graph) {
    return apply(regions,sc1,sc2,sc3,minLabel,graph);
  }

  /*
   * Alias for merge()
   */
  template<class F>
  bool regionGraphColor<F>::apply(const weight_type& threshold,
				  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(threshold,graph,equivalences);
  }
  
  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphColor<F>::apply(graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(graph,equivalences);
  }
  
  /*
   * Alias for merge()
   */
  template<class F>
  bool regionGraphColor<F>::apply(const weight_type& threshold,
                                  const int minLabel,
				  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(threshold,minLabel,graph,equivalences);
  }
  
  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphColor<F>::apply(const int minLabel,
                                  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(minLabel,graph,equivalences);
  }


  /*
   * Alias for generate()
   */
  template<class F>
  bool regionGraphColor<F>::apply(const matrix<int>& regions,
				  const int minLabel,
				  graph_type& graph) {
    return parent::apply(regions,minLabel,graph);
  }


  // -------------------------------------------------------------------
  // Reimplementable methods
  // -------------------------------------------------------------------

  template<class F>
  bool 
  regionGraphColor<F>::checkInternalData(const point& regionsSize,
                                         const int maxRegionIndex) const {
    return ( (c1.size() == regionsSize) &&
             (c2.size() == regionsSize) &&
             (c3.size() == regionsSize) );
  }

  template<class F>
  bool 
  regionGraphColor<F>::considerForEdgeData(const point& p1,
                                           const point& p2,
                                           edge_data_type& edgeData) {
    edgeData++;
    return true;
  }

  template<class F>
  bool 
  regionGraphColor<F>::considerForNodeData(const point& p1,
                                           const int label,
                                           node_type& nodeData) {
    nodeData.consider(trgbPixel<float>(c1.at(p1),c2.at(p1),c3.at(p1)));
    return true;
  }


  // --------------------------------------------------
  // regionGraphGray
  // --------------------------------------------------
  template<class F>
  regionGraphGray<F>::regionGraphGray() 
    : parent() {
  }

  template<class F>
  regionGraphGray<F>::regionGraphGray(const parameters& par) 
    : parent(par) {
  }

  template<class F>
  regionGraphGray<F>::regionGraphGray(const regionGraphGray& other)
    : parent() {
    copy(other);
  }

  template<class F>
  regionGraphGray<F>::~regionGraphGray() {
  }

  // returns the name of this type
  template<class F>
  const char* regionGraphGray<F>::getTypeName() const {
    return "regionGraphGray<F>";
  }

  // clone member
  template<class F>
  functor* regionGraphGray<F>::clone() const {
    return new regionGraphGray(*this);
  }

  template<class F>
  bool regionGraphGray<F>::apply(const matrix<int>& regions,
                                  const channel& chnl,
                                  const int minLabel,
                                  graph_type& graph) {
    
    if (regions.size() != chnl.size()) {
      this->setStatusString("image and labeled mask have different sizes");
      return false;
    }

    c1.copy(chnl);

    return parent::apply(regions,minLabel,graph);
  }

  /*
   * Alias for merge()
   */
  template<class F>
  bool regionGraphGray<F>::apply(const weight_type& threshold,
				 graph_type& graph,
				 ivector& equivalences) {
    return parent::apply(threshold,graph,equivalences);
  }

  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphGray<F>::apply(graph_type& graph,
				 ivector& equivalences) {
    return parent::apply(graph,equivalences);
  }
  
  template<class F>
  bool regionGraphGray<F>::apply(const weight_type& threshold,
                                  const int minLabel,
				  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(threshold,minLabel,graph,equivalences);
  }
  
  /*
   * Alias for merge() taking the threshold from the parameters
   */
  template<class F>
  bool regionGraphGray<F>::apply(const int minLabel,
                                  graph_type& graph,
				  ivector& equivalences) {
    return parent::apply(minLabel,graph,equivalences);
  }

  /*
   * Alias for generate()
   */
  template<class F>
  bool regionGraphGray<F>::apply(const matrix<int>& regions,
                                 const int minLabel,
                                 graph_type& graph) {
    return parent::apply(regions,minLabel,graph);
  }

  // -------------------------------------------------------------------
  // Reimplementable methods
  // -------------------------------------------------------------------

  template<class F>
  bool 
  regionGraphGray<F>::checkInternalData(const point& regionsSize,
                                         const int maxRegionIndex) const {
    return (c1.size() == regionsSize);
  }

  template<class F>
  bool 
  regionGraphGray<F>::considerForEdgeData(const point& p1,
                                           const point& p2,
                                           edge_data_type& edgeData) {
    edgeData++;
    return true;
  }

  template<class F>
  bool 
  regionGraphGray<F>::considerForNodeData(const point& p1,
                                           const int label,
                                           node_type& nodeData) {
    nodeData.consider(c1.at(p1));
    return true;
  }



}
#include "ltiUndebug.h"
