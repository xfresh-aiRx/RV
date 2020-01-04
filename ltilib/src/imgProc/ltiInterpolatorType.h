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
 * file .......: ltiInterpolatorType.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 21.11.2003
 * revisions ..: $Id: ltiInterpolatorType.h,v 1.1 2003/11/24 06:48:48 alvarado Exp $
 */

#ifndef _LTI_INTERPOLATOR_TYPE_H_
#define _LTI_INTERPOLATOR_TYPE_H_

#include "ltiIoHandler.h"

/**
 * \file ltiInterpolatorType.h Defines the enumeration type eInterpolatorType
 *       used in many places to specify which kind of interpolation should be
 *       employed.
 */

namespace lti {

  /**
   * Interpolation Type.
   *
   * This type is used to indicate how sub-pixel position should be computed.
   */
  enum eInterpolatorType {
    NearestNeighborInterpolator, /**< Nearest Neighbor Interpolator */
    BilinearInterpolator,        /**< Bilinear Interpolator */
    BiquadraticInterpolator,     /**< Biquadratic Taps Interpolator */
    BicubicInterpolator          /**< Bicubic Interpolator */
  };
  
  /**
   * read function for eInterpolatorType.
   *
   * @ingroup gStorable
   */
  bool read(ioHandler& handler,eInterpolatorType& data);

  /**
   * write function for eInterpolatorType.
   *
   * @ingroup gStorable
   */
  bool write(ioHandler& handler,const eInterpolatorType& data);

  
}
#endif
