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
 * file .......: ltiMaskImage.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 6.8.2003
 * revisions ..: $Id: ltiMaskImage.h,v 1.6 2005/06/16 12:35:11 doerfler Exp $
 */

#ifndef _LTI_MASK_IMAGE_H_
#define _LTI_MASK_IMAGE_H_

#include "ltiImage.h"
#include "ltiFunctor.h"

namespace lti {
  /**
   * This functor is used to mask color images with labeled or
   * unlabeled masks.  You can give a color palette to indicate which
   * colors should be used for each region.  Using the alpha (or
   * dummy) rgbPixel component in the palette entries you can specify
   * the transparency degree of each region in the mask.  This way,
   * you can completely suppress regions (like background), leave regions
   * complete untouched, or overlay colored patches.
   * 
   * Example
   *
   * \code
   *
   * #include <ltiKMeansSegmentation.h>
   * #include <ltiMaskImage.h>
   * #include <ltiViewer.h>
   * #include <cstdio>
   * ...
   * 
   * lti::image img,masked;
   * lti::imatrix mask;
   *
   * // let's assume, you load a colored image into img
   *
   * // simple segmentation 
   * lti::kMeansSegmentation segmenter;
   * segmenter.apply(img,mask);
   *
   * // get a masked image
   * lti::maskImage masker(true); // assume labeled mask and enhance borders
   * masker.apply(img,mask,masked);
   *
   * // show original image and masked one
   * lti::viewer vieworig("Original");
   * lti::viewer viewmask("Masked");
   *
   * vieworig.show(img);
   * viewmask.show(masked);
   *
   * getchar(); // wait for ENTER key pressed
   * \endcode
   */
  class maskImage : public functor {
  public:
    /**
     * the parameters for the class maskImage
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
       * If true, the given mask will be assumed as labeled and all values
       * will be considered as a different object label.  If false, only 
       * the background label will be considered as background and the rest
       * of labels as objects.  In the latter case the "colors" entry 0 will
       * be used for background and the colors entry 1 for not-background.
       * 
       * Default value: false
       */
      bool assumeLabeledMask;

      /**
       * background label used to the background, in case assumeLabeldMask is
       * false.
       * 
       * Default value: 0
       */
      int backgroundLabel;
      
      /**
       * Colors used to mask each label.  If the mask being used
       * contains more labels than color entries, a modulo-like
       * operation will be used to choose the next color.  The color
       * entry 0 is used only one.  Let k be the index and n the
       * number of color entries.  The color used will be k if k<n,
       * 1+(k%(n-1)) otherwise.
       *
       * The \e dummy component of each rgbPixel will be used to
       * indicate the transparency level: 0 means completely opaque
       * and 255 totally transparent.  This way, you can place colored
       * indicators in visualization tasks.
       * 
       * Default value: - (0,0,0,0),         // black background opaque
       *                - (255,255,255,255), // white 100% transparent
       *                - (255,128,128,128), // red 50% transparent
       *                - (128,255,128,128), // green 50% transparent
       *                - (128,128,255,128), // blue 50% transparent
       *                - (255,255,0,128),   // yellow 50% transparent
       *                - (255,0,255,128),   // magenta 50% transparent
       *                - (0,255,255,128)    // cyan 50% transparent
       */
      palette colors;

      /**
       * Border colors.
       *
       * This parameters is only used in case you activate
       * \a enhanceRegionBorders, which is normally set to false.
       *
       * If this palette is empty, the colors for the borders will be
       * the solid colors specied in \a colors, which are a the same time
       * the used for the region, with the transparency specified there.
       *
       * You can optionally specify the colors for the borders, also
       * with transparency, so that you have much more flexibility with
       * the produced masks.
       *
       * Default value: empty palette
       */
      palette borderColors;
      

      /**
       * If assumeLabeledMask is true, you can also activate a region
       * border enhancement.
       *
       * If the \a borderColors is empty, the borders of the regions will be
       * drawn without any transparency, using the colors in \a colors.  Note
       * that in this case, the enhancement will be noticeable only if the
       * used colors have some transparency!
       *
       * If the \a borderColors parameters is not empty, the colors therein,
       * with the respective transparency will be used.
       * 
       * Default value: false
       */
      bool enhanceRegionBorders;

    };

    /**
     * default constructor
     */
    maskImage();

    /**
     * create a functor with the specified parameter attributes
     */
    maskImage(const bool assumeLabeledMask,
              const bool enhanceRegionBorders=true);

    /**
     * Construct a functor using the given parameters
     */
    maskImage(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    maskImage(const maskImage& other);

    /**
     * destructor
     */
    virtual ~maskImage();

    /**
     * returns the name of this type ("maskImage")
     */
    virtual const char* getTypeName() const;

    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param colors color entries used to mask the image.  The "dummy" entry
     *        of each pixel denotes the transparency of the color used: 0 means
     *        opaque and 255 totally transparent.  The \e colors attribute of
     *        the parameters class will be ignored.
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel8& mask,
               const palette& colors,
               image& dest) const;

    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param colors color entries used to mask the image.  The "dummy" entry
     *        of each pixel denotes the transparency of the color used: 0 means
     *        opaque and 255 totally transparent. The \e colors attribute of
     *        the parameters class will be ignored.
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const imatrix& mask,
               const palette& colors,
               image& dest) const;

    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const channel8& mask,
               image& dest) const;

    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src,
               const imatrix& mask,
               image& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    maskImage& copy(const maskImage& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    maskImage& operator=(const maskImage& other);

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
     * combine two pixels considering the transparency of the second one
     */
    inline void combine(const rgbPixel& src,
                        const rgbPixel& mask,
                        rgbPixel& dest) const;

    /**
     * fix the index to fit into the colors
     */
    inline int fixIdx(const int idx,const int palLastIdx) const;


    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param colors color entries used to mask the image.  The "dummy" entry
     *        of each pixel denotes the transparency of the color used: 0 means
     *        opaque and 255 totally transparent. The \e colors attribute of
     *        the parameters class will be ignored.
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool labeled(const image& src,
                 const imatrix& mask,
                 const palette& colors,
                 image& dest) const;

    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param colors color entries used to mask the image.  The "dummy" entry
     *        of each pixel denotes the transparency of the color used: 0 means
     *        opaque and 255 totally transparent. The \e colors attribute of
     *        the parameters class will be ignored.
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool unlabeled(const image& src,
                 const imatrix& mask,
                 const palette& colors,
                 image& dest) const;
    
    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param colors color entries used to mask the image.  The "dummy" entry
     *        of each pixel denotes the transparency of the color used: 0 means
     *        opaque and 255 totally transparent. The \e colors attribute of
     *        the parameters class will be ignored.
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool labeled(const image& src,
                 const channel8& mask,
                 const palette& colors,
                 image& dest) const;

    /**
     * Mask the src image using the given mask and colors for mask entries.
     * @param src image with the source data.
     * @param mask labeled or unlabeled mask
     * @param colors color entries used to mask the image.  The "dummy" entry
     *        of each pixel denotes the transparency of the color used: 0 means
     *        opaque and 255 totally transparent. The \e colors attribute of
     *        the parameters class will be ignored.
     * @param dest destination masked image.
     * @return true if apply successful or false otherwise.
     */
    bool unlabeled(const image& src,
                   const channel8& mask,
                   const palette& colors,
                   image& dest) const;
    
  };
}

#endif

