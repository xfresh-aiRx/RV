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
 * file .......: ltiDelaunayTriangulation.cpp
 * authors ....: Daniel Beier
 * organization: LTI, RWTH Aachen
 * creation ...: 7.5.2003
 * revisions ..: $Id: ltiDelaunayTriangulation_template.h,v 1.9 2005/06/06 09:02:47 doerfler Exp $
 */

//TODO: include files
#include "ltiDelaunayTriangulation.h"

namespace lti {
 
  // --------------------------------------------------
  // delaunayTriangulation
  // --------------------------------------------------

  // default constructor
  template <class T>
  delaunayTriangulation<T>::delaunayTriangulation()
    : functor(){

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // create an instance of the parameters with the default values
    parameters defaultParameters;
    // set the default parameters
    setParameters(defaultParameters);


  }

  // default constructor
  template <class T>
  delaunayTriangulation<T>::delaunayTriangulation(const parameters& par)
    : functor() {

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    // set the given parameters
    setParameters(par);
  }


  // copy constructor
  template <class T>
  delaunayTriangulation<T>::delaunayTriangulation(const delaunayTriangulation& other)
    : functor() {
    copy(other);
  }

  // destructor
  template <class T>
  delaunayTriangulation<T>::~delaunayTriangulation() {
  }

  // returns the name of this type
  template <class T>
  const char* delaunayTriangulation<T>::getTypeName() const {
    return "delaunayTriangulation";
  }

  // copy member
  template <class T>
  delaunayTriangulation<T>&
    delaunayTriangulation<T>::copy(const delaunayTriangulation<T>& other) {
      functor::copy(other);

    //TODO: comment the attributes of your functor
    // If you add more attributes manually, do not forget to do following:
    // 1. indicate in the default constructor the default values
    // 2. make sure that the copy member also copy your new attributes, or
    //    to ensure there, that these attributes are properly initialized.

    return (*this);
  }

  // alias for copy member
  template <class T>
  delaunayTriangulation<T>&
    delaunayTriangulation<T>::operator=(const delaunayTriangulation<T>& other) {
    return (copy(other));
  }


  // clone member
  template <class T>
  functor* delaunayTriangulation<T>::clone() const {
    return new delaunayTriangulation<T>(*this);
  }

  // return parameters
  template <class T>
  const typename delaunayTriangulation<T>::parameters&
  delaunayTriangulation<T>::getParameters() const {
    const parameters* par =
      dynamic_cast<const parameters*>(&functor::getParameters());
    if(isNull(par)) {
      throw invalidParametersException(getTypeName());
    }
    return *par;
  }


  template <class T>
  void delaunayTriangulation<T>::initialize(const std::vector<tpoint<T> >& src, std::vector<int>& triangles) {
  
    //Calculate special points p_1, p_2, p_3 that are needed for the algorithm, but will be discarded at the end.
    calcSpecialPoints(src);

    //initialize "triangles" with one single triangle containing of the three dummy points and update adjacency matrix
    triangles.clear();
    triangles.push_back(-3);
    triangles.push_back(-2);
    triangles.push_back(-1);

    //Initialize the directed acyclic graph (DAG) that helps locating the triangle that contains a given point
    dagRoot = new dagNode;
    dagRoot->pointIndex[0] = -3;
    dagRoot->pointIndex[1] = -2;
    dagRoot->pointIndex[2] = -1;
    dagRoot->triVecIndex = 0;
    dagRoot->children.clear();

    //initialize dagAccessList:
    //this means e.g. that point p-1 can be found in the (current) leaf node dagRoot in the DAG.
    dagAccessList.resize(src.size()+3);
    dagAccessList[0].push_back(dagRoot);
    dagAccessList[1].push_back(dagRoot);
    dagAccessList[2].push_back(dagRoot);

    dagEraseList.clear();
    dagEraseList.push_back(dagRoot);

  }



  // -------------------------------------------------------------------
  // The apply-methods!
  // -------------------------------------------------------------------

  
  // On place apply for type std::vector<tpoint<T> >!
  template <class T>
  bool delaunayTriangulation<T>::apply(const std::vector<tpoint<T> >& src, 
                                       std::vector<int>& triangles) {
  
    //we need at least three points
    if (src.size() < 3)
      return false;

    //initialize: calculate specialpoints, init the triangle-vector,
    //the DAG and the dagAccessList
    initialize(src, triangles);

    //iterators for the DAG and the child nodes
    dagNode* itDAG;
    typename std::list<dagNode*>::iterator itChildren;
    int onEdge;
    //Due to rounding errors and the problem of the discretization of
    //lines it might happen that a point is not found in any triangle
    //if it lies very close to an edge. In this case the point will be
    //assigned to the triangle that contained the edge with the
    //minimum distance to the point.
    dagNode* nearestTriangle = 0;
    T minDist(0), dist(0);
    
    unsigned int i;
    //for every point of src find triangle containing point src[i]
    for (i=0; i<src.size(); ++i) {

      //start at root of DAG
      itDAG = dagRoot;
      onEdge = 0;//(1 > insideTest(i, src, dagRoot->pointIndex[0], dagRoot->pointIndex[1], dagRoot->pointIndex[2]));

      //iterate through DAG until we have reached a leaf node
      while (itDAG->children.size() > 0) {

        //find child node(s) containing point
        itChildren = itDAG->children.begin();
        while (itChildren != itDAG->children.end()) {
          
          onEdge = -1 + insideTest(i, src, (*itChildren)->pointIndex[0], (*itChildren)->pointIndex[1], (*itChildren)->pointIndex[2], dist);

          //point in the interior of triangle or on an edge
          if (onEdge >= 0)
            break;

          //not in triangle, in case the point won't be found in any triangle store infos so it can be assigned to
          //the "best" triangle later on
          if (itChildren == itDAG->children.begin() || dist < minDist) {
            minDist = dist;
            nearestTriangle = (*itChildren);
          }
          ++itChildren;

        }

        //descend in the DAG
        //if the node wasn't inside any triangle (due to rounding error) assign it to the best match
        if (itChildren == itDAG->children.end()) {
          onEdge = 0;
          itDAG = nearestTriangle;
        }
        else        
          itDAG = (*itChildren);

      }


      //if point lies in the interior of a triangle then split this triangle into three new ones as follows...
      if (!onEdge) {

        //memorize old triangle-points for later legalize-operations
        int pos = itDAG->triVecIndex;
        int oldP1 = triangles[pos];
        int oldP2 = triangles[pos+1];
        int oldP3 = triangles[pos+2];

        //split including updating of triangle-vector, DAG and dagAccessList
        split1to3(src, triangles, i, itDAG);

        legalizeEdge(src, i, oldP1, oldP2, triangles);
        legalizeEdge(src, i, oldP2, oldP3, triangles);
        legalizeEdge(src, i, oldP3, oldP1, triangles);
        
      }
      //the new point lies on an edge (not on a point!) -> split two triangles into four
      else if (onEdge != 4) {
        
        //memorize old triangle-points for later legalize-operations
        // int pos = itDAG->triVecIndex;
        
        //determine the edge that the new point falls onto
        int p1(0), p2(0), k(0);
        if (onEdge == 1) {
          p1 = itDAG->pointIndex[0];
          p2 = itDAG->pointIndex[1];
          k  = itDAG->pointIndex[2];
        }
        else if (onEdge == 2) {
          p1 = itDAG->pointIndex[1];
          p2 = itDAG->pointIndex[2];
          k  = itDAG->pointIndex[0];
        }
        else if (onEdge == 3) {
          p1 = itDAG->pointIndex[2];
          p2 = itDAG->pointIndex[0];
          k  = itDAG->pointIndex[1];
        }


        //find the second triangles in the DAG with the edge p1,p2
        //using the dagAccessList (this is where this structure is
        //really needed)
        dagNode* tri = NULL;
        typename std::list<dagNode*>::iterator itAL1, itAL2;
        for (itAL1 = dagAccessList[p1+3].begin(); 
             itAL1 != dagAccessList[p1+3].end(); ++itAL1)
          for (itAL2 = dagAccessList[p2+3].begin();
               itAL2 != dagAccessList[p2+3].end(); ++itAL2) {
            //pointers to the same triangle that is not the one we
            //already found
            if ((*itAL1) == (*itAL2) && (*itAL1) != itDAG)
              tri = (*itAL1);
          }

        //the third point of the found triangle
        int l = -1;
        for (int j=0; j<3; ++j)
          if (tri->pointIndex[j] != p1 && tri->pointIndex[j] != p2)
            l = tri->pointIndex[j];

        //split including updating of triangle-vector, DAG and dagAccessList
        split2to4(src, triangles, i, p1, p2, k, l, itDAG, tri);

        legalizeEdge(src, i, p1, l, triangles);
        legalizeEdge(src, i, l, p2, triangles);
        legalizeEdge(src, i, p2, k, triangles);
        legalizeEdge(src, i, k, p1, triangles);
      
      }
    
    }//for (i=0; i<src.size(); ++i)

    
    //remove special points and their incident edges
    std::vector<int>::iterator itTri;
    for (itTri = triangles.begin(); itTri != triangles.end(); ) {
      if ((*itTri) < 0) {
        //maybe not the most efficient way
        itTri = triangles.erase(itTri);
        itTri = triangles.erase(itTri);
        itTri = triangles.erase(itTri);
      }
      else {
        ++itTri;
        ++itTri;
        ++itTri;
      }
    }

    //free memory that was used by DAG
    deleteDAG();

    return true;
  };



  //splitting one triangle into three when point r (inside of triangle) is inserted
  template <class T>
  void delaunayTriangulation<T>::split1to3(const std::vector<tpoint<T> >& src, std::vector<int>& triangles, int r, dagNode* leaf) {

    //get the position of the triangle in the std::vector<...>
    int pos = leaf->triVecIndex;
    dagNode* newNode;

    int oldP1 = triangles[pos];
    int oldP2 = triangles[pos+1];
    int oldP3 = triangles[pos+2];
    
    //overwrite exisiting triangle in triangle-vector,
    //note: r is greater than any other element in the list, so the list maintains sorted (every
    //triangle in the list starts with the point that has the smallest index).
    triangles[pos+2] = r;
    
    newNode = new dagNode;
    //note: oldP1 < oldP2 < r
    //update DAG and dagAccessList
    newNode->pointIndex[0] = oldP1;
    newNode->pointIndex[1] = oldP2;
    newNode->pointIndex[2] = r;
    newNode->triVecIndex = pos;
    leaf->children.push_back(newNode);

    dagAccessList[r+3].push_back(newNode);
    //overwrite itDAG with newNode
    typename std::list<dagNode*>::iterator it = dagAccessList[oldP1+3].begin();
    while ((*it) != leaf)
      ++it;
    (*it) = newNode;
    it = dagAccessList[oldP2+3].begin();
    while ((*it) != leaf)
      ++it;
    (*it) = newNode;

    dagEraseList.push_back(newNode);
    
    //new triangle
    //note: oldP2 < oldP3 < r
    triangles.push_back(oldP2);
    triangles.push_back(oldP3);
    triangles.push_back(r);
    
    //update DAG
    newNode = new dagNode;
    newNode->pointIndex[0] = oldP2;
    newNode->pointIndex[1] = oldP3;
    newNode->pointIndex[2] = r;
    newNode->triVecIndex = triangles.size()-3;//third to last position in vector
    leaf->children.push_back(newNode);

    //update dagAccessList
    dagAccessList[r+3].push_back(newNode);
    dagAccessList[oldP2+3].push_back(newNode);
    //overwrite itDAG with newNode
    it = dagAccessList[oldP3+3].begin();
    while ((*it) != leaf)
      ++it;
    (*it) = newNode;

    dagEraseList.push_back(newNode);

    //another new triangle
    //note: oldP1 < oldP3 < r
    triangles.push_back(oldP1);
    triangles.push_back(oldP3);
    triangles.push_back(r);
    
    newNode = new dagNode;
    newNode->pointIndex[0] = oldP1;
    newNode->pointIndex[1] = oldP3;
    newNode->pointIndex[2] = r;
    newNode->triVecIndex = triangles.size()-3;
    leaf->children.push_back(newNode);

    //update dagAccessList
    dagAccessList[r+3].push_back(newNode);
    dagAccessList[oldP1+3].push_back(newNode);
    dagAccessList[oldP3+3].push_back(newNode);

    dagEraseList.push_back(newNode);

  }


  /*
   * splitting two triangles into four when point r (on edge p1,p2) is inserted. k is assumed to be the third
   * point of the triangle p1,p2,k (defined by leaf1) and l the third point of p1,p2,l (->leaf2)
   */
  template <class T>
  void delaunayTriangulation<T>::split2to4(const std::vector<tpoint<T> >& src, std::vector<int>& triangles,
    int r, int p1, int p2, int k, int l, dagNode* leaf1, dagNode* leaf2) {

    //get the position of the triangle in the std::vector<...>
    int pos1 = leaf1->triVecIndex;
    int pos2 = leaf2->triVecIndex;
    dagNode* newNode;

    //1) split p1,p2,k into p1,r,k and p2,r,k

    //overwrite exisiting triangle (leaf1) in triangle-vector,
    //note: list must maintain sorted (every triangle in the list starts with the 
    //point that has the smallest index).
    int ind1 = p1, ind2 = r, ind3 = k;
    sort3(ind1, ind2, ind3);
    triangles[pos1]   = ind1;
    triangles[pos1+1] = ind2;
    triangles[pos1+2] = ind3;
        
    newNode = new dagNode;
    //update DAG and dagAccessList
    newNode->pointIndex[0] = ind1;
    newNode->pointIndex[1] = ind2;
    newNode->pointIndex[2] = ind3;
    newNode->triVecIndex = pos1;
    leaf1->children.push_back(newNode);

    dagAccessList[r+3].push_back(newNode);
    //overwrite itDAG with newNode
    typename std::list<dagNode*>::iterator it = dagAccessList[p1+3].begin();
    while ((*it) != leaf1)
      ++it;
    (*it) = newNode;
    it = dagAccessList[k+3].begin();
    while ((*it) != leaf1)
      ++it;
    (*it) = newNode;

    dagEraseList.push_back(newNode);
    
    //new triangle
    ind1 = p2, ind2 = r, ind3 = k;
    sort3(ind1, ind2, ind3);
    triangles.push_back(ind1);
    triangles.push_back(ind2);
    triangles.push_back(ind3);
    
    //update DAG
    newNode = new dagNode;
    newNode->pointIndex[0] = ind1;
    newNode->pointIndex[1] = ind2;
    newNode->pointIndex[2] = ind3;
    newNode->triVecIndex = triangles.size()-3;//third to last position in vector
    leaf1->children.push_back(newNode);

    //update dagAccessList
    dagAccessList[r+3].push_back(newNode);
    dagAccessList[k+3].push_back(newNode);
    //overwrite itDAG with newNode
    it = dagAccessList[p2+3].begin();
    while ((*it) != leaf1)
      ++it;
    (*it) = newNode;

    dagEraseList.push_back(newNode);

    //2) split p1,p2,l into p1,r,l and p2,r,l

    //overwrite exisiting triangle (leaf2) in triangle-vector,
    ind1 = p1, ind2 = r, ind3 = l;
    sort3(ind1, ind2, ind3);
    triangles[pos2]   = ind1;
    triangles[pos2+1] = ind2;
    triangles[pos2+2] = ind3;
        
    newNode = new dagNode;
    //update DAG and dagAccessList
    newNode->pointIndex[0] = ind1;
    newNode->pointIndex[1] = ind2;
    newNode->pointIndex[2] = ind3;
    newNode->triVecIndex = pos2;
    leaf2->children.push_back(newNode);

    dagAccessList[r+3].push_back(newNode);
    //overwrite itDAG with newNode
    it = dagAccessList[p1+3].begin();
    while ((*it) != leaf2)
      ++it;
    (*it) = newNode;
    it = dagAccessList[l+3].begin();
    while ((*it) != leaf2)
      ++it;
    (*it) = newNode;

    dagEraseList.push_back(newNode);
    
    //new triangle
    ind1 = p2, ind2 = r, ind3 = l;
    sort3(ind1, ind2, ind3);
    triangles.push_back(ind1);
    triangles.push_back(ind2);
    triangles.push_back(ind3);
    
    //update DAG
    newNode = new dagNode;
    newNode->pointIndex[0] = ind1;
    newNode->pointIndex[1] = ind2;
    newNode->pointIndex[2] = ind3;
    newNode->triVecIndex = triangles.size()-3;//third to last position in vector
    leaf2->children.push_back(newNode);

    //update dagAccessList
    dagAccessList[r+3].push_back(newNode);
    dagAccessList[l+3].push_back(newNode);
    //overwrite itDAG with newNode
    it = dagAccessList[p2+3].begin();
    while ((*it) != leaf2)
      ++it;
    (*it) = newNode;

    dagEraseList.push_back(newNode);

  }




  

  /*
   * The algorithm starts with a triangle defined by three special
   * points so that all points from the given point list are inside
   * this triangle. This routine computes these three points.
   */
  template <class T>
  void delaunayTriangulation<T>::calcSpecialPoints(const std::vector<tpoint<T> >& src) {
    
    if (src.size() == 0)
      return;

    T minX = src[0].x;
    T minY = src[0].y;
    T maxX = src[0].x;
    T maxY = src[0].y;
    
    //calculate maxima and minima...
    unsigned int i;
    for (i=1; i<src.size(); ++i) {
      if (src[i].x < minX) 
        minX = src[i].x;

      if (src[i].x > maxX) 
        maxX = src[i].x;

      if (src[i].y < minY) 
        minY = src[i].y;

      if (src[i].y > maxY) 
        maxY = src[i].y;
    }
    
    //...then calculate the extent of the point list
    // T xExt = maxX - minX;
    // T yExt = maxY - minY;

    //calculate the center of the minimal rectangle that includes all
    //points of src
    tpoint<T> center;
    center.x = static_cast<T>((minX + maxX) / 2.0f);
    center.y = static_cast<T>((minY + maxY) / 2.0f);

    //two constants that are half the length of the x-extent (y-extent)
    T xM = center.x - minX;
    T yM = center.y - minY;
    //increase values so real points won't be on the lines connecting
    //the three special points
    xM = static_cast<T>(xM * 1.2);
    yM = static_cast<T>(yM * 1.2);

    const float inftyFac=getParameters().specialPointsFactor;

    //determine points using the constants xM and yM
    specialPoint[2].x = static_cast<T>(center.x + inftyFac * xM);
    specialPoint[2].y = center.y;

    specialPoint[1].x = center.x;
    specialPoint[1].y = static_cast<T>(center.y + inftyFac * yM);

    specialPoint[0].x = static_cast<T>(center.x - inftyFac * xM);
    specialPoint[0].y = static_cast<T>(center.y - inftyFac * yM);
  }




  /*
   * Performs a hit test on the point p. Returns 0 if p is outside the
   * triangle that references points from src, 1 if p is inside the
   * triangle , 2 if it lies on edge src[ind1],src[ind2], 3 if it lies
   * on edge src[ind2],src[ind3], 4 if it lies on edge
   * src[ind3],src[ind1], and 5 if it falls onto a point. At the end,
   * the minimal distance of the point to an edge of the triangle will
   * be stored in "minDist".
   */
  template <class T>
  int delaunayTriangulation<T>::insideTest(const int ind_p, const std::vector<tpoint<T> >& src,
    const int ind1, const int ind2, const int ind3, T& minDist) const {

    T x, y, x1, x2, x3, y1, y2, y3;
    T z_1_2, z_2_3, z_3_1;
    bool inSide;

    const float epsilon=getParameters().epsilon;

    x = src[ind_p].x;
    y = src[ind_p].y;	

    x1 = (ind1 > -1 ? src[ind1].x : specialPoint[ind1+3].x);
    x2 = (ind2 > -1 ? src[ind2].x : specialPoint[ind2+3].x);
    x3 = (ind3 > -1 ? src[ind3].x : specialPoint[ind3+3].x);

    // perform bounding box test on x
    if (x < min(x1,x2,x3) || x > max(x1,x2,x3)) 
      return false;

    y1 = (ind1 > -1 ? src[ind1].y : specialPoint[ind1+3].y);
    y2 = (ind2 > -1 ? src[ind2].y : specialPoint[ind2+3].y);
    y3 = (ind3 > -1 ? src[ind3].y : specialPoint[ind3+3].y);

    // perform bounding box test on y
    if (y < min(y1,y2,y3) || y > max(y1,y2,y3))
      return false;

    //z-component of cross product, if all z-components are positive
    //or all negative then the point is inside
    z_1_2 = x*y2-x*y1-x1*y2-x2*y+x2*y1+x1*y;
    z_2_3 = -y*x3+y3*x-x*y2+x2*y-x2*y3+x3*y2;
    z_3_1 = y*x3-x1*y-x3*y1-y3*x+x1*y3+x*y1;
		
    T m_dD = -x2*y3+x2*y1+x1*y3+x3*y2-x3*y1-x1*y2;
    
//     std::cerr << "z_1_2=" << z_1_2
//               << " z_2_3=" << z_2_3
//               << " z_3_1=" << z_3_1 <<"\n";
//     std::cerr << "m_dD: " << m_dD << "\n";

    //Due to the problem of discretization of lines we need a small
    //buffer for this calculation
    inSide = (z_1_2 >= -epsilon && z_1_2 <= m_dD + epsilon &&//if the points are sorted clockwise
              z_2_3 >= -epsilon && z_2_3 <= m_dD + epsilon &&
              z_3_1 >= -epsilon && z_3_1 <= m_dD + epsilon) ||
             (z_1_2 <= epsilon && z_1_2 >= m_dD - epsilon &&//if the points are sorted counterclockwise
              z_2_3 <= epsilon && z_2_3 >= m_dD - epsilon &&
              z_3_1 <= epsilon && z_3_1 >= m_dD - epsilon);
    
    assert(m_dD != 0);
    z_1_2 /= m_dD;
    z_2_3 /= m_dD;
    z_3_1 /= m_dD;
    z_1_2 = abs(z_1_2);
    z_2_3 = abs(z_2_3);
    z_3_1 = abs(z_3_1);

//     std::cerr << "inside= " << inSide << " z_1_2=" << z_1_2
//               << " z_2_3=" << z_2_3
//               << " z_3_1=" << z_3_1 <<"\n";

    //store minimum distance to an edge
    minDist = min(z_1_2, z_2_3, z_3_1);
    
    //if the point is inside the triangle determine "where" it is...
    if (inSide) {
     
      //point on edge p1,p2
      if (z_1_2 < epsilon) {
        //on point
        if (z_2_3 < epsilon || z_3_1 < epsilon)
          return 5;
        //on edge
        else
          return 2;
      }
      //point on edge p2,p3
      else if (z_2_3 < epsilon) {
        if (z_1_2 < epsilon || z_3_1 < epsilon)
          return 5;
        //on edge
        else
          return 3;
      }
      //point on edge p3,p1
      else if (z_3_1 < epsilon) {
        if (z_1_2 < epsilon || z_2_3 < epsilon)
          return 5;
        //on edge
        else
          return 4;
      }
      else
       //point in the interior of triangle
      return 1;
    }
    else //point outside triangle
      return 0;
  }

   



  // Find the circle that circumscribes 3 points
  template <class T>
  bool delaunayTriangulation<T>::getCircle(const std::vector<tpoint<T> >& src, int ind1, int ind2, int ind3, tpoint<T>& center, double& radSqu) const {
    
    //Special point or normal point?
    tpoint<T> p1 = (ind1 > -1 ? src[ind1] : specialPoint[ind1+3]);
    tpoint<T> p2 = (ind2 > -1 ? src[ind2] : specialPoint[ind2+3]);
    tpoint<T> p3 = (ind3 > -1 ? src[ind3] : specialPoint[ind3+3]);

    //we don't want infinite slopes, (or 0 slope), since we'll divide by the  slope, "ma", below)}
    if (p1.x == p2.x || p1.y == p2.y) {
      //swap p2 and p3
      tpoint<T> temp = p2;
      p2 = p3;
      p3 = temp;
    } 
    if (p2.x == p3.x) {
      //swap p1 and p2
      tpoint<T> temp = p1;
      p1 = p2;
      p2 = temp;
    } 

    //slope variables
    double ma(0.0), mb(0.0);
    bool result = true;

    if (p1.x != p2.x)
      ma = (static_cast<double>(p2.y-p1.y))/(p2.x-p1.x);
    else 
      result = false;

    if (p2.x != p3.x)
      mb = (static_cast<double>(p3.y-p2.y))/(p3.x-p2.x);
    else 
      result = false;

    if (ma == 0.0 && mb == 0.0)
      result = false;

    if (result) {
      //if ma=mb then showmessage('Invalid call to getcenter');
      center.x = static_cast<T>( (ma*mb*(p1.y-p3.y)+mb*(p1.x+p2.x)-ma*(p2.x+p3.x))/(2*(mb-ma)) );
      center.y = static_cast<T>( -(center.x-(p1.x+p2.x)/2.0f)/ma + (p1.y+p2.y)/2.0f);

      radSqu = euclDistSquare(p1, center);
    }
    return result;
  }




  template <class T>
  inline double delaunayTriangulation<T>::euclDistSquare(const tpoint<T>& p1, const tpoint<T>& p2) const {
    T dx = p1.x - p2.x;
    T dy = p1.y - p2.y;

    return static_cast<double>(dx*dx + dy*dy);
  }




  template <class T>
  void 
  delaunayTriangulation<T>::legalizeEdge(const std::vector<tpoint<T> >& src,
                                         int r,
                                         int p1,
                                         int p2, 
                                         std::vector<int>& triangles) {

//     std::cerr << "r: " << r << " p1: " << p1 << " p2: " << p2 << "\n";

    //find the two triangles in the DAG with the edge p1,p2 using the
    //dagAccessList (this is where this structure is really needed)
    dagNode* tri1 = NULL;
    dagNode* tri2 = NULL;
    typename std::list<dagNode*>::iterator itAL1, itAL2;

    for (itAL1 = dagAccessList[p1+3].begin(); 
         itAL1 != dagAccessList[p1+3].end(); ++itAL1)
      for (itAL2 = dagAccessList[p2+3].begin();
           itAL2 != dagAccessList[p2+3].end(); ++itAL2) {

        //pointers to the same triangle
        if ((*itAL1) == (*itAL2))
          if (tri1 == NULL) {
            tri1 = (*itAL1);
          }
          else {
            tri2 = (*itAL2);
            break;
          }
      }

    //if p1,p2 is an outside edge that belongs to only one triangle, nothing has to be done
    if (tri2 == NULL)
      return;

    //find missing point l of the triangle that shares the edge (p1,p2) with the triangle (p1,p2,r).
    int l=-4, ind1, ind2;
    for (int i=0; i<3; ++i) {
      ind1 = tri1->pointIndex[i];
      if (ind1 != r && ind1 != p1 && ind1 != p2) {
        l = ind1;
        break;
      }

      ind2 = tri2->pointIndex[i];
      if (ind2 != r && ind2 != p1 && ind2 != p2) {
        l = ind2;
        break;
      }
    }
    
    assert(l != -4);
  
    //if the two triangles p1,p2,r and p1,p2,l are not delaunay then perform edge flips to
    //get a triangulation that is delaunay
    if (isIllegal(src, r, p1, p2, l)) {

//       std::cerr << "is illegal, l:" << l << "\n";
      
      //replace edge p1,p2 with r,l
      //1.) update DAG: add two nodes to the DAG as children of tri1 and tri2
      dagNode* newNode1 = new dagNode;
      newNode1->pointIndex[0] = p1;
      newNode1->pointIndex[1] = r;
      newNode1->pointIndex[2] = l;
      newNode1->triVecIndex = tri1->triVecIndex;
      
      tri1->children.push_back(newNode1);
      tri2->children.push_back(newNode1);

      dagNode* newNode2 = new dagNode;
      newNode2->pointIndex[0] = p2;
      newNode2->pointIndex[1] = r;
      newNode2->pointIndex[2] = l;
      newNode2->triVecIndex = tri2->triVecIndex;

      tri1->children.push_back(newNode2);
      tri2->children.push_back(newNode2);
      
      //2.) replace edges in triangle-vector (note: indices must be sorted)
      int i1 = p1, i2 = r, i3 = l;
      sort3(i1, i2, i3);
      triangles[tri1->triVecIndex] = i1;
      triangles[tri1->triVecIndex+1] = i2;
      triangles[tri1->triVecIndex+2] = i3;

      i1 = p2, i2 = r; i3 = l;
      sort3(i1, i2, i3);
      triangles[tri2->triVecIndex] = i1;
      triangles[tri2->triVecIndex+1] = i2;
      triangles[tri2->triVecIndex+2] = i3;

      //3.) update dagAccessList
      //p1 is now part of the triangle p1,r,l (newNode1)-> overwrite obsolete tri1-entry, delete obsolete tri2-entry
      for (itAL1 = dagAccessList[p1+3].begin(); itAL1 != dagAccessList[p1+3].end(); ) {
        if ((*itAL1) == tri1) {
          (*itAL1) = newNode1;
          ++itAL1;
        }
        else if ((*itAL1) == tri2) {
          itAL1 = dagAccessList[p1+3].erase(itAL1);
        }
        else
          ++itAL1;
      }

      //p2 is now part of the triangle p2,r,l (newNode2) -> overwrite obsolete tri1-entry, delete obsolete tri2-entry
      for (itAL1 = dagAccessList[p2+3].begin(); itAL1 != dagAccessList[p2+3].end(); ) {
        if ((*itAL1) == tri1) {
          (*itAL1) = newNode2;
          ++itAL1;
        }
        else if ((*itAL1) == tri2) {
          itAL1 = dagAccessList[p2+3].erase(itAL1);
        }
        else
          ++itAL1;
      }

      //r is now part of the triangles defined by newNode1 and newNode2, delete obsolete entries
      for (itAL1 = dagAccessList[r+3].begin(); itAL1 != dagAccessList[r+3].end(); ++itAL1) {
        if ((*itAL1) == tri1 || (*itAL1) == tri2) {
          (*itAL1) = newNode1;
          dagAccessList[r+3].push_back(newNode2);
          break;
        }
      }

      //l is now part of the triangles defined by newNode1 and newNode2, delete obsolete entries
      for (itAL1 = dagAccessList[l+3].begin(); itAL1 != dagAccessList[l+3].end(); ++itAL1) {
        if ((*itAL1) == tri1 || (*itAL1) == tri2) {
          (*itAL1) = newNode1;
          dagAccessList[l+3].push_back(newNode2);
          break;
        }
      }

      dagEraseList.push_back(newNode1);
      dagEraseList.push_back(newNode2);

      //check if new edges need to be legalized...
      legalizeEdge(src, r, p1, l, triangles);
      legalizeEdge(src, r, l, p2, triangles);

    }

  }


  template <class T>
  void delaunayTriangulation<T>::sort3(int& i1, int& i2, int& i3) const {
    //insertion sort with three elements
    int temp;
    if (i1 > i2) {
      temp = i1;
      i1 = i2;
      i2 = temp;
    }
    if (i2 > i3) {
      temp = i2;
      i2 = i3;
      i3 = temp;
    }
    if (i1 > i2) {
      temp = i1;
      i1 = i2;
      i2 = temp;
    }
  }


  /*
   * Test if the triangles p1,p2,r and p1,p2,l are illegal
   */
  template <class T>
  bool delaunayTriangulation<T>::isIllegal(const std::vector<tpoint<T> >& src, int r, int p1, int p2, int l) {
    //Note: r always has a positive index

    //We have to distinguish between several cases...
    //1.) p1 and p2 are both special points -> legal, keep edges
    if (p1 < 0 && p2 < 0) {
      return false;
    }
    //2.) no special points involved (normal case) -> illegal iff r lies inside the circle defined by p1, p2 and l
    else if (p1 > -1 && p2 > -1 && l > -1) {
      tpoint<T> center;
      double radSqu;
      getCircle(src, p1, p2, l, center, radSqu);
      if (euclDistSquare(center, src[r]) < radSqu) {
        return true;
      }
      else
        return false;
    }
    //3.) exactly one point p1,p2 is a special point and l is not -> illegal
    else if ( (p1 < 0 && p2 > -1 && l > -1) ||
              (p2 < 0 && p1 > -1 && l > -1)) {
      tpoint<T> center;
      double radSqu;
      getCircle(src, p1, p2, l, center, radSqu);
      if (euclDistSquare(center, src[r]) < radSqu) {
        return true;
      }
      else
        return false;
    }
    //4.) l is a special point and p1,p2(,r) are not -> legal
    else if (l < 0 && p1 > -1 && p2 > -1) {
      return false;
    }
    //5.)if two points are special points...
    //5a.) p1 and l are special points
    else if (p1 < 0 && p2 > -1 && l < 0) {
      if (p1 < l)
        return false;
      else {
        tpoint<T> center;
        double radSqu;
        getCircle(src, p1, p2, l, center, radSqu);
        if (euclDistSquare(center, src[r]) < radSqu) {
          return true;
        }
        else
          return false;
      }
    }
    //5b.) p2 and l are special points
    else if (p2 < 0 && p1 > -1 && l < 0) {
      if (p2 < l)
        return false;
      else {
        tpoint<T> center;
        double radSqu;
        getCircle(src, p1, p2, l, center, radSqu);
        if (euclDistSquare(center, src[r]) < radSqu) {
          return true;
        }
        else
          return false;
      }
    }
    //there shouldn't be any other cases
    else 
      throw exception("Unknown case in delaunayTriangulation::isIllegal(...)!!!");
    
    return false;
  }


  
  // clean-up memory
  template <class T>
  void delaunayTriangulation<T>::deleteDAG() {
    
    typename std::list<dagNode*>::iterator it;
    
    for (it=dagEraseList.begin(); it!=dagEraseList.end(); ++it)
      delete (*it);
        
  }


  /*
  template <class T>
  void delaunayTriangulation<T>::testTriangulation(const std::vector<tpoint<T> >& src, const std::vector<int>& tri) const {
    double radSqu;
    tpoint<T> center;
    for (int i=0; i<tri.size(); i+=3) {
      getCircle(src, tri[i], tri[i+1], tri[i+2], center, radSqu);
      
      for (int j=0; j<src.size(); ++j) {
        if (tri[i] != j && tri[i+1] != j && tri[i+2] != j) {
          double ed = euclDistSquare(center, src[j]);
          if (ed < radSqu) {
            std::cout << "point " << j << " and triangle (" << tri[i] << "," << tri[i+1] << "," << tri[i+2] << ") not ok." << "radSqu-ed: " << radSqu-ed << std::endl;
            getchar();
          }
          //else
            //std::cout << "point " << j << " and triangle (" << tri[i] << "," << tri[i+1] << "," << tri[i+2] << ") ok." << std::endl;
        }
      }

    }
  }

*/

}

