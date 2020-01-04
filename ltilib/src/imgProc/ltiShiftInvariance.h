/*
 * Copyright (C) 1998 - 2004
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
 * file .......: ltiShiftInvariance.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 31.5.2001
 * revisions ..: $Id: ltiShiftInvariance.h,v 1.11 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SHIFT_INVARIANCE_H_
#define _LTI_SHIFT_INVARIANCE_H_

#include "ltiImage.h"
#include "ltiVector.h"

#include "ltiGlobalFeatureExtractor.h"

namespace lti {

  /**
   * This functor takes a vector, which is supposed to be periodic, and
   * generates a second "normalized" shifted one.  It uses different modi.
   *
   * This first one uses the property of the fourier transform to represent a
   * vector "shift" as the addition of a factor k*w (w=2*Pi*f) to the
   * phase of its representation in the frecuency space.  To get rid
   * of this term, the phase is twice derived, and the result is
   * subracted from the first order derivative.  This resulting vector
   * should represent a constant value k, but due to the phase
   * "discontinuities" is not the case.  The average of the vector
   * values which lie in a given tolerance interval will be used to
   * calculate the shift.
   *
   * This hasn't beed documented anywere
   *
   * The second one calculates and centers the first momentum of the
   * cyclic vector at the position 0
   */
  class shiftInvariance : public globalFeatureExtractor {
  public:
    /**
     * the parameters for the class shiftInvariance
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
       * write the parameters in the given ioHandler
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters
      enum eMode {
        Fourier, /**< Try to eliminate the phase term cause by signal
                      shift */
        Momentum /**< shift the signal in a way that the first cyclic momentum
                      is position at 0 */
      };

      /**
       * the mode to be used.
       * Default value: Momentum
       */
      eMode mode;

    };

    /**
     * default constructor
     */
    shiftInvariance();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    shiftInvariance(const shiftInvariance& other);

    /**
     * destructor
     */
    virtual ~shiftInvariance();

    /**
     * returns the name of this type ("shiftInvariance")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /**
     * operates on the given %parameter.
     * @param srcdest dvector with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(dvector& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& src,dvector& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @param shft will contain the number of elements the vector was
     *             shifted
     * @return true if apply successful or false otherwise.
     */
    bool apply(const dvector& src,dvector& dest,int& shft) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    shiftInvariance& copy(const shiftInvariance& other);

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
     * returns the angle normalized to the interval 0..2Pi
     */
    double normAngle(const double& x) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if apply successful or false otherwise.
     *
     * The derivatives of first and second order will be computed directly
     * from the phase of the fourier transform of the source.
     */
    bool direct(const dvector& src,dvector& dest,int& shft) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if apply successful or false otherwise.
     *
     * Shift will be calculated based on the first momentum.
     */
    bool momentum(const dvector& src,dvector& dest,int& shft) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src dvector with the source data.
     * @param dest dvector where the result will be left.
     * @return true if apply successful or false otherwise.
     *
     * The derivatives of first and second order will be computed indirectly
     * through the cosine and sine of the phase.  There are still some
     * theoretical problems with this method, but it works fine.
     *
     * This is much slower than the direct method, and it requires much more
     * memory.
     *
     */
    bool arctan(const dvector& src,dvector& dest,int& shft) const;

    /**
     * consider discontinuities in the magnitud to interpolate
     * missing phase elements
     */
    bool fixArg(const dvector& mag,dvector& arg) const;

    /**
     * cyclic shift of the given vector
     */
    bool shift(const dvector& in,const int& shft,dvector& out) const;
  };
}

#endif

