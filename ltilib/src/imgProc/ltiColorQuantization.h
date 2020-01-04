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
 * file .......: ltiColorQuantization.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 8.5.2001
 * revisions ..: $Id: ltiColorQuantization.h,v 1.10 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_QUANTIZATION_H_
#define _LTI_COLOR_QUANTIZATION_H_


#include "ltiFunctor.h"
#include "ltiImage.h"
#include "ltiVector.h"

namespace lti {

  /**
   * Abstract parent class for all color quantization algorithms
   * All color quantization functors must overload the apply-member defined
   * here @see colorQuantization::apply.
   * The final quantize color image can be generated using the lti::usePalette
   * functor.
   */
  class colorQuantization : public functor {
  public:
    /**
     * the parameters for the class colorQuantization
     */
    class parameters : public functor::parameters {
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
       * Number of colors that the resulting channel must have.
       * Usually this value must be between 2 and 256, but each functor
       * can modify this requirement.  Default value: 256
       */
      int numberOfColors;

    };

    /**
     * default constructor
     */
    colorQuantization();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    colorQuantization(const colorQuantization& other);

    /**
     * destructor
     */
    virtual ~colorQuantization();

    /**
     * returns the name of this type ("colorQuantization")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param src original image with the true-color data
     * @param dest channel8 where the indexes of the also calculated palette
     *             will be.
     * @param thePalette the color palette used by the channel.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,
                       channel8& dest,
                       palette& thePalette) const = 0;

    /**
     * operates on the given %parameter.
     * @param srcdest image with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest image with only the number of colors specified in
     *             the parameters
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorQuantization& copy(const colorQuantization& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

