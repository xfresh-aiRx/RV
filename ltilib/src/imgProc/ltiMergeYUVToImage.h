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
 * file .......: ltiSplitImageToYUV.h
 * authors ....: Volker Schmirgel
 * organization: LTI, RWTH Aachen
 * creation ...: 17.10.2003
 * revisions ..: $Id: ltiMergeYUVToImage.h,v 1.6 2004/05/03 12:18:38 alvarado Exp $
 */

#ifndef _LTI_MERGE_Y_U_V_TO_IMAGE_H_
#define _LTI_MERGE_Y_U_V_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Creates RGB values from given YUV values by
   * merging float or ubyte values to an rgbPixel,
   * merging channels(floats) or channel8s(ubytes) to an Image  
   *
   * For most accurate results we use values of the  
   * ITU-RS601 (formerly known as CCIR-601)
   * value Range here: Y [16...235], U/V [16..240] 
   * \code
   * R = Y + 1.370705 (V - 128)
   * G = Y - 0.698001 (V - 128) -0.337633 (U - 128)
   * B = Y + 1.732446 (U - 128)   
   * \endcode
   * @ingroup gColor
   */
  class mergeYUVToImage : public mergeImage {
  public:

    /**
     * constructor
     */
    mergeYUVToImage(void);

    
    /**
     * destructor
     */
    ~mergeYUVToImage();

    /**
     * return the name of this type
     */
    virtual const char* getTypeName() const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYUVToImage& copy(const mergeYUVToImage& other);
    
    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    mergeYUVToImage& operator=(const mergeYUVToImage& other);

    /**
     * returns a pointer to a clone of the functor.
     */
    virtual functor* clone() const;

    /**
     * merge channels Y, U, V to an image
     * @param Y the Y channel, i.e. black&white
     * @param U the U channel, chromatic 
     * @param V the V channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const matrix<float>& Y,
                       const matrix<float>& U,
                       const matrix<float>& V,
                       image& img) const;

    /**
     * merge  8-bit-channels Y, U, V to an image
     * @param Y the Y channel, i.e. black&white
     * @param U the U channel, chromatic 
     * @param V the V channel, chromatic
     * @param img the image to be splitted
     */
    virtual bool apply(const channel8& Y,
                       const channel8& U,
                       const channel8& V,
                       image& img) const;

    /**
     * merge the  values Y, U and V
     * to a pixel
     * @param Y the Y value, i.e. black&white
     * @param U the U  value, chromatic 
     * @param V the V value, chromatic 
     * @param pixel the merged pixel
     */
    inline virtual bool apply(const float& Y,
		       const float& U,
		       const float& V,
		       rgbPixel& pixel) const;

    /**
     * merge the   8-bit-values Y, U and V
     * to a pixel
     * @param Y the Y value, i.e. black&white
     * @param U the U  value, chromatic 
     * @param V the V value, chromatic 
     * @param pixel the merged pixel
     */ 
    inline virtual bool apply(const ubyte& Y,
                              const ubyte& U,
                              const ubyte& V,
                              rgbPixel& pixel) const;

    /**
     * merge the  8-bit-values Y, U and V
     * to a pixel
     * @param Y the Y value, i.e. black&white
     * @param U the U  value, chromatic 
     * @param V the V value, chromatic 
     * @param pixel the merged pixel
     * quick version with lookup tables and shifting, 
     * but not as accurate as the normal version
     */ 
    inline virtual bool applyQuick(const ubyte& Y,
                                   const ubyte& U,
                                   const ubyte& V,
                                   rgbPixel& pixel) const;

    /**
     * merge  8-bit-channels Y, U, V to an image
     * @param Y the Y channel, i.e. black&white
     * @param U the U channel, chromatic 
     * @param V the V channel, chromatic
     * @param img the image to be splitted
     * Quick, with lookup tables and shifting
     */
    virtual bool applyQuick(const channel8& Y,
                            const channel8& U,
                            const channel8& V,
                            image& img) const;

  protected:

    /**
     * Initialize the Look-Up-Tables
     */
    virtual void initializeLUTs();


    /**
     * look up tables to accelerate conversion YUV -> RGB
     */
    //
    static const int* lutUg;
    static const int* lutUb;
    static const int* lutVg;
    static const int* lutVr;
    //

  };

  //
  // -------------------------------------
  // Implementation of inline methods
  // -------------------------------------
  //

  // create rgbPixel (ubyte)  from YUV float values
  inline bool mergeYUVToImage::apply(const float& c1,
                                     const float& c2,
                                     const float& c3,
                                     rgbPixel& pixel) const {

    const float Y = c1 * 255.0f;
    const float U = c2 * 255.0f;
    const float V = c3 * 255.0f;

    //   For most accurate results we use values of the  
    //   ITU-RS601 (formerly known as CCIR-601)
    //   value Range here: Y [16...235], U/V [16..240] 
    //   R = Y + 1.370705 (V - 128)
    //   G = Y - 0.698001 (V - 128) -0.337633 (U - 128)
    //   B = Y + 1.732446 (U - 128)

    // new version with clamping
    float R = (Y + 1.370705f * (V - 128));
    float G = (Y - 0.698001f * (V - 128) - 0.337633f * (U - 128));
    float B = (Y + 1.732446f * (U - 128));

    // clamping
    if (R > 255) {
      R = 255.f;
    } else if ( R < 0 ) {
      R = 0.f;
    }
    if (G > 255) {
      G = 255.f;
    } else if ( G < 0 ) {
      G = 0;
    }
    if (B > 255) {
      B = 255.f;
    } else if ( B < 0 ) {
      B = 0.f;
    }

    pixel.set(static_cast<ubyte>(R),
              static_cast<ubyte>(G),
              static_cast<ubyte>(B),
              0);

    return true;
  }

  // merge 8-bit-values to create an rgbPixel
  inline bool mergeYUVToImage::apply(const ubyte& c1,
                                     const ubyte& c2,
                                     const ubyte& c3,
                                     rgbPixel& pixel) const {

    const float Y = static_cast<float>(c1);
    const float U = static_cast<float>(c2);
    const float V = static_cast<float>(c3);
    // see above for explanation of the magic numbers

    // new version with clamping
    float R = (Y + 1.370705f * (V - 128));
    float G = (Y - 0.698001f * (V - 128) - 0.337633f * (U - 128));
    float B = (Y + 1.732446f * (U - 128));

    // clamping
    if (R > 255) {
      R = 255.f;
    } else if ( R < 0 ) {
      R = 0.f;
    }
    if (G > 255) {
      G = 255.f;
    } else if ( G < 0 ) {
      G = 0;
    }
    if (B > 255) {
      B = 255.f;
    } else if ( B < 0 ) {
      B = 0.f;
    }

    pixel.set(static_cast<ubyte>(R),
              static_cast<ubyte>(G),
              static_cast<ubyte>(B),
              0);

    return true;
  }
 
  // merge 8-bit-values to create an rgbPixel qiuck version with lookup
  // tables, but not as accurate as the normal version use "initializeLUTs()"
  // once before using applyQuick!!
  inline bool mergeYUVToImage::applyQuick(const ubyte& c1,
                                          const ubyte& c2,
                                          const ubyte& c3,
                                          rgbPixel& pixel) const {

    const int Y = static_cast<int>(c1);
    const int U = static_cast<int>(c2);
    const int V = static_cast<int>(c3);

    // see above for explanation of the magic numbers
    //r = (Y + 1.370705f * (V - 128));
    //g = (Y - 0.698001f * (V - 128) - 0.337633f * (U - 128));
    //b = (Y + 1.732446f * (U - 128));
    // the following calculations do the same thing
    pixel.set(static_cast<ubyte>( Y + (lutVr[V]>>8) ),
              static_cast<ubyte>( Y - ((lutUg[U] + lutVg[V])>>8) ),
              static_cast<ubyte>( Y + (lutUb[U]>>8) ),
              0);

    return true;
  }

}
#endif



