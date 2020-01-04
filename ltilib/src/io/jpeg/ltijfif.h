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
 * file .......: jfif.h
 * authors ....: John Miano, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 27.10.00
 * revisions ..: $Id: ltijfif.h,v 1.4 2003/02/17 07:16:03 alvarado Exp $
 */

#ifndef _LTI_JFIF_H_
#define _LTI_JFIF_H_


//
//  JFIF Definitions
//

#include "ltijpeg.h"

#include "ltiIOFunctor.h"	  // für endianness!



namespace lti {
  namespace jpeg {

/**
 * JFIF Definitions
 */
class JfifHeader
{
public:
  JfifHeader();
  ~JfifHeader();

  /**
   * anonymous type for the variable data.  It encapsulates some information
   * for the jpeg image format.
   */
  struct {
    lti::uint16 length ;
    lti::ubyte identifier [5] ;
    lti::ubyte version [2] ;
    lti::ubyte units ;
    lti::uint16 xdensity ;
    lti::uint16 ydensity ;
    lti::ubyte xthumbnail ;
    lti::ubyte ythumbnail ;
  } data;

  int size() { return sizeof(data); };

  void read(std::istream &in_stream);
  void write(std::ostream &out_stream);

private:
  lti::endianness *big_endian;
};

/**
 * JfifExtension class
 */
struct JfifExtension
{
  lti::uint16 length ;
  lti::ubyte identifier [5] ;
  lti::ubyte extension_code ;
  enum { JpegThumbnail = 0x10,
         OneByteThumbnail = 0x11,
         ThreeByteThumbnail = 0x13, } ;
  // Extension Data follows
} ;

// To implement 12-bit data the return values would have to be
// at least 12 bits wide.

// Functions for YCbCr/RGB colorspace conversion
JPEGSAMPLE YCbCrToR (int yy, int cb, int cr) ;
JPEGSAMPLE YCbCrToG (int yy, int cb, int cr) ;
JPEGSAMPLE YCbCrToB (int yy, int cb, int cr) ;

JPEGSAMPLE RGBToY (JPEGSAMPLE red, JPEGSAMPLE green, JPEGSAMPLE blue) ;
JPEGSAMPLE RGBToCb (JPEGSAMPLE red, JPEGSAMPLE green, JPEGSAMPLE blue) ;
JPEGSAMPLE RGBToCr (JPEGSAMPLE red, JPEGSAMPLE green, JPEGSAMPLE blue) ;

}
}

#endif

