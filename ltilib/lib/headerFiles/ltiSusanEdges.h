/*
 * Copyright (C) 1998, 1999, 2000, 2001
 * Lehrstuhl fuer Technische Informatik, RWTH-Aachen, Germany
 *
 * This file can be used with the LTI-Computer Vision Library (LTI-Lib)
 * (please note this file is NOT part of the LTI-Lib)
 * This code is based on the original C code by S. Smith, which contains
 * following header:
 *
 **********************************************************************
 *
 * SUSAN Version 2l by Stephen Smith
 *
 * Oxford Centre for Functional Magnetic Resonance Imaging of the
 * Brain, Department of Clinical Neurology, Oxford University, Oxford,
 * UK (Previously in Computer Vision and Image Processing Group - now
 * Computer Vision and Electro Optics Group - DERA Chertsey, UK)
 *
 * Email:    steve@fmrib.ox.ac.uk
 * WWW:      http://www.fmrib.ox.ac.uk/~steve
 *
 * (C) Crown Copyright (1995-1999), Defence Evaluation and Research Agency,
 * Farnborough, Hampshire, GU14 6TD, UK
 * DERA WWW site:
 * http://www.dera.gov.uk/
 * DERA Computer Vision and Electro Optics Group WWW site:
 * http://www.dera.gov.uk/imageprocessing/dera/group_home.html
 * DERA Computer Vision and Electro Optics Group point of contact:
 * Dr. John Savage, jtsavage@dera.gov.uk, +44 1344 633203
 *
 * A UK patent has been granted: "Method for digitally processing
 * images to determine the position of edges and/or corners therein for
 * guidance of unmanned vehicle", UK Patent 2272285. Proprietor:
 * Secretary of State for Defence, UK. 15 January 1997
 *
 * This code is issued for research purposes only and remains the
 * property of the UK Secretary of State for Defence. This code must
 * not be passed on without this header information being kept
 * intact. This code must not be sold.
 **********************************************************************
 *
 *
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
 *
 */

/*----------------------------------------------------------------
 * project ....: LTI Digital Image/Signal Processing Library
 * file .......: ltiSusanEdges.h
 * authors ....: Stefan Syberichs
 * organization: LTI, RWTH Aachen
 * creation ...: 30.03.2000
 * revisions ..: $Id: ltiSusanEdges.h,v 1.22 2005/07/22 08:19:05 libuda Exp $
 */

#include "ltiConfig.h"

#ifndef HAVE_SUSAN
#ifdef _LTI_MSC_VER
#pragma message("Configuration Error:")
#pragma message("Change in src/basics/ltiWinConfig.h HAVE_SUSAN to 1")
#else
#warning "Configuration Error:"
#warning "Change in src/basics/config.h HAVE_SUSAN to 1"
#endif
#endif

#ifndef _LTI_SUSANEDGES_H_
#define _LTI_SUSANEDGES_H_

#include "ltiEdgeDetector.h"

namespace lti {

  /**
   * The Susan Edge Detector.  susanEdges is a modifier-frontend for
   * the susan-class for edge detection.  The susan-principle is a
   * fast method for finding <b> binary edges </b> in images and
   * channels. The result is a black-and-white map (image or channel
   * format), where white pixels (255) mark an edge and black pixels
   * (0) are background area.  in General, the sensitivity es
   * controlled by the parameter <code> threshold</code>, which is an
   * unsigned integer between 1 and 255, where small thresholds
   * produce many edges and high thresholds detect only rough
   * edges. The default is 20.
   *
   * Example:
   * \code
   *
   *  lti::image theimage;
   *  lti::channel8 RED, GREEN, BLUE;
   *
   *  // to store the results:
   *  lti::image edgeImage;
   *  lti::chanel8 edgeChannel;
   *
   *  // ...
   *  // load image, split into channel8's
   *
   *  lti::susanEdges detect;
   *
   *  lti::susanEdges::parameters detectparams;
   *
   *  // default parameters are: RGBmerge, threshold = 20...
   *  detectparams.edgeMerge = lti::susanEdges::parameters::RGBseparate;
   *  detectparams.threshold = 15;
   *  detect.setParameters(detectparams);
   *
   *  // choose one of these:
   *
   *  detect.apply(theimage, edgeImage);    // on copy for images
   *  detect.apply(theimage);               // on place for image
   *  detect.apply(RED, edgeChannel);       // on copy for channel8's
   *  detect.apply(RED);                    // on place for channel8
   *  detect.apply(theimage, edgechannel);  // on copy for input image and
   *                                           output channel8
   *  \endcode
   */
  class susanEdges : public edgeDetector {
  public:
    /** Parameter class for susanEdges. */
    class parameters : public edgeDetector::parameters {

    public:
      /**
       * constructor
       */
      parameters();

      /**
       * copy constrcutor
       */
      parameters(const parameters& other);

      /**
       * destructor
       */
      virtual ~parameters();

      /**
       * returns the name of the parameter object
       */
      virtual const char* getTypeName() const;

      /**
       * copy member
       */
      parameters& copy(const parameters& other);

      /**
       * clone member
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
      // the parameters
      // ------------------------------------------------

      /**
       * the brightness threshold. controls the sensitivity of edge
       * detection.  threshold is an integer value between 1 and 255,
       * where 1 finds many edges and 255 almost none.
       *
       * The default is 20.
       */
      int threshold;

    };

    /**
     * constructor
     */
    susanEdges();

    /**
     * constructor with parameters
     */
    susanEdges(const parameters& par);


    /**
     * copy constructor
     */
    susanEdges(const susanEdges& other);

    /**
     * destructor
     */
    virtual ~susanEdges();

    /**
     * returns the type of the object
     */
    virtual const char* getTypeName() const;

    /**
     * clone member
     */
    virtual functor* clone() const;

    /**
     *  set the susanEdges::parameters.
     */
    virtual bool updateParameters();

    /**
     * returns current parameters.
     */
    const parameters& getParameters() const;

    /**
     * OnPlace version of apply for channels (lti::channel8).
     *
     * @param srcdest channel with the source data.  The result
     *                will be left here too.
     * @return true if ok, false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * OnPlace version of apply for lti::channel8 data
     * @param srcdest channel8  with the source data.
     *                The resulting edge will be left here too.
     * @return true if ok, false otherwise.
     * @exception susanError is thrown if the parameters are out of range.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the result in each channel of image.
     * @param srcdest image with the source data.  The result
     *                will be left here too.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(image& srcdest) const;

    /**
     * operates on a copy of the given parameters (channel)
     * @param src channel with the source data.
     * @param dest channel, where the result will be left.
     * @return true if ok, false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const channel& src,channel8& dest) const;

    /**
     * operates on a copy of the given lti::channel8 input.
     *
     * @param src lti::channel8 with the source data.
     * @param dest lti::channel8, where the binary edge will be left.
     * @return true if ok, false otherwise.
     * @exception susanError is thrown if the parameters are out of range.
     */
    virtual bool apply(const channel8& src, channel8& dest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the result in each channel of the destination image.
     *
     * @param src image with the source data.
     * @param dest image where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src,image& dest) const;

    /**
     * Compute the edges for the red, green and blue components of the image
     * and leave the sum of the results in the given channel.  Note that
     * if you give as parameters::edgeValue or parameters::noEdgeValue values
     * greater than 255/3, you can have some overflow effects on the resulting
     * channel.
     *
     * @param src image with the source data.
     * @param dest channel8 where the result will be left.
     * @return true if apply successful or false otherwise.
     */
    virtual bool apply(const image& src, channel8& dest) const;

  };

} // end of namespace

#endif

