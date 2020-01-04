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
* file .......: ltiWeightedGraph.h
* authors ....: Jens Paustenbach
* organization: LTI, RWTH Aachen
* creation ...: 25.04.2003
* revisions ..: $Id: ltiWeightedGraph.h,v 1.11 2005/06/16 12:37:35 doerfler Exp $
*/

#ifndef _LTI_WEIGHTED_GRAPH_H_
#define _LTI_WEIGHTED_GRAPH_H_

#include "ltiImage.h"
#include "ltiSTLIoInterface.h"

#include <map>
#include <list>

namespace lti {
  /**
  * Weighted Graph container class.
  * This class allows the  representation of weighted undirected graphs.
  * This class is implemented as template with tree template arguments.
  * The first P is the point type, that describes the position of the node.
  * This can be a e.g. a dvector or a rgbPixel. Second parameter is D.
  * This is the data type of the data a node contains, and third W is 
  * the type of the weights of the edges between the nodes.
  *
  * The following code will build this graph:
  *
  * \code
  * #include "ltiWeightedGraph.h"
  * #include "ltiL2Distance.h"
  *
  * double a[]={ 10,10, 12,12, 11,12, 11,11, 10,11,
  *               0, 0,  1, 0,  2, 0,  1, 1,  2, 1,
  *              10, 0, 12, 0, 11, 0, 11, 1, 10, 1}; 
  *
  * dmatrix data;
  * data.useExternData(15,2,a); // don't copy the data, but use it!
  * 
  * l2Distantor<double> distantor;
  * weightedGraph<dvector,int,l2Distantor<double>::distance_type> myGraph;
  * 
  * int i,j;
  * for (i=0; i<data.rows(); i++) {
  *   graph.insertNode(data.getRow(i),i);
  * }
  *
  * for (i=0; i<data.rows(); i++) {
  *   for (i=i+1; j<data.rows(); j++) {
  *     graph.insertEdge(i,j,distantor(data.getRow(i),data.getRow(j)));
  *   }
  * }
  * \endcode
  */
  template<class P, class D=int, class W=double>
  class weightedGraph : public mathObject {
  public:

    /**
     * the elements of the graph are instances of this class
     */
    class node {
    public:

      /** 
       * standard constructor
       */
      node() {
        data = D(0);
        point = P(0);
        id=0;
      }

      /** 
       * copy constructor
       */
      node(const node& other) { copy(other); }

      /**
       * constructor
       */
      node(const P& position, const D& value, const int nb) {
        data = value;
        point = position;
        id = nb;
      }

      /** 
       * destructor
       */
      ~node() { }

      /** 
       * copy the contents of other into this object
       */
      node& copy(const node& other) {
        data = other.getData();
        point = other.getPoint();
        id = other.getId();
        return *this;
      }

      //
      // data methods

      /** 
       * sets the data that the node contains
       */
      void setData(const D& value) { data=value; }

      /** 
       * returns the data, saved in the node
       */
      D& getData() { return data; }

      /** 
       * returns the data, saved in the node
       */
      const D& getData() const { return data; }

      //
      // point methods

      /**
       * sets the position of the node
       */
      void setPoint(const P& pos) { point = pos; }

      /** 
       * returns the data, saved in the node
       */
      P& getPoint() { return point; }

      /** 
       * returns the data, saved in the node
       */
      const P& getPoint() const { return point; }


      /** 
       * returns the id of this node
       */
      int getId() const { return id; }

      /** 
       * connects this node with an other node with the given id
       */
      void connect(const int id) {
        connections.push_back(id); 
      }

      /** 
       * disconnects this node with an other node with the given id
       */
      void disconnect(const int id) {
        connections.remove(id);
      }

      /** 
       * disconnects this node from all other nodes.
       */
      void disconnect() {
        connections.clear();
      }

      /** 
       * returns a list with the ids of all with this connected nodes.
       */
      const std::list<int>& getConnections() const {
        return connections; 
      }

      /** 
       * Sets the list with the ids of nodes this node is connected to.
       */
      void setConnections(std::list<int>& con) {
        connections=con; 
      }

    protected:

      /** 
       * the data that the node contains
       */
      P point;

      /** 
       * the data that the node contains
       */
      D data;

      /** 
       * list with the ids, of all connected nodes
       */
      std::list<int> connections;

      /** 
       * the id of this node
       */
      int id;
    }; // class node


    //
    // data types
    //

    /**
     * type of the data in the nodes
     */
    typedef D value_type;

    /**
     * type of the point in the nodes
     */
    typedef P point_type;

    /**
     * type of the weight of the edges
     */
    typedef W weight_type;

    /**
     * An edge is identified with the ids of the nodes it connects.
     */
    typedef std::pair<int,int> edge;
        


    //
    // constructors
    //

    /**
     * default constructor creates an emppty graph;
     */
    weightedGraph();

    /**
     * create this graph as a copy of another graph
     * @param other the tree to be copied.
     */
    weightedGraph(const weightedGraph<P,D,W>& other);

    /**
     * destructor
     */
    virtual ~weightedGraph();



    //
    // inherited methods
    //

    /**
     * returns the name of this class: "weightedGraph"
     */
    const char* getTypeName() const;

    /**
     * assigment operator.
     * copy the contents of <code>other</code> in this %object.
     * @param other the source graph to be copied.
     */
    weightedGraph<P,D,W>& copy(const weightedGraph<P,D,W>& other);


    /**
     * create a clone of this graph
     * @return a pointer to a copy of this graph
     */
    virtual mathObject* clone() const;



    //
    // general graph methods
    //

    /**
     * returns true if the graph is empty
     */
    bool empty() const;

    /**
     * Clears the graph. All nodes and edges will be removed
     */
    void clear();
 
   /**
    * Clears the Edges of the graph. All nodes will be unconnected
    */
    void clearEdges();



    //
    // node methods
    //

    /**
     * Inserts a node that contains the given data into the tree
     * Returns the ID of the new node.
     */
    int insertNode(const D& data,const P& point);

    /**
     * Inserts a node with the specified id and data in the tree
     */
    void insertNode(int id, const D& data, const P& point);

    /**
     * Removes node with given id. All edges from an to this node will also 
     * be removed.
     */
    bool removeNode(int id);

    /**
     * Fetch pointer to the specified node.
     * Return true, if successful.
     */
    bool getNode(int id, node* &nodePtr) const;

    /**
     * Returns the map with the nodes. As key the id of the node is used.
     */
    const std::map<int,node*>& getNodes() const;



    //
    // high level access methods
    //

    /**
     * Fills the node with id with the given data
     */
    void setData(const int id, const D& data);

    /**
     * Fetch data from the specified node
     * Returns false, if node doesn't exist, true otherwise
     */
    bool getData(const int id, D& data) const;

    /**
     * Set position of a single node
     */
    void setPoint(const int id, const P& pos);


    /**
     * Fetch position of the specified node
     * Returns false, if node doesn't exist, true otherwise
     */
    bool getPoint(const int id, P& pos) const;


    /**
     * Sets the weight of the edge between node with idA and node with idB.
     */
    void setWeight(const int idA, const int idB,const W& weight);

    /**
     * Returns the weight of the edge between node with idA and node with idB.
     */
    W getWeight(const int idA, const int idB) const {
      // implementation needs to be inline, due to visual c++ template deficiencies
      typename std::map<edge, W>::const_iterator it = 
        edges.find(edge(idA,idB));
      if (it!=edges.end()) {
        return it->second;
      } else {
        return W(0);
      }
    };



    //
    // edge methods
    //

    /**
     * Inserts an edge between node with idA and node with idB into the tree.
     */
    void insertEdge(const int idA,const int idB, const W& weight);

    /**
     * removes the edge between nodes with ids idA and idB
     */
    void removeEdge(const int idA, const int idB);

    /**
     * check, wether two nodes are adjacent (connected)
     */
    bool adjacentNodes(const int idA, const int idB) const {
      return (edges.find(edge(idA,idB)) != edges.end());
    }

    /**
     * Returns the map with the edges of the graph.
     */
    const std::map<edge,W> getEdges() const {  return edges; }



    /**
     * write the object in the given ioHandler
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

    /**
     * read the object from the given ioHandler
     */    
    virtual bool read(ioHandler& handler,const bool complete=true);

    /**
     * 
     */
    void show(image& canvas);



  private:

    /**
     * map with pointers to all nodes in the graph
     */
    std::map<int,node*> nodes;

    /**
     * map with the ids of the connected nodes and the weight of the edge
     */
    std::map<edge,W> edges;

    /**
     * a counter for the id of the nodes
     */
    int nodeCounter;


};

}
#include "ltiWeightedGraph_template.h"

#endif
 
