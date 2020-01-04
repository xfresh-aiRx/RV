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
 * file .......: ltiTemporalTemplate.h
 * authors ....: Pablo Alvarado
 * organization: LTI, RWTH Aachen
 * creation ...: 4.12.2000
 * revisions ..: $Id: ltiTemporalTemplate.h,v 1.13 2005/06/16 12:35:44 doerfler Exp $
 */

#ifndef _LTI_TEMPORAL_TEMPLATE_H_
#define _LTI_TEMPORAL_TEMPLATE_H_

#include "ltiObject.h"
#include "ltiImage.h"
#include "ltiTransform.h"

namespace lti {

  /**
   * This functor implements temporal templates, as described by James
   * Davis and Aaron Bobick in "The Representation and Recognition of
   * Action Using Temporal Templates", MIT Media Lab, Tech. Report 402,
   * IEEE Conference on Computer Vision and Pattern Recognition (CVPR'97)
   * The apply()-methods return the calculated Motion-History-Image coded in
   * a channel, where the value 1.0 means there was motion between the last
   * two images.
   */
  class temporalTemplate : public transform {
  public:
    /**
     * the parameters for the class temporalTemplate
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
       * tau specify the number of images in the past to be considered for the
       * temporal template.  Default value: 5
       */
      int tau;

      /**
       * The difference between the last two images will consider a pixel value
       * change as relevant only if the difference is greater as this value, or
       * if useAverage is true, as this value multiplied by the average.
       * Default value: 0.1
       */
      float threshold;

      /**
       * if useAverage is true, the average of the difference will be
       * considered to adapt the threshold, otherwise, the threshold
       * will be considered as it is.
       * Default value: false
       */
      bool useAverage;
    };

    /**
     * default constructor
     */
    temporalTemplate();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    temporalTemplate(const temporalTemplate& other);

    /**
     * destructor
     */
    virtual ~temporalTemplate();

    /**
     * returns the name of this type ( "temporalTemplate" )
     */
    virtual const char* getTypeName() const;

    /**
     * reset the internal accumulators, so that the next apply will be
     * considered as the first image to detect motion
     */
    void reset();

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.
     * @return true if successful, false otherwise
     * Please note that all considered images MUST have the same size,
     * otherwise your previous information will be lost.  The first
     * image will determine the used image size.  You can restart with
     * another size using reset().
     */
    bool apply(channel& srcdest);

    /**
     * operates on a copy of the given %parameters.
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if successful, false otherwise
     * Please note that all considered images MUST have the same size,
     * otherwise your previous information will be lost.  The first
     * images will determine the used image size.  You can restart with
     * another size using reset().
     */
    bool apply(const image& src,channel& dest);

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.
     * @return true if successful, false otherwise
     * Please note that all considered images MUST have the same size,
     * otherwise your previous information will be lost.  The first
     * images will determine the used image size.  You can restart with
     * another size using reset().
     */
    bool apply(const channel& src,channel& dest);

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if successful, false otherwise
     * Please note that all considered images MUST have the same size,
     * otherwise your previous information will be lost.  The first
     * images will determine the used image size.  You can restart with
     * another size using reset().
     */
    bool apply(const channel8& src,channel& dest);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    temporalTemplate& copy(const temporalTemplate& other);

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
     * calculates the difference image.  The given image will substitute
     * the lastChannel, and at the end will be empty()
     * returns the threshold to be used.
     */
    float difference(channel& newChannel, channel& diff);

    /**
     * calculates the difference image.  The given image will substitute
     * the lastChannel, and at the end will be empty()
     * returns the threshold to be used.
     */
    float difference(channel8& newChannel, channel& diff);

    /**
     * calculates the difference image.  The given image will substitute
     * the lastChannel, and at the end will be empty()
     * returns the threshold to be used.
     */
    float difference(image& newImage, channel& diff);

    /**
     * accumulate the given difference image in the MHI.  Diff will
     * contain the same value as mhi after calling this member.
     */
    void accumulate(channel& diff,const float& threshold);

    /**
     * the last channel, required to calculate D(x,y,t)
     */
    channel lastChannel;

    /**
     * the last channel8, required to calculate D(x,y,t)
     */
    channel8 lastChannel8;

    /**
     * the last image, required to calculate D(x,y,t)
     */
    image lastImage;

    /**
     * The Motion History Image
     */
    channel mhi;
  };
}

#endif

