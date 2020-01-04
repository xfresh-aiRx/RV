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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiPointSetNormalization.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 29.3.2004
 * revisions ..: $Id: ltiPointSetNormalization.h,v 1.3 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_POINT_SET_NORMALIZATION_H_
#define _LTI_POINT_SET_NORMALIZATION_H_

#include "ltiFunctor.h"
#include "ltiVector.h"
#include "ltiPoint.h"

namespace lti {

  /**
   * A parent class for normalizations of point sets. All derived
   * normalizations may include translation and scaling and are
   * defined by the following formula:
   *
   * \code
   * tpoint<T> normedPt;
   * tpoint<T> pt;
   * tpoint<T> scale;
   * tpoint<T> shift;
   * normedPt = scale * pt + shift
   * \endcode
   *
   * If estimating e.g. a transform by least squares it is advised to perform
   * such a normalization. A transform between normalized data might not be
   * the same as for the  original data. Therefore it is important to follow
   * the above definition because otherwise the undo-operations implemented
   * for e.g. the transformEstimator will not work.
   *
   * @see isotropicNormalization
   */
  class pointSetNormalization : public functor {
  public:
    /**
     * The parameters for the class pointSetNormalization
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Default constructor
       */
      parameters();

      /**
       * Copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * Destructor
       */
      ~parameters();

      /**
       * Returns name of this type
       */
      const char* getTypeName() const;

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * Copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * Returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * Write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * Read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * This function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * This function is required by MSVC only, as a workaround for a
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
     * Default constructor
     */
    pointSetNormalization();

    /**
     * Construct a functor using the given parameters
     */
    pointSetNormalization(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    pointSetNormalization(const pointSetNormalization& other);

    /**
     * Destructor
     */
    virtual ~pointSetNormalization();

    /**
     * Returns the name of this type ("pointSetNormalization")
     */
    virtual const char* getTypeName() const;

    /**
     * Normalizes the supplied point vector.
     * operates on the given %parameter.
     * @param srcdest vector<fpoint> with the source data. The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(vector<fpoint>& srcdest) const =  0;

    /**
     * Normalizes the supplied point vector as defined by
     * normedPt = scale * pt + shift. The parameters scale and shift
     * are computed automatically.
     * operates on the given %parameter.
     * @param srcdest vector<fpoint> with the source data.  The result
     *                 will be left here too.
     * @param scale fpoint where the applied scale will be left
     * @param shift fpoint where the applied shift will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(vector<fpoint>& srcdest,
		       fpoint& scale, fpoint& shift) const = 0;

    /**
     * Normalizes the supplied point vector.
     * operates on the given %parameter.
     * @param srcdest vector<dpoint> with the source data. The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(vector<dpoint>& srcdest) const = 0;

    /**
     * Normalizes the supplied point vector as defined by
     * normedPt = scale * pt + shift. The parameters scale and shift
     * are computed automatically.
     * operates on the given %parameter.
     * @param srcdest vector<dpoint> with the source data.  The result
     *                 will be left here too.
     * @param scale dpoint where the applied scale will be left
     * @param shift dpoint where the applied shift will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(vector<dpoint>& srcdest,
		       dpoint& scale, dpoint& shift) const = 0; 

    /**
     * Normalizes the supplied point vector.
     * operates on a copy of the given %parameters.
     * @param src vector<fpoint> with the source data.
     * @param dest vector<fpoint> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const vector<fpoint>& src,vector<fpoint>& dest) const
      = 0;

    /**
     * Normalizes the supplied point vector as defined by
     * normedPt = scale * pt + shift. The parameters scale and shift
     * are computed automatically.
     * operates on a copy of the given %parameters.
     * @param src vector<fpoint> with the source data.
     * @param dest vector<fpoint> where the result will be left.
     * @param scale fpoint where the applied scale will be left
     * @param shift fpoint where the applied shift will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const vector<fpoint>& src,vector<fpoint>& dest,
		       fpoint& scale, fpoint& shift) const = 0;

    /**
     * Normalizes the supplied point vector.
     * operates on a copy of the given %parameters.
     * @param src vector<dpoint> with the source data.
     * @param dest vector<dpoint> where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const vector<dpoint>& src,vector<dpoint>& dest) const
      = 0;

    /**
     * Normalizes the supplied point vector as defined by
     * normedPt = scale * pt + shift. The parameters scale and shift
     * are computed automatically.
     * operates on a copy of the given %parameters.
     * @param src vector<dpoint> with the source data.
     * @param dest vector<dpoint> where the result will be left.
     * @param scale dpoint where the applied scale will be left
     * @param shift dpoint where the applied shift will be left
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const vector<dpoint>& src,vector<dpoint>& dest,
		       dpoint& scale, dpoint& shift) const = 0;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pointSetNormalization& copy(const pointSetNormalization& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    pointSetNormalization& operator=(const pointSetNormalization& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const = 0;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  };
}

#endif

