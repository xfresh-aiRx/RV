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
 * file .......: ltiAdjacencyGraphFunctor.cpp
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.10.2003
 * revisions ..: $Id: ltiRegionGraphFunctor_template.h,v 1.1 2004/05/06 10:27:57 alvarado Exp $
 */

#undef _LTI_DEBUG
//#define _LTI_DEBUG 1
#include "ltiDebug.h"

#ifdef _LTI_DEBUG
#include <iostream>
#include "ltiLispStreamHandler.h"

namespace std {
  inline ostream& operator<<(ostream& out,lti::ioObject& obj) {
    lti::lispStreamHandler lsh(out);
    obj.write(lsh);
    return out;
  }
}

#endif

namespace lti {
  // --------------------------------------------------
  // regionGraphFunctor::parameters
  // --------------------------------------------------

  // implementation on header file due to MS VC++ bug

  // --------------------------------------------------
  // regionGraphFunctor
  // --------------------------------------------------

  // default constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const bool initParams)
    : functor() {
    if (initParams) {
      // create an instance of the parameters with the default values
      parameters defaultParameters;
      // set the default parameters
      setParameters(defaultParameters);
    }
  }

  // default constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(
                                   const typename parameters::eMergeMode& mode,
                                   const weight_type& threshold)
    : functor() {
    // create an instance of the parameters with the default values
    parameters defaultParameters(mode,threshold);
    // set the default parameters
    setParameters(defaultParameters);
  }

  // default constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const parameters& par)
    : functor() {

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template<class G>
  regionGraphFunctor<G>::regionGraphFunctor(const regionGraphFunctor<G>& other)
    : functor() {
    copy(other);
  }

  // destructor
  template<class G>
  regionGraphFunctor<G>::~regionGraphFunctor() {
  }

  // returns the name of this type
  template<class G>
  const char* regionGraphFunctor<G>::getTypeName() const {
    return "regionGraphFunctor<G>";
  }

  // copy member
  template<class G>
  regionGraphFunctor<G>&
  regionGraphFunctor<G>::copy(const regionGraphFunctor<G>& other) {
    functor::copy(other);

    return (*this);
  }

  // alias for copy member
  template<class G>
  regionGraphFunctor<G>&
  regionGraphFunctor<G>::operator=(const regionGraphFunctor<G>& other) {
    return (copy(other));
  }


  // clone member
  template<class G>
  functor* regionGraphFunctor<G>::clone() const {
    return new regionGraphFunctor<G>(*this);
  }

  // return parameters
  template<class G>
  const typename regionGraphFunctor<G>::parameters&
  regionGraphFunctor<G>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }

  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  // On place apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const matrix<int>& regions,
                                    const int minLabel,
                                    graph_type& graph)  {

    return generate(regions,minLabel,graph);
  };

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const weight_type& threshold,
                                    graph_type& graph,
                                    ivector& equivalences)  {

    return merge(threshold,graph,equivalences);
  };

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(graph_type& graph,
                                    ivector& equivalences)  {

    return merge(getParameters().mergeThreshold,graph,equivalences);
  };

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const weight_type& threshold,
                                    const int minLabel,
                                    graph_type& graph,
                                    ivector& equivalences)  {

    return merge(threshold,minLabel,graph,equivalences);
  };

  // On copy apply for type matrix<int>!
  template<class G>
  bool regionGraphFunctor<G>::apply(const int minLabel,
                                    graph_type& graph,
                                    ivector& equivalences)  {

    return merge(getParameters().mergeThreshold,minLabel,graph,equivalences);
  };

  // -------------------------------------------------------------------
  // Reimplementable methods
  // -------------------------------------------------------------------

  template<class G>
  bool 
  regionGraphFunctor<G>::checkInternalData(const point& regionsSize,
                                           const int maxRegionIndex) const {
    // default implementation does nothing:
    // internal data (which is nonexisten) has always the proper size!
    return true;
  }

  template<class G>
  bool 
  regionGraphFunctor<G>::considerForEdgeData(const point& p1,
                                             const point& p2,
                                             edge_data_type& edgeData) {
    // default implementation does nothing:
    // internal data (which is nonexistent) has always the proper size!
    return true;
  }

  template<class G>
  bool 
  regionGraphFunctor<G>::considerForNodeData(const point& p1,
                                             const int label,
                                             node_type& edgeData) {
    // default implementation does nothing:
    // internal data (which is nonexistent) has always the proper size!
    return true;
  }

  template<class G>
  bool 
  regionGraphFunctor<G>::prepareEdgeAndNodeData(graph_type& graph) {
    _lti_debug4(graph << std::endl);

    return true;
  }

  // -------------------------------------------------------------------
  // The functionality methods
  // -------------------------------------------------------------------

  template<class G>
  bool regionGraphFunctor<G>::generate(const matrix<int>& regions,
                                       const int minLabel,
                                       graph_type& graph) {

    // clear the graph
    graph.clear();

    const int lastLabel = regions.maximum();

    if (!checkInternalData(regions.size(),lastLabel)) {
      setStatusString("Internal data check failed");
      return false;
    }

    // set the number of nodes of the graph
    graph.resize(lastLabel+1);

    return generateWorker(regions,minLabel,graph);
  }

  template<class G>
  bool regionGraphFunctor<G>::generate(const matrix<int>& regions,
                                       const int minLabel,
                                       const std::vector<node_type>& data,
                                       graph_type& graph) {

    // clear the graph
    graph.clear();

    const int lastLabel = regions.maximum();

    if (!checkInternalData(regions.size(),lastLabel)) {
      setStatusString("Internal data check failed");
      return false;
    }

    graph.resize(lastLabel+1);

    int i;
    const int s = min(static_cast<int>(data.size()),lastLabel+1);
    
    for (i=0;i<s;++i) {
      graph.getNodeData(i) = data[i];
    }

    return generateWorker(regions,minLabel,graph);
  }


  template<class G>
  bool regionGraphFunctor<G>::generateWorker(const matrix<int>& regions,
                                             const int minLabel,
                                             graph_type& graph) {

    // work mostly with the down diagonal adjacency matrix, so that
    // the first edge iterators of each row with higher probability 
    // point to a valid edge.


    int l1,l2,a,b;
    point p;

    // create the huge adjacency matrix 
    for (p.y=0;p.y<regions.lastRow();++p.y) {
      for (p.x=0;p.x<regions.lastColumn();++p.x) {
        // update node's data
        a=regions.at(p);
        considerForNodeData(p,a,graph.getNodeData(a));

        if (a != (b=regions.at(p.y,p.x+1))) {
          minmax(a,b,l1,l2);
          if (l2 >= minLabel) {
            considerForEdgeData(p,point(p.x+1,p.y),
                                graph.forceTopologicalEdge(l2,l1));
          }
        }

        if (a != (b=regions.at(p.y+1,p.x))) {
          minmax(a,b,l1,l2);
          if (l2 >= minLabel) {
            considerForEdgeData(p,point(p.x,p.y+1),
                                graph.forceTopologicalEdge(l2,l1));
          }
        }
      }

      // last column
      a=regions.at(p);
      considerForNodeData(p,a,graph.getNodeData(a));

      if (a != (b=regions.at(p.y+1,p.x))) {
        minmax(a,b,l1,l2);
        if (l2 >= minLabel) {
            considerForEdgeData(p,point(p.x,p.y+1),
                                graph.forceTopologicalEdge(l2,l1));
        }
      }
    }

    // last row
    for (p.x=0;p.x<regions.lastColumn();++p.x) {
      a=regions.at(p);
      considerForNodeData(p,a,graph.getNodeData(a));

      if (a != (b=regions.at(p.y,p.x+1))) {
        minmax(a,b,l1,l2);
        if (l2 >= minLabel) {
          considerForEdgeData(p,point(p.x+1,p.y),
                              graph.forceTopologicalEdge(l2,l1));
        }
      } 
    }

    // last pixel
    a=regions.at(p);
    considerForNodeData(p,a,graph.getNodeData(a));

    // ------------------------------------------------------------------------
    
    // The adjacency matrix is ready with all preliminary node and edge
    // data.
    // Before we continue computing the edge weights, a few pre-computations
    // can be necessary:
    if (!prepareEdgeAndNodeData(graph)) {
      return false;
    }

    // ------------------------------------------------------------------------
    // save first the data in temporary vectors and construct the
    // whole queue at once, which is much much faster than inserting
    // each element one after another.

    if (edge_traits::Symmetric) {
      mirrorEdgeData(graph);
    }

    return graph.recomputeAllWeights();
  }
  
  // mirrorEdgeData()
  template<class G>
  bool regionGraphFunctor<G>::mirrorEdgeData(graph_type& graph) const {
    typename graph_type::const_iterator nit,enit;
    typename graph_type::edge_iterator it,eit;
    typename graph_type::node_pair p;
    // for all nodes
    for (nit=graph.begin(),enit=graph.end();nit!=enit;++nit) {
      for (it=graph.begin(nit.id()),eit=graph.end(nit.id());it!=eit;++it) {
        p = graph.getEdge(it);
        if (p.second < p.first) {
          graph.getEdgeData(p.second,p.first) = *it;
        } else {
          break; // nothing more to do for this node
        }
      }
    }

    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::affinityMatrix(const graph_type& graph,
                                             matrix<weight_type>& affinity,
                                             const weight_type noEdgeValue
                                             ) const {

    typename graph_type::const_iterator nit,enit;
    typename graph_type::const_edge_iterator it,eit;
    typename graph_type::node_pair p;

    const int size = graph.lastValidId()+1;
    affinity.resize(size,size,noEdgeValue,false,true); // init with zero
    weight_type w;
    
    // for all nodes
    for (nit=graph.begin(),enit=graph.end();nit!=enit;++nit) {
      // for all edges
      for (it=graph.begin(nit.id()),eit=graph.end(nit.id());it!=eit;++it) {
        p = graph.getEdge(it);   // get pair of nodes
        if (!edge_traits::Symmetric ||  (p.second < p.first)) {
          // still edges with data:
          w = graph.getEdgeWeight(it);
          affinity.at(p.first,p.second) = w;
          if (edge_traits::Symmetric) {
            affinity.at(p.second,p.first) = w;
          }
        } else {
          // end of relevant data
          break; 
        }
      }
    }
    
    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::merge(const weight_type& threshold,
                                    graph_type& graph,
                                    ivector& equivalences) const {
    const int lastLabel = graph.lastValidId();
    const parameters& par = getParameters();
    equivalences.resize(lastLabel+1,lastLabel+1,false,true);
    weight_type w;
    node_pair p;
    typename graph_type::id_type surviver;

    // this takes O(n) time!
    int numNodes = graph.totalAdjacentNodes();

    if (par.mergeMode == parameters::Fast) {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) &&
             (numNodes > par.minRegionNumber)) {
        surviver = graph.topologicalMerge(p);
        numNodes--;
        if (surviver == p.first) {
          // p.first kept
          equivalences.at(p.second)=p.first;
        } else {
          // p.second kept
          equivalences.at(p.first)=p.second;
        }      
      }
    } else {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) &&
             (numNodes > par.minRegionNumber)) {
        surviver = graph.mergeNodes(p);
        numNodes--;
        if (surviver == p.first) {
          // p.first kept
          equivalences.at(p.second)=p.first;
        } else {
          // p.second kept
          equivalences.at(p.first)=p.second;
        }
      }
    }

    // fix the equivalences vector
    int i,j;
    for (i=0;i<equivalences.size();++i) {
      j=i;
      while (equivalences.at(j)<j) {
        j=equivalences.at(j);
      }
      equivalences.at(i)=j;
    }

    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::merge(const weight_type& threshold,
                                    const int minLabel,
                                    graph_type& graph,
                                    ivector& equivalences) const {

    const int lastLabel = graph.lastValidId();
    const parameters& par = getParameters();
    int i,j;
    weight_type w;
    node_pair p;
    typename graph_type::id_type surviver;

    // this takes O(n) time!
    int numNodes = graph.totalAdjacentNodes();

    equivalences.resize(lastLabel+1,lastLabel+1,false,false);
    for (i=0;i<equivalences.size();++i) {
      equivalences.at(i) = i;
    }

    if (par.mergeMode == parameters::Fast) {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) && 
             (numNodes > par.minRegionNumber)) {
        if (max(equivalences.at(p.first),
                equivalences.at(p.second)) >= minLabel) {
          _lti_debug("Merging " << p.first << " and " << p.second << 
                     " (dist= " << w << " )" << std::endl);
          // if at least one label is not in the protected node heap merge
          // the nodes
          surviver = graph.topologicalMerge(p);
          numNodes--;
        
          if (surviver == p.first) {
            // p.first kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.second) = surviver;
          } else {
            // p.second kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.first)=surviver;
          }
        } else {
          _lti_debug("Removing edge between " << p.first << " and " << 
                     p.second << " (dist= " << w << " )" << std::endl);
          // it seems that this edge would merge protected nodes: remove the 
          // edge
          graph.removeEdge(p);
        }
      }
    } else {
      while (graph.getLowestWeightEdge(p,w) && 
             (w <= threshold) && 
             (numNodes > par.minRegionNumber)) {
        if (max(equivalences.at(p.first),
                equivalences.at(p.second)) >= minLabel) {
          _lti_debug("Merging " << p.first << " and " << p.second << 
                     " (dist= " << w << " )" << std::endl);
          // if at least one label is not in the protected node heap merge
          // the nodes
          surviver = graph.mergeNodes(p);
          numNodes--;
        
          if (surviver == p.first) {
            // p.first kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.second) = surviver;
          } else {
            // p.second kept
            while (equivalences.at(surviver) < surviver) {
              surviver = equivalences.at(surviver);
            }

            equivalences.at(p.first)=surviver;
          }
        } else {
          _lti_debug("Removing edge between " << p.first << " and " << 
                     p.second << " (dist= " << w << " )" << std::endl);
          // it seems that this edge would merge protected nodes: remove the 
          // edge
          graph.removeEdge(p);
        }
      }
    }

    // fix the equivalences vector
    for (i=0;i<equivalences.size();++i) {
      j=i;
      while (equivalences.at(j)<j) {
        j=equivalences.at(j);
      }
      equivalences.at(i)=j;
    }

    return true;
  }


  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                             const imatrix& regions,
                                                   imatrix& newRegions,
                                             const bool compact) const {

    if (regions.empty()) {
      setStatusString("Labeled mask was empty.");
      return false;
    }

    ivector labels;
    const ivector* theLabels = 0;
    if (compact) {
      compactLabels(equivalences,labels);
      theLabels = &labels;
    } else {
      theLabels = &equivalences;
    }


    newRegions.resize(regions.size(),int(),false,false);
    int y,x;
    for (y=0;y<regions.rows();++y) {
      for (x=0;x<regions.columns();++x) {
        newRegions.at(y,x) = theLabels->at(regions.at(y,x));
      }
    }

    return true;
  }

  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                                   imatrix& regions,
                                                   ivector& regionSizes,
                                             const bool compact) const {
    imatrix tmp;
    if (reassignLabels(equivalences,regions,tmp,regionSizes,compact)) {
      tmp.detach(regions);
      return true;
    }
    return false;
  }
  
  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                             const imatrix& regions,
                                                   imatrix& newRegions,
                                                   ivector& regionSizes,
                                             const bool compact) const {

    if (regions.empty()) {
      setStatusString("Labeled mask was empty.");
      return false;
    }

    int size;
    ivector labels;
    const ivector* theLabels = 0;
    if (compact) {
      size = compactLabels(equivalences,labels);
      theLabels = &labels;
      
    } else {
      theLabels = &equivalences;
      size = equivalences.size();
    }

    regionSizes.resize(size,0,false,true);

    newRegions.resize(regions.size(),int(),false,false);
    int y,x,i;
    for (y=0;y<regions.rows();++y) {
      for (x=0;x<regions.columns();++x) {
        i=theLabels->at(regions.at(y,x));
        newRegions.at(y,x) = i;
        regionSizes.at(i)++;
      }
    }

    return true;
  }


  template<class G>
  bool regionGraphFunctor<G>::reassignLabels(const ivector& equivalences,
                                                   imatrix& regions,
                                             const bool compact) const {
    
    imatrix tmp;
    if (reassignLabels(equivalences,regions,tmp,compact)) {
      tmp.detach(regions);
      return true;
    }
    
    return false;
  }

  template<class G>
  int regionGraphFunctor<G>::compactLabels(const ivector& equivalences,
                                           ivector& newEquivalences) const {
    
    // ------------------------------------
    // use only as many labels as necessary
    // ------------------------------------
    ivector usedLabels(equivalences.size(),-1);
    int i;

    // which labels are used?
    for (i=0;i<equivalences.size();++i) {
      usedLabels.at(equivalences.at(i))=1;
    }

    // reassign labels
    int l1=0;
    for (i=0;i<usedLabels.size();++i) {
      if (usedLabels.at(i)>0) {
        usedLabels.at(i)=l1;
        l1++;
      }
    }
    
    newEquivalences.resize(equivalences.size(),0,false,false);
    

    // now equivalences must be updated
    for (i=0;i<equivalences.size();++i) {
      newEquivalences.at(i)=usedLabels.at(equivalences.at(i));
    }
    
    return l1;
 }
}

#include "ltiUndebug.h"
