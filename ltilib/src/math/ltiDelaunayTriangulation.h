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
 * file .......: ltiDelaunayTriangulation.h
 * authors ....: Daniel Beier
 * organization: LTI, RWTH Aachen
 * creation ...: 7.5.2003
 * revisions ..: $Id: ltiDelaunayTriangulation.h,v 1.12 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_DELAUNAY_TRIANGULATION_H_
#define _LTI_DELAUNAY_TRIANGULATION_H_

#include "ltiPoint.h"
#include "ltiMatrix.h"
#include <vector>
#include <list>

#include "ltiFunctor.h"

namespace lti {
  /**
   * This class performs a delaunay triangulation on a given
   * std::vector<tpoint<T> >. A triangulation T of a given set of
   * points P is called Delaunay if the circumcircle of any triangle
   * of T does not contain a point of P. The triangulation is
   * therefore angle-optimal. A triangulation T is angle-optimal if
   * the sorted vector of angles A(T)=(alpha_1,alpha_2,...,alpha_3m)
   * is the largest angle-vector. An angle vector A(T) is larger then
   * A(T') if there exists an i so that alpha_j = alpha'_j for every j
   * < i and alpha_i > alpha'_i. Optimal triangulations avoid "skinny"
   * triangles. The implemented algorithm is in O(n*log(n)) where n is
   * the number of points. The discription of the algorithm can be
   * found in: de Berg, van Krefeld: Computational Geometry (Chapter
   * 9: Delaunay Triangulations).
   *
   * \b Note: Depending on the supplied data this algorithm may fail
   * due to numerical and implementation issues. It usually works well
   * for points that correspond to pixel positions in a image. For
   * other uses adjusting the parameters::epsilon and
   * parameters::specialPointsFactor will probably lead to a
   * satisfactory result.
   *
   * This failure can manifest itself in two ways: 
   * - An assertion (l != 4) is thrown in debug mode (a segmentation
   * fault in release). Probably need higher value for
   * parameters::specialPointsFactor
   * - There are edges missing on the convex hull, solve as above.
   */
  template <class T>
  class delaunayTriangulation : public functor {
  public:
    /**
     * the parameters for the class delaunayTriangulation
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : functor::parameters() {
        epsilon=0.001f;
        specialPointsFactor=100.f;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : functor::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {};

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "delaunayTriangulation::parameters";
      };

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
          (const functor::parameters&) = functor::parameters::copy;
        (this->*p_copy)(other);
# endif
        
        epsilon = other.epsilon;
        specialPointsFactor = other.specialPointsFactor;

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

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
# ifndef _LTI_MSC_6
        virtual bool write(ioHandler& handler, const bool complete=true) const
# else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
        bool writeMS(ioHandler& handler, const bool complete=true) const
# endif
        {
          bool b = true;
          if (complete) {
            b = handler.writeBegin();
          }

          if (b) {
            lti::write(handler,"specialPointsFactor",specialPointsFactor);
            lti::write(handler,"epsilon",epsilon);
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

# ifdef _LTI_MSC_6
        virtual bool write(ioHandler& handler, const bool complete=true) const {
          // ...we need this workaround to cope with another really awful MSVC bug.
          return writeMS(handler,complete);
        }
# endif

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      //virtual bool read(ioHandler& handler,const bool complete=true);
# ifndef _LTI_MSC_6
      virtual bool read(ioHandler& handler, const bool complete=true)
# else
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler, const bool complete=true)
# endif
      {
        bool b = true;
        if (complete) {
          b = handler.readBegin();
        }

        if (b) {
            lti::read(handler,"specialPointsFactor",specialPointsFactor);
            lti::read(handler,"epsilon",epsilon);
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

# ifdef _LTI_MSC_6
      virtual bool read(ioHandler& handler, const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
# endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      

      /**
       * The algorithm uses a virtual triangle consisting of three
       * special points. It must at least include all given
       * points. However, the distance of these points from the actual
       * points influences the bahaviour of the algorithm with respect
       * to small dents in the convex hull.
       *
       * A large value will give you the complete, correct Delaunay
       * triangulation. A small value excludes triangles that share
       * one edge with the convex hull and have a 'large' angle
       * opposite that edge. This behavior can be useful in some
       * applications where 'degenerate' triangles are not useful.
       *
       * Default: 100. Gives the correct result for most applications
       */
      float specialPointsFactor;

      /**
       * This value is used for determining when a point falls on an
       * edge. Thus the construction of the triangulation can be
       * influenced quite strongly by this value.
       *
       * Note that the values depends on the given data.
       *
       * Default: 0.001. Works well for pixel position ranges
       */
      float epsilon;
    };

  private:
    //this class defines a directed acyclic graph (DAG) which helps to
    //locate triangles that contain a given point.
    class dagNode {
    public:
      //references to the three points of the triangle. Whenever this
      //method is used it is assumed that a std::vector<tpoint> exists
      //somewhere that corresponds to the indices stored in
      //pointIndex[3]
      int pointIndex[3];

      //if a node is a leaf then it stores a reference to a triangle
      //of the current triangulation. If it is an inner node this
      //class-attribute is not used.
      int triVecIndex;

      //child nodes
      std::list<dagNode*> children;
    };

  public:

    /**
     * default constructor
     */
    delaunayTriangulation();

    /**
     * Construct a functor using the given parameters
     */
    delaunayTriangulation(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    delaunayTriangulation(const delaunayTriangulation& other);

    /**
     * destructor
     */
    virtual ~delaunayTriangulation();

    /**
     * returns the name of this type ("delaunayTriangulation")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!
    
    /**
     * Performing a delaunay triangulation on the given set of points
     * and storing the result in 'triangles'.
     * @param src the source data.
     * @param triangles a std::vector containing the triangles as a
     *        result of the delaunay triangulation.  the indices 0..2 belong
     *        to the first triangle, 3..5 to the second, and so on.
     * @return true if successful or false otherwise.
     */
    bool apply(const std::vector<tpoint<T> >& src,std::vector<int>& triangles);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    delaunayTriangulation& copy(const delaunayTriangulation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    delaunayTriangulation& operator=(const delaunayTriangulation& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    //void testTriangulation(const std::vector<tpoint<T> >& src,
    //                       const std::vector<int>& tri) const;

  private:
   
    void initialize(const std::vector<tpoint<T> >& src, 
                    std::vector<int>& triangles);

    // The algorithm starts with a triangle defined by three points
    // (private class-members) so that all points from the given point
    // list are inside this triangle. This routine computes these
    // three points.
    void calcSpecialPoints(const std::vector<tpoint<T> >& src);

    // splitting one triangle into three when point r (inside of
    // triangle) is inserted.
    void split1to3(const std::vector<tpoint<T> >& src,
                   std::vector<int>& triangles, int r, dagNode* leaf);

    // splitting two triangles into four when point r (on edge p1,p2)
    // is inserted
    void split2to4(const std::vector<tpoint<T> >& src,
                   std::vector<int>& triangles,
                   int r, int p1, int p2, int k, int l,
                   dagNode* leaf1, dagNode* leaf2);

    // Tests if a point is inside a triangle
    int insideTest(const int ind_p, const std::vector<tpoint<T> >& src,
                   const int ind1, const int ind2, const int ind3, T& minDist) const;

    // flip an edge if delaunay property is violated
    void legalizeEdge(const std::vector<tpoint<T> >& src, int r, int p1, int p2, std::vector<int>& triangles);

    // test if an edge violates the delaunay property
    bool isIllegal(const std::vector<tpoint<T> >& src, int r, int p1, int p2, int l);

    // Find the circle that circumscribes 3 points
    bool getCircle(const std::vector<tpoint<T> >& src, int p1, int p2, int p3, tpoint<T>& center, double& rad) const;

    // calculate the squared euclidean distance
    inline double euclDistSquare(const tpoint<T>& p1, const tpoint<T>& p2) const;

    // insertion sort with three elements
    void sort3(int& i1, int& i2, int& i3) const;

    // clean-up memory
    void deleteDAG();

    // this directed acyclic graph keeps track of all the
    // triangulations made. Its root is the triangle special points.
    // Every time a triangle splits into two or three triangles when a
    // new point is inserted the leaf node of the graph belonging to
    // this triangle becomes an inside node and two or three child
    // nodes are created (Two if the point is on an edge and three
    // otherwise).
    dagNode* dagRoot;

    // special points that define an initial large triangle for the algorithm
    tpoint<T> specialPoint[3];

    // for quick access to all leaves of the DAG with point i this
    // vector is used
    std::vector<std::list<dagNode*> > dagAccessList;

    // to free memory at the end store all nodes that were created with
    // new in this list
    std::list<dagNode*> dagEraseList;

  };
}

#endif

