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
 * file .......: ltiMinimumSpanningTreeOfKeytype.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.5.2003
 * revisions ..: $Id: ltiMinimumSpanningTreeOfKeytype.h,v 1.7 2004/05/03 12:20:01 alvarado Exp $
 */

#ifndef _LTI_MINIMUM_SPANNING_TREE_OF_KEYTYPE_H_
#define _LTI_MINIMUM_SPANNING_TREE_OF_KEYTYPE_H_

#include "ltiMinimumSpanningTree.h"
#include "ltiL2Distance.h"

namespace lti {
  /**
   * Computes a minimum spanning tree.
   * This class computes a minimum spanning tree from a matrix where each 
   * element in the matrix represents one key (data point), e.g. a matrix 
   * of rgbPixel (which is an image).
   */
  template <class K, class V=int, class Distantor=l2SquareDistantor<K> >
  class minimumSpanningTreeOfKeytype 
    : public minimumSpanningTree<K,V,Distantor> {
  public:

    /**
     * This class uses the same parameters as its base class
     */
    typedef typename minimumSpanningTree<K,V,Distantor>::parameters parameters;
    /**
     * Distance type
     */
    typedef typename 
    minimumSpanningTree<K,V,Distantor>::distance_type distance_type;

    /**
     * Key Value Type
     */
    typedef typename
    minimumSpanningTree<K,V,Distantor>::key_value_type key_value_type;

    /**
     * default constructor
     */
    minimumSpanningTreeOfKeytype();

    /**
     * Construct a functor using the given parameters
     */
    minimumSpanningTreeOfKeytype(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    minimumSpanningTreeOfKeytype(const minimumSpanningTreeOfKeytype& other);

    /**
     * destructor
     */
    virtual ~minimumSpanningTreeOfKeytype();

    /**
     * returns the name of this type ("minimumSpanningTreeOfKeytype")
     */
    virtual const char* getTypeName() const;

    /**
     * computes the minimum spanning tree of the given data in matrix
     * @param src the data that is used for building
     * @param datas data set
     * @param graph the tree will be returned here.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const matrix<K>& src,const matrix<V>& datas,
               weightedGraph<K,V,distance_type>& graph) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimumSpanningTreeOfKeytype& copy(const minimumSpanningTreeOfKeytype& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    minimumSpanningTreeOfKeytype& operator=(const minimumSpanningTreeOfKeytype& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#include "ltiMinimumSpanningTreeOfKeytype_template.h"

#endif

