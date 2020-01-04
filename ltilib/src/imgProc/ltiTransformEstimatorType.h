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
 * file .......: ltiTransformEstimatorType.h
 * authors ....: Claudia Goenner
 * organization: LTI, RWTH Aachen
 * creation ...: 18.4.2004
 * revisions ..: $Id: ltiTransformEstimatorType.h,v 1.1 2004/04/30 18:00:23 goenner Exp $
 */

#ifndef _LTI_TRANSFORM_ESTIMATOR_TYPE_H_
#define _LTI_TRANSFORM_ESTIMATOR_TYPE_H_

#include "ltiIoHandler.h"

/**
 * \file ltiTransformEstimatorType.h Defines the enumeration type
 *       eTransformEstimatorType used in many places to specify which
 *        kind of transform should be employed.
 */

namespace lti {

  /**
   * Transform Estimator Type.
   *
   * This type is used to indicate which transform shall be estimated. 
   */
  enum eTransformEstimatorType {
    Homography8DofEstimator, /**<transformEstimator for homography with 8 DOF */
    Homography9DofEstimator, /**<transformEstimator for homography with 9 DOF */
    AffineEstimator, /**<transformEstimator for affine transformations */
    SimilarityEstimator, /**<transformEstimator for similarity transformations */
    NoTransformEstimator /**< no transformEstimator specified */
  };
  
  /**
   * read function for eTransformEstimatorType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eTransformEstimatorType& data);

  /**
   * write function for eTransformEstimatorType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eTransformEstimatorType& data);

  
}
#endif
