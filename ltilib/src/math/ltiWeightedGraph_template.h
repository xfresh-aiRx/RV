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


/*----------------------------------------------------------------
* project ....: LTI Digital Image/Signal Processing Library
* file .......: ltiWeightedGraph_template.h
* authors ....: Jens Paustenbach
* organization: LTI, RWTH Aachen
* creation ...: 25.04.2003
* revisions ..: $Id: ltiWeightedGraph_template.h,v 1.15 2005/07/29 11:41:01 haehnel Exp $
*/

#include <map>
#include "ltiDraw.h"

namespace lti {

    template<class P, class D, class W>
    weightedGraph<P,D,W>::weightedGraph() {
      nodeCounter=0;
    };

    template<class P,class D,class W>
    weightedGraph<P,D,W>::weightedGraph(const weightedGraph<P,D,W>& other) {
      copy(other);
    };
   
    template<class P,class D,class W>
    weightedGraph<P,D,W>::~weightedGraph(){
      clear();
    };
   
    template<class P,class D,class W>
    weightedGraph<P,D,W>& 
    weightedGraph<P,D,W>::copy(const weightedGraph<P,D,W>& other) {
      nodes=other.getNodes();
      edges=other.getEdges();
      nodeCounter=other.nodeCounter;
      return *this;
    };
   
    template<class P,class D,class W>
    mathObject* weightedGraph<P,D,W>::clone() const {
      weightedGraph<P,D,W>* tmp = new weightedGraph<P,D,W>(*this);
      return tmp;
    }

    template<class P,class D,class W>
    const char* weightedGraph<P,D,W>::getTypeName() const {
      return "weightedGraph";
    };

    template<class P,class D,class W>
    int weightedGraph<P,D,W>::insertNode(const D& data, const P& point) {
      int newId = nodeCounter++;
      node* tmp=new node(point,data,newId);
      nodes[newId]=tmp;
      return newId;
    };
   
    template<class P,class D,class W>
    void weightedGraph<P,D,W>::insertNode(int id,const D& data,const P& point){
      if (nodes.find(id) == nodes.end()) {
        node* tmp=new node(point,data,id);
        nodes[id]=tmp;
        if (id > nodeCounter)
          nodeCounter = id + 1;
        else
          ++nodeCounter;
      }
    };
   
    template<class P,class D,class W>
    void weightedGraph<P,D,W>::insertEdge(const int idA,const int idB,
                                          const W& weight) {
      if (edges.find(edge(idA,idB))==edges.end()) {
        edges[edge(idA,idB)]=weight;
        edges[edge(idB,idA)]=weight;
        nodes[idA]->connect(idB);
        nodes[idB]->connect(idA);
      }
    };
   
    template<class P,class D,class W>
    void weightedGraph<P,D,W>::setWeight(const int idA, const int idB,
                                       const W& weight) {
      if (edges.find(edge(idA,idB))!=edges.end()) {
        edges[edge(idA,idB)]=weight;
        edges[edge(idB,idA)]=weight;
      }
    };
    template<class P,class D,class W>
    void weightedGraph<P,D,W>::setData(const int idA, const D& data) {
      nodes[idA]->setData(data);
    };

    template<class P,class D,class W>
    bool weightedGraph<P,D,W>::getData(const int id, D& data) const {
      node* nodePtr;
      if (!getNode(id, nodePtr))
        return false;

      data = nodePtr->getData();
      return true;
    }

    template<class P,class D,class W>
    void weightedGraph<P,D,W>::setPoint(const int idA, const P& pos) {
      nodes[idA]->setPoint(pos);
    };

    template<class P,class D,class W>
    bool weightedGraph<P,D,W>::getPoint(const int id, P& pos) const {
      node* nodePtr;
      if (!getNode(id, nodePtr))
        return false;

      pos = nodePtr->getPoint();
      return true;
    }

    template<class P,class D,class W>
    void weightedGraph<P,D,W>::removeEdge(const int idA, const int idB) {
      nodes[idA]->disconnect(idB);
      nodes[idB]->disconnect(idA);
      edges.erase(edge(idA,idB));
      edges.erase(edge(idB,idA));
    };

    template<class P,class D,class W> 
    bool weightedGraph<P,D,W>::removeNode(int id) {
      std::list<int> a; // collect all edges from this node
      a=nodes[id]->getConnections();
      std::list<int>::iterator it;
      for (it=a.begin(); it!=a.end(); it++) {
        removeEdge((*it),id);
      }
      nodes.erase(id);
      return true;
    };

    template<class P,class D,class W> 
    bool weightedGraph<P,D,W>::getNode(int id, node* &nodePtr) const {
      
      typename std::map<int, node*>::const_iterator it = nodes.find(id);
      if (it == nodes.end())
        return false;

      nodePtr = it->second;
      return true;
    }

    template<class P,class D,class W> 
    void weightedGraph<P,D,W>::clear() {
      typename std::map<int,node*>::iterator it;
      for (it=nodes.begin(); it!=nodes.end(); it++) {
        delete (it->second);
      }
      nodes.clear();
      edges.clear();
    }

    template<class P,class D,class W> 
    bool weightedGraph<P,D,W>::empty() const {
      return (nodes.empty() && edges.empty());
    }

    template<class P,class D,class W> 
    void weightedGraph<P,D,W>::clearEdges() {
      typename std::map<int,node*>::iterator it;
      for (it=nodes.begin(); it!=nodes.end(); it++) {
        it->second->disconnect();
      }
      edges.clear();
    }

  // again a nasty work-around for MSC
#if defined(_LTI_MSC_6) 
    template<class P, class D, class W>
    const std::map<int, weightedGraph<P,D,W>::node*>&
    weightedGraph<P,D,W>::getNodes() const {
      return nodes;
    }
#else
    template<class P, class D, class W>
    const std::map<int, typename weightedGraph<P,D,W>::node*>&
    weightedGraph<P,D,W>::getNodes() const {
      return nodes;
    }
#endif

    template<class P,class D,class W>
    bool weightedGraph<P,D,W>::write(ioHandler& handler, 
                                   const bool complete) const {
        bool b(true);
        if (complete) {
          b = handler.writeBegin();
        }

        b = b && lti::write(handler, "numNodes", nodes.size());
        b = b && lti::write(handler, "numEdges", edges.size());
        typename std::map<int,node*>::const_iterator it;
        for (it=nodes.begin(); it!=nodes.end(); it++) {
          lti::write(handler,"id",it->first);
          lti::write(handler,"data",it->second->getData());
          lti::write(handler,"point",it->second->getPoint());
          lti::write(handler,"connections",it->second->getConnections());
        }
        typename std::map<std::pair<int,int>,W>::const_iterator it2;
        for (it2=edges.begin(); it2!=edges.end(); it2++) {
          lti::write(handler,"edge",it2->first);
          lti::write(handler,"weight",it2->second);
        }

        if (complete) {
          b = b && handler.writeEnd();
        }

        return b;
    }

    template<class P,class D,class W>
    bool weightedGraph<P,D,W>::read(ioHandler& handler, const bool complete) {
        bool b(true);
        if (complete) {
            b = handler.readBegin();
        }

        int numNodes,numEdges;
        b = b && lti::read(handler,"numNodes",numNodes);
        b = b && lti::read(handler,"numEdges",numEdges);
        int i,id;
        D data;
        P point;
        std::list<int> con;
        node* tmp;
        for (i=0; i<numNodes; i++) {
            lti::read(handler,"id",id);
            lti::read(handler,"data",data);
            lti::read(handler,"point",point);
            lti::read(handler,"connections",con);
            tmp = new node(point,data,id);
            tmp->setConnections(con);
            nodes[id]=tmp;
        }
        std::pair<int,int> theEdge;
        W weight;
        for (i=0; i<numEdges; i++) {
            lti::read(handler,"edge",theEdge);
            lti::read(handler,"weight",weight);
            edges[theEdge]=weight;
        }

        if (complete) {
            b = b && handler.readEnd();
        }
        return b;
    }



    /////////////////////////////////////////////////////////////////
    //   CLASS NODE
    ///////////////////////////////////////////////////////////////////


    template<class P,class D,class W>
    void weightedGraph<P,D,W>::show(image& canvas) {
        draw<rgbPixel> drawer;
        drawer.use(canvas);
        typename std::map<int,node*>::iterator nodeIt;
        for (nodeIt=nodes.begin(); nodeIt!=nodes.end(); nodeIt++) {
          drawer.marker(static_cast<int>(nodeIt->second->getPoint().at(0)*35+20), static_cast<int>(nodeIt->second->getPoint().at(1)*35+20),"or");
        }
        typename std::map<std::pair<int,int>,W>::iterator edgeIt;
        drawer.setColor(lti::Green);
        for (edgeIt=edges.begin(); edgeIt!=edges.end(); edgeIt++) {
            drawer.line(static_cast<int>(nodes[edgeIt->first.first]->getPoint().at(0)*35+20),
                        static_cast<int>(nodes[edgeIt->first.first]->getPoint().at(1)*35+20),
                        static_cast<int>(nodes[edgeIt->first.second]->getPoint().at(0)*35+20),
                        static_cast<int>(nodes[edgeIt->first.second]->getPoint().at(1)*35+20));
        }
    }

}
