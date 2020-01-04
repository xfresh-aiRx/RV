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
 * file .......: ltiSequentialBackwardSearch.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.7.2002
 * revisions ..: $Id: ltiSequentialBackwardSearch.h,v 1.4 2004/05/03 12:17:11 alvarado Exp $
 */

#ifndef _LTI_SEQUENTIAL_BACKWARD_SEARCH_H_
#define _LTI_SEQUENTIAL_BACKWARD_SEARCH_H_

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiPlusLTakeAwayR.h"

namespace lti {
  /**
   *  This class implements the sequential backward search algorithm for 
   *  feature selection. It is based on the PlusRTakeAwayR algorithm with 
   *  parameters r=1 and l=0. It starts with all features from the source data
   *  an in each iteration the least significant feature is deleted from 
   *  the destination matrix until the predetermined number of features is
   *  reached.
   */
  class sequentialBackwardSearch : public plusLTakeAwayR {
  public:
    /**
     * default constructor
     */
    sequentialBackwardSearch();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    sequentialBackwardSearch(const sequentialBackwardSearch& other);

    /**
     * destructor
     */
    virtual ~sequentialBackwardSearch();

    /**
     * returns the name of this type ("sequentialBackwardSearch")
     */
    virtual const char* getTypeName() const;
  
    /**
     * selects the nbFeatures best features from src and puts them leave 
     * them in dest.
     * @param src dmatrix with the source data.
     * @param srcIds ivector with the ids corresponding to the point in src
     * @param dest dmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,const ivector& srcIds,dmatrix& dest);
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sequentialBackwardSearch& copy(const sequentialBackwardSearch& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sequentialBackwardSearch& operator=(const sequentialBackwardSearch& other);

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



