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
 * file .......: ltiBackgroundModel.h
 * authors ....: Ruediger Weiler
 * organization: LTI, RWTH Aachen
 * creation ...: 19.1.2019
 * revisions ..: $Id: ltiBackgroundModel.h,v 1.13 2005/06/16 09:32:41 doerfler Exp $
 */

#ifndef _LTI_BACKGROUND_MODEL_H_
#define _LTI_BACKGROUND_MODEL_H_

//TODO: include only those files which are needed in this header!!
#include "ltiImage.h"
#include "ltiSerialVectorStats.h"
#include <vector>

// TODO: Check this include to parent class:
#include "ltiSegmentation.h"

namespace lti {
  //TODO: comment your class
  /**
   * Implementation of an part of the paper 'Pfinder: Real-Time Tracking of the Human Body'
   * from Wren, Azarbayejani, Darrell and Pentland published in
   * IEEE PAMI July 1997, vol 19, no 7, pp. 780-785.
   * This functor can separate things in front of a background.
   * The background model is generate of same images displaying only the
   * background. With this model you can than separate objects witch belong
   * not to the background. The model is a statistic representation of
   * every pixel in the scene. And the foreground objects are alle
   * colored faces witch do not fit to the statistic model. The rule
   * wether a pixel fit ore not is the malanowis distance, a distance between
   * a vector(r,g,b) to the mean vector. Furthermore you can adapt the model
   * with the images displaying back and foreground. The claas adapt, if you
   * want, all pixels with the malanowis smaler as a threshold. The normal
   * way of operation is: addd with the 'addBackgrund' function same images,
   * with only background, to the model, than use the apply function with
   * an image with fore and background and you get in the channel8 the
   * malanowis of each pixel to the background. You can set the outputTreshold
   * parameter for displying only foreground objects (try 500). If you set the
   * flag binaryOutput you get the background black an the foreground white.
   */
  class backgroundModel : public segmentation {
  public:
    /**
     * the parameters for the class backgroundModel
     */
    class parameters : public segmentation::parameters {
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
       * The value of each pixel in the distance output channel
       * must be grater than outputThreshold to be drawn. Every other
       * pixels are set to zero.
       */
      float outputThreshold;

      /**
       * If this is set true every value bigger than outputThreshold is set to
       * one.  every other value is set to zero. You get a kind of a mask as
       * output.
       */
      bool binaryOutput;

      /**
       * Al pixels with a malanowis distance smaler than 'adaptationThreshold'
       * are adept. The other pixel are not touched because they do not belong
       * to the background.
       */
      float adaptationThreshold;

      /**
       * If this is set true the backgroundModel is updatet on every apply.
       * Only the regions with a diatance smaler than adaptationThreshold
       * become adapt.
       */
      bool adaptModel;

      /**
       * The weight of the new images added adaptModelive to the background.
       * Range 0 to 1. If 'alpha' is set to 0.5 the saved model and the new
       * image witch become adapt are wighted 50%. If 'alpha' is set to 0 the
       * new image is ignored.
       *
       * If 'alpha' is set to 1 the new model constist only of the new image.
       */
      float alpha;


    };

    /**
     * default constructor
     */
    backgroundModel();

    /**
     * copy constructor
     * @param other the object to be copied
     */
    backgroundModel(const backgroundModel& other);

    /**
     * destructor
     */
    virtual ~backgroundModel();

    /**
     * returns the name of this type ("backgroundModel")
     */
    virtual const char* getTypeName() const;

    /**
     * The function calculates the malanowis distance of each pixel in the image
     * to the same pixel in the model. The outpout channel is filled with the rules
     * in the parameters. If adaptModel is set true the backgroundmoldel is updated
     * with the background of the image 'src'. Every pixel with a distance smaler than
     * 'adaptaionThreshold' is background of the image.
     * @param src image with the source data.
     * @param dest channel with the distance of each pixel
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src, channel& dest);

    /**
     * Implements first the apply with channel and cast than the result into
     * a channel8.
     * @param src image with the source data.
     * @param dest channel with the distance of each pixel
     * @return true if apply successful or false otherwise.
     */
    bool apply(const image& src, channel8& dest);


    /**
     * Adds a image to the backgroud model. You must call this function serveral times
     * with images displaing the same scene with no foreground objects. The functor creates
     * the initial backgroundmodel based on thus images. It is absolutly reqired that
     * all the images disply the same scene.
     *
     */
    bool addBackground(const image& src);

    /**
     * With this fuction you can adapt the model by hand. You can define a mask with the
     * values 0 and not 0. The image and the mask must have the same size. The model is
     * adept with the image at every point where the mask is equal to zero.
     */
    bool adaptBackground(const image& src, const channel8& mask);

    /**
     * This function returns an image of the background model.
     * The image consist of the mean color values of each pixel.
     */
    bool getModel(image& model);

    /**
     * This function emptys the model saved in the class.
     * After calling this funktion you have to add backgroundimages
     * before you can call the apply function
     */
    bool clearMoldel();

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    backgroundModel& copy(const backgroundModel& other);

    /**
     * alias for copy member
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    backgroundModel& operator=(const backgroundModel& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * Update functor's parameters
     *
     * Initialize internal data according to the parameters
     */
    virtual bool updateParameters();

    /**
     * write the functor in the given ioHandler. The default implementation
     * is to write just the parameters object.
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool write(ioHandler& handler,
                       const bool complete=true) const;

    /**
     * read the parameters from the given ioHandler. The default implementation
     * is to read just the parameters object.
     * @param handler the ioHandler to be used
     * @param complete if true (the default) the enclosing begin/end will
     *        be also written, otherwise only the data block will be written.
     * @return true if write was successful
     */
    virtual bool read(ioHandler& handler,const bool complete=true);
  protected:

    /**
     * For each point of the image a statistic functor. This functor creates
     * the variance and the mean value of alle pixels in the  images added to the model.
     */
    std::vector< std::vector< serialVectorStats<float> > > bgStat;

  };
}

#endif

