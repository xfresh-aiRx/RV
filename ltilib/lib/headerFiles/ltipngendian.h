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
 * file .......: ltipngendian.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: Byte conversion functions for little endian systems.
 * revisions ..: $Id: ltipngendian.h,v 1.4 2003/02/17 07:16:14 alvarado Exp $
 */

#ifndef _LTI_LITTLEENDIAN_H
#define _LTI_LITTLEENDIAN_H

#include "ltiTypes.h"

namespace lti{
  namespace png {
inline uint16 SystemToLittleEndian (uint16 value)
{
  return value ;
}
inline uint32 SystemToLittleEndian (uint32 value)
{
  return value ;
}
inline uint16 SystemToBigEndian (uint16 value)
{
  return (uint16) ((value&0xFF) << 8)|((value&0xFF00)>>8) ;
}
inline uint32 SystemToBigEndian (uint32 value)
{
  return ((value&0xFF000000L)>>24)|((value&0xFF0000L)>>8)
         | ((value&0xFF00L)<<8) | ((value&0xFFL)<<24) ;
}

inline uint16 LittleEndianToSystem (uint16 value)
{
  return value ;
}
inline uint32 LittleEndianToSystem (uint32 value)
{
  return value ;
}
inline uint16 BigEndianToSystem (uint16 value)
{
  return (uint16) ((value&0xFF) << 8)|((value&0xFF00)>>8) ;
}
inline uint32 BigEndianToSystem (uint32 value)
{
  return ((value&0xFF000000L)>>24)|((value&0xFF0000L)>>8)
         | ((value&0xFF00L)<<8) | ((value&0xFFL)<<24) ;
}
}
} // namespace lti

#endif

