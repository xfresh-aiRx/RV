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
 * file .......: ltiCostFunction.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 4.9.2002
 * revisions ..: $Id: ltiCostFunction.h,v 1.3 2004/05/03 12:17:11 alvarado Exp $
 */

#ifndef _LTI_COST_FUNCTION_H_
#define _LTI_COST_FUNCTION_H_

#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"
#include <list> 

using std::list;

namespace lti {
  /**
   * Base class for all cost functions.
   * The cost functions are used to evaluate feature subsets of data sets.
   * These cost functions are used by the feature selectors.
   */
  class costFunction : public functor   {
  public:

    /**
     * default constructor
     */
    costFunction();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    costFunction(const costFunction& other);

    /**
     * destructor
     */
    virtual ~costFunction();

    /**
     * returns the name of this type ("costFunction")
     */
    virtual const char* getTypeName() const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    costFunction& copy(const costFunction& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    costFunction& operator=(const costFunction& other);

    /**
     * Set the src data points and ids used for computation of 
     * the cost function.
     */
    virtual void setSrc(const dmatrix& src, const ivector& srcIds);

    /**
     * computes the cost function with the given subset of features
     * @param in ivector with the features that should be included.
     * @param dist the result of the cost function.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const ivector& in,double& dist);

    /**
     * computes the cost function with the given subset of features
     * @param in ivector with the features that should be included
     *           for computing the function
     * @return the result of the cost function
     */
   virtual double apply(const ivector& in);

    /**
     * computes the cost function with the given subset of features
     * @param in std::list with the features that should be inclueded
     *           for computing the function
     * @return the result of the cost function
     */
    virtual double apply(const std::list<int>& in);

    /**
     * computes the cost function with the given subset of features
     * @param in std::list with the features that should be inclueded
     *           for computing the function
     * @param value the result of the cost function
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const std::list<int>& in,double& value)=0;


    protected:
      /** 
       * the source data points
       */
      dmatrix pSrc;

      /** 
       * the src ids corresponding to the data points
       */
      ivector pSrcIds; 
  };
}

#endif








