/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 *
 * This code is based on the original C++ code by John M. Miano
 * (Colosseum Builders, Inc.) which contains following licence: (the
 * original licence file can be found on 00readme.txt in the io/png
 * and io/jpeg directories):
 *
 ******************************************************************************
 *
 * Colosseum Builders C++ Image Library
 *
 * Copyright 1997, 1998, 1999
 * All Rights Reserved
 *
 * Permision is granted to use, modify this software provided these conditions
 * are followed:
 *
 * 1. You understand this software comes with no warranties of any kind
 *    whatsoever.
 * 2. You assume full responsibility for the use of this software and transmit
 *    no liability to the authors.
 * 3. You assume the responsility for obtaining licenses for from any patent
 *    holders.
 * 4. If you distribute this code in source form, you must include this file
 *    unmodified and document the changes you have made.
 * 5. If you incorporate this code in a product distributed without source code
 *    you state included in your documentation that it is based in part on
 *    software from Colosseum Builders, Inc.
 *
 *    THIS SOFTWARE IS PROVIDED AS IS. WE CAN'T TEST IT ON YOUR SYSTEM SO
 *    USE AT YOUR OWN RISK. YOU HAVE THE SOURCE SO YOU HAVE NO EXCUSES.
 *
 ******************************************************************************
 *
 * The modifications of the original code are mainly the adaptation
 * for the LTI-Lib types and the use of namespaces to create an own
 * scope for the original functions
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltipngsystemspecific.h
 * authors ....: Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 15.11.99
 * description.: platform specific declarations for ltilib
 * revisions ..: $Id: ltipngsystemspecific.h,v 1.9 2003/10/30 10:02:50 alvarado Exp $
 */


#ifndef _LTI_PNGSYSTEMSPECIFIC_H
#define _LTI_PNGSYSTEMSPECIFIC_H


#include "ltiTypes.h"

// 2002/01/28
// added "pack push", because "pack(1)" results in serious errors
// when including this header before other headers

#ifdef _LTI_MSC_VER
# pragma pack (push, PACKSTATE_LTI_PNGSYSTEMSPECIFIC_H)
#endif

namespace lti{
  namespace png {

  const int PNGMAX8BITSAMPLEVALUE = 255;

  // Length of the PNG signature.
  const int PngSignatureSize = 8 ;
  const ubyte PngSignature [PngSignatureSize] = {
    137, 80, 78, 71, 13, 10, 26, 10,
  } ;

  enum PngColorType {
    Grayscale = 0,
    RGB = 2,
    Palette = 3,
    GrayscaleAlpha = 4,
    RGBAlpha = 6,
  } ;

  // Filter type defintiions
  enum  PngFilterType {
    FILTERNONE = 0,
    FILTERSUB = 1,
    FILTERUP = 2,
    FILTERAVERAGE = 3,
    FILTERPAETH = 4,
  } ;


  //////////////////////////////////////////////////////////////////////
  // platform specifics for the PNG codec part of ltilib
  //////////////////////////////////////////////////////////////////////

  // Physical layout of the IDAT header.
# ifdef _LTI_MSC_VER
# pragma warning (disable : 4103)
# pragma pack (1)
  struct PngImageHeader {
    uint32 width;
    uint32 height;
    ubyte bitdepth;
    ubyte colortype;
    ubyte compressionmethod;
    ubyte filtermethod;
    ubyte interlacemethod;
  } ;
# elif _LTI_GNUC
  struct PngImageHeader {
    uint32 width __attribute__ ((packed));
    uint32 height __attribute__ ((packed));
    ubyte bitdepth ;
    ubyte colortype ;
    ubyte compressionmethod ;
    ubyte filtermethod ;
    ubyte interlacemethod ;
  } ;
#endif

  // Physical Layout of a cHRM chunk
# ifdef _LTI_MSC_VER
# pragma warning (disable : 4103)
# pragma pack (1)
  struct PngChromaticitiesData {
    uint32 whitepointx ;
    uint32 whitepointy ;
    uint32 redx ;
    uint32 redy ;
    uint32 greenx ;
    uint32 greeny ;
    uint32 bluex ;
    uint32 bluey ;
  };
# elif _LTI_GNUC
  struct PngChromaticitiesData {

    uint32 whitepointx __attribute__ ((packed));
    uint32 whitepointy __attribute__ ((packed));
    uint32 redx __attribute__ ((packed));
    uint32 redy __attribute__ ((packed));
    uint32 greenx __attribute__ ((packed));
    uint32 greeny __attribute__ ((packed));
    uint32 bluex __attribute__ ((packed));
    uint32 bluey __attribute__ ((packed));
  } ;
# endif

// Physical layout of a pPYs chunk
# ifdef _LTI_MSC_VER
# pragma warning (disable : 4103)
# pragma pack (1)
  struct PngPixelDimensions {
    uint32 pixelsx ;
    uint32 pixelsy ;
    ubyte unit ;
  };
# elif _LTI_GNUC
  struct PngPixelDimensions {
    uint32 pixelsx __attribute__ ((packed));
    uint32 pixelsy __attribute__ ((packed));
    ubyte unit __attribute__ ((packed));
  };
#endif

// Physical layout of a tIME chunk
# ifdef _LTI_MSC_VER
# pragma warning (disable : 4103)
# pragma pack (1)
  struct PngTimeData {
    uint16 year ;
    ubyte month ;
    ubyte day ;
    ubyte hour ;
    ubyte minute ;
    ubyte second ;
  };
# elif _LTI_GNUC
  struct PngTimeData {
    uint16 year __attribute__ ((packed));
    ubyte month ;
    ubyte day ;
    ubyte hour ;
    ubyte minute ;
    ubyte second ;
  } ;
# endif

  //
  //  Description:
  //    Path predictor function defined in section 6.6 of the PNG standard.
  //
  //  Parameters:
  //    left:  The pixel value for the value to the left of the current pixel.
  //    above: The value for the pixel above the current pixel.
  //    upperleft: The value for the pixel diagonally above and to the right
  //                of the current pixel.
  //
  static inline unsigned int PaethPredictor (ubyte left,
                                             ubyte above,
                                             ubyte upperleft) {
    int pp = left + above - upperleft ;
    int pa, pb, pc ;
    if (pp > left)
      pa = pp - left ;
    else
      pa = left - pp ;
    if (pp > above)
      pb = pp - above ;
    else
      pb = above - pp ;
    if (pp > upperleft)
      pc = pp - upperleft ;
    else
      pc = upperleft - pp ;

    if (pa <= pb && pa <= pc)
      return left ;
    else if (pb <= pc)
      return above ;
    else
      return upperleft ;
  }


  inline uint32 ChunkType (const byte type [5]) {
    uint32 value = type [0] | (type [1] << 8)
      | (type [2] << 16) | (type [3] << 24) ;
    return value ;
  }

  } // namespac png
} // namespace lti

#ifdef _LTI_MSC_VER
# pragma pack (pop, PACKSTATE_LTI_PNGSYSTEMSPECIFIC_H)
#endif

#endif

