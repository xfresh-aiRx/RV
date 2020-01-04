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
 * file .......: ltiBoundingBox.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 9.10.2000
 * revisions ..: $Id: ltiBoundingBox.h,v 1.18 2005/06/16 12:38:53 doerfler Exp $
 */

#ifndef _LTI_BOUNDING_BOX_H_
#define _LTI_BOUNDING_BOX_H_

#include "ltiObject.h"
#include "ltiMatrix.h"
#include "ltiContour.h"
#include "ltiLocation.h"
#include "ltiSegmentation.h"
#include "ltiTypes.h"

namespace lti {
  /**
   * Generate an image of a region of another one specified through a
   * contour-instance.
   *
   * After segmenting the biggest object on an image or channel,
   * it is usually desirable to get a new image with the extracted object only.
   * This class will allow to generate such an image.  The position of the
   * desired object will be indicated by the area points of a contour object.
   *
   * Type T of the template specify the type of the matrix to be used (for
   * images this will be rgbPixel and for channels float).
   *
   * Example:
   *
   * \code
   *
   * lti::loadBMP loader;
   * lti::image img,biggestObject;
   * lti::channel8 mask;
   *
   * loader.load("img/leo040801_00_019.bmp",img); // load an image
   *
   * lti::regionGrowing segmenter;  // functor for segmentation
   *
   * segmenter.apply(img,mask);     // get a mask to differentiate background
   *                                // and object.
   *
   * // get the biggest object on the mask
   * lti::objectsFromMask ofm;
   * std::list<areaPoints> objs;
   * ofm.apply(mask,objs); // the first object on "objs" is the biggest one
   *                       // on the mask!
   * // get a new image with the biggest object only
   * lti::boundingBox<image::value_type> bbox;
   * lti::boundingBox<image::value_type>::parameters bboxParam;
   * bboxParam.borderSize = 5; // the border will have 5 pixels (each side!)
   * bboxParam.centerOfGravity = false;
   * bbox.apply(img,objs.front(),biggestObject);
   *
   * \endcode
   */
  template<class T>
  class boundingBox : public segmentation {
  public:
    // implemetation of parameters here due to a MSVC++ bug
    /**
     * the parameters for the class boundingBox
     */
    class parameters : public segmentation::parameters {
    public:
      /**
       * default constructor
       */
      parameters() : segmentation::parameters() {
        borderSize = int(1);
        centerOfGravity = bool(false);
        backgroundColor = T(0);
        justSuppressBackground = false;
        optimalBox = false;
        useLengths = false;
      };

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other) : segmentation::parameters() {
        copy(other);
      };

      /**
       * destructor
       */
      ~parameters() {
      };

      /**
       * returns name of this type
       */
      const char* getTypeName() const {
        return "boundingBox::parameters";
      };

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other) {
#     ifndef _LTI_MSC_6
        // MS Visual C++ 6 is not able to compile this...
        segmentation::parameters::copy(other);
#     else
        // ...so we have to use this workaround.
        // Conditional on that, copy may not be virtual.
        segmentation::parameters& (segmentation::parameters::* p_copy)
          (const segmentation::parameters&) =
          segmentation::parameters::copy;
        (this->*p_copy)(other);
#     endif

        borderSize = other.borderSize;
        centerOfGravity = other.centerOfGravity;
        backgroundColor = other.backgroundColor;
        justSuppressBackground = other.justSuppressBackground;
        optimalBox = other.optimalBox;
        useLengths = other.useLengths;

        return *this;
      };

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const {
        return new parameters(*this);
      };

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be
       *        written.
       * @return true if write was successful
       */
#     ifndef _LTI_MSC_6
      virtual bool write(ioHandler& handler,const bool complete=true) const
#     else
        bool writeMS(ioHandler& handler,const bool complete=true) const
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.writeBegin();
          }

          if (b) {
            lti::write(handler,"borderSize",borderSize);
            lti::write(handler,"centerOfGravity",centerOfGravity);
            lti::write(handler,"backgroundColor",backgroundColor);
            lti::write(handler,"justSuppressBackground",
                       justSuppressBackground);
            lti::write(handler,"optimalBox",optimalBox);
            lti::write(handler,"useLengths",useLengths);
          }

#       ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...
          b = b && segmentation::parameters::write(handler,false);
#       else
          bool (segmentation::parameters::* p_writeMS)(ioHandler&,
                                                   const bool) const =
            segmentation::parameters::writeMS;
          b = b && (this->*p_writeMS)(handler,false);
#       endif

          if (complete) {
            b = b && handler.writeEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        virtual bool write(ioHandler& handler,
                           const bool complete = true) const {
           // ...we need this workaround to cope with another really
           // awful MSVC bug.
           return writeMS(handler,complete);
        }
#     endif

        /**
         * read the parameters from the given ioHandler
         * @param handler the ioHandler to be used
         * @param complete if true (the default) the enclosing begin/end will
         *        be also read, otherwise only the data block will be read.
         * @return true if write was successful
         */
#     ifndef _LTI_MSC_6
        virtual bool read(ioHandler& handler,const bool complete = true)
#     else
        bool readMS(ioHandler& handler,const bool complete=true)
#     endif
        {
          bool b = true;
          if (complete) {
            b = handler.readBegin();
          }

          if (b) {
            lti::read(handler,"borderSize",borderSize);
            lti::read(handler,"centerOfGravity",centerOfGravity);
            lti::read(handler,"backgroundColor",backgroundColor);
            lti::read(handler,"justSuppressBackground",justSuppressBackground);
            lti::read(handler,"optimalBox",optimalBox);
            lti::read(handler,"useLengths",useLengths);
          }

#     ifndef _LTI_MSC_6
          // This is the standard C++ code, which MS Visual C++ 6 is not
          // able to compile...

          b = b && segmentation::parameters::read(handler,false);
#     else
          bool (segmentation::parameters::* p_readMS)(ioHandler&,const bool) =
          segmentation::parameters::readMS;
          b = b && (this->*p_readMS)(handler,false);
#     endif

          if (complete) {
            b = b && handler.readEnd();
          }

          return b;
        }

#     ifdef _LTI_MSC_6
        virtual bool read(ioHandler& handler,const bool complete=true) {
          // ...we need this workaround to cope with another really awful MSVC
          // bug.
          return readMS(handler,complete);
        }
#      endif


      // --------------------------------------------------
      // boundingBox::parameters
      // --------------------------------------------------

      /**
       * The extracted image will have at each side a border of (at least)
       * the specified number of pixels.  If "centerOfGravity" is false, the
       * border will have exactly "borderSize" pixels.
       *
       * Default value: 1
       */
      int borderSize;

      /**
       * If "true", the center of gravity of the object will be forced
       * to be at the center of the new image.  If false, only the
       * borders of the object will be considered in the calculations.
       *
       * Default value: false;
       */
      bool centerOfGravity;

      /**
       * Background color
       *
       * Default value: T(0) (i.e. Black for images or 0 for channels)
       */
      T backgroundColor;

      /**
       * Suppress background only.  If true, the borderSize and centerOfGravity
       * parameters will be ignored, and the resulting image will have exactly
       * the same dimensions than the input image.  If false, the size of the
       * resulting image will be determined by the other parameters.
       *
       * Default value: false.
       */
      bool justSuppressBackground;

      /**
       * If false, just the principal components of the convex hull will be
       * used to determine the rectLocation's angle.  If true, an optimal
       * angle will be determined, that the area of the rectLocation minimizes.
       *
       * Default value: false
       */
      bool optimalBox;

      /**
       * If true, the bounding box length is used for determining
       * which axes is the main axes. If false, this is determined
       * by a PCA of the contour points.
       *
       * Default value: false
       */
      bool useLengths;

    };

    /**
     * default constructor
     */
    boundingBox();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    boundingBox(const boundingBox& other);

    /**
     * destructor
     */
    virtual ~boundingBox();

    /**
     * returns the name of this type ("boundingBox")
     */
    virtual const char* getTypeName() const;

    /**
     * Take the object specified by the area points, and
     * create a new image with only the object and a border of the specified
     * size on the srcdest.
     * @param srcdest matrix with the source data.  The result
     *                 will be left here too.
     * @param obj a contour which specifies which points of the image
     *            belong to the object.  Only the area points, center and
     *            boundary points
     * @return true if successful, false otherwise
     */
    bool apply(matrix<T>& srcdest,const areaPoints& obj) const;

    /**
     * Take the object specified by the area points on the given contour, and
     * create a new image with only the object and a border of the specified
     * size on the srcdest.
     * @param src matrix with the source data.
     * @param obj the areaPoints list which specifies which points of
     *            the image belong to the object.  The boundary must
     *            be calculated before calling this member (i.e. you
     *            need to call obj.computeBoundary()).  This is not
     *            done automatically, because many functors (like
     *            lti::objectsFromMask) compute the bounding box
     *            directly and computing it twice can cost too much
     *            time!
     * @param dest matrix where the destination data will be stored.
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& src,
               const areaPoints& obj,
               matrix<T>& dest) const;

    /**
     * Take the object specified by the area points on the given contour, and
     * create a new image with only the object and a border of the specified
     * size on the srcdest.
     * @param src matrix with the source data.
     * @param obj the areaPoints list which specifies which points of
     *            the image belong to the object.  The boundary must
     *            be calculated before calling this member (i.e. you
     *            need to call obj.computeBoundary()).  This is not
     *            done automatically, because many functors (like
     *            lti::objectsFromMask) compute the bounding box
     *            directly and computing it twice can cost too much
     *            time!
     * @param dest matrix where the destination data will be stored.
     * @param loc a rectangular location that indicates the smallest
     *            rectangular region that contains the hole object
     * @return true if successful, false otherwise
     */
    bool apply(const matrix<T>& src,
               const areaPoints& obj,
               matrix<T>& dest,
               rectLocation& loc) const;


    /**
     * computes the oriented rectangular bounding box for the given
     * area points, and leaves the result as a rectLocation.
     *
     * @param obj areaPoints with the object description
     * @param loc the rectangular location will represent the smallest
     *            rectangular region that contains all points of the
     *            areaPoints obj.
     * @return true if successful, false otherwise.
     */
    bool apply(const areaPoints& obj,
               rectLocation& loc) const;

    /**
     * computes the oriented rectangular bounding box for the given
     * io points, and leaves the result as a rectLocation.
     *
     * @param obj io points with the object description
     * @param loc the rectangular location will represent the smallest
     *            rectangular region that contains all points of the
     *            io points obj.
     * @return true if successful, false otherwise.
     */
    bool apply(const ioPoints& obj,
               rectLocation& loc) const;

    /**
     * computes the oriented rectangular bounding box for the given
     * point list, and leaves the result as a rectLocation.
     *
     * @param obj point list the object description
     * @param loc the rectangular location will represent the smallest
     *            rectangular region that contains all points of the list.
     * @return true if successful, false otherwise.
     */
    bool apply(const pointList& obj,
               rectLocation& loc) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    boundingBox& copy(const boundingBox& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:
    /**
     * calculate the source box of the source image, the size of the
     * destination image, and the position of the box on the destination
     * image.
     * @param obj the area points of the object.  It is
     *            assumed that the boundary of the object is up to date.
     *            (see lti::pointList::updateBoundary())
     * @param destSize size of the destination image
     * @param delta translation vector of the area points to the new image.
     */
    void calculatePositions(const areaPoints& obj,
                            point& destSize,
                            point& delta) const;


    /**
     * compute the location given the angle in the location.
     */
    void computeLocation(const pointList& pts,
                         rectLocation& loc) const;

    /**
     * location from the convex hull
     */
    bool computeLocationFromCH(const pointList& convexHull,
                               rectLocation& loc) const;
  };
}

#endif

