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
 * file .......: ltiRelativeThresholding.h
 * authors ....: Arnd Hannemann
 * organization: LTI, RWTH Aachen
 * creation ...: 4.1.2004
 * revisions ..: $Id: ltiRelativeThresholding.h,v 1.6 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_RELATIVE_THRESHOLDING_H_
#define _LTI_RELATIVE_THRESHOLDING_H_

#include "ltiThresholding.h"

namespace lti {
  /**
   * This class implements percentual Thresholding. 
   * The parameter values highThreshold  and lowThreshold inherited from
   * thresholding::parameters are interpreted as percentages.
   * i.e. lowThreshold = 0.5f means that approximately the lower 50% of the 
   * pixels will be cut off.
   *
   * It uses a simple histogramm with thresholding::parameters::bins 
   * to calculate the absolute thresholds.
   *
   * \code
   * #include ltiRelativeThresholding.h
   * 
   * lti::relativeThresholding::parameters rtPar;
   * rtPar.lowThreshold   = 0.5f;
   * // creates a black&white mask
   * rtPar.outRegionValue = 0.0f;
   * rtPar.inRegionValue  = 1.0f;
   * rtPar.keepInRegion   = false;
   *
   * lti::relativeThresholding rt(rtPar);
   * rt.apply(sourcechannel,destchannel);
   * \endcode 
   */
  class relativeThresholding : public thresholding  {
  public:
    /**
     * the parameters for the class relativeThresholding
     */
    class parameters : public thresholding::parameters {
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
       * How many bins are used in the histogramm which is used
       * used to calculate absolute thresholds.
       *
       * Default: 256
       */
      int bins;

    };

    /**
     * default constructor
     */
    relativeThresholding();

    /**
     * Construct a functor using the given parameters
     */
    relativeThresholding(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    relativeThresholding(const relativeThresholding& other);

    /**
     * destructor
     */
    virtual ~relativeThresholding();

    /**
     * returns the name of this type ("relativeThresholding")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @return true if apply successful or false otherwise.
     */
    bool apply(channel8& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const channel8& src,channel8& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    relativeThresholding& copy(const relativeThresholding& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    relativeThresholding& operator=(const relativeThresholding& other);

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

