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
 * file .......: ltiSerialLDA.cpp
 * authors ....: Pablo Alvarado, Jochen Wickel
 * organization: LTI, RWTH Aachen
 * creation ...: 06.02.03
 * revisions ..: $Id: ltiSerialLDA.cpp,v 1.5 2005/04/07 12:34:23 doerfler Exp $
 */

#ifdef _LTI_MSC_6
// a weird warning from MSVC++ is a false alarm!
#pragma warning(disable:4660)
#pragma warning(disable:4800) // warnings for matrix<bool> disabled
#pragma warning(disable:4804) // warnings for matrix<bool> disabled
#endif

#include "ltiSerialLDA.h"

#ifdef HAVE_LAPACK
#include "ltiSerialLDA_template.h"

namespace lti {
  // explicit instantiations

  template class serialLDA<double>;
  template class serialLDA<float>;
}

#endif
