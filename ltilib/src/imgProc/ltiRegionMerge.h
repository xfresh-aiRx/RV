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


/*--------------------------------------------------------------------
 * project ....: LTI-Lib: Image Processing and Computer Vision Library
 * file .......: ltiRegionMerge.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 18.3.2002
 * revisions ..: $Id: ltiRegionMerge.h,v 1.10 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_REGION_MERGE_H_
#define _LTI_REGION_MERGE_H_

#include "ltiContour.h"
#include "ltiImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This functor uses a similarity matrix (see lti::similarityMatrix) and
   * a threshold value given in the parameters to decide if two objects in
   * a mask (also returned by lti::similarityMatrix or by lti::objectsFromMask)
   * should be merged or not.
   *
   * The result is a new mask with less objects than the original one.
   *
   * See also \ref segmPage.
   */
  class regionMerge : public functor {
  public:
    /**
     * the parameters for the class regionMerge
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
       * Smallest required similarity in the similarity matrix to
       * decide if two regions need to be merged or not.
       *
       * Default value: 0.001
       */
      double threshold;

    };

    /**
     * default constructor
     */
    regionMerge();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    regionMerge(const regionMerge& other);

    /**
     * destructor
     */
    virtual ~regionMerge();

    /**
     * returns the name of this type ("regionMerge")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on a copy of the given %parameters.
     * @param srcmask source mask.  Each object must be represented by one
     *                              label.
     * @param simMat The similarity matrix.  The size of the matrix must
     *               be at least equal the number of labels plus one.
     * @param destmask resulting mask with merged objects.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const imatrix& srcmask,
               const dmatrix& simMat,
               imatrix& destmask) const;

    bool apply(const imatrix& srcmask,
	       const dmatrix& simMat,
	       const dvector& thresholds,
	       imatrix& destmask) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionMerge& copy(const regionMerge& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    regionMerge& operator=(const regionMerge& other);

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

