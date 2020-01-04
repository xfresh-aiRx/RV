/*
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004
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
 * file .......: ltiHuMoments.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 23.10.2001
 * revisions ..: $Id: ltiHuMoments.h,v 1.15 2005/06/16 12:35:10 doerfler Exp $
 */

#ifndef _LTI_HU_MOMENTS_H_
#define _LTI_HU_MOMENTS_H_

#include "ltiMath.h"
#include "ltiGlobalFeatureExtractor.h"

namespace lti {
  /**
   * Extracts the 7 moment invariants as described in "Visual Pattern Recognition
   * by Moment Invariants" by Ming-Kuei Hu (IRE Transactions on Information Theory,
   * 1962).
   */
  class huMoments : public globalFeatureExtractor {
  public:
    // constant indices for accessing the result vectors
    static const int  hu1;
    static const int  hu2;
    static const int  hu3;
    static const int  hu4;
    static const int  hu5;
    static const int  hu6;
    static const int  hu7;
    static const int  xcog;
    static const int  ycog;
    static const int  eigen1;
    static const int  eigen2;
    static const int  orientation;
    static const int  m00;

    /**
     * the parameters for the class huMoments
     */
    class parameters : public globalFeatureExtractor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------

      /**
       * Since feature values can be very small, they can be scaled by the
       * monotonic function -sign(x)*log(abs(x)) by setting this to true.
       * Default is false
       */
      bool scaling;

    };

    /**
     * default constructor
     */
    huMoments();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    huMoments(const huMoments& other);

    /**
     * destructor
     */
    virtual ~huMoments();

    /**
     * returns the name of this type ("huMoments")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest dvector containing the 7 hu moments.
     * @param more additional feature vector with
     * <ul>
     * <li>xcog
     * <li>ycog
     * <li>eigen1
     * <li>eigen2
     * <li>orientation [-Pi/2,Pi/2]
     * <li>m00
     * </ul>
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, dvector& dest, dvector& more) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest dvector containing the 7 hu moments.
     * @param more additional feature vector with
     * <ul>
     * <li>xcog
     * <li>ycog
     * <li>eigen1
     * <li>eigen2
     * <li>orientation [-Pi/2,Pi/2]
     * <li>m00
     * </ul>
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, dvector& dest, dvector& more) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest dvector containing the 7 hu moments.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, dvector& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest dvector containing the 7 hu moments.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, dvector& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param rect bounding rectangle of data to be regarded.
     * @param dest dvector containing the 7 hu moments.
     * @param more additional feature vector with
     * <ul>
     * <li>xcog
     * <li>ycog
     * <li>eigen1
     * <li>eigen2
     * <li>orientation [-Pi/2,Pi/2]
     * <li>m00
     * </ul>
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, const rectangle& rect, dvector& dest, dvector& more) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param rect bounding rectangle of data to be regarded.
     * @param dest dvector containing the 7 hu moments.
     * @param more additional feature vector with
     * <ul>
     * <li>xcog
     * <li>ycog
     * <li>eigen1
     * <li>eigen2
     * <li>orientation [-Pi/2,Pi/2]
     * <li>m00
     * </ul>
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, const rectangle& rect, dvector& dest, dvector& more) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param rect bounding rectangle of data to be regarded.
     * @param dest dvector containing the 7 hu moments.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src, const rectangle& rect, dvector& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param rect bounding rectangle of data to be regarded.
     * @param dest dvector containing the 7 hu moments.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src, const rectangle& rect, dvector& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    huMoments& copy(const huMoments& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    huMoments& operator=(const huMoments& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:

    /**
     * This is an internal function for computing sign(x)*log(abs(x))
     * @param x value
     * @return sign(x)*log(abs(x))
     */
    double logn(double& x) const;
  };
}

#endif

