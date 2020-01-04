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
 * file .......: ltiLkmColorQuantization.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 15.5.2001
 * revisions ..: $Id: ltiLkmColorQuantization.h,v 1.12 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_LKM_COLOR_QUANTIZATION_H_
#define _LTI_LKM_COLOR_QUANTIZATION_H_

#include "ltiColorQuantization.h"

namespace lti {
  /**
   * Color quantization functor based on the local k-Means algorithm
   * of O. Verevka and J. W. Buchanan (see original paper
   * <a href="http://www.cs.ualberta.ca/~oleg/quantization.html">here</a>).
   *
   * The modifications are basically in the use of a LUT for the
   * computation of the L2 norm, which makes the algorithm as
   * efficient as using the originally proposed L(1/2) norm and the
   * consideration of a direct neighbourhood to avoid some problems
   * with simple quantization problems.
   */
  class lkmColorQuantization : public colorQuantization {
  public:
    /**
     * the parameters for the class lkmColorQuantization
     */
    class parameters : public colorQuantization::parameters {
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


      /**
       * This parameter defines how strong
       * is the influence of an changing paletteIndex
       * towards the neighbour paletteIndex is.
       * Default value: 0.1
       */
      double neighbour;

      /**
       * this parameter is the startvalue
       * for the weight of a new pixel.
       * Default value: 0.6
       */
      double learnRate;

      /**
       * This parameter is the factor,
       * the learnRate will be multiplicated with
       * in every iteration
       * It defines how fast the learnRate convergates to zero
       * Default value: 0.5
       */
      double shrinkLearnRate;

       /**
       * this parameter sets the upper limit for
       * the iterations, if the algorithm hasn't
       * convergated yet
       * Default value: 10
       */
      int maxIterations;
    };

    /**
     * default constructor
     */
    lkmColorQuantization();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    lkmColorQuantization(const lkmColorQuantization& other);

    /**
     * destructor
     */
    virtual ~lkmColorQuantization();

    /**
     * returns the name of this type ("lkmColorQuantization")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @param lkmCPalette the color palette with the quantized colors
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               channel8& dest,
               lti::palette& lkmCPalette) const;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(image& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest image with only the number of colors specified in
     *             the parameters
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,image& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    lkmColorQuantization& copy(const lkmColorQuantization& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  private:
    // look up table for the square of a number between -255 and 255
    static const int* sqrLUT;
  };
}

#endif

