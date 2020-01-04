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
 * file .......: ltiTriangularMatrixType.h
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 7.7.2003
 * revisions ..: $Id: ltiTriangularMatrixType.h,v 1.2 2005/06/16 12:37:35 doerfler Exp $
 */

#include "ltiIoObject.h"

#ifndef _LTI_TRIANGULAR_MATRIX_TYPE_H_
#define _LTI_TRIANGULAR_MATRIX_TYPE_H_

namespace lti {

  /**
   * This is a helper class that defines the enum eTriangularType and
   * has one member triangularType of that type. It is inherited by
   * the parameter classes of functors that work on regular matrices
   * that are triangular by definition. The triangular type determines
   * whether such a matrix is lower or upper triangular.
   */
  class triangularMatrixType : public ioObject {

  protected:
    
    triangularMatrixType() {};
    
  public:
    
    /**
     * A triangular matrix can have its values above or below the
     * main diagonal which is called upper or lower triangular
     * matrix, respectively.
     */
    enum eTriangularType {
      Upper, /**< the data matrix is upper triangular*/ 
      Lower  /**< the data matrix is lower triangular*/
    };

    /**
     * Sets whether a matrix that is triangular by defintion is upper
     * or lower triangular.
     */
    eTriangularType triangularType;

    /**
     * returns name of this type
     */
    const char* getTypeName() const {
      return "triangularMatrixType";
    };
    
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    triangularMatrixType& copy(const triangularMatrixType& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    triangularMatrixType& operator=(const triangularMatrixType& other);

    /**
     * write the data in the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    bool write(ioHandler& handler, const bool complete=true) const;

    /**
     * read the data from the given ioHandler
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    bool read(ioHandler& handler,const bool complete=true);

  };
}

#endif
