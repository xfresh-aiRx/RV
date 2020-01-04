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


/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiMacroSymbols.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 01.04.99
 * revisions ..: $Id: ltiMacroSymbols.h,v 1.8 2005/11/24 14:24:03 doerfler Exp $
 */

#ifndef _LTI_MACRO_SYMBOLS_H_
#define _LTI_MACRO_SYMBOLS_H_

/**
 * \file ltiMacroSymbols.h contains several definitions of macros to
 *       determine which compiler is being used.  Since this file is
 *       included by ltiObject.h, their contents should be defined in every
 *       LTI-Lib file.
 */

/**
 * Macro to recognize if the Windows 32-bit library is available
 */
#if defined(_WIN32) 
#define _LTI_WIN32 _WIN32
#elif defined(WIN32) 
#define _LTI_WIN32 WIN32
#endif

/**
 * Macro to recognize if the MS Visual C++ compiler 6.0 or higher
 * (including the .NET family) is being used.
 */
#if defined(_MSC_VER) 
#define _LTI_MSC_VER _MSC_VER
#elif defined(MSC_VER)
#define _LTI_MSC_VER MSC_VER
#endif

/**
 * Macro to recognize if the MS Visual C++ compiler .NET 2003 is being used.
 */
#if defined(_LTI_MSC_VER) && (_LTI_MSC_VER >= 1310)
#define _LTI_MSC_DOT_NET _LTI_MSC_VER
#endif

/**
 * Macro to recognize if a previous version of MS Visual C++ compiler
 * .NET 2003 is being used.
 */
#if defined(_LTI_MSC_VER) && (_LTI_MSC_VER < 1310)
#define _LTI_MSC_6 _LTI_MSC_VER
#endif

/**
 * Macro to recognize if a GNU C++ compiler is being used
 */
#if defined(__GNUC__) 
#define _LTI_GNUC __GNUC__
#endif

/**
 * Macro to recognize if a GNU C++ compiler 2.95 is being used
 */
#if defined(_LTI_GNUC) && (_LTI_GNUC <= 2)
#define _LTI_GNUC_2 _LTI_GNUC
#endif

/**
 * Macro to recognize if a GNU C++ compiler 3.0 or greater is being used
 */
#if defined(_LTI_GNUC) && (_LTI_GNUC >= 3)
#define _LTI_GNUC_3 _LTI_GNUC
#endif

/**
 * Macro to recognize if a GNU C++ compiler 3.4.0 or greater is being used
 */
#if defined(_LTI_GNUC) && (_LTI_GNUC >= 3) && (__GNUC_MINOR__ >= 4)
#define _LTI_GNUC_3_4 _LTI_GNUC
#endif

/**
 * Macro to recognize if a GNU C++ compiler 4.0.0 or greater is being used
 */
#if defined(_LTI_GNUC) && (_LTI_GNUC >= 4)
#define _LTI_GNUC_4 _LTI_GNUC
#endif

#if defined(__INTEL_COMPILER)
/**
 * \def _LTI_ICC 
 *
 * Macro to recognize if an Intel compiler is being used. Note that
 * _LTI_GNUC and some _LTI_GNUC_x are also defined in this case
 * (unless switched off manually). This is quite useful in most cases
 * since gcc and icc behave mostly equivalently.
 */
#define _LTI_ICC __INTEL_COMPILER
#endif

/**
 * Macro to recognize if a Linux system is being used
 */ 
#if defined(__linux__) 
#define _LTI_LINUX
#endif

/**
 * Macro to recognize if a CygWin system is being used
 */
#if defined(__CYGWIN__) || defined(__CYGWIN32) || defined(_MINGW32__)
#define _LTI_CYGWIN
#endif

/**
 * Macro to recognize if an Apple system is used
 */
#if defined(__APPLE__)
#define _LTI_MACOSX
#endif


#endif
