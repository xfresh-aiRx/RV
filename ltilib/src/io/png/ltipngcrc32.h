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
 * file .......: ltipngcrc32.h
 * authors ....: John M. Miano miano@colosseumbuilders.com
 *               Stefan Syberichs syberichs@techinfo.rwth-aachen.de
 * organization: LTI, RWTH Aachen
 * creation ...: 04.11.99
 * description.: definition of the Crc32 class. The Crc32 class
 *               is generated from a template.
 *               CRC Size: 32 Bits
 *               CRC Polynomial:  0x04C11DB7
 *               Reversed: true
 *               Initial CRC Register Value: FFFFFFFF
 *               Final CRC XOR Value: FFFFFFFF
 * revisions ..: $Id: ltipngcrc32.h,v 1.5 2003/02/17 07:16:13 alvarado Exp $
 */

#ifndef _LTI_CRC32_H
#define _LTI_CRC32_H

#include "ltiTypes.h"


namespace lti {
  namespace png {
    /**
     * Class Crc32
     */
    class Crc32 {
    public:
      Crc32 () ;
      Crc32 (const Crc32 &) ;
      ~Crc32 () {}
      Crc32 &operator=(const Crc32 &) ;

      uint32 value () const ;
      void reset () ;
      void update (const char *buffer, unsigned int length) ;

    private:
      /**
       * CRC Table Definition
       */
      struct CrcTable {
        enum { MAXBYTEVALUES = 256 } ;
        CrcTable () ;
        uint32 values [MAXBYTEVALUES] ;
      } ;


      static const CrcTable crc_table ;


      uint32 crc_register ;
    } ;

  }

} // namespace lti

#endif

