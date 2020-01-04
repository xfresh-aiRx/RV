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
 * file .......: ltiBhattacharyyaDistOfSubset.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 22.10.2002
 * revisions ..: $Id: ltiBhattacharyyaDistOfSubset.h,v 1.4 2004/05/03 12:17:11 alvarado Exp $
 */

#ifndef _LTI_BHATTACHARYYA_DIST_OF_SUBSET_H_
#define _LTI_BHATTACHARYYA_DIST_OF_SUBSET_H_

#include "ltiCostFunction.h"
#include <map>
#include <list>

namespace lti {
  /**
   *  The class implements the bhattacharyya distance as cost function. The 
   *  cost function evaluates a subset of features from a dataset.
   *  First you have to initialize this class with the hole data set 
   *  and the ids to which cluster each data point belongs. To do 
   *  this you have to call the setSrc method. This must be done before 
   *  you can compute the cost function.
   *  Then you can choose a subset of features. The cost function then will be 
   *  the average bhattacharyya distance of all clusters.
   */
  class bhattacharyyaDistOfSubset : public costFunction {
  public:
    /**
     * default constructor
     */
    bhattacharyyaDistOfSubset();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    bhattacharyyaDistOfSubset(const bhattacharyyaDistOfSubset& other);

    /**
     * destructor
     */
    virtual ~bhattacharyyaDistOfSubset();

    /**
     * returns the name of this type ("bhattacharyyaDistOfSubset")
     */
    virtual const char* getTypeName() const;
  
    /**
     * Set the src data, that is used for the cost function. 
     * This must be done before you call one of the apply methods. 
     * Later each feature can be adressed by its column number in this src 
     * matrix.
     */
    virtual void setSrc(const dmatrix& src, const ivector& srcIds);

    /**
     * computes the bhattacharyya distance with the given subset of features
     * @param in std::list with the features that should be included
     *           for computing the function
     * @param dist the result of the cost function.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::list<int>& in,double& dist);
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bhattacharyyaDistOfSubset& copy(const bhattacharyyaDistOfSubset& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bhattacharyyaDistOfSubset& operator=(const bhattacharyyaDistOfSubset& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

 protected:
    /** 
     * Each element of the list contains all data points belonging to one 
     * cluster.
     */
    std::list<dmatrix> clusters;
  };
}

#endif



