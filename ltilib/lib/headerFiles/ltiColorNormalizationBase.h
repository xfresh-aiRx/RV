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
 * file .......: ltiColorNormalizationBase.h
 * authors ....: Suat Akyol, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 8.11.2000,30.03.2003
 * revisions ..: $Id: ltiColorNormalizationBase.h,v 1.6 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_COLOR_NORMALIZATION_BASE_H_
#define _LTI_COLOR_NORMALIZATION_BASE_H_

#include "ltiImage.h"
#include "ltiModifier.h"

namespace lti {
  /**
   * Parent class for color normalization methods usually used to eliminate
   * the effects of illumination changes.
   *
   * \ingroup gColor
   */
  class colorNormalizationBase : public modifier {
  public:
    /**
     * the parameters for the class colorNormalizationBase
     */
    class parameters : public modifier::parameters {
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

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------
    };

    /**
     * default constructor
     */
    colorNormalizationBase();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    colorNormalizationBase(const colorNormalizationBase& other);

    /**
     * destructor
     */
    virtual ~colorNormalizationBase();

    /**
     * returns the name of this type ("colorNormalizationBase")
     */
    virtual const char* getTypeName() const;

    /**
     * Normalize the colors on the given image and write the result there too.
     * @param srcdest image with the source data.
     * The result will be left here too.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(image& srcdest) const=0;

    /**
     * Normalize the colors on the image \a src and leave the result on the
     * \a dest one.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const=0;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    colorNormalizationBase& copy(const colorNormalizationBase& other);

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

