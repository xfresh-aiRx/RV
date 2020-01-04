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
 * file .......: ltiMergeOCPToImage.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiMergeOCPToImage.h,v 1.1 2003/11/14 16:07:21 alvarado Exp $
 */

#ifndef _LTI_MERGE_OCP_TO_IMAGE_H_
#define _LTI_MERGE_OCP_TO_IMAGE_H_

#include "ltiMergeImage.h"

namespace lti {

  /**
   * Merge linear opponent color channels OCP
   *
   * The three opponent channels are defined here as follows:
   * 
   * \f[ \begin{pmatrix} c1\\c2\\c3 \end{pmatrix} =
   *     \begin{pmatrix} 0.5  & -0.5  & 0.0 \\
   *                    -0.25 & -0.25 & 0.5 \\
   *                     0.33 &  0.33 & 0.33\end{pmatrix} 
   *     \begin{pmatrix} R\\G\\B \end{pmatrix} 
   * \f]
   *
   * @ingroup gColor
   */
  class mergeOCPToImage : public mergeImage {

  public:
    /**
     * returns the name of this type
     */
    virtual const char* getTypeName() const;

    /**
     * returns a poinbter to a clone of the functor
     */
    virtual functor* clone() const;

    /**
     * on-copy operator for 32-bit "floating-point" channels
     */
    virtual bool apply(const matrix<float>& c1,
                       const matrix<float>& c2,
                       const matrix<float>& c3,
                       image& img) const;
    /**
     * on-copy operator for 8-bit channels.
     * expects positive values !
     * @see OCPsplit operation
     */
    virtual bool apply(const channel8& c1,
                       const channel8& c2,
                       const channel8& c3,
                       image& img) const;

    /**
     * on-copy operator for 32-bit "floating-point" values
     */
    virtual bool apply(const float& c1,
                       const float& c2,
                       const float& c3,
                       rgbPixel& pixel) const;
    /**
     * on-copy operator for 8-bit values.
     * expects positive values !
     * @see OCPsplit operation
     */
    virtual bool apply(const ubyte& c1,
                       const ubyte& c2,
                       const ubyte& c3,
                       rgbPixel& pixel) const;
  };
}

#endif

