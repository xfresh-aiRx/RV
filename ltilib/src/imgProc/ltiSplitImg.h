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
 * file .......: ltiSplitImg.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImg.h,v 1.25 2005/06/27 14:36:53 canzler Exp $
 */


/**
 * \file ltiSplitImg.h Deprecated.  Use ltiSplitImageTo???.h instead
 */

#ifdef _LTI_MSC_VER
#pragma message("ltiSplitImg.h deprecated, use ltiSplitImageTo???.h instead!")
#else
#warning "ltiSplitImg.h deprecated, use ltiSplitImageTo???.h instead!"
#endif

#ifndef _LTI_SPLIT_IMG
#define _LTI_SPLIT_IMG
#include "ltiSplitImageToCIELuv.h"
#include "ltiSplitImageToHSI.h"
#include "ltiSplitImageToHLS.h"
#include "ltiSplitImageToHSV.h"
#include "ltiSplitImageToOCP.h"
#include "ltiSplitImageToRGB.h"
#include "ltiSplitImageTorgI.h"
#include "ltiSplitImageToxyY.h"
#include "ltiSplitImageToXYZ.h"
#include "ltiSplitImageToYIQ.h"
#include "ltiSplitImageToGSC.h"
#include "ltiSplitImageToGSL.h"
#endif
