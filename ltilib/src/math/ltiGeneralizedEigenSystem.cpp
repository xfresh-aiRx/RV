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
 * file .......: ltiGeneralizedEigenSystem.cpp
 * authors ....: Peter Doerfler
 * organization: LTI, RWTH Aachen
 * creation ...: 11.7..2003
 * revisions ..: $Id: ltiGeneralizedEigenSystem.cpp,v 1.3 2003/10/30 13:01:58 sltilib Exp $
 */

#include "ltiMacroSymbols.h"
#ifdef _LTI_MSC_6
// a weird warning from MSVC++ is a false alarm!
#pragma warning(disable:4660)
// allow explicit instantiation of abstract class
#pragma warning(disable:4661)
#endif

#include "ltiGeneralizedEigenSystem.h"
#include "ltiGeneralizedEigenSystem_template.h"

namespace lti {
  template class generalizedEigenSystem<float>;
  template class generalizedEigenSystem<double>;

} // namespace lti
