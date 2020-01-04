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
 * file .......: ltiPointSetNormalizationType.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 18.4.2004
 * revisions ..: $Id: ltiPointSetNormalizationType.h,v 1.1 2004/05/19 17:59:26 goenner Exp $
 */

#ifndef _LTI_POINT_SET_ESTIMATOR_TYPE_H_
#define _LTI_POINT_SET_ESTIMATOR_TYPE_H_

#include "ltiIoHandler.h"

/**
 * \file ltiPointSetNormalizationType.h Defines the enumeration type
 *       ePointSetNormalizationType used in many places to specify which
 *        kind of pointSet should be employed.
 */

namespace lti {

  /**
   * PointSet Normalization Type.
   *
   * This type is used to indicate how point sets shall be normalized or if
   * they are normalized at all.
   */
  enum ePointSetNormalizationType {
    IsotropicNormalization, /**<pointSetNormalization in an isotropic manner */
    NoPointSetNormalization /**<no pointSetNormalization specified */
  };
  
  /**
   * read function for ePointSetNormalizationType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,ePointSetNormalizationType& data);

  /**
   * write function for ePointSetNormalizationType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const ePointSetNormalizationType& data);

  
}
#endif
