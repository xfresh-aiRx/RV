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
 * file .......: ltiborderSignature.h
 * authors ....: Ruediger Weiler
 * organization: LTI, RWTH Aachen
 * creation ...: 7.6.2001
 * revisions ..: $Id: ltiBorderSignature.h,v 1.14 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BORDER_SIGNATURE_H_
#define _LTI_BORDER_SIGNATURE_H_

// include files which are needed in this header!!
#include "ltiShapeFeatureExtractor.h"
#include "ltiTypes.h"
#include "ltiPointList.h"

// include to parent class:
#include "ltiFeatureExtractor.h"

namespace lti {
  /**
   *  This functor extracts a functional border description from a borderPoint list.
   *  Therefore it divides the 2D-space into radial segments of equal size, originating
   *  from the given "center". Then it evaluates some function for the borderPoints, that
   *  lie within each segment. The result is stored in a lti::dvector.
   *
   *  The starting segments angle can be specified by the parameter "orientation".
   *  There are two methods of operation: "area" and "distance".
   *  The "area" method calculates the area proportion within each segment, normalized with
   *  the total area. The "distance" method calulates either the minimum, maximum or the
   *  average distance to the center point within a segment. This is specified by the
   *  "distanceType" parameter.
   */
  class borderSignature : public shapeFeatureExtractor {
  public:
    /**
     * the parameters for the class borderSignature
     */
    class parameters : public featureExtractor::parameters {
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


      /**  method type.
	     *   This are the two modes of operation
       */
      enum eMethod {
        distance, /**< the distance to the border within each segment */
        area      /**< the area inside the border region within each segment */
      };

      /**  distance type.
	     *   This are the sub-modes for the "distance" method
       */
      enum eDistanceType {
        min,      /**< search the minimum distance within each segment */
        max,      /**< search the maximum distance */
        average   /**< calculate the average distance */
      };
      /**
       * The origin of the segments, can be e.g. the center of gravity of a 2D-shape
       * the borderpoints (see lti::geometricFeatures).
       * The default value is (0.0 , 0.0).
       */
      tpoint<double> center;

      /**
       * The number of segments that the 2D-space is divided into.
       * Note: If you want to use FFT later set the value to pow(2,x).
       * The default value is 32.
       */
      int numberOfSegments;

      /**
       * This parameter specifies the orientation of the first segment.
       * The segments are ordered clockwise cyclic around "center".
       * An angle in radient between -2Pi and 2Pi is required.
       * The default value is 0.0 (Horizontal orientation).
       */
      double orientation;

      /**
       * The mode of operation (see eMethod).
       */
      eMethod method;

      /**
       * This specifies the sub-method of the "distance" method (see eDistanceType).
       */
      eDistanceType distanceType;

    };

    /**
     * default constructor
     */
    borderSignature();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    borderSignature(const borderSignature& other);

    /**
     * destructor
     */
    virtual ~borderSignature();

    /**
     * returns the name of this type ("borderSignature")
     */
    virtual const char* getTypeName() const;



    /**
     * operates on a copy of the given %parameters.
     * @param src pointList with the source data.
     * @param segmentation dvector where the signature will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const pointList& src, dvector& segmentation) const;


    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    borderSignature& copy(const borderSignature& other);

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

