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
 * file .......: ltiOgdFilter.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 25.5.2000
 * revisions ..: $Id: ltiOgdFilter.h,v 1.15 2005/06/16 12:35:42 doerfler Exp $
 */

#ifndef _LTI_OGD_FILTER_H_
#define _LTI_OGD_FILTER_H_

#include "ltiOgdKernels.h"
#include "ltiFilter.h"
#include "ltiConvolution.h"

namespace lti {
  /**
   * oriented gaussian derivatives steerable filters
   *
   * This functions make use of the ogd kernels (lti::ogd1Kernel and
   * lti::ogd2Kernel).
   *
   * If you need to calculate more than one image for different directions,
   * use the generateBasis and useBasis members.
   *
   * If you want to calculate just one image, use the apply members.
   */
  class ogdFilter : public filter {
  public:
    /**
     * the parameters for all ogd filters
     */
    class parameters : public filter::parameters {
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
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      /**
       * Order of the OGD to be used (1 or 2)
       *
       * Default value: 1
       */
      int order;

      /**
       * size of the used filter kernel (the 2D kernel has a dimension
       * size x size)
       *
       * Default value: 5
       */
      int size;

      /**
       * Variance of the Gaussian.
       *
       * If you give -1.0 here, the variance will be computed from the
       * given size, exaclty as for the Gaussian Kernels.
       *
       * Default value: 1.4
       */
      double variance;

      /**
       * orientation in radians (0 rad is equivalent to the positive
       * x-Axis)
       *
       * Default value: 0.0
       */
      double angle;
    };

    static const double useAngleOfParameters;

    /**
     * default constructor
     */
    ogdFilter();

    /**
     * default constructor with parameters
     */
    ogdFilter(const parameters& par);

    /** 
     * copy constructor
     * @param other the object to be copied
     */
    ogdFilter(const ogdFilter& other);

    /**
     * destructor
     */
    virtual ~ogdFilter();

    /**
     * returns the name of this type ("ogdFilter")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given parameter.
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @result true if ok, false otherwise.
     */
    bool apply(channel& srcdest) const;

    /** 
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @result true if ok, false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /** 
     * first order ogd derivative combination.
     *
     * If the basis channels are known (i.e. the original channel
     * convolved with the two basis kernels), you can use this function
     * to generate the oriented image in a very efficient manner.  You
     * can generate the basis channels with the generateBasisOGD1() member
     * function or you can generate them by yourself using the
     * lti::ogd1Kernel.
     *
     * The interpolation functions are cosine for the basis0 and sinus for
     * the basis1.
     *
     * @param basis0 the first basis channel
     * @param basis1 the second basis channel
     * @param dest   the filtered image
     * @param angle  the used direction (in radians).
     *               this value must be between -2000 Pi and +2000 Pi.
     *               If not given, the angle member of the parameters will
     *               be used!
     * @return true if ok, false otherwise.
     */
    bool useBasisOgd1(const channel& basis0,
                          const channel& basis1,
                                channel& dest,
                          const double& angle = useAngleOfParameters);

    /** 
     * generate the basis channels of the first order OGD
     * @param src the source channel
     * @param basis0 the source channel convolved with the first basis
     *               kernel
     * @param basis1 the source channel convolved with the second basis
     *               kernel.
     */
    void generateBasisOgd1(const channel& src,
                                 channel& basis0,
                                 channel& basis1);

    /**
     * second order ogd derivative combination
     *
     * If the basis channels are known (i.e. the original channel
     * convolved with the three basis kernels), you can use this function
     * to generate the oriented image in a very efficient manner.  You
     * can generate the basis channels with the generateBasisOgd2() member
     * function or you can generate them by yourself using the
     * lti::ogd1Kernel.
     *
     * The interpolation functions are cos^2 for the basis0, sin^2 for
     * the basis1 and 2*cos*sin for the basis2.
     *
     * @param basis0 the first basis channel
     * @param basis1 the second basis channel
     * @param basis2 the third basis channel
     * @param dest   the filtered image
     * @param angle  the direction to be used
     */
    channel& useBasisOgd2(const channel& basis0,
                          const channel& basis1,
                          const channel& basis2,
                                channel& dest,
                          const double&  angle = useAngleOfParameters);

    /**
     * generate the basis channels of the second order Ogd
     * @param src the source channel
     * @param basis0 the source channel convolved with the first basis
     *               kernel
     * @param basis1 the source channel convolved with the second basis
     *               kernel.
     * @param basis2 the source channel convolved with the third basis
     *               kernel.
     */
    void generateBasisOgd2(const channel& src,
                                 channel& basis0,
                                 channel& basis1,
                                 channel& basis2);

    /** 
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    ogdFilter& copy(const ogdFilter& other);

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

