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
 * file .......: ltiSequentialForwardSearch.h
 * authors ....: Jens Paustenbach
 * organization: LTI, RWTH Aachen
 * creation ...: 8.7.2002
 * revisions ..: $Id: ltiSequentialForwardSearch.h,v 1.4 2004/05/03 12:17:11 alvarado Exp $
 */

#ifndef _LTI_SEQUENTIAL_FORWARD_SEARCH_H_
#define _LTI_SEQUENTIAL_FORWARD_SEARCH_H_

//TODO: include only those files which are needed in this header!! 

#include "ltiVector.h"
#include "ltiMatrix.h"
#include "ltiPlusLTakeAwayR.h"

namespace lti {
  /**
   *  This class implements the sequential forward search algorithm for 
   *  feature selection. It is based on the PlusRTakeAwayR Algorithm with 
   *  parameters r=0 and l=1. It starts with an empty destination matrix. 
   *  Each iteration inserts the most significant feature from the source 
   *  data set into the destination until the predetermined number of features
   *  is reached.
   */
  class sequentialForwardSearch : public plusLTakeAwayR {
  public:
    /**
     * default constructor
     */
    sequentialForwardSearch();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    sequentialForwardSearch(const sequentialForwardSearch& other);

    /**
     * destructor
     */
    virtual ~sequentialForwardSearch();

    /**
     * returns the name of this type ("sequentialForwardSearch")
     */
    virtual const char* getTypeName() const;
  

    /**
     * copy the nbFeatures best features to the dest matrix
     * @param src dmatrix with the source data.
     * @param srcIds ids of the corresponding rows of src
     * @param dest dmatrix where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dmatrix& src,const ivector& srcIds,
               dmatrix& dest) ;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sequentialForwardSearch& copy(const sequentialForwardSearch& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    sequentialForwardSearch& operator=(const sequentialForwardSearch& other);

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



