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
 * creation ...: 17.12.2002
 * revisions ..: $Id: ltiSplitImageToYUV.h,v 1.5 2004/05/03 12:18:39 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_Y_U_V_H_
#define _LTI_SPLIT_IMAGE_TO_Y_U_V_H_

#include "ltiSplitImage.h"

namespace lti {
  /**
   * Computes the YUV values from a given RGB color representation (rgbPixel).
   *
   * For most accurate results we use values of the  
   * ITU-RS601 (formerly known as CCIR-601)
   * value Range here: Y [16...235], U/V [16..240]
   * \code
   * Y =  0.256635*R + 0.503971*G + 0.098218*B + 16
   * U = -0.148135*R - 0.290902*G + 0.439036*B + 128.0 
   * V =  0.439327*R - 0.367673*G - 0.071655*B + 128.0 
   * \endcode
   *
   * in the norm Cb is used instead of U and Cr instead of V but here
   * we work with YUV.
   *
   * Most common values found in the literature and internet are
   * \code
   * Y =  0.299*R + 0.587*G + 0.114*B 
   * U = -0.169*R - 0.331*G + 0.500*B + 128.0 
   * V =  0.500*R - 0.419*G - 0.081*B + 128.0 
   * \endcode
   * other transformation values can be found but are usually just a result
   * of nomalization .
   *
   * Gonzales and Woods only deal with the American YIQ standard.
   *
   * @ingroup gColor
   */
  class splitImageToYUV : public splitImage {
  public:

    /**
     * default constructor
     */
    splitImageToYUV();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    splitImageToYUV(const splitImageToYUV& other);

    /**
     * destructor
     */
    virtual ~splitImageToYUV();

    /**
     * returns the name of this type ("splitImageToYUV")
     */
    virtual const char* getTypeName() const;
      
    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToYUV& copy(const splitImageToYUV& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    splitImageToYUV& operator=(const splitImageToYUV& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;


    /**
     *  split pixel into float values (between 0 and 1)
     */
    inline virtual bool apply(const rgbPixel& pixel,
                              float& c1,
                              float& c2,
                              float& c3) const; 

    /**
     * Split pixel into 8-bit values (between 0 and 255)
     *
     * N.B.: when casting the transformation result to unsigned shorts
     * (8-bit channel) major rounding errors will occur.
     *
     * As a result, the merging operation might produce negative
     * values or values > 1, which are truncated subsequently.
     *
     * When accurate Y, U and V values are required, prefer float values
     */
    inline virtual bool apply(const rgbPixel& pixel,
                              ubyte& c1,
                              ubyte& c2,
                              ubyte& c3) const;  

    /**
     *  split pixel into float channels
     */   
    virtual bool apply(const image& img,
                       channel& c1,
                       channel& c2,
		       channel& c3) const;
    
    /**
     * Split image into 8-bit channels.
     *
     * N.B.: when casting the transformation result to unsigned shorts
     * (8-bit channel) major rounding errors will occur.
     *
     * As a result, the merging operation might produce negative
     * values or values > 1, which are truncated subsequently.  When
     * accurate Y, U and V values are required, prefer float values.
     */
    virtual bool apply(const image& img,
                       channel8& c1,
                       channel8& c2,
		       channel8& c3) const;

  };


  //
  // --------------------------------
  // implementation of inline methods
  // --------------------------------
  //

  // split pixel into float values (between 0 and 1)
  inline bool splitImageToYUV::apply(const rgbPixel& pixel,
                                     float& c1,
                                     float& c2,
                                     float& c3) const {
    
    const float red = float ( pixel.getRed() );
    const float green = float ( pixel.getGreen() );
    const float blue = float ( pixel.getBlue() );
    
    //   most common values in literature and internet
    //   Y =  0.299*R + 0.587*G + 0.114*B 
    //   U = -0.169*R - 0.331*G + 0.500*B + 128.0 
    //   V =  0.500*R - 0.419*G - 0.081*B + 128.0 
    //   other transformation values can be found but are usually just a result
    //   of nomalization 
    //   Gonzales and Woods only deal with the American YIQ standard

    //   For most accurate results we use values of the  
    //   ITU-RS601 (formerly known as CCIR-601)
    //   value Range here: Y [16...235], U/V [16..240]
    //   Y =  0.256635*R + 0.503971*G + 0.098218*B + 16
    //   U = -0.148135*R - 0.290902*G + 0.439036*B + 128.0 
    //   V =  0.439327*R - 0.367673*G - 0.071655*B + 128.0 
    //   in the norm Cb is used instead of U and Cr instead of V
    //   but here we work with YUV 

    c1 = ( red *0.256635f +
          green *0.503971f +
          blue *0.098218f +16)/255.0f;      // Y
    c2 = (red *(-0.148135f) +
          green *(-0.290902f) +
          blue *0.439036f +128)/255.0f;      // U
    c3 = (red *0.439327f +
          green *(-0.367673f) +
          blue *(-0.071655f)+128)/255.0f;   // V
    
    return true;
  }

  // split pixel into 8-bit values (between 0 and 255)
  // N.B.: when casting the transformation result to unsigned shorts
  // (8-bit channel) major rounding errors will occur.
  // As a result, the merging operation might
  // produce negative values or values > 1,  which are truncated subsequently.
  // When accurate Y, U and V values are required, prefer float values!
  inline bool splitImageToYUV::apply(const rgbPixel& pixel,
                                     ubyte& c1,
                                     ubyte& c2,
                                     ubyte& c3) const {

    const float red = float ( pixel.getRed() );
    const float green = float ( pixel.getGreen() );
    const float blue = float ( pixel.getBlue() );

  // see above for explanation of magic numbers
    c1 = (ubyte)(red *0.256635f +
                 green *0.503971f +
                 blue *0.098218f +16);      // Y 
    c2 = (ubyte)(red *(-0.148135f) +
                 green *(-0.290902f) +
                 blue *0.439036f +128);      // U
    c3 = (ubyte)(red *0.439327f +
                 green *(-0.367673f) +
                 blue *(-0.071655f)+128);   // V

    return true;
  }

  // ---------------------------------------
  // end of implementation of inline methods
  // ---------------------------------------

}

#endif



