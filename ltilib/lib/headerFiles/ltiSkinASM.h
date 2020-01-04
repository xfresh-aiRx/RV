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
 * file .......: ltiSkinASM.h
 * authors ....: Holger Fillbrandt, Benjamin Winkler
 * organization: LTI, RWTH Aachen
 * creation ...: 1.3.2002
 * revisions ..: $Id: ltiSkinASM.h,v 1.9 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_SKIN_A_S_M_H_
#define _LTI_SKIN_A_S_M_H_


#include "ltiGradientASM.h"

namespace lti {
  /**
   * This is an alignment strategy for activeShapeModels (ASM), that works on a gradient
   * channel and a skin probability channel, to trace skin colored objects.
   *
   * In addition to the feature selection in the class gradientASM, feature points have
   * to be skin colored in the inside region and non-skin colored in the outside region.
   * Points are valid, when they only contain skin points on the inside of the shape
   * (along the normal, within a distance <= 'parameters::searchExtent').
   * If no such points are found, search is continued further inside the shape (see
   * parameters::shrinkExtent).
   * When too many points are completely surrounded by skin, search for a skin border
   * is continued further outside the shape (see parameters::enlargeExtent).
   *
   * <b>Important note:</b> Due to the inside/outside nature, the points of the shape
   * must be given in clockwise order!
   */
  class skinASM : public gradientASM {
  public:
    /**
     * the parameters for the class skinASM
     */
    class parameters : public gradientASM::parameters {
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
       * copy the contents of a parameters object
       * @param other the parameters object to be copied
       * @return a reference to this parameters object
       */
      parameters& operator=(const parameters& other);


      /**
       * returns a pointer to a clone of the parameters
       */
      virtual functor::parameters* clone() const;

      /**
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * read the parameters from the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool read(ioHandler& handler,const bool complete=true);

#     ifdef _LTI_MSC_6
      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is also public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters
      // ------------------------------------------------


      /**
       * the outside radius in which skin borders are searched, if too many borderpoints are
       * inside a skin-area (must be higher than searchExtent, to produce an effect).
       *
       * default: 30
       */
      int enlargeExtent;

      /**
       * the inside radius in which skin borders are searched, if no skin is inside search extent
       * (must be higher than searchExtent, to produce an effect).
       *
       * default: 15
       */
      int shrinkExtent;

      /**
       * defines how many points of the border are allowed to lie inside skin regions
       * (valid: 0.0...1.0; meaning 0% to 100%)
       * if too many points are inside skin regions, the shape is enlarged.
       *
       * default: 0.7
       */
      float maxInsideSkin;

      /**
       * threshold to decide, if a pixel is skin or not (valid: 0.0...1.0)
       *
       * default: 0.5
       */
      float skinThreshold;

      /**
       * reliability: only skin borders considered to be reliable
       *
       * default: true
       */
      bool onlySkinBordersReliable;
    };

    /**
     * default constructor
     */
    skinASM();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    skinASM(const skinASM& other);

    /**
     * destructor
     */
    virtual ~skinASM();

    /**
     * returns the name of this type ("skinASM")
     */
    virtual const char* getTypeName() const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    skinASM& copy(const skinASM& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    skinASM& operator=(const skinASM& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;


    /**
     * align shape on the given gradient channel.
     * iteratively, scan for best border as described above
     */
    pointDistributionModel::shape& apply(pointDistributionModel::shape& srcdest,
                                         const channel &gradientChannel,
                                         const channel &skinProbChannel) const;

  protected:

    /**
     * iteratively align shape on the given gradient and skin probality channels.
     * @return a reference to the <code>srcdest</code>.
     */
    pointDistributionModel::shape& adjustShape(pointDistributionModel::shape &srcdest,
                                               const channel &gradientChannel,
                                               const channel &skinProbChannel,
                                               fvector &pointReliability) const;

  };
}

#endif

