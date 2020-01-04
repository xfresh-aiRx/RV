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
 * file .......: ltiMahalanobisDistance.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 13.8.2002
 * revisions ..: $Id: ltiMahalanobisDistance.h,v 1.4 2004/05/03 12:20:01 alvarado Exp $
 */

#ifndef _LTI_MAHALANOBIS_DISTANCE_H_
#define _LTI_MAHALANOBIS_DISTANCE_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiVarianceFunctor.h"
#include "ltiLinearAlgebraFunctor.h"

namespace lti {
  /**
   *  This class computes the mahalanobis distance.
   *  The distance can either be computed as distance between a point and
   *  a matrix with a distribution or direct from two points and a given 
   *  covariance matrix, where one point is expected to be the mean of the 
   *  distribution.<p>
   *  The mahalanobis distance is defined as follows:
   *  \f[ d=\sqrt{(z_1-z_2)^T\Sigma^{-1}(z_1-z_2)} \f]
   *  where \f$ \Sigma \f$ is the covariance matrix of the distribution.
   */
  class mahalanobisDistance : public linearAlgebraFunctor {
  public:
    /**
     * default constructor
     */
    mahalanobisDistance();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    mahalanobisDistance(const mahalanobisDistance& other);

    /**
     * destructor
     */
    virtual ~mahalanobisDistance();

    /**
     * returns the name of this type ("mahalanobisDistance")
     */
    virtual const char* getTypeName() const;
  
    /**
     * computes the mahalanobis distance between the point p1 and 
     * the given distribution
     * @param p1 the point whose distance to the distribution is computed.
     * @param p2 the mean of the distribution .
     * @param coV the covarianz matrix of the distribution .
     * @param distance the mahalanobis distance.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& p1,const dvector& p2, const dmatrix& coV, 
               double& distance) const;

    /**
     * computes the mahalanobis distance between the point p1 and 
     * the given distribution
     * @param p1 the point whose distance to the distribution is computed.
     * @param p2 the mean of the distribution .
     * @param coV the covarianz matrix of the distribution .
     * @return the mahalanobis distance.
     */
    double apply(const dvector& p1,const dvector& p2,
                 const dmatrix& coV) const;

    /**
     * computes the mahalanobis distance between the point p1 and 
     * the given distribution
     * @param p1 the point whose distance to the distribution is computed.
     * @param cluster the distribution.
     * @param distance the mahalanobis distance.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& p1,const dmatrix& cluster, 
               double& distance) const;

    /**
     * computes the mahalanobis distance between the point p1 and 
     * the given distribution
     * @param p1 the point whose distance to the distribution is computed.
     * @param cluster the mean of the distribution .
     * @return the mahalanobis distance.
     */
    double apply(const dvector& p1,const dmatrix& cluster) const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mahalanobisDistance& copy(const mahalanobisDistance& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mahalanobisDistance& operator=(const mahalanobisDistance& other);

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



