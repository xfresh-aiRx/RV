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
 * file .......: ltiInterpolator.h
 * authors ....: Peter Doerfler, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 3.5.2003
 * revisions ..: $Id: ltiInterpolator.h,v 1.9 2005/06/16 12:37:34 doerfler Exp $
 */

#ifndef _LTI_INTERPOLATOR_H_
#define _LTI_INTERPOLATOR_H_

#include "ltiFunctor.h"
#include "ltiBoundaryType.h"

namespace lti {
  /**
   * This abstract class parents all interpolation functors.
   *
   * In interpolation a set of point, a tabulated function, is
   * given. These points are often called samples. The goal is to find
   * function values between the given data.
   *
   * There are many different interpolation schemes depending on the
   * application. The samples can be equally or variably spaced. The
   * former occurs mainly in images but can be useful for other data
   * as well (see equallySpacedSamplesInterpolator). The latter is
   * most often met in the interpolation of graphs (1D functions in an
   * n-dimensional space). See also
   * variablySpacedSamplesInterpolation.
   */
  class interpolator : public functor {
  public:
    /**
     * the parameters for the class interpolator
     */
    class parameters : public functor::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * create parameters with the given boundary type
       */
      parameters(const eBoundaryType btype);

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
      virtual functor::parameters* clone() const=0;

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
       * Specify how the boundaries will be used. 
       *
       * See the documentation for lti::eBoundaryType for more information
       */
      eBoundaryType boundaryType;

    };

    /**
     * default constructor
     */
    interpolator();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    interpolator(const interpolator& other);

    /**
     * destructor
     */
    virtual ~interpolator();

    /**
     * returns the name of this type ("interpolator")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    interpolator& copy(const interpolator& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    interpolator& operator=(const interpolator& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Shortcut to set the boundaryType parameter at once
     *
     * @return true, if the boundary could be set, or false if no
     *               parameters have been set yet.
     */
    bool setBoundaryType(const eBoundaryType& boundType);
  };
}

#endif

