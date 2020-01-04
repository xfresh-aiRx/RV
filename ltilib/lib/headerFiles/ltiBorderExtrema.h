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
 * file .......: ltiBorderExtrema.h
 * authors ....: Ruediger Weiler
 * organization: LTI, RWTH Aachen
 * creation ...: 11.6.2001
 * revisions ..: $Id: ltiBorderExtrema.h,v 1.15 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BORDER_EXTREMA_H_
#define _LTI_BORDER_EXTREMA_H_

#include "ltiModifier.h"

namespace lti {
  class borderPoints; //declaration in ltiContour.h
  class polygonPoints; //declaration in ltiPolygonPoints.h

  /**
   *  This functor extracts minimum and maximum positions along a border
   *  in terms of distance to a given center point.
   *  The extrema are saved in a polygonPoint either separate or in
   *  one List alternately as min, max, min, ... always starting with
   *  a minimum.
   */
  class borderExtrema : public modifier {
  public:
    /**
     * the parameters for the class borderExtrema
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

      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters


      /**
       * The minimum number of rising/falling edges, before an extremum can
       * be detected. Corresponds to ignoring minor changes in border.
       * At the same time this is the minimum number of pixels along
       * the border between two extrema.
       * Default is 5.
       */
      int minTolerance;

      /**
       * Distances are calculated between borderPoints and "center".
       * The default value is (0.0, 0.0).
       */
      tpoint<double> center;

    };

    /**
     * default constructor
     */
    borderExtrema();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    borderExtrema(const borderExtrema& other);

    /**
     * destructor
     */
    virtual ~borderExtrema();

    /**
     * returns the name of this type ("borderExtrema")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param src borderPoints with the source border
     * @param min the polygonPoints who take the minima
     * @param max the polygonPoints who take the maxima
     * @return true if apply successful or false otherwise
     */
    bool apply(const borderPoints& src, polygonPoints& min, polygonPoints& max) const;

    /**
     * operates on the given %parameter.
     * @param src borderPoints with the source border
     * @param minMax the polygonPoints who take the extrema in order
     *                starting with a minima
     * @return true if apply successful or false otherwise.
     */
    bool apply(const borderPoints& src, polygonPoints& minMax) const;


    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    borderExtrema& copy(const borderExtrema& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * sets the functor's parameters.
     */
    virtual bool updateParameters();
  };
}

#endif

