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
 * file .......: ltiVariablySpacedSamplesInterpolator.h
 * authors ....: Peter Doerfler, Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 3.5.2003
 * revisions ..: $Id: ltiVariablySpacedSamplesInterpolator.h,v 1.8 2005/06/16 12:37:35 doerfler Exp $
 */

#ifndef _LTI_VARIABLY_SPACED_SAMPLES_INTERPOLATOR_H_
#define _LTI_VARIABLY_SPACED_SAMPLES_INTERPOLATOR_H_

#include "ltiInterpolator.h"

namespace lti {
  /**
   * This abstract class parents all interpolation functors, whose
   * samples have varying distances.
   */
  class variablySpacedSamplesInterpolator : public interpolator {
  public:
    /**
     * the parameters for the class variablySpacedSamplesInterpolator
     */
    class parameters : public interpolator::parameters {
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
      virtual functor::parameters* clone() const = 0;

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
    };

    /**
     * default constructor
     */
    variablySpacedSamplesInterpolator();

    /**
     * Construct a functor using the given parameters
     */
    variablySpacedSamplesInterpolator(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    variablySpacedSamplesInterpolator(const variablySpacedSamplesInterpolator& other);

    /**
     * destructor
     */
    virtual ~variablySpacedSamplesInterpolator();

    /**
     * returns the name of this type ("variablySpacedSamplesInterpolator")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    variablySpacedSamplesInterpolator&
    copy(const variablySpacedSamplesInterpolator& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    variablySpacedSamplesInterpolator&
    operator=(const variablySpacedSamplesInterpolator& other);

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

