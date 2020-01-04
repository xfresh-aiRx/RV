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
 * file .......: ltiColorNormalization.h
 * authors ....: Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 8.11.2000
 * revisions ..: $Id: ltiColorNormalization.h,v 1.15 2005/06/27 14:49:07 canzler Exp $
 */


/**
 * \file ltiColorNormalization.h deprecated.  Use
 * ltiGrayWorldNormalization.h instead
 */

#ifdef _LTI_MSC_VER
#pragma message("ltiColorNormalization.h deprecated, use ltiGrayWorldNormalization.h instead!")
#else
#warning "ltiColorNormalization.h deprecated, use ltiGrayWorldNormalization.h instead!"
#endif

#ifndef _LTI_COLOR_NORMALIZATION_H_
#define _LTI_COLOR_NORMALIZATION_H_

#include "ltiGrayWorldNormalization.h"

namespace lti {
  typedef grayWorldNormalization colorNormalization;
}

#endif

