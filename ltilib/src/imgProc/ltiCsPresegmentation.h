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
 * file .......: ltiCsPresegmentation.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 8.11.2001
 * revisions ..: $Id: ltiCsPresegmentation.h,v 1.11 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_CS_PRESEGMENTATION_H_
#define _LTI_CS_PRESEGMENTATION_H_

#include "ltiImage.h"
#include "ltiSegmentation.h"
#include "ltiKMColorQuantization.h"

namespace lti {
  /**
   * The csPresegmentation functor (states for color space presegmentation)
   * tries to detect which pixels belong to the background and which not.
   * For its task there are some assumtions/requirements:
   *
   * - The background must be homogeneous, i.e. no textures are allowed.
   *   The best results are obtained with a black background, but this is
   *   not necessary.
   * - The number of "perceivable" colors in the image is less than
   *   the number of quantization colors given in the parameters.
   * - There is a high probability that the border of the image
   *   contains only background pixels.
   *
   * These conditions are not met in many applications, for which other
   * segmentation approaches have to be found.
   *
   * The algorithm works as follows:
   *
   * First, a color quantization using k-Means is done.  This produces
   * usually a "noisy" index mask and a palette.  A median filter or a
   * k-Nearest-Neighbor Filter smooths this mask.  Assuming that the border
   * of the image contains mostly background pixels, some simple
   * statistics are done, to decide which palette entries belong to
   * the background.  Pixels belonging to other palette entries are
   * taken as the object pixels.
   *
   * See the lti::csPresegmentation::parameters for more information.
   * */
  class csPresegmentation : public segmentation {
  public:
    /**
     * the parameters for the class csPresegmentation
     */
    class parameters : public segmentation::parameters {
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
       * Size of the median filter kernel or k-Nearest-Neighbor filter
       * used to smooth the quantization mask.
       *
       * Default value: 5
       */
      int smoothingKernelSize;

      /**
       * Parameters for the k-means color quantization.
       *
       * Default values: quantParameters.numberOfColors=12
       *                 quantParameters.thresholdDeltaPalette = 0.5f
       *                 quantParameters.maximalNumberOfIterations = 50
       */
      kMColorQuantization::parameters quantParameters;

      /**
       * If false, the quantization algorithms uses the last palette as
       * preinitialization, to speed up the segmentation of several images
       * showing the same object from different perspectives.  If true,
       * the last quantization will be always ignored.
       *
       * Default value: false
       */
      bool useAlwaysNewPalette;

      /**
       * Maximal size of the border of the image to be considered to
       * contain almost background pixels.  The real border will have the
       * smallest number between this parameter and one half of the
       * rows or columns of the image being used.
       *
       * Default value: 16
       */
      int borderSize;

      /**
       * constant to define all border elements
       */
      static const int All;

      /**
       * constant to define the top border element
       */
      static const int Top;

      /**
       * constant to define the bottom border element
       */
      static const int Bottom;

      /**
       * constant to define the left border element
       */
      static const int Left;

      /**
       * constant to define the right border element
       */
      static const int Right;

      /**
       * Decides which parts of the border should be considered for
       * the statistics.  You can use combinations of constants Top,
       * Bottom, Left and Right or All to produce the wished results.
       *
       * Default value: lti::csPresegmentation::parameters::All
       *
       * Examples:
       *
       * \code
       * lti::csPresegmentation::parameters param;
       *
       * // to consider all but the bottom of the image:
       * param.borderParts = lti::csPresegmentation::parameters::All &
       *                     ~lti::csPresegmentation::parameters::Bottom;
       *
       *
       * // to consider only the left and right borders
       * param.borderParts = lti::csPresegmentation::parameters::Left |
       *                     lti::csPresegmentation::parameters::Right;
       * \endcode
       *
       * Bottom and Top parts have the whole width of the image.  Left
       * and Right parts have a height equal to the height of the image minus
       * twice the borderSize:
       *
       * \code
       * _____________________
       * |        Top        |
       * |___________________|
       * | L|             |R |
       * | e|             |i |
       * | f|             |g |
       * | t|             |h |
       * |  |             |t |
       * |__|_____________|__|
       * |      Bottom       |
       * |___________________|
       *
       * \endcode
       *
       */
      int borderParts;

      /**
       * If true, all color entries found withing the given border will be
       * assumed to be background, undependently of their values.  If false,
       * only the most representative colors in the border will be assumed
       * to be background.
       *
       * Default value: false;
       */
      bool forceBorderToBackground;

      /**
       * If true, the value in the segmentation mask for each objekt will
       * correspond to the detected color palette entry in the color
       * quantization.  If false, the value in the mask for the non-background
       * pixels will be 255
       *
       * Default value: false;
       */
      bool labelObjects;

      /**
       * Tolerance for background colors.
       *
       * Mean and Variance values will be computed for each
       * RGB-component of all palette entries that are candidates to
       * be background (at the beginning all entries in the palette
       * are background candidates).  Let t be this tolerance value.
       * All entries in the palette, which are candidates to be
       * background must satisfy for each color component c in RGB:
       * \f$(c-mean)^2 \le t \cdot variance\f$ in order to remain
       * as background candidates.
       *
       * Default value: 9;
       */
      float backgroundTolerance;

      /**
       * type to specify the smoothing filter to be used
       */
      enum eFilterType {
        Nothing = 0,    /**< No smoothing filter should be used */
        Median = 1,     /**< Median Filter */
        KNearest = 2    /**< K-Nearest-Neighbour Filter */
      };

      /**
       * Smoothing filter to be used.
       *
       * Default: Median
       */
      eFilterType filterType;
    };

    /**
     * default constructor
     */
    csPresegmentation();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    csPresegmentation(const csPresegmentation& other);

    /**
     * destructor
     */
    virtual ~csPresegmentation();

    /**
     * returns the name of this type ("csPresegmentation")
     */
    virtual const char* getTypeName() const;

    /**
     * apply the presegmentation algorithm to the given image and
     * return a mask where the background pixels will be mark with the
     * value zero and the non-background pixels with a non-zero value.
     *
     * Note that some information of this segmentation is "stored" in
     * the internal state, which affects the next segmentation steps.
     * (see lti::csPresegmentation::parameters::useAlwaysNewPalette)
     *
     * @param src image with the source data.
     * @param mask the mask with the found objects.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,channel8& mask);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    csPresegmentation& copy(const csPresegmentation& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    csPresegmentation& operator=(const csPresegmentation& other);

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
     * last used palette
     */
    palette lastPalette;

    /**
     * valid background returns true if the given pixel lies between
     * the mean value and the given variance.
     */
    inline bool validBackground(const rgbPixel& p,
                                const trgbPixel<float>& mean,
                                const trgbPixel<float>& var,
                                const float& tolerance) const;
  };
}

#endif

