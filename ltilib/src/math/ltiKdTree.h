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
 * file .......: ltiKdTree.h
 * authors ....: Frederik Lange, Pablo Alvarado, Jens Rietzschel
 * organization: LTI, RWTH Aachen
 * creation ...: 9.1.2003
 * revisions ..: $Id: ltiKdTree.h,v 1.31 2005/08/05 12:37:09 libuda Exp $
 */

#ifndef _LTI_KD_TREE_H_
#define _LTI_KD_TREE_H_

#include "ltiMathObject.h"
#include "ltiMath.h"
#include "ltiQuickMedian.h"
#include "ltiL2Distance.h"
#include "ltiSTLIoInterface.h"
#include "ltiTypeInfo.h"

#include <list>
#include <map>

namespace lti {

  /**
   * A class for k-d tree.
   *
   * A k-d tree is a generalization of the simple binary tree used for
   * sorting and searching.  At each level of the tree, a
   * n-dimensional subspace is split in two subspaces at a given
   * dimension.  The leaves of the tree contain a "bucket" of data
   * within the described subspace.
   *
   * You can consider data for building with the add() method. Then
   * you can either build() the tree from that data, discarding the
   * old data, or rebuild() the tree, which will then contain the data
   * added since the last call to build() or rebuild() plus the newly
   * added data.
   *
   * This type allows to search in an efficient way for the
   * more similar neighbors of a given point.
   *
   * The type T describes a point in an n-dimensional space.  Usually,
   * you will use vector<float> or vector<double>, but types like
   * rgbPixel, trgbPixel<float>, tpoint<float> are also supported.
   * The type T MUST provide following methods/operators:

   * - copy %operator=()
   * - access operator[]()
   * - size() method
   * - must define the type <code>value_type</code>, which describes the 
   *   type of each element in the container.
   * - comparison %operator==()
   *
   * (lti::vector, lti::tpoint, lti::tpoint3D, lti::trgbpixel and 
   * lti::rgbPixel provide this interface)
   *
   * The type D describes the data contained at each node, which is per default
   * just an integer value.  It must provide following methods/operators
   * - copy operator=()
   *
   * The type U specifies a "distantor" policy, i.e. it specifies the
   * distance to be used while computing the nearest neighbors.  If
   * not given, the default value will be l2SquareDistantor, which
   * provides, as the name says, the square of the euclidian distance.
   * Only Minkowski distances are allowed.  If you need your own
   * distance measure you can create your policy following the
   * interfaces of one of the classes lti::l2Distantor,
   * lti::l1Distantor or l2SquareDistantor.
   *
   * \Note: Elements added to the tree after a call to build() or
   * rebuild() are not saved when calling write().
   *
   * The original kd-Tree algorithm for k-neareast neighbors problems was
   * introduced in:
   * Friedman, J.H., Bentley, J.L. and Finkel, R.A.  An Algorithm for Finding
   *     Best Matches in Logarithmic Expected Time. ACM Transaction on
   *     Mathematical Software, Vol. 3, No. 3, September 1977, Pages 209-225
   *
   * @ingroup gAggregate
   */
  template <class T,class D=int,class U=l2SquareDistantor<T> >
  class kdTree : public mathObject, public status {
  public:
    /**
     * Value type at each dimension of the space
     */
    typedef typename T::value_type value_type;

    /**
     * Type used to accumulate data of value_type elements
     */
    typedef typename typeInfo<value_type>::accumulation_type acc_type;

    /**
     * At the leave nodes, a list of elements of this type will contain
     * the points and their corresponding data.
     *
     * Implementation must be here due to MS VC++ bug
     */
    class element : public ioObject {
    public:

      /**
       * n-dimensional point representing the position of this element
       * in the n-dimensional space
       */
      T point;

      /**
       * data contained in this element.
       */
      D data;

      /**
       * Constructor
       */
      element() : ioObject(),point(T()),data(D()) {
      };

      /**
       * Constructor
       *
       * @param pos n-dimensional vector position of this element
       * @param dat data contained in this element
       */
      element(const T& pos, const D& dat)
        : ioObject(),point(pos),data(dat) {
      };

      /**
       * Copy constructor
       */
      element(const element& other) 
        : ioObject(),point(other.point),data(other.data) {
      };

      /**
       * destructor
       */
      ~element() {
      };

      /**
       * shortcut to access the point value at each dimension
       */
      inline value_type& operator[](const int a) {return point[a];}

      /**
       * shortcut to access the point value at each dimension
       */
      inline const value_type& operator[](const int a) const {return point[a];}

      /**
       * shortcut to access the size of the points stored
       */
      inline int size() const {
        return point.size();
      }
      
      /**
       * copy method
       */
      element& copy(const element& other) {
        point=other.point;
        data=other.data;
        return *this;
      }

      /**
       * copy operator
       */
      inline element& operator=(const element& other) {
        return copy(other);
      }

      /**
       * clone member
       */
      element* clone() const {
        return new element(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        if (complete) {
          b = handler.readBegin();
        }

        b = b && lti::read(handler,point);
        b = b && handler.readDataSeparator();
        b = b && lti::read(handler,data);

        if (complete) {
          b= b && handler.readEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b = handler.writeBegin();
        }

        b = b && lti::write(handler,point);
        b = b && handler.writeDataSeparator();
        b = b && lti::write(handler,data);

        if (complete) {
          b= b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif
    };

    /**
     * Class of nodes of the kd-tree
     *
     * Implementation must be here due to MS VC++ bug
     */
    class node : public ioObject {
      // the enclosing class is a friend of mine.
      friend class kdTree<T,D,U>;
    public:

      /**
       * type used for the container of elements.
       * A std::vector is for the search much more appropriate because
       * the access through iterators is about a factor 5 faster than the
       * std::list type.
       */
      typedef std::vector<element*> points_type;

      /**
       * @name Attributes
       */
      //@{
      /**
       * Points stored in this node.  These are pointers to the data
       * allocated in the kdTree class, this way the data can be
       * "transfered" more efficiently between different nodes.  The
       * memory managment is done by this node.
       */
      points_type points;

      /**
       *  the left subtree (lower coordinate) from the split plane
       */
      node* left;

      /**
       *  the right subtree (higher coordinate) from the split plane
       */
      node* right;

      /**
       * the dimension in which the subnodes are split.  This is
       * called in the original paper "discriminator"
       */
      int splitDim;

      /**
       * Value at the split dimension where the splitting takes place.
       * Usually this value corresponds to the median at the given
       * split dimension.
       *
       */
      value_type partition;
      //@}

      /**
       * constructor
       */
      node() 
        : ioObject(),points(),left(0),right(0),splitDim(0),
          partition(value_type()) {
      };

      /**
       * copy constructor.
       * Makes a deep copy, i.e. the pointers will be different
       */
      node(const node& other) 
        : ioObject(),points(),left(0),right(0) {
        copy(other);
      }

      /**
       * destructor
       */
      ~node() {
        clearPoints();

        if(left!=0) {
          delete left;
          left=0;
        }
        if(right!=0) {
          delete right;
          right=0;
        }
      };

      /**
       * insert a pointer to an element into the points list.  
       *
       * The pointed element will be inserted as is (without copying), and
       * the memory managment is controled by the node itself.
       *
       * The dimensionality of each element MUST be equal than the one
       * of the first element added.
       */
      void add(element* f) {
        points.push_back(f);
      }

      /**
       * return true if this node is a leaf.
       */
      inline bool isLeaf() const {
        return (!points.empty()); 
      }

      /**
       * copy method
       */
      node& copy(const node& other) {

        // copy the data
        splitDim = other.splitDim;
        partition = other.partition;

        // remove the current elements
        clearPoints();

        // deep copy the other elements
        typename points_type::const_iterator cit,ceit;
        for (cit=other.points.begin(),ceit=other.points.end();
             cit!=ceit;
             ++cit) {
          points.push_back((*cit)->clone());
        }
        
        // remove old data
        if (notNull(left)) {
          delete left;
          left = 0;
        }

        if (notNull(right)) {
          delete right;
          right = 0;
        }

        // copy children if necessary.
        if (notNull(other.left)) {
          left = other.left->clone();
        }

        if (notNull(other.right)) {
          right = other.right->clone();
        }

        return *this;
      }

      /**
       * copy method
       */
      inline node& operator=(const node& other) {
        return copy(other);
      }


      /**
       * clone method
       */
      node* clone() const {
        return new node(*this);
      }

#     ifndef _LTI_MSC_6
      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler &handler, const bool complete=true)
#     else
      bool readMS(ioHandler& handler,const bool complete=true)
#     endif
      {
        bool b=true;

        delete left;
        left = 0;
        delete right;
        right = 0;

        if (complete) {
          b = handler.readBegin();
        }

        b = b && lti::read(handler,"splitDim",splitDim);
        b = b && lti::read(handler,"partition",partition);
        
        // read the points with their corresponding data
        clearPoints();

        // -----------------------------------------------------

        b = b && handler.readBegin();              // points field (1)
        b = b && handler.trySymbol("points");      //
        b = b && handler.readKeyValueSeparator();  //
        b = b && handler.readBegin();              // vector data  (2)

        // -----------------------------
        int i;
        element* tmpElem;
        int pointsSize;
        b = b && handler.read("size",pointsSize);

        points.resize(pointsSize);

        b = b && handler.readBegin(); // data block of vector      (3)
        if (pointsSize > 0) {
          for (i=0;i<pointsSize-1;++i) {
            tmpElem = new element;
            b = b && tmpElem->read(handler);
            points[i] = tmpElem;
            tmpElem=0;
            b = b && handler.readDataSeparator();
          }
          tmpElem = new element;
          b = b && tmpElem->read(handler);
          points[i] = tmpElem;
          tmpElem=0;
        }
        b = b && handler.readEnd();   // data block of vector      (2)
        
        // -----------------------------

        b = b && handler.readEnd(); // vector data                 (1)
        b = b && handler.readEnd(); // points field                (0)

        // -----------------------------------------------------
        

        // load the children


        // left node first
        b = b && handler.readBegin();              // left scope  (1)
        b = b && handler.trySymbol("left");        //
        b = b && handler.readKeyValueSeparator();  //
        b = b && handler.readBegin();              // left data   (2)
                                                   //
        if (!handler.tryEnd()) {                   // 
          left = new node;                         //
          // recursively read the children         //
          b = b && left->read(handler,false);      //
          b = b && handler.readEnd();              // left data   (1)
        }                                          //
                                                   //
        b = b && handler.readEnd();                // left scope  (0)

        // right node next
        b = b && handler.readBegin();              // right scope (1)
        b = b && handler.trySymbol("right");       //
        b = b && handler.readKeyValueSeparator();  //
        b = b && handler.readBegin();              // right data  (2)
                                                   //
        if (!handler.tryEnd()) {                   // 
          right = new node();                      //
          // recursively read the children         //
          b = b && right->read(handler,false);     //
          b = b && handler.readEnd();              // right data  (1)
        }                                          //
                                                   //
        b = b && handler.readEnd();                // right scope (0)

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
       *        be also read, otherwise only the data block will be read.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return readMS(handler,complete);
      }
#     endif

#     ifndef _LTI_MSC_6
      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
      bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
      {
        bool b=true;

        if (complete) {
          b = handler.writeBegin();
        }

        b = b && lti::write(handler,"splitDim",splitDim);
        b = b && lti::write(handler,"partition",partition);
        
        // write from the points only the index
        // -------------------------------------------------------

        b = b && handler.writeBegin();             // points field
        b = b && handler.writeSymbol("points");    //
        b = b && handler.writeKeyValueSeparator(); //
                                                   //
        b = b && handler.writeBegin();             // vector data

        // ------------------------------------------
        b = b && handler.write("size",static_cast<unsigned int>(points.size()));
        b = b && handler.writeBegin(); // data block of vector
        if (!points.empty()) {                     //      |
          typename points_type::const_iterator it; //      |
          it=points.begin();                       //      |
          b = b && (*it)->write(handler);          //      |
          for (++it;it!=points.end();++it) {       //      |
            b = b && handler.writeDataSeparator(); //      |
            b = b && (*it)->write(handler);        //      |
          }                                        //      |
        }                                          //      |
        b = b && handler.writeEnd();   // data block of vector
        
        // -------------------------------------------

        b = b && handler.writeEnd(); // vector data
        b = b && handler.writeEnd(); // points field

        // -------------------------------------------------------

        b = b && handler.writeEOL();  
        
        // Save the children

        // left node first
        b = b && handler.writeBegin();             // left child   (1)
        b = b && handler.writeSymbol("left");      //
        b = b && handler.writeKeyValueSeparator(); //
        b = b && handler.writeBegin();             // node block   (2)
        if (notNull(left)) {                       //
          // recursivelly save the children        // 
          b = b && left->write(handler,false);     //
        }                                          //
        b = b && handler.writeEnd();               // node block   (1)
        b = b && handler.writeEnd();               // left child   (0)
        b = b && handler.writeEOL();  
        
        // right node next
        b = b && handler.writeBegin();             // right child  (1)
        b = b && handler.writeSymbol("right");     //
        b = b && handler.writeKeyValueSeparator(); //
        b = b && handler.writeBegin();             // node block   (2)
        if (notNull(right)) {                      //
          // recursivelly save the children        //
          b = b && right->write(handler,false);    //
        }                                          //
        b = b && handler.writeEnd();               // node block   (1)
        b = b && handler.writeEnd();               // right child  (0)

        b = b && handler.writeEOL();

        if (complete) {
          b=b && handler.writeEnd();
        }

        return b;
      }

#     ifdef _LTI_MSC_6
      /**
       * write the parameters to the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also writen, otherwise only the data block will be writen.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) {
        // ...we need this workaround to cope with another really awful MSVC
        // bug.
        return writeMS(handler,complete);
      }
#     endif


    protected:
      /**
       * deep clear points
       */
      void clearPoints() {
        // delete the elements
        typename points_type::iterator it,eit;
        for (it=points.begin(),eit=points.end();it!=eit;++it) {
          delete (*it);
          (*it)=0;
        }

        points.clear();
      }

      /**
       * split the data stored at this node considering the axis with
       * the highest variance.
       *
       * @param maxCount maximum bucket size (how many elements are allowed
       *                 to be in a leaf node)
       * @param levels number of levels required until now for this tree.
       * @param leaves at the end this variable (which should be initialized
       *        with zero externally) contains the number of leaves in the tree
       */
      void subdivide(const int maxCount,int& levels,int& leaves) {
        const int n = static_cast<int>(points.size());
        if (n <= maxCount) {
          // we don't need to split.  This now is small enough
          leaves++;
          levels=1;
          return;
        }

        // remove the old children and create empty ones.
        if (notNull(left)) {
          delete left;
          left = 0;
        }
        left = new node();

        if (notNull(right)) {
          delete right;
          right = 0;
        }
        right = new node();      

        // split dimension at this node
        // remember split dimension (splitDim is a class attribute)
        const int dim = splitDim = getDimWithHighestVariance();

        // get the median of the splitDim as the value to split the space
        // remember split position
        const value_type medVal = partition = getMedianVal(dim);

        const int lc = n/2;
        int npleft(0),npeq(0); // number of points in the left side
        typename points_type::iterator it=points.begin();
        std::list<element*> stack;
        // split the points into the two children. 
        while (it!=points.end()) {
          if (((*it)->point[dim] < medVal)) {
            left->add(*it);
            npleft++;
          } else if (((*it)->point[dim] > medVal)) {
            right->add(*it);
          } else {
            npeq++;
            stack.push_back(*it);
          }
          ++it;
        }

        // the remaining median-equal values need to be split so
        // that both children have the same number of points
        while (npleft<lc) {
          left->add(stack.front());
          stack.pop_front();
          npleft++;
        }
        if (!stack.empty()) {
          right->add(stack);
        }
        
        points.clear();

        // recursively split the data into both child nodes
        int llev,rlev;
        left->subdivide(maxCount,llev,leaves);
        right->subdivide(maxCount,rlev,leaves);
        levels = 1+max(llev,rlev);
      }

      /**
       * get the dimension of the data with the highest variance.
       * We assume the whole data set to be considered is located
       * at the points vector.
       */
      int getDimWithHighestVariance() const {
        assert(!points.empty() && notNull(points.front()));

        // get the used dimension
        const int dim = points.front()->size();

        assert(dim>0);
        
        // total number of points
        const int n = static_cast<int>(points.size());

        // type for squares
        typedef typename typeInfo<value_type>::square_accumulation_type
          sqrType;

        // sum of all elements
        vector<sqrType> sum(dim,sqrType(0));
        // variance
        vector<sqrType> var(dim,sqrType(0));

        typename points_type::const_iterator it;
        int mxi,j;
        sqrType tmp,mx;

        // compute some temporary variables with the sum of elements and
        // the sum of square of elements
        for (it=points.begin();it!=points.end();++it) {
          // we cannot just add the points, because we don't really know
          // the type.  So let's add the elements separately
          for (j=0;j<dim;++j) {
            tmp=(*(*it))[j];
            sum[j]+=tmp;
            var[j]+=(tmp*tmp);
          }
        }

        // compute not the variance, but m x variance, to save some time,
        // and get the dimension where the maximal one is found
        mxi=0;
        tmp=sum[mxi];
        mx = var[mxi]-(tmp*tmp/n); // the first "variance" for dimension 0
        for (j=1;j<dim;++j) {      // the rest of "variances"
          tmp = sum[j];
          tmp = var[j]-(tmp*tmp/n);
          if (tmp>mx) {            // if greater
            mxi=j;                 // remember it.
            mx=tmp;
          }
        }

        return mxi;
      }

      /**
       * get the median value at the given search dimension
       *
       * @param searchDim search dimension
       * @return the median of the given dimension
       */
      value_type getMedianVal(const int searchDim) const {
        int i;
        const int n = static_cast<int>(points.size());

        vector<value_type> val(false,n); // don't initialize

        // get elements in the split dimension
        typename points_type::const_iterator it;
        for (i=0,it=points.begin(); it!=points.end();++it,++i) {
          val[i] = (*(*it))[searchDim];
        }

        // the quickMedian functor can be created just once and const,
        // because we don't need to change the parameters and the required
        // apply is const.  Even multi-thread applications would use different
        // stack-frames, and should work. (This should be tested!!!)
        static const quickMedian<value_type> qmed;

        // get the median of desired dimension
        return qmed.apply(val);
      }

      /**
       * search for exactly the given key in the list of elements.  If
       * found, return true, otherwise return false and leave the
       * element instance untouched.
       * 
       * For floating types this method usually returns false, because
       * the exact match of the key is very unprobable.  This method is
       * therefore used mostly for integer value_types 
       */
      bool getPoint(const T& key,element& elem) const {
        if (!points.empty()) {
          typename points_type::const_iterator it,eit;
          for (it=points.begin(),eit=points.end();it!=eit;++it) {
            if ((*it)->point == key) {
              elem=*(*it);
              return true;
            }
          }
        }
        return false;
      }

      /**
       * search for exactly the given key in the list of elements.  If
       * found, return true, otherwise return false and leave the
       * element instance untouched.
       * 
       * For floating types this method usually returns false, because
       * the exact match of the key is very unprobable.  This method is
       * therefore used mostly for integer value_types 
       */
      bool getPoint(const T& key,std::list<element>& elems) const {
        bool found=false;
        if (!points.empty()) {
          typename points_type::const_iterator it,eit;
          for (it=points.begin(),eit=points.end();it!=eit;++it) {
            if ((*it)->point == key) {
              elems.push_back(*(*it));
              found=true;
            }
          }
        }
        return found;
      }

      /**
       * insert all elements in the given list into the internal
       * elements vector.
       *
       * The pointed elements will be inserted as they are (without copying),
       * and the memory managment is controled by this node itself.
       *
       * The dimensionality of each element MUST be equal than the one
       * of the first added element.
       */
      void add(std::list<element*>& pts) {
        typename std::list<element*>::iterator it=pts.begin();
        while (it!=pts.end()) {
          points.push_back(*it);
          ++it;
        }
      }

    };  // end of node class
  
    // ----------------------------------------------------------------------
    // kdTree Definition
    // ----------------------------------------------------------------------

  public:

    /**
     * Return type of the size() member
     */
    typedef int size_type;

    /**
     * Default constructor
     */
    kdTree();

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    kdTree(const kdTree& other);

    /**
     * Destructor
     */
    virtual ~kdTree();

    /**
     * Returns the name of this type ("kdTree")
     */
    virtual const char* getTypeName() const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kdTree& copy(const kdTree& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kdTree& operator=(const kdTree& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual mathObject* clone() const;

    /**
     * Clear the tree.
     *
     * All elements belonging to the tree will be removed.  Also all elements
     * added until now will be deleted.
     */
    virtual void clear();

    /**
     * Check if the tree is empty
     */
    virtual bool empty() const;

    /**
     * Get the number of elements added to the tree with the function add().
     *
     * Note that these points are not really \b in the tree until you
     * call build() or rebuild(). After calling either the added
     * points are flushed and the return value of this function is
     * zero.
     *
     * @see size()
     */
    virtual int getNumberOfAddedElements() const;

    /**
     * Get the number of elements in the built tree.
     *
     * Note that this value can differ substantially from the value
     * returned by getNumberOfAddedElements(). The latter don't belong
     * to the tree yet. Thus, adding elements, then calling build and
     * then adding more elements, generally results in two different
     * values.
     *
     * The return value is zero if no tree has been built yet.
     *
     * @see getNumberOfAddedElements()
     */
    virtual int size() const;

    /**
     * Get the number of leaves in the tree.
     *
     * Return zero if the tree hasn't been build yet.
     */
    virtual int getNumberOfLeaves() const;

    /**
     * Get the number of levels of the tree.
     *
     * Return zero if the tree hasn't been build yet.
     */
    virtual int getLevels() const;

    /**
     * Get pointer to root node
     *
     * Return a null pointer if the tree hasn't been build yet.
     */
    virtual node* getRoot();

    /**
     * @name Search methods
     */
    //@{

    /**
     * Search for an element with exactly the given position in the
     * hyperspace.
     *
     * This is the classical search in a kd-tree, that assumes that 
     * a leaf node contains exactly the given point.
     * If found, the contents of the element will by copied in the 
     * elem attributes and true will be returned.   Otherwise false will
     * be returned.
     *
     * If the key is present more than once, only the "first" one will
     * be returned, which is the first one the list of points at the
     * left-most node containing it.
     *
     * @param key point in the n-dimensional space you are looking for.
     * @param elem the contents of the found point will be left here.
     * @return true if key was found, otherwise false.
     */
    bool searchExactly(const T& key,element& elem) const;

    /**
     * Search for all elements with exactly the given position in the
     * hyperspace.
     *
     * This is the classical search in a kd-tree, that assumes that 
     * a leaf node contains exactly the given point.
     * If found, the contents of the element will by copied in the 
     * elem attributes and true will be returned.   Otherwise false will
     * be returned.
     *
     * @param key point in the n-dimensional space you are looking for.
     * @param elems a list containing copies to all found elements with the
     *              given keys
     * @return true if at least one key was found, otherwise false.
     */
    bool searchExactly(const T& key,std::list<element>& elems) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     *
     * If found, a pointer to the element will be written in the 
     * \a elem parameters and true will be returned.   Otherwise false will
     * be returned and the pointer will be set to zero.
     *
     * The pointer is not const to allow you to change the \a data of the
     * element, but if you also change the \a point, the tree will be corrupted
     * and will not work appropriately.  There is no way to change the search
     * keys dynamically in a kd-tree without a heavy load of computation.  If
     * that is what you are looking for, you should consider rebuilding the
     * entire tree (expensive!).
     *
     * @param key point in the n-dimensional space you are looking for.
     * @param elem pointer to the contents of the point found, or zero if
     *             not found.  You should \b never delete the pointed data.
     * @return true if key was found, otherwise false (i.e. tree empty).
     *
     * \warning This method is not thread safe.  In order to provide a
     *          fast search mechanism for the nearest neighbor some
     *          temporary variables are stored in the tree class itself,
     *          and therefore it is not possible to search the
     *          nearest neighbor in the same tree from different
     *          threads.
     */
    bool searchNearest(const T& key,element*& elem) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     *
     * If found, a pointer to the element will be written in the 
     * \a elem parameters and true will be returned.   Otherwise false will
     * be returned and the pointer will be set to zero.
     *
     * The pointer is not const to allow you to change the \a data of the
     * element, but if you also change the \a point the tree will be corrupted
     * and will not work appropriately.  There is no way to change the search
     * keys dynamically in a kd-tree without a heavy load of computation.  If
     * that is what you are looking for, you should consider rebuilding the
     * entire tree (expensive!).
     *
     * @param key point in the n-dimensional space you are looking for.
     * @param elem pointer to the contents of the point found, or zero if
     *             not found.  You should \b never delete the pointed data.
     * @param dist distance between the points.  The type of distance is 
     *             determined by the used distanctor (the third template
     *             type of the tree, which return per default the square
     *             of the euclidean distance).
     * @return true if key was found, otherwise false (i.e. tree empty).
     *
     * \warning This method is not thread safe.  In order to provide a
     *          fast search mechanism for the nearest neighbor some
     *          temporary variables are stored in the tree class itself,
     *          and therefore it is not possible to search the
     *          nearest neighbor in the same tree from different
     *          threads.
     */
    bool searchNearest(const T& key,element*& elem,acc_type& dist) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     *
     * If found, the contents of the element will by copied in the 
     * elem parameters and true will be returned.   Otherwise false will
     * be returned.
     * @param key point in the n-dimensional space you are looking for.
     * @param elem the contents of the found point will be left here.
     * @return true if key was found, otherwise false (i.e. tree empty).
     *
     * \warning This method is not thread safe.  In order to provide a
     *          fast search mechanism for the nearest neighbor some
     *          temporary variables are stored in the tree class itself,
     *          and therefore it is not possible to search the
     *          nearest neighbor in the same tree from different
     *          threads.
     */
    bool searchNearest(const T& key,element& elem) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     *
     * If found, the contents of the element will by copied in the 
     * elem parameters and true will be returned.   Otherwise false will
     * be returned.
     * @param key point in the n-dimensional space you are looking for.
     * @param elem the contents of the found point will be left here.
     * @param dist distance between the points.  The type of distance is 
     *             determined by the used distanctor (the third template
     *             type of the tree, which return per default the square
     *             of the euclidean distance).
     * @return true if key was found, otherwise false (i.e. tree empty).
     *
     * \warning This method is not thread safe.  In order to provide a
     *          fast search mechanism for the nearest neighbor some
     *          temporary variables are stored in the tree class itself,
     *          and therefore it is not possible to search the
     *          nearest neighbor in the same tree from different
     *          threads.
     */
    bool searchNearest(const T& key,element& elem,acc_type& dist) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     *
     * If found, the contents of the element will by copied in the 
     * elem parameters and true will be returned.   Otherwise false will
     * be returned.
     * @param key point in the n-dimensional space you are looking for.
     * @param data the contents of the found nearest point will be left here.
     * @return true if key was found, otherwise false (i.e. tree empty).
     *
     * \warning This method is not thread safe.  In order to provide a
     *          fast search mechanism for the nearest neighbor some
     *          temporary variables are stored in the tree class itself,
     *          and therefore it is not possible to search the
     *          nearest neighbor in the same tree from different
     *          threads.
     */
    bool searchNearest(const T& key,D& data) const;

    /**
     * Search for the nearest k elements in the tree to the given key point.
     *
     * If you are looking just for the nearest elements (i.e. k=1) use the
     * searchNearest(const T&,D&) or searchNearest(const T&,element&) methods
     * instead.  They are optimized for this case and are much faster.
     *
     * @param k number of elements you are looking for
     * @param key the point in the n-dimensional space you are looking for
     * @param neighbors contains the pointers to the found elements.  You
     *                  should NEVER delete these elements.
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchNearest(const int k,
                       const T& key,
                       std::list<element*>& neighbors) const;
    
    /**
     * Search for the nearest k elements in the tree to the given key point.
     *
     * If you are looking just for the nearest elements (i.e. k=1) use the
     * searchNearest(const T&,D&) or searchNearest(const T&,element&) methods
     * instead.  They are optimized for this case and are much faster.
     *
     * @param k number of elements you are looking for.
     * @param key the point in the n-dimensional space you are looking for.
     * @param neighbors is a multimap containing as access key the
     *                  square of the euclidean distance or the result of
     *                  the given distantor between the
     *                  corresponding element* and the search key.
     *                  Remember that if you iterate the multimap, the
     *                  elements will be sorted in increasing order of
     *                  the distance (i.e. the nearest element first).
     *                  You should NEVER delete the pointed elements.
     * @return true if k elements were found, or false, if the tree contains
     *         less than k elements
     */
    bool searchNearest(const int k,
                       const T& key,
                       std::multimap<acc_type,element*>& neighbors) const;

    /**
     * Search for elements in an hyper-spheric neighbourhood of a given key
     * point, i.e. search for all elements that reside within a hypersphere
     * with the given radius centered at the given key.  Note that the
     * hypersphere is always defined using an Euclidean distance, no matter
     * of the specified distanctor policy.
     *
     * The returned list of elements is not sorted.  If you need it sorted
     * you should use the searchWithin() method with the multimap result.
     *
     * @param key point at which the hypersphere must be centered
     * @param dist allowed distance from the key point.  Note that this
     *             represents exaclty the radius of the hypersphere and not
     *             the square of it.
     * @param elems list of elements found.  You should NEVER delete the
     *              pointed elements.
     */
    bool searchWithin(const T& key,
		      const acc_type& dist,
		      std::list<element*>& elems) const;
      

    /**
     * Search for elements in an hyper-spheric neighbourhood of a given key
     * point, i.e. search for all elements that reside within a hypersphere
     * with the given radius centered at the given key.  Note that the
     * hypersphere is always defined using an Euclidean distance, no matter
     * of the specified distanctor policy.
     *
     * The returned multimap is always sorted in ascending order of
     * the distances (this is a property of all std::multimaps).  If you
     * want to save some time and don't need the sorted data consider using
     * the other searchWithin() method, which stores the result in a std::list.
     *
     * @param key point at which the hypersphere must be centered
     * @param dist allowed distance from the key point.  Note that this
     *             represents exaclty the radius of the hypersphere and not
     *             the square of it.
     * @param neighbors multimap of elements found.  You should NEVER delete
     *              the pointed elements.  Note that the keys used to sort
     *              the multimap are the square of the distances to the
     *              given key point.  This is done this way to save the
     *              time of applying the "sqrt()" function, which takes some
     *              time.
     */
    bool searchWithin(const T& key,
		      const acc_type& dist,
		      std::multimap<acc_type,element*>& neighbors) const;


    /**
     * Search the best-bin-first algorithm of Beis and Lowe.
     *
     * Use this method only if the dimensionality of your data is high
     * enough.  The time savings take effect only if the kd-tree is big
     * enough and the dimensionality of the data too.  Make a few tests
     * with your data in order to detect if this approximation is good enough
     * and fast enough for you.  Otherwise use the traditional search with
     * searchNearest()
     *
     * @param k number of elements you are looking for
     * @param key the point in the n-dimensional space you are looking for
     * @param neighbors contains the pointers to the found elements.  You
     *                  should NEVER delete these elements.
     * @param emax is the maximal number of visits allowed for leaf nodes.
     *             (in the original paper is called \f$E_{max}\f$).  Of course
     *             there can be more visits than emax if it is necessary to 
     *             find at least k neighbors.
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchBestBinFirst(const int k,
                            const T& key,
                            const int emax,
                            std::list<element*>& neighbors) const;


    /**
     * Search the best-bin-first algorithm of Beis and Lowe.
     *
     * Use this method only if the dimensionality of your data is high
     * enough.  The time savings take effect only if the kd-tree is big
     * enough and the dimensionality of the data too.  Make a few tests
     * with your data in order to detect if this approximation is good enough
     * and fast enough for you.  Otherwise use the traditional search with
     * searchNearest()
     *
     * @param k number of elements you are looking for
     * @param key the point in the n-dimensional space you are looking for
     * @param neighbors contains the pointers to the found elements.  You
     *                  should NEVER delete these elements.
     * @param emax is the maximal number of visits allowed for leaf nodes.
     *             (in the original paper is called \f$E_{max}\f$).
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchBestBinFirst(const int k,
                            const T& key,
                            const int emax,
                            std::multimap<acc_type,element*>& neighbors) const;

    /**
     * Search for all points lying within the given hyperbox.
     * @param boxMin point representing the lowest values at each coordinate
     *               building the lower limits of the bounding box.
     * @param boxMax point representing the highest values at each coordinate
     *               building the upper limits of the bounding box.
     * @param neighbors list of pointer to the elements found until now.
     * @return true if any element was found.
     */
    bool searchRange(const T& boxMin,
                     const T& boxMax,
                     std::list<element*>& neighbors) const;

    //@}

    /**
     * Consider the given element to be inserted in the tree after calling
     * the build(const int) method.
     *
     * The dimensionality of each point MUST be equal than the one
     * first added, otherwise an assertion will be thrown when building the 
     * tree.
     *
     * @param point n-dimensional point representint the position of
     *              this element * in the n-dimensional space
     * @param data data contained in this element.
     */
    void add(const T& point,const D& data);
 
    /**
     * Build the kd-Tree for all nodes added with the add() method, using
     * the given bucket size as the maximum number of elements a node
     * can contain.  The previous tree will be destroyed before the new one
     * is constructed.
     * the 
     * @param bucketSize maximum size of elements a node can contain (default
     *                   value is 1).
     * @return true if successful, false otherwise.
     */
    bool build(const int bucketSize = 1);

    /**
     * Rebuild the kd-Tree for all elements added with the add()
     * method and those already present in the tree, using the given
     * bucket size as the maximum number of elements a node can
     * contain. Although the previous tree will be destroyed before
     * the new one is constructed it will contain all elements
     * contained in the old one.
     *
     * Rebuilding the tree is quite slow since the elements have to be
     * extracted and copied from the existing tree first. This is
     * necessary since a kd-tree needs to know the complete set of
     * data for building.
     *  
     * @param bucketSize maximum size of elements a node can contain (default
     *                   value is 1).
     * @return true if successful, false otherwise.
     */
    bool rebuild(const int bucketSize = 1);

    /**
     * Read the parameters from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also read, otherwise only the data block will be read.
     * @return true if write was successful
     */
    virtual bool read(ioHandler &handler, const bool complete=true);

    /**
     * Write the parameters in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be
     *        written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,const bool complete=true) const;

  protected:
    /**
     * The root node
     */
    node *root;
    
    /**
     * Number of levels in the tree
     */
    int levels;

    /**
     * Number of elements contained in the tree
     */
    int numElements;

    /**
     * Number of elements added to the tree (see add())
     */
    int numAddedElements;

    /**
     * Number of leaf nodes in the tree
     */
    int numLeaves;

    /**
     * Bounding Box
     * After creating the tree with build(), this matrix will be a
     * 2 x n matrix containing at the first row the minimum possible values
     * for the current type, and the second row the maximum values.
     */
    matrix<value_type> totalBounds;

    /**
     * Elements that will be contained in the tree.
     * All nodes contain pointers to the elements objects, that will be
     * transfered to the nodes in the tree with the build method.  
     * Thereafter, this list will be empty.
     */
    std::list<element*> points;

    /** 
     * Instance of the policy class used for computing the distances.
     */ 
    U distantor;

  private:
    /**
     * Read recursivelly the points contained in the node
     */
    void getDataInSubtree(node* nodePtr,std::list<element*>& data) const;

    /**
     * Search for an element with exactly the given position in the
     * hyperspace.
     *
     * This is the classical search in a kd-tree, that assumes that 
     * a leaf node contains exactly the given point.
     * If found, the contents of the element will by copied in the 
     * elem attributes and true will be returned.   Otherwise false will
     * be returned, and the elem will be left unchanged.
     *
     * @param nptr node at which the search begins.
     * @param key point in the n-dimensional space you are looking for.
     * @param elem the contents of the found point will be left here.
     * @return true if key was found, otherwise false.
     */
    bool searchExactly(const node* nptr,const T& key,element& elem) const;

    /**
     * Search for all elements with exactly the given position in the
     * hyperspace.
     *
     * This is the classical search in a kd-tree, that assumes that 
     * a leaf node contains exactly the given point.
     * If found, the contents of the element will by copied in the 
     * elem attributes and true will be returned.   Otherwise false will
     * be returned.
     *
     * @param nptr node at which the search begins.
     * @param key point in the n-dimensional space you are looking for.
     * @param elems a list containing copies to all found elements with the
     *              given keys
     * @return true if at least one key was found, otherwise false.
     */
    bool searchExactly(const node* nptr,
                       const T& key,std::list<element>& elems) const;


    /**
     * Check if the hypersphere is within bounds.  This is called in
     * the original paper "ball within bounds).
     * It only returns true if the distance at each component between
     * the key and the bounds is greater than the given distance.
     * (dist expects here the square of the distance, as it has been
     * stored in the multimap).
     */ 
    inline bool 
    checkHypersphereWithinBounds(const T& key,
                                 const matrix<value_type>& bounds,
                                 const acc_type& dist) const;

    /**
     * Check if the bounds overlap the hypersphere.  It was called in
     * the original paper "bounds overlap ball".
     */
    inline bool 
    checkBoundsOverlapHypersphere(const T& key,
                                  const matrix<value_type>& bounds,
                                  const acc_type& dist) const;

    /**
     * Search for the nearest k elements in the tree to the given key point.
     * @param nptr pointer to the subtree where the elements need to be
     *             searched for.
     * @param k number of elements you are looking for
     * @param key the point in the n-dimensional space you are looking for
     * @param bounds hyperbox where the search is taking place
     * @param neighbors contains the pointers to the found elements.  You
     *                  should NEVER delete these elements.  As multimap
     *                  it is always sorted after the "acc_typed" key,
     *                  which is in this case the square distance of the
     *                  element to the key.
     * @param neighSize the number of the the neighbors found.
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchNearest(const node* nptr,
                       const int k,
                       const T& key,
                       matrix<value_type>& bounds,
                       std::multimap<acc_type,element*>& neighbors) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     * @param nptr pointer to the subtree where the elements need to be
     *             searched for.
     * @param key the point in the n-dimensional space you are looking for
     * @param bounds hyperbox where the search is taking place
     * @param neighbors contains the pointers to the found elements.  You
     *                  should NEVER delete these elements.  As multimap
     *                  it is always sorted after the "acc_typed" key,
     *                  which is in this case the square distance of the
     *                  element to the key.
     * @param neighSize the number of the the neighbors found.
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     *
     * \warning This method is not thread safe.  In order to provide a
     *          fast search mechanism for the nearest neighbor some
     *          temporary variables are stored in the tree class itself,
     *          and therefore it is not possible to search the
     *          nearest neighbor in the same tree from different
     *          threads.
     */
    bool searchNearest(const node* nptr,
                       const T& key,
                       std::pair<acc_type,element*>& neighbors) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     * @param nptr pointer to the node root of the subtree to be evaluated
     * @param key the point in the n-dimensional space you are looking for
     * @param dist distance radius to search point in
     * @param bounds hyperbox where the search is taking place
     * @param elems contains pointers to the found elements
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchWithin(const node* nptr,
		      const T& key,
		      const acc_type& dist,
		      matrix<value_type>& bounds,
		      std::list<element*>& elems) const;

    /**
     * Search for the nearest element in the tree to the given key point.
     * @param nptr pointer to the node root of the subtree to be evaluated
     * @param key the point in the n-dimensional space you are looking for
     * @param dist distance radius to search point in
     * @param bounds hyperbox where the search is taking place
     * @param elems contains pointers to the found elements
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchWithin(const node* nptr,
		      const T& key,
		      const acc_type& dist,
		      matrix<value_type>& bounds,
		      std::multimap<acc_type,element*>& neighbors) const;

    /**
     * Search for all points lying within the given hyperbox.
     * @param nptr pointer to the node root of the subtree to be evaluated
     * @param boxMin point representing the lowest values at each coordinate
     *               building the lower limits of the bounding box.
     * @param boxMax point representing the highest values at each coordinate
     *               building the upper limits of the bounding box.
     * @param neighbors list of pointer to the elements found until now.
     * @return true if completed.
     */
    bool searchRange(const node* nptr,
                     const T& boxMin,
                     const T& boxMax,
                     std::list<element*>& neighbors) const;

    /**
     * Check if a point lies withing the given bounding box.
     * @return true if point lies within the box, false otherwise.
     */
    inline bool withinBox(const T& boxMin,
                          const T& boxMax,
                          const T& key) const;
    
    /**
     * Check if a box lies withing the given bounding box.
     * @return true if box lies within the bbox, false otherwise.
     */
    inline bool withinBox(const matrix<value_type>& bbox,
                          const T& boxMin,
                          const T& boxMax) const;

    /**
     * Get the square of the L2 distance of the given point to the
     * node's hyperbox.
     *
     * If the point lies withing the hyperbox, the distance will be zero.
     *
     * @param na hyperbox representing the node's covered subspace.
     *           The first row represents the minimum and the second the
     *           maximum values.  It must be na.columns()==indexPoint.size()
     * @param indexPoint the point to be compared with the hyperbox.
     */
    inline acc_type minDistancePointToBox(const T& indexPoint,
                                          const matrix<value_type>& na) const;
    
    /**
     * Search the best-bin-first algorithm of Beis and Lowe.
     *
     * Use this method only if the dimensionality of your data is high
     * enough.  The time savings take effect only if the kd-tree is big
     * enough and the dimensionality of the data too.  Make a few tests
     * with your data in order to detect if this approximation is good enough
     * and fast enough for you.  Otherwise use the traditional search with
     * searchNearest()
     *
     * @param node pointer to the subtree where the elements need to be
     *             searched for.
     * @param k number of elements you are looking for
     * @param key the point in the n-dimensional space you are looking for
     * @param neighbors contains the pointers to the found elements.  You
     *                  should NEVER delete these elements.
     * @param emax maximum number of leaves to be visited.
     * @return true if k elements were found, or false, if the tree contains
     *             less than k elements
     */
    bool searchBestBinFirst(const node* root,
                            const int k,
                            const T& key,
                            std::multimap<acc_type,element*>& neighbors,
                            const int emax) const;

  private:
    /**
     * @name searchNearest Temporary variables
     *
     * The search of the nearest neighbors is optimized avoiding the
     * creation of many temporary variables.  
     */
    //@{
    
    /**
     * Minimum components of the bounding box.
     * It shares the memory with the first row of boundsBox.
     */
    mutable vector<value_type> boundsMin;

    /**
     * Maximum components of the bounding box
     * It shares the memory with the second row of boundsBox.
     */
    mutable vector<value_type> boundsMax;

    /**
     * Matrix containing the bounding box (first row min, second row max)
     */
    mutable matrix<value_type> boundsBox;
    //@}

  };

}

#include "ltiKdTree_template.h"

#endif


