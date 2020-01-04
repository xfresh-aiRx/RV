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
 * file .......: ltiQuadTreeSegmentation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 15.12.2003
 * revisions ..: $Id: ltiQuadTreeSegmentation.h,v 1.2 2005/06/16 12:35:43 doerfler Exp $
 */

#ifndef _LTI_QUAD_TREE_SEGMENTATION_H_
#define _LTI_QUAD_TREE_SEGMENTATION_H_

#include "ltiMacroSymbols.h"

// only for compilers different than VC++ 6.0 available
#ifdef _LTI_MSC_6

#pragma message("Insufficient C++ Template Support for lti::regionGraphMeans.")
#pragma message("You need a newer compiler")

#else

#include "ltiVector.h"
#include "ltiSegmentation.h"
#include "ltiTree.h"
#include "ltiImage.h"

namespace lti {
  /**
   * Quad-Tree based color image segmentation method.
   *
   * This is a very simple but inefficient method to segment color
   * images.  It is provided because it is one of the classical
   * methods of split-and-merge segmentation techniques.
   *
   * A quad-tree based partition of the image if first done, and after that
   * neighbor regions are merged if they are similar enough.
   */
  class quadTreeSegmentation : public segmentation {
  public:
    /**
     * The parameters for the class quadTreeSegmentation
     */
    class parameters : public segmentation::parameters {
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

      /**
       * If any of the std. deviation of the R,G or B components of the colors
       * in a region exceeds one of the given threshold, the region will be
       * split.
       *
       * The value range of the given components is assumed to be between
       * 0 and 255.
       *
       * Default value: 5
       */
      float splitThreshold;

      /**
       * If two neighbor regions have in all three color components a
       * mean distance under the given threshold, they will be merged.
       *
       * Default value: 3
       */
      float mergeThreshold;
      
      /**
       * Minimum region size in the split stage.
       *
       * This value must always be greater or equal (1,1)
       *
       * Default value: (3,3)
       */
      point minRegionSize;
    };

    /**
     * Default constructor
     */
    quadTreeSegmentation();

    /**
     * Construct a functor using the given parameters
     */
    quadTreeSegmentation(const parameters& par);

    /**
     * Copy constructor
     * @param other the object to be copied
     */
    quadTreeSegmentation(const quadTreeSegmentation& other);

    /**
     * Destructor
     */
    virtual ~quadTreeSegmentation();

    /**
     * Returns the name of this type ("quadTreeSegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * Generate a second color image, where each found region gets the mean
     * color obtain from the original image.
     *
     * This is not the real segmentation, but for visualization purposes some
     * times this is the desired functionality.
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,image& dest) const;

    /**
     * Generate a second color image, where each found region gets the mean
     * color obtain from the original image.
     *
     * This is not the real segmentation, but for visualization purposes some
     * times this is the desired functionality.
     *
     * @param src image with the source data.
     * @param mask labeled mask with the segmented regions.
     * @param pal color palette for each region (the mean value)
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
                     imatrix& mask,
                     palette& pal) const;


    /**
     * Split the given image using a quad-tree strategy.
     *
     * Each detected region will have its own label in the resulting mask.
     *
     * @param img input color image
     * @param mask labeled mask with one integer ID per detected region.
     * @return true if successful or false otherwise.
     */
    bool split(const image& img,
               imatrix& mask) const;

    /**
     * Copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    quadTreeSegmentation& copy(const quadTreeSegmentation& other);

    /**
     * Alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    quadTreeSegmentation& operator=(const quadTreeSegmentation& other);

    /**
     * Returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * Returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * Compute the std. deviations in the given region of the image
     */
    bool stats(const image& img,
               const point& from,
               const point& to,
               trgbPixel<float>& stats) const;
    /**
     * Recursively compute the quad-tree for the image
     */
    bool split(const image& img,
               const float& threshold,
               const point& minSize,
               tree<rectangle>::node& qtree) const;
  };
}

#endif
#endif
