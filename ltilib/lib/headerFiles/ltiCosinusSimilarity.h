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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiCosinusSimilarity.h
 * authors ....: Stefan Syberichs, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 30.1.2001
 * revisions ..: $Id: ltiCosinusSimilarity.h,v 1.5 2004/05/03 13:30:09 alvarado Exp $
 */

#ifndef _COSINUS_SIMILARITY_H_
#define _COSINUS_SIMILARITY_H_

#include "ltiSimilarityFunctor.h"

namespace lti {

  /**
   * Quadratic Cosinus Similarity
   *
   * This class computes the similarity of two vectors or matrices
   * by means of the quadratic cosinus of the angle between the two vectors.
   *
   */
  template <class T>
  class cosinus2Similarity : public similarityFunctor<T> {
  public:
    /**
     * default constructor
     */
    cosinus2Similarity();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    cosinus2Similarity(const cosinus2Similarity<T>& other);

    /**
     * destructor
     */
    virtual ~cosinus2Similarity();

    /**
     * returns the name of this type ("cosinus2Similarity")
     */
    virtual const char* getTypeName() const;

    /**
     * calculates the quadratic cosinus-type of similarity of the
     * vectors a and b.
     *
     * If both vectors have different sizes, the returned value will
     * be negative!
     *
     * @param a the first vector<T>
     * @param b the second vector<T>
     * @return the cos2-type similarity of a and b
     */
    T apply(const vector<T>& a, const vector<T>& b) const;

    /**
     * Take the matrices as vectors and compute the similarity of
     * them.
     * @return the cos2-type of similarity of a and b
     */
    T apply(const matrix<T>& a, const matrix<T>& b) const;

    /**
     * Compute the similarity for each row-vector in the matrix a
     * and b.
     *
     * If b.size() != a.columns() an empty vector will be returned.
     * Otherwise, the element i of the vector dest will contain the
     * cos2 similarity between the i-th row of a and b.
     * @return always null
     */
    vector<T>& apply(const matrix<T>& a,
                     const vector<T>& b,
                           vector<T>& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    cosinus2Similarity& copy(const cosinus2Similarity& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

  };


}

#include "ltiCosinusSimilarity_template.h"

#endif

