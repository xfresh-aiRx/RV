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
 * file .......: ltiLinearMixer.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 21.1.2002
 * revisions ..: $Id: ltiLinearMixer.h,v 1.7 2005/02/02 14:16:23 doerfler Exp $
 */

#ifndef _LTI_LINEAR_MIXER_H_
#define _LTI_LINEAR_MIXER_H_

#include <vector>
#include "ltiFunctor.h"
#include "ltiMatrix.h"
#include "ltiVector.h"


namespace lti {
  /**
   * Implements a mixing operator for vectors and matrices. Mixing
   * means that you have a mixing matrix M and a list of vectors
   * (or matrices) v[i].
   * Then the result of a mix is another list of vectors.
   * Example: Let the Mixing matrix be
   * <pre>
   *     [ 1 0 1 ]
   * M = [ 0 1 0 ]
   * </pre>
   * Let the source vector list be
   * <pre>
   * v = { [ 0 0 1 1 ], [ 0 1 0 1 ], [ 1 0 0 0 ] }
   * </pre>
   * Then, the result is:
   * <pre>
   * v= { [ 0 0 1 1 ] + [ 1 0 0 0 ] , [ 0 1 0 1 ] } = { [ 1 0 1 1 ], [ 0 1 0 1 ] }
   * </pre>
   * So, the number of rows of the mixing matrix is the number of
   * result vectors, and the number of columns of the mixing
   * matrix must be the number of source vectors.
   */
  template <class T>
  class linearMixer : public functor {
  public:

    /**
     * default constructor
     */
    linearMixer();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    linearMixer(const linearMixer& other);

    /**
     * destructor
     */
    virtual ~linearMixer();

    /**
     * returns the name of this type ("linearMixer")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearMixer<T>& copy(const linearMixer<T>& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    linearMixer<T>& operator=(const linearMixer<T>& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * On-Copy Apply method for the mixing functor.
     * The functor takes the mixing matrix and the source vectors.
     * The result is returned in the second list. This list must contain
     * pointers to already allocated vector objects. Its size must be
     * equal to the number of rows of the mixing matrix. The size of the
     * list of source vectors must be equal to the number of columns of
     * the mixing matrix.
     * @param mixer the mixing matrix
     * @param src the source vector list
     * @param dest the destination vector list.
     * @return true if the function could be performed, false otherwise.
     */
    bool apply(const matrix<T>& mixer,
               const std::vector< const vector<T> * > & src,
               const std::vector< vector<T> * > & dest) const;

    /**
     * On-Copy Apply method for the mixing functor.
     * The functor takes the mixing matrix and the source matrices.
     * The result is returned in the second list. This list must contain
     * pointers to already allocated matrix objects. Its size must be
     * equal to the number of rows of the mixing matrix. The size of the
     * list of source matrices must be equal to the number of columns of
     * the mixing matrix.
     * @param mixer the mixing matrix
     * @param src the source matrix list
     * @param dest the destination matrix list.
     * @return true if the function could be performed, false otherwise.
     */
    bool apply(const matrix<T>& mixer,
               const std::vector< const matrix<T> * > & src,
               const std::vector< matrix<T> * > & dest
#ifdef _LTI_MSC_6
               // this is very ugly: MSC cannot distinguish between the formal
               // argument list of both apply methods (another %$&,! MS bug)
               // therefore we have to define a dummy argument here
               , bool msbug = true
#endif
               ) const;
  };

}

#include "ltiLinearMixer_template.h"


#endif


