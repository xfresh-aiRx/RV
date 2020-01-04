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
 * file .......: ltiSusanCorners.h
 * authors ....: Frederik Lange
 * organization: LTI, RWTH Aachen
 * creation ...: 27.3.2001
 * revisions ..: $Id: ltiSusanCorners.h,v 1.19 2005/06/16 12:35:44 doerfler Exp $
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

#ifndef _LTI_SUSAN_CORNERS_H_
#define _LTI_SUSAN_CORNERS_H_

#include "ltiImage.h"
#include "ltiCornerDetector.h"
#include "ltiPointList.h"
#include "ltiMath.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace lti {
  /**
   *  Corner finder with the Susan low level image processing algorithm
   */
  class susanCorners : public cornerDetector {
  public:
    /**
     * the parameters for the class susanCorners
     */
    class parameters : public cornerDetector::parameters {
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

      //TODO: comment the parameters of your functor
      // If you add more parameters manually, do not forget to do following:
      // 1. indicate in the default constructor the default values
      // 2. make sure that the copy member also copy your new parameters
      // 3. make sure that the read and write members also read and
      //    write your parameters

      /**
       * size of the circular kernel, unused at the moment, default is 5
       */
      int kernelSize;

      /**
       * threshold for similarity criteria, for difference in
       * brightness (default 20)
       */
      int threshold;

      /**
       * threshold for the form of the corner (default 290)
       */
      int geometricThreshold;
    };

    /**
     * default constructor
     */
    susanCorners();

    /**
     * constructor with parameters
     */
    susanCorners(const parameters& par);

    /**
     * copy constructor
     * @param other the object to be copied
     */
    susanCorners(const susanCorners& other);

    /**
     * destructor
     */
    virtual ~susanCorners();

    /**
     * returns the name of this type ("susanCorners")
     */
    virtual const char* getTypeName() const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel8 with the source data.  The result
     *                 will be left here too.  An image with the corners only
     *                 will be returned.  If you want to overlay the corners
     *                 with the input image use apply(chnl,chnl)
     * @return true if ok, false otherwise.
     */
    virtual bool apply(channel8& srcdest) const;

    /**
     * operates on the given %parameter.
     * @param srcdest channel with the source data.  The result
     *                 will be left here too.  An image with the corners only
     *                 will be returned.  If you want to overlay the corners
     *                 with the input image use apply(chnl,chnl)
     * @return true if ok, false otherwise.
     */
    virtual bool apply(channel& srcdest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel8 with the source data.
     * @param dest channel where the result will be left.  If this
     *             channel is the same input channel or its size is the same of
     *             the src channel, the corners will be "overlayed" in the
     *             channel without cleaning it.  If this channel has another
     *             size than the dest, it will be resized and only the corners
     *             will be seen. The value of a corner will be always 1.0f.
     * @return true if ok, false otherwise.
     */
    virtual bool apply(const channel8& src,channel8& dest) const;

    /**
     * operates on a copy of the given %parameters.
     * @param src channel with the source data.
     * @param dest channel where the result will be left.  If this
     *             channel is the same input channel or its size is the same of
     *             the src channel, the corners will be "overlayed" in the
     *             channel without cleaning it.  If this channel has another
     *             size than the dest, it will be resized and only the corners
     *             will be seen. The value of a corner will be always 1.0f.
     * @return true if ok, false otherwise.
     */
    virtual bool apply(const channel& src,channel& dest) const;

    /**
     * result is given back as a list of points
     * @return true if ok, false otherwise.
     */
    virtual bool apply(const channel& src, pointList& dest) const;

    /**
     * result is given back as a list of points
     * @return true if ok, false otherwise.
     */
    virtual bool apply(const channel8& src, pointList& dest) const;

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    susanCorners& copy(const susanCorners& other);

    /**
     * copy data of "other" functor.
     * @param other the functor to be copied
     * @return a reference to this functor object
     */
    susanCorners& operator=(const susanCorners& other);

    /**
     * returns a pointer to a clone of this functor.
     */
    virtual functor* clone() const;

    /**
     * returns used parameters
     */
    const parameters& getParameters() const;

    /**
     * set the parameters
     * This member makes a copy of <em>theParam</em>: the functor
     * will keep its own copy of the parameters!
     * @return true if successful, false otherwise
     */
    virtual bool updateParameters();

  private:

    /**
     * brightness LUT
     */
    ubyte* brightnessLUT;

    /**
     * sets up an LUT for faster processing
     */
    void setupBrightnessLUT(ubyte*& bp,const int& thresh,const int& form);

    /**
     * delete the LUT
     */
    void deleteBrightnessLUT(ubyte*& bp);

    /**
     * detects the corners in the image
     */
    void detect(const ubyte* in,
                const ubyte* bp,
                const int& max_no,
                const int& geometricThreshold,
                const int& x_size,
                const int& y_size,
                pointList& corners) const;

    /**
     * internal round returns integers and not floats
     */
    inline int round(const float x) const;
  };
}

#endif

