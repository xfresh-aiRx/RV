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
 * file .......: ltiMulticlassNormalizedCuts.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 10.10.2003
 * revisions ..: $Id: ltiMulticlassNormalizedCuts.h,v 1.5 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_MULTICLASS_NORMALIZED_CUTS_H_
#define _LTI_MULTICLASS_NORMALIZED_CUTS_H_


#include "ltiMatrix.h"
#include "ltiVector.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * Multiclass normalized cuts.
   *
   * This class implements the k-ways normalized cuts algorithm proposed by
   * Stella Yu in:
   * 
   * S. Yu, "Computational Models of Perceptual Organization". Ph.D Thesis,
   *        Carnegie Mellon University, May 2003.
   *
   * The algorithm is general enough to be applied to any graph partitioning
   * problem given the affinity matrix w, which contains the weights between
   * each two nodes.  It must be symmetric (and obviously square) and its 
   * values must be greater of equal zero.
   *
   * The weights denote usually similarity measures (and not distances) between
   * two nodes.
   *
   * The output of the algorithm is an integer vector with labels for each of
   * the n nodes of the graph, corresponding to the indices of the vectors in
   * the affinity matrix.
   *
   * If you compiled the LTI-Lib with LAPACK support, this vector will work
   * much faster than without.  This is because it makes use of an
   * eigensolution functor and the singular value decomposition functor, which
   * are both efficiently computed by the LAPACK.
   */
  class multiclassNormalizedCuts : public functor {
  public:
    /**
     * the parameters for the class multiclassNormalizedCuts
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Number of classes the graph need to be separated into.
       * This value must be greater of equal 2.
       *
       * Default value: 2
       */
      int k;

    };

    /**
     * default constructor
     */
    multiclassNormalizedCuts();

    /**
     * Construct a functor using the given parameters
     */
    multiclassNormalizedCuts(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    multiclassNormalizedCuts(const multiclassNormalizedCuts& other);

    /**
     * destructor
     */
    virtual ~multiclassNormalizedCuts();

    /**
     * returns the name of this type ("multiclassNormalizedCuts")
     */
    virtual const char* getTypeName() const;

    /**
     * Compute the k-way partitioning of a graph with affinity matrix
     * "weights".
     *
     * @param weights the affinity matrix containing the weights between each
     *                two nodes of the graph.  This square matrix must also be
     *                symmetric.  The elements weights.at(i,j) denote the 
     *                similarity between the nodes i and j and must be positive
     *                or equal zero.  Please note that this are similarity 
     *                metrics and \b not distance metrics.
     *                In the original paper is denoted with W.
     * @param xstar   (X* in the paper) is a vector containing the label
     *                of each node.  Each values will be between 0 and k-1,
     *                where k is the value given in the parameters object.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& weights,
                     ivector& xstar) const;

    /**
     * Compute the k-way partitioning of a graph with affinity matrix
     * "weights".
     *
     * @param k       number of classes to be found, i.e. k subgraphs will be
     *                detected.
     * @param weights the affinity matrix containing the weights between each
     *                two nodes of the graph.  This square matrix must also be
     *                symmetric.  The elements must be positive or equal zero,
     *                and correspond to similarity metrics (not distance 
     *                metrics).  In the original paper is denoted with W
     * @param xstar   (X* in the paper) is a vector containing the label
     *                of each node.  The values will be between 0 and k-1 where
     *                k is the value given as argument (the value k in the
     *                parameters object will  be ignored).
     * @return true if apply successful or false otherwise.
     */
    bool apply(const int k,
               const dmatrix& weights,
                     ivector& xstar) const;

    /**
     * Compute the k-way partitioning of a graph with affinity matrix
     * "weights".
     *
     * @param k       number of classes to be found, i.e. k subgraphs will be
     *                detected.
     * @param weights the affinity matrix containing the weights between each
     *                two nodes of the graph.  This square matrix must also be
     *                symmetric.  The elements must be positive or equal zero,
     *                and correspond to similarity metrics (not distance 
     *                metrics).  In the original paper is denoted with W
     * @param degree  the degree vector, defined as the sum of elements of each
     *                row of the weights.
     * @param xstar   (X* in the paper) is a vector containing the label
     *                of each node.  The values will be between 0 and k-1 where
     *                k is the value given as argument (the value k in the
     *                parameters object will  be ignored).
     * @return true if apply successful or false otherwise.
     */
    bool apply(const int k,
               const dmatrix& weights,
               const dvector& degree,
                     ivector& xstar) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    multiclassNormalizedCuts& copy(const multiclassNormalizedCuts& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    multiclassNormalizedCuts& operator=(const multiclassNormalizedCuts& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


  protected:
    /**
     * Merge regions using the "k-Way normalized cuts" method of Stella Yu
     * (See S. Yu "Computational Models of Perceptual Organization" Tech. 
     * Report CMU-RI-TR-03-14, Carnegie Mellon University, May 2003.
     *
     * The main difference with the original paper is that the graph is not
     * formed of single connected pixels, but regions using a simpler faster
     * method to over-partition the image.
     * 
     * The resulting regions mask will contain only \a k labels.
     *
     * @param W the weights matrix (or affinity matrix) containing the 
     *          weight (similarity and NOT distance) between each to nodes
     *          of the graph representation.
     * @param D degree vector containing the sum of elements of each row of W
     * @param k number of classes the final partition should have.
     * @param Xstar the resulting vector contains for each corresponding node
     *              a label name (from 0 to k-1) for the corresponding label.
     */
    bool kWayNormalizedCut(const dmatrix& W,
                           const dvector& D,
                           const int k,
                           ivector& Xstar) const;
   
    /**
     * Compute the degree vector of the given affinity matrix W.
     * It is defined as the sum of the rows.
     */
    void computeDegree(const dmatrix& W, dvector& D) const;

  };
}

#endif

