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
 * file .......: ltiSkeleton.h
 * authors ....: Daniel Ruijters
 * organization: LTI, RWTH Aachen
 * creation ...: 3.8.2000
 * revisions ..: $Id: ltiSkeleton.h,v 1.8 2004/05/03 12:18:39 alvarado Exp $
 */

#ifndef _LTI_SKELETON_H_
#define _LTI_SKELETON_H_

#include "ltiObject.h"
#include "ltiDilation.h"
#include "ltiErosion.h"
#include "ltiBinaryKernels.h"
#include "ltiTransform.h"

namespace lti {

  /**
   * Homotopy preserving Skeleton.
   *
   * This class implements the algorithm of Ji and Piper (IEEE
   * Transactions on Pattern Analysis and Maschine Intelligence,
   * vol. 14, no. 6, june 1992).
   *
   * The generated skeletons will keep the original homotopy of the
   * input channel.
   *
   * The input data must be segmented, with values 0 (background) and not 0
   * (object).
   *
   * @ingroup gMorphology
   */
  class skeleton : public transform {
  public:
    /**
     * the parameters for the class skeleton
     */
    class parameters : public transform::parameters {
    public:
      /**
       * default constructor
       */
      parameters();

      /**
       * copy constructor
       * @param other the parameters object to be copied
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      ~parameters();

      /**
       * returns name of this type
       */
      const char* getTypeName() const;

      /**
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& copy(const parameters& other);

      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * type of kernel
       */
      enum eKernelType {
        CityBlock,  /*!< city block kernel */
        ChessBoard, /*!< chessboard kernel */
        Euclidean   /*!< eucledian kernel */
      };

      /**
       * the type of the kernel.  (Default type: CityBlock)
       */
      eKernelType kernelType;

      /**
       * Value given to the skeleton pixels (Default: 255)
       */
      int formPointsValue;

      /**
       * Value given to the homotopy-preserving pixels (Default: 255)
       */
      int jPointsValue;
    };

    /**
     * default constructor
     */
    skeleton();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    skeleton(const skeleton& other);

    /**
     * destructor
     */
    virtual ~skeleton();

    /**
     * returns the name of this type ("skeleton")
     */
    virtual const char* getTypeName() const;

    //TODO: comment your apply methods!

    /**
     * operates on the given parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    channel& apply(channel& srcdest) const;

    /**
     * operates on the given parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.
     * @result a reference to the <code>srcdest</code>.
     */
    channel8& apply(channel8& srcdest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    channel& apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @result a reference to the <code>dest</code>.
     */
    channel8& apply(const channel8& src,channel8& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    skeleton& copy(const skeleton& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

  protected:

    /**
     * provides logical or of two channel8s
     */
    channel8& img_or(const channel8& src1, channel8& srcdest) const;

    /**
     * returns img \ stamp. (img with all pixels of stamp left out)
     */
    channel8& difference(const channel8& img,
                         const channel8& stamp,
                         channel8& dest) const;

    /**
     * calculate the jpoints for every pixel in src
     * jpoints are needed to preserve homotopy
     */
    channel8& calc_jpoints(const channel8& src,
                           const channel8& match,
                           channel8& dest) const;

    channel8& calc_hpoints(const channel8& src,
                           const channel8& match1,
                           const channel8& match2,
                           channel8& dest) const;

    /**
     * sets all non-zero values of a channel to int val
     */
    channel8& visualize(channel8& srcdest, const int& val) const;

  };
}

#endif

