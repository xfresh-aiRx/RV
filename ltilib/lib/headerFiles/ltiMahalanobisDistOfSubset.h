/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002
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
 * file .......: ltiMahalanobisDistOfSubset.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 22.10.2002
 * revisions ..: $Id: ltiMahalanobisDistOfSubset.h,v 1.4 2004/05/03 12:17:11 alvarado Exp $
 */

#ifndef _LTI_MAHALANOBIS_DIST_OF_SUBSET_H_
#define _LTI_MAHALANOBIS_DIST_OF_SUBSET_H_

#include "ltiCostFunction.h"
#include <list>
#include <map>

namespace lti {
  /**
   * This class implements a cost function for evaluating a subset of features
   * based on the mahalanobis distance.
   * A great value of this cost function shows well seperated clusters. First,
   * for all points in the data set the mahalanobis distance to each 
   * cluster in distribution is computed. Then the result of this 
   * costFunction is the average of all these distances.
   * Before you can compute the cost function use the setSrc method to
   * pass the complete data set with all features to this class. Later 
   * the features are accessed only by their column number.
   */
  class mahalanobisDistOfSubset : public costFunction {
  public:

    /**
     * default constructor
     */
    mahalanobisDistOfSubset();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    mahalanobisDistOfSubset(const mahalanobisDistOfSubset& other);

    /**
     * destructor
     */
    virtual ~mahalanobisDistOfSubset();

    /**
     * returns the name of this type ("mahalanobisDistOfSubset")
     */
    virtual const char* getTypeName() const;
  
    /**
     * Set the src data, that is used for computation. This must be done 
     * before apply methods are called. Later each feature can be adressed 
     * by its column number, because usually a column represents a feature.
     */
    virtual void setSrc(const dmatrix& src, const ivector& srcIds);

    /**
     * computes the mahalanobis distance with the given subset of features
     * @param value the result of the cost function.
     * @param in std::list with the features that should be inclueded
     *           for computing the function
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::list<int>& in,double& value);
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mahalanobisDistOfSubset& copy(const mahalanobisDistOfSubset& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mahalanobisDistOfSubset& operator=(const mahalanobisDistOfSubset& other);

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
     *  A list with each element containing a matrix with one cluster.
     *  This list is build from the contructor.
     */
    list<dmatrix> clusters;


  };
}

#endif
