/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003
 * Yossi Rubner, Computer Science Department, Stanford University
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
 * file .......: ltiEarthMoversDistance.h
 * authors ....: Yossi Rubner; Peter Doerfler
 * organization: CS, Stanford University; LTI, RWTH Aachen
 * creation ...: 29.10.2003
 * revisions ..: $Id: ltiEarthMoversDistance.h,v 1.7 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_EARTH_MOVERS_DISTANCE_H_
#define _LTI_EARTH_MOVERS_DISTANCE_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiGenericMatrix.h"
#include "ltiL2Distance.h"
#include "ltiDistanceFunctor.h"

#undef _LTI_DEBUG
//#define _LTI_DEBUG 2
#include "ltiDebug.h"

namespace lti {
  /**
   * The Earth Mover's Distance (EMD) is a robust distance measure
   * between two distributions. Each distribution has a number of
   * clusters n,m with corresponding weights. There is a ground
   * distance between two clusters which specifies the cost of moving
   * some weight from one cluster to the other. The EMD measures the
   * relative total cost of transforming one distribution into the
   * other by moving the weights around. This is done by solving the
   * transportation problem.
   *
   * Template type C specifies the type of the Cluster centers,
   * e.g. dvector or rgbPixel. Type W is the type of vector<W> which
   * is used for the weights of the clusters and template type D
   * specifies the ground distance to be used for the clusters. It
   * must be a Distantor<C> (see e.g. l2Distantor<T>.  Note that
   * instead of this calculation the cost matrix can be given in
   * parameters::costMatrix.
   *
   * There are four types of applys:
   * -# emd between two vector<W>
   * -# emd between two matrix<W>
   * -# emd between two matrix<C::value_type>, vector<W>
   * -# emd between two std::vector<C>, vector<W>
   *
   * The first two options are formulated in this way to achieve
   * compliance with the definition of distanceFunctor<T>. The cluster
   * centers used for the calculation of the cost matrix are taken to
   * be the indices of the vector/matrix. If this is not desired a
   * cost matrix must be given in the parameters since the Distantor D
   * is not used (due to possible type conflicts). Apply-types 3 and 4
   * use cluster centers and a weights vector. The cluster centers are
   * defined in two ways for convenience. Type 3 is more practical for
   * cluster centers that are C= vector<T> -like. Then the matrix<T>
   * contains cluster centers in the rows. For C=rgbPixel type 4 is
   * more convenient than first constructing a matrix<ubyte> for type
   * 3.
   *
   * Depending on the apply method used the flow between the two
   * distributions is also returned. The first distribution is indexed
   * in the rows the second in columns. Assuming to clusters for the
   * first and three clusters for the second distribution a flow
   * matrix could look like this:
   * \code
   * 0.3   0.1     0.4
   * 0     0       0.2
   * \endcode
   * From the first cluster of distribution1 there is a flow of 0.3 to
   * the first cluster, 0.1 to the second and 0.4 to the third cluster
   * of distribution2. From the second cluster of distribution1 there
   * is a flow of 0.2 to the third cluster of distribution2. From the
   * flow %matrix we can see that the weight vectors were [0.8 0.2]
   * and [0.3 0.1 0.6] for distributions one and two,
   * respectively. The total cost is calculated by summing up the
   * products between each flow and its cost.
   *
   * Note that neither the number of clusters nor the sum of weights
   * for the to distributions have to be equal. However, this gives
   * probably unwanted results for distances between vectors and matrices.
   *
   * Unlike other distanceFunctor<T> classes the EMD does not define a
   * norm on a distribution. All corresponding apply methods throw an
   * InvalidMethod exception.
   *
   * \b Note: This functor is \b not multi-thread safe!
   *
   * The core of this functor was implemented by Yossi Rubner,
   * Computer Science Department, Stanford University,
   * rubner@cs.stanford.edu,
   * http://vision.stanford.edu/~rubner. 
   *
   * Encapsulation and template support originally designed by Peter
   * Doerfler, LTI, RWTH-Aachnen.
   */
  template< class W, class C=vector<W>, class D=l2Distantor<C> >
  class earthMoversDistance : public distanceFunctor<W> {
  public:
    /**
     * the parameters for the class earthMoversDistance
     */
    class parameters : public distanceFunctor<W>::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : distanceFunctor<W>::parameters() {
        
        costMatrix = matrix<W>(0,0);
        epsilon = 1E-6;
        maxSteps = 500;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : distanceFunctor<W>::parameters() {
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
        return "earthMoversDistance::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        distanceFunctor<W>::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        distanceFunctor<W>::parameters& (distanceFunctor<W>::parameters::* p_copy)
          (const distanceFunctor<W>::parameters&) =
          distanceFunctor<W>::parameters::copy;
        (this->*p_copy)(other);
#     endif

        
        costMatrix.copy(other.costMatrix);
        epsilon = other.epsilon;
        maxSteps = other.maxSteps;
        
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
          
            lti::write(handler,"costMatrix",costMatrix);
            lti::write(handler,"epsilon",epsilon);
            lti::write(handler,"maxSteps",maxSteps);
          }

#     ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not able to
          // compile...
          b = b && distanceFunctor<W>::parameters::write(handler,false);
#     else
          bool (distanceFunctor<W>::parameters::* p_writeMS)(ioHandler&,
                                                             const bool) const =
            distanceFunctor<W>::parameters::writeMS;
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
          
            lti::read(handler,"costMatrix",costMatrix);
            lti::read(handler,"epsilon",epsilon);
            lti::read(handler,"maxSteps",maxSteps);
          }

#       ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && distanceFunctor<W>::parameters::read(handler,false);
#       else
          bool (distanceFunctor<W>::parameters::* p_readMS)(ioHandler&,
                                                            const bool) =
            distanceFunctor<W>::parameters::readMS;
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      

      /**
       * Usually the template type D (for distance) of the
       * earthMoversDistance is used to calculate the ground
       * distance. For some data type, espacially when comparing
       * feature vectors, it is difficult to find a distance measure
       * which describes the knowledge of relationship between the
       * elements of the feature vector correctly. For these cases
       * give the costMatrix explicitly here. The ground distance
       * measure will be disregarded for the calculations. Note that
       * for distances between vectors or matrices of weights the
       * diagonal is usually 0 (no cost to move weight between equal
       * indices). Default empty.
       */
      matrix<W> costMatrix;
      
      /**
       * A small value which is equal to the accepted rounding error
       * per operation. Default 1E-6.
       */
      W epsilon;

      /**
       * maximum number of steps to find the optimal flow with the
       * least cost. Default 500.
       */
      int maxSteps;
    };

    /**
     * value type of the cluster type C
     */
    typedef typename C::value_type cluster_value_type;

    /**
     * default constructor
     */
    earthMoversDistance();

    /**
     * Construct a functor using the given parameters
     */
    earthMoversDistance(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    earthMoversDistance(const earthMoversDistance& other);

    /**
     * destructor
     */
    virtual ~earthMoversDistance();

    /**
     * returns the name of this type ("earthMoversDistance")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    earthMoversDistance& copy(const earthMoversDistance& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    earthMoversDistance& operator=(const earthMoversDistance& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Calculation of norms not available for Earth Mover's Distance!
     * @throws lti::functor::invalidMethodException
     */
    virtual bool apply(const vector<W>& v, W& norm) const {
      throw invalidMethodException("Calculation of norms not available for Earth Mover's Distance!");
    };

    /**
     * Calculation of norms not available for Earth Mover's Distance!
     * @throws lti::functor::invalidMethodException
     */
    virtual W apply(const vector<W>& v) const {
      throw invalidMethodException("Calculation of norms not available for Earth Mover's Distance!");
    };

    /**
     * Calculation of norms not available for Earth Mover's Distance!
     * @throws lti::functor::invalidMethodException
     */
    virtual bool apply(const matrix<W>& m, vector<W>& norms) const {
      throw invalidMethodException("Calculation of norms not available for Earth Mover's Distance!");
    };

    /**
     * Calculation of norms not available for Earth Mover's Distance!
     * @throws lti::functor::invalidMethodException
     */
    virtual bool apply(const matrix<W>& m, W& norm) const {
      throw invalidMethodException("Calculation of norms not available for Earth Mover's Distance!");
    };

    /**
     * Calculation of norms not available for Earth Mover's Distance!
     * @throws lti::functor::invalidMethodException
     */
    virtual W apply(const matrix<W>& m) const {
      throw invalidMethodException("Calculation of norms not available for Earth Mover's Distance!");
    };


    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<W>
     * @param b the second vector<W>
     * @param dist the distance between the vectors
     * @return false on error -> see status string
     */
    virtual bool apply(const vector<W>& a, const vector<W>& b,
                       W& dist) const;

    /**
     * calculate the distance and the flow between the vectors a and b.
     * @param a the first vector<W>
     * @param b the second vector<W>
     * @param dist the distance between the vectors
     * @param flow the flow matrix for \a a and \a b
     * @return false on error -> see status string
     */
    virtual bool apply(const vector<W>& a, const vector<W>& b,
                       W& dist, matrix<W>& flow) const {
      bool ok=apply(a,b,dist);
      ok = ok && findFlow(a.size(), b.size(), flow);
      return ok;
    }

    /**
     * calculate the distance between the vectors a and b
     * @param a the first vector<W>
     * @param b the second vector<W>
     * @return the distance between the vectors or zero if calculation failed.
     */
    virtual W apply(const vector<W>& a, const vector<W>& b) const {
      W tmp;
      if (apply(a,b,tmp)) {
        return tmp;
      } else {
        return W(0);
      }
    };
    
    /**
     * calculate the distances between the rows or columns of the
     * matrices a and b, determined by the parameters rowWise.
     * @param a the first vector<W>
     * @param b the second vector<W>
     * @param dists the distances between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<W>& a, const matrix<W>& b,
                       vector<W>& dists) const;

    /**
     * Calculate the distance between each row or column of m
     * depending on the value of rowWise and the vector v.
     * @param m the matrix<W>
     * @param v the vector to be compared with
     * @param dest the vector with the distances to the vector v
     * @return false on error
     */
    virtual bool apply(const matrix<W>& m, const vector<W>& v,
                       vector<W>& dest) const;

    /**
     * calculate something like the distance between the matrices a and b:
     * both matrices are seen as vectors.
     * @param a the first matrix<W>
     * @param b the second matrix<W>
     * @param dist the 'distance' between the matrices
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<W>& a, const matrix<W>& b,
                       W& dist) const;

    /**
     * calculate something like the distance and the flow between the
     * matrices a and b: both matrices are seen as vectors.
     * @param a the first matrix<W>
     * @param b the second matrix<W>
     * @param dist the 'distance' between the matrices
     * @param flow the flow matrix for \a a and \a b
     * @return false on error -> see status string
     */
    virtual bool apply(const matrix<W>& a, const matrix<W>& b,
                       W& dist, matrix<W>& flow) const {
      bool ok=apply(a,b,dist);
      ok = ok && findFlow(a.rows()*a.columns(), b.rows()*b.columns(), flow);
      return ok;
    }

    /**
     * calculate something like the distance between the matrices a and b:
     * both matrices are seen as vectors.
     * @param a the first matrix<W>
     * @param b the second matrix<W>
     * @return the 'distance' between the matrices, if calculation fails
     *         0 is returned.
     */
    virtual W apply(const matrix<W>& a, const matrix<W>& b) const {
      W tmp;
      if (apply(a,b,tmp)) {
        return tmp;
      } else {
        return W(0);
      }

    };

    /**
     * calculate the EMD between to distributions each defined by its
     * clusters and the respective weights.
     * @param clusters1 each element of the vector contains a centroid of
     *                  a cluster of distribution 1
     * @param clusters2 each element of the vector contains a centroid of
     *                  a cluster of distribution 2
     * @param weights1 the weights corresponding to \a clusters1
     * @param weights2 the weights corresponding to \a clusters2
     * @param dist the EMD between the distributions
     * @return false if operation fails.
     */
    virtual bool apply(const std::vector<C>& clusters1,
                       const std::vector<C>& clusters2,
                       const vector<W>& weights1,
                       const vector<W>& weights2,
                       W& dist) const;

    /**
     * calculate the EMD and the flow between to distributions each
     * defined by its clusters and the respective weights.
     * @param clusters1 each element of the vector contains a centroid of
     *                  a cluster of distribution 1
     * @param clusters2 each element of the vector contains a centroid of
     *                  a cluster of distribution 2
     * @param weights1 the weights corresponding to \a clusters1
     * @param weights2 the weights corresponding to \a clusters2
     * @param dist the EMD between the distributions
     * @param flow the flow matrix for the two distributions
     * @return false if operation fails.
     */
    virtual bool apply(const std::vector<C>& clusters1,
                       const std::vector<C>& clusters2,
                       const vector<W>& weights1,
                       const vector<W>& weights2,
                       W& dist, matrix<W>& flow) const {
      bool ok=apply(clusters1, clusters2, weights1, weights2, dist);
      ok = ok && findFlow(weights1.size(), weights2.size(), flow);
      return ok;
    }

    /**
     * calculate the EMD between to distributions each defined by its
     * clusters and the respective weights.
     * @param clusters1 each element of the vector contains a centroid of
     *                  a cluster of distribution 1
     * @param clusters2 each element of the vector contains a centroid of
     *                  a cluster of distribution 2
     * @param weights1 the weights corresponding to \a clusters1
     * @param weights2 the weights corresponding to \a clusters2
     * @return the EMD between the distributions, if calculation fails
     *         0 is returned.
     */
    virtual W apply(const std::vector<C>& clusters1,
                    const std::vector<C>& clusters2,
                    const vector<W>& weights1,
                    const vector<W>& weights2) const {
      W tmp;
      if (apply(clusters1, clusters2, weights1, weights2, tmp)) {
        return tmp;
      } else {
        return W(0);
      }
    };

    /**
     * calculate the EMD between to distributions each defined by its
     * clusters and the respective weights.
     * @param clusters1 each row of the matrix contains a centroid of
     *                  a cluster of distribution 1
     * @param clusters2 each row of the matrix contains a centroid of
     *                  a cluster of distribution 2
     * @param weights1 the weights corresponding to \a clusters1
     * @param weights2 the weights corresponding to \a clusters2
     * @param dist the EMD between the distributions
     * @return false if operation fails.
     */
    virtual bool apply(const matrix<cluster_value_type>& clusters1,
                       const matrix<cluster_value_type>& clusters2,
                       const vector<W>& weights1,
                       const vector<W>& weights2,
                       W& dist) const;

    /**
     * calculate the EMD and the flow between to distributions each
     * defined by its clusters and the respective weights.
     * @param clusters1 each row of the matrix contains a centroid of
     *                  a cluster of distribution 1
     * @param clusters2 each row of the matrix contains a centroid of
     *                  a cluster of distribution 2
     * @param weights1 the weights corresponding to \a clusters1
     * @param weights2 the weights corresponding to \a clusters2
     * @param dist the EMD between the distributions
     * @param flow the flow matrix for the two distributions
     * @return false if operation fails.
     */
    virtual bool apply(const matrix<cluster_value_type>& clusters1,
                       const matrix<cluster_value_type>& clusters2,
                       const vector<W>& weights1,
                       const vector<W>& weights2,
                       W& dist, matrix<W>& flow) const {
      bool ok=apply(clusters1, clusters2, weights1, weights2, dist);
      ok = ok && findFlow(weights1.size(), weights2.size(), flow);
      return ok;
    }

    /**
     * calculate the EMD between to distributions each defined by its
     * clusters and the respective weights.
     * @param clusters1 each row of the matrix contains a centroid of
     *                  a cluster of distribution 1
     * @param clusters2 each row of the matrix contains a centroid of
     *                  a cluster of distribution 2
     * @param weights1 the weights corresponding to \a clusters1
     * @param weights2 the weights corresponding to \a clusters2
     * @return the EMD between the distributions, if calculation fails
     *         0 is returned.
     */
    virtual W apply(const matrix<cluster_value_type>& clusters1,
                    const matrix<cluster_value_type>& clusters2,
                    const vector<W>& weights1,
                    const vector<W>& weights2) const {
      W tmp;
      if (apply(clusters1, clusters2, weights1, weights2, tmp)) {
        return tmp;
      } else {
        return W(0);
      }
    };

  protected:

    /**
     * type for a single linked list
     */
    typedef struct single_node {
      int i;
      W val;
      single_node* next;
    } single_node;

    /**
     * type for a double linked list
     */
    typedef struct double_node {
      int i,j;
      W val;
      double_node* nextC;
      double_node* nextR;
    } double_node;

    /** the number of clusters in the first distribution */
    mutable int sz1;
    /** the number of clusters in the second distribution */
    mutable int sz2;

    /**
     * The cost matrix. If parameters::costMatrix is used, this matrix
     * uses the same data, else the costMatrix is calculated using the
     * ground distance D.
     */
    mutable matrix<W> costMatrix;

    /**
     * The maximum value in the costMatrix
     */
    mutable W maxCost;

    /**
     * The maximum of the total weights of supply and demand
     */
    mutable W maxWeight;

    /**
     * The minimum of the total weights of supply and demand
     */
    mutable W minWeight;

    /**
     * Datastructure containing the basic variables.
     */
    mutable double_node* basicVars;

    /**
     * Pointers from first distribution into the basicVars array.
     */
    mutable double_node** distr1Basic;

    /**
     * Pointers from second distribution into the basicVars array.
     */
    mutable double_node** distr2Basic;

    /** pointer for efficient handling of basicVars */
    mutable double_node* endBasicVars;

    /** pointer for efficient handling of basicVars */
    mutable double_node* enterBasicVars;

    /**
     * If true the two clusters i and j of the two distributions form
     * a basic variable.
     */
    mutable genericMatrix<bool> isBasicVar;

    /**
     * inits data structures. returns true if the sum of weights
     * (earth) is significantly different in the two distributions. If
     * this is the case the weight vector with less weight gets an
     * extra entry with the difference.
     */
    bool init(vector<W>& weights1, vector<W>& weights2) const;


    /**
     * Finds the costMatrix for the given supply and demand. 
     */
    bool calcCost(const ivector& supply, const ivector& demand) const;

    /**
     * Finds the costMatrix for the given supply and demand. 
     */
    bool calcCost(const vector<point>& supply,
                  const vector<point>& demand) const;

    /**
     * Finds the costMatrix for the given supply and demand. 
     */
    bool calcCost(const std::vector<C>& supply,
                  const std::vector<C>& demand) const;

    /**
     * Finds the costMatrix for the given supply and demand. 
     */
    bool calcCost(const matrix<cluster_value_type>& supply, 
                  const matrix<cluster_value_type>& demand) const;

    /**
     * Actual management of the calculation of the emd.  Parameters
     * are the two weights vectors as well as their original
     * lengths. The total flow is returned.
     */
    bool emd(vector<W>& weights1, vector<W>& weights2,
             const int& slen, const int& dlen, W& totalFlow) const;


    /**
     * Initial solution of the transportation problem on the given weights.
     */
    bool russel(vector<W>& weights1, vector<W>& weights2) const;

    /**
     * Helper method for russel
     */
    void addBasicVariable(int minI, int minJ, 
                          vector<W>& weights1, vector<W>& weights2,
                          single_node *PrevUMinI, single_node *PrevVMinJ,
                          single_node *UHead) const;

    /**
     * Extracts the basic variables from isBasisVar
     */
    void findBasicVariables(single_node *U, single_node *V) const;

    /**
     * Checks if the current solution is optimal.
     */
    int isOptimal(single_node *U, single_node *V) const;

    /**
     * Find a new/better solution to the transportation problem
     */
    void newSolution() const;
    
    /**
     * Tries to find a loop in the current solution
     */
    int findLoop(double_node **Loop) const;

    /**
     * Constructs a matrix containing the flow from distribution1 to
     * distribution2. Where the first is in rows and the second in
     * columns.
     */
    bool findFlow(const int& slen, const int& dlen, matrix<W>& flow) const;

#ifdef _LTI_DEBUG  
    void dumpVariables() const;
#endif
    
  };

}

#include "ltiUndebug.h"
#include "ltiEarthMoversDistance_template.h"

#endif

  
