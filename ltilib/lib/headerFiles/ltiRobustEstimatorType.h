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
 * file .......: ltirobustEstimatorType.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 18.4.2004
 * revisions ..: $Id: ltiRobustEstimatorType.h,v 1.1 2004/05/19 17:59:26 goenner Exp $
 */

#ifndef _LTI_ROBUST_ESTIMATOR_TYPE_H_
#define _LTI_ROBUST_ESTIMATOR_TYPE_H_

#include "ltiIoHandler.h"

/**
 * \file ltiRobustEstimatorType.h Defines the enumeration type
 *       eRobustEstimatorType used in many places to specify which
 *       kind of robust estimator should be employed.
 */

namespace lti {

  /**
   * Robust Estimator Type.
   *
   * 
   */
  enum eRobustEstimatorType {
    RansacEstimator, /**<robustEstimator with RANSAC */
    NoRobustEstimator /**<no robustEstimator specified */
  };
  
  /**
   * read function for eRobustEstimatorType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eRobustEstimatorType& data);

  /**
   * write function for eRobustEstimatorType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eRobustEstimatorType& data);

  
}
#endif
