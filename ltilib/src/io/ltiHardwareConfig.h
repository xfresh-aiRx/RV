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
 * file .......: ltiHardwareConfig.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 24.11.99
 */

#ifndef _LTI_HARDWARE_CONFIG_H_
#define _LTI_HARDWARE_CONFIG_H_

/**
 * @file ltiHardwareConfig.h Contains definition macros to compile hardware
 * related functors.
 *
 * This file contains definition macros used as flags to compile hardware
 * related functors like frame grabbers.  Usually these functors check if
 * the corresponding symbol is defined, and only in that cases the class
 * will be compiled.
 *
 * If you work with Linux, GCC and GNU-Make, you can optionally leave this file
 * untouched and provide in the corresponding symbol using a -D directive in
 * the file extrapaths.mk.
 * 
 * For example, to compile the ToUCam camera, your extrapaths.mk file would
 * look:
 * \code
 * EXTRACXXFLAGS=-D_USE_PHILIPS_TOUCAM
 * EXTRAINCLUDEPATH=
 * EXTRALIBPATH=
 * EXTRALIBS=
 * \endcode
 */

// use QuickCam
// #define _USE_QUICKCAM_GRABBER 1

// use Imaging Frame Grabber
// #define _USE_ITI_FRAME_GRABBER 1

// use Matrix Vision Frame Grabber
// #define _USE_MATRIX_VISION_GRABBER 1

// use SiliconSoftware Micro Enable with Channel-Link Frame Grabber
// #define _USE_MICRO_ENABLE_FRAME_GRABBER 1

// use Philips ToUcam Pro
// #define _USE_PHILIPS_TOUCAM 1

// use leutron frame grabber
// #define _USE_LEUTRON_GRABBER

#endif  // _LTI_HARDWARE_CONFIG_H_

