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
 * file .......: ltiDrawFlowField.h
 * authors ....: Bernd Mussmann, Suat Akyol
 * organization: LTI, RWTH Aachen
 * creation ...: 16.8.2000
 * revisions ..: $Id: ltiDrawFlowField.h,v 1.20 2005/06/16 09:32:42 doerfler Exp $
 */

#ifndef _LTI_DRAW_FLOW_FIELD_H_
#define _LTI_DRAW_FLOW_FIELD_H_

// include files which are needed in this header!!
#include "ltiGaussKernels.h"
#include "ltiDownsampling.h"
#include "ltiDraw.h"
#include "ltiImage.h"
#include "ltiFilledUpsampling.h"

// include parent class:
#include "ltiFunctor.h"

namespace lti {
  /**
   * Draws the visualization of a two dimensional flowfield.
   * The channels of the x and y component are the input %parameters,
   * a picture of the flowfield is the output. The input channels must
   * have the same dimensions.
   */
  class drawFlowField : public functor {
  public:
    /**
     * the parameters for the class drawFlowField
     */
    class parameters : public functor::parameters {
    public:
      /**
       * Type for choosing overlayMode
       */
      enum eOverlayMode {
        NoOverlay,  /*!< draws flow field on black background */
        OnFlowfield,/*!< draws flow field on channel, which is
                         given for output                     */
        OnValue,    /*!< draws flow field on internally computed
                         value of itself                      */
        OnPhase     /*!< draws flow field on internally computed
                         phase of itself                      */
      };

      /**
       * Type for choosing vectorMode
       */
      enum eVectorMode {
        Arrows,  /*!< draws flow vectors as arrows*/
        Lines    /*!< draws flow vectors as lines */
      };

      /**
       * Type for choosing ScalingMode
       */
      enum eScalingMode {
        GridWidth,  /*!< longest vector will be scaled to gridWidth*/
        NoScaling   /*!< vectors will not be scaled in any way     */
      };

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
       * write the parameters in the given ioHandler
       * @param handler the ioHandler to be used
       * @param complete if true (the default) the enclosing begin/end will
       *        be also written, otherwise only the data block will be written.
       * @return true if write was successful
       */
      virtual bool write(ioHandler& handler,const bool complete=true) const;

      /**
       * write the parameters in the given ioHandler
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
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use read() instead!
       */
      bool readMS(ioHandler& handler,const bool complete=true);

      /**
       * this function is required by MSVC only, as a workaround for a
       * very awful bug, which exists since MSVC V.4.0, and still by
       * V.6.0 with all bugfixes (so called "service packs") remains
       * there...  This method is public due to another bug, so please
       * NEVER EVER call this method directly: use write() instead!
       */
      bool writeMS(ioHandler& handler,const bool complete=true) const;
#     endif

      // ------------------------------------------------
      // the parameters of functor
      // ------------------------------------------------

      /**
       * Tells, how much larger the output flow field channel should
       * be compared to the the input channel(s).
       * zoomfactor = 1,2,... (default=1)
       */
      int zoomfactor;

      /**
       * Input channels are sampled by this factor.
       * samplerate = 1,2,... (default=5)
       */
      int samplerate;

      /**
       * If value of current samplingpoint is smaller than the percentage
       * (given by threshold) of maximum value, flow line will not be drawn.
       * threshold = 0.0 ... 1.0 (default=0.1)
       */
      float threshold;

      /**
       * overlayMode can be one of: noOverlay (default), onFlowfield, onValue,
       * onPhase.
       *
       * noOverlay = draws flow field on black background
       *
       * onFlowfield = draws flow field on channel, which is handed over
       *               for output
       *
       * onValue/onPhase = draws flow field on internally computed value/phase
       */
      eOverlayMode overlayMode;

      /**
       * vectorMode can be one of: arrows(default), lines.
       *
       * arrows = draws flow vectors as lines
       *
       * lines = draws flow vectors as arrows
       *
       */
      eVectorMode vectorMode;

      /**
       * scalingMode can be one of: NoScaling, GridWidth(default).
       *
       * NoScaling = vectors will not be scaled
       *
       * GridWidth = vectors will be scaled, so that the longest one
       * becomes as long as grid width(=samplerate*zoomfactor)
       */
      eScalingMode scalingMode;

      /**
       * Color of flow line.
       * linecolor = 0.0 ... 1.0 (default=0.3)
       */
      float linecolor;

      /**
       * Color of flow line tip.
       * tipcolor = 0.0 ... 1.0 (default=1.0)
       */
      float tipcolor;
    };


    /**
     * default constructor
     */
    drawFlowField();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    drawFlowField(const drawFlowField& other);

    /**
     * destructor
     */
    virtual ~drawFlowField();

    /**
     * returns the name of this type ("drawFlowField")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameters.
     * @param x_component channel must be the flow component in x-direction.
     * @param y_component channel must be the flow component in y-direction.
     * @return true if successful, false otherwise
     */
    bool apply(channel& x_component,channel& y_component) const;

    /**
     * operates on a copy of the given %parameters.
     * @param x_component channel must be the flow component in x-direction.
     * @param y_component channel must be the flow component in y-direction.
     * @param flowfield contains the resulting visualization.
     * @return true if successful, false otherwise
     */
    bool apply(const channel& x_component,
               const channel& y_component,
                     channel& flowfield) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    drawFlowField& copy(const drawFlowField& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;
  };
}

#endif

