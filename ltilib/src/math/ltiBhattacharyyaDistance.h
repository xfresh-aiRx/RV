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
 * file .......: ltiBhattacharyyaDistance.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 23.8.2002
 * revisions ..: $Id: ltiBhattacharyyaDistance.h,v 1.4 2004/05/03 12:20:01 alvarado Exp $
 */

#ifndef _LTI_BHATTACHARYYA_DISTANCE_H_
#define _LTI_BHATTACHARYYA_DISTANCE_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiVarianceFunctor.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   *  Computes the bhattacharyya distance of two distributions.
   *  Either two matrices with the distributions, with each row representing a
   *  data point, or the means and the covarianz matrices of both distributions
   *  are given the the functor.
   */
  class bhattacharyyaDistance : public linearAlgebraFunctor {
  public:
    /**
     * default constructor
     */
    bhattacharyyaDistance();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    bhattacharyyaDistance(const bhattacharyyaDistance& other);

    /**
     * destructor
     */
    virtual ~bhattacharyyaDistance();

    /**
     * returns the name of this type ("bhattacharyyaDistance")
     */
    virtual const char* getTypeName() const;
  
    /**
     * compute the distance between the two distributions 
     * @param cluster1 dmatrix with the first distribution
     * @param cluster2 dmatrix with the second one
     * @return the distance
     */
    double apply(const dmatrix& cluster1,const dmatrix& cluster2) const;

    /**
     * compute the distance between the two distributions 
     * @param cluster1 dmatrix with the first distribution
     * @param cluster2 dmatrix with the second one
     * @param distance the result will be left here
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& cluster1,const dmatrix& cluster2,
               double& distance) const;

    /**
     * compute the distance between the two distributions 
     * @param centroid1 dvector with the first centroid
     * @param cov1 dmatrix with the covariance matrix of distribution 1
     * @param centroid2 dvector with the second centroid
     * @param cov2 dmatrix with the covariance matrix of distribution 2
     * @return the distance
     */
    double apply(const dvector& centroid1,const dmatrix& cov1,
                 const dvector& centroid2,const dmatrix& cov2) const;

    /**
     * compute the distance between the two distributions 
     * @param centroid1 dvector with the first centroid
     * @param cov1 dmatrix with the covariance matrix of distribution 1
     * @param centroid2 dvector with the second centroid
     * @param cov2 dmatrix with the covariance matrix of distribution 2
     * @param distance the result will be left here
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& centroid1,const dmatrix& cov1,
               const dvector& centroid2,const dmatrix& cov2,
               double& distance) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bhattacharyyaDistance& copy(const bhattacharyyaDistance& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    bhattacharyyaDistance& operator=(const bhattacharyyaDistance& other);

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

#endif




