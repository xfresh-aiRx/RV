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
 * file .......: ltiKMeansSegmentation.h
 * authors ....: Axel Berner
 * organization: LTI, RWTH Aachen
 * creation ...: 3.2.2002
 * revisions ..: $Id: ltiKMeansSegmentation.h,v 1.16 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_K_MEANS_SEGMENTATION_H_
#define _LTI_K_MEANS_SEGMENTATION_H_

#include "ltiKMColorQuantization.h"
#include "ltiSegmentation.h"

namespace lti {
  /**
   * A segmentation algorithm which is based on a color-quantization
   * algorithm followed by a smoothing filter.  The quantization of
   * the colors is done with the k-Means algorithm, which creates an
   * optimal color-palette, to which all original image colors are
   * mapped to.  The smoothing filter (None, Median, K-Nearest, ...)
   * can be choosen with the parameters objects.
   */
  class kMeansSegmentation : public segmentation {
  public:
    /**
     * the parameters for the class kMeansSegmentation
     */
    class parameters : public segmentation::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * construct a parameters object with the given number of
       * quantization colors.
       *
       * @param numCols number of colors
       */
      parameters(const int numCols);

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
       * number of colors for the image quantization and other
       * quantization parameters
       *
       * Default:  numberOfColors: 16
       *           thresholdDeltaPalette: 1
       */
      kMColorQuantization::parameters quantParameters;

      /**
       * Smooth filter type
       */
      enum eSmoothFilterType {
        Nothing  = 0,     /**< Do not smooth the color quantized image */
        Median   = 1,     /**< Use Median Filter (lti::medianFilter)   */
        KNearest = 2      /**< Use K-Nearest Neighbor filter
                           * (lti::kNearestNeighFilter)
                           */
      };

      /**
       * kind of smoothing-filter.  Use the constants defined in the parameters
       * to specify the filter type:  Nothing, Median or KNearest
       *
       * Default: KNearest
       */
      eSmoothFilterType smoothFilter;

      /**
       * kerner size of the smoothing-filter
       *
       * Default: 5
       */
      int kernelSize;
    };

    /**
     * default constructor
     */
    kMeansSegmentation();

    /**
     * default constructor with parameters object
     */
    kMeansSegmentation(const parameters& par);

    /**
     * construct a segmentation functor with the given numbers of colors
     * in the quantization stage.
     */
    kMeansSegmentation(const int numCols);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    kMeansSegmentation(const kMeansSegmentation& other);

    /**
     * destructor
     */
    virtual ~kMeansSegmentation();

    /**
     * returns the name of this type ("kMeansSegmentation")
     */
    virtual const char* getTypeName() const;

    //-------------------------------------------------------------------------

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest resulting labeled mask
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,matrix<int>& dest) const;

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest resulting labeled mask
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,channel8& dest) const;

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest resulting labeled mask
     * @param pal the color palette found by the k-Means algorithm
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,matrix<int>& dest,palette& pal) const;

    /**
     * operates on the given %parameter.
     * @param src image with the source data.
     * @param dest resulting labeled mask
     * @param pal the color palette found by the k-Means algorithm
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,channel8& dest,palette& pal) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kMeansSegmentation& copy(const kMeansSegmentation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    kMeansSegmentation& operator=(const kMeansSegmentation& other);

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

