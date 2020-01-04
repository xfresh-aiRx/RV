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
 * file .......: ltiSplitImageToOCP.h
 * authors ....: Pablo Alvarado, Stefan Syberichs, Thomas Rusert
 * organization: LTI, RWTH Aachen
 * creation ...: 19.04.99
 * revisions ..: $Id: ltiSplitImageToOCP.h,v 1.2 2004/01/11 10:36:26 alvarado Exp $
 */

#ifndef _LTI_SPLIT_IMAGE_TO_OCP_H_
#define _LTI_SPLIT_IMAGE_TO_OCP_H_

#include "ltiSplitImage.h"

namespace lti {

  /**
   * Split image in an oponent colour system
   *
   * The three opponent channels are defined here as follows:
   * 
   * \f[ \begin{pmatrix}c1\\c2\\c3\end{pmatrix} =
   *     \begin{pmatrix} 0.5  & -0.5  & 0.0 \\
   *                    -0.25 & -0.25 & 0.5 \\
   *                     0.33 &  0.33 & 0.33\end{pmatrix} 
   *     \begin{pmatrix}R\\G\\B\end{pmatrix} 
   * \f]
   *
   *
   * @ingroup gColor
   */
  class splitImageToOCP : public splitImage {
  public:

    /**
     * returns the name of this type
     */
    virtual const char* getTypename() const;

    /**
     * returns a pointer to a clone of the functor
     */
    virtual functor* clone() const;

    /**
     * Get the three opponent color channels.
     * @param img input image to be split.
     * @param c1 Red - Green channel
     * @param c2 Blue - Yellow channel
     * @param c3 Intensity channel
     */
    virtual bool apply(const image& img,
                       channel& c1,
                       channel& c2,
                       channel& c3) const;
    /**
     * Get the three opponent color channels.
     * @param img input image to be split.
     * @param c1 Red - Green channel
     * @param c2 Blue - Yellow channel
     * @param c3 Intensity channel
     *
     * The original value range for the output channels should be
     * between -127.5 and 127.5.  The ubyte type cannot represent this
     * range, so the first two channels are scaled and shifted
     * by 127.5 to reach the desired output range from 0 to 255
     */
    virtual bool apply(const image& img,
                       channel8& c1,
                       channel8& c2,
                       channel8& c3) const;

    /**
     * on-copy
     */
    virtual bool apply(const rgbPixel& pixel,
                       float& c1,
                       float& c2,
                       float& c3) const;
    /**
     * on-copy
     */
    virtual bool apply(const rgbPixel& pixel,
                       ubyte& c1,
                       ubyte& c2,
                       ubyte& c3) const;

  };

} // namespace lti
#endif

