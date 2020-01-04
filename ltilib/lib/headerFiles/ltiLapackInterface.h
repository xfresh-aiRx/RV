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
 * file .......: ltiLapackInterface.h
 * authors ....: Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 13.11.2002
 * revisions ..: $Id: ltiLapackInterface.h,v 1.7 2005/02/25 12:56:38 libuda Exp $
 */

#ifndef _LTI_LAPACK_INTERFACE_H_
#define _LTI_LAPACK_INTERFACE_H_

#include "ltiConfig.h"

#ifdef HAVE_LAPACK

#include "ltiException.h"
#include "ltiMutex.h"

/**
 * \file ltiLapackInterface.h Definition of Interface for LAPack based functors
 */

namespace lti {

  /**
   * @defgroup lapack LAPack based functors
   * This group contains all classes and functors that use
   * functions from the Linear Algebra Package (LAPack) library.
   * See lti::lapackInterface for more information.
   *
   * @ingroup gMath
   */

  /**
   * Interface object for LAPACK functions. This class contains
   * locking methods for LAPACK.
   * If you implement an interface class for a LAPACK method, you
   * should inherit from this class. As an example for such a
   * method, see the lti::generalEigenVectors class.
   *
   * When implementing an interface, you obviously need the prototype
   * of the LAPACK method. There are two methods for this:
   * - download clapack.h from http://www.netlib.org/clapack/clapack.h
   *   and simply include it. But: Do not rely on anyone
   * - copy and paste the required prototype into your source code.
   *
   * @ingroup lapack
   *
   * @see \ref lapack
   */
  class lapackInterface {
  public:

    /**
     * Exception class denoting an illegal matrix format (non-connected)
     * All matrices used by LAPACK routines must be connected. If
     * a matrix is not, the method throws this exception.
     */
    class matrixNotConnected: public lti::exception {
    public:
      /**
       * Default constructor
       */
      matrixNotConnected()
        : exception("LAPack methods not available fornon-connected matrices") {
      };

      /**
       * Return the name of this class
       */
      virtual const char* getTypeName() const {
        return "lapackInterface::matrixNotConnected";
      };
    };

    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const {
      return "lapackInterface";
    };

  protected:
    /**
     * Default constructor
     */
    lapackInterface();

    /**
     * Destructor
     */
    virtual ~lapackInterface();

    /**
     * Lock the LAPack interface
     *
     * Unfortunately, LAPACK is not thread-safe. Therefore we must
     * use a mutex to protect threads from concurrent execution.
     */
    inline void lockInterface() const {
      lola.lock();
    }

    /**
     * Unlock the LAPack interface
     *
     * Unfortunately, LAPACK is not thread-safe. Therefore we must
     * use a mutex to protect threads from concurrent execution.
     */
    inline void unlockInterface() const {
      lola.unlock();
    };

  private:
    /**
     * the mutex used to protect the interface.
     */
     static mutex lola;
  };

}

#endif

#endif

